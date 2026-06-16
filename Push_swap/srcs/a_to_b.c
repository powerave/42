/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   a_to_b.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbias <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 15:15:57 by jbias             #+#    #+#             */
/*   Updated: 2025/02/10 15:16:32 by jbias            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/push_swap.h"

void	current_index(t_stack *stack)
{
	int	i;
	int	median;

	i = 0;
	if (!stack)
		return ;
	median = stack_len(stack) / 2;
	while (stack)
	{
		stack->index = i;
		if (i <= median)
			stack->above_median = true;
		else
			stack->above_median = false;
		stack = stack->next;
		i++;
	}
}

static void	set_target_a(t_stack *a, t_stack *b)
{
	t_stack		*current_b;
	t_stack		*target;
	long		closest_index;

	while (a)
	{
		closest_index = LONG_MIN;
		current_b = b;
		while (current_b)
		{
			if (current_b->nbr < a->nbr && current_b->nbr > closest_index)
			{
				closest_index = current_b->nbr;
				target = current_b;
			}
			current_b = current_b->next;
		}
		if (closest_index == LONG_MIN)
			a->target = find_max(b);
		else
			a->target = target;
		a = a->next;
	}
}

static void	find_cost(t_stack *a, t_stack *b)
{
	int	len_a;
	int	len_b;

	len_a = stack_len(a);
	len_b = stack_len(b);
	while (a)
	{
		a->cost = a->index;
		if (!(a->above_median))
			a->cost = len_a - (a->index);
		if (a->target->above_median)
			a->cost += a->target->index;
		else
			a->cost += len_b - (a->target->index);
		a = a->next;
	}
}

void	set_cheapest(t_stack *stack)
{
	long		cheapest_value;
	t_stack		*cheapest;

	if (!stack)
		return ;
	cheapest_value = LONG_MAX;
	while (stack)
	{
		if (stack->cost < cheapest_value)
		{
			cheapest_value = stack->cost;
			cheapest = stack;
		}
		stack = stack->next;
	}
	cheapest->cheapest = true;
}

void	init_all_a(t_stack *a, t_stack *b)
{
	current_index(a);
	current_index(b);
	set_target_a(a, b);
	find_cost(a, b);
	set_cheapest(a);
}
