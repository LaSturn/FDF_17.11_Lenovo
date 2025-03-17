/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   transform_points.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csturny <csturny@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 11:34:44 by csturny           #+#    #+#             */
/*   Updated: 2025/03/13 18:25:35 by csturny          ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "fdf.h"



t_point apply_transformations(t_display *display, t_point point) {
    int x = point.x * display->zoom;
    int y = point.y * display->zoom;
    int z = point.z * (display->zoom / SF) * display->flat_f;
    apply_rot_x(display, &y, &z);
    apply_rot_y(display, &x, &z);
    apply_rot_z(display, &x, &y);
    if (display->is_isometric)
        apply_isometric(display, &x, &y, z);
    else
        apply_parallel(display, &x, &y, z);
    x += display->shift_x;
    y += display->shift_y;
    point.x = x;
    point.y = y;
    // Keep original z for color, don’t overwrite
    // point.z = z; // Remove this line
    //ft_printf("Transform: orig_z=%d, new_x=%d, new_y=%d\n", point.z, x, y);
    return point;
}


t_point create_map_point(int x, int y, t_display *display) {
    t_point point;
    point.x = x;
    point.y = y;
    point.z = display->map->z_ij[y][x];
    if (display->use_altitude_colors == 3)
        point.color = get_blue_gradient_color(display, point.z); // Replace with blue gradient
    else if (display->use_altitude_colors == 2)
        point.color = get_rainbow_pulse_color(display, point.z);
    else if (display->use_altitude_colors == 1)
        point.color = get_color_by_altitude(display, point.z);
    else if (display->map->is_color)
        point.color = display->map->color[y][x];
    else
        point.color = get_color_by_altitude(display, point.z);
    return point;
}



// NOUVEAU 
void apply_isometric(t_display *display, int *x, int *y, int z) 
{
    (void)display;
	int prev_x = *x;
    int prev_y = *y;
    *x = (prev_x - prev_y) * cos(0.52359877559); // 30°
    *y = -z + (prev_x + prev_y) * sin(0.52359877559);
}

void apply_parallel(t_display *display, int *x, int *y, int z) 
{
    (void)display;
	*x = *x; // No change
    *y = *y - z; // Shift y by z
}


/* ci-dessous transform_rotation.c*/

// ok verifie
// rotation in place
// rotation around x-axis

void	apply_rot_x(t_display *display, int *y, int *z)
{
	float	prev_y;

	prev_y = *y;
	*y = (prev_y * cos(display->rot_x)) + (*z * sin(display->rot_x));
	*z = (-prev_y * sin(display->rot_x)) + (*z * cos(display->rot_x));
}
// ok verifie
// rotation in place
// rotation around y-axis
void	apply_rot_y(t_display *display, int *x, int *z)
{
	float	prev_x;

	prev_x = *x;
	*x = (*x * cos(display->rot_y)) + (*z * sin(display->rot_y));
	*z = (-prev_x * sin(display->rot_y)) + (*z * cos(display->rot_y));
}
// ok verifie
// rotation in place
// rotation around z-axis
void	apply_rot_z(t_display *display, int *x, int *y)
{
	float	prev_x;

	prev_x = *x;
	*x = (*x * cos(display->rot_z)) + (*y * sin(display->rot_z));
	*y = (prev_x * sin(display->rot_z)) + (*y * cos(display->rot_z));
}

int get_max_int(int v1, int v2)
{

	if(v1 > v2)
		return (v1);
	return (v2);
}
