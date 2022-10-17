#ifndef CAMERA_H
#define CAMERA_H
#include "vector.h"

typedef struct s_camera {
	t_point origin;
	t_point lookat;
	t_vec vup;
	double vfov;
	double ratio;

	t_point lower_left_corner;
	t_vec horizontal;
	t_vec vertical;
	

} t_camera;

t_camera create_camera(t_point lookfrom, t_point lookat, t_vec vup, double vfov, double aspect_ratio);

#endif