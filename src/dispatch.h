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

/*
 * This file is to be included in main.cc so it will have forward
 * declarations of functions and variables defined in other object
 * files.
 */

#ifdef __cplusplus
extern "C" {
#endif

#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

/* From dispatch.c, the dispatch function that actually runs the child
 * jobs. */
pid_t dispatch(const char *command);

/* From options.c, our option variables and command line option
 * parsing function. */
extern long opt_num;
extern bool opt_verbose;
extern long opt_line_max;
extern char *prog_name;
int options(int argc, char **argv);

#ifdef __cplusplus
}
#endif
