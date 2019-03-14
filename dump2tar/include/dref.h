/*
 * dump2tar - tool to dump files and command output into a tar archive
 *
 * Reference counting for directory handles
 *
 * Copyright IBM Corp. 2016
 */

#ifndef DREF_H
#define DREF_H

#include <dirent.h>
#include <stdbool.h>

/* Multiple jobs may refer to an open DIR * - need reference counting */
struct dref {
	DIR *dd;
	int dirfd;
	unsigned int count;
};

struct dref *dref_create(const char *dirname);
struct dref *dref_get(struct dref *dref);
void dref_put(struct dref *dref);

#endif /* DREF_H */
