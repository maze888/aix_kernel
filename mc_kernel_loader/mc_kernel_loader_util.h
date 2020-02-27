#ifndef _MC_KERNEL_LOADER_UTIL_H_
#define _MC_KERNEL_LOADER_UTIL_H_

#include  <stdio.h>
#include  <stdlib.h>
#include  <unistd.h>
#include  <string.h>
#include  <errno.h>
#include  <sys/types.h>
#include  <sys/stat.h>
#include  <sys/shm.h>
#include  <sys/device.h>
#include  <sys/sysconfig.h>

// to shared memory
int save_mc_kernel_id(const char *path, mid_t mc_kernel_id);
int load_mc_kernel_id(const char *path, mid_t *mc_kernel_id);

#endif
