/*
 * Copyright IBM Corp. 2016
 *
 * Author(s): Peter Oberparleiter <oberpar@linux.vnet.ibm.com>
 */

#ifndef UDEV_ZFCP_LUN_H
#define UDEV_ZFCP_LUN_H

#include "exit_code.h"
#include "misc.h"

struct device;

void udev_zfcp_lun_add_device_ids(struct util_list *);
bool udev_zfcp_lun_exists(const char *);
exit_code_t udev_zfcp_lun_read_device(struct device *);
exit_code_t udev_zfcp_lun_write_device(struct device *);
exit_code_t udev_zfcp_lun_remove_rule(const char *);

#endif /* UDEV_ZFCP_LUN_H */
