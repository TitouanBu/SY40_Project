#include "CodingFunctions.h"

pthread_mutex_t mutex;
pthread_cond_t patienterInt, patienterExt;
pthread_t tidUsager[NB_USAGER];

PlaceParking parking[NUM_P];

int countB = 0;
int nbAttInt = 0;
int nbAttExt = 0;

/*  Fonctions thread synchronisation

	pthread_cond_wait(&patienter, &mutex);
	pthread_cond_signal(&patienter);
	
	pthread_mutex_lock(&mutex);
	pthread_mutex_unlock(&mutex);
*/

bool isParkingPlein(Usager u_p)
{
	int i = 0;
	if(u_p.isAbonne){
		for(i = 0; i < NUM_P; i++)
		{
			if(parking[i].idUsager == -1)
				return false;
		}
	}else{
		for(i = NUM_P_ABONNE; i < NUM_P; i++)
		{
			if(parking[i].idUsager == -1)
				return false;
		}
	}
	return true;
}

void chercheStationnement(Usager* u_p)
{
	int i = 0;
	if(u_p->isAbonne){
		for(i = 0; i < NUM_P; i++)
		{
			if(parking[i].idUsager == -1)
				stationner(u_p, &parking[i]);
		}
	}else{
		for(i = NUM_P_ABONNE; i < NUM_P; i++)
		{
			if(parking[i].idUsager == -1)
				stationner(u_p, &parking[i]);
		}
	}
}

int circuler(Usager* usager_p)
{
	if(usager_p->stationnement == -1)
	{
		//Usager n'est pas garer, arret de la fonction
		//printf("\nUsager n°%d n'est pas gare!\n", usager_p->id);
		return 1;
	}

	parking[usager_p->stationnement].idUsager = -1;
	usager_p->stationnement = -1;
	return 0;
}

void usagerInterParking(Usager* u_p)
{
	/* Liberer le stationnement */
	circuler(u_p);
	printParking(parking);
	fflush(stdout);

	pthread_mutex_unlock(&mutex);
	// si déja quelqu'un à la barriere
	if(countB > 0)
	{
		nbAttInt++;
		printAction("Patiente", u_p->id, 1);
		pthread_cond_wait(&patienterInt, &mutex);
		nbAttInt--;
	}

	/* Passage de la barriere*/
	countB++; // accède à la barrière
	printAction("Accede à la barriere", u_p->id, 1);
	pthread_mutex_unlock(&mutex);
	sleep(1); //temps pour passer la barriere
	pthread_mutex_lock(&mutex);
	countB--; // s'eloigne de la barrière
	printAction("Sort du parking", u_p->id, 1);

	/* Signal pour l'usager suivant */
	// si un usager de l'interieur attend
	if(nbAttInt > 0)
	{
		pthread_mutex_unlock(&mutex);
		pthread_cond_signal(&patienterInt);
	}else{
		// si un usager de l'exterieur attend
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
	/* Le parking est-il plein? */
	if(isParkingPlein(*u_p))
	{
		printAction("Pas de place libre", u_p->id, 0);
		exit(0);
	}

	pthread_mutex_lock(&mutex);
	// si déja quelqu'un à la barriere
	if(countB > 0)
	{
		nbAttExt++;
		printAction("Patiente", u_p->id, 0);
		pthread_cond_wait(&patienterExt, &mutex);
		nbAttExt--;
	}

	/* Passage de la barriere*/
	countB++; // accède à la barrière
	printAction("Accede à la barriere", u_p->id, 0);
	pthread_mutex_unlock(&mutex);
	sleep(1); //temps pour passer la barriere
	pthread_mutex_lock(&mutex);
	countB--; // s'eloigne la barriere
	printAction("Entre dans le parking", u_p->id, 0);

	/* Se garer */
	// cherche un stationnement et se gare
	chercheStationnement(u_p);
	printParking(parking);
	fflush(stdout);

	/* Signal pour l'usager suivant */
	// si un usager de l'interieur attend
	if(nbAttInt > 0)
	{
		pthread_mutex_unlock(&mutex);
		pthread_cond_signal(&patienterInt);
	}else{
		// si un usager de l'exterieur attend
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

	sleep(2); //temps de stationnement sur la place de parking

	//usagerInterParking(&u);
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

	sleep(1);
	system("clear");

	// Initialisation SIGINT //
	struct sigaction sa;
	sa.sa_handler = &handle_sigint;
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, NULL);


	// Initialisation Parking //
	initParking(parking);

/*
	Usager u8 = initAbonne(8);
	modifUsager(&u8, false, 8);
	Usager u28 = initAbonne(28);
	modifUsager(&u28, true, 28);
	Usager u25 = initAbonne(25);
	modifUsager(&u25, false, 25);
	circuler(&u8);
	circuler(&u28);
	circuler(&u25);
	printParking(parking);
	if(isParkingPlein(u8)){
		printf("\n\n OUI \n");
	}else{
		printf("\n\n NON \n");
	}
	chercheStationnement(&u28);
	printParking(parking);
*/


	if(AFFICHE_ACTION){
		printf("\n\n\tExterieur Parking\t\tInterieur Parking\n");
	}
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