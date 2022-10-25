/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hako <hako@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/21 20:29:20 by hako              #+#    #+#             */
/*   Updated: 2022/10/25 13:21:04 by hako             ###   ########.fr       */
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
