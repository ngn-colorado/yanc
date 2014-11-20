#include <arpa/inet.h>
#include "of_actions.h"


#include <string.h>

uint16_t OFActions::fill(unsigned char* buf)
{
	unsigned char* cur = buf;
	std::string       str(this->actions);
	std::stringstream ss(str);

	do {
		struct ofp_action_header* hdr = (struct ofp_action_header*) cur;
		std::string tok;
		ss >> tok;

		if (tok.empty())
			continue;

		else if (tok == "output")
			fill_output(hdr);
		
		else if (tok == "vlan")
			fill_vlan(hdr);

		else if (tok == "vlan_pcp")
			fill_vlan_pcp(hdr);

		else if (tok == "strip_vlan")
			fill_strip_vlan(hdr);

		else if (tok == "dl_src")
			fill_dl_addr(hdr, OFPAT_SET_DL_SRC, (Eth) this->dl_src);

		else if (tok == "dl_dst")
			fill_dl_addr(hdr, OFPAT_SET_DL_DST, (Eth) this->dl_dst);

		else if (tok == "nw_src")
			fill_nw_addr(hdr, OFPAT_SET_NW_SRC, ((IP32)this->nw_src).address());

		else if (tok == "nw_dst")
			fill_nw_addr(hdr, OFPAT_SET_NW_DST, ((IP32)this->nw_dst).address());

		else if (tok == "nw_tos")
			fill_nw_tos(hdr);

		else if (tok == "tp_src")
			fill_tp_port(hdr, OFPAT_SET_TP_SRC, this->tp_src);

		else if (tok == "tp_dst")
			fill_tp_port(hdr, OFPAT_SET_TP_DST, this->tp_dst);

		else {
			std::cerr << "OFActions: ignoring unknown action: <" << tok << ">" << std::endl;
			continue;
		}

		cur += ntohs(hdr->len);

	} while (ss);

	return cur - buf;
}

void OFActions::fill_output(struct ofp_action_header* hdr)
{
	struct ofp_action_output* msg = (struct ofp_action_output*) hdr;
	msg->type    = htons(OFPAT_OUTPUT);
	msg->len     = htons(sizeof(struct ofp_action_output));
	msg->port    = htons(this->output);
	msg->max_len = htons(this->output_max_len);
}

void OFActions::fill_vlan(struct ofp_action_header* hdr)
{
	struct ofp_action_vlan_vid* msg = (struct ofp_action_vlan_vid*) hdr;
	msg->type     = htons(OFPAT_SET_VLAN_VID);
	msg->len      = htons(sizeof(struct ofp_action_vlan_vid));
	msg->vlan_vid = htons(this->vlan);
}

void OFActions::fill_vlan_pcp(struct ofp_action_header* hdr)
{
	struct ofp_action_vlan_pcp* msg = (struct ofp_action_vlan_pcp*) hdr;
	msg->type     = htons(OFPAT_SET_VLAN_PCP);
	msg->len      = htons(sizeof(*msg));
	msg->vlan_pcp = htons(this->vlan_pcp);
}

void OFActions::fill_strip_vlan(struct ofp_action_header* hdr)
{
	hdr->type     = htons(OFPAT_STRIP_VLAN);
	hdr->len      = htons(sizeof(*hdr));
}

void OFActions::fill_dl_addr(struct ofp_action_header* hdr, uint16_t type, uint64_t addr)
{
	struct ofp_action_dl_addr* msg = (struct ofp_action_dl_addr*) hdr;
	msg->type       = htons(type);
	msg->len        = htons(sizeof(*msg));
	msg->dl_addr[0] = (addr >> 40) & 0xFF;
	msg->dl_addr[1] = (addr >> 32) & 0xFF;
	msg->dl_addr[2] = (addr >> 24) & 0xFF;
	msg->dl_addr[3] = (addr >> 16) & 0xFF;
	msg->dl_addr[4] = (addr >>  8) & 0xFF;
	msg->dl_addr[5] = (addr >>  0) & 0xFF;
}

void OFActions::fill_nw_addr(struct ofp_action_header* hdr, uint16_t type, uint32_t addr)
{
	struct ofp_action_nw_addr* msg = (struct ofp_action_nw_addr*) hdr;
	msg->type    = htons(type);
	msg->len     = htons(sizeof(*msg));
	msg->nw_addr = htonl(addr);
}

void OFActions::fill_nw_tos(struct ofp_action_header* hdr)
{
	struct ofp_action_nw_tos* msg = (struct ofp_action_nw_tos*) hdr;
	msg->type    = htons(OFPAT_SET_NW_TOS);
	msg->len     = htons(sizeof(*msg));
	msg->nw_tos  = this->nw_tos;
}

void OFActions::fill_tp_port(struct ofp_action_header* hdr, uint16_t type, uint16_t port)
{
	struct ofp_action_tp_port* msg = (struct ofp_action_tp_port*) hdr;
	msg->type    = htons(type);
	msg->len     = htons(sizeof(*msg));
	msg->tp_port = htons(port);
}
