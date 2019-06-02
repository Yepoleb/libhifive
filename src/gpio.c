#include <stdint.h>

#include "gpio.h"

const char* const GPIO_REG_NAMES[] = {
    "INPUT_VAL",
    "INPUT_EN",
    "OUTPUT_EN",
    "OUTPUT_VAL",
    "PULLUP_EN",
    "DRIVE",
    "RISE_IE",
    "RISE_IP",
    "FALL_IE",
    "FALL_IP",
    "HIGH_IE",
    "HIGH_IP",
    "LOW_IE",
    "LOW_IP",
    "IOF_EN",
    "IOF_SEL",
    "OUT_XOR"
};

void gpio_set_mode(volatile uint32_t* base, uint32_t pinmask, enum GpioMode mode)
{
    switch (mode)
    {
    case GPIO_NONE:
        *OFFSET(base, GPIO_INPUT_EN) &= ~pinmask;
        *OFFSET(base, GPIO_OUTPUT_EN) &= ~pinmask;
        *OFFSET(base, GPIO_IOF_EN) &= ~pinmask;
        break;
    case GPIO_INPUT:
        *OFFSET(base, GPIO_INPUT_EN) &= ~pinmask;
        *OFFSET(base, GPIO_INPUT_EN) |= pinmask;
        *OFFSET(base, GPIO_IOF_EN) &= ~pinmask;
        break;
    case GPIO_OUTPUT:
        *OFFSET(base, GPIO_INPUT_EN) &= ~pinmask;
        *OFFSET(base, GPIO_OUTPUT_EN) |= pinmask;
        *OFFSET(base, GPIO_IOF_EN) &= ~pinmask;
        break;
    case GPIO_BOTH:
        *OFFSET(base, GPIO_INPUT_EN) |= pinmask;
        *OFFSET(base, GPIO_OUTPUT_EN) |= pinmask;
        *OFFSET(base, GPIO_IOF_EN) &= ~pinmask;
        break;
    case GPIO_IOF0:
        *OFFSET(base, GPIO_IOF_EN) |= pinmask;
        *OFFSET(base, GPIO_IOF_SEL) &= ~pinmask;
        break;
    case GPIO_IOF1:
        *OFFSET(base, GPIO_IOF_EN) |= pinmask;
        *OFFSET(base, GPIO_IOF_SEL) |= pinmask;
        break;
    }
}
