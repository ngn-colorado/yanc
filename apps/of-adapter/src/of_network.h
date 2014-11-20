#ifndef OF_NETWORK_H
#define OF_NETWORK_H

#include <map>
#include <stdint.h>
#include "openflow-legacy/openflow.h"
#include <om/net/stream_connection.h>
#include <om/ipc/dbus/connection.h>
#include "of_switch.h"

class OFNetwork {

public:
	OFNetwork(const std::string& path, uint8_t of_version=OFP_VERSION_1_0);

	void shutdown();
	
	void new_connection(om::net::StreamConnection&);
	void closed_connection(om::net::StreamConnection&);
	void handle_message(om::net::StreamConnection&, struct ofp_header&);
	void handle_error(const struct ofp_error_msg&);

	int send_hello(om::net::StreamConnection&) const;
	int send_features_request(om::net::StreamConnection&) const;
	int send_get_config_request(om::net::StreamConnection&) const;

	// make singleton dbus connection object
	void set_dbus_connection(om::ipc::dbus::Connection*);
	om::ipc::dbus::Connection* dbus_connection();

	OFSwitch* switch_by_name(std::string& name) const;

	~OFNetwork();

	std::map<om::net::StreamConnection*, OFSwitch*> switches;
	
private:
	std::string _fs_path;
	uint8_t _of_version;
	om::ipc::dbus::Connection* _dbus;
};

#endif
