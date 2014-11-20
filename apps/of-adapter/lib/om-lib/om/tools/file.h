//
//  Olli's C++ Library [https://bitbucket.org/omichel/om-lib]
//  tools/file.h
//  (c) 2013 Oliver Michel <oliver dot michel at editum dot de>
//  http://ngn.cs.colorado/~oliver
//


#ifndef OM_TOOLS_FILE_H
#define OM_TOOLS_FILE_H

#include <stdexcept>
#include <string>


namespace om {
	namespace tools {
		namespace file {

			template <class T>
			void write_uint_lit(std::string file_name, T data) 
				throw(std::runtime_error);

			template <class T>
			T read_uint_lit(std::string file_name) 
				throw(std::runtime_error);

			template <class T>
			void write_int_lit(std::string file_name, T data) 
				throw(std::runtime_error);

			template <class T>
			T read_int_lit(std::string file_name) throw(std::runtime_error);

			void write_data(std::string file_name, const char* buf, size_t len) 
				throw(std::runtime_error);

			void copy(std::string from, std::string to)
				throw(std::runtime_error);

			void copy_all_files(std::string from, std::string to)
				throw(std::runtime_error);

			void touch(std::string file)
				throw(std::runtime_error);
		}
	}
}

#endif
