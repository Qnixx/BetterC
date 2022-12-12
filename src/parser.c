#include <parser.h>
#include <lexer.h>
#include <ast.h>
#include <gen_x64.h>
#include <stdio.h>
#include <stdlib.h>
#include <diag.h>

#define DEBUG 1


static token_t last_token;

static inline void passert(cc_context* cc_ctx, tokentype_t tt, const char* what) {
  if (last_token.type != tt) {
    cc_diag_err(cc_ctx, "expected \"%s\"\n", what);
    exit(1);
  }
}

static uint8_t scan_token(cc_context* cc_ctx) {
  return lexer_scan(&last_token, cc_ctx);
}


static ast_nodetype_t tok2op(cc_context* cc_ctx, tokentype_t type) {
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
      cc_diag_err(cc_ctx, "syntax error\n\n");
      if (DEBUG) cc_diag_note(cc_ctx, "debug: tokentype %d\n", type);
      exit(1);
  }

  __builtin_unreachable();
}


static astnode_t* primary_expr(cc_context* cc_ctx) {
  astnode_t* n;

  switch (last_token.type) {
    case TT_INTLIT:
      n = mkastleaf(A_INTLIT, last_token.val_int);
      scan_token(cc_ctx);
      return n;
    default:
      printf("Syntax error (line %d)\n", cc_ctx->current_line);
      exit(1);
      break;
  }

  __builtin_unreachable();
}


astnode_t* binary_expr(cc_context* cc_ctx) {
  astnode_t* left = primary_expr(cc_ctx);
  if (last_token.type == TT_SEMI) {
    return left;
  }

  ast_nodetype_t nodetype = tok2op(cc_ctx, last_token.type);
  scan_token(cc_ctx);

  astnode_t* right = binary_expr(cc_ctx);
  passert(cc_ctx, TT_SEMI, ";");
  return mkastnode(nodetype, left, right, 0);
}



void parse(cc_context* cc_ctx) {
  int scan_ret = scan_token(cc_ctx);
  gen_x64_init();

  while (scan_ret) {
    scan_ret = scan_token(cc_ctx);
  }
}
