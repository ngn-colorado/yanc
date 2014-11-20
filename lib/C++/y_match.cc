#include "y_match.h"

YMatch::YMatch(std::string path) :
	wildcards(    path +   "/wildcards"),
	in_port(      path +   "/in_port"),
	dl_src(       path +   "/dl_src"),
	dl_dst(       path +   "/dl_dst"),
	dl_vlan(      path +   "/dl_vlan"),
	dl_vlan_pcp(  path +   "/dl_vlan_pcp"),
	dl_type(      path +   "/dl_type"),
	nw_tos(       path +   "/nw_tos"),
	nw_proto(     path +   "/nw_proto"),
	nw_src(       path +   "/nw_src"),
	nw_dst(       path +   "/nw_dst"),
	tp_src(       path +   "/tp_src"),
	tp_dst(       path +   "/tp_dst"),
	_path(path) {}

std::string YMatch::path() const
{
	return _path;
}

