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

# FIXED add a -L call to include readline
NAME	= minishell

SRC		= minishell.c paths.c parse.c builtins.c substitute.c

OBJ		= $(SRC:.c=.o)

#	libft variables
FTDIR	= ./libft
FTLIB	= -L $(FTDIR) -lft

LIBS	= $(FTLIB) -lreadline

CFLAGS = -Werror -Wall -Wextra
CC		= cc

HEADERS	= -I $(FTDIR)

all: $(NAME)

$(NAME): libft $(OBJ)
#	$(CC) $(OBJ) -o $(NAME)
	$(CC) $(OBJ) $(HEADERS) $(LIBS) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

libft:
	make -C $(FTDIR)

.PHONY: clean, all, fclean, re, libft

clean:
	/bin/rm -f *.o
	/bin/rm -f __.SYMDEF
	/bin/rm -rf *.dSYM
	make clean -C $(FTDIR)

fclean: clean
	/bin/rm -f $(NAME)
	make fclean -C $(FTDIR)

re: fclean all
