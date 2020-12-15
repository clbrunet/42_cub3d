/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   col.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clbrunet <clbrunet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/11 17:59:21 by clbrunet          #+#    #+#             */
/*   Updated: 2020/12/13 18:22:28 by clbrunet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "draw.h"

void	draw_wall(unsigned const col, t_hit const *hit,
		t_texture const *texture, t_vars const *v)
{
	unsigned	texture_col;
	t_color		color;
	int			y;
	unsigned	i;

	texture_col = (double)hit->offset / ((double)BLOCK_SIZE / texture->width);
	y = v->player.height - hit->height / 2;
	i = 0;
	while (i < hit->height)
	{
		if (y >= (signed)v->res.y)
			break ;
		if (y >= 0)
		{
			color.full = mlx_img_pixel_get_value(&texture->img, texture_col,
					i * texture->height / hit->height);
			color = fog(color, hit->distance);
			(*v->pixel_put_ft)(v, col, y, color);
		}
		y++;
		i++;
	}
}

static void	get_sprite_data(unsigned *texture_col, t_texture *texture,
		t_hit const *hit, t_vars const *v)
{

	if (hit->map_char == 'h')
		*texture = v->textures.regeneration;
	else
		*texture = v->textures.monster;
	*texture_col = (double)hit->offset / ((double)BLOCK_SIZE / texture->width);
}

void	draw_sprite(unsigned const col, t_hit const *hit, t_vars const *v)
{
	unsigned	texture_col;
	t_color		color;
	int			y;
	unsigned	i;
	t_texture	texture;

	get_sprite_data(&texture_col, &texture, hit, v);
	y = v->player.height - hit->height / 2;
	i = 0;
	while (i < hit->height)
	{
		if (y >= (signed)v->res.y)
			break ;
		if (y >= 0)
		{
			color.full = mlx_img_pixel_get_value(&texture.img, texture_col,
					i * texture.height / hit->height);
			color = fog(color, hit->distance);
			if (!color.bytes.alpha)
				(*v->pixel_put_ft)(v, col, y, color);
		}
		y++;
		i++;
	}
}

void	draw_floor(t_ray const *ray, t_hit const *hit, t_vars const *v)
{
	int			y;
	int			i;
	double		dist;
	t_dvector	inter;
	t_color		color;

	y = v->res.y / 2 + hit->height / 2;
	i = v->player.height + hit->height / 2;
	while (i < (signed)v->res.y)
	{
		dist = (double)(BLOCK_SIZE_2 * v->project_dist)
			/ (double)(y - (double)v->res.y / 2)
			/ cos(fabs(v->player.angle - ray->angle));
		inter.x = v->player.pos.x + cos(ray->angle) * dist;
		if (inter.x < 0)
			inter.x = 0;
		inter.y = v->player.pos.y - sin(ray->angle) * dist;
		if (inter.y < 0)
			inter.y = 0;
		color.full = mlx_img_pixel_get_value(&v->textures.floor.img,
				(double)((int)inter.x % BLOCK_SIZE) / ((double)BLOCK_SIZE
					/ (double)v->textures.floor.width), (double)((int)inter.y
					% BLOCK_SIZE) / ((double)BLOCK_SIZE
						/ v->textures.floor.height));
		color = fog(color, dist);
		(*v->pixel_put_ft)(v, ray->col, y - (v->res.y / 2
					- v->player.height), color);
		y++;
		i++;
	}
}

void	draw_ceilling(t_ray const *ray, t_hit const *hit, t_vars const *v)
{
	int			y;
	int			i;
	double		dist;
	t_dvector	inter;
	t_color		color;

	y = v->res.y / 2 - hit->height / 2;
	i = v->player.height - hit->height / 2;
	while (i >= 0)
	{
		dist = (BLOCK_SIZE_2 * v->project_dist
				/ (double)((double)v->res.y / 2 - y))
			/ cos(fabs(v->player.angle - ray->angle));
		inter.x = v->player.pos.x + cos(ray->angle) * dist;
		if (inter.x < 0)
			inter.x = 0;
		inter.y = v->player.pos.y - sin(ray->angle) * dist;
		if (inter.y < 0)
			inter.y = 0;
		color.full = mlx_img_pixel_get_value(&v->textures.ceilling.img,
				(double)((int)inter.x % BLOCK_SIZE) / ((double)BLOCK_SIZE
					/ v->textures.ceilling.width), (double)((int)inter.y
						% BLOCK_SIZE) / ((double)BLOCK_SIZE
							/ v->textures.ceilling.height));
		color = fog(color, dist);
		(*v->pixel_put_ft)(v, ray->col, y - (v->res.y / 2
					- v->player.height), color);
		y--;
		i--;
	}
}
