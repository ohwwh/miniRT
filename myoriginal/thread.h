#ifndef THREAD_H
#define THREAD_H
#include <pthread.h>
#include "ray.h"
#include "vector.h"
#include "camera.h"


typedef struct s_data
{
    t_camera camera;
    void **world;
    int window_width;
	int window_height;
    int i;
    int j;
} t_data;



#endif
