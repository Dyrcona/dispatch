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

// Function to read a line from an istream and push it onto a list.
std::size_t read_to_list(std::istream& in, std::list<char *>& l) {
	std::string str;
	std::getline(in, str);
	if (in.good()) {
		std::size_t size = str.size();
		if (size > 0) {
			char *c = new char[size+1];
			std::strncpy(c, str.c_str(), size);
			c[size] = 0;
			l.push_back(c);
		}
		return size;
	}
	return 0;
} 

