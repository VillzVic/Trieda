#include "TipoCurso.h"

TipoCurso::TipoCurso( void )
{

}

TipoCurso::~TipoCurso( void )
{

}

void TipoCurso::le_arvore( ItemTipoCurso & elem )
{
   this->setId( elem.id() );
   this->nome = elem.nome();
}
