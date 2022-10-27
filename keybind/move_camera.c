/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   move_camera.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohw <ohw@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/25 00:14:44 by ohw               #+#    #+#             */
/*   Updated: 2022/10/27 03:31:25 by ohw              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	camera_move(t_minirt *vars)
{
	t_vec	d;
	double	dir;

	if (vars->is_move == W || vars->is_move == S)
	{
		d = vars->scene.camera.forward;
		if (vars->is_move == W)
			dir = 1;
		else
			dir = -1;
	}
	else if (vars->is_move == A || vars->is_move == D)
	{
		d = vars->scene.camera.right;
		if (vars->is_move == D)
			dir = 1;
		else
			dir = -1;
	}
	else
		return ;
	d = vec_scalar_mul(vec_scalar_mul(d, dir),
			vars->scene.camera.distance / CAM_SPEED);
	vars->scene.camera.origin = vec_sum(vars->scene.camera.origin, d);
}

void	camera_rotate(t_minirt *vars)
{
	t_vec	axis;
	t_vec	new_dir;
	double	dir;

	if (vars->is_move == UP || vars->is_move == DOWN)
	{
		axis = vars->scene.camera.right;
		if (vars->is_move == UP)
			dir = -1;
		else
			dir = 1;
	}
	else if (vars->is_move == LEFT || vars->is_move == RIGHT)
	{
		axis = vars->scene.camera.up;
		if (vars->is_move == RIGHT)
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
