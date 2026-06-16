# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jbias <marvin@42.fr>                       +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/02/05 13:43:56 by jbias             #+#    #+#              #
#    Updated: 2025/02/05 14:32:59 by jbias            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRCS = $(wildcard srcs/*.c) $(wildcard LIBFT/*.c) $(wildcard LIBFT/PRINTF/*.c)
OBJ = $(SRCS:.c=.o)
NAME = push_swap
CFLAGS = -Werror -Wextra -Wall -g
CC = gcc

all : $(NAME)

$(NAME) : $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@
clean : 
	rm -f $(OBJ)

fclean : clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
