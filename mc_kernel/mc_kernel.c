/**
	* AIX MagicCrypto Kernel Module 
*/

#include <sys/device.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/errno.h>
#include <sys/syslog.h>
#include <sys/proc.h>

int test_call(int arg)
{
	return 0;
}

static void curr_rusage()
{
	/*struct rusage u;*/
	struct rusage64 u;

	memset(&u, 0x00, sizeof(u));

	/*
	if ( getrusage64(RUSAGE_THREAD, &u) != 0 ) {
		bsdlog(LOG_DEBUG | LOG_KERN, "getrusage() is failed: %d\n", getuerror());
		return;
	}
	*/
	
	bsdlog(LOG_DEBUG | LOG_KERN, "ru_utime(usec): %ld\n", u.ru_utime.tv_usec);
	bsdlog(LOG_DEBUG | LOG_KERN, "ru_stime(usec): %ld\n", u.ru_stime.tv_usec);
	bsdlog(LOG_DEBUG | LOG_KERN, "ru_maxrss:      %ld\n", u.ru_maxrss);
	bsdlog(LOG_DEBUG | LOG_KERN, "ru_ixrss:       %ld\n", u.ru_ixrss);
	bsdlog(LOG_DEBUG | LOG_KERN, "ru_idrss:       %ld\n", u.ru_idrss);
	bsdlog(LOG_DEBUG | LOG_KERN, "ru_minflt:      %ld\n", u.ru_minflt);
	bsdlog(LOG_DEBUG | LOG_KERN, "ru_nswap:       %ld\n", u.ru_nswap);
	bsdlog(LOG_DEBUG | LOG_KERN, "ru_inblock:     %ld\n", u.ru_inblock);
	bsdlog(LOG_DEBUG | LOG_KERN, "ru_oublock:     %ld\n", u.ru_oublock);
	bsdlog(LOG_DEBUG | LOG_KERN, "ru_msgsnd:      %ld\n", u.ru_msgsnd);
	bsdlog(LOG_DEBUG | LOG_KERN, "ru_msgrcv:      %ld\n", u.ru_msgrcv);
	bsdlog(LOG_DEBUG | LOG_KERN, "ru_nsignals:    %ld\n", u.ru_nsignals);
	bsdlog(LOG_DEBUG | LOG_KERN, "ru_nvcsw:       %ld\n", u.ru_nvcsw);
	bsdlog(LOG_DEBUG | LOG_KERN, "ru_nivcsw:      %ld\n", u.ru_nivcsw);
}

static void curr_time()
{
	struct timestruc_t ts;
	
	memset(&ts, 0x00, sizeof(ts));

	curtime(&ts);

	bsdlog(LOG_DEBUG | LOG_KERN, "ts sec: %d\n", ts.tv_nsec);
	bsdlog(LOG_DEBUG | LOG_KERN, "lbolt:  %d\n", lbolt);
}

static void entropy_test()
{
	curr_time();
	curr_rusage();
}

int mc_kernel_init(int cmd, struct uio *uio)
{
	bsdlog(LOG_DEBUG | LOG_KERN, "Enter hello_init:: command = 0x%x \n", cmd);

	if (cmd == CFG_INIT) {
		entropy_test();
	}
	else if (cmd == CFG_TERM) {
		bsdlog(LOG_DEBUG | LOG_KERN, " Terminating Hello World   KernExt \n");
	}
	else {
		bsdlog(LOG_DEBUG | LOG_KERN, " Unknown command to  Adv  KernExt: %d \n", cmd);
	}

	return 0;
}
