/**
 * util_libc_example - Example program for util_libc
 *
 * Copyright IBM Corp. 2016
 */

//! [code]
#include <stdio.h>
#include <stdlib.h>

#include "lib/util_libc.h"
#include "lib/util_panic.h"

/*
 * Demonstrate that out of memory is automatically handled via panic()
 */
int main(void)
{
	unsigned long ulong_max = (unsigned long)-1;
	void *ptr;
	char *zeroes, *str;

	/* Use util_strcat_realloc() for string concatenation */
	fprintf(stderr, "Try to concatenate \"Hello\",  \", \" and \"world!\": ");
	str = util_strdup("Hello");
	str = util_strcat_realloc(str, ", ");
	str = util_strcat_realloc(str, "world!");
	fprintf(stderr, "result = \"%s\"\n", str);
	free(str);

	/* One byte allocation should work */
	fprintf(stderr, "Try to allocate 1 byte: ");
	ptr = util_malloc(1);
	fprintf(stderr, "done\n");

	/* One byte zeroed-allocation should work */
	fprintf(stderr, "Try to allocate 1 byte initialized with zeroes: ");
	zeroes = util_zalloc(1);
	fprintf(stderr, "done\n");
	util_assert(*zeroes == 0, "Garbage found in zero initialized memory\n");

	/* The next allocation will probably fail */
	fprintf(stderr, "Try to allocate %lu bytes:\n", ulong_max);
	ptr = util_malloc(ulong_max);

	fprintf(stderr, "You should not see me (ptr=%p)!\n", ptr);
	return EXIT_FAILURE;
}
//! [code]
