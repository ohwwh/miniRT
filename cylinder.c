#include "minirt.h"

/*t_hit_record hit_caps(t_hit_record saved, t_ray *ray, t_objs *cy)
{
	t_objs top_cap;
	t_hit_record hr;

	set_vec(&top_cap.center, cy->dir.x, cy->dir.y, cy->dir.z);
	top_cap.center = unit_vec(top_cap.center);
	top_cap.center = vec_scalar_mul(top_cap.center, cy->height);
	top_cap.center = vec_sum(top_cap.center, cy->center);
	set_vec(&top_cap.dir, cy->dir.x, cy->dir.y, cy->dir.z);
	set_vec(&top_cap.color, cy->color.x, cy->color.y, cy->color.z);

	hr = hit_plane(saved, ray, &top_cap);
	if (powf(hr.p.x - top_cap.center.x, 2.) + powf(hr.p.y - top_cap.center.y, 2.) + powf(hr.p.z - top_cap.center.z, 2.) <= powf(cy->radius, 2.))
		saved = hr;
	hr = hit_plane(saved, ray, cy);
	if (powf(hr.p.x - cy->center.x, 2.) + powf(hr.p.y - cy->center.y, 2.) + powf(hr.p.z - cy->center.z, 2.) <= powf(cy->radius, 2.))
		saved = hr;
	return (saved);
}

t_hit_record hit_cylinder(t_hit_record saved, t_ray *ray, t_objs *cy)
{   
    t_hit_record hr;
	double	m;
	t_vec	oc;
    t_vec   normalized;
    t_discriminant d;
    double  h1, h2;

	// hr.t 구하기 //
    normalized = unit_vec(cy->dir);
	oc = vec_sub(ray->origin, cy->center);
	d.a = vdot(ray->dir, ray->dir) - (vdot(ray->dir, normalized)
			* vdot(ray->dir, normalized));
	d.b = 2 * (vdot(ray->dir, oc) - (vdot(ray->dir, normalized)
				* vdot(oc, normalized)));
	d.c = vdot(oc, oc)
		- (vdot(oc, normalized) * vdot(oc, normalized))
		- (cy->radius) * (cy->radius);
	d.Dsc = d.b * d.b - 4 * d.a * d.c;
	if (d.Dsc < EPS)
		hr.t = -1.0;
	else
    {
        d.t1 = (-d.b + sqrt(d.Dsc)) / (2 * d.a);
	    d.t2 = (-d.b - sqrt(d.Dsc)) / (2 * d.a);
	    if (d.t1 < EPS)
		    hr.t = -1.0;
		else
		{
	    	h1 = vdot(ray->dir, normalized) * d.t1 + vdot(oc, normalized);
	    	h2 = vdot(ray->dir, normalized) * d.t2 + vdot(oc, normalized);
	    	if (h2 >= EPS && h2 <= cy->height)
		    	hr.t = d.t2;
	    	else if (h1 >= EPS && h1 <= cy->height)
		    	hr.t = d.t1;
	    	else
            	hr.t = -1.0;
		}
    }

	if (((saved.t > hr.t || saved.t == -1) && hr.t > EPS))
	{
		hr.color = cy->color;
		hr.p = vec_sum(ray->origin, vec_scalar_mul(ray->dir, hr.t));
		oc = unit_vec(cy->dir);
		m = vdot(ray->dir, vec_scalar_mul(oc, hr.t))
			+ vdot(vec_sub(ray->origin, cy->center), oc);
		hr.normal = unit_vec(vec_sub(vec_sub(hr.p, cy->center),
					vec_scalar_mul(oc, m)));
		saved = hr;
	}
    return (saved);
}*/