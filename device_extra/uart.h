enum UartMode
{
    UART_DISABLED,
    UART_8N1,
    UART_8N2
};

static inline void uart_init(uart dev, enum UartMode mode, uint32_t divisor)
{
    uart_txctrl txctrl = {false};
    uart_rxctrl rxctrl = {false};
    switch(mode) {
    case UART_8N1:
        txctrl.txen = true;
        txctrl.nstop = 0; // means 1 stop bits
        rxctrl.rxen = true;
        break;
    case UART_8N2:
        txctrl.txen = true;
        txctrl.nstop = 1; // means 2 stop bits
        rxctrl.rxen = true;
        break;
    case UART_DISABLED:
        break;
    }

    uart_txctrl_set(dev, txctrl);
    uart_rxctrl_set(dev, rxctrl);
    uart_div_set(dev, divisor - 1);
}

static inline bool uart_write(uart dev, uint8_t b)
{
    uint32_t status = __atomic_fetch_or(&dev.p[UART_TXDATA_OFFSET], (uint32_t)b, __ATOMIC_RELAXED);
    return status == 0;
}

static inline bool uart_read(uart dev, uint8_t* b)
{
    uart_rxdata rxdata = uart_rxdata_get(dev);
    *b = rxdata.data;
    return !rxdata.empty;
}

static inline void uart_write_blocking(uart dev, const void* data, size_t size)
{
    const unsigned char* data_char = data;
    for (size_t i = 0; i < size; i++) {
        while (!uart_write(dev, data_char[i])) {}
    }
}

static inline void uart_print_blocking(uart dev, const char* str)
{
    while (*str != '\0') {
        while (!uart_write(dev, *str)) {}
        str++;
    }
}

static inline void uart_read_blocking(uart dev, void* data, size_t size)
{
    unsigned char* data_char = data;
    for (size_t i = 0; i < size; i++) {
        while (!uart_read(dev, &data_char[i])) {}
    }
}
