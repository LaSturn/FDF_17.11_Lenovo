/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_lines.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csturny <csturny@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 14:14:20 by csturny           #+#    #+#             */
/*   Updated: 2025/03/14 09:54:20 by csturny          ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "fdf.h"


void bresenham(t_display *display, t_point start, t_point end) {
    t_point current;
    t_point sign;
    t_point delta;
    int line;
    int tmp;

    initialize_bresenham(&start, &end, &delta, &sign);
    line = delta.x - delta.y;
    current = start;
    while (current.x != end.x || current.y != end.y) {
        current.color = interpolate_color(current, start, end, delta);
        put_pixel_to_img(display, current.x, current.y, current.color);
        tmp = line * 2;
        if (tmp > -delta.y) {
            line -= delta.y;
            current.x += sign.x;
        }
        if (tmp < delta.x) {
            line += delta.x;
            current.y += sign.y;
        }
    }
}


// ok verifie
// ici initialiattion pour l alogo
void	initialize_bresenham(t_point *start, t_point *end,	t_point *delta, t_point *sign)
{
	delta->x = get_abs_value(end->x - start->x);
	delta->y = get_abs_value(end->y - start->y);
	sign->x = -1;
	sign->y = -1;
	if (start->x < end->x)
		sign->x = 1;
	if (start->y < end->y)
		sign->y = 1;
}
/*
// fonction debug a supprimer a la fin
void debug_put_pixel_to_img(int x, int y, int color)
{
    printf("Put pixel at (%d, %d) with color 0x%X\n", x, y, color);
}
*/

/* new */
void	update_transformed_points(t_display *display) // Precomputes points.
{
	int y;
	int x;

	y = -1;
	//ft_printf("Updating transformed points: height %d, width %d\n", display->map->map_height, display->map->map_width); //debugg
	while (++y < display->map->map_height)
	{
		x = -1;
		if (!display->transformed[y]) 
		{
            ft_printf("Error: transformed[%d] is NULL\n", y);
			print_error_and_exit("Invalid transformed row", 1);
        }
		while (++x < display->map->map_width)
		{
			display->transformed[y][x] = apply_transformations(display, create_map_point(x, y, display));
		}
	}
	//ft_printf("Transformed points updated\n");
}


/* ci-dessous render_point.c*/

// a supprimer si tab fonctionne
void render_transformed_map(t_display *display) {
    int x;
    int y;
    if (!display->needs_redraw)
        return;
    ft_bzero(display->img->img_addr, WIN_HEIGHT * WIN_WIDTH * (display->img->bpp / 8));
    if (display->zoom) {
        update_transformed_points(display);
        y = -1;
        while (++y < display->map_height) {
            x = -1;
            while (++x < display->map_width) {
                if (display->points_only)
                    plot_point(display, display->transformed[y][x].x, display->transformed[y][x].y, display->transformed[y][x].color);
                else {
                    if (x < display->map_width - 1)
                        bresenham(display, display->transformed[y][x], display->transformed[y][x + 1]);
                    if (y < display->map_height - 1)
                        bresenham(display, display->transformed[y][x], display->transformed[y + 1][x]);
                }
            }
        }
    }
    // Debug prints...
    mlx_put_image_to_window(display->mlx_ptr, display->window_ptr, display->img->img_ptr, 0, 0);
    // More debug prints...
    display_control_info(display);
    display->needs_redraw = 0;
    //ft_printf("je suis dans render_transformed_map: a la fin\n");
}

