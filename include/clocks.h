#pragma once
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>

// PRCI Clocks

// HFROSC - Internal high frequency ring oscillator - 72Mhz
// Default clock

uint32_t hfrosc_getdiv(void);
void hfrosc_setdiv(uint32_t div);
uint32_t hfrosc_gettrim(void);
void hfrosc_settrim(uint32_t trim);
bool hfrosc_geten(void);
void hfrosc_seten(bool enabled);
bool hfrosc_getrdy(void);

// HFXOSC - External high frequency quartz oscillator - 16Mhz

bool hfxosc_geten(void);
void hfxosc_seten(bool enabled);
bool hfxosc_getrdy(void);

// PLL - Internal high frequency PLL

enum PllSource
{
    PLLSOURCE_HFROSC = 0, ///< Internal HF Oscillator
    PLLSOURCE_HFXOSC = 1 ///< Eternal HF Oscillator
};

uint32_t pll_getr(void);
void pll_setr(uint32_t r);
uint32_t pll_getf(void);
void pll_setf(uint32_t f);
uint32_t pll_getq(void);
void pll_setq(uint32_t q);
bool pll_getsel(void);
void pll_setsel(bool selected);
enum PllSource pll_getrefsel(void);
void pll_setrefsel(enum PllSource source);
bool pll_getbypass(void);
void pll_setbypass(bool bypass);
bool pll_getlock(void);
uint32_t pll_getoutdiv(void);
void pll_setoutdiv(uint32_t divider);

// AON Clocks

// RTC - Realtime clock - 32.768kHz

uint64_t rtc_getcounter(void);
void rtc_setcounter(uint64_t counter);
uint32_t rtc_getscale(void);
void rtc_setscale(uint32_t scale);
bool rtc_getenalways(void);
void rtc_setenalways(bool enabled);
bool rtc_getip(uint32_t index);
void rtc_setip(uint32_t index, bool pending);
uint32_t rtc_getcmp(uint32_t index);
void rtc_setcmp(uint32_t index, uint32_t cmp);

// LFROSC - Internal low frequency ring oscillator - 32kHz

uint32_t lfrosc_getdiv(void);
void lfrosc_setdiv(uint32_t divider);
uint32_t lfrosc_gettrim(void);
void lfrosc_settrim(uint32_t trim);
bool lfrosc_geten(void);
void lfrosc_seten(bool enabled);
bool lfrosc_getrdy(void);

// LFCLK_MUX - Low frequency clock mux

enum LfclkSource
{
    LFCLK_LFROSC = 0, ///< Internal LF Oscillator
    LFCLK_EXT = 1 ///< External LF Oscillator
};

enum LfclkSource lfclk_getsel(void);
void lfclk_setsel(enum LfclkSource source);
enum LfclkSource lfclk_getpin(void);

// See also: Hardware performance monitor (mcycle)
