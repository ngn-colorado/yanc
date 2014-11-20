#ifndef _Y_ACTIONS_
#define _Y_ACTIONS_

#include <stdint.h>
#include <string>
#include "dirent.h"

class YActions
{
 public:
	YActions(std::string path);

	std::string path() const;

	Dirent<std::string> actions;
        Dirent<uint16_t>   output;
        Dirent<uint16_t>   output_max_len;
        DirentEth          dl_src;
        DirentEth          dl_dst;
        Dirent<uint16_t>   vlan;
        Dirent<uint8_t>    vlan_pcp;
        Dirent<uint16_t>   dl_type;
        Dirent<uint8_t>    nw_tos;
        DirentIP32         nw_src;
        DirentIP32         nw_dst;
        Dirent<uint16_t>   tp_src;
        Dirent<uint16_t>   tp_dst;

 private:
	std::string _path;
};

#endif/*_Y_ACTIONS_*/
