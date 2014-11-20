#include <errno.h>
#include <string.h>
#include <iostream>
	using std::cout;
	using std::endl;
	using std::ostream;
#include <fstream>
	using std::fstream;
#include <string>
	using std::string;
#include <stdexcept>
	using std::runtime_error;
#include "raw.h"

DirentRaw::~DirentRaw()
{
	if (_fstream.is_open())
		_fstream.close();
}

void DirentRaw::set(const char* buf, size_t len)
{
	_fstream.open(_path.c_str(), std::ios::out|std::ios::trunc|std::ios::binary);
	_fstream.write(buf, len);
	_fstream.close();
}

ssize_t DirentRaw::get(char** buf)
{
	long len;

	_fstream.open(_path.c_str(), std::ios::in|std::ios::binary);
	_fstream.seekg(0, std::ios::end);
	len = _fstream.tellg();
	_fstream.seekg(0, std::ios::beg);

	if ((*buf = new char[len]) == NULL)
		return -ENOMEM;

	_fstream.read(*buf, len);
	_fstream.close();
	return len;
}

ssize_t DirentRaw::get(char* buf, ssize_t bufsiz)
{
	long len;

	_fstream.open(_path.c_str(), std::ios::in|std::ios::binary);
	_fstream.seekg(0, std::ios::end);
	len = _fstream.tellg();
	_fstream.seekg(0, std::ios::beg);

	if (bufsiz < len) {
		_fstream.close();
		return -ENOSPC;
	}

	_fstream.read(buf, len);
	_fstream.close();
	return len;
}

#ifdef RAW_TEST

int main(void)
{
	DirentRaw raw("/net/Raw");

	char* gbuf;
	ssize_t slen = raw.get(&gbuf);
	cout << slen << " " << gbuf << endl;

	const char* buf = "Hello, World!\0";
	size_t      len = strlen(buf) + 1;
	raw.set(buf, len);

	slen = raw.get(&gbuf);
	cout << slen << " " << gbuf << endl;

	return 0;
}
#endif

