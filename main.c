#include "CodingFunctions.h"

pthread_mutex_t mutex;
pthread_cond_t patienterInt, patienterExt;
pthread_t* tidUsager;
pthread_t tidChrono;

PlaceParking* parking;
Heure temps;

int countB = 0;
int nbAttInt = 0;
int nbAttExt = 0;
int debordement = 0;
long numThread = 0;
int NUM_P_ABONNE = 2;
int NUM_P_NABONNE = 4;
int NUM_P;

/*------ Fonctions liées aux usagers et au parking ------*/

// retourne le nombre de place de parking libre pour un usager
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

// permet à un Usager de se garer sur une place de parking si les conditions sont respectées
int stationner(Usager* usager_p, PlaceParking* place_p)
{
	//si usager n'est pas déjà stationner
	if(usager_p->stationnement != -1)
	{
		//Usager deja garer, arret de la fonction
		//printf("\nUsager n°%d est déjà garer!\n", usager_p->id);
		return 1;
	}

	//si la place est libre
	if(place_p->idUsager == -1)
	{
		// si place abonne, on verifie que usager est abonne
		if(!(place_p->isAbonne && !usager_p->isAbonne)){
			place_p->idUsager = usager_p->id;
			usager_p->stationnement = place_p->id;
			return 0;
		}
	}
	return 1;
}

// permet à un usager de quitter sa place de parking
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

// permet à un usager de chercher une place libre puis de stationner dessus
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

/*------ Fonction du débordement ------*/

// calcul le debordement en fonction du nombre total de place du parking et de l'heure
// A 18h, le nombre de place "enlevée" par le débordement est égale à NUM_P/2
// A 19h, le nombre de place "enlevée" par le débordement est égale à NUM_P/3
// Ainsi de suite jusqu'à minuit puis debordement = 0 jusqu'à 18h le lendemain
int calculDebordement(int div)
{
	float deb = NUM_P;
	deb/=div;
	return (int) deb;
}

/*------ Fonctions thread Usager/Parking ------*/

