#include "minirt.h"

int transpose(t_minirt *data, int keycode, int type) // object sphere
{	
	t_objs *tmp;

	tmp = data->scene.objs;
	if (keycode == W)
	{
		while (tmp)
		{
			if (tmp->type == type)
			{
				tmp->cen.y += 10;
			}
			tmp = tmp->next;
		}
		rt_render(data);
	}
	else if (keycode == A)
	{
		while (tmp)
		{
			if (tmp->type == type)
			{
				tmp->cen.x += 10;
			}
			tmp = tmp->next;
		}
		rt_render(data);
	}
	if (keycode == D)
	{
		while (tmp)
		{
			if (tmp->type == type)
			{
				tmp->cen.z += 10;
			}
			tmp = tmp->next;
		}
		rt_render(data);
	}
	return (0);
}

int	ft_close(t_minirt *data)
{
	mlx_clear_window(data->mlx.mlx, data->mlx.mlx_win);
	mlx_destroy_window(data->mlx.mlx, data->mlx.mlx_win);
	exit(0);
}

int	keybind(int keycode, t_minirt *data)
{
	static int status = 0;

	printf("keycode=%d\n", keycode);
	if (keycode == ESC)
		ft_close(data);
	if (status == 0)
		status = keycode;
	else if (status == TWO)
	{
		status = 0;
		transpose(data, keycode, SP);
	}
	else if (status == THREE)
	{
		status = 0;
		transpose(data, keycode, CY);
	}
	return (0);
}