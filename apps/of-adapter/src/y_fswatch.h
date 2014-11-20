//
// yanc: Yet Another Network Controller
// OpenFlow Adapter
//
// (c) 2013  Matthew Monaco  <matthew.monaco@colorado.edu>
//           Oliver Michel   <oliver.michel@colorado.edu>
//
// License: TBD
//

#ifndef Y_FSWATCH_H
#define Y_FSWATCH_H

#define YFS_MAX_IDENT_LEN 64

enum y_fswatch_type {
  y_fswatch_type_flows_dir         = 0,
  y_fswatch_type_flows_staged      = 1,
  y_fswatch_type_packetout_dir     = 2,
  y_fswatch_type_packetout_staged  = 3
};

struct y_fswatch {
  y_fswatch_type type;
  uint64_t dpid;
  char name[YFS_MAX_IDENT_LEN];
};

#endif
