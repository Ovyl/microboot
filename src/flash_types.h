/*
 * Copyright (C) Evan Stoddard
 */

/**
 * @file flash_types.h
 * @author Evan Stoddard
 * @brief Typedefs for flash configuration
 */

#ifndef FLASH_TYPES_H_
#define FLASH_TYPES_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************
 * Definitions
 *****************************************************************************/

/*****************************************************************************
 * Structs, Unions, Enums, & Typedefs
 *****************************************************************************/

/**
 * @brief Enum defining partition type
 *
 */
typedef enum {
    FLASH_PARTITION_BOOTLOADER,
    FLASH_PARTITION_SLOT_PRIMARY,
    FLASH_PARTITION_SLOT_SECONDARY,
    FLASH_PARTITION_DATA,
} flash_partition_type_t;

/**
 * @brief Partition typedef
 *
 */
typedef struct flash_partition_t {
    flash_partition_type_t type;
    uint32_t addr;
    uint32_t size;
} flash_partition_t;

/*****************************************************************************
 * Function Prototypes
 *****************************************************************************/

#ifdef __cplusplus
}
#endif
#endif /* FLASH_TYPES_H_ */