#include "GuiTree.h"

int add_child(gui_tree_node* child, gui_tree_node* parent){
	child->offset_x = parent->offset_x + child->offset_rect->x;
	child->offset_y = parent->offset_y + child->offset_rect->y;
	child->children.len = 0;
	child->parent = parent;

	return (add_node(&parent->children, child, sizeof(gui_tree_node)));

}

void free_tree(gui_tree_node* root) {
	if ((root != NULL) && (root->surface != NULL)) {
		SDL_FreeSurface(root->surface);
		free(root->offset_rect);
		free_list(&root->children, free_tree);
		free(root);
	}
}

void free_tree_without_root(gui_tree_node* root) {
	if (root != NULL) {
		free_list(&root->children, free_tree);
	}
}
