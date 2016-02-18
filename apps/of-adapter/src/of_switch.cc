#include <sys/stat.h>
#include <sys/types.h>
#include <sys/inotify.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <byteswap.h>
#include <stdexcept>
#include <sstream>
#include <iomanip>
#include "of_switch.h"
#include "openflow_tools.h"

#include <om/ipc/dbus/message.h>
using std::cerr;
using std::endl;
using std::string;
using std::stringstream;


OFSwitch::OFSwitch(string parent_path, const struct ofp_switch_features& msg,
	om::net::StreamConnection* sc, om::ipc::dbus::Connection* dbus)
	:	YSwitch(parent_path + "/" + OFSwitch::name_from(msg)),
		_sc(sc),
		_dbus(dbus),
		_of_version(msg.header.version)
{
	uint16_t num_ports;

	std::cerr << "OFSwitch [INFO]: FEATURES_REPLY " << this->path()
		<< ", OFP_VERSION=0x" << std::hex << (unsigned) _of_version 
		<< ", DPID=0x" << bswap_64(msg.datapath_id) << std::dec << std::endl;

	if (_of_version != OFP_VERSION_1_0 && _of_version != OFP_VERSION_1_1) {
		std::cerr << "OFSwitch [ERROR]: connection attempt from unsupported OF "
			<< "version 0x" << std::hex << (unsigned) _of_version << std::dec
			<< std::endl;
		return;
	}

	this->datapath_id  = bswap_64(msg.datapath_id); // todo
	this->n_buffers    = ntohl(msg.n_buffers);
	this->n_tables     = msg.n_tables;
	this->capabilities = ntohl(msg.capabilities);
	this->actions      = ntohl(msg.actions);

	num_ports = ntohs(msg.header.length) - sizeof(struct ofp_switch_features);

	if (_of_version == OFP_VERSION_1_0) {

		if(num_ports % sizeof(struct ofp_phy_port))
			throw std::runtime_error("non-integral port number");

		num_ports /= sizeof(struct ofp_phy_port);

	} else if (_of_version == OFP_VERSION_1_1) {

		if(num_ports % sizeof(struct ofp11_port))
			throw std::runtime_error("non-integral port number");

		num_ports /= sizeof(struct ofp11_port);
	}

	for (int i = 0; i < num_ports; i++)
		_add_port(msg.ports[i]);

	om::ipc::dbus::Message m(DBUS_MESSAGE_TYPE_SIGNAL);
	m.set_interface("edu.colorado.cs.ngn.yanc");
	m.set_path("/");
	m.set_member("NewSwitch");
	m.append_string(this->path());

	_dbus->send(m);
}

OFSwitch::~OFSwitch()
{
	int r;
	r = rmdir(this->path().c_str());
	if (r != 0)
		cerr << "OFSwitch WARNING unable to rmdir() " << this->path() << endl;
}

void OFSwitch::read_switch_config(uint8_t v, const struct ofp_switch_config& msg)
{
	cerr << "OFSwitch[" << this->path()
	     << "] DEBUG received GET_CONFIG_REPLY" << endl;

	this->flags = ntohs(msg.flags);
	this->miss_send_len = ntohs(msg.miss_send_len);
}

void OFSwitch::handle_packet_in(uint8_t v, const struct ofp11_packet_in& msg)
{
	uint32_t port_no = ntohl(msg.in_port);

	std::cerr << "OFSwitch[" << this->path()
		<< "] DEBUG received PACKET_IN on port " << port_no
		<< " for " << ofp_packet_in_reason_name(msg.reason) << std::endl;

	OFPort* port = this->ports_by_no[port_no];

	OFPacketIn* packet_in = new OFPacketIn(this->path() + "/packet_in", msg, *port);

	if (!packet_in) {
		cerr << "OFSwitch WARNING unable to create OFPacketIn: "
		     << this->path() << endl;
		return;
	}

	om::ipc::dbus::Message m(DBUS_MESSAGE_TYPE_SIGNAL);
	m.set_interface("edu.colorado.cs.ngn.yanc");
	m.set_path("/");
	m.set_member("PacketIn");
	m.append_string(packet_in->path());
	_dbus->send(m);


	// Just create the directory and forget about it
	packet_in->managed(false);
	delete packet_in;
}

