#include "minirt.h"

t_hit_record hit_caps(t_hit_record saved, t_ray *ray, t_objs *cy)
{
	t_objs top_cap;
	t_hit_record hr;

	top_cap.center.x = cy->dir.x;
	top_cap.center.y = cy->dir.y;
	top_cap.center.z = cy->dir.z;

	top_cap.center = unit_vec(top_cap.center);

	top_cap.center.x = cy->height * top_cap.center.x + cy->center.x;
	top_cap.center.y = cy->height * top_cap.center.y + cy->center.y;
	top_cap.center.z = cy->height * top_cap.center.z + cy->center.z;
	
	top_cap.dir.x = cy->dir.x;
	top_cap.dir.y = cy->dir.y;
	top_cap.dir.z = cy->dir.z;

	top_cap.color.x = cy->color.x;
	top_cap.color.y = cy->color.y;
	top_cap.color.z = cy->color.z;

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
    double  D; // 판별식
    t_vec   normalized;
    double  a;
    double  b;
    double  c;
    double  t1;
    double  t2;
    double  h1, h2;

	// hr.t 구하기 //
    normalized = unit_vec(cy->dir);
	oc = vec_sub(ray->origin, cy->center);
	a = vdot(ray->dir, ray->dir) - (vdot(ray->dir, normalized)
			* vdot(ray->dir, normalized));
	b = 2 * (vdot(ray->dir, oc) - (vdot(ray->dir, normalized)
				* vdot(oc, normalized)));
	c = vdot(oc, oc)
		- (vdot(oc, normalized) * vdot(oc, normalized))
		- (cy->radius) * (cy->radius);
	D = b * b - 4 * a * c;
	if (D < EPS)
		hr.t = -1.0;
	else
    {
        t1 = (-b + sqrt(D)) / (2 * a);
	    t2 = (-b - sqrt(D)) / (2 * a);
	    if (t1 < EPS)
		    hr.t = -1.0;
		else
		{
	    	h1 = vdot(ray->dir, normalized) * t1 + vdot(oc, normalized);
	    	h2 = vdot(ray->dir, normalized) * t2 + vdot(oc, normalized);
	    	if (h2 >= EPS && h2 <= cy->height)
		    	hr.t = t2;
	    	else if (h1 >= EPS && h1 <= cy->height)
		    	hr.t = t1;
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
}