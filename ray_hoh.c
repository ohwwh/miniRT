/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_hoh.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hako <hako@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/21 20:29:38 by hako              #+#    #+#             */
/*   Updated: 2022/10/21 20:29:39 by hako             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void set_light_attribute(t_scene *sc, double min)
{
	t_light *tmp;

	tmp = sc->light;
	while (tmp)
	{
		tmp->object.center = tmp->src;
		tmp->object.color = create_vec(45, 45, 45);
		tmp->object.type = SP;
		tmp->object.mat = -1;
		tmp->object.radius = min / 2;
		tmp->object.next = 0;
		tmp = tmp->next;
	}
}

int create_light_object(t_scene *sc)
{
	t_objs	*tmp;
	double min;

	min = INFINITY;
	tmp = sc->objs;
	while (tmp)
	{
		if (tmp->type != PL)
		{
			if (min > tmp->radius)
				min = tmp->radius;
		}
		tmp = tmp->next;
	}
	if (min == INFINITY)
		min = 10;
	set_light_attribute(sc, min);
	return (1);
}

t_ray ray(t_point origin, t_vec dir)
{
	t_ray ret;
	ret.origin = origin;
	ret.dir = dir;
	return (ret);
}

double reflectance(double cos, double ref_ratio)
{
	double r0;

	r0 = (1 - ref_ratio) / (1 + ref_ratio);
	r0 = r0 * r0;
	return (r0 + (1 - r0) * powf((1 - cos), 5));
}

t_vec refract(t_vec v, t_vec n, double e, double cos)
{
	t_vec perp;
	t_vec parallel;

	perp = vec_scalar_mul(vec_sum(v, vec_scalar_mul(n, cos)), e);
	//e * (v + n * cos_theta)
	parallel = vec_scalar_mul(n, -sqrt(fabs(1.0 - powf(vec_len(perp), 2))));
	return (vec_sum(perp, parallel));
}

t_vec reflect(t_vec v, t_vec n)
{
	return (vec_sub(v, vec_scalar_mul(n, 2*vdot(v, n))));
}

t_vec random_to_sphere(double radius, double distance_squared)
{
	double r1 = random_double(0,1,7);
	double r2 = random_double(0,1,7);
	double z = 1 + r2 * (sqrt(1 - radius*radius/distance_squared)-1);

	double phi = 2 * PI *r1;
	double x = cos(phi)*sqrt(1-z*z);
	double y = sin(phi)*sqrt(1-z*z);

	return (create_vec(x, y, z));
}

double cosine_pdf_value(const t_vec* dir, const t_vec* w)
{
	double pdf;
    double cos;

	cos = vdot(unit_vec(*dir), *w);
    if (cos < 0.0)
        pdf = 0;
    else
        pdf = cos / PI;
	return (pdf);
}

void generate_scattered(t_hit_record *rec, t_ray *scattered, t_onb *uvw)
{
	t_vec ray_path;

	ray_path = local(uvw, random_cosine_direction()); //ray를 쏜 곳으로부터, 코사인 분포를 따르는 랜덤 벡터를 생성
	*scattered = ray(rec->p, ray_path);
}

void generate_light_sample_rect(t_hit_record *rec, t_ray *scattered, t_objs *light)
{
	t_point random_point;
	t_vec ray_path;

	if (light->type == 4)
	{
		random_point = create_vec(random_double(light->center.x, light->center.y, 7),
		random_double(light->dir.x, light->dir.y, 7), light->radius); // 광원의 크기 안에서 포인트를 랜덤 생성
	}
	else if (light->type == 5)
	{
		random_point = create_vec(light->radius, random_double(light->center.x, light->center.y, 7),
		random_double(light->dir.x, light->dir.y, 7));
	}
	else if (light->type == 6)
	{
		random_point = create_vec(random_double(light->center.x, light->center.y, 7),
		light->radius, random_double(light->dir.x, light->dir.y, 7)); 
	}
	ray_path = vec_sub(random_point, rec->p); // ray를 쏜 곳(시선)으로부터 위에서 생성한 광원 속 랜덤 지점의 벡터
	*scattered = ray(rec->p, ray_path);
}

void generate_light_sample_sphere(t_hit_record *rec, t_ray *scattered, t_objs *light)
{
	t_onb uvw;
	t_point random_point;
	t_vec ray_path;
	double distance_squared;

	ray_path = vec_sub(light->center, rec->p); //반사지점에서 광원의 중심까지의 벡터
	distance_squared = powf(vec_len(ray_path), 2); //위의 벡터 거리의 제곱
	uvw = create_onb(ray_path); //위의 벡터 거리를 이용한 onb 생성
	ray_path = local(&uvw, random_to_sphere(light->radius, distance_squared));
	*scattered = ray(rec->p, ray_path);
	//반사 지점부터 광원의 랜덤지점까지의 벡터 생성
}

double sphere_light_pdf_value(t_hit_record* rec, t_ray* scattered, t_objs* light)
{
	t_hit_record rec_new;
	const double length_squared = powf(vec_len(scattered->dir), 2);
	double cos_max;
	double angle;

	if (!light)
		return (0);
	rec_new.t = -1.0;
	hit_sphere(light, scattered, &rec_new);
	if (rec_new.t < 0.001)
		return 0;
	cos_max = sqrt(1 - (light->radius * light->radius / 
	powf(vec_len(vec_sub(light->center, scattered->origin)), 2)));
	angle = 2 * PI * (1 - cos_max);

	return (1 / angle);
}

double rectangle_light_pdf_value(t_hit_record *rec, t_ray* scattered, t_objs* light)
{
	t_hit_record rec_new;
	const double length_squared = powf(vec_len(scattered->dir), 2);
	double area;
	double cosine;

	if (!light)
		return (0);
	rec_new.t = -1.0;
	if (light->type == 4)
		hit_rectangle_xy(light, scattered, &rec_new);
	else if (light->type == 5)
		hit_rectangle_yz(light, scattered, &rec_new);
	else if (light->type == 6)
		hit_rectangle_xz(light, scattered, &rec_new);
	if (rec_new.t < 0.001)
		return 0;
	area = (light->center.y - light->center.x) * (light->dir.y - light->dir.x);
	cosine = fabs(vdot(scattered->dir, rec_new.normal) / sqrt(length_squared));

	return ((rec_new.t * rec_new.t * length_squared) / (cosine * area));
}

double light_pdf_value(t_ray* ray_path, t_objs* light)
{
	t_hit_record rec;
	const double length_squared = powf(vec_len(ray_path->dir), 2);

	if (!light)
		return (0);
	rec.t = -1.0;
	if (light->type == 4)
		hit_rectangle_xy(light, ray_path, &rec);
	else if (light->type == 5)
		hit_rectangle_yz(light, ray_path, &rec);
	else if (light->type == 6)
		hit_rectangle_xz(light, ray_path, &rec);
	else if (light->type == 3)
		hit_sphere(light, ray_path, &rec);
	if (rec.t < 0.001)
		return 0;
	double area = (light->center.y - light->center.x) * (light->dir.y - light->dir.x);
	double distance_squared = rec.t * rec.t * length_squared;
	double cosine = fabs(vdot(ray_path->dir, rec.normal) / sqrt(length_squared));


	double cos_max = sqrt(1 - (light->radius * light->radius / 
	powf(vec_len(vec_sub(light->center, ray_path->origin)), 2)));
	double angle = 2 * 3.1415926535897932385 * (1 - cos_max);

	if (light->type == 3)
		return (1 / angle);
	else
		return distance_squared / (cosine * area);
}

double mixture_pdf_value_before(t_hit_record* rec, t_ray* scattered, t_objs* light)
{
	t_onb uvw;
	t_onb uvw_sphere;
	t_point random_point;
	double distance_squared;
	double t;
	t_vec ray_path; //원본 코드의 generate가 최종적으로 만드는 것

	if (!light)
		t = 0;
	else
		t = 0.5;
	uvw = create_onb(rec->normal);
	if (random_double(0,1,7) < t) //광원 샘플링, 부동소수점 오차 보정
	{
		if (light->type == 3)
		{
			ray_path = vec_sub(light->center, rec->p); //반사지점에서 광원의 중심까지의 벡터
			distance_squared = powf(vec_len(ray_path), 2); //위의 벡터 거리의 제곱
			uvw_sphere = create_onb(ray_path); //위의 벡터 거리를 이용한 onb 생성
			ray_path = local(&uvw_sphere, random_to_sphere(light->radius, distance_squared)); 
			//반사 지점부터 광원의 랜덤지점까지의 벡터 생성
		}
		else
		{
			if (light->type == 4)
			{
				random_point = create_vec(random_double(light->center.x, light->center.y, 7),
				random_double(light->dir.x, light->dir.y, 7), light->radius); // 광원의 크기 안에서 포인트를 랜덤 생성
			}
			else if (light->type == 5)
			{
				random_point = create_vec(light->radius, random_double(light->center.x, light->center.y, 7),
				random_double(light->dir.x, light->dir.y, 7));
			}
			else if (light->type == 6)
			{
				random_point = create_vec(random_double(light->center.x, light->center.y, 7),
				light->radius, random_double(light->dir.x, light->dir.y, 7)); 
			}
			ray_path = vec_sub(random_point, rec->p); // ray를 쏜 곳(시선)으로부터 위에서 생성한 광원 속 랜덤 지점의 벡터
		}
		
		*scattered = ray(rec->p, ray_path);

	}
	else //난반사 샘플링
	{
		ray_path = local(&uvw, random_cosine_direction()); //ray를 쏜 곳으로부터, 코사인 분포를 따르는 랜덤 벡터를 생성
		*scattered = ray(rec->p, ray_path);
	}
	
	return (t * light_pdf_value(scattered, light) + (1 - t) * cosine_pdf_value(&(rec->normal), &(uvw.w)));
}

/*void generate_ray(t_hit_record* rec, t_ray* scattered, t_light *temp)
{
	const double t = 0.5;

	
}*/

