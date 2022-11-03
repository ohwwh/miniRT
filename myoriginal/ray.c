#include "miniRT.h"

t_ray ray(t_point origin, t_vec dir)
{
	t_ray ret;
	ret.origin = origin;
	ret.dir = dir;
	return (ret);
}

t_point ray_end(t_ray* ray, double t)
{
	t_point ret;
	
	ret.x = ray->origin.x + t * ray->dir.x;
	ret.y = ray->origin.y + t * ray->dir.y;
	ret.z = ray->origin.z + t * ray->dir.z;
	return (ret);
}

double reflectance(double cos, double ref_ratio)
{
	double r0 = (1 - ref_ratio) / (1 + ref_ratio);
	r0 = r0 * r0;
	return (r0 + (1 - r0) * powf((1 - cos), 5));
}

t_vec refract(t_vec v, t_vec n, double e, double cos)
{
	t_vec perp = vec_scalar_mul(vec_sum(v, vec_scalar_mul(n, cos)), e);
	//e * (v + n * cos_theta)
	t_vec parallel = vec_scalar_mul(n, -sqrt(fabs(1.0 - powf(vec_len(perp), 2))));
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

	double phi = 2 * 3.1415926535897932385*r1;
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
        pdf = cos / 3.1415926535897932385;
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
	//const double length_squared = powf(vec_len(scattered->dir), 2);
	double cos_max;
	double angle;
	double temp;

	if (!light)
		return (0);
	rec_new.t = -1.0;
	hit_sphere(light, scattered, &rec_new);
	if (rec_new.t < 0.001)
		return 0;
	
	temp = (light->radius * light->radius / 
	powf(vec_len(vec_sub(light->center, scattered->origin)), 2));
	/*if (temp > 1)
		return (1);*/
	cos_max = sqrt(1 - temp);
	
	angle = 2 * 3.1415926535897932385 * (1 - cos_max);

	return (1 / angle);
}

double rectangle_light_pdf_value(t_hit_record *rec, t_ray* scattered, t_objs* light)
{
	t_hit_record rec_new;
	const double length_squared = powf(vec_len(scattered->dir), 2);
	double area;
	double distance_squared;
	double cosine;

	if (!light)
		return (0);
	rec_new.t = -1.0;
	//rec_new.tmin = 0.001;
	//rec_new.tmax = INFINITY;
	if (light->type == 4)
		hit_rectangle_xy(light, scattered, &rec_new);
	else if (light->type == 5)
		hit_rectangle_yz(light, scattered, &rec_new);
	else if (light->type == 6)
		hit_rectangle_xz(light, scattered, &rec_new);
	if (rec_new.t < 0.001)
		return 0;
	area = (light->center.y - light->center.x) * (light->dir.y - light->dir.x);
	distance_squared = rec_new.t * rec_new.t * length_squared;
	cosine = fabs(vdot(scattered->dir, rec_new.normal) / sqrt(length_squared));

	return distance_squared / (cosine * area);
}

