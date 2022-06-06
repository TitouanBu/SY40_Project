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
#define DEBUG false //Variable pour afficher ou non les debugs

#define NUM_P_ABONNE 10	//Nombre de places réservées aux abonnés
#define NUM_P_NABONNE 10	//Nombre de places pour non abonnés
#define NB_ABONNE 5	//Nombre de thread abonnés
#define NB_NONABONNE 10	//Nombre de thread non abonnés
/*--------------------------------------------------*/

pthread_t tidAbonne[NB_ABONNE], tidNonAbonne[NB_NONABONNE];
pthread_mutex_t mutex;


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


#endif