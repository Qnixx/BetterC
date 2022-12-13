#ifndef GEN_X64_H_
#define GEN_X64_H_

#include <ast.h>
#include <stdio.h>
#include <context.h>


typedef int8_t reg_t;


reg_t cc_x64_gen(astnode_t* node, reg_t reg, int parent_ast_top);
void cc_gen_x64_init(cc_context* _cc_ctx);
void cc_gen_x64_elf(void);
extern FILE* g_outfile;


#endif
