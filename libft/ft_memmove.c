/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hako <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/14 23:08:17 by hako              #+#    #+#             */
/*   Updated: 2021/11/14 23:26:16 by hako             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dst, const void *src, size_t len)
{
	size_t	index;

	if (!dst && !src)
		return (0);
	if (dst < src)
	{
		index = 0;
		while (index < len)
		{
			((unsigned char *)dst)[index] = ((unsigned char *)src)[index];
			index++;
		}
	}
	else
	{
		index = len;
		while (index)
		{
			((unsigned char *)dst)[index - 1]
				= ((unsigned char *)src)[index - 1];
			index--;
		}
	}
	return (dst);
}
