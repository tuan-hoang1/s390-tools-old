/*
 * Copyright IBM Corp. 2016
 *
 * Author(s): Peter Oberparleiter <oberpar@linux.vnet.ibm.com>
 */

#ifndef QETH_H
#define QETH_H

#define	QETH_MOD_NAME		"qeth"
#define QETH_CCWGROUPDRV_NAME	"qeth"
#define QETH_CCWDRV_NAME	"qeth"
#define QETH_ROOTDRV_NAME	"qeth"
#define	QETH_NUM_DEVS		3

struct devtype;
struct namespace;

extern struct devtype qeth_devtype;
extern struct namespace qeth_namespace;

#endif /* QETH_H */
