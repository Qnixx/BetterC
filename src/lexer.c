#include <lexer.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <diag.h>

#define TO_LOWER(c) (c | (1 << 5))
#define SHOULD_IGNORE(c) (c == '\t' || c == '\f' || c == ' ' || c == '\r' || c == '\n' || c == '\0')
#define IS_DIGIT(c) (c >= '0' && c <= '9')
#define IS_ALPHA(c) (TO_LOWER(c) >= 'a' && TO_LOWER(c) <= 'z')


/* Internal variables */
static size_t buf_idx = 0;

/* Internal/external variables */
const char* g_lex_id = NULL;      /* Last identifier that lexer has scanned */

/* Externs */
extern char* g_input_buf;


/* Functions */
static inline void spare(void) {
  --buf_idx;
}


static inline char peek(unsigned int n) {
  return g_input_buf[buf_idx + n];
}


static void skip_chr(cc_context* cc_ctx) {
  while (SHOULD_IGNORE(g_input_buf[buf_idx])) {
    if (g_input_buf[buf_idx] == '\0') {
      return;
    }

    if (g_input_buf[buf_idx] == '\n') {
      ++cc_ctx->current_line;
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


static uint64_t scanint(char c) {
  int k = 0, val = 0;

  while ((k = chrpos("0123456789", c)) >= 0) {
    val = val * 10 + k;
    c = get_next_char();
  }

  /* Spare the non integer */
  spare();
  return val;
}


static uint64_t scan_hex(char c) {
  c = get_next_char();
  
  char* buf = calloc(2, sizeof(char));
  size_t buf_idx = 0;
  while (IS_DIGIT(c) || IS_ALPHA(c)) {
    buf[buf_idx++] = c;
    buf = realloc(buf, sizeof(char) * (buf_idx + 2));
    c = get_next_char();
  }

  spare();

  uint64_t val = strtol(buf, NULL, 16);
  free(buf);
  return val;
}


static char* scan_identifier(char c) {
  char* buf = calloc(2, sizeof(char));
  size_t buf_idx = 0;

  while (IS_ALPHA(c) || IS_DIGIT(c) || c == '_') {
    buf[buf_idx++] = c;
    buf = realloc(buf, sizeof(char) * (buf_idx + 2));
    c = get_next_char();
  }

  spare();
  return buf;
}


static tokentype_t id(const char* id) {
  switch (*id) {
    case 'u':
      if (strcmp(id, "u8") == 0) return TT_U8;
      else return TT_ID;
    case 'v':
      if (strcmp(id, "void") == 0) return TT_VOID;
      else return TT_ID;
    case 'r':
      if (strcmp(id, "return") == 0) return TT_RETURN;
      else return TT_ID;
    case 'g':
      if (strcmp(id, "global") == 0) return TT_GLOBAL;
      else return TT_ID;
    case '_':
      if (strcmp(id, "__builtin_line") == 0) return TT_BUILTIN_LINE;
      else return TT_ID;
    default:
      return TT_ID;
  }
}


uint8_t lexer_scan(token_t* out, cc_context* cc_ctx) {
  char c;
  
  skip_chr(cc_ctx);
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
    case '(':
      out->type = TT_LPAREN;
      break;
    case ')':
      out->type = TT_RPAREN;
      break;
    case '{':
      out->type = TT_LBRACE;
      break;
    case '}':
      out->type = TT_RBRACE;
      break;
    default:
      if (IS_DIGIT(c)) {
        out->type = TT_INTLIT;

        if (get_next_char() == 'x') {
          out->val_int = scan_hex(c);
        } else {
          spare();
          out->val_int = scanint(c);
        }
        break;
      } else if (IS_ALPHA(c) || c == '_') {
        if (g_lex_id != NULL) {
          /*
           *  Free the last identifier
           *  if there was one.
           *
           */
          free((char*)g_lex_id);
        }
      
        g_lex_id = scan_identifier(c);
        out->type = id(g_lex_id);
        break;
      }
      
      cc_diag_err(cc_ctx, "invalid token found while scanning\n");
      exit(1);
  }

  return 1;
}
