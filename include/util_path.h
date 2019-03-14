/**
 * @defgroup util_path_h util_path: Path interface
 * @{
 * @brief Work with paths
 *
 * Copyright IBM Corp. 2016
 */

#ifndef UTIL_PATH_H
#define UTIL_PATH_H

#include <stdbool.h>

char *util_path_sysfs(const char *fmt, ...);

bool util_path_is_readable(const char *fmt, ...);
bool util_path_is_writable(const char *fmt, ...);
bool util_path_is_dir(const char *fmt, ...);
bool util_path_is_reg_file(const char *fmt, ...);

#endif /** UTIL_PATH_H @} */
