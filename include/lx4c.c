#include "lx4c.h"
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

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

/* readers */
inline static void read_cmd(lx4c_lexer *lex) {
  while (lex->pos < lex->len && isalpha((unsigned char)lex->src[lex->pos])) {
    lex->pos++;
  }
}

inline static void read_number(lx4c_lexer *lex) {
  while (lex->pos < lex->len && (isdigit((unsigned char)lex->src[lex->pos]) ||
                                 lex->src[lex->pos] == '.')) {
    lex->pos++;
  }
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
    read_cmd(lex);
    tok.type  = TOK_CMD;
    tok.start = lex->src + cmd_start;
    tok.len   = lex->pos - cmd_start;
    return tok;
  }

  /* number: 42, 3.14 */
  if (isdigit((unsigned char)c)) {
    read_number(lex);
    tok.type = TOK_NUMBER;
    tok.len  = lex->pos - (tok.start - lex->src);
    return tok;
  }

  /* ident */
  if (isalpha((unsigned char)c)) {
    lex->pos++;
    tok.type = TOK_IDENT;
    tok.len  = 1;
    return tok;
  }

  /* single char tokens */
  lex->pos++;
  tok.len = 1;
  switch (c) {
    case '{': tok.type = TOK_LBRACE; break;
    case '}': tok.type = TOK_RBRACE; break;
    case '[': tok.type = TOK_LBRACKET; break;
    case ']': tok.type = TOK_RBRACKET; break;
    case '(': tok.type = TOK_LPAREN; break;
    case ')': tok.type = TOK_RPAREN; break;
    case '^': tok.type = TOK_SUP; break;
    case '_': tok.type = TOK_SUB; break;
    default: tok.type = TOK_OTHER; break;
  }

  return tok;
}

/* ----------- */
/* lx4c Parser */
/* ----------- */

typedef struct {
  lx4c_lexer lex;
  lx4c_token curr;
  lx4c_token peek;
} lx4c_parser;

static void parser_init(lx4c_parser *p, const char *src, size_t len) {
  lexer_init(&p->lex, src, len);
  p->curr = tokenize_next(&p->lex); /* first */
  p->peek = tokenize_next(&p->lex); /* second */
}

static lx4c_token parser_advance(lx4c_parser *p) {
  lx4c_token tok = p->curr;
  p->curr        = p->peek;
  p->peek        = tokenize_next(&p->lex); /* second */
  return tok;
}

lx4c_node *lx4c_parse(const char *latex, size_t len) {
  lx4c_parser p;
  parser_init(&p, latex, len);
}
