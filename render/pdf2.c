/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pdf2.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohw <ohw@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/25 00:48:25 by ohw               #+#    #+#             */
/*   Updated: 2022/10/25 00:49:41 by ohw              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	generate_scattered(t_hit_record *rec, t_ray *scattered, t_onb *uvw)
{
	t_vec	ray_path;

	ray_path = local(uvw, random_cosine_direction());
	*scattered = ray(rec->p, ray_path);
}

void	generate_light_sample_sphere(
	t_hit_record *rec, t_ray *scattered, t_objs *light)
{
	t_onb	uvw;
	t_vec	ray_path;
	double	distance_squared;

	ray_path = vec_sub(light->center, rec->p);
	distance_squared = powf(vec_len(ray_path), 2);
	uvw = create_onb(ray_path);
	ray_path = local(&uvw, random_to_sphere(light->radius, distance_squared));
	*scattered = ray(rec->p, ray_path);
}

void	generate_random_importance(
	t_hit_record *rec, t_ray *scattered, t_light *temp, t_onb *uvw)
{
	const double	t = 0.5;

	if (random_double(0, 1, 7) < t)
	{
		if (temp->object.type == 3)
			generate_light_sample_sphere(rec, scattered, &temp->object);
	}
	else
		generate_scattered(rec, scattered, uvw);
}
