//
//  Olli's C++ Library
//  tools/time.h
//  (c) 2012 Oliver Michel
//

#ifndef OM_TOOLS_TIME_H
#define OM_TOOLS_TIME_H

#include <sys/time.h>

#include <cmath>

#include "tools.h"

namespace om {
  namespace tools {
    namespace time {

      double utimediff(timeval a, timeval b);
      int utimediff_us(timeval a, timeval b);
      timeval timeval_from_sec(double s);
      timeval timeval_from_usec(int us);
      int usec_from_timeval(timeval t);
      double sec_from_timeval(timeval t);
    }
  }
}
#endif
