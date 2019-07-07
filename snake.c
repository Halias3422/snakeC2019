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
	apple->is_eaten = 0;
}


t_snake		*delete_first_node_snake(t_snake *tail)
{
	t_snake	*head;

	tail = tail->next;
	head = tail->prev;
	tail->prev = NULL;
	free(head);
	return (tail);
}
t_snake		*move_snake_left(t_snake *tail)
{
	tail = delete_first_node_snake(tail);
	while (tail->next)	
		tail = tail->next;
	tail->x = tail->prev->x - 1;
	tail->y = tail->prev->y;
	tail = add_list_back(tail);
	while (tail->prev)
		tail = tail->prev;
	return (tail);
}

t_snake		*move_snake_down(t_snake *tail)
{
	tail = delete_first_node_snake(tail);
	while (tail->next)
		tail = tail->next;
	tail->x = tail->prev->x;
	tail->y = tail->prev->y + 1;
	tail = add_list_back(tail);
	while (tail->prev)
		tail = tail->prev;
	return (tail);
}

t_snake		*move_snake_right(t_snake *tail)
{
	tail = delete_first_node_snake(tail);
	while (tail->next)
		tail = tail->next;
	tail->x = tail->prev->x + 1;
	tail->y = tail->prev->y;
	tail = add_list_back(tail);
	while (tail->prev)
		tail = tail->prev;
	return (tail);
}

t_snake		*move_snake_up(t_snake *tail)
{
	tail = delete_first_node_snake(tail);
	while (tail->next)
		tail = tail->next;
	tail->x = tail->prev->x;
	tail->y = tail->prev->y - 1;
	tail = add_list_back(tail);
	while (tail->prev)
		tail = tail->prev;
	return (tail);
}

int		main(void)
{
	WINDOW	*win;
	int		is_dead = 0;
	t_snake *snake;
	t_apple	*apple;
	int		direction = 3;
	int		last_direction = 3;
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
	apple->is_eaten = 1;
	nodelay(stdscr, TRUE);
	while (is_dead == 0)
	{
		print_snake(snake, win);
		if (apple->is_eaten == 1)
			print_apple(win, snake, apple, ((COLS / 2) / 3) + 1, (((COLS / 2) / 3) + (COLS / 1.5)) - 1, ((LINES / 2) / 2) + 1, (((LINES / 2) / 2) + LINES / 1.5) - 1);
		input = getch();
		last_direction = direction;
		if (input == KEY_UP && last_direction != 3)
			direction = 1;
		else if (input == KEY_RIGHT && last_direction != 4)
			direction = 2;
		else if (input == KEY_DOWN && last_direction != 1)
			direction = 3;
		else if (input == KEY_LEFT && last_direction != 2)
			direction = 4;
		move(0, 0);
		printw("dir = %d last_dir = %d\n", direction, last_direction);
		if (direction == 1)
			snake = move_snake_up(snake);
		else if (direction == 2)
			snake = move_snake_right(snake);
		else if (direction == 3)
			snake = move_snake_down(snake);
		else if (direction == 4)
			snake = move_snake_left(snake);	
		wrefresh(win);
		usleep(300000);
	}
	getch();
	endwin();
	return (0);
}
