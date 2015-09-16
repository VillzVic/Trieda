#include "DivisaoCreditos.h"

int DivisaoCreditos::fakeId = 0;

DivisaoCreditos::DivisaoCreditos(void)
{
}

DivisaoCreditos::DivisaoCreditos(DivisaoCreditos const & div_Creds)
{
   this->setId( div_Creds.getId() );
   this->creditos = div_Creds.creditos;

   for(unsigned d = 0; d < 8; ++d)
   {
	   this->dia[d] = div_Creds.dia[d];
   }
}

DivisaoCreditos::DivisaoCreditos( int dia1, int dia2, int dia3, int dia4, int dia5, int dia6, int dia7, int idValue )
{
   this->setId( idValue );
   creditos = dia1 + dia2 + dia3 + dia4 + dia5 + dia6 + dia7;
   dia[0] = dia7;
   dia[1] = dia1;
   dia[2] = dia2;
   dia[3] = dia3;
   dia[4] = dia4;
   dia[5] = dia5;
   dia[6] = dia6;
   dia[7] = dia7;
}

DivisaoCreditos::~DivisaoCreditos(void)
{
}

void DivisaoCreditos::le_arvore(ItemDivisaoCreditos& elem)
{
   this->setId( elem.id() );
   creditos = elem.creditos();
   dia[0] = elem.dia7();
   dia[1] = elem.dia1();
   dia[2] = elem.dia2();
   dia[3] = elem.dia3();
   dia[4] = elem.dia4();
   dia[5] = elem.dia5();
   dia[6] = elem.dia6();
   dia[7] = elem.dia7();
}
