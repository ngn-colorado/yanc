//
// yanc: Yet Another Network Controller
// OpenFlow Adapter
//
// (c) 2013  Matthew Monaco  <matthew.monaco@colorado.edu>
//           Oliver Michel   <oliver.michel@colorado.edu>
//
// License: TBD
//


#ifndef YANC_OF_ADAPTER_LIB_OPENFLOW_OPENFLOW_H
#define YANC_OF_ADAPTER_LIB_OPENFLOW_OPENFLOW_H

#include "openflow-1.0.h"
#include "openflow-1.1-extensions.h"

#include <stdint.h>

const uint8_t OFP_VERSION_1_0  = 0x01;
const uint8_t OFP_VERSION_1_1  = 0x02;
const uint8_t OFP_VERSION_1_2  = 0x03;
const uint8_t OFP_VERSION_1_3  = 0x04;
const uint8_t OFP_VERSION_1_4  = 0x05;

#endif
