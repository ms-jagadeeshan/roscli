#include <curses.h>
#include <stdlib.h>
#include <menu.h>

#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))
#define CTRLD 4

char *choices[] = {
	"1",
	"2",
	"3",
	"4",
	"5",
	"6",
	"7",
	"8",
	"9",
	"10",
	"11",
	"12",
	"13",
	"14",
	"15",
	"16",
	(char *)NULL,
};

int main()
{
	ITEM **my_items;
	int c;
	MENU *my_menu;
	WINDOW *my_menu_win;
	WINDOW *tmp_win;
	int n_choices, i;

	/* Initialize curses */
	initscr();
	start_color();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);
	init_pair(1, COLOR_RED, COLOR_BLACK);
	init_pair(2, COLOR_CYAN, COLOR_BLACK);

	/* Create items */
	n_choices = ARRAY_SIZE(choices);
	my_items = (ITEM **)calloc(n_choices, sizeof(ITEM *));
	for (i = 0; i < n_choices; ++i)
		my_items[i] = new_item(choices[i], choices[i]);

	/* Crate menu */
	my_menu = new_menu((ITEM **)my_items);

	/* Set menu option not to show the description */
	menu_opts_off(my_menu, O_SHOWDESC);

	/* Create the window to be associated with the menu */
	my_menu_win = newwin(10, 30, 0, 0);
	keypad(my_menu_win, TRUE);

	tmp_win = newwin(40, 70, 0, 30);

	/* Set main window and sub window */
	set_menu_win(my_menu, my_menu_win);
	set_menu_sub(my_menu, derwin(my_menu_win, 6, 28, 3, 2));
	set_menu_format(my_menu, 5, 3);
	set_menu_mark(my_menu, "> ");

	/* Print a border around the main window and print a title */
	box(my_menu_win, 0, 0);
	box(tmp_win, 0, 0);

	attron(COLOR_PAIR(2));
	mvprintw(LINES - 5, 0, "TAB - Change focus");
	mvprintw(LINES - 4, 0, "PageUp/PageDown - scroll");
	mvprintw(LINES - 3, 0, "Arrow Keys - navigation");
	mvprintw(LINES - 2, 0, "(q - Exit)");
	attroff(COLOR_PAIR(2));
	refresh();

	mvwprintw(my_menu_win, 1, 1, "Select the layout:");

	/* Post the menu */
	post_menu(my_menu);
	wrefresh(my_menu_win);

	scrollok(tmp_win, TRUE);
	keypad(tmp_win, TRUE);
	char *layout[] = {"1)               2)               3)",
					  "+---------+      +---------+      +----+----+",
					  "|         |      |         |      |    |    |",
					  "|         |      +---------+      |    |    |",
					  "|         |      |         |      |    |    |",
					  "+---------+      +---------+      +----+----+",
					  "",
					  "4)               5)               6)",
					  "+---------+      +----+----+      +----+----+",
					  "|         |      |    |    |      |    |    |",
					  "+----+----+      +----+----+      |    +----+",
					  "|    |    |      |         |      |    |    |",
					  "+----+----+      +---------+      +----+----+",
					  "",
					  "7)               8)               9)",
					  "+----+----+      +---------+      +-+-+-+-+",
					  "|    |    |      +---------+      | | | | |",
					  "+----+    |      +---------+      | | | | |",
					  "|    |    |      +---------+      | | | | |",
					  "+----+----+      +---------+      +-+-+-+-+",
					  "",
					  "10)               11)              12)",
					  "+----+----+      +---------+      +---------+",
					  "|    |    |      |         |      +---------+",
					  "+----+----+      +----+----+      +----+----+",
					  "+---------+      |    +----+      |    |    |",
					  "+---------+      +----+----+      +----+----+",
					  "",
					  "13)               14)             15)",
					  "+--+--+----+      +----+--+--+      +----+----+",
					  "|  |  |    |      |    |  |  |      |    |    |",
					  "|  |  +----+      +----+  |  |      +----+----+",
					  "|  |  |    |      |    |  |  |      |    |    |",
					  "+--+--+----+      +----+--+--+      +----+----+",
					  "",
					  "16)",
					  "CUSTOM"};
	int layout_arr_size = ARRAY_SIZE(layout);
	// mvwprintw(tmp_win, 3, 3, layout);
	int x_offset = 0;
	for (int i = x_offset; i < layout_arr_size; i++)
	{
		mvwprintw(tmp_win, i + 1 - x_offset, 3, layout[i]);
	}
	wrefresh(tmp_win);

	WINDOW *current_win = my_menu_win;
	while ((c = wgetch(current_win)) != 'q')
	{
		if (c == 10)
		{
			int option = atoi(item_name(current_item(my_menu)));
			fprintf(stderr, "%s", item_name(current_item(my_menu)));
			unpost_menu(my_menu);
			free_menu(my_menu);
			for (i = 0; i < n_choices; ++i)
				free_item(my_items[i]);
			endwin();
			return option;
			break;
		}
		if (c == 9)
		{
			if (current_win == my_menu_win)
			{
				current_win = tmp_win;
				mvwprintw(tmp_win, 0, 1, "FOCUSED");
				for (int i = 0; i < 7; i++)
					mvwaddch(my_menu_win, 0, i + 1, ((acs_map[(unsigned char)(('q'))])));
			}
			else
			{
				current_win = my_menu_win;
				mvwprintw(my_menu_win, 0, 1, "FOCUSED");

				for (int i = 0; i < 7; i++)
					mvwaddch(tmp_win, 0, i + 1, ((acs_map[(unsigned char)(('q'))])));
			}
			wrefresh(tmp_win);
			wrefresh(my_menu_win);
			continue;
		}
		if (current_win == tmp_win)
		{
			switch (c)
			{
			case KEY_DOWN:
			case 'j':
				x_offset += 1;
				if (x_offset >= layout_arr_size)
				{
					x_offset = 0;
				}
				werase(tmp_win);
				box(tmp_win, 0, 0);
				for (int i = x_offset; i < layout_arr_size; i++)
				{
					mvwprintw(tmp_win, i + 1 - x_offset, 3, layout[i]);
				}
				break;
			case KEY_UP:
			case 'k':
				x_offset -= 1;
				if (x_offset < 0)
				{
					x_offset = layout_arr_size - 1;
				}
				werase(tmp_win);
				box(tmp_win, 0, 0);
				for (int i = x_offset; i < layout_arr_size; i++)
				{
					mvwprintw(tmp_win, i + 1 - x_offset, 3, layout[i]);
				}
				break;
			case KEY_NPAGE:
				x_offset += LINES - 1;
				if (x_offset >= layout_arr_size)
				{
					if (LINES > layout_arr_size)
						x_offset = 0;
					else
						x_offset = layout_arr_size - LINES;
				}
				werase(tmp_win);
				box(tmp_win, 0, 0);
				for (int i = x_offset; i < layout_arr_size; i++)
				{
					mvwprintw(tmp_win, i + 1 - x_offset, 3, layout[i]);
				}
				break;
			case KEY_PPAGE:
				x_offset -= LINES - 1;
				if (x_offset < 0)
				{
					x_offset = 0;
				}
				werase(tmp_win);
				box(tmp_win, 0, 0);
				for (int i = x_offset; i < layout_arr_size; i++)
				{
					mvwprintw(tmp_win, i + 1 - x_offset, 3, layout[i]);
				}
				break;
			}

			/**
			 * @brief "Focused" text display
			 *
			 */
			mvwprintw(tmp_win, 0, 1, "FOCUSED");
			for (int i = 0; i < 7; i++)
				mvwaddch(my_menu_win, 0, i + 1, ((acs_map[(unsigned char)(('q'))])));
		}
		else
		{
			/**
			 * @brief "Focused" text display
			 *
			 */
			mvwprintw(my_menu_win, 0, 1, "FOCUSED");
			for (int i = 0; i < 7; i++)
				mvwaddch(tmp_win, 0, i + 1, ((acs_map[(unsigned char)(('q'))])));

			switch (c)
			{
			case KEY_DOWN:
			case 'j':
				menu_driver(my_menu, REQ_DOWN_ITEM);
				break;
			case KEY_UP:
			case 'k':
				menu_driver(my_menu, REQ_UP_ITEM);
				break;
			case KEY_LEFT:
			case 'h':
				menu_driver(my_menu, REQ_LEFT_ITEM);
				break;
			case KEY_RIGHT:
			case 'l':
				menu_driver(my_menu, REQ_RIGHT_ITEM);
				break;
			case KEY_NPAGE:
				menu_driver(my_menu, REQ_SCR_DPAGE);
				break;
			case KEY_PPAGE:
				menu_driver(my_menu, REQ_SCR_UPAGE);
				break;
			}
		}

		wrefresh(tmp_win);
		wrefresh(my_menu_win);
	}

	/* Unpost and free all the memory taken up */
	unpost_menu(my_menu);
	free_menu(my_menu);
	for (i = 0; i < n_choices; ++i)
		free_item(my_items[i]);
	endwin();
	return 0;
}