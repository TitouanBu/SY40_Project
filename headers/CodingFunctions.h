#ifndef _CODING_FUNCTIONS_H
#define _CODING_FUNCTIONS_H

/*------Ajout des librairies nécessaires au projet------*/
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
/*--------------------------------------------------*/

/*------Mise en place des variables globales------*/
#define false  0
#define true  1
bool activeDebug=false;	//Variable pour afficher ou enlever les debugs

#define NUM_P_ABONNE 10	//Nombre de places réservées aux abonnés
#define NUM_P_NABONNE 10	//Nombre de places pour non abonnés
#define numAbonne 30	//Nombre de thread abonnés
#define numNAbonne 30	//Nombre de thread non abonnés
/*--------------------------------------------------*/

pthread_t tidAbonne[numAbonne],tidNonAbonne[numNAbonne];
pthread_mutex_t mutex;

//Fonction pour debug du code
void debug (char *message)
{
	if (activeDebug)
	{
		printf("\n%s\n",message);
	}
}


/*------Fonctions pour gestion des threads ------*/
//Fonction executee par chaque abonne
void *fonc_abonne(void *n)
{
	printf("Abonne numero %d, TID : %ld\n",
			(int) n,pthread_self());
}

//Fonction executee par chaque non-abonne
void *fonc_non_abonne(void *n)
{
	printf("Non-Abonne numero %d, TID : %ld\n",
			(int) n,pthread_self());
}

//Fonction pour créer N abonne(s)
void create_threads(int Abonne, int NonAbonne)
{
	for (int i = 0; i < Abonne; ++i)
	{
		pthread_create(tidAbonne+i,0,fonc_abonne, (void *) i);
		//usleep(100000);
	}

	for (int i = 0; i < Abonne; ++i)
	{
		pthread_create(tidNonAbonne+i,0,fonc_non_abonne, (void *) i);
		//usleep(100000);
	}
}

//Fonction pour créer N abonne(s)
void end_threads(int Abonne, int NonAbonne)
{
	for(int i=0;i<Abonne;++i)
        	pthread_join(tidAbonne[i],NULL);

	for(int i=0;i<NonAbonne;++i)
        	pthread_join(tidNonAbonne[i],NULL);
}

/*--------------------------------------------------*/

#endif