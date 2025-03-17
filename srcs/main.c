/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csu-ubuntu <csu-ubuntu@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 10:19:24 by csturny           #+#    #+#             */
/*   Updated: 2025/03/17 10:15:57 by csu-ubuntu       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

int	main(int ac, char **av)
{
	t_map	*map;
	char	*file;

	map = NULL;
	if (ac != 2)
		print_error_and_exit("Nombre d'arguments invalide", 1);
	//ft_printf("Arguments corrects !\n");
	//ft_printf("Initialisation du processus de lecture des données de la carte...\n");
	check_file_readable(av[1]); // attention peut etre a double car dans init_map je fait aussi un check!!!
	//ft_printf("Le fichier %s existe et est lisible !\n", av[1]);
	file = av[1];
	map = parse_map_dimensions(file);
	if (!map)
		print_error_and_exit("Impossible d'initialiser la carte", 1);
	//ft_printf("Carte initialisée avec succès.\n");
	get_map(file, map);
	get_z_min_max(map); // pour obtenir min_z et max_z (pour gerer affichage en suite)
	//ft_printf("Mon min_z est : %d\n", map->min_z);
	//ft_printf("Mon man_z est : %d\n", map->max_z);
	//ft_printf("Dans le main: je suis avant start_mlx");
	start_mlx(map);
	//ft_printf("Dans le main: je suis apres start_mlx");
	//ft_printf("Je suis a la fin de mon main");
	return (0);
}

/* display_init.c */

// copie coller 
int start_mlx(t_map *map)
{
    t_display *display;
    //ft_printf("Starting MLX...\n");
    display = ft_calloc(1, sizeof(t_display));
    if (!display)
        print_error_and_exit("Display allocation failed", 0);
    //ft_printf("start_mlx: allocation memoire pour structure display REUSSI!\n");
    if (!map)
	{
        free(display);
        print_error_and_exit("NULL map passed to start_mlx", 0);
    }
    initialize_display(display, map);
    //ft_printf("allocat_display_struct: allocation memoire structure displau: REUSSI!\n");
    initialize_mlx(display);
    configure_default_display(display);
    //debug_display(display);
    display->is_isometric = 1; // Ensure isometric start
	// possible que depuis ici soit le bonus ou  plus ou moins!!!
    mlx_hook(display->window_ptr, KPE, (1L << 0), handel_key_in, display);
    mlx_hook(display->window_ptr, CE, StructureNotifyMask, shutdown_fdf, display);
    mlx_loop_hook(display->mlx_ptr, update_color_time, display); // rainbow
	//ft_printf("je suis avant: render_transformed_map (is_isometric = %d)\n", display->is_isometric);
    render_transformed_map(display);
    //ft_printf("je suis suis apres: render_transformed_map\n");
    //ft_printf("je suis suis apres mlx_loop\n");
    mlx_loop(display->mlx_ptr);
    free_all_struct_display(display);
    return (EXIT_SUCCESS);
}

// a suprmier si foncitonne avec parallel
// a rendre plus lisible -> creer une sous fonction pour allouer de la mémoir pour tranformed 2D Array for precomputed coordinates
void initialize_display(t_display *display, t_map *map)
{
    int y;
    if (display == NULL)
        print_error_and_exit("disolay is NULL", 1);
    if (!map || map->map_height <= 0 || map->map_width <= 0)
    {
        free(display);
		print_error_and_exit("Invalid map dimensions", 1);
    }
    display->map = map;
    display->map_width = map->map_width;  // Store width
    display->map_height = map->map_height; // Store height
    //ft_printf("Map height: %d, width: %d\n", map->map_height, map->map_width);
    display->img = ft_calloc(1, sizeof(t_imagedata));
    if (!display->img)
    {
        free_all_struct_display(display);
        print_error_and_exit("img is NULL", 1);
    }
    display->transformed = ft_calloc(map->map_height, sizeof(t_point *));
    if (!display->transformed)
    {
        free_all_struct_display(display);
		print_error_and_exit("Failed to allocate transformed array", 1);
    }
    y = -1;
    while (++y < map->map_height)
    {
        display->transformed[y] = ft_calloc(map->map_width, sizeof(t_point));
        if (!display->transformed[y])
        {
            ft_printf("Failed to allocate row %d of transformed\n", y);
            free_all_struct_display(display);
			print_error_and_exit("Failed to allocate transformed row", 1);
        }
    }
    //ft_printf("Transformed array allocated: %d rows, %d cols\n", map->map_height, map->map_width);

    // Initialize tile system (after all arrays are set up)
    display->tiles = NULL;  // Initialize to NULL before calling init function
}

