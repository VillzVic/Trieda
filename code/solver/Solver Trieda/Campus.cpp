#include "Campus.h"
#include "Oferta.h"

Campus::Campus( void )
{
   totalSalas = 0;
   maiorSala  = 0;
   custo = 0;
   possuiAlunoFormando=false;
}

Campus::~Campus( void )
{
	// apagar clusters unidades
	auto it = clustersUnidades.begin();
	for(; it != clustersUnidades.end(); it++)
	{
		ConjUnidades *cjt = *it;
		//clustersUnidades.erase(it);
		if(cjt) delete cjt;
	}
	clustersUnidades.clear();

	// apagar unidades
	unidades.deleteElements();
	// apagar professores
	professores.deleteElements();
	// apagar horarios
	horarios.deleteElements();

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

/*
	Dado um id de curso e um id de disciplina, retorna as ofertas existentes
	no campus para o curso especificado, cujos currículos contêm a disciplina
	especificada.
*/
GGroup<Oferta*, LessPtr<Oferta>> Campus::retornaOfertasComCursoDisc( int idCurso, Disciplina *d )
{
	GGroup<Oferta*, LessPtr<Oferta>> ofts;
   
	//ITERA_GGROUP_LESSPTR( itOft, this->ofertas, Oferta )
	GGroup< Oferta *, LessPtr< Oferta > >::iterator itOft = this->ofertas.begin();
	for ( ; itOft != this->ofertas.end(); itOft++ )
	{
		Oferta *oft = *(itOft);
		if ( oft->getCursoId() == idCurso )
		{
			if ( oft->possuiDisciplina( d ) )
			{
				ofts.add( oft );
			}
		}

	}
	return ofts;
}

GGroup<Calendario*> Campus::getCalendarios()
{
   GGroup< Calendario * > calendarios;

   ITERA_GGROUP( itHor, this->horarios, Horario )
   {
	   calendarios.add( itHor->horario_aula->getCalendario() );	
   }

   return calendarios;
}