/*
 * Copyright IBM Corp. 2016
 *
 * Author(s): Peter Oberparleiter <oberpar@linux.vnet.ibm.com>
 */

#ifndef ZFCP_LUN_H
#define ZFCP_LUN_H

#include <stdint.h>

#include "ccw.h"

#define ZFCP_LUN_NAME		"zfcp-lun"
#define SCSI_ATTR_PREFIX	"scsi_dev"

struct subtype;
struct namespace;

extern struct subtype zfcp_lun_subtype;
extern struct namespace zfcp_lun_namespace;

struct zfcp_lun_devid {
	struct ccw_devid fcp_dev;
	uint64_t wwpn;
	uint64_t lun;
} __attribute__ ((packed));

exit_code_t zfcp_lun_parse_devid(struct zfcp_lun_devid *, const char *, err_t);
int zfcp_lun_cmp_devids(struct zfcp_lun_devid *, struct zfcp_lun_devid *);
char *zfcp_lun_devid_to_str(struct zfcp_lun_devid *);

#endif /* ZFCP_LUN_H */
