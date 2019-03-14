/*
 * Copyright IBM Corp. 2016
 *
 * Author(s): Peter Oberparleiter <oberpar@linux.vnet.ibm.com>
 */

#ifndef ZFCP_HOST_H
#define ZFCP_HOST_H

#include "exit_code.h"

struct subtype;

extern struct subtype zfcp_host_subtype;

exit_code_t zfcp_host_check_npiv(const char *, int *);

#endif /* ZFCP_HOST_H */
