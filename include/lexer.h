#ifndef LEXER_H_
#define LEXER_H_


#include <stdint.h>
#include <stddef.h>
#include <context.h>


typedef enum {
  TT_INTLIT,
  TT_SEMI,
  TT_STAR,
  TT_SLASH,
  TT_PLUS,
  TT_MINUS,
  TT_ID,
  TT_VOID,
  TT_U8,
  TT_LPAREN,
  TT_RPAREN,
  TT_LBRACE,
  TT_RBRACE,
  TT_RETURN,
} tokentype_t;


typedef struct {
  tokentype_t type;
  size_t val_int;
} token_t;


/* Returns 1 if there are more tokens left */
uint8_t lexer_scan(token_t* out, cc_context* cc_ctx);
extern const char* g_lex_id;

#endif

