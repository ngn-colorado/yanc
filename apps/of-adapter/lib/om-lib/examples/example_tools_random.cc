//
//  Olli's C++ Library
//  examples/example_tools_random.cc
//  (c) 2012 Oliver Michel
//

#include <iostream>

#include <om/tools/random.h>

int main(int argc, char const *argv[]) {

  for(size_t i = 0; i <= 500; i++)
    std::cout << om::tools::random::bounded_exponential_sample(0.8, 0.5, 2.5) << std::endl;

  return 0;
}
