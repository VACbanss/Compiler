#pragma once
#include <stdlib.h>

struct node
{
	int kind;
	struct node* o1, * o2, * o3;
	typedef struct node node;
	int val;
	node* new_node(int k);
};