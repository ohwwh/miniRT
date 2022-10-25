NAME=miniRT

CC = gcc
CFLAGS = -Wall -Wextra -Werror
FRAMEW = -Lmlx -lmlx -framework OpenGL -framework Appkit

SRCS = main.c parse.c parse_objs.c utils.c parse_equip.c \
		render.c vector.c ray.c color.c \
		move.c hit.c random.c camera.c objects.c \
		vector2.c vector3.c parse_utils.c \
		hit_sphere.c hit_plane.c hit_cylinder.c \
		path_render.c move_camera.c move_object.c \
		keypress.c init.c ray_create.c ray_utils.c color2.c \
		pdf.c pdf2.c scatter.c
INCS = ./includes/
OBJS = $(SRCS:.c=.o)

all : $(NAME)

$(NAME):		$(OBJS)
				make -C ./libft
				make -C ./mlx
				cp ./libft/libft.a libft.a
				cp ./mlx/libmlx.a ./libmlx.a
				$(CC) -g3 $(OBJS) libft.a -I$(INCS) -o $(NAME) $(FRAMEW)

%.o:			%.c
				$(CC) -I $(INCS) -c $< -o $@

clean :
	make --no-print-directory clean -C ./libft
	rm -f $(OBJS)

fclean :
	make --no-print-directory fclean -C ./libft
	make clean -C ./mlx
	rm -f $(OBJS)
	rm -f $(NAME)
	rm -f libmlx.a
	rm -f libft.a

re :
	$(MAKE) fclean
	$(MAKE) all

.PHONY : all clean fclean re