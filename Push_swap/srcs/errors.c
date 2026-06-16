/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbias <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 15:32:12 by jbias             #+#    #+#             */
/*   Updated: 2025/02/10 15:35:26 by jbias            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/push_swap.h"

int	syntax_error(char *str)
{
	if (!(str[0] == '+' || str[0] == '-' || ft_isdigit(str[0])))
		return (-1);
	if ((str[0] == '+' || str[0] == '-') && !(ft_isdigit(str[1])))
		return (-1);
	if (str[0] == '+' || str[0] == '-')
		str++;
	while (*str)
	{
		if (!(ft_isdigit(*str)))
			return (-1);
		str++;
	}
	return (0);
}

int	error_duplicate(t_stack *a, int n)
{
	if (!a)
		return (0);
	while (a)
	{
		if (a->nbr == n)
			return (1);
		a = a->next;
	}
	return (0);
}

void	free_stack(t_stack **stack)
{
	t_stack	*temp;
	t_stack	*current;

	if (!stack)
		return ;
	current = *stack;
	while (current)
	{
		temp = current->next;
		current->nbr = 0;
		free(current);
		current = temp;
	}
	*stack = NULL;
}

void	free_errors(t_stack **a, char **str)
{
	free_stack(a);
	free_tab(str);
	write(2, "Error\n", 6);
	exit(1);
}
