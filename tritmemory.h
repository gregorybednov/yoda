#ifndef TRITMEMORY_H
#define TRITMEMORY_H
#include <limits.h>
#include <stddef.h>
#include <stdint.h>

enum trit { YO = 0, YES = 1, NO = 2 };

struct ternary_pointer {
	size_t char_here;
	uint_fast16_t *here;
};

enum trit from_memory (struct ternary_pointer where);
void to_memory (struct ternary_pointer where, enum trit value);

#endif
