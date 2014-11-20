#include "y_port.h"

YPort::YPort(std::string path) :
	Directory(    path),
	port_no(      path + "/port_no"),
	hw_addr(      path + "/hw_addr"),
	name(         path + "/name"),
	config(       path + "/config"),
	state(        path + "/state"),
	f_curr(       path + "/f_curr"),
	f_advertised( path + "/f_advertised"),
	f_supported(  path + "/f_supported"),
	f_peer(       path + "/f_peer")
	{}
