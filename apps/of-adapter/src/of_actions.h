#ifndef _OF_ACTIONS_
#define _OF_ACTIONS_

#include "y_actions.h"
#include "openflow-legacy/openflow.h"

class OFActions : public YActions {
 public:
	OFActions(std::string path) : YActions(path) {}
	uint16_t fill(unsigned char*);
 private:
	void fill_output(struct ofp_action_header*);
        void fill_vlan(struct ofp_action_header*);
        void fill_vlan_pcp(struct ofp_action_header*);
        void fill_strip_vlan(struct ofp_action_header*);
        void fill_dl_addr(struct ofp_action_header*, uint16_t type, uint64_t addr);
        void fill_nw_addr(struct ofp_action_header*, uint16_t type, uint32_t addr);
        void fill_nw_tos(struct ofp_action_header*);
        void fill_tp_port(struct ofp_action_header*, uint16_t type, uint16_t port);
};

#endif/*_OF_ACTIONS_*/
