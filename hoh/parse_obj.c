# include "ft_mlx.h"
# include "camera.h"
# include "light.h"
# include "miniRT.h"
# include "objects.h"
# include "vector.h"
# include "ray.h"

void	parse_sphere(t_object *sc, char **tokens)
{
	t_object	*obj;

	obj = (t_object *)malloc(sizeof(t_object));
	obj->next = sc;
	sc = obj;
	if (!tokens || !tokens[1] || !tokens[2] || !tokens[3] || tokens[4])
		err_handler("invalid sphere");
	obj->type = SP;
	obj->center = get_vec(tokens[1]);
	obj->radius = ft_atod(tokens[2]) / 2.0;
	if (obj->radius <= 0)
		err_handler("invalid radius sphere");
	obj->color = get_color(tokens[3]);
}

void	parse_cylinder(t_vars *sc, char **tokens)
{
	t_object	*obj;

	if (!tokens || !tokens[1] || !tokens[2] || !tokens[3] || !tokens[4]
		|| !tokens[5])
		err_handler("invalid cylinder");
	obj = (t_object *)malloc(sizeof(t_object));
	obj->next = sc;
	sc = obj;

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
}

void	parse_plane(t_vars *sc, char **tokens)
{
	t_object	*obj;

	if (!tokens || !tokens[1] || !tokens[2] || !tokens[3] || tokens[4])
		err_handler("invalid plane!");
	obj = (t_object *)malloc(sizeof(t_object));
	obj->next = sc;
	sc = obj;
	
	obj->type = PL;
	obj->center = get_vec(tokens[1]);
	obj->dir = get_vec(tokens[2]);
	if (obj->dir.x > 1 || obj->dir.y > 1 || obj->dir.z > 1)
		err_handler("invalid orientation plane");
	if (obj->dir.x < -1 || obj->dir.y < -1 || obj->dir.z < -1)
		err_handler("invalid orientation plane");
	obj->color = get_color(tokens[3]);
}