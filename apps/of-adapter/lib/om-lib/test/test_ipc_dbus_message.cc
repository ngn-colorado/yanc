
#include "test.h"

#include <om/ipc/dbus/message.h>

int main()
{

	om::ipc::dbus::Message msg(DBUS_MESSAGE_TYPE_SIGNAL);
	

	test::set_name("ipc::dbus::Message");

	test::expect_false<bool>(msg.has_member(), 
		"has_interface() responds correcly on false");	
	
	msg.set_interface("de.editum.test");
	
	test::expect_equal<std::string>(msg.interface(), "de.editum.test",
		"sets and retrievesthe interface name");
	
	test::expect_true<bool>(msg.has_interface(), 
		"has_interface() responds correcly");

	test::expect_false<bool>(msg.has_member(), 
		"has_member() responds correcly on false");
	
	msg.set_member("Test");
	
	test::expect_equal<std::string>(msg.member(), "Test",
		"sets and retrieves the member name");
	
	test::expect_true<bool>(msg.has_member(), 
		"has_member() responds correcly on true");

	test::expect_false<bool>(msg.has_destination(), 
		"has_destination() responds correcly on false");
	
	msg.set_destination("de.editum.test");
	
	test::expect_equal<std::string>(msg.destination(), "de.editum.test",
		"sets and retrieves the destination name");
	
	test::expect_true<bool>(msg.has_destination(), 
		"has_destination() responds correcly on true");

	test::expect_false<bool>(msg.has_path(), 
		"has_path() responds correcly on false");
	
	msg.set_path("/de/editum/test");
	
	test::expect_equal<std::string>(msg.path(), "/de/editum/test",
		"sets and retrieves the path name");
	
	test::expect_true<bool>(msg.has_path(), 
		"has_path() responds correcly on true");

	test::expect_equal<size_t>(msg.num_args(), 0,
		"num_args() returns the number of arguments of a message");

	test::expect_nothrow([&msg](){
		msg.append_int32(21);
	}, "append_int32() does not throw an exception");

	test::expect_nothrow([&msg](){
		msg.append_string("is only half the truth");
	}, "append_string() does not throw an exception");

	test::expect_nothrow([&msg](){
		msg.get_int32(0);
	}, "get_<T>() does not throw an exception when arg pos is within bounds.");

	test::expect_throw([&msg](){
		msg.get_int32(2);
	}, "get_<T>() throws an exception when arg pos is out of bounds.");

	test::expect_throw([&msg](){
		msg.get_int32(1);
	}, "get_<T>() throws an when pos refers to a different data type");

	test::expect_equal<int32_t>(msg.get_int32(0), 21,
		"get_int() returns the right value from a dbus message");

	test::expect_equal<std::string>(msg.get_string(1), "is only half the truth",
		"get_string() returns the right value from a dbus message");

	test::expect_equal<size_t>(msg.num_args(), 2,
		"num_args() returns the number of arguments of a message");

	std::cout << "message summary: " << std::endl;
	std::cout << msg << std::endl;


	return 0;

}
