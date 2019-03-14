/*
 * Copyright IBM Corp. 2016
 *
 * Author(s): Peter Oberparleiter <oberpar@linux.vnet.ibm.com>
 */

#ifndef MODPROBE_H
#define MODPROBE_H

#include "exit_code.h"
#include "misc.h"

struct attrib;
struct setting_list;

exit_code_t modprobe_read_settings(const char *, const char *,
				   struct attrib **, struct setting_list **);
exit_code_t modprobe_write_settings(const char *, const char *,
				    struct setting_list *);

#endif /* MODPROBE_H */
