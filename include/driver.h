#ifndef DRIVER_H
#define DRIVER_H

#define NUM_DRIVERS 20

// Structure pour représenter un pilote
typedef struct {
    char name[50];         // Nom
    int number;            // Numéro
    char team[50];         // Equipe
    int points;            // Points
} Driver;

// cette fonction est une sorte de constructeur de structure pour faire les pilotes
// -> voir utilisation dans <driver.c>
void initialize_driver(Driver *driver, const char *name, int number, const char *team);

// Pour initialiser tous les pilotes en une fois
void initialize_all_drivers(Driver drivers[]);

#endif // DRIVER_H
