/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hako <hako@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/21 20:28:58 by hako              #+#    #+#             */
/*   Updated: 2022/10/21 20:28:59 by hako             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	init_rt(t_minirt *data)
{
	data->mlx.mlx = mlx_init();
	data->mlx.mlx_win = mlx_new_window(data->mlx.mlx, WIDTH, HEIGHT, "miniRT");
	data->mlx.img = mlx_new_image(data->mlx.mlx, WIDTH, HEIGHT);
	data->mlx.addr = mlx_get_data_addr(data->mlx.img, &data->mlx.bits_per_pixel, \
	&data->mlx.line_length, &data->mlx.endian);
	data->scene.objs = NULL;
	data->scene.amb.count = 0;
	data->scene.camera.count = 0;
	data->scene.light = NULL;
	data->is_move = -1;
	data->is_trace = 0;
	data->mode = 0;
	data->scene.anti = 1;
	data->scene.changed = 0;
	//생성 실패 시 에러처리 해야 함
}

void sampling(t_minirt *vars, int x, int y)
{
	double u;
	double v;
	t_ray init_ray;

	u = (((double)x + random_double(0, 1, vars->scene.anti)) * 2 / WIDTH) - 1;
	v = (((double)y + random_double(0, 1, vars->scene.anti)) * 2 / HEIGHT) - 1;
	init_ray = ray_primary(&(vars->scene.camera), u, v);
	if (x == 0 && y == 0)
		x = x;
	if (vars->is_trace == 1)
		vars->ray.color = vec_sum(vars->ray.color, ray_color(init_ray, &vars->scene, MAX_DEPTH));
	else
		vars->ray.color = vec_sum(vars->ray.color, ray_color_raw(init_ray, &vars->scene));
}

void path_render(t_minirt *vars)
{
	int	x;
	int	y;
	int	s;
	
	y = HEIGHT - 1;
	while (y -- >= 0)
	{
		x = 0;
		if (vars->is_trace == 1)
		{
			printf("\rScanlines remaining: %d", y);
			fflush(stdout);
		}
		while (x ++ < WIDTH)
		{
			vars->ray.color = create_vec(0, 0, 0);
			s = 0;
			while (s ++ < vars->scene.anti)
				sampling(vars, x, y);
			vars->ray.color = vec_division(vars->ray.color, vars->scene.anti);
			put_color(&vars->mlx, x - 1, HEIGHT - 2 - y, rgb_to_int(vars->ray.color));
		}
	}
	mlx_put_image_to_window(vars->mlx.mlx, vars->mlx.mlx_win, vars->mlx.img, 0, 0); // 무슨 차이지....
}

int	main(int ac, char **av)
{
	t_minirt	data;
	int			fd;

	if (check_file(ac, av))
		err_handler("wrong argument");
	fd = open(av[1], O_RDONLY);
	init_rt(&data);
	parse(&data.scene, fd);

	data.scene.camera.distance = vec_len(vec_sub(data.scene.camera.origin, 
	data.scene.objs->center));
	//data.scene.light->distance = vec_len(vec_sub(data.scene.light->object.center, 
	//data.scene.objs->center));
	//빛이 없는 경우 / 오브젝트가 없는 경우?

	create_light_object(&data.scene);
	set_camera(&data.scene.camera);
	//data.scene.light = 0;
	//data.scene.amb.ratio = 1;
	path_render(&data);
	mlx_hook(data.mlx.mlx_win, 2, 0, &keypress, &data);
	mlx_hook(data.mlx.mlx_win, 3, 0, &keyrelease, &data);
	mlx_hook(data.mlx.mlx_win, 4, 0, &scroll, &data);
	mlx_hook(data.mlx.mlx_win,  17, 0L, ft_close, &data);
	mlx_loop_hook(data.mlx.mlx, &key_hook_move, &data);
	mlx_loop(data.mlx.mlx);
	return (0);
}