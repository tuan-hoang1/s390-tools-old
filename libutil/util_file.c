/*
 * util - Utility function library
 *
 * Read and write files
 *
 * Copyright IBM Corp. 2016
 */

#include <linux/limits.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <errno.h>
#include <limits.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "util_base.h"
#include "util_file.h"
#include "util_panic.h"
#include "util_libc.h"
#include "util_prg.h"

/*
 * Read the first line of a file into given buffer
 */
static int file_gets(char *str, size_t size, const char *path)
{
	char *p, *end;
	FILE *fp;
	int rc;

	/* In case of error we always return empty string */
	str[0] = 0;
	/* Read the string */
	fp = fopen(path, "r");
	if (!fp)
		return -1;
	p = fgets(str, size, fp);
	if (p) {
		/* Check for end of line */
		end = memchr(str, '\n', size);
		if (end)
			*end = 0;
		else
			str[size - 1] = 0;
	}
	if (strlen(str) == 0) {
		rc = -1;
		goto out_fclose;
	}
	rc = 0;
out_fclose:
	fclose(fp);
	return rc;
}

/**
 * Read the first line of a file
 *
 * If read is successful 'str' contains first line of a file without the
 * trailing newline. If read fails, an empty string is returned for 'str'
 * The resulting string will always be null-terminated.
 *
 * @param[out] str    Result buffer
 * @param[in]  size   Size of the result buffer
 * @param[in]  fmt    Format string for generation of the path name
 * @param[in]  ...    Parameters for format string
 *
 * @retval     0      File was read
 * @retval    -1      Error while reading file
 */
int util_file_read_line(char *const str, size_t size, const char *fmt, ...)
{
	char path[PATH_MAX];
	va_list ap;

	/* Construct the file name */
	UTIL_VSPRINTF(path, fmt, ap);

	return file_gets(str, size, path);
}

/*
 * Write string to a file
 */
static int file_puts(const char *str, const char *path)
{
	FILE *fp;
	int rc;

	/* write the string */
	fp = fopen(path, "w");
	if (!fp)
		return -1;
	if (fputs(str, fp) == EOF) {
		rc = -1;
		goto out_fclose;
	}
	rc = 0;
out_fclose:
	fclose(fp);
	return rc;
}

/**
 * Write string to a file without the terminating null byte
 *
 * @param[in] str       Content is to be written
 * @param[in] fmt       Format string for generation of the path name
 * @param[in] ...       Parameters for format string
 *
 * @retval    0         Write was successful
 * @retval   -1         Error while writing file
 */
int util_file_write_s(const char *str, const char *fmt, ...)
{
	char path[PATH_MAX];
	va_list ap;

	/* Construct the file name */
	UTIL_VSPRINTF(path, fmt, ap);

	return file_puts(str, path);
}

/**
 * Write signed long value to a file according to given base
 *
 * @param[in] val       Value is to be written
 * @param[in] base      Base for conversion, either 8, 10, or 16
 * @param[in] fmt       Format string for generation of the path name
 * @param[in] ...       Parameters for format string
 *
 * @retval    0         Write was successful
 * @retval   -1         Error while writing file
 */
int util_file_write_l(long val, int base, const char *fmt, ...)
{
	char *str, path[PATH_MAX];
	va_list ap;
	int rc;

	/* Construct the file name */
	UTIL_VSPRINTF(path, fmt, ap);

	switch (base) {
	case 8:
		util_asprintf(&str, "%lo", val);
		break;
	case 10:
		util_asprintf(&str, "%ld", val);
		break;
	case 16:
		util_asprintf(&str, "%lx", val);
		break;
	default:
		util_panic("Invalid base: %d\n", base);
	}
	rc = file_puts(str, path);
	free(str);
	return rc;
}

/**
 * Write signed long long value to a file according to given base
 *
 * @param[in] val       Value is to be written
 * @param[in] base      Base for conversion, either 8, 10, or 16
 * @param[in] fmt       Format string for generation of the path name
 * @param[in] ...       Parameters for format string
 *
 * @retval    0         Write was successful
 * @retval   -1         Error while writing file
 */
int util_file_write_ll(long long val, int base, const char *fmt, ...)
{
	char *str, path[PATH_MAX];
	va_list ap;
	int rc;

	/* Construct the file name */
	UTIL_VSPRINTF(path, fmt, ap);

	switch (base) {
	case 8:
		util_asprintf(&str, "%llo", val);
		break;
	case 10:
		util_asprintf(&str, "%lld", val);
		break;
	case 16:
		util_asprintf(&str, "%llx", val);
		break;
	default:
		util_panic("Invalid base: %d\n", base);
	}
	rc = file_puts(str, path);
	free(str);
	return rc;
}

/**
 * Write unsigned long value to a file according to given base
 *
 * @param[in] val       Value is to be written
 * @param[in] base      Base for conversion, either 8, 10, or 16
 * @param[in] fmt       Format string for generation of the path name
 * @param[in] ...       Parameters for format string
 *
 * @retval    0         Write was successful
 * @retval   -1         Error while writing file
 */
int util_file_write_ul(unsigned long val, int base, const char *fmt, ...)
{
	char *str, path[PATH_MAX];
	va_list ap;
	int rc;

	/* Construct the file name */
	UTIL_VSPRINTF(path, fmt, ap);

	switch (base) {
	case 8:
		util_asprintf(&str, "%lo", val);
		break;
	case 10:
		util_asprintf(&str, "%lu", val);
		break;
	case 16:
		util_asprintf(&str, "%lx", val);
		break;
	default:
		util_panic("Invalid base: %d\n", base);
	}
	rc = file_puts(str, path);
	free(str);
	return rc;
}

