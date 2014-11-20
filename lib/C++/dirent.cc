#include "dirent.h"
#include <stdint.h>

template <>
Dirent<std::string>& Dirent<std::string>::operator= (std::string val)
{
	_fstream.open(_path.c_str(), std::ios::out|std::ios::trunc);
	_fstream << val;
	_fstream.close();

	return *this;
}

template<>
Dirent<std::string>::operator std::string()
{
	std::string val;
	
	_fstream.open(_path.c_str(), std::ios::in|std::ios::binary);
	_fstream.seekg(0, std::ios::end);
	val.resize(_fstream.tellg());
	_fstream.seekg(0, std::ios::beg);
	_fstream.read(&val[0], val.size());
	_fstream.close();
	val.resize(_fstream.gcount());

	return val;
}

template<>
Dirent<uint8_t>& Dirent<uint8_t>::operator= (uint8_t val)
{
	std::stringstream str;
	str << (uint16_t) val;

	_fstream.open(_path.c_str(), std::ios::out|std::ios::trunc);
	_fstream << str.str();
	_fstream.close();

	return *this;
}

template<>
Dirent<int8_t>& Dirent<int8_t>::operator= (int8_t val)
{
	std::stringstream str;
	str << (int16_t) val;

	_fstream.open(_path.c_str(), std::ios::out|std::ios::trunc);
	_fstream << str.str();
	_fstream.close();

	return *this;
}

#ifdef DIRENT_TEST
int main(void)
{
	Dirent<uint8_t>      u8("/net/_u8");
	Dirent<uint16_t>    u16("/net/_u16");
	Dirent<uint32_t>    u32("/net/_u32");
	Dirent<uint64_t>    u64("/net/_u64");
	Dirent<int8_t>       i8("/net/_i8");
	Dirent<int16_t>     i16("/net/_i16");
	Dirent<int32_t>     i32("/net/_i32");
	Dirent<int64_t>     i64("/net/_i64");
	Dirent<std::string> str("/net/_str");
	Dirent<std::string> ip32("/net/_ip32");

	uint8_t     _u8;
	uint16_t    _u16;
	uint32_t    _u32;
	uint64_t    _u64;
	int8_t      _i8;
	int16_t     _i16;
	int32_t     _i32;
	int64_t     _i64;
	std::string _str;
	std::string _ip32;

	std::cout << "u8: " << u8 << std::endl;
	u8 = 80;
	std::cout << "u8: " << u8 << std::endl;
	_u8 = u8;
	std::cout << "u8: " << _u8 << std::endl;

	std::cout << "u16: " << u16 << std::endl;
	u16 = 1600;
	std::cout << "u16: " << u16 << std::endl;
	_u16 = u16;
	std::cout << "u16: " << _u16 << std::endl;

	std::cout << "u32: " << u32 << std::endl;
	u32 = 320000;
	std::cout << "u32: " << u32 << std::endl;
	_u32 = u32;
	std::cout << "u32: " << _u32 << std::endl;

	std::cout << "u64: " << u64 << std::endl;
	u64 = 640000000;
	std::cout << "u64: " << u64 << std::endl;
	_u64 = u64;
	std::cout << "u64: " << _u64 << std::endl;

	std::cout << "i8: " << i8 << std::endl;
	i8 = -80;
	std::cout << "i8: " << i8 << std::endl;
	_i8 = i8;
	std::cout << "i8: " << _i8 << std::endl;

	std::cout << "i16: " << i16 << std::endl;
	i16 = -1600;
	std::cout << "i16: " << i16 << std::endl;
	_i16 = i16;
	std::cout << "i16: " << _i16 << std::endl;

	std::cout << "i32: " << i32 << std::endl;
	i32 = -320000;
	std::cout << "i32: " << i32 << std::endl;
	_i32 = i32;
	std::cout << "i32: " << _i32 << std::endl;

	std::cout << "i64: " << i64 << std::endl;
	i64 = -640000000;
	std::cout << "i64: " << i64 << std::endl;
	_i64 = i64;
	std::cout << "i64: " << _i64 << std::endl;

	std::cout << "str: " << (std::string) str << std::endl;
	str = "Hello, World!";
	std::cout << "str: " << (std::string) str << std::endl;
	_str = str;
	std::cout << "str: " << _str << std::endl;

	std::cout << "ip32: " << (std::string) ip32 << std::endl;
	ip32 = "x192.168.1.1/24";
	std::cout << "ip32: " << (std::string) ip32 << std::endl;
	_ip32 = ip32;
	std::cout << "ip32: " << _ip32 << std::endl;
	return 0;
}
#endif

