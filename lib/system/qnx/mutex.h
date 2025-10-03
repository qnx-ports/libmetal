/*
 * Copyright (c) 2025, QNX Software Systems Limited
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*
 * @file	qnx/mutex.h
 * @brief	QNX mutex primitives for libmetal.
 */

#ifndef __METAL_MUTEX__H__
#error "Include metal/mutex.h instead of metal/generic/mutex.h"
#endif

#ifndef __METAL_QNX_MUTEX__H__
#define __METAL_QNX_MUTEX__H__

#include <pthread.h>
#include <errno.h>
#include <metal/assert.h>
#include <metal/errno.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef pthread_mutex_t metal_mutex_t;

/*
 * METAL_MUTEX_DEFINE - used for defining and initializing a global or
 * static singleton mutex
 */
#define METAL_MUTEX_DEFINE(m) metal_mutex_t m = PTHREAD_MUTEX_INITIALIZER

static inline void __metal_mutex_init(metal_mutex_t *mutex)
{
	(mutex = mutex);
  metal_assert(pthread_mutex_init(mutex, NULL) == EOK);
}

static inline void __metal_mutex_deinit(metal_mutex_t *mutex)
{
	(mutex = mutex);
	metal_assert(pthread_mutex_destroy(mutex) == EOK);
}

static inline int __metal_mutex_try_acquire(metal_mutex_t *mutex)
{
  if (pthread_mutex_trylock(mutex) != EOK)
    return 0;
  return 1;
}

static inline void __metal_mutex_acquire(metal_mutex_t *mutex)
{
	(mutex = mutex);
	metal_assert(pthread_mutex_lock(mutex) == EOK);
}

static inline void __metal_mutex_release(metal_mutex_t *mutex)
{
	(mutex = mutex);
	metal_assert(pthread_mutex_unlock(mutex) == EOK);
}

static inline int __metal_mutex_is_acquired(metal_mutex_t *mutex)
{
	if (pthread_mutex_trylock(mutex) == EBUSY)
		return 1;
	return 0;
}

#ifdef __cplusplus
}
#endif

#endif /* __METAL_QNX_MUTEX__H__ */
