/*
 * Copyright (C) Evan Stoddard
 */

/**
 * @file flash_interface.h
 * @author Evan Stoddard
 * @brief
 */

#ifndef FLASH_INTERFACE_H_
#define FLASH_INTERFACE_H_

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************
 * Definitions
 *****************************************************************************/

/*****************************************************************************
 * Structs, Unions, Enums, & Typedefs
 *****************************************************************************/

/*****************************************************************************
 * Function Prototypes
 *****************************************************************************/

/**
 * @brief Erase page/block
 *
 * @param addr Address of first page/block
 * @param size Size in pages/block
 */
bool flash_interface_flash_erase(uint32_t addr, uint32_t size);

/**
 * @brief Read data from flash
 *
 * @param addr Start address
 * @param dst Pointer of destination
 * @param size Size in bytes
 */
void flash_interface_flash_read(uint32_t addr, void *dst, uint32_t size);

/**
 * @brief Write data to flash
 *
 * @param addr Start address
 * @param src Source buffer
 * @param size Size of buffer (must be aligned to power of 2)
 */
bool flash_interface_flash_write(uint32_t addr, void *src, uint32_t size);

#ifdef __cplusplus
}
#endif
#endif /* FLASH_INTERFACE_H_ */