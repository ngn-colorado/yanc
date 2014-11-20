#ifndef _OF_PORT_
#define _OF_PORT_

#include "y_port.h"
#include "openflow-legacy/openflow.h"

class OFPort : public YPort {
 public:
	OFPort(std::string par, const struct ofp11_port&);
	void parse(const struct ofp11_port&);
	void fill(struct ofp_port_mod&);
	static std::string name_from(const struct ofp11_port&);
 private:
};

#endif/*_OF_PORT_*/
