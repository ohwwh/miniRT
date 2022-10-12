#include "minirt.h"

void set_camera_param(t_cam *cam)
{
	cam->aspect_r = (double) WIDTH / (double) HEIGHT;
	cam->theta = cam->fov * PI / 180.0;
	cam->height = tan(cam->theta / 2);
	cam->width = cam->aspect_r * cam->height;
	cam->forward = cam->dir;
	cam->forward.x += EPS;
	cam->right = unit_vec(vcross(vec_scalar_mul(cam->forward, -1), create_vec(0.0, -1.0, 0.0)));
	cam->up = unit_vec(vcross(vec_scalar_mul(cam->forward, -1), cam->right));
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
            // saved = hit_cylinder(saved, ray, tmp);
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
        color = calcul_color(&data->scene, hr, amb);
		return (color);
	}
	return (vec_scalar_mul(data->scene.amb.col, data->scene.amb.ratio));
}

t_ray       ray_primary(t_cam *cam, double u, double v)
{
    t_ray   ray;

    ray.origin = cam->cen;
    ray.dir = vec_sum(vec_sum(vec_scalar_mul(cam->up, v * cam->height),
				vec_scalar_mul(cam->right, u * cam->width)), cam->forward);
    ray.dir = unit_vec(ray.dir);
    return (ray);
}