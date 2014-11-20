#include <arpa/inet.h>
#include <iostream>
#include <sstream>
#include "of_packet_out.h"
using namespace std;

OFPacketOut::OFPacketOut(string path, om::net::StreamConnection* sc) :
	YPacketOut(path),
	actions(path+"/actions"),
	_sc(sc)
{
	cerr << "OFPacketOut DEBUG new " << this->path() << endl;
/*
	_cbr->add_fs_callback(path + "/state",
			IN_CLOSE_WRITE, cb_state_change, (void*)this);
*/
	update();
}


void OFPacketOut::update()
{
	unsigned char buf[4096];
	struct ofp_packet_out* msg = (struct ofp_packet_out*) buf;
	uint16_t len, alen;
	char* data;

	string state = (string) this->state;
	size_t pos   = state.find_last_not_of(" \t\n\r");

if (pos != string::npos)
		state = state.substr(0,pos+1);

	len  = sizeof(struct ofp_packet_out);
	alen = this->actions.fill((unsigned char*) &msg->actions);
	len += alen;

	data = ((char*) buf) + len;
	len += this->data.get(data, sizeof(buf) - len);

	msg->header.version = OFP_VERSION_1_0;
	msg->header.type    = OFPT_PACKET_OUT;
	msg->header.length  = htons(len);
	msg->header.xid     = htonl(0);
	msg->buffer_id      = htonl(this->buffer_id);
	msg->in_port        = htons(this->in_port);
	msg->actions_len    = htons(alen);

	cerr << "OFPacketOut DEBUG " << this->path()
	     << " sent PACKET_OUT of length " << len << endl;
	this->_sc->send(buf, len);
}


/*
void cb_state_change(std::string _, uint32_t __, void* p_)
{
	OFPacketOut* out = (OFPacketOut*) p_;
	unsigned char buf[4096];
	struct ofp_packet_out* msg = (struct ofp_packet_out*) buf;
	uint16_t len, alen;
	char* data;

	string state = (string) out->state;
	size_t pos   = state.find_last_not_of(" \t\n\r");
	if (pos != string::npos)
		state = state.substr(0,pos+1);

	if (state != "ready")
		return;

	len  = sizeof(struct ofp_packet_out);
	alen = out->actions.fill((unsigned char*) &msg->actions);
	len += alen;

	data = ((char*) buf) + len;
	len += out->data.get(data, sizeof(buf) - len);

	msg->header.version = OFP_VERSION_1_0;
	msg->header.type    = OFPT_PACKET_OUT;
	msg->header.length  = htons(len);
	msg->header.xid     = htonl(0);
	msg->buffer_id      = htonl(out->buffer_id);
	msg->in_port        = htons(out->in_port);
	msg->actions_len    = htons(alen);

	cerr << "OFPacketOut DEBUG " << out->path()
	     << " sent PACKET_OUT of length " << len << endl;
	out->_sc->send(buf, len);
}
*/