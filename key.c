#include "minirt.h"

int transpose(t_minirt *data, t_keycode keycode, int type, int *status) // object sphere
{	
	t_objs *tmp;

	*status = -1;
	tmp = data->scene.objs;
	if (keycode == W)
	{
		while (tmp)
		{
			if (tmp->type == type)
			{
				tmp->center.y += 5;
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
				tmp->center.x += 5;
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
				tmp->center.z += 5;
			}
			tmp = tmp->next;
		}
		rt_render(data);
	}
	return (0);
}

int transpose_light(t_minirt *data, t_keycode keycode, int *status)
{
	t_light *light = data->scene.light;

	*status = -1;
	if (keycode == W)
	{
		light->src.y += 5;
	}
	else if (keycode == A)
	{
		light->src.x += 5;
	}
	else if (keycode == D)
	{
		light->src.z += 5;
	}
	rt_render(data);
	return (0);
}

int	ft_close(t_minirt *data)
{
	mlx_clear_window(data->mlx.mlx, data->mlx.mlx_win);
	mlx_destroy_window(data->mlx.mlx, data->mlx.mlx_win);
	
	t_light *light;
	t_objs *obj;

	while (data->scene.light)
	{
		light = data->scene.light;
		data->scene.light = data->scene.light->next;
		free(light);
	}
	while (data->scene.objs)
	{
		obj = data->scene.objs;
		data->scene.objs = data->scene.objs->next;
		free(obj);
	}
	exit(0);
}

int cam_transpose(t_minirt *data, t_keycode keycode, int *status) // 1
{
	*status = -1;
	if (keycode == W)
	{
		data->scene.cam.cen.y += 5;		
		rt_render(data);
	}
	else if (keycode == A)
	{
		data->scene.cam.cen.x += 5;		
		rt_render(data);
	}
	if (keycode == D)
	{
		data->scene.cam.cen.z += 5;
		rt_render(data);
	}
	return (0);
}

int cam_rotate(t_minirt *data, t_keycode keycode, int *status) // 0
{
	double x;
	double y;
	double z;

	*status = -1;
	if (keycode == W)
	{
		x = data->scene.cam.dir.x;
		y = data->scene.cam.dir.y;
		z = data->scene.cam.dir.z;
		data->scene.cam.dir.x = z * sin(ROTATE) + x * cos(ROTATE);
		data->scene.cam.dir.z = z * cos(ROTATE) - x * sin(ROTATE);
		rt_render(data);
	}
	else if (keycode == A)
	{
		x = data->scene.cam.dir.x;
		y = data->scene.cam.dir.y;
		z = data->scene.cam.dir.z;
		data->scene.cam.dir.y = y * cos(ROTATE) - z * sin(ROTATE);
		data->scene.cam.dir.z = y * sin(ROTATE) + z * cos(ROTATE);
		rt_render(data);
	}
	if (keycode == D)
	{
		x = data->scene.cam.dir.x;
		y = data->scene.cam.dir.y;
		z = data->scene.cam.dir.z;
		data->scene.cam.dir.x = x * cos(ROTATE) - y * sin(ROTATE);
		data->scene.cam.dir.y = x * sin(ROTATE) + y * cos(ROTATE);
		rt_render(data);
	}
	return (1);
}

int rotate(t_minirt *data, t_keycode keycode, int type, int *status)
{
	t_objs *tmp;
	double x;
	double y;
	double z;

	*status = -1;
	tmp = data->scene.objs;
	if (keycode == W) // y축 중심 회전
	{
		while (tmp)
		{
			if (tmp->type == type)
			{
				printf("y axis\n");
				x = tmp->dir.x;
				y = tmp->dir.y;
				z = tmp->dir.z;
				tmp->dir.x = z * sin(ROTATE) + x * cos(ROTATE);
				tmp->dir.z = z * cos(ROTATE) - x * sin(ROTATE);
			}
			tmp = tmp->next;
		}
		rt_render(data);
	}
	else if (keycode == A) // x축 중심 회전
	{
		while (tmp)
		{
			if (tmp->type == type)
			{
				printf("x axis\n");
				x = tmp->dir.x;
				y = tmp->dir.y;
				z = tmp->dir.z;
				tmp->dir.y = y * cos(ROTATE) - z * sin(ROTATE);
				tmp->dir.z = y * sin(ROTATE) + z * cos(ROTATE);
			}
			tmp = tmp->next;
		}
		rt_render(data);
	}
	if (keycode == D) // z축 중심 회전
	{
		while (tmp)
		{
			if (tmp->type == type)
			{
				printf("z axis\n");
				x = tmp->dir.x;
				y = tmp->dir.y;
				z = tmp->dir.z;
				tmp->dir.x = x * cos(ROTATE) - y * sin(ROTATE);
				tmp->dir.y = x * sin(ROTATE) + y * cos(ROTATE);
			}
			tmp = tmp->next;
		}
		rt_render(data);
	}
	return (0);
}

int	keybind(int keycode, t_minirt *data)
{
	static int status = -1;

	printf("keycode=%d\n", keycode);
	if (keycode == ESC)
		ft_close(data);
	if (status == -1 && ((18 <= keycode && keycode <= 23) || keycode == 29))
		status = keycode;
	else if (status != -1)
	{
		if (status == ONE)
			cam_transpose(data, keycode, &status);
		else if (status == ZERO)
			cam_rotate(data, keycode, &status);
		else if (status == TWO)
			transpose(data, keycode, SP, &status);
		else if (status == THREE)
			transpose(data, keycode, CY, &status);
		else if (status == FOUR)
			transpose_light(data, keycode, &status);
		else if (status == FIVE)
			rotate(data, keycode, CY, &status);
		else if (status == SIX)
			rotate(data, keycode, PL, &status);
	}
	printf("status : %d\n", status);
	return (0);
}