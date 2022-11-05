/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohw <ohw@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/19 18:49:45 by hako              #+#    #+#             */
/*   Updated: 2022/11/05 15:55:56 by ohw              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

t_vec	get_vec(char *s)
{
	char	**params;
	t_vec	cord;

	params = ft_split(s, ',');
	if (!params || !params[1])
		err_handler("invalid coordinates");
	if (params[2])
		cord = make_vec(ft_atod(params[0]), ft_atod(params[1]), ft_atod(params[2]));
	else
		cord = make_vec(ft_atod(params[0]), ft_atod(params[1]), 0);
	free_split(params);
	return (cord);
}

void	parse_line(char *id, char **tokens, t_scene *sc)
{
	if (id[0] == 'A' && id[1] == '\0')
		parse_ambient(sc, tokens);
	else if (id[0] == 'C' && id[1] == '\0')
		parse_camera(sc, tokens);
	else if (id[0] == 'L' && id[1] == '\0')
		parse_light(sc, tokens);
	else if (id[0] == 'L' && id[1] == 's')
		parse_light_sphere(sc, tokens);
	else if (id[0] == 'L' && id[1] == 'x' && id[2] == 'y')
		parse_light_rectangle_xy(sc, tokens);
	else if (id[0] == 'L' && id[1] == 'y' && id[2] == 'z')
		parse_light_rectangle_yz(sc, tokens);
	else if (id[0] == 'L' && id[1] == 'x' && id[2] == 'z')
		parse_light_rectangle_xz(sc, tokens);
	else if (id[0] == 's' && id[1] == 'p' && id[2] == '\0')
		parse_sphere(sc, tokens);
	else if (id[0] == 'p' && id[1] == 'l' && id[2] == '\0')
		parse_plane(sc, tokens);
	else if (id[0] == 'c' && id[1] == 'y' && id[2] == '\0')
		parse_cylinder(sc, tokens);
	else if (id[0] == 'x' && id[1] == 'y' && id[2] == '\0')
		parse_rectangle_xy(sc, tokens);
	else if (id[0] == 'y' && id[1] == 'z' && id[2] == '\0')
		parse_rectangle_yz(sc, tokens);
	else if (id[0] == 'x' && id[1] == 'z' && id[2] == '\0')
		parse_rectangle_xz(sc, tokens);
	else if (id[0] == 'b' && id[1] == 'x' && id[2] == '\0')
		parse_box(sc, tokens);
	else
		err_handler("invalid object type");
}

void	parse(t_scene *sc, int fd)
{
	char	**tokens;
	char	*str;
	char	*trimed_str;

	while (1)
	{
		str = get_next_line(fd);
		if (!str)
			break ;
		trimed_str = ft_strtrim(str, "\n");
		free(str);
		tokens = ft_split(trimed_str, ' ');
		free(trimed_str);
		if (tokens == NULL)
			break ;
		if (*tokens)
		{
			parse_line(*tokens, tokens, sc);
		}
		free_split(tokens);
	}
	close(fd);
}
