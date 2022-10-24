/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hit.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hako <hako@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/21 20:29:03 by hako              #+#    #+#             */
/*   Updated: 2022/10/24 19:24:44 by hako             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	set_face_normal(t_hit_record *rec, t_ray *ray, t_vec outward_normal)
{
	rec->front_face = vdot(ray->dir, outward_normal) < 0.0;
	if (rec->front_face != 0)
		rec->normal = unit_vec(outward_normal);
	else
		rec->normal = vec_scalar_mul(unit_vec(outward_normal), -1);
}

int	find_hitpoint_light(t_ray *ray, t_light *light, t_hit_record *rec)
{
	t_light	*temp;

	temp = light;
	while (temp)
	{
		if (temp->object.type == SP)
			hit_sphere(&temp->object, ray, rec);
		else if (temp->object.type == PL)
			hit_plane(&temp->object, ray, rec);
		else if (temp->object.type == CY)
		{
			hit_cylinder(&temp->object, ray, rec);
			hit_caps(&temp->object, ray, rec);
		}
		/*else if (temp->object.type == 4)
			hit_rectangle_xy(&temp->object, ray, rec);
		else if (temp->object.type == 5)
			hit_rectangle_yz(&temp->object, ray, rec);
		else if (temp->object.type == 6)
			hit_rectangle_xz(&temp->object, ray, rec);*/
		temp = temp->next;
	}
	return (1);
}

int	find_hitpoint_path(t_ray *ray, t_objs *objs,
		t_light *light, t_hit_record *rec)
{
	t_objs	*tmp;
	int		end;

	tmp = objs;
	while (tmp)
	{
		if (tmp->mat == -1)
			continue ;
		if (tmp->type == SP)
			hit_sphere(tmp, ray, rec);
		else if (tmp->type == PL)
			hit_plane(tmp, ray, rec);
		else if (tmp->type == CY)
		{
			hit_cylinder(tmp, ray, rec);
			hit_caps(tmp, ray, rec);
		}
		/*else if (tmp->type == 4)
			hit_rectangle_xy(tmp, ray, rec);
		else if (tmp->type == 5)
			hit_rectangle_yz(tmp, ray, rec);
		else if (tmp->type == 6)
			hit_rectangle_xz(tmp, ray, rec);*/
		tmp = tmp->next;
	}
	if (light && light->count != 0)
		find_hitpoint_light(ray, light, rec);
	return (1);
}

void	set_record(t_objs *s, t_ray *r, t_hit_record *rec, double root)
{
	rec->t = root;
	rec->color = s->color;
	rec->mat = s->mat;
	rec->refraction = s->refraction;
	rec->specular = s->specular;
	rec->fuzzy = s->fuzzy;
	rec->type = s->type;
}

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
}
