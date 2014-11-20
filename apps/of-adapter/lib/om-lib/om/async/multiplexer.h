//
//  Olli's C++ Library [https://bitbucket.org/omichel/om-lib]
//  async/multiplexer.h
//  (c) 2014 Oliver Michel <oliver dot michel at editum dot de>
//  http://ngn.cs.colorado/~oliver
//

#ifndef OM_ASYNC_MULTIPLEXER
#define OM_ASYNC_MULTIPLEXER

#include <functional>
#include <map>
#include <stdexcept>

#include "multiplex_interface.h"

namespace om {
	namespace async {

		class Multiplexer {

		public:

			typedef std::function<void (int)> timeout_callback;
			
			typedef std::function<void (int)> event_handler;

			typedef std::function<int ()> timeout_generator;

			Multiplexer();
			Multiplexer(Multiplexer& copy_from) = delete;
			Multiplexer& operator=(Multiplexer& copy_from) = delete;

			void set_timeout_generator(timeout_generator tg);
			void set_timeout_callback(timeout_callback cb);

			virtual void add_interface(om::async::MultiplexInterface* iface)
				throw(std::runtime_error, std::logic_error);

			virtual void remove_interface(om::async::MultiplexInterface* iface)
				throw(std::runtime_error, std::logic_error);

			virtual void add_descriptor(int fd, event_handler handler)
				throw(std::runtime_error, std::logic_error);

			virtual void remove_descriptor(int fd)
				throw(std::runtime_error, std::logic_error);

			virtual void dispatch() = 0;

			bool monitors_fd(int fd);

			virtual ~Multiplexer();

		protected:

			struct callback_context
			{	
				callback_context();
				
				callback_context(
					event_handler h, om::async::MultiplexInterface* i);
				
				callback_context(const callback_context& copy_from);
				
				event_handler handler;	
				om::async::MultiplexInterface* interface;
			};

			std::map<int, callback_context> _fds;
			timeout_callback _timeout_callback;
			timeout_generator _timeout_generator;

		};
	}
}

#endif