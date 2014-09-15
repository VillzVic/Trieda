#include "Unidade.h"
#include "ConjUnidades.h"

Unidade::Unidade( void )
	: conjunto(nullptr)
{
   maior_sala = 0;
}

Unidade::~Unidade( void )
{

}

void Unidade::le_arvore( ItemUnidade & elem )
{
   this->setId( elem.id() );
   codigo = elem.codigo();
   nome = elem.nome();

   ITERA_SEQ( it_salas, elem.salas(), Sala )
   {
      Sala * sala = new Sala();
      sala->le_arvore( *it_salas );
      salas.add( sala );
   }

   ITERA_SEQ( it_hora, elem.horariosDisponiveis(), Horario )
   {
      Horario * horario = new Horario();
      horario->le_arvore( *it_hora );
      horarios.add( horario );
   }
}

bool Unidade::possuiSala( int idSala )
{
	ITERA_GGROUP_LESSPTR( itSala, this->salas, Sala )
    {
		if ( itSala->getId() == idSala )
        {
			return true;
        }
	}
	return false;
	
}