#include "snake.h"
/*
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
*/

t_snake	*add_list_back(t_snake *snake)
{
	t_snake	*new;

	new = (t_snake*)malloc(sizeof(t_snake));
//	new = NULL;
	new->next = NULL;
	new->prev = snake;
	snake->next = new;
	return(snake);
}

void	create_snake(t_snake *snake, int y, int x)
{
	int	snake_init = 0;
	snake->next = NULL;
	snake->prev = NULL;
	while (snake_init++ < 4)
	{
		snake->x = x;
		snake->y = y++;
		snake = add_list_back(snake);
		snake = snake->next;
	}
	while (snake->prev)
		snake = snake->prev;
	endwin();
	exit(0);
}

void	print_snake(t_snake *snake, WINDOW *win)
{
	while (snake->next)
	{	
		mvwaddch(win, snake->y, snake->x, '#');
		snake = snake->next;
	}
}


void	get_apple_position(t_snake *snake, t_apple *apple, int minx, int maxx, int miny, int maxy)
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
}

void	print_apple(t_apple *apple)
{
	move(apple->y, apple->x);
	printw("@");
	apple->is_eaten = 0;
}

int		check_if_snake_eats_apple(t_snake *snake, t_apple *apple)
{
	move(0, 0);
	if (snake->prev->x + ((COLS / 2) / 3) == apple->x && snake->prev->y + ((LINES / 2) / 2) == apple->y)
	{
		apple->is_eaten = 1;
		return (1);
	}
	return (0);
}

t_snake		*delete_first_node_snake(t_snake *tail, WINDOW *win)
{
	t_snake	*head;
	while (tail->prev->prev)
		tail = tail->prev;
	head = tail->prev;
	mvwaddch(win, head->y, head->x, ' ');
	tail->prev = NULL;
	free(head);
	return (tail);
}

t_snake		*move_snake_left(t_snake *tail, WINDOW *win, t_apple *apple)
{	
	while (tail->next)	
		tail = tail->next;
	tail->x = tail->prev->x - 1;
	tail->y = tail->prev->y;
	tail = add_list_back(tail);
	if (check_if_snake_eats_apple(tail, apple) == 0)
		tail = delete_first_node_snake(tail, win);
	return (tail);
}

t_snake		*move_snake_down(t_snake *tail, WINDOW *win, t_apple *apple)
{
	while (tail->next)
		tail = tail->next;
	tail->x = tail->prev->x;
	tail->y = tail->prev->y + 1;
	tail = add_list_back(tail);
	if (check_if_snake_eats_apple(tail, apple) == 0)
		tail = delete_first_node_snake(tail, win);
	return (tail);
}

t_snake		*move_snake_right(t_snake *tail, WINDOW *win, t_apple *apple)
{
	while (tail->next)
		tail = tail->next;
	tail->x = tail->prev->x + 1;
	tail->y = tail->prev->y;
	tail = add_list_back(tail);
if (check_if_snake_eats_apple(tail, apple) == 0)
		tail = delete_first_node_snake(tail, win);
	return (tail);
}

t_snake		*move_snake_up(t_snake *tail, WINDOW *win, t_apple *apple)
{
	while (tail->next)
		tail = tail->next;
	tail->x = tail->prev->x;
	tail->y = tail->prev->y - 1;
	tail = add_list_back(tail);
	if (check_if_snake_eats_apple(tail, apple) == 0)
		tail = delete_first_node_snake(tail, win);
	return (tail);
}

int		check_is_dead(t_snake *snake) { t_snake	*tail; tail = snake;
	while (snake->next->next)
		snake = snake->next;
	while (tail->next->next->next)
	{	
		if (tail->x == snake->x && tail->y == snake->y)
			return (1);
		tail = tail->next;
	}
	if (snake->x >= ((COLS / 2) / 3) + COLS / 1.5 || snake->y >= ((LINES / 2) / 2) + LINES / 1.5) 
		return (1);
	return (0);
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
	//nodelay(stdscr, TRUE);
	while (is_dead == 0)
	{
		print_snake(snake, win);
		if (apple->is_eaten == 1)
			get_apple_position(snake, apple, ((COLS / 2) / 3) + 1, (((COLS / 2) / 3) + (COLS / 1.5)) - 1, ((LINES / 2) / 2) + 1, (((LINES / 2) / 2) + LINES / 1.5) - 1);
		print_apple(apple);
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
		if (direction == 1)
			snake = move_snake_up(snake, win, apple);
		else if (direction == 2)
			snake = move_snake_right(snake, win, apple);
		else if (direction == 3)
			snake = move_snake_down(snake, win, apple);
		else if (direction == 4)
			snake = move_snake_left(snake, win, apple);	
		is_dead = check_is_dead(snake);
		timeout(150);
		wrefresh(win);
	}
	getch();
	endwin();
	return (0);
}
