#ifndef __LFS_CHAR_UTILS_H__
#define __LFS_CHAR_UTILS_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "lfs.h"

lfs_ssize_t lfs_fgetc(lfs_t *lfs, lfs_file_t *file, char *c);
lfs_ssize_t lfs_readLine(lfs_t *lfs, lfs_file_t *file, void *buffer, lfs_size_t n);

lfs_ssize_t lfs_fputc(lfs_t *lfs, lfs_file_t *file, const char *c);
lfs_ssize_t lfs_writeLine(lfs_t *lfs, lfs_file_t *file, void *buffer, lfs_size_t n);

#ifdef __cplusplus
}
#endif


#endif // __LFS_CHAR_UTILS__