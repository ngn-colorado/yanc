//
// yanc: Yet Another Network Controller
// OpenFlow Adapter
//
// (c) 2013  Matthew Monaco  <matthew.monaco@colorado.edu>
//           Oliver Michel   <oliver.michel@colorado.edu>
//
// License: TBD
//

#include <functional>
#include <iostream>
#include <string>
#include <sys/epoll.h>
#include <sys/types.h>
#include <unistd.h>

#include <om/async/epoll_wrapper.h>
#include <om/net/net.h>
#include <om/tools/file.h>
#include <om/tools/tools.h>
#include <om/tools/string.h>

#include "openflow-legacy/openflow.h"
#include "openflow_tools.h"
#include "yanc_of_adapter.h"

YancOFAdapter::YancOFAdapter(std::string fs_path, std::string dbus_addr,
	int of_port)
	
	:	_fs_path(fs_path),
		_dbus_addr(dbus_addr),
		_of_port(of_port),
		_net(fs_path, OFP_VERSION_1_0),
		_epoll(),
		_dbus_conn(),
		_net_listener(
			om::net::tp_addr("0.0.0.0", om::net::tp_proto_tcp, of_port), 
			std::bind(&YancOFAdapter::new_switch, this, std::placeholders::_1)
		)
{
	pid_t pid = getpid();

	om::tools::msg_stream(1) << "YancOFAdapter [INFO]:  started with pid/"
		<< pid << std::endl;

	_epoll.add_interface(&_net_listener, EPOLLIN | EPOLLERR);

	om::tools::msg_stream(1) << "YancOFAdapter [INFO]:  fd/" << _net_listener.fd()
		<< " listening on tcp:0.0.0.0:" << _of_port << std::endl;

	_init_dbus(dbus_addr);
}

void YancOFAdapter::dbus_connected(om::ipc::dbus::Connection* d)
{
	om::tools::msg_stream(1) << "YancOFAdapter [INFO]:  fd/" << d->fd()
		<< " dbus connection established: " << _dbus_addr << std::endl;
	
	_epoll.add_interface(d, EPOLLIN | EPOLLERR);

	_set_dbus_signal_handlers(d);

	_net.set_dbus_connection(d);

	om::ipc::dbus::Message m(DBUS_MESSAGE_TYPE_SIGNAL);
	m.set_interface("edu.colorado.cs.ngn.yanc");
	m.set_path("/edu/colorado/cs/ngn/yanc");
	m.set_member("AdapterStarted");
	_dbus_conn.send(m);
}

void YancOFAdapter::new_switch(om::net::StreamListener* l)
{
	om::net::tp_addr remote;
	int new_fd = l->accept(&remote);

	om::net::StreamConnection* cxn = new om::net::StreamConnection(new_fd, remote,
		std::bind(&YancOFAdapter::receive_data, this, std::placeholders::_1)
	);

	if(this->bufs[cxn] || this->curs[cxn])
		om::tools::msg_stream(2) << "YancOFAdapter [DEBUG]: bufs or curs not null"
			<< std::endl;

	this->bufs[cxn] = (unsigned char*) malloc(YOF_BUF_LEN);
	this->curs[cxn] = this->bufs[cxn];

	if(this->bufs[cxn] == NULL) {
		std::cerr << "YancOFAdapter [ERROR]: no memory for connection buffer"
			<< std::endl;
		cxn->close();
		delete cxn;
		return;
	}

	_net.new_connection(*cxn);
	
	if(_epoll.monitors_fd(cxn->fd())) {
		std::cerr << "YancOFAdapter [ERROR]: new_switch: fd/" << cxn->fd()
			<< " is being monitored already, aborting operation" << std::endl;
	} else {

		_epoll.add_interface(cxn, EPOLLIN | EPOLLERR);

		om::tools::msg_stream(1) << "YancOFAdapter [INFO]:  fd/" << cxn->fd()
			<< " new connection " << *cxn << std::endl;
	}
}

void YancOFAdapter::receive_data(om::net::StreamConnection* c)
{
	unsigned char*     buf = this->bufs[c];
	unsigned char*     cur = this->curs[c];
	struct ofp_header* hdr = (struct ofp_header*) buf;
	ssize_t            bytes;
	uint16_t           len;
	size_t             remaining;

	if(!buf || !cur) {
		std::cerr << "YancOFAdapter [ERROR]: NULL: " << *c << std::endl;
		return;
	}

	if(cur - buf < (ssize_t) sizeof(struct ofp_header)) {
		remaining = sizeof(struct ofp_header) - (cur-buf);
	} else {
		len = ntohs(hdr->length);
		if (cur - buf < len)
			remaining = len - (cur-buf);
		else
			remaining = 0;
	}

	if(remaining) {
		bytes = c->receive(cur, remaining);
		cur += bytes;
		remaining -= bytes;

		if(bytes < 1) {
			std::cerr << "YancOFAdapter [ERROR]: received " << bytes
				<< " of ofp_packet_header." << " Closing connection." << std::endl;
			
			_net.closed_connection(*c);
			_epoll.remove_interface(c);
			
			c->close();
			
			cur = buf;
			free(this->bufs[c]);
			this->bufs.erase(c);
			this->curs.erase(c);
			
			return;
		}
	}

	if(remaining)
		goto out;
	else if((cur - buf) < ntohs(hdr->length))
		goto out;


	_net.handle_message(*c, *hdr);
	cur = buf;		

out:
		this->bufs[c] = buf;
		this->curs[c] = cur;		
}


