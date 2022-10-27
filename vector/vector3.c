/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector3.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohw <ohw@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/24 17:21:52 by hako              #+#    #+#             */
/*   Updated: 2022/10/27 00:34:19 by ohw              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

t_vec	unit_vec(t_vec vec)
{
	double	len;

	len = vec_len(vec);
	if (len == 0)
		return (create_vec(1, 0, 0));
	vec.x /= len;
	vec.y /= len;
	vec.z /= len;
	return (vec);
}

t_vec	vmin(t_vec vec1, t_vec vec2)
{
	if (vec1.x > vec2.x)
		vec1.x = vec2.x;
	if (vec1.y > vec2.y)
		vec1.y = vec2.y;
	if (vec1.z > vec2.z)
		vec1.z = vec2.z;
	return (vec1);
}

int	near_zero(t_vec vec)
{
	double	s;

	s = 1e-8;
	return ((fabs(vec.x) < s) && (fabs(vec.y) < s) && (fabs(vec.z) < s));
}

t_vec	rotate(t_vec axis, t_vec vec, int dir)
{
	t_vec			new_dir;
	const double	c = (1 - cos(dir * 0.1));
	const double	s = sin(dir * 0.1);

	new_dir.x = -vec.x * c * axis.y * axis.y
		- vec.z * s * axis.y + c * vec.y * axis.x * axis.y
		- vec.x * c * axis.z * axis.z + vec.y * s * axis.z
		+ c * vec.z * axis.x * axis.z + vec.x;
	new_dir.y = vec.y - c * vec.y * axis.x * axis.x
		+ vec.z * s * axis.x + vec.x * c * axis.x * axis.y
		- c * vec.y * axis.z * axis.z
		- vec.x * s * axis.z + c * vec.z * axis.y * axis.z;
	new_dir.z = vec.z - c * vec.z * axis.x * axis.x
		- vec.y * s * axis.x - c * vec.z * axis.y * axis.y + vec.x * s * axis.y
		+ vec.x * c * axis.x * axis.z + c * vec.y * axis.y * axis.z;
	return (new_dir);
}
