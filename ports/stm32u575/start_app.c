/*
 * Copyright (C) Evan Stoddard
 */

/**
 * @file start_app.c
 * @author Evan Stoddard
 * @brief stm32f0xx function to kick into application
 */

#include "microboot.h"
#include "main.h"

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

void start_app(image_t *image) {

    // Get pointer to image location in flash along with Stack Pointer and Program Counter
    uint32_t *app = (uint32_t*)image->image_start;
    uint32_t sp = app[0];
    uint32_t pc = app[1];

    // Easy way to kick into applications reset handler
    void (*application_reset_handler)(void) = (void *)pc;

    // This part has a vector relocatable vector table
    // Set that register to the application's vector table
    SCB->VTOR = (uint32_t)app;

    // Memory barriers to insure instruction cache is fully flushed
    __DMB();
    __ISB();

    // We don't want any interrupts to happen at this point
	__disable_irq();

    // Update stack pointer
    __asm volatile ("MSR msp, %0" : : "r" (sp) : );

    // Kick into application
    application_reset_handler();
}