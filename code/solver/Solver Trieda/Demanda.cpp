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

bool Demanda::operator < ( const Demanda & right ) const
{
   if ( this->getId() < right.getId() )
   {
      return true;
   }
   else if ( this->getId() > right.getId() )
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

   if ( quantidade < right.quantidade )
   {
      return true;
   }
   else if ( quantidade > right.quantidade )
   {
      return false;
   }

   return false;
}

bool Demanda::operator > ( const Demanda & right ) const
{
   if ( this->getId() < right.getId() )
   {
      return false;
   }
   else if ( this->getId() > right.getId() )
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

   if ( quantidade < right.quantidade )
   {
      return false;
   }
   else if ( quantidade > right.quantidade )
   {
      return true;
   }

   return false;
}

bool Demanda::operator == ( const Demanda & right ) const
{
	return ( !( *this < right ) && !( right < *this ) );
}

bool Demanda::operator != ( const Demanda & right ) const
{
   return !( *this == right );
}

bool Demanda::ehSubstitutaPossivel( int discId )
{
	map<int,Disciplina*>::iterator itFinder = discIdSubstitutasPossiveis.find(discId);
	if ( itFinder != discIdSubstitutasPossiveis.end() )
		return true;
	return false;
}

void Demanda::addSubstitutaPossivel( Disciplina* discSubstituta )
{
	discIdSubstitutasPossiveis[discSubstituta->getId()] = discSubstituta;
}

// preencher set com disciplina original + substitutas
void Demanda::getAllDiscPossiveis (unordered_set<Disciplina*> &disciplinas) const
{
	// inserir original
	disciplinas.insert(disciplina);

	// inserir substitutas
	for(auto it = discIdSubstitutasPossiveis.cbegin(); it != discIdSubstitutasPossiveis.cend(); ++it)
		disciplinas.insert(it->second);
}