/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   random.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hako <hako@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/21 20:29:28 by hako              #+#    #+#             */
/*   Updated: 2022/10/21 20:29:29 by hako             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "random.h"

double random_double(double min, double max, int anti)
{
	if (anti == 0 || anti == 1)
		return (0);
	return ((max-min) * ((double)rand() / (double)RAND_MAX) + min);
}

t_vec rand_sphere()
{
	t_vec ret;

	while (1)
	{
		ret = create_vec(random_double(-1, 1, 7),
			random_double(-1,1,7), random_double(-1, 1, 7));
		if (vec_len(ret) >= 1.0)
			continue ;
		return (ret);
	}
}

t_vec rand_hemi_sphere(t_vec normal)
{
	t_vec ret;
	ret = rand_sphere();
	if (vdot(ret, normal) > 0.0)
		return (ret);
	else
		return (vec_scalar_mul(ret, -1));
}

t_vec random_cosine_direction()
{
	double pi = 3.1415926535897932385;
	double r1 = random_double(0, 1, 7);
	double r2 = random_double(0, 1, 7);
	double z = sqrt(1 - r2);

	double phi = 2 * pi * r1;
	double x = cos(phi) * sqrt(r2);
	double y = sin(phi) * sqrt(r2);

	return (create_vec(x, y, z));
}

t_onb create_onb(t_vec n)
{
	t_onb ret;
	t_vec a;

	ret.w = unit_vec(n);
	if (fabs(ret.w.x) > 0.9)
		a = create_vec(0, 1, 0);
	else
		a = create_vec(1, 0, 0);
	ret.v = unit_vec(vcross(ret.w, a));
	ret.u = vcross(ret.w, ret.v);

	return (ret);
}

t_vec local(t_onb *onb, t_vec a)
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


