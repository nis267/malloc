# **************************************************************************** #
#                                                           LE - /             #
#                                                               /              #
#    Makefile                                         .::    .:/ .      .::    #
#                                                  +:+:+   +:    +:  +:+:+     #
#    By: dewalter <dewalter@student.le-101.fr>      +:+   +:    +:    +:+      #
#                                                  #+#   #+    #+    #+#       #
#    Created: 2020/01/17 13:06:25 by dewalter     #+#   ##    ##    #+#        #
#    Updated: 2020/02/09 14:36:53 by dewalter    ###    #+. /#+    ###.fr      #
#                                                          /                   #
#                                                         /                    #
# **************************************************************************** #

ifeq ($(HOSTTYPE),)
	HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

# ifeq ($(BONUS),)
# 	BONUS := 0
# endif


# ifeq ($(shell uname), Linux)
# TIME := /usr/bin/time -v
# TEST_SH := test/run_linux.sh
# else
# TIME := /usr/bin/time -l
# TEST_SH := test/run_mac.sh
# endif


HNAME := libft_malloc_$(HOSTTYPE).so
NAME := libft_malloc.so

DIR_LIB = ./libft/
DIR_OBJ = ./obj/
DIR_SRC = ./src/
DIR_INC = ./include/
DIR_INC2 = -I ./include/ -I ./libft/include

OBJ :=	obj/realloc.o \
		obj/malloc.o \
		obj/free.o   \
		obj/realloc.o \
		obj/calloc.o \
		obj/show_alloc_mem.o

FLAGS := -Wall -Wextra -Werror -g -fPIC #-DBONUS=$(BONUS) -rdynamic

all: $(NAME) lib

lib:
		@if !(make -q -C $(DIR_LIB)); then \
			rm -f $(OBJ); \
			rm -f $(NAME); \
			make -C $(DIR_LIB);\
		fi;

$(NAME): $(HNAME)
	rm -f $@
	ln -s $< $@

$(HNAME): $(OBJ)
	@make -C $(DIR_LIB)
	gcc $(FLAGS) -L $(DIR_LIB) -lft -shared -o $@ $^

$(DIR_OBJ)%.o: $(DIR_SRC)%.c $(DIR_INC)malloc.h Makefile
	@mkdir -p obj
	@gcc -o $@ -c $< $(DIR_INC2) $(FLAGS)

clean:
	rm -rf obj
	@make clean -C $(DIR_LIB)

fclean: clean #clean_tests
	rm -f $(NAME) $(HNAME)
	@make fclean -C $(DIR_LIB)

re: fclean all