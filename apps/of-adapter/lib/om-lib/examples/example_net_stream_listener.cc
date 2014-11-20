//
//  Olli's C++ Library [https://bitbucket.org/omichel/om-lib]
//  examples/example_net_stream_listener.cc
//  (c) 2013 Oliver Michel <oliver dot michel at editum dot de>
//  http://ngn.cs.colorado/~oliver
//

#include <iostream>
#include <set>
#include <string>
#include <typeinfo> 

#include <om/net/agent.h>
#include <om/net/net.h>
#include <om/net/stream_connection.h>
#include <om/net/stream_listener.h>

class ListeningExampleAgent : public om::net::Agent {}; 

int main(int argc, char const *argv[]) {

	// initiate the agent
	ListeningExampleAgent a;

	auto read_handler = [&a](om::net::StreamConnection* c) {
		unsigned char buf[1024] = {0};
		c->receive(buf, 1024);
		std::cout << buf << std::endl;
	};

	auto conn_handler = [&a, &read_handler](om::net::StreamListener* l) {
		std::cout << "new connection" << std::endl;
		om::net::tp_addr remote;
		a.add_interface(
			new om::net::StreamConnection(l->accept(&remote), remote, read_handler)
		);	
	};

	om::net::StreamListener* listener = new om::net::StreamListener(
		om::net::tp_addr("0.0.0.0", om::net::tp_proto_tcp, 42742), conn_handler);

	a.add_interface(listener);


	// start up the agent
	a.run();

	return 0;
}
