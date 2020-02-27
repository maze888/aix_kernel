/**
	* MagicCrypto Kernel Module Loader for AIX
*/

#include "mc_kernel_loader_util.h"

static void usage()
{
	printf("Dreamsecurity MagicCrypto Kernel Extension Loader for AIX\n");
	printf("  Copyright (c) 2020 Dreamsecurity Corporation\n\n");
	printf("Usage: mc_kernel_loader <ctl_opt> [<kernex object>]\n\n");
	printf("where ctl_opt can be any of:\n");
	printf("  -l: to load the kernel ext(<kernex object>)\n");
	printf("  -u: to unload the kernel ext(<kernex object>)\n");
	printf("  -q: to query the kernel ext(<kernex object>)\n\n");
	
	exit(0);
}

static int mc_load(const char *path)
{
	struct cfg_load cfg_load;

	if ( !path ) {
		fprintf(stderr, "invalid argument: path is null\n");
		return -1;
	}

	memset(&cfg_load, 0x00, sizeof(cfg_load));
	
	cfg_load.path = path;
	cfg_load.libpath = NULL; /* no library */
	cfg_load.kmid = 0;

	if ( access(path, F_OK) < 0 ) {
		fprintf(stderr, "can't access kernel extention binary: (path: %s, errmsg: %s, errno: %d)\n", path, strerror(errno), errno);
		return -1;
	}
	
	if ( sysconfig(SYS_QUERYLOAD, &cfg_load, sizeof(cfg_load)) < 0 ) {
		fprintf(stderr, "kernel extention query fail: (path: %s, errmsg: %s, errno: %d)\n", path, strerror(errno), errno);
		return -1;
	}
	
	if ( cfg_load.kmid > 0 ) {
		printf("%s already running.\n", path);
		return 0;
	}
	
	if ( sysconfig(SYS_SINGLELOAD, &cfg_load, sizeof(cfg_load)) < 0 ) {
		fprintf(stderr, "kernel extention load fail: (path: %s, errmsg: %s, errno: %d)\n", path, strerror(errno), errno);
		return -1;
	}

	return 0;
}

static int mc_unload(const char *path)
{
	int rv;
	struct cfg_load cfg_load;
	
	if ( !path ) {
		fprintf(stderr, "invalid argument: path is null\n");
		return -1;
	}

	memset(&cfg_load, 0x00, sizeof(cfg_load));
	
	cfg_load.path = path;
	cfg_load.libpath = NULL; /* no library */
	cfg_load.kmid = 0;
	
	if ( access(path, F_OK) < 0 ) {
		fprintf(stderr, "can't access kernel extention binary: (path: %s, errmsg: %s, errno: %d)\n", path, strerror(errno), errno);
		return -1;
	}
	
	if ( sysconfig(SYS_QUERYLOAD, &cfg_load, sizeof(cfg_load)) < 0 ) {
		fprintf(stderr, "kernel extention query fail: (path: %s, errmsg: %s, errno: %d)\n", path, strerror(errno), errno);
		return -1;
	}

	if ( cfg_load.kmid == 0 ) {
		printf("%s already removed. (or not exist)\n", path);
		return 0;
	}

	if ( sysconfig(SYS_KULOAD, &cfg_load, sizeof(struct cfg_load)) < 0 ) {
		fprintf(stderr, "kernel extention unload fail: (path: %s, errmsg: %s, errno: %d)\n", path, strerror(errno), errno);
		return -1;
	}

	return 0;
}

static mid_t mc_query(const char *path)
{
	struct cfg_load cfg_load;
	
	if ( !path ) {
		fprintf(stderr, "invalid argument: path is null\n");
		return -1;
	}

	memset(&cfg_load, 0x00, sizeof(cfg_load));
	
	cfg_load.path = path;
	cfg_load.libpath = NULL; /* no library */
	cfg_load.kmid = 0;

	if ( access(path, F_OK) < 0 ) {
		fprintf(stderr, "can't access kernel extention binary: (path: %s, errmsg: %s, errno: %d)\n", path, strerror(errno), errno);
		return -1;
	}
	
	if ( sysconfig(SYS_QUERYLOAD, &cfg_load, sizeof(cfg_load)) < 0 ) {
		fprintf(stderr, "kernel extention unload fail: (path: %s, errmsg: %s, errno: %d)\n", path, strerror(errno), errno);
		return -1;
	}
	
	if ( cfg_load.kmid == 0 ) {
		printf("%s already removed. (or not exist)\n", path);
		return 0;
	}

	return cfg_load.kmid;
}

int main(int argc, char **argv)
{
	int opt;
	mid_t kmid;

	if ( getuid() != 0 ) {
		fprintf(stderr, "must be root\n");
		goto out;
	}

	opt = getopt(argc, argv, "m:l:u:q:k:h");
	switch(opt) {
		case 'l':
			if ( mc_load(optarg) < 0 ) {
				fprintf(stderr, "mc_load() is failed\n");
				goto out;
			}
			break;
		case 'u':
			if ( mc_unload(optarg) < 0 ) {
				fprintf(stderr, "mc_unload() is failed\n");
				goto out;
			}
			break;
		case 'q':
			if ( (kmid = mc_query(optarg)) < 0 ) {
				fprintf(stderr, "mc_query() is failed\n");
				goto out;
			}
			if ( kmid > 0 ) printf("%s id: %d\n", optarg, kmid);
			break;
		case 'h':
		default:
			usage();
	}

#if 0
			case 'i': /* Initialize a KernExt */
				/* Initialize  the kernel extension  */
				opt_kmod.kmid = opt_load.kmid;
				opt_kmod.cmd = CFG_INIT;
				opt_kmod.mdiptr = NULL;
				opt_kmod.mdilen = 0;
				if (sysconfig(SYS_CFGKMOD,&opt_kmod,sizeof(struct cfg_kmod))) {
					perror("sysconfig(SYS_CFGKMOD)");  /* print error message */
				}
				else {
					printf(" Extension Initialized \n");
				}
				
				break;
			case 't': /* Terminate the kernel extension */
				/* Check if KernExt is loaded */
				if (opt_load.kmid == 0) {
					fprintf(stderr, "Extension not loaded\n");
				}
				else {
					opt_kmod.kmid = opt_load.kmid;
					opt_kmod.cmd  = CFG_TERM;    /* Terminate  the kernel extension */
					opt_kmod.mdiptr = NULL;
					opt_kmod.mdilen = 0;
					if (sysconfig(SYS_CFGKMOD,&opt_kmod,sizeof(struct  cfg_kmod)))
						perror("sysconfig(SYS_CFGKMOD)");  /* print error */
					else
						fprintf(stderr, " KernExtension  Terminated \n");
				}
				break;
	}
#endif

	return 0;

out:
	return 1;
}