double mixture_pdf_value(t_hit_record* rec, t_ray* scattered, t_light* light)
{
	double t;
	double light_pdf_val;
	t_onb uvw;
	t_light *temp;
	int idx;

	light_pdf_val = 0.0;
	temp = light;
	uvw = create_onb(rec->normal);
	if (!light || !light->count || !get_light_size(light->object))
	{
		generate_scattered(rec, scattered, &uvw);
		return (cosine_pdf_value(&(rec->normal), &(uvw.w)));
	}
	else
		t = 0.5;

	idx = rand() % light->count;
	while (idx --)
		temp = temp->next;
	if (temp->object.mat != -1 || !get_light_size(temp->object))
	{
		generate_scattered(rec, scattered, &uvw);
		return (cosine_pdf_value(&(rec->normal), &(uvw.w)));
	}
	if (random_double(0,1,7) < t) //광원 샘플링
	{
		if (temp->object.type == 3)
			generate_light_sample_sphere(rec, scattered, &temp->object);
		else
			generate_light_sample_rect(rec, scattered, &temp->object);
	}
	else //난반사 샘플링
		generate_scattered(rec, scattered, &uvw);

	temp = light;
	while (temp)
	{
		if (temp->object.type == 3)
			light_pdf_val += sphere_light_pdf_value(rec, scattered, &temp->object);
		else
			light_pdf_val += rectangle_light_pdf_value(rec, scattered, &temp->object);
		temp = temp->next;
	}
	return (t * light_pdf_val / light->count + (1 - t) * cosine_pdf_value(&(rec->normal), &(uvw.w)));
}

