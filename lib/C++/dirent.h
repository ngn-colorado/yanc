#ifndef _DIRENT_H_
#define _DIRENT_H_

#include <string>
#include <fstream>
#include <iostream>
#include <sstream>

#include "eth.h"
#include "ip32.h"
#include "raw.h"

// There are currently two bugs with this class
// - int8_t doesn't work right because its ASCII value is used
// - std::string must be cast when using operator<<
// And the big TODO item
// - cache values and used with inotify

template <typename T>
class Dirent
{
 public:
	Dirent(std::string path) :
		_path(path)
	{
	}

	Dirent(const Dirent& dirent) :
		_path(dirent._path)
	{
	}

	~Dirent()
	{ 
		if (_fstream.is_open())
			_fstream.close();
	}

	Dirent& operator= (T val)
	{
		std::stringstream str;
		str << val;

		_fstream.open(_path.c_str(), std::ios::out|std::ios::trunc);
		_fstream << str.str();
		_fstream.close();

		return *this;
	}

	operator T()
	{
		T val;

		_fstream.open(_path.c_str(), std::ios::in);
		_fstream >> val;
		_fstream.close();

		return val;
	}

 private:
	std::string    _path;
	std::fstream   _fstream;
};

#endif/*_DIRENT_H_*/
