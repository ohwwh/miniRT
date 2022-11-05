/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pdf.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohw <ohw@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/25 00:40:46 by ohw               #+#    #+#             */
/*   Updated: 2022/11/05 14:28:20 by ohw              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

t_vec	random_to_sphere(double radius, double distance_squared)
{
	double	r1 = random_double(0, 1, 7);
	double	r2 = random_double(0, 1, 7);
	double	z = 1 + r2 * (sqrt(1 - radius * radius / distance_squared) - 1);

	double	phi = 2 * PI * r1;
	double	x = cos(phi) * sqrt(1 - z * z);
	double	y = sin(phi) * sqrt(1 -z * z);

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

double	rectangle_light_pdf(
	t_hit_record *rec, t_ray *scattered, t_objs *light)
{
	t_hit_record	rec_new;
	const double	length_squared = powf(vec_len(scattered->dir), 2);
	double			area;
	double			cosine;

	if (!light)
		return (0);
	rec_new.t = -1.0;
	if (light->type == 4)
		hit_rectangle_xy(light, scattered, &rec_new);
	else if (light->type == 5)
		hit_rectangle_yz(light, scattered, &rec_new);
	else if (light->type == 6)
		hit_rectangle_xz(light, scattered, &rec_new);
	if (rec_new.t < 0.001)
		return (0);
	area = (light->center.y - light->center.x) * (light->dir.y - light->dir.x);
	cosine = fabs(vdot(scattered->dir, rec_new.normal) / sqrt(length_squared));
	return ((rec_new.t * rec_new.t * length_squared) / (cosine * area));
}

double	sphere_light_pdf(
	t_hit_record *rec, t_ray *scattered, t_objs *light)
{
	t_hit_record	rec_new;
	const double	length_squared = powf(vec_len(scattered->dir), 2);
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
	double			pdf_temp;
	double			pdf_sum;
	const double	t = LT;

	light_pdf_val = 0.0;
	pdf_sum = 0.0;
	temp = light;
	/*while (temp)
	{
		if (temp->object.type == 3)
			light_pdf_val += sphere_light_pdf(rec, scattered, &temp->object);
		else
			light_pdf_val += rectangle_light_pdf(rec, scattered, &temp->object);
		temp = temp->next;
	}
	return (t * light_pdf_val / light->count
		+ (1 - t) * cosine_pdf(&(rec->normal), &(uvw->w)));*/

	while (temp)
	{
		if (temp->object.type == 3)
			pdf_temp = sphere_light_pdf(rec, scattered, &temp->object);
		else
			pdf_temp = rectangle_light_pdf(rec, scattered, &temp->object);
		pdf_sum += pdf_temp;
		light_pdf_val += (pdf_temp * pdf_temp);
		temp = temp->next;
	}
	if (pdf_sum < EPS)
		pdf_sum = 1;
	return (t * light_pdf_val / pdf_sum + (1 - t) * cosine_pdf(&(rec->normal), &(uvw->w)));

	/*while (temp)
	{
		if (temp->object.type == 3)
			pdf_temp = sphere_light_pdf(rec, scattered, &temp->object);
		else
			pdf_temp = rectangle_light_pdf(rec, scattered, &temp->object);
		pdf_sum += pdf_temp;
		light_pdf_val += (pdf_temp * pdf_temp);
		temp = temp->next;
	}
	pdf_temp = cosine_pdf(&(rec->normal), &(uvw->w));
	pdf_sum += pdf_temp;
	light_pdf_val += (pdf_temp * pdf_temp);
	if (pdf_sum < EPS)
		pdf_sum = 1;
	return (light_pdf_val / pdf_sum); //brdf pdf까지 가중평균. 어둡다??*/
}

double	mixture_pdf_value(t_hit_record *rec, t_ray *scattered, t_light *light)
{
	t_onb	uvw;
	t_light	*temp;
	int		idx;

	temp = light;
	uvw = create_onb(rec->normal);
	if (!light)
	{
		generate_scattered(rec, scattered, &uvw);
		return (cosine_pdf(&(rec->normal), &(uvw.w)));
	}
	idx = rand() % light->count;
	while (temp && light->count && idx --)
		temp = temp->next;
	generate_random_importance(rec, scattered, temp, &uvw);
	return (get_pdf(rec, scattered, light, &uvw));
}
