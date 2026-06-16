# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jbias <marvin@42.fr>                       +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/02/25 05:20:16 by jbias             #+#    #+#              #
#    Updated: 2025/03/07 10:56:23 by jbias            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRCS = $(wildcard srcs/*.c) $(wildcard LIBFT/*.c) $(wildcard PRINTF/*.c)
BONUS = $(wildcard bonus/*.c) $(wildcard LIBFT/*.c) $(wildcard PRINTF/*.c)

OBJ = $(SRCS:.c=.o)
OBJ_BONUS = $(BONUS:.c=.o)

CC = gcc
CFLAGS = -Werror -Wextra -Wall -g

NAME = pipex
NAME_BONUS = pipex_bonus

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

bonus: $(NAME_BONUS)

$(NAME_BONUS): $(OBJ_BONUS)
	$(CC) $(CFLAGS) $(OBJ_BONUS) -o $(NAME_BONUS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(OBJ_BONUS)

fclean: clean
	rm -f $(NAME) $(NAME_BONUS)

re: fclean all

re_bonus: fclean bonus

.PHONY: all clean fclean re bonus re_bonus