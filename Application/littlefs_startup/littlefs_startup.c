/*
 * littlefs_startup.c
 *
 * Created: may 2024
 *  Author: PI
 */ 

#define LFS_NO_DEBUG

#include "lfs.h"
#include "asf_uart.h"
#include "littlefs_startup.h"

#include "driver_w25qxx.h"

// TODO: use MSP to get w25qxx handle
extern w25qxx_handle_t w25q128_handle;


volatile int32_t ITM_RxBuffer;

int _write(int32_t file, uint8_t *ptr, int32_t len){// is used to send printf messages through SWO
    for (int i = 0; i < len; i++){
        ITM_SendChar(*ptr++);
    }
    return len;
}

int _read(int file, char *ptr, int len)
{
    (void)file;
    int DataIdx;

    for (DataIdx = 0; DataIdx < len; DataIdx++)
    {
        *ptr++ = ITM_ReceiveChar();
        //*ptr++ = __io_getchar();
    }

    return len;
}

// create module with the following functions
// TODO: glue w25q128_handle into the struct lfs_config *c????

// Read a region in a block. Negative error codes are propagated to the user.
static int user_provided_block_device_read(const struct lfs_config *c, lfs_block_t block, lfs_off_t off, void *buffer, lfs_size_t size)
{
    /* extract block-size from lfs_config *c */
    uint32_t blockSize = c->block_size;
    uint32_t addr = blockSize * block + off;
    /* extract w25q128_handle from const struct lfs_config *c */
    w25qxx_handle_t *flashChipDriverHandle = (w25qxx_handle_t*)c->context;
    uint8_t res = w25qxx_read(flashChipDriverHandle, addr, (uint8_t *)buffer, size);
    return res;
}

 // Program a region in a block. The block must have previously been erased. Negative error codes are propagated to the user.
 // May return LFS_ERR_CORRUPT if the block should be considered bad.
static int user_provided_block_device_prog(const struct lfs_config *c, lfs_block_t block, lfs_off_t off, const void *buffer, lfs_size_t size)
{
    /* extract block-size from lfs_config *c */
    uint32_t blockSize = c->block_size;
    uint32_t addr = blockSize * block + off;
    /* extract w25q128_handle from const struct lfs_config *c */
    w25qxx_handle_t *flashChipDriverHandle = (w25qxx_handle_t*)c->context;
    uint8_t res = w25qxx_page_program(flashChipDriverHandle, addr, (uint8_t *)buffer, size);
    return res;
}

 // Erase a block. A block must be erased before being programmed. The state of an erased block is undefined. Negative error codes
 // are propagated to the user. May return LFS_ERR_CORRUPT if the block should be considered bad.
static int user_provided_block_device_erase(const struct lfs_config *c, lfs_block_t block)
{
    /* extract block-size from lfs_config *c */
    uint32_t blockSize = c->block_size;
    uint32_t addr = blockSize * block;
    /* extract w25q128_handle from const struct lfs_config *c */
    w25qxx_handle_t *flashChipDriverHandle = (w25qxx_handle_t*)c->context;
    uint8_t res = w25qxx_sector_erase_4k(flashChipDriverHandle, addr);
    return res;
}

// Sync the state of the underlying block device. Negative error codes are propagated to the user.
static int user_provided_block_device_sync(const struct lfs_config *c)
{
    return 0;
}



 // variables used by the filesystem
lfs_t lfs;
lfs_file_t file;

 // configuration of the filesystem is provided by this struct
static const struct lfs_config cfg = {

     // flash memory chip driver handle
     .context = (void*)&w25q128_handle,

     // block device operations
     .read  = user_provided_block_device_read,
     .prog  = user_provided_block_device_prog,
     .erase = user_provided_block_device_erase,
     .sync  = user_provided_block_device_sync,

     // block device configuration
     .read_size = 1,            // can read 1?? or 256 byte
     .prog_size = 256,          // can program 256 byte
     .block_size = 4096,        // erasable block size is 4096 byte
     .block_count = 256,        // 128 Mbit chip has 256 blocks of 4096 byte
     .cache_size = 256,         // should be multiple of read_size/prog_size
     .lookahead_size = 256,     // should be multiple of read_size/prog_size
     .block_cycles = 500,
 };


uint8_t Littlefs_Startup(void)
{
     // mount the filesystem
     int err = lfs_mount(&lfs, &cfg);

     // reformat if we can't mount the filesystem, this should only happen on the first boot
     if (err)
     {
         lfs_format(&lfs, &cfg);
         lfs_mount(&lfs, &cfg);
         uart_print("littlefs: ERROR - could not mount littlefs!\n");
     }

     // read current count
     uint32_t boot_count = 0;
     lfs_file_open(&lfs, &file, "boot_count", LFS_O_RDWR | LFS_O_CREAT);
     lfs_file_read(&lfs, &file, &boot_count, sizeof(boot_count));

     // update boot count
     boot_count += 1;
     lfs_file_rewind(&lfs, &file);
     lfs_file_write(&lfs, &file, &boot_count, sizeof(boot_count));

     // remember the storage is not updated until the file is closed successfully
     lfs_file_close(&lfs, &file);

     // release any resources we were using
     lfs_unmount(&lfs);

     // print the boot count
     uart_print("littlefs: boot_count: %d\n", boot_count);

     return 0;
 }