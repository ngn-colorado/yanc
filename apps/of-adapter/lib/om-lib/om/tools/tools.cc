//
//  Olli's C++ Library
//  tools/tools.cc
//  (c) 2012 Oliver Michel
//

#include "tools.h"

namespace om {
  namespace tools {
    struct om::tools::options_t options;
  }
}

std::ostream& om::tools::msg_stream(int level) {

  std::clog.clear(level <= options.log_level
    ? std::ios_base::goodbit : std::ios_base::badbit);
  
  return std::clog;
}
