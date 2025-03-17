/*
// tester init_map

int main(int argc, char *argv[])
{
    (void)argc; // Ignorer l'avertissement sur 'argc' inutilisé
    t_map *map;
    
    map = init_map(argv[1]);  // Initialise map à partir du fichier passé en argument

    // Utilise map ici, par exemple pour afficher des informations
    ft_printf("Carte initialisée avec succès.\n");
    ft_printf("Dimensions de la carte: largeur %d x hauteur %d\n", map->map_width, map->map_height);
    
    // Ou manipule les données de la carte selon tes besoins
    // par exemple : afficher chaque ligne ou chaque valeur de la carte
    
    // mtn on peut checher le format de la carte pour voir si est valide
    ft_printf("la valeur de map_width est %d\n", map->map_width);
    check_map_format(argv[1], map); 

    // get_map: allouer memoire pour matrice 2D z_ij et color
    // stocker les valeurs (z et color) dans la matrice 
    // z_ij avec ft_atoi et color avec ft_hexatoi
    get_map(argv[1], map);
    print_map_data(map); // imprimer les données de la carte
    ft_printf("je suis passe par print ma data!!!\n");
    free_map_memory(map); // libérer la mémoire allouée pour la carte
    return 0;
}*/
/*
// test pour imprimer les données de la carte

void print_map_data(t_map *map)
{
    int i, j;

    // Imprimer le tableau z_ij (valeurs numériques)
    printf("z_ij (valeurs numériques):\n");
    i = 0;
    while (i < map->map_height)
    {
        j = 0;
        while (j < map->map_width)
        {
            printf("%d ", map->z_ij[i][j]);  // Affiche la valeur de z_ij
            j++;            
        }
        printf("\n");
        i++;
    }

    // Imprimer le tableau colors (conflicting types for ‘main’; have ‘int(void)’uleurs en hexadécimal)
    printf("\ncolors (couleurs en hexadécimal):\n");
    i = 0;
    while (i < map->map_height)
    {
        j = 0;
        while (j < map->map_width)
        {
            printf("%d ", map->color[i][j]);  // Affiche la couleur en hexadécimal avec un format de 10 caractères (0x + 8 chiffres)
            j++;
        }
        printf("\n");
        i++;
    }
}
*/
/*
int main(int ac, char **av)
{
    t_map *map;
    char *file_name;

    map = NULL;
    if (ac == 2)
    {
        printf("Starting reading map data\n"); // !!! changer en ft_printf
        file_name = av[1];
        map = init_map(file_name); // semlbe ok jusqu ici
        
    }
    else
        exit_error_msg("There is somthing wrong with the number of args!\n");
    printf("Map reading is done\n"); // !!! changer en ft_printf
    return (0);
}*/
/*
// test de get_next_line pour voir si il y a des leak
void test_file(const char *filename)
{
    int fd = open(filename, O_RDONLY);
    if (fd < 0)
    {
        perror("Error opening file");
        return;
    }

    printf("Testing file: %s\n", filename);
    char *line;
    int line_number = 1;
    
    while ((line = get_next_line(fd)) != NULL)
    {
        printf("Line %d: %s", line_number, line);  // Affiche chaque ligne lue
        free(line);                                // Libère la mémoire après chaque appel
        line_number++;
    }

    close(fd);
    printf("End of file: %s\n\n", filename);
}

int main(void)
{
    // Tests de différents types de fichiers
    test_file("test_maps/42.fdf");

    // Ajoutez d'autres tests si besoin, en fonction des cas limites à couvrir
    ft_printf("avant 0\n");
    return 0;
}
*/

/*
// test pour voir si get next line a un Null a la fin
// test ok
int main(void)
{
    char **split;
    int i;

    split = ft_split("Hello World 42", ' ');
    if (!split)
    {
        printf("Error: ft_split returned NULL\n");
        return (1);
    }

    i = 0;
    while (split[i])
    {
        printf("split[%d]: %s\n", i, split[i]);
        i++;
    }

    // Vérifiez si le dernier élément est NULL
    printf("split[%d]: %p (should be NULL)\n", i, (void *)split[i]);

    // Libérez la mémoire
    free_matrix(split);

    return (0);
}*/