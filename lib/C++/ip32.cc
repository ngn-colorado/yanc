#include <stdio.h>
#include <iomanip>
#include <iostream>
	using std::cout;
	using std::endl;
	using std::ostream;
#include <fstream>
	using std::fstream;
#include <string>
	using std::string;
#include <sstream>
	using std::stringstream;
#include <stdexcept>
	using std::runtime_error;
#include "ip32.h"

void IP32::_init(const string& addr)
{
	int n = sscanf(addr.c_str(), "%hhd.%hhd.%hhd.%hhd/%hhd",
		&_a, &_b, &_c, &_d, &_m);
	if (n != 5)
		_a = _b = _c = _d = _m = 0;
}

IP32::operator string() const
{
	stringstream str;
	str << std::dec
	     << (int) _a << "."
	     << (int) _b << "."
	     << (int) _c << "."
	     << (int) _d << "/"
	     << (int) _m;

	return str.str();
}

uint32_t IP32::address() const
{
	return (_a << 24) | (_b << 16) | (_c << 8) | _d;
}

uint8_t IP32::mask() const
{
	return _m;
}

ostream& operator<<(std::ostream& out, const IP32& eth)
{
	out << (string) eth;
	return out;
}


DirentIP32::~DirentIP32()
{
	if (_fstream.is_open())
		_fstream.close();
}

DirentIP32& DirentIP32::operator= (const IP32& val)
{
	_fstream.open(_path.c_str(), std::ios::out|std::ios::trunc);
	_fstream << (string) val;
	_fstream.close();
	return *this;
}

DirentIP32::operator IP32()
{
	std::string str;
	_fstream.open(_path.c_str(), std::ios::in);
	_fstream.seekg(0, std::ios::end);
	str.resize(_fstream.tellg());
	_fstream.seekg(0, std::ios::beg);
	_fstream.read(&str[0], str.size());
	_fstream.close();
	str.resize(_fstream.gcount());

	return IP32(str);
}

#ifdef IP32_TEST

int main(void)
{
	IP32 a("192.168.1.100/24");
	IP32 b = string("192.168.2.200/24");
	IP32 c = a;
	IP32 d = b;

	cout << a << endl;
	cout << b << endl;
	cout << c << endl;
	cout << d << endl;

	string   s = a;
	cout << s << endl;
	cout << IP32(s) << endl;

	DirentIP32 foo("foo");
	cout << (IP32) foo << endl;
	foo = IP32("10.0.1.100/8");
	cout << (IP32) foo << endl;

	return 0;
}
#endif
