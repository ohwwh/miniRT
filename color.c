#include "minirt.h"

t_vec calcul_ratio(t_vec col1, t_vec col2, double ratio)
{
	t_vec	ret;

	ret.x = col1.x * (col2.x / 255) * ratio;
	ret.y = col1.y * (col2.y / 255) * ratio;
	ret.z = col1.z * (col2.z / 255) * ratio;
	return (ret);
}

t_vec	add_color(t_vec col1, t_vec col2)
{
	t_vec	res;

	res = vec_sum(col1, col2);
	res = vmin(res, create_vec(255, 255, 255));
	return (res);
}

t_vec diffuse(t_hit_record hr, t_light *light, double d)
{
    t_vec	diff;
    t_vec   color;

    color.x = 255;
    color.y = 255;
    color.z = 255;
	diff = calcul_ratio(hr.color, color, d * light->ratio);
	return (diff);
}

int	shadow(t_scene *sc, t_hit_record hr, t_light *light)
{
	t_vec			hit_light;
	t_ray		sh_ray;
	t_hit_record		sh_hr;
	t_vec		hit_sh;

	hit_light = vec_sub(light->src, hr.p);
	sh_ray.origin = hr.p;
	sh_ray.dir = unit_vec(hit_light);
	sh_hr = find_hitpoint(&sh_ray, sc->objs);
	hit_sh = vec_sub(sh_hr.p, sh_ray.origin);
	if (sh_hr.t > EPS && (vec_len(hit_light) > vec_len(hit_sh)))
		return (1);
	return (0);
}


t_vec	calcul_color(t_scene *sc, t_hit_record hr, t_vec amb)
{
	t_light		*light;
	t_vec		ret;
	t_vec		hit_light;
	double		d;

	ret = create_vec(0, 0, 0);
	light = sc->light;
	if (!light)
		return (amb);
	if (shadow(sc, hr, light))
	{
		ret = add_color(ret, amb);
		// printf("shadow\n");
	}
	else
	{
		hit_light = vec_sub(light->src, hr.p);
		d = vdot(unit_vec(hit_light), hr.normal);
		ret = add_color(ret, amb);
		if (d >= 0)
			ret = add_color(ret, diffuse(hr, light, d));
	}
	return (ret);
}