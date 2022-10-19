#include "minirt.h"

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

t_vec rotate(t_vec axis, t_minirt* vars, int dir)
{
	double c = (1 - cos(dir * 0.1));
	double s = sin(dir * 0.1);
	double x = axis.x;
	double y = axis.y;
	double z = axis.z;

	double i = vars->scene.camera.forward.x;
	double j = vars->scene.camera.forward.y;
	double k = vars->scene.camera.forward.z;

	t_vec new_dir;

	new_dir.x = -i*c*y*y-k*s*y+c*j*x*y-i*c*z*z+j*s*z+c*k*x*z+i;
	new_dir.y = j-c*j*x*x+k*s*x+i*c*x*y-c*j*z*z-i*s*z+c*k*y*z;
	new_dir.z = k-c*k*x*x-j*s*x-c*k*y*y+i*s*y+i*c*x*z+c*j*y*z;

	return (new_dir);
}

void camera_move(t_minirt* vars)
{
	t_vec dir;
	t_vec delta;
	double d;

	if (vars->is_move == 13 || vars->is_move == 1)
	{
		dir = vars->scene.camera.forward;
		if (vars->is_move == 13)
			d = 1;
		else
			d = -1;
	}
	else if (vars->is_move == 0 || vars->is_move == 2)
	{
		dir = vars->scene.camera.right;
		if (vars->is_move == 2)
			d = 1;
		else
			d = -1;
	}
	else
		return ;
	delta = vec_division(vec_scalar_mul(dir, d), 1);
	t_vec new_org = vec_sum(vars->scene.camera.origin, delta);
	vars->scene.camera.origin = new_org;
}

void camera_rotate(t_minirt* vars)
{
	t_vec axis;
	t_vec delta;
	double d;

	if (vars->is_move == 126 || vars->is_move == 125)
	{
		axis = vars->scene.camera.right;
		if (vars->is_move == 126)
			d = -1;
		else
			d = 1;
	}
	else if (vars->is_move == 123 || vars->is_move == 124)
	{
		axis = vars->scene.camera.up;
		if (vars->is_move == 124)
			d = 1;
		else
			d = -1;
	}
	else
		return ;
	t_vec new_dir = rotate(axis, vars, d);
	vars->scene.camera.forward = new_dir;
	vars->scene.camera.dir = new_dir;
}

void camera_zoom(t_minirt* vars)
{
	double new_fov;

	if (vars->is_move == 4 || vars->is_move == 5)
	{
		if ((vars->is_move == 4 && vars->scene.camera.fov <= 10)
			|| (vars->is_move == 5 && vars->scene.camera.fov >= 170))
		{
			printf("cannot zoom more\n");
			vars->is_move = -1;
			return ;
		}
		if (vars->is_move == 4)
			new_fov = vars->scene.camera.fov - 10;
		else
			new_fov = vars->scene.camera.fov + 10;
		vars->scene.camera.fov = new_fov;
		vars->is_move = -1;
	}
}

int key_hook_move(t_minirt* vars)
{
	if (vars->scene.changed == 1)
	{
		//path_render(*vars);
		rt_render(vars);
		vars->scene.changed = 0;
	}
	if (vars->is_trace == 0 && vars->is_move != -1)
	{
		camera_move(vars);
		camera_rotate(vars);
		camera_zoom(vars);
		set_camera(&vars->scene.camera);
		//path_render(*vars);
		rt_render(vars);
	}
	return (1);
}

int	cam_key(t_minirt *vars, int keycode)
{
	if (keycode == W || keycode == A || keycode == S || keycode == D)
		key_press_move(vars, keycode);
	else if (keycode == UP || keycode == LEFT || keycode == RIGHT || keycode == DOWN)
		key_press_rotate(vars, keycode);
	else if (keycode == 15)
		key_press_mode_change(vars, keycode);
	return (1);
}

int transpose_obj(t_minirt *data, t_keycode keycode, int type, int *status) // object sphere
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

int rotate_obj(t_minirt *data, t_keycode keycode, int type, int *status)
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

int	keypress(int keycode, t_minirt* vars)
{
	static int status = -1;

	if (keycode == ESC)
		ft_close(vars);
	if ((status == -1 || status == ONE) && ((18 <= keycode && keycode <= 23) || keycode == 29))
		status = keycode;
	else if (status != -1)
	{
		if (status == TWO)
			transpose_obj(vars, keycode, SP, &status);
		else if (status == THREE)
			transpose_obj(vars, keycode, CY, &status);
		else if (status == FOUR)
			transpose_light(vars, keycode, &status);
		else if (status == FIVE)
			rotate_obj(vars, keycode, CY, &status);
		else if (status == SIX)
			rotate_obj(vars, keycode, PL, &status);
		else if (status == ONE)
			cam_key(vars, keycode);
	}
	return (0);
}

int	keyrelease(int keycode, t_minirt* vars)
{
	// printf("key release=%d\n", keycode);
	if (keycode == 13)
		vars->is_move = -1;
	else if (keycode == 0)
		vars->is_move = -1;
	else if (keycode == 1)
		vars->is_move = -1;
	else if (keycode == 2)
		vars->is_move = -1;
	else if (keycode == 126)
		vars->is_move = -1;
	else if (keycode == 123)
		vars->is_move = -1;
	else if (keycode == 125)
		vars->is_move = -1;
	else if (keycode == 124)
		vars->is_move = -1;
	return (0);
}

int scroll(int mousecode, int x, int y, t_minirt* vars)
{
	if (vars->is_trace == 1)
		printf("cannot zoom here\n");
	else if (mousecode == 4 || mousecode == 5)
		vars->is_move = mousecode;
	return (0);
}

void key_press_move(t_minirt* vars, int keycode)
{
	if (vars->is_trace == 1)
		printf("cannot move here\n");
	else
		vars->is_move = keycode;
}

void key_press_rotate(t_minirt* vars, int keycode)
{
	if (vars->is_trace == 1)
		printf("cannot rotate here\n");
	else
		vars->is_move = keycode;
}

void key_press_mode_change(t_minirt* vars, int keycode)
{
	if (vars->is_trace == 0)
	{
		vars->is_trace = 1;
		vars->scene.anti = 10;
		vars->scene.changed = 1;
	}
	else
	{
		vars->is_trace = 0;
		vars->scene.anti = 1;
		vars->scene.changed = 1;
	}
}