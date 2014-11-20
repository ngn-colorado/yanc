//
// yanc: Yet Another Network Controller
// OpenFlow Adapter
//
// (c) 2013  Matthew Monaco  <matthew.monaco@colorado.edu>
//           Oliver Michel   <oliver.michel@colorado.edu>
//
// License: TBD
//

#ifndef YANC_OF_ADAPTER_H
#define YANC_OF_ADAPTER_H

#include <map>

#include <om/async/epoll_wrapper.h>
#include <om/ipc/dbus/connection.h>
#include <om/net/agent.h>
#include <om/net/stream_connection.h>
#include <om/net/stream_listener.h>

#include "of_network.h"

#define YOF_BUF_LEN 4096

class YancOFAdapter /*: public om::net::Agent, public CallbackRegistrar */
{
 public:
	YancOFAdapter(std::string yanc_fs_path, std::string dbus_addr, int port=6633);
	~YancOFAdapter();

	void dbus_connected(om::ipc::dbus::Connection* d);

	void new_switch(om::net::StreamListener* l);
	void receive_data(om::net::StreamConnection* c);

	DBusHandlerResult new_flow(
		om::ipc::dbus::Connection* d, DBusMessage* msg);

	DBusHandlerResult flow_mod(
		om::ipc::dbus::Connection* d, DBusMessage* msg);

	DBusHandlerResult packet_out(
		om::ipc::dbus::Connection* d, DBusMessage* msg);

	DBusHandlerResult default_dbus_handler(
		om::ipc::dbus::Connection* d, DBusMessage* msg);

	void run();
	void shutdown();

 private:

 	void _init_dbus(std::string& dbus_addr);
	void _set_dbus_signal_handlers(om::ipc::dbus::Connection* d);

	std::string _fs_path;
	std::string _dbus_addr;
	int         _of_port;
	OFNetwork   _net;

	om::async::EPollWrapper    _epoll;
	om::ipc::dbus::Connection  _dbus_conn;
	om::net::StreamListener    _net_listener;

	std::map<om::net::StreamConnection*, unsigned char*> bufs;
	std::map<om::net::StreamConnection*, unsigned char*> curs;
};

#endif
