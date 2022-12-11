#ifndef reg_t_H_
#define reg_t_H_

#include <gen_x64.h>


reg_t reg_load(int64_t value);
reg_t reg_add(reg_t r1, reg_t r2);
reg_t reg_mul(reg_t r1, reg_t r2);
reg_t reg_sub(reg_t r1, reg_t r2);
reg_t reg_div(reg_t r1, reg_t r2);
void freeall_regs(void);
void free_reg(reg_t r);
reg_t alloc_reg(void);


extern const char* g_rregs[4];
extern const char* g_bregs[4];

#endif
