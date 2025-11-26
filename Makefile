# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ncarrera <ncarrera@student.42madrid.com    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/11/11 13:06:14 by ncarrera          #+#    #+#              #
#    Updated: 2025/11/26 02:50:23 by ncarrera         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Compiler settings and flags
NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror -I./headers

# Linker flags
LDFLAGS = -lreadline -L./libft -lft

# Libft
LIBFT = libft/libft.a

# Project sources
SRCS = srcs/main.c srcs/parser.c srcs/executor.c srcs/builtins.c \
	   srcs/parser_utils.c srcs/executor_utils.c srcs/parser_nodes.c \
	   srcs/tokenizer.c srcs/executor_redirs.c srcs/path_utils.c
OBJS = $(SRCS:.c=.o)

# Colours
NC=\033[0m
Purple=\033[0;35m
Cyan=\033[1;36m
Red=\033[0;31m
LC=\033[2K


# Make rules
all: $(NAME)

$(NAME): $(LIBFT) $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) $(LDFLAGS) -o $(NAME)

$(LIBFT):
	@make -C libft -s

%.o: %.c
	@printf "$(Cyan)Compiling: $(Purple)$<$(NC)\n"
	@$(CC) $(CFLAGS) -c $(LDFLAGS) $< -o $@
clean:
	@printf "$(Red)Deleting: $(Purple)$(OBJS)$(NC)\n"
	@rm -f $(OBJS)
	@make clean -C libft -s

fclean: clean
	@printf "$(Red)Deleting: $(Purple)$(NAME)$(NC)\n"
	@rm -f $(NAME)
	@make fclean -C libft -s
re: fclean all
reclean:
	@$(MAKE) re -s
	@$(MAKE) clean -s

.PHONY: all clean fclean re reclean
