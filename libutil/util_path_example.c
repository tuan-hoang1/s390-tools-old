/**
 * util_path_example - Example program for util_path
 *
 * Copyright IBM Corp. 2016
 */

//! [code]
#include <dirent.h>
#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "util_path.h"
#include "util_prg.h"

/*
 * Define program description
 */
const struct util_prg prg = {
	.desc	= "Sample for util_dirlibrary",
	.copyright_vec = {
		{
			.owner = "IBM Corp.",
			.pub_first = 2001,
			.pub_last = 2016,
		},
		UTIL_PRG_COPYRIGHT_END
	}
};

/*
 * Test util_path_sysfs()
 */
static void test_util_path_sysfs(void)
{
	const char * const subsys_vec[] = {"cpu", "memory"};
	char *path;
	int i;

	/* Construct sysfs paths for the subsystems */
	for (i = 0; i < 2; i++) {
		path = util_path_sysfs("devices/system/%s", subsys_vec[i]);
		printf("Path for %6s: \"%s\"\n", subsys_vec[i], path);
		free(path);
	}
}

/*
 * Test path
 */
static void  test_path(const char *path)
{
	printf("%-20s: ", path);
	if (util_path_is_readable(path))
		printf("read=yes, ");
	else
		printf("read=no , ");
	if (util_path_is_writable(path))
		printf("write=yes, ");
	else
		printf("write=no , ");
	if (util_path_is_reg_file(path))
		printf("reg_file=yes");
	else
		printf("reg_file=no ");
	if (util_path_is_dir(path))
		printf("dir=yes");
	else
		printf("dir=no ");
	printf("\n");
}


/*
 * Test util_path_is_xxx()
 */
static void test_util_path_is_xxx(void)
{
	test_path("util_path_example.c");
	test_path("/tmp");
	test_path("i_do_not_exist");
}

/*
 * Usage: util_path_example [sysfs mount point] | "is_xxx"
 */
int main(int argc, char *argv[])
{
	util_prg_init(&prg);

	if (argc < 2)
		goto out_fail;
	if (strcmp(argv[1], "sysfs") == 0) {
		if (argc == 3) {
			/* Change sysfs path via SYSFS_ROOT env variable */
			setenv("SYSFS_ROOT", argv[2], 1);
		}
		test_util_path_sysfs();
	} else if (strcmp(argv[1], "is_xxx") == 0) {
		test_util_path_is_xxx();
	} else {
		goto out_fail;
	}
	return EXIT_SUCCESS;
out_fail:
	errx(EXIT_FAILURE, "Usage: %s sysfs <path> | is_xxx", argv[0]);
}
//! [code]
