/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hako <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/14 22:36:14 by hako              #+#    #+#             */
/*   Updated: 2021/11/14 22:36:17 by hako             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dest, const char *src, size_t size)
{
	char	*dst;
	int		i;
	size_t	dst_len;
	size_t	remaing;

	dst = dest;
	remaing = size;
	while (remaing-- != 0 && *dst != '\0')
		dst++;
	dst_len = dst - dest;
	remaing = size - dst_len;
	if (remaing == 0)
		return (dst_len + ft_strlen(src));
	i = 0;
	while (src[i] != '\0')
	{
		if (remaing > 1)
		{
			*dst++ = src[i];
			remaing--;
		}
		i++;
	}
	*dst = '\0';
	return (dst_len + i);
}
