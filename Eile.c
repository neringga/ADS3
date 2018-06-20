#include <stdio.h>
#include <stdlib.h>

#include "Eile.h"
#include "Eile_Data.h"

int SukurtiEile(Eile *El)
{
	//if (El->Eile_Yra == 1) return 1;
	
	El->Prad = NULL;
	El->Eile_Yra = 1;
	return 0;
}


int ArTusciaEile(Eile El)
{
	if (El.Eile_Yra != 1) return 1;
	
	if (El.Prad == NULL) return 2;
	else return 0;
}
int IterptiElementa(Eile *El, Eile_Data Naujas_Data)
{
	Eile_info Laik_ptr, Naujas = malloc(sizeof(struct Eile_info));
	if (Naujas == NULL) return 1;
	if (El->Eile_Yra != 1) return 1;
	
	Naujas->Data = Naujas_Data;
	
	Laik_ptr = El->Prad;
	if (El->Prad == NULL) 
	{
		Naujas->Kitas = El->Prad;
		El->Prad = Naujas;
		return 0;
	}

	while(Laik_ptr->Kitas != NULL)
	{
		Laik_ptr = Laik_ptr->Kitas;
	}
		
	Laik_ptr->Kitas = Naujas;
	Naujas->Kitas = NULL;
	return 0;
}

int PasalintiElementa(Eile *El, Eile_Data *Pirmas_Data)
{
	if (El->Eile_Yra != 1) return 1;
	if (El->Prad == NULL) return 2;
	
	Eile_info Laik_ptr = El->Prad;

	*Pirmas_Data = Laik_ptr->Data;

	El->Prad = Laik_ptr->Kitas;
	free(Laik_ptr);
	return 0;
}
int PirmasElementas(Eile El, Eile_Data *Pirmas_Data)
{
	if (El.Eile_Yra != 1) return 1;
	if (El.Prad == NULL) return 2;
	
	*Pirmas_Data = El.Prad->Data;
	return 0;
}
int ArPilnaEile(Eile El)
{
	if (El.Eile_Yra != 1) return 1;
	
	Eile_info Laik_ptr = malloc(sizeof(struct Eile_info));
	if (Laik_ptr == NULL) return 3;
	else 
	{ 
		free(Laik_ptr); 
		return 0; 
	}
}

int IstrintiEile(Eile *El)
{
	if (El->Eile_Yra != 1) return 1;
	
	Eile_info Laik_ptr;
	while (El->Prad != NULL)
	{
		Laik_ptr = El->Prad;
		El->Prad = Laik_ptr->Kitas;
		free(Laik_ptr);
	}
	El->Eile_Yra = 0;
	return 0;
}