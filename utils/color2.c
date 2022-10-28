/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohw <ohw@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/25 00:22:17 by ohw               #+#    #+#             */
/*   Updated: 2022/10/28 12:06:57 by ohw              ###   ########.fr       */
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

void firefly(t_vec *color)
{
	const double r = 10;
	
	if (color->x > r)
		color->x = r;
	if (color->y > r)
		color->y = r;
	if (color->z > r)
		color->z = r;
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
