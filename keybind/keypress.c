/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keypress.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohw <ohw@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/25 00:04:57 by ohw               #+#    #+#             */
/*   Updated: 2022/10/27 03:28:17 by ohw              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

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
		vars->scene.anti = ANTI;
		vars->scene.changed = 1;
	}
	else
	{
		vars->is_trace = 0;
		vars->scene.anti = 1;
		vars->scene.changed = 1;
	}
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
	else if (keycode == 8)
	{
		t_vec unit;

		unit = unit_vec(vars->scene.camera.dir);
		printf("origin: (%lf, %lf, %lf), direction: (%lf, %lf, %lf)\n",
			vars->scene.camera.origin.x, vars->scene.camera.origin.y,
			vars->scene.camera.origin.z,
			unit.x, unit.y, unit.z);
	}
	return (0);
}
