/*
 * Copyright IBM Corp. 2016
 *
 * Author(s): Peter Oberparleiter <oberpar@linux.vnet.ibm.com>
 */

#ifndef TABLE_ATTRIBS_H
#define TABLE_ATTRIBS_H

#include "misc.h"

struct util_list;
struct devtype;
struct subtype;
struct attrib;

struct table_attrib {
	struct subtype *st;
	struct attrib *attrib;
};

struct table_attrib *table_attrib_new(struct subtype *, struct attrib *);
void table_attribs_show(struct util_list *, int, int, struct devtype *);
void table_attribs_show_details(struct util_list *, struct devtype *);

#endif /* TABLE_ATTRIBS_H */
