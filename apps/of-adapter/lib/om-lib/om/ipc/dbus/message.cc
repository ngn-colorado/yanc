//
//  Olli's C++ Library [https://bitbucket.org/omichel/om-lib]
//  ipc/dbus/message.cc
//  (c) 2014 Oliver Michel <oliver dot michel at editum dot de>
//  http://ngn.cs.colorado/~oliver
//

#include "message.h"

#include <iostream>
#include <sstream>

om::ipc::dbus::Message::Message(DBusMessage* msg)
	:	_message(msg),
		_type(dbus_message_get_type(msg)),
		_creation_mechanism(mechanism_casted) {}

om::ipc::dbus::Message::Message(int type)
	throw(std::runtime_error)
	:	_message(0), 
		_type(type),
		_creation_mechanism(mechanism_created)
{
	if(!(_message = dbus_message_new(_type)))
		throw std::runtime_error("Message: not enough memory");
}

om::ipc::dbus::Message::Message(int type, std::string destination,
	std::string interface, std::string member)
	throw(std::runtime_error)
	:	_message(0), 
		_type(type),
		_creation_mechanism(mechanism_casted)
{
	if(!(_message = dbus_message_new(_type)))
		throw std::runtime_error("Message: not enough memory");

	set_destination(destination);
	set_interface(interface);
	set_member(member);
}

om::ipc::dbus::Message::Message(Message& copy_from)
	: 	_message(dbus_message_copy(copy_from._message)),
		_type(copy_from._type),
		_creation_mechanism(copy_from._creation_mechanism) {}

om::ipc::dbus::Message& om::ipc::dbus::Message::operator=(Message& copy_from)
{
	_message = dbus_message_copy(copy_from._message);
	_type = copy_from._type;
	_creation_mechanism = copy_from._creation_mechanism;

	return *this;
}

std::string om::ipc::dbus::Message::destination()
{
	const char* destination = dbus_message_get_destination(_message);
	return destination ? std::string(destination) : std::string();
}

void om::ipc::dbus::Message::set_destination(std::string destination)
{
	dbus_message_set_destination(_message, destination.c_str());
}

bool om::ipc::dbus::Message::has_destination()
{
	return dbus_message_get_destination(_message) ? true : false;
}

std::string om::ipc::dbus::Message::interface()
{
	const char* interface = dbus_message_get_interface(_message);
	return interface ? std::string(interface) : std::string();
}

void om::ipc::dbus::Message::set_interface(std::string interface)
{
	dbus_message_set_interface(_message, interface.c_str());
}

bool om::ipc::dbus::Message::has_interface()
{
	return dbus_message_get_interface(_message) ? true : false;
}

std::string om::ipc::dbus::Message::member()
{
	const char* member = dbus_message_get_member(_message);
	return member ? std::string(member) : std::string();
}

void om::ipc::dbus::Message::set_member(std::string member)
{
	dbus_message_set_member(_message, member.c_str());
}

bool om::ipc::dbus::Message::has_member()
{
	return dbus_message_get_member(_message) ? true : false;
}

std::string om::ipc::dbus::Message::path()
{
	const char* path = dbus_message_get_path(_message);
	return path ? std::string(path) : std::string();
}

void om::ipc::dbus::Message::set_path(std::string path)
{
	dbus_message_set_path(_message, path.c_str());
}

bool om::ipc::dbus::Message::has_path()
{
	return dbus_message_get_path(_message) ? true : false;
}

size_t om::ipc::dbus::Message::num_args()
{
	size_t i = 0;
	DBusMessageIter iter;

	// catch logic error when message has no arguments and return 0
	try { _init_iter(&iter); } catch(std::logic_error) { return 0; }
	
	do{ i++; } while (dbus_message_iter_next(&iter));

	return i;
}

void om::ipc::dbus::Message::append_int32(int32_t arg)
{
	dbus_message_append_args(_message, DBUS_TYPE_INT32, &arg, DBUS_TYPE_INVALID);
}

int32_t om::ipc::dbus::Message::get_int32(size_t pos)
	throw(std::logic_error, std::invalid_argument, std::out_of_range)
{
	int32_t val;
	DBusMessageIter iter;

	_init_iter(&iter);
	_advance_iter(&iter, pos);

	if(dbus_message_iter_get_arg_type(&iter) != DBUS_TYPE_INT32)
		throw std::logic_error("Message::get_int32(): argumet is not int32");

	dbus_message_iter_get_basic(&iter, &val);

	return val;
}

