/**
 * @defgroup util_rec_h util_rec: Record interface
 * @{
 * @brief Print records in different output formats
 *
 * Copyright IBM Corp. 2016
 */

#ifndef LIB_UTIL_REC_H
#define LIB_UTIL_REC_H

/**
 * Opaque handle for a record
 *
 * The util_rec structure describes:
 *
 * - A set of fields
 * - An output format with the required formatting attributes (e.g. character
 *   for header separator)
 */
struct util_rec;

/**
 * Alignment in util_rec tables
 */
enum util_rec_align {
	/** Align field left */
	UTIL_REC_ALIGN_LEFT,
	/** Align field right */
	UTIL_REC_ALIGN_RIGHT,
};

struct util_rec *util_rec_new_wide(const char *hdr_sep);
struct util_rec *util_rec_new_csv(const char *col_sep);
struct util_rec *util_rec_new_long(const char *hdr_sep, const char *col_sep,
				   const char *key, int key_size, int val_size);
void util_rec_free(struct util_rec *rec);

void util_rec_def(struct util_rec *rec, const char *key,
		  enum util_rec_align align, int width, const char *hdr);
void util_rec_set(struct util_rec *rec, const char *key, const char *fmt, ...);
const char *util_rec_get(struct util_rec *rec, const char *key);

void util_rec_print_hdr(struct util_rec *rec);
void util_rec_print(struct util_rec *rec);

#endif /** LIB_UTIL_REC_H @} */
