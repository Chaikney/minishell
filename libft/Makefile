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

NAME = libft.a

SRC = ft_atoi.c ft_isalnum.c ft_isalpha.c ft_isascii.c ft_isdigit.c \
			ft_isprint.c ft_putchar_fd.c ft_putstr_fd.c ft_strlcat.c \
			ft_strlcpy.c ft_strlen.c ft_strncmp.c ft_strnstr.c ft_tolower.c \
			ft_toupper.c ft_putnbr_fd.c ft_bzero.c ft_strchr.c ft_strrchr.c \
			ft_putendl_fd.c ft_memset.c ft_memcpy.c ft_memcmp.c ft_memchr.c \
			ft_calloc.c ft_strdup.c ft_substr.c ft_strjoin.c ft_striteri.c \
			ft_strmapi.c ft_memmove.c ft_itoa.c ft_putnbr_fd.c ft_strtrim.c \
			ft_split.c get_next_line.c get_next_line_utils.c ft_hasnewline.c \
			ft_get_digits.c ft_powerof10.c ft_strtoupper.c ft_reverse_string.c \
			pf_putchar_fd.c pf_putstr_fd.c pf_puthex_fd.c pf_putsigned_fd.c  \
			ft_printf.c pf_putunsigned_fd.c ft_lstmap.c ft_lstlast.c \
			ft_lstnew.c ft_lstlast.c ft_lstnew.c ft_lstsize.c ft_lstadd_front.c \
			ft_lstadd_back.c ft_lstdelone.c ft_lstclear.c ft_lstiter.c \
			ft_lstcount_str.c ft_lstcount_num.c ft_lstsnip.c ft_lstmin.c ft_lstmax.c \
			ft_lstsorted.c ft_lstprint.c ft_stackpop.c ft_stackpeek.c ft_lstswap_top.c \
			ft_lstrotate.c ft_lstrotate_rev.c ft_lstpush_to.c

CFLAGS = -Werror -Wall -Wextra
CC = gcc
NAME = libft.a

OBJ = $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)
	ar rcs $(NAME) $(OBJ)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean, all, fclean, test, re

test: fclean
	$(CC) $(CFLAGS) -c *.c

clean:
	/bin/rm -f $(OBJ)

fclean: clean
	/bin/rm -f $(NAME)

re: fclean all
