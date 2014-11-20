#ifndef _Y_PORT_
#define _Y_PORT_

#include <stdint.h>
#include <string>
#include "dirent.h"
#include "directory.h"

class YPort : public Directory
{
 public:
	YPort(std::string path);

	Dirent<uint32_t>    port_no;
	DirentEth           hw_addr;;
	Dirent<std::string> name;
	Dirent<uint32_t>    config;
	Dirent<uint32_t>    state;
	Dirent<uint32_t>    f_curr;
	Dirent<uint32_t>    f_advertised;
	Dirent<uint32_t>    f_supported;
	Dirent<uint32_t>    f_peer;
};

#endif/*_Y_PORT_*/
