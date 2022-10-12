#include "minirt.h"

void print_vec(t_vec v)
{
    printf("x : %lf, y : %lf, z : %lf\n", v.x, v.y, v.z);
}

void print_type(int type)
{
    if (type == CY)
        printf("type : %s\n", "Cylinder");
    else if (type == SP)
        printf("type : %s\n", "Shpere");
    else if (type == PL)
        printf("type : %s\n", "Plane");
}

void print_objects(t_objs *objs)
{
    t_objs *tmp;
    
    tmp = objs;
    while (tmp)
    {
        print_type(tmp->type);
        printf("center : "); print_vec(tmp->cen);
        printf("direction : "); print_vec(tmp->dir);
        printf("p : "); print_vec(tmp->p);
        printf("color : "); print_vec(tmp->col);
        printf("norm : "); print_vec(tmp->norm);
        tmp = tmp->next;
    }
}

void print_scene(t_scene sc)
{
    printf("-------- cam --------\n");
    printf("center >> "); print_vec(sc.cam.cen);
    printf("direction >> "); print_vec(sc.cam.dir);
    printf("fov >> "); printf("%lf\n", sc.cam.fov);

    printf("-------- light --------\n");
    printf("src pos >> "); print_vec(sc.light->src);
    printf("ratio >> "); printf("%lf\n", sc.light->ratio);

    printf("-------- amb --------\n");
    printf("ratio >> "); printf("%lf\n", sc.amb.ratio);
    printf("color >> "); print_vec(sc.amb.col);

    printf("-------- objects --------\n");
    print_objects(sc.objs);
}