//
//  Olli's C++ Library [https://bitbucket.org/omichel/om-lib]
//  tools/file.cc
//  (c) 2013 Oliver Michel <oliver dot michel at editum dot de>
//  http://ngn.cs.colorado/~oliver
//

#include <stdio.h>
#include <stdint.h>
#include <fstream>

#include <om/tools/dir.h>

#include "file.h"

template<class T>
void om::tools::file::write_uint_lit(std::string file_name, T data)
	throw(std::runtime_error)
{

	std::ofstream file(file_name.c_str(), std::ios::out | std::ios::trunc);

	if(file.good()) {

		file << (sizeof(T) > 1 ? (T) data : (unsigned) data);
		file.close();

	} else throw std::runtime_error("write_uint_lit: failed opening file");
}

template<class T>
T om::tools::file::read_uint_lit(std::string file_name)
	throw(std::runtime_error)
{

	T value = 0;
	std::ifstream file(file_name.c_str(), std::ios::in);

	if(file.good()) {
		
		file >> value;

	} else throw std::runtime_error("read_uint_lit: failed opening file");

	file.close();

	return value;
}

template<class T>
void om::tools::file::write_int_lit(std::string file_name, T data)
	throw(std::runtime_error)
{

	std::ofstream file(file_name.c_str(), std::ios::out | std::ios::trunc);

	if(file.good()) {

		file << (sizeof(T) > 1 ? (T) data : data);
		file.close();

	} else throw std::runtime_error("write_int_lit: failed opening file");
}

template<class T>
T om::tools::file::read_int_lit(std::string file_name)
	throw(std::runtime_error)
{

	T value = 0;
	std::ifstream file(file_name.c_str(), std::ios::in);

	if(file.good()) {
		
		file >> value;

	} else throw std::runtime_error("read_int_lit: failed opening file");

	file.close();

	return value;
}

void om::tools::file::write_data(std::string file_name, const char* buf, size_t len)
	throw(std::runtime_error)
{
	std::ofstream file(file_name.c_str(), std::ios::out | std::ios::trunc | std::ios::binary);

	if(file.good()) {

		file.write(buf, len);
		file.close();

	} else throw std::runtime_error("write_data: failed openining file");
}

void om::tools::file::copy(std::string from, std::string to)
	throw(std::runtime_error)
{

	std::ifstream from_file(from);
	std::ofstream to_file(to);

	if(from_file.is_open() && to_file.is_open()) {

		while(from_file.good())
			to_file.put(from_file.get());

		from_file.close(), to_file.close();

	} else throw std::runtime_error("copy: failed openining file");
}

void om::tools::file::copy_all_files(std::string from, std::string to)
	throw(std::runtime_error)
{
	for(om::tools::dir::entry e : om::tools::dir::contents(from))
		if(e.type == om::tools::dir::file_type_reg)
			om::tools::file::copy(from + "/" + e.name, to + "/" + e.name);
}

void om::tools::file::touch(std::string file)
	throw(std::runtime_error)
{

	std::ofstream fs(file);

	if(!fs.is_open())
		throw std::runtime_error("touch: failed openining file");

	fs.close();
}

template void om::tools::file::write_uint_lit<uint8_t>(std::string file_name, 
	uint8_t data);

template uint8_t om::tools::file::read_uint_lit<uint8_t>(std::string file_name);

template void om::tools::file::write_uint_lit<uint16_t>(std::string file_name, 
	uint16_t data);

template uint16_t om::tools::file::read_uint_lit<uint16_t>(std::string file_name);

template void om::tools::file::write_uint_lit<uint32_t>(std::string file_name, 
	uint32_t data);

template uint32_t om::tools::file::read_uint_lit<uint32_t>(std::string file_name);

template void om::tools::file::write_uint_lit<uint64_t>(std::string file_name, 
	uint64_t data);

template uint64_t om::tools::file::read_uint_lit<uint64_t>(std::string file_name);



template void om::tools::file::write_int_lit<int8_t>(std::string file_name, 
	int8_t data);

template int8_t om::tools::file::read_int_lit<int8_t>(std::string file_name);

template void om::tools::file::write_int_lit<int16_t>(std::string file_name, 
	int16_t data);

template int16_t om::tools::file::read_int_lit<int16_t>(std::string file_name);

template void om::tools::file::write_int_lit<int32_t>(std::string file_name, 
	int32_t data);

template int32_t om::tools::file::read_int_lit<int32_t>(std::string file_name);

template void om::tools::file::write_int_lit<int64_t>(std::string file_name, 
	int64_t data);

template int64_t om::tools::file::read_int_lit<int64_t>(std::string file_name);
