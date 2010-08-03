#include "DivisaoCreditos.h"

DivisaoCreditos::DivisaoCreditos(void)
{
}

DivisaoCreditos::~DivisaoCreditos(void)
{
}

void DivisaoCreditos::le_arvore(ItemDivisaoCreditos& elem)
{
   id = elem.id();
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
