/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohw <ohw@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/25 00:22:17 by ohw               #+#    #+#             */
/*   Updated: 2022/10/25 00:36:18 by ohw              ###   ########.fr       */
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

int	rgb_to_int(t_color c)
{
	if (c.x != c.x)
		c.x = 0;
	if (c.y != c.y)
		c.y = 0;
	if (c.z != c.z)
		c.z = 0;
	return ((int)(255.999 * sqrt(clamp(c.x))) << 16
	| (int)(255.999 * sqrt(clamp(c.y))) << 8
	| (int)(255.999 * sqrt(clamp(c.z))));
}

t_color	get_sky_color(t_ray r)
{
	double	t;

	t = 0.5 * (unit_vec((r.dir)).y + 1.0);
	return (create_vec(
			(1.0 - t) + (0.5 * t), (1.0 - t) + (0.7 * t),
			(1.0 - t) + (1.0 * t)));
}
