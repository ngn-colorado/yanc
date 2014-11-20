
#include <unistd.h>
#include <iostream>

#include <om/net/net.h>
#include <om/async/epoll_wrapper.h>
#include <om/async/multiplex_interface.h>

#include <om/ipc/dbus/dbus.h>
#include <om/ipc/dbus/message.h>
#include <om/ipc/dbus/connection.h>

#include <sys/epoll.h>


class DBusHandler {

public:

	DBusHandler() {}

	void read_stdin(int fd)
	{
		std::string buf;
		std::cin >> buf;
		std::cout << "read stdin: " << buf <<  std::endl;
	}

	void dbus_connected(om::ipc::dbus::Connection* c)
	{
		std::cout << "DBusHandler: connected()" << std::endl;
	}

	DBusHandlerResult default_handler(om::ipc::dbus::Connection* c, DBusMessage* m)
	{
		std::cout << "DBusHandler: default_handler()" << std::endl;

		om::ipc::dbus::Message p(m);
		std::cout << p.description() << std::endl;


		return DBUS_HANDLER_RESULT_HANDLED;
	}

	DBusHandlerResult test_handler(om::ipc::dbus::Connection* c, DBusMessage* m)
	{
		std::cout << "DBusHandler: test_handler()" << std::endl;

		om::ipc::dbus::Message p(m);
		std::cout << p.description() << std::endl;

		return DBUS_HANDLER_RESULT_HANDLED;
	}
};

int main(int argc, char const *argv[])
{
	using namespace std::placeholders;

	DBusHandler handler;
	om::async::EPollWrapper epoll;

	om::ipc::dbus::Connection c;

	c.set_default_handler(
		std::bind(&DBusHandler::default_handler, &handler, _1, _2)
	);

	c.open("unix:path=/tmp/test_bus", "de.editum.omlib.examples.DBus",
		std::bind(&DBusHandler::dbus_connected, &handler, _1)
	);

	c.set_signal_handler("de.editum.omlib", "Test",
		std::bind(&DBusHandler::test_handler, &handler, _1, _2)
	);

	epoll.add_interface(&c, EPOLLIN | EPOLLERR);

	epoll.dispatch();

	return 0;
}
