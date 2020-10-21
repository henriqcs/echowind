/* error_functions.h
 *
 * Header file for error_functions.c
 */

#ifndef ERROR_FUNCTIONS_H_
#define ERROR_FUNCTIONS_H_

void err_msg(const char *format, ...);
void err_exit(const char *format, ...);
void fatal(const char *format, ...);

#endif
