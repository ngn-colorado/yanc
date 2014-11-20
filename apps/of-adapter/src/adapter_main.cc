//
// yanc: Yet Another Network Controller
// OpenFlow Adapter
//
// (c) 2013  Matthew Monaco  <matthew.monaco@colorado.edu>
//           Oliver Michel   <oliver.michel@colorado.edu>
//
// License: TBD
//

#include <cstdlib>
#include <iostream>
#include <cstring>
#include <csignal>
#include <unistd.h>

#include <om/net/net.h>
#include <om/tools/logger.h>
#include <om/tools/tools.h>

#include "yanc_of_adapter.h"

namespace yanc {
  namespace of_adapter {

    YancOFAdapter* main_instance;
  
    struct of_adapter_options_t {
      bool help;
      int port;
      int verbosity;
      std::string fs_path;
      std::string dbus_addr;
    } options;

    // forward declare usage/help functions
    void parse_options(int argc, char *argv[]);
    void print_help();
    void print_usage();
    void setup_signals();
    void signal_handler(int signum);
  };
};

int main(int argc, char *argv[]) {

  yanc::of_adapter::setup_signals();
  yanc::of_adapter::parse_options(argc, argv);

  yanc::of_adapter::main_instance = new YancOFAdapter(
    yanc::of_adapter::options.fs_path,
    yanc::of_adapter::options.dbus_addr,
    yanc::of_adapter::options.port
  );

  yanc::of_adapter::main_instance->run();

	return 0;
}

void yanc::of_adapter::parse_options(int argc, char *argv[]) {

  int opt = 0, opt_count = 0;
  opterr = 0;

  if(argc < 3)
    yanc::of_adapter::print_usage();

  // initialize option variable defaults
  yanc::of_adapter::options.verbosity = 0;
  yanc::of_adapter::options.help = false;
  yanc::of_adapter::options.port = 6633;

  while((opt = getopt(argc, argv, "hvp:")) != -1) {
    switch(opt) {
      case 'p':
        // TCP listening port no
        yanc::of_adapter::options.port = atoi(optarg);
        opt_count += 2;
        break;
      case 'h': 
        // display help
        yanc::of_adapter::options.help = true;
        opt_count++;
        break;
      case 'v': 
        // use verbose output
        yanc::of_adapter::options.verbosity++;
        opt_count++;
        break;
      default:
        yanc::of_adapter::print_usage();
    }
  }

  if(yanc::of_adapter::options.help)
    yanc::of_adapter::print_help();

  om::tools::options.log_level = yanc::of_adapter::options.verbosity;

  if(argc > optind+1 && argc < optind+3) {
    yanc::of_adapter::options.fs_path = std::string(argv[optind]);
    yanc::of_adapter::options.dbus_addr = std::string(argv[optind+1]); 
  } else 
    yanc::of_adapter::print_usage();
}

void yanc::of_adapter::print_help() {

  std::cout << "Usage: yanc-of-adapter [-v[v[v]]] [-h] [-p listening-port] "
   << "<yanc-fs-path> <dbus-addr>" << std::endl << std::endl;

  std::cout << "Connects the yanc filesystem to multiple OpenFlow switches"
    << std::endl << std::endl;

  std::cout << "(c) 2013 Oliver Michel <oliver.michel@colorado.edu>" << std::endl;
  std::cout << "         Matthew Monaco <matthew.monaco@colorado.edu>" << std::endl;
  std::cout << std::endl;
  std::cout << "         University of Colorado Boulder" << std::endl << std::endl;

  exit(EXIT_SUCCESS);
}

void yanc::of_adapter::print_usage() {

  std::cout << "Usage: yanc-of-adapter [-v[v[v]]] [-h] [-p listening-port] "
    << "<yanc-fs-path> <dbus-addr>" << std::endl;
  std::cout << "Try `yanc-of-adapter -h' for more information." << std::endl;

  exit(EXIT_FAILURE);
}

void yanc::of_adapter::setup_signals() {

  if(signal(SIGINT, signal_handler) == SIG_ERR)
    std::cerr << "YancOFAdapter: failed setting up SIGINT handler" << std::endl;

  if(signal(SIGTERM, signal_handler) == SIG_ERR)
    std::cerr << "YancOFAdapter: failed setting up SIGINT handler" << std::endl;
}

void yanc::of_adapter::signal_handler(int signum) {

  std::cerr << "YancOFAdapter [FATAL]: caught signal " << strsignal(signum)
    << std::endl;

  yanc::of_adapter::main_instance->shutdown();

  std::cerr << "YancOFAdapter [FATAL]: exit now" << std::endl;

  exit(EXIT_SUCCESS);
}
