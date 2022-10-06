/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hako <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/14 23:16:00 by hako              #+#    #+#             */
/*   Updated: 2021/11/16 11:27:23 by hako             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	char	*cpy;

	cpy = (char *)s + ft_strlen(s);
	while (c >= 256)
		c = c % 256;
	if (c == '\0')
		return (cpy);
	while (s != cpy)
	{
		cpy--;
		if (*cpy == c)
			return (cpy);
	}
	return (0);
}
