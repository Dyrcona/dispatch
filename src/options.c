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
#ifdef __cplusplus
extern "C" {
#endif

#include <getopt.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

/*
 * The following code uses getopt_long() in a manner that is portable
 * to both GNU Lib C and the BSD Lib C implementations.  It makes no
 * use of any of the options or features that differ between these
 * implementations.  If you're building this software on a system
 * without getopt_long, then you may need to install GLibC.
 */

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

int options(int argc, char **argv) {

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

	struct option long_options[] = {
		{"num", required_argument, 0, 'n'},
		{"verbose", no_argument, 0, 'v'},
		{0, 0, 0, 0}
	};

	while (1) {
		int c = getopt_long(argc, argv, "n:v", long_options, 0);
		if (c == -1) {
			break;
		} else if (c == 'n') {
			char *endptr;
			opt_num = strtol(optarg, &endptr, 0);
			if (opt_num == 0 && endptr == optarg) {
				fprintf(stderr, "%s: Bad format for num argument: %s\n", prog_name,
					optarg);
				exit(EXIT_FAILURE);
			}
		} else if (c == 'v') {
			opt_verbose = true;
		} else {
			/* getopt_long reports an unrecognized option, so we dont' have to. */
			exit(EXIT_FAILURE);
		}
	}

	/* Check for the maximum number of processes per user id.  We will
	 * throw a fit if opt_num is set to this value or higher. */
	long max_child = sysconf(_SC_CHILD_MAX);
	if (opt_num >= max_child) {
		fprintf(stderr, "%s: Number of requested processes (%ld) exceeds the "
			"maximum allowed by the system (%ld).\n", prog_name, opt_num, max_child);
		exit(EXIT_FAILURE);
	} else if (opt_num <= 0) {
		fprintf(stderr, "%s: Num argument must be positive integer\n", prog_name);
		exit(EXIT_FAILURE);
	}

	/* Return the value of optind so that our caller can process any
	 * non-option command line arguments.  (These should be
	 * filenames.) */
	return optind ? optind : 1;
}

#ifdef __cplusplus
}
#endif
