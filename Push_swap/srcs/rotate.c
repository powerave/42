/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rotate.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbias <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 14:11:58 by jbias             #+#    #+#             */
/*   Updated: 2025/02/10 15:25:22 by jbias            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/push_swap.h"

static void	rotate(t_stack **a)
{
	t_stack		*c;

	if (!*a || !(*a)->next)
		return ;
	c = *a;
	*a = (*a)->next;
	(*a)->prev = NULL;
	c->prev = find_last(*a);
	c->prev->next = c;
	c->next = NULL;
}

void	ra(t_stack **a, bool print)
{
	rotate(a);
	if (!print)
		ft_printf("ra\n");
}

void	rb(t_stack **b, bool print)
{
	rotate(b);
	if (!print)
		ft_printf("rb\n");
}

void	rr(t_stack **a, t_stack **b, bool print)
{
	rotate(a);
	rotate(b);
	if (!print)
		ft_printf("rr\n");
}
