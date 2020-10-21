/* error_functions.c
 *
 * Some standard error handling routines used by various programs.
 */

#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "ename.c.inc"
#include "error_functions.h"

#define BUF_SIZE 500

static void
terminate(int use_exit)
{
	char *s;

	s = getenv("EF_DUMPCORE");

	if (s != NULL && *s != '\0')
		abort();
	else if (use_exit)
		exit(EXIT_FAILURE);
	else
		_exit(EXIT_FAILURE);
}

static void
output_error(int use_err, int err, int flush_stdout, const char *format, va_list ap)
{
	char buf[BUF_SIZE];
	char user_msg[BUF_SIZE];
	char err_text[BUF_SIZE];

	vsprintf(user_msg, format, ap);

	if (use_err) {
		sprintf(err_text, " [%s %s]",
			(err > 0 && err <= MAX_ENAME)
			? ename[err]
			: "?UNKNOWN?",
			strerror(err));
	} else {
		sprintf(err_text, ":");
	}

	sprintf(buf, "ERROR%s %s\n", err_text, user_msg);

	if (flush_stdout)
		fflush(stdout);
	fputs(buf, stderr);
	fflush(stderr);
}

void
err_msg(const char *format, ...)
{
	va_list arg_list;
	int saved_errno;

	saved_errno = errno;

	va_start(arg_list, format);
	output_error(1, errno, 1, format, arg_list);
	va_end(arg_list);

	errno = saved_errno;
}

void
err_exit(const char *format, ...)
{
	va_list arg_list;

	va_start(arg_list, format);
	output_error(1, errno, 0, format, arg_list);
	va_end(arg_list);

	terminate(0);
}

void
fatal(const char *format, ...)
{
	va_list arg_list;

	va_start(arg_list, format);
	output_error(0, 0, 1, format, arg_list);
	va_end(arg_list);

	terminate(1);
}
