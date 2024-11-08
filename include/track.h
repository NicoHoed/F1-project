#ifndef TRACK_H
#define TRACK_H

#define TRACK_LOCATION_MAX_LENGTH 50
#define GP_NAME_MAX_LENGTH 50

typedef struct {
    char location[TRACK_LOCATION_MAX_LENGTH];
    char gp_name[GP_NAME_MAX_LENGTH];
    int laps;
} Circuit;

// Fonction pour initialiser un circuit
void initialize_circuit(Circuit *circuit, const char *location, const char *gp_name, int laps);

// Fonction pour initialiser tous les circuits
void initialize_all_circuits(Circuit circuits[]);

#endif // TRACK_H
