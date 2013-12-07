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
#include <iostream>
#include <cstdarg>
#include <cstdio>

// From options.c:
extern long opt_line_max;
extern char *prog_name;

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
