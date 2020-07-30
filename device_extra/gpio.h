enum GpioMode
{
    GPIO_NONE, ///< Neither input nor output are enabled
    GPIO_INPUT, ///< Only input is enabled
    GPIO_OUTPUT, ///< Only output is enabled
    GPIO_BOTH, ///< Both input and output are enabled
    GPIO_IOF0, ///< Pin is passed through to hardware IO function 0 (SPI/I2C/UART)
    GPIO_IOF1 ///< Pin is passed through to hardware IO function 1 (PWM)
};

static inline void gpio_set_mode(uint32_t pinmask, enum GpioMode mode)
{
    gpio dev = gpio0;
    switch (mode)
    {
    case GPIO_NONE:
        dev.p[GPIO_INPUT_EN_OFFSET] &= ~pinmask;
        dev.p[GPIO_OUTPUT_EN_OFFSET] &= ~pinmask;
        dev.p[GPIO_IOF_EN_OFFSET] &= ~pinmask;
        break;
    case GPIO_INPUT:
        dev.p[GPIO_INPUT_EN_OFFSET] &= ~pinmask;
        dev.p[GPIO_OUTPUT_EN_OFFSET] |= pinmask;
        dev.p[GPIO_IOF_EN_OFFSET] &= ~pinmask;
        break;
    case GPIO_OUTPUT:
        dev.p[GPIO_INPUT_EN_OFFSET] &= ~pinmask;
        dev.p[GPIO_OUTPUT_EN_OFFSET] |= pinmask;
        dev.p[GPIO_IOF_EN_OFFSET] &= ~pinmask;
        break;
    case GPIO_BOTH:
        dev.p[GPIO_INPUT_EN_OFFSET] |= pinmask;
        dev.p[GPIO_OUTPUT_EN_OFFSET] |= pinmask;
        dev.p[GPIO_IOF_EN_OFFSET] &= ~pinmask;
        break;
    case GPIO_IOF0:
        dev.p[GPIO_IOF_EN_OFFSET] |= pinmask;
        dev.p[GPIO_IOF_SEL_OFFSET] &= ~pinmask;
        break;
    case GPIO_IOF1:
        dev.p[GPIO_IOF_EN_OFFSET] |= pinmask;
        dev.p[GPIO_IOF_SEL_OFFSET] |= pinmask;
        break;
    }
}

