# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: panger <panger@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/03/20 14:09:46 by panger            #+#    #+#              #
#    Updated: 2024/07/29 16:06:41 by panger           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		=	webserv
CC			=	c++

HEADERS		=	parsing.hpp								\
				Server.hpp								\
				enums.hpp								\
				Request.hpp

RAW_SOURCES	=	main.cpp								\
				Request.cpp								\
				parsing/req_parsing_reqline.cpp			\
				parsing/req_parsing_headers.cpp			\
				parsing/conf_parsing.cpp				\
				method_handler.cpp


OBJ_DIR		=	.build
SRC_DIR		=	src/
INC_DIR		=	includes/

CPPFLAGS	=	-Wall -Wextra -std=c++98 -MMD -MP -g3 #-Werror

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
