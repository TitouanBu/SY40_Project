#include "CodingFunctions.h"

/*------Fonction de debug du code ------*/
void debug (char *message)
{
	if (DEBUG)
	{
		printf("\n%s\n",message);
	}
}

/*------Fonctions pour gestion des threads ------*/

//Fonction executee par chaque abonne
void *fonc_abonne(void *n)
{
	printf("\nAbonne numero %ld\n",(long) n);
	printf("TID : %ld\n",pthread_self());
	
}

//Fonction executee par chaque non-abonne
void *fonc_non_abonne(void *n)
{
	printf("\nNon-Abonne numero %ld\n",(long) n);
	printf("TID : %ld\n",pthread_self());
}

//Fonction pour créer N abonne(s)
void create_threads(int Abonne, int NonAbonne)
{
	for (long i = 0; i < Abonne; ++i)
	{
		pthread_create(tidAbonne+i,0,fonc_abonne, (void *) i);
		usleep(10000);
	}

	for (long i = 0; i < NonAbonne; ++i)
	{
		pthread_create(tidNonAbonne+i,0,fonc_non_abonne, (void *) i);
		usleep(10000);
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

/*------ Fonctions pour gestion de SIGINT ------*/

void handle_sigint(int sig){
	end_threads(NB_ABONNE,NB_NONABONNE);
	printf("\n\n----- Fin Programme! -----\n");
	exit(0);
}
