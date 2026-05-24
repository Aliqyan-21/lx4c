#include <stdio.h>
#include "lx4c.h"
#include "lx_to_mathml.h"

int main(void) {
  char *s[] = {"\\alpha + \\beta = \\gamma", "\\frac{a}{b}",
               "\\sqrt[3]{x + y}"};
  for (int i = 0; i < 3; ++i) {
    lx4c_node *root = lx4c_parse(s[i], strlen(s[i]));
    print_ast(root);
    char *ml = lx4c_to_mathml(root, 0);
    printf("result: %s\n", ml);
    lx4c_free(root);
  }
  return 0;
}
