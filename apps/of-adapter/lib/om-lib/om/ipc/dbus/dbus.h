//
//  Olli's C++ Library [https://bitbucket.org/omichel/om-lib]
//  ipc/dbus/dbus.h
//  (c) 2014 Oliver Michel <oliver dot michel at editum dot de>
//  http://ngn.cs.colorado/~oliver
//

#ifndef OM_IPC_DBUS_DBUS_H
#define OM_IPC_DBUS_DBUS_H

#include <dbus/dbus.h>

#include <stdexcept>
#include <string>

namespace om {
	namespace ipc {
		namespace dbus {

			void init_msg_read_iter(DBusMessageIter* iter, DBusMessage* msg)
				throw(std::logic_error);

			std::string read_string(DBusMessageIter* iter, DBusMessage* msg)
				throw(std::logic_error);


			std::string msg_summary(DBusMessage* msg);

			struct message_signature
			{
				message_signature();
				message_signature(std::string i, std::string m);
				message_signature(DBusMessage* msg);

				std::string match_string(std::string type);

				bool operator==(const message_signature& other) const;
				bool operator<(const message_signature& other) const;

				std::string iface;
				std::string method;
			};
		}
	}
}

#endif
