# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>
# include <fcntl.h>
# include <math.h>

#include "../mlx/mlx.h"
#include "../libft/libft.h"

# define HIGHT 600
# define WIDTH 900
# define ESC 53

# define CY 1
# define PL 2
# define SP 3

typedef struct	s_mlx
{
	void	*mlx;
	void	*mlx_win;
	void	*img;
	char	*addr;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
}				t_mlx;

typedef struct s_vec
{
	double	x;
	double	y;
	double	z;
}	t_vec;

typedef struct t_cam
{
	t_vec	cen;
	t_vec	dir;
	double	fov;
	int		count;
}	t_cam;

typedef struct s_light
{
	t_vec			src;
	double			ratio;
	struct s_light	*next;
}	t_light;

typedef struct s_amb
{
	t_vec	col;
	double	ratio;
	int 	count;
}	t_amb;

typedef struct s_objs
{
	int				type;
	t_vec			cen;
	t_vec			dir;
	t_vec			p;
	t_vec			col;
	t_vec			norm;
	struct s_objs	*next;
}	t_objs;

typedef struct s_scene
{
	t_vec	col;
	t_cam	cam;
	t_light	*light;
	t_amb	amb;
	t_objs	*objs;
}	t_scene;

typedef struct	s_minirt
{
	t_mlx		mlx;
	t_scene		scene;
}               t_minirt;

int		check_file(int ac, char **av);
void 	err_handler(char *msg);

t_vec	get_color(char *s);
t_vec	get_vec(char *s);
void	parse_line(char *id, char **tockens, t_scene *sc);
void	parse(t_scene *sc, int fd);

void	parse_sphere(t_scene *sc, char **tockens);
void	parse_cylinder(t_scene *sc, char **tockens);
void	parse_plane(t_scene *sc, char **tockens);

t_light	*alloc_light(t_scene *sc);
void	parse_ambient(t_scene *sc, char **tokens);
void	parse_camera(t_scene *sc, char **tokens);
void	parse_light(t_scene *sc, char **tokens);

double	ft_atod(const char *str);
void	free_split(char **s);

void print_scene(t_scene sc);

void	rt_render(t_minirt *data);