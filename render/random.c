/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   random.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohw <ohw@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/21 20:29:28 by hako              #+#    #+#             */
/*   Updated: 2022/10/30 00:53:01 by ohw              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "random.h"

double	random_double(double min, double max, int anti)
{
	if (anti == 0 || anti == 1)
		return (0);
	return ((max - min) * ((double)rand() / (double)RAND_MAX) + min);
}

t_vec	rand_sphere(void)
{
	t_vec	ret;

	while (1)
	{
		ret = create_vec(random_double(-1, 1, 7),
				random_double(-1, 1, 7), random_double(-1, 1, 7));
		if (vec_len(ret) >= 1.0)
			continue ;
		return (ret);
	}
}

t_vec	random_unit_disk(void)
{
	t_vec	ret;
	
	while (1) 
	{
        set_vec(&ret, random_double(-1, 1, 7), random_double(-1, 1, 7), 0);
        if (powf(vec_len(ret), 2) >= 1)	
			continue;
        return (ret);
    }
}

t_vec	random_cosine_direction(void)
{
	const double	r1 = random_double(0, 1, 7);
	const double	r2 = random_double(0, 1, 7);
	const double	z = sqrt(1 - r2);
	const double	phi = 2 * PI * r1;

	return (create_vec(cos(phi) * sqrt(r2), sin(phi) * sqrt(r2), sqrt(1 - r2)));
}

t_onb	create_onb(t_vec n)
{
	t_onb	ret;
	t_vec	a;

	ret.w = unit_vec(n);
	if (fabs(ret.w.x) > 0.9)
		a = create_vec(0, 1, 0);
	else
		a = create_vec(1, 0, 0);
	ret.v = unit_vec(vcross(ret.w, a));
	ret.u = vcross(ret.w, ret.v);
	return (ret);
}

t_vec	local(t_onb *onb, t_vec a)
{
	return (
		vec_sum(
			vec_scalar_mul(onb->u, a.x),
			vec_sum(
				vec_scalar_mul(onb->v, a.y),
				vec_scalar_mul(onb->w, a.z)
			)
		)
	);
}
