#ifndef DRIVERS_H
#define DRIVERS_H

typedef struct {
    int number;
    char name[50];
    char team[50];
} Driver;

// Charge les pilotes depuis un fichier
int load_drivers(const char *filename, Driver drivers[], int max_drivers);

#endif
