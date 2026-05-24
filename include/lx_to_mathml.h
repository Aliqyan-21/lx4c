#ifndef LX_TO_MATHML_H
#define LX_TO_MATHML_H

#include "lx4c.h"

typedef struct lx4c_visitor lx4c_visitor;

struct lx4c_visitor {
  void (*visit_ident)(lx4c_node *node, void *ctx, const lx4c_visitor *v);
  void (*visit_number)(lx4c_node *node, void *ctx, const lx4c_visitor *v);
  void (*visit_op)(lx4c_node *node, void *ctx, const lx4c_visitor *v);
  void (*visit_text)(lx4c_node *node, void *ctx, const lx4c_visitor *v);
  void (*visit_row)(lx4c_node *node, void *ctx, const lx4c_visitor *v);
  void (*visit_frac)(lx4c_node *node, void *ctx, const lx4c_visitor *v);
  void (*visit_sqrt)(lx4c_node *node, void *ctx, const lx4c_visitor *v);
  void (*visit_root)(lx4c_node *node, void *ctx, const lx4c_visitor *v);
  void (*visit_sup)(lx4c_node *node, void *ctx, const lx4c_visitor *v);
  void (*visit_sub)(lx4c_node *node, void *ctx, const lx4c_visitor *v);
  void (*visit_subsup)(lx4c_node *node, void *ctx, const lx4c_visitor *v);
  void (*visit_over)(lx4c_node *node, void *ctx, const lx4c_visitor *v);
  void (*visit_under)(lx4c_node *node, void *ctx, const lx4c_visitor *v);
  void (*visit_unknown)(lx4c_node *node, void *ctx, const lx4c_visitor *v);
};

void lx4c_accept(lx4c_node *node, const lx4c_visitor *v, void *ctx);

char *lx4c_to_mathml(lx4c_node *root);

#endif  //! LX_TO_MATHML_H
