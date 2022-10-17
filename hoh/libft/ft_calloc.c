/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hako <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/14 22:47:11 by hako              #+#    #+#             */
/*   Updated: 2021/11/14 22:47:15 by hako             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t count, size_t size)
{
	void	*array;
	size_t	total;

	total = count * size;
	array = malloc(total);
	if (!array)
		return (0);
	ft_memset(array, 0, total);
	return (array);
}
