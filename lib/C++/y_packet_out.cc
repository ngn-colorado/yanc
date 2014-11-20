#include "y_packet_out.h"

YPacketOut::YPacketOut(std::string path) :
	Directory(  path),
	buffer_id(  path + "/buffer_id"),
	in_port(    path + "/in_port"),
	data(       path + "/data"),
	state(      path + "/state"),
	actions(    path + "/actions")
{}

