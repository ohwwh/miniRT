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

double	ft_atod(const char *str)
{
	double	nb;
	int		signe;
	int		i;
	int		aux;

	i = 0;
	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
		i++;
	signe = 1;
	if (str[i] == '-' || str[i] == '+')
		if (str[i++] == '-')
			signe = -1;
	nb = 0;
	while (str[i] >= '0' && str[i] <= '9')
		nb = nb * 10 + (str[i++] - '0');
	if (str[i] != '.')
		return ((double)nb * signe);
	aux = 10;
	while (str[++i] >= '0' && str[i] <= '9')
	{
		nb = nb + (double)(str[i] - '0') / aux;
		aux = aux * 10;
	}
	return ((double)nb * signe);
}