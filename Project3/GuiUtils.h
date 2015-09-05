#ifndef GUIUTILS_
#define GUIUTILS_


#include "GuiTree.h"
#include "Chess.h"

#define create_button create_image
#define ACTIVE 600
#define UNACTIVE 0

/***********************/
/**** LIST OF FUNCS ****/
/***********************/
gui_tree_node* create_window(int width, int height);
int create_panel(int x_offset, int y_offset, int width, int height, gui_tree_node* parent);
int create_image(Sint16 xcut, Sint16 ycut, Sint16 xlocation, Sint16 ylocation, Uint16 width, Uint16 height, SDL_Surface* image_surface, gui_tree_node* parent);
int draw_gui_tree_node(gui_tree_node* gui_tree_node);
int is_inside_gui_tree_node(gui_tree_node* gui_tree_node, int x, int y);
int draw_tree(gui_tree_node * tree_root);

#endif