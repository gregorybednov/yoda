#include "stack.h"

#ifdef DEBUG
#include <stdio.h>
#endif

static int signs(size_t a, size_t b) {
	int res = 0;
	if (a<b) { res = -1; }
	if (a>b) { res =  1; }
	return res;
}

static int signi(int a, int b) {
	int res = 0;
	if (a<b) { res = -1; }
	if (a>b) { res =  1; }
	return res;
}

enum trit isDelimiter (struct stack *x, struct stackpos *pos) {
	enum trit res;
	size_t xh;
	size_t xch;
	if (pos->dl_here_here > (size_t)0) {
		xh  = x->dl_here[pos->dl_here_here-(size_t)1];
		xch = x->dl_char_here[pos->dl_here_here-(size_t)1];
	} else {
		xh = (size_t)0;
		xch = (size_t)0;
	}
	switch (signi(signs(pos->here, xh)*3+signs(pos->char_here, xch), 0)) {
		case -1:res = YO;  break;
		case  0:res = YES; break;
		case  1:res = NO;  break;
		default:res = YO; /*wat???*/ break;
	}
	return res;
}

static void here_inc(struct stackpos *pos) {
	pos->char_here = (pos->char_here+(size_t)2)%WORD_BITS;
	if (pos->char_here == (size_t)0) {pos->here++;}
	pos->error = (pos->here == pos->depth);
}

static void here_dec(struct stackpos *pos) {
	if (pos->char_here==0 && pos->here==0) {
		pos->error = NO;
	} else {
		if (!pos->char_here) { pos->char_here = WORD_BITS; pos->here--; }
		pos->char_here -= (size_t)2;
	}
}
#ifdef DEBUG
static void print_hch (char c1, struct stackpos pos, char c2) {
	assert(printf("%c%lu, %lu%c", c1, pos.here, pos.char_here,c2));
}
static void print_trit(enum trit value) {
	switch (value) {
		case YO: assert(printf("+")); break;
		case YES:assert(printf("0")); break;
		case NO: assert(printf("-")); break;
		default: assert(printf("?")); break;
	}
}
#endif

void push_trit(struct stack *x, struct stackpos *pos, enum trit value) {
	static const word b11 = 3;
	word three;
	word wval;
	three = b11;
	wval  = value;
	three = ~(three << pos->char_here);
	wval  = (wval	<< pos->char_here);
	x->mem[pos->here] = (x->mem[pos->here] & three) | wval;
	here_inc(pos);
#ifdef DEBUG
	print_trit(value);
	print_hch('(',*pos,')');
#endif
}

void pop_trit (struct stack *x, struct stackpos *pos, enum trit *value) {
	static const word b11 = 3;
	static const word b100 = 4;
	word three;
	word wval;
	three= b11;
	here_dec(pos);
	wval = x->mem[pos->here];
	three <<= pos->char_here;
	wval = wval & three;
	wval  >>= pos->char_here;
	(*value) = wval%b100;
#ifdef DEBUG
	print_trit(*value);
	print_hch('[',*pos,']');
#endif
}

void push_here(struct stack *x, struct stackpos *pos) {
	x->dl_here[pos->dl_here_here] = pos->here;
	x->dl_char_here[pos->dl_here_here] = pos->char_here;
	pos->dl_here_here++;
}

/*static int cond_no_dl (struct stack *x, struct stackpos *pos) { return (pos->here || pos->char_here); }
static int cond_dl (struct stack *x, struct stackpos *pos){ return (isDelimiter(x, pos)==NO); }
*/
void from (struct stack *src, struct stackpos *pos, ioword *dst) {/* convertion from trit/bitlevel substring to 'normal' fixed-width machine word value as number in balanced ternary numeral system */
	enum trit tmp;
	/*int (*any_delimiter)(struct stack *, struct stackpos *);*/
	*dst = 0;
	tmp = YO;
	/*any_delimiter = pos->dl_here_here ? cond_dl : cond_no_dl;*/
#ifdef DEBUG
	assert(printf("from:\t"));
#endif
	while (isDelimiter(src, pos)==NO) {
		pop_trit(src, pos, &tmp);
		(*dst) = (*dst)*3;
		if (tmp==YES) {(*dst)++;}
		if (tmp==NO)  {(*dst)--;}
	}
#ifdef DEBUG
	assert(printf("\n"));
#endif
}

void to (ioword src, struct stack *dst, struct stackpos *pos) {
	ioword a = src;
#ifdef DEBUG
	assert(printf("to:\t"));
#endif
	while (a!=0) {
		ioword r = a%3;
		a = a/3; /*+(r==2)-(r==-2);*/
		if (r==2) {a++;}
		if (r==-2) {a--;}
		if (r<0) {r+=3;}
		push_trit(dst, pos, r);
	}
#ifdef DEBUG
	assert(printf("\n"));
#endif
}
