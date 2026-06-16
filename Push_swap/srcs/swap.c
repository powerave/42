/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   swap.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbias <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 14:37:06 by jbias             #+#    #+#             */
/*   Updated: 2025/02/10 14:38:43 by jbias            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/push_swap.h"

static void	swap(t_stack **a)
{
	if (!*a || !((*a)->next))
		return ;
	*a = (*a)->next;
	(*a)->prev->prev = *a;
	(*a)->prev->next = (*a)->next;
	if ((*a)->next)
		(*a)->next->prev = (*a)->prev;
	(*a)->next = (*a)->prev;
	(*a)->prev = NULL;
}

void	sa(t_stack **a, bool print)
{
	swap(a);
	if (!print)
		ft_printf("sa\n");
}

void	sb(t_stack **b, bool print)
{
	swap(b);
	if (!print)
		ft_printf("sb\n");
}

void	ss(t_stack **a, t_stack **b, bool print)
{
	swap(a);
	swap(b);
	if (!print)
		ft_printf("ss\n");
}
