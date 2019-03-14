/*
 * chzcrypt - Tool to modify zcrypt configuration
 *
 * Copyright IBM Corp. 2008, 2016
 */

#include <sys/types.h>
#include <sys/stat.h>

#include <argz.h>
#include <err.h>
#include <errno.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>

#include "util_base.h"
#include "util_path.h"
#include "util_file.h"
#include "util_libc.h"
#include "util_opt.h"
#include "util_panic.h"
#include "util_prg.h"
#include "util_scandir.h"
#include "zt_common.h"

#include "misc.h"

/*
 * Private data
 */
struct chzcrypt_l {
	int verbose;
} l;

struct chzcrypt_l *chzcrypt_l = &l;

/*
 * Program configuration
 */
const struct util_prg prg = {
	.desc	= "Modify zcrypt configuration.",
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
		.option = { "enable", no_argument, NULL, 'e'},
		.argument = "DEVICE_IDS",
		.desc = "Set the given cryptographic device(s) online"
	},
	{
		.option = { "disable", no_argument, NULL, 'd'},
		.argument = "DEVICE_IDS",
		.desc = "Set the given cryptographic device(s) offline",
	},
	{
		.option = { "all", no_argument, NULL, 'a'},
		.desc = "Set all available cryptographic device(s) "
			"online/offline, must be used in conjunction "
			"with the enable or disable option",
	},
	{
		.option = { "poll-thread-enable", no_argument, NULL, 'p'},
		.desc = "Enable zcrypt's poll thread",
	},
	{
		.option = { "poll-thread-disable", no_argument, NULL, 'n'},
		.desc = "Disable zcrypt's poll thread",
	},
	{
		.option = { "config-time", required_argument, NULL, 'c'},
		.argument = "TIMEOUT",
		.desc = "Set configuration timer for re-scanning the AP bus "
			"to TIMEOUT seconds",
	},
	{
		.option = { "poll-timeout", required_argument, NULL, 't'},
		.argument = "TIMEOUT",
		.desc = "Set poll timer to run poll tasklet all TIMEOUT "
			"nanoseconds after a request has been queued",
	},
	{
		.option = { "default-domain", required_argument, NULL, 'q'},
		.argument = "DOMAIN",
		.desc = "Set new default domain to DOMAIN",
	},
	{
		.option = { "verbose", no_argument, NULL, 'V'},
		.desc = "Print verbose messages",
	},
	UTIL_OPT_HELP,
	UTIL_OPT_VERSION,
	UTIL_OPT_END
};

/*
 * Print if verbose is set
 */
#define verbose(x...)			\
do {					\
	if (!l.verbose)			\
		break;			\
	printf(x);			\
} while (0)

/*
 * Set poll settings
 */
static void poll_thread_set(const char *mode_str)
{
	long mode, mode_read = -1;
	char *attr;

	sscanf(mode_str, "%ld", &mode);
	if (mode == 1)
		verbose("Enabling poll thread.\n");
	else
		verbose("Disabling poll thread.\n");
	attr = util_path_sysfs("bus/ap/poll_thread");
	if (!util_path_is_writable(attr))
		errx(EXIT_FAILURE, "error - unsupported version of zcrypt or driver not loaded!");
	util_file_write_l(mode, 10, attr);
	util_file_read_l(&mode_read, 10, attr);
	if (mode != mode_read)
		errx(EXIT_FAILURE, "error - unable change poll thread settings!");
	free(attr);
}

/*
 * Set timer
 */
static void config_time_set(const char *timeout_str)
{
	long timeout, timeout_read;
	char *attr;

	if (sscanf(timeout_str, "%ld", &timeout) != 1) {
		errx(EXIT_FAILURE, "error - invalid configuration timeout '%s'!",
			 timeout_str);
	}
	attr = util_path_sysfs("bus/ap/config_time");
	verbose("Setting configuration timer to %ld seconds.\n", timeout);
	if (!util_path_is_writable(attr))
		errx(EXIT_FAILURE, "error - unsupported version of zcrypt or driver not loaded!");
	util_file_write_l(timeout, 10, attr);
	util_file_read_l(&timeout_read, 10, attr);
	if (timeout != timeout_read)
		errx(EXIT_FAILURE, "error - unable to change configuration timer to %ld seconds!", timeout);
	free(attr);
}

