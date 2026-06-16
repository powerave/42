/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   push.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbias <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 14:38:59 by jbias             #+#    #+#             */
/*   Updated: 2025/02/10 14:39:52 by jbias            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/push_swap.h"

static void	push(t_stack **dst, t_stack **src)
{
	t_stack		*c;

	if (!*src)
		return ;
	c = *src;
	*src = (*src)->next;
	if (*src)
		(*src)->prev = NULL;
	c->prev = NULL;
	if (!*dst)
	{
		*dst = c;
		c->next = NULL;
	}
	else
	{
		c->next = *dst;
		c->next->prev = c;
		*dst = c;
	}
}

void	pa(t_stack **a, t_stack **b, bool print)
{
	push(a, b);
	if (!print)
		ft_printf("pa\n");
}

void	pb(t_stack **b, t_stack **a, bool print)
{
	push(b, a);
	if (!print)
		ft_printf("pb\n");
}
