//
//  Olli's C++ Library [https://bitbucket.org/omichel/om-lib]
//  async/multiplex_interface.cc
//  (c) 2014 Oliver Michel <oliver dot michel at editum dot de>
//  http://ngn.cs.colorado/~oliver
//


#include "multiplex_interface.h"

om::async::MultiplexInterface::MultiplexInterface()
	: _fd(-1) {}

om::async::MultiplexInterface::MultiplexInterface(int fd)
	: _fd(fd) {}

int om::async::MultiplexInterface::fd()
{
	return _fd;
}

void om::async::MultiplexInterface::set_fd(int fd)
{
	_fd = fd;
}

om::async::MultiplexInterface::~MultiplexInterface() {}
