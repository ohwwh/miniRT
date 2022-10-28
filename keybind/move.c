/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   move.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohw <ohw@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/21 20:28:52 by hako              #+#    #+#             */
/*   Updated: 2022/10/28 10:47:58 by ohw              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

int	ft_close(t_minirt *data)
{
	t_light	*light;
	t_objs	*obj;

	free(data->thr[0].sh);
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
	exit(0);
}

int	key_hook_move(t_minirt *vars)
{
	if (vars->scene.changed == 1)
	{
		if (vars->is_trace == 1)
			path_render_threaded(vars);
		else if (vars->is_trace == 0)
			raw_render(vars);
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
		raw_render(vars);
	}
	return (1);
}

int	keyrelease(int keycode, t_minirt *vars)
{
	if (keycode == W)
		vars->is_move = -1;
	else if (keycode == A)
		vars->is_move = -1;
	else if (keycode == S)
		vars->is_move = -1;
	else if (keycode == D)
		vars->is_move = -1;
	else if (keycode == UP)
		vars->is_move = -1;
	else if (keycode == LEFT)
		vars->is_move = -1;
	else if (keycode == DOWN)
		vars->is_move = -1;
	else if (keycode == RIGHT)
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
		printf("%d , %d\n", x, y);
	return (0);
}
