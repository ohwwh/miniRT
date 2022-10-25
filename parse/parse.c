/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hako <hako@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/19 18:49:45 by hako              #+#    #+#             */
/*   Updated: 2022/10/24 17:38:09 by hako             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

t_vec	get_vec(char *s)
{
	char	**params;
	t_vec	cord;

	params = ft_split(s, ',');
	if (!params || !params[1] || !params[2] || params[3])
		err_handler("invalid coordinates");
	cord = make_vec(ft_atod(params[0]), ft_atod(params[1]), ft_atod(params[2]));
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
	else if (id[0] == 's' && id[1] == 'p' && id[2] == '\0')
		parse_sphere(sc, tokens);
	else if (id[0] == 'p' && id[1] == 'l' && id[2] == '\0')
		parse_plane(sc, tokens);
	else if (id[0] == 'c' && id[1] == 'y' && id[2] == '\0')
		parse_cylinder(sc, tokens);
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
