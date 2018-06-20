#ifndef EILE_H
#define EILE_H

#include "Eile_Data.h"

typedef struct Eile_info
{
	Eile_Data Data;
	struct Eile_info *Kitas;
} *Eile_info;

typedef struct Eile
{
	Eile_info Prad;
	char Eile_Yra;
} Eile;

int SukurtiEile(Eile *El);
int ArTusciaEile(Eile El);
int ArPilnaEile(Eile El);
int IterptiElementa(Eile *El, Eile_Data Naujas_Data);
int PasalintiElementa(Eile *El, Eile_Data *Pirmas_Data);
int PirmasElementas(Eile El, Eile_Data *Pirmas_Data);
int IstrintiEile(Eile *El);

#endif