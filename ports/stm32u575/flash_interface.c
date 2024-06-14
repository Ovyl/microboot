/*
 * Copyright (C) Evan Stoddard
 */

/**
 * @file flash_interface.c
 * @author Evan Stoddard
 * @brief Example flash interface for stm32u5xx
 */

#include <stdint.h>
#include <string.h>
#include "stm32u5xx.h"
#include "stm32u5xx_hal_flash.h"
#include "stm32u5xx_hal_flash_ex.h"
#include <stdbool.h>

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
 * @brief
 *
 */
#define FLASH_PAGE_SIZE_BYTES (8U * 1024U)

/**
 * @brief Number of pages per bank
 *
 */
#define FLASH_BANK_SIZE_PAGES (128U)

/*****************************************************************************
 * Variables
 *****************************************************************************/

/*****************************************************************************
 * Prototypes
 *****************************************************************************/

/*****************************************************************************
 * Functions
 *****************************************************************************/

bool flash_interface_flash_erase(uint32_t addr, uint32_t size) {
    // Unlock flash
    HAL_FLASH_Unlock();

    uint32_t start = addr - FLASH_WRITE_OFFSET;
    uint32_t initial_page = start / FLASH_PAGE_SIZE_BYTES;

    uint32_t num_pages = size/FLASH_PAGE_SIZE_BYTES;
    if ((size % FLASH_BANK_SIZE_PAGES) != 0) {
        num_pages++;
    }

    // Create erase structure
    FLASH_EraseInitTypeDef erase_struct;
    erase_struct.Banks = FLASH_BANK_1;
    erase_struct.NbPages = num_pages - 1;
    erase_struct.Page = initial_page;
    erase_struct.TypeErase = FLASH_TYPEERASE_PAGES;

    uint32_t page_error;

    // Erase flash
    uint32_t ret = HAL_FLASHEx_Erase(&erase_struct, &page_error);

    // Lock flash
    HAL_FLASH_Lock();

    return (ret == HAL_OK);
}

void flash_interface_flash_read(uint32_t addr, void *dst, uint32_t size) {
    void *src = (void *)addr;
    memcpy(dst, src, size);
}

bool flash_interface_flash_write(uint32_t addr, void *src, uint32_t size) {
    // Unlock flash
    HAL_FLASH_Unlock();

    uint8_t *src_buf = (uint8_t*)src;
    uint32_t src_offset = 0;
    uint32_t dst_offset = addr;

    uint32_t ret = HAL_OK;

    // Write bytes to flash
    while (src_offset < size) {
        uint32_t bytes_remaining = size - src_offset;

        uint32_t chunk_size = 0;

        if (bytes_remaining < FLASH_WRITE_WIDTH_BYTES) {
            chunk_size = bytes_remaining;
        } else {
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