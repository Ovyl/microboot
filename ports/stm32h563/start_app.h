/*
 * Copyright (C) Evan Stoddard
 */

/**
 * @file start_app.h
 * @author Evan Stoddard
 * @brief
 */

#ifndef START_APP_H_
#define START_APP_H_

#include <stdint.h>
#include "microboot.h"

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
 * @brief Boot into fetched image
 *
 * @param image Image to boot into
 */
void start_app(image_t *image);

#ifdef __cplusplus
}
#endif
#endif /* START_APP_H_ */