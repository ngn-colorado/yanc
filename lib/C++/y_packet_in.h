#ifndef _Y_PACKET_IN_
#define _Y_PACKET_IN_

#include <stdint.h>
#include <string>
#include "dirent.h"
#include "directory.h"

class YPacketIn : public Directory
{
public:
	YPacketIn(std::string path);
	
	Dirent<uint32_t>    buffer_id;
	Dirent<uint16_t>    total_len;
	DirentRaw           data;
	Dirent<uint32_t>    in_port;
	Dirent<uint8_t>     reason;
};

#endif/*_Y_PACKET_OUT_*/
