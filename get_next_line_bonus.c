/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moboulah <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/06 16:12:39 by moboulah          #+#    #+#             */
/*   Updated: 2026/11/11 22:29:27 by moboulah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

static char	*final_word(char *buf)
{
	int		i;
	char	*final;

	i = 0;
	while (buf[i] != '\n' && buf[i])
		i++;
	final = malloc(i + 2);
	if (!final)
		return (NULL);
	i = 0;
	while (buf[i] != '\n' && buf[i])
	{
		final[i] = buf[i];
		i++;
	}
	final[i] = '\n';
	final[i + 1] = '\0';
	return (final);
}

static char	*append_str(char *str, char *buf)
{
	char	*s;

	s = ft_strjoin(str, buf);
	free(str);
	return (s);
}

static char	*handle_rest(char **rest, char **str)
{
	char	*rest_tmp;

	if (!*rest)
		return (NULL);
	if (ft_strchr(*rest, '\n'))
	{
		rest_tmp = final_word(*rest);
		*str = append_str(*str, rest_tmp);
		free(rest_tmp);
		rest_tmp = ft_strdup(ft_strchr(*rest, '\n') + 1);
		free(*rest);
		*rest = rest_tmp;
		return (*str);
	}
	*str = append_str(*str, *rest);
	free(*rest);
	*rest = NULL;
	return (NULL);
}

static char	*fill_str(int fd, char *str, char **rest)
{
	char	*buf;
	char	*tbuf;
	int		n;

	n = 1;
	buf = malloc(BUFFER_SIZE + 1);
	if (!buf)
		return (free(str), free(*rest), *rest = NULL, NULL);
	while (n > 0)
	{
		n = read(fd, buf, BUFFER_SIZE);
		if (n < 0)
			return (free(buf), free(*rest), *rest = NULL, free(str), NULL);
		buf[n] = '\0';
		if (ft_strchr(buf, '\n'))
		{
			*rest = ft_strdup(ft_strchr(buf, '\n') + 1);
			tbuf = final_word(buf);
			return (str = append_str(str, tbuf), free(tbuf), free(buf), str);
		}
		str = append_str(str, buf);
	}
	if (!*str)
		return (free(buf), free(str), NULL);
	return (free(buf), str);
}

char	*get_next_line(int fd)
{
	static char	*rest[OPEN_MAX];
	char		*str;
	char		*str_wrest;

	if (fd < 0 || fd >= OPEN_MAX || BUFFER_SIZE <= 0)
		return (NULL);
	str = ft_strdup("");
	str_wrest = handle_rest(&rest[fd], &str);
	if (str_wrest)
		return (str_wrest);
	return (fill_str(fd, str, &rest[fd]));
}
