//
//  Olli's C++ Library [https://bitbucket.org/omichel/om-lib]
//  net/datagram_socket.cc
//  (c) 2014 Oliver Michel <oliver dot michel at editum dot de>
//  http://ngn.cs.colorado/~oliver
//

#include <errno.h>
#include <cstring>
#include <unistd.h>
#include "datagram_socket.h"

om::net::DatagramSocket::DatagramSocket()
	: om::async::MultiplexInterface() {}

om::net::DatagramSocket::DatagramSocket(const om::net::tp_addr addr,
	std::function<void (om::net::DatagramSocket*)> read_handler) 
	throw(std::runtime_error, std::invalid_argument)
	: om::async::MultiplexInterface()
{
	this->open(addr, read_handler);
}

int om::net::DatagramSocket::open(const om::net::tp_addr addr,
	std::function<void (om::net::DatagramSocket*)> read_handler) 
	throw(std::runtime_error, std::logic_error, std::invalid_argument)
{
	if(MultiplexInterface::fd() >= 0) 
		throw std::logic_error("Socket already opened");
	
	if(addr.proto != om::net::tp_proto_udp)
		throw std::invalid_argument("ip_endpoint must be an UDP endpoint");

	int fd, yes = 1;
	struct sockaddr_in addr_struct;

	// create new datagram socket
	if((fd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
		throw std::runtime_error("socket(): " + std::string(strerror(errno)));

	// reuse socket address if in use
	if(setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
		throw std::runtime_error("setsockopt(): " + std::string(strerror(errno)));

	// fill sockaddr_in struct
	om::net::sockaddr_from_tp_addr(addr, &addr_struct);

	// bind newly created socket to address
	if(bind(fd, (struct sockaddr*)&addr_struct, sizeof(addr_struct)) == -1)
		throw std::runtime_error("bind(): " + std::string(strerror(errno)));

	MultiplexInterface::set_fd(fd);

	_addr = addr;
	_read_handler = read_handler;

	return fd;
}

void om::net::DatagramSocket::ready()
	throw(std::runtime_error, std::logic_error)
{
	if(_read_handler)
		_read_handler(this);
	else
		throw std::logic_error("DatagramSocket: no read handler set");
}

int om::net::DatagramSocket::send(const om::net::tp_addr remote_addr, 
	const unsigned char* tx_data, const size_t data_len)
{
	struct sockaddr_in remote_addr_struct;
	om::net::sockaddr_from_tp_addr(remote_addr, &remote_addr_struct);
	
	int tx_bytes = sendto(MultiplexInterface::fd(), tx_data, data_len, 0, 
		(struct sockaddr *)&remote_addr_struct, sizeof(remote_addr_struct));

	return tx_bytes;
}

int om::net::DatagramSocket::receive(om::net::tp_addr* from, 
	unsigned char* rx_buf, const size_t buf_len)
{
	struct sockaddr_in rx_addr;
	socklen_t addr_len = sizeof(rx_addr);

	int rx_bytes = recvfrom(MultiplexInterface::fd(), rx_buf, buf_len, 0,
		(struct sockaddr *)&rx_addr, &addr_len);

	if(rx_bytes == -1)
		throw std::runtime_error("recvfrom(): " + std::string(strerror(errno)));

	from->addr = om::net::nw_addr(rx_addr.sin_addr.s_addr);
	from->port = ntohs(rx_addr.sin_port);
	from->proto = om::net::tp_proto_udp;

	return rx_bytes;
}

void om::net::DatagramSocket::close() 
	throw(std::logic_error, std::runtime_error)
{
	if(MultiplexInterface::fd() == 0)
		throw std::logic_error("Socket was not yet opened");

	if(::close(MultiplexInterface::fd()) == 0)
		MultiplexInterface::set_fd(0);
	else
		throw std::runtime_error("::close(): " + std::string(strerror(errno)));

	::close(MultiplexInterface::fd());
}

om::net::DatagramSocket::~DatagramSocket()
{
	if(MultiplexInterface::fd() != 0)
		::close(MultiplexInterface::fd());
}
