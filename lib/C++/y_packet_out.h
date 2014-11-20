#ifndef _Y_PACKET_OUT_
#define _Y_PACKET_OUT_

#include <stdint.h>
#include <string>
#include "dirent.h"
#include "directory.h"
#include "y_actions.h"

class YPacketOut : public Directory
{
 public:
	YPacketOut(std::string path);

	Dirent<uint32_t>    buffer_id;
	Dirent<uint16_t>    in_port;
	DirentRaw           data;
	Dirent<std::string> state;
	YActions            actions;
};

#endif/*_Y_PACKET_OUT_*/
