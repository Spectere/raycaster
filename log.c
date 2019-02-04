/* log.c - Application logging functionality. */

#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#include "defs.h"
#include "log.h"

void lprint(log_level_t level, const char *format, ...) {
	char output_format[LOG_LEN];

	va_list args;
	va_start(args, format);

	if(strlen(format) + 10 > LOG_LEN) {
		fprintf(stderr, "[!!!] Log string is too long!\n");
		vfprintf(stdout, format, args);
		fprintf(stdout, "\n");
		va_end(args);
		return;
	}

	switch(level) {
		case DEBUG:
			strcpy(output_format, "[DEBUG] ");
			break;
		case INFO:
			strcpy(output_format, "[INFO] ");
			break;
		case WARN:
			strcpy(output_format, "[WARN] ");
			break;
		case ERROR:
			strcpy(output_format, "[ERROR] ");
			break;
		default:
			/* If no valid severity was passed, initialize the string. */
			strcpy(output_format, "");
			break;
	}

	strcat(output_format, format);
	strcat(output_format, "\n");

	if(level == ERROR)
		vfprintf(stderr, output_format, args);
	else
		vfprintf(stdout, output_format, args);

	va_end(args);
}
