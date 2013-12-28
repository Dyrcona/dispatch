# Copyright © 2013 Jason J.A. Stephenson <jason@sigio.com>

# This file is part of dispatch.

# dispatch is free software: you can redistribute it and/or modify it
# under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.

# dispatch is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with dispatch.  If not, see <http://www.gnu.org/licenses/>.

# Build our command line utility.
bin_PROGRAMS = dispatch
dispatch_CPPFLAGS = -I$(top_srcdir)/include -DLOCALEDIR=\"$(localedir)\"
dispatch_LDFLAGS = -L$(top_srcdir)/lib
dispatch_SOURCES = main.cc options.c read_to_list.cc cerr_printf.cc
dispatch_LDADD = -ldispatch @LTLIBBOOST_LOCALE@ @LTLIBBOOST_IOSTREAMS@ \
                  @LTLIBBOOST_PROGRAM_OPTIONS@

