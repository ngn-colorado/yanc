//
//  Olli's C++ Library [https://bitbucket.org/omichel/om-lib]
//  net/agent.cc
//  (c) 2012 Oliver Michel <oliver dot michel at editum dot de>
//  http://ngn.cs.colorado/~oliver
//

#include <cstdio>
#include <cstring>
#include <errno.h>
#include <typeinfo>
#include <unistd.h>

#include <om/net/inotify_handler.h>
#include <om/tools/random.h>
#include <om/tools/time.h>

#include "agent.h"

om::net::Agent::Agent() 
	: 	_interfaces(new std::map<int, om::net::IOInterface*>), 
		_timeout_mode(om::net::Agent::timeout_mode_none),
		_manual_timeout(0), _uniform_lower(0), _uniform_upper(0),
		_exponential_lambda(0), _current_timeout() {}

om::net::Agent::Agent(const om::net::Agent& copy_from) 
	: 	_interfaces(copy_from._interfaces), 
		_timeout_mode(copy_from._timeout_mode),
		_manual_timeout(copy_from._manual_timeout),
		_uniform_lower(copy_from._uniform_lower),
		_uniform_upper(copy_from._uniform_upper),
		_exponential_lambda(copy_from._exponential_lambda),
		_current_timeout(copy_from._current_timeout) {}

om::net::Agent& om::net::Agent::operator=(const om::net::Agent& copy_from)
{
	_interfaces = copy_from._interfaces;
	_timeout_mode = copy_from._timeout_mode;
	_manual_timeout = copy_from._manual_timeout;
	_uniform_lower = copy_from._uniform_lower;
	_uniform_upper = copy_from._uniform_upper;
	_exponential_lambda = copy_from._exponential_lambda;
	_current_timeout = copy_from._current_timeout;

	return *this;
}

std::map<int, om::net::IOInterface*>* om::net::Agent::interfaces() const
{
	return _interfaces;
}

std::map<int, om::net::IOInterface*>
	om::net::Agent::interfaces(std::type_index type) const
{
	std::map<int, IOInterface*> return_ifaces;

	for(std::pair<int, IOInterface*> iface : *_interfaces)
		if(std::type_index(typeid(iface.second)) == type)
			return_ifaces[iface.first] = iface.second;

	return return_ifaces;
}

void om::net::Agent::set_timeout_mode(om::net::Agent::timeout_mode_t t)
{
	_timeout_mode = t;
}

om::net::Agent::timeout_mode_t om::net::Agent::timeout_mode() const
{
	return _timeout_mode;
}

void om::net::Agent::set_manual_timeout(double t)
{
	_manual_timeout = t;
}

double om::net::Agent::manual_timeout() const
{
	return _manual_timeout;
}

void om::net::Agent::set_uniform_lower(double l)
{
	_uniform_lower = l;
}

double om::net::Agent::uniform_lower() const
{
	return _uniform_lower;
}

void om::net::Agent::set_uniform_upper(double u)
{
	_uniform_upper = u;
}

double om::net::Agent::uniform_upper() const
{
	return _uniform_upper;
}

void om::net::Agent::set_exponential_lambda(double l)
{
	_exponential_lambda = l;
}

double om::net::Agent::exponential_lambda() const
{
	return _exponential_lambda;
}

void om::net::Agent::add_interface(om::net::IOInterface* iface)
	throw(std::logic_error)
{
	if(_interfaces->insert(std::make_pair(iface->fd(), iface)).second) {

		iface->add_to_fd_set(&_fds);
		this->_update_fd_max();

	} else
		throw std::logic_error("device is already added to this agent");
}

void om::net::Agent::remove_interface(IOInterface* iface)
	throw(std::logic_error) {

	std::map<int, IOInterface*>::iterator i = _interfaces->find(iface->fd());

	if(i != _interfaces->end()) {

		_interfaces->erase(i);
		iface->remove_from_fd_set(&_fds);
		this->_update_fd_max();

	} else 
		throw std::logic_error("device is not registered with this agent");
}

void om::net::Agent::_update_fd_max()
{
	// set initial max_fd to stdout
	int max = 2;

	for(std::pair<int,om::net::IOInterface*> iface : *_interfaces)
		max = iface.first > max ? iface.first : max;

	_fd_max = max;
}

