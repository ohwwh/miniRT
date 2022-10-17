#include "minirt.h"

void set_camera_param(t_camera *cam)
{
    double theta;
    double h;

	cam->ratio = (double) WIDTH / (double) HEIGHT;
	theta = cam->fov * PI / 180.0;
	cam->height = tan(theta / 2);
	cam->width = cam->ratio * cam->height;
    cam->forward = cam->lookat;
	cam->forward.x += EPS;
    cam->vup = create_vec(0,1,0);
	cam->right = unit_vec(vcross(vec_scalar_mul(cam->forward, -1), create_vec(0.0, -1.0, 0.0)));
	cam->up = unit_vec(vcross(vec_scalar_mul(cam->forward, -1), cam->right));


    t_vec w = unit_vec(vec_sub(cam->origin, cam->lookat)); //ㅋㅏ메라 뒤통수
	t_vec u = unit_vec(vcross(cam->vup, w));  //right
	t_vec v = vcross(w, u);

    cam->horizontal = vec_scalar_mul(u, cam->width);
	cam->vertical = vec_scalar_mul(v, cam->height);
	cam->lower_left_corner = create_vec(cam->origin.x + (-cam->horizontal.x / 2) + (-cam->vertical.x / 2) + (-w.x)
		,cam->origin.y + (- cam->horizontal.y / 2) + (-cam->vertical.y / 2) + (-w.y) 
		,cam->origin.z + (- cam->horizontal.z / 2) + (-cam->vertical.z / 2) + (-w.z));

}

t_hit_record find_hitpoint(t_ray *ray, t_objs *objs)
{
    t_objs *tmp;
    t_hit_record saved;
    
    tmp = objs;
    saved.t = -1.0;
    while (tmp)
    {
        if (tmp->type == SP)
        {
            saved = hit_sphere(saved, ray, tmp);
        }
        else if (tmp->type == PL)
        {
            saved = hit_plane(saved, ray, tmp);
        }
        else if (tmp->type == CY)
        {
            saved = hit_cylinder(saved, ray, tmp);
	        saved = hit_caps(saved, ray, tmp);
        }
        tmp = tmp->next;
    }
    return (saved);
}

int	is_inside(t_vec ray, t_vec norm)
{
	if (vdot(ray, norm) > 0)
		return (1);
	return (0);
}

t_vec get_raycolor(t_minirt *data)
{
    t_hit_record hr;
    t_vec amb;
    t_vec color;

    hr = find_hitpoint(&data->ray, data->scene.objs);
    if (hr.t > EPS)
	{
        amb = calcul_ratio(hr.color, data->scene.amb.col, data->scene.amb.ratio);
        if (is_inside(data->ray.dir, hr.normal)) // 카메라가 객체 안에 있는지 위치 파악
        {
            hr.normal = vec_scalar_mul(hr.normal, -1);
        }
        color = calcul_color(&data->scene, hr, amb, data->ray);
		return (color);
	}
	return (vec_scalar_mul(data->scene.amb.col, data->scene.amb.ratio));
}

t_ray       ray_primary(t_camera *cam, double u, double v)
{
    t_ray   ray;

    ray.origin = cam->origin;
    ray.dir = vec_sum(vec_sum(vec_scalar_mul(cam->up, v * cam->height),
				vec_scalar_mul(cam->right, u *  cam->width)), cam->forward);
    ray.dir = unit_vec(ray.dir);
    return (ray);
}