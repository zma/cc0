#include "libi0/stddef.h"

long space;

// To create a qsort task in SPACE1

space = SPACE1;
runner qsort(0, 100)
    using v[0,,100]
in space
;
