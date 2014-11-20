//
//  Olli's C++ Library [https://bitbucket.org/omichel/om-lib]
//  tools/dir.cc
//  (c) 2014 Oliver Michel <oliver dot michel at editum dot de>
//  http://ngn.cs.colorado/~oliver
//

#include "dir.h"

#include <algorithm>
#include <dirent.h>
#include <error.h>
#include <string.h>
#include <unistd.h>

std::vector<om::tools::dir::entry> om::tools::dir::contents(std::string path) 
	throw(std::runtime_error)
{
	struct dirent* dirent;
	DIR* dir;
	std::vector<om::tools::dir::entry> v;

	if((dir = opendir(path.c_str())) != NULL) {

		while((dirent = readdir(dir)) != NULL)
      	v.push_back({std::string(dirent->d_name), dirent->d_type});

	} else {
		throw std::runtime_error("om::tools::dir::contents(): failed opening directory");
	}

	return v;
}


bool om::tools::dir::contains(std::string path, std::string entry)
	throw(std::runtime_error)
{
	for(om::tools::dir::entry e : om::tools::dir::contents(path))
		if(e.name == entry)
			return true;

	return false;
}

void om::tools::dir::mkdir(std::string path, mode_t mode)
	throw(std::runtime_error)
{
	if(::mkdir(path.c_str(), mode) < 0)
		throw std::runtime_error("om::tools::dir::mkdir(): mkdir failed: " +
			std::string(strerror(errno)));
}


void om::tools::dir::rmdir(std::string path)
	throw(std::runtime_error)
{
	if(::rmdir(path.c_str()) < 0)
		throw std::runtime_error("om::tools::dir::rmdir(): rmdir failed: " +
			std::string(strerror(errno)));		
}
