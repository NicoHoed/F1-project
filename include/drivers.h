#ifndef DRIVERS_H
#define DRIVERS_H

#define MAX_DRIVERS 20  // Nombre maximal de pilotes

typedef struct {
    int id;
    int number;
    char name[50];
    char team[50];
    char nationality[30];
} Driver;


int load_drivers(const char *filename, Driver *drivers, int max_drivers);

#endif // DRIVERS_H
