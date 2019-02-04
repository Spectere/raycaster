/* log.h - Application logging functionality. */

#ifndef _LOG_H
#define _LOG_H

/* Severity levels used by lprint(). */
typedef enum {
	DEBUG,
	INFO,
	WARN,
	ERROR
} log_level_t;

void lprint(log_level_t level, const char *format, ...);

#endif /* _LOG_H */
