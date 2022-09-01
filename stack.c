#include "stack.h"

static void here_inc(struct m *pos) {
	pos->where.char_here = (pos->where.char_here+(size_t)2)%WORD_BITS;
	if (pos->where.char_here == (size_t)0) {pos->where.here++;}
	pos->error_state = (pos->where.here == pos->upper);
}

static void here_dec(struct m *pos) {
	if (pos->where.char_here == 0 && pos->where.here == pos->lower) {
		pos->error_state = NO;
	} else {
		if (!pos->where.char_here) { pos->where.char_here = WORD_BITS; pos->where.here--; }
		pos->where.char_here -= (size_t)2;
	}
}

void push_trit(struct m *x, enum trit value) {
	to_memory(x->where, value);
	here_inc(x);
}

void pop_trit (struct m *x, enum trit *value) {
	here_dec(x);
	(*value) = from_memory(x->where);
}
