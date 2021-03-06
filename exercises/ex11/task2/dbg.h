/**
 * Jonas' debug macros
 * all messages print to stderr - stream
 */

#ifndef __dbg_h__
#define __dbg_h__

#include <errno.h>
#include <stdio.h>
#include <string.h>

// colors
#define KRED "\x1B[31m"
#define KGRN "\x1B[32m"
#define KYEL "\x1B[33m"
#define KBLU "\x1B[36m"
// used to reset to 'normal' console color after print
#define RESET "\x1B[0m"

#ifdef NDEBUG
#define debug(M, ...)
#define debug_multi(M, ...)
#else
#define debug(M, ...) \
	fprintf(stderr, "[DEBUG] %s:%d:%s: " M "\n" RESET, __FILE__, __LINE__, __PRETTY_FUNCTION__, \
	        ##__VA_ARGS__)
#define debug_multi(M, ...) \
	fprintf(stderr, \
	        "[DEBUG] pid:" KYEL "%d" RESET " ppid:" KYEL "%d" RESET " %s:%d:%s: " M "\n" RESET, \
	        getpid(), getppid(), __FILE__, __LINE__, __PRETTY_FUNCTION__, ##__VA_ARGS__)
#endif
/**
 * Prints a clean error from errno
 * or 'None' if none exists
 */
#define clean_errno() (errno == 0 ? "None" : strerror(errno))

/**
 * prints a formatted error message to stderr
 */
#define log_err(M, ...) \
	fprintf(stderr, KRED "[ERROR] errno: '%s': " M "\n" RESET, clean_errno(), ##__VA_ARGS__)

/**
 * prints a formatted warn message to stderr
 */
#define log_warn(M, ...) \
	fprintf(stderr, KYEL "[WARN] errno: '%s': " M "\n" RESET, clean_errno(), ##__VA_ARGS__)

#define log_info(M, ...) fprintf(stdout, KBLU "[INFO] " M "\n" RESET, ##__VA_ARGS__)

#define log_success(M, ...) fprintf(stdout, KGRN "[SUCCESS] " M "\n" RESET, ##__VA_ARGS__)

/**
 * checks a condition and prints an error message and jumps to 'error' label if the condition is not
 * met
 */
#define check(A, M, ...) \
	if (!(A)) { \
		log_err(M, ##__VA_ARGS__); \
		errno = 0; \
		goto error; \
	}

/**
 * checks a condition and prints a debug message and jumps to 'continue' label if the condition is
 * not met handy for recursion or loops
 */
#define check_continue(A, M, ...) \
	if (!(A)) { \
		log_info(M, ##__VA_ARGS__); \
		goto cont; \
	}

/**
 * logs an error message and jumps to 'error' label
 */
#define sentinel(M, ...) \
	{ \
		log_err(M, ##__VA_ARGS__); \
		errno = 0; \
		goto error; \
	}

/**
 * check for NULL-pointer
 */
#define check_mem(A) check((A), "Out of memory")

/**
 * checks a condition and if it fails, create a debug log then jump to 'error' label
 */
#define check_debug(A, M, ...) \
	if (!(A)) { \
		debug(M, ##__VA_ARGS__); \
		errno = 0; \
		goto error; \
	}

#endif
