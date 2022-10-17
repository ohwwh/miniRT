#include "hit.h"
#include "ray.h"

int front_face(t_ray *r, t_record* rec)
{
	if (vdot(r->dir, rec->normal) > 0.0)
	{
		rec->normal = vec_scalar_mul(rec->normal, -1);
		return (0);
	}
	return (1);
}

void set_face_normal(t_record* rec, t_ray *ray, t_vec outward_normal)
{
	rec->front_face = vdot(ray->dir, outward_normal) < 0.0;
	if (rec->front_face != 0)
		rec->normal = outward_normal;
	else
		rec->normal = vec_scalar_mul(outward_normal, -1);
}

int find_hitpoint(t_ray* ray, t_object *objs, t_object *light, t_record* rec)
{
    t_object *tmp;
	int end;
    
    tmp = objs;
	/*end = 0;
    while (1)
    {
		if (!tmp)
		{
			if (!end)
			{
				tmp = light;
				end = 1;
			}
			else
				return (1) ;
		}
        if (tmp->type == 3)
            hit_sphere(tmp, ray, rec);
        else if (tmp->type == 1)
			hit_plane(tmp, ray, rec);
        else if (tmp->type == 2)
        {
            hit_cylinder(tmp, ray, rec);
			hit_caps(tmp, ray, rec);
        }
		else if (tmp->type == 4)
			hit_rectangle_xy(tmp, ray, rec);
		else if (tmp->type == 5)
			hit_rectangle_yz(tmp, ray, rec);
		else if (tmp->type == 6)
			hit_rectangle_xz(tmp, ray, rec);
        tmp = tmp->next;
    }*/
	while (tmp)
    {
        if (tmp->type == 3)
            hit_sphere(tmp, ray, rec);
        else if (tmp->type == 1)
			hit_plane(tmp, ray, rec);
        else if (tmp->type == 2)
        {
            hit_cylinder(tmp, ray, rec);
			hit_caps(tmp, ray, rec);
        }
		else if (tmp->type == 4)
			hit_rectangle_xy(tmp, ray, rec);
		else if (tmp->type == 5)
			hit_rectangle_yz(tmp, ray, rec);
		else if (tmp->type == 6)
			hit_rectangle_xz(tmp, ray, rec);
        tmp = tmp->next;
    }
    return (1);
}

int hit_caps(t_object *cy, t_ray *ray, t_record *rec)
{
	t_object top_cap;
	t_record hr;
	t_record hr2;

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
	top_cap.mat = cy->mat;
	top_cap.refraction = cy->refraction;

	hr = *rec;
	hr2 = *rec;
	hit_plane(&top_cap, ray, &hr);
	if (powf(hr.p.x - top_cap.center.x, 2) + powf(hr.p.y - top_cap.center.y, 2) + powf(hr.p.z - top_cap.center.z, 2) <= powf(cy->radius, 2))
	{
		hr2 = hr;
		*rec = hr;
	}
	hit_plane(cy, ray, &hr2);
	if (powf(hr2.p.x - cy->center.x, 2) + powf(hr2.p.y - cy->center.y, 2) + powf(hr2.p.z - cy->center.z, 2) <= powf(cy->radius, 2))
		*rec = hr2;
	return (0);
}

int hit_sphere(t_object* s, t_ray* r, t_record* rec)
{
	t_record hr;
	t_vec oc = create_vec(r->origin.x - s->center.x
	,r->origin.y - s->center.y
	,r->origin.z - s->center.z);
	double sqrtd;
	double root;
	double a = vdot((r->dir), (r->dir));
	double b = vdot(oc, (r->dir));
	double c = vdot(oc, oc) - s->radius * s->radius;
	double discriminant = b * b - a * c;
	if (discriminant < 0)
			return (0);
	sqrtd = sqrt(discriminant);
	root = (-b - sqrtd) / a;
	if (root < rec->t_min || rec->t_max < root)
	{
		root = (-b + sqrtd) / a;
		if (root < rec->t_min || rec->t_max < root)
			return (0);
	}
	rec->t = root;
	rec->p = ray_end(r, root);
	rec->t_max = root;
	set_face_normal(rec, r, vec_division(vec_sub(rec->p, s->center), s->radius));
	rec->color = s->color;
	rec->mat = s->mat;
	rec->refraction = s->refraction;
	return (1);
}

