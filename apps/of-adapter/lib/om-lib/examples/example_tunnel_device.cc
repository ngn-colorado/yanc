//
//  Olli's C++ Library
//  examples/ex_tun_device.cc
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

// use an agent to respond to interface activity ansynchronously

// om::net::Agent::device_ready is the only pure virtual function
// that needs to be implemented

class ExampleAgent : public om::net::Agent {

  void device_ready(timeval* timestamp, om::net::IOInterface* iface) {

    size_t rx_bytes = 0;
    unsigned char rx_buf[1500] = {0};

    rx_bytes = ((om::net::TunnelDevice*)iface)->read(rx_buf, sizeof(rx_buf));
    std::cout << "ExampleAgent#device_ready: received " << rx_bytes << std::endl; 
  }
};


int main(int argc, char const *argv[]) {

  // request the tun interface from operating system  
  om::net::TunnelDevice* tun = new om::net::TunnelDevice("tun0");

  // allocate previously defined agent
  ExampleAgent* a = new ExampleAgent;
  
  // add any object that inherits from om::net::IOInterface to the agent
  a->add_interface(tun);

  // fire up the agent (blocking)
  a->run();

  delete a, delete tun;

  return 0;
}
