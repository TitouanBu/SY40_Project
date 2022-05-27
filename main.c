#include "headers/CodingFunctions.h"

int main(int argc, char const *argv[])
{
	int nombre_abonne,nombre_n_abonne;
	if(argc < 2){
		nombre_abonne = 10;
		nombre_n_abonne = 10;
	}else if(argc == 2){
		nombre_abonne = atoi(argv[1]);
		nombre_n_abonne = atoi(argv[1]);
	}else if (argc>2){
		nombre_abonne = atoi(argv[1]);
		nombre_n_abonne = atoi(argv[2]);
	}

	create_abonne(nombre_abonne);
	create_non_abonne(nombre_n_abonne);
	return 0;
}