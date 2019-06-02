#include <stdint.h>
#include <stdbool.h>
#include <assert.h>

#include "clocks.h"
#include "hifive-common.h"


// PRCI Clocks

#define PRCI_BASE 0x10008000

// HFROSC

static volatile uint32_t* const PRCI_HFROSCCFG = (uint32_t*)(PRCI_BASE + 0x00);

#define PRCI_HFROSCEN 30
#define PRCI_HFROSCRDY 31

uint32_t hfrosc_getdiv(void)
{
    return (*PRCI_HFROSCCFG & 63) + 1;
}

void hfrosc_setdiv(uint32_t div)
{
    assert(div > 1 && div <= 64);
    *PRCI_HFROSCCFG = (*PRCI_HFROSCCFG & ~63) | (div - 1);
}

uint32_t hfrosc_gettrim(void)
{
    return (*PRCI_HFROSCCFG >> 16) & 31;
}

void hfrosc_settrim(uint32_t trim)
{
    assert(trim < 31);
    *PRCI_HFROSCCFG = (*PRCI_HFROSCCFG & ~(31 << 16)) | (trim << 16);
}

bool hfrosc_geten(void)
{
    return *PRCI_HFROSCCFG & (1 << PRCI_HFROSCEN);
}

void hfrosc_seten(bool enabled)
{
    REGBIT_SET(PRCI_HFROSCCFG, PRCI_HFROSCEN, enabled);
}

bool hfrosc_getrdy(void)
{
    return *PRCI_HFROSCCFG & (1 << PRCI_HFROSCRDY);
}

// HFXOSC

static volatile uint32_t* const PRCI_HFXOSCCFG = (uint32_t*)(PRCI_BASE + 0x04);

#define PRCI_HFXOSCEN 30
#define PRCI_HFXOSCRDY 31

bool hfxosc_geten(void)
{
    return *PRCI_HFXOSCCFG & (1 << PRCI_HFXOSCEN);
}

void hfxosc_seten(bool enabled)
{
    REGBIT_SET(PRCI_HFXOSCCFG, PRCI_HFXOSCEN, enabled);
}

bool hfxosc_getrdy(void)
{
    return *PRCI_HFXOSCCFG & (1 << PRCI_HFXOSCEN);
}

// PLL

static volatile uint32_t* const PRCI_PLLCFG = (uint32_t*)(PRCI_BASE + 0x08);
static volatile uint32_t* const PRCI_PLLOUTDIV = (uint32_t*)(PRCI_BASE + 0x0C);

#define PRCI_PLLSEL 16
#define PRCI_PLLREFSEL 17
#define PRCI_PLLBYPASS 18
#define PRCI_PLLLOCK 31

#define PRCI_PLLOUTDIVBY1 8

uint32_t pll_getr(void)
{
    return *PRCI_PLLCFG & 3;
}

void pll_setr(uint32_t r)
{
    assert(r <= 4);
    *PRCI_PLLCFG = (*PRCI_PLLCFG & ~3) | r;
}

uint32_t pll_getf(void)
{
    uint32_t regvalue = (*PRCI_PLLCFG >> 4) & 63;
    return (regvalue + 1) << 1;
}

void pll_setf(uint32_t f)
{
    assert(((f & 1) == 0) && (f <= 128));
    uint32_t regvalue = (f - 1) >> 1;
    *PRCI_PLLCFG = (*PRCI_PLLCFG & ~(63 << 4)) | (regvalue << 4);
}

uint32_t pll_getq(void)
{
    uint32_t regvalue = (*PRCI_PLLCFG >> 10) & 3;
    switch (regvalue) {
    case 1: return 2;
    case 2: return 4;
    case 3: return 8;
    default: return 0;
    }
}

void pll_setq(uint32_t q)
{
    assert(q == 2 || q == 4 || q == 8);
    uint32_t regvalue;
    switch (q)
    {
    case 2: regvalue = 1; break;
    case 4: regvalue = 2; break;
    case 8:
    default: regvalue = 3; break;
    }
    *PRCI_PLLCFG = (*PRCI_PLLCFG & ~(3 << 10)) | (regvalue << 10);
}

bool pll_getsel(void)
{
    return *PRCI_PLLCFG & (1 << PRCI_PLLSEL);
}

void pll_setsel(bool selected)
{
    REGBIT_SET(PRCI_PLLCFG, PRCI_PLLSEL, selected);
}

enum PllSource pll_getrefsel(void)
{
    return (*PRCI_PLLCFG >> PRCI_PLLREFSEL) & 1;
}

void pll_setrefsel(enum PllSource source)
{
    REGBIT_SET(PRCI_PLLCFG, PRCI_PLLREFSEL, source);
}

bool pll_getbypass(void)
{
    return *PRCI_PLLCFG & (1 << PRCI_PLLBYPASS);
}

void pll_setbypass(bool bypass)
{
    REGBIT_SET(PRCI_PLLCFG, PRCI_PLLBYPASS, bypass);
}

