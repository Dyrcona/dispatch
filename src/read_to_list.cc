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
#include <cstring>

// From options.c:
extern long opt_line_max;

// Function to read a line from an istream and push it onto a list.
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
