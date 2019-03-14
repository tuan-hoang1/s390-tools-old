/*
 * Copyright IBM Corp. 2016
 *
 * Author(s): Peter Oberparleiter <oberpar@linux.vnet.ibm.com>
 */

#ifndef OPTS_H
#define OPTS_H

#include "exit_code.h"

#define OPTS_MAX	255
#define OPTS_CONFLICT(x, ...)    { .op = (x), \
				  .conflicts = ((int []) { __VA_ARGS__, 0 })}

struct opts_conflict {
	int op;
	int *conflicts;
};

struct option;

exit_code_t opts_check_conflict(int, int[OPTS_MAX + 1], struct opts_conflict *,
				const struct option *);

#endif /* OPTS_H */
