
#include <unistd.h>
#include <iostream>
#include <random>

#include <om/net/net.h>
#include <om/net/datagram_socket.h>
#include <om/async/epoll_wrapper.h>
#include <om/async/multiplex_interface.h>

#include <sys/epoll.h>

class AsyncHandler {

public:

	AsyncHandler() : _random_engine(), _dist(2500) {}

	int timeout_generator()
	{
		return _dist(_random_engine);
	}

	void timeout_triggered(int t)
	{
		std::cout << "timeout triggered: " << t << std::endl;
	}

	void read_stdin(int fd)
	{
		std::string buf;
		std::cin >> buf;
		std::cout << "read stdin: " << buf <<  std::endl;
	}

	void read_from_socket(om::net::DatagramSocket* sock)
	{
		om::net::tp_addr remote_addr;
		unsigned char recv_buf[1024] = {0};

		sock->receive(&remote_addr, recv_buf, 1024);
		sock->send(remote_addr, recv_buf, 1024);

		std::cout << "read from socket: " << recv_buf << std::endl;		
	}

private:

	std::default_random_engine _random_engine;
	std::poisson_distribution<int> _dist;
};


int main(int argc, char const *argv[])
{
	using namespace std::placeholders;

	om::net::DatagramSocket udp_sock;
	om::async::EPollWrapper epoll;
	AsyncHandler handler;

	// setup timout mechanism
	epoll.set_timeout_generator(
		std::bind(&AsyncHandler::timeout_generator, &handler)
	);

	epoll.set_timeout_callback(
		std::bind(&AsyncHandler::timeout_triggered, &handler, _1)
	);

	// setup udp socket and bind incoming data handler
	om::net::tp_addr listen_addr("0.0.0.0", om::net::tp_proto_udp, 42742);

	udp_sock.open(listen_addr,
		std::bind(&AsyncHandler::read_from_socket, &handler, _1)
	);

	epoll.add_interface(&udp_sock, EPOLLIN);

	// use lambda expression to process standard input
	epoll.add_descriptor(STDIN_FILENO, EPOLLIN,
		[](int fd){
			std::string buf;
			std::cin >> buf;
			std::cout << "read stdin: " << buf <<  std::endl;
		});

	epoll.dispatch();

	return 0;
}
