/*
 * Copyright (C) Evan Stoddard
 */

/**
 * @file image_types.h
 * @author Evan Stoddard
 * @brief Typedefs for images
 */

#ifndef IMAGE_TYPES_H_
#define IMAGE_TYPES_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************
 * Definitions
 *****************************************************************************/

#define MICROBOOT_IMAGE_HEADER_MAGIC 0xE5B0074EU
#define MICROBOOT_IMAGE_FOOTER_MAGIC 0xE5B007F0U

/**
 * @brief Image header size with padding.
 *
 */
#ifndef IMAGE_HEADER_SIZE_BYTES
#define IMAGE_HEADER_SIZE_BYTES 0x200U
#endif

/*****************************************************************************
 * Structs, Unions, Enums, & Typedefs
 *****************************************************************************/

/**
 * @brief Image header typedef
 *
 */
typedef struct image_header_t {
    uint32_t magic;
    uint32_t image_size;
    uint16_t version_major;
    uint16_t version_minor;
    uint16_t version_patch;
    uint32_t software_type;
} __attribute__((packed)) image_header_t;

/**
 * @brief Header with padding
 *
 */
typedef union image_header_padded_t {
    image_header_t header;
    uint8_t bytes[IMAGE_HEADER_SIZE_BYTES];
} __attribute__((packed)) image_header_padded_t;

/**
 * @brief Image footer typedef
 *
 */
typedef struct image_footer_t {
    uint32_t magic;
    uint32_t crc;
} __attribute__((packed)) image_footer_t;

/**
 * @brief Image typedef
 *
 */
typedef struct image_t {
    image_header_t header;
    uint32_t *image_start;
    image_footer_t footer;
} image_t;

/*****************************************************************************
 * Function Prototypes
 *****************************************************************************/

#ifdef __cplusplus
}
#endif
#endif /* IMAGE_TYPES_H_ */