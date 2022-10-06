#include "minirt.h"

void	parse_sphere(t_scene *sc, char **tokens)
{
	t_objs	*obj;

	obj = (t_objs *)malloc(sizeof(t_objs));
	obj->next = sc->objs;
	sc->objs = obj;
	if (!tokens || !tokens[1] || !tokens[2] || !tokens[3] || tokens[4])
		err_handler("invalid sphere");
	obj->type = SP;
	obj->cen = get_vec(tokens[1]);
	obj->p.x = ft_atod(tokens[2]);
	if (obj->p.x <= 0)
		err_handler("invalid diameter sphere");
	obj->col = get_color(tokens[3]);
}

void	parse_cylinder(t_scene *sc, char **tokens)
{
	t_objs	*obj;

	if (!tokens || !tokens[1] || !tokens[2] || !tokens[3] || !tokens[4]
		|| !tokens[5])
		err_handler("invalid cylinder");
	obj = (t_objs *)malloc(sizeof(t_objs));
	obj->next = sc->objs;
	sc->objs = obj;

	obj->type = CY;
	obj->cen = get_vec(tokens[1]);
	obj->dir = get_vec(tokens[2]);
	if (obj->dir.x > 1 || obj->dir.y > 1 || obj->dir.z > 1)
		err_handler("invalid orientation cylinder");
	if (obj->dir.x < -1 || obj->dir.y < -1 || obj->dir.z < -1)
		err_handler("invalid orientation cylinder");
	obj->p.x = ft_atod(tokens[3]);
	obj->p.y = ft_atod(tokens[4]);
	if (obj->p.x <= 0 || obj->p.y <= 0)
		err_handler("invalid diameter cy");
	obj->col = get_color(tokens[5]);
}

void	parse_plane(t_scene *sc, char **tokens)
{
	t_objs	*obj;

	if (!tokens || !tokens[1] || !tokens[2] || !tokens[3] || tokens[4])
		err_handler("invalid plane!");
	obj = (t_objs *)malloc(sizeof(t_objs));
	obj->next = sc->objs;
	sc->objs = obj;
	
	obj->type = PL;
	obj->cen = get_vec(tokens[1]);
	obj->dir = get_vec(tokens[2]);
	if (obj->dir.x > 1 || obj->dir.y > 1 || obj->dir.z > 1)
		err_handler("invalid orientation plane");
	if (obj->dir.x < -1 || obj->dir.y < -1 || obj->dir.z < -1)
		err_handler("invalid orientation plane");
	obj->col = get_color(tokens[3]);
}