#pragma once
#include <stdint.h>
#include <stdbool.h>

#include "hifive-common.h"

static volatile uint32_t* const PWM0 = (uint32_t*)0x10015000ul;
static volatile uint32_t* const PWM1 = (uint32_t*)0x10025000ul;
static volatile uint32_t* const PWM2 = (uint32_t*)0x10035000ul;

#define PWM_CFG 0x00
#define PWM_COUNT 0x08
#define PWM_SCALED 0x10
#define PWM_CMP0 0x20
#define PWM_CMP1 0x24
#define PWM_CMP2 0x28
#define PWM_CMP3 0x2C

#define PWMC_SCALE 0
#define PWMC_STICKY 8
#define PWMC_ZEROCMP 9
#define PWMC_DEGLITCH 10
#define PWMC_ENALWAYS 12
#define PWMC_ENONESHOT 13
#define PWMC_CMP0CENTER 16
#define PWMC_CMP1CENTER 17
#define PWMC_CMP2CENTER 18
#define PWMC_CMP3CENTER 19
#define PWMC_CMP0GANG 24
#define PWMC_CMP1GANG 25
#define PWMC_CMP2GANG 26
#define PWMC_CMP3GANG 27
#define PWMC_CMP0IP 28
#define PWMC_CMP1IP 29
#define PWMC_CMP2IP 30
#define PWMC_CMP3IP 31


static inline uint32_t pwm_getcounter(volatile uint32_t* pwm)
{
    return *OFFSET(pwm, PWM_COUNT);
}

static inline void pwm_setcounter(volatile uint32_t* pwm, uint32_t value)
{
    *OFFSET(pwm, PWM_COUNT) = value;
}

static inline uint32_t pwm_getscaledcounter(volatile uint32_t* pwm)
{
    return *OFFSET(pwm, PWM_SCALED);
}

static inline void pwm_setscaledcounter(volatile uint32_t* pwm, uint32_t value)
{
    *OFFSET(pwm, PWM_SCALED) = value;
}

static inline uint32_t pwm_getcmp(volatile uint32_t* pwm, uint32_t index)
{
    return *OFFSET(pwm, PWM_CMP0 + index * 4);
}

static inline void pwm_setcmp(volatile uint32_t* pwm, uint32_t index, uint32_t value)
{
    *OFFSET(pwm, PWM_CMP0 + index * 4) = value;
}

static inline uint32_t pwm_getscale(volatile uint32_t* pwm)
{
    return *OFFSET(pwm, PWM_CFG) & 0xF;
}

static inline void pwm_setscale(volatile uint32_t* pwm, uint32_t value)
{
    *OFFSET(pwm, PWM_CFG) = value & 0xF;
}

#define PWM_CFG_IMPL(funcname, constname) \
static inline bool pwm_get##funcname(volatile uint32_t* pwm) \
{ \
    return *OFFSET(pwm, PWM_CFG) & (1 << constname); \
} \
static inline void pwm_set##funcname(volatile uint32_t* pwm, bool value) \
{ \
    if (value) { \
        *OFFSET(pwm, PWM_CFG) |= (1 << constname); \
    } else { \
        *OFFSET(pwm, PWM_CFG) &= ~(1 << constname); \
    } \
}

PWM_CFG_IMPL(sticky, PWMC_STICKY)
PWM_CFG_IMPL(zerocmp, PWMC_ZEROCMP)
PWM_CFG_IMPL(deglitch, PWMC_DEGLITCH)
PWM_CFG_IMPL(enalways, PWMC_ENALWAYS)
PWM_CFG_IMPL(enoneshot, PWMC_ENONESHOT)

#define PWM_CFG_IMPL_IDX(funcname, constname) \
static inline bool pwm_get##funcname(volatile uint32_t* pwm, uint32_t index) \
{ \
    return *OFFSET(pwm, PWM_CFG) & (1 << (constname + index)); \
} \
static inline void pwm_set##funcname(volatile uint32_t* pwm, uint32_t index, bool value) \
{ \
    if (value) { \
        *OFFSET(pwm, PWM_CFG) |= (1 << (constname + index)); \
    } else { \
        *OFFSET(pwm, PWM_CFG) &= ~(1 << (constname + index)); \
    } \
}

PWM_CFG_IMPL_IDX(cmpcenter, PWMC_CMP0CENTER)
PWM_CFG_IMPL_IDX(cmpgang, PWMC_CMP0GANG)
PWM_CFG_IMPL_IDX(cmpip, PWMC_CMP0IP)
