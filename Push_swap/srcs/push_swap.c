/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   push_swap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbias <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 14:40:05 by jbias             #+#    #+#             */
/*   Updated: 2025/02/10 14:41:01 by jbias            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/push_swap.h"

char	**get_str(int ac, char **av)
{
	int		i;
	int		j;
	char	**str;

	j = 0;
	i = 0;
	str = ft_calloc(ac, sizeof(char *));
	if (!str)
		return (NULL);
	while (av[i])
	{
		str[i] = ft_calloc(ft_strlen(av[i]), (sizeof(char) + 1));
		j = 0;
		while (av[i][j])
		{
			str[i][j] = av[i][j];
			j++;
		}
		i++;
	}
	return (str);
}

int	main(int ac, char **av)
{
	t_stack	*a;
	t_stack	*b;
	char	**str;

	str = get_str(ac, av + 1);
	a = NULL;
	b = NULL;
	if (ac == 1 || (ac == 2 && !av[1][0]))
		return (free_tab(str), 1);
	else if (ac == 2)
	{
		free_tab(str);
		str = split(av[1], ' ');
	}
	init_stack_a(&a, str);
	if (!is_sorted(a))
	{
		if (stack_len(a) == 2)
			sa(&a, false);
		else if (stack_len(a) == 3)
			sort_three(&a);
		else
			sort_stacks(&a, &b);
	}
	return (free_stack(&a), free_tab(str), 0);
}
