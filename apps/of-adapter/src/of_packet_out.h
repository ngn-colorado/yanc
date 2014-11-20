#ifndef _OF_PACKET_OUT_
#define _OF_PACKET_OUT_

#include <string>
#include <map>
#include <om/net/stream_connection.h>
#include "openflow-legacy/openflow.h"
#include "y_packet_out.h"
#include "of_actions.h"

class OFPacketOut : public YPacketOut {
 public:
	OFPacketOut(std::string path, om::net::StreamConnection*);
	OFActions actions;

	void update();	


	//friend void cb_state_change(std::string path, uint32_t mode, void* pktout);
 private:
	om::net::StreamConnection* _sc;
};

// void cb_state_change(std::string path, uint32_t mode, void* pktout);

#endif/*_OF_PACKET_OUT_*/
