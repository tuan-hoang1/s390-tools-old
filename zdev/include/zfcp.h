/*
 * Copyright IBM Corp. 2016
 *
 * Author(s): Peter Oberparleiter <oberpar@linux.vnet.ibm.com>
 */

#ifndef ZFCP_H
#define ZFCP_H

#include "exit_code.h"
#include "misc.h"

#define	ZFCP_MOD_NAME		"zfcp"
#define ZFCP_CCWDRV_NAME	"zfcp"

struct devtype;

extern struct devtype zfcp_devtype;

exit_code_t zfcp_check_allow_lun_scan(int *, config_t);

#endif /* ZFCP_H */
