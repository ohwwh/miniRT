/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohw <ohw@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/25 00:10:20 by ohw               #+#    #+#             */
/*   Updated: 2022/10/25 00:11:16 by ohw              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	init_rt(t_minirt *data)
{
	data->mlx.mlx = mlx_init();
	data->mlx.mlx_win = mlx_new_window(data->mlx.mlx, WIDTH, HEIGHT, "miniRT");
	data->mlx.img = mlx_new_image(data->mlx.mlx, WIDTH, HEIGHT);
	data->mlx.addr = mlx_get_data_addr(data->mlx.img, &data->mlx.bits_per_pixel,
			&data->mlx.line_length, &data->mlx.endian);
	data->scene.objs = NULL;
	data->scene.amb.count = 0;
	data->scene.camera.count = 0;
	data->scene.light = NULL;
	data->is_move = -1;
	data->is_trace = 0;
	data->mode = 0;
	data->scene.anti = 1;
	data->scene.changed = 0;
}

void	set_init_distance(t_minirt *data)
{
	if (!data->scene.light->count)
		return ;
	data->scene.camera.distance = vec_len(vec_sub(data->scene.camera.origin,
				data->scene.objs->center));
	data->scene.light->distance = vec_len(
			vec_sub(data->scene.light->object.center,
				data->scene.objs->center));
}

void	set_light_attribute(t_scene *sc, double min)
{
	t_light	*tmp;

	tmp = sc->light;
	while (tmp)
	{
		tmp->object.center = tmp->src;
		tmp->object.color = create_vec(45, 45, 45);
		tmp->object.type = SP;
		tmp->object.mat = -1;
		tmp->object.radius = min / 2;
		tmp->object.next = 0;
		tmp = tmp->next;
	}
}

int	create_light_object(t_scene *sc)
{
	t_objs	*tmp;
	double	min;

	if (!sc->light->count)
		return (0);
	min = INFINITY;
	tmp = sc->objs;
	while (tmp)
	{
		if (tmp->type != PL)
		{
			if (min > tmp->radius)
				min = tmp->radius;
		}
		tmp = tmp->next;
	}
	if (min == INFINITY)
		min = 10;
	set_light_attribute(sc, min);
	return (1);
}
