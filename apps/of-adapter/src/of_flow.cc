#include <arpa/inet.h>
#include "of_flow.h"
using std::string;
using std::cerr;
using std::endl;

OFFlow::OFFlow(om::net::StreamConnection* sc, /*CallbackRegistrar* cbr,*/ string path)
	: YFlow(path)
	, match(path + "/match")
	, actions(path + "/actions")
	, _sc(sc)
//	, _cbr(cbr)
{
	cerr << "OFFlow DEBUG new " << this->path() << endl;
/*
	_cbr->add_fs_callback(path + "/version",
			IN_MODIFY, cb_flow_version_change, (void*)this);
*/
	_prev_version = this->version;
	this->update();
}

OFFlow::~OFFlow()
{
	// rm callback
}


void OFFlow::update()
{
	uint32_t ver = version;
	unsigned char buf[4096];
	struct ofp_flow_mod* mod = (struct ofp_flow_mod*) buf;
	uint16_t len;

	_prev_version = ver;

	len = fill(*mod);
	mod->header.version  = 0x1;
	mod->header.type     = OFPT_FLOW_MOD;
	mod->header.length   = htons(len);
	mod->header.xid      = htonl(0);

	cerr << "OFFlow DEBUG " << this->path() << " sent FLOW_MOD of length " << len << endl;

	_sc->send(buf, len);
}

uint16_t OFFlow::fill(struct ofp_flow_mod& mod)
{
	uint16_t len = sizeof(struct ofp_flow_mod);

	if (this->version == 1)
		mod.command = htons(OFPFC_ADD);
	else
		mod.command = htons(OFPFC_MODIFY);

	mod.command   = htons(OFPFC_ADD);
	mod.cookie    = this->cookie;
	mod.idle_timeout = htons(this->idle_timeout);
	mod.hard_timeout = htons(this->hard_timeout);
	mod.priority  = htons(this->priority);
	mod.buffer_id = htonl(this->buffer_id);
	mod.out_port  = htons(OFPP_NONE);
	mod.flags     = htons(0);

	match.fill(mod.match);
	len += actions.fill((unsigned char*) &mod.actions);

	return len;
}

void cb_flow_version_change(string _, uint32_t __, void* f_)
{
	OFFlow* flow = (OFFlow*) f_;
	uint32_t ver = flow->version;
	unsigned char buf[4096];
	struct ofp_flow_mod* mod = (struct ofp_flow_mod*) buf;
	uint16_t len;

	if (flow->_prev_version >= ver)
		return;
	flow->_prev_version = ver;

	len = flow->fill(*mod);
	mod->header.version  = 0x1;
	mod->header.type     = OFPT_FLOW_MOD;
	mod->header.length   = htons(len);
	mod->header.xid      = htonl(0);

	cerr << "OFFlow DEBUG " << flow->path() << " sent FLOW_MOD of length " << len << endl;

	flow->_sc->send(buf, len);
}
