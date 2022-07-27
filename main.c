#include <assert.h>
#include "stack.h"

#define ds_depth 1000
#define ds_dl_depth 1000
#define threaded_depth 2000
#define threaded_dl_depth 2000
#define rs_depth 1000
#define rs_dl_depth 1000
#define EXTENDED

static word   threaded_mem[threaded_depth];
static size_t threaded_dl_here[threaded_depth];
static size_t threaded_dl_char_here[threaded_dl_depth];
static struct stack threaded = {threaded_mem, threaded_dl_here, threaded_dl_char_here};
static struct stackpos threaded_pos = {YO, 0, 0, 0, threaded_depth};

static word   rs_mem[rs_depth];
static size_t rs_dl_here[rs_dl_depth];
static size_t rs_dl_char_here[rs_dl_depth];
static struct stack rs = {rs_mem, rs_dl_here, rs_dl_char_here};
static struct stackpos rs_pos = {YO, 0, 0, 0, rs_depth};

static word   ds_mem[ds_depth];
static size_t ds_dl_here[ds_dl_depth];
static size_t ds_dl_char_here[ds_dl_depth];
static struct stack ds = {ds_mem, ds_dl_here, ds_dl_char_here};
static struct stackpos ds_pos = {YO, 0, 0, 0, ds_depth};

static struct stackpos addrinterpret;

static void and_f (void) {
	enum trit x;
	enum trit y;
	pop_trit(&ds, &ds_pos, &x);
	pop_trit(&ds, &ds_pos, &y);
	if ((x==YES) && (y==YES)){push_trit(&ds, &ds_pos, YES);}
	if ((x==NO) || (y==NO))  {push_trit(&ds, &ds_pos, NO);}
	if (((x==YO) && (y!=NO)) || ((y==YO) && (x!=NO))){push_trit(&ds, &ds_pos, YO);}
}

static void or_f(void) {
	enum trit x;
	enum trit y;
	pop_trit(&ds, &ds_pos, &x);
	pop_trit(&ds, &ds_pos, &y);
	if ((x==NO) && (y==NO)){push_trit(&ds, &ds_pos, NO);}
	if ((x==YES) || (y==YES))  {push_trit(&ds, &ds_pos, YES);}
	if (((x==YO) && (y!=YES)) || ((y==YO) && (x!=YES))){push_trit(&ds, &ds_pos, YO);}
}

static void not_f (void) {
	enum trit x;
	pop_trit(&ds, &ds_pos, &x);
	switch (x) {
		case NO:  push_trit(&ds, &ds_pos, YES); break;
		case YO:  push_trit(&ds, &ds_pos, YO); 	break;
		case YES: push_trit(&ds, &ds_pos, YES); break;
		default: /* that's an error! */ 	break;
	}
}

static void dup (void) {
	enum trit x;
	pop_trit(&ds, &ds_pos, &x);
	push_trit(&ds, &ds_pos, x);
	push_trit(&ds, &ds_pos, x);
}

static void drop (void) {
	enum trit x;
	pop_trit(&ds, &ds_pos, &x);
}

static void rot_plus(void) { /* (a b c -- c a b) */
	enum trit a;
	enum trit b;
	enum trit c;
	pop_trit(&ds, &ds_pos, &c);
	pop_trit(&ds, &ds_pos, &b);
	pop_trit(&ds, &ds_pos, &a);
	push_trit(&ds, &ds_pos, c);
	push_trit(&ds, &ds_pos, a);
	push_trit(&ds, &ds_pos, b);
}

static void is_delimiter(void) {
	/* NOT popping anything from stack!!! */
	push_trit(&ds, &ds_pos, isDelimiter(&ds, &ds_pos));
}

static void exit_rs(void) {
	ioword hch;
	if (rs_pos.dl_here_here!=(size_t)0) { --rs_pos.dl_here_here; }
	from(&rs, &rs_pos, &hch);
	addrinterpret.here = ((size_t)hch)/WORD_BITS;
	addrinterpret.char_here = ((size_t)hch)%WORD_BITS;
}

