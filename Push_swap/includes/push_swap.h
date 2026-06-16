/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   push_swap.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbias <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 13:56:36 by jbias             #+#    #+#             */
/*   Updated: 2025/02/10 14:08:50 by jbias            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PUSH_SWAP_H
# define PUSH_SWAP_H

# include <unistd.h>
# include <stddef.h>
# include <limits.h>
# include <stdio.h>
# include <stdlib.h>
# include <stdbool.h>
# include "libft.h"
# include "get_next_line.h"
# include "ft_printf.h"

typedef struct s_stack {
	int						nbr;
	int						index;
	int						cost;
	bool					above_median;
	bool					cheapest;
	struct s_stack			*target;
	struct s_stack			*next;
	struct s_stack			*prev;
}	t_stack;

void	sa(t_stack **a, bool print);
void	sb(t_stack **b, bool print);
void	ss(t_stack **a, t_stack **b, bool print);
void	pa(t_stack **a, t_stack **b, bool print);
void	pb(t_stack **a, t_stack **b, bool print);
void	ra(t_stack **a, bool print);
void	rb(t_stack **b, bool print);
void	rr(t_stack **a, t_stack **b, bool print);
void	rra(t_stack **a, bool print);
void	rrb(t_stack **b, bool print);
void	rrr(t_stack **a, t_stack **b, bool print);
void	current_index(t_stack *stack);
void	set_cheapest(t_stack *stack);
void	init_all_a(t_stack *a, t_stack *b);
void	init_all_b(t_stack *a, t_stack *b);
int		syntax_error(char *str);
int		error_duplicate(t_stack *a, int n);
void	free_stack(t_stack **stack);
void	free_errors(t_stack **a, char **str);
void	init_stack_a(t_stack **a, char **av);
t_stack	*find_cheapest(t_stack *stack);
void	prepare_push(t_stack **stack, t_stack *top, char stack_name);
int		stack_len(t_stack *stack);
t_stack	*find_last(t_stack *stack);
bool	is_sorted(t_stack *stack);
t_stack	*find_min(t_stack *stack);
t_stack	*find_max(t_stack *stack);
void	sort_stacks(t_stack **a, t_stack **b);
void	sort_three(t_stack **a);
char	**split(char *s, char sep);

#endif
