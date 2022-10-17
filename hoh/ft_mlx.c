#include "ft_mlx.h"
#include "vector.h"
#include "random.h"

double clamp(double x)
{
    if (x < 0)
        return (0);
    if (x > 1)
        return (1);
    return (x);
}

int rgb_to_int(t_color c)
{
	return ((int)(255.999 * 0) << 24 | (int)(255.999 * sqrt(clamp(c.x))) << 16 | (int)(255.999 * sqrt(clamp(c.y))) << 8 | (int)(255.999 * sqrt(clamp(c.z))));
} //감마보정?

/*int rgb_to_int(t_color c)
{
	return ((int)(255.999 * 0) << 24 | (int)(255.999 * (clamp(c.x))) << 16 | (int)(255.999 * (clamp(c.y))) << 8 | (int)(255.999 * (clamp(c.z))));
}*/

/*int rgb_to_int(t_color c)
{
	return ((int)(255.999 * 0) << 24 | (int)(255.999 * c.x) << 16 | (int)(255.999 * c.y) << 8 | (int)(255.999 * c.z));
}*/

void ft_pixel_put(t_vars *vars, int x, int y, int color)
{
	mlx_pixel_put(vars->mlx, vars->win, x, y, color);
}

void	ft_mlx_init(t_vars *vars)
{	
	vars->mlx = mlx_init();
	if (!vars->mlx)
		printf("Error\nmlx_init fail\n");
	vars->win = mlx_new_window(vars->mlx, vars->window_width, vars->window_height, "menu");
	if (!vars->win)
	{
		printf("Error\nmlx_new_window fail\n");
		exit(0);
	}
}

void	ft_mlx_new(t_vars *vars, int x, int y, char *name)
{
	vars->win = mlx_new_window(vars->mlx, x, y, name);
	if (!vars->win)
	{
		printf("Error\nmlx_new_window fail\n");
		exit(0);
	}
}
