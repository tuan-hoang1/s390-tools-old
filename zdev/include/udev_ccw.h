/*
 * Copyright IBM Corp. 2016
 *
 * Author(s): Peter Oberparleiter <oberpar@linux.vnet.ibm.com>
 */

#ifndef UDEV_CCW_H
#define UDEV_CCW_H

#include "exit_code.h"
#include "misc.h"

struct device;

bool udev_ccw_exists(const char *, const char *);
exit_code_t udev_ccw_read_device(struct device *);
exit_code_t udev_ccw_write_device(struct device *);
exit_code_t udev_ccw_write_cio_ignore(const char *);

#endif /* UDEV_CCW_H */
