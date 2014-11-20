
#include <stdexcept>
#include <iostream>
#include <arpa/inet.h>
#include <unistd.h>
#include "of_network.h"
#include "openflow_tools.h"

OFNetwork::OFNetwork(const std::string& fs_path, uint8_t of_version)
	: 	_fs_path(fs_path),
		_of_version(of_version),
		_dbus(0) {}

OFNetwork::~OFNetwork()
{
	this->shutdown();
}

void OFNetwork::shutdown()
{
	std::map<om::net::StreamConnection*,OFSwitch*>::iterator it;

	for (it = this->switches.begin(); it != this->switches.end(); it++) {

		om::net::StreamConnection* sc = it->first;
		OFSwitch*                  sw = it->second;
		
		delete sw;

		sc->close();
		delete sc;
	}
}

void OFNetwork::new_connection(om::net::StreamConnection& cxn)
{
	this->send_hello(cxn);
}

void OFNetwork::closed_connection(om::net::StreamConnection& cxn)
{
	OFSwitch* sw = this->switches[(&cxn)];
	if (sw) {
		std::string    path = sw->path();
		this->switches.erase(&cxn);
		delete sw;
		rmdir(path.c_str());
	}
}

void OFNetwork::handle_message(om::net::StreamConnection& cxn,
	struct ofp_header& hdr)
{
	OFSwitch* sw = this->switches[&cxn];

	// messages which do not require the switch to be known
	switch (hdr.type) {

		case OFPT_HELLO:
			if (sw) {
				std::cerr << "OFNetwork WARNING received HELLO from existing switch "
				     << sw << std::endl;
			} else {
				std::cerr << "OFNetwork INFO received HELLO from " << cxn 
					<< std::endl;
				this->send_features_request(cxn);
				//this->send_get_config_request(cxn);
			}
			return;
		case OFPT_ERROR:
			this->handle_error(*(struct ofp_error_msg*) &hdr);
			return;
		
		case OFPT_ECHO_REQUEST:
			hdr.type = OFPT_ECHO_REPLY;
			cxn.send((const unsigned char*) &hdr, ntohs(hdr.length));
			return;
		
		case OFPT_FEATURES_REPLY:
			if (sw) {
				std::cerr << "OFNetwork WARNING received unsolicited FEAUTRES_REPLY"
					<< std::endl;
			} else {

				try {
					sw = new OFSwitch(_fs_path + "/switches", 
						*(struct ofp_switch_features*)&hdr, &cxn, _dbus);
				} catch (std::runtime_error& e) {
					std::cerr << "OFNetwork [WARN]:  unable to create OFSwitch: "
						<< e.what() << std::endl;
					return;
				}
				switches[&cxn] = sw;
			}
			return;

		case OFPT_GET_CONFIG_REPLY:  break;
		case OFPT_PACKET_IN:         break;
		case OFPT_FLOW_REMOVED:      break;
		case OFPT_PORT_STATUS:       break;

		default:
			std::cerr << "OFNetwork [WARNING]: received unsupported message " 
				<< ofp_msg_name(hdr.type) << std::endl;
			break;
	}

	if (!sw) {
		std::cerr << "OFNetwork [WARNING] received " << (int) hdr.type
			<< " from unknown switch " << &cxn << std::endl;
		return;
	}

	// messages that do require the switch to be known
	switch (hdr.type) {

		case OFPT_GET_CONFIG_REPLY:
			sw->read_switch_config(hdr.version, *(struct ofp_switch_config*) &hdr);
			return;
		
		case OFPT_PACKET_IN:
			switch(hdr.version) {
				case OFP_VERSION_1_0: 
					sw->handle_packet_in(hdr.version, (struct ofp_packet_in&) hdr);
					break;
				case OFP_VERSION_1_1:
					sw->handle_packet_in(hdr.version, (struct ofp11_packet_in&) hdr);
					break;
			}
			return;
		
		case OFPT_FLOW_REMOVED:
			sw->handle_flow_removed(hdr.version, *(struct ofp_flow_removed*) &hdr);
			return;
/*		
		case OFPT_PORT_STATUS:
			sw->handle_port_status_change(hdr.version, *(struct ofp_port_status*) &hdr);
			return;
*/
		default:
			std::cerr << "OFNetwork [WARNING]: received unsupported message " 
				<< ofp_msg_name(hdr.type) << std::endl;
			return;
		}
}

int OFNetwork::send_hello(om::net::StreamConnection& cxn) const
{
	struct ofp_header hdr;

	hdr.version = this->_of_version;
	hdr.type    = OFPT_HELLO;
	hdr.length  = htons(sizeof(hdr));
	hdr.xid     = htonl(0);

	return cxn.send((const unsigned char*) &hdr, sizeof(hdr));
}

int OFNetwork::send_features_request(om::net::StreamConnection& cxn) const
{
	struct ofp_header hdr;

	hdr.version = this->_of_version;
	hdr.type    = OFPT_FEATURES_REQUEST;
	hdr.length  = htons(sizeof(hdr));
	hdr.xid     = htonl(0);

	return cxn.send((const unsigned char*) &hdr, sizeof(hdr));
}

int OFNetwork::send_get_config_request(om::net::StreamConnection& cxn) const
{
	struct ofp_header hdr;

	hdr.version = this->_of_version;
	hdr.type    = OFPT_GET_CONFIG_REQUEST;
	hdr.length  = htons(sizeof(hdr));
	hdr.xid     = htonl(0);

	return cxn.send((const unsigned char*) &hdr, sizeof(hdr));
}

void OFNetwork::handle_error(const struct ofp_error_msg& msg)
{
	std::cerr << "OFNetwork ERROR, received "
	     << ofp_error_name(ntohs(msg.type), ntohs(msg.code)) << std::endl;
}

void OFNetwork::set_dbus_connection(om::ipc::dbus::Connection* c)
{
	_dbus = c;
}

om::ipc::dbus::Connection* OFNetwork::dbus_connection()
{
	return _dbus;
}


OFSwitch* OFNetwork::switch_by_name(std::string& name) const
{
	for(auto i : switches) {
		OFSwitch* s = i.second;
		if(s->name() == name)
			return s;
	}
	return 0;
}
