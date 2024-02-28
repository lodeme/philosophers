# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: louis.demetz <louis.demetz@student.42.f    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/02/27 09:50:52 by louis.demet       #+#    #+#              #
#    Updated: 2024/02/28 23:16:07 by louis.demet      ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME =		philo
CC =		cc
CFLAGS =	-Werror -Wextra -Wall -g
INCLUDES =	-Iincludes
SRC =		src/main.c \
			src/threads.c \
			src/cycles.c \
			src/init.c \
			src/utils.c \
			src/free.c
OBJ_DIR =	obj/
OBJ =		$(SRC:src/%.c=$(OBJ_DIR)%.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(INCLUDES) $(OBJ) -o $(NAME)

$(OBJ_DIR)%.o: src/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
