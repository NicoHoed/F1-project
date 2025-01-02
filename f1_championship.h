#ifndef F1_CHAMPIONSHIP_H
#define F1_CHAMPIONSHIP_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <unistd.h>
#include <time.h>

#define NUM_CARS 20          // Nombre de voitures
#define NUM_SECTORS 3        // Nombre de secteurs
#define MAX_LAP_TIME 999.9f  // Temps de tour maximal, utilisé comme valeur de non-temps valide
#define RESET    "\033[0m"   // Code de couleur pour le formatage terminal
#define RED     "\033[31m"   // Code couleur rouge
#define GREEN   "\033[32m"   // Code couleur vert

// Structure représentant les données d'une voiture
struct CarTime {
    int car_number;                  // Numéro de la voiture
    float sector_times[NUM_SECTORS]; // Temps pour chaque secteur
    float best_lap_time;             // Meilleur temps de tour réalisé par cette voiture
    int pit_stop;                    // 0 si voiture en piste, 1 si voiture aux stands
    int out;                         // 0 si encore en course, 1 si sortie de course
    int points;                      // Points accumulés par la voiture
};

// Structure représentant la mémoire partagée des voitures
struct SharedMemory {
    struct CarTime cars[NUM_CARS];         // Tableau contenant les infos des voiture
    float best_sector_times[NUM_SECTORS];
    float best_race_lap_time;
    int best_race_lap_car;
};

// Fonction qui initialiser la mémoire partagée avec des valeurs par défaut
void initialize_shared_memory(struct SharedMemory *shm);

// Fonction pour verrouiller un sémaphore
void sem_lock(int sem_id);

// Fonction pour déverrouiller un sémaphore
void sem_unlock(int sem_id);

// Fonction pour générer un temps aléatoire entre un min et un max
float generate_random_time(float min_time, float max_time);

// Fonction pour simuler un tour pour une voiture
void simulate_lap(struct SharedMemory *shm, int car_index, int sem_id);

// Fonction pour enregistrer les résultats de la session dans un fichier
void save_session_results(struct SharedMemory *shm, const char *filename);

// Fonction pour afficher les classements actuels durant les session
void print_current_standings(struct SharedMemory *shm);

// Fonction pour trier les voitures par temps de tour le plus rapide
void sort_cars_by_lap_time(struct SharedMemory *shm);

// Fonction pour attribuer les points (Sprint ou Course Dimanche)
void allocate_points(struct SharedMemory *shm, int race_type);

// Fonction pour afficher les classements du championnat
void print_championship_standings(struct SharedMemory *shm);

#endif
