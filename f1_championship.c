#include "f1_championship.h"

// Sort the cars by their best lap time
void sort_cars_by_lap_time(struct SharedMemory *shm) {
  struct CarTime temp;
  for (int i = 0; i < NUM_CARS - 1; i++) {
    for (int j = i + 1; j < NUM_CARS; j++) {
      if (shm->cars[j].best_lap_time < shm->cars[i].best_lap_time) {
        temp = shm->cars[i];
        shm->cars[i] = shm->cars[j];
        shm->cars[j] = temp;
      }
    }
  }
}

void initialize_shared_memory(struct SharedMemory *shm) {
  int car_numbers[NUM_CARS] = {1,  11, 16, 55, 63, 44, 31, 10, 4,  81,
                               77, 24, 14, 18, 20, 27, 22, 3,  23, 2};
  for (int i = 0; i < NUM_CARS; i++) {
    shm->cars[i].car_number = car_numbers[i];
    shm->cars[i].best_lap_time = MAX_LAP_TIME;
    shm->cars[i].pit_stop = 0;
    shm->cars[i].out = 0;
    shm->cars[i].points = 0;
    for (int j = 0; j < NUM_SECTORS; j++) {
      shm->cars[i].sector_times[j] = 0.0f;
    }
  }
  for (int i = 0; i < NUM_SECTORS; i++) {
    shm->best_sector_times[i] = MAX_LAP_TIME;
  }
  shm->best_race_lap_time = MAX_LAP_TIME;
  shm->best_race_lap_car = -1;
}

void allocate_points(struct SharedMemory *shm, int race_type) {
  // Points distribution
  int sprint_points[8] = {8, 7, 6, 5, 4, 3, 2, 1};
  int race_points[10] = {25, 20, 15, 10, 8, 6, 5, 3, 2, 1};

  // Sort cars by best lap time to determine finishing order
  sort_cars_by_lap_time(shm);

  // Allocate points based on the race type
  if (race_type == 1) { // Sprint race
    for (int i = 0; i < 8; i++) {
      shm->cars[i].points += sprint_points[i];
    }
  } else if (race_type == 2) { // Sunday race
    for (int i = 0; i < 10; i++) {
      shm->cars[i].points += race_points[i];
    }
    // Award an additional point for the fastest lap if within the top 10
    if (shm->best_race_lap_car != -1) {
      for (int i = 0; i < 10; i++) {
        if (shm->cars[i].car_number == shm->best_race_lap_car) {
          shm->cars[i].points += 1;
          break;
        }
      }
    }
  }
}

void sem_lock(int sem_id) {
  struct sembuf sem_op;
  sem_op.sem_num = 0;
  sem_op.sem_op = -1; // P()
  sem_op.sem_flg = 0;
  semop(sem_id, &sem_op, 1);
}

void sem_unlock(int sem_id) {
  struct sembuf sem_op;
  sem_op.sem_num = 0;
  sem_op.sem_op = 1; // V()
  sem_op.sem_flg = 0;
  semop(sem_id, &sem_op, 1);
}

float generate_random_time(float min_time, float max_time) {
  return min_time + ((float)rand() / RAND_MAX) * (max_time - min_time);
}

void simulate_lap(struct SharedMemory *shm, int car_index, int sem_id) {
  sem_lock(sem_id);

  struct CarTime *car = &shm->cars[car_index];

  if (car->out) {
    sem_unlock(sem_id);
    return;
  }
  if (car->pit_stop) {
    usleep(250);
    car->pit_stop = 0;
    sem_unlock(sem_id);
    return;
  }

  // Generate random times for each sector
  float lap_time = 0.0f;
  for (int i = 0; i < NUM_SECTORS; i++) {
    car->sector_times[i] = generate_random_time(25.0f, 45.0f);
    lap_time += car->sector_times[i];
    if (car->sector_times[i] < shm->best_sector_times[i]) {
      shm->best_sector_times[i] = car->sector_times[i];
    }
  }

  if (lap_time < car->best_lap_time) {
    car->best_lap_time = lap_time;
  }
  // Check if this is the fastest lap of the race
  if (lap_time < shm->best_race_lap_time) {
    shm->best_race_lap_time = lap_time;
    shm->best_race_lap_car = car->car_number;
  }

  // Randomly decide if the car goes to the pit or is out
  if (rand() % 100 < 5) {
    car->pit_stop = 1;
  }
  if (rand() % 100 < 2) {
    car->out = 1;
  }

  sem_unlock(sem_id);
}

