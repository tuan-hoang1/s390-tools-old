/**
 * lszcrypt - Display zcrypt devices and configuration settings
 *
 * Copyright IBM Corp. 2008, 2016
 */

#include <err.h>
#include <stdint.h>

#include "util_base.h"
#include "util_panic.h"
#include "util_path.h"
#include "util_scandir.h"
#include "util_file.h"
#include "util_opt.h"
#include "util_prg.h"
#include "util_proc.h"
#include "util_rec.h"
#include "zt_common.h"

/*
 * Private data
 */
struct lszcrypt_l {
	int verbose;
} l;

struct lszcrypt_l *lszcrypt_l = &l;

/*
 * Capabilities
 */
#define CAP_RSA2K	"RSA 2K Clear Key"
#define CAP_RSA4K	"RSA 4K Clear Key"
#define CAP_CCA		"CCA Secure Key"
#define CAP_RNG		"Long RNG"
#define CAP_EP11	"EP11 Secure Key"

/*
 * Card types
 */
#define MASK_APSC	0x80000000
#define MASK_RSA4K	0x60000000
#define MASK_COPRO	0x10000000
#define MASK_ACCEL	0x08000000
#define MASK_EP11	0x04000000

/*
 * Program configuration
 */
const struct util_prg prg = {
	.desc = "Display zcrypt device and configuration information.",
	.args = "[DEVICE_IDS]",
	.copyright_vec = {
		{
			.owner = "IBM Corp.",
			.pub_first = 2008,
			.pub_last = 2016,
		},
		UTIL_PRG_COPYRIGHT_END
	}
};

/*
 * Configuration of command line options
 */
static struct util_opt opt_vec[] = {
	{
		.option = {"bus", 0, NULL, 'b'},
		.desc = "Show AP bus attributes then exit",
	},
	{
		.option = { "capability", required_argument, NULL, 'c'},
		.argument = "DEVICE_ID",
		.desc = "Show the capabilities of a cryptographic device",
	},
	{
		.option = {"domains", 0, NULL, 'd'},
		.desc = "Show the configured AP usage and control domains",
	},
	{
		.option = {"verbose", 0, NULL, 'V'},
		.desc = "Print verbose messages",
	},
	UTIL_OPT_HELP,
	UTIL_OPT_VERSION,
	UTIL_OPT_END
};

/*
 * Show bus
 */
static void show_bus(void)
{
	long domain, max_domain, config_time, value;
	unsigned long long poll_timeout;
	const char *poll_thread, *ap_interrupts;
	char *ap;

	ap = util_path_sysfs("bus/ap");
	util_file_read_l(&domain, 10, "%s/ap_domain", ap);
	util_file_read_l(&max_domain, 10, "%s/ap_max_domain_id", ap);
	util_file_read_l(&config_time, 10, "%s/config_time", ap);
	util_file_read_ull(&poll_timeout, 10, "%s/poll_timeout", ap);
	util_file_read_l(&value, 10, "%s/poll_thread", ap);
	if (value == 1)
		poll_thread = "enabled";
	else
		poll_thread = "disabled";
	util_file_read_l(&value, 10, "%s/ap_interrupts", ap);
	if (value == 1)
		ap_interrupts = "enabled";
	else
		ap_interrupts = "disabled";
	printf("ap_domain=0x%lx\n", domain);
	printf("ap_max_domain_id=0x%lx\n", max_domain);
	if (util_path_is_reg_file("%s/ap_interrupts", ap))
		printf("ap_interrupts are %s\n", ap_interrupts);
	printf("config_time=%ld (seconds)\n", config_time);
	printf("poll_thread is %s\n", poll_thread);
	if (util_path_is_reg_file("%s/poll_timeout", ap))
		printf("poll_timeout=%llu (nanoseconds)\n", poll_timeout);
	free(ap);
}

/*
 * Print domain array using util_rec
 */
static void show_domains_util_rec(char *domain_array[])
{
	struct util_rec *rec = util_rec_new_wide("-");
	char buf[256];
	int i, x, n;

	util_rec_def(rec, "domain", UTIL_REC_ALIGN_RIGHT, 6, "DOMAIN");
	for (i = 0; i < 16; i++) {
		sprintf(buf, "%02x", i);
		util_rec_def(rec, buf, UTIL_REC_ALIGN_RIGHT, 2, buf);
	}

	util_rec_print_hdr(rec);
	n = 0;
	for (i = 0; i < 16; i++) {
		sprintf(buf, "%02x", i * 16);
		util_rec_set(rec, "domain", buf);
		for (x = 0; x < 16; x++) {
			sprintf(buf, "%02x", x);
			util_rec_set(rec, buf, domain_array[n++]);
		}
		util_rec_print(rec);
	}
	util_rec_free(rec);
	printf("------------------------------------------------------\n");
	printf("C: Control domain \n");
	printf("U: Usage domain\n");
	printf("B: Both (Control + Usage domain)\n");
}

