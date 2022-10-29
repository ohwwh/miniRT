/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohw <ohw@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/25 00:22:17 by ohw               #+#    #+#             */
/*   Updated: 2022/10/28 18:02:41 by ohw              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

double	clamp(double x)
{
	if (x < 0)
		return (0);
	if (x > 1)
		return (1);
	return (x);
}

int dmax(double a, double b, double c)
{
	if (a > b)
	{
		if (a > c)
			return (1);
		else
			return (3);
	}
	else
	{
		if (b > c)
			return (2);
		else
			return (3);
	}
}

void firefly(t_vec *color)
{
	const double	r = 15;
	const int		ret = dmax(color->x, color->y, color->z);
	double			a;

	if (ret == 1)
	{
		if (color->x > r)
		{
			a = r / color->x;
			color->x = r;
			color->y *= a;
			color->z *= a;
		}
	}
	else if (ret == 2)
	{
		if (color->y > r)
		{
			a = r / color->y;
			color->y = r;
			color->x *= a;
			color->z *= a;
		}
	}
	else
	{
		if (color->z > r)
		{
			a = r / color->z;
			color->z = r;
			color->x *= a;
			color->y *= a;
		}
	}
	
}

double tone_mapper(double x)
{
	double A = 0.15;
    double B = 0.50;
    double C = 0.10;
    double D = 0.20;
    double E = 0.02;
    double F = 0.30;

    return ((x*(A*x+C*B)+D*E)/(x*(A*x+B)+D*F))-E/F;
}

int	rgb_to_int(t_color c)
{
	t_vec tone;
	
	if (c.x != c.x)
		c.x = 0;
	if (c.y != c.y)
		c.y = 0;
	if (c.z != c.z)
		c.z = 0;
	return ((int)(255.999 * sqrt(clamp(c.x))) << 16
	| (int)(255.999 * sqrt(clamp(c.y))) << 8
	| (int)(255.999 * sqrt(clamp(c.z))));
	/*return ((int)(255.999 * sqrt(tone_mapper(c.x))) << 16
	| (int)(255.999 * sqrt(tone_mapper(c.y))) << 8
	| (int)(255.999 * sqrt(tone_mapper(c.z))));*/
}

t_color	get_sky_color(t_ray r)
{
	double	t;

	t = 0.5 * (unit_vec((r.dir)).y + 1.0);
	return (create_vec(
			(1.0 - t) + (0.5 * t), (1.0 - t) + (0.7 * t),
			(1.0 - t) + (1.0 * t)));
}
