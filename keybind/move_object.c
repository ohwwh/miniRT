/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   move_object.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohw <ohw@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/25 00:14:49 by ohw               #+#    #+#             */
/*   Updated: 2022/10/25 00:14:54 by ohw              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

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

	if (data->is_move == W || data->is_move == S)
	{
		d = data->scene.camera.up;
		if (data->is_move == W)
			dir = 1;
		else
			dir = -1;
	}
	else if (data->is_move == A || data->is_move == D)
	{
		d = data->scene.camera.right;
		if (data->is_move == D)
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

	if (data->is_move == UP || data->is_move == DOWN)
	{
		axis = data->scene.camera.right;
		if (data->is_move == UP)
			d = -1;
		else
			d = 1;
	}
	else if (data->is_move == LEFT || data->is_move == RIGHT)
	{
		axis = data->scene.camera.up;
		if (data->is_move == RIGHT)
			d = 1;
		else
			d = -1;
	}
	else
		return ;
	non_light_rotate(data, axis, d, type);
}
