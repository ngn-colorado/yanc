//
//  Olli's C++ Library [https://bitbucket.org/omichel/om-lib]
//  net/stream_connection.cc
//  (c) 2013 Oliver Michel <oliver dot michel at editum dot de>
//  http://ngn.cs.colorado/~oliver
//

#include <cstring>
#include <unistd.h>

#include "stream_connection.h"

om::net::StreamConnection::StreamConnection()
	: om::async::MultiplexInterface() {}

om::net::StreamConnection::StreamConnection(int fd,
	std::function<void (om::net::StreamConnection*)> read_handler)
	throw(std::logic_error, std::invalid_argument)
	: om::async::MultiplexInterface()
{
	this->attach(fd, read_handler);
}

om::net::StreamConnection::StreamConnection(int fd, om::net::tp_addr remote_addr,
	std::function<void (om::net::StreamConnection*)> read_handler)
	throw(std::logic_error, std::invalid_argument)
	: om::async::MultiplexInterface()
{
	this->attach(fd, remote_addr, read_handler);
}

om::net::tp_addr om::net::StreamConnection::remote_addr()
{
	return _remote_addr;
}

void om::net::StreamConnection::attach(int fd,
	std::function<void (om::net::StreamConnection*)> read_handler)
	throw(std::logic_error, std::invalid_argument)
{
	if(MultiplexInterface::fd() != -1)
		throw std::logic_error("StreamConnection is already attached to a socket");

	if(fd < 0)
		throw std::invalid_argument("Invalid file descriptor");

	MultiplexInterface::set_fd(fd);
	_read_handler = read_handler;
}

void om::net::StreamConnection::attach(int fd, om::net::tp_addr remote_addr,
	std::function<void (om::net::StreamConnection*)> read_handler)
	throw(std::logic_error, std::invalid_argument)
{
	_remote_addr = remote_addr;
	this->attach(fd, read_handler);
}

void om::net::StreamConnection::ready()
	throw(std::runtime_error, std::logic_error)
{
	if(_read_handler)
		_read_handler(this);
	else
		throw std::logic_error("StreamConnection: no read handler set");
}

int om::net::StreamConnection::send(const unsigned char* tx_buf,
	const size_t buf_len)
{
	int tx_bytes = ::send(MultiplexInterface::fd(), tx_buf, buf_len, 0);
	return tx_bytes;
}

int om::net::StreamConnection::receive(unsigned char* rx_buf,
	const size_t buf_len)
{
	int rx_bytes = ::recv(MultiplexInterface::fd(), rx_buf, buf_len, 0);
	return rx_bytes;
}

void om::net::StreamConnection::close()
	throw(std::logic_error, std::runtime_error)
{
	if(MultiplexInterface::fd() == 0)
		throw std::logic_error("Socket was already ::closed or never opened");

	if(::close(MultiplexInterface::fd()) == 0)
		MultiplexInterface::set_fd(0);
	else
		throw std::runtime_error("::close(): " + std::string(strerror(errno)));
}

om::net::StreamConnection::~StreamConnection()
{
	if(MultiplexInterface::fd() != 0)
		::close(MultiplexInterface::fd());
}

namespace om {
	namespace net {
		std::ostream& operator<<(std::ostream& out, 
			const om::net::StreamConnection& sc)
		{
			out << "StreamConnection(" << sc._remote_addr.to_string() << ")";
			return out;
		}
	}
}
