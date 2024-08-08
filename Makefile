# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: chaikney <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/04/20 11:36:56 by chaikney          #+#    #+#              #
#    Updated: 2023/05/17 15:40:23 by chaikney         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	= minishell

SRC		= minishell.c paths.c parse.c builtins.c substitute.c tokens.c \
		helpers.c pipes.c input.c ms_echo.c signals.c exit.c env.c cd.c \
		prompt.c execute.c ms_export.c sort.c env_vars.c \
		checks.c ms_unset.c

OBJ		= $(SRC:.c=.o)

#	libft variables
FTDIR	= ./libft
FTLIB	= -L $(FTDIR) -lft
LIBFT	= $(FTDIR)/libft.a

LIBS	= $(FTLIB) -lreadline

CFLAGS = -Werror -Wall -Wextra -ggdb
CC		= cc

HEADERS	= -I $(FTDIR)

all: $(NAME)

$(NAME): $(OBJ) $(LIBFT)
	$(CC) $(OBJ) $(HEADERS) $(LIBS) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(LIBFT):
	make -C $(FTDIR)

.PHONY: clean, all, fclean, re

clean:
	/bin/rm -f *.o
	/bin/rm -f __.SYMDEF
	/bin/rm -rf *.dSYM
	make clean -C $(FTDIR)

fclean: clean
	/bin/rm -f $(NAME)
	make fclean -C $(FTDIR)

re: fclean all
