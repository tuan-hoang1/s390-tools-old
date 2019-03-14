/*
 * Copyright IBM Corp. 2016
 *
 * Author(s): Peter Oberparleiter <oberpar@linux.vnet.ibm.com>
 */

#ifndef DEVICE_H
#define DEVICE_H

#include "exit_code.h"
#include "hash.h"
#include "misc.h"
#include "util.h"

struct subtype;
struct setting_list;

/**
 * struct device_state - Represent the state of a device in a configuration
 * @settings: List of attribute settings
 * @exists: Device exists
 * @modified: Device state has been modified
 * @deconfigured: Device has been deconfigured
 * @definable: Device can be defined
 * @blacklisted: Device is on the blacklist
 */
struct device_state {
	struct setting_list *settings;
	unsigned int exists:1;
	unsigned int modified:1;
	unsigned int deconfigured:1;
	unsigned int definable:1;
	unsigned int blacklisted:1;
};

/**
 * struct device - Represent a device configuration
 * @subtype: Pointer to device subtype definition
 * @id: Textual device ID
 * @devid: Parsed device ID
 * @node: Node for adding this device to a list
 * @active: Device state in the active configuration
 * @persistent: Device state in the persistent configuration
 * @errors: A strlist of error and warning messages issued for the device
 * @processed: Device has been processed
 */
struct device {
	/* Static data */
	struct subtype *subtype;
	char *id;
	void *devid;

	/* Dynamic data */
	struct util_list_node node;

	struct device_state active;
	struct device_state persistent;

	unsigned int processed:1;
};

/**
 * struct device_list - A list of devices
 * @hash: Hash for maintaining a list of devices
 * @modified: Indication of whether this list was modified
 */
struct device_list {
	struct hash hash;
	unsigned int modified:1;
};

struct device *device_new(struct subtype *, const char *);
void device_free(struct device *);
void device_reset(struct device *, config_t);
void device_print(struct device *, int);

bool device_needs_writing(struct device *, config_t);
exit_code_t device_apply_strlist(struct device *, config_t, struct util_list *);
exit_code_t device_apply_settings(struct device *, config_t,
				  struct util_list *);
void device_add_modules(struct util_list *, struct device *);
char *device_read_active_attrib(struct device *, const char *);
void device_read_active_settings(struct device *, read_scope_t);
exit_code_t device_write_active_settings(struct device *);
exit_code_t device_check_settings(struct device *, config_t, err_t);

struct device_list *device_list_new(struct subtype *);
void device_list_free(struct device_list *);
void device_list_add(struct device_list *, struct device *);
struct device *device_list_find(struct device_list *, const char *,
				struct device *);
void device_list_print(struct device_list *, int);

#endif /* DEVICE_H */
