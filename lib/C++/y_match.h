#ifndef _Y_MATCH_
#define _Y_MATCH_

#include <stdint.h>
#include <string>
#include "dirent.h"

class YMatch
{
 public:
	YMatch(std::string path);

	std::string path() const;

	Dirent<uint32_t>   wildcards;
	Dirent<uint16_t>   in_port;
        DirentEth          dl_src;
        DirentEth          dl_dst;
	Dirent<uint16_t>   dl_vlan;
	Dirent<uint8_t>    dl_vlan_pcp;
	Dirent<uint16_t>   dl_type;
	Dirent<uint8_t>    nw_tos;
	Dirent<uint8_t>    nw_proto;
        DirentIP32         nw_src;
        DirentIP32         nw_dst;
	Dirent<uint16_t>   tp_src;
	Dirent<uint16_t>   tp_dst;

 private:
	std::string _path;
};

#endif/*_Y_MATCH_*/
