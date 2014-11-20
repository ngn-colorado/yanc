//
//  Olli's C++ Library
//  examples/example_raw_socket.cc
//  (c) 2012 Oliver Michel
//

#include <iostream>
#include <set>
#include <string>

#include <netinet/in.h>

#include <om/net/net.h>
#include <om/net/raw_socket.h>

int main(int argc, char const *argv[]) {

  om::net::RawSocket* sock = new om::net::RawSocket();

  om::net::nw_addr dest = "192.168.1.5";
  const char* msg = "hello";
  
  sock->send(dest, (const char*) msg, 6);

  delete sock;

  return 0;
}
