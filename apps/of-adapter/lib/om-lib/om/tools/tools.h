//
//  Olli's C++ Library
//  tools/tools.h
//  (c) 2012 Oliver Michel
//

#ifndef OM_TOOLS_TOOLS_H
#define OM_TOOLS_TOOLS_H

#include <iostream>

namespace om {
  namespace tools {

    struct options_t {
      options_t() : log_level(0) {}
      int log_level;
    };

    extern struct options_t options;
    
    std::ostream& msg_stream(int level = 0);
  }
}

#endif
