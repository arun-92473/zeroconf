/*
 * Copyright (C) cozybit, Inc. 2007-2010. All Rights Reserved.
 *
 * Use and redistribution subject to licensing terms.
 *
 * mdns_port.h: porting layer for mdns
 *
 * The porting layer is comprised of functions that must be implemented and
 * linked with mdns.  Various compiler and system defines are also implemented
 * in this file.  Feel free to expand these to work with you system.
 */

#ifndef __MDNS_PORT_H__
#define __MDNS_PORT_H__

/* system-dependent definitions */
#if MDNS_SYSTEM == LINUX
/* bsd socket stuff */
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h> /* close */

#include <stdio.h>  /* sprintf */
#include <stdint.h> /* for uint8_t and friends */
#include <string.h> /* memset, memcpy, memmove, strlen, strchr, strcpy */

#else
#error "mdns target system is not defined"
#endif

/* compiler-dependent definitions */
#ifdef __GNUC__
/* wrappers to define packed structures */
#define BEGIN_PACK
#define END_PACK __attribute__((__packed__))

#else
#error "mdns compiler is not defined"
#endif


/*
 * mdsn_thread_entry: Thread entry point function
 */
typedef void (*mdns_thread_entry)(void *data);

/*
 * mdns_thread_create: Create a thread
 *
 * mdns_thread_create should create and launch the thread.  mdns launches only
 * a single thread, so the implementation of this function can be fairly
 * simple.
 *
 * entry: thread entry point function
 * data: data to be passed to entry when thread is launched
 *
 * Returns: NULL on failure; a pointer to an opaque type that represents the
 * thread on success.  This return type is passed to the other thread
 * functions.
 *
 */
void *mdns_thread_create(mdns_thread_entry entry, void *data);

/*
 * mdns_thread_delete: Delete a thread
 *
 * t: pointer to thread to be deleted
 *
 */
void mdns_thread_delete(void *t);

/*
 * mdns_thread_yield: yield to other runnable threads
 *
 * Some mdns routines need to yeild after sending commands to the mdns thread.
 * This allows the mdns thread to run and respond to the command.
 */
void mdns_thread_yield(void);

/*
 * mdns_log: printf-like function to write log messages
 *
 * The mdns daemon will write log messages depending on its build-time
 * log-level.  See mdns.h for details.
 */
void mdns_log(const char *fmt, ...);

/*
 * mdns_time_ms: get current time in milliseconds
 *
 * The mdns daemon needs a millisecond up counter for calculating timeouts.
 * The base of the count is arbitrary.  For example, this function could return
 * the number of milliseconds since boot, or since the beginning of the epoch,
 * etc.  Wrap-around is handled internally.  The precision should be to the
 * nearest 10ms.
 */
uint32_t mdns_time_ms(void);

#endif /* __MDNS_PORT_H__ */