void OFSwitch::handle_packet_in(uint8_t v, const struct ofp_packet_in& in)
{
	/* 
	 * Date: Feb 16, 2016
	 * Edited by: Sohil
	 * Size of packet_in is 24 bytes in OpenFlow 1.1 as opposed to 20 bytes in OpenFlow 1.0. So adding 4 bytes  
	 */
	uint8_t* ofp11_in = new uint8_t[ntohs(in.header.length) + 4];
	
	ofp11_packet_in_from_ofp_packet_in(in, (ofp11_packet_in&)*ofp11_in);
	handle_packet_in(OFP_VERSION_1_1, (ofp11_packet_in&)*ofp11_in);

	delete ofp11_in;
}

void OFSwitch::handle_flow_removed(uint8_t v, const struct ofp_flow_removed& msg)
{
	std::string reason;
	switch (msg.reason) {
		case OFPRR_IDLE_TIMEOUT: reason = "IDLE_TIMEOUT"; break;
		case OFPRR_HARD_TIMEOUT: reason = "HARD_TIMEOUT"; break;
		case OFPRR_DELETE:       reason = "DELETE"; break;
		default:                 reason = "UNKNOWN";
	}

	cerr << "OFSwitch[" << this->path()
	     << "] DEBUG received FLOW_REMOVED for " << reason << endl;
}

/*
void OFSwitch::handle_port_status_change(uint8_t v, const struct ofp_port_status& msg)
{
	OFPort* port = this->ports_by_no[ntohs(msg.desc.port_no)];

	switch (msg.reason) {
	case OFPPR_ADD:

		if (port) {
			cerr << "OFSwitch WARNING received OFPPR_ADD for existing port" << endl;
		} else {

			cerr << "OFSwitch INFO received OFPPR_ADD for port_no "
		     	     << msg.desc.port_no << ": " << msg.desc.name << endl;
		     	this->_add_port(msg.desc);
		}
		break;

	case OFPPR_MODIFY:

		if (!port) {
			cerr << "OFSwitch WARNING received OFPPR_MODIFY for non-existent port "
			     << "no " << msg.desc.port_no << ": " << msg.desc.name << endl;
		} else {
			cerr << "OFSwitch INFO received OFPPR_MODIFY for port_no "
		     	     << msg.desc.port_no << ": " << msg.desc.name << endl;
		     	port->parse(msg.desc);
		}
		break;

	case OFPPR_DELETE:

		if (!port) {
			cerr << "OFSwitch WARNING received OFPPR_DELETE for non-existent port "
			     << "no " << msg.desc.port_no << ": " << msg.desc.name << endl;
		} else {
			cerr << "OFSwitch INFO received OFPPR_DELETE for port_no "
		     	     << msg.desc.port_no << ": " << msg.desc.name << endl;
		     	this->_del_port(port);
		}
		break;
	}
}
*/
void OFSwitch::new_flow(std::string& name)
{
	OFFlow* flow = new OFFlow(this->_sc, this->path() + "/flows/" + name);
	this->flows_by_name[name] = flow;
}

void OFSwitch::flow_mod(std::string& name) {}

void OFSwitch::rm_flow(std::string& name)
{
	OFFlow* flow = this->flows_by_name[name];
	this->flows_by_name.erase(name);
	cerr << "OFSwitch DEBUG deleted flow " << this->path() << "/flows/" << name << endl;
	delete flow;
}

void OFSwitch::new_packet_out(std::string& name)
{
	new OFPacketOut(path()+"/packet_out/"+name, _sc);	
}

void OFSwitch::_add_port(const struct ofp_phy_port& phy_port)
{
	ofp11_port ofp11_port;
	ofp11_port_from_ofp_phy_port(phy_port, ofp11_port);

	OFPort* port = new OFPort(this->path() + "/ports", ofp11_port);

	this->ports_by_no[port->port_no] = port;
	this->ports_by_name[(string)port->name] = port;	
}

void OFSwitch::_add_port(const struct ofp11_port& phy_port)
{
	OFPort* port = new OFPort(this->path() + "/ports", phy_port);

	this->ports_by_no[port->port_no] = port;
	this->ports_by_name[(string)port->name] = port;
}

void OFSwitch::_del_port(OFPort* port)
{
	this->ports_by_no.erase(port->port_no);
	this->ports_by_name.erase((string)port->name);
	delete port;
}

uint8_t OFSwitch::of_version() const
{
	return _of_version;
}

std::string OFSwitch::name_from(const struct ofp_switch_features& sf)
{
	uint16_t msg_len = ntohs(sf.header.length);
	stringstream ss;

	if (msg_len <= sizeof(struct ofp_switch_features) || sf.ports[0].port_no != OFPP_LOCAL) {
		ss << "s" << bswap_64(sf.datapath_id);
	} else {
		std::string name = sf.ports[0].name;
		if (!name.empty())
			return name;
		else
			ss << sf.ports[0].port_no;
	}

	return ss.str();
}
