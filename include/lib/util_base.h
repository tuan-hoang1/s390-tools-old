/*
 * util - Utility function library
 *
 * General helper functions
 *
 * Copyright IBM Corp. 2013
 */

#ifndef LIB_UTIL_BASE_H
#define LIB_UTIL_BASE_H

#include <stdio.h>
#include <stdlib.h>

void util_hexdump(FILE *fh, const char *tag, const void *data, int cnt);
void util_hexdump_grp(FILE *fh, const char *tag, const void *data, int group,
		      int cnt, int indent);
void util_print_indented(const char *str, int indent);

#define UTIL_ARRAY_SIZE(array) (sizeof(array) / sizeof((array)[0]))

#define MIN(x, y)		\
({				\
	__typeof__(x) _x = (x);	\
	__typeof__(y) _y = (y);	\
				\
	_x < _y ? _x : _y;	\
})

#define MAX(x, y)		\
({				\
	__typeof__(x) _x = (x);	\
	__typeof__(y) _y = (y);	\
				\
	_x > _y ? _x : _y;	\
})

static inline void util_ptr_vec_free(void **ptr_vec, int count)
{
	int i;

	for (i = 0; i < count; i++)
		free(ptr_vec[i]);
	free(ptr_vec);
}

#endif /* LIB_UTIL_BASE_H */
