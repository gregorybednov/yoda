#include "stack.h"
#include "tritmemory.h"

#define INSTRUCTIONS_MAX 32

enum instruction {
	NOOP,
	NOR,
	PLUS, ZERO, MNUS,
	DUPL, DROP, SWAP,
	OUT, IN
};

struct node {
	enum instruction instructions[INSTRUCTIONS_MAX];
	struct ternary_pointer gpio[3];
	size_t ii;
	struct m stack;
};

void tick(struct node *computer);