/**
 * Write unsigned long long value to a file according to given base
 *
 * @param[in] val       Value is to be written
 * @param[in] base      Base for conversion, either 8, 10, or 16
 * @param[in] fmt       Format string for generation of the path name
 * @param[in] ...       Parameters for format string
 *
 * @retval    0         Write was successful
 * @retval   -1         Error while writing file
 */
int util_file_write_ull(unsigned long long val, int base, const char *fmt, ...)
{
	char *str, path[PATH_MAX];
	va_list ap;
	int rc;

	/* Construct the file name */
	UTIL_VSPRINTF(path, fmt, ap);

	switch (base) {
	case 8:
		util_asprintf(&str, "%llo", val);
		break;
	case 10:
		util_asprintf(&str, "%llu", val);
		break;
	case 16:
		util_asprintf(&str, "%llx", val);
		break;
	default:
		util_panic("Invalid base: %d\n", base);
	}
	rc = file_puts(str, path);
	free(str);
	return rc;
}

/**
 * Read a file and convert it to signed long according to given base
 *
 * @param[out] val      Buffer for value
 * @param[in]  base     Base for conversion, either 8, 10, or 16
 * @param[in]  fmt      Format string for generation of the path name
 * @param[in]  ...      Parameters for format string
 *
 * @retval     0        Long integer has been read correctly
 * @retval    -1        Error while reading file
 */
int util_file_read_l(long *val, int base, const char *fmt, ...)
{
	char path[PATH_MAX], buf[512];
	va_list ap;
	int count;

	/* Construct the file name */
	UTIL_VSPRINTF(path, fmt, ap);

	if (file_gets(buf, sizeof(buf), path))
		return -1;
	switch (base) {
	case 8:
		count = sscanf(buf, "%lo", val);
		break;
	case 10:
		count = sscanf(buf, "%ld", val);
		break;
	case 16:
		count = sscanf(buf, "%lx", val);
		break;
	default:
		util_panic("Invalid base: %d\n", base);
	}
	return (count == 1) ? 0 : -1;
}

/**
 * Read a file and convert it to signed long long according to given base
 *
 * @param[out] val      Buffer for value
 * @param[in]  base     Base for conversion, either 8, 10, or 16
 * @param[in]  fmt      Format string for generation of the path name
 * @param[in]  ...      Parameters for format string
 *
 * @retval     0        Long integer has been read correctly
 * @retval    -1        Error while reading file
 */
int util_file_read_ll(long long *val, int base, const char *fmt, ...)
{
	char path[PATH_MAX], buf[512];
	va_list ap;
	int count;

	/* Construct the file name */
	UTIL_VSPRINTF(path, fmt, ap);

	if (file_gets(buf, sizeof(buf), path))
		return -1;
	switch (base) {
	case 8:
		count = sscanf(buf, "%llo", val);
		break;
	case 10:
		count = sscanf(buf, "%lld", val);
		break;
	case 16:
		count = sscanf(buf, "%llx", val);
		break;
	default:
		util_panic("Invalid base: %d\n", base);
	}
	return (count == 1) ? 0 : -1;
}

/**
 * Read a file and convert it to unsigned long according to given base
 *
 * @param[out] val      Buffer for value
 * @param[in]  base     Base for conversion, either 8, 10, or 16
 * @param[in]  fmt      Format string for generation of the path name
 * @param[in]  ...      Parameters for format string
 *
 * @retval     0        Long integer has been read correctly
 * @retval    -1        Error while reading file
 */
int util_file_read_ul(unsigned long *val, int base, const char *fmt, ...)
{
	char path[PATH_MAX], buf[512];
	va_list ap;
	int count;

	/* Construct the file name */
	UTIL_VSPRINTF(path, fmt, ap);

	if (file_gets(buf, sizeof(buf), path))
		return -1;
	switch (base) {
	case 8:
		count = sscanf(buf, "%lo", val);
		break;
	case 10:
		count = sscanf(buf, "%lu", val);
		break;
	case 16:
		count = sscanf(buf, "%lx", val);
		break;
	default:
		util_panic("Invalid base: %d\n", base);
	}
	return (count == 1) ? 0 : -1;
}

/**
 * Read a file and convert it to unsigned long long according to given base
 *
 * @param[out] val      Buffer for value
 * @param[in]  base     Base for conversion, either 8, 10, or 16
 * @param[in]  fmt      Format string for generation of the path name
 * @param[in]  ...      Parameters for format string
 *
 * @retval     0        Long integer has been read correctly
 * @retval    -1        Error while reading file
 */
int util_file_read_ull(unsigned long long *val, int base, const char *fmt, ...)
{
	char path[PATH_MAX], buf[512];
	va_list ap;
	int count;

	/* Construct the file name */
	UTIL_VSPRINTF(path, fmt, ap);

	if (file_gets(buf, sizeof(buf), path))
		return -1;
	switch (base) {
	case 8:
		count = sscanf(buf, "%llo", val);
		break;
	case 10:
		count = sscanf(buf, "%llu", val);
		break;
	case 16:
		count = sscanf(buf, "%llx", val);
		break;
	default:
		util_panic("Invalid base: %d\n", base);
	}
	return (count == 1) ? 0 : -1;
}
