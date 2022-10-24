/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hako <hako@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/21 20:29:20 by hako              #+#    #+#             */
/*   Updated: 2022/10/24 17:49:51 by hako             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "camera.h"

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

	/*double theta = degrees_to_radians(vfov);
	double h = tan(theta/2);
	double viewport_height = 2.0 * h;
	double viewport_width = aspect_ratio * viewport_height;

	t_vec w = unit_vec(vec_sub(lookfrom, dir)); //ㅋㅏ메라 뒤통수
	t_vec u = unit_vec(vcross(vup, w));  //right
	t_vec v = vcross(w, u); //up*/

	ret.origin = lookfrom;
	//ret.dir = dir; -> 바라보는 "지점"
	ret.dir = unit_vec(vec_sub(dir, lookfrom)); // -> 바라보는 "방향"
	ret.vup = vup;
	ret.fov = vfov;
	ret.ratio = aspect_ratio;

	/*ret.horizontal = vec_scalar_mul(u, viewport_width);
	ret.vertical = vec_scalar_mul(v, viewport_height);
	ret.lower_left_corner = create_vec(ret.origin.x + (-ret.horizontal.x / 2) + (-ret.vertical.x / 2) + (-w.x)
		,ret.origin.y + (- ret.horizontal.y / 2) + (-ret.vertical.y / 2) + (-w.y) 
		,ret.origin.z + (- ret.horizontal.z / 2) + (-ret.vertical.z / 2) + (-w.z));*/
	//(orgin) - (horizontal / 2) - (vertical / 2) - w
	

	/*double theta = degrees_to_radians(vfov);
	double h = tan(theta/2);
	ret.viewport_height = 2.0 * h;
	ret.viewport_width = aspect_ratio * ret.viewport_height;

	t_vec w = unit_vec(vec_sub(lookfrom, dir)); //ㅋㅏ메라 뒤통수
	t_vec u = unit_vec(vcross(vup, w));  //right
	t_vec v = vcross(w, u); //up

	ret.origin = lookfrom;
	ret.dir = dir;
	ret.vup = vup;
	ret.fov = vfov;
	ret.ratio = aspect_ratio;

	ret.horizontal = vec_scalar_mul(u, viewport_width);
	ret.vertical = vec_scalar_mul(v, viewport_height);
	ret.lower_left_corner = create_vec(ret.origin.x + (-ret.horizontal.x / 2) + (-ret.vertical.x / 2) + (-w.x)
		,ret.origin.y + (- ret.horizontal.y / 2) + (-ret.vertical.y / 2) + (-w.y) 
		,ret.origin.z + (- ret.horizontal.z / 2) + (-ret.vertical.z / 2) + (-w.z));
	//(orgin) - (horizontal / 2) - (vertical / 2) - w*/
	return (ret);
}
