#ifndef _CODING_FUNCTIONS_H
#define _CODING_FUNCTIONS_H

/*------Ajout des librairies nécessaires au projet------*/
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
/*--------------------------------------------------*/

#define false  0
#define true  1

/*------Mise en place des variables globales------*/
bool activeDebug=false;	//Variable pour afficher ou enlever les debugs
/*--------------------------------------------------*/

void debug (char *message)
{
	if (activeDebug)
	{
		printf("\n%s\n",message);
	}
}

#endif