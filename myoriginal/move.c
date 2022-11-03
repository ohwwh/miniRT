#include "miniRT.h"

t_vec rotate(t_vec axis, t_minirt* vars, int dir)
{
	double c = (1 - cos(dir * 0.1));
	double s = sin(dir * 0.1);
	double x = axis.x;
	double y = axis.y;
	double z = axis.z;

	double i = vars->scene.camera.forward.x;
	double j = vars->scene.camera.forward.y;
	double k = vars->scene.camera.forward.z;

	t_vec new_dir;

	new_dir.x = -i*c*y*y-k*s*y+c*j*x*y-i*c*z*z+j*s*z+c*k*x*z+i;
	new_dir.y = j-c*j*x*x+k*s*x+i*c*x*y-c*j*z*z-i*s*z+c*k*y*z;
	new_dir.z = k-c*k*x*x-j*s*x-c*k*y*y+i*s*y+i*c*x*z+c*j*y*z;

	return (new_dir);
}

void camera_move(t_minirt* vars)
{
	t_vec dir;
	t_vec delta;
	double d;

	if (vars->is_move == 13 || vars->is_move == 1)
	{
		dir = vars->scene.camera.forward;
		if (vars->is_move == 13)
			d = 1;
		else
			d = -1;
	}
	else if (vars->is_move == 0 || vars->is_move == 2)
	{
		dir = vars->scene.camera.right;
		if (vars->is_move == 2)
			d = 1;
		else
			d = -1;
	}
	else
		return ;
	delta = vec_scalar_mul(vec_scalar_mul(dir, d), vars->scene.camera.distance / 500);
	t_vec new_org = vec_sum(vars->scene.camera.origin, delta);
	vars->scene.camera.origin = new_org;
}

void camera_rotate(t_minirt* vars)
{
	t_vec axis;
	t_vec delta;
	double d;

	if (vars->is_move == 126 || vars->is_move == 125)
	{
		axis = vars->scene.camera.right;
		if (vars->is_move == 126)
			d = -1;
		else
			d = 1;
	}
	else if (vars->is_move == 123 || vars->is_move == 124)
	{
		axis = vars->scene.camera.up;
		if (vars->is_move == 124)
			d = 1;
		else
			d = -1;
	}
	else
		return ;
	t_vec new_dir = rotate(axis, vars, d);
	vars->scene.camera.forward = new_dir;
	vars->scene.camera.dir = new_dir;
}

void camera_zoom(t_minirt* vars)
{
	double new_fov;

	if (vars->is_move == 4 || vars->is_move == 5)
	{
		if ((vars->is_move == 4 && vars->scene.camera.fov <= 10)
			|| (vars->is_move == 5 && vars->scene.camera.fov >= 170))
		{
			printf("cannot zoom more\n");
			vars->is_move = -1;
			return ;
		}
		if (vars->is_move == 4)
			new_fov = vars->scene.camera.fov - 10;
		else
			new_fov = vars->scene.camera.fov + 10;
		vars->scene.camera.fov = new_fov;
		vars->is_move = -1;
	}
}

int key_hook_move(t_minirt* vars)
{
	if (vars->scene.changed == 1)
	{
		path_render(*vars);
		vars->scene.changed = 0;
	}
	if (vars->is_trace == 0 && vars->is_move != -1)
	{
		camera_move(vars);
		camera_rotate(vars);
		camera_zoom(vars);
		set_camera(&vars->scene.camera);
		path_render(*vars);
	}
	return (1);
}

int	keypress(int keycode, t_minirt* vars)
{
	//printf("keycode=%d\n", keycode);
	if (keycode == 13 || keycode == 0 || keycode == 1 || keycode == 2)
		key_press_move(vars, keycode);
	else if (keycode == 126 || keycode == 123 || keycode == 124 || keycode == 125)
		key_press_rotate(vars, keycode);
	else if (keycode == 15)
		key_press_mode_change(vars, keycode);
	return (0);
}

int	keyrelease(int keycode, t_minirt* vars)
{
	//printf("key release=%d\n", keycode);
	if (keycode == 13)
		vars->is_move = -1;
	else if (keycode == 0)
		vars->is_move = -1;
	else if (keycode == 1)
		vars->is_move = -1;
	else if (keycode == 2)
		vars->is_move = -1;
	else if (keycode == 126)
		vars->is_move = -1;
	else if (keycode == 123)
		vars->is_move = -1;
	else if (keycode == 125)
		vars->is_move = -1;
	else if (keycode == 124)
		vars->is_move = -1;
	return (0);
}

int scroll(int mousecode, int x, int y, t_minirt* vars)
{
	if (vars->is_trace == 1)
		printf("cannot zoom here\n");
	else if (mousecode == 4 || mousecode == 5)
		vars->is_move = mousecode;
}

void key_press_move(t_minirt* vars, int keycode)
{
	if (vars->is_trace == 1)
		printf("cannot move here\n");
	else
		vars->is_move = keycode;
}

void key_press_rotate(t_minirt* vars, int keycode)
{
	if (vars->is_trace == 1)
		printf("cannot rotate here\n");
	else
		vars->is_move = keycode;
}

void key_press_mode_change(t_minirt* vars, int keycode)
{
	if (vars->is_trace == 0)
	{
		vars->is_trace = 1;
		vars->scene.anti = 100;
		vars->scene.changed = 1;
	}
	else
	{
		vars->is_trace = 0;
		vars->scene.anti = 1;
		vars->scene.changed = 1;
	}
}