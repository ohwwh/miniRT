/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hako <hako@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/19 18:53:39 by hako              #+#    #+#             */
/*   Updated: 2022/10/19 18:56:39 by hako             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	free_split(char **s)
{
	int	i;

	i = 0;
	while (s[i])
	{
		free(s[i]);
		i++;
	}
	free(s);
}

double	int_part(const char *str, int *i)
{
	double	nb;

	nb = 0;
	while (str[*i] >= '0' && str[*i] <= '9')
		nb = nb * 10 + (str[(*i)++] - '0');
	return (nb);
}

double	decimal_part(const char *str)
{
	double	nb;
	int		aux;
	int		i;

	i = 0;
	nb = 0;
	if (str[i] == '\0')
		return (nb);
	aux = 10;
	if (str[i] == '.')
	{
		while (str[++i] >= '0' && str[i] <= '9')
		{
			nb = nb + (double)(str[i] - '0') / aux;
			aux = aux * 10;
		}
	}
	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
    	i++;
  	if (str[i] != '\0')
    	err_handler("wrong argument");
  	return (nb);
}

double	ft_atod(const char *str)
{
	double	nb;
	int		signe;
	int		i;

	i = 0;
	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
		i++;
	signe = 1;
	if (str[i] == '-' || str[i] == '+')
		if (str[i++] == '-')
			signe = -1;
	nb = int_part(str, &i);
	nb += decimal_part(str + i);
	return ((double)(nb * signe));
}
