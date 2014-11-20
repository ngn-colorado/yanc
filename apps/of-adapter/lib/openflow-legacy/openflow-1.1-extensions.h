
#ifndef YANC_OF_ADAPTER_LIB_OPENFLOW_OPENFLOW_1_1_EXTENSIONS_H
#define YANC_OF_ADAPTER_LIB_OPENFLOW_OPENFLOW_1_1_EXTENSIONS_H

/* Description of a port */
struct ofp11_port {

	uint32_t port_no;
	uint8_t  pad[4];
	uint8_t  hw_addr[OFP_ETH_ALEN];
	uint8_t  pad2[2];

	char name[OFP_MAX_PORT_NAME_LEN]; /* Null-terminated */

	uint32_t config;        /* Bitmap of OFPPC_* flags. */
	uint32_t state;         /* Bitmap of OFPPS_* flags. */
   
   /* 
   	Bitmaps of OFPPF_* that describe features.  All bits zeroed if
   	unsupported or unavailable.
   */

	uint32_t curr; /* Current features. */
	uint32_t advertised; /* Features being advertised by the port. */
	uint32_t supported; /* Features supported by the port. */
	uint32_t peer; /* Features advertised by peer. */
	uint32_t curr_speed; /* Current port bitrate in kbps. */
	uint32_t max_speed; /* Max port bitrate in kbps */
};

OFP_ASSERT(sizeof(struct ofp11_port) == 64);

/* Packet received on port (datapath -> controller). */
struct ofp11_packet_in {
	
	struct ofp_header header;

	uint32_t buffer_id; /* ID assigned by datapath. */
	uint32_t in_port; /* Port on which frame was received. */
	uint32_t in_phy_port; /* Physical Port on which frame was received. */
	uint16_t total_len; /* Full length of frame. */
	uint8_t  reason; /* Reason packet is being sent (one of OFPR_*) */
	uint8_t  table_id; /* ID of the table that was looked up */
	uint8_t  data[0]; /* Ethernet frame, halfway through 32-bit word,
                        so the IP header is 32-bit aligned. The
                        amount of data is inferred from the length
                        field in the header. Because of padding,
                        offset of(struct ofp_packet_in, data) ==
                        sizeof(struct ofp_packet_in) - 2. */
};
OFP_ASSERT(sizeof(struct ofp11_packet_in) == 24);

#endif
