/* ******************************************************************************************** */
#define NO_ERROR 0
#define ERROR 1
#define TXT_ERR_ARG		"Error: argument\n"
#define TXT_ERR_FILE	"Error: Operation file corrupted\n"

/* ******************************************************************************************** */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

/* ******************************************************************************************** */
typedef struct s_zone
{
	int width;
	int heigth;
	char back;
	int total;
} t_zone;

typedef struct s_circle
{
	char type;
	float ctr_x;
	float ctr_y;
	float radius;
	char drawn_char;
} t_circle;

/* ******************************************************************************************** */
int ft_strlen(char *str)
{
	int i;

	i = 0;
	while (str[i] != '\0')
		i++;
	return (i);
}

/* ******************************************************************************************** */
void clean_all(char **map, FILE **ptr_file)
{
	if (*map)
		free(*map);
	*map = NULL;

	if(*ptr_file)
		if((fclose(*ptr_file)) == 0)
			*ptr_file = NULL;

}

/* ******************************************************************************************** */
char *get_zone_information(t_zone *z, FILE *ptr_file)
{
	char *map;
	int ret;

	map = NULL;

	if ((ret = fscanf(ptr_file, "%d %d %c\n", &z->width, &z->heigth, &z->back)) != 3)
		return NULL;

	if (z->width <= 0 || z->width > 300 || z->heigth <= 0 || z->heigth >300)
		return NULL;

	z->total = z->width * z->heigth;

	if ((map = (char *)malloc(z->total * sizeof(char))) == NULL)
		return NULL;

	memset(map, z->back, z->total);

	return (map);
}

/* ******************************************************************************************** */
bool is_in_disk(int x, int y, t_circle *c)
{
	float distance;
	float delta_x;
	float delta_y;

	delta_x = (float)x - c->ctr_x;
	delta_y = (float)y - c->ctr_y;
	
	distance = sqrtf(powf(delta_x, 2) + powf(delta_y, 2));

	if (distance <= c->radius)
		return true;

	return false;	
}

/* ******************************************************************************************** */
bool is_on_edge(int x, int y, t_circle *c)
{
	float distance;
	float delta_x;
	float delta_y;

	delta_x = (float)x - c->ctr_x;
	delta_y = (float)y - c->ctr_y;
	
	distance = sqrtf(powf(delta_x, 2) + powf(delta_y, 2));

	if (c->radius - distance < 1)
		return true;

	return false;	
}

/* ******************************************************************************************** */
void draw_circle (char *map, t_circle *c, t_zone *z)
{
	int x;
	int y;

	y = 0;
	while (y < z->heigth)
	{
		x = 0;
		while (x < z->width)
		{
			if (is_in_disk(x, y, c))
			{
				if((c->type == 'c' && is_on_edge(x, y, c)) || (c->type == 'C'))
					map[x + y * z->width] = c->drawn_char;			
			}
			x++;
		}		
		y++;
	}
}


/* ******************************************************************************************** */
char *do_map (FILE *ptr_file, t_zone *z, t_circle *c, char *map)
{
	int res;

	while (1)
	{
		res = fscanf(ptr_file, "%c %f %f %f %c\n", &c->type, &c->ctr_x, &c->ctr_y, &c->radius, &c->drawn_char);

		if (res == EOF)
			break;

		if (res != 5)
			return NULL;

		if (!(c->type == 'c' || c->type == 'C'))
			return NULL;

		if (c->radius <= 0)
			return NULL;

		draw_circle(map, c, z);

	}

	return (map);

}

/* ******************************************************************************************** */
void print_map(char *map, t_zone *z)
{
	int y;

	y = 0;
	while (y < z->heigth)
	{
		write(1, &map[y * z->width], z->width);
		write(1, "\n", 1);
		y++;
	}
}

/* ******************************************************************************************** */
int main(int argc, char **argv)
{
	FILE *ptr_file;
	char *map;

	t_zone z;
	t_circle c;


	map = NULL;
	ptr_file = NULL;

	if (argc == 2)
	{

		if((ptr_file = fopen(argv[1], "r")) == NULL)
		{
			write(1, TXT_ERR_FILE, ft_strlen(TXT_ERR_FILE));
			goto is_error;
		}

		if ((map = get_zone_information(&z, ptr_file)) == NULL)
			goto is_error;

		if ((do_map(ptr_file, &z, &c, map)) == NULL)
			goto is_error;

		print_map(map, &z);

	}
	else
	{
		write(1, TXT_ERR_ARG, ft_strlen(TXT_ERR_ARG));
		goto is_error;
	}

	clean_all(&map, &ptr_file);
	return (NO_ERROR);

is_error:
	clean_all(&map, &ptr_file);
	return (ERROR);
}






/* ******************************************************************************************** */