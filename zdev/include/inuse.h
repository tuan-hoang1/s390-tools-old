/*
 * Copyright IBM Corp. 2016
 *
 * Author(s): Peter Oberparleiter <oberpar@linux.vnet.ibm.com>
 */

#ifndef INUSE_H
#define INUSE_H

struct util_list;
struct device;

void inuse_exit(void);
struct util_list *inuse_get_resources(struct device *dev);

#endif /* INUSE_H */
