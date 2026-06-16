/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbias <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 14:34:38 by jbias             #+#    #+#             */
/*   Updated: 2025/02/10 14:36:33 by jbias            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/push_swap.h"

int	stack_len(t_stack *stack)
{
	int	count;

	if (!stack)
		return (0);
	count = 0;
	while (stack)
	{
		stack = stack->next;
		count++;
	}
	return (count);
}

t_stack	*find_last(t_stack *stack)
{
	if (!stack)
		return (NULL);
	while (stack->next)
		stack = stack->next;
	return (stack);
}

bool	is_sorted(t_stack *stack)
{
	if (!stack)
		return (1);
	while (stack->next)
	{
		if (stack->nbr > stack->next->nbr)
			return (false);
		stack = stack->next;
	}
	return (true);
}

t_stack	*find_min(t_stack *stack)
{
	long	min;
	t_stack	*min_stack;

	if (!stack)
		return (NULL);
	min = LONG_MAX;
	while (stack)
	{
		if (stack->nbr < min)
		{
			min = stack->nbr;
			min_stack = stack;
		}
		stack = stack->next;
	}
	return (min_stack);
}

t_stack	*find_max(t_stack *stack)
{
	long	max;
	t_stack	*max_stack;

	if (!stack)
		return (NULL);
	max = LONG_MIN;
	while (stack)
	{
		if (stack->nbr > max)
		{
			max = stack->nbr;
			max_stack = stack;
		}
		stack = stack->next;
	}
	return (max_stack);
}