double light_pdf_value(t_ray* ray_path, t_objs* light)
{
	// 일단 xy사각 광원만
	t_hit_record rec;
	const double length_squared = powf(vec_len(ray_path->dir), 2);

	if (!light)
		return (0);
	rec.t = -1.0;
	//rec.tmin = 0.001;
	//rec.tmax = INFINITY;

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

double mixture_pdf_value(t_hit_record* rec, t_ray* scattered, t_light* light)
{
	double t;
	double light_pdf_val;
	t_onb uvw;
	double weight;
	double w_sum;
	double size;
	double pdf;
	t_light *temp;
	int idx;

	pdf = 0.0;
	light_pdf_val = 0.0;
	w_sum = 0.0;
	temp = light;
	uvw = create_onb(rec->normal);
	if (!light || !get_light_size(light->object))
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
	//size = get_light_size(*(temp->object));
	size = 1;
	w_sum += size;
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
	pdf = (t * light_pdf_val / light->count + (1 - t) * cosine_pdf_value(&(rec->normal), &(uvw.w)));

	return (pdf);
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
        scat_pdf = cos / 3.1415926535897932385;
	return (scat_pdf);
}

double scatter(t_ray* r, t_hit_record* rec, t_ray* scattered, t_light* light)
{
	t_onb uvw;
	t_vec dir;
	//t_cosine_pdf pdf;
	double pdf;

	if (rec->mat == 0)
	{
		//코사인 분포를 따르는 랜덤 난반사 구현
		/*uvw = create_onb(rec->normal);
		dir = local(&uvw, random_cosine_direction()); //코사인 분포를 따르는 랜덤 벡터를 생성
		*scattered = ray(rec->p, unit_vec(dir)); //난반사
		//pdf = vdot(uvw.w, scattered->dir) / pi;
		pdf = cosine_pdf_value(&(rec->normal), &(uvw.w)); // 이부분 진심으로 이해가 안간다*/


		//광원을 샘플링. 
		//ray(rec->p, unit_vec(dir))에서 위에서 생성한 dir대신 
		//각 광원의 크기에 한정하여 랜덤 생성한 벡터를 집어넣는다.
		//일단 xz사각형 광원만
		/*t_point random_point;
		t_vec ray_path;

		random_point = create_vec(random_double(light->center.x, light->center.y, 7), light->radius,
		random_double(light->dir.x, light->dir.y, 7)); // 광원의 크기 안에서 벡터를 랜덤 생성
		ray_path = vec_sub(random_point, rec->p); // ray를 쏜 곳(시선)으로부터 광원 속 랜덤 지점의 벡터
		*scattered = ray(rec->p, ray_path);
		pdf = light_pdf_value(scattered, light);*/

		if (random_double(0,1,7) > rec->specular)
			pdf = mixture_pdf_value(rec, scattered, light);
		else
		{
			*scattered = ray(rec->p, reflect(unit_vec(r->dir), rec->normal));
			if (vdot(scattered->dir, rec->normal) <= 0) //이 조건식은 무슨 의미인가??
				rec->color = create_vec(0, 0, 0);
			return (1);
		}
		//pdf = mixture_pdf_value(rec, scattered, light);
		return (pdf);
	}
	else if (rec->mat == 1)
	{
		*scattered = ray(rec->p, reflect(unit_vec(r->dir), rec->normal));
		if (vdot(scattered->dir, rec->normal) <= 0) //이 조건식은 무슨 의미인가??
			rec->color = create_vec(0, 0, 0);
		return (1);
	}
	else if (rec->mat == 2)
	{
		if (rec->refraction == 0)
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
		return (1);
	}
	else if (rec->mat == -1)
	{
		return (1);
	}
}

t_color ray_color_2(t_ray r, t_objs* world, t_light* light)
{
	t_hit_record rec;
	double t;

	rec.t = -1.0;
	rec.tmin = 0.001;
	//rec.tmax = INFINITY;
	find_hitpoint_path(&r, world, light, &rec, MAX_DEPTH);
	if (rec.t != -1)
		return (rec.color);
	t = 0.5 * (unit_vec((r.dir)).y + 1.0);
	return (vec_scalar_mul(
		create_vec((1.0 - t) + (0.5 * t), (1.0 - t) + (0.7 * t), (1.0 - t) + (1.0 * t)), 1)
	);
}

t_color ray_color(t_ray r, t_objs* world, t_light* light, int depth)
{
	double t;
	double pdf;
	t_hit_record rec;
	t_color color;
	t_ray scattered;

	rec.t = -1.0;
	rec.tmin = 0.001;
	//rec.tmax = INFINITY;

	if (depth <= 0)
        return (create_vec(0,0,0));
	find_hitpoint_path(&r, world, light, &rec, depth);
	if (rec.t >= 0.0)
	{
		pdf = scatter(&r, &rec, &scattered, light);
		if (rec.mat != -1)
		{
			color = vec_mul(vec_scalar_mul(rec.color, scattering_pdf(&scattered, &rec)), 
			vec_division(ray_color(scattered, world, light, depth - 1), pdf));
		}
		else
			color = rec.color;
		return (color);
	}
	t = 0.5 * (unit_vec((r.dir)).y + 1.0);
	return (vec_scalar_mul(
		create_vec((1.0 - t) + (0.5 * t), (1.0 - t) + (0.7 * t), (1.0 - t) + (1.0 * t)), 1)
	);
}