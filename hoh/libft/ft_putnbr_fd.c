/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hako <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/14 22:44:10 by hako              #+#    #+#             */
/*   Updated: 2021/11/16 12:38:47 by hako             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putnbr_fd(int n, int fd)
{
	unsigned int	uns_num;

	if (n < 0)
	{
		write(fd, "-", 1);
		uns_num = -1 * n;
	}
	else
		uns_num = n;
	if (uns_num >= 10)
		ft_putnbr_fd(uns_num / 10, fd);
	ft_putchar_fd('0' + (uns_num % 10), fd);
}
