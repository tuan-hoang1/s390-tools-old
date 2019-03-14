/*
 * Copyright IBM Corp. 2016
 *
 * Author(s): Peter Oberparleiter <oberpar@linux.vnet.ibm.com>
 */

#ifndef UDEV_CCWGROUP_H
#define UDEV_CCWGROUP_H

#include "exit_code.h"
#include "misc.h"

struct device;
struct util_list;

bool udev_ccwgroup_exists(const char *, const char *);
exit_code_t udev_ccwgroup_read_device(struct device *);
exit_code_t udev_ccwgroup_write_device(struct device *);
void udev_ccwgroup_add_device_ids(const char *, struct util_list *);
exit_code_t udev_ccwgroup_remove_rule(const char *, const char *);

#endif /* UDEV_CCWGROUP_H */
