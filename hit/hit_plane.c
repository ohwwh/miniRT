/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hit_plane.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hako <hako@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/24 19:24:27 by hako              #+#    #+#             */
/*   Updated: 2022/10/24 19:25:00 by hako             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	hit_plane(t_objs *pl, t_ray *ray, t_hit_record *rec)
{
	t_vec	x;
	t_vec	normal;
	double	b;
	double	a;
	double	root;

	normal = unit_vec(pl->dir);
	x = vec_sub(ray->origin, pl->center);
	b = vdot(ray->dir, normal);
	if (b != 0)
	{
		a = vdot(x, normal);
		root = -a / b;
		if (root < EPS)
			return ;
	}
	else
		return ;
	if (root < EPS || (rec->t != -1 && rec->t < root))
		return ;
	set_record(pl, ray, rec, root);
	rec->p = vec_sum(ray->origin, vec_scalar_mul(ray->dir, root));
	set_face_normal(rec, ray, pl->dir);
}
