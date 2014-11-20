//
//  Olli's C++ Library
//  tools/time.cc
//  (c) 2012 Oliver Michel
//

#include "time.h"

double om::tools::time::utimediff(timeval a, timeval b) {
  
  long sec, usec;
  sec  = b.tv_sec  - a.tv_sec;
  usec = b.tv_usec - a.tv_usec;
  return (sec * 1000000 + usec)/1000000.0;
}

int om::tools::time::utimediff_us(timeval a, timeval b) {

  int sec, usec;
  sec  = b.tv_sec  - a.tv_sec;
  usec = b.tv_usec - a.tv_usec;
  return(sec * 1000000 + usec);
}

timeval om::tools::time::timeval_from_sec(double s) {

  return om::tools::time::timeval_from_usec((int)(s * pow(10,6)));
}

timeval om::tools::time::timeval_from_usec(int us) {

  timeval t;
  int usec_rmdr = us % (int)pow(10,6);
  t.tv_sec = (us - usec_rmdr) / (int)pow(10,6);
  t.tv_usec = usec_rmdr;

  return t;
}

int om::tools::time::usec_from_timeval(timeval t) {

  return t.tv_sec * pow(10,6) + t.tv_usec;
}

double om::tools::time::sec_from_timeval(timeval t) {

  return t.tv_sec + t.tv_usec/(double)pow(10,6);
}