// attention j ai ajoutun teste
// ok verifie
// mettre les valeurs initialiser les valeurs pour display (donner valeur par defaut adapte a la carte a afficher)
void	configure_default_display(t_display *display)
{
	/*
	// pour teste mettre tout ca sous une sous fonction
	ft_printf("je suis dans la fonction configure_default_display\n");
	if (!display) {
        printf("Erreur: display est NULL dans configure_default_display\n");
        return;
    }

    if (!display->img) {
        printf("Erreur: display->img est NULL dans configure_default_display\n");
        return;
    }

    if (!display->img->img_addr) {
        printf("Erreur: display->img->img_addr est NULL dans configure_defau                                                                                 
        return;
    }*/
	display->zoom = get_max_int((WIN_WIDTH / display->map->map_width / 2), (WIN_HEIGHT / display->map->map_height / 2)); // si le temps a adapter avec la hauteur 
	display->shift_x = WIN_WIDTH / 2;
	display->shift_y = (WIN_HEIGHT - display->map->map_height * display->zoom) / 2;
	display->is_isometric = 1;
	display->flat_f = 1;
	display->rot_x = 0;
	display->rot_y = 0;
	display->rot_z = 0;                                                                                 
	display->needs_redraw = 1; // 1 for first reneder: 1 means "redraw needed," 0 means "no redraw needed."
	display->points_only = 0; // Start in wireframe mode
    display->point_size = 2;  // Default size for points: A 5x5 pixel square (size ±2) for visibility.
	display->use_altitude_colors = 0; // Default to current behavior
	display->color_time = 0.0;
	//ft_printf("je suis dans la fonction configure_default_display anvant get_max_int\n");
	// Appel à la fonction de debug
	//debug_display(display);

    // Initialize the tile system
    init_tile_system(display);
}
/*
// fonction de debug a la fin mettre en commentaire et dans un fichier debug si necessaire
void debug_display(t_display *display)
{
	ft_printf("\n=== DEBUG DISPLAY CONFIG ===\n");
	ft_printf("Shift X: %d\n", display->shift_x);
	ft_printf("Shift Y: %d\n", display->shift_y);
	ft_printf("Rotation X: %d\n", display->rot_x);
	ft_printf("Rotation Y: %d\n", display->rot_y);
	ft_printf("Rotation Z: %d\n", display->rot_z);
	ft_printf("Zoom: %d\n", display->zoom);
	ft_printf("Isometric: %d\n", display->is_isometric);
	ft_printf("Flat factor: %d\n", display->flat_f);
	if (display->map)
	{
		ft_printf("Map Width: %d\n", display->map->map_width);
		ft_printf("Map Height: %d\n", display->map->map_height);
	}
	else
	{
		ft_printf("Map is NULL!\n");
	}
	ft_printf("===========================\n\n");
}
*/


// a revoir
// ok verifie   
// enlever les chech pas neécessaire!!!    
// ancien                                                                          
int initialize_mlx(t_display *display)
{
	display->mlx_ptr = mlx_init(); // ici mlx fonction mlx_init()
	if (display->mlx_ptr == NULL)
	{
		print_error_without_exit("mlx_init() a échoué", 0);
		return (0);
	}
	//printf("mlx_init: OK\n");
	display->img->img_ptr = mlx_new_image(display->mlx_ptr, WIN_WIDTH, WIN_HEIGHT); // ici mlx funciton
	if (display->img->img_ptr == NULL)
	{
		print_error_without_exit("mlx_new_image() a échoué", 0);
		mlx_destroy_display(display->mlx_ptr);
		free(display->mlx_ptr);
		return (0);
	}
	//printf("mlx_new_image: OK\n");
	display->img->img_addr = mlx_get_data_addr(display->img->img_ptr, &display->img->bpp, &display->img->size_line, &display->img->endianness); // ici mlx funciton !!! dans orignial endianness
	if (display->img->img_addr == NULL)
	{
		print_error_without_exit("mlx_get_data_addr() a échoué", 0);
		mlx_destroy_image(display->mlx_ptr, display->img->img_ptr); 
		mlx_destroy_display(display->mlx_ptr); 
		free(display->mlx_ptr); 
		return (0);
	}
	//printf("mlx_get_data_addr: OK\n");
	display->window_ptr = mlx_new_window(display->mlx_ptr, WIN_WIDTH, WIN_HEIGHT, "Sturny's FdF"); // // ici mlx funciton
	if (display->window_ptr == NULL)
	{
		print_error_without_exit("mlx_new_window() a échoué", 0);
		mlx_destroy_image(display->mlx_ptr, display->img->img_ptr); 
		mlx_destroy_display(display->mlx_ptr); 
		free(display->mlx_ptr); 
		return (0);
	}
	//printf("mlx_new_window: OK\n");
	return (1);  // Tout s'est bien passé
}



/* input_key.c*/

