/*
 * util - Utility function library
 *
 * General helper functions
 *
 * Copyright IBM Corp. 2013
 */

#include "util.h"

/*
 * Print hexdump for buffer with variable group parameter
 */
void util_hexdump_grp(FILE *fh, const char *tag, const void *data, int grp,
		      int count, int indent)
{
	const char *buf = data;
	int i, first = 1;

	for (i = 0; i < count; i++) {
		if (first) {
			fprintf(fh, "%*s", indent, " ");
			if (tag)
				fprintf(fh, "%s: ", tag);
			fprintf(fh, "%08x: ", i);
			first = 0;
		}
		fprintf(fh, "%02x", buf[i]);
		if (i % 16 == 15 || i + 1 == count) {
			fprintf(fh, "\n");
			first = 1;
		} else if (i % grp == grp - 1) {
			fprintf(fh, " ");
		}
	}
}

/*
 * Print hexdump for buffer with fix grp parameter
 */
void util_hexdump(FILE *fh, const char *tag, const void *data, int count)
{
	util_hexdump_grp(fh, tag, data, sizeof(long), count, 0);
}
