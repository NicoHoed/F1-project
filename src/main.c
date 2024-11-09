#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>
#include "track.h"

// Fonction qui simule une voiture effectuant des tours et affiche le temps en format `x : xx : xxx`
void simulate_car(int laps, Circuit circuit) {
  for (int i = 1; i <= laps; i++) {
    // Génère un temps de tour aléatoire en fonction du circuit (min_time_ms à max_time_ms)
    int lap_time_ms = (rand() % (circuit.max_time_ms - circuit.min_time_ms + 1)) + circuit.min_time_ms;

    // Calcul des minutes, secondes et millisecondes
    int minutes = lap_time_ms / 60000;
    int seconds = (lap_time_ms % 60000) / 1000;
    int milliseconds = lap_time_ms % 1000;

    // Affichage du temps au format souhaité
    printf("Tour %d : Temps de tour = %d : %02d : %03d\n", i, minutes, seconds, milliseconds);

    // Pause pour simuler le temps de tour (ici on simule juste un affichage rapide)
    usleep(500000); // Délai de 500 ms pour éviter de ralentir trop l’exécution
  }
}

int main() {
  srand(time(NULL));

  Circuit circuits[24];
  initialize_all_circuits(circuits);  // Initialiser tous les circuits

  pid_t pid = fork();  // Création d’un processus enfant

  if (pid < 0) {
    perror("Erreur lors de la création du processus enfant");
    exit(1);
  } else if (pid == 0) {
    // Code du processus enfant (simulation de la voiture)
    printf("Processus voiture démarré (PID : %d)\n", getpid());
    simulate_car(circuits[0].laps, circuits[0]);  // Par exemple, simulateur de 5 tours pour le premier circuit
    printf("Processus voiture terminé.\n");
    exit(0); // Fin du processus enfant
  } else {
    printf("Processus principal attend la fin de la voiture...\n");
    wait(NULL);  // Attendre que le processus enfant se termine
    printf("Course terminée.\n");
  }

  return 0;
}
