#include "minirt.h"

void	put_color(t_mlx *data, int x, int y, int color)
{
	char	*dst;

	dst = data->addr + (y * data->line_length + x * (data->bits_per_pixel / 8));
	*(unsigned int *)dst = color;
}

int		convert_rgb(int r, int g, int b)
{
	return (((r & 0xff) << 16) + ((g & 0xff) << 8) + (b & 0xff));
}

t_ray ray(t_point origin, t_vec dir)
{
	t_ray ret;
	ret.origin = origin;
	ret.dir = dir;
	return (ret);
}

void	define_color(t_minirt *data)
{
	int y = HEIGHT - 1;
	int x = 0;
	double u;
	double v;
	t_vec dir;

	while (y >= 0)
	{
		x = 0;
		while (x < WIDTH)
		{
			u = ((double)x * 2 / WIDTH) - 1;
			v = (double)y * 2 / HEIGHT - 1;
			data->ray = ray_primary(&data->scene.camera, u, v);
			data->ray.color = get_raycolor(data); // 대강 이런식으로 구현할 것
			put_color(&data->mlx, x, HEIGHT - 1 - y,
				convert_rgb(data->ray.color.x, data->ray.color.y, data->ray.color.z));	
			x++;
		} 
		y--;
	}
}

void	rt_render(t_minirt *data)
{
	set_camera(&data->scene.camera);
	define_color(data);
	mlx_put_image_to_window(data->mlx.mlx, data->mlx.mlx_win, data->mlx.img, 0, 0);
	mlx_key_hook(data->mlx.mlx_win, keybind, data);
	mlx_hook(data->mlx.mlx_win,  17, 0L, ft_close, data);
	mlx_loop(data->mlx.mlx);
}

/*void path_render(t_minirt vars)
{
	double u;
	double v;
	t_vec dir;
	t_ray ray_tmp;
	t_color color;
	
	for (int y = data->mlx.window_height - 1; y >= 0; --y)
	{
		if (data->is_trace == 1)
		{
			printf("\rScanlines remaining: %d", y);
			fflush(stdout);
		}
		for (int i = 0; i < data->mlx.window_width; ++i)
		{
			//기본 색상을 어떻게 지정할까? 원하는 색의 rgb값 / 256 * anti 
			//물체마다 따로 지정하려면 어떻C게 하나?
			color = create_vec(0, 0, 0);
			for (int s = 0; s < data->scene.anti; s ++)
			{
				u = ((double)i + random_double(-1,1,data->scene.anti)) / (data->mlx.window_width-1);
				v = ((double)y + random_double(-1,1,data->scene.anti)) / (data->mlx.window_height-1);
				dir = create_vec(data->scene.camera.lower_left_corner.x + (u * data->scene.camera.horizontal.x) + (v * data->scene.camera.vertical.x) - data->scene.camera.origin.x,
				data->scene.camera.lower_left_corner.y + (u * data->scene.camera.horizontal.y) + (v * data->scene.camera.vertical.y) - data->scene.camera.origin.y,
				data->scene.camera.lower_left_corner.z + (u * data->scene.camera.horizontal.z) + (v * data->scene.camera.vertical.z) - data->scene.camera.origin.z);
				ray_tmp = ray(data->scene.camera.origin, dir);
				if (i == 0 && y == 319)
					i = i;
				if (data->is_trace == 1)
					color = vec_sum(color, ray_color(ray_tmp, data->scene.obys, data->scene.light, MAX_DEPTH));
					//여러 개의 광원이 있을 때는?
					//광원을 어떻게 구분해서 인자로 넣을 건지?
				else
					color = vec_sum(color, ray_color_2(ray_tmp, data->scene.world, data->scene.light));
			}
			color = vec_division(color, data->scene.anti);
			put_color(&data->mlx, i, HEIGHT - 1 - y, rgb_to_int(color));
			//ft_pixel_put(&vars, i, data->mlx.window_height - 1 - y, rgb_to_int(color));
		}
	}
	mlx_put_image_to_window(data->mlx.mlx, data->mlx.mlx_win, data->mlx.img, 0, 0); // 무슨 차이지....
}*/