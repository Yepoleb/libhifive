#pragma once
#include <stdint.h>

#define PIN(npin) (1ul << npin)

static inline void delay(uint32_t count) // 2 cycles per count
{
    if (count == 0) return;
    __asm__ volatile (
        "delay_dec%=:\n"
        "addi %[count], %[count], -1\n"
        "bnez %[count], delay_dec%="
        : [count] "+r" (count)
    );
}
