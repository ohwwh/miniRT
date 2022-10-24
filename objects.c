/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   objects.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hako <hako@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/21 20:28:47 by hako              #+#    #+#             */
/*   Updated: 2022/10/24 17:35:17 by hako             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "objects.h"

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
