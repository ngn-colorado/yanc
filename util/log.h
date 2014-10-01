/**
 * yanc: Yet Another Network Controller
 *
 * Copyright 2013 Matt Monaco <matthew.monaco@colorado.edu>
 *
 * License TBD
 *
**/

#ifndef _YANC_LOG_H_
#define _YANC_LOG_H_

#include <syslog.h>

#define log_line(fmt, args...) \
	syslog(LOG_DEBUG, "%s:%s():%d: " fmt, \
	__FILE__, __func__, __LINE__, ## args)

#define log_debug(fmt, args...) \
	syslog(LOG_DEBUG, fmt, ## args)

#define log_info(fmt, args...) \
	syslog(LOG_INFO, fmt, ## args)

#define log_notice(fmt, args...) \
	syslog(LOG_NOTICE, fmt, ## args)

#define log_warn(fmt, args...) \
	syslog(LOG_ERR, fmt, ## args)

#define log_err(fmt, args...) \
	syslog(LOG_ERR, fmt, ## args)

#define log(lvl, fmt, args...) \
	syslog(lvl, ftm, ## args)

#define log_init(lvl) \
	openlog(NULL,LOG_PERROR,LOG_DAEMON); \
	setlogmask(LOG_UPTO(lvl))

#endif /*_YANC_LOG_H_*/
