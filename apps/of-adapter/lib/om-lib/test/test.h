
#include <iostream>
#include <iomanip>
#include <functional>

namespace test {

	static std::string current_test_name;

	static const std::string pass = " \x1b[32m[PASS]\x1b[0m";
	static const std::string fail = " \x1b[31m[FAIL]\x1b[0m";
	static const size_t desc_width = 100;

	void set_name(std::string name)
	{
		current_test_name = name;
	}

	template<typename T>
	bool expect_equal(T a, T b, std::string desc)
	{
		std::cout << std::setw(desc_width) << std::left
			<< (current_test_name + ": " + desc + " (==)");

		if(a == b) {
			std::cout << pass << std::endl;
			return true;
		} else {
			std::cout << fail << std::endl;
			return false;
		}
	}

	template<typename T>
	bool expect_not_equal(T a, T b, std::string desc)
	{
		std::cout << std::setw(desc_width) << std::left
			<< (current_test_name + ": " + desc + " (!=)");

		if(a != b) {
			std::cout << pass << std::endl;
			return true;
		} else {
			std::cout << fail << std::endl;
			return false;
		}
	}

	template<typename T>
	bool expect_true(T a, std::string desc)
	{
		std::cout << std::setw(desc_width) << std::left
			<< (current_test_name + ": " + desc + " (T)");

		if(a) {
			std::cout << pass << std::endl;
			return true;
		} else {
			std::cout << fail << std::endl;
			return false;
		}
	}

	template<typename T>
	bool expect(T a, std::string desc)
	{
		return expect_true<T>(a, desc);
	}

	template<typename T>
	bool expect_false(T a, std::string desc)
	{
		std::cout << std::setw(desc_width) << std::left 
			<< (current_test_name + ": " + desc + " (F)");

		if(!a) {
			std::cout << pass << std::endl;
			return true;
		} else {
			std::cout << fail << std::endl;
			return false;
		}
	}

	bool expect_nothrow(std::function<void ()> code, std::string desc)
	{
		std::cout << std::setw(desc_width) << std::left 
			<< (current_test_name + ": " + desc + " (nothrow)");

		try {
			code();
			std::cout << pass << std::endl;
			return true;
		} catch (...) {
			std::cout << fail << std::endl;
			return false;
		}
	}


	bool expect_throw(std::function<void ()> code, std::string desc)
	{
		std::cout << std::setw(desc_width) << std::left 
			<< (current_test_name + ": " + desc + " (throw)");

		try {
			code();
			std::cout << fail << std::endl;
			return false;
		} catch (...) {
			std::cout << pass << std::endl;
			return true;
		}
	}

};
