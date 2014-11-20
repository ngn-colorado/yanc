#include <string.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdexcept>
#include "of_port.h"
using namespace std;

OFPort::OFPort(string par, const struct ofp11_port& phy_port)
	: YPort(par+"/"+OFPort::name_from(phy_port))
{
	this->parse(phy_port);
}

void OFPort::parse(const struct ofp11_port& msg)
{
	this->port_no = ntohl(msg.port_no);
	this->hw_addr = msg.hw_addr;
	this->name    = msg.name;
	this->config  = ntohl(msg.config);
	this->state   = ntohl(msg.state);

	this->f_curr       = ntohl(msg.curr);
	this->f_advertised = ntohl(msg.advertised);
	this->f_supported  = ntohl(msg.supported);
	this->f_peer       = ntohl(msg.peer);
}

void OFPort::fill(struct ofp_port_mod& msg)
{
	uint64_t a = (Eth) this->hw_addr;

	msg.port_no   = htons(this->port_no);
	msg.hw_addr[0] = (a >> 40) & 0xFF;
	msg.hw_addr[1] = (a >> 32) & 0xFF;
	msg.hw_addr[2] = (a >> 24) & 0xFF;
	msg.hw_addr[3] = (a >> 16) & 0xFF;
	msg.hw_addr[4] = (a >>  8) & 0xFF;
	msg.hw_addr[5] = (a >>  0) & 0xFF;
	memcpy(msg.hw_addr, ((uint8_t*) (uint64_t) (Eth) this->hw_addr) + 2, 6);
	msg.config    = htonl(this->config);
	msg.mask      = -1;
	msg.advertise = htonl(this->f_advertised);
}

std::string OFPort::name_from(const struct ofp11_port& phy_port)
{
	string name = std::string(phy_port.name);
	if (!name.empty())
		return name;

	stringstream ss;
	ss << phy_port.port_no;
	return ss.str();
}
