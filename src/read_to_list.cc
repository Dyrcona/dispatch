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
#include <istream>
#include <list>
#include <string>
#include <cstring>
#include <boost/iostreams/stream.hpp>
#include <boost/iostreams/concepts.hpp>

namespace io = boost::iostreams;

// A class to implement a boost stream sink for our list of commands.
class list_sink : public io::sink {
public:
  list_sink(std::list<char *>& l) : list(l) {}
  std::streamsize write(const char_type *s, std::streamsize n) {
    char *c = new char[n+1];
    std::strncpy(c, s, n);
    c[n] = 0;
    list.insert(list.end(), c);
    return n + 1;
  }
private:
  std::list<char *>& list;
};

// Function to read a line from an istream and push it onto a list.
std::size_t read_to_list(std::istream& in, std::list<char *>& l) {
  io::stream<list_sink> out(l);
  std::string str;
  std::getline(in, str);
  if (in.good()) {
    out << str;
    out.flush();
    return str.size();
  }
  return 0;
}

