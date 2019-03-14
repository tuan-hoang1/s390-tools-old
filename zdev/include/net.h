/*
 * Copyright IBM Corp. 2016
 *
 * Author(s): Peter Oberparleiter <oberpar@linux.vnet.ibm.com>
 */

#ifndef NET_H
#define NET_H

#include <stdbool.h>

struct util_list;
struct devnode;

bool net_add_linked_devnodes(struct util_list *, struct devnode *);
bool net_add_vlan_base(struct util_list *, struct devnode *);
bool net_add_bonding_base(struct util_list *, struct devnode *);

#endif /* NET_H */
