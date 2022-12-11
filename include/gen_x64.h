#ifndef GEN_X64_H_
#define GEN_X64_H_

#include <ast.h>
#include <stdio.h>


typedef int8_t reg_t;


reg_t gencode(astnode_t* node);
void gen_x64_init(void);
extern FILE* g_outfile;


#endif