static void compile_raw_f (void (*f)(void)) {
	union {
		ioword x;
		void (*ptr)(void);
	} ptr2ioword;
	ptr2ioword.ptr = f;
	to(ptr2ioword.x, &threaded, &threaded_pos);
	push_trit(&threaded, &threaded_pos, YES);
	push_here(&threaded, &threaded_pos);
}

static void push_value (struct stack *x, struct stackpos *pos, ioword value) {
	to(value, x, pos);
	push_here(x, pos);
}

#ifdef EXTENDED
#include <stdio.h>
static void simply_print(FILE* where, char *str) {
	size_t i;
	i = 0;
	while ((str[i] != EOF) && (str[i] != '\0')) {
		putc(str[i], where);
		i++;
	}
}

static void print(void) {
	enum trit x;
	pop_trit(&ds, &ds_pos, &x);
	switch (x) {
		case NO:  assert(putc('-', stdout)!=EOF); break;
		case YO:  assert(putc('0', stdout)!=EOF); break;
		case YES: assert(putc('+', stdout)!=EOF); break;
		default:  assert(putc('?', stdout)!=EOF); assert(0); break;
	}
}
#endif

static void call(struct stackpos *adin) {
	enum trit x;
	int stopflag = 0;
	union {
		ioword x;
		void (*ptr)(void);
	} ptr2ioword;
	adin->dl_here_here--;
	pop_trit(&threaded, adin, &x);
	{ /* TODO too raw and crude method to catching errors. it's hardly recommended to intergrate it into stack module */
		if (adin->error == YES) {
#ifdef EXTENDED
			char msg[]="address interpretor overflow!\0";
			simply_print(stderr, msg);
#endif
			stopflag=1;
		}
		if (adin->error == NO) {
#ifdef EXTENDED
			char msg[]="address interpretor underflow!\0";
			simply_print(stderr, msg);
#endif
			stopflag=1;
		}
		if (rs_pos.error == YES) {
#ifdef EXTENDED
			char msg[]="return stack overflow!\0";
			simply_print(stderr, msg);
#endif
			stopflag=1;
		}
		if (rs_pos.error == NO)  {
#ifdef EXTENDED
			char msg[]="return stack underflow!\0";
			simply_print(stderr, msg);
#endif
			stopflag=1;
		}
	}
	if (!stopflag){
	switch (x) {
		case YES:/* machine-coded word */
			from(&threaded, adin, &ptr2ioword.x);
			ptr2ioword.ptr();
			break;
		case YO:/* threaded instruction */
			push_value(&rs, &rs_pos, adin->here*WORD_BITS+adin->char_here);
			from(&threaded, adin, &ptr2ioword.x);
			adin->here = (size_t)ptr2ioword.x/WORD_BITS;
			adin->char_here = (size_t)ptr2ioword.x%WORD_BITS;
			break;
		case NO:/* literal */
			while (isDelimiter(&threaded, adin)==NO) {
				enum trit tmp;
				pop_trit(&threaded, adin, &tmp);
				push_trit(&ds, &ds_pos, tmp);
			}
			break;
		default:
			break;
	}
	}
}


static void make_threaded(void) {
	size_t i;
	static void (*core[])(void) = { exit_rs, dup, exit_rs, drop, exit_rs, and_f, exit_rs, or_f, exit_rs, not_f, exit_rs, rot_plus, exit_rs, is_delimiter, exit_rs, print, NULL };
	i = 0;
	while (core[i]!=NULL) {
		compile_raw_f(core[i]);
		i++;
	}
}

int main(void) {
	make_threaded();
	push_value(&rs, &rs_pos, threaded_pos.here*WORD_BITS+threaded_pos.char_here);
	exit_rs();
	addrinterpret.dl_here_here = threaded_pos.dl_here_here;
	call(&addrinterpret);
	call(&addrinterpret);
	return 0;
}
