#include "Campus.h"

Campus::Campus( void )
{
   totalSalas = 0;
   maiorSala  = 0;
   custo = 0;
}

Campus::~Campus( void )
{

}

void Campus::le_arvore( ItemCampus & elem ) 
{
   this->setId( elem.id() );
   this->setCodigo( elem.codigo() );
   this->setNome( elem.nome() );
   this->setCusto( elem.custo() );

   ITERA_SEQ( it_unidades, elem.unidades(), Unidade )
   {
      Unidade * unidade = new Unidade();
      unidade->le_arvore( *it_unidades );
      unidades.add( unidade );
   }

   // O campo 'id_operacional_professor' é utilizado na classe
   // que representa a 'SolucaoOperacional', para acessar a matriz
   // da solução. O id_operacional deve ser preenchido de forma
   // sequencial, começando a partir de zero.
   int id_operacional_professor = 0;

   ITERA_SEQ( it_professores, elem.professores(), Professor )
   {
      Professor * professor = new Professor();

      professor->le_arvore( *it_professores );
      professor->setIdOperacional( id_operacional_professor );
      id_operacional_professor++;

      professores.add( professor );
   }

   ITERA_SEQ( it_horarios, elem.horariosDisponiveis(), Horario )
   {
      Horario * horario = new Horario();
      horario->le_arvore( *it_horarios );
      horarios.add( horario );
   }
}
