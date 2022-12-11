#ifndef LEXER_H_
#define LEXER_H_


#include <stdint.h>
#include <stddef.h>


typedef enum {
  TT_INTLIT,
  TT_SEMI,
  TT_STAR,
  TT_SLASH,
  TT_PLUS,
  TT_MINUS
} tokentype_t;


typedef struct {
  tokentype_t type;
  size_t val_int;
} token_t;


/* Returns 1 if there are more tokens left */
uint8_t lexer_scan(token_t* out);


#endif
