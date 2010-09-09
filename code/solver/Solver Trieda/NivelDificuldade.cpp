#include "NivelDificuldade.h"
#include <iostream>

NivelDificuldade::NivelDificuldade(void)
{
}

NivelDificuldade::~NivelDificuldade(void)
{
}

void NivelDificuldade::le_arvore(ItemNivelDificuldade& elem) 
{
   id = elem.id();
   nome = elem.nome();
}