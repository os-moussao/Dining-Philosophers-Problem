NAME := philo

RM := rm -rf

CC := cc

CFLAGS := -Wall -Werror -Wextra

.PHONY: all clean fclean re

CFILES := philosophers.c ft_atoi.c utils.c utils2.c

SRCS := $(CFILES)

OBJS := $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ -lpthread

%.o: %.c philosophers.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all
