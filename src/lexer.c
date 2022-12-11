#include <lexer.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define TO_LOWER(c) (c | (1 << 5))
#define SHOULD_IGNORE(c) (c == '\t' || c == '\f' || c == ' ' || c == '\r' || c == '\n' || c == '\0')
#define IS_DIGIT(c) (c >= '0' && c <= '9')
#define IS_ALPHA(c) (TO_LOWER(c) >= 'a' && TO_LOWER(c) <= 'z')


static size_t buf_idx = 0;
static size_t line_number = 1;
extern char* g_input_buf;  



static inline void spare(void) {
  --buf_idx;
}


static void skip_chr(void) {
  while (SHOULD_IGNORE(g_input_buf[buf_idx])) {
    if (g_input_buf[buf_idx] == '\0') {
      return;
    }

    if (g_input_buf[buf_idx] == '\n') {
      ++line_number;
    }

    ++buf_idx;
  }

  return;
}


static char get_next_char(void) {
  if (g_input_buf[buf_idx] == '\0') return '\0';
  return g_input_buf[buf_idx++];
}

static int chrpos(const char* s, char c) {
  char* p = strchr(s, c);
  return p ? p - s : -1;
}


static int scanint(char c) {
  int k = 0, val = 0;

  while ((k = chrpos("0123456789", c)) >= 0) {
    val = val * 10 + k;
    c = get_next_char();
  }

  /* Spare the non integer */
  spare();
  return val;
}


uint8_t lexer_scan(token_t* out) {
  char c;
  
  skip_chr();
  c = get_next_char();
  
  if (c == '\0') return 0;

  switch (c) {
    case ';':
      out->type = TT_SEMI;
      break;
    case '*':
      out->type = TT_STAR;
      break;
    case '/':
      out->type = TT_SLASH;
      break;
    case '+':
      out->type = TT_PLUS;
      break;
    case '-':
      out->type = TT_MINUS;
      break;
    default:
      if (IS_DIGIT(c)) {
        out->type = TT_INTLIT;
        out->val_int = scanint(c);
        printf("%d\n", out->val_int);
        break;
      }

      printf("Invalid token '%c' found while scanning (line %d)\n", c, line_number);
      exit(1);
  }

  return 1;
}
