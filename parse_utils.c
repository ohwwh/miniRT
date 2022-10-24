/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hako <hako@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/24 17:36:13 by hako              #+#    #+#             */
/*   Updated: 2022/10/24 17:38:01 by hako             ###   ########.fr       */
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
