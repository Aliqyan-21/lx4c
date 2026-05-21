#include "lx4c.h"
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
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

const int CMD_T_SIZE = (sizeof(CMD_TABLE) / sizeof(CMD_TABLE[0]));

static const cmd_entry *cmd_lookup(const char *name, size_t len) {
  for (size_t i = 0; i < CMD_T_SIZE; ++i) {
    if (strlen(CMD_TABLE[i].name) == len &&
        strncmp(CMD_TABLE[i].name, name, len) == 0) {
      return &CMD_TABLE[i];
    }
  }
  return NULL;
}

static lx4c_node *parse_group(lx4c_parser *p) {
  // todo: implement
  return NULL;
}

static lx4c_node *node_alloc(lx4c_node_type type) {
  lx4c_node *n = calloc(1, sizeof(lx4c_node));
  if (n) { n->type = type; }
  return n;
}

static lx4c_node *parse_atom(lx4c_parser *p) {
  lx4c_token tok = parser_advance(p);
  lx4c_node *n   = NULL;

  switch (tok.type) {
    case TOK_IDENT:
      n = node_alloc(LX4C_NODE_IDENT);
      if (!n) { return NULL; }
      n->value     = tok.start;
      n->value_len = tok.len;
      n->symbol    = NULL;
      return n;
    case TOK_NUMBER:
      n = node_alloc(LX4C_NODE_NUMBER);
      if (!n) { return NULL; }
      n->value     = tok.start;
      n->value_len = tok.len;
      return n;
    case TOK_CMD: {
      const cmd_entry *e = cmd_lookup(tok.start, tok.len);
      if (!e) {
        n = node_alloc(LX4C_NODE_UNKNOWN);
        if (!n) { return NULL; }
        n->value     = tok.start;
        n->value_len = tok.len;
        return n;
      }
      switch (e->kind) {
        case CMD_IDENT:
          n = node_alloc(LX4C_NODE_IDENT);
          if (!n) { return NULL; }
          n->value     = tok.start;
          n->value_len = tok.len;
          n->symbol    = e->symbol;
          return n;
        case CMD_OP:
          n = node_alloc(LX4C_NODE_OP);
          if (!n) { return NULL; }
          n->value     = tok.start;
          n->value_len = tok.len;
          n->symbol    = e->symbol;
          return n;
        case CMD_FRAC: {
          /* \frac{numr}{denr} */
          lx4c_node *numr = parse_group(p);
          lx4c_node *denr = parse_group(p);
          n               = node_alloc(LX4C_NODE_FRAC);
          if (!n) { return NULL; }
          n->children = malloc(2 * sizeof(lx4c_node *));
          if (!n->children) { return NULL; }
          n->children[0] = numr;
          n->children[1] = denr;
          n->child_count = 2;
          return n;
        }
        case CMD_SQRT: {
          /* \sqrt[n]{x} or \sqrt{x} */
          if (p->curr.type == TOK_LBRACKET) {
            parser_advance(p);
            lx4c_node *root_n = parse_group(p);
            lx4c_node *body   = parse_group(p);
            n                 = node_alloc(LX4C_NODE_ROOT);
            if (!n) { return NULL; }
            n->children = malloc(2 * sizeof(lx4c_node *));
            if (!n->children) { return NULL; }
            n->children[0] = body;
            n->children[1] = root_n;
            n->child_count = 2;
          } else {
            lx4c_node *body = parse_group(p);
            n               = node_alloc(LX4C_NODE_SQRT);
            if (!n) { return NULL; }
            n->children = malloc(1 * sizeof(lx4c_node *));
            if (!n->children) { return NULL; }
            n->children[0] = body;
            n->child_count = 1;
          }
          return n;
        }
        case CMD_OVER:
        case CMD_VEC: {
          lx4c_node *body = parse_group(p);
          n               = node_alloc(LX4C_NODE_OVER);
          if (!n) { return NULL; }
          n->symbol   = e->symbol;
          n->children = malloc(1 * sizeof(lx4c_node *));
          if (!n->children) { return NULL; }
          n->children[0] = body;
          n->child_count = 1;
          return n;
        }
        case CMD_UNDER: {
          lx4c_node *body = parse_group(p);
          n               = node_alloc(LX4C_NODE_UNDER);
          if (!n) { return NULL; }
          n->symbol   = e->symbol;
          n->children = malloc(1 * sizeof(lx4c_node *));
          if (!n->children) { return NULL; }
          n->children[0] = body;
          n->child_count = 1;
          return n;
        }
        case CMD_TEXT: {
          /* \text{hello} - parse raw text inside braces */
          lx4c_node *body = parse_group(p);
          n               = node_alloc(LX4C_NODE_TEXT);
          if (!n) { return NULL; }
          n->children = malloc(1 * sizeof(lx4c_node *));
          if (!n->children) { return NULL; }
          n->children[0] = body;
          n->child_count = 1;
          return n;
        }
        default: return NULL;
      }
    }
    case TOK_OTHER:
      n = node_alloc(LX4C_NODE_OP);
      if (!n) { return NULL; }
      n->value     = tok.start;
      n->value_len = tok.len;
      n->symbol    = NULL;
      return n;

    case TOK_LBRACE: return parse_group(p);
    default: return NULL;
  }
}

lx4c_node *lx4c_parse(const char *latex, size_t len) {
  static const char *tests[] = {
    "x", "42", "3.14", "\\alpha", "\\sin", "\\leq", "+", "\\unknown",
  };

  for (size_t i = 0; i < sizeof(tests) / sizeof(tests[0]); i++) {
    lx4c_parser p;
    parser_init(&p, tests[i], strlen(tests[i]));
    lx4c_node *n = parse_atom(&p);
    if (!n) {
      printf("%-12s → NULL\n", tests[i]);
      continue;
    }
    printf("%-12s → type=%-10d value=%.*s symbol=%s\n", tests[i], n->type,
           (int)n->value_len, n->value, n->symbol ? n->symbol : "NULL");
  }
  return NULL;
}
