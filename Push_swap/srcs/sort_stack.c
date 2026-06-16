/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_stack.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbias <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 14:05:09 by jbias             #+#    #+#             */
/*   Updated: 2025/02/10 15:12:07 by jbias            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/push_swap.h"

static void	move_a_to_b(t_stack **a, t_stack **b)
{
	t_stack		*cheapest;

	cheapest = find_cheapest(*a);
	if (cheapest->above_median && cheapest->target->above_median)
	{
		while (*b != cheapest->target && *a != cheapest)
			rr(a, b, false);
		current_index(*a);
		current_index(*b);
	}
	else if (!(cheapest->above_median) && !(cheapest->target->above_median))
	{
		while (*b != cheapest->target && *a != cheapest)
			rrr(a, b, false);
		current_index(*a);
		current_index(*b);
	}
	prepare_push(a, cheapest, 'a');
	prepare_push(b, cheapest->target, 'b');
	pb(b, a, false);
}

static void	move_b_to_a(t_stack **a, t_stack **b)
{
	prepare_push(a, (*b)->target, 'a');
	pa(a, b, false);
}

static void	smallest_to_top(t_stack **a)
{
	while ((*a)->nbr != find_min(*a)->nbr)
	{
		if (find_min(*a)->above_median)
			ra(a, false);
		else
			rra(a, false);
	}
}

void	sort_stacks(t_stack **a, t_stack **b)
{
	int	len_a;

	len_a = stack_len(*a);
	if (len_a-- > 3 && !is_sorted(*a))
		pb(b, a, false);
	if (len_a-- > 3 && !is_sorted(*a))
		pb(b, a, false);
	while (len_a-- > 3 && !is_sorted(*a))
	{
		init_all_a(*a, *b);
		move_a_to_b(a, b);
	}
	sort_three(a);
	while (*b)
	{
		init_all_b(*a, *b);
		move_b_to_a(a, b);
	}
	current_index(*a);
	smallest_to_top(a);
}
