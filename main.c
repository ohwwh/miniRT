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
	data->is_trace = 1;
	data->scene.anti = 100;
	data->scene.changed = 0;
	//생성 실패 시 에러처리 해야 함
}

void path_render(t_minirt vars)
{
	double u;
	double v;
	t_vec dir;
	t_ray init_ray;
	t_color color;
	
	for (int y = HEIGHT - 1; y >= 0; --y)
	{
		if (vars.is_trace == 1)
		{
			printf("\rScanlines remaining: %d", y);
			fflush(stdout);
		}
		for (int x = 0; x < WIDTH; ++x)
		{
			color = create_vec(0, 0, 0);
			for (int s = 0; s < vars.scene.anti; s ++)
			{
				u = (((double)x + random_double(0, 1, vars.scene.anti)) * 2 / WIDTH) - 1;
				v = (((double)y + random_double(0, 1, vars.scene.anti)) * 2 / HEIGHT) - 1;
				init_ray = ray_primary(&(vars.scene.camera), u, v);
				if (x == 230 && y == 300)
					x = x;
				if (vars.is_trace == 1)
					color = vec_sum(color, ray_color(init_ray, vars.scene.objs, vars.scene.light, MAX_DEPTH));
					//여러 개의 광원이 있을 때는?
					//광원을 어떻게 구분해서 인자로 넣을 건지?
				else
					color = vec_sum(color, ray_color_2(init_ray, vars.scene.objs, vars.scene.light));
			}
			color = vec_division(color, vars.scene.anti);
			put_color(&vars.mlx, x, HEIGHT - 1 - y, rgb_to_int(color));
			//put_color(&vars.mlx, x, HEIGHT - 1 - y,
			//	convert_rgb(color.x, color.y, color.z));
		}
	}
	mlx_put_image_to_window(vars.mlx.mlx, vars.mlx.mlx_win, vars.mlx.img, 0, 0); // 무슨 차이지....
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
	//rt_render(&data);
	data.scene.light = 0;
	set_camera(&data.scene.camera);
	path_render(data);
	mlx_hook(data.mlx.mlx_win, 2, 0, &keypress, &data);
	mlx_hook(data.mlx.mlx_win, 3, 0, &keyrelease, &data);
	mlx_hook(data.mlx.mlx_win, 4, 0, &scroll, &data);
	mlx_hook(data.mlx.mlx_win,  17, 0L, ft_close, &data);
	mlx_loop_hook(data.mlx.mlx, &key_hook_move, &data);
	mlx_loop(data.mlx.mlx);
	return (0);
}