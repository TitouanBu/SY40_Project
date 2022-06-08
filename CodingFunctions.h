#ifndef _CODING_FUNCTIONS_H
#define _CODING_FUNCTIONS_H

/*------Ajout des librairies nécessaires au projet------*/
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/wait.h>
#include <pthread.h>
#include <signal.h>
/*--------------------------------------------------*/

/*------Mise en place des variables globales------*/
#define false  0
#define true  1
#define DEBUG true //Variable pour afficher ou non les debugs

#define NUM_P_ABONNE 4	//Nombre de places réservées aux abonnés
#define NUM_P_NABONNE 6 //Nombre de places pour non abonnés
#define NUM_P NUM_P_NABONNE + NUM_P_ABONNE //Nombre de place totale
#define NB_ABONNE 5	//Nombre de thread abonnés
#define NB_NABONNE 10	//Nombre de thread non abonnés
/*--------------------------------------------------*/

pthread_t tidAbonne[NB_ABONNE], tidNonAbonne[NB_NABONNE];
pthread_mutex_t mutex;

/*------------- Structures -------------*/

typedef struct Usager
{
	int id; //numero d'identification de l'usager
	bool isAbonne; //true si abonne
	int stationnement; //numero de la place (-1 si non stationné)
}Usager;

typedef struct PlaceParking
{
	int id; //numero de la place
	bool isAbonne; //true si place réservé aux abonnés
	bool isLibre; //true si place est libre
}PlaceParking;

/*--------------------------------------*/

/*------ Fonction de debug ------*/
void debug (char *message);

/*------ Fonctions pour gestion des threads ------*/

//Fonction executee par chaque abonne
void *fonc_abonne(void *n);

//Fonction executee par chaque non-abonne
void *fonc_non_abonne(void *n);

//Fonction pour créer N abonne(s)
void create_threads(int Abonne, int NonAbonne);

//Fonction pour créer N abonne(s)
void end_threads(int Abonne, int NonAbonne);


/*------ Fonction pour la gestion de SIGINT ------*/

void handle_sigint(int sig);

/*------ Fonctions pour les Usagers ------*/

// initialiser un Usager
Usager initUsager(int id_p);

// modifier les caractéristiques d'un Usager
void modifUsager(Usager* u_p, bool isAbonne_p, int stationnement_p);

// afficher les caractéristiques d'un Usager
void printUsager(Usager u_p);

/*------ Fonctions pour le parking ------*/

// initialiser une place de parking
PlaceParking initPlaceParking(int id_p, bool isAbonne_p, bool isLibre_p);

// initialiser un parking
void initParking(PlaceParking* parking_p);

// afficher les caractéristiques d'une place de parking
void printPlaceParking(PlaceParking place_p);

// afficher les caractéristiques de tout le parking
void printParking(PlaceParking* parking_p);

// Permet à un Usager de se garer sur une place de parking si
// les conditions sont respectées
int seGarer(Usager* usager_p, PlaceParking* place_p);

#endif