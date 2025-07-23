/*
 * Copyright (c) 2025, QNX Software Systems Limited
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*
 * @file	qnx/init.c
 * @brief	QNX libmetal initialization.
 */

#include <sys/types.h>
#include <metal/sys.h>
#include <metal/device.h>
#include <metal/irq.h>
#include <metal/cache.h>
#include <metal/utilities.h>

struct metal_state _metal;
struct cache_ctrl __qnx_cache_control;
struct metal_bus metal_generic_bus;

void metal_init_page_sizes()
{
	_metal.page_size = sysconf(_SC_PAGESIZE);
	_metal.page_shift = metal_log2(_metal.page_size);
}

int metal_sys_init(const struct metal_init_params *params)
{
	FILE *urandom;
	unsigned int seed;
  int ret;

	/* Set random seed */
  urandom = fopen("/dev/urandom", "r");
	if (!urandom) {
		metal_log(METAL_LOG_ERROR, "Failed to open /dev/urandom (%s)\n",
			  strerror(errno));
		return -errno;
	}
	if (fread(&seed, 1, sizeof(seed), urandom) <= 0) {
		metal_log(METAL_LOG_DEBUG, "Failed fread /dev/urandom\n");
	}
	fclose(urandom);
	srand(seed);

	/* Initialize page size */
  metal_init_page_sizes();

	/* Initialize IRQ */
	ret = metal_qnx_irq_init();
	if (ret != 0) {
		metal_log(METAL_LOG_ERROR, "IRQ init failed\n",
				strerror(-ret));
		return ret;
	}

	/* Initialize cache */
	memset(&__qnx_cache_control, 0, sizeof(__qnx_cache_control));
	__qnx_cache_control.fd = NOFD;

	ret = cache_init(0, &__qnx_cache_control, NULL);
	if (ret == -1) {
		metal_log(METAL_LOG_ERROR, "Cache init failed\n",
				strerror(errno));
		return -errno;
	}

	/* Initialize generic bus */
	ret = metal_bus_register(&metal_generic_bus);
	if (ret != 0) {
		metal_log(METAL_LOG_DEBUG, "Generic bus init failed\n",
				strerror(-errno));
	}

	metal_unused(params);

	/* Set process pmap */
  ret = open("/proc/self/pmap", O_RDONLY | O_CLOEXEC);
  if (ret == -1) {
    metal_log(METAL_LOG_DEBUG, "Failed pmap open - %s\n",
				strerror(errno));
  }
  _metal.pagemap_fd = ret;

  return 0;
}

void metal_sys_finish(void)
{
	cache_fini(&__qnx_cache_control);
	metal_qnx_irq_shutdown();
	metal_bus_unregister(&metal_generic_bus);
}

