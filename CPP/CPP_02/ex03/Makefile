CXX = g++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98
NAME = bsp
SRCS = main.cpp Point.cpp Fixed.cpp bsp.cpp
OBJS = $(SRCS:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
