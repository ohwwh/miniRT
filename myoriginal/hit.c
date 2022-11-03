#include "miniRT.h"
#define EPS 0.01

void set_face_normal(t_hit_record* rec, t_ray *ray, t_vec outward_normal)
{
	rec->front_face = vdot(ray->dir, outward_normal) < 0.0;
	if (rec->front_face != 0)
		rec->normal = unit_vec(outward_normal);
	else
		rec->normal = vec_scalar_mul(unit_vec(outward_normal), -1);
}

int find_hitpoint_light(t_ray* ray, t_light *light, t_hit_record* rec, int depth)
{
	t_light *temp;

	/*if (depth == MAX_DEPTH)
		return (1);*/
	temp = light;
	while (temp)
    {
        if (temp->object.type == 3)
            hit_sphere(&temp->object, ray, rec);
        else if (temp->object.type == 1)
			hit_plane(&temp->object, ray, rec);
        else if (temp->object.type == 2)
        {
            hit_cylinder(&temp->object, ray, rec);
			hit_caps(&temp->object, ray, rec);
        }
		else if (temp->object.type == 4)
			hit_rectangle_xy(&temp->object, ray, rec);
		else if (temp->object.type == 5)
			hit_rectangle_yz(&temp->object, ray, rec);
		else if (temp->object.type == 6)
			hit_rectangle_xz(&temp->object, ray, rec);
        temp = temp->next;
    }
	return (1);
}

int find_hitpoint_path(t_ray* ray, t_objs *objs, t_light *light, t_hit_record* rec, int depth)
{
    t_objs *tmp;
	int end;
    
    tmp = objs;
	while (tmp)
    {
		if (tmp->mat == -1)
			continue ;
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
	if (light && light->count != 0)
		find_hitpoint_light(ray, light, rec, depth);
    return (1);
}

void hit_caps(t_objs *cy, t_ray *ray, t_hit_record *rec)
{
	t_objs top_cap;
	t_hit_record hr;
	t_hit_record hr2;

	set_vec(&top_cap.center, cy->dir.x, cy->dir.y, cy->dir.z);
	top_cap.center = unit_vec(top_cap.center);
	top_cap.center = vec_scalar_mul(top_cap.center, cy->height);
	top_cap.center = vec_sum(top_cap.center, cy->center);
	set_vec(&top_cap.dir, cy->dir.x, cy->dir.y, cy->dir.z);
	set_vec(&top_cap.color, cy->color.x, cy->color.y, cy->color.z);
	top_cap.mat = cy->mat;
	top_cap.refraction = cy->refraction;
	top_cap.specular = cy->specular;
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
}

void hit_sphere(t_objs* s, t_ray* r, t_hit_record* rec)
{
	t_hit_record hr;
	t_vec oc;
	t_discriminant d;
	double root;

	oc = vec_sub(r->origin, s->center);
	d.a = vdot((r->dir), (r->dir));
	d.b = vdot(oc, (r->dir));
	d.c = vdot(oc, oc) - s->radius * s->radius;
	d.Dsc = d.b * d.b - d.a * d.c;
	if (d.Dsc < 0)
			return ;
	root = (-d.b - sqrt(d.Dsc)) / d.a;
	if (root < EPS || (rec->t != -1 && rec->t < root))
	{
		root = (-d.b + sqrt(d.Dsc)) / d.a;
		if (root < EPS || (rec->t != -1 && rec->t < root))
			return ;
	}
	rec->t = root;
	rec->p = ray_end(r, root);
	rec->tmax = root;
	set_face_normal(rec, r, vec_division(vec_sub(rec->p, s->center), s->radius));
	rec->color = s->color;
	rec->mat = s->mat;
	rec->refraction = s->refraction;
	rec->specular = s->specular;
	rec->type = s->type;
}

void hit_cylinder(t_objs *cy, t_ray *ray, t_hit_record *rec)
{   
	double	m;
	t_vec	oc;
	t_discriminant d;
    t_vec   normalized;
    double  h1, h2;
	double root;

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
		return ;
	else
    {
        d.t1 = (-d.b + sqrt(d.Dsc)) / (2 * d.a);
	    d.t2 = (-d.b - sqrt(d.Dsc)) / (2 * d.a);
		if (d.t1 < EPS)
			return ;
		else
		{
	    	h1 = vdot(ray->dir, normalized) * d.t1 + vdot(oc, normalized);
	    	h2 = vdot(ray->dir, normalized) * d.t2 + vdot(oc, normalized);
			if (h2 >= EPS && h2 <= cy->height)
				root = d.t2;
			else if (h1 >= EPS && h1 <= cy->height)
				root = d.t1;
			else
				return ;
		}
    }
	if (root < EPS || (rec->t != -1 && rec->t < root))
		return ;
	rec->t = root;
	rec->tmax = root;
	rec->mat = cy->mat;
	rec->refraction = cy->refraction;
	rec->specular = cy->specular;
	rec->color = cy->color;
	rec->p = vec_sum(ray->origin, vec_scalar_mul(ray->dir, root));
	oc = unit_vec(cy->dir);
	m = vdot(ray->dir, vec_scalar_mul(oc, root))
		+ vdot(vec_sub(ray->origin, cy->center), oc);
	set_face_normal(rec, ray, 
		unit_vec(vec_sub(vec_sub(rec->p, cy->center),
		vec_scalar_mul(oc, m)))
	);
}

void hit_plane(t_objs *pl, t_ray *ray, t_hit_record* rec)
{
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
		if (root < EPS)
            return ;
	}
    else
		return ;
    if (root < EPS || (rec->t != -1 && rec->t < root))
		return ;
	rec->t = root;
	rec->mat = pl->mat;
	rec->refraction = pl->refraction;
	rec->specular = pl->specular;
	rec->color = pl->color;
	rec->p = vec_sum(ray->origin, vec_scalar_mul(ray->dir, root));
	//rec->normal = pl->dir;
	/*if (vdot(ray->dir, rec->normal) > __DBL_EPSILON__) // 부동 소수점 오차 범위 내에서 비교
		rec->normal = unit_vec(vec_scalar_mul(pl->dir, -1));*/
	//rec->front_face = front_face(ray, rec);
	set_face_normal(rec, ray, pl->dir);
}

