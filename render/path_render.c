/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_render.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohw <ohw@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/25 00:08:40 by ohw               #+#    #+#             */
/*   Updated: 2022/10/25 00:37:12 by ohw              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

t_color	ray_color_raw(t_ray r, t_scene *sc)
{
	t_hit_record	rec;
	double			t;

	rec.t = -1.0;
	find_hitpoint_path(&r, sc->objs, sc->light, &rec);
	if (rec.t > EPS)
		return (rec.color);
	t = 0.5 * (unit_vec((r.dir)).y + 1.0);
	return (vec_scalar_mul(get_sky_color(r), 1));
}

t_color	ray_color(t_ray r, t_scene *sc, int depth)
{
	t_hit_record	rec;
	t_ray			scattered;
	double			t;
	double			pdf;

	rec.t = -1.0;
	if (depth <= 0)
		return (create_vec(0, 0, 0));
	find_hitpoint_path(&r, sc->objs, sc->light, &rec);
	if (rec.t >= EPS)
	{
		pdf = scatter(&r, &rec, &scattered, sc->light);
		if (rec.mat != -1)
		{
			r.color = vec_mul(
					vec_scalar_mul(rec.color, scattering_pdf(&scattered, &rec)),
					vec_division(ray_color(scattered, sc, depth - 1), pdf));
		}
		else
			r.color = rec.color;
		return (r.color);
	}
	t = 0.5 * (unit_vec((r.dir)).y + 1.0);
	return (vec_scalar_mul(get_sky_color(r), sc->amb.ratio * 0.5));
}

void	sampling(t_minirt *vars, int x, int y)
{
	double	u;
	double	v;
	t_ray	init_ray;

	u = (((double)x + random_double(0, 1, vars->scene.anti)) * 2 / WIDTH) - 1;
	v = (((double)y + random_double(0, 1, vars->scene.anti)) * 2 / HEIGHT) - 1;
	init_ray = ray_primary(&(vars->scene.camera), u, v);
	if (vars->is_trace == 1)
		vars->ray.color = vec_sum(vars->ray.color,
				ray_color(init_ray, &vars->scene, MAX_DEPTH));
	else
		vars->ray.color = vec_sum(vars->ray.color,
				ray_color_raw(init_ray, &vars->scene));
}

void	path_render(t_minirt *v)
{
	int	x;
	int	y;
	int	s;

	y = HEIGHT - 1;
	while (y -- >= 0)
	{
		x = 0;
		if (v->is_trace == 1)
		{
			printf("\rScanlines remaining: %d", y);
			fflush(stdout);
		}
		while (x ++ < WIDTH)
		{
			v->ray.color = create_vec(0, 0, 0);
			s = 0;
			while (s ++ < v->scene.anti)
				sampling(v, x, y);
			v->ray.color = vec_division(v->ray.color, v->scene.anti);
			put_color(&v->mlx, x - 1,
				HEIGHT - 2 - y, rgb_to_int(v->ray.color));
		}
	}
	mlx_put_image_to_window(v->mlx.mlx, v->mlx.mlx_win, v->mlx.img, 0, 0);
}
