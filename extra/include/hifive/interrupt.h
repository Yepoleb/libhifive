#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>


static volatile uint32_t* const PLIC_PRIO_ADDR =    (uint32_t*)0x0C000004ul;
static volatile uint32_t* const PLIC_PENDING_ADDR = (uint32_t*)0x0C001000ul;
static volatile uint32_t* const PLIC_MIE_ADDR =     (uint32_t*)0x0C002000ul;
static volatile uint32_t* const PLIC_MITRESH_ADDR = (uint32_t*)0x0C200000ul;
static volatile uint32_t* const PLIC_MICLAIM_ADDR = (uint32_t*)0x0C200004ul;

typedef enum plic_src
{
    PLIC_SRC_AON_WDOG = 1,
    PLIC_SRC_AON_RTC = 2,
    PLIC_SRC_UART0 = 3,
    PLIC_SRC_UART1 = 4,
    PLIC_SRC_QSPI0 = 5,
    PLIC_SRC_SPI1 = 6,
    PLIC_SRC_SPI2 = 7,
    PLIC_SRC_GPIO = 8,
    PLIC_SRC_GPIO0 = 8,
    PLIC_SRC_GPIO1 = 9,
    PLIC_SRC_GPIO2 = 10,
    PLIC_SRC_GPIO3 = 11,
    PLIC_SRC_GPIO4 = 12,
    PLIC_SRC_GPIO5 = 13,
    PLIC_SRC_GPIO6 = 14,
    PLIC_SRC_GPIO7 = 15,
    PLIC_SRC_GPIO8 = 16,
    PLIC_SRC_GPIO9 = 17,
    PLIC_SRC_GPIO10 = 18,
    PLIC_SRC_GPIO11 = 19,
    PLIC_SRC_GPIO12 = 20,
    PLIC_SRC_GPIO13 = 21,
    PLIC_SRC_GPIO14 = 22,
    PLIC_SRC_GPIO15 = 23,
    PLIC_SRC_GPIO16 = 24,
    PLIC_SRC_GPIO17 = 25,
    PLIC_SRC_GPIO18 = 26,
    PLIC_SRC_GPIO19 = 27,
    PLIC_SRC_GPIO20 = 28,
    PLIC_SRC_GPIO21 = 29,
    PLIC_SRC_GPIO22 = 30,
    PLIC_SRC_GPIO23 = 31,
    PLIC_SRC_GPIO24 = 32,
    PLIC_SRC_GPIO25 = 33,
    PLIC_SRC_GPIO26 = 34,
    PLIC_SRC_GPIO27 = 35,
    PLIC_SRC_GPIO28 = 36,
    PLIC_SRC_GPIO29 = 37,
    PLIC_SRC_GPIO30 = 38,
    PLIC_SRC_GPIO31 = 39,
    PLIC_SRC_PWM0 = 40,
    PLIC_SRC_PWM0_0 = 40,
    PLIC_SRC_PWM0_1 = 41,
    PLIC_SRC_PWM0_2 = 42,
    PLIC_SRC_PWM0_3 = 43,
    PLIC_SRC_PWM1 = 44,
    PLIC_SRC_PWM1_0 = 44,
    PLIC_SRC_PWM1_1 = 45,
    PLIC_SRC_PWM1_2 = 46,
    PLIC_SRC_PWM1_3 = 47,
    PLIC_SRC_PWM2 = 48,
    PLIC_SRC_PWM2_0 = 48,
    PLIC_SRC_PWM2_1 = 49,
    PLIC_SRC_PWM2_2 = 50,
    PLIC_SRC_PWM2_3 = 51,
    PLIC_SRC_I2C = 52
} plic_src;

static inline uint32_t plic_src_offset(plic_src src)
{
    return (uint32_t)src >> 5;
}

static inline uint32_t plic_src_bitmask(plic_src src)
{
    return 1 << ((uint32_t)src & 31);
}

static inline void plic_priority_set(plic_src src, int prio)
{
    PLIC_PRIO_ADDR[(int)src] = prio;
}

static inline int plic_priority_get(plic_src src)
{
    return PLIC_PRIO_ADDR[(int)src];
}



static volatile uint32_t* const CLINT_MSIP =       (uint32_t*)0x02000000ul;
static volatile uint32_t* const CLINT_MTIMECMP_H = (uint32_t*)0x02004000ul;
static volatile uint32_t* const CLINT_MTIMECMP_L = (uint32_t*)0x02004004ul;
static volatile uint32_t* const CLINT_MTIME_H =    (uint32_t*)0x0200bFF8ul;
static volatile uint32_t* const CLINT_MTIME_L =    (uint32_t*)0x0200bFFCul;
