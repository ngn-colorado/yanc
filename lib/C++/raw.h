#ifndef _RAW_H_
#define _RAW_H_

#include <stdint.h>
#include <string>
#include <fstream>

class DirentRaw
{
 public:
	DirentRaw(std::string p) : _path(p) {};
	~DirentRaw();
	void    set(const char* buf, size_t length);
	ssize_t get(char** buf); 
	ssize_t get(char* buf, ssize_t bufsiz);
 private:
	std::string  _path;
	std::fstream _fstream;
};

#endif/*_RAW_H_*/
