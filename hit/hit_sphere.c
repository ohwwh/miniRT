/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hit_sphere.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohw <ohw@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/24 19:20:54 by hako              #+#    #+#             */
/*   Updated: 2022/11/04 10:32:20 by ohw              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	hit_sphere(t_objs *s, t_ray *r, t_hit_record *rec)
{
	t_hit_record	hr;
	t_vec			oc;
	t_discriminant	d;
	double			root;

	oc = vec_sub(r->origin, s->center);
	d.a = vdot((r->dir), (r->dir));
	d.b = vdot(oc, (r->dir));
	d.c = vdot(oc, oc) - s->radius * s->radius;
	d.dsc = d.b * d.b - d.a * d.c;
	if (d.dsc < EPS)
		return ;
	root = (-d.b - sqrt(d.dsc)) / d.a;
	if (root < EPS || (rec->t != -1 && rec->t < root))
	{
		root = (-d.b + sqrt(d.dsc)) / d.a;
		if (root < EPS || (rec->t != -1 && rec->t < root))
			return ;
	}
	rec->p = ray_end(r, root);
	set_face_normal(rec, r,
		vec_division(vec_sub(rec->p, s->center), s->radius));
	set_record(s, r, rec, root);
}
