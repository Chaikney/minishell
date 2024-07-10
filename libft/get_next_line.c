/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chaikney <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/21 14:01:27 by chaikney          #+#    #+#             */
/*   Updated: 2023/08/21 14:01:51 by chaikney         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// returns a NT'd string that contains a newline if there are any in the file
// returns NULL on allocation or read failure.
static char	*run_to_nextline(int fd, char *remains)
{
	char		*buffer;
	int			bytesread;

	bytesread = 1;
	buffer = malloc((BUFFER_SIZE + 1) * sizeof(char));
	if (!buffer)
		return (NULL);
	while ((ft_hasnewline(remains) == 0) && (bytesread != 0))
	{
		bytesread = read(fd, buffer, BUFFER_SIZE);
		if (bytesread == -1)
			return (free (remains), free(buffer), NULL);
		buffer[bytesread] = '\0';
		remains = gnl_strjoin(remains, buffer);
	}
	free (buffer);
	return (remains);
}

// Work out the space needed for bring_me_the_head
static char	*fill_the_head(char *skull)
{
	int		i;
	char	*brain;

	i = 0;
	while (skull[i] && (skull[i] != '\n'))
		i++;
	if (skull[i] != '\n')
		brain = (char *) malloc(sizeof(char) * (i + 1));
	else
		brain = (char *) malloc(sizeof(char) * (i + 2));
	return (brain);
}

// Takes a string and returns a copy of it up to
// (and including) the first newline.
static char	*bring_me_the_head(char *alfredo)
{
	int		i;
	char	*head;

	if (!alfredo[0])
		return (NULL);
	head = fill_the_head(alfredo);
	if (!head)
		return (NULL);
	i = 0;
	while (alfredo[i] && (alfredo[i] != '\n'))
	{
		head[i] = alfredo[i];
		i++;
	}
	if (alfredo[i] == '\n')
	{
		head[i] = alfredo[i];
		i++;
	}
	head[i] = '\0';
	return (head);
}

// Take string, return anything that it contains after
// the first newline. NULL if the NL is at the end.
// Free the string we were given, as the return replaces it
static char	*bring_me_the_rest(char *alfredo)
{
	int		i;
	int		j;
	char	*garcia;

	i = 0;
	while (alfredo[i] && alfredo[i] != '\n')
		i++;
	if (!alfredo[i])
		return (free (alfredo), NULL);
	garcia = malloc((ft_strlen(alfredo) - i + 1) * sizeof(char));
	if (!garcia)
		return (NULL);
	i++;
	j = 0;
	while (alfredo[i] != '\0')
		garcia[j++] = alfredo[i++];
	garcia[j] = '\0';
	free (alfredo);
	return (garcia);
}

// Return a line read from file descriptor fd
// (NULL if error, or nothing else to read)
char	*get_next_line(int fd)
{
	static char	*remains[2048];
	char		*line;

	if ((fd < 0) || (BUFFER_SIZE <= 0))
		return (NULL);
	remains[fd] = run_to_nextline(fd, remains[fd]);
	if (!remains[fd])
		return (NULL);
	line = bring_me_the_head(remains[fd]);
	remains[fd] = bring_me_the_rest(remains[fd]);
	if (!line)
		free (remains[fd]);
	return (line);
}
