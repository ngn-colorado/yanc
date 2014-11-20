//
// yanc: Yet Another Network Controller
// OpenFlow Adapter
//
// (c) 2013  Matthew Monaco  <matthew.monaco@colorado.edu>
//           Oliver Michel   <oliver.michel@colorado.edu>
//
// License: TBD
//

#include <stdint.h>
#include <string>
#include <sstream>
#include <netinet/in.h>
#include <string.h>
#include <iostream>

#include "openflow_tools.h"

std::string ofp_msg_name(uint8_t type) {

	switch(type) {

		case OFPT_HELLO: return std::string("OFPT_HELLO");
		case OFPT_ERROR: return std::string("OFPT_ERROR");
		case OFPT_ECHO_REQUEST: return std::string("OFPT_ECHO_REQUEST");
		case OFPT_ECHO_REPLY: return std::string("OFPT_ECHO_REPLY");
		case OFPT_VENDOR: return std::string("OFPT_EXPERIMENTER");

		case OFPT_FEATURES_REQUEST: return std::string("OFPT_FEATURES_REQUEST");
		case OFPT_FEATURES_REPLY: return std::string("OFPT_FEATURES_REPLY");
		case OFPT_GET_CONFIG_REQUEST: return std::string("OFPT_GET_CONFIG_REQUEST");
		case OFPT_GET_CONFIG_REPLY: return std::string("OFPT_GET_CONFIG_REPLY");
		case OFPT_SET_CONFIG: return std::string("OFPT_SET_CONFIG");

		case OFPT_PACKET_IN: return std::string("OFPT_PACKET_IN");
		case OFPT_FLOW_REMOVED: return std::string("OFPT_FLOW_REMOVED");
		case OFPT_PORT_STATUS: return std::string("OFPT_PORT_STATUS");

		case OFPT_PACKET_OUT: return std::string("OFPT_PACKET_OUT");
		case OFPT_FLOW_MOD: return std::string("OFPT_FLOW_MO");
		case OFPT_PORT_MOD: return std::string("OFPT_PORT_MOD");

		case OFPT_STATS_REQUEST: return std::string("OFPT_STATS_REQUEST");
	 	case OFPT_STATS_REPLY: return std::string("OFPT_STATS_REPLY");

		case OFPT_BARRIER_REQUEST: return std::string("OFPT_BARRIER_REQUEST");
		case OFPT_BARRIER_REPLY: return std::string("OFPT_BARRIER_REPLY");

		case OFPT_QUEUE_GET_CONFIG_REQUEST: 
			return std::string("OFPT_QUEUE_GET_CONFIG_REQUEST");
		case OFPT_QUEUE_GET_CONFIG_REPLY:
			return std::string("OFPT_QUEUE_GET_CONFIG_REPLY");

		default: return std::string("OF MSG TYPE UNKNOWN");
	};

}

