/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   objects.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohw <ohw@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/21 20:28:47 by hako              #+#    #+#             */
/*   Updated: 2022/10/26 23:58:22 by ohw              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

double	get_light_size(t_objs object)
{
	double	rad;
	double	x;
	double	y;

	rad = object.radius;
	x = object.center.y - object.center.x;
	y = object.dir.y - object.dir.x;
	if (object.type == 3)
		return (rad * rad * PI);
	else if (object.type == 4 || object.type == 5 || object.type == 6)
		return (x * y);
	else
	{
		printf("unsupported\n");
		return (-1);
	}
}

t_objs create_sphere(t_point c, double r, t_color color, int mat)
{
	t_objs ret;

    ret.type = 3;
	ret.center = c;
	ret.radius = r;
	ret.color = color;
	ret.mat = mat;
	ret.refraction = 1.5;
	ret.specular = 0;
	return (ret);
}

t_objs create_cylinder(t_point c, double r, double h, t_vec dir, t_color color, int mat)
{
	t_objs ret;

	ret.type = 2;
	ret.center = c;
	ret.radius = r;
	ret.height = h;
	ret.dir = dir;
	ret.color = color;
	ret.mat = mat;
	ret.refraction = 1.5;
	ret.specular = 0;
	return (ret);
}

t_objs create_plane(t_point c, t_vec dir, t_color color, int mat)
{
	t_objs ret;

	ret.type = 1;
	ret.center = c;
	ret.dir = dir;
	ret.color = color;
	ret.mat = mat;
	ret.refraction = 1.5;
	ret.specular = 0;
	return (ret);
}

t_objs create_rectangle_xy(t_vec x, t_vec y, double k, t_color color, int mat)
{
	t_objs ret;

	ret.type = 4;
	ret.center = x;
	ret.dir = y;
	ret.radius = k;
	ret.color = color;
	ret.mat = mat;
	ret.refraction = 1.5;
	ret.specular = 0;

	return (ret);
}

t_objs create_rectangle_yz(t_vec y, t_vec z, double k, t_color color, int mat)
{
	t_objs ret;

	ret.type = 5;
	ret.center = y;
	ret.dir = z;
	ret.radius = k;
	ret.color = color;
	ret.mat = mat;
	ret.refraction = 1.5;
	ret.specular = 0;

	return (ret);
}

t_objs create_rectangle_xz(t_vec x, t_vec z, double k, t_color color, int mat)
{
	t_objs ret;

	ret.type = 6;
	ret.center = x;
	ret.dir = z;
	ret.radius = k;
	ret.color = color;
	ret.mat = mat;
	ret.refraction = 1.5;
	ret.specular = 0;

	return (ret);
}

