#include "Curriculo.h"

Curriculo::Curriculo( void )
{

}

Curriculo::~Curriculo( void )
{

}

void Curriculo::le_arvore( ItemCurriculo & elem )
{
	this->setId( elem.id() );
    this->setSemanaLetivaId( elem.semanaLetivaId() );
	this->setCodigo( elem.codigo() );
		
	ITERA_SEQ( it_dp, elem.disciplinasPeriodo(), DisciplinaPeriodo )
	{
		int periodo = it_dp->periodo();
		int disc_id = it_dp->disciplinaId();
			
		this->ids_disciplinas_periodo.add( std::make_pair( periodo, disc_id ) );

		periodos.add( periodo );
		
		ITERA_SEQ( it_corr, it_dp->disciplinasCoRequisito(), DisciplinaCoRequisito )
		{			
			disciplinasId_correquisitos[periodo][disc_id].insert( it_corr->disciplinaCoRequisitoId() );
		}
	}
}

void Curriculo::refDisciplinaPeriodo( GGroup< Disciplina *, LessPtr< Disciplina > > disciplinas )
{
	GGroup< std::pair< int, int > >::iterator
		it_ids = ids_disciplinas_periodo.begin();
	
	for (; it_ids != ids_disciplinas_periodo.end(); it_ids++ )
	{
		int id_periodo = ( *it_ids ).first;
		int id_disciplina = ( *it_ids ).second;
		
		GGroup< Disciplina *, LessPtr< Disciplina > >::iterator
			it_disc = disciplinas.begin();

		for (; it_disc != disciplinas.end(); it_disc++ )
		{
			if ( it_disc->getId() == id_disciplina )
			{
				disciplinas_periodo[*it_disc] = id_periodo;

				break;
			}
		}
	}

	ids_disciplinas_periodo.clear();
}

int Curriculo::getMaxCreds( int dia )
{
	map<int, int>::iterator it = mapMaxCreds.find(dia);
	if(it == mapMaxCreds.end())
	{
		int cred = calendario->getNroDeHorariosAula(dia);
		mapMaxCreds[dia] = cred;
		return cred;
	}
	else
		return it->second;
}


int Curriculo::getPeriodo( Disciplina *d )
{
	map < Disciplina*, int, LessPtr< Disciplina > >::iterator it = disciplinas_periodo.find(d);

	if(it != disciplinas_periodo.end())
		return it->second;
	return -1;
}

int Curriculo::getPeriodoEquiv( Disciplina *d )
{
	map < Disciplina*, int, LessPtr< Disciplina > >::iterator it = disciplinas_periodo.find(d);

	if(it != disciplinas_periodo.end())
		return it->second;

	ITERA_GGROUP_LESSPTR( itDisc, d->discEquivalentes, Disciplina )
	{		
		map < Disciplina*, int, LessPtr< Disciplina > >::iterator it = disciplinas_periodo.find( *itDisc );

		if ( it != disciplinas_periodo.end() )
			return it->second;
	}

	return -1;
}

bool Curriculo::possuiDisciplina( Disciplina *d )
{
	map < Disciplina*, int, LessPtr< Disciplina > >::iterator it = disciplinas_periodo.find(d);

	if(it != disciplinas_periodo.end())
		return true;

	if ( possuiDisciplinaComoEquiv( d->getId() ) )
		return true;

	return false;
}

GGroup< Calendario*, LessPtr<Calendario> > Curriculo::retornaSemanasLetivasNoPeriodo( int periodo )
{
	GGroup< Calendario*, LessPtr<Calendario> > calendarios;

	map< int, GGroup< Calendario*, LessPtr<Calendario> > >::iterator it = semanasLetivas.find(periodo);

	if(it != semanasLetivas.end())
		return it->second;

	return calendarios;
}

 bool Curriculo::possuiDisciplinaComoEquiv( int deq_id )
 {
	 if ( ids_discs_substitutas.find( deq_id ) != ids_discs_substitutas.end() )
		 return true;
	 else
		 return false;
 }

