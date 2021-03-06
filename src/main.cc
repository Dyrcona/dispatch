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
#include <vector>
#include <string>
#include <locale>
#include <sys/types.h>
#include <sys/wait.h>
#include <boost/locale.hpp>
#include <boost/iostreams/device/file.hpp>
#include <boost/iostreams/filtering_stream.hpp>
#include "command_input_filter.hh"

#include <config.h>
#include <dispatch.h>

// Implemented in read_to_list.cc: A helper function to copy lines
// from a std::istream and add them to the end of a std::list<char *>.
extern std::size_t read_to_list(std::istream& in, std::list<char *>& l);

/* From options.cc, our option variables and command line option
 * parsing function. */
extern long opt_num;
extern bool opt_verbose;
extern char *prog_name;
extern std::vector<std::string> filelist;
extern void options(int argc, char **argv);

using boost::locale::format;
using boost::locale::translate;
namespace io = boost::iostreams;

int main(int argc, char **argv) {
  // Setup the locale for l10n.
  boost::locale::generator gen;
  gen.add_messages_path(LOCALEDIR);
  gen.add_messages_domain(PACKAGE);
  // Set the default locale.
  std::locale::global(gen(""));
  // Use it.
  std::cout.imbue(std::locale());
  std::cerr.imbue(std::locale());

  // Get command line options, if any.
  options(argc, argv);

  // Create a list to hold the commands and read the commands from the
  // files listed on the command line.
  std::list<char *> commands;
  std::vector<std::string>::iterator it;
  for (it = filelist.begin(); it != filelist.end(); it++) {
    io::filtering_istream fis;
    fis.push(command_input_filter());
    fis.push(io::file_source(it->c_str()));
    while (fis.good()) {
      read_to_list(fis, commands);
    }
  }

  // If we don't have files, then we must have commands on stdin.
  if (commands.empty()) {
    io::filtering_istream fis;
    fis.push(command_input_filter());
    fis.push(std::cin);
    do {
      read_to_list(fis, commands);
    } while (fis.good());
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
      pid_t child = dispatch(command);
      if (child != -1) {
        running.push_back(child);
        if (opt_verbose) {
          //TRANSLATORS: {1} is a command line. {2} is a process id number.
          std::cerr << format(translate("Started {1} with child process id {2}.")) % command % child << std::endl;
        }
      } else {
        //TRANSLATORS: {1} is a command line.
        std::cerr << format(translate("Failed to start {1}.")) % command << std::endl;
      }
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
          //TRANSLATORS: {1} is a process id number. {2} is a status integer.
          std::cerr << format(translate("Child process {1} exited with status {2}.")) % child
            % WEXITSTATUS(status) << std::endl;
        } else if (WIFSIGNALED(status)) {
          //TRANSLATORS: {1} is a process id number. {2} is a signal number.
          std::cerr << format(translate("Child process {1} terminated by signal {2}.")) % child
            % WTERMSIG(status) << std::endl;
        }
        if (opt_verbose) {
          std::cerr << format(translate("Processed {1,number} command out of {2,number}.",
              "Processed {1,number} commands out of {2,number}.", processed)) % processed % goal << std::endl;
        }
      }
    }
  }

  // Just to be pedantic.
  return 0;
}