/*
 * Set poll timeout
 */
static void poll_timeout_set(const char *poll_timeout_str)
{
	long poll_timeout, poll_timeout_read;
	char *attr;

	if (sscanf(poll_timeout_str, "%ld", &poll_timeout) != 1) {
		errx(EXIT_FAILURE, "error - invalid poll timeout '%s'!",
			 poll_timeout_str);
	}
	attr = util_path_sysfs("bus/ap/poll_timeout");
	verbose("Setting poll timeout to %ld seconds.\n", poll_timeout);
	if (!util_path_is_writable(attr))
		errx(EXIT_FAILURE, "error - unsupported version of zcrypt or driver not loaded!");
	util_file_write_l(poll_timeout, 10, attr);
	util_file_read_l(&poll_timeout_read, 10, attr);
	if (poll_timeout != poll_timeout_read)
		errx(EXIT_FAILURE, "error - unable to change poll_timeout to %ld seconds!", poll_timeout);
	free(attr);
}

/*
 * Set default domain
 */
static void default_domain_set(const char *default_domain_str)
{
	long max_dom, default_domain, default_domain_read;
	char *attr, *ap_max_domain_id;

	sscanf(default_domain_str, "%li", &default_domain);
	ap_max_domain_id = util_path_sysfs("bus/ap/ap_max_domain_id");
	util_file_read_l(&max_dom, 10, ap_max_domain_id);
	if (default_domain < 0 || default_domain > max_dom) {
		errx(EXIT_FAILURE, "error - invalid default domain '%s'!",
			 default_domain_str);
	}
	attr = util_path_sysfs("bus/ap/ap_domain");
	verbose("Setting default domain to %ld.\n", default_domain);
	if (!util_path_is_writable(attr))
		errx(EXIT_FAILURE, "error - unsupported version of zcrypt or driver not loaded!");
	util_file_write_l(default_domain, 10, attr);
	util_file_read_l(&default_domain_read, 10, attr);
	if (default_domain != default_domain_read)
		errx(EXIT_FAILURE, "error - unable to change default domain to %ld!", default_domain);
	free(ap_max_domain_id);
	free(attr);
}

/*
 * Print invalid commandline error message and then exit with error code
 */
#define invalid_cmdline_exit(x...)			\
do {							\
	fprintf(stderr, "%s: ", program_invocation_short_name);	\
	fprintf(stderr, x);				\
	util_prg_print_parse_error();			\
	exit(EXIT_FAILURE);				\
} while (0)

/*
 * Get device list from sysfs
 */
static void dev_list_all(char **argz, size_t *len)
{
	struct dirent **de_vec;
	int count, i;
	char *path;

	path = util_path_sysfs("bus/ap/devices/");
	count = util_scandir(&de_vec, NULL, "%s/card.*", path);
	if (count < 0)
		errx(EXIT_FAILURE, "Could not read directory: %s", path);
	*argz = NULL;
	*len = 0;
	for (i = 0; i < count; i++)
		util_assert(argz_add(argz, len, de_vec[i]->d_name) == 0,
			    "Out of memory\n");
	util_scandir_free(de_vec, count);
	free(path);
}

/*
 * Get device list from commandline
 */
static void dev_list_argv(char **argz, size_t *len, char * const argv[])
{
	if (argv[0] == NULL)
		errx(EXIT_FAILURE, "Need to specify at least one device ID");

	util_assert(argz_create(argv, argz, len) == 0, "Out of memory\n");
}

/*
 * Describe adapter ids
 */
void print_adapter_id_help(void)
{
	printf("\n");
	printf("DEVICE_IDS\n");
	printf("  List of cryptographic device ids separated by blanks which will be set\n");
	printf("  online/offline. Must be used in conjunction with the enable or disable option.\n");

	printf("  DEVICE_ID could either be card device id ('<card-id>') or queue device id\n");
	printf("  '<card-id>.<domain-id>').\n");
	printf("  \n");
	printf("EXAMPLE:\n");
	printf("  Disable the cryptographic device with card id '02' (inclusive all queues).\n");
	printf("  #>chzcrypt -d 02\n");
	printf("  \n");
	printf("  Enable the cryptographic devices with card id '03' and domain id '0005'.\n");
	printf("  #>chzcrypt -e 03.0005\n");
	printf("  \n");

}

