#include <stdlib.h>
#include <string.h>
#include <libgen.h>
#include <pthread.h>
#include "util.h"
#include "util/log.h"

/*
 * This is safe and easy but needs to be optimized
*/

static pthread_spinlock_t _breakuplock;

void breakup_init()
{
	int r = pthread_spin_init(&_breakuplock, 0);
	if (r != 0)
		log_warn("lock not acquired: %d", r);
}

void breakup_path(const char* _path, char** dir, char** base)
{
	char *path, *tmp;

	path = strdup(_path);

	pthread_spin_lock(&_breakuplock);

	tmp  = dirname(path);
	*dir = strdup(tmp);

	strcpy(path, _path);
	tmp   = basename(path);
	*base = strdup(tmp);

	pthread_spin_unlock(&_breakuplock);

	free(path);
}

