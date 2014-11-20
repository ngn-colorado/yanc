//
//  Olli's C++ Library [https://bitbucket.org/omichel/om-lib]
//  net/io_interface.cc
//  (c) 2012 Oliver Michel <oliver dot michel at editum dot de>
//  http://ngn.cs.colorado/~oliver
//

#include "io_interface.h"

om::net::IOInterface::IOInterface() 
	: _fd(0) {}

om::net::IOInterface::IOInterface(const om::net::IOInterface &copy_from) 
	: _fd(copy_from._fd) {}

om::net::IOInterface& 
	om::net::IOInterface::operator=(const om::net::IOInterface& copy_from) {

	_fd = copy_from._fd;
	return *this;
}

int om::net::IOInterface::fd() {

	return _fd;
}

void om::net::IOInterface::add_to_fd_set(fd_set *fds)
	throw(std::logic_error) {

	if(_fd == 0)
		throw std::logic_error("add_to_fd_set(): Interface is not opened");

	FD_SET(_fd, fds);
}

void om::net::IOInterface::remove_from_fd_set(fd_set *fds)
	throw(std::logic_error) {

	if(_fd == 0)
		throw std::logic_error("remove_from_fd_set: Interface is not opened");
	
	FD_CLR(_fd, fds);
}
