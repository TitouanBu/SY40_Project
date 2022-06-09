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
	//end_threads(NB_ABONNE,NB_NABONNE);
	printf("\n\n----- Fin Programme! -----\n");
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
PlaceParking initPlaceParking(int id_p, bool isAbonne_p, bool isLibre_p)
{
	PlaceParking pp;
	pp.id = id_p;
	pp.isAbonne = isAbonne_p;
	pp.isLibre = isLibre_p;
	return pp;
}

// initialiser un parking
void initParking(PlaceParking* parking_p)
{
	int i = 0;
	for(i = 0; i < NUM_P_NABONNE; i++)
	{
		//printf("\nInitialisation place n°%d",i);
		PlaceParking place = initPlaceParking(i, false, true);
		parking_p[i] = place;
	}

	for(i = NUM_P_NABONNE; i < NUM_P; i++)
	{
		//printf("\nInitialisation place n°%d",i);
		PlaceParking place = initPlaceParking(i, true, true);
		parking_p[i] = place;
	}
}

// // afficher les caractéristiques d'une place de parking
void printPlaceParking(PlaceParking place_p)
{
	printf("\n\nPlace n°%d :", place_p.id);

	if(place_p.isAbonne){
		printf("\n\tPlace Abonne : Oui");
	}else{
		printf("\n\tPlace Abonne : Non");
	}

	if(place_p.isLibre){
		printf("\n\tEtat : Libre");
	}else{
		printf("\n\tEtat : Occupe");
	}
}

// afficher les caractéristiques de tout le parking
void printParking(PlaceParking* parking_p)
{
	for(int i = 0; i < NUM_P; i++)
	{
		printPlaceParking(parking_p[i]);
	}
}

// Permet à un Usager de se garer sur une place de parking si
// les conditions sont respectées
int seGarer(Usager* usager_p, PlaceParking* place_p)
{
	if(usager_p->stationnement != -1)
	{
		printf("\nUsager n°%d est déjà garer!\n", usager_p->id);
		return 1;
	}

	//si la place est libre
	if(place_p->isLibre)
	{
		// si place abonne, on verifie que usager est abonne
		if(!(place_p->isAbonne && !usager_p->isAbonne)){
			place_p->isLibre = false;
			usager_p->stationnement = place_p->id;
			return 0;
		}
	}
	return 1;
}