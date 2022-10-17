#include "light.h"

t_light create_light(t_color color, t_point origin)
{
	t_light ret;
	ret.color = color;
	ret.origin = origin;
	return (ret);
}

t_color point_light_get(t_ray* ray, t_record* rec, t_light* light)
{
	t_color    diffuse;
	double kd;
	t_color specular;
	t_vec light_dir;
	t_vec view_dir;
	t_vec reflect_dir;
	double spec;
	double ksn;
	double ks;

	/*kd = fmax(vdot(rec->normal, light_dir), 0.0);// diffuse strength;
    diffuse = vec_scalar_mul(light->color, kd);*/

	light_dir = unit_vec(vec_sub(light->origin, rec->p));
	view_dir = unit_vec(vec_scalar_mul(ray->dir, -1));
    reflect_dir = reflect(vec_scalar_mul(light_dir, -1), rec->normal);
    ksn = 64; // shininess value
    ks = 0.5; // specular strength
    spec = pow(fmax(vdot(view_dir, reflect_dir), 0.0), ksn);
    specular = vec_scalar_mul(vec_scalar_mul(light->color, ks), spec);
    //return (vec_sum(specular, diffuse));
	return (specular);
}
