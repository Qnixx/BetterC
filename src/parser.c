#include <parser.h>
#include <lexer.h>
#include <ast.h>
#include <gen_x64.h>
#include <stdio.h>
#include <stdlib.h>

#define DEBUG 1


static token_t last_token;

static inline void passert(tokentype_t tt, const char* what) {
  if (last_token.type != tt) {
    printf("Expected \"%s\" (line %d)\n", last_token.line);
    exit(1);
  }
}

static uint8_t scan_token(void) {
  return lexer_scan(&last_token);
}


static ast_nodetype_t tok2op(tokentype_t type) {
  switch (type) {
    case TT_PLUS:
      return A_ADD;
    case TT_MINUS:
      return A_SUB;
    case TT_STAR:
      return A_MUL;
    case TT_SLASH:
      return A_DIV;
    default:
      printf("Syntax error (line %d)\n", last_token.line);
      if (DEBUG) printf("DEBUG: Invalid token in %s()\n", __func__);
      exit(1);
  }

  __builtin_unreachable();
}


static astnode_t* primary_expr(void) {
  astnode_t* n;

  switch (last_token.type) {
    case TT_INTLIT:
      n = mkastleaf(A_INTLIT, last_token.val_int);
      scan_token();
      return n;
    default:
      printf("Syntax error (line %d)\n", last_token.line);
      exit(1);
      break;
  }

  __builtin_unreachable();
}


astnode_t* binary_expr(void) {
  astnode_t* left = primary_expr();
  if (last_token.type == TT_SEMI) {
    return left;
  }

  ast_nodetype_t nodetype = tok2op(last_token.type);
  scan_token();

  astnode_t* right = binary_expr();
  passert(TT_SEMI, ";");
  return mkastnode(nodetype, left, right, 0);
}

void parse(void) {
  scan_token();
  gen_x64_init();
  gencode(binary_expr());
}
