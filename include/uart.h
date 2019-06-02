#pragma once
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "hifive-common.h"

static volatile uint32_t* const UART0 = (uint32_t*)0x10013000ul;
static volatile uint32_t* const UART1 = (uint32_t*)0x10023000ul;

#define UART_TXDATA 0x00
#define UART_RXDATA 0x04
#define UART_TXCTRL 0x08
#define UART_RXCTRL 0x0C
#define UART_IE 0x10
#define UART_IP 0x14
#define UART_DIV 0x18

#define UART_TXEN (1 << 0)
#define UART_NSTOP (1 << 1)
#define UART_RXEN (1 << 0)

#define UART_TX (1 << 0)
#define UART_RX (1 << 1)

enum UartMode
{
    UART_DISABLED,
    UART_8N1,
    UART_8N2
};

static inline bool uart_write(volatile uint32_t* uart, uint8_t b)
{
    uint32_t status = __atomic_fetch_or(OFFSET(uart, UART_TXDATA), (uint32_t)b, __ATOMIC_RELAXED);
    return status == 0;
}

static inline bool uart_read(volatile uint32_t* uart, uint8_t* b)
{
    uint32_t rxdata = *OFFSET(uart, UART_RXDATA);
    *b = (uint8_t)rxdata;
    return (rxdata & (1ul << 31)) == 0;
}

static inline void uart_enableinterrupts(volatile uint32_t* uart, uint32_t mask)
{
    *OFFSET(uart, UART_IE) |= mask;
}

static inline void uart_disableinterrupts(volatile uint32_t* uart, uint32_t mask)
{
    *OFFSET(uart, UART_IE) &= ~mask;
}

static inline uint32_t uart_pending(volatile uint32_t* uart)
{
    return *OFFSET(uart, UART_IP);
}

static inline uint32_t uart_getdivisor(volatile uint32_t* uart)
{
    return *OFFSET(uart, UART_DIV) + 1;
}

static inline void uart_setdivisor(volatile uint32_t* uart, uint32_t divisor)
{
    *OFFSET(uart, UART_DIV) = divisor - 1;
}

void uart_txmode(volatile uint32_t* uart, enum UartMode mode);
void uart_txcnt(volatile uint32_t* uart, uint32_t count);
void uart_rxmode(volatile uint32_t* uart, enum UartMode mode);
void uart_rxcnt(volatile uint32_t* uart, uint32_t count);
void uart_init(volatile uint32_t* uart, enum UartMode mode, uint32_t divisor);
void uart_write_blocking(volatile uint32_t* uart, const void* data, size_t size);
void uart_print_blocking(volatile uint32_t* uart, const char* str);
void uart_read_blocking(volatile uint32_t* uart, void* data, size_t size);
