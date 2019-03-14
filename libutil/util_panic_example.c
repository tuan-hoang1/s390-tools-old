/**
 * util_panic_example - Example program for util_panic
 *
 * Copyright IBM Corp. 2016
 */

//! [code]
#include <sys/resource.h>
#include <sys/time.h>

#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "util_panic.h"

/* Make functions noinline to have a nice backtrace */
#define __noinline __attribute__((noinline))

/*
 * Test util_panic()
 */
__noinline void panic_test_func(void)
{
	fprintf(stderr, "Testing util_panic() now\n");

	util_panic("Adieu beautiful world ...\n");

	fprintf(stderr, "You should not see this\n");
}

/*
 * Test util_panic() with errno handling
 */
__noinline void panic_errno_test_func(void)
{
	const char *file = "i_do_not_exist";

	if (fopen("i_do_not_exist", "r") == NULL) {
		util_panic("Open file \"%s\" failed: %s\n",
			   file, strerror(errno));
	}
	fprintf(stderr, "You should not see this\n");
}

/*
 * Test util_assert()
 */
__noinline void assert_test_func(void)
{
	char *drink_actual   = "beer";

	fprintf(stderr, "Testing util_assert() now\n");

	util_assert(strcmp(drink_actual, "water") == 0,
		    "We expected \"%s\" but got \"%s\"\n",
		    "water", drink_actual);

	fprintf(stderr, "You should not see this\n");
}

/*
 * Demonstrate util_panic() and util_assert()
 */
int main(int argc, char *argv[])
{
	struct rlimit rlim_unlimited = {-1, -1};
	struct rlimit rlim_zero = {0, 0};

	if (argc != 2)
		goto fail;

	if (strcmp(argv[1], "util_panic") == 0) {
		fprintf(stderr, "Disable core files: ulimit -c 0\n");
		setrlimit(RLIMIT_CORE, &rlim_zero);
		/* Do the panic */
		panic_test_func();
	} else if (strcmp(argv[1], "util_panic_errno") == 0) {
		setrlimit(RLIMIT_CORE, &rlim_unlimited);
		/* Do the panic */
		panic_errno_test_func();
	} else if (strcmp(argv[1], "util_assert") == 0) {
		fprintf(stderr, "Enable core files: ulimit -c unlimited\n");
		setrlimit(RLIMIT_CORE, &rlim_unlimited);
		/* Do the assertion */
		assert_test_func();
	}
fail:
	fprintf(stderr, "Usage: %s util_panic|util_panic_errno|util_assert\n", argv[0]);
	return EXIT_FAILURE;
}
//! [code]
