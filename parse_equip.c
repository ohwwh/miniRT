#include "minirt.h"

t_light	*alloc_light(t_scene *sc)
{
	t_light	*new_light;

	new_light = malloc(sizeof(t_light));
	if (!new_light)
		err_handler("allocation failed\n");
	new_light->next = sc->light;
	sc->light = new_light;
	return (new_light);
}

void	parse_ambient(t_scene *sc, char **tokens)
{
	if (!tokens || !tokens[1] || !tokens[2] || tokens[3])
		err_handler("invalid ambiant!");
	if (sc->amb.count != 0)
		err_handler("too many ambiant");
	sc->amb.count++;
	sc->amb.ratio = ft_atod(tokens[1]);
	if (sc->amb.ratio < 0 || sc->amb.ratio > 1)
		err_handler("enter ambient lighting ratio in range [0.0,1.0]");
	sc->amb.col = get_color(tokens[2]);
}

void	parse_camera(t_scene *sc, char **tokens)
{
	if (!tokens || !tokens[1] || !tokens[2] || !tokens[3] || tokens[4])
		err_handler("invalid camera !");
	if (sc->cam.count != 0)
		err_handler("too many camera");
	sc->cam.count++;
	sc->cam.cen = get_vec(tokens[1]);
	sc->cam.dir = get_vec(tokens[2]);
	if (sc->cam.dir.x > 1 || sc->cam.dir.y > 1 || sc->cam.dir.z > 1)
		err_handler("invalid orientation camera");
	if (sc->cam.dir.x < -1 || sc->cam.dir.y < -1 || sc->cam.dir.z < -1)
		err_handler("invalid orientation camera");
	// if (sc->cam.dir.x == 0 && sc->cam.dir.y == 0 && sc->cam.dir.z == 0)
	// 	err_handler("invalid orientation camera");
	sc->cam.fov = ft_atod(tokens[3]);
	if (sc->cam.fov < 0 || sc->cam.fov > 180)
		err_handler("FOV  in range [0,180]");
}

void	parse_light(t_scene *sc, char **tokens)
{
	t_light	*new;

	if (!tokens || !tokens[1] || !tokens[2] || !tokens[3] || tokens[4])
		err_handler("invalid light !");
	new = alloc_light(sc);
	new->src = get_vec(tokens[1]);
	new->ratio = ft_atod(tokens[2]);
	if (new->ratio < 0 || new->ratio > 1)
		err_handler("enter the light brightness ratio in range [0.0,1.0]");
}