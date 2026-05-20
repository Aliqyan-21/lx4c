#include "lx4c.h"
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>

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

/* --------------- */
/* lexer/tokenizer */
/* --------------- */

typedef struct {
  const char *src; /* the input */
  size_t      len; /* total length of input */
  size_t      pos; /* current position */
} lx4c_lexer;

/* utility functions */
static inline bool is_end(lx4c_lexer *lex) { return lex->pos >= lex->len; }
static inline void skip_whitespace(lx4c_lexer *lex) {
  while (!is_end(lex) && isspace(lex->src[lex->pos])) { lex->pos++; }
}

static void lexer_init(lx4c_lexer *lex, const char *src, size_t len) {
  lex->src = src;
  lex->len = len;
  lex->pos = 0;
}

static lx4c_token tokenize_next(lx4c_lexer *lex) {
  lx4c_token tok;
  skip_whitespace(lex);

  /* EOF */
  if (is_end(lex)) {
    tok.type  = TOK_EOF;
    tok.start = lex->src + lex->pos;
    tok.len   = 0;
    return tok;
  }

  char c    = lex->src[lex->pos];
  tok.start = lex->src + lex->pos;

  /* backlash command: \frac, \beta, ... */
  if (c == '\\') {
    lex->pos++;
    size_t cmd_start = lex->pos;
    while (lex->pos < lex->len && isalpha((unsigned char)lex->src[lex->pos])) {
      lex->pos++;
    }
    tok.type  = TOK_CMD;
    tok.start = lex->src + cmd_start;
    tok.len   = lex->pos - cmd_start;
    return tok;
  }

  return tok;
}

lx4c_node *lx4c_parse(const char *latex, size_t len) {
  lx4c_lexer lex;
  lexer_init(&lex, "\\frac", 5);
  lx4c_token tok = tokenize_next(&lex);
  printf("%zu\n", tok.len);
  printf("%s\n", tok.start);
}