int hit_cylinder(t_object *cy, t_ray *ray, t_record *rec)
{   
    t_record hr;
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
	double root;

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
	if (D < rec->t_min)
		return (0);
	else
    {
        t1 = (-b + sqrt(D)) / (2 * a);
	    t2 = (-b - sqrt(D)) / (2 * a);
		if (t1 < rec->t_min)
			return (0);
		else
		{
	    	h1 = vdot(ray->dir, normalized) * t1 + vdot(oc, normalized);
	    	h2 = vdot(ray->dir, normalized) * t2 + vdot(oc, normalized);
			if (h2 >= rec->t_min && h2 <= cy->height)
				root = t2;
			else if (h1 >= rec->t_min && h1 <= cy->height)
				root = t1;
			else
				return (0);
		}
    }
	if (root < rec->t_min || rec->t_max < root)
		return (0);
	rec->t = root;
	rec->t_max = root;
	rec->mat = cy->mat;
	rec->refraction = cy->refraction;
	rec->color = cy->color;
	rec->p = vec_sum(ray->origin, vec_scalar_mul(ray->dir, root));
	oc = unit_vec(cy->dir);
	m = vdot(ray->dir, vec_scalar_mul(oc, root))
		+ vdot(vec_sub(ray->origin, cy->center), oc);
	set_face_normal(rec, ray, 
		unit_vec(vec_sub(vec_sub(rec->p, cy->center),
		vec_scalar_mul(oc, m)))
	);
    return (1);
}

int hit_plane(t_object *pl, t_ray *ray, t_record* rec)
{
	t_record hr;
    t_vec	x;
	t_vec	normal;
	double	b;
	double	a;
	double root;

	normal = unit_vec(pl->dir);
	x = vec_sub(ray->origin, pl->center);
	b = vdot(ray->dir, normal);
	if (b != 0)
	{
		a = vdot(x, normal);
		root = -a / b;
		if (root < rec->t_min)
            return (0);
	}
    else
		return (0);
    if (root < rec->t_min || rec->t_max < root)
		return (0);
	rec->t = root;
	rec->t_max = root;
	rec->mat = pl->mat;
	rec->refraction = pl->refraction;
	rec->color = pl->color;
	rec->p = vec_sum(ray->origin, vec_scalar_mul(ray->dir, root));
	rec->normal = pl->dir;
	if (vdot(ray->dir, rec->normal) > __DBL_EPSILON__) // 부동 소수점 오차 범위 내에서 비교
		rec->normal = unit_vec(vec_scalar_mul(pl->dir, -1));
	rec->front_face = front_face(ray, rec);
    return (1);
}

int hit_rectangle_xy(t_object *rect, t_ray *ray, t_record* rec)
{
	double t = (rect->radius - ray->origin.z) / ray->dir.z;
    if (t < rec->t_min || t > rec->t_max)
        return (0);
    double x = ray->origin.x + t * ray->dir.x;
    double y = ray->origin.y + t * ray->dir.y;
    if (x < rect->center.x || x > rect->center.y
	|| y < rect->dir.x || y > rect->dir.y)
        return (0);
    rec->u = (x - rect->center.x) / (rect->center.y - rect->center.x);
    rec->v = (y - rect->dir.x) / (rect->dir.y - rect->dir.x);
    rec->t = t;
	rec->t_max = t;
	rec->mat = rect->mat;
	rec->refraction = rect->refraction;
    t_vec outward_normal = create_vec(0, 0, 1);
    set_face_normal(rec, ray, outward_normal);
    rec->p = ray_end(ray, t);
	rec->color = rect->color;
    return (1);
}

int hit_rectangle_yz(t_object *rect, t_ray *ray, t_record* rec)
{
	double t = (rect->radius - ray->origin.x) / ray->dir.x;
    if (t < rec->t_min || t > rec->t_max)
        return (0);
    double y = ray->origin.y + t * ray->dir.y;
    double z = ray->origin.z + t * ray->dir.z;
    if (y < rect->center.x || y > rect->center.y
	|| z < rect->dir.x || z > rect->dir.y)
        return (0);
    rec->u = (y - rect->center.x) / (rect->center.y - rect->center.x);
    rec->v = (z - rect->dir.x) / (rect->dir.y - rect->dir.x);
    rec->t = t;
	rec->t_max = t;
	rec->mat = rect->mat;
	rec->refraction = rect->refraction;
    t_vec outward_normal = create_vec(1, 0, 0);
    set_face_normal(rec, ray, outward_normal);
    rec->p = ray_end(ray, t);
	rec->color = rect->color;
    return (1);
}

int hit_rectangle_xz(t_object *rect, t_ray *ray, t_record* rec)
{
	double t = (rect->radius - ray->origin.y) / ray->dir.y;
    if (t < rec->t_min || t > rec->t_max)
        return (0);
    double x = ray->origin.x + t * ray->dir.x;
    double z = ray->origin.z + t * ray->dir.z;
    if (x < rect->center.x || x > rect->center.y
	|| z < rect->dir.x || z > rect->dir.y)
        return (0);
    rec->u = (x - rect->center.x) / (rect->center.y - rect->center.x);
    rec->v = (z - rect->dir.x) / (rect->dir.y - rect->dir.x);
    rec->t = t;
	rec->t_max = t;
	rec->mat = rect->mat;
	rec->refraction = rect->refraction;
    t_vec outward_normal = create_vec(0, 1, 0);
    set_face_normal(rec, ray, outward_normal);
    rec->p = ray_end(ray, t);
	rec->color = rect->color;
    return (1);
}