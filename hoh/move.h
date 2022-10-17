#ifndef MOVE_H
#define MOVE_H
#include "ft_mlx.h"
#include "miniRT.h"

int	keybind(int keycode, t_vars* vars);
int	keyrelease(int keycode, t_vars* vars);
int scroll(int mousecode, int x, int y, t_vars* vars);
int key_hook_move(t_vars* vars);
void key_press_w(t_vars* vars);
void key_press_a(t_vars* vars);
void key_press_s(t_vars* vars);
void key_press_d(t_vars* vars);
void key_press_up(t_vars* vars);
void key_press_left(t_vars* vars);
void key_press_down(t_vars* vars);
void key_press_right(t_vars* vars);

#endif