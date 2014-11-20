//
//  Olli's C++ Library
//  examples/ex_tun_device.cc
//  (c) 2012 Oliver Michel
//

#include <iostream>
#include <set>
#include <string>

#include <netinet/in.h>

#include <om/net/net.h>
#include <om/net/datagram_socket.h>

int main(int argc, char const *argv[]) {

  // bind to any source ip address (0.0.0.0)
  om::net::tp_addr addr("192.168.56.4", om::net::tp_proto_udp, 42742);

  om::net::DatagramSocket* sock = new om::net::DatagramSocket(addr);

  om::net::tp_addr dest("192.168.60.1", om::net::tp_proto_udp, 42742);
  const char* msg = "hello";


  for(size_t i = 0; i < 5; i++) {  
    //send 6 bytes of data to 192.168.1.5 on port 42742
    sock->send(dest, (const unsigned char*) msg, 6);
  }
  // close and delete socket
  sock->close();
  delete sock;

  return 0;
}