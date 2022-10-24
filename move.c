/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   move.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hako <hako@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/21 20:28:52 by hako              #+#    #+#             */
/*   Updated: 2022/10/24 18:57:55 by hako             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	object_move(t_minirt *data, int type);
void	object_rotate(t_minirt *data, int type);

int	ft_close(t_minirt *data)
{
	t_light	*light;
	t_objs	*obj;

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

t_vec	rotate(t_vec axis, t_vec vec, int dir)
{
    t_vec           new_dir;
	const double    c = (1 - cos(dir * 0.1));
	const double    s = sin(dir * 0.1);

	new_dir.x = - vec.x * c * axis.y * axis.y 
	- vec.z * s * axis.y + c * vec.y * axis.x * axis.y 
	- vec.x * c * axis.z * axis.z + vec.y * s * axis.z
	+ c * vec.z * axis.x * axis.z + vec.x;
	new_dir.y = vec.y - c * vec.y * axis.x * axis.x 
	+ vec.z * s * axis.x + vec.x * c * axis.x * axis.y 
	- c * vec.y * axis.z * axis.z - vec.x * s * axis.z + c * vec.z * axis.y * axis.z;
	new_dir.z = vec.z - c * vec.z * axis.x * axis.x
	- vec.y * s * axis.x - c * vec.z * axis.y * axis.y + vec.x * s * axis.y 
	+ vec.x * c * axis.x * axis.z + c * vec.y * axis.y * axis.z;

	return (new_dir);
}

void	camera_move(t_minirt *vars)
{
	t_vec	d;
	double	dir;

	if (vars->is_move == 13 || vars->is_move == 1)
	{
		d = vars->scene.camera.forward;
		if (vars->is_move == 13)
			dir = 1;
		else
			dir = -1;
	}
	else if (vars->is_move == 0 || vars->is_move == 2)
	{
		d = vars->scene.camera.right;
		if (vars->is_move == 2)
			dir = 1;
		else
			dir = -1;
	}
	else
		return ;
	d = vec_scalar_mul(vec_scalar_mul(d, dir),
			vars->scene.camera.distance / 10);
	vars->scene.camera.origin = vec_sum(vars->scene.camera.origin, d);
}

void	camera_rotate(t_minirt *vars)
{
	t_vec	axis;
	t_vec	new_dir;
	double	dir;

	if (vars->is_move == 126 || vars->is_move == 125)
	{
		axis = vars->scene.camera.right;
		if (vars->is_move == 126)
			dir = -1;
		else
			dir = 1;
	}
	else if (vars->is_move == 123 || vars->is_move == 124)
	{
		axis = vars->scene.camera.up;
		if (vars->is_move == 124)
			dir = 1;
		else
			dir = -1;
	}
	else
		return ;
	new_dir = rotate(axis, vars->scene.camera.forward, dir);
	vars->scene.camera.forward = new_dir;
	vars->scene.camera.dir = new_dir;
}

void	camera_zoom(t_minirt *vars)
{
	double	new_fov;

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

int	key_hook_move(t_minirt *vars)
{
	if (vars->scene.changed == 1)
	{
		if (vars->is_trace == 1 || vars->is_trace == 0)
			path_render(vars);
		else if (vars->is_trace == 2)
			rt_render(vars);
		vars->scene.changed = 0;
	}
	if (vars->is_trace == 0 && vars->is_move != -1)
	{
		if (vars->mode == 0)
		{
			camera_move(vars);
			camera_rotate(vars);
			camera_zoom(vars);
			set_camera(&vars->scene.camera);
		}
		else if (vars->mode != 0)
		{
			object_move(vars, vars->mode);
			object_rotate(vars, vars->mode);
		}
		path_render(vars);
	}
	return (1);
}

void	light_move(t_minirt *vars, t_vec delta)
{
	t_light	*tmp;

	tmp = vars->scene.light;
	while (tmp)
	{
		tmp->object.center = vec_sum(tmp->object.center, delta);
		tmp->src = vec_sum(tmp->src, delta);
		tmp = tmp->next;
	}
}

void	non_light_move(t_minirt *vars, int type, t_vec delta)
{
	t_objs	*tmp;

	tmp = vars->scene.objs;
	while (tmp)
	{
		if (tmp->type == type)
			tmp->center = vec_sum(tmp->center, delta);
		tmp = tmp->next;
	}
}

void	object_move(t_minirt *data, int type)
{
	t_vec	d;
	double	dir;

	if (data->is_move == 13 || data->is_move == 1)
	{
		d = data->scene.camera.up;
		if (data->is_move == 13)
			dir = 1;
		else
			dir = -1;
	}
	else if (data->is_move == 0 || data->is_move == 2)
	{
		d = data->scene.camera.right;
		if (data->is_move == 2)
			dir = 1;
		else
			dir = -1;
	}
	else
		return ;
	d = vec_scalar_mul(vec_scalar_mul(d, dir), 1);
	if (type == -1)
		return (light_move(data, d));
	return (non_light_move(data, type, d));
}

void	non_light_rotate(t_minirt *data, t_vec axis, double d, int type)
{
	t_objs	*tmp;

	tmp = data->scene.objs;
	while (tmp)
	{
		if (tmp->type == type && tmp->type != PL)
			tmp->dir = rotate(axis, tmp->dir, d);
		tmp = tmp->next;
	}
}

void	object_rotate(t_minirt *data, int type)
{
	t_objs	*tmp;
	t_vec	axis;
	double	d;

	if (data->is_move == 126 || data->is_move == 125)
	{
		axis = data->scene.camera.right;
		if (data->is_move == 126)
			d = -1;
		else
			d = 1;
	}
	else if (data->is_move == 123 || data->is_move == 124)
	{
		axis = data->scene.camera.up;
		if (data->is_move == 124)
			d = 1;
		else
			d = -1;
	}
	else
		return ;
	non_light_rotate(data, axis, d, type);
}

int	keypress(int keycode, t_minirt *vars)
{
	if (keycode == W || keycode == A || keycode == S || keycode == D)
		key_press_move(vars, keycode);
	else if (keycode == UP || keycode == LEFT
		|| keycode == RIGHT || keycode == DOWN)
		key_press_rotate(vars, keycode);
	else if (keycode == 15 || keycode == 35
		|| keycode == 18 || keycode == 19 || keycode == 20)
		key_press_mode_change(vars, keycode);
	else if (keycode == ESC)
		ft_close(vars);
	return (0);
}

int	keyrelease(int keycode, t_minirt *vars)
{
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

int	scroll(int mousecode, int x, int y, t_minirt *vars)
{
	if (vars->is_trace != 0)
		printf("cannot zoom here\n");
	else if (mousecode == 4 || mousecode == 5)
		vars->is_move = mousecode;
	else if (mousecode != 0)
		printf("%d , %d\n", vars->x, vars->y);
	return (0);
}

void	key_press_move(t_minirt *vars, int keycode)
{
	if (vars->is_trace == 1)
		printf("cannot move here\n");
	else
		vars->is_move = keycode;
}

void	key_press_rotate(t_minirt *vars, int keycode)
{
	if (vars->is_trace == 1)
		printf("cannot rotate here\n");
	else
		vars->is_move = keycode;
}

void	object_select_mode(t_minirt *vars, int keycode)
{
	if (vars->mode == 0)
	{
		if (keycode == 18)
			vars->mode = SP;
		else if (keycode == 19)
			vars->mode = CY;
		else if (keycode == 20)
			vars->mode = -1;
	}
	else if (vars->mode != 0)
		vars->mode = 0;
}

void	key_press_mode_change(t_minirt *vars, int keycode)
{
	if (vars->is_trace == 0)
	{
		if (keycode == 18 || keycode == 19 || keycode == 20)
			return (object_select_mode(vars, keycode));
		else if (keycode == 15)
			vars->is_trace = 1;
		else if (keycode == 35)
			vars->is_trace = 2;
		vars->scene.anti = 100;
		vars->scene.changed = 1;
	}
	else
	{
		vars->is_trace = 0;
		vars->scene.anti = 1;
		vars->scene.changed = 1;
	}
}
