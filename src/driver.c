#include <driver.h>
#include <string.h>

// Initialisation d'un pilote avec son nom, numéro et équipe
void initialize_driver(Driver *driver, const char *name, int number, const char *team) {
    strncpy(driver->name, name, sizeof(driver->name) - 1);
    driver->name[sizeof(driver->name) - 1] = '\0';
    driver->number = number;
    strncpy(driver->team, team, sizeof(driver->team) - 1);
    driver->team[sizeof(driver->team) - 1] = '\0';
    driver->points = 0;
}

// Initialisation de tous les pilotes
void initialize_all_drivers(Driver drivers[]) {
    // Exemple de trois pilotes. Ajoute ici tous les pilotes nécessaires.
    initialize_driver(&drivers[0], "Lewis Hamilton", 44, "Mercedes");
    initialize_driver(&drivers[1], "Max Verstappen", 1, "Red Bull");
    initialize_driver(&drivers[2], "Charles Leclerc", 16, "Ferrari");
    // Ajoute les autres pilotes de manière similaire...
}
