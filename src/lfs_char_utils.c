#include "lfs_char_utils.h"

lfs_ssize_t lfs_fgetc(lfs_t *lfs, lfs_file_t *file, char *c)
{
  return lfs_file_read(lfs, file, c, sizeof(char));
}

lfs_ssize_t lfs_readLine(lfs_t *lfs, lfs_file_t *file, void *buffer, lfs_size_t n)
{
  lfs_ssize_t numRead;
  lfs_size_t totRead;
  char *buf;
  char ch;

  if( n <= 0 || buffer == NULL) {
    return LFS_ERR_INVAL;
  }

  buf = buffer;

  totRead = 0;
  for(;;) {
    numRead = lfs_fgetc(lfs, file, &ch);

    if(numRead < 0) {
      return numRead;
    }
    else if(numRead == 0) {
      if(totRead == 0)
        return 0;
      else
        break;
    } else {
      if ((totRead < n - 1) && (ch != '\r')) {
        totRead++;
        *buf++ = ch;
      }

      if(ch == '\n')
        break;
    }
  }

  *buf = '\0';
  return totRead;
}

lfs_ssize_t lfs_fputc(lfs_t *lfs, lfs_file_t *file, const char *c)
{
  return lfs_file_write(lfs, file, c, sizeof(char));
}

lfs_ssize_t lfs_writeLine(lfs_t *lfs, lfs_file_t *file, void *buffer, lfs_size_t n)
{
  lfs_ssize_t numWrite;
  lfs_size_t totWrite;
  char *buf;
  lfs_size_t bufPos;
  char ch;
  static const char nl = '\n';

  if( n <= 0 || buffer == NULL) {
    return LFS_ERR_INVAL;
  }

  buf = buffer;
  bufPos = 0;
  totWrite = 0;

  for(;;) {
    ch = buf[bufPos++];
    
    numWrite = lfs_fputc(lfs, file, (ch == '\0' ? &nl : &ch));

    if(numWrite <= 0) {
      if(totWrite == 0)
        return numWrite;
      else 
        return totWrite;
    }
    else {
      totWrite++;
      if((bufPos < n) && (ch != '\0')) {
        continue;
      }
      else {
        return totWrite;
      }
    }
  }
}
