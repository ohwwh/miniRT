# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ohw <ohw@student.42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/10/23 02:36:10 by ohw               #+#    #+#              #
#    Updated: 2022/10/27 15:28:10 by ohw              ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC=gcc
CFLAGS=
NAME=miniRT
NAME_BONUS=miniRT_bonus
MLXDIR=../mlx
LIBDIR=../libohw
LIBINCSDIR=./libohw/includes
SRCSDIR=./
INCSDIR=./
SRCSBNSDIR=./
SRCS_NAME=main.c vector.c ft_mlx.c ray.c objects.c random.c camera.c move.c hit.c
SRCS=$(addprefix $(SRCSDIR), $(SRCS_NAME))

BONUS_NAME=${SRCS_NAME:.c=_bonus.c}

BONUS=$(addprefix $(SRCSBNSDIR), $(BONUS_NAME))

OBJS=${SRCS:.c=.o}

OBJSBNS=${BONUS:.c=.o}

.PHONY: all bonus clean fclean re

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@ -I$(INCSDIR) -I$(MLXDIR)

all : $(NAME)

bonus : $(NAME_BONUS)

$(NAME) : $(OBJS)
	make -C $(MLXDIR)
	make -C $(LIBDIR)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS) -L$(MLXDIR) -L$(LIBDIR) -I$(INCSDIR) -lmlx -lohw -framework OpenGL -framework Appkit

$(NAME_BONUS) : $(OBJSBNS)
	make -C $(MLXDIR)
	make -C $(LIBDIR)
	$(CC) $(CFLAGS) -o $(NAME_BONUS) $(OBJSBNS) -L$(MLXDIR) -L$(LIBDIR) -I$(INCSDIR) -lmlx -lohw -framework OpenGL -framework Appkit

clean:
	# make -C $(MLXDIR) clean
	# make -C $(LIBDIR) clean
	rm -f $(OBJS) $(OBJSBNS)

fclean: clean
	# make -C $(LIBDIR) fclean
	rm -f $(NAME) $(NAME_BONUS)

re: fclean all
