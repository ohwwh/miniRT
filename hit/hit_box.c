#include "minirt.h"


void	hit_box(t_objs *box, t_ray *ray, t_hit_record* rec)
{
	double	t0;
	double	t1;
	double	mn;
	double	mx;

	mn = (box->center.x - ray->origin.x) / ray->dir.x;
	mx = (box->dir.x - ray->origin.x) / ray->dir.x;
	t0 = fmin(mn, mx);
	t1 = fmax(mn, mx);
	rec->tmin = t0;
	rec->tmax = t1;
	if (rec->tmax <= rec->tmin)
		return ;
	
	mn = (box->center.y - ray->origin.y) / ray->dir.y;
	mx = (box->dir.y - ray->origin.y) / ray->dir.y;
	t0 = fmin(mn, mx);
	t1 = fmax(mn, mx);
	rec->tmin = fmax(t0, rec->tmin);
	rec->tmax = fmin(t1, rec->tmax);
	if (rec->tmax <= rec->tmin)
		return ;
	
	mn = (box->center.z - ray->origin.z) / ray->dir.z;
	mx = (box->dir.z - ray->origin.z) / ray->dir.z;
	t0 = fmin(mn, mx);
	t1 = fmax(mn, mx);
	rec->tmin = fmax(t0, rec->tmin);
	rec->tmax = fmin(t1, rec->tmax);
	if (rec->tmax <= rec->tmin)
		return ;

	if (rec->t != - 1 && rec->tmax > rec->t)
		return ;
	set_record(box, ray, rec, rec->tmax);
	return ;
}

//center -> box->center bound
//dir -> maximum bound