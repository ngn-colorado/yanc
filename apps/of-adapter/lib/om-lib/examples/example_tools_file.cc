//
//  Olli's C++ Library
//  (c) 2013 Oliver Michel <oliver.michel@editum.de>
//

#include <iostream>
#include <stdint.h>

#include <om/tools/file.h>

int main(int argc, char const *argv[]) {

  uint16_t t16 = 0xFFFF;

  om::tools::file::write_uint_lit<uint16_t>("/home/olli/src/om-lib/test.txt", t16);

  uint8_t t8 = 0;

  t8 = om::tools::file::read_uint_lit<uint8_t>("/home/olli/src/om-lib/test2.txt");

  std::cout << "read " << (unsigned) t8 << std::endl;

  return 0;
}
