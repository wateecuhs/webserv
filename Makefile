# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: alermolo <alermolo@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/03/20 14:09:46 by panger            #+#    #+#              #
#    Updated: 2024/08/02 17:18:26 by alermolo         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		=	webserv
CC			=	c++

HEADERS		=	parsing.hpp								\
				Socket.hpp								\
				enums.hpp								\
				Request.hpp								\
				utils.hpp								\
				Location.hpp							\
				exceptions.hpp

RAW_SOURCES	=	main.cpp								\
				Request.cpp								\
				Socket.cpp								\
				parsing/conf_parsing.cpp				\
				parsing/conf_utils.cpp					\
				parsing/req_parsing_reqline.cpp			\
				parsing/req_parsing_headers.cpp			\
				method_handler.cpp						\
				Location.cpp							\
				utils.cpp								\
				cgi_handler.cpp							\
				exceptions.cpp							\
				utils.cpp


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
