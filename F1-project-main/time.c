/*
Role : fichier contenant les déclarations de fonctions liées au temps

Créateurs : YiddishGun & M-Momin le 27/07/22
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "preprocess.h"
#include "time.h"

//génère un entier entre temps min et temps max
int genererTemps() {

	return rand() % (TEMPSMAXTOUR + 1 - TEMPSMINTOUR) + TEMPSMINTOUR;

	//return rand() % (((TEMPSMAXTOUR + 1 - TEMPSMINTOUR) + TEMPSMINTOUR) / 1000);
}

//assigner les temps à la structure + conversion
void assignerTemps(SectorTime *temps) {

	temps->milisecondes = genererTemps();
	temps->convertTime = round(temps->milisecondes) / 1000;
}