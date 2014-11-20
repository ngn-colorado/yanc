//
//  Olli's C++ Library
//  tools/random.cc
//  (c) 2012 Oliver Michel
//

#include "random.h"

void om::tools::random::seed() {

  srand(time(NULL));
}

double om::tools::random::uniform_sample(double lower, double upper) {

  return (rand()/double(RAND_MAX)*(upper-lower)+lower);
}

double om::tools::random::exponential_sample(double lambda) {

  return (-log(rand()/double(RAND_MAX)))/lambda;
}

double om::tools::random::bounded_exponential_sample(double lambda, double lower, 
  double upper) {

  double s = 0;

  do {
    s = (-log(rand()/double(RAND_MAX)))/lambda;
  } while(s < lower || s > upper);

  return s;
}
    