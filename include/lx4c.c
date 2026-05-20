#include "lx4c.h"
#include <ctype.h>
#include <stdbool.h>

typedef enum lx4c_token_type {
  TOK_IDENT,  /* single letter only: x,y, E */
  TOK_NUMBER, /* digits */
  TOK_CMD,    /* backlash command: \frac, \sqrt, \beta, etc. */
  TOK_LBRACE,
  TOK_RBRACE,
  TOK_LBRACKET,
  TOK_RBRACKET,
  TOK_LPAREN,
  TOK_RPAREN,
  TOK_SUP,   /* ^ */
  TOK_SUB,   /* _ */
  TOK_OTHER, /* any unrecognized token */
  TOK_EOF,
} lx4c_token_type;

typedef struct {
  lx4c_token_type type;
  const char     *start;
  size_t          len;
} lx4c_token;
