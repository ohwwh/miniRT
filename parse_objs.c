/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_objs.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hako <hako@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/19 18:53:04 by hako              #+#    #+#             */
/*   Updated: 2022/10/19 18:53:13 by hako             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void parse_sphere(t_scene *sc, char **tokens)
{
	t_objs *obj;

	obj = (t_objs *)malloc(sizeof(t_objs));
	obj->next = sc->objs;
	sc->objs = obj;
	if (!tokens || !tokens[1] || !tokens[2] || !tokens[3] || tokens[4])
		err_handler("invalid sphere");
	obj->type = SP;
	obj->center = get_vec(tokens[1]);
	obj->radius = ft_atod(tokens[2]) / 2.0;
	if (obj->radius <= 0)
		err_handler("invalid radius sphere");
	obj->color = get_color(tokens[3]);
	obj->mat = 0;
	obj->refraction = 1.5;
	obj->specular = 0.6;
	obj->fuzzy = 0;
}

void parse_cylinder(t_scene *sc, char **tokens)
{
	t_objs *obj;

	if (!tokens || !tokens[1] || !tokens[2] || !tokens[3] || !tokens[4] || !tokens[5])
		err_handler("invalid cylinder");
	obj = (t_objs *)malloc(sizeof(t_objs));
	obj->next = sc->objs;
	sc->objs = obj;
	obj->type = CY;
	obj->center = get_vec(tokens[1]);
	obj->dir = get_vec(tokens[2]);
	if (obj->dir.x > 1 || obj->dir.y > 1 || obj->dir.z > 1)
		err_handler("invalid orientation cylinder");
	if (obj->dir.x < -1 || obj->dir.y < -1 || obj->dir.z < -1)
		err_handler("invalid orientation cylinder");
	obj->radius = ft_atod(tokens[3]) / 2.0;
	obj->height = ft_atod(tokens[4]);
	if (obj->radius <= 0 || obj->height <= 0)
		err_handler("invalid radius cy");
	obj->color = get_color(tokens[5]);
	obj->mat = 0;
	obj->refraction = 1.5;
	obj->specular = 0.6;
	obj->fuzzy = 0;
}

void parse_plane(t_scene *sc, char **tokens)
{
	t_objs *obj;

	if (!tokens || !tokens[1] || !tokens[2] || !tokens[3] || tokens[4])
		err_handler("invalid plane!");
	obj = (t_objs *)malloc(sizeof(t_objs));
	obj->next = sc->objs;
	sc->objs = obj;
	obj->type = PL;
	obj->center = get_vec(tokens[1]);
	obj->dir = get_vec(tokens[2]);
	if (obj->dir.x > 1 || obj->dir.y > 1 || obj->dir.z > 1)
		err_handler("invalid orientation plane");
	if (obj->dir.x < -1 || obj->dir.y < -1 || obj->dir.z < -1)
		err_handler("invalid orientation plane");
	obj->color = get_color(tokens[3]);
	obj->mat = 0;
	obj->refraction = 1.5;
	obj->specular = 0;
	obj->fuzzy = 0.5;
}
