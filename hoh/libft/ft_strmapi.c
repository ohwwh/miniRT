/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hako <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/14 23:00:38 by hako              #+#    #+#             */
/*   Updated: 2021/11/16 10:24:30 by hako             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	size_t	index;
	size_t	length;
	char	*str;

	if (!s || !f)
		return (0);
	index = 0;
	length = ft_strlen(s);
	str = (char *)ft_calloc(length + 1, sizeof(char));
	if (!str)
		return (0);
	while (index < length)
	{
		str[index] = (*f)(index, s[index]);
		index++;
	}
	str[index] = '\0';
	return (str);
}
