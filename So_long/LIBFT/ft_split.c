/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbias <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 15:11:20 by jbias             #+#    #+#             */
/*   Updated: 2024/11/21 17:48:56 by jbias            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/so_long.h"

static void	*ft_free(char **spl, int len)
{
	while (len >= 0)
	{
		free(spl[len]);
		len--;
	}
	free (spl);
	return (0);
}

static	int	ft_countstr(char *str, char c)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (str[i])
	{
		while (str[i] == c && str[i] != '\0')
			i++;
		if (str[i])
			count++;
		while (str[i] != c && str[i] != '\0')
			i++;
	}
	return (count);
}

static char	*ft_dostr(char *s, char c)
{
	int		i;
	int		len;
	char	*str;

	i = 0;
	len = 0;
	while ((s[len] != c) && s[len] != '\0')
		len++;
	str = (char *)malloc(sizeof(char) * len + 1);
	if (!str)
		return (NULL);
	while (i < len)
	{
		str[i] = s[i];
		i++;
	}
	str[i] = '\0';
	return (str);
}

char	**ft_split(char const *s, char c)
{
	char		**spl;
	size_t		i;

	i = 0;
	if (!s)
		return (NULL);
	spl = (char **)malloc(sizeof(char *) * (ft_countstr((char *)s, c) + 1));
	if (!spl)
		return (NULL);
	while (*s)
	{
		while ((*s == c) && *s)
			s++;
		if (*s)
		{
			spl[i] = ft_dostr((char *)s, c);
			if (!spl[i])
				return (ft_free(spl, i));
			i++;
		}
		while (*s != c && *s)
			s++;
	}
	spl[i] = 0;
	return (spl);
}

/*int	main(void)
{
	char **split;
	
	split = ft_split("xxxxxxxxhello!", 'x');
	return (0);
}*/
