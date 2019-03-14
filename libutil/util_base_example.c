/**
 * util_base_example - Example program for util_base
 *
 * Copyright IBM Corp. 2017
 */

//! [code]
#include <stdio.h>

#include "lib/util_base.h"

#define INDENTATION	24

const char *msg =
"There is a theory which states that if ever anybody discovers exactly "
"what the Universe is for and why it is here, it will instantly disappear "
"and be replaced by something even more ...\n"
"\n"
"            ... bizarre\n"
"            ... and inexplicable.\n"
"\n"
"There is another theory which states that this has already happened.";

/*
 * Demonstrate util_base functions
 */
int main(void)
{
	printf("TEST: util_print_indented(msg, %d)\n", INDENTATION);
	printf("----------------------------------\n");
	printf("%-*s", INDENTATION, "Douglas Adams:");
	util_print_indented(msg, INDENTATION);
	return EXIT_SUCCESS;
}
//! [code]