double scattering_pdf(t_ray* scattered, t_hit_record* rec)
{
    double scat_pdf;
    double cos;

	if (rec->mat != 0)
		return (1);
    cos = vdot(rec->normal, unit_vec(scattered->dir));
    if (cos < 0)
        scat_pdf = 0;
    else
        scat_pdf = cos / PI;
	return (scat_pdf);
}

double scatter_refraction(t_ray* r, t_hit_record* rec, t_ray* scattered)
{
	t_vec dir;
	double ref_ratio;
	const double cos = fmin(vdot(vec_scalar_mul(unit_vec(r->dir), -1), rec->normal), 1.0);
	const double sin = sqrt(1.0 - cos * cos);

	if (rec->refraction == 0)
		printf("refraction is not set\n");
	if (rec->front_face)
		ref_ratio = 1.0 / rec->refraction;
	else
		ref_ratio = rec->refraction;
	if (ref_ratio * sin > 1.0 || 
	reflectance(cos, ref_ratio) > random_double(0,1,7))
		dir = reflect(unit_vec(r->dir), rec->normal);
	else
		dir = refract(unit_vec(r->dir), rec->normal, ref_ratio, cos);
	*scattered = ray(rec->p, dir);
	return (1);
}

double scatter_reflect(t_ray* r, t_hit_record* rec, t_ray* scattered)
{
	t_vec fuzziness;

	fuzziness = vec_scalar_mul(rand_sphere(), rec->fuzzy);
	*scattered = ray(rec->p, vec_sum(reflect(unit_vec(r->dir), rec->normal), fuzziness));
	if (vdot(scattered->dir, rec->normal) <= 0) //이 조건식은 무슨 의미인가??
		rec->color = create_vec(0, 0, 0);
	return (1);
}

