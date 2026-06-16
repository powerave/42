/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   words.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbias <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 14:16:09 by jbias             #+#    #+#             */
/*   Updated: 2025/02/10 14:32:39 by jbias            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/push_swap.h"

static int	count_words(char *s, char sep)
{
	int		count;
	int		i;
	bool	inside_word;

	i = 0;
	count = 0;
	while (s[i])
	{
		inside_word = false;
		while (s[i] == sep)
			i++;
		while (s[i] != sep && s[i])
		{
			if (inside_word == false)
			{
				inside_word = true;
				count++;
			}
			i++;
		}
	}
	return (count);
}

static char	*get_next_word(char *s, char sep)
{
	static int	index = 0;
	char		*next_word;
	int			len;
	int			i;

	i = 0;
	len = 0;
	while (s[index] == sep)
		++index;
	while ((s[index + len] != sep) && s[index + len])
		++len;
	next_word = ft_calloc((len + 1), sizeof(char));
	while ((s[index] != sep) && s[index])
		next_word[i++] = s[index++];
	return (next_word);
}

char	**split(char *s, char sep)
{
	int		words;
	char	**result;
	int		i;

	i = 0;
	words = count_words(s, sep);
	if (!words)
		exit(1);
	result = ft_calloc((words + 2), sizeof(char *));
	while (words-- > 0)
	{
		result[i] = get_next_word(s, sep);
		i++;
	}
	return (result);
}
