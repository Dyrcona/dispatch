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
#include <boost/iostreams/concepts.hpp>
#include <boost/iostreams/operations.hpp>
#include <cctype>

namespace io = boost::iostreams;

#ifndef COMMAND_INPUT_FILTER_INCL
#define COMMAND_INPUT_FILTER_INCL 1

/*
 * Our command line input filter class.  It is used to filter the
 * command line input whether read from files or passed in via
 * standard input.
 *
 * It filters the input in the following manner:
 *
 * All characters following the comment_char on a line are ignored.
 * The comment_char itself is also ignored.
 *
 * Blank lines and white space at the beginning of a line are ignored.
 *
 * Backslash (\) at the end of a line marks a line continuation, so
 * that the following line is treated as part of the current command
 * line.  Backslash anywhere else is treated like a normal character.
 *
 * Using the above rules, a stream will return a single command
 * pipeline for each call to getline().
 */

class command_input_filter : public io::input_filter {
public:
  explicit command_input_filter() : comment_char('#'), skip(false), hold(0),
                                    skipws(true) {}
  template<typename Source>
  int get(Source& src) {
    int c;
    while (true) {
      // If we have something in hold, we return it.
      if (hold) {
        c = hold;
        hold = 0;
        break;
      }
      // Get the next character from the stream.
      c = io::get(src);
      // If we have EOF or are interrupted, return.
      if (c == EOF || c == io::WOULD_BLOCK)
        break;
      // Check for skipping whitespace at the beginning of a line.
      if (std::isspace(c) && skipws)
        continue;
      else
        skipws = false;
      // Skip the rest of the line if we have a comment character.
      skip = c == comment_char ? true : c == '\n' ? false : skip;
      if (!skip) {
        // Check for backslash.
        if (c == '\\') {
          // Get the next character to see what follows.
          hold = io::get(src);
          // Bug out if we get EOF, etc.
          if (hold == EOF || hold == io::WOULD_BLOCK)
            break;
          else if (hold == '\n') {
            // Backslash before a newline means we join the next line.
            hold = 0;
            // Reset skipws whenever we read a newline.
            skipws = true;
            continue;
          }
        }
        // Reset skipws whenever we read a newline.
        if (!skipws && c == '\n')
          skipws = true;
        break;
      }
    }
    return c;
  }

  template<typename Source>
  void close(Source& src) {
    skip = false;
    skipws = true;
    hold = 0;
  }

private:
  char comment_char;
  bool skip;
  int hold;
  bool skipws;
};

#endif
