/*
 * Copyright (c) 2025, QNX Software Systems Limited
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*
 * @file	qnx/io.c
 * @brief	QNX libmetal io operations.
 */

#include <metal/io.h>
#include <metal/sys.h>

int metal_sys_io_mem_map(struct metal_io_region *io)
{
	void *addr;
  int flags = io->mem_flags | MAP_SHARED | MAP_PHYS;
  addr = mmap(NULL, io->size, PROT_READ | PROT_WRITE, flags, NOFD, io->physmap);
  if (addr == MAP_FAILED)
    return -errno;

  io->virt = addr;
  return 0;
}

int metal_sys_io_mem_unmap(struct metal_io_region *io)
{
  int ret;
  if (io->virt == NULL) return 0;

  ret = munmap(io->virt, io->size);
  if (ret != 0)
    return -errno;

  return 0;
}
