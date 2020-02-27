#include "mc_kernel_loader_util.h"

#define MC_KERNEL_ID_SIZE (sizeof(mid_t))

int save_mc_kernel_id(const char *path, mid_t mc_kernel_id)
{
	int shmid;
	key_t key;
	void *p;

	if ( (key = ftok(path, path[0])) < 0 ) {
		fprintf(stderr, "ftok() is failed: (path: %s, errmsg: %s, errno: %d\n", path, strerror(errno), errno);
		goto out;
	}

	if ( (shmid = shmget(key, MC_KERNEL_ID_SIZE, IPC_CREAT|IPC_EXCL|0600)) < 0 ) {
		if ( errno != EEXIST ) {
			fprintf(stderr, "shmget() is failed: (key: %d, size: %d, flag: IPC_CREATE|IPC_EXCL|0600, errmsg: %s, errno: %d)\n", key, MC_KERNEL_ID_SIZE, strerror(errno), errno);
			goto out;
		}

		if ( (shmid = shmget(key, MC_KERNEL_ID_SIZE, 0)) < 0 ) {
			fprintf(stderr, "shmget() is failed: (key: %d, size: %d, flag: 0, errmsg: %s, errno: %d)\n", key, MC_KERNEL_ID_SIZE, strerror(errno), errno);
			goto out;
		}
	}

	if ( (p = shmat(shmid, 0, 0)) == (char *)(-1) ) {
		fprintf(stderr, "shmat() is failed: (errmsg: %s, errno: %d)\n", strerror(errno), errno);
		goto out;
	}

	memcpy(p, &mc_kernel_id, sizeof(mid_t));
	
	if ( shmdt(p) < 0 ) {
		// 이 경우는 발생할 수 없지만, 혹시 모르니 출력한다.
		fprintf(stderr, "shmdt() is failed: (errmsg: %s, errno: %d)\n", strerror(errno), errno);
	}

	return 0;

out:
	return -1;
}

int load_mc_kernel_id(const char *path, mid_t *mc_kernel_id)
{
	int shmid;
	key_t key;
	void *p;

	if ( (key = ftok(path, path[0])) < 0 ) {
		fprintf(stderr, "ftok() is failed: (path: %s, errmsg: %s, errno: %d\n", path, strerror(errno), errno);
		goto out;
	}

	if ( (shmid = shmget(key, MC_KERNEL_ID_SIZE, 0)) < 0 ) {
		fprintf(stderr, "shmget() is failed: (key: %d, size: %d, flag: 0, errmsg: %s, errno: %d)\n", key, MC_KERNEL_ID_SIZE, strerror(errno), errno);
		goto out;
	}

	if ( (p = shmat(shmid, 0, 0)) == (char *)(-1) ) {
		fprintf(stderr, "shmat() is failed: (errmsg: %s, errno: %d)\n", strerror(errno), errno);
		goto out;
	}

	memcpy(mc_kernel_id, p, sizeof(mid_t));
	
	if ( shmdt(p) < 0 ) {
		// 이 경우는 발생할 수 없지만, 혹시 모르니 출력한다.
		fprintf(stderr, "shmdt() is failed: (errmsg: %s, errno: %d)\n", strerror(errno), errno);
	}
	
	return 0;

out:
	return -1;
}
