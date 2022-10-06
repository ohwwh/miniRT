/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hako <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/14 23:13:10 by hako              #+#    #+#             */
/*   Updated: 2021/11/17 11:00:56 by hako             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static	int	check_len(size_t *len, const char *haystack)
{
	if (*len > ft_strlen(haystack))
		*len = ft_strlen(haystack);
	if (*len <= 0)
		return (0);
	return (1);
}

char	*ft_strnstr(const char *haystack, const char *needle, size_t len)
{
	size_t	i;
	size_t	j;

	if (*needle == '\0')
		return ((char *)haystack);
	if (!check_len(&len, haystack))
		return (0);
	i = 0;
	while (i < len)
	{
		if (haystack[i] == needle[0])
		{
			j = 0;
			while ((i + j) < len && needle[j] != '\0')
			{
				if (haystack[i + j] != needle[j])
					break ;
				j++;
			}
			if (needle[j] == '\0')
				return ((char *)(haystack + i));
		}
		i++;
	}
	return (0);
}
