# ifndef SNAKE_H
#define SNAKE_H

#include <unistd.h>
#include <string.h>
#include <time.h>
#include <curses.h>
#include <stdlib.h>

typedef struct s_snake
{
	int	x;
	int	y;
	struct s_snake	*next;
	struct s_snake	*prev;
}			t_snake;

typedef struct s_apple
{
	int	x;
	int	y;
}		t_apple;

#endif
