#include "minirt.h"

/*t_hit_record hit_plane(t_hit_record saved, t_ray *ray, t_objs *pl)
{
    t_hit_record hr;
    t_vec	x;
	t_vec	normal;
	double	b;
	double	a;
	// if (pl->type == CY)
	// {
	// 	printf("%lf %lf %lf\n", pl->cen.x, pl->cen.y, pl->cen.z);
	// }

	normal = unit_vec(pl->dir);
	x = vec_sub(ray->origin, pl->center);
	b = vdot(ray->dir, normal);
	if (b != 0)
	{
		a = vdot(x, normal);
		hr.t = -a / b;
		if (hr.t < EPS)
            hr.t = -1.0;
	}
    else
	    hr.t = -1.0;
    
    if (((saved.t > hr.t || saved.t == -1) && hr.t > EPS))
	{
		hr.color = pl->color;
		hr.p = vec_sum(ray->origin, vec_scalar_mul(ray->dir, hr.t));
		hr.normal = pl->dir;
		if (vdot(ray->dir, hr.normal) > __DBL_EPSILON__) // 부동 소수점 오차 범위 내에서 비교
			hr.normal = unit_vec(vec_scalar_mul(pl->dir, -1));
		saved = hr;
	}
    return (saved);
}*/