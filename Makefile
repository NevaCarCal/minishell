# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ncarrera <ncarrera@student.42madrid.com    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/11/11 13:06:14 by ncarrera          #+#    #+#              #
#    Updated: 2026/02/10 20:48:21 by ncarrera         ###   ########.fr        #
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
SRCS = srcs/main.c \
	   srcs/builtin/builtins_dirs.c srcs/builtin/builtins_envs.c \
	   srcs/builtin/builtins_export.c srcs/builtin/builtins_rem_ex.c \
	   srcs/builtin/path_utils.c srcs/parsing/ft_parsing.c \
	   srcs/parsing/parser_nodes.c srcs/parsing/parser_utils.c \
	   srcs/parsing/parser.c srcs/parsing/redir_handler.c \
	   srcs/execution/executor_pipe_wait.c srcs/execution/executor_redirs.c \
	   srcs/execution/executor_utils.c srcs/execution/executor.c \
	   srcs/shell/shell.c srcs/tokenizer.c
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
	@$(CC) $(CFLAGS) -c $< -o $@
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
