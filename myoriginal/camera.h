#ifndef CAMERA_H
#define CAMERA_H
#include "vector.h"

typedef struct s_camera {
	t_point origin;
	t_vec dir;
	t_vec vup;
	double fov;
	double ratio;
	int count;
	double distance;

	t_vec right;
	t_vec forward;
	t_vec up;
	double viewport_width;
	double viewport_height;

	/*t_point lower_left_corner;
	t_vec horizontal;
	t_vec vertical;*/
	

} t_camera;

t_camera create_camera(t_point lookfrom, t_point , t_vec vup, double vfov, double aspect_ratio);
void set_camera(t_camera *cam);

#endif