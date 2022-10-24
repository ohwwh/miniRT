/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ohw <ohw@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/21 20:28:58 by hako              #+#    #+#             */
/*   Updated: 2022/10/25 00:10:41 by ohw              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

int	main(int ac, char **av)
{
	t_minirt	data;
	int			fd;

	if (check_file(ac, av))
		err_handler("wrong argument");
	fd = open(av[1], O_RDONLY);
	init_rt(&data);
	parse(&data.scene, fd);
	set_init_distance(&data);
	create_light_object(&data.scene);
	set_camera(&data.scene.camera);
	path_render(&data);
	mlx_hook(data.mlx.mlx_win, 2, 0, &keypress, &data);
	mlx_hook(data.mlx.mlx_win, 3, 0, &keyrelease, &data);
	mlx_hook(data.mlx.mlx_win, 4, 0, &scroll, &data);
	mlx_hook(data.mlx.mlx_win, 17, 0L, ft_close, &data);
	mlx_loop_hook(data.mlx.mlx, &key_hook_move, &data);
	mlx_loop(data.mlx.mlx);
	return (0);
}
