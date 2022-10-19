/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hako <hako@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/19 18:49:45 by hako              #+#    #+#             */
/*   Updated: 2022/10/19 18:52:08 by hako             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

int	check_file(int ac, char **av)
{
	int	i;
	int	fd;

	fd = 0;
	if (ac != 2)
		return (1);
	if (!av[1])
		return (1);
	i = ft_strlen(av[1]);
	if (i < 4)
		return (1);
	if (av[1][i - 1] == 't' || av[1][i - 2] == 'r' || av[1][i - 3] == '.')
	{
		fd = open(av[1], O_RDONLY);
		if (fd < 0)
			return (1);
		close(fd);
	}
	else
		return (1);
	return (0);
}

void	err_handler(char *msg)
{
	printf("Error\n%s", msg);
	exit(1);
}

t_bool	is_valid_color(char *s)
{
	int		i;

	i = 0;
	while (s[i])
	{
		if (!((s[i] >= '0' && s[i] <= '9') || s[i] == '+' || s[i] == '-'))
			return (FALSE);
		i++;
	}
	return (TRUE);
}

t_vec	get_color(char *s)
{
	char	**params;
	t_vec	cord;

	params = ft_split(s, ',');
	if (!params || !params[1] || !params[2] || params[3])
		err_handler("invalid color!");
	if (!(is_valid_color(params[0]) && is_valid_color(params[1])
			&& is_valid_color(params[2])))
		err_handler("invalid color!");
	cord = (t_vec){ft_atoi(params[0]), ft_atoi(params[1]), ft_atoi(params[2])};
	if (cord.x > 255 || cord.y > 255 || cord.z > 255)
		err_handler("invalid color");
	if (cord.x < 0 || cord.y < 0 || cord.z < 0)
		err_handler("invalid color");
	free_split(params);
	cord = vec_division(cord, 255);
	return (cord);
}

t_vec	make_vec(double x, double y, double z)
{
	t_vec	vec;

	vec.x = x;
	vec.y = y;
	vec.z = z;
	return (vec);
}

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
