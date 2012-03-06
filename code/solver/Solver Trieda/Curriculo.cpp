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
				/*disciplinas_periodo.add(
				std::make_pair( id_periodo, ( *it_disc ) ) );*/
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
	/*GGroup< std::pair< int, Disciplina * > >::iterator it = disciplinas_periodo.begin();

	for (; it != disciplinas_periodo.end(); it++ )
	{
		int id_periodo = ( *it ).first;
		int id_disciplina = ( *it ).second->getId();

		if ( id_disciplina == d->getId() )
			return id_periodo;
	}*/

	map < Disciplina*, int, LessPtr< Disciplina > >::iterator it = disciplinas_periodo.find(d);

	if(it != disciplinas_periodo.end())
		return it->second;

	return 0;
}


bool Curriculo::possuiDisciplina( Disciplina *d )
{
	/*GGroup< std::pair< int, Disciplina * > >::iterator it = disciplinas_periodo.begin();

	for (; it != disciplinas_periodo.end(); it++ )
	{
		if ( (*it).second->getId() == idDisciplina )
			return true;
   }*/

	map < Disciplina*, int, LessPtr< Disciplina > >::iterator it = disciplinas_periodo.find(d);

	if(it != disciplinas_periodo.end())
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