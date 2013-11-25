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

#include <unistd.h>
#include <errno.h>
#include <stdlib.h>

pid_t dispatch(const char *command)
{
	static const char *shell = "/bin/sh";
	pid_t child = fork();
	if (child == 0) {
		char *const environ[] = { NULL };
		char *const args[] = { (char *)shell, "-c", (char *)command, NULL };
		if (execve(shell, args, environ) == -1) 
			exit(errno);
	}
	return child;
}

#ifdef __cplusplus
}
#endif
