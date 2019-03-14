/*
 * dasd - Library for DASD related functions
 *
 * DASD related helper functions for accessing device information via sysfs
 *
 * Copyright IBM Corp. 2016
 */

#ifndef DASD_SYS_H
#define DASD_SYS_H

#include <stdio.h>
#include "u2s.h"

int dasd_sys_raw_track_access(char *);
int dasd_reset_chpid(char *, char *);

#endif /* DASD_SYS_H */
