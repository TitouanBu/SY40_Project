#include "CodingFunctions.h"

pthread_mutex_t mutex;
pthread_cond_t patienterInt, patienterExt;
pthread_t tidUsager[NB_USAGER];

int countB = 0;
int nbAttInt = 0;
int nbAttExt = 0;
PlaceParking parking[NUM_P];

/*  Fonctions thread synchronisation

	pthread_cond_wait(&patienter, &mutex);
	pthread_cond_signal(&patienter);
	
	pthread_mutex_lock(&mutex);
	pthread_mutex_unlock(&mutex);
*/

void usagerInterParking(Usager* u_p)
{
	pthread_mutex_unlock(&mutex);
	if(countB > 0)
	{
		nbAttInt++;
		printAction("Patiente", u_p->id, 1);
		pthread_cond_wait(&patienterInt, &mutex);
		nbAttInt--;
	}

	countB++; // accède à la barrière
	printAction("Accede à la barriere", u_p->id, 1);
	pthread_mutex_unlock(&mutex);
	sleep(2); //utilise un ticket ou le code
	pthread_mutex_lock(&mutex);
	countB--; // s'éloigne de la barrière
	printAction("Sort du parking", u_p->id, 1);

	if(nbAttInt > 0)
	{
		pthread_mutex_unlock(&mutex);
		pthread_cond_signal(&patienterInt);
	}else{
		if(nbAttExt > 0)
		{
			pthread_mutex_unlock(&mutex);
			pthread_cond_signal(&patienterExt);
		}else{
			pthread_mutex_unlock(&mutex);
		}
	}
}

void usagerExterParking(Usager* u_p)
{
	pthread_mutex_lock(&mutex);
	if(countB > 0)
	{
		nbAttExt++;
		printAction("Patiente", u_p->id, 0);
		pthread_cond_wait(&patienterExt, &mutex);
		nbAttExt--;
	}

	countB++; // accède à la barrière
	printAction("Accede à la barriere", u_p->id, 0);
	pthread_mutex_unlock(&mutex);
	sleep(2); //temps pour passer la barriere
	pthread_mutex_lock(&mutex);
	countB--;
	printAction("Entre dans le parking", u_p->id, 0);

	if(nbAttInt > 0)
	{
		pthread_mutex_unlock(&mutex);
		pthread_cond_signal(&patienterInt);
	}else{
		if(nbAttExt > 0)
		{
			pthread_mutex_unlock(&mutex);
			pthread_cond_signal(&patienterExt);
		}else{
			pthread_mutex_unlock(&mutex);
		}
	}
}

//Fonction executee par chaque abonne
void *fonc_usager(int id_p)
{
	Usager u;
	if(id_p < NB_ABONNE){
		u = initAbonne(id_p);
		//printf("\nCréation Thread Abonne n°%d", u.id);
	}else{
		u = initNonAbonne(id_p);
		//printf("\nCréation Thread non-Abonne n°%d", u.id);
	}

	usagerExterParking(&u);

	sleep(5); //temps de stationnement sur la place de parking

	usagerInterParking(&u);
}


//Fonction pour créer N abonne(s)
void create_threads()
{
	for (long i = 0; i < NB_ABONNE; ++i)
	{
		if(pthread_create(tidUsager+i,0,(void *(*)())fonc_usager, (void *) i) == -1)
		{
			debug("Creation thread abonne");
		}	
		usleep(10000);
	}

	for (long i = NB_ABONNE; i < NB_USAGER; ++i)
	{
		if(pthread_create(tidUsager+i,0,(void *(*)())fonc_usager, (void *) i) == -1)
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
	//initParking(parking);
	//printParking(parking);

	printf("\n\n\tExterieur Parking\t\tInterieur Parking\n");


	create_threads();
	end_threads();



	printf("\n\n----- Fin Simulation! -----\n");
	return 0;
}


/*
	printf("\n\n");
	for (int i = 0; i <= 24*6; ++i)
	{
		usleep(1000000);
		printf("\r%d minutes ecoulees ...",i*10);
		fflush(stdout);
	}
*/