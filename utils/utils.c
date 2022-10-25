/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hako <hako@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/19 18:53:39 by hako              #+#    #+#             */
/*   Updated: 2022/10/24 17:55:13 by hako             ###   ########.fr       */
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

t_vec	calcul_color(t_scene *sc, t_hit_record hr, t_vec amb, t_ray ray)
{
	t_light		*light;
	t_vec		ret;
	t_vec		hit_light;
	double		d;

	ret = create_vec(0, 0, 0);
	light = sc->light;
	if (!light)
		return (amb);
	if (shadow(sc, hr, light))
		ret = add_color(ret, amb);
	else
	{
		hit_light = vec_sub(light->src, hr.p);
		d = vdot(unit_vec(hit_light), hr.normal);
		ret = add_color(ret, amb);
		if (d >= 0)
			ret = add_color(ret, diffuse(hr, light, d));
		ret = add_color(ret, specular(hr, light, ray));
		ret = vec_scalar_mul(ret, LUMEN * light->ratio);
	}
	return (vmin(ret, create_vec(1, 1, 1)));
}
