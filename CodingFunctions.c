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



/*------ Fonctions pour gestion de SIGINT ------*/

void handle_sigint(int sig){
	//end_threads();
	printf("\n\n----- Fin Simulation! -----\n");
	exit(0);
}

/*------ Fonctions pour les Usagers ------*/

// initialiser un abonne
Usager initAbonne(int id_p)
{
	Usager u;
	u.id = id_p;
	u.isAbonne = true;
	u.stationnement = -1;
	return u;
}

// initialiser un non abonne
Usager initNonAbonne(int id_p)
{
	Usager u;
	u.id = id_p;
	u.isAbonne = false;
	u.stationnement = -1;
	return u;
}

// modifier les caractéristiques d'un Usager
void modifUsager(Usager* u_p, bool isAbonne_p, int stationnement_p)
{
	u_p->isAbonne = isAbonne_p;
	u_p->stationnement = stationnement_p;
}

// afficher les caractéristiques d'un Usager
void printUsager(Usager u_p)
{
	printf("\nUsager n°%d :", u_p.id);

	if(u_p.isAbonne){
		printf("\n\tAbonne : Oui");
	}else{
		printf("\n\tAbonne : Non");
	}

	if(u_p.stationnement == -1){
		printf("\n\tStationnement : Non stationné");
	}else{
		printf("\n\tStationnement : place n°%d",u_p.stationnement);
	}
}


/*------ Fonctions pour le parking ------*/

// initialiser une place de parking
PlaceParking initPlaceParking(int id_p, bool isAbonne_p, int idUsager_p)
{
	PlaceParking pp;
	pp.id = id_p;
	pp.isAbonne = isAbonne_p;
	pp.idUsager = idUsager_p;
	return pp;
}

// initialiser un parking
void initParking(PlaceParking* parking_p)
{
	int i = 0;
	// initialisation au début du parking des places abonnés
	for(i = 0; i < NUM_P_ABONNE; i++)
	{
		//printf("\nInitialisation place n°%d",i);
		PlaceParking place = initPlaceParking(i, true, -1);
		parking_p[i] = place;
	}

	// initialisation en fin de parking des places non-abonnés
	for(i = NUM_P_ABONNE; i < NUM_P; i++)
	{
		//printf("\nInitialisation place n°%d",i);
		PlaceParking place = initPlaceParking(i, false, -1);
		parking_p[i] = place;
	}
}

// // afficher les caractéristiques d'une place de parking
void printPlaceParking(PlaceParking place_p)
{
	if(place_p.idUsager == -1){
		if(place_p.isAbonne){
			printf("   '|");
		}else{
			printf("    |");
		}
		
	}else{
		if(place_p.isAbonne){
			if(place_p.idUsager < 10){
				printf("U0%d'|", place_p.idUsager);
			}else{
				printf("U%d'|", place_p.idUsager);
			}
		}else{
			if(place_p.idUsager < 10){
				printf("U0%d |", place_p.idUsager);
			}else{
				printf("U%d |", place_p.idUsager);
			}
		}
	}
}

// afficher les caractéristiques de tout le parking
void printParking(PlaceParking* parking_p)
{
	if(AFFICHE_PARKING)
	{
		//system("clear");
		int count = 0;
		printf("\n  |");
		for(int i = 0; i < NUM_P; i++)
		{
			printPlaceParking(parking_p[i]);
			if(count == 9 && i < NUM_P-1){
				count = 0;
				printf("\n  |");
			}else{
				count++;
			}
		
		}
		printf("\n");
		fflush(stdout);
	}
}

// Permet à un Usager de se garer sur une place de parking si
// les conditions sont respectées
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

// Gere l'affichage des messages d'actions
void printAction(char* message_p, int id_p, int pos_p)
{
	if(AFFICHE_ACTION)
	{
		if(pos_p == 0){
			printf("\t\t\t\t\t\t\t\t\t\tU%d : %s\n", id_p, message_p);
		}

		if(pos_p == 1){
			printf("\t\t\t\t\t\t\t\t\t\t\t\t\t\t\tU%d : %s\n", id_p, message_p);
		}
	}
}

// Fonction pour attendre un temps aléatoire compris entre 0 et max secondes
void attente_aleatoire(int max)
{
	srand(time(NULL));
	int nbalea=rand()%max+1;
	sleep(nbalea);
}

// Fonction pour obtenir un boolean aléatoirement
bool boolean_aleatoire()
{
	srand(time(NULL));
	return rand() & 1;
}