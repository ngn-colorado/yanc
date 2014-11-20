//
//  Olli's C++ Library [https://bitbucket.org/omichel/om-lib]
//  ipc/dbus/message.h
//  (c) 2014 Oliver Michel <oliver dot michel at editum dot de>
//  http://ngn.cs.colorado/~oliver
//

#ifndef OM_IPC_DBUS_MESSAGE_H
#define OM_IPC_DBUS_MESSAGE_H

#include <dbus/dbus.h>
#include <stdexcept>
#include <string>
#include <typeinfo>
#include <vector>
#include <iostream>

namespace om {
	namespace ipc {
		namespace dbus {

			class Message {

			public:

				enum mechanism
				{ 
					mechanism_created = 0,
					mechanism_casted  = 1
				};
	
				Message() = delete;
				Message(DBusMessage* msg);
				Message(int type) throw (std::runtime_error);
				Message(int type, std::string destination, std::string interface,
					std::string member) throw (std::runtime_error);
				
				Message(Message& copy_from);

				Message& operator=(Message& copy_from);

				std::string destination();
				void set_destination(std::string destination);
				bool has_destination();
				std::string interface();
				void set_interface(std::string interface);
				bool has_interface();
				std::string member();
				void set_member(std::string member);
				bool has_member();
				std::string path();
				void set_path(std::string path);
				bool has_path();

				size_t num_args();

				void append_int32(int32_t arg);
				int32_t get_int32(size_t pos)
					throw(std::logic_error, std::invalid_argument, std::out_of_range);

				void append_string(std::string arg);
				std::string get_string(size_t pos)
					throw(std::logic_error, std::invalid_argument, std::out_of_range);

				std::vector<int> arguments() const;

				std::string description() const;

				static std::string arg_type_description(int arg_type);

				~Message();

			private:

				DBusMessage* _message;
				int _type;

				mechanism _creation_mechanism;

				void _init_iter(DBusMessageIter* iter) const
					throw(std::logic_error);

				void _advance_iter(DBusMessageIter* iter, size_t k) const
					throw(std::out_of_range);

				friend class Connection;

				friend std::ostream& operator<<(std::ostream& os, const Message& msg)
				{
					return (os << msg.description());
				}

			};

		}
	}
}

#endif