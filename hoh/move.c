#include "move.h"

t_vec   micro_vec(t_vec vec)
{
	double len = vec_len(vec);
	if (len == 0)
	{
		printf("Error : the length of vector is 0\n");
		exit(1);
	}
	vec.x /= 5;
	vec.y /= 5;
	vec.z /= 5;
	return (vec);
}

int key_hook_move(t_vars* vars)
{
	if (vars->changed == 1)
	{
		print_init(*vars);
		vars->changed = 0;
	}
	if (vars->is_trace == 0)
	{
		if (vars->is_move == 13){
			t_vec dir = vec_sub(vars->camera.lookat, vars->camera.origin);
			t_vec new_org = vec_sum(vars->camera.origin, micro_vec(dir));
			t_vec new_lookat = vec_sum(vars->camera.lookat, micro_vec(dir));
			vars->camera = create_camera(new_org, new_lookat, vars->camera.vup, vars->camera.vfov, vars->camera.ratio);
			print_init(*vars);
		}
		else if (vars->is_move == 0){
			t_vec dir = vec_sub(vars->camera.lookat, vars->camera.origin);
			//t_vec a_dir = create_vec(1, vars->camera.origin.y, (-(dir.x + (dir.y * vars->camera.origin.y)) / dir.z));
			t_vec a_dir = vcross(vars->camera.vup, dir);
			t_vec cross = vcross(a_dir, dir);
			if (vdot(cross, create_vec(0,1,0)) > 0)
				vec_scalar_mul(a_dir, -1);
			t_vec new_org = vec_sum(vars->camera.origin, micro_vec(a_dir));
			t_vec new_lookat = vec_sum(vars->camera.lookat, micro_vec(a_dir));
			vars->camera = create_camera(new_org, new_lookat, vars->camera.vup, vars->camera.vfov, vars->camera.ratio);
			print_init(*vars);
		}
		else if (vars->is_move == 1){
			t_vec dir = vec_sub(vars->camera.origin, vars->camera.lookat);
			t_vec new_org = vec_sum(vars->camera.origin, micro_vec(dir));
			t_vec new_lookat = vec_sum(vars->camera.lookat, micro_vec(dir));
			vars->camera = create_camera(new_org, new_lookat, vars->camera.vup, vars->camera.vfov, vars->camera.ratio);
			print_init(*vars);
		}
		else if (vars->is_move == 2){
			t_vec dir = vec_sub(vars->camera.lookat, vars->camera.origin);
			t_vec a_dir = vcross(vars->camera.vup, dir);
			t_vec cross = vcross(a_dir, dir);
			if (vdot(cross, create_vec(0,1,0)) < 0)
				vec_scalar_mul(a_dir, -1);
			t_vec new_org = vec_sub(vars->camera.origin, micro_vec(a_dir));
			t_vec new_lookat = vec_sub(vars->camera.lookat, micro_vec(a_dir));
			vars->camera = create_camera(new_org, new_lookat, vars->camera.vup, vars->camera.vfov, vars->camera.ratio);
			print_init(*vars);
		}
		else if (vars->is_move == 126)
		{
			t_vec new_lookat = create_vec(vars->camera.lookat.x,
			vars->camera.lookat.y * cos(0.5) - 
			vars->camera.lookat.z * sin(0.5),
			vars->camera.lookat.y * sin(0.5) + vars->camera.lookat.z * cos(0.5));
			vars->camera = create_camera(vars->camera.origin, new_lookat, vars->camera.vup, vars->camera.vfov, vars->camera.ratio);
			print_init(*vars);
		}
		else if (vars->is_move == 123)
		{
			t_vec new_lookat = create_vec(vars->camera.lookat.z * sin(0.5) + 
			vars->camera.lookat.x * cos(0.5), 
			vars->camera.lookat.y,
			vars->camera.lookat.z * cos(0.5) - vars->camera.lookat.x * sin(0.5));
			vars->camera = create_camera(vars->camera.origin, new_lookat, vars->camera.vup, vars->camera.vfov, vars->camera.ratio);
			print_init(*vars);
		}
		else if (vars->is_move == 125)
		{
			t_vec new_lookat = create_vec(vars->camera.lookat.x,
			vars->camera.lookat.y * cos(-0.5) - 
			vars->camera.lookat.z * sin(-0.5),
			vars->camera.lookat.y * sin(-0.5) + vars->camera.lookat.z * cos(-0.5));
			vars->camera = create_camera(vars->camera.origin, new_lookat, vars->camera.vup, vars->camera.vfov, vars->camera.ratio);
			print_init(*vars);
		}
		else if (vars->is_move == 124)
		{
			t_vec new_lookat = create_vec(vars->camera.lookat.z * sin(-0.5) + 
			vars->camera.lookat.x * cos(-0.5), 
			vars->camera.lookat.y,
			vars->camera.lookat.z * cos(-0.5) - vars->camera.lookat.x * sin(-0.5));
			vars->camera = create_camera(vars->camera.origin, new_lookat, vars->camera.vup, vars->camera.vfov, vars->camera.ratio);
			print_init(*vars);
		}
		/*else if (vars->is_move == 4 || vars->is_move == 5)
		{
			double new_vfov;

			if (vars->camera.vfov >= 170 || vars->camera.vfov <= 10)
			{
				printf("cannot zoom more\n");
				vars->is_move = -1;
				return (0);
			}
			if (vars->is_move == 4)
				new_vfov = vars->camera.vfov - 10;
			else
				new_vfov = vars->camera.vfov + 10;
			vars->camera = create_camera(vars->camera.origin, vars->camera.lookat, 
			vars->camera.vup, new_vfov, vars->camera.ratio);
			vars->is_move = -1;
			print_init(*vars);
		}*/ // 무한 로딩 걸리는 이유를 찾아라
	}
	return (1);
}

