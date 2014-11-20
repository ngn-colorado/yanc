//
//  Olli's C++ Library [https://bitbucket.org/omichel/om-lib]
//  net/io_interface.h
//  (c) 2012 Oliver Michel <oliver dot michel at editum dot de>
//  http://ngn.cs.colorado/~oliver
//
//  Provides an abstraction for I/O interfaces compatible with om::net::Agent
//

#ifndef OM_NET_IO_INTERFACE_H
#define OM_NET_IO_INTERFACE_H

#include <stdexcept>
#include <sys/select.h>

#include <om/net/net.h>

namespace om {
	namespace net {

		class IOInterface {

		public:
			explicit IOInterface();
			IOInterface(const IOInterface& copy_from);
			IOInterface& operator=(const IOInterface& copy_from);

			int fd();

			void add_to_fd_set(fd_set* fds)
				throw(std::logic_error);
			
			void remove_from_fd_set(fd_set* fds)
				throw(std::logic_error);

			virtual void handle_read() = 0;

			virtual void close() {}
			virtual ~IOInterface() {}
		
		protected:
			int _fd;

		};
	}
}

#endif
