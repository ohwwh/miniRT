#include "minirt.h"

t_hit_record hit_sphere(t_hit_record saved, t_ray *ray, t_objs *sp)
{
    t_hit_record hr;
    t_vec oc;
    double a;
    double b;
    double c;
    double t;
    double t1;
    double t2;

    oc = vec_sub(ray->origin, sp->cen);
	a = vdot(ray->dir, ray->dir);
	b = 2.0 * vdot(oc, ray->dir);
	c = vdot(oc, oc) - (sp->p.x / 2.0) * (sp->p.x / 2.0);
	t = b * b - (4 * a * c);
	if (t < EPS)
		hr.t = -1;
	t1 = (b * (-1.0) - sqrt(t)) / (2.0 * a);
	t2 = (b * (-1.0) + sqrt(t)) / (2.0 * a);
	if (t1 * t2 > EPS)
	{
		if (t1 > EPS)
        {
            if (t1 < t2)
                hr.t = t1;
            else
                hr.t = t2;
        }
        else
		    hr.t = -1;
	}
	else if (t1 > EPS)
		hr.t = t1;
	else 
        hr.t = t2;

    if (((saved.t > hr.t || saved.t == -1) && hr.t > EPS))
	{
		hr.color = sp->col;
		hr.p = vec_sum(ray->origin, vec_scalar_mul(ray->dir, hr.t));
		hr.normal = unit_vec(vec_sub(hr.p, sp->cen));
		saved = hr;
	}
    return (saved);
}