//
//  Olli's C++ Library [https://bitbucket.org/omichel/om-lib]
//  net/stream_listener.h
//  (c) 2014 Oliver Michel <oliver dot michel at editum dot de>
//  http://ngn.cs.colorado/~oliver
//
//  implements a listening TCP socket
//

#ifndef OM_NET_STREAM_LISTENER_H
#define OM_NET_STREAM_LISTENER_H

#include <functional>
#include <ostream>
#include <stdexcept>

#include <om/net/net.h>
#include <om/async/multiplex_interface.h>

namespace om {
	namespace net {

		class StreamListener : public om::async::MultiplexInterface {
		
		public:
			
			// the length of the pending connection backlog for connections
			// that have not been accepted yet 
			static const int PENDING_CONN_QLEN = 32;

			// constructs a new StreamListener object without opening a socket
			explicit StreamListener();

			// constructs a new StreamListener object and opens the socket
			explicit StreamListener(const om::net::tp_addr addr, 
				std::function<void (om::net::StreamListener*)> new_conn_handler) 
				throw(std::runtime_error, std::invalid_argument);

			// begins listening on a specified TCP port and passes new incoming
			// connections to an instance of om::net::Agent
			int open(const om::net::tp_addr addr, 
				std::function<void (om::net::StreamListener*)> new_conn_handler)
				throw(std::runtime_error, std::logic_error, std::invalid_argument);

			// to be invoked by multiplexer
			void ready()
				throw(std::runtime_error, std::logic_error);

			// accepts a new incoming connection and returns a new fd for this
			// connection, the peer address is written into remote_addr
			int accept(om::net::tp_addr* remote_addr)
				throw(std::runtime_error);

			// terminates the listener
			void close()
				throw(std::logic_error, std::runtime_error);
			
			// default destructor
			~StreamListener();

			friend std::ostream& operator<<(std::ostream&, const StreamListener&);

		private:
			
			om::net::tp_addr _addr;
			std::function<void (om::net::StreamListener*)> _new_conn_handler;

			StreamListener(const om::net::StreamListener &copy_from);
			StreamListener& operator=(StreamListener& copy_from);
		};
	}
}

#endif
