#include <stdint.h>
#include <stddef.h>

#include "uart.h"

void uart_txmode(volatile uint32_t* uart, enum UartMode mode)
{
    uint32_t ctrl = *OFFSET(uart, UART_TXCTRL);
    switch (mode)
    {
    case UART_DISABLED:
        ctrl = ctrl & ~0x3;
    case UART_8N1:
        ctrl = (ctrl & ~0x3) | UART_TXEN;
    case UART_8N2:
        ctrl = ctrl | UART_TXEN | UART_NSTOP;
    }
    *OFFSET(uart, UART_TXCTRL) = ctrl;
}

void uart_txcnt(volatile uint32_t* uart, uint32_t count)
{
    *OFFSET(uart, UART_TXCTRL) = (*OFFSET(uart, UART_TXCTRL) & 0x70000) | (count << 16);
}

void uart_rxmode(volatile uint32_t* uart, enum UartMode mode)
{
    if (mode == UART_DISABLED) {
        *OFFSET(uart, UART_RXCTRL) &= ~1;
    } else {
        *OFFSET(uart, UART_RXCTRL) |= 1;
    }
}

void uart_rxcnt(volatile uint32_t* uart, uint32_t count)
{
    *OFFSET(uart, UART_RXCTRL) = (*OFFSET(uart, UART_RXCTRL) & 0x70000) | (count << 16);
}

void uart_init(volatile uint32_t* uart, enum UartMode mode, uint32_t divisor)
{
    uart_txmode(uart, mode);
    uart_rxmode(uart, mode);
    uart_setdivisor(uart, divisor);
}

void uart_write_blocking(volatile uint32_t* uart, const void* data, size_t size)
{
    const unsigned char* data_char = data;
    for (size_t i = 0; i < size; i++) {
        while (!uart_write(uart, data_char[i])) {}
    }
}

void uart_print_blocking(volatile uint32_t* uart, const char* str)
{
    while (*str != '\0') {
        while (!uart_write(uart, *str)) {}
        str++;
    }
}

void uart_read_blocking(volatile uint32_t* uart, void* data, size_t size)
{
    unsigned char* data_char = data;
    for (size_t i = 0; i < size; i++) {
        while (!uart_read(uart, &data_char[i])) {}
    }
}
