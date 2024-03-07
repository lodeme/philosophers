# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lodemetz <lodemetz@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/02/27 09:50:52 by louis.demet       #+#    #+#              #
#    Updated: 2024/03/07 17:36:50 by lodemetz         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME =		philo_bonus
CC =		cc
CFLAGS =	-Werror -Wextra -Wall -g
INCLUDES =	-Iincludes
SRC =		src/main.c \
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
