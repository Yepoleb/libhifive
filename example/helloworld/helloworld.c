#include <stdbool.h>
#include <stdio.h>

#include "gpio.h"
#include "pwm.h"
#include "uart.h"

int main()
{
    gpio_set_mode(GPIO0, PIN(22) | PIN(19) | PIN(21), GPIO_IOF1);

    pwm_setzerocmp(PWM1, true);
    pwm_setcmp(PWM1, 0, 255);
    pwm_setenalways(PWM1, true);

    // Set LED color
    pwm_setcmp(PWM1, 3, 64);
    pwm_setcmp(PWM1, 1, 255);
    pwm_setcmp(PWM1, 2, 255);

    uart_init(UART0, UART_8N1, 139);
    printf("Hello world!\r\n");
}