void hit_rectangle_xy(t_objs *rect, t_ray *ray, t_hit_record* rec)
{
	double t = (rect->radius - ray->origin.z) / ray->dir.z;
    if (t < EPS || (rec->t != -1 && rec->t < t))
        return ;
    double x = ray->origin.x + t * ray->dir.x;
    double y = ray->origin.y + t * ray->dir.y;
    if (x < rect->center.x || x > rect->center.y
	|| y < rect->dir.x || y > rect->dir.y)
        return ;
   // rec->u = (x - rect->center.x) / (rect->center.y - rect->center.x);
    //rec->v = (y - rect->dir.x) / (rect->dir.y - rect->dir.x);
    rec->t = t;
	rec->tmax = t;
	rec->mat = rect->mat;
	rec->refraction = rect->refraction;
	rec->specular = rect->specular;
    set_face_normal(rec, ray, create_vec(0, 0, 1));
    rec->p = ray_end(ray, t);
	rec->color = rect->color;
}

void hit_rectangle_yz(t_objs *rect, t_ray *ray, t_hit_record* rec)
{
	double t = (rect->radius - ray->origin.x) / ray->dir.x;
    if (t < EPS || (rec->t != -1 && rec->t < t))
        return ;
    double y = ray->origin.y + t * ray->dir.y;
    double z = ray->origin.z + t * ray->dir.z;
    if (y < rect->center.x || y > rect->center.y
	|| z < rect->dir.x || z > rect->dir.y)
        return ;
    //rec->u = (y - rect->center.x) / (rect->center.y - rect->center.x);
    //rec->v = (z - rect->dir.x) / (rect->dir.y - rect->dir.x);
    rec->t = t;
	rec->tmax = t;
	rec->mat = rect->mat;
	rec->refraction = rect->refraction;
	rec->specular = rect->specular;
    set_face_normal(rec, ray, create_vec(1, 0, 0));
    rec->p = ray_end(ray, t);
	rec->color = rect->color;
}

void hit_rectangle_xz(t_objs *rect, t_ray *ray, t_hit_record* rec)
{
	double t = (rect->radius - ray->origin.y) / ray->dir.y;
    if (t < EPS || (rec->t != -1 && rec->t < t))
        return ;
    double x = ray->origin.x + t * ray->dir.x;
    double z = ray->origin.z + t * ray->dir.z;
    if (x < rect->center.x || x > rect->center.y
	|| z < rect->dir.x || z > rect->dir.y)
        return ;
    //rec->u = (x - rect->center.x) / (rect->center.y - rect->center.x);
    //rec->v = (z - rect->dir.x) / (rect->dir.y - rect->dir.x);
    rec->t = t;
	rec->tmax = t;
	rec->mat = rect->mat;
	rec->refraction = rect->refraction;
	rec->specular = rect->specular;
    set_face_normal(rec, ray, create_vec(0, 1, 0));
    rec->p = ray_end(ray, t);
	rec->color = rect->color;
}