// gère la relation usager-parking à l'intèrieur du parking
void usagerInterParking(Usager* u_p)
{
	/* Liberer le stationnement */
	circuler(u_p);
	printAction("Quitte sa place", u_p->id, 1, u_p->isAbonne);
	//printParking(parking);
	fflush(stdout);

	pthread_mutex_unlock(&mutex);
	// si déja quelqu'un à la barriere
	if(countB > 0)
	{
		nbAttInt++;
		printAction("Patiente", u_p->id, 1, u_p->isAbonne);
		pthread_cond_wait(&patienterInt, &mutex);
		nbAttInt--;
	}

	/* Passage de la barriere*/
	countB++; // accède à la barrière
	printAction("Accede à la barriere", u_p->id, 1, u_p->isAbonne);
	pthread_mutex_unlock(&mutex);
	sleep(1); //temps pour passer la barriere
	pthread_mutex_lock(&mutex);
	countB--; // s'eloigne de la barrière
	printAction("Sort du parking", u_p->id, 1, u_p->isAbonne);

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

// gère la relation usager-parking à l'extèrieur du parking
void usagerExterParking(Usager* u_p)
{

	pthread_mutex_lock(&mutex);
	// si déja quelqu'un à la barriere
	if(countB > 0)
	{
		nbAttExt++;
		printAction("Patiente", u_p->id, 0, u_p->isAbonne);
		pthread_cond_wait(&patienterExt, &mutex);
		nbAttExt--;
	}

	/* Passage de la barriere*/
	countB++; // accède à la barrière
	printAction("Accede à la barriere", u_p->id, 0, u_p->isAbonne);
	pthread_mutex_unlock(&mutex);
	sleep(1); //temps pour passer la barriere
	pthread_mutex_lock(&mutex);
	countB--; // s'eloigne la barriere
	printAction("Entre dans le parking", u_p->id, 0, u_p->isAbonne);

	/* Se garer */
	// cherche un stationnement et se gare
	chercheStationnement(u_p);
	printAction("Se gare sur une place", u_p->id, 0, u_p->isAbonne);
	//printParking(parking);
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

//Fonction thread usager
void *fonc_usager(void * arg)
{
	Usager u;
	long *tab = (long *) arg;
	long id_p = (long)tab[0];
	long isAbonne = (long)tab[1];

	if(isAbonne == 1){
		u = initAbonne(id_p);
		//printf("\nCréation Thread Abonne n°%d", u.id);
	}else{
		u = initNonAbonne(id_p);
		//printf("\nCréation Thread non-Abonne n°%d", u.id);
	}

	pthread_mutex_lock(&mutex);
	int cond = nbPlaceLibreParking(u) - nbAttExt - countB - debordement;
	if(cond <= 0){
		pthread_mutex_unlock(&mutex);
		printAction("Part (0 place ou trop de monde à la barrière)", u.id, 0, u.isAbonne);
	}else{
		pthread_mutex_unlock(&mutex);
		usagerExterParking(&u);
		//sleep(10); //temps de stationnement sur la place de parking
		attente_aleatoire(12);
		usagerInterParking(&u);
	}
}

//Fonction pour générer les threads usager 
void create_threads()
{
	long tab[2]={0};
	while(true){
		tab[0]=numThread;
		attente_aleatoire(3);
		if(boolean_aleatoire()){
			tab[1]=1;
			if(pthread_create(tidUsager+numThread,0,(void *(*)())fonc_usager, (void *) tab) == -1)
			{
				debug("Creation thread abonne");
			}
		}else{
			tab[1]=0;
			if(pthread_create(tidUsager+numThread,0,(void *(*)())fonc_usager, (void *) tab) == -1)
			{
				debug("Creation thread non-abonne");
			}
		}
		numThread++;
	}
}

//Fonction pour terminer les threads usager
void end_threads()
{
	for(int i=0;i<numThread;++i)
        	pthread_join(tidUsager[i],NULL);
}


/*------ Fonctions thread chrono ------*/

//Fonction du thread du chrono
void *fonc_chrono()
{
	while(true)
	{
		sleep(1);
		temps.min += 10;
		if(temps.min >= 60){
			temps.min = 00;
			temps.h++;
			if(temps.h >= 24){
				temps.h = 0;
			}
		}

		if(temps.min == 0){
			printf("\n\n\r%dh0%d\n", temps.h, temps.min);
			fflush(stdout);
		}else{
			printf("\n\n\r%dh%d\n", temps.h, temps.min);
			fflush(stdout);
		}
		if (temps.h == 18)
		{
			debordement = calculDebordement(2);
		}else if(temps.h==19){
			debordement = calculDebordement(3);
		}else if(temps.h==20){
			debordement = calculDebordement(4);
		}else if(temps.h==21){
			debordement = calculDebordement(5);
		}else if(temps.h==22){
			debordement = calculDebordement(6);
		}else if(temps.h==23){
			debordement = calculDebordement(7);
		}else{
			debordement = 0;
		}
		printf("debordement : %d, Place Parking : %d\n",debordement, NUM_P);
		printParking(parking, NUM_P);		
	}
}

//Fonction pour créer le thread chrono
void create_chrono()
{
	if(pthread_create(&tidChrono,0,(void *(*)())fonc_chrono,NULL) == -1)
	{
		debug("Creation thread chrono");
	}
}

//Fonction pour terminer le thread chrono
void end_chrono()
{
    pthread_join(tidChrono,NULL);
}

/*------ Fonctions pour gestion de SIGINT ------*/

void handle_sigint(int sig){
	//end_threads();
	//end_chrono();
	free(parking);
	free(tidUsager);
	printf("\n\n----- Fin Simulation! -----\n");
	exit(0);
}

/*---------------- Main ----------------*/

int main(int argc, char const *argv[])
{
	if (argc >= 2)
	{
		if(atoi(argv[1])>0){
			NUM_P_ABONNE = atoi(argv[1]);
		}
		if (argc >= 3)
		{
			if(atoi(argv[2])>0){
				NUM_P_NABONNE = atoi(argv[2]);
			}
			if (argc >= 4)
			{
				if (atoi(argv[3])>= 0 && atoi(argv[3])<24)
				{
					temps.h = atoi(argv[3]);
				}
				if (argc >= 5)
				{
					if (atoi(argv[4])>= 0 && atoi(argv[4])<24)
					{
						temps.min = atoi(argv[3]);
					}
				}
			}else{
				temps.h = 14;
				temps.min = 0;
			}
		}
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
	system("clear");

	NUM_P = NUM_P_ABONNE + NUM_P_NABONNE;
	parking = malloc(sizeof(PlaceParking) * NUM_P);
	tidUsager = malloc(sizeof(pthread_t) * NUM_P);
	// Initialisation Parking //
	initParking(parking,NUM_P_ABONNE,NUM_P);
	create_chrono();
	if(AFFICHE_ACTION){
		printf("\t\t\t\t\t\t\t\t\t\tExterieur du parking\t\t\tInterieur du parking");
	}
	while(true){
		create_threads();
		end_threads();
	}
	end_chrono();
	
	printf("\n\n----- Fin Simulation! -----\n");
	return 0;
}
