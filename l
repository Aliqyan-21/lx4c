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
}
