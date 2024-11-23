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

#define NUM_CARS 20
#define NUM_SECTORS 3
#define MAX_LAP_TIME 999.9f
#define RESET    "\033[0m"
#define RED     "\033[31m"
#define GREEN    "\033[32m"

struct CarTime {
    int car_number;
    float sector_times[NUM_SECTORS];
    float best_lap_time;
    int pit_stop; // 0 if on track, 1 if in pits
    int out;      // 0 if still racing, 1 if out of race
    int points;
};

struct SharedMemory {
    struct CarTime cars[NUM_CARS];
    float best_sector_times[NUM_SECTORS];
    float best_race_lap_time;
    int best_race_lap_car;
};

void initialize_shared_memory(struct SharedMemory *shm);
void sem_lock(int sem_id);
void sem_unlock(int sem_id);
float generate_random_time(float min_time, float max_time);
void simulate_lap(struct SharedMemory *shm, int car_index, int sem_id);
void save_session_results(struct SharedMemory *shm, const char *filename);
void print_current_standings(struct SharedMemory *shm);
void sort_cars_by_lap_time(struct SharedMemory *shm);
void allocate_points(struct SharedMemory *shm, int race_type);
void print_championship_standings(struct SharedMemory *shm);
#endif