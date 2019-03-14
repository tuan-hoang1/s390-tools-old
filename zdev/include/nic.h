/*
 * Copyright IBM Corp. 2016
 *
 * Author(s): Peter Oberparleiter <oberpar@linux.vnet.ibm.com>
 */

#ifndef NIC_H
#define NIC_H

#include <stdbool.h>

enum nic_type {
	nic_qdio,
	nic_hipers,
	nic_iedn,
	nic_inmn,
};

enum nic_target {
	nic_vswitch,
	nic_lan,
};

#define NIC_OWNER_LEN	9
#define NIC_NAME_LEN	9

struct nic_data {
	enum nic_type type;
	enum nic_target target;
	char owner[NIC_OWNER_LEN];
	char name[NIC_NAME_LEN];
};

bool nic_data_get(const char *, struct nic_data *);
void nic_data_print(struct nic_data *, int);
bool nic_vswitch_get_layer2(const char *, int *);
bool nic_lan_get_layer2(const char *, const char *, int *);

#endif /* NIC_H */
