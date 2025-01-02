/*
Role : fichier contenant les prototypes des fonctions liées au générations de temps

Créateurs : YiddishGun & M-Momin le 27/07/22
*/


#ifndef time_h
#define time_h

#include <stdio.h>
#include <stdlib.h>

//structure de temps d'un secteur
typedef struct SectorTime SectorTime; //declaration de la structure
struct SectorTime { //definition de la struct

	int milisecondes; // temps en ms
	float convertTime; // temps converti en sec
};

//prototypes de fonctions
int genererTemps(); //fonction pour generer un temps aleatoire
void assignerTemps(SectorTime* temps); // fonction pour assigner et convertir les temps


#endif // !time_h
