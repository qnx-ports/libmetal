/*
 * Copyright (c) 2025, QNX Software Systems Limited
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*
 * @file	qnx/sys.h
 * @brief	QNX system primitives for libmetal.
 */

#include <metal/device.h>

int metal_generic_dev_sys_open(struct metal_device *dev)
{
  struct metal_io_region *io;
  unsigned int i, ret;

  /* map I/O memory regions */
	for (i = 0; i < dev->num_regions; i++) {
    io = &dev->regions[i];
		ret = metal_sys_io_mem_map(io);
    if (ret != 0)
      return ret;
	}

	return 0;
}

int metal_generic_dev_sys_close(struct metal_device *dev)
{
  struct metal_io_region *io;
  unsigned int i, ret;

  /* unmap I/O memory regions */
  for (i = 0; i < dev->num_regions; i++) {
    io = &dev->regions[i];
    ret = metal_sys_io_mem_unmap(io);
    if (ret != 0)
      return ret;
  }

  return 0;
}
