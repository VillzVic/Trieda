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
   this->setId( elem.id() );
   nome = elem.nome();
}