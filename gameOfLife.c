#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <inttypes.h>
#include <time.h>
#include <errno.h>

typedef struct
{
	int width;
	int hight;
	bool **grid;
} grid_t;
grid_t *make_grid(int width, int hight)
{
	grid_t *g = calloc(1, sizeof(grid_t));
	g->width = width;
	g->hight = hight;
	bool **rows = calloc(hight, sizeof(bool *));
	for (int i = 0; i < hight; i++)
	{
		rows[i] = calloc(width, sizeof(bool));
	}
	g->grid = rows;
	return g;
}

void print_grid(grid_t *g)
{
	for (int i = 0; i < g->hight; i++)
	{
		for (int j = 0; j < g->width; j++)
		{
			printf("%c ", (g->grid[i][j]) ? '#' : '.');
		}
		printf("\n");
	}
}

void free_grid(grid_t *g)
{
	for (int i = 0; i < g->hight; i++)
	{
		free(g->grid[i]);
	}
	free(g->grid);
	free(g);
}

int count_neighbors(grid_t *g, int x, int y)
{
	return g->grid[y - 1][x - 1] + g->grid[y - 1][x] + g->grid[y - 1][x + 1] + g->grid[y][x - 1] + g->grid[y][x + 1] + g->grid[y + 1][x - 1] + g->grid[y + 1][x] + g->grid[y + 1][x + 1];
}

grid_t *evolve_grid(grid_t *g)
{
	grid_t *newg = make_grid(g->width, g->hight);
	for (int i = 1; i < g->hight - 1; i++)
	{
		for (int j = 1; j < g->width - 1; j++)
		{
			switch (count_neighbors(g, j, i))
			{
			case 2:
				newg->grid[i][j] = g->grid[i][j];
				break;
			case 3:
				newg->grid[i][j] = true;
				break;
			default:
				newg->grid[i][j] = false;
				break;
			}
		}
	}
	free_grid(g);
	return newg;
}

grid_t *get_grid_from_File(const char *filename)
{
	FILE *f = fopen(filename, "r");
	int width = 0;
	int height = 0;
	fscanf(f, "%i\n", &width);
	fscanf(f, "%i\n", &height);
	grid_t *g = make_grid(width, height);
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			char c = fgetc(f);
			if (c == '#')
			{
				g->grid[i][j] = true;
			}
		}
		if (fgetc(f) != '\n')
		{
			printf("Wrong fileformat!\n");
			return g;
		}
	}
	fclose(f);
	return g;
}

uint32_t get_iterations(const char *input)
{
	errno = 0;
	char *endptr = NULL;
	uint32_t iterations = strtol(input, &endptr, 10);
	if (errno)
	{
		printf("Invalid number of interations.\n");
		exit(1);
	}
	if (endptr == input)
	{
		printf("Number of interations wasn't provided\n");
		exit(1);
	}
	return iterations;
}

int main(int argc, char **argv)
{
	if (argc != 3)
	{
		printf("Usage: ./gameOfLife [iterations] [file]\n");
		return 1;
	}

	grid_t *my_grid = get_grid_from_File(argv[2]);

	for (int i = 1; i <= get_iterations(argv[1]); i++)
	{
		system("@cls||clear");
		printf("Iteration %d:\n", i);
		print_grid(my_grid);
		my_grid = evolve_grid(my_grid);
		usleep(50000);
	}

	free_grid(my_grid);
	return 0;
}
