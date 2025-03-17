#include "fdf.h"
/*
// ok fonctionne
// test pour get_next_line
test_f.txt: doit etre dans le meme dossier que fdf.aout


int main(void)
{
    int fd;
    char *line;

    // Ouverture du fichier test_f.txt directement
    fd = open("test_f.txt", O_RDONLY);  // Fichier ouvert statiquement ici
    if (fd < 0)
    {
        perror("Error opening file");
        return (1);
    }

    // Appel de get_next_line pour lire chaque ligne du fichier
    while ((line = get_next_line(fd)) != NULL)
    {
        ft_putstr_fd(line, 1);  // Affiche la ligne lue
        free(line);  // N'oublie pas de libérer la mémoire allouée pour chaque ligne
    }

    close(fd);
    return (0);
}
*/

/*
#include <stdio.h>
#include <string.h>  // pour utiliser la fonction strlcpy

//test pour ft_printf
// ok pas besoin de foat dans printf

int main() {
    int a = 42;
    char *str = "Hello, world!";
    

    // Mélange de types dans un seul appel
    ft_printf("Valeur entière: %d, chaîne: %sf\n", a, str,);
    ft_printf("Test entier: %d\n", a);
    ft_printf("Test chaîne: %s\n", str);
   
    
    printf("Valeur entière: %d, chaîne: %s, flottant: %.2f\n", a, str, pi);
    printf("Test entier: %d\n", a);
    printf("Test chaîne: %s\n", str);
    printf("Test flottant: %.2f\n", pi);

    return 0;
}
*/

/*
test libft ok
int main() 
{
    // Déclaration des variables
    char src[] = "Hello, world!";
    char dest[20];  // Une taille assez grande pour contenir la chaîne source
    size_t size = sizeof(dest);  // Taille du tableau destinataire

    // Affichage avant copie
    ft_printf("Avant copie:\n");
    ft_printf("Source: %s\n", src);
    ft_printf("Destination (vide): '%s'\n", dest);

    // Appel de ta fonction ft_strlcpy
    size_t ret = ft_strlcpy(dest, src, size);

    // Affichage après copie
    ft_printf("\nAprès copie:\n");
    ft_printf("Source: %s\n", src);
    ft_printf("Destination: '%s'\n", dest);
    ft_printf("Taille de la chaîne copiée: %zu\n", ret);

    return 0;
}
*/   
    
    /*
// tester init_map
int main(int argc, char *argv[])
{
    (void)argc; // Ignorer l'avertissement sur 'argc' inutilisé
    t_map *map;
    
    map = init_map(argv[1]);  // Initialise map à partir du fichier passé en argument

    // Utilise map ici, par exemple pour afficher des informations
    printf("Carte initialisée avec succès.\n");
    printf("Dimensions de la carte: %d x %d\n", map->map_width, map->map_height);
    
    // Ou manipule les données de la carte selon tes besoins
    // par exemple : afficher chaque ligne ou chaque valeur de la carte

    return 0;
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
}
*/