void save_session_results(struct SharedMemory *shm, const char *filename) {
  FILE *file = fopen(filename, "w");
  if (file == NULL) {
    perror("Failed to open file");
    return;
  }

  fprintf(file, "Car\tBest Lap Time\tSector 1\tSector 2\tSector 3\n");
  for (int i = 0; i < NUM_CARS; i++) {
    fprintf(file, "%d\t%.2f\t%.2f\t%.2f\t%.2f\n", shm->cars[i].car_number,
            shm->cars[i].best_lap_time, shm->cars[i].sector_times[0],
            shm->cars[i].sector_times[1], shm->cars[i].sector_times[2]);
  }
  fprintf(file, "\nBest Sector Times:\n");
  fprintf(file, "S1: %.2f\nS2: %.2f\nS3: %.2f\n", shm->best_sector_times[0],
          shm->best_sector_times[1], shm->best_sector_times[2]);

  float best_lap = MAX_LAP_TIME;
  int best_car_index = -1;
  for (int i = 0; i < NUM_CARS; i++) {
    if (shm->cars[i].best_lap_time < best_lap) {
      best_lap = shm->cars[i].best_lap_time;
      best_car_index = i;
    }
  }

  if (best_car_index != -1) {
    fprintf(file, "\nBest Lap Time: %.2f by Car %d\n", best_lap,
            shm->cars[best_car_index].car_number);
  }

  fclose(file);
}

void print_current_standings(struct SharedMemory *shm) {
  printf("-----------------------------------------------------------------\n");
  printf("| Pos | Car # | Best Lap  T  |  S.1    S.2    S.3  | Pit | Status "
         "|    Diff   |\n");
  printf("-----------------------------------------------------------------\n");

  int positions[NUM_CARS];
  for (int i = 0; i < NUM_CARS; i++) {
    positions[i] = i;
  }

  // Sort the cars by best lap time (ascending order)
  for (int i = 0; i < NUM_CARS - 1; i++) {
    for (int j = i + 1; j < NUM_CARS; j++) {
      if (shm->cars[positions[j]].best_lap_time <
          shm->cars[positions[i]].best_lap_time) {
        int temp = positions[i];
        positions[i] = positions[j];
        positions[j] = temp;
      }
    }
  }

  float previous_best_time = 0.0f;
  for (int i = 0; i < NUM_CARS; i++) {
    struct CarTime *car = &shm->cars[positions[i]];
    if (car->out && i == 0) {
      previous_best_time = car->best_lap_time;
    }

    printf("| %3d | %5d |   %.2f sec  | %.2f, %.2f, %.2f | %3s | %6s |", i + 1,
           car->car_number, car->best_lap_time, car->sector_times[0],
           car->sector_times[1], car->sector_times[2],
           car->pit_stop ? "P" : " ", car->out ? RED"OUT"RESET : GREEN"RUN"RESET);

    if (i > 0 && !car->out) {
      printf(" +%.2f sec", car->best_lap_time - previous_best_time);
    } else {
      printf("          "); // Empty space for the first car or if car is out
    }

    printf(" |\n");

    // Update previous best time
    if (!car->out) {
      previous_best_time = car->best_lap_time;
    }
  }

  printf("-----------------------------------------------------------------\n");
}

#include "f1_championship.h"

void print_championship_standings(struct SharedMemory *shm) {
  printf("===============================================\n");
  printf("|           CHAMPIONSHIP STANDINGS            |\n");
  printf("===============================================\n");
  printf("| Pos | Car # | Total Points |  Best Lap     |\n");
  printf("===============================================\n");

  // Sort cars by their total points (descending order)
  int positions[NUM_CARS];
  for (int i = 0; i < NUM_CARS; i++) {
    positions[i] = i;
  }

  for (int i = 0; i < NUM_CARS - 1; i++) {
    for (int j = i + 1; j < NUM_CARS; j++) {
      if (shm->cars[positions[j]].points > shm->cars[positions[i]].points) {
        int temp = positions[i];
        positions[i] = positions[j];
        positions[j] = temp;
      }
    }
  }

  for (int i = 0; i < NUM_CARS; i++) {
    struct CarTime *car = &shm->cars[positions[i]];

    // Color coding for the top 3 positions
    const char *color = "";
    const char *reset_color = "\033[0m";
    if (i == 0) {
      color = "\033[1;32m"; // Green for 1st place
    } else if (i == 1) {
      color = "\033[1;33m"; // Yellow for 2nd place
    } else if (i == 2) {
      color = "\033[1;34m"; // Blue for 3rd place
    }

    printf("|%s %3d%s | %5d |     %-10d |    %-10f|\n", color, i + 1,
           reset_color, car->car_number, car->points, car->best_lap_time);

    printf("-----------------------------------------------\n");
  }

  printf("===============================================\n");
}