/*
 * Show domains
 */
static void show_domains(void)
{
	char ctrl_domain_mask[67], usag_domain_mask[67], byte_str[3] = {};
	int ctrl_chunk, usag_chunk;
	char *ap, *domain_array[32 * 8 + 4];
	int i, x, n;
	uint8_t dom_mask_bit;

	ap = util_path_sysfs("bus/ap");
	util_file_read_line(ctrl_domain_mask, sizeof(ctrl_domain_mask),
			    "%s/ap_control_domain_mask", ap);
	util_file_read_line(usag_domain_mask, sizeof(usag_domain_mask),
			    "%s/ap_usage_domain_mask", ap);
	/* remove leading '0x' from domain mask string */
	memmove(&ctrl_domain_mask[0], &ctrl_domain_mask[2],
		sizeof(ctrl_domain_mask) - 2);
	memmove(&usag_domain_mask[0], &usag_domain_mask[2],
		sizeof(usag_domain_mask) - 2);
	n = 0;
	for (i = 0; i < 32; i++) {
		dom_mask_bit = 0x80;
		memcpy(byte_str, &ctrl_domain_mask[i * 2], 2);
		sscanf(byte_str, "%02x", &ctrl_chunk);
		memcpy(byte_str, &usag_domain_mask[i * 2], 2);
		sscanf(byte_str, "%02x", &usag_chunk);
		for (x = 1; x <= 8; x++) {
			if (ctrl_chunk & dom_mask_bit &&
			    usag_chunk & dom_mask_bit)
				domain_array[n] = "B"; /* c/u */
			else if (ctrl_chunk & dom_mask_bit)
				domain_array[n] = "C";
			else if (usag_chunk & dom_mask_bit)
				domain_array[n] = "U";
			else
				domain_array[n] = ".";
			dom_mask_bit = dom_mask_bit >> 1;
			n += 1;
		}
	}
	for (i = n; i < 260; i++)
		domain_array[n++] = "";

	show_domains_util_rec(domain_array);
	return;
}

/*
 * Show capability
 */
static void show_capability(const char *id_str)
{
	unsigned long func_val;
	long hwtype, id;
	char *p, *dev, card[7];

	id = strtol(id_str, &p, 0);
	if (id < 0 || id > 255 || p == id_str || *p != '\0')
		errx(EXIT_FAILURE, "error - '%s' is an invalid cryptographic device id!", id_str);
	snprintf(card, sizeof(card), "card%02lx", id);
	dev = util_path_sysfs("devices/ap/%s", card);
	if (!util_path_is_dir(dev))
		errx(EXIT_FAILURE, "error - cryptographic device %s does not exist!", card);
	util_file_read_l(&hwtype, 10, "%s/hwtype", dev);
	/* If sysfs attribute is missing, set functions to 0 */
	if (util_file_read_ul(&func_val, 16, "%s/ap_functions", dev))
		func_val = 0x00000000;
	/* Skip devices, which are not supported by zcrypt layer */
	if (!util_path_is_readable("%s/type", dev) &&
	    !util_path_is_readable("%s/online", dev)) {
		printf("Detailed capability information for %s (hardware type %ld) is not available.\n", card, hwtype);
		return;
	}
	printf("%s provides capability for:\n", card);
	switch (hwtype) {
	case 6:
	case 8:
		if (func_val & MASK_RSA4K)
			printf("%s", CAP_RSA4K);
		else
			printf("%s", CAP_RSA2K);
		break;
	case 7:
	case 9:
		printf("%s\n", CAP_RSA4K);
		printf("%s\n", CAP_CCA);
		printf("%s", CAP_RNG);
		break;
	case 10:
	case 11:
		if (func_val & MASK_ACCEL) {
			if (func_val & MASK_RSA4K)
				printf("%s", CAP_RSA4K);
			else
				printf("%s", CAP_RSA2K);
		} else if (func_val & MASK_COPRO) {
			printf("%s\n", CAP_RSA4K);
			printf("%s\n", CAP_CCA);
			printf("%s", CAP_RNG);
		} else if (func_val & MASK_EP11) {
			printf("%s", CAP_EP11);
		} else {

			printf("Detailed capability information for %s (hardware type %ld) is not available.", card, hwtype);
		}
		break;
	default:
			printf("Detailed capability information for %s (hardware type %ld) is not available.", card, hwtype);
		break;
	}
	printf("\n");
}

