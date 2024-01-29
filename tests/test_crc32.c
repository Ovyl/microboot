/*
 * Copyright (C) Evan Stoddard
 */

/**
 * @file test_crc32.c
 * @author Evan Stoddard
 * @brief 
 */

#include "unity.h"
#include "crc32.h"
#include <stdio.h>

/*****************************************************************************
 * Definitions
 *****************************************************************************/

/*****************************************************************************
 * Variables
 *****************************************************************************/

/*****************************************************************************
 * Prototypes
 *****************************************************************************/

/*****************************************************************************
 * Functions
 *****************************************************************************/

void setUp(void) {

}

void tearDown(void) {

}

void test_crc32_(void) {
    // Setup and Expectations
    char buf[] = "feedbeef";
    uint32_t crc32_val = 0xFDCB37BF;

    // Test
    uint32_t val = crc32(buf, 8);
    
    // Assertions
    TEST_ASSERT_EQUAL(val, crc32_val);
}