#include "fdf.h"
// ok verifie
void    zoom_in_and_out(int keycode, t_display *display)
{
    if (keycode == KC_O)
    {
        if (display->zoom > 0)
            display->zoom -= 5 * ZOOM;
    }
    if (keycode == KC_I)
    {
        if (display->zoom < INT_MAX)
            display->zoom += 5 * ZOOM;
    }
}
// ok verifie
void    translation(int keycode, t_display *display)
{
    if (keycode == KC_W)
		display->shift_y -= 5 * TRANS;
	if (keycode == KC_A)
		display->shift_x -= 5 * TRANS;
	if (keycode == KC_S)
		display->shift_y += 5 * TRANS;
	if (keycode == KC_D)
		display->shift_x += 5 * TRANS;
}
// ok verifie
void rotation(int keycode, t_display *display)
{
	float rot_step;
        // 100 pas mal
    rot_step = ROT * (100.0 / display->zoom); // Base step adjusted by zoom jouer avec 100 ou 1000 pour vitesse de rotation
    if (rot_step > 0.1)
        rot_step = 0.1;
    if (keycode == KC_1)
		display->rot_x += rot_step;
	if (keycode == KC_2)
		display->rot_x -= rot_step;
	if (keycode == KC_3)
		display->rot_y += rot_step;
	if (keycode == KC_4)
		display->rot_y -= rot_step;
	if (keycode == KC_5)
		display->rot_z += rot_step;
	if (keycode == KC_6)
		display->rot_z -= rot_step;
}
// ok verifie
void    activ_ff(int keycode, t_display *display)
{
	if (keycode == KC_8)
	{
		if (display->flat_f < 10)
			display->flat_f += 5 * FF;
	}
	if (keycode == KC_7)
	{
		if (display->flat_f > 0)
			display->flat_f -= 5 * FF;
	}
}

// ok verifie
/**
 * @brief Handles key inputs and performs corresponding actions.
 * 
 * This function takes a keycode and a pointer to a display structure. Depending 
 * on the keycode, it performs different actions:
 * - KC_ESC: Closes the window.
 * - KC_R: Resets the display variables.
 * - KC_TAB: Toggles the isometric view and parallel view.
 * - KC_I or KC_O: Handles zooming in or out.
 * - KC_S, KC_W, KC_D, or KC_A: Handles moving the display.
 * - KC_1 to KC_6: Handles rotating the display.
 * - KC_7 or KC_8: Adjusts the flattening factor.
 * After performing the corresponding action, it redraws the display.
 * 
 * @param keycode The keycode that determines which action to perform.
 * @param vars The display variables that are used or modified by the actions.
 * @return Always returns 0.
 */

int handel_key_in(int keycode, t_display *display)
{
    //ft_printf("Dans hook, handel_key_in: je viens de rentrer\n");
    if (keycode == KC_ESC) // Fermer le programme si la touche Échap est enfoncée 
    {
        win_exit_and_cleanup(display);
        return (0);
    }
    if (keycode == KC_R) // Réinitialiser les variables d'affichage si la touche R est enfoncée
        configure_default_display(display);
    else if (keycode == KC_TAB) // Basculer entre la vue isométrique et la vue parallèle avec Tab
    {
        //ft_printf("Toggling projection: isometric %d -> %d\n", display->is_isometric, !display->is_isometric);
        display->is_isometric =!display->is_isometric; // je ne comprends pas exactement ce que c est
        display->needs_redraw = 1;
    }
    else if (keycode == KC_I || keycode == KC_O) //Modifier le zoom avec les touches I et O
    {
        zoom_in_and_out(keycode, display);
        display->needs_redraw = 1;
    }
    else if (keycode == KC_S || keycode == KC_W || keycode == KC_D || keycode == KC_A)
    {
        translation(keycode, display);
        display->needs_redraw = 1;
    }
		
    else if (keycode == KC_1 || keycode == KC_2 || keycode == KC_3
		|| keycode == KC_4 || keycode == KC_5 || keycode == KC_6)
    {
        rotation(keycode, display);
        display->needs_redraw = 1;
    }
    else if (keycode == KC_7 || keycode == KC_8)
    {
        activ_ff(keycode, display);
        display->needs_redraw = 1;
    }
    else if (keycode == KC_P) 
    {
        //ft_printf("Toggling points_only: %d -> %d\n", display->points_only, !display->points_only);
        display->points_only = !display->points_only;
        display->needs_redraw = 1;
    }
    else if (keycode == KC_C)
    {
        display->use_altitude_colors = !display->use_altitude_colors;
        //ft_printf("Toggling color mode: use_altitude_colors = %d\n", display->use_altitude_colors);
        display->needs_redraw = 1;
    }
    else if (keycode == KC_V)
    {
        display->use_altitude_colors = (display->use_altitude_colors + 1) % 4;
        //ft_printf("Color mode: %d (0=default, 1=altitude, 2=rainbow pulse, 3=rainbow shift)\n", display->use_altitude_colors);
        display->needs_redraw = 1;
        render_transformed_map(display); // Immediate redraw
    }
    //ft_printf("Dans hook, handel_key_in: je suis avant render_transformed_map\n");
    render_transformed_map(display);
    //ft_printf("Dans hook, handel_key_in: j'ai passe render_transformed_map\n");
    return (0);
}

