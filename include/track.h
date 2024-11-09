#ifndef TRACK_H
#define TRACK_H

#define TRACK_LOCATION_MAX_LENGTH 50
#define GP_NAME_MAX_LENGTH 50

typedef struct {
    char location[TRACK_LOCATION_MAX_LENGTH];
    char gp_name[GP_NAME_MAX_LENGTH];
    int laps;
    int min_time_ms;  // Temps de tour minimum en millisecondes
    int max_time_ms;  // Temps de tour maximum en millisecondes
} Circuit;

// Fonction pour initialiser un circuit
void initialize_circuit(Circuit *circuit, const char *location, const char *gp_name, int laps, int min_time_ms, int max_time_ms);

// Fonction pour initialiser tous les circuits du championnat
void initialize_all_circuits(Circuit circuits[]);

#endif