/*
 * Read subdevice default attributes
 */
static void read_subdev_rec_default(struct util_rec *rec, const char *grp_dev,
				    const char *sub_dev)
{
	unsigned long facility;
	char buf[256];

	util_file_read_line(buf, sizeof(buf), "%s/type", grp_dev);
	util_rec_set(rec, "type", buf);

	util_file_read_line(buf, sizeof(buf), "%s/%s/online", grp_dev, sub_dev);
	if (strcmp(buf, "0") == 0)
		util_rec_set(rec, "online", "offline");
	else
		util_rec_set(rec, "online", "online");

	util_file_read_ul(&facility, 16, "%s/ap_functions", grp_dev);
	if (facility & MASK_COPRO)
		util_rec_set(rec, "mode", "CCA-Coproc");
	else if (facility & MASK_ACCEL)
		util_rec_set(rec, "mode", "Accelerator");
	else if (facility & MASK_EP11)
		util_rec_set(rec, "mode", "EP11-Coproc");
	else
		util_rec_set(rec, "mode", "Unknown");

	util_file_read_line(buf, sizeof(buf), "%s/%s/request_count",
			    grp_dev, sub_dev);
	util_rec_set(rec, "request_count", buf);
}

/*
 * Read subdevice verbose attributes
 */
static void read_subdev_rec_verbose(struct util_rec *rec, const char *grp_dev,
				    const char *sub_dev)
{
	unsigned long facility;
	char buf[256];
	long depth;

	if (l.verbose == 0)
		return;

	util_file_read_line(buf, sizeof(buf), "%s/%s/pendingq_count",
		       grp_dev, sub_dev);
	util_rec_set(rec, "pendingq_count", buf);

	util_file_read_line(buf, sizeof(buf), "%s/%s/requestq_count",
		       grp_dev, sub_dev);
	util_rec_set(rec, "requestq_count", buf);

	util_file_read_line(buf, sizeof(buf), "%s/hwtype", grp_dev);
	util_rec_set(rec, "hwtype", buf);

	util_file_read_l(&depth, 10, "%s/depth", grp_dev);
	util_rec_set(rec, "depth", "%02d", depth + 1);

	util_file_read_ul(&facility, 16, "%s/ap_functions", grp_dev);
	util_rec_set(rec, "facility", "0x%08x", facility);
}

/*
 * Show one subdevice
 */
static void show_subdevice(struct util_rec *rec, const char *grp_dev,
			   const char *sub_dev)
{
	if (!util_path_is_dir("%s/%s", grp_dev, sub_dev))
		errx(EXIT_FAILURE, "error - cryptographic device %s/%s does not exist!", grp_dev, sub_dev);

	/* Skip devices, which are not supported by zcrypt layer */
	if (!util_path_is_readable("%s/type", grp_dev) &&
	    !util_path_is_readable("%s/%s/online", grp_dev, sub_dev))
		return;

	util_rec_set(rec, "card", sub_dev);
	read_subdev_rec_default(rec, grp_dev, sub_dev);
	read_subdev_rec_verbose(rec, grp_dev, sub_dev);

	util_rec_print(rec);
}

/*
 * Show subdevices
 */
static void show_subdevices(struct util_rec *rec, const char *grp_dev)
{
	struct dirent **dev_vec;
	int i, count;

	count = util_scandir(&dev_vec, alphasort, "%s/..\\....", grp_dev);
	if (count == -1)
		errx(EXIT_FAILURE, "error - cryptographic device driver zcrypt is not loaded!");
	for (i = 0; i < count; i++) {
		show_subdevice(rec, grp_dev, dev_vec[i]->d_name);
	}
}

/*
 * Read default attributes
 */
