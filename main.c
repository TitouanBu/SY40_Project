#include "headers/CodingFunctions.h"

int main(int argc, char const *argv[])
{
	int duree=10;

	if (argc > 2)
	{
		duree = atoi(argv[1]);
	}

	create_threads(numAbonne,numNAbonne);
	end_threads(numAbonne,numNAbonne);

	for (int i = 0; i <= 24*6; ++i)
	{
		usleep(1000000);
		printf("\r %d minutes ecoulees ...",i*10);
			fflush(stdout);
	}
	return 0;
}