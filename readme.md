## LX4C
A minimal LaTeX math parser in C. Parses LaTeX math notation into an AST.

## Usage
Include `lx4c.h` and `lx4c.c` in your project.

```c
lx4c_node *root = lx4c_parse("\\frac{x^{2}}{\\alpha + 1}", strlen(...));
/* walk the AST, can do what u what */
lx4c_free(root);
```

## Scope
Not a full LaTeX engine. Covers the ~20 constructs needed for technical writing:
greek letters, operators, fractions, roots, sub/superscripts, accents.

###### Code is the documentation.
