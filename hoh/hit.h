#ifndef HIT_H
#define HIT_H
#include "vector.h"
#include "objects.h"

typedef struct s_ray t_ray;
typedef struct s_record t_record;

int front_face(t_ray *r, t_record* rec);
int find_hitpoint(t_ray* ray, t_object *objs, t_object *light, t_record* rec);
int hit_sphere(t_object* s, t_ray* r, t_record* rec);
int hit_cylinder(t_object *cy, t_ray *ray, t_record *rec);
int hit_caps(t_object *cy, t_ray *ray, t_record *rec);
int hit_plane(t_object *pl, t_ray *ray, t_record* rec);
int hit_rectangle_xy(t_object *rect, t_ray *ray, t_record* rec);
int hit_rectangle_yz(t_object *rect, t_ray *ray, t_record* rec);
int hit_rectangle_xz(t_object *rect, t_ray *ray, t_record* rec);

#endif