bool Curriculo::possuiDiscEmPeriodo( int periodo )
{
	map < Disciplina*, int, LessPtr< Disciplina > >::iterator 
		itDiscPeriod = disciplinas_periodo.begin();

	for ( ; itDiscPeriod != disciplinas_periodo.end(); itDiscPeriod++ )
	{
		if ( itDiscPeriod->second == periodo )
			return true;
	}
	return false;
}

bool Curriculo::possuiCorrequisito( int disciplinaId )
{
   auto itGrades = disciplinasId_correquisitos.begin();
   for ( ; itGrades != disciplinasId_correquisitos.end(); itGrades++ )
   {
	   auto itDisc = itGrades->second.find(disciplinaId);
	   if ( itDisc != itGrades->second.end() )
	   {
		   if ( itDisc->second.size() > 0 )
			   return true;
	   }
   }
   return false;
}

bool Curriculo::possuiCorrequisito( int disciplinaId, int periodo )
{
   auto itGrades = disciplinasId_correquisitos.find(periodo);
   if ( itGrades != disciplinasId_correquisitos.end() )
   {
	   auto itDisc = itGrades->second.find(disciplinaId);
	   if ( itDisc != itGrades->second.end() )
	   {
		   if ( itDisc->second.size() > 0 )
			   return true;
	   }
   }
   return false;
}

void Curriculo::getCorrequisitos( int disciplinaId, set<int> &correquisitos )
{
	correquisitos.clear();

   auto itGrades = disciplinasId_correquisitos.begin();
   for ( ; itGrades != disciplinasId_correquisitos.end(); itGrades++ )
   {
	   auto itDisc = itGrades->second.find(disciplinaId);
	   if ( itDisc != itGrades->second.end() )
	   {
		   correquisitos = itDisc->second;
	   }
   }
}

void Curriculo::getCorrequisitos( int disciplinaId, int periodo, set<int> &correquisitos )
{
	correquisitos.clear();

   auto itGrades = disciplinasId_correquisitos.find(periodo);
   if ( itGrades != disciplinasId_correquisitos.end() )
   {
	   auto itDisc = itGrades->second.find(disciplinaId);
	   if ( itDisc != itGrades->second.end() )
	   {
		   correquisitos = itDisc->second;
	   }
   }
}




//bool Curriculo::possuiCorrequisito( int disciplinaId )
//{
//   auto itGrades = grades.begin();
//   for ( ; itGrades != grades.end(); itGrades++ )
//   {
//	   auto itDisc = itGrades->second.disciplina_correquisitos.find(disciplinaId);
//	   if ( itDisc != itGrades->second.disciplina_correquisitos.end() )
//	   {
//		   if ( itDisc->second.size() > 0 )
//			   return true;
//	   }
//   }
//   return false;
//}
//
//bool Curriculo::possuiCorrequisito( int disciplinaId, int periodo )
//{
//   auto itGrades = grades.find(periodo);
//   if ( itGrades != grades.end() )
//   {
//	   auto itDisc = itGrades->second.disciplina_correquisitos.find(disciplinaId);
//	   if ( itDisc != itGrades->second.disciplina_correquisitos.end() )
//	   {
//		   if ( itDisc->second.size() > 0 )
//			   return true;
//	   }
//   }
//   return false;
//}
//
//void Curriculo::getCorrequisitos( int disciplinaId, set<int> &correquisitos )
//{
//	correquisitos.clear();
//
//   auto itGrades = grades.begin();
//   for ( ; itGrades != grades.end(); itGrades++ )
//   {
//	   auto itDisc = itGrades->second.disciplina_correquisitos.find(disciplinaId);
//	   if ( itDisc != itGrades->second.disciplina_correquisitos.end() )
//	   {
//		   correquisitos = itDisc->second;
//	   }
//   }
//}
//
//void Curriculo::getCorrequisitos( int disciplinaId, int periodo, set<int> &correquisitos )
//{
//	correquisitos.clear();
//
//   auto itGrades = grades.find(periodo);
//   if ( itGrades != grades.end() )
//   {
//	   auto itDisc = itGrades->second.disciplina_correquisitos.find(disciplinaId);
//	   if ( itDisc != itGrades->second.disciplina_correquisitos.end() )
//	   {
//		   correquisitos = itDisc->second;
//	   }
//   }
//}
//

