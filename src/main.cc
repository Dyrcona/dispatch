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
#include <algorithm>
#include <fstream>
#include <iostream>
#include <list>
#include <cstring>
#include <cstdarg>
#include "dispatch.h"

// A helper function to output a formatted string to std::cerr.  This
// function prepends the prog_name global to the output.  It also
// helps with future i18n work, since it is much easier to apply i18n
// to a printf-style format string than to a series of strings that
// use the << operator.  After all, the argument order may need to
// change in some languages.
void cerr_printf(const char *fmt, ...) {
	// TODO: Add some error checking.
	static char *outBuf = new char[opt_line_max];
	va_list ap;
	va_start(ap, fmt);
	vsnprintf(outBuf, opt_line_max, fmt, ap);
	va_end(ap);
	std::cerr << prog_name << ": " << outBuf;
}

// Another helper function to read a line from an istream and push it
// onto a list.
std::streamsize read_to_list(std::istream& in, std::list<char *>& l) {
	static char *buf = new char[opt_line_max];
	in.getline(buf, opt_line_max);
	if (in.good()) {
		std::streamsize in_size = in.gcount();
		if (in_size > 0) {
			char *c = new char[in_size];
			std::strncpy(c, buf, in_size);
			l.push_back(c);
		}
		return in_size;
	}
	return 0;
} 

int main(int argc, char **argv) {
	// Get command line options, if any.
	int argi = options(argc, argv);

	// Create a list to hold the commands and read the commands from the
	// files listed on the command line.
	std::list<char *> commands;
	while (argi < argc) {
		std::ifstream ifs(argv[argi], std::ifstream::in);
		while (ifs.good()) {
			read_to_list(ifs, commands);
		}
		ifs.close();
		argi++;
	}

	// If we don't have files, then we must have commands on stdin.
	if (commands.empty()) {
		do {
			read_to_list(std::cin, commands);			
		} while (std::cin.good());
	}

	// Create a list to track the running processes.
	std::list<pid_t> running;
	// How many commands we have:
	int goal = commands.size();
	// How many have been run;
	int processed = 0;

	while (processed < goal) {
		if (!commands.empty() && running.size() < opt_num) {
			char *command = commands.front();
			if (opt_verbose)
				cerr_printf("Dispatching %s\n", command);
			pid_t child = dispatch(command);
			if (child != -1)
				running.push_back(child);
			else
				cerr_printf("Failed to dispatch %s\n", command);
			commands.pop_front();
			delete[] command;
		} else {
			int status;
			pid_t child = wait(&status);
			std::list<pid_t>::iterator it =
				find(running.begin(), running.end(), child);
			if (it != running.end()) {
				running.remove(child);
				processed++;
				if (WIFEXITED(status) && (opt_verbose || WEXITSTATUS(status))) {
					cerr_printf("Child %d exited with status %d\n", child,
						WEXITSTATUS(status));
				} else if (WIFSIGNALED(status)) {
					cerr_printf("Child  %d terminated by signal %d\n", child,
						WTERMSIG(status));
				}
				if (opt_verbose) {
					cerr_printf("Processed %d of %d\n", processed, goal);
				}
			}
		}
	}

	// Just to be pedantic.
	return 0;
}
