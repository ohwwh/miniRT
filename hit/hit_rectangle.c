/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hit_rectangle.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohw <ohw@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/26 23:54:08 by ohw               #+#    #+#             */
/*   Updated: 2022/10/28 11:03:05 by ohw              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void hit_rectangle_xy(t_objs *rect, t_ray *ray, t_hit_record* rec)
{
	double t = (rect->radius - ray->origin.z) / ray->dir.z;
    if (t < EPS || (rec->t != -1 && rec->t < t))
        return ;
    double x = ray->origin.x + t * ray->dir.x;
    double y = ray->origin.y + t * ray->dir.y;
    if (x < rect->center.x || x > rect->center.y
	|| y < rect->dir.x || y > rect->dir.y)
        return ;
    rec->t = t;
	rec->tmax = t;
	rec->mat = rect->mat;
	rec->refraction = rect->refraction;
	rec->specular = rect->specular;
	rec->fuzzy = rect->fuzzy;
    set_face_normal(rec, ray, create_vec(0, 0, 1));
    rec->p = ray_end(ray, t);
	rec->color = rect->color;
	rec->type = rect->type;
}

void hit_rectangle_yz(t_objs *rect, t_ray *ray, t_hit_record* rec)
{
	double t = (rect->radius - ray->origin.x) / ray->dir.x;
	if (t < EPS || (rec->t != -1 && rec->t < t))
        return ;
	double y = ray->origin.y + t * ray->dir.y;
	double z = ray->origin.z + t * ray->dir.z;
	if (y < rect->center.x || y > rect->center.y
		|| z < rect->dir.x || z > rect->dir.y)
        return ;
    rec->t = t;
	rec->tmax = t;
	rec->mat = rect->mat;
	rec->refraction = rect->refraction;
	rec->specular = rect->specular;
	rec->fuzzy = rect->fuzzy;
	set_face_normal(rec, ray, create_vec(1, 0, 0));
	rec->p = ray_end(ray, t);
	rec->color = rect->color;
	rec->type = rect->type;
}

void	hit_rectangle_xz(t_objs *rect, t_ray *ray, t_hit_record* rec)
{
	double	t;
	double	x;
	double	z;

	t = (rect->radius - ray->origin.y) / ray->dir.y;
	x = ray->origin.x + t * ray->dir.x;
	z = ray->origin.z + t * ray->dir.z;
	if (t < EPS || (rec->t != -1 && rec->t < t))
		return ;
	if (x < rect->center.x || x > rect->center.y
		|| z < rect->dir.x || z > rect->dir.y)
		return ;
	rec->t = t;
	rec->tmax = t;
	rec->mat = rect->mat;
	rec->refraction = rect->refraction;
	rec->fuzzy = rect->fuzzy;
	rec->specular = rect->specular;
	set_face_normal(rec, ray, create_vec(0, 1, 0));
	rec->p = ray_end(ray, t);
	rec->color = rect->color;
	rec->type = rect->type;
}