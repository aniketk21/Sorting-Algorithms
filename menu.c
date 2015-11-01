#include "sort_animation.h"

char *choices[] = {
		"1. Bubble Sort",
		"2. Quicksort",
		"3. Selection Sort",
		(char *)NULL,
		  };

void menu(FILE *f1, char *a) {
	int c, y_orig, x_orig, y_new, x_new, n_choices, i, selected_index, selected = 0;
	const char *ch = NULL;
	ITEM **my_items, *current_selection;
	MENU *menu;
	WINDOW *menu_window;

	fscanf(f1, "%[^\n]", a);

	init_pair(1, COLOR_CYAN, COLOR_BLACK);

	/* Create items */
	n_choices = ARRAY_SIZE(choices);
	my_items = (ITEM **)calloc(n_choices, sizeof(ITEM *));
	for(i = 0; i < n_choices; ++i)
		my_items[i] = new_item(choices[i], ch);
	
	getmaxyx(stdscr, y_orig, x_orig);
	y_new = ceil(0.3658 * y_orig); /* For placing the menu box at the center */
	x_new = ceil(0.3472 * x_orig);
	
	/* Create menu */
	menu = new_menu((ITEM **)my_items);

	/* Create the window to be associated with the menu */
	menu_window = newwin(10, 40, y_new, x_new);
	keypad(menu_window, TRUE);

	/* Set main window and sub window */
	set_menu_win(menu, menu_window);
	set_menu_sub(menu, derwin(menu_window, 6, 38, 4, 1));

	/* Set menu mark to the string " > " */
	set_menu_mark(menu, " > ");
	
	/* Print a border around the window and print a title for menu_window and info_win */
	box(menu_window, 0, 0);
	wattron(menu_window, A_BOLD);
	print_in_middle(menu_window, 1, 0, 40, "Select a Sorting Algorithm", COLOR_PAIR(1));
	wattroff(menu_window, A_BOLD);
	mvwaddch(menu_window, 2, 0, ACS_LTEE);
	mvwhline(menu_window, 2, 1, ACS_HLINE, 38);
	mvwaddch(menu_window, 2, 39, ACS_RTEE);
	wrefresh(menu_window);	

	attron(A_REVERSE);
	mvprintw(y_orig - 3, 0, "%s\n", a);
	fgetc(f1);
	fscanf(f1, "%[^\n]", a);
	mvprintw(y_orig - 2, 0, "%s\n", a);
	fgetc(f1);
	attroff(A_REVERSE);
	refresh();
        
	/* Post the menu */
	post_menu(menu);
	wrefresh(menu_window);
	
	while((c = wgetch(menu_window)) != KEY_BACKSPACE) {
		switch(c) {
			case KEY_DOWN:
				menu_driver(menu, REQ_DOWN_ITEM);
				break;
			case KEY_UP:
				menu_driver(menu, REQ_UP_ITEM);
				break;
			case ENTER: /* Enter */
			{
				current_selection = current_item(menu);
				selected_index = item_index(current_selection);
				if(selected_index == 0) {
					selected = 1;
					scr_dump("screen_state");
					clear();
					bubblesort();
					refresh();
				}
				else if(selected_index == 1) {
					selected = 1;
					scr_dump("screen_state");
					clear();
					mvprintw(10, 10, "Qsort");
					refresh();
				}
				else if(selected_index == 2) {
					selected = 1;
					scr_dump("screen_state");
					clear();
					mvprintw(10, 10, "Ssort");
					refresh();
				}
				break;
			}
		}
		if(selected) {
			scr_restore("screen_state");
			doupdate();
		}
		selected = 0;
		wrefresh(menu_window);
	}

	/* Unpost and free all the memory taken up */
	unpost_menu(menu);
	free_menu(menu);
	for(i = 0; i < n_choices; ++i)
		free_item(my_items[i]);
}		
