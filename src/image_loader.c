/*
 * Copyright (C) Evan Stoddard
 */

/**
 * @file image_loader.c
 * @author Evan Stoddard
 * @brief Handles loading and validating images
 */

#include <stdbool.h>
#include "image_loader.h"
#include "flash_interface.h"
#include "crc32.h"

/*****************************************************************************
 * Definitions
 *****************************************************************************/

/*****************************************************************************
 * Variables
 *****************************************************************************/

/*****************************************************************************
 * Prototypes
 *****************************************************************************/

/**
 * @brief Get image header from partition
 *
 * @param partition Partition
 * @param image Image to read data into
 */
static void image_loader_get_image_header(flash_partition_t *partition, image_t *image);

/**
 * @brief Get image footer from partition
 *
 * @param partition Partition
 * @param image Image to read data into
 */
static void image_loader_get_image_footer(flash_partition_t *partition, image_t *image);

/**
 * @brief Validate CRC-32 of image
 *
 * @param partition Partition
 * @param image Image information
 * @return true Valid CRC-32
 * @return false Invalid CRC-32
 */
static bool image_loader_validate_image_crc(flash_partition_t *partition, image_t *image);

/*****************************************************************************
 * Functions
 *****************************************************************************/

bool image_loader_get_image_info(flash_partition_t *partition, image_t *image) {
    if (!image)
        return false;

    // Get image header
    image_loader_get_image_header(partition, image);

    // Check if magic value is correct
    if (image->header.magic != MICROBOOT_IMAGE_HEADER_MAGIC) {
        return false;
    }

    // Ensure reported image size is not larger than partition
    uint32_t total_size = image->header.image_size;
    total_size += sizeof(image_header_padded_t);
    total_size += sizeof(image_footer_t);
    if (total_size > partition->size) {
        return false;
    }

    // Get image footer
    image_loader_get_image_footer(partition, image);

    // Check if magic value is correct
    if (image->footer.magic != MICROBOOT_IMAGE_FOOTER_MAGIC) {
        return false;
    }

    // Calculate start address of image
    image->image_start = (uint32_t *)(partition->addr + sizeof(image_header_padded_t));

    return image_loader_validate_image(partition, image);
}

bool image_loader_validate_image(flash_partition_t *partition, image_t *image) {
    // Validate image CRC
    return image_loader_validate_image_crc(partition, image);
}

void image_loader_get_image_header(flash_partition_t *partition, image_t *image) {
    flash_interface_flash_read(partition->addr, &image->header, sizeof(image_header_t));
}

void image_loader_get_image_footer(flash_partition_t *partition, image_t *image) {
    // Calculate offset to footer
    uint32_t offset = partition->addr;
    offset += sizeof(image_header_padded_t);
    offset += image->header.image_size;

    // Get reader footer from partition
    flash_interface_flash_read(offset, &image->footer, sizeof(image_footer_t));
}

bool image_loader_validate_image_crc(flash_partition_t *partition, image_t *image) {
    if (!partition)
        return false;

    if (!image)
        return false;

    uint32_t calculated_crc32 = crc32(image->image_start, image->header.image_size);

    // Check CRC
    return (calculated_crc32 == image->footer.crc);
}