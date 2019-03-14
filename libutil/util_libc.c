/*
 * util - Utility function library
 *
 * Handle standard errors for libc functions
 *
 * Copyright IBM Corp. 2016
 */

#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "util_base.h"
#include "util_panic.h"
#include "util_libc.h"

/*
 * Return size as string of largest unit, e.g. 1025 = "1 KiB"
 */
static void format_size(char *str, size_t size)
{
	static const char * const unit_vec[] =
		{"byte", "KiB", "MiB", "GiB", "TiB", "PiB", "EiB"};
	unsigned int i;

	for (i = 0; i < UTIL_ARRAY_SIZE(unit_vec); i++) {
		if (size / 1024 == 0) {
			sprintf(str, "%zu %s", size, unit_vec[i]);
			return;
		}
		size /= 1024;
	}
	sprintf(str, "huge");
}

static void __util_oom(const char *func, const char *file, int line,
		       size_t size)
{
	char size_str[256];

	fprintf(stderr, "%s: Failed to allocate memory",
		program_invocation_short_name);
	if (size > 0) {
		format_size(size_str, size);
		fprintf(stderr, " (%s)", size_str);
	}
	fprintf(stderr, " at %s:%d %s()\n", file, line, func);
	exit(EXIT_FAILURE);
}

/*
 * Allocate memory or exit in case of failure
 */
void *__util_malloc(const char *func, const char *file, int line, size_t size)
{
	void *buf;

	buf = malloc(size);

	if (buf == NULL)
		__util_oom(func, file, line, size);

	return buf;
}

/*
 * Allocate zero-initialized memory or exit in case of failure
 */
void *__util_zalloc(const char *func, const char *file, int line, size_t size)
{
	void *buf = __util_malloc(func, file, line, size);

	memset(buf, 0, size);

	return buf;
}

/*
 * Re-allocate memory or exit in case of failure
 */
void *__util_realloc(const char *func, const char *file, int line,
		     void *ptr, size_t size)
{
	void *buf;

	buf = realloc(ptr, size);

	if (buf == NULL)
		__util_oom(func, file, line, size);

	return buf;
}

/*
 * Duplicate a string buffer or exit in case of failure
 */
void *__util_strdup(const char *func, const char *file, int line,
		    const char *str)
{
	void *buf = strdup(str);

	if (buf == NULL)
		__util_oom(func, file, line, strlen(str) + 1);

	return buf;
}

/*
 * Print to newly allocated string or exit in case of failure
 */
int __util_vasprintf(const char *func, const char *file, int line,
		     char **strp, const char *fmt, va_list ap)
{
	int rc;

	rc = vasprintf(strp, fmt, ap);
	if (rc == -1)
		__util_oom(func, file, line, 0);

	return rc;
}

/*
 * Print to newly allocated string or exit in case of failure
 */
int __util_asprintf(const char *func, const char *file, int line,
		    char **strp, const char *fmt, ...)
{
	va_list ap;
	int rc;

	va_start(ap, fmt);
	rc = __util_vasprintf(func, file, line, strp, fmt, ap);
	va_end(ap);
	return rc;
}

/*
 * Print to string buffer or exit in case of failure
 */
int __util_vsprintf(const char *func, const char *file, int line,
		    char *str, const char *fmt, va_list ap)
{
	int rc;

	rc = vsprintf(str, fmt, ap);
	if (rc == -1)
		__util_assert("rc != -1", func, file, line,
			      rc != -1, "Could not format string\n");
	return rc;
}
