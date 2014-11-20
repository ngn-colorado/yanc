//
//  Olli's C++ Library [https://bitbucket.org/omichel/om-lib]
//  ipc/dbus/dbus.cc
//  (c) 2014 Oliver Michel <oliver dot michel at editum dot de>
//  http://ngn.cs.colorado/~oliver
//

#include "dbus.h"

#include <sstream>

void om::ipc::dbus::init_msg_read_iter(DBusMessageIter* iter, DBusMessage* msg)
	throw(std::logic_error)
{
	if(!dbus_message_iter_init(msg, iter))
		throw std::logic_error("init_msg_iter: message has no arguments");
}


std::string om::ipc::dbus::read_string(DBusMessageIter* iter, DBusMessage* msg)
	throw(std::logic_error)
{
	char* param;

	if(dbus_message_iter_get_arg_type(iter) != DBUS_TYPE_STRING)
		throw std::logic_error("read_string: argumet is not a string");
	else
		dbus_message_iter_get_basic(iter, &param);

	return std::string(param);
}

std::string om::ipc::dbus::msg_summary(DBusMessage* msg)
{
	std::stringstream ss;

	std::string iface, path, sender, destination, member, error_name;

	if(dbus_message_get_type(msg) == DBUS_MESSAGE_TYPE_METHOD_CALL)
		ss << "MC   ";
	else if(dbus_message_get_type(msg) == DBUS_MESSAGE_TYPE_METHOD_RETURN)
		ss << "MR   ";
	else if(dbus_message_get_type(msg) == DBUS_MESSAGE_TYPE_SIGNAL)
		ss << "SIG  ";
	else if(dbus_message_get_type(msg) == DBUS_MESSAGE_TYPE_ERROR)
		ss << "ERR  ";

	if(dbus_message_get_sender(msg))
		sender = dbus_message_get_sender(msg);
	else
		sender = "-";

	if(dbus_message_get_destination(msg))
		destination = dbus_message_get_destination(msg);
	else
		destination = "-";

	ss << sender << " -> " << destination << std::endl;

	if(dbus_message_get_type(msg) != DBUS_MESSAGE_TYPE_ERROR) {

		if(dbus_message_get_interface(msg))
			iface = dbus_message_get_interface(msg);
		else
			iface = "-";

		if(dbus_message_get_path(msg))
			path = dbus_message_get_path(msg);
		else
			path = "-";

		if(dbus_message_get_member(msg))
			member = dbus_message_get_member(msg);
		else
			member = "-";

		ss << "     " << path << " " << iface << "." << member;
	
	} else {	

		if(dbus_message_get_error_name(msg))
			error_name = dbus_message_get_error_name(msg);
		else
			error_name = "-";

		ss << "     " << error_name;
	}

	return ss.str();
}

om::ipc::dbus::message_signature::message_signature()
	: iface(), method() {}

om::ipc::dbus::message_signature::message_signature(
	std::string i, std::string m)
	: iface(i), method(m) {}

om::ipc::dbus::message_signature::message_signature(DBusMessage* msg)
	: 	iface(dbus_message_get_interface(msg)),
		method(dbus_message_get_member(msg)) {}

std::string om::ipc::dbus::message_signature::match_string(std::string type)
{
	std::stringstream ss;

	ss << "type='" << type << "',"
		<< "interface='" << iface << "',"
		<< "member='" << method << "'";

	return ss.str();
}

bool om::ipc::dbus::message_signature::operator==(
	const message_signature& other) const
{
	return iface == other.iface && method == other.method;
}

bool om::ipc::dbus::message_signature::operator<(
	const message_signature& other) const
{
	return iface < other.iface || method < other.method;
}