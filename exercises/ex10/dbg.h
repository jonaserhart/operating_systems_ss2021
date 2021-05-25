#ifndef __dbg_h__
#define __dbg_h__

#include <errno.h>
#include <stdio.h>
#include <string.h>

#define KRED "\x1B[31m"
#define KGRN "\x1B[32m"
#define KYEL "\x1B[33m"
#define KBLU "\x1B[34m"
#define RESET "\x1B[0m"

#ifdef NDEBUG
#define debug(M, ...)
#else
#define debug(M, ...) \
	fprintf(stderr, KBLU "DEBUG %s:%d:%s: " M "\n" RESET, __FILE__, __LINE__, __PRETTY_FUNCTION__, \
	        ##__VA_ARGS__)
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
	fprintf(stderr, KRED "[ERROR] (%s:%d:%s: errno: %s) " M "\n", __FILE__, __LINE__, \
	        __PRETTY_FUNCTION__, clean_errno(), ##__VA_ARGS__)

/**
 * prints a formatted warn message to stderr
 */
#define log_warn(M, ...) \
	fprintf(stdout, KYEL "[WARN] (%s:%d:%s: errno: %s) " M "\n" RESET, __FILE__, __LINE__, \
	        __PRETTY_FUNCTION__, clean_errno(), ##__VA_ARGS__)

#define log_info(M, ...) \
	fprintf(stdout, KBLU "[INFO] (%s:%d:%s) " M "\n" RESET, __FILE__, __LINE__, \
	        __PRETTY_FUNCTION__, ##__VA_ARGS__)

#define check(A, M, ...) \
	if (!(A)) { \
		log_err(M, ##__VA_ARGS__); \
		errno = 0; \
		goto error; \
	}

#define check_continue(A, M, ...) \
	if (!(A)) { \
		log_info(M, ##__VA_ARGS__); \
		goto cont; \
	}

#define sentinel(M, ...) \
	{ \
		log_err(M, ##__VA_ARGS__); \
		errno = 0; \
		goto error; \
	}

#define check_mem(A) check((A), "Out of memory")

#define check_debug(A, M, ...) \
	if (!(A)) { \
		debug(M, ##__VA_ARGS__); \
		errno = 0; \
		goto error; \
	}

#endif
