/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hako <hako@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/21 20:16:39 by hako              #+#    #+#             */
/*   Updated: 2022/10/21 20:17:16 by hako             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

t_vec create_vec(double x, double y, double z)
{
    t_vec vec;
    vec.x = x;
    vec.y = y;
    vec.z = z;
    return (vec);
}

void set_vec(t_vec *vec, double x, double y, double z)
{
    vec->x = x;
    vec->y = y;
    vec->z = z;
}

double vec_len(t_vec vec)
{
    return (sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z));
}

t_vec vec_sum(t_vec vec1, t_vec vec2)
{
    vec1.x += vec2.x;
    vec1.y += vec2.y;
    vec1.z += vec2.z;
    return (vec1);
}

t_vec      vec_sub(t_vec vec, t_vec vec2)
{
    vec.x -= vec2.x;
    vec.y -= vec2.y;
    vec.z -= vec2.z;
    return (vec);
}

t_vec vec_scalar_mul(t_vec vec, double s)
{
    vec.x *= s;
    vec.y *= s;
    vec.z *= s;
    return (vec);
}

t_vec   vec_mul(t_vec vec1, t_vec vec2)
{
    vec1.x *= vec2.x;
    vec1.y *= vec2.y;
    vec1.z *= vec2.z;
    return (vec1);
}

t_vec      vec_division(t_vec vec, double t)
{
    vec.x *= 1 / t;
    vec.y *= 1 / t;
    vec.z *= 1 / t;

    return vec;
}

double  vdot(t_vec vec, t_vec vec2)
{
    return (vec.x * vec2.x + vec.y * vec2.y + vec.z * vec2.z);
}

t_vec  vcross(t_vec vec1, t_vec vec2)
{
    t_vec vec;

    vec.x = vec1.y * vec2.z - vec1.z * vec2.y;
    vec.y = vec1.z * vec2.x - vec1.x * vec2.z;
    vec.z = vec1.x * vec2.y - vec1.y * vec2.x;
    return (vec);
}

t_vec      unit_vec(t_vec vec)
{
    double len = vec_len(vec);
    int *ptr;
    if (len == 0)
    {
        *ptr = 0;
        printf("Error : the length of vector is 0\n");
        exit(1);
    }
    vec.x /= len;
    vec.y /= len;
    vec.z /= len;
    return (vec);
}

t_vec  vmin(t_vec vec1, t_vec vec2)
{
    if (vec1.x > vec2.x)
        vec1.x = vec2.x;
    if (vec1.y > vec2.y)
        vec1.y = vec2.y;
    if (vec1.z > vec2.z)
        vec1.z = vec2.z;
    return (vec1);
}

int near_zero(t_vec vec)
{
    const double s = 1e-8;

    return ((fabs(vec.x) < s) && (fabs(vec.y) < s) && (fabs(vec.z) < s));
}