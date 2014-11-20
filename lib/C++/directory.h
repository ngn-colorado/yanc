#ifndef _DIRECTORY_H_
#define _DIRECTORY_H_

#include <string>

class Directory
{
 public:
	Directory(const std::string& path);
	~Directory();
	std::string path() const;
	std::string name() const;
	bool managed() const;
	bool managed(bool);
 private:
	std::string _path;
	bool        _managed;
};

#endif/*_DIRECTORY_H_*/
