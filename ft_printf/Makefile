# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jbias <marvin@42.fr>                       +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/11/22 13:58:38 by jbias             #+#    #+#              #
#    Updated: 2024/11/29 14:19:40 by jbias            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRC =	ft_hexlower_len.c ft_hexupper_len.c ft_printf.c ft_putnbr_len.c ft_strchr.c \
	ft_putchar.c ft_get_adr.c ft_putstr.c ft_putnbru_len.c

OBJ = $(SRC:.c=.o)

FLAGS = -Werror -Wextra -Wall

NAME = libftprintf.a

all: $(NAME)

$(NAME): $(OBJ)
	ar -rcs $(NAME) $(OBJ)

##$(OBJ): $(SRC)
##	cc $(CFLAGS) -c $(SRC) -I./

clean:
	rm -f $(OBJ)

fclean: clean 
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean 
