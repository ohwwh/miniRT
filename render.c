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

void	define_color(t_minirt *data)
{
	int y = HEIGHT - 1;
	int x = 0;

	while (y >= 0)
	{
		x = 0;
		while (x < WIDTH)
		{
			data->u = (double)x * 2 / WIDTH - 1;
			data->v = (double)y * 2 / HEIGHT - 1;
			data->ray = ray_primary(&data->scene.cam, data->u, data->v); // 대강 이런식으로 구현할 것
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
	set_camera_param(&data->scene.cam);
	define_color(data);
	mlx_put_image_to_window(data->mlx.mlx, data->mlx.mlx_win, data->mlx.img, 0, 0);
	mlx_key_hook(data->mlx.mlx_win, keybind, data);
	mlx_hook(data->mlx.mlx_win,  17, 0L, ft_close, data);
	mlx_loop(data->mlx.mlx);
}