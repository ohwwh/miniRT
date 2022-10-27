NAME=miniRT

CC = gcc
CFLAGS = -Wall -Wextra -Werror
FRAMEW = -Lmlx -lmlx -framework OpenGL -framework Appkit

SRCS = main.c ./parse/parse.c ./parse/parse_objs.c ./utils/utils.c ./parse/parse_equip.c \
		./render/render.c ./vector/vector.c ./ray/ray.c ./utils/color.c \
		./keybind/move.c ./hit/hit.c ./render/random.c ./utils/camera.c ./render/objects.c \
		./vector/vector2.c ./vector/vector3.c ./parse/parse_utils.c \
		./hit/hit_sphere.c ./hit/hit_plane.c ./hit/hit_cylinder.c ./hit/hit_rectangle.c \
		./render/path_render.c ./keybind/move_camera.c ./keybind/move_object.c \
		./keybind/keypress.c ./utils/init.c ./ray/ray_create.c ./ray/ray_utils.c ./utils/color2.c \
		./render/pdf.c ./render/pdf2.c ./render/scatter.c ./render/thread.c
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