void om::net::Agent::_check_read_interfaces(timeval* timestamp)
{
	if(FD_ISSET(STDIN_FILENO+1, &_read_fds)) {
		char read_buf[1024] = {0};
		if(fgets(read_buf, 1024, stdin) != NULL)
			this->read_stdin(read_buf, strlen(read_buf), timestamp);
	}

	for(std::pair<int,om::net::IOInterface*> iface : *_interfaces) {
		// if the particular interface is readable
		if(FD_ISSET(iface.first, &_read_fds))
			// access value (interface) in map and invoke handle_read on it
			((*_interfaces)[iface.first])->handle_read();
	}  
}

timeval om::net::Agent::_next_timeout_timeval() 
	throw(std::invalid_argument, std::logic_error)
{
	switch(_timeout_mode) {
		
		case om::net::Agent::timeout_mode_manual:
			if(fabs(_manual_timeout) <= 0.0001)
				throw std::invalid_argument("_next_timeout_timeval(): manual t/o is 0");
			return om::tools::time::timeval_from_sec(_manual_timeout);
			break;
			
		case om::net::Agent::timeout_mode_uniform:

			if(fabs(_uniform_upper) <= 0.0001 && fabs(_uniform_upper) <= 0.0001)
				throw std::invalid_argument("_next_timeout_timeval(): uniform args are 0");
			return om::tools::time::timeval_from_sec(
				om::tools::random::uniform_sample(_uniform_lower, _uniform_upper)
			);
			break;
		
		case om::net::Agent::timeout_mode_exponential:

			if(fabs(_exponential_lambda) <= 0.0001)
				throw std::invalid_argument("_next_timeout_timeval(): exp args are 0");
			return om::tools::time::timeval_from_sec(
				om::tools::random::bounded_exponential_sample(_exponential_lambda, 0.25, 2000.0)
			);      
			break;

		case om::net::Agent::timeout_mode_none:
			throw std::logic_error("_next_timeout_timeval(): t/o mode set to none");
			break;
	}

	throw std::logic_error("_next_timeout_timeval(): no t/o mode set");    
}

om::net::IOInterface* om::net::Agent::random_interface() const
	throw(std::logic_error)
{
	int j = 0;
	std::map<int, IOInterface*>* interfaces = this->interfaces();

	if(!interfaces->size())
		throw std::logic_error("random_interface(): no interfaces avail");

	int r = (int) om::tools::random::uniform_sample(0, interfaces->size());
	std::map<int, int> fds;
	
	for(std::pair<int,om::net::IOInterface*> iface : *_interfaces)
		fds[j++] = iface.first;
	
	return interfaces->operator[](fds[r]);
}

om::net::IOInterface* 
	om::net::Agent::random_interface(std::type_index type) const
	throw(std::logic_error)
{
	int j = 0;
	std::map<int, IOInterface*> interfaces = this->interfaces(type);

	if(!interfaces.size())
		throw std::logic_error("random_interface(): no interfaces avail");

	int r = (int) om::tools::random::uniform_sample(0, interfaces.size());  
	std::map<int, int> fds;

	for(std::pair<int,om::net::IOInterface*> iface : interfaces)
		fds[j++] = iface.first;

	return interfaces[fds[r]];
}

om::net::IOInterface* om::net::Agent::interface_by_fd(int fd) const
	throw(std::logic_error)
{
	std::map<int, IOInterface*>::iterator i = this->interfaces()->find(fd);
		
	if(i == this->interfaces()->end())
		throw std::logic_error("interface_by_fd(): iface does not exist");

	return i->second;
}

void om::net::Agent::run()
	throw(std::runtime_error)
{
	timeval* timeout_ptr = 0;
	timeval timestamp = {0,0}, timeout_copy = {0,0};
	int n_read_devices = -1;

	gettimeofday(&timestamp, 0);
	this->agent_start(&timestamp);

	while(1) { // main application loop

		_read_fds = _fds;

		if(_timeout_mode != om::net::Agent::timeout_mode_none) {
			_current_timeout = this->_next_timeout_timeval();
			timeout_copy = _current_timeout, timeout_ptr = &_current_timeout;
		}

		// call select, pass null-pointers for write and error fds
		n_read_devices = select(_fd_max+1, &_read_fds, 0, 0, timeout_ptr);

		gettimeofday(&timestamp, 0);

		if(n_read_devices == 0) { // timeout triggered
		
			this->timeout_trigger(&timestamp, &timeout_copy);
		
		} else if(n_read_devices > 0) { // n devices ready for reading
		
			this->_check_read_interfaces(&timestamp);

		} else if(n_read_devices == -1) { // error occured
		
			throw std::runtime_error("select(): " + std::string(strerror(errno)));
		}
	}
}

om::net::Agent::~Agent()
{
	delete _interfaces;
}
