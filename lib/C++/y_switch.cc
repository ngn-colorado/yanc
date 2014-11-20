#include "y_switch.h"

YSwitch::YSwitch(std::string path) :
	Directory(path),
	version(        path + "/version"),
	datapath_id(    path + "/datapath_id"),
	n_buffers(      path + "/n_buffers"),
	n_tables(       path + "/n_tables"),
	capabilities(   path + "/capabilities"),
	actions(        path + "/actions"),
	flags(          path + "/flags"),
	miss_send_len(  path + "/miss_send_len")
	{}
