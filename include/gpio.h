#pragma once
#include <stdint.h>

#include "hifive-common.h"

#define GPIO_INPUT_VAL 0x00
#define GPIO_INPUT_EN 0x04
#define GPIO_OUTPUT_EN 0x08
#define GPIO_OUTPUT_VAL 0x0C
#define GPIO_PULLUP_EN 0x10
#define GPIO_DRIVE 0x14 // unused
#define GPIO_RISE_IE 0x18
#define GPIO_RISE_IP 0x1C
#define GPIO_FALL_IE 0x20
#define GPIO_FALL_IP 0x24
#define GPIO_HIGH_IE 0x28
#define GPIO_HIGH_IP 0x2C
#define GPIO_LOW_IE 0x30
#define GPIO_LOW_IP 0x34
#define GPIO_IOF_EN 0x38
#define GPIO_IOF_SEL 0x3C
#define GPIO_OUT_XOR 0x40

extern const char* const GPIO_REG_NAMES[];

enum GpioMode
{
    GPIO_NONE, ///< Neither input nor output are enabled
    GPIO_INPUT, ///< Only input is enabled
    GPIO_OUTPUT, ///< Only output is enabled
    GPIO_BOTH, ///< Both input and output are enabled
    GPIO_IOF0, ///< Pin is passed through to hardware IO function 0 (SPI/I2C/UART)
    GPIO_IOF1 ///< Pin is passed through to hardware IO function 1 (PWM)
};

static volatile uint32_t* const GPIO0 = (uint32_t*)0x10012000ul;

void gpio_set_mode(volatile uint32_t* base, uint32_t pinmask, enum GpioMode mode);

static inline int gpio_read(volatile uint32_t* base, uint32_t pinmask)
{
    return (*OFFSET(base, GPIO_INPUT_VAL) & pinmask) > 0;
}

static inline void gpio_write(volatile uint32_t* base, uint32_t pinmask, int value)
{
    if (value) {
        *OFFSET(base, GPIO_OUTPUT_VAL) |= pinmask;
    } else {
        *OFFSET(base, GPIO_OUTPUT_VAL) &= ~pinmask;
    }
}

static inline void gpio_toggle(volatile uint32_t* base, uint32_t pinmask)
{
    *OFFSET(base, GPIO_OUTPUT_VAL) ^= pinmask;
}

static inline void gpio_pullup(volatile uint32_t* base, uint32_t pinmask, int value)
{
    if (value) {
        *OFFSET(base, GPIO_PULLUP_EN) |= pinmask;
    } else {
        *OFFSET(base, GPIO_PULLUP_EN) &= ~pinmask;
    }
}

static inline void gpio_invert(volatile uint32_t* base, uint32_t pinmask, int value)
{
    if (value) {
        *OFFSET(base, GPIO_OUT_XOR) |= pinmask;
    } else {
        *OFFSET(base, GPIO_OUT_XOR) &= ~pinmask;
    }
}
