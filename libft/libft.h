/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chaikney <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/21 11:18:49 by chaikney          #+#    #+#             */
/*   Updated: 2023/05/17 10:14:12 by chaikney         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H
# include <unistd.h>
# include <limits.h>
# include <stdlib.h>
# include <stdarg.h>	// for the variadic parameters in ft_printf

// set default size for get_next_line
# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 100
# endif

// set upper limit on file descriptors
# ifndef RLIMIT_NOFILE
#  define RLIMIT_NOFILE 1048
# endif

// typedef for use with lists and bonus
// NOTE this defines a *node* not a list. Has a pointer to any kind of content,
// and a pointer to another (the next) node.
typedef struct s_list
{
	void			*content;
	struct s_list	*next;
}	t_list;

void	ft_bzero(void *s, size_t n);
void	ft_strtoupper(char *str);
void	ft_putchar_fd(char c, int fd);
void	ft_putnbr_fd(int n, int fd);
void	ft_putstr_fd(char *s, int fd);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
int		ft_hasnewline(char *s);
int		ft_get_digits(int n, int base);
int		ft_powerof10(int n);
int		pf_putchar_fd(char c, int fd);
int		pf_putstr_fd(char *s, int fd);
int		pf_put_signed_fd(int nbr, int fd);
int		pf_put_unsigned_fd(unsigned int nbr, int fd);
int		pf_puthex_fd(unsigned long nbr, char f, int fd);
int		ft_atoi(const char *str);
int		ft_isalnum(int c);
int		ft_isalpha(int c);
int		ft_isascii(int c);
int		ft_isdigit(int c);
int		ft_isprint(int c);
int		ft_tolower(int c);
int		ft_toupper(int c);
int		ft_memcmp(const void *s1, const void *s2, size_t n);
int		ft_printf(const char *fstr, ...);
size_t	ft_strlcat(char *dst, const char *src, size_t dstsize);
size_t	ft_strlcpy(char *dst, const char *src, size_t dstsize);
size_t	ft_strlen(const char *s);
char	*ft_strnstr(const char *haystack, const char *needle, size_t len);
void	ft_putendl_fd(char *s, int fd);
void	*ft_memchr(const void *s, int c, size_t n);
void	*ft_calloc(size_t count, size_t size);
char	*ft_itoa(int n);
char	*ft_strdup(const char *s1);
char	*ft_substr(char const *s, unsigned int start, size_t len);
char	*ft_strjoin(char const *s1, char const *s2);
char	*ft_strtrim(char const *s1, char const *set);
char	**ft_split(char const *s, char c);
void	ft_striteri(char *s, void (*f)(unsigned int, char*));
char	*ft_strmapi(char const *s, char (*f)(unsigned int, char));
void	*ft_memmove(void *dst, const void *src, size_t len);
char	*gnl_strjoin(char *s1, char *s2);
char	*get_next_line(int fd);
char	*ft_reverse_string(char *str, size_t len);
void	*ft_memcpy(void *dst, const void *src, size_t n);
void	*ft_memset(void *b, int c, size_t n);
char	*ft_strchr(const char *s, int c);
char	*ft_strrchr(const char *s, int c);
t_list	*ft_lstlast(t_list *lst);
t_list	*ft_lstnew(void *content);
int		ft_lstsize(t_list *lst);
void	ft_lstadd_front(t_list **lst, t_list *toadd);
void	ft_lstadd_back(t_list **lst, t_list *toadd);
t_list	*ft_lstlast(t_list *lst);
t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void*));
void	ft_lstiter(t_list *lst, void (*f)(void*));
void	ft_lstdelone(t_list *lst, void (*del)(void *));
void	ft_lstclear(t_list **lst, void (*del)(void *));
int		ft_lstcount_str(t_list *lst, char *str);
int		ft_lstcount_num(t_list *lst, int n);
void	ft_lstsnip(t_list **lst, t_list *node);
int		ft_lstsorted(t_list *stack);
long	ft_lstmin(t_list *stack);
long	ft_lstmax(t_list *stack);
int		ft_lstprint(t_list *printme, char delim);
void	ft_lstpush_to(t_list **src, t_list **dest);
void	ft_lstrotate(t_list **stack);
void	ft_lstrotate_rev(t_list **stack);
void	ft_lstswap_top(t_list **stack);
void	ft_stackpop(t_list *stack);
int		ft_stackpeek(t_list *stack);
#endif
