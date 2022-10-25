/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hit_cylinder.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hako <hako@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/24 19:23:15 by hako              #+#    #+#             */
/*   Updated: 2022/10/24 19:57:49 by hako             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

t_discriminant	get_discriminant(t_objs *cy, t_ray *ray,
	t_vec normalized, t_vec oc)
{
	t_discriminant	d;

	d.a = vdot(ray->dir, ray->dir) - (vdot(ray->dir, normalized)
			* vdot(ray->dir, normalized));
	d.b = 2 * (vdot(ray->dir, oc) - (vdot(ray->dir, normalized)
				* vdot(oc, normalized)));
	d.c = vdot(oc, oc) - (vdot(oc, normalized) * vdot(oc, normalized))
		- (cy->radius) * (cy->radius);
	d.dsc = d.b * d.b - 4 * d.a * d.c;
	return (d);
}

double	do_hit_cylinder(t_discriminant d, t_vec oc, t_objs *cy, t_ray *ray)
{
	double		h1;
	double		h2;
	t_vec		normalized;

	d.t1 = (-d.b + sqrt(d.dsc)) / (2 * d.a);
	d.t2 = (-d.b - sqrt(d.dsc)) / (2 * d.a);
	normalized = unit_vec(cy->dir);
	if (d.t1 < EPS)
		return (-1);
	else
	{
		h1 = vdot(ray->dir, normalized) * d.t1 + vdot(oc, normalized);
		h2 = vdot(ray->dir, normalized) * d.t2 + vdot(oc, normalized);
		if (h2 >= EPS && h2 <= cy->height)
			return (d.t2);
		else if (h1 >= EPS && h1 <= cy->height)
			return (d.t1);
		else
			return (-1);
	}
	return (-1);
}

void	hit_cylinder(t_objs *cy, t_ray *ray, t_hit_record *rec)
{
	double			m;
	t_vec			oc;
	t_discriminant	d;
	t_vec			normalized;
	double			root;

	normalized = unit_vec(cy->dir);
	oc = vec_sub(ray->origin, cy->center);
	d = get_discriminant(cy, ray, normalized, oc);
	if (d.dsc < EPS)
		return ;
	root = do_hit_cylinder(d, oc, cy, ray);
	if (root < EPS || (rec->t != -1 && rec->t < root))
		return ;
	set_record(cy, ray, rec, root);
	rec->p = vec_sum(ray->origin, vec_scalar_mul(ray->dir, root));
	m = vdot(ray->dir, vec_scalar_mul(normalized, root))
		+ vdot(vec_sub(ray->origin, cy->center), normalized);
	set_face_normal(rec, ray, unit_vec(vec_sub(vec_sub(rec->p,
					cy->center), vec_scalar_mul(normalized, m))));
}

void	do_hit_caps(t_objs top_cap, t_objs *cy, t_ray *ray, t_hit_record *rec)
{
	t_hit_record	hr;
	t_hit_record	hr2;

	hr = *rec;
	hr2 = *rec;
	hit_plane(&top_cap, ray, &hr);
	if (powf(hr.p.x - top_cap.center.x, 2) + powf(hr.p.y - top_cap.center.y, 2)
		+ powf(hr.p.z - top_cap.center.z, 2) <= powf(cy->radius, 2))
	{
		hr2 = hr;
		*rec = hr;
	}
	hit_plane(cy, ray, &hr2);
	if (powf(hr2.p.x - cy->center.x, 2) + powf(hr2.p.y - cy->center.y, 2)
		+ powf(hr2.p.z - cy->center.z, 2) <= powf(cy->radius, 2))
		*rec = hr2;
}

void	hit_caps(t_objs *cy, t_ray *ray, t_hit_record *rec)
{
	t_objs			top_cap;

	set_vec(&top_cap.center, cy->dir.x, cy->dir.y, cy->dir.z);
	top_cap.center = unit_vec(top_cap.center);
	top_cap.center = vec_scalar_mul(top_cap.center, cy->height);
	top_cap.center = vec_sum(top_cap.center, cy->center);
	set_vec(&top_cap.dir, cy->dir.x, cy->dir.y, cy->dir.z);
	set_vec(&top_cap.color, cy->color.x, cy->color.y, cy->color.z);
	top_cap.mat = cy->mat;
	top_cap.refraction = cy->refraction;
	top_cap.specular = cy->specular;
	top_cap.fuzzy = cy->fuzzy;
	do_hit_caps(top_cap, cy, ray, rec);
}
