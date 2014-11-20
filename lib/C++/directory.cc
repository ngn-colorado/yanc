#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>
#include <string>
	using std::string;
#include <stdexcept>
	using std::runtime_error;
#include "directory.h"

Directory::Directory(const string& path)
	: _path(path)
	, _managed(false)
{
	struct stat buf;
	int r;

	r = stat(path.c_str(), &buf);

	if (r == 0) {
		if (S_ISDIR(buf.st_mode))
			return;
		else
			throw runtime_error(
				"Directory ERROR " + path + " exists and is not a directory.");

	} else if (errno != ENOENT) {

		throw runtime_error(
			"Directory ERROR " + path + " unable to stat: " + strerror(errno));

	} else if (mkdir(path.c_str(), 0755) != 0) {

		throw runtime_error(
			"Directory ERROR " + path + " unable to mkdir: " + strerror(errno));
	} else {

		_managed = true;
	}
}

string Directory::path() const
{
	return _path;
}

std::string Directory::name() const
{
	return _path.substr(_path.find_last_of('/') + 1);
}

Directory::~Directory()
{
	if (_managed)
		rmdir(_path.c_str());
}

bool Directory::managed() const
{
	return _managed;
}

bool Directory::managed(bool m)
{
	bool tmp = _managed;
	_managed = m;
	return tmp;
}
