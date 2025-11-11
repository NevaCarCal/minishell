# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ncarrera <ncarrera@student.42madrid.com    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/11/11 13:06:14 by ncarrera          #+#    #+#              #
#    Updated: 2025/11/11 14:32:21 by ncarrera         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Compiler settings and flags
NAME = minishell
CC = cc
CFLAGS = -I./headers -I./libft -Wall -Werror -Wextra

# Linker flags
LDFLAGS = -L./libft -lft

# Project sources
SRCS = ./srcs/main.c
OBJS = $(SRCS:.c=.o)

LIBFT_DIR = ./libft
LIBFT_A = $(LIBFT_DIR)/libft.a

# Colours
NC=\033[0m
Purple=\033[0;35m
Cyan=\033[1;36m
Red=\033[0;31m
LC=\033[2K

# Make rules
all: $(LIBFT_A) $(NAME)

$(NAME): $(OBJS)
	@$(CC) $(OBJS) $(CFLAGS) $(LDFLAGS) -o $(NAME)

$(LIBFT_A):
	@printf "$(Cyan)Compiling: $(Purple)libft.a$(NC)\n"
	@make -C $(LIBFT_DIR) -s

%.o: %.c
	@printf "$(Cyan)Compiling: $(Purple)$(notdir $<)$(NC)\n"
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@printf "$(Red)Deleting: $(Purple)Minishell objs$(NC)\n"
	@rm -f $(OBJS)
	@make clean -C $(LIBFT_DIR) -s
fclean: clean
	@printf "$(Red)Deleting: $(Purple) Minishell$(NC)\n"
	@rm -f $(NAME)
	@make fclean -C $(LIBFT_DIR) -s
re: fclean all
reclean:
	@$(MAKE) fclean -s
	@$(MAKE) all -s
	@$(MAKE) clean -s

.PHONY: all clean fclean re reclean