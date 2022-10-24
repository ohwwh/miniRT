/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hako <hako@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/21 20:28:20 by hako              #+#    #+#             */
/*   Updated: 2022/10/24 17:26:22 by hako             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	put_color(t_mlx *data, int x, int y, int color)
{
	char	*dst;

	dst = data->addr + (y * data->line_length + x * (data->bits_per_pixel / 8));
	*(unsigned int *)dst = color;
}

void	define_color(t_minirt *data)
{
	int		y;
	int		x;
	double	u;
	double	v;
	t_vec	dir;

	y = HEIGHT - 1;
	x = 0;
	while (y >= 0)
	{
		x = 0;
		while (x < WIDTH)
		{
			u = ((double)x * 2 / WIDTH) - 1;
			v = (double)y * 2 / HEIGHT - 1;
			if (x == 230 && y == 250)
				x = x;
			data->ray = ray_primary(&data->scene.camera, u, v);
			data->ray.color = get_raycolor(data);
			put_color(&data->mlx, x, HEIGHT - 1 - y,
				rgb_to_int(data->ray.color));
			x++;
		}
		y--;
	}
}

void	rt_render(t_minirt *data)
{
	set_camera(&data->scene.camera);
	define_color(data);
	mlx_put_image_to_window(data->mlx.mlx,
		data->mlx.mlx_win, data->mlx.img, 0, 0);
}
