/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   move_camera.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohw <ohw@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/25 00:14:44 by ohw               #+#    #+#             */
/*   Updated: 2022/10/25 00:14:45 by ohw              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

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
