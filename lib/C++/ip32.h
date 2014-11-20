#ifndef _IP32_H_
#define _IP32_H_

#include <stdint.h>
#include <string>
#include <fstream>

class IP32
{
 public:
	IP32(const std::string& a) { _init(a); }
	IP32& operator=(const std::string& a) { _init(a); return *this; }
	operator std::string() const;

	friend std::ostream& operator<<(std::ostream&, const IP32&);

	uint32_t address() const;
	uint8_t  mask() const;

 private:
	void _init(const std::string&);
	void _init(uint64_t);

	uint8_t _a, _b, _c, _d, _m;
};

class DirentIP32
{
 public:
	DirentIP32(std::string p) : _path(p) {};
	~DirentIP32();
	DirentIP32& operator=(const IP32& val);
	operator IP32();
 private:
	std::string  _path;
	std::fstream _fstream;
};

#endif/*_ETH_H_*/
