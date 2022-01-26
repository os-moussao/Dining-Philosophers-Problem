NAME := philo

RM := rm -rf

CC := cc

CFLAGS := -Wall -Werror -Wextra

.PHONY: all clean fclean re

# INCLUDE := includes/

# SRCS_DIR := srcs/

CFILES := philosophers.c ft_atoi.c utils.c
# HFILES := $(INCLUDE)ft_printf.h

SRCS := $(CFILES)
# SRCS := $(addprefix $(SRCS_DIR), $(CFILES) ft_printf_bonus.c)

OBJS := $(SRCS:.c=.o)
# OBJSB := $(SRCSB:.c=.o)

all: $(NAME)

bonus: $(OBJSB)
	$(AR) $(NAME) $^

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ -lpthread

%.o: %.c # $(HFILES)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all