double scatter_diffuse(t_ray* r, t_hit_record* rec, t_ray* scattered, t_light* light)
{
	double pdf;

	if (random_double(0,1,7) > rec->specular)
		pdf = mixture_pdf_value(rec, scattered, light);
	else
	{
		*scattered = ray(rec->p, reflect(unit_vec(r->dir), rec->normal));
		if (vdot(scattered->dir, rec->normal) <= 0) //이 조건식은 무슨 의미인가??
			rec->color = create_vec(0, 0, 0);
		return (1);
	}
	return (pdf);
}

double scatter(t_ray* r, t_hit_record* rec, t_ray* scattered, t_light* light)
{
	t_onb uvw;
	t_vec dir;
	double pdf;

	if (rec->mat == 0)
		return (scatter_diffuse(r, rec, scattered, light));
	else if (rec->mat == 1)
		return (scatter_reflect(r, rec, scattered));
	else if (rec->mat == 2)
	{
		/*if (rec->refraction == 0)
			printf("refraction is not set\n");
		t_vec attenuation = create_vec(1, 1, 1);
		t_vec dir;
		double ref_ratio;
		double cos = fmin(vdot(vec_scalar_mul(unit_vec(r->dir), -1), rec->normal), 1.0);
		double sin = sqrt(1.0 - cos * cos);
		
		if (rec->front_face)
			ref_ratio = 1.0 / rec->refraction;
		else
			ref_ratio = rec->refraction;

		if (ref_ratio * sin > 1.0 || 
		reflectance(cos, ref_ratio) > random_double(0,1,7))
			dir = reflect(unit_vec(r->dir), rec->normal);
		else
			dir = refract(unit_vec(r->dir), rec->normal, ref_ratio, cos);
		*scattered = ray(rec->p, dir);
		return (1);*/
		return (scatter_refraction(r, rec, scattered));
	}
	else if (rec->mat == -1)
		return (1);
}

t_color ray_color_raw(t_ray r, t_scene* sc)
{
	t_hit_record rec;
	double t;

	rec.t = -1.0;
	/*find_hitpoint_path(&r, world, light, &rec);
	if (rec.t != -1)
		return (rec.color);
	t = 0.5 * (unit_vec((r.dir)).y + 1.0);
	return (vec_scalar_mul(
		create_vec((1.0 - t) + (0.5 * t), (1.0 - t) + (0.7 * t), (1.0 - t) + (1.0 * t)), 1)
	);*/

	t_hit_record hr;

	hr.t = -1.0;
	hr = find_hitpoint(&r, sc->objs);
	if (hr.t > EPS)
		return (hr.color);
	t = 0.5 * (unit_vec((r.dir)).y + 1.0);
	return (vec_scalar_mul(
		create_vec((1.0 - t) + (0.5 * t), (1.0 - t) + (0.7 * t), (1.0 - t) + (1.0 * t)), 1)
	);
}

t_color ray_color(t_ray r, t_scene* sc, int depth)
{
	double t;
	double pdf;
	t_hit_record rec;
	t_color color;
	t_ray scattered;

	rec.t = -1.0;
	if (depth <= 0)
        return (create_vec(0,0,0));
	find_hitpoint_path(&r, sc->objs, sc->light, &rec);
	if (rec.t >= 0.0)
	{
		pdf = scatter(&r, &rec, &scattered, sc->light);
		if (rec.mat != -1)
		{
			color = vec_mul(vec_scalar_mul(rec.color, scattering_pdf(&scattered, &rec)), 
			vec_division(ray_color(scattered, sc, depth - 1), pdf));
		}
		else
			color = rec.color;
		return (color);
	}
	t = 0.5 * (unit_vec((r.dir)).y + 1.0);
	return (vec_scalar_mul(
		create_vec((1.0 - t) + (0.5 * t), (1.0 - t) + (0.7 * t), (1.0 - t) + (1.0 * t)), sc->amb.ratio * 0.1)
	);
}