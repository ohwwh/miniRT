/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hako <hako@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/19 18:36:33 by hako              #+#    #+#             */
/*   Updated: 2022/10/24 18:13:21 by hako             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CAMERA_H
# define CAMERA_H

# include "vector.h"
# define WIDTH 640
# define HEIGHT 320
# define PI 3.14159265358979323846
# define EPS 0.001

typedef struct t_camera
{
	t_vec	origin;
	t_vec	dir;
	double	distance;
	double	fov;
	double	ratio;
	double	viewport_height;
	double	viewport_width;
	t_vec	forward;
	t_vec	vup;
	t_vec	up;
	t_vec	right;
	int		count;

}	t_camera;

//t_camera create_camera(t_point lookfrom, t_point dir, t_vec vup, double vfov, double aspect_ratio);
void	set_camera(t_camera *cam);

#endif