/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hako <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/14 22:27:02 by hako              #+#    #+#             */
/*   Updated: 2021/11/16 10:51:53 by hako             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*new_list;
	t_list	*src_next;
	t_list	*new_next;

	if (!lst)
		return (0);
	new_list = ft_lstnew(f(lst->content));
	if (!new_list)
		return (0);
	new_next = new_list;
	src_next = lst->next;
	while (src_next)
	{
		new_next->next = ft_lstnew(f(src_next->content));
		if (!new_next->next)
		{
			ft_lstclear(&new_list, del);
			return (0);
		}
		new_next = new_next->next;
		src_next = src_next->next;
	}
	return (new_list);
}
