/*
 * Copyright © 2013 Jason J.A. Stephenson <jason@sigio.com>
 *
 * This file is part of dispatch.
 *
 * dispatch is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * dispatch is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with dispatch.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <boost/program_options.hpp>
#include <boost/locale.hpp>
#include <vector>
#include <string>
#include <exception>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>

namespace po = boost::program_options;
using boost::locale::format;
using boost::locale::translate;

/* How many children to fork. */
long opt_num;

/* Whether or not we want progress messages. */
bool opt_verbose = false;

/* Maximum length of a line.  This is not really an option, so much as
 * it is a constraint imposed by the system.  We name it like an
 * option anyway. */
long opt_line_max;

/* Name that we were called with. */
char *prog_name;

/* Vector to hold the list of command files passed on the command line. */
std::vector<std::string> filelist;

void options(int argc, char **argv) {

	/*
	 * Set a default for the global opt_num variable using the
	 * _SC_NPROCESSORS_ONLN sysconf value.
	 */
	opt_num = sysconf(_SC_NPROCESSORS_ONLN);
	/* Make sure it is at least 2. */
	opt_num = opt_num > 1 ? opt_num : 2;

	/* Get line max via sysconf. */
	opt_line_max = sysconf(_SC_LINE_MAX);

	/* Get the name we were called with. */
	prog_name = argv[0];

	po::options_description opts("Allowed Options");
	opts.add_options()
		("help", "Print help message to standard error and exit.")
		("verbose,v", "Report execution progress of child processes.")
		("num,n", po::value<long>(&opt_num), "Number of processes to fork.")
		("file,f", po::value<std::vector<std::string> >(&filelist),
			"Read list of commands from a file.");

	po::positional_options_description popts;
	popts.add("file", -1);

	po::variables_map vm;
	try {
		po::store(po::command_line_parser(argc, argv).options(opts)
			.positional(popts).run(), vm);
		po::notify(vm);
	} catch (po::multiple_occurrences& mo) {
		std::cerr << mo.what() << std::endl;
		std::exit(EXIT_FAILURE);
	} catch (std::exception& e) {
		std::cerr << translate("An exception occurred while parsing the command line options:")
							<< std::endl << e.what() << std::endl;
		std::exit(EXIT_FAILURE);
	}

	/* Check for help paramater and if found, print the help to
	 * std::cerr and then exit. */
	if (vm.count("help")) {
		std::cerr << opts << std::endl;
		std::exit(EXIT_SUCCESS);
	}

	// Check for the verbose option.
	if (vm.count("verbose"))
		opt_verbose = true;

	/* Check for the maximum number of processes per user id.  We will
	 * throw a fit if opt_num is set to this value or higher. */
	long max_child = sysconf(_SC_CHILD_MAX);
	if (opt_num >= max_child) {
		std::cerr << format(translate("The number of requested processes ({1,number}) exceeds the "
				"maximum allowed by the system ({2,number}).")) % opt_num % max_child << std::endl;
		std::exit(EXIT_FAILURE);
	} else if (opt_num <= 0) {
				std::cerr << translate("The num argument must be a positive integer.") << std::endl;
		std::exit(EXIT_FAILURE);
	}
}
