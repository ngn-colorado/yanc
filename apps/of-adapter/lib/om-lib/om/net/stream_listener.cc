//
//  Olli's C++ Library [https://bitbucket.org/omichel/om-lib]
//  net/stream_listener.cc
//  (c) 2014 Oliver Michel <oliver dot michel at editum dot de>
//  http://ngn.cs.colorado/~oliver
//

#include <cstring>
#include <errno.h>
#include <unistd.h>

#include "stream_listener.h"

om::net::StreamListener::StreamListener()
	:	om::async::MultiplexInterface() {}

om::net::StreamListener::StreamListener(const om::net::tp_addr addr,
	std::function<void (om::net::StreamListener*)> new_conn_handler)
	throw(std::runtime_error, std::invalid_argument)
	:	om::async::MultiplexInterface()
{
	this->open(addr, new_conn_handler);
}

int om::net::StreamListener::open(const om::net::tp_addr addr,
	std::function<void (om::net::StreamListener*)> new_conn_handler)
	throw(std::runtime_error, std::logic_error, std::invalid_argument)
{
	int fd, yes = 1;
	struct sockaddr_in addr_struct;

	if(MultiplexInterface::fd() >= 0) 
		throw std::logic_error("Socket already opened");
  
	if(addr.proto != om::net::tp_proto_tcp)
		throw std::invalid_argument("ip_endpoint must be a TCP endpoint");

	// set new connection handling function
	_new_conn_handler = new_conn_handler;

	// create new stream socket
	if((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		::close(fd);
		throw std::runtime_error("socket(): " + std::string(strerror(errno)));
	}
		
	// reuse socket address if in use
	if(setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) < 0) {
		::close(fd);
		throw std::runtime_error("setsockopt(): " + std::string(strerror(errno)));
	}
		
	// fill sockaddr_in struct
	om::net::sockaddr_from_tp_addr(addr, &addr_struct);

	// bind newly created socket to address
	if(bind(fd, (struct sockaddr*)&addr_struct, sizeof(addr_struct)) < 0) {
		::close(fd);
		throw std::runtime_error("bind(): " + std::string(strerror(errno)));
	}
		
	// begin listening
	if(listen(fd, PENDING_CONN_QLEN) < 0) {
		::close(fd);
		throw std::runtime_error("bind(): " + std::string(strerror(errno)));
	}
	
	_addr = addr;
	
	MultiplexInterface::set_fd(fd);

	return fd;
}

void om::net::StreamListener::ready()
	throw(std::runtime_error, std::logic_error)
{
	if(_new_conn_handler)
		_new_conn_handler(this);
	else
		throw std::logic_error("StreamListener: no new connection handler set");
}

int om::net::StreamListener::accept(om::net::tp_addr* remote_addr)
	throw(std::runtime_error)
{
	int incoming_fd = -1;

	struct sockaddr_in addr_struct;
	socklen_t addr_struct_len = sizeof(struct sockaddr_in);
	
	incoming_fd = ::accept(
		MultiplexInterface::fd(),
		(sockaddr*)&addr_struct,
		&addr_struct_len
	);

	// accept new incoming connection and return new fd
	if(incoming_fd < 0)
		throw std::runtime_error("accept(): " + std::string(strerror(errno)));	

	if(remote_addr) {
		om::net::tp_addr_from_sockaddr(&addr_struct, remote_addr);
		remote_addr->proto = om::net::tp_proto_tcp;
	}
		
	return incoming_fd;
}

void om::net::StreamListener::close()
	throw(std::logic_error, std::runtime_error)
{
	if(MultiplexInterface::fd() == 0)
		throw std::logic_error("Socket was already ::closed or never opened");

	if(::close(MultiplexInterface::fd()) == 0)
		MultiplexInterface::set_fd(0);
	else
		throw std::runtime_error("::close(): " + std::string(strerror(errno)));
}

om::net::StreamListener::~StreamListener()
{
	if(MultiplexInterface::fd() != 0)
		::close(MultiplexInterface::fd());
}

namespace om {
	namespace net {
		std::ostream& operator<<(std::ostream& out, 
			const om::net::StreamListener& sl)
		{
			out << "StreamListener(" << sl._addr.to_string() << ")";
			return out;
		}
	}
}
