/*
 * Copyright IBM Corp. 2016
 *
 * Author(s): Peter Oberparleiter <oberpar@linux.vnet.ibm.com>
 */

#ifndef LCS_H
#define LCS_H

#define	LCS_MOD_NAME		"lcs"
#define LCS_CCWGROUPDRV_NAME	"lcs"
#define LCS_CCWDRV_NAME		"lcs"
#define LCS_ROOTDRV_NAME	"lcs"
#define	LCS_NUM_DEVS		2

struct devtype;
struct namespace;

extern struct devtype lcs_devtype;
extern struct namespace lcs_namespace;

#endif /* LCS_H */
