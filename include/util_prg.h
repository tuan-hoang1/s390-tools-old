/**
 * @defgroup util_prg_h util_prg: Program interface
 * @{
 * @brief Print standard program messages
 *
 * Copyright IBM Corp. 2016
 */

#ifndef UTIL_PRG_H
#define UTIL_PRG_H

#include <err.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Copyright description
 */
struct util_prg_copyright {
	/** Name of the copyright owner, e.g. IBM */
	const char *owner;
	/** Year of first publishing */
	int pub_first;
	/** Year of last major changes */
	int pub_last;
};

/**
 * @brief Coypright end marker
 */
#define UTIL_PRG_COPYRIGHT_END {NULL, 0, 0}

/**
 * Program description
 */
struct util_prg {
	/** Description for help  */
	const char *desc;
	/** Positional arguments  */
	const char *args;
	/** Copyright list */
	struct util_prg_copyright copyright_vec[];
};

void util_prg_init(const struct util_prg *prg);
void util_prg_print_parse_error(void);
void util_prg_print_required_arg(const char *option);
void util_prg_print_invalid_option(const char *option);
void util_prg_print_arg_error(const char *arg_name);
void util_prg_print_version(void);
void util_prg_print_help(void);

#endif /** UTIL_PRG_H @} */
