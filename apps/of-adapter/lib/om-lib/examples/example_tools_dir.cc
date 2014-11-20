//
//  Olli's C++ Library [https://bitbucket.org/omichel/om-lib]
//  examples/example_tools_dir.cc
//  (c) 2014 Oliver Michel <oliver dot michel at editum dot de>
//  http://ngn.cs.colorado/~oliver
//

#include <om/tools/dir.h>
#include <iostream>
#include <vector>
#include <string>

int main(int argc, char const *argv[]) {

	for(om::tools::dir::entry e : om::tools::dir::contents("/"))
		if(e.type == om::tools::dir::file_type_dir)
			std::cout << e.name << std::endl;
}