//
//  Olli's C++ Library [https://bitbucket.org/omichel/om-lib]
//  tools/dbus_send_sig/dbus_call_meth.cc
//  (c) 2014 Oliver Michel <oliver dot michel at editum dot de>
//  http://ngn.cs.colorado/~oliver
//

#include <iostream>
#include <unistd.h>

#include <om/net/dbus_adapter.h>

// example: dbus_call_meth unix:path=/tmp/test_bus

om::net::DBusMethodCall method_call;

void connected(om::net::DBusAdapter* dbus)
{
	dbus->call_method(method_call);
}

int main(int argc, char const *argv[]) 
{
	if(argc < 5) {
		std::cerr << "usage: dbus_call_meth <bus> <dest> <obj> <iface> <m_name>" << std::endl;
		std::exit(EXIT_FAILURE);
	}

	method_call.set_dest(std::string(argv[2]));
	method_call.set_path(std::string(argv[3]));
	method_call.set_iface(std::string(argv[4]));
	method_call.set_method_name(std::string(argv[5]));

	om::net::DBusAdapter dbus;
	dbus.connect(std::string(argv[1]), "de.editum.omlib.dbus_call_meth", connected);

	return(EXIT_SUCCESS);
}
