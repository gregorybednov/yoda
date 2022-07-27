#ifndef STACK_H
#define STACK_H
#include <limits.h>
#include <stddef.h>
#include <stdint.h>

/*typedef signed int word;*/
typedef intmax_t ioword;
typedef unsigned int word;
enum trit { YO = 0, YES = 1, NO = 2 };
#define WORD_BITS (CHAR_BIT*sizeof(word)-2)

struct stackpos {
	enum trit error;	/* error state of stack */
	size_t here;		/* 'here' value of stack - returns machine word WHERE TO WRITE new values */
	size_t char_here;	/* 'here' analog for bitlevel-in-machineword ops */
	size_t dl_here_here;	/* the same for delimiters stack */
	size_t depth;		/* depth of stack. required for catching overflow */
};

struct stack {
	word *mem;		/* pointer to main stack memory */
	size_t *dl_here;	/* pointer to delimiter-marked 'here' values */
	size_t *dl_char_here;	/* pointer to delimiter-marked 'char_here' values; use in pairs with dl_here */
};
extern enum trit isDelimiter (struct stack *x, struct stackpos *pos);
extern void push_trit(struct stack *x, struct stackpos *pos, enum trit value);
extern void pop_trit (struct stack *x, struct stackpos *pos, enum trit *value);
extern void push_here(struct stack *x, struct stackpos *pos);
extern void from (struct stack *src, struct stackpos *pos, ioword *dst); /* convertion from trit/bitlevel substring to 'normal' fixed-width machine word value as number in balanced ternary numeral system */
extern void to (ioword src, struct stack *dst, struct stackpos *pos); /* convertion to trit/bitlevel substring from fixed-width machine word value*/
#endif