/*
 * Parse options and execute the command
 */
int main(int argc, char *argv[])
{
	const char *online, *online_text = NULL, *poll_thread, *config_time;
	const char *poll_timeout, *default_domain;
	char *path, *dev_path, *dev, *dev_list, device[256], online_read[2];
	bool all = false, actionset = false;
	size_t len;
	int id, dom, c, i, j;

	for (i=0; i < argc; i++)
		for (j=2; j < (int) strlen(argv[i]); j++)
			if (argv[i][j] == '_')
				argv[i][j] = '-';

	online = poll_thread = config_time = poll_timeout = default_domain = NULL;
	util_prg_init(&prg);
	util_opt_init(opt_vec, NULL);
	while (1) {
		c = util_opt_getopt_long(argc, argv);
		if (c == -1)
			break;
		switch (c) {
		case 'e':
			actionset = true;
			online = "1";
			online_text = "online";
			break;
		case 'd':
			actionset = true;
			online = "0";
			online_text = "offline";
			break;
		case 'a':
			all = true;
			break;
		case 'p':
			actionset = true;
			poll_thread = "1";
			break;
		case 'n':
			actionset = true;
			poll_thread = "0";
			break;
		case 'c':
			actionset = true;
			config_time = optarg;
			break;
		case 't':
			actionset = true;
			poll_timeout = optarg;
			break;
		case 'q':
			actionset = true;
			default_domain = optarg;
			break;
		case 'V':
			l.verbose = true;
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
	if (!actionset)
		invalid_cmdline_exit("error - missing argument!\n");
	path = util_path_sysfs("bus/ap");
	if (!util_path_is_dir(path))
		errx(EXIT_FAILURE, "error - cryptographic device driver zcrypt is not loaded!");
	free(path);
	if (poll_thread) {
		poll_thread_set(poll_thread);
		return EXIT_SUCCESS;
	}
	if (config_time) {
		config_time_set(config_time);
		return EXIT_SUCCESS;
	}
	if (poll_timeout) {
		poll_timeout_set(poll_timeout);
		return EXIT_SUCCESS;
	}
	if (default_domain) {
		default_domain_set(default_domain);
		return EXIT_SUCCESS;
	}
	if (all)
		dev_list_all(&dev_list, &len);
	else
		dev_list_argv(&dev_list, &len, &argv[optind]);

	if (online && len == 0)
		errx(EXIT_FAILURE, "error - missing cryptographic device id(s)!");

	for (dev = dev_list; dev != NULL; dev = argz_next(dev_list, len, dev)) {
		if (strncmp(dev, "card", 4) == 0) {
			/* dev == "card2" */
			sscanf(dev, "card%02x", &id);
			sprintf(device, "card%02x", id);
		} else if (strncmp(dev, "0x", 2) == 0) {
			/* dev == "0x.." */
			sscanf(dev, "0x%02x", &id);
			sprintf(device, "card%02x", id);
		} else if (misc_regex_match(dev, "^[0-9a-fA-F]+$")) {
			/* dev == "2" */
			sscanf(dev, "%02x", &id);
			sprintf(device, "card%02x", id);
		} else {
			/* Form: 01.0003 ? */
			if (sscanf(dev, "%02x.%04x", &id, &dom) != 2)
				errx(EXIT_FAILURE, "error - cryptographic device %s malformed!\n", dev);
			sprintf(device, "card%02x/%02x.%04x", id, id, dom);
		}
		dev_path = util_path_sysfs("bus/ap/devices/%s", device);
		if (!util_path_is_dir(dev_path))
			errx(EXIT_FAILURE, "error - cryptographic device %s does not exist!", device);
		if (!util_path_is_writable("%s/online", dev_path))
			continue;
		verbose("Setting cryptographic device %s %s\n", device, online_text);
		util_file_write_s(online, "%s/online", dev_path);
		util_file_read_line(online_read, sizeof(online_read), "%s/online", dev_path);
		if (strcmp(online, online_read) != 0)
			errx(EXIT_FAILURE, "error - unable to set cryptographic device %s %s", device, online_text);
		free(dev_path);
	}
	free(dev_list);
	return EXIT_SUCCESS;
}
