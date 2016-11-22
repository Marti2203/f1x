#include <iostream>
#include <boost/program_options.hpp>
#include <boost/log/trivial.hpp>
#include "F1XConfig.h"

namespace po = boost::program_options;

using namespace std;

int main (int argc, char *argv[])
{
  po::positional_options_description positional;
  positional.add("source", 1);
  
  // Declare supported options.
  po::options_description general("Usage: f1x PATH OPTIONS\n\nSupported options");
  general.add_options()
    ("files,f", po::value<string>()->multitoken()->value_name("RELPATH..."), "list of source files to repair")
    ("tests,t", po::value<string>()->multitoken()->value_name("ID..."), "list of test IDs")
    ("test-timeout,T", po::value<string>()->value_name("MS"), "test execution timeout (default: none)")
    ("driver,d", po::value<string>()->value_name("PATH"), "test driver")
    ("build,b", po::value<string>()->value_name("CMD"), "build command (default: make -e)")
    ("output,o", po::value<string>()->value_name("PATH"), "output directory (default: $PWD/f1x-DATE-TIME)")
    ("verbose,v", po::value<int>()->value_name("LEVEL")->implicit_value(1), "extended output")
    ("help,h", "produce help message and exit")
    ("version", "print version and exit")
    ("first-found", "terminate search on first found patch")
    ;

  po::options_description hidden("Hidden options");
  hidden.add_options()  
    ("source", po::value<string>(), "source directory")
    ;

  po::options_description all("All options");
  all.add(general).add(hidden);
  
  po::variables_map vm;
  po::store(po::command_line_parser(argc, argv).options(all).positional(positional).run(), vm);
  po::notify(vm);

  if (vm.count("help")) {
    cout << general << "\n";
    return 1;
  }

  if (vm.count("version")) {
    cout << "f1x " << F1X_VERSION_MAJOR <<
               "." << F1X_VERSION_MINOR <<
               "." << F1X_VERSION_PATCH << "\n";
    return 1;
  }

  if (!vm.count("source")) {
    BOOST_LOG_TRIVIAL(error) << "source directory is not specified (use --help)\n";
    return 1;
  }

  BOOST_LOG_TRIVIAL(info) << "test-timeout was set to " << vm["test-timeout"].as<int>();
  BOOST_LOG_TRIVIAL(info) << "source was set to " << vm["source"].as<string>();
  
  return 0;
}
