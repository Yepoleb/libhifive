#include <stdbool.h>
#include <stdio.h>

#include "hifive/gpio.h"
#include "hifive/pwm.h"
#include "hifive/uart.h"
#include "hifive/util.h"

int main(void)
{
    gpio_set_mode(PIN(22) | PIN(19) | PIN(21), GPIO_IOF1);

    pwm_pwmcfg pwmcfg = {0};
    pwmcfg.pwmzerocmp = true;
    pwmcfg.pwmenalways = true;

    pwm_pwmcmp0_set(pwm1, 255);
    pwm_pwmcfg_set(pwm1, pwmcfg);

    // Set LED color
    pwm_pwmcmp3_set(pwm1, 64);
    pwm_pwmcmp1_set(pwm1, 255);
    pwm_pwmcmp2_set(pwm1, 255);

    // Initialize with baudrate 115200
    // 16Mhz (tlclk) / 139 = 115107Hz
    uart_init(uart0, UART_8N1, 139);
    printf("Hello world!\r\n");
    return 0;
}
