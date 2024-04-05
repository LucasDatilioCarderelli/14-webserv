NAME	=	webServer

INCLUDE	=	-I ./includes

MKDIR_P	=	mkdir -p
CC		=	c++ -Wall -Wextra -Werror -O3 -std=c++98
CC      +=	-pedantic-errors -Wshadow -Wno-shadow

FILES	=	main.cpp

SRC_DIR	=	sources/
OBJ_DIR	=	build/

SRC		=	$(addprefix $(SRC_DIR), $(FILES))
OBJ		=	$(addprefix $(OBJ_DIR), $(FILES:%.cpp=%.o))

ITALIC_LIGHTER_YELLOW	= "\e[1;3;33m"
ITALIC_BOLD_GREEN		= "\e[0;3;92m"
RESET_COLOR				= "\e[0m"

ifndef ECHO
	T := $(shell $(MAKE) $(MAKECMDGOALS) -n ECHO="counter" | grep -c "counter")
endif

ifeq ($(firstword $(MAKECMDGOALS)), $(shell echo re))
	T := $(shell expr $(words $(FILES)) + 1)
endif

N := x
C = $(words $(N))$(eval N := x $(N))
P = "`expr $(C) '*' 100 / $(T)`"
ECHO = echo $(ITALIC_BOLD_GREEN)"[$(P)%]\t"

all:		$(NAME)

$(NAME):	$(OBJ_DIR) $(OBJ)
	@$(CC) $(OBJ) $(INCLUDE) -o $(NAME)
	@$(ECHO) $(NAME) created! $(RESET_COLOR)

$(OBJ_DIR)%.o:	$(SRC_DIR)%.cpp
	@$(ECHO) Compiling $< $(RESET_COLOR)
	@$(CC) $(INCLUDE) -c $< -o $@

$(OBJ_DIR):
	@$(MKDIR_P) $(OBJ_DIR)

clean:
	@rm -fr $(OBJ_DIR)
	@echo $(ITALIC_LIGHTER_YELLOW)"Objects deleted"$(RESET_COLOR)

fclean:		clean
	@rm -fr $(NAME)
	@echo $(ITALIC_LIGHTER_YELLOW)$(NAME)" deleted"$(RESET_COLOR)

re:			fclean all

run:		all
	@./$(NAME)

.PHONY: all, clean, fclean, re, run