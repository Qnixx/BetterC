#ifndef AST_H_
#define AST_H_

#include <stdint.h>

typedef enum {
  A_ADD,
  A_SUB,
  A_MUL,
  A_DIV,
  A_INTLIT,
  A_FUNC,
  A_ID,
  A_GLUE,
  A_RET,
} ast_nodetype_t;


typedef struct AstNode {
  ast_nodetype_t op;
  struct AstNode* left;
  struct AstNode* mid;
  struct AstNode* right;

  union {
    uint64_t val_int;
    uint64_t id;
  };
} astnode_t;


astnode_t* mkastnode(ast_nodetype_t op, astnode_t* left, astnode_t* mid, astnode_t* right, uint64_t val_int);
astnode_t* mkastleaf(ast_nodetype_t op, uint64_t val_int);
astnode_t* mkastunary(ast_nodetype_t op, astnode_t* left, uint64_t val_int);
void destroy_ast_nodes(void);

#endif
