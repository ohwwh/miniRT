/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hako <hako@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/19 15:32:42 by hoh               #+#    #+#             */
/*   Updated: 2022/10/17 11:29:02 by hako             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#define MAX_DEPTH 50

void print_init(t_vars vars)
{
	double u;
	double v;
	t_vec dir;
	t_ray ray_tmp;
	t_color color;
	int k = 0;
	for (int j = vars.window_height - 1; j >= 0; --j)
	{
		if (vars.is_trace == 1)
		{
			printf("\rScanlines remaining: %d", j);
			fflush(stdout);
		}
		for (int i = 0; i < vars.window_width; ++i)
		{
			//기본 색상을 어떻게 지정할까? 원하는 색의 rgb값 / 256 * anti 
			//물체마다 따로 지정하려면 어떻C게 하나?
			//t_color color = create_vec(0, 0, 0);
			color = create_vec(0, 0, 0);
			for (int s = 0; s < vars.anti; s ++)
			{
				u = ((double)i + random_double(-1,1,vars.anti)) / (vars.window_width-1);
				v = ((double)j + random_double(-1,1,vars.anti)) / (vars.window_height-1);
				dir = create_vec(vars.camera.lower_left_corner.x + (u * vars.camera.horizontal.x) + (v * vars.camera.vertical.x) - vars.camera.origin.x,
				vars.camera.lower_left_corner.y + (u * vars.camera.horizontal.y) + (v * vars.camera.vertical.y) - vars.camera.origin.y,
				vars.camera.lower_left_corner.z + (u * vars.camera.horizontal.z) + (v * vars.camera.vertical.z) - vars.camera.origin.z);
				ray_tmp = ray(vars.camera.origin, dir);
				if (i == 286 && j == 210)
					i = i;
				if (vars.is_trace == 1)
					color = vec_sum(color, ray_color(ray_tmp, vars.world, vars.light, MAX_DEPTH));
					//여러 개의 광원이 있을 때는?
					//광원을 어떻게 구분해서 인자로 넣을 건지?
				else
					color = vec_sum(color, ray_color_2(ray_tmp, vars.world, vars.light));
			}
			color = vec_division(color, vars.anti);
			ft_pixel_put(&vars, i, vars.window_height - 1 - j, rgb_to_int(color));
		}
	}
}

int	main(int ac, char **av)
{
	t_vars	data;
	int			fd;

	if (check_file(ac, av))
		err_handler("wrong argument");
	fd = open(av[1], O_RDONLY);
	init_rt(&data);
	parse(&data.world, fd);
	rt_render(&data);
	return (0);
}