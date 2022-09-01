#include "tritmemory.h"

enum trit from_memory (struct ternary_pointer where) {
	static const uint_fast16_t b11 = 3;
	static const uint_fast16_t b100 = 4;
	return (((*(where.here)) & (b11 << (where.char_here))) >> where.char_here) % b100;
}

void to_memory (struct ternary_pointer where, enum trit value) {
	static const uint_fast16_t b11 = 3;
	(*(where.here)) = ((*(where.here)) & ~(b11 << where.char_here )) | (value << where.char_here);
}

/*

Older versions of push_trit and pop_trit from stack.c. Most operations implements from_memory and to_memory algorithms

void push_trit (struct m *x, enum trit value) {
	static const uint_fast16_t b11 = 3;
	uint_fast16_t three;
	uint_fast16_t wval;
	three = b11;
	wval  = value;
	three = ~(three << x->char_here);
	wval  = (wval	<< x->char_here);
	x->mem[x->here] = (x->mem[x->here] & three) | wval;
	here_inc(x);
}

void pop_trit (struct m *x, enum trit *value) {
	here_dec(x);
	(*value) = from_memory(x->char_here, x->here, x->mem);
	
	static const uint_fast16_t b11 = 3;
	static const uint_fast16_t b100 = 4;
	uint_fast16_t three;
	uint_fast16_t wval;
	three = b11;
	here_dec(x);
	wval = x->mem[x->here];
	three <<= x->char_here;
	wval = wval & three;
	wval  >>= x->char_here;
	(*value) = wval%b100;
}
*/
