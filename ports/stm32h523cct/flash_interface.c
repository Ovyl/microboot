/*
 * Copyright (C) Evan Stoddard
 */

/**
 * @file flash_interface.c
 * @author Evan Stoddard
 * @brief Example flash interface for stm32u5xx
 */

#include "stm32h5xx.h"
#include "stm32h5xx_hal_flash.h"
#include "stm32h5xx_hal_flash_ex.h"
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

/*****************************************************************************
 * Definitions
 *****************************************************************************/

/**
 * @brief While reads are memory mapped to 0x0, flash starts at 0x08000000
 *
 */
#define FLASH_WRITE_OFFSET 0x08000000U

/**
 * @brief Width of Flash Writes
 *
 */
#define FLASH_WRITE_WIDTH_BYTES (4U * 4U)

/**
 * @brief Sector size in bytes
 *
 */
#define FLASH_SECTOR_SIZE_BYTES (8U * 1024U)

/**
 * @brief Number of sectors per bank
 *
 */
#define FLASH_BANK_SIZE_SECTORS (16)

/*****************************************************************************
 * Variables
 *****************************************************************************/

/*****************************************************************************
 * Prototypes
 *****************************************************************************/

/*****************************************************************************
 * Functions
 *****************************************************************************/

bool flash_interface_flash_erase(uint32_t addr, uint32_t size)
{
    // Unlock flash
    HAL_FLASH_Unlock();

    uint32_t start = addr - FLASH_WRITE_OFFSET;
    uint32_t initial_sector = start / FLASH_SECTOR_SIZE_BYTES;

    uint32_t bank = FLASH_BANK_1;

    if (initial_sector >= FLASH_BANK_SIZE_SECTORS) {
        bank = FLASH_BANK_2;
        initial_sector -= FLASH_BANK_SIZE_SECTORS;
    }

    uint32_t num_sectors = size / FLASH_SECTOR_SIZE_BYTES;
    if ((size % FLASH_BANK_SIZE_SECTORS) != 0) {
        num_sectors++;
    }

    // Create erase structure
    FLASH_EraseInitTypeDef erase_struct;
    erase_struct.Banks = bank;
    erase_struct.Sector = initial_sector;
    erase_struct.NbSectors = num_sectors;
    erase_struct.TypeErase = FLASH_TYPEERASE_SECTORS;

    uint32_t sector_error;

    // Erase flash
    uint32_t ret = HAL_FLASHEx_Erase(&erase_struct, &sector_error);

    // Lock flash
    HAL_FLASH_Lock();

    return (ret == HAL_OK);
}

void flash_interface_flash_read(uint32_t addr, void *dst, uint32_t size)
{
    void *src = (void *)addr;
    memcpy(dst, src, size);
}

bool flash_interface_flash_write(uint32_t addr, void *src, uint32_t size)
{
    // Unlock flash
    HAL_FLASH_Unlock();

    uint8_t *src_buf = (uint8_t *)src;
    uint32_t src_offset = 0;
    uint32_t dst_offset = addr;

    uint32_t ret = HAL_OK;

    // Write bytes to flash
    while (src_offset < size) {
        uint32_t bytes_remaining = size - src_offset;

        uint32_t chunk_size = 0;

        if (bytes_remaining < FLASH_WRITE_WIDTH_BYTES) {
            chunk_size = bytes_remaining;
        }
        else {
            chunk_size = FLASH_WRITE_WIDTH_BYTES;
        }

        uint8_t tmp[FLASH_WRITE_WIDTH_BYTES] = {0};
        memcpy(tmp, &src_buf[src_offset], chunk_size);

        __disable_irq();
        ret = HAL_FLASH_Program(FLASH_TYPEPROGRAM_QUADWORD, dst_offset, (uint32_t)tmp);
        __enable_irq();

        src_offset += FLASH_WRITE_WIDTH_BYTES;
        dst_offset += FLASH_WRITE_WIDTH_BYTES;
    }

    // Lock flash
    HAL_FLASH_Unlock();

    return (ret == HAL_OK);
}