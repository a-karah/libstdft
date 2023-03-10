/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akarahan <akarahan@student.42istanbul.com. +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/20 11:39:57 by akarahan          #+#    #+#             */
/*   Updated: 2022/12/21 16:24:19 by akarahan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static void	append(char **s1, char *s2)
{
	char	*tmp;

	tmp = ft_strjoin(*s1, s2);
	free(*s1);
	*s1 = tmp;
}

static char	*save_and_clear(char **line)
{
	char	*out;
	char	*tmp;
	char	*nl;

	nl = ft_strchr(*line, '\n');
	if (nl)
	{
		out = ft_substr(*line, 0, nl - *line + 1);
		tmp = ft_strdup(nl + 1);
		free(*line);
		*line = tmp;
		if (!**line)
		{
			free(*line);
			*line = NULL;
		}
		return (out);
	}
	out = ft_strdup(*line);
	free(*line);
	*line = NULL;
	return (out);
}

static int	read_and_append(char **line, int fd)
{
	ssize_t	bytes_read;
	char	*buf;

	buf = (char *)malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!buf)
		return (-1);
	bytes_read = read(fd, buf, BUFFER_SIZE);
	while (bytes_read > 0)
	{
		buf[bytes_read] = 0;
		if (!*line)
			*line = ft_strdup("");
		append(line, buf);
		if (ft_strchr(buf, '\n'))
			break ;
		bytes_read = read(fd, buf, BUFFER_SIZE);
	}
	free(buf);
	return (bytes_read);
}

char	*get_next_line(int fd)
{
	static char	*persistent[MAX_FD];
	char		*ret_line;
	int			bytes_read;

	if (fd < 0 || BUFFER_SIZE < 1 || fd > MAX_FD)
		return (NULL);
	bytes_read = read_and_append(&persistent[fd], fd);
	if (bytes_read < 0)
		return (NULL);
	if (!bytes_read && !persistent[fd])
		return (NULL);
	ret_line = save_and_clear(&persistent[fd]);
	return (ret_line);
}
