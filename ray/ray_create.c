/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_create.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohw <ohw@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/25 00:44:17 by ohw               #+#    #+#             */
/*   Updated: 2022/11/04 15:22:10 by ohw              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

t_ray	ray(t_point origin, t_vec dir)
{
	t_ray	ret;

	ret.origin = origin;
	ret.dir = dir;
	return (ret);
}

t_ray	ray_primary(t_camera *cam, double u, double v)
{
	t_vec	rd;
	t_ray	ray;
	t_vec	offset;
	t_point	focul_point;

	ray.p = 1;
	ray.origin = cam->origin;
	ray.dir = vec_sum(vec_sum(vec_scalar_mul(cam->up, v * cam->viewport_height),
				vec_scalar_mul(cam->right,
					u * cam->viewport_width)), cam->forward);
	if (cam->aperture < EPS)
	{
		ray.dir = unit_vec(ray.dir);
		return (ray);
	}
	rd = vec_scalar_mul(random_unit_disk(), cam->aperture / 2);
	offset = vec_sum(vec_scalar_mul(cam->right, rd.x),
			vec_scalar_mul(cam->up, rd.y));			
	focul_point = ray_end(&ray, cam->distance);
	ray.origin = vec_sum(cam->origin, offset);
	ray.dir = vec_sub(focul_point, ray.origin);
	ray.dir = unit_vec(ray.dir);
	
	return (ray);
}

t_point	ray_end(t_ray *ray, double t)
{
	t_point	ret;

	ret.x = ray->origin.x + t * ray->dir.x;
	ret.y = ray->origin.y + t * ray->dir.y;
	ret.z = ray->origin.z + t * ray->dir.z;
	return (ret);
}
