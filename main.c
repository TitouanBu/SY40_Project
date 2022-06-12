#include "CodingFunctions.h"

pthread_mutex_t mutex;
pthread_cond_t patienterInt, patienterExt;
pthread_t tidUsager[NB_USAGER];
pthread_t tidChrono;

PlaceParking parking[NUM_P];
Heure temps;

int countB = 0;
int nbAttInt = 0;
int nbAttExt = 0;

int nbPlaceLibreParking(Usager u_p)
{
	int i = 0, count = 0;
	if(u_p.isAbonne){
		for(i = 0; i < NUM_P; i++)
		{
			if(parking[i].idUsager == -1)
				count++;
		}
	}else{
		for(i = NUM_P_ABONNE; i < NUM_P; i++)
		{
			if(parking[i].idUsager == -1)
				count++;
		}
	}
	return count;
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
	printAction("Quitte sa place", u_p->id, 1);
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
	printAction("Se gare sur une place", u_p->id, 0);
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

	pthread_mutex_lock(&mutex);
	int cond = nbPlaceLibreParking(u) - nbAttExt - countB;
	if(cond <= 0){
		pthread_mutex_unlock(&mutex);
		printAction("Part (aucune place libre)", u.id, 0);
	}else{
		pthread_mutex_unlock(&mutex);
		usagerExterParking(&u);
		sleep(10); //temps de stationnement sur la place de parking
		usagerInterParking(&u);
	}
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

//Fonction thread du chrono
void *fonc_chrono()
{
	while(true)
	{
		sleep(1);
		temps.min += 10;
		if(temps.min == 60){
			temps.min = 00;
			temps.h++;
			if(temps.h == 24){
				temps.h = 0;
			}
		}

		//afficher chrono
		if(temps.min == 0){
			printf("%dh0%d\n", temps.h, temps.min);
		}else{
			printf("%dh%d\n", temps.h, temps.min);
		}	
	}
}

//Fonction pour créer N abonne(s)
void create_chrono()
{
	if(pthread_create(&tidChrono,0,(void *(*)())fonc_chrono,NULL) == -1)
	{
		debug("Creation thread chrono");
	}
}

//Fonction pour créer N abonne(s)
void end_chrono()
{
    pthread_join(tidChrono,NULL);
}

int main(int argc, char const *argv[])
{
	if (argc >= 2)
	{
		// si au moins 1 argument
		temps.h = atoi(argv[1]);
		temps.min = atoi(argv[2]);
		printf("\nduree choisit : %dh%d\n", temps.h, temps.min);
	}else{
		// si aucun argument
		temps.h = 14;
		temps.min = 0;
		printf("\nduree par defaut : %dh0%d\n", temps.h, temps.min);
	}

	// Initialisation SIGINT //
	struct sigaction sa;
	sa.sa_handler = &handle_sigint;
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, NULL);

	// Initialisation Parking //
	initParking(parking);

	//create_chrono();
	if(AFFICHE_ACTION){
		printf("\n\n\tExterieur Parking\t\tInterieur Parking\n");
	}
	create_threads();
	end_threads();
	//end_chrono();
	
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

/*
	Usager u0 = initAbonne(0);
	modifUsager(&u0, true, -1);
	chercheStationnement(&u0);
	circuler(&u1);
	printParking(parking);
	if(isParkingPlein(u3)){
		printf("\n\n OUI \n");
	}else{
		printf("\n\n NON \n");
	}
*/