std::string ofp_error_name(uint16_t type, uint16_t code) {
	
	switch(type) {
		case OFPET_HELLO_FAILED:
			switch (code) {
				case OFPHFC_INCOMPATIBLE: 
					return std::string("HELLO_FAILED/INCOMPATIBLE");
				case OFPHFC_EPERM: 
					return std::string("HELLO_FAILED/EPERM");
				default: 
					return std::string("HELLO_FAILED/" + code);
			}

		case OFPET_BAD_REQUEST:
			switch(code) {
				case OFPBRC_BAD_VERSION:
					return std::string("BAD_REQUEST/BAD_VERSION");
				case OFPBRC_BAD_TYPE:
					return std::string("BAD_REQUEST/BAD_TYPE");
				case OFPBRC_BAD_STAT:
					return std::string("BAD_REQUEST/BAD_STAT");
				case OFPBRC_BAD_VENDOR:
					return std::string("BAD_REQUEST/BAD_VENDOR");
				case OFPBRC_BAD_SUBTYPE:
					return std::string("BAD_REQUEST/BAD_SUBTYPE");
				case OFPBRC_EPERM:
					return std::string("BAD_REQUEST/EPERM");
				case OFPBRC_BAD_LEN:
					return std::string("BAD_REQUEST/BAD_LEN");
				case OFPBRC_BUFFER_EMPTY:
					return std::string("BAD_REQUEST/BUFFER_EMPTY");
				case OFPBRC_BUFFER_UNKNOWN:
					return std::string("BAD_REQUEST/BUFFER_UNKNOWN");
				default:
					return std::string("BAD_REQUEST/" + code);
			}

		case OFPET_BAD_ACTION:
			switch (code) {
				case OFPBAC_BAD_TYPE:
					return std::string("BAD_ACTION/BAD_TYPE");
				case OFPBAC_BAD_LEN:
					return std::string("BAD_ACTION/BAD_LEN");
				case OFPBAC_BAD_VENDOR:
					return std::string("BAD_ACTION/BAD_VENDOR");
				case OFPBAC_BAD_VENDOR_TYPE:
					return std::string("BAD_ACTION/BAD_VENDOR_TYPE");
				case OFPBAC_BAD_OUT_PORT:
					return std::string("BAD_ACTION/BAD_OUT_PORT");
				case OFPBAC_BAD_ARGUMENT:
					return std::string("BAD_ACTION/BAD_ARGUMENT");
				case OFPBAC_EPERM:
					return std::string("BAD_ACTION/BAD_EPERM");
				case OFPBAC_TOO_MANY:
					return std::string("BAD_ACTION/BAD_TOO_MANY");
				case OFPBAC_BAD_QUEUE:
					return std::string("BAD_ACTION/BAD_BAD_QUEUE");
				default:
					return std::string("BAD_ACTION/" + code);
			}

		case OFPET_FLOW_MOD_FAILED:
			switch (code) {
				case OFPFMFC_ALL_TABLES_FULL:
					return std::string("FLOW_MOD_FAILED/ALL_TABLES_FULL");
				case OFPFMFC_OVERLAP:
					return std::string("FLOW_MOD_FAILED/OVERLAP");
				case OFPFMFC_EPERM:
					return std::string("FLOW_MOD_FAILED/EPERM");
				case OFPFMFC_BAD_EMERG_TIMEOUT:
					return std::string("FLOW_MOD_FAILED/BAD_EMERG_TIMEOUT");
				case OFPFMFC_BAD_COMMAND:
					return std::string("FLOW_MOD_FAILED/BAD_COMMAND");
				case OFPFMFC_UNSUPPORTED:
					return std::string("FLOW_MOD_FAILED/UNSUPPORTED");
				default:
					return std::string("FLOW_MOD_FAILED/" + code);
			}

		case OFPET_PORT_MOD_FAILED:
			switch (code) {
				case OFPPMFC_BAD_PORT:
					return std::string("PORT_MOD_FAILED/BAD_PORT");
				case OFPPMFC_BAD_HW_ADDR:
					return std::string("PORT_MOD_FAILED/BAD_HW_ADDR");
				default:
					return std::string("PORT_MOD_FAILED/" + code);
			}

		case OFPET_QUEUE_OP_FAILED:

			switch (code) {
				case OFPQOFC_BAD_QUEUE:
					return std::string("QUEUE_OP_FAILED/BAD_QUEUE");
				case OFPQOFC_EPERM:
					return std::string("QUEUE_OP_FAILED/EPERM");
				default:
					return std::string("QUEUE_OP_FAILED/" + code);
			}
		}
	return std::string("OFPT_ERROR/" + type);
}

std::string ofp_packet_in_reason_name(uint8_t reason)
{
	std::string reason_str;

	switch (reason) {
		case OFPR_NO_MATCH: reason_str = "NO_MATCH"; break;
		case OFPR_ACTION:   reason_str = "ACTION"; break;
		default:            reason_str = "UNKNOWN";
	}

	return reason_str;
}

void populate_ofp_header_n(struct ofp_header* header, uint8_t version,
	uint8_t type, uint16_t length, uint32_t xid)
{
	header->version = version;
	header->type = type;
	header->length = htons(length);
	header->xid = htonl(xid);
}


void ofp11_port_from_ofp_phy_port(const struct ofp_phy_port& phy_port, 
	struct ofp11_port& ofp11_port)
{
	ofp11_port.port_no     = htonl((uint32_t)ntohs(phy_port.port_no));

	memcpy(ofp11_port.hw_addr, phy_port.hw_addr, OFP_ETH_ALEN);
	memcpy(ofp11_port.name, phy_port.name, OFP_MAX_PORT_NAME_LEN);

	ofp11_port.config      = phy_port.config;
	ofp11_port.state       = phy_port.state;

	ofp11_port.curr        = phy_port.curr;
	ofp11_port.advertised  = phy_port.advertised;
	ofp11_port.supported   = phy_port.supported;
	ofp11_port.peer        = phy_port.peer;

	// new fields in OF 1.1
	ofp11_port.curr_speed  = 0;
	ofp11_port.max_speed   = 0;
}

void ofp11_packet_in_from_ofp_packet_in(const struct ofp_packet_in& ofp_in, 
	struct ofp11_packet_in& ofp11_in)
{
	ofp11_in.header      = ofp_in.header;

	ofp11_in.buffer_id   = ofp_in.buffer_id;
	ofp11_in.in_port     = htonl((uint32_t)ntohs(ofp_in.in_port));

	// treat in_phy_port like in_port
	ofp11_in.in_phy_port = htonl((uint32_t)ntohs(ofp_in.in_port));

	ofp11_in.total_len   = ofp_in.total_len;
	ofp11_in.reason      = ofp_in.reason;
	ofp11_in.table_id    = 0;

	size_t len = ntohs(ofp_in.header.length) - sizeof(struct ofp_packet_in);
	
	memcpy(ofp11_in.data, ofp_in.data, len);
}
