/*
 * Copyright IBM Corp. 2016
 *
 * Author(s): Peter Oberparleiter <oberpar@linux.vnet.ibm.com>
 */

#ifndef QETH_AUTO_H
#define QETH_AUTO_H

#include "misc.h"

struct util_list;
struct ccwgroup_devid;
struct ccw_devid;

void qeth_auto_add_ids(struct util_list *);
exit_code_t qeth_auto_get_devid(struct ccwgroup_devid *, struct ccw_devid *,
				err_t);
exit_code_t qeth_auto_is_possible(struct ccwgroup_devid *, err_t);

#endif /* QETH_AUTO_H */
