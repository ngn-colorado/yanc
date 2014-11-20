#include "y_packet_in.h"

YPacketIn::YPacketIn(std::string path) :
	Directory(  path),
	buffer_id(  path + "/buffer_id"),
	total_len(  path + "/total_len"),
	data(       path + "/data"),
	in_port(    path + "/in_port"),
	reason(     path + "/reason")
	{}
	