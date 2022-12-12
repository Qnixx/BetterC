#include <parser.h>
#include <lexer.h>
#include <ast.h>
#include <gen_x64.h>
#include <symbol.h>
#include <stdio.h>
#include <stdlib.h>
#include <diag.h>

#define DEBUG 1

static token_t last_token;
static const char* const PTYPE_STR[] = {
  [P_VOID] = "void",
  [P_U8] = "u8"
};

static inline void passert(cc_context* cc_ctx, tokentype_t tt, const char* what) {
  if (last_token.type != tt) {
    cc_diag_err(cc_ctx, "expected \"%s\"\n", what);
    exit(1);
  }
}

static void assert_type(cc_context* cc_ctx) {
  switch (last_token.type) {
    case TT_VOID:
      break;
    case TT_U8:
      break;
    default:
      cc_diag_err(cc_ctx, "expected <type>\n");
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


static SYM_PTYPE tok2ptype(tokentype_t tt) {
  switch (tt) {
    case TT_VOID:
      return P_VOID;
    case TT_U8:
      return P_U8;
    default:
      return P_VOID;
  }
}

/* Ensures there is no redefinition of a global */
static void check_glob_redef(cc_context* cc_ctx, const char* name) {
  if (lookup_glob(name) != -1) {
    cc_diag_err(cc_ctx, "redefinition of \"%s\"\n", name);
    exit(1);
  }
}


static astnode_t* compound_statement(cc_context* cc_ctx) {
  passert(cc_ctx, TT_LBRACE, "{");
  scan_token(cc_ctx);

  passert(cc_ctx, TT_RBRACE, "}");
  scan_token(cc_ctx);

  return NULL;
}


static astnode_t* function_def(cc_context* cc_ctx) {
  assert_type(cc_ctx);  /* Ensure there's a type */

  /* Get the ptype from token */
  SYM_PTYPE ptype = tok2ptype(last_token.type);
  scan_token(cc_ctx);
  
  /* Ensure there is no redefinition */
  check_glob_redef(cc_ctx, g_lex_id);

  /* Push the global symbol */
  size_t sym_id = symtbl_push_glob(g_lex_id, S_FUNCTION);
  g_symtbl[sym_id].is_global = 1;
  g_symtbl[sym_id].ptype = ptype;

  /* Verify syntax is correct */
  scan_token(cc_ctx);
  passert(cc_ctx, TT_LPAREN, "(");
  scan_token(cc_ctx);

  if (last_token.type == TT_RPAREN) {
    /*
     *  If this if statement executes then
     *  we must have gotten:
     *
     *  type func_name()
     *  and not:
     *  type func_name(void)
     *
     */
    cc_diag_warning(cc_ctx, "BetterC standard recommends: %s %s(void)\n", PTYPE_STR[ptype], g_symtbl[sym_id].name);
    scan_token(cc_ctx);
  } else if (last_token.type == TT_VOID) {
    scan_token(cc_ctx);
    passert(cc_ctx, TT_RPAREN, ")");
    scan_token(cc_ctx);
  } else {
    // TODO: Allow arguments.
    cc_diag_err(cc_ctx, "BetterC does not support arguments YET.\n");
    exit(1);
  }
  
  return mkastnode(A_FUNC, mkastleaf(A_ID, sym_id), compound_statement(cc_ctx), 0);
}

void parse(cc_context* cc_ctx) {
  int scan_ret = scan_token(cc_ctx);
  gen_x64_init();
  init_symtbls();

  while (scan_ret) {
    gencode(function_def(cc_ctx));
    // gencode(binary_expr(cc_ctx));
    scan_ret = scan_token(cc_ctx);
  }
}
