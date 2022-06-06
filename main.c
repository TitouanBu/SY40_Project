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

	create_threads(NB_ABONNE,NB_NONABONNE);

	debug("test debug");

	printf("\n");
	for (int i = 0; i <= 24*6; ++i)
	{
		usleep(1000000);
		printf("\r%d minutes ecoulees ...",i*10);
		fflush(stdout);
	}

	printf("\n");
	return 0;
}