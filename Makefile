# **************************************************************************** #
#                                                           LE - /             #
#                                                               /              #
#    Makefile                                         .::    .:/ .      .::    #
#                                                  +:+:+   +:    +:  +:+:+     #
#    By: dewalter <dewalter@student.le-101.fr>      +:+   +:    +:    +:+      #
#                                                  #+#   #+    #+    #+#       #
#    Created: 2020/01/17 13:06:25 by dewalter     #+#   ##    ##    #+#        #
#    Updated: 2020/01/17 14:45:38 by dewalter    ###    #+. /#+    ###.fr      #
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

OBJ :=	obj/malloc.o

FLAGS := -Wall -Wextra -Werror -g -fPIC #-DBONUS=$(BONUS) -rdynamic

all: $(NAME)

$(NAME): $(HNAME)
	rm -f $@
	ln -s $< $@

$(HNAME): $(OBJ)
	gcc $(FLAGS) -shared -o $@ $^

obj/%.o: src/%.c include/malloc.h Makefile
	@mkdir -p obj
	gcc $(FLAGS) -I inc -o $@ -c $<

clean:
	rm -rf obj

fclean: clean clean_tests
	rm -f $(NAME) $(HNAME)

re: fclean all