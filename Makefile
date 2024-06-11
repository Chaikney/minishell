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
NAME = minishell
LIBFT_DIR = ./libft
LIBFT = $(LIBFT_DIR)/libft.a

SRCS = minishell.o paths.o parse.o builtins.o
OBJS = $(SRCS:.c=.o)
INCLUDES = -I $(LIBFT_DIR)
LIBS = -L $(LIBFT_DIR) -lft -lreadline

CC = cc
CFLAGS = -Wall -Wextra -Werror

all: $(NAME)

$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJS) $(INCLUDES) $(LIBS) -o $(NAME)

$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

clean:
	rm -f $(OBJS)
	$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	rm -f $(NAME)
	$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re
NAME	= minishell

SRC		= minishell.c paths.c parse.c builtins.c

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
