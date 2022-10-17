/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hako <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/14 22:30:49 by hako              #+#    #+#             */
/*   Updated: 2021/11/14 22:33:10 by hako             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	is_setpart(const char c, const char *set)
{
	while (*set)
	{
		if (*set == c)
			return (1);
		set++;
	}
	return (0);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	size_t	start;
	size_t	end;
	char	*ans;

	if (!s1 && !set)
		return (0);
	start = 0;
	end = ft_strlen(s1);
	while (s1[start] && is_setpart(s1[start], set))
		start++;
	while (s1[end - 1] && is_setpart(s1[end - 1], set) && end > start)
		end--;
	ans = (char *)malloc(sizeof(char) * (end - start + 1));
	if (!ans)
		return (NULL);
	ft_strlcpy(ans, &s1[start], end - start + 1);
	return (ans);
}