int	keybind(int keycode, t_vars* vars)
{
	//printf("keycode=%d\n", keycode);
	if (keycode == 13)
		key_press_w(vars);
	else if (keycode == 0)
		key_press_a(vars);
	else if (keycode == 1)
		key_press_s(vars);
	else if (keycode == 2)
		key_press_d(vars);
	if (keycode == 126)
		key_press_up(vars);
	else if (keycode == 123)
		key_press_left(vars);
	else if (keycode == 125)
		key_press_down(vars);
	else if (keycode == 124)
		key_press_right(vars);
	else if (keycode == 15)
	{
		if (vars->is_trace == 0)
		{
			vars->is_trace = 1;
			vars->anti = 100;
			vars->changed = 1;
		}
		else
		{
			vars->is_trace = 0;
			vars->anti = 1;
			vars->changed = 1;
		}
	}
	return (0);
}

int	keyrelease(int keycode, t_vars* vars)
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

int scroll(int mousecode, int x, int y, t_vars* vars)
{
	if (vars->is_trace == 1)
		printf("cannot zoom here\n");
	if (mousecode == 4)
		vars->is_move = 4;
	else if (mousecode == 5)
		vars->is_move = 5;
	return (0);
}

void key_press_w(t_vars* vars)
{
	if (vars->is_trace == 1)
		printf("cannot move here\n");
	else
		vars->is_move = 13;
}

void key_press_a(t_vars* vars)
{
	if (vars->is_trace == 1)
		printf("cannot move here\n");
	else
		vars->is_move = 0;
}

void key_press_s(t_vars* vars)
{
	if (vars->is_trace == 1)
		printf("cannot move here\n");
	else
		vars->is_move = 1;
}

void key_press_d(t_vars* vars)
{
	if (vars->is_trace == 1)
		printf("cannot move here\n");
	else
		vars->is_move = 2;
}

void key_press_up(t_vars* vars)
{
	if (vars->is_trace == 1)
		printf("cannot move here\n");
	else
		vars->is_move = 126;
}

void key_press_left(t_vars* vars)
{
	if (vars->is_trace == 1)
		printf("cannot move here\n");
	else
		vars->is_move = 123;
}

void key_press_down(t_vars* vars)
{
	if (vars->is_trace == 1)
		printf("cannot move here\n");
	else
		vars->is_move = 125;
}

void key_press_right(t_vars* vars)
{
	if (vars->is_trace == 1)
		printf("cannot move here\n");
	else
		vars->is_move = 124;
}
