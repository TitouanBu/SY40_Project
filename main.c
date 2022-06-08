#include "CodingFunctions.h"


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
	
	Usager user0 = initUsager(0);
	Usager user1 = initUsager(1);
	modifUsager(&user0, true, -1);
	modifUsager(&user1, false, -1);
	seGarer(&user0, &parking[7]);
	seGarer(&user1, &parking[8]);

	printParking(parking);
	


	//create_threads(NB_ABONNE,NB_NONABONNE);
/*
	printf("\n\n");
	for (int i = 0; i <= 24*6; ++i)
	{
		usleep(1000000);
		printf("\r%d minutes ecoulees ...",i*10);
		fflush(stdout);
	}
*/

	//end_threads(NB_ABONNE,NB_NABONNE);
	printf("\n\n");
	return 0;
}