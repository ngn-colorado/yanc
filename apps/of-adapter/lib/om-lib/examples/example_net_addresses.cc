//
//  Olli's C++ Library
//  examples/example_net_addresses.cc
//  (c) 2012 Oliver Michel
//

#include <iostream>
#include <set>
#include <string>
#include <cstring>

#include <netinet/in.h>

#include <om/tools/string.h>
#include <om/net/net.h>

int main(int argc, char const *argv[]) {

  // construct IP addresses using simple char* literals
  om::net::nw_addr addr1 = "255.255.255.255";
  om::net::nw_addr addr2 = "121.19.4.22";

  // a full L4 socket specification is given by the 3-tuple ip, protocol, port
  om::net::tp_addr e1 = om::net::tp_addr(addr1, 0x6, 80);
  om::net::tp_addr e2 = om::net::tp_addr(addr2, 0x6, 80);

  // pairs can be built on both the network and transport layer
  om::net::nw_pair p1(addr1, addr2);
  om::net::tp_pair p2(e1, e2);

  // when directly setting up connection-pairs, shorter statements are possible:
  om::net::tp_pair p3(
    om::net::tp_addr("192.168.0.1", om::net::tp_proto_udp, 8283),
    om::net::tp_addr("192.168.0.2", om::net::tp_proto_udp, 8283)
  );

  om::net::nw_addr addr3 = "0.0.0.0";

  // check if all bytes are set to zero
  if(addr3.is_empty())
    std::cout << "0.0.0.0 is empty" << std::endl;

  // read nw_addr from characters (e.g. when parsing packets)
  unsigned char ip[] = {0x12, 0x0, 0x01, 0xf9};
  om::net::nw_addr addr4(ip[0], ip[1], ip[2], ip[3]);
  std::cout << "read from bytes: " << addr4.to_string() << std::endl;

  uint32_t int_rep = addr4.packed();
  om::net::nw_addr addr5(int_rep);

  if(addr4 == addr5)
    std::cout << "integer conversion successful" << std::endl;

  // explicitely call conversion functions to c++ and c strings
  std::cout << addr2.to_string() << std::endl;
  std::cout << addr3.to_cstring() << std::endl;

  // translate between transport layer addresses and sockaddr_in structs 
  sockaddr_in i;

  om::net::sockaddr_from_tp_addr(&e1, &i);
  om::net::sockaddr_from_tp_addr(e1, &i);

  om::net::tp_addr e3;
  om::net::tp_addr_from_sockaddr(&i, &e3);

  std::set<om::net::tp_pair> flowtable;

  flowtable.insert(p3);

  om::net::tp_pair p4(
    om::net::tp_addr("192.168.1.1", om::net::tp_proto_udp, 8283),
    om::net::tp_addr("192.168.1.2", om::net::tp_proto_udp, 8283)
  );  

  flowtable.insert(p4);

  if(flowtable.find(p3) != flowtable.end())
    std::cout << " flowtable contains p3" << std::endl;
  else
    std::cout << " flowtables does not contain p3" << std::endl;

  unsigned char buf[14] = {0}, buf1[7] = {0}, buf2[7] = {0};

  e1.copy_bytes(buf1);
  e2.copy_bytes(buf2);

  memcpy(buf, buf1, 7);
  memcpy(buf+7, buf2, 7);

  return 0;
}
