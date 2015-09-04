#ifndef LIST_
#define LIST_

#include <stdio.h>
#include <stdlib.h>

#define FALSE 0
#define TRUE 1

typedef struct node{
	void * data;
	struct node *next;
} node; 
//linked list node

typedef struct {
	unsigned len;
	node* first;
	node* last;
}linked_list; 
//linked list

void *memcpy(void *str1, const void *str2, size_t n);

int add_node(linked_list* list, void * data, size_t data_size);
void concat(linked_list* list1, linked_list list2);
void free_list(linked_list * del_moves, void (* free_func)());
void free_nodes(node* del_node, void(*free_func)());

#endif