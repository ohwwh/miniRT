#include "minirt.h"

/*t_hit_record hit_sphere(t_hit_record saved, t_ray *ray, t_objs *sp)
{
    t_hit_record hr;
    t_vec oc;
	t_discriminant d;
    double t;


    oc = vec_sub(ray->origin, sp->center);
	d.a = vdot(ray->dir, ray->dir);
	d.b = 2.0 * vdot(oc, ray->dir);
	d.c = vdot(oc, oc) - (sp->radius) * (sp->radius);
	t = d.b * d.b - (4 * d.a * d.c);
	if (t < EPS)
		hr.t = -1;
	d.t1 = (d.b * (-1.0) - sqrt(t)) / (2.0 * d.a);
	d.t2 = (d.b * (-1.0) + sqrt(t)) / (2.0 * d.a);
	if (d.t1 * d.t2 > EPS)
	{
		if (d.t1 > EPS)
        {
            if (d.t1 < d.t2)
                hr.t = d.t1;
            else
                hr.t = d.t2;
        }
        else
		    hr.t = -1;
	}
	else if (d.t1 > EPS)
		hr.t = d.t1;
	else 
        hr.t = d.t2;

    if (((saved.t > hr.t || saved.t == -1) && hr.t > EPS))
	{
		hr.color = sp->color;
		hr.p = vec_sum(ray->origin, vec_scalar_mul(ray->dir, hr.t));
		hr.normal = unit_vec(vec_sub(hr.p, sp->center));
		saved = hr;
	}
    return (saved);
}*/