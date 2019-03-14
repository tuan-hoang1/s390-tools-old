/*
 * Misc - Local helper functions
 *
 * Copyright IBM Corp. 2016
 */

#include <string.h>

#include "util_libc.h"

/**
 * Concatenate two strings or exit in case of failure
 *
 * @param[in] str1  String 1 to concatenate
 * @param[in] str2  String 2 to concatenate
 *
 * @returns   Pointer to newly allocated string
 */
char *misc_strcat_realloc(char **str1, const char *str2)
{
	char *buf;

	if (*str1) {
		buf = util_realloc(*str1, strlen(*str1) + strlen(str2) + 1);
		strcat(buf, str2);
	} else {
		buf = util_strdup(str2);
	}
	*str1 = buf;
	return buf;
}
