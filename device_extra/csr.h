typedef struct csr_mtvec
{
    trap_mode mode;
    void (*base)(void);
} csr_mtvec;

static inline csr_mtvec csr_mtvec_get()
{
    csr_mtvec result;
    uint32_t reg_value;
    __asm__ volatile("csrr %0, mtvec" : "=r"(reg_value));
    result.mode = (trap_mode)(reg_value & 3);
    result.base = (void (*)(void))(reg_value & 0xffffffffc);
    return result;
}

static inline void csr_mtvec_set(csr_mtvec values)
{
    uint32_t reg_value = 0;
    reg_value |= (uint32_t)values.mode & 3;
    reg_value |= (uint32_t)values.base & 0xffffffffc;
    __asm__ volatile("csrw mtvec, %0" :: "r"(reg_value));
}
