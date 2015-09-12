#include "GuiUtils.h"

//return a window (root of the UI tree)
gui_tree_node* create_window(int width, int height){
	gui_tree_node* window;
	window = (gui_tree_node*)malloc(sizeof(gui_tree_node));
	if (window == NULL){
		return NULL;
	}
	window->parent = NULL;
	window->children.len = 0;
	window->offset_x = 0;
	window->offset_y = 0;
	window->offset_rect = NULL;
	window->surface = SDL_SetVideoMode(width, height, 0, SDL_HWSURFACE | SDL_DOUBLEBUF);
	if (window->surface == NULL){
		printf("window creation failed: %s\n", SDL_GetError());
		free(window);
		return NULL;
	}
	return window;
}

//return a dialog window (child of the root of the UI tree)
int create_dialog_window(int width, int height, gui_tree_node* parent){
	gui_tree_node dialog_window = { 0 };
	dialog_window.parent = parent;
	dialog_window.children.len = 0;
	dialog_window.offset_x = 0;
	dialog_window.offset_y = 0;
	dialog_window.offset_rect = (SDL_Rect*)malloc(sizeof(SDL_Rect));
	if (dialog_window.offset_rect == NULL) {
		//printf("ERROR: failed to allocate memory.");
		free(dialog_window.surface);
		return FALSE;
	}
	dialog_window.offset_rect->x = 0;
	dialog_window.offset_rect->y = 0;
	dialog_window.offset_rect->h = height;
	dialog_window.offset_rect->w = width;

	dialog_window.surface = SDL_SetVideoMode(width, height, 0, SDL_HWSURFACE | SDL_DOUBLEBUF);
	if (dialog_window.surface == NULL){
		printf("window creation failed: %s\n", SDL_GetError());
		free(dialog_window.offset_rect);
		return FALSE;
	}
	return add_child(&dialog_window, parent);
}

//creating a pannel and connecting it to its parent in the UI tree, return if succeeded
int create_panel(int x_offset, int y_offset, int width, int height, gui_tree_node* parent){
	gui_tree_node panel;
	Uint32 rmask, gmask, bmask, amask;

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
	rmask = 0xff000000;
	gmask = 0x00ff0000;
	bmask = 0x0000ff00;
	amask = 0x000000ff;
#else
	rmask = 0x000000ff;
	gmask = 0x0000ff00;
	bmask = 0x00ff0000;
	amask = 0xff000000;
#endif

	panel.surface = SDL_CreateRGBSurface(SDL_HWSURFACE, width, height, 32, rmask, gmask, bmask, amask);
	if (panel.surface == NULL){
		return FALSE;
		//printf("error: memory allocation failed \n");
	}
	panel.children.len = 0;
	panel.offset_rect = (SDL_Rect*)malloc(sizeof(SDL_Rect));
	if (panel.offset_rect == NULL) {
		//printf("ERROR: failed to allocate memory.");
		free(panel.surface);
		return FALSE;
	}
	panel.offset_rect->x = x_offset;
	panel.offset_rect->y = y_offset;
	panel.offset_rect->h = height;
	panel.offset_rect->w = width;

	if (SDL_SetAlpha(panel.surface, 0, 0) != 0) {
		printf("ERROR: failed to set alpha properties for panel: %s\n", SDL_GetError());
	}

	return add_child(&panel, parent);
}

//creating an image and connecting it to its parent in the UI tree, return if succeeded
int create_image(Sint16 xcut, Sint16 ycut, Sint16 xlocation, Sint16 ylocation,
	Uint16 width, Uint16 height, SDL_Surface* image_surface, gui_tree_node* parent){
	gui_tree_node img;
	SDL_Rect img_rect;

	img.children.len = 0;
	img.offset_rect = (SDL_Rect*)malloc(sizeof(SDL_Rect));
	if (img.offset_rect == NULL) {
		return FALSE;
		//printf("ERROR: failed to allocate memory.");
	}
	img.offset_rect->x = xlocation;
	img.offset_rect->y = ylocation;
	img.surface = SDL_ConvertSurface(image_surface, image_surface->format, SDL_SWSURFACE);
	if (img.surface == NULL){
		free(img.offset_rect);
		return FALSE;
	}
	SDL_GetClipRect(img.surface, &img_rect);
	img_rect.x = xcut;
	img_rect.y = ycut;
	img_rect.w = width;
	img_rect.h = height;
	SDL_SetClipRect(img.surface, &img_rect);
	return add_child(&img, parent);
}

//creating a button and connecting it to its parent in the UI tree, return if succeeded
/*int create_button(Sint16 x_offset, Sint16 y_offset, Sint16 x_rect, Sint16 y_rect,
	Uint16 width, Uint16 height, SDL_Surface* image_surface, gui_tree_node* parent, int marked){
	return create_image(x_offset, y_offset, x_rect, y_rect, width, height, image_surface, parent, marked);
}*/

// draw the whole UI tree
int draw_tree(gui_tree_node * tree_root) {
	node *current_node = NULL;
	gui_tree_node *current_child = NULL;
	current_node = tree_root->children.first;
	while ((current_node != NULL) && (tree_root->children.len != 0)){
		current_child = current_node->data;
		draw_tree(current_child);
		current_node = current_node->next;
	}
	if (tree_root->parent != NULL) { //draw everything but tree's root
		if (!draw_gui_tree_node(tree_root))
			return FALSE;
	}
	return TRUE;
}

int draw_gui_tree_node(gui_tree_node* gui_tree_node) {
	if (SDL_BlitSurface(gui_tree_node->surface, &(gui_tree_node->surface->clip_rect),
						gui_tree_node->parent->surface, gui_tree_node->offset_rect) != 0) {
		SDL_FreeSurface(gui_tree_node->surface);
		//printf("ERROR: failed to blit image: %s\n", SDL_GetError());
		return FALSE;
	}
	return TRUE;
}

// check if a mouse click is inside a control
int is_inside_gui_tree_node(gui_tree_node *gui_tree_node, int x, int y) {
	if (gui_tree_node != NULL) {
		//if conrtol not error gui_tree_node
		if (gui_tree_node->offset_rect == NULL) {
			return 1;
		}
		if ((x >= gui_tree_node->offset_x) &&
			(x <= gui_tree_node->offset_x + gui_tree_node->surface->clip_rect.w) &&
			(y >= gui_tree_node->offset_y) &&
			(y <= gui_tree_node->offset_y + gui_tree_node->surface->clip_rect.h)) {
			return 1;
		}
	}
	return 0;
}