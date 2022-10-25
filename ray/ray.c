/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohw <ohw@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/21 20:28:27 by hako              #+#    #+#             */
/*   Updated: 2022/10/25 00:45:18 by ohw              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

t_hit_record	find_hitpoint(t_ray *ray, t_objs *objs)
{
	t_objs			*tmp;
	t_hit_record	saved;

	tmp = objs;
	saved.t = -1.0;
	while (tmp)
	{
		if (tmp->type == SP)
			hit_sphere(tmp, ray, &saved);
		else if (tmp->type == PL)
			hit_plane(tmp, ray, &saved);
		else if (tmp->type == CY)
		{
			hit_cylinder(tmp, ray, &saved);
			hit_caps(tmp, ray, &saved);
		}
		tmp = tmp->next;
	}
	return (saved);
}

int	is_inside(t_vec ray, t_vec norm)
{
	if (vdot(ray, norm) > 0)
		return (1);
	return (0);
}

t_vec	get_raycolor(t_minirt *data)
{
	t_hit_record	hr;
	t_vec			amb;
	t_vec			color;

	hr = find_hitpoint(&data->ray, data->scene.objs);
	if (hr.t > EPS)
	{
		amb = calcul_ratio(hr.color,
				data->scene.amb.col, data->scene.amb.ratio);
		if (is_inside(data->ray.dir, hr.normal))
			hr.normal = vec_scalar_mul(hr.normal, -1);
		color = calcul_color(&data->scene, hr, amb, data->ray);
		return (color);
	}
	return (vec_scalar_mul(data->scene.amb.col, data->scene.amb.ratio));
}