static void read_rec_default(struct util_rec *rec, const char *grp_dev)
{
	unsigned long facility;
	char buf[256];

	util_file_read_line(buf, sizeof(buf), "%s/type", grp_dev);
	util_rec_set(rec, "type", buf);

	util_file_read_ul(&facility, 16, "%s/ap_functions", grp_dev);
	if (facility & MASK_COPRO)
		util_rec_set(rec, "mode", "CCA-Coproc");
	else if (facility & MASK_ACCEL)
		util_rec_set(rec, "mode", "Accelerator");
	else if (facility & MASK_EP11)
		util_rec_set(rec, "mode", "EP11-Coproc");
	else
		util_rec_set(rec, "mode", "Unknown");

	util_file_read_line(buf, sizeof(buf), "%s/online", grp_dev);
	if (strcmp(buf, "0") == 0)
		util_rec_set(rec, "online", "offline");
	else
		util_rec_set(rec, "online", "online");

	util_file_read_line(buf, sizeof(buf), "%s/request_count", grp_dev);
	util_rec_set(rec, "request_count", buf);
}

/*
 * Read verbose attributes
 */
static void read_rec_verbose(struct util_rec *rec, const char *grp_dev)
{
	unsigned long facility;
	char buf[256];
	long depth;

	if (l.verbose == 0)
		return;

	util_file_read_line(buf, sizeof(buf), "%s/pendingq_count", grp_dev);
	util_rec_set(rec, "pendingq_count", buf);

	util_file_read_line(buf, sizeof(buf), "%s/requestq_count", grp_dev);
	util_rec_set(rec, "requestq_count", buf);

	util_file_read_line(buf, sizeof(buf), "%s/hwtype", grp_dev);
	util_rec_set(rec, "hwtype", buf);

	util_file_read_l(&depth, 10, "%s/depth", grp_dev);
	util_rec_set(rec, "depth", "%02d", depth + 1);

	util_file_read_ul(&facility, 16, "%s/ap_functions", grp_dev);
	util_rec_set(rec, "facility", "0x%08x", facility);
}

/*
 * Show device: device is in the form "card00", "card01", ...
 */
static void show_device(struct util_rec *rec, const char *device)
{
	char *grp_dev, card[3];

	util_rec_set(rec, "card", card);

	strcpy(card, &device[4]);
	grp_dev = util_path_sysfs("devices/ap/%s", device);
	if (!util_path_is_dir(grp_dev)) {
		errx(EXIT_FAILURE, "error - cryptographic device %s does not exist!",
			 card);
	}
	/* Skip devices, which are not supported by zcrypt layer */
	if (!util_path_is_readable("%s/type", grp_dev) &&
	    !util_path_is_readable("%s/online", grp_dev)) {
		goto out_free;
	}
	util_rec_set(rec, "card", card);

	read_rec_default(rec, grp_dev);
	read_rec_verbose(rec, grp_dev);

	util_rec_print(rec);
	show_subdevices(rec, grp_dev);
out_free:
	free(grp_dev);
}

/*
 * Define the *default* attributes
 */
static void define_rec_default(struct util_rec *rec)
{
	util_rec_def(rec, "card", UTIL_REC_ALIGN_LEFT, 11, "CARD.DOMAIN");
	util_rec_def(rec, "type", UTIL_REC_ALIGN_LEFT, 5, "TYPE");
	util_rec_def(rec, "mode", UTIL_REC_ALIGN_LEFT, 11, "MODE");
	util_rec_def(rec, "online", UTIL_REC_ALIGN_LEFT, 7, "STATUS");
	util_rec_def(rec, "request_count", UTIL_REC_ALIGN_RIGHT, 11,
		     "REQUEST_CNT");
}

/*
 * Define the *verbose* attributes
 */
static void define_rec_verbose(struct util_rec *rec)
{
	if (l.verbose == 0)
		return;
	util_rec_def(rec, "pendingq_count", UTIL_REC_ALIGN_RIGHT, 12,
		     "PENDINGQ_CNT");
	util_rec_def(rec, "requestq_count", UTIL_REC_ALIGN_RIGHT, 12,
		     "REQUESTQ_CNT");
	util_rec_def(rec, "hwtype", UTIL_REC_ALIGN_RIGHT, 7, "HW_TYPE");
	util_rec_def(rec, "depth", UTIL_REC_ALIGN_RIGHT, 7, "Q_DEPTH");
	util_rec_def(rec, "facility", UTIL_REC_ALIGN_LEFT, 10, "FUNCTIONS");
}

/*
 * Show all devices
 */
