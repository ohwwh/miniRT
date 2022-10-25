/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pdf.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohw <ohw@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/25 00:40:46 by ohw               #+#    #+#             */
/*   Updated: 2022/10/25 00:53:14 by ohw              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

t_vec	random_to_sphere(double radius, double distance_squared)
{
	const double	z = 1
		+ (random_double(0, 1, 7)
			* (sqrt(1 - radius * radius / distance_squared) - 1));
	const double	phi = 2 * PI * random_double(0, 1, 7);
	const double	x = cos(phi) * sqrt(1 - z * z);
	const double	y = sin(phi) * sqrt(1 - z * z);

	return (create_vec(x, y, z));
}

double	cosine_pdf(const t_vec *dir, const t_vec *w)
{
	double	pdf;
	double	cos;

	cos = vdot(unit_vec(*dir), *w);
	if (cos < 0.0)
		pdf = 0;
	else
		pdf = cos / PI;
	return (pdf);
}

double	sphere_light_pdf(t_ray *scattered, t_objs *light)
{
	t_hit_record	rec_new;
	double			cos_max;
	double			angle;

	if (!light)
		return (0);
	rec_new.t = -1.0;
	hit_sphere(light, scattered, &rec_new);
	if (rec_new.t < 0.001)
		return (0);
	cos_max = sqrt(1 - (light->radius * light->radius
				/ powf(vec_len(vec_sub(light->center, scattered->origin)), 2)));
	angle = 2 * PI * (1 - cos_max);
	return (1 / angle);
}

double	get_pdf(t_hit_record *rec, t_ray *scattered, t_light *light, t_onb *uvw)
{
	t_light			*temp;
	double			light_pdf_val;
	const double	t = 0.5;

	light_pdf_val = 0.0;
	temp = light;
	while (temp)
	{
		if (temp->object.type == 3)
			light_pdf_val += sphere_light_pdf(scattered, &temp->object);
		temp = temp->next;
	}
	return (t * light_pdf_val / light->count
		+ (1 - t) * cosine_pdf(&(rec->normal), &(uvw->w)));
}

double	mixture_pdf_value(t_hit_record *rec, t_ray *scattered, t_light *light)
{
	t_onb	uvw;
	t_light	*temp;
	int		idx;

	temp = light;
	uvw = create_onb(rec->normal);
	idx = rand() % light->count;
	while (temp && light->count && idx --)
		temp = temp->next;
	if (!light || !light->count
		|| temp->object.mat != -1 || !get_light_size(temp->object))
	{
		generate_scattered(rec, scattered, &uvw);
		return (cosine_pdf(&(rec->normal), &(uvw.w)));
	}
	generate_random_importance(rec, scattered, temp, &uvw);
	return (get_pdf(rec, scattered, light, &uvw));
}
