#ifndef _Y_SWITCH_
#define _Y_SWITCH_

#include <stdint.h>
#include <string>
#include <map>
#include "dirent.h"
#include "directory.h"
#include "y_flow.h"
#include "y_port.h"

class YSwitch : public Directory
{
public:
	YSwitch(std::string path);

	Dirent<uint8_t>  version;
	Dirent<uint64_t> datapath_id;
	Dirent<uint32_t> n_buffers;
	Dirent<uint8_t>  n_tables;
	Dirent<uint32_t> capabilities;
	Dirent<uint32_t> actions;
	Dirent<uint16_t> flags;
	Dirent<uint16_t> miss_send_len;

	std::map<std::string, YFlow> flows;
	std::map<std::string, YPort> ports;
};

#endif
