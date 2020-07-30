#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>

#include "hifive/gpio.h"
#include "hifive/pwm.h"
#include "hifive/util.h"

/*
gpio 22 red
gpio 19 green
gpio 21 blue
*/

#define MAIN_CLOCK_RATE 16000000
#define USEC_TO_DELAY(usec) ((usec) * (MAIN_CLOCK_RATE / 1000000 / 2))
#define MSEC_TO_DELAY(msec) ((msec) * (MAIN_CLOCK_RATE / 1000 / 2))

struct RGBColor
{
    float r;
    float g;
    float b;
};

struct HSVColor
{
    float h;
    float s;
    float v;
};

struct RGBColor gammacorrect(struct RGBColor rgb)
{
    rgb.r = powf(rgb.r, 1/2.2f);
    rgb.g = powf(rgb.g, 1/2.2f);
    rgb.b = powf(rgb.b, 1/2.2f);
    return rgb;
}

struct RGBColor hsv_to_rgb(struct HSVColor hsv)
{
    float c = hsv.v * hsv.s;
    float h1 = hsv.h * 6.f;
    float x = c * (1 - fabsf(fmodf(h1, 2.f) - 1));
    struct RGBColor rgb1 = {0, 0, 0};
    switch ((int)h1)
    {
    case 0: rgb1.r = c; rgb1.g = x; rgb1.b = 0; break;
    case 1: rgb1.r = x; rgb1.g = c; rgb1.b = 0; break;
    case 2: rgb1.r = 0; rgb1.g = c; rgb1.b = x; break;
    case 3: rgb1.r = 0; rgb1.g = x; rgb1.b = c; break;
    case 4: rgb1.r = x; rgb1.g = 0; rgb1.b = c; break;
    case 5:
    case 6: rgb1.r = c; rgb1.g = 0; rgb1.b = x; break;
    }
    float m = hsv.v - c;
    struct RGBColor rgb = {rgb1.r + m, rgb1.g + m, rgb1.b + m};
    return rgb;
}

int main(void)
{
    gpio_set_mode(PIN(22) | PIN(19) | PIN(21), GPIO_IOF1);

    pwm_pwmcfg pwmcfg = {0};
    pwmcfg.pwmzerocmp = true;
    pwmcfg.pwmenalways = true;

    pwm_pwmcmp0_set(pwm1, 255);
    pwm_pwmcfg_set(pwm1, pwmcfg);

    for (uint8_t hue = 0; ; hue++) {
        struct HSVColor c1 = {hue / 255.f, 1.f, 0.5f};
        struct RGBColor c2 = gammacorrect(hsv_to_rgb(c1));
        pwm_pwmcmp3_set(pwm1, (uint32_t)(c2.r * 255));
        pwm_pwmcmp1_set(pwm1, (uint32_t)(c2.g * 255));
        pwm_pwmcmp2_set(pwm1, (uint32_t)(c2.b * 255));
        delay(MSEC_TO_DELAY(4));
    }
    return 0;
}
