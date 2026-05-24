#include <stdio.h>
#include "lx4c.h"
#include "lx_to_mathml.h"

int main(void) {
  char *s[] = {
    "x = \\frac{-b \\pm \\sqrt{b^2 - 4ac}}{2a}",
    "a_i^2 + b_i^2 = c_i^2",
    "\\sqrt[n]{1 + x^n}",

    "\\overline{AB}",
    "\\vec{F} = m \\cdot a",

    "\\underline{AB}",

    "\\sum_{i=0}^{n} x_i",
    "\\int_{0}^{\\infty} e^{x}",

    "\\unknown",
    "\\frac{\\unknown}{x}",

    "\\frac{\\sqrt{x^2 + y^2}}{\\sum_{i=0}^{n} a_i}",

    "\\alpha^2 + \\beta^2 = \\gamma^2",

    "E = mc^2",
  };
  int n = sizeof(s) / sizeof(s[0]);
  for (int i = 0; i < n; ++i) {
    lx4c_node *root = lx4c_parse(s[i], strlen(s[i]));
    // print_ast(root);
    char      *ml   = lx4c_to_mathml(root, 0);
    printf("%s\n", ml);
    lx4c_free(root);
    free(ml);
  }
  return 0;
}
