#include <arpa/inet.h>
#include <iostream>
#include <sstream>
#include "of_packet_in.h"
using namespace std;

OFPacketIn::OFPacketIn(string parent, const struct ofp11_packet_in& msg, OFPort& port)
	: YPacketIn(parent + "/" + _name_from(msg, port))
{
	if (!this->managed()) {
		cerr << "OFPacketIn WARNING packet_in directory already exists" << endl;
		this->managed(true);
	}

	this->buffer_id = ntohl(msg.buffer_id);
	this->total_len = ntohs(msg.total_len);
	this->in_port   = ntohl(msg.in_port);
	this->reason    = msg.reason;

	this->data.set((const char*) msg.data, total_len);

}

std::string OFPacketIn::_name_from(const struct ofp11_packet_in& msg, OFPort& port)
{
	stringstream str;
	uint32_t port_no = ntohl(msg.in_port);
	unsigned count   = ++OFPacketIn::_counter[port_no];
	str << (std::string) port.name << "_" << count;
	return str.str();
}

std::map<uint32_t,unsigned> OFPacketIn::_counter;
