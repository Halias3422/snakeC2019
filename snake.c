#include "snake.h"

t_snake	*add_list_back(t_snake *snake)
{
	t_snake *new;

	new = NULL;
	new = (t_snake*)malloc(sizeof(t_snake));
	while (snake->next != NULL)
		snake = snake->next;
	snake->next = new;
	new->prev = snake;
	return (new);
}

void	create_snake(t_snake *snake, int y, int x)
{
	int	snake_init = 0;
	
	while (snake_init++ < 4)
	{
		snake->x = x;
		snake->y = y++;
		snake = add_list_back(snake);
	}
}

void	print_snake(t_snake *snake, WINDOW *win)
{
	int y = 0;
	while (snake->next)
	{
		mvwaddch(win, snake->y, snake->x, '#');
		snake = snake->next;
	}
}

void	print_apple(WINDOW *win, t_snake *snake, t_apple *apple, int minx, int maxx, int miny, int maxy)
{
	t_snake *head;
	int		error = 0;

	srand(time(NULL));
	head = snake;
	apple->x = minx + rand() % (maxx - minx);
	apple->y = miny + rand() % (maxy - miny);
	while (snake->next)
	{
		if (snake->x == apple->x && error++)
			apple->x = minx + rand() % (maxx - minx);
		if (snake->y == apple->y && error++)
			apple->y = miny + rand() % (maxy - miny);
		if (error > 0)
		{
			error = 0;
			snake = head;
		}
		else
			snake = snake->next;
	}
	move(apple->y, apple->x);
	printw("@");
	//mvwaddch(win, apple->y, apple->x, '@');
}

void		move_snake_up(t_snake *tail)
{
	t_snake	*head;

	head = tail->next;
	free(tail);
	tail = NULL;
	head->prev = NULL;
	tail = head;
/*	while (head->next)
		head = head->next;
	head->x = head->prev->x;
	head->y = head->prev->y - 1;
	head = add_list_back(head);*/}

int		main(void)
{
	WINDOW	*win;
	int		is_dead = 0;
	t_snake *snake;
	t_apple	*apple;
	int		direction = 3;
	int		input;

	initscr();
	cbreak();
	noecho();
	curs_set(0);
	keypad(stdscr, TRUE);
	refresh();
	win = newwin(LINES / 1.5, COLS / 1.5, (LINES / 2) / 2, (COLS / 2) / 3);
	box(win, ACS_VLINE, ACS_HLINE);
	snake = (t_snake*)malloc(sizeof(t_snake));
	apple = (t_apple*)malloc(sizeof(t_apple));
	create_snake(snake, (LINES / 2) / 2, (COLS / 2) / 1.5);
	int y = 0;
	while (is_dead == 0)
	{
		print_snake(snake, win);
		move(y++, 0);
		print_apple(win, snake, apple, ((COLS / 2) / 3) + 1, (((COLS / 2) / 3) + (COLS / 1.5)) - 1, ((LINES / 2) / 2) + 1, (((LINES / 2) / 2) + LINES / 1.5) - 1);
	//refresh();
	input = getch();
	if (input == KEY_UP)
		direction = 1;
	else if (input == KEY_LEFT)
		direction = 2;
	else if (input == KEY_DOWN)
		direction = 3;
	else if (input == KEY_RIGHT)
		direction == 4;
	if (direction == 1)
	{
		move_snake_up(snake);
	}
	wrefresh(win);
		usleep(300);
	}
getch();
	endwin();
	return (0);
}
