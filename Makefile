# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: alermolo <alermolo@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/03/20 14:09:46 by panger            #+#    #+#              #
#    Updated: 2024/08/26 14:18:23 by alermolo         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		=	webserv
CC			=	c++

HEADERS		=	parsing.hpp								\
				VirtualServer.hpp						\
				enums.hpp								\
				Request.hpp								\
				utils.hpp								\
				Location.hpp							\
				Client.hpp								\
				Server.hpp								\
				Socket.hpp								\
				exceptions.hpp

RAW_SOURCES	=	main.cpp								\
				Request.cpp								\
				VirtualServer.cpp						\
				parsing/conf_utils.cpp					\
				exceptions.cpp							\
				Socket.cpp								\
				Client.cpp								\
				Server.cpp								\
				Location.cpp							\
				utils.cpp



OBJ_DIR		=	.build
SRC_DIR		=	src/
INC_DIR		=	includes/

CPPFLAGS	=	-Wall -Wextra -std=c++98 -MMD -MP -g3 -Werror #-fsanitize=address,undefined -D_GLIBCXX_DEBUG

SRCS		=	$(addprefix $(SRC_DIR), $(RAW_SOURCES))
INCLUDES	=	$(addprefix $(INC_DIR), $(HEADERS))
OBJECTS		=	$(addprefix $(OBJ_DIR)/, $(RAW_SOURCES:.cpp=.o))
DEPS		=	$(addprefix $(OBJ_DIR)/, $(RAW_SOURCES:.cpp=.d))

.DEFAULT_GOAL = all

-include $(DEPS)

all:
	$(MAKE) $(NAME)

$(NAME): $(OBJECTS)
	$(CC) $(CPPFLAGS) $^ -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)%.cpp
	mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS)  -c $< -I $(INC_DIR) -o $@

re: fclean all

clean:
	$(RM) -rf $(OBJ_DIR)

fclean: clean
	$(RM) $(NAME)

.PHONY: all debug leaks re clean fclean
