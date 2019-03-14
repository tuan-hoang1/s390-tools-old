/*
 * Copyright IBM Corp. 2016
 *
 * Author(s): Peter Oberparleiter <oberpar@linux.vnet.ibm.com>
 */

#ifndef TABLE_TYPES_H
#define TABLE_TYPES_H

#include "misc.h"

struct util_list;

exit_code_t table_types_show(struct util_list *, int, int);

#endif /* TABLE_TYPES_H */
