#ifndef _OF_MATCH_
#define _OF_MATCH_

#include "y_match.h"
#include "openflow-legacy/openflow.h"

class OFMatch : public YMatch {
 public:
	OFMatch(std::string path) : YMatch(path) {}
	void fill(struct ofp_match&);
};

#endif/*_OF_MATCH_*/
