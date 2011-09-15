#include "Demanda.h"

Demanda::Demanda( void )
{
	oferta = NULL;
	disciplina = NULL;
   quantidade = -1;
}

Demanda::Demanda( Demanda const & demanda )
{
   quantidade = demanda.quantidade;
   disciplina_id = demanda.disciplina_id;
   oferta_id = demanda.oferta_id;

   oferta = demanda.oferta;
   disciplina = demanda.disciplina;
}

Demanda::~Demanda( void )
{

}

void Demanda::le_arvore( ItemDemanda & elem )
{
   this->setId( elem.id() );
	this->setQuantidade( elem.quantidade() );
	this->setDisciplinaId( elem.disciplinaId() );
	this->setOfertaId( elem.ofertaCursoCampiId() );
}

bool Demanda::operator < ( const Demanda& right )
{
   if ( quantidade < right.quantidade )
   {
      return true;
   }
   else if ( quantidade > right.quantidade )
   {
      return false;
   }

   if ( disciplina_id < right.disciplina_id )
   {
      return true;
   }
   else if ( disciplina_id > right.disciplina_id )
   {
      return false;
   }

   if ( oferta_id < right.oferta_id )
   {
      return true;
   }
   else if ( oferta_id > right.oferta_id )
   {
      return false;
   }

   return false;
}

bool Demanda::operator > ( const Demanda& right )
{
   if ( quantidade < right.quantidade )
   {
      return false;
   }
   else if ( quantidade > right.quantidade )
   {
      return true;
   }

   if ( disciplina_id < right.disciplina_id )
   {
      return false;
   }
   else if ( disciplina_id > right.disciplina_id )
   {
      return true;
   }

   if ( oferta_id < right.oferta_id )
   {
      return false;
   }
   else if ( oferta_id > right.oferta_id )
   {
      return true;
   }

   return false;
}

bool Demanda::operator == ( const Demanda & right )
{
   return ( ( quantidade == right.quantidade )
     && ( disciplina_id == right.disciplina_id )
     && ( oferta_id == right.oferta_id ) );
}

bool Demanda::operator != ( const Demanda & right )
{
   return !( *this == right );
}
