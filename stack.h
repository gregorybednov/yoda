#ifndef STACK_H
#define STACK_H
#include <limits.h>
#include <stddef.h>
#include <stdint.h>
#include "tritmemory.h"
#define WORD_BITS (CHAR_BIT*sizeof(uint_fast16_t)-2)
#define STACK_MAX 10
struct m {
	enum trit error_state;
	uint_fast16_t *lower;
	uint_fast16_t *upper;
	struct ternary_pointer where;
	uint_fast16_t mem[STACK_MAX];
};

void push_trit(struct m *x, enum trit value);
void pop_trit (struct m *x, enum trit *value);
#endif
