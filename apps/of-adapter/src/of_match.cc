#include <arpa/inet.h>
#include "of_match.h"

void OFMatch::fill(struct ofp_match& match)
{
	uint64_t dl_src, dl_dst;
	uint32_t wildcards;

	match.in_port   = htons(this->in_port);

	dl_src = (Eth) this->dl_src;
	dl_dst = (Eth) this->dl_dst;
	match.dl_src[0] = (dl_src >> 40) & 0xFF;
	match.dl_src[1] = (dl_src >> 32) & 0xFF;
	match.dl_src[2] = (dl_src >> 24) & 0xFF;
	match.dl_src[3] = (dl_src >> 16) & 0xFF;
	match.dl_src[4] = (dl_src >>  8) & 0xFF;
	match.dl_src[5] = (dl_src >>  0) & 0xFF;

	match.dl_dst[0] = (dl_dst >> 40) & 0xFF;
	match.dl_dst[1] = (dl_dst >> 32) & 0xFF;
	match.dl_dst[2] = (dl_dst >> 24) & 0xFF;
	match.dl_dst[3] = (dl_dst >> 16) & 0xFF;
	match.dl_dst[4] = (dl_dst >>  8) & 0xFF;
	match.dl_dst[5] = (dl_dst >>  0) & 0xFF;

	match.dl_vlan       = htons(this->dl_vlan);
	match.dl_vlan_pcp   = this->dl_vlan_pcp;
	match.dl_type       = htons(this->dl_type);

	match.nw_tos   = this->nw_tos;
	match.nw_proto = this->nw_proto;
	match.nw_src   = htonl(((IP32)this->nw_src).address());
	match.nw_dst   = htonl(((IP32)this->nw_dst).address());

	wildcards  = this->wildcards;
	wildcards |= ((IP32)this->nw_src).mask() << OFPFW_NW_SRC_SHIFT;
	wildcards |= ((IP32)this->nw_dst).mask() << OFPFW_NW_DST_SHIFT;

	match.tp_src   = htons(this->tp_src);
	match.tp_dst   = htons(this->tp_dst);

	match.wildcards = htonl(wildcards);
}