static void show_devices_all(void)
{
	struct util_rec *rec = util_rec_new_wide("-");
	struct dirent **dev_vec;
	int i, count;
	char *path;

	/* Define the record */
	define_rec_default(rec);
	define_rec_verbose(rec);

	/* Scan the devices */
	path = util_path_sysfs("devices/ap/");
	count = util_scandir(&dev_vec, alphasort, "%s/card[0-9a-fA-F]+", path);
	if (count == -1)
		errx(EXIT_FAILURE, "error - cryptographic device driver zcrypt is not loaded!");
	util_rec_print_hdr(rec);
	for (i = 0; i < count; i++) {
		show_device(rec, dev_vec[i]->d_name);
	}
	free(path);
}

/*
 * Show devices specified on commandline
 */
static void show_devices_argv(char *argv[])
{
	struct util_rec *rec = util_rec_new_wide("-");
	struct dirent **dev_vec, **subdev_vec;
	char *grp_dev, *path, card[7], sub_dev[7];
	int id, dom, i, n, dev_cnt, sub_cnt;

	/* Define the record */
	define_rec_default(rec);
	define_rec_verbose(rec);

	util_rec_print_hdr(rec);
	for (i = 0; argv[i] != NULL; i++) {
		id = -1;
		dom = -1;
		if (!sscanf(argv[i], "%x.%x", &id, &dom)) {
			if (!sscanf(argv[i], ".%x", &dom)) {
				printf("Invalid adpater id!\n");
				return;
			}
			/* list specific domains of all adapters */
			path = util_path_sysfs("devices/ap/");
			dev_cnt = util_scandir(&dev_vec, alphasort,
					       "%s/card[0-9a-fA-F]+/", path);
			free(path);
			if (dev_cnt == -1)
				errx(EXIT_FAILURE, "error - cryptographic device driver zcrypt is not loaded!");
			for (i = 0; i < dev_cnt; i++) {
				path = util_path_sysfs("devices/ap/%s",
						       dev_vec[i]->d_name);
				sub_cnt = util_scandir(&subdev_vec, alphasort,
						       "%s/[0-9a-fA-F]+.%04x/", path,
						       dom);
				for (n = 0; n < sub_cnt; n++) {
					show_subdevice(rec, path,
						       subdev_vec[n]->d_name);
				}
				free(path);
			}
			return;
		}
		/* at least the id field was valid */
		if (id >= 0 && dom >= 0) {	/* single subdevice */
			sprintf(sub_dev, "%02x.%04x", id, dom);
			grp_dev = util_path_sysfs("devices/ap/card%02x", id);
			show_subdevice(rec, grp_dev, sub_dev);
			free(grp_dev);
		} else {			/* group device */
			sprintf(card, "card%02x", id);
			show_device(rec, card);
		}
	}
}

/*
 * Describe adapter ids
 */
void print_adapter_id_help(void)
{
	printf("\n");
	printf("DEVICE_IDS\n");
	printf("  List of cryptographic device ids separated by blanks which will be displayed.\n");
	printf("  DEVICE_ID could either be card device id ('<card-id>') or queue device id\n");
	printf("  '<card-id>.<domain-id>'). To filter all devices according to a dedicated\n");
	printf("  domain just provide '.<domain-id>'.\n");
	printf("  If no ids are given, all available devices are displayed.\n");
	printf("  \n");
	printf("EXAMPLE:\n");
	printf("  List all cryptographic devices with card id '02'.\n");
	printf("  #>lszcrypt 02\n");
	printf("  \n");
	printf("  List cryptographic devices with card id '02' and domain id '0005'.\n");
	printf("  #>lszcrypt 02.0005\n");
	printf("  \n");
}

/*
 * Entry point
 */
int main(int argc, char **argv)
{
	int c;

	util_prg_init(&prg);
	util_opt_init(opt_vec, NULL);
	while (1) {
		c = util_opt_getopt_long(argc, argv);
		if (c == -1)
			break;
		switch (c) {
		case 'b':
			show_bus();
			return EXIT_SUCCESS;
		case 'c':
			show_capability(optarg);
			return EXIT_SUCCESS;
		case 'd':
			show_domains();
			return EXIT_SUCCESS;
		case 'V':
			l.verbose++;
			break;
		case 'h':
			util_prg_print_help();
			util_opt_print_help();
			print_adapter_id_help();
			return EXIT_SUCCESS;
		case 'v':
			util_prg_print_version();
			return EXIT_SUCCESS;
		default:
			util_opt_print_parse_error(c, argv);
			return EXIT_FAILURE;
		}
	}
	if (optind == argc)
		show_devices_all();
	else
		show_devices_argv(&argv[optind]);
	return EXIT_SUCCESS;
}
