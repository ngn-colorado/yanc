//
//  Olli's C++ Library
//  examples/example_net_packing.cc
//  (c) 2012 Oliver Michel
//

#include <iostream>

#include <om/net/net.h>

int main(int argc, char const *argv[]) {

  unsigned char c1 = 0x7f;
  unsigned char carr1[] = {0x7f};

  std::cout << "converted: " << (int) om::net::unpack_uint8(c1) << std::endl;
  std::cout << "converted: " << (int) om::net::unpack_uint8(carr1) << std::endl;

  unsigned char carr2[] = {0xff, 0xff};

  std::cout << "converted: " << (int) om::net::unpack_uint16(carr2) << std::endl;

  unsigned int i8 = 234;
  unsigned char c8 = 0x0;

  om::net::pack_uint8(i8, &c8);
  std::cout << "converted: " << std::hex << (unsigned) c8 << std::endl;

  unsigned int i16 = 56020;
  unsigned char c16[] = {0x0, 0x0};

  om::net::pack_uint16(i16, c16);
  std::cout << "converted: " << std::hex << (unsigned) c16[0] 
    << (unsigned) c16[1] << std::endl;

  return 0;
}
