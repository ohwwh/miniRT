/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hako <hako@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/18 09:32:13 by hako              #+#    #+#             */
/*   Updated: 2022/10/25 13:17:34 by hako             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*extract(char const *s, unsigned int start, size_t len)
{
	size_t	string_len;
	size_t	i;
	char	*substr;

	if (s == NULL)
		return (NULL);
	string_len = ft_strlen(s);
	if (start > string_len)
		return (ft_strdup(""));
	if (start + len > string_len)
		len = string_len - start;
	substr = malloc((len + 1) * sizeof(char));
	if (substr == NULL)
		return (NULL);
	i = 0;
	while (s[start + i] && i < len)
	{
		substr[i] = s[start + i];
		i++;
	}
	substr[i] = '\0';
	return (substr);
}

static char	*format_line(char **buffer, char *swap, int new_line_index)
{
	char	*line_formated;

	if (new_line_index < 0)
	{
		if (!**buffer && !swap)
		{
			free(*buffer);
			*buffer = NULL;
			return (NULL);
		}
		line_formated = ft_strdup(*buffer);
		free(*buffer);
		*buffer = NULL;
		return (line_formated);
	}
	*buffer = extract(swap, new_line_index + 1, ft_strlen(swap));
	swap[new_line_index + 1] = '\0';
	line_formated = swap;
	swap = NULL;
	return (line_formated);
}

static char	*get_line(int fd, char **buffer, char *read_buffer)
{
	int		read_bytes;
	char	*swap;
	char	*find_new_line;
	int		new_line_position;

	swap = NULL;
	find_new_line = ft_strchr(*buffer, '\n');
	while (find_new_line == NULL)
	{
		read_bytes = read(fd, read_buffer, BUFFER_SIZE);
		if (read_bytes <= 0)
			return (format_line(buffer, swap, -1));
		read_buffer[read_bytes] = '\0';
		swap = ft_strjoin(*buffer, read_buffer);
		free(*buffer);
		*buffer = swap;
		find_new_line = ft_strchr(*buffer, '\n');
	}
	swap = *buffer;
	new_line_position = 0;
	while (swap[new_line_position] != '\n')
		new_line_position++;
	return (format_line(buffer, swap, new_line_position));
}

char	*get_next_line(int fd)
{
	static char	*buffer[MAX_FD];
	char		*read_buffer;
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0 || fd > MAX_FD)
		return (NULL);
	if (!buffer[fd])
		buffer[fd] = ft_strdup("");
	read_buffer = (char *)malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!read_buffer)
		return (NULL);
	line = get_line(fd, &buffer[fd], read_buffer);
	free(read_buffer);
	return (line);
}
