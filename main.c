#include "CodingFunctions.h"

pthread_mutex_t mutex;
pthread_cond_t patienter;
pthread_t tidUsager[NB_USAGER];

int NbUsagerAttente = 0;

/*  Fonctions thread synchronisation

	pthread_cond_wait (&patienter, &mutex);
	pthread_cond_signal (&patienter);
	
	pthread_mutex_lock(&mutex);
	pthread_mutex_unlock(&mutex);
*/

//Fonction executee par chaque abonne
void *fonc_abonne(int id_p)
{
	Usager u = initAbonne(id_p);
	printf("\n\nCréation Thread Abonne n°%d", u.id);
	printUsager(u);
	/*
	pthread_cond_wait (&patienter, &mutex);
	printf("\nUsager %d ---> accède à la barrière", u_p->id);
	*/
}

//Fonction executee par chaque non abonne
void *fonc_non_abonne(int id_p)
{
	Usager u = initNonAbonne(id_p);
	printf("\n\nCréation Thread Abonne n°%d", u.id);
	printUsager(u);
}


//Fonction pour créer N abonne(s)
void create_threads()
{
	for (long i = 0; i < NB_ABONNE; ++i)
	{
		Usager u = initAbonne(i);
		if(pthread_create(tidUsager+i,0,(void *(*)())fonc_abonne, (void *) i) == -1)
		{
			debug("Creation thread abonne");
		}	
		usleep(10000);
	}

	for (long i = NB_ABONNE; i < NB_USAGER; ++i)
	{
		Usager u = initNonAbonne(i);
		if(pthread_create(tidUsager+i,0,(void *(*)())fonc_non_abonne, (void *) i) == -1)
		{
			debug("Creation thread non-abonne");
		}
		usleep(10000);
	}
}

//Fonction pour créer N abonne(s)
void end_threads()
{
	for(int i=0;i<NB_ABONNE;++i)
        	pthread_join(tidUsager[i],NULL);

	for(int i=NB_ABONNE;i<NB_USAGER;++i)
        	pthread_join(tidUsager[i],NULL);
}


int main(int argc, char const *argv[])
{
	
	int duree;
	if (argc >= 2)
	{
		// si au moins 1 argument
		duree = atoi(argv[1]);
		printf("\nduree choisit : %d\n", duree);
	}else{
		// si aucun argument
		duree = 10;
		printf("\nduree par defaut : %d\n", duree);
	}

	// Initialisation SIGINT //
	struct sigaction sa;
	sa.sa_handler = &handle_sigint;
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, NULL);

	// Initialisation Parking //
	PlaceParking parking[NUM_P];
	initParking(parking);

	create_threads();
	end_threads();


/*
	printf("\n\n");
	for (int i = 0; i <= 24*6; ++i)
	{
		usleep(1000000);
		printf("\r%d minutes ecoulees ...",i*10);
		fflush(stdout);
	}
*/

	printf("\n\nFIN PROG\n\n");
	return 0;
}