void om::ipc::dbus::Message::append_string(std::string arg)
{
	// copy to char array: lvalue as operand for & needed
	const char* arg_c_copy = arg.c_str();

	dbus_message_append_args(_message, DBUS_TYPE_STRING, &arg_c_copy,
		DBUS_TYPE_INVALID);
}

std::string om::ipc::dbus::Message::get_string(size_t pos)
	throw(std::logic_error, std::invalid_argument, std::out_of_range)
{
	char* val;
	DBusMessageIter iter;
	
	_init_iter(&iter);
	_advance_iter(&iter, pos);

	if(dbus_message_iter_get_arg_type(&iter) != DBUS_TYPE_STRING)
		throw std::logic_error("Message::get_string(): argumet is not string");

	dbus_message_iter_get_basic(&iter, &val);

	return std::string(val);
}

std::vector<int> om::ipc::dbus::Message::arguments() const
{
	std::vector<int> v;

	DBusMessageIter iter;
	
	// catch logic error when message has no arguments and return empty vector
	try { _init_iter(&iter); } catch(std::logic_error) { return v; }

	do {
		v.push_back(dbus_message_iter_get_arg_type(&iter));
	} while (dbus_message_iter_next(&iter));

	return v;
}

std::string om::ipc::dbus::Message::description() const
{
	std::stringstream ss;

	std::string iface, path, sender, destination, member, error_name;

	if(dbus_message_get_type(_message) == DBUS_MESSAGE_TYPE_METHOD_CALL)
		ss << "MC   ";
	else if(dbus_message_get_type(_message) == DBUS_MESSAGE_TYPE_METHOD_RETURN)
		ss << "MR   ";
	else if(dbus_message_get_type(_message) == DBUS_MESSAGE_TYPE_SIGNAL)
		ss << "SIG  ";
	else if(dbus_message_get_type(_message) == DBUS_MESSAGE_TYPE_ERROR)
		ss << "ERR  ";

	if(dbus_message_get_sender(_message))
		sender = dbus_message_get_sender(_message);
	else
		sender = "-";

	if(dbus_message_get_destination(_message))
		destination = dbus_message_get_destination(_message);
	else
		destination = "-";

	ss << sender << " -> " << destination << std::endl;

	if(dbus_message_get_type(_message) != DBUS_MESSAGE_TYPE_ERROR) {

		if(dbus_message_get_interface(_message))
			iface = dbus_message_get_interface(_message);
		else
			iface = "-";

		if(dbus_message_get_path(_message))
			path = dbus_message_get_path(_message);
		else
			path = "-";

		if(dbus_message_get_member(_message))
			member = dbus_message_get_member(_message);
		else
			member = "-";

		ss << "     " << path << " " << iface << "." << member;
	
	} else {	

		if(dbus_message_get_error_name(_message))
			error_name = dbus_message_get_error_name(_message);
		else
			error_name = "-";

		ss << "     " << error_name;
	}

	for(int t : arguments())
		ss << std::endl << "     " << arg_type_description(t);

	ss << std::endl;

	return ss.str();
}

std::string om::ipc::dbus::Message::arg_type_description(int arg_type)
{
	switch(arg_type) {
		case DBUS_TYPE_INT32:    return "DBUS_TYPE_INT32";
		case DBUS_TYPE_STRING:   return "DBUS_TYPE_STRING";
		default:                 return "DBUS_TYPE_UNKNOWN";
	}
}

om::ipc::dbus::Message::~Message()
{
	if(_message != 0 && _creation_mechanism == mechanism_created)
		dbus_message_unref(_message);
}

void om::ipc::dbus::Message::_init_iter(DBusMessageIter* iter) const
	throw(std::logic_error)
{
	if(!dbus_message_iter_init(_message, iter))
		throw std::logic_error("Message::_init_iter(): message has no arguments");
}

void om::ipc::dbus::Message::_advance_iter(DBusMessageIter* iter, size_t k) const
	throw(std::out_of_range)
{
	for(size_t i = 0; i < k; i++) {
		if(!dbus_message_iter_next(iter))
			throw std::out_of_range("Message::_advance_iter(): out of range");
	}
}
