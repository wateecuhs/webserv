# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: panger <panger@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/03/20 14:09:46 by panger            #+#    #+#              #
#    Updated: 2024/07/11 13:04:03 by panger           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		=	webserv
CC			=	c++

HEADERS		=	

SOURCES		=	main.cpp


OBJ_DIR		=	.build
SRC_DIR		=	src

CPPFLAGS	=	-Wall -Wextra -std=c++98 -MMD -MP

SRCS	=	$(addprefix $(SRC_DIR)/, $(SOURCES))
OBJECTS	=	$(addprefix $(OBJ_DIR)/, $(SRCS:.cpp=.o))
DEPS	=	$(addprefix $(OBJ_DIR)/, $(SOURCES:.cpp=.d))

.DEFAULT_GOAL = all

-include $(DEPS)

all:
	$(MAKE) $(NAME)

$(NAME): $(OBJECTS)
	$(CC) $(CPPFLAGS) $^ -o $@

$(OBJ_DIR)/%.o: %.cpp $(HEADERS)
	mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) -c $< -o $@

re: fclean all

clean:
	$(RM) -rf $(OBJ_DIR)

fclean: clean
	$(RM) $(NAME)

.PHONY: all debug leaks re clean fclean