/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pdf2.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohw <ohw@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/25 00:48:25 by ohw               #+#    #+#             */
/*   Updated: 2022/10/27 14:54:37 by ohw              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	generate_scattered(t_hit_record *rec, t_ray *scattered, t_onb *uvw)
{
	t_vec	ray_path;

	ray_path = local(uvw, random_cosine_direction());
	//ray를 쏜 곳으로부터, 코사인 분포를 따르는 랜덤 벡터를 생성
	*scattered = ray(rec->p, ray_path);
}

void	generate_light_sample_rect(
	t_hit_record *rec, t_ray *scattered, t_objs *light)
{
	t_point	random_point;
	t_vec	ray_path;

	if (light->type == 4)
	{
		random_point = create_vec(
				random_double(light->center.x, light->center.y, 7),
				random_double(light->dir.x, light->dir.y, 7),
				light->radius); // 광원의 크기 안에서 포인트를 랜덤 생성
	}
	else if (light->type == 5)
	{
		random_point = create_vec(light->radius,
				random_double(light->center.x, light->center.y, 7),
				random_double(light->dir.x, light->dir.y, 7));
	}
	else if (light->type == 6)
	{
		random_point = create_vec(
				random_double(light->center.x, light->center.y, 7),
				light->radius, random_double(light->dir.x, light->dir.y, 7));
	}
	ray_path = vec_sub(random_point, rec->p); // ray를 쏜 곳(시선)으로부터 위에서 생성한 광원 속 랜덤 지점의 벡터
	*scattered = ray(rec->p, ray_path);
}

void	generate_light_sample_sphere(
	t_hit_record *rec, t_ray *scattered, t_objs *light)
{
	t_onb	uvw;
	t_point	random_point;
	t_vec	ray_path;
	double	distance_squared;

	ray_path = vec_sub(light->center, rec->p);//반사지점에서 광원의 중심까지의 벡터
	distance_squared = powf(vec_len(ray_path), 2);//위의 벡터 거리의 제곱
	uvw = create_onb(ray_path);//위의 벡터 거리를 이용한 onb 생성
	ray_path = local(&uvw, random_to_sphere(light->radius, distance_squared));
	*scattered = ray(rec->p, ray_path);//반사 지점부터 광원의 랜덤지점까지의 벡터 생성
}

void	generate_random_importance(
	t_hit_record *rec, t_ray *scattered, t_light *temp, t_onb *uvw)
{
	const double	t = LT;

	if (random_double(0, 1, 7) < t) //광원 샘플링
	{
		if (temp->object.type == 3)
			generate_light_sample_sphere(rec, scattered, &temp->object);
		else
			generate_light_sample_rect(rec, scattered, &temp->object);
	}
	else //난반사 샘플링
		generate_scattered(rec, scattered, uvw);
}
