# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: panger <panger@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/03/20 14:09:46 by panger            #+#    #+#              #
#    Updated: 2024/07/22 10:57:50 by panger           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		=	webserv
CC			=	c++

HEADERS		=	parsing.hpp								\
				Server.hpp

RAW_SOURCES	=	main.cpp								\
				parsing/parsing.cpp


OBJ_DIR		=	.build
SRC_DIR		=	src/
INC_DIR		=	includes/

CPPFLAGS	=	-Wall -Wextra -Werror -std=c++98 -MMD -MP

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