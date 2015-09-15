#ifndef TREE_
#define TREE_

#include "List.h"
#include <SDL.h>
#include <SDL_video.h>

typedef struct gui_tree_node{
	linked_list children;
	struct gui_tree_node *parent;
	SDL_Surface* surface;
	SDL_Rect* offset_rect;
	int offset_x;
	int offset_y;
} gui_tree_node;


int add_child(gui_tree_node* child, gui_tree_node* parent);
void free_tree(gui_tree_node* parent);
void free_tree_without_root(gui_tree_node* root);


#endif