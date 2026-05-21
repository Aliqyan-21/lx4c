#include "lx4c.h"

int main(void) {
  lx4c_node *root = lx4c_parse("\\frac{x^{2}}{\\alpha + 3.14}", 27);
  print_ast(root);
  lx4c_free(root);
  return 0;
}
