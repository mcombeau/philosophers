CC		= gcc
CFLAGS	= -Werror -Wall -Wextra -pthread -g
NAME	= philo_bonus

# Run 'make re BUILD=pretty' for color formatting.
# Run 'make re BUILD=debug' for debug formatting.
BUILD	= none
ifeq ($(BUILD), pretty)
	CFLAGS	+= -D DEBUG_FORMATTING=1
endif
ifeq ($(BUILD), debug)
	CFLAGS	+= -D DEBUG_FORMATTING=1 -fsanitize=thread -g
endif

SRC_PATH = sources/
OBJ_PATH = objects/

SRC		=	main.c \
			parsing.c \
			grim_reaper.c \
			time.c \
			philosopher.c \
			philosopher_utils.c \
			init.c \
			utils.c \
			ipc.c \
			output.c \
			cleanup.c \
			exit.c
SRCS	= $(addprefix $(SRC_PATH), $(SRC))
OBJ		= $(SRC:.c=.o)
OBJS	= $(addprefix $(OBJ_PATH), $(OBJ))

INC		= -I ./includes/

all: $(NAME)

$(OBJ_PATH)%.o: $(SRC_PATH)%.c
	@mkdir -p $(OBJ_PATH)
	$(CC) $(CFLAGS) -c $< -o $@ $(INC)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@

clean:
	rm -rf $(OBJ_PATH)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all re clean fclean