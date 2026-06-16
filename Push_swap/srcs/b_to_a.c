/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   b_to_a.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbias <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 15:13:39 by jbias             #+#    #+#             */
/*   Updated: 2025/02/10 15:15:52 by jbias            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/push_swap.h"

static void	find_target_b(t_stack *a, t_stack *b)
{
	t_stack	*current_a;
	t_stack	*target_b;
	long	target;

	while (b)
	{
		target = LONG_MAX;
		current_a = a;
		while (current_a)
		{
			if (current_a->nbr > b->nbr && current_a->nbr < target)
			{
				target = current_a->nbr;
				target_b = current_a;
			}
			current_a = current_a->next;
		}
		if (target == LONG_MAX)
			b->target = find_min(a);
		else
			b->target = target_b;
		b = b->next;
	}
}

void	init_all_b(t_stack *a, t_stack *b)
{
	current_index(a);
	current_index(b);
	find_target_b(a, b);
}
