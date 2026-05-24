#include "lx_to_mathml.h"

void lx4c_accept(lx4c_node *node, const lx4c_visitor *v, void *ctx) {
  if (!node || !v) { return; }

  switch (node->type) {
    case LX4C_NODE_IDENT: v->visit_ident(node, ctx, v); break;
    case LX4C_NODE_NUMBER: v->visit_number(node, ctx, v); break;
    case LX4C_NODE_OP: v->visit_op(node, ctx, v); break;
    case LX4C_NODE_TEXT: v->visit_text(node, ctx, v); break;
    case LX4C_NODE_ROW: v->visit_row(node, ctx, v); break;
    case LX4C_NODE_FRAC: v->visit_frac(node, ctx, v); break;
    case LX4C_NODE_SQRT: v->visit_sqrt(node, ctx, v); break;
    case LX4C_NODE_ROOT: v->visit_root(node, ctx, v); break;
    case LX4C_NODE_SUP: v->visit_sup(node, ctx, v); break;
    case LX4C_NODE_SUB: v->visit_sub(node, ctx, v); break;
    case LX4C_NODE_SUBSUP: v->visit_subsup(node, ctx, v); break;
    case LX4C_NODE_OVER: v->visit_over(node, ctx, v); break;
    case LX4C_NODE_UNDER: v->visit_under(node, ctx, v); break;
    default: v->visit_unknown(node, ctx, v); break;
  }
}
