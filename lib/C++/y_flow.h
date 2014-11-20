#ifndef _Y_FLOW_
#define _Y_FLOW_

#include <stdint.h>
#include <string>
#include "dirent.h"
#include "y_match.h"
#include "y_actions.h"

class YFlow
{
 public:
	YFlow(std::string path);

	std::string path() const;

        Dirent<uint64_t>   cookie;
        Dirent<uint16_t>   priority;
        Dirent<uint32_t>   version;
        Dirent<uint16_t>   idle_timeout;
        Dirent<uint16_t>   hard_timeout;
	Dirent<uint32_t>   buffer_id;
        YMatch             match;
        YActions           actions;

 private:
	std::string _path;
};

#endif/*_Y_FLOW_*/
