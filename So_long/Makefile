# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jbias <marvin@42.fr>                       +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/01/06 10:56:57 by jbias             #+#    #+#              #
#    Updated: 2025/01/23 11:50:13 by jbias            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = so_long
SRCS = $(wildcard srcs/*.c) $(wildcard LIBFT/*.c) $(wildcard printf/*.c)

OBJ = $(SRCS:.c=.o)

MLX_DIR	= minilibx-linux
MLX_A	= $(MLX_DIR)/libmlx.a

CC = cc
CFLAGS = -Wall -Wextra -Werror -I $(MLX_DIR) -g
MLX_FLAGS = -lm -lbsd -lX11 -lXext -L .linux

all: $(NAME)

$(NAME): $(OBJ) $(MLX_A)
	$(CC) $(CFLAGS) $(OBJ) $(MLX_A) $(MLX_FLAGS) -o $(NAME)

$(MLX_A):
	make -C $(MLX_DIR)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)
	make clean -C $(MLX_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re

