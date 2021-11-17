#include "block_device.h"
#include "lfs.h"
#include "w25qxx.h"

int block_device_read(const struct lfs_config *c, lfs_block_t block, lfs_off_t off, void *buffer, lfs_size_t size)
{
  W25Qx_Read((uint8_t*)buffer, (block * c->block_size + off), size);
  return 0;
}

int block_device_prog(const struct lfs_config *c, lfs_block_t block, lfs_off_t off, const void *buffer, lfs_size_t size)
{
  W25Qx_Write((uint8_t*)buffer, (block * c->block_size + off), size);
  return 0;
}

int block_device_erase(const struct lfs_config *c, lfs_block_t block)
{
  W25Qx_Erase_Block(block * c->block_size);
  return 0;
}

int block_device_sync(const struct lfs_config *c)
{
  return 0;
}
