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

    initialize_driver(&drivers[0], "Max Verstappen", 1, "Red Bull");
    initialize_driver(&drivers[1], "Sergio Perez", 11, "Red Bull");

    initialize_driver(&drivers[2], "Lewis Hamilton", 44, "Mercedes");
    initialize_driver(&drivers[3], "George Russel", 63, "Mercedes");

    initialize_driver(&drivers[4], "Charles Leclerc", 16, "Ferrari");
    initialize_driver(&drivers[5], "Carlos Sainz", 55, "Ferrari");

    initialize_driver(&drivers[6], "Lando Norris", 4, "McLaren");
    initialize_driver(&drivers[7], "Oscar Piastri", 81, "McLaren");

    initialize_driver(&drivers[8], "Fernando Alonso", 14, "Aston Martin");
    initialize_driver(&drivers[9], "Lance stroll", 18, "Aston Martin");

    initialize_driver(&drivers[10], "Pierre Gasly", 10, "Alpine");
    initialize_driver(&drivers[11], "Esteban Ocon", 31, "Alpine");

    initialize_driver(&drivers[12], "Alexander Albon", 23, "Williams");
    initialize_driver(&drivers[13], "Franco Colapinto", 43, "Williams");

    initialize_driver(&drivers[14], "Liam Lawson", 3, "Racing bulls");
    initialize_driver(&drivers[15], "Yuki Tsunoda", 22, "Racing bulls");

    initialize_driver(&drivers[16], "Zhou Guanyu", 24, "Kick Sauber");
    initialize_driver(&drivers[17], "Valtteri Bottas", 77, "Kick Sauber");

    initialize_driver(&drivers[18], "Kevin Magnussen", 20, "Haas");
    initialize_driver(&drivers[19], "Nico Hulkenberg", 27, "Haas");

}
