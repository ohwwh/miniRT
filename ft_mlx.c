/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_mlx.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hako <hako@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/21 20:29:08 by hako              #+#    #+#             */
/*   Updated: 2022/10/21 20:29:09 by hako             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

double clamp(double x)
{
    if (x < 0)
        return (0);
    if (x > 1)
        return (1);

	/*float A = 0.15;
    float B = 0.50;
    float C = 0.10;
    float D = 0.20;
    float E = 0.02;
    float F = 0.30;

	double ret = (x*(A*x+C*B)+D*E)/(x*(A*x+B)+D*F) - E/F;
    return (ret);*/
    return (x);
}


int rgb_to_int(t_color c)
{
	if (c.x != c.x)
		c.x = 0;
	if (c.y != c.y)
		c.y = 0;
	if (c.z != c.z)
		c.z = 0;
	return ((int)(255.999 * 0) << 24 | (int)(255.999 * sqrt(clamp(c.x))) << 16 | (int)(255.999 * sqrt(clamp(c.y))) << 8 | (int)(255.999 * sqrt(clamp(c.z))));
	//return ((int)(255.999 * (clamp(c.x))) << 16 | (int)(255.999 * (clamp(c.y))) << 8 | (int)(255.999 * (clamp(c.z))));
} //감마보정?



void ft_pixel_put(t_minirt *vars, int x, int y, int color)
{
	mlx_pixel_put(vars->mlx.mlx, vars->mlx.mlx_win, x, y, color);
}

/*void	ft_mlx_init(t_minirt *vars)
{	
	vars->mlx.mlx = mlx_init();
	if (!vars->mlx.mlx)
		printf("Error\nmlx_init fail\n");
	vars->mlx.mlx_win = mlx_new_window(vars->mlx.mlx, vars->mlx.window_width, vars->mlx.window_height, "menu");
	if (!vars->mlx.mlx_win)
	{
		printf("Error\nmlx_new_window fail\n");
		exit(0);
	}
}

void	ft_mlx_new(t_minirt *vars, int x, int y, char *name)
{
	vars->mlx.mlx_win = mlx_new_window(vars->mlx.mlx, x, y, name);
	if (!vars->mlx.mlx_win)
	{
		printf("Error\nmlx_new_window fail\n");
		exit(0);
	}
}*/
