//
//  Olli's C++ Library
//  examples/ex_net_agent.cc
//  (c) 2012 Oliver Michel
//

#include <unistd.h>
#include <sys/types.h>

#include <ctime>
#include <iostream>

#include <om/net/net.h>
#include <om/net/agent.h>
#include <om/net/io_interface.h>
#include <om/net/tunnel_device.h>
#include <om/net/datagram_socket.h>

// use an agent to respond to interface activity ansynchronously

// om::net::Agent::device_ready is the only pure virtual function
// that needs to be implemented

class ExampleAgent : public om::net::Agent {

  void device_ready(timeval* timestamp, om::net::IOInterface* iface) {

  }

  void timeout_trigger(timeval* timestamp, timeval* timeout_tv) {

    std::cout << "timeout triggered" << std::endl;

    std::cout << "random interface: " << this->random_interface()->fd() 
      << std::endl;

    std::cout << "random dgram sock interface: "
      << this->random_interface(om::net::IOInterface::iface_type_sock_dgram)->fd()
      << std::endl;
  }
};

int main(int argc, char const *argv[]) {

  om::net::tp_addr s1addr("0.0.0.0", om::net::tp_proto_udp, 42741);
  om::net::tp_addr s2addr("0.0.0.0", om::net::tp_proto_udp, 42742);
  om::net::tp_addr s3addr("0.0.0.0", om::net::tp_proto_udp, 42743);
  om::net::tp_addr s4addr("0.0.0.0", om::net::tp_proto_udp, 42744);

  om::net::DatagramSocket* s1 = new om::net::DatagramSocket(s1addr);
  om::net::DatagramSocket* s2 = new om::net::DatagramSocket(s2addr);
  om::net::DatagramSocket* s3 = new om::net::DatagramSocket(s3addr);
  om::net::DatagramSocket* s4 = new om::net::DatagramSocket(s4addr);

  // allocate previously defined agent
  ExampleAgent* a = new ExampleAgent;

  // configure a time out mode
  a->set_timeout_mode(om::net::Agent::timeout_mode_manual);
  a->set_manual_timeout(0.2);
  
  // add any object that inherits from om::net::IOInterface to the agent
  a->add_interface(s1);
  a->add_interface(s2);
  a->add_interface(s3);
  a->add_interface(s4);

  // fire up the agent (blocking)
  a->run();

  delete a, delete s1, delete s2, delete s3, delete s4;

  return 0;
}