/*
remplace par
static void display_state(t_display *display, int *vertical_pos)
static void display_controls(t_display *display, int *vertical_pos)
static void display_rotation_and_exit(t_display *display, int *vertical_pos)
void display_control_info(t_display *display)
static char *get_color_mode_str(int mode)

void display_control_info(t_display *display) {
    void *mlx = display->mlx_ptr;
    void *win = display->window_ptr;
    int vertical_pos = 20;
    char *projection_str = display->is_isometric ? "Isometric" : "Parallel";
    char *color_mode_str;
    char *tmp; // Add this variable to store temporary strings

    switch (display->use_altitude_colors) {
        case 0: color_mode_str = "Default"; break;
        case 1: color_mode_str = "Altitude"; break;
        case 2: color_mode_str = "Rainbow Pulse"; break;
        case 3: color_mode_str = "Rainbow Shift"; break;
        default: color_mode_str = "Unknown"; break;
    }

    mlx_string_put(mlx, win, 25, vertical_pos += 30, WHITE, "Current State:");
    
    // Fix for first memory leak: store, use, then free
    tmp = ft_strjoin("Projection: ", projection_str);
    mlx_string_put(mlx, win, 40, vertical_pos += 20, B_YELLOW, tmp);
    free(tmp); // Free the allocated string after use
    
    // Fix for second memory leak: store, use, then free
    tmp = ft_strjoin("Color Mode: ", color_mode_str);
    mlx_string_put(mlx, win, 40, vertical_pos += 20, B_YELLOW, tmp);
    free(tmp); // Free the allocated string after use

    mlx_string_put(mlx, win, 25, vertical_pos += 30, WHITE, "Controls:");
    mlx_string_put(mlx, win, 40, vertical_pos += 20, B_YELLOW, RESET);
    mlx_string_put(mlx, win, 40, vertical_pos += 20, B_YELLOW, "Reset Isometric: Press 'R'");
    mlx_string_put(mlx, win, 40, vertical_pos += 20, B_YELLOW, "Reset Parallel: Press 'T'");
    mlx_string_put(mlx, win, 40, vertical_pos += 20, B_YELLOW, MOVE);
    mlx_string_put(mlx, win, 40, vertical_pos += 20, B_YELLOW, ZOOMIO);
    mlx_string_put(mlx, win, 40, vertical_pos += 20, B_YELLOW, FLAT);
    mlx_string_put(mlx, win, 25, vertical_pos += 30, WHITE, "Rotate View:");
    mlx_string_put(mlx, win, 40, vertical_pos += 20, B_YELLOW, ROTX);
    mlx_string_put(mlx, win, 40, vertical_pos += 20, B_YELLOW, ROTY);
    mlx_string_put(mlx, win, 40, vertical_pos += 20, B_YELLOW, ROTZ);
    mlx_string_put(mlx, win, 25, vertical_pos += 30, WHITE, "Projection:");
    mlx_string_put(mlx, win, 40, vertical_pos += 20, B_YELLOW, "Toggle Isometric/Parallel: Press 'Tab'");
    mlx_string_put(mlx, win, 40, vertical_pos += 20, B_YELLOW, "Cycle Color Mode (Default/Altitude/Pulse/Shift): Press 'V'");
    mlx_string_put(mlx, win, 40, vertical_pos += 20, B_YELLOW, "Toggle Points/Lines: Press 'P'");
    mlx_string_put(mlx, win, 25, vertical_pos += 30, WHITE, "Exit:");
    mlx_string_put(mlx, win, 40, vertical_pos += 20, B_YELLOW, CLOSE);
}
*/

static void display_state(t_display *display, int *vertical_pos)
{
    void *mlx = display->mlx_ptr;
    void *win = display->window_ptr;
    char *projection_str = display->is_isometric ? "Isometric" : "Parallel";
    char *color_mode_str = get_color_mode_str(display->use_altitude_colors);
    char *tmp;

    mlx_string_put(mlx, win, 25, *vertical_pos += 30, WHITE, "Current State:");
    tmp = ft_strjoin("Projection: ", projection_str);
    mlx_string_put(mlx, win, 40, *vertical_pos += 20, BLUE, tmp);
    free(tmp);
    tmp = ft_strjoin("Color Mode: ", color_mode_str);
    mlx_string_put(mlx, win, 40, *vertical_pos += 20, BLUE, tmp);
    free(tmp);
}

static void display_controls(t_display *display, int *vertical_pos)
{
    void *mlx = display->mlx_ptr;
    void *win = display->window_ptr;

    mlx_string_put(mlx, win, 25, *vertical_pos += 30, WHITE, "Controls:");
    mlx_string_put(mlx, win, 40, *vertical_pos += 20, BLUE, RESET);
    mlx_string_put(mlx, win, 40, *vertical_pos += 20, BLUE, "Reset Isometric: Press 'R'");
    mlx_string_put(mlx, win, 40, *vertical_pos += 20, BLUE, "Reset Parallel: Press 'T'");
    mlx_string_put(mlx, win, 40, *vertical_pos += 20, BLUE, MOVE);
    mlx_string_put(mlx, win, 40, *vertical_pos += 20, BLUE, ZOOMIO);
    mlx_string_put(mlx, win, 40, *vertical_pos += 20, BLUE, FLAT);
}

