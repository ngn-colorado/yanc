//
//  Olli's C++ Library [https://bitbucket.org/omichel/om-lib]
//  net/tunnel_device.cc
//  (c) 2013 Oliver Michel <oliver dot michel at editum dot de>
//  http://ngn.cs.colorado/~oliver
//  

#include <cstring>
#include <fcntl.h>
#include <linux/if_tun.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include "tunnel_device.h"

om::net::TunnelDevice::TunnelDevice()
	: om::net::IOInterface(),
		_flags(IFF_TUN | IFF_NO_PI) {}

om::net::TunnelDevice::TunnelDevice(std::string name,
	std::function<void (om::net::TunnelDevice*)> read_handler) 
	throw(std::runtime_error) 
	: om::net::IOInterface(), _name(name),
		_flags(IFF_TUN | IFF_NO_PI)
{  
	this->open(read_handler);
}

short int om::net::TunnelDevice::flags()
{
	return _flags;
}

void om::net::TunnelDevice::set_flags(short int flags)
{
	_flags = flags;
}

int om::net::TunnelDevice::open(
	std::function<void (om::net::TunnelDevice*)> read_handler) 
	throw(std::runtime_error, std::logic_error)
{
	if(_fd != 0) throw std::logic_error("Device already opened");

	struct ifreq ifr;
	int fd = -1;
	char tun_name[IFNAMSIZ];
	strcpy(tun_name, _name.c_str());
	
	// open tunnel device
	if((fd = ::open("/dev/net/tun", O_RDWR)) < 0)
		throw std::runtime_error("cannot open /dev/net/tun: " 
			+ std::string(strerror(errno)));

	// set up interface request
	memset(&ifr, 0, sizeof(ifr));
	strncpy(ifr.ifr_name, tun_name, IFNAMSIZ);
	ifr.ifr_flags = _flags;

	// request interface
	if((ioctl(fd, TUNSETIFF, (void *)&ifr)) < 0) {
		::close(fd);
		throw std::runtime_error("ioctl(): failed requesting virtual interface: " 
			+ std::string(strerror(errno)));
	}

	_fd = fd;
	_read_handler = read_handler;
	
	return fd;
}

void om::net::TunnelDevice::handle_read()
	throw(std::runtime_error, std::logic_error)
{
	if(_read_handler)
		_read_handler(this);
	else
		throw std::logic_error("TunnelDevice: no read handler set");
}

int om::net::TunnelDevice::write(const unsigned char *tx_data, 
	const size_t data_len)
{
	return ::write(_fd, tx_data, data_len);
}
			
int om::net::TunnelDevice::read(unsigned char *rx_buf, const size_t buf_len)
{
	return ::read(_fd, rx_buf, buf_len);
}

void om::net::TunnelDevice::close() 
	throw(std::logic_error)
{
	if(_fd == 0)
		throw std::logic_error("Device was not yet opened");

	::close(_fd);
}

om::net::TunnelDevice::~TunnelDevice()
{
	if(_fd != 0) {
		if(::close(_fd) == 0)
			_fd = 0;
		else
			throw std::runtime_error("::close(): " + std::string(strerror(errno)));   
	}
}