bool pll_getlock(void)
{
    return *PRCI_PLLCFG & (1 << PRCI_PLLLOCK);
}

uint32_t pll_getoutdiv(void)
{
    uint32_t regvalue = *PRCI_PLLOUTDIV;
    if (regvalue & (1 << PRCI_PLLOUTDIVBY1)) {
        return 1;
    } else {
        return ((regvalue & 63) + 1) << 1;
    }
}

void pll_setoutdiv(uint32_t divider)
{
    assert(divider == 1 || ((divider & 1) == 0 && divider <= 128));
    if (divider == 1) {
        *PRCI_PLLOUTDIV = (1 << PRCI_PLLOUTDIVBY1);
    } else {
        *PRCI_PLLOUTDIV = (divider >> 1) - 1;
    }
}

// Process monitor configuration - undocumented

//static volatile uint32_t* const PRCI_PROCMONCFG = (uint32_t*)(PRCI_BASE + 0xF0);

// AON Clocks

#define AON_BASE 0x10000000

// RTC

static volatile uint32_t* const RTC_COUNTHI = (uint32_t*)(AON_BASE + 0x4C);
static volatile uint32_t* const RTC_COUNTLO = (uint32_t*)(AON_BASE + 0x48);
static volatile uint32_t* const RTC_CFG = (uint32_t*)(AON_BASE + 0x40);
static volatile uint32_t* const RTC_CMP0 = (uint32_t*)(AON_BASE + 0x60);

#define RTC_SCALE 0
#define RTC_ENALWAYS 12
#define RTC_IP0 28

uint64_t rtc_getcounter(void)
{
    return (uint64_t)(*(RTC_COUNTHI)) << 32 | *RTC_COUNTLO;
}

void rtc_setcounter(uint64_t counter)
{
    *RTC_COUNTHI = counter >> 32;
    *RTC_COUNTLO = counter & 0xFFFFFFFF;
}

uint32_t rtc_getscale(void)
{
    return *RTC_CFG & 0xF;
}

void rtc_setscale(uint32_t scale)
{
    assert(scale < 16);
    *RTC_CFG = (*RTC_CFG & ~0xF) | scale;
}

bool rtc_getenalways(void)
{
    return *RTC_CFG & (1 << RTC_ENALWAYS);
}

void rtc_setenalways(bool enabled)
{
    REGBIT_SET(RTC_CFG, RTC_ENALWAYS, enabled);
}

bool rtc_getip(uint32_t index)
{
    assert(index == 0);
    return *RTC_CFG & (1 << RTC_IP0);
}

void rtc_setip(uint32_t index, bool pending)
{
    assert(index == 0);
    REGBIT_SET(RTC_CFG, RTC_IP0, pending);
}

uint32_t rtc_getcmp(uint32_t index)
{
    assert(index == 0);
    return *RTC_CMP0;
}

void rtc_setcmp(uint32_t index, uint32_t cmp)
{
    assert(index == 0);
    *RTC_CMP0 = cmp;
}

// LFROSC

static volatile uint32_t* const LFROSC_CFG = (uint32_t*)(AON_BASE + 0x70);

#define LFROSC_EN 30
#define LFROSC_RDY 31

uint32_t lfrosc_getdiv(void)
{
    return (*LFROSC_CFG & 63) + 1;
}

void lfrosc_setdiv(uint32_t divider)
{
    // TODO: Figure out how this really works because it's undocumented
    assert(divider > 0 && divider <= 64);
    *LFROSC_CFG = (*LFROSC_CFG & 63) | (divider - 1);
}

uint32_t lfrosc_gettrim(void)
{
    return (*LFROSC_CFG >> 16) & 31;
}

void lfrosc_settrim(uint32_t trim)
{
    *LFROSC_CFG = (*LFROSC_CFG & ~(31 << 16)) | (trim << 16);
}

bool lfrosc_geten(void)
{
    return *LFROSC_CFG & (1 << LFROSC_EN);
}

void lfrosc_seten(bool enabled)
{
    REGBIT_SET(LFROSC_CFG, LFROSC_EN, enabled);
}

bool lfrosc_getrdy(void)
{
    return *LFROSC_CFG & (1 << LFROSC_RDY);
}

// LFCLK_MUX

static volatile uint32_t* const LFCLK_MUX = (uint32_t*)(AON_BASE + 0x7C);

#define LFCLK_SEL 0
#define LFCLK_MUX_STATUS 31

enum LfclkSource lfclk_getsel(void)
{
    return *LFCLK_MUX & 1;
}

void lfclk_setsel(enum LfclkSource source)
{
    REGBIT_SET(LFCLK_MUX, LFCLK_SEL, source);
}

enum LfclkSource lfclk_getpin(void)
{
    return ((*LFCLK_MUX >> 31) & 1) ^ 1; // Internal clock is high
}

