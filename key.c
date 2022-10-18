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
				tmp->center.y += STEP;
			tmp = tmp->next;
		}
		rt_render(data);
	}
	else if (keycode == A)
	{
		while (tmp)
		{
			if (tmp->type == type)
				tmp->center.x += STEP;
			tmp = tmp->next;
		}
		rt_render(data);
	}
	if (keycode == D)
	{
		while (tmp)
		{
			if (tmp->type == type)
				tmp->center.z += STEP;
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
		light->src.y += STEP;
	else if (keycode == A)
		light->src.x += STEP;
	else if (keycode == D)
		light->src.z += STEP;
	rt_render(data);
	return (0);
}

int	ft_close(t_minirt *data)
{
	t_light *light;
	t_objs *obj;

	mlx_clear_window(data->mlx.mlx, data->mlx.mlx_win);
	mlx_destroy_window(data->mlx.mlx, data->mlx.mlx_win);
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
	// system("leaks miniRT");
	exit(0);
}

int cam_transpose(t_minirt *data, t_keycode keycode, int *status) // 1
{
	*status = -1;
	if (keycode == W)
	{
		data->scene.camera.origin.y += STEP;
		rt_render(data);
	}
	else if (keycode == A)
	{
		data->scene.camera.origin.x += STEP;
		rt_render(data);
	}
	if (keycode == D)
	{
		data->scene.camera.origin.z += STEP;
		rt_render(data);
	}
	return (0);
}

int cam_rotate(t_minirt *data, t_keycode keycode, int *status) // 0
{
	t_vec tmp;

	*status = -1;
	set_vec(&tmp, data->scene.camera.dir.x, data->scene.camera.dir.y, data->scene.camera.dir.z);
	if (keycode == W)
	{
		data->scene.camera.dir.x = tmp.z * sin(ROTATE) + tmp.x * cos(ROTATE);
		data->scene.camera.dir.z = tmp.z * cos(ROTATE) - tmp.x * sin(ROTATE);
		rt_render(data);
	}
	else if (keycode == A)
	{
		data->scene.camera.dir.y = tmp.y * cos(ROTATE) - tmp.z * sin(ROTATE);
		data->scene.camera.dir.z = tmp.y * sin(ROTATE) + tmp.z * cos(ROTATE);
		rt_render(data);
	}
	if (keycode == D)
	{
		data->scene.camera.dir.x = tmp.x * cos(ROTATE) - tmp.y * sin(ROTATE);
		data->scene.camera.dir.y = tmp.x * sin(ROTATE) + tmp.y * cos(ROTATE);
		rt_render(data);
	}
	return (1);
}

int rotate(t_minirt *data, t_keycode keycode, int type, int *status)
{
	t_objs *tmp;
	t_vec ori;

	*status = -1;
	tmp = data->scene.objs;
	if (keycode == W) // y축 중심 회전
	{
		while (tmp)
		{
			if (tmp->type == type)
			{
				printf("y axis\n");
				set_vec(&ori, tmp->dir.x, tmp->dir.y, tmp->dir.z);
				tmp->dir.x = ori.z * sin(ROTATE) + ori.x * cos(ROTATE);
				tmp->dir.z = ori.z * cos(ROTATE) - ori.x * sin(ROTATE);
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
				set_vec(&ori, tmp->dir.x, tmp->dir.y, tmp->dir.z);
				tmp->dir.y = ori.y * cos(ROTATE) - ori.z * sin(ROTATE);
				tmp->dir.z = ori.y * sin(ROTATE) + ori.z * cos(ROTATE);
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
				set_vec(&ori, tmp->dir.x, tmp->dir.y, tmp->dir.z);
				tmp->dir.x = ori.x * cos(ROTATE) - ori.y * sin(ROTATE);
				tmp->dir.y = ori.x * sin(ROTATE) + ori.y * cos(ROTATE);
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
	return (0);
}