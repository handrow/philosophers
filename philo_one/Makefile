NAME = philo_one

CC = clang
CC_FLAGS = -Wall -Wextra -Werror -O3

DIR_O = objs
OBJS = $(addprefix $(DIR_O)/,$(FILES:.c=.o))
FILES = main.c utils.c atomic.c globals.c init_helper.c printer.c timing.c worker.c

DEPS = $(OBJS:.o=.d)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CC_FLAGS) -o $(NAME) $(OBJS)
	
$(DIR_O)/%.o: %.c
	mkdir -p $(DIR_O)
	$(CC) $(CC_FLAGS) -o $@ -c $< -MMD

clean:
	rm -rf $(DIR_O)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re

-include $(DEP)