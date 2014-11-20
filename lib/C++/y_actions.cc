#include "y_actions.h"

YActions::YActions(std::string path) :
	actions(      path +   "/actions"),
	output(       path +   "/output"),
	output_max_len(path +  "/output_max_len"),
	dl_src(       path +   "/dl_src"),
	dl_dst(       path +   "/dl_dst"),
	vlan(         path +   "/vlan"),
	vlan_pcp(     path +   "/vlan_pcp"),
	dl_type(      path +   "/type"),
	nw_tos(       path +   "/nw_tos"),
	nw_src(       path +   "/nw_src"),
	nw_dst(       path +   "/nw_dst"),
	tp_src(       path +   "/tp_src"),
	tp_dst(       path +   "/tp_dst"),
	_path(path) {}

std::string YActions::path() const
{
	return _path;
}

