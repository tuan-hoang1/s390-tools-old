/*
 * Copyright IBM Corp. 2016
 *
 * Author(s): Peter Oberparleiter <oberpar@linux.vnet.ibm.com>
 */

#ifndef MODULE_H
#define MODULE_H

#include "exit_code.h"
#include "misc.h"

struct setting_list;
struct attrib;

void module_exit(void);

void module_load_suppress(int);

bool module_loaded(const char *);
exit_code_t module_load(const char *, const char **, struct setting_list *,
			err_t);
exit_code_t module_get_params(const char *, struct attrib **,
			      struct setting_list **);
bool module_set_params(const char *, struct setting_list *);
void module_try_load_once(const char *, const char *);

#endif /* MODULE_H */
