//
//  Olli's C++ Library
//  tools/logger.cc
//  (c) 2012 Oliver Michel
//  University of Vienna, University of Illinois at Urbana-Champaign
//

#include <iostream>
#include <stdexcept>
#include "logger.h"

om::tools::Logger* om::tools::Logger::_instance = 0;

om::tools::Logger::Logger() {}

om::tools::Logger::Logger(std::string filename) 
	: _filename(filename), _fstream() {

	_fstream.open(_filename.c_str(), std::ios::app);
}

om::tools::Logger* om::tools::Logger::initialize(std::string filename) {

	if(_instance != 0)
		throw std::logic_error("om::tools::Logger already initialized");
	else
		_instance = new Logger(filename);

	return _instance;
}

om::tools::Logger* om::tools::Logger::instance() {

	if(_instance == 0)
		throw std::logic_error("om::tools::Logger not yet initialized");

	return _instance;
}
