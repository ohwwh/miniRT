#include "minirt.h"

void	thread_init(t_minirt *vars)
{
	for (int i=0; i<TH; i++)
	{
		vars->thr[i].thr_num = i;
		vars->thr[0].sh->work[i] = 0;
		vars->thr[i].color = create_vec(0, 0, 0);
	}
	vars->thr[0].sh->x = 0;
	vars->thr[0].sh->y = HEIGHT - 2;
	pthread_mutex_init(&(vars->thr[0].sh->mutex), 0);
	vars->thr[0].sh->sampling = 0;
	vars->thr[0].sh->working = TH;
	vars->thr[0].sh->vars = vars;
}

t_color color_sum(t_minirt *vars)
{
	t_color ret;

	ret = create_vec(0, 0, 0);
	for (int i = 0; i < TH; i ++)
	{
		ret = vec_sum(ret, vars->thr[i].color);
	}

	return (ret);
}

int equal(int work[], int idx)
{
	for (int i=0; i<TH; i++)
	{
		if (idx != i && work[idx] - work[i] > 0)
			return (0);
	}
	return (1);
}

void *routine(void *data)
{
	t_thread	*pdata;
	t_ray		init_ray;
	double		u;
	double		v;
	int 		x;
	int			y;
	int			s;
	
	//srand(time(0));
	pdata = (t_thread *)data;
	while (1)
	{
		while (1)
		{
			pthread_mutex_lock(&(pdata->sh->mutex));
			//printf("thr_num[%d], working: %d\n", pdata->thr_num, pdata->sh->working);
			if (pdata->sh->working != 0 && pdata->sh->sampling == 0 && equal(pdata->sh->work, pdata->thr_num) == 1)
			{
				//printf("thr_num[%d]: i'm break, papa!\n", pdata->thr_num);
				x = pdata->sh->x;
				y = pdata->sh->y;
				//pdata->sh->working ++;
				pthread_mutex_unlock(&(pdata->sh->mutex));
				break ;
			}
			pthread_mutex_unlock(&(pdata->sh->mutex));
		}
		s = 0;
		while (s ++ < pdata->sh->vars->scene.anti / TH)
		{
			u = (((double)x + random_double(0, 1, pdata->sh->vars->scene.anti)) * 2 / WIDTH) - 1;
			v = (((double)y + random_double(0, 1, pdata->sh->vars->scene.anti)) * 2 / HEIGHT) - 1;
			//pthread_mutex_lock(&(pdata->sh->mutex));
			init_ray = ray_primary(&(pdata->sh->vars->scene.camera), u, v);
			if (pdata->sh->vars->is_trace == 1)
				pdata->color = vec_sum(pdata->color,
						ray_color(init_ray, &pdata->sh->vars->scene, MAX_DEPTH));
			else
				pdata->color = vec_sum(pdata->color,
						ray_color_raw(init_ray, &pdata->sh->vars->scene));
			//pthread_mutex_unlock(&(pdata->sh->mutex));
			//printf("%lf, %lf, %lf\n", pdata->color.x, pdata->color.y, pdata->color.z);
		}
		pthread_mutex_lock(&(pdata->sh->mutex));
		if (pdata->sh->working > 0)
			pdata->sh->working --;
		pdata->sh->work[pdata->thr_num] ++;
		//sleep(1);
		//	printf("thr_num[%d]: %lf, %lf, %lf, working: %d\n", pdata->thr_num, pdata->color.x, pdata->color.y, pdata->color.z, pdata->sh->work[pdata->thr_num]);
		pthread_mutex_unlock(&(pdata->sh->mutex));
		if (x == WIDTH - 1 && y == 0)
			return (0);
	}
}

void	path_render_threaded(t_minirt *vars)
{
	int	x;
	int	y;
	int	s;
	int	i = 0;
	time_t		start, end;
	double		result;

	start = time(NULL);
	y = HEIGHT - 1;
	thread_init(vars);
	while (i < TH)
	{
		if (pthread_create(&((vars->thr[i].thr)), 0, routine, (void *)(&(vars->thr[i]))))
		{
			printf("thread create error!\n");
			return ;
		}
		pthread_detach(((vars->thr)[i].thr));
		i ++;
	}
	while (--y >= 0)
	{
		x = 0;
		if (vars->is_trace == 1)
		{
			printf("\rScanlines remaining: %d", y);
			fflush(stdout);
		}
		while (++x < WIDTH)
		{
			while (1)
			{
				pthread_mutex_lock(&(vars->thr[0].sh->mutex));
				if (vars->thr[0].sh->working == 0)
				{
					//printf("i'm' break, son\n");
					vars->thr[0].sh->sampling = 1;
					vars->thr[0].sh->x = x;
					vars->thr[0].sh->y = y;
					break ;
				}
				pthread_mutex_unlock(&(vars->thr[0].sh->mutex));
			}
			vars->ray.color = vec_division(color_sum(vars), vars->scene.anti);
			//sleep(1);
				//printf("%lf, %lf, %lf\n", vars->thr[0].color.x, vars->thr[0].color.y, vars->thr[0].color.z);
				//printf("%lf, %lf, %lf\n", vars->thr[1].color.x, vars->thr[1].color.y, vars->thr[1].color.z);
				//printf("%lf, %lf, %lf\n", vars->thr[2].color.x, vars->thr[2].color.y, vars->thr[2].color.z);
				//printf("%lf, %lf, %lf", vars->ray.color.x, vars->ray.color.y, vars->ray.color.z);
			pthread_mutex_unlock(&(vars->thr[0].sh->mutex));
			put_color(&vars->mlx, x - 2,
				HEIGHT - 1 - y, rgb_to_int(vars->ray.color));
			pthread_mutex_lock(&(vars->thr[0].sh->mutex));
			//printf("\n");
			for (int k = 0; k < TH; k ++)
				vars->thr[k].color = create_vec(0, 0, 0);
			vars->thr[0].sh->working = TH;
			vars->thr[0].sh->sampling = 0;
			pthread_mutex_unlock(&(vars->thr[0].sh->mutex));
		}
	}
	mlx_put_image_to_window(vars->mlx.mlx, vars->mlx.mlx_win, vars->mlx.img, 0, 0);
	end = time(NULL);
	result = (double)(end - start);
	printf("\n%f\n", result);
	pthread_mutex_destroy(&(vars->thr[0].sh->mutex));
}