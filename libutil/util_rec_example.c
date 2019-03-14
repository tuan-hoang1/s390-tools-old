/**
 * util_rec_example - Example program for util_rec
 *
 * Copyright IBM Corp. 2016
 */

//! [code]
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "util_rec.h"

/*
 * Print three records in specified format
 */
static void print_records(const char *format, struct util_rec *rec)
{
	static const char * const size_vec[] = {"small", "medium", "large"};
	static const char * const name_vec[] = {"zero", "one", "two"};
	int i;

	printf("###########################################################\n");
	printf("# %s\n\n", format);

	/* Define fields of record */
	util_rec_def(rec, "number", UTIL_REC_ALIGN_LEFT, 6, "Number");
	util_rec_def(rec, "name", UTIL_REC_ALIGN_LEFT, 10, "Name");
	util_rec_def(rec, "size", UTIL_REC_ALIGN_RIGHT, 15, "Size");

	/* Print record header (is a nop for long format) */
	util_rec_print_hdr(rec);

	for (i = 0; i < 3; i++) {
		/* Fill fields of record with values */
		util_rec_set(rec, "number", "%d", i);
		util_rec_set(rec, "name", name_vec[i]);
		util_rec_set(rec, "size", size_vec[i]);
		/* Print the record */
		util_rec_print(rec);
	}
	printf("\n");
}

/*
 * Print records in "wide", "long", and "csv" format
 */
int main(void)
{
	struct util_rec *rec;

	rec = util_rec_new_wide("-");
	print_records("Wide format", rec);
	util_rec_free(rec);

	rec = util_rec_new_long("-", ":", "number", 30, 20);
	print_records("Long format", rec);
	util_rec_free(rec);

	rec = util_rec_new_csv(",");
	print_records("CSV format", rec);
	util_rec_free(rec);

	return EXIT_SUCCESS;
}
//! [code]
