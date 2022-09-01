#include "node.h"

void tick(struct node *computer) {
	enum trit x;
	enum trit y; 
	switch (computer->instructions[computer->ii]) {
		case NOOP: case PLUS: case ZERO: case MNUS: case THEN: break;
		case NOR: case SWAP:			/* ATTENTION!!! TWO-ARGUMENTS OPERANDS ARE REVERSED!!!! */
			pop_trit(&(computer->stack), &y); 
		case DUPL: case DROP: case IN: case OUT: case IF_YES: case IF_NO: case IF_UNK:
			pop_trit(&(computer->stack), &x);
			break;
	}
	
	switch (computer->instructions[computer->ii]) {
		case NOOP: case DROP: break;
		case DUPL:
			y = x; break;
		case SWAP:
			{
				enum trit tmp = x;
				x = y;
				y = tmp;
			}
			break;
		case PLUS: x = YES; break;
		case MNUS: x =  NO; break;
		case ZERO: x =  YO; break;
		case OUT: to_memory(computer->gpio[y], x); break;
		case NOR: 
			x = (unsigned)(!!(x%2+y%2)<<1) | ((x+y)/4); /* a bit of Dark Side magic. Of course, tested on every combination of {0, 1, 2} */
			break;
		case IN:	x = from_memory(computer->gpio[x]); break;
		case IF_YES:	/*TODO*/
		case IF_NO:	/*TODO*/
		case IF_UNK:	/*TODO*/
		case THEN:	/*TODO*/
			break;
	}
	
	switch (computer->instructions[computer->ii]) {
		case NOOP: case DROP: case OUT: case IF_YES: case IF_NO: case IF_UNK: case THEN: break;
		case DUPL: case SWAP:				/*ATTENTION!!! TWO-RESULT OPERATIONS ARE REVERSED */
			push_trit(&(computer->stack), y);	
		case NOR: case PLUS: case ZERO: case MNUS: case IN:
			push_trit(&(computer->stack), x);	
			break;
	}
	computer->ii++;
	computer->ii %= INSTRUCTIONS_MAX;
}
