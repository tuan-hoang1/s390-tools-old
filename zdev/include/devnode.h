/*
 * Copyright IBM Corp. 2016
 *
 * Author(s): Peter Oberparleiter <oberpar@linux.vnet.ibm.com>
 */

#ifndef DEVNODE_H
#define DEVNODE_H

#include "misc.h"

struct util_list;

typedef enum {
	BLOCKDEV,
	CHARDEV,
	NETDEV,
} devnode_t;

struct devnode {
	devnode_t type;

	unsigned int major;
	unsigned int minor;

	char name[];
};

struct devnode *devnode_new(devnode_t, unsigned int, unsigned int,
			    const char *);
struct devnode *devnode_copy(struct devnode *);
void devnode_print(struct devnode *, int);

struct devnode *devnode_from_node(const char *, err_t);
struct devnode *devnode_from_devfile(const char *, const char *, devnode_t);
struct devnode *devnode_from_path(const char *path);

int devnode_cmp(struct devnode *, struct devnode *);
int devnode_add_block_from_sysfs(struct util_list *, const char *);
int devnode_add_net_from_sysfs(struct util_list *, const char *);
char *devnode_readlink(struct devnode *);

#endif /* DEVNODE_H */
