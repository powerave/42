CC = gcc
CFLAGS = -Wall -Wextra -Werror -g
SRCS = philosopher.c utils.c utils2.c utils3.c
OBJS = $(SRCS:.c=.o)
NAME = philo

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re