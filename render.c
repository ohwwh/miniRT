#include "minirt.h"

int	ft_close(t_minirt *data)
{
	mlx_clear_window(data->mlx.mlx, data->mlx.mlx_win);
	mlx_destroy_window(data->mlx.mlx, data->mlx.mlx_win);
	exit(0);
}

int	keybind(int keycode, t_minirt *data)
{
	printf("keycode=%d\n", keycode);
	if (keycode == ESC)
		ft_close(data);
	return (0);
}

void	rt_render(t_minirt *data)
{
	mlx_put_image_to_window(data->mlx.mlx, data->mlx.mlx_win, data->mlx.img, 0, 0);
	mlx_key_hook(data->mlx.mlx_win, keybind, data);
	mlx_hook(data->mlx.mlx_win,  17, 0L, ft_close, data);
	mlx_loop(data->mlx.mlx);
}