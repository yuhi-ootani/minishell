# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: oyuhi <oyuhi@student.42tokyo.jp>           +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/02/11 11:52:00 by otaniyuhi         #+#    #+#              #
#    Updated: 2025/02/27 12:00:46 by oyuhi            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell
CC = gcc
CFLAGS = -Wall -Wextra -Werror 

# Readline library path for macOS
LIBS = -L/usr/local/opt/readline/lib -I/usr/local/opt/readline/include -lreadline
LIBFT = libft/libft.a
LIBFT_REPO = https://github.com/yuhi-ootani/lifbt_all.git

LIBFT_DIR = libft
MAIN_DIR = srcs/main
PROMPT_DIR = srcs/prompt
LEXER_DIR = srcs/lexer
PARSER_DIR = srcs/parser
EXECUTOR_DIR = srcs/executor
INC_DIR = includes
SIGNALS_DIR = srcs/signals
BUILTINS_DIR = srcs/builtins
UTILS_DIR = srcs/utils
EXECUTOR_DIR = srcs/executor



		
SRCS = $(MAIN_DIR)/main.c $(PROMPT_DIR)/prompt.c $(LEXER_DIR)/lexer.c $(LEXER_DIR)/token_word.c $(PARSER_DIR)/parser.c \ 
       $(SIGNALS_DIR)/signals.c $(BUILTINS_DIR)/built_in.c $(BUILTINS_DIR)/echo_pwd_env.c $(UTILS_DIR)/utils.c \
	   $(EXECUTOR)/single_command.c
OBJS = $(SRCS:.c=.o)

all: $(LIBFT) $(NAME) 

$(LIBFT) :
	 @echo "🔍 Checking for libft..."
	 @if [ ! -d "$(LIBFT_DIR)" ]; then \
	 	echo "📥 Cloning libft..."; \
	 	git clone  --quiet $(LIBFT_REPO)  $(LIBFT_DIR); \
	 fi
	 @make -C $(LIBFT_DIR) --silent

$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) -o $(NAME) $(LIBS)


%.o: %.c
	$(CC) $(CFLAGS) -I$(INC_DIR) -c $< -o $@

clean:
	@if [ -d "$(LIBFT_DIR)" ]; then \
		make clean -C $(LIBFT_DIR) --silent; \
	fi
	rm -f $(OBJS)

fclean: clean
	@if [ -d "$(LIBFT_DIR)" ]; then \
		rm -rf $(LIBFT_DIR); \
		echo "🧹 Removing libft directory..."; \
	fi
	rm -f $(NAME)


re: fclean all


.PHONY: all clean fclean re