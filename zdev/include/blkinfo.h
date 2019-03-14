/*
 * Copyright IBM Corp. 2016
 *
 * Author(s): Peter Oberparleiter <oberpar@linux.vnet.ibm.com>
 */

#ifndef BLKINFO_H
#define BLKINFO_H

struct util_list;
struct devnode;

void blkinfo_exit(void);

struct util_list *blkinfo_get_ancestor_devnodes(struct devnode *);
struct util_list *blkinfo_get_same_uuid_devnodes(struct devnode *);
struct devnode *blkinfo_get_devnode_by_path(const char *);
void blkinfo_add_mountpoints(struct util_list *);
void blkinfo_add_swap_devnodes(struct util_list *);

#endif /* BLKINFO_H */
