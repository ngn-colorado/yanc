//
//  Olli's C++ Library
//  examples/example_net_stream_client.cc
//  (c) 2013 Oliver Michel
//

#include <iostream>
#include <set>
#include <string>
#include <cstdlib>
#include <typeinfo> 

#include <om/net/net.h>
#include <om/net/agent.h>
#include <om/net/stream_client.h>

class StreamExampleAgent : public om::net::Agent {

public:

  void device_ready(timeval* timestamp, om::net::IOInterface* iface) {

    om::net::StreamClient* sock = 0;
    unsigned char rx_buf[1500] = {0};
    ssize_t rx_bytes = 0;

    if(typeid(*iface) == typeid(om::net::StreamClient)) {
      
      sock = dynamic_cast<om::net::StreamClient*>(iface);
      rx_bytes = sock->receive(rx_buf, sizeof(rx_buf));
      std::cout << rx_bytes << "B received: " << rx_buf;

      if(rx_bytes > 0) {
        const char* msg = "thx";
        sock->send((const unsigned char*) msg, 4);
        std::cout << "said thanks" << std::endl;
      } else if(rx_bytes == 0) {
        std::cout << "SHUTDOWN" << std::endl;
        this->remove_interface(sock);
        sock->close();
        delete sock;
        exit(0);
      } else if(rx_bytes == -1) {
        std::cout << "ERROR" << std::endl;
      }

    }
  }

  void read_stdin(char* buf, size_t len, timeval* timestamp) {

    std::cout << "stdin: " << len << ":" << buf << std::endl;
  }

}; 


int main(int argc, char const *argv[]) {

  // open a connection to port 42742 at 192.168.56.2
  om::net::tp_addr remote("192.168.56.2", om::net::tp_proto_tcp, 42742);
  om::net::StreamClient* sock = new om::net::StreamClient(remote);

  StreamExampleAgent* a = new StreamExampleAgent;
  a->add_interface(sock);

  const char* msg = "hello";
  sock->send((const unsigned char*) msg, 6);

  a->run();

  return 0;
}
