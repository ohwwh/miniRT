/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hako <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/14 22:59:49 by hako              #+#    #+#             */
/*   Updated: 2021/11/14 22:59:51 by hako             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	unsigned char	*arr1;
	unsigned char	*arr2;

	if (n == 0)
		return (0);
	arr1 = (unsigned char *)s1;
	arr2 = (unsigned char *)s2;
	while ((*arr1 == *arr2) && n - 1 > 0)
	{
		arr1++;
		arr2++;
		n--;
	}
	return (*arr1 - *arr2);
}
