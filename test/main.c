#include <stdio.h>
#include "lx4c.h"
#include "lx_to_mathml.h"

int main(void) {
  lx4c_node *root = lx4c_parse("c + 3.14", 8);
  print_ast(root);
  char *ml = lx4c_to_mathml(root, 0);
  printf("result: %s\n", ml);
  lx4c_free(root);
  return 0;
}
