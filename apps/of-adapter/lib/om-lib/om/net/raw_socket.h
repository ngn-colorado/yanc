//
//  Olli's C++ Library
//  net/raw_socket.h
//  (c) 2012 Oliver Michel
//

#ifndef OM_NET_RAW_SOCKET_H
#define OM_NET_RAW_SOCKET_H

#include <stdexcept>
#include <string>

#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "net.h"
#include "io_interface.h"

namespace om {
  namespace net {

    class RawSocket 
      : public virtual om::net::IOInterface {

    public:

      explicit RawSocket(const int family = AF_INET, 
        const int protocol = IPPROTO_RAW)
        throw(std::runtime_error);

      RawSocket(const om::net::RawSocket& copy_from);

      RawSocket& operator=(RawSocket& copy_from);

      int send(const om::net::nw_addr remote_addr, const char* tx_data, 
        const size_t data_len);
      
      int receive(om::net::nw_addr *from, int* protocol, unsigned char *rx_buf, 
        const size_t buf_len);

      ~RawSocket();

    private:

      int _family;
      int _protocol;

      int open(const int family, const int protocol) throw(std::runtime_error);
    };
  }
}

#endif
