/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scatter.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohw <ohw@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/25 00:40:55 by ohw               #+#    #+#             */
/*   Updated: 2022/10/26 23:51:09 by ohw              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

double	scattering_pdf(t_ray *scattered, t_hit_record *rec)
{
	double	scat_pdf;
	double	cos;

	if (rec->mat != 0)
		return (1);
	cos = vdot(rec->normal, unit_vec(scattered->dir));
	if (cos < 0)
		scat_pdf = 0;
	else
		scat_pdf = cos / PI;
	return (scat_pdf);
}

double	scatter_refraction(t_ray *r, t_hit_record *rec, t_ray *scattered)
{
	t_vec			dir;
	double			ref_ratio;
	const double	cos
		= fmin(vdot(vec_scalar_mul(unit_vec(r->dir), -1), rec->normal), 1);
	const double	sin = sqrt(1.0 - cos * cos);

	if (rec->refraction == 0)
		printf("refraction is not set\n");
	if (rec->front_face)
		ref_ratio = 1.0 / rec->refraction;
	else
		ref_ratio = rec->refraction;
	if (ref_ratio * sin > 1.0
		|| reflectance(cos, ref_ratio) > random_double(0, 1, 7))
		dir = reflect(unit_vec(r->dir), rec->normal);
	else
		dir = refract(unit_vec(r->dir), rec->normal, ref_ratio, cos);
	*scattered = ray(rec->p, dir);
	return (1);
}

double	scatter_reflect(t_ray *r, t_hit_record *rec, t_ray *scattered)
{
	t_vec	fuzziness;

	fuzziness = vec_scalar_mul(rand_sphere(), rec->fuzzy);
	*scattered = ray(rec->p,
			vec_sum(reflect(unit_vec(r->dir), rec->normal), fuzziness));
	/*if (rec->mat != 1)
		set_vec(&rec->color, 1, 1, 1);*/
	if (vdot(scattered->dir, rec->normal) <= 0) //이 조건식은 무슨 의미인가??
		set_vec(&rec->color, 0, 0, 0);
	return (1);
}

double	scatter_diffuse(
	t_ray *r, t_hit_record *rec, t_ray *scattered, t_light *light)
{
	double	pdf;

	if (random_double(0, 1, 7) > rec->specular)
		pdf = mixture_pdf_value(rec, scattered, light);
	else
		pdf = scatter_reflect(r, rec, scattered);
	return (pdf);
}

double	scatter(t_ray *r, t_hit_record *rec, t_ray *scattered, t_light *light)
{
	t_onb	uvw;
	t_vec	dir;
	double	pdf;

	if (rec->mat == 0)
		return (scatter_diffuse(r, rec, scattered, light));
	else if (rec->mat == 1)
		return (scatter_reflect(r, rec, scattered));
	else if (rec->mat == 2)
		return (scatter_refraction(r, rec, scattered));
	else if (rec->mat == -1)
		return (1);
	else
		return (1);
}
