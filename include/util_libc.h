/**
 * @defgroup util_libc_h util_libc: Libc wrapper interface
 * @{
 * @brief Handle standard errors for libc functions
 *
 * Copyright IBM Corp. 2016
 */

#ifndef UTIL_LIBC_H
#define UTIL_LIBC_H

#include <stdio.h>

/**
 * Allocate memory or panic in case of failure
 *
 * @param[in] size  Number of bytes to be allocated
 *
 * @returns   Pointer to allocated memory buffer
 */
#define util_malloc(size)		\
	__util_malloc(__func__, __FILE__, __LINE__, size)

void *__util_malloc(const char *func, const char *file, int line, size_t size);

/**
 * Allocate zero-initialized memory or panic in case of failure
 *
 * @param[in] size  Number of bytes to be allocated
 *
 * @returns   Pointer to allocated memory buffer initialized with zeroes
 */
#define util_zalloc(size)		\
	__util_zalloc(__func__, __FILE__, __LINE__, size)

void *__util_zalloc(const char *func, const char *file, int line, size_t size);


/**
 * Re-allocate memory or exit in case of failure
 *
 * @param[in] ptr  Pointer ot old memory buffer
 * @param[in] size Number of bytes to be allocated
 *
 * @returns   Pointer to allocated memory buffer
 */
#define util_realloc(ptr, size)		\
	__util_realloc(__func__, __FILE__, __LINE__, ptr, size)

void *__util_realloc(const char *func, const char *file, int line,
		     void *ptr, size_t size);

/**
 * Duplicate a string buffer or exit in case of failure
 *
 * @param[in] str String to be duplicated
 *
 * @returns   Pointer to newly allocated and copied string
 */
#define util_strdup(str)		\
	__util_strdup(__func__, __FILE__, __LINE__, str)

void *__util_strdup(const char *func, const char *file, int line,
		    const char *str);

/**
 * Print to allocated string or exit in case of failure
 *
 * @param[in] strp  String to be allocated
 * @param[in] fmt   Format string for generation of string
 * @param[in] ap    Parameters for format string
 *
 * @returns   num   Number of formatted characters
 */
#define util_vasprintf(strp, fmt, ap)	\
	__util_vasprintf(__func__, __FILE__, __LINE__, strp, fmt, ap)

#define UTIL_VASPRINTF(strp, fmt, ap)			\
do {							\
	va_start(ap, fmt);				\
	util_vasprintf(strp, fmt, ap);			\
	va_end(ap);					\
} while (0)

int __util_vasprintf(const char *func, const char *file, int line,
		     char **strp, const char *fmt, va_list ap);

/**
 * Print to newly allocated string or exit in case of failure
 *
 * @param[in] strp   String to be allocated
 * @param[in] ...    Format string and parameters for format string
 *
 * @returns   num    Number of formatted characters
 */
#define util_asprintf(strp, ...)	\
	__util_asprintf(__func__, __FILE__, __LINE__, strp, ##__VA_ARGS__)

int __util_asprintf(const char *func, const char *file, int line,
		    char **strp, const char *fmt, ...);


/**
 * Print to string buffer or exit in case of failure
 *
 * @param[in] str   String buffer
 * @param[in] fmt   Format string for generation of string
 * @param[in] ap    Parameters for format string
 *
 * @returns   num   Number of formatted characters
 */
#define util_vsprintf(str, fmt, ap)	\
	__util_vsprintf(__func__, __FILE__, __LINE__, str, fmt, ap)

#define UTIL_VSPRINTF(str, fmt, ap)			\
do {							\
	va_start(ap, fmt);				\
	util_vsprintf(str, fmt, ap);			\
	va_end(ap);					\
} while (0)

int __util_vsprintf(const char *func, const char *file, int line,
		    char *str, const char *fmt, va_list ap);

#endif /** UTIL_LIBC_H @} */
