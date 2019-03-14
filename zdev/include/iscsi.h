/*
 * Copyright IBM Corp. 2016
 *
 * Author(s): Peter Oberparleiter <oberpar@linux.vnet.ibm.com>
 */

#ifndef ISCSI_H
#define ISCSI_H

struct devnode;

struct devnode *iscsi_get_net_devnode(struct devnode *);

#endif /* ISCSI_H */
