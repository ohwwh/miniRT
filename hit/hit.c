/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hit.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohw <ohw@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/21 20:29:03 by hako              #+#    #+#             */
/*   Updated: 2022/11/05 16:55:20 by ohw              ###   ########.fr       */
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
		else if (temp->object.type == 4)
			hit_rectangle_xy(&temp->object, ray, rec);
		else if (temp->object.type == 5)
			hit_rectangle_yz(&temp->object, ray, rec);
		else if (temp->object.type == 6)
			hit_rectangle_xz(&temp->object, ray, rec);
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
		else if (tmp->type == 4)
			hit_rectangle_xy(tmp, ray, rec);
		else if (tmp->type == 5)
			hit_rectangle_yz(tmp, ray, rec);
		else if (tmp->type == 6)
			hit_rectangle_xz(tmp, ray, rec);
		/*else if (tmp->type == BX)
			hit_box(tmp, ray, rec);*/
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
