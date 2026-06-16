/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reverse.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbias <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 14:13:55 by jbias             #+#    #+#             */
/*   Updated: 2025/02/10 14:33:39 by jbias            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/push_swap.h"

static void	rev_rotate(t_stack **a)
{
	t_stack	*last;

	if (!*a || !(*a)->next)
		return ;
	last = find_last(*a);
	last->prev->next = NULL;
	last->next = *a;
	last->prev = NULL;
	*a = last;
	last->next->prev = last;
}

void	rra(t_stack **a, bool print)
{
	rev_rotate(a);
	if (!print)
		ft_printf("rra\n");
}

void	rrb(t_stack **b, bool print)
{
	rev_rotate(b);
	if (!print)
		ft_printf("rrb\n");
}

void	rrr(t_stack **a, t_stack **b, bool print)
{
	rev_rotate(a);
	rev_rotate(b);
	if (!print)
		ft_printf("rrr\n");
}
