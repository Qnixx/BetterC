#include <reg.h>
#include <stdint.h>


const char* g_rregs[4] = {"r8", "r9", "r10", "r11"};
const char* g_bregs[4] = {"r8b", "r9b", "r10b", "r11b"};
static uint8_t regbmp = 0xFF;


reg_t alloc_reg(void) {
  for (int i = 0; i < sizeof(regbmp)*8; ++i) {
    if (regbmp & (1 << i)) {
      regbmp &= ~(1 << i);
      return i;
    }
  }

  return -1;
}


void free_reg(reg_t r) {
  if (r > (sizeof(regbmp)*8) - 1 || r < 0) {
    return;
  }

  regbmp |= (1 << r);
}


void freeall_regs(void) {
  regbmp = 0xFF;
}


reg_t reg_load(int64_t value) {
  reg_t r = alloc_reg();
  fprintf(g_outfile, "\tmov %s, %d\n", g_rregs[r], value);
  return r;
}


reg_t reg_add(reg_t r1, reg_t r2) {
  fprintf(g_outfile, "\tadd %s, %s\n", g_rregs[r1], g_rregs[r2]);
  free_reg(r2);
  return r1;
}


reg_t reg_sub(reg_t r1, reg_t r2) {
  fprintf(g_outfile, "\tsub %s, %s\n", g_rregs[r1], g_rregs[r2]);
  free_reg(r2);
  return r1;
}

reg_t reg_mul(reg_t r1, reg_t r2) {
  fprintf(g_outfile, "\timul %s, %s\n", g_rregs[r1], g_rregs[r2]);
  free_reg(r2);
  return r1;
}

reg_t reg_div(reg_t r1, reg_t r2) {
  fprintf(g_outfile, "\tmov rax, %s\n", g_rregs[r1]);
  fprintf(g_outfile, "\tcqo\n");
  fprintf(g_outfile, "\tidiv %s\n", g_rregs[r2]);
  fprintf(g_outfile, "\tmov %s, rax\n", g_rregs[r1]);
  free_reg(r2);
  return r1;
}

void reg_printint(reg_t r) {
  fprintf(g_outfile, "\tmov rdi, %s\n", g_rregs[r]);
  fprintf(g_outfile, "\tcall printint\n");
  free_reg(r);
}
