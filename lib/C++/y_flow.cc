#include "y_flow.h"

YFlow::YFlow(std::string path) :
	cookie(       path +   "/cookie"),
	priority(     path +   "/priority"),
	version(      path +   "/version"),
	idle_timeout( path +   "/idle_timeout"),
	hard_timeout( path +   "/hard_timeout"),
	buffer_id(    path +   "/buffer_id"),
	match(        path +   "/match"),
	actions(      path +   "/actions"),
	_path(path) {}

std::string YFlow::path() const
{
	return _path;
}

