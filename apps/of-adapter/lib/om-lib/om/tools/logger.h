//
//  Olli's C++ Library
//  tools/logger.h
//  (c) 2012 Oliver Michel
//

#ifndef OM_TOOLS_LOGGER_H
#define OM_TOOLS_LOGGER_H

#include <string>
#include <fstream>

namespace om {
	namespace tools {

		class Logger {

		// implements singleton-pattern

		public:
			static Logger* initialize(std::string filename);
			static Logger* instance();
			std::ofstream& fstream() { return _fstream; }

		private:
			static Logger* _instance;
			std::string _filename;
			std::ofstream _fstream;
			Logger();
			Logger(std::string filename);
			Logger(const Logger&);
			Logger& operator=(const Logger&);
		};
	}
}

#endif
