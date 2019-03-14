/*
 * Copyright IBM Corp. 2016
 *
 * Author(s): Peter Oberparleiter <oberpar@linux.vnet.ibm.com>
 */

#ifndef CTC_H
#define CTC_H

#include <stdbool.h>

#define	CTC_MOD_NAME		"ctcm"
#define CTC_CCWGROUPDRV_NAME	"ctcm"
#define CTC_CCWDRV_NAME		"ctcm"
#define CTC_ROOTDRV_NAME	"ctcm"
#define	CTC_NUM_DEVS		2

struct devtype;
struct namespace;
struct ccw_devid;

extern struct devtype ctc_devtype;
extern struct namespace ctc_namespace;

void ctc_exit(void);
bool ctc_confirm(struct ccw_devid *);

#endif /* CTC_H */
