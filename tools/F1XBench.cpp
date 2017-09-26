/*
  This file is part of f1x.
  Copyright (C) 2016  Sergey Mechtaev, Gao Xiang, Shin Hwei Tan, Abhik Roychoudhury

  f1x is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <iostream>
#include <ctime>
#include <sstream>
#include <string>

#include <boost/filesystem.hpp>

#include <boost/program_options.hpp>

#include <boost/log/trivial.hpp>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>

#include "Config.h"
#include "Util.h"


namespace po = boost::program_options;
namespace fs = boost::filesystem;

using std::vector;
using std::string;


void initializeTrivialLogger(bool quiet) {
  boost::log::add_common_attributes();
  boost::log::register_simple_formatter_factory< boost::log::trivial::severity_level, char >("Severity");
  boost::log::add_console_log(std::cerr, boost::log::keywords::format = "[%TimeStamp%] [%Severity%]\t%Message%");
  if (quiet) {
    boost::log::core::get()->set_filter(boost::log::trivial::severity >= boost::log::trivial::info);
  } else {
    boost::log::core::get()->set_filter(boost::log::trivial::severity >= boost::log::trivial::debug);
  }
}


int main (int argc, char *argv[]) {
  po::positional_options_description positional;
  positional.add("defect", -1);
  
  // Declare supported options.
  po::options_description general("Usage: f1x-bench [DEFECT] OPTIONS\n\nSupported options");
  general.add_options()
    ("root", po::value<string>()->value_name("PATH"), "benchmark root (default: current directory)")
    ("output", po::value<string>()->value_name("PATH"), "output directory (default: results-TIME)")
    ("timeout", po::value<unsigned>()->value_name("SEC"), "timeout for individual defect")
    ("fetch", "perform fetch command and exit")
    ("set-up", "perform set-up command and exit")
    ("run", "perform run command and exit")
    ("tear-down", "perform tear-down command and exit")
    ("cmd", "print f1x command and exit")
    ("quiet", "print compact summary")
    ("help", "produce help message and exit")
    ("version", "print version and exit")
    ;

  po::options_description hidden("Hidden options");
  hidden.add_options()  
    ("defect", po::value<string>(), "defect identifier")
    ;

  po::options_description allOptions("All options");
  allOptions.add(general).add(hidden);

  po::variables_map vm;

  try {
    po::store(po::command_line_parser(argc, argv).options(allOptions).positional(positional).run(), vm);
    po::notify(vm);
  } catch(po::error& e) {
    BOOST_LOG_TRIVIAL(error) << e.what() << " (use --help)";
    return 0;
  }

  if (vm.count("help")) {
    std::cout << general << std::endl;
    return 1;
  }

  if (vm.count("version")) {
    std::cout << "f1x-bench " << F1X_VERSION_MAJOR <<
                    "." << F1X_VERSION_MINOR <<
                    "." << F1X_VERSION_PATCH << std::endl;
    return 1;
  }

  bool quiet = false;
  if (vm.count("quiet")) {
    quiet = true;
  }

  initializeTrivialLogger(quiet);

  fs::path root(fs::current_path());
  if (vm.count("root")) {
    fs::path specifiedRoot(vm["root"].as<string>());
    root = fs::absolute(specifiedRoot);
    if (! (fs::exists(root) && fs::is_directory(root))) {
      BOOST_LOG_TRIVIAL(error) << "benchmark directory " << root.string() << " does not exist";
      return 1;
    }
  }
  BOOST_LOG_TRIVIAL(debug) << "benchmark root: " << root.string();
  if (! fs::exists(root / "benchmark.json")) {
    BOOST_LOG_TRIVIAL(error) << "benchmark.json does not exist";
    return 1;
  }
  if (! fs::exists(root / "tests.json")) {
    BOOST_LOG_TRIVIAL(error) << "tests.json does not exist";
    return 1;
  }

  unsigned timeout = 0; // means no timeout
  if (vm.count("timeout")) {
    timeout = vm["timeout"].as<unsigned>();
  }

  fs::path output;
  if (!vm.count("output")) {
    std::time_t now = std::time(0);
    struct std::tm tstruct;
    char timeRepr[80];
    tstruct = *localtime(&now);
    strftime(timeRepr, sizeof(timeRepr), "-%Y_%m_%d-%H_%M_%S", &tstruct);
    std::stringstream name;
    name << "results" << timeRepr;
    output = fs::path(name.str());
  } else {
    output = fs::path(vm["output"].as<string>());
  }

  output = fs::absolute(output);
  if (fs::exists(output)) {
    BOOST_LOG_TRIVIAL(warning) << "existing "<< output << " will be overwritten";
    fs::remove_all(output);
  }
  fs::create_directory(output);

  BOOST_LOG_TRIVIAL(debug) << "output directory: " << output;
  
  return 0;
}