static void display_rotation_and_exit(t_display *display, int *vertical_pos)
{
    void *mlx = display->mlx_ptr;
    void *win = display->window_ptr;

    mlx_string_put(mlx, win, 25, *vertical_pos += 30, WHITE, "Rotate View:");
    mlx_string_put(mlx, win, 40, *vertical_pos += 20, BLUE, ROTX);
    mlx_string_put(mlx, win, 40, *vertical_pos += 20, BLUE, ROTY);
    mlx_string_put(mlx, win, 40, *vertical_pos += 20, BLUE, ROTZ);
    mlx_string_put(mlx, win, 25, *vertical_pos += 30, WHITE, "Projection:");
    mlx_string_put(mlx, win, 40, *vertical_pos += 20, BLUE, "Toggle Isometric/Parallel: Press 'Tab'");
    mlx_string_put(mlx, win, 40, *vertical_pos += 20, BLUE, "Cycle Color Mode (Default/Altitude/Pulse/Gradient): Press 'V'");
    mlx_string_put(mlx, win, 40, *vertical_pos += 20, BLUE, "Toggle Points/Lines: Press 'P'");
    mlx_string_put(mlx, win, 25, *vertical_pos += 30, WHITE, "Exit:");
    mlx_string_put(mlx, win, 40, *vertical_pos += 20, BLUE, CLOSE);
}

void display_control_info(t_display *display)
{
    int vertical_pos;

    vertical_pos = 20;
    display_state(display, &vertical_pos);
    display_controls(display, &vertical_pos);
    display_rotation_and_exit(display, &vertical_pos);
}

char *get_color_mode_str(int mode)
{
    if (mode == 0)
        return ("Default");
    if (mode == 1)
        return ("Altitude");
    if (mode == 2)
        return ("Rainbow Pulse");
    if (mode == 3)
        return ("Blue Gradient"); // Updated from "Rainbow Shift"
    return ("Unknown");
}



// fonction de base
// ok verifie
// voir si j ai vraiment bsoin des deux conditions dasn mon code!!!!!
// fonction en construction
// ✅ Place un pixel coloré dans l’image en mémoire
// 🖼️ Conclusion : Cette fonction est essentielle pour modifier des pixels individuellement avant d'afficher l’image. 🚀

void	put_pixel_to_img(t_display *display, int x, int y, int color)
{
	int	pixel;
	
	//ft_printf("je suis rentre dans ma focntion: put_pixel_to_img\n");
	if (x >= WIN_WIDTH || y >= WIN_HEIGHT || x < 0 || y < 0)
		return ;
	pixel = (y * display->img->size_line) + (x * (display->img->bpp / 8));
	if (display->img->endianness == 1)
	{
		display->img->img_addr[pixel + 0] = (color >> AS);  //alpha
		display->img->img_addr[pixel + 1] = (color >> RS) & BM; // red
		display->img->img_addr[pixel + 2] = (color >> GS) & BM; // green
		display->img->img_addr[pixel + 3] = (color & BM); //blue
	}
	else if (display->img->endianness == 0)
	{
		display->img->img_addr[pixel + 0] = (color & BM); // blue
		display->img->img_addr[pixel + 1] = (color >> GS) & BM; //geen
		display->img->img_addr[pixel + 2] = (color >> RS) & BM;  // red
		display->img->img_addr[pixel + 3] = (color >> AS); // alpha
	}
	//debug_put_pixel_to_img(x, y, color); // fonction debugg
}

/*
// test
// put_pixel_to_img dans environnement de test!!!!
void debug_pixel_data(int pixel, char *addr)
{
	printf("Pixel data at offset %d: [%02X %02X %02X %02X]\n",
		pixel,
		(unsigned char)addr[pixel + 0],
		(unsigned char)addr[pixel + 1],
		(unsigned char)addr[pixel + 2],
		(unsigned char)addr[pixel + 3]);
}
*/



// porblem avec plot ici
void plot_point(t_display *display, int x, int y, int color)
{
	int	size;
	int dy;
	int dx;

	size = display->point_size;
	dy = -size;	
	while (dy <= size)
	{
		dx = -size;
		while (dx <= size)
		{
			if (x + dx >= 0 && x + dx < WIN_WIDTH && y + dy >= 0 && y +dy < WIN_HEIGHT)
				put_pixel_to_img(display, x + dx, y + dy, color);
			dx++;
		}
		dy++;
	}	
}
