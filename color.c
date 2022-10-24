/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hako <hako@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/21 20:29:13 by hako              #+#    #+#             */
/*   Updated: 2022/10/24 17:55:19 by hako             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

t_vec	calcul_ratio(t_vec col1, t_vec col2, double ratio)
{
	t_vec	ret;

	ret.x = (col1.x) * col2.x * ratio;
	ret.y = col1.y * (col2.y) * ratio;
	ret.z = col1.z * (col2.z) * ratio;
	return (ret);
}

t_vec	add_color(t_vec col1, t_vec col2)
{
	t_vec	res;

	res = vec_sum(col1, col2);
	res = create_vec(clamp(res.x), clamp(res.y), clamp(res.z));
	return (res);
}

t_vec	diffuse(t_hit_record hr, t_light *light, double d)
{
	t_vec	diff;
	t_vec	color;

	color.x = 1;
	color.y = 1;
	color.z = 1;
	diff = calcul_ratio(hr.color, color, d * light->ratio);
	return (diff);
}

t_vec	specular(t_hit_record hr, t_light *light, t_ray ray)
{
	double	spec;
	t_vec	view_dir;
	t_vec	reflect_dir;
	t_vec	specular;
	t_vec	light_dir;

	light_dir = unit_vec(vec_sub(light->src, hr.p));
	view_dir = unit_vec(vec_scalar_mul(ray.dir, -1));
	reflect_dir = reflect(vec_scalar_mul(light_dir, -1), hr.normal);
	spec = pow(fmax(vdot(view_dir, reflect_dir), 0.0), KSN);
	specular = vec_scalar_mul(vec_scalar_mul(create_vec(1, 1, 1), KS), spec);
	return (specular);
}

int	shadow(t_scene *sc, t_hit_record hr, t_light *light)
{
	t_vec			hit_light;
	t_ray			sh_ray;
	t_hit_record	sh_hr;
	t_vec			hit_sh;

	hit_light = vec_sub(light->src, hr.p);
	sh_ray.origin = hr.p;
	sh_ray.dir = unit_vec(hit_light);
	sh_hr = find_hitpoint(&sh_ray, sc->objs);
	hit_sh = vec_sub(sh_hr.p, sh_ray.origin);
	if (sh_hr.t > EPS && (vec_len(hit_light) > vec_len(hit_sh)))
		return (1);
	return (0);
}