DBusHandlerResult YancOFAdapter::new_flow(
	om::ipc::dbus::Connection* d, DBusMessage* msg)
{
	om::ipc::dbus::Message received(msg);

	std::string path = received.get_string(0);

	std::vector<std::string> path_components;
	om::tools::string::split(path, '/', path_components);

	std::string switch_name = path_components[2];
	std::string flow_name = path_components[4];

	om::tools::msg_stream(1) << "YancOFAdapter [INFO]:  NewFlow: "
		<< switch_name << "/" << flow_name << std::endl;

	OFSwitch* sw = _net.switch_by_name(switch_name);

	if(sw)
		sw->new_flow(flow_name);

	return DBUS_HANDLER_RESULT_HANDLED;
}

DBusHandlerResult YancOFAdapter::flow_mod(
	om::ipc::dbus::Connection* d, DBusMessage* msg)
{

	om::ipc::dbus::Message received(msg);

	std::string path = received.get_string(0);

	std::vector<std::string> path_components;
	om::tools::string::split(path, '/', path_components);

	std::string switch_name = path_components[2];
	std::string flow_name = path_components[4];

	om::tools::msg_stream(1) << "YancOFAdapter [INFO]:  FlowMod: "
		<< switch_name << "/" << flow_name << std::endl;

	OFSwitch* sw = _net.switch_by_name(switch_name);

	if(sw)
		sw->flow_mod(flow_name);

	return DBUS_HANDLER_RESULT_HANDLED;
}

DBusHandlerResult YancOFAdapter::packet_out(
	om::ipc::dbus::Connection* d, DBusMessage* msg)
{
	om::ipc::dbus::Message received(msg);

	std::string path = received.get_string(0);

	std::vector<std::string> path_components;
	om::tools::string::split(path, '/', path_components);

	std::string switch_name = path_components[2];
	std::string pout_name = path_components[4];

	om::tools::msg_stream(1) << "YancOFAdapter [INFO]:  PacketOut: "
		<< switch_name << "/" << pout_name << std::endl;

	OFSwitch* sw = _net.switch_by_name(switch_name);

	if(sw)
		sw->new_packet_out(pout_name);

	return DBUS_HANDLER_RESULT_HANDLED;
}

DBusHandlerResult YancOFAdapter::
	default_dbus_handler(om::ipc::dbus::Connection* d, DBusMessage* m)
{
	om::ipc::dbus::Message received(m);

	if(received.interface() == "edu.colorado.cs.ngn.yanc") {

		om::tools::msg_stream(2) << "YancOFAdapter [DEBUG]: RX SIGNAL "
			<< received.member() << std::endl;
	}

	return DBUS_HANDLER_RESULT_HANDLED;
}

YancOFAdapter::~YancOFAdapter()
{
	_net.shutdown();
	this->shutdown();
}

void YancOFAdapter::run()
{
	_epoll.dispatch();
}

void YancOFAdapter::shutdown()
{
	om::tools::msg_stream(1) << "YancOFAdapter [INFO]:  shutting down"
		<< std::endl;	
}

void YancOFAdapter::_init_dbus(std::string& dbus_addr)
{
	using namespace std::placeholders;

	_dbus_conn.set_default_signal_handler(
		std::bind(&YancOFAdapter::default_dbus_handler, this, _1, _2)
	);

	_dbus_conn.set_default_method_call_handler(
		std::bind(&YancOFAdapter::default_dbus_handler, this, _1, _2)
	);

	_dbus_conn.set_default_method_return_handler(
		std::bind(&YancOFAdapter::default_dbus_handler, this, _1, _2)
	);

	_dbus_conn.set_default_error_handler(
		std::bind(&YancOFAdapter::default_dbus_handler, this, _1, _2)
	);

	_dbus_conn.open(
		dbus_addr, "edu.colorado.cs.ngn.yanc.OFAdapter",
		std::bind(&YancOFAdapter::dbus_connected, this, _1)
	);
}

void YancOFAdapter::_set_dbus_signal_handlers(om::ipc::dbus::Connection* d)
{
	using namespace std::placeholders;

	d->set_signal_handler("edu.colorado.cs.ngn.yanc", "NewFlow",
		std::bind(&YancOFAdapter::new_flow, this, _1, _2)
	);

	d->set_signal_handler("edu.colorado.cs.ngn.yanc", "FlowMod",
		std::bind(&YancOFAdapter::flow_mod, this, _1, _2)
	);

	d->set_signal_handler("edu.colorado.cs.ngn.yanc", "PacketOut",
		std::bind(&YancOFAdapter::packet_out, this, _1, _2)
	);
}
