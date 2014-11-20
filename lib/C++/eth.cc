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
#include "eth.h"

void Eth::_init(const string& addr)
{
	int n = sscanf(addr.c_str(), "%02hhx:%02hhx:%02hhx:%02hhx:%02hhx:%02hhx",
		&_a, &_b, &_c, &_d, &_e, &_f);
	if (n != 6)
		_a = _b = _c = _d = _e = _f = 0;
}

void Eth::_init(uint64_t addr)
{
	if (addr & 0xFFFF000000000000)
		throw runtime_error("Eth(uint16_t): upper bits not 0");

	_a = (addr & 0xFF0000000000) >> 40; 
	_b = (addr & 0x00FF00000000) >> 32;
	_c = (addr & 0x0000FF000000) >> 24;
	_d = (addr & 0x000000FF0000) >> 16;
	_e = (addr & 0x00000000FF00) >>  8;
	_f = (addr & 0x0000000000FF) >>  0;
}

void Eth::_init(const uint8_t addr[6])
{
	_a = addr[0];
	_b = addr[1];
	_c = addr[2];
	_d = addr[3];
	_e = addr[4];
	_f = addr[5];
}

Eth::operator string() const
{
	stringstream str;
	str << std::hex << std::setfill('0')
	    << std::setw(2) << (int) _a << ":"
	    << std::setw(2) << (int) _b << ":"
	    << std::setw(2) << (int) _c << ":"
	    << std::setw(2) << (int) _d << ":"
	    << std::setw(2) << (int) _e << ":"
	    << std::setw(2) << (int) _f;

	return str.str();
}

Eth::operator uint64_t() const
{
	uint64_t i = 0;
	i |= _a; i <<= 8;
	i |= _b; i <<= 8;
	i |= _c; i <<= 8;
	i |= _d; i <<= 8;
	i |= _e; i <<= 8;
	i |= _f;
	return i;
}

ostream& operator<<(std::ostream& out, const Eth& eth)
{
	out << (string) eth;
	return out;
}


DirentEth::~DirentEth()
{
	if (_fstream.is_open())
		_fstream.close();
}

DirentEth& DirentEth::operator= (const Eth& val)
{
	_fstream.open(_path.c_str(), std::ios::out|std::ios::trunc);
	_fstream << (string) val;
	_fstream.close();
	return *this;
}

DirentEth::operator Eth()
{
	std::string str;
	_fstream.open(_path.c_str(), std::ios::in);
	_fstream.seekg(0, std::ios::end);
	str.resize(_fstream.tellg());
	_fstream.seekg(0, std::ios::beg);
	_fstream.read(&str[0], str.size());
	_fstream.close();
	str.resize(_fstream.gcount());

	return Eth(str);
}

#ifdef ETH_TEST

int main(void)
{
	Eth a("01:02:03:aa:bb:cc");
	Eth b(0x010203aabbcc);
	Eth c = string("01:02:03:aa:bb:cc");
	Eth d = 0x010203aabbcc;
	Eth e = a;
	Eth f = c;

	cout << a << endl;
	cout << b << endl;
	cout << c << endl;
	cout << d << endl;
	cout << e << endl;
	cout << f << endl;

	string   s = a;
	uint64_t i = a;

	cout << s << endl;
	cout << (string) Eth(i) << endl;

	DirentEth foo("foo");
	cout << (Eth) foo << endl;
	foo = 0x112233445566;
	cout << (Eth) foo << endl;

	return 0;
}
#endif
