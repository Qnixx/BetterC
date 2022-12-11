#include <parser.h>
#include <lexer.h>
#include <stdio.h>


static token_t last_token;


static uint8_t scan_token(void) {
  return lexer_scan(&last_token);
}


void parse(void) {
  while (scan_token());
}
