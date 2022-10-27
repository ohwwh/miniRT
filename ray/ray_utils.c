/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohw <ohw@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/25 00:16:52 by ohw               #+#    #+#             */
/*   Updated: 2022/10/26 23:42:49 by ohw              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

double	reflectance(double cos, double ref_ratio)
{
	double	r0;

	r0 = (1 - ref_ratio) / (1 + ref_ratio);
	r0 = r0 * r0;
	return (r0 + (1 - r0) * powf((1 - cos), 5));
}

t_vec	refract(t_vec v, t_vec n, double e, double cos)
{
	t_vec	perp;
	t_vec	parallel;

	perp = vec_scalar_mul(vec_sum(v, vec_scalar_mul(n, cos)), e);
	//e * (v + n * cos_theta)
	parallel = vec_scalar_mul(n, -sqrt(fabs(1.0 - powf(vec_len(perp), 2))));
	return (vec_sum(perp, parallel));
}

t_vec	reflect(t_vec v, t_vec n)
{
	return (vec_sub(v, vec_scalar_mul(n, 2 * vdot(v, n))));
}
