#ifndef _OF_PACKET_IN_
#define _OF_PACKET_IN_

#include <string>
#include <map>
#include "openflow-legacy/openflow.h"
#include "y_packet_in.h"
#include "of_port.h"

class OFPacketIn : public YPacketIn {
 public:
	OFPacketIn(std::string parent_path, const struct ofp11_packet_in&, OFPort&);

 private:
	static std::string _name_from(const struct ofp11_packet_in&, OFPort&);
	static std::map<uint32_t, unsigned> _counter; // per-port counters
};

#endif/*_OF_PACKET_IN_*/
