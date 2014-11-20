//
//  Olli's C++ Library [https://bitbucket.org/omichel/om-lib]
//  async/epoll_wrapper.cc
//  (c) 2014 Oliver Michel <oliver dot michel at editum dot de>
//  http://ngn.cs.colorado/~oliver
//

#include "epoll_wrapper.h"

#include <sys/epoll.h>
#include <string>
#include <cstring>
#include <iostream>
#include <sstream>

om::async::EPollWrapper::EPollWrapper(size_t size_hint)
	throw(std::runtime_error)
	: Multiplexer(), _epfd(-1)
{
	if((_epfd = epoll_create(size_hint)) < 0)
		throw std::runtime_error("EPollWrapper: epoll_create: "
			+ std::string(strerror(errno)));
}

void om::async::EPollWrapper::add_interface(
	om::async::MultiplexInterface* iface, uint32_t events)
	throw(std::runtime_error, std::logic_error)
{
	Multiplexer::add_interface(iface);
	
	struct epoll_event ev { .events = events, .data = { .fd = iface->fd() } };

	if(epoll_ctl(_epfd, EPOLL_CTL_ADD, iface->fd(), &ev) < 0) {

		if(errno == EEXIST || errno == ENOENT)
			throw std::logic_error("EPollWrapper: add_interface: "
				+ std::string("epoll_ctl: ")
				+ std::string(strerror(errno))
			);
		else
			throw std::runtime_error("EPollWrapper: add_interface: "
				+ std::string("epoll_ctl: ")
				+ std::string(strerror(errno))
			);
	}
}

void om::async::EPollWrapper::remove_interface(
	om::async::MultiplexInterface* iface)
	throw(std::runtime_error, std::logic_error)
{
	Multiplexer::remove_interface(iface);

	if(epoll_ctl(_epfd, EPOLL_CTL_DEL, iface->fd(), 0) < 0) {

		if(errno == ENOENT)
			throw std::logic_error("EPollWrapper: remove_interface: "
				+ std::string("epoll_ctl: ")
				+ std::string(strerror(errno))
			);
		else
			throw std::runtime_error("EPollWrapper: remove_interface: "
				+ std::string("epoll_ctl: ")
				+ std::string(strerror(errno))
			);;
	}
}


void om::async::EPollWrapper::add_descriptor(int fd, uint32_t events,
	event_handler handler)
	throw(std::runtime_error, std::logic_error)
{
	Multiplexer::add_descriptor(fd, handler);

	struct epoll_event ev { .events = events, .data = { .fd = fd } };

	if(epoll_ctl(_epfd, EPOLL_CTL_ADD, fd, &ev) < 0) {

		if(errno == EEXIST || errno == ENOENT)
			throw std::logic_error("EPollWrapper: add_interface: "
				+ std::string("epoll_ctl: ")
				+ std::string(strerror(errno))
			);
		else
			throw std::runtime_error("EPollWrapper: add_interface: "
				+ std::string("epoll_ctl: ")
				+ std::string(strerror(errno))
			);
	}
}

void om::async::EPollWrapper::remove_descriptor(int fd)
	throw(std::runtime_error, std::logic_error)
{
	Multiplexer::remove_descriptor(fd);

	if(epoll_ctl(_epfd, EPOLL_CTL_DEL, fd, 0) < 0) {

		if(errno == ENOENT)
			throw std::logic_error("EPollWrapper: remove_descriptor: "
				+ std::string("epoll_ctl: ")
				+ std::string(strerror(errno))
			);
		else
			throw std::runtime_error("EPollWrapper: remove_descriptor: "
				+ std::string("epoll_ctl: ")
				+ std::string(strerror(errno))
			);;
	}
}

std::string om::async::EPollWrapper::description()
{
	std::stringstream ss;

	ss << "Multiplexer: " << std::endl;

	for(std::pair<int, callback_context> f : _fds)
		std::cout << f.first << std::endl;

	return ss.str();
}

void om::async::EPollWrapper::dispatch()
	throw(std::runtime_error)
{
	int timeout = -1, nfds = 0;
	static struct epoll_event events[_qlen];

	// set initial timeout if timeout generator is registered
	if(_timeout_generator)
		timeout = _timeout_generator();

	while(1) {

		// wait on registered file descriptors
		nfds = epoll_wait(_epfd, events, _qlen, timeout);

		if(nfds < 0) { // handle epoll error

			throw std::runtime_error("EPollWrapper: dispatch: "
				+ std::string("epoll_wait: ")
				+ std::string(strerror(errno))
			);

		} else if(nfds == 0) { // handle epoll timeout

			timeout = _trigger_timeout(timeout);

		} else { // handle regular epoll return

			for(int i = 0; i < nfds; i++) { // iterate over fds and invoke callback
				
				callback_context* cb = &(_fds[events[i].data.fd]);

				if(cb->interface != 0) {
					// if target is derived from om::async::MultiplexInterface
					cb->interface->ready();
				} else {
					// if target is generic file descriptor
					cb->handler(events[i].data.fd);
				}
			}
		}
	}
}

int om::async::EPollWrapper::_trigger_timeout(int timeout)
	throw(std::logic_error)
{
	if(!_timeout_callback)
		throw std::logic_error("EPollWrapper: dispatch: "
			+ std::string("no timeout callback function set")
		);
	else {
		_timeout_callback(timeout);
		return _timeout_generator();
	}
}
