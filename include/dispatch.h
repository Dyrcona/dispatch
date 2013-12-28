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

#ifndef DISPATCH_H_INCL
#define DISPATCH_H_INCL 1

#ifdef __cplusplus
extern "C" {
#endif

#include <unistd.h>

/* A function to "dispatch" command via /bin/sh -c. See man 3 dispatch. */
pid_t dispatch(const char *command);

#ifdef __cplusplus
}
#endif

#endif /* DISPATCH_H_INCL */
