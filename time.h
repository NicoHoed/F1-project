/*
Role : fichier contenant les prototypes des fonctions liées au générations de temps

Créateurs : YiddishGun & M-Momin le 27/07/22
*/


#ifndef time_h
#define time_h

#include <stdio.h>
#include <stdlib.h>

//structure de temps d'un secteur
typedef struct SectorTime SectorTime;
struct SectorTime {

	int milisecondes;
	float convertTime;
};

//prototypes de fonctions
int genererTemps();
void assignerTemps(SectorTime* temps);


#endif // !time_h