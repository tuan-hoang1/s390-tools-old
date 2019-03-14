/*
 * Copyright IBM Corp. 2016
 *
 * Author(s): Peter Oberparleiter <oberpar@linux.vnet.ibm.com>
 */

#ifndef FINDMNT_H
#define FINDMNT_H

struct util_list;

struct util_list *findmnt_get_devnodes_by_path(const char *path);

#endif /* FINDMNT_H */
