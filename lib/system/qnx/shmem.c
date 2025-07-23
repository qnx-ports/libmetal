/*
 * Copyright (c) 2025, QNX Software Systems Limited
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*
 * @file	qnx/shmem.c
 * @brief	QNX libmetal shared memory handling.
 */

#include <metal/shmem.h>

int metal_shmem_open(const char *name, size_t size,
		     struct metal_io_region **io)
{
	int ret;

	ret = metal_shmem_open_generic(name, size, io);
	if (!ret)
		return ret;

	ret = metal_open(name, 1);
	if (!ret)
		return ret;

	void **addr = NULL;

	ret = metal_map(ret, 0, size, 0, 0, addr);
	if (!ret)
		return ret;

	const metal_phys_addr_t *physmap = (unsigned long int *)__qnx_get_physical_address(*addr, size);

	metal_io_init(*io, NULL, physmap, size, MAX_PAGE_SHIFT, 0, NULL);

	ret = metal_unmap(addr, size);
	if (ret != 0) {
		metal_log(METAL_LOG_DEBUG, "unmapping temporary \
			virtual address failed\n", strerror(-ret));
	}

	return 0;
}
