#include "lfs_demo.h"

#include "lfs.h"
#include "lfs_char_utils.h"

#include "w25qxx.h"
#include "block_device.h"

lfs_t lfs;
lfs_file_t file;

int lfs_demo(void) {
    const struct lfs_config cfg = {
        // block device operations
        .read = block_device_read,
        .prog = block_device_prog,
        .erase = block_device_erase,
        .sync = block_device_sync,

        // block device configuarions
        .read_size = 256,
        .prog_size = 256,
        .block_size = 4096,
        .cache_size = 256,
        .block_count = 2048,
        .lookahead_size = 16,
        .block_cycles = 500,
    };

    // mount the filesystem
    int err = lfs_mount(&lfs, &cfg);

    // reformat if we can't mount the filesystem
    // this should only happen on the first boot
    if(err) {
        lfs_format(&lfs, &cfg);
        lfs_mount(&lfs, &cfg);
    }

    err = lfs_file_open(&lfs, &file, "info.txt", LFS_O_RDONLY);

    if(err == LFS_ERR_NOENT) {
        err = lfs_file_open(&lfs, &file, "info.txt", LFS_O_RDWR | LFS_O_CREAT);
        err = lfs_writeLine(&lfs, &file, "GD32E230F8", 80);
        err = lfs_writeLine(&lfs, &file, "Jigsy Board V1.0", 80);
        err = lfs_file_close(&lfs, &file);

        err = lfs_file_open(&lfs, &file, "info.txt", LFS_O_RDONLY);
    }

    if(!err) {
        char buffer[80];

        printf("\r\n");
        while(lfs_readLine(&lfs, &file, buffer, 80) > 0) {
            printf("%s", buffer);
        }
        lfs_file_close(&lfs, &file);
    }

    W25Qx_Parameter W25Qx_Para;
    W25Qx_Get_Parameter(&W25Qx_Para);

    printf("Flash Id: 0x%lx\r\n", W25Qx_Para.FLASH_ID);
    printf("Capacity: %ldM\r\n", W25Qx_Para.FLASH_CAPACITY / (1024*1024));
    printf("Sector Count: %ld\r\n", W25Qx_Para.SECTOR_COUNT);
    printf("Sector Size: %ld bytes\r\n", W25Qx_Para.SECTOR_SIZE);
    printf("Subsector Count: %ld\r\n", W25Qx_Para.SUBSECTOR_COUNT);
    printf("Subsector Size: %ld bytes\r\n", W25Qx_Para.SUBSECTOR_SIZE);
    printf("Page Size: %ld bytes\r\n", W25Qx_Para.PAGE_SIZE);

    // read current count
    uint32_t boot_count = 0;
    err = lfs_file_open(&lfs, &file, "boot_count", LFS_O_RDWR | LFS_O_CREAT);
    if(!err) {
        lfs_file_read(&lfs, &file, &boot_count, sizeof(boot_count));
        // update boot count
        boot_count++;
        lfs_file_rewind(&lfs, &file);
    }
    lfs_file_write(&lfs, &file, &boot_count, sizeof(boot_count));

    // remember the storage is not updated until the file is closed successfully
    lfs_file_close(&lfs, &file);

    // print the boot count
    printf("boot_count: %ld\r\n", boot_count);

    uint32_t loop_count = 0;
    while (1)
    {
        gpio_bit_set(LED_PORT, LED_PIN);
        delay_1ms(50);
        gpio_bit_reset(LED_PORT, LED_PIN);
        delay_1ms(500);

        err = lfs_file_open(&lfs, &file, "loop_count", LFS_O_RDWR | LFS_O_CREAT);
        if(!err) {
            lfs_file_read(&lfs, &file, &loop_count, sizeof(loop_count));
            loop_count++;
            lfs_file_rewind(&lfs, &file);
        }
        lfs_file_write(&lfs, &file, &loop_count, sizeof(loop_count));
        lfs_file_close(&lfs, &file);

        printf("Loop count = %ld\r\n", loop_count++);
    }

    // release any resources we were using (knowing we'll never get here!)
    lfs_unmount(&lfs);
}