#ifndef _ETH_H_
#define _ETH_H_

#include <stdint.h>
#include <string>
#include <fstream>

class Eth
{
 public:
	Eth(const std::string& a) { _init(a); }
	Eth& operator=(const std::string& a) { _init(a); return *this; }
	operator std::string() const;

	Eth(uint64_t a) { _init(a); }
	Eth& operator=(uint64_t a) { _init(a); return *this; }
	operator uint64_t() const;

	Eth(const uint8_t a[6]) { _init(a); }
	Eth& operator=(const uint8_t a[6]) { _init(a); return *this; }

	friend std::ostream& operator<<(std::ostream&, const Eth&);

 private:
	void _init(const std::string&);
	void _init(uint64_t);
	void _init(const uint8_t[6]);

	uint8_t _a, _b, _c, _d, _e, _f;
};

class DirentEth
{
 public:
	DirentEth(std::string p) : _path(p) {};
	~DirentEth();
	DirentEth& operator=(const Eth& val);
	operator Eth();
 private:
	std::string  _path;
	std::fstream _fstream;
};

#endif/*_ETH_H_*/
