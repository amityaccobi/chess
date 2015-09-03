#include "List.h"

/**********************/
/****  linked list ****/
/**********************/

//adds a move-nove to a moves-linked-list
int add_node(linked_list* list, void * data, size_t data_size){
	node* new_node = (node*)malloc(sizeof(node));
	if (new_node == NULL) {
		return 0;
	}
	new_node->data = malloc(data_size);
	if (new_node == NULL) {
		free(new_node);
		return 0;
	}
	memcpy(new_node->data, data, data_size);
	new_node->next = NULL;
	if (list->len != 0)
		list->last->next = new_node;
	else // list is empty
		list->first = new_node;
	list->last = new_node;
	list->len += 1;
	return 1;
}

//concatenate 2 linked lists
void concat(linked_list* list1, linked_list list2){
	if (list2.len != 0) {
		if (list1->len != 0)
			list1->last->next = list2.first;
		else // list1 is empty
			list1->first = list2.first;
		list1->last = list2.last;
		list1->len += list2.len;
	}
}

//free linked-list allocated memory
void free_list(linked_list * del_moves){
	if (del_moves->len != 0){
		free_nodes(del_moves->first);
		del_moves->len = 0;
	}
}

//free linked-list-node's allocated memory
void free_nodes(node* del_node){
	if (del_node != NULL){
		free_nodes(del_node->next);
		free(del_node->data);
		free(del_node);
	}
}