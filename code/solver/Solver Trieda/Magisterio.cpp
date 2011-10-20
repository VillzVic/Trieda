#include "Magisterio.h"

Magisterio::Magisterio()
{
	disciplina = NULL;
}

Magisterio::~Magisterio( void )
{

}

void Magisterio::le_arvore( ItemProfessorDisciplina & elem )
{
	nota = elem.nota();
	preferencia = elem.preferencia();
	disciplina_id = elem.disciplinaId();
}

bool Magisterio::operator < ( const Magisterio & right ) const
{
   if ( disciplina_id < right.disciplina_id ) return true;
   if ( disciplina_id > right.disciplina_id ) return false;

   if ( nota < right.nota ) return true;
   if ( nota > right.nota ) return false;

   if ( preferencia < right.preferencia ) return true;
   if ( preferencia > right.preferencia ) return false;

   return false;
}

bool Magisterio::operator > ( const Magisterio & right ) const
{
   if ( disciplina_id < right.disciplina_id ) return false;
   if ( disciplina_id > right.disciplina_id ) return true;

   if ( nota < right.nota ) return false;
   if ( nota > right.nota ) return true;

   if ( preferencia < right.preferencia ) return false;
   if ( preferencia > right.preferencia ) return true;

   return false;
}

bool Magisterio::operator == ( const Magisterio & right ) const
{
   return (
      ( disciplina_id == right.disciplina_id ) && 
      ( nota == right.nota ) && 
      ( preferencia == right.preferencia ) );
}

bool Magisterio::operator != ( const Magisterio & right ) const
{
   return !( *this == right );
}