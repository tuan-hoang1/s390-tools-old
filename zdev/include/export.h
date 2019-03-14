/*
 * Copyright IBM Corp. 2016
 *
 * Author(s): Peter Oberparleiter <oberpar@linux.vnet.ibm.com>
 */

#ifndef EXPORT_H
#define EXPORT_H

#include <stdio.h>

#include "exit_code.h"
#include "misc.h"

struct device;
struct devtype;

typedef enum {
	export_device,
	export_devtype,
} export_t;

struct export_object {
	export_t type;
	union {
		struct devtype *dt;
		struct device *dev;
	} ptr;
};

exit_code_t export_write_device(FILE *, struct device *, config_t, int *);
exit_code_t export_write_devtype(FILE *, struct devtype *, config_t, int *);
exit_code_t export_read(FILE *, const char *, struct util_list *);

#endif /* EXPORT_H */
