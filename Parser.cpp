#include "Parser.h"

node* new_node(int k)
{
	node* x = (node*)malloc(sizeof(node)); x->kind = k; return x;
}