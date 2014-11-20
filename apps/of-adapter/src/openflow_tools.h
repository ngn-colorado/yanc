//
// yanc: Yet Another Network Controller
// OpenFlow Adapter
//
// (c) 2013  Matthew Monaco  <matthew.monaco@colorado.edu>
//           Oliver Michel   <oliver.michel@colorado.edu>
//
// License: TBD
//

#ifndef OPENFLOW_TOOLS_H
#define OPENFLOW_TOOLS_H

#include <openflow-legacy/openflow.h>

std::string ofp_msg_name(uint8_t type);
std::string ofp_error_name(uint16_t type, uint16_t code);

std::string ofp_packet_in_reason_name(uint8_t reason);

void populate_ofp_header_n(struct ofp_header* header, uint8_t version,
	uint8_t type, uint16_t length, uint32_t xid);


void ofp11_port_from_ofp_phy_port(const struct ofp_phy_port&, struct ofp11_port&);
void ofp11_packet_in_from_ofp_packet_in(const struct ofp_packet_in&, struct ofp11_packet_in&);

#endif
