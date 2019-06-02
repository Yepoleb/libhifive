#define PIN(npin) (1ul << npin)
#define OFFSET(base, offset) ((base) + (offset) / sizeof(base))
// TODO split into compile time and runtime optimized versions
#define REGBIT_SET(reg, offset, value) \
    *(reg) = (*(reg) & ~(1 << (offset))) | ((value) << (offset));
