/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hako <hako@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/21 20:28:27 by hako              #+#    #+#             */
/*   Updated: 2022/10/24 18:35:17 by hako             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

t_point	ray_end(t_ray *ray, double t)
{
	t_point	ret;

	ret.x = ray->origin.x + t * ray->dir.x;
	ret.y = ray->origin.y + t * ray->dir.y;
	ret.z = ray->origin.z + t * ray->dir.z;
	return (ret);
}

/*void set_camera(t_camera *cam)
{
    double theta;
    double h;

	cam->ratio = (double) WIDTH / (double) HEIGHT;
	theta = cam->fov * PI / 180.0;
	cam->viewport_height = tan(theta / 2);
	cam->viewport_width = cam->ratio * cam->viewport_height;
    cam->forward = cam->dir;
	cam->forward.x += EPS;
    cam->vup = create_vec(0,1,0);
	cam->right = unit_vec(vcross(vec_scalar_mul(cam->forward, -1), create_vec(0.0, -1.0, 0.0)));
	cam->up = unit_vec(vcross(vec_scalar_mul(cam->forward, -1), cam->right));

}*/

t_hit_record	find_hitpoint(t_ray *ray, t_objs *objs)
{
	t_objs			*tmp;
	t_hit_record	saved;

	tmp = objs;
	saved.t = -1.0;
	while (tmp)
	{
		if (tmp->type == SP)
			hit_sphere(tmp, ray, &saved);
		else if (tmp->type == PL)
			hit_plane(tmp, ray, &saved);
		else if (tmp->type == CY)
		{
			hit_cylinder(tmp, ray, &saved);
			hit_caps(tmp, ray, &saved);
		}
		tmp = tmp->next;
	}
	return (saved);
}

int	is_inside(t_vec ray, t_vec norm)
{
	if (vdot(ray, norm) > 0)
		return (1);
	return (0);
}

t_vec	get_raycolor(t_minirt *data)
{
	t_hit_record	hr;
	t_vec			amb;
	t_vec			color;

	hr = find_hitpoint(&data->ray, data->scene.objs);
	if (hr.t > EPS)
	{
		amb = calcul_ratio(hr.color,
				data->scene.amb.col, data->scene.amb.ratio);
		if (is_inside(data->ray.dir, hr.normal))
			hr.normal = vec_scalar_mul(hr.normal, -1);
		color = calcul_color(&data->scene, hr, amb, data->ray);
		return (color);
	}
	return (vec_scalar_mul(data->scene.amb.col, data->scene.amb.ratio));
}

t_ray	ray_primary(t_camera *cam, double u, double v)
{
	t_ray	ray;

	ray.origin = cam->origin;
	ray.dir = vec_sum(vec_sum(vec_scalar_mul(cam->up, v * cam->viewport_height),
				vec_scalar_mul(cam->right,
					u * cam->viewport_width)), cam->forward);
	ray.dir = unit_vec(ray.dir);
	return (ray);
}
