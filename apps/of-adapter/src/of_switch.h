#ifndef _OF_SWITCH_
#define _OF_SWITCH_

#include <om/net/stream_connection.h>
#include <om/ipc/dbus/connection.h>
#include "y_switch.h"
#include "of_port.h"
#include "of_flow.h"
#include "of_packet_in.h"
#include "of_packet_out.h"
#include "openflow-legacy/openflow.h"


class OFSwitch : public YSwitch {
 public:
	OFSwitch(std::string parent_path, const struct ofp_switch_features&,
		om::net::StreamConnection*, om::ipc::dbus::Connection*);
	~OFSwitch();

	void parse(const struct ofp_switch_config&);
	void parse(const struct ofp11_packet_in&);
	void parse(const struct ofp_flow_removed&);
	void parse(const struct ofp_port_status&);

	void read_switch_config(uint8_t v, const struct ofp_switch_config&);

	void handle_packet_in(uint8_t v, const struct ofp11_packet_in&);
	void handle_packet_in(uint8_t v, const struct ofp_packet_in&);

	void handle_flow_removed(uint8_t v, const struct ofp_flow_removed&);
//	void handle_port_status_change(uint8_t v, const struct ofp_port_status&);

	void new_flow(std::string& name);
	void flow_mod(std::string& name);
	void rm_flow(std::string& name);

	void new_packet_out(std::string& name);

	uint8_t of_version() const;

	static std::string name_from(const struct ofp_switch_features&);

 private:
	om::net::StreamConnection* _sc;
	om::ipc::dbus::Connection* _dbus;

	uint8_t _of_version;

	void _add_port(const struct ofp_phy_port&);
	void _add_port(const struct ofp11_port&);
	void _del_port(OFPort*);

	std::map<uint32_t, OFPort*>     ports_by_no;
	std::map<std::string, OFPort*>  ports_by_name;
	std::map<std::string, OFFlow*>  flows_by_name;
};

#endif/*_OF_SWITCH_*/
