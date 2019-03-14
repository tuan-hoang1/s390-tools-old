/*
 * util - Utility function library
 *
 * Print standard program messages
 *
 * Copyright IBM Corp. 2016
 */

#include <errno.h>
#include <stdio.h>

#include "util_prg.h"
#include "zt_common.h"

/*
 * Private data
 */
static struct util_prg_l {
	const struct util_prg *prg;
} l;

struct util_prg_l *util_prg_l = &l;

/**
 * Print program usage information for the --help option
 */
void util_prg_print_help(void)
{
	printf("Usage: %s [OPTIONS]", program_invocation_short_name);
	if (l.prg->args)
		printf(" %s", l.prg->args);
	printf("\n\n%s\n\n", l.prg->desc);
}

/**
 * Print program version information for the --version option
 */
void util_prg_print_version(void)
{
	const struct util_prg_copyright *copyright;

	printf("%s version %s\n", program_invocation_short_name,
	       RELEASE_STRING);
	copyright = l.prg->copyright_vec;
	while (copyright->owner) {
		if (copyright->pub_first == copyright->pub_last)
			printf("Copyright %s %d\n", copyright->owner,
			       copyright->pub_first);
		else
			printf("Copyright %s %d, %d\n", copyright->owner,
			       copyright->pub_first, copyright->pub_last);
		copyright++;
	}
}

/*
 * Ask user to use the --help option
 */
void util_prg_print_parse_error(void)
{
	fprintf(stderr, "Try '%s --help' for more information.\n",
		program_invocation_short_name);
}

/**
 * An option has been specified that is not supported
 *
 * @param[in] option  Option string (short or long)
 */
void util_prg_print_invalid_option(const char *opt_name)
{
	fprintf(stderr, "%s: Invalid option '%s'\n",
		program_invocation_short_name, opt_name);
	util_prg_print_parse_error();
}

/**
 * A required argument for an option is missing
 *
 * @param[in] option  Option string
 */
void util_prg_print_required_arg(const char *opt_name)
{
	fprintf(stderr, "%s: Option '%s' requires an argument\n",
		program_invocation_short_name, opt_name);
	util_prg_print_parse_error();
}

/**
 * A superfluous invalid positional argument has been specified
 *
 * @param[in] arg_name  Name of the invalid argument
 */
void util_prg_print_arg_error(const char *arg_name)
{
	fprintf(stderr, "%s: Invalid argument '%s'\n",
		program_invocation_short_name, arg_name);
	util_prg_print_parse_error();
}

/**
 * Initialize the program module
 *
 * @param[in] prg Program description
 */
void util_prg_init(const struct util_prg *prg)
{
	l.prg = prg;
}
