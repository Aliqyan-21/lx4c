#ifndef LX4C_H
#define LX4C_H

#include <stddef.h>
#include <stdlib.h>
#ifdef __cplusplus
extern "C" {
#endif

typedef enum lx4c_node_type {
  /* leaf nodes */
  LX4C_NODE_IDENT,  /*  x,y,E */
  LX4C_NODE_NUMBER, /*  42, 3.14 */
  LX4C_NODE_OP,     /*  +, =, \leq, \times */
  LX4C_NODE_TEXT,   /* \text{hello} */

  /* strcture nodes */
  LX4C_NODE_ROW,    /* seq of nodes */
  LX4C_NODE_FRAC,   /* \frac{a}{b} */
  LX4C_NODE_SQRT,   /* \sqrt{x} */
  LX4C_NODE_ROOT,   /* \sqrt[n]{x} */
  LX4C_NODE_SUP,    /* x^{n} */
  LX4C_NODE_SUB,    /* x_{i} */
  LX4C_NODE_SUBSUP, /* x_{i}^{n} */
  LX4C_NODE_OVER,   /* \overline, \vec */
  LX4C_NODE_UNDER,  /* \underline */

  LX4C_NODE_UNKNOWN, /* unrecognized token */
} lx4c_node_type;

typedef struct lx4c_node lx4c_node;
typedef struct lx4c_node {
  lx4c_node_type type;

  /* leaf value is valid for IDENT, NUMBER, OP, TEXT, UNKNOWN */
  const char *value; /* original input string pointer */
  size_t      value_len;

  /* for OP/OVER/UNDER */
  const char *symbol; /* e.g. "\leq" -> "<=", NULL if unused */

  /* children -> for valid structural nodes */
  lx4c_node **children;
  int         child_count;
} lx4c_node;

typedef enum {
  CMD_IDENT,
  CMD_OP,
  CMD_FRAC,
  CMD_SQRT,
  CMD_OVER,
  CMD_UNDER,
  CMD_VEC,
  CMD_TEXT,
  CMD_UNKNOWN,
} cmd_kind;

typedef struct {
  const char *name;
  cmd_kind    kind;
  const char *symbol;
} cmd_entry;

static cmd_entry CMD_TABLE[] = {
  /* the greek letters */
  {"alpha",      CMD_IDENT, "α"  },
  {"beta",       CMD_IDENT, "β"  },
  {"gamma",      CMD_IDENT, "γ"  },
  {"delta",      CMD_IDENT, "δ"  },
  {"epsilon",    CMD_IDENT, "ε"  },
  {"theta",      CMD_IDENT, "θ"  },
  {"lambda",     CMD_IDENT, "λ"  },
  {"mu",         CMD_IDENT, "μ"  },
  {"pi",         CMD_IDENT, "π"  },
  {"sigma",      CMD_IDENT, "σ"  },
  {"phi",        CMD_IDENT, "φ"  },
  {"omega",      CMD_IDENT, "ω"  },
  {"Gamma",      CMD_IDENT, "Γ"  },
  {"Delta",      CMD_IDENT, "Δ"  },
  {"Sigma",      CMD_IDENT, "Σ"  },
  {"Omega",      CMD_IDENT, "Ω"  },
  /* the named functions */
  {"sin",        CMD_IDENT, "sin"},
  {"cos",        CMD_IDENT, "cos"},
  {"tan",        CMD_IDENT, "tan"},
  {"log",        CMD_IDENT, "log"},
  {"ln",         CMD_IDENT, "ln" },
  {"exp",        CMD_IDENT, "exp"},
  {"max",        CMD_IDENT, "max"},
  {"min",        CMD_IDENT, "min"},
  {"lim",        CMD_IDENT, "lim"},
  /* the operators */
  {"leq",        CMD_OP,    "≤"  },
  {"geq",        CMD_OP,    "≥"  },
  {"neq",        CMD_OP,    "≠"  },
  {"times",      CMD_OP,    "×"  },
  {"cdot",       CMD_OP,    "·"  },
  {"div",        CMD_OP,    "÷"  },
  {"pm",         CMD_OP,    "±"  },
  {"infty",      CMD_OP,    "∞"  },
  {"partial",    CMD_OP,    "∂"  },
  {"nabla",      CMD_OP,    "∇"  },
  {"sum",        CMD_OP,    "∑"  },
  {"prod",       CMD_OP,    "∏"  },
  {"int",        CMD_OP,    "∫"  },
  {"approx",     CMD_OP,    "≈"  },
  {"in",         CMD_OP,    "∈"  },
  {"subset",     CMD_OP,    "⊂"  },
  {"cup",        CMD_OP,    "∪"  },
  {"cap",        CMD_OP,    "∩"  },
  {"to",         CMD_OP,    "→"  },
  {"rightarrow", CMD_OP,    "→"  },
  {"leftarrow",  CMD_OP,    "←"  },
  /* the structural */
  {"frac",       CMD_FRAC,  NULL },
  {"sqrt",       CMD_SQRT,  NULL },
  {"overline",   CMD_OVER,  "‾"  },
  {"vec",        CMD_VEC,   "→"  },
  {"underline",  CMD_UNDER, "_"  },
  {"text",       CMD_TEXT,  NULL },
};

/* Parse a latex math string, args:
 * - latex : input string, does not neet to be null-terminated
 * - len : length of input
 * - returns - root node, or NULL on allocation failure and others(?)
 */
lx4c_node *lx4c_parse(const char *latex, size_t len);

/* free the full AST returned by the above function */
void lx4c_free(lx4c_node *root);

#ifdef __cplusplus
}
#endif

#endif  //! LX4C_H
