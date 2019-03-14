/*
 * Copyright IBM Corp. 2016
 *
 * Author(s): Peter Oberparleiter <oberpar@linux.vnet.ibm.com>
 */

#ifndef UDEV_H
#define UDEV_H

#include "exit_code.h"
#include "util.h"

extern int udev_need_settle;

/* Single key-operator-value entry in a udev rule line.*/
struct udev_entry_node {
	struct util_list_node node;
	char *key;
	char *op;
	char *value;
};

/* Single udev line in a udev rule file. */
struct udev_line_node {
	struct util_list_node node;
	struct util_list entries;
	char *line;
};

/* Udev rule file. */
struct udev_file {
	struct util_list lines;
};

exit_code_t udev_read_file(const char *, struct udev_file **);
void udev_free_file(struct udev_file *);
void udev_file_print(struct udev_file *);

void udev_get_device_ids(const char *, struct util_list *);
exit_code_t udev_remove_rule(const char *, const char *);

void udev_settle(void);

#endif /* UDEV_H */
