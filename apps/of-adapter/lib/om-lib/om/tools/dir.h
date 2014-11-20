//
//  Olli's C++ Library [https://bitbucket.org/omichel/om-lib]
//  tools/dir.h
//  (c) 2014 Oliver Michel <oliver dot michel at editum dot de>
//  http://ngn.cs.colorado/~oliver
//

#ifndef OM_TOOLS_DIR_H
#define OM_TOOLS_DIR_H

#include <string>
#include <vector>
#include <stdexcept>
#include <stdint.h>
#include <sys/stat.h>
#include <sys/types.h>

namespace om {
	namespace tools {
		namespace dir {

			struct entry {
				std::string name;
				uint8_t type;
			};

			enum file_type {
				file_type_unknown =  0,
				file_type_fifo    =  1,
				file_type_chr     =  2,
				file_type_dir     =  4,
				file_type_blk     =  6,
				file_type_reg     =  8,
				file_type_lnk     = 10,
				file_type_sock    = 12,
				file_type_wht     = 14
			};

			std::vector<om::tools::dir::entry> contents(std::string path)
				throw(std::runtime_error);

			bool contains(std::string path, std::string entry)
				throw(std::runtime_error);

			void mkdir(std::string path, mode_t mode)
				throw(std::runtime_error);

			void rmdir(std::string path)
				throw(std::runtime_error);
		}
	}
}

#endif
