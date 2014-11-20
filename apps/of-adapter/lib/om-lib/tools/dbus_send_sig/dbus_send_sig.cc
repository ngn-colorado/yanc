//
//  Olli's C++ Library [https://bitbucket.org/omichel/om-lib]
//  tools/dbus_send_sig/dbus_send_sig.cc
//  (c) 2014 Oliver Michel <oliver dot michel at editum dot de>
//  http://ngn.cs.colorado/~oliver
//

#include <iostream>
#include <unistd.h>

#include <om/net/dbus_adapter.h>

// example: dbus_send_sig unix:path=/tmp/test_bus /TestObj org.TestInterface Hello

om::net::DBusSignal sig;

void connected(om::net::DBusAdapter* dbus)
{
	dbus->send_signal(sig);
}

int main(int argc, char const *argv[]) 
{
	if(argc < 3) {
		std::cerr << "usage: dbus_send_sig <bus> <obj> <iface> <name>" << std::endl;
		std::exit(EXIT_FAILURE);
	}

	sig.set_path(std::string(argv[2]));
	sig.set_iface(std::string(argv[3]));
	sig.set_name(std::string(argv[4]));

	om::net::DBusAdapter dbus;

	dbus.connect(std::string(argv[1]), "de.editum.omlib.dbus_send_sig", connected);

	return(EXIT_SUCCESS);
}
