//
// yanc: Yet Another Network Controller
// OpenFlow Adapter
//
// (c) 2013  Oliver Michel   <oliver.michel@colorado.edu>
//           Matthew Monaco  <matthew.monaco@colorado.edu>
//
// License: TBD
//

#ifndef YANC_OF_FLOW_H
#define YANC_OF_FLOW_H

#include <string>

#include <om/net/stream_connection.h>
#include "y_flow.h"
#include "of_match.h"
#include "of_actions.h"
#include "openflow-legacy/openflow.h"
//#include "callback_registrar.h"

class OFFlow : public YFlow
{
 public:
	OFFlow(om::net::StreamConnection* sc, /*CallbackRegistrar* cbr,*/ std::string path);
	~OFFlow();

	OFMatch        match;
	OFActions      actions;

	friend void cb_flow_version_change(std::string path, uint32_t mode, void* flow);


 	void update();

	uint16_t fill(struct ofp_flow_mod&);
 private:
	om::net::StreamConnection* _sc;
	//CallbackRegistrar* _cbr;
	uint32_t          _prev_version;
};

void cb_flow_version_change(std::string path, uint32_t mode, void* flow);

#endif
