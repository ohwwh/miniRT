/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohw <ohw@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/21 20:29:20 by hako              #+#    #+#             */
/*   Updated: 2022/10/26 23:37:33 by ohw              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

double	degrees_to_radians(double degrees)
{
	return (degrees * PI / 180.0);
}

void	set_camera(t_camera *cam)
{
	double	theta;
	double	h;

	cam->ratio = ((double)WIDTH / (double)HEIGHT);
	theta = cam->fov * PI / 180.0;
	cam->viewport_height = tan(theta / 2);
	cam->viewport_width = cam->ratio * cam->viewport_height;
	cam->forward = cam->dir;
	cam->forward.x += EPS;
	cam->vup = create_vec(0, 1, 0);
	cam->right = unit_vec(vcross(vec_scalar_mul(cam->forward, -1),
				create_vec(0.0, -1.0, 0.0)));
	cam->up = unit_vec(vcross(vec_scalar_mul(cam->forward, -1), cam->right));
}

t_camera	create_camera(t_point lookfrom, t_point dir, t_vec vup, double vfov, double aspect_ratio)
{
	t_camera	ret;

	ret.origin = lookfrom;
	//ret.dir = dir; -> 바라보는 "지점"
	ret.dir = unit_vec(vec_sub(dir, lookfrom)); // -> 바라보는 "방향"
	ret.vup = vup;
	ret.fov = vfov;
	ret.ratio = aspect_ratio;

	return (ret);
}
