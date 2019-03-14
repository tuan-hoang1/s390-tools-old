/*
 * Misc - Local helper functions for libutil
 *
 * Copyright IBM Corp. 2016
 */

#include <stdlib.h>
#include <string.h>

#include "lib/util_libc.h"
#include "misc.h"

/*
 * Print a string while accounting for a given indent value, characters per line
 * limit, and line breaks ('\n') within the string.
 */
void misc_print_formatted(const char *desc_in, int indent)
{
	char *word, *line, *desc, *desc_ptr;
	int word_len, pos = indent;

	desc = desc_ptr = util_strdup(desc_in);
	line = strsep(&desc, "\n");
	while (line) {
		word = strsep(&line, " ");
		pos = indent;
		while (word) {
			word_len = strlen(word);
			if (pos + word_len + 1 > MAX_CHARS_PER_LINE) {
				printf("\n%*s", indent, "");
				pos = indent;
			}
			if (pos == 0)
				printf("%s", word);
			else
				printf(" %s", word);
			pos += word_len + 1;
			word = strsep(&line, " ");
		}
		if (desc)
			printf("\n%*s", indent, "");
		line =  strsep(&desc, "\n");
	}
	printf("\n");
	free(desc_ptr);
}
