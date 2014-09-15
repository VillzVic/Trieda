#include "SolucaoTaticoInicial.h"

#include "VariablePre.h"
#include "VariableTatInt.h"
#include "VariableTatico.h"
#include <fstream>

#include "CentroDados.h"

SolucaoTaticoInicial::SolucaoTaticoInicial() 
{
}

SolucaoTaticoInicial::~SolucaoTaticoInicial() {}


void SolucaoTaticoInicial::addAula( int campusId, Disciplina* disciplina, int turma, Aula* aula )
{ 
	mapAulas[campusId][disciplina][turma].add(aula);

	VariablePre vo;
    vo.reset();
    vo.setType( VariablePre::V_PRE_OFERECIMENTO );		// o_{i,d,s}
    vo.setTurma( turma );								// i
    vo.setDisciplina( disciplina );						// d
	vo.setUnidade( aula->getUnidade() );				// u
    vo.setSubCjtSala( aula->getCjtSala() );				// tps
	addInSol( vo );
			
	VariableTatInt vx;
	vx.reset();
	vx.setType( VariableTatInt::V_CREDITOS );							// x_{i,d,s,t,hi,hf}
	vx.setTurma( turma );												// i
	vx.setDisciplina( disciplina );										// d
	vx.setUnidade( aula->getUnidade() );								// u
	vx.setSubCjtSala( aula->getCjtSala() );								// tps  
	vx.setDia( aula->getDiaSemana() );									// t
	vx.setHorarioAulaInicial( aula->getHorarioAulaInicial() );			// hi
	vx.setHorarioAulaFinal( aula->getHorarioAulaFinal() );				// hf
	vx.setDateTimeInicial( aula->getDateTimeInicial() );				// dti
	vx.setDateTimeFinal( aula->getDateTimeFinal() );					// dtf
	addInSol( vx );
}

void SolucaoTaticoInicial::addAlunoDem( Campus *campus, Disciplina* disciplina, int turma, AlunoDemanda* alunoDemanda, bool fixar )
{ 
	mapCampusDiscTurma_AlunosDemanda[campus->getId()][disciplina][turma].add(alunoDemanda); 

	if ( fixar )
	{
		if ( nroAlunosDemandaAtendidos.find( campus->getId() ) == nroAlunosDemandaAtendidos.end() )
			nroAlunosDemandaAtendidos[campus->getId()] = 1;
		else
			nroAlunosDemandaAtendidos[campus->getId()] ++;
	}

	VariablePre vs;
	vs.reset();
	vs.setType( VariablePre::V_PRE_ALOCA_ALUNO_TURMA_DISC );		// s_{a,i,d,cp}
	vs.setAluno( alunoDemanda->getAluno() );						// a
	vs.setDisciplina( disciplina );									// d
	vs.setTurma( turma );											// i
	vs.setCampus( campus );											// cp
	addInSol( vs );

	VariableTatInt vts;
	vts.reset();
	vts.setType( VariableTatInt::V_ALOCA_ALUNO_TURMA_DISC );		// s_{a,i,d,cp}
	vts.setAluno( alunoDemanda->getAluno() );						// a
	vts.setDisciplina( disciplina );								// d
	vts.setAlunoDemanda( alunoDemanda );							// alDem
	vts.setTurma( turma );											// i
	vts.setCampus( campus );										// cp
	addInSol( vts );
}

void SolucaoTaticoInicial::addAlunosDem( Campus *campus, Disciplina* disciplina, int turma, GGroup<AlunoDemanda*, LessPtr<AlunoDemanda>> alunosDemanda, bool fixar )
{ 
	int old_size = mapCampusDiscTurma_AlunosDemanda[campus->getId()][disciplina][turma].size();

	mapCampusDiscTurma_AlunosDemanda[campus->getId()][disciplina][turma].add(alunosDemanda);

	int dif = mapCampusDiscTurma_AlunosDemanda[campus->getId()][disciplina][turma].size() - old_size;
	
	if ( fixar )
	{
		if ( nroAlunosDemandaAtendidos.find( campus->getId() ) == nroAlunosDemandaAtendidos.end() )
			nroAlunosDemandaAtendidos[campus->getId()] = dif;
		else
			nroAlunosDemandaAtendidos[campus->getId()] += dif;
	}

	ITERA_GGROUP_LESSPTR( itAlDem, alunosDemanda, AlunoDemanda )
	{
		VariablePre vs;
		vs.reset();
		vs.setType( VariablePre::V_PRE_ALOCA_ALUNO_TURMA_DISC );	// s_{a,i,d,cp}
		vs.setAluno( itAlDem->getAluno() );							// a
		vs.setDisciplina( disciplina );								// d
		vs.setTurma( turma );										// i
		vs.setCampus( campus );										// cp
		addInSol( vs );

		VariableTatInt vts;
		vts.reset();
		vts.setType( VariableTatInt::V_ALOCA_ALUNO_TURMA_DISC );		// s_{a,i,d,cp}
		vts.setAluno( itAlDem->getAluno() );							// a
		vts.setDisciplina( disciplina );								// d
	    vts.setAlunoDemanda( *itAlDem );								// alDem
		vts.setTurma( turma );											// i
		vts.setCampus( campus );										// cp
		addInSol( vts );
	}
}

void SolucaoTaticoInicial::addAlunoTurma( int campusId, Disciplina* disciplina, int turma, Aluno* aluno, bool fixar )
{ 
	mapAluno_CampusTurmaDisc[aluno][campusId][disciplina] = std::make_pair(turma,fixar); 
}

void SolucaoTaticoInicial::removeAula( int campusId, Disciplina* disciplina, int turma, Aula *aula )
{	   
	std::map< int /*campusId*/, std::map< Disciplina*, std::map< int /*turma*/, GGroup<Aula*, LessPtr<Aula>> >,
		LessPtr<Disciplina> > >::iterator itMap1 = mapAulas.find( campusId );
	if ( itMap1 != mapAulas.end() )
	{
		std::map< Disciplina*, std::map< int /*turma*/, GGroup<Aula*, LessPtr<Aula>> >,
			LessPtr<Disciplina> > *map2 = & itMap1->second;

		std::map< Disciplina*, std::map< int /*turma*/, GGroup<Aula*, LessPtr<Aula>> >,
			LessPtr<Disciplina> >::iterator itMap2 = (*map2).find( disciplina );
		if ( itMap2 != (*map2).end() )
		{
			std::map< int /*turma*/, GGroup<Aula*, LessPtr<Aula>> > *map3 = & itMap2->second;

			std::map< int /*turma*/, GGroup<Aula*, LessPtr<Aula>> >::iterator itMap3 = (*map3).find( turma );
			if ( itMap3 != (*map3).end() )
			{
				GGroup<Aula*, LessPtr<Aula>> *aulas = & itMap3->second;
				GGroup<Aula*, LessPtr<Aula>>::iterator itAula = (*aulas).find( aula );
				if ( itAula != (*aulas).end() )
				{
					(*aulas).remove( itAula );
					
					VariablePre vo;
					vo.reset();
					vo.setType( VariablePre::V_PRE_OFERECIMENTO );		// o_{i,d,s}
					vo.setTurma( turma );								// i
					vo.setDisciplina( disciplina );						// d
					vo.setUnidade( aula->getUnidade() );				// u
					vo.setSubCjtSala( aula->getCjtSala() );				// tps
					removeFromSol( vo );
			
					VariableTatInt vx;
					vx.reset();
					vx.setType( VariableTatInt::V_CREDITOS );							// x_{i,d,s,t,hi,hf}
					vx.setTurma( turma );												// i
					vx.setDisciplina( disciplina );										// d
					vx.setUnidade( aula->getUnidade() );								// u
					vx.setSubCjtSala( aula->getCjtSala() );								// tps  
					vx.setDia( aula->getDiaSemana() );									// t
					vx.setHorarioAulaInicial( aula->getHorarioAulaInicial() );			// hi
					vx.setHorarioAulaFinal( aula->getHorarioAulaFinal() );				// hf
					vx.setDateTimeInicial( aula->getDateTimeInicial() );				// dti
					vx.setDateTimeFinal( aula->getDateTimeFinal() );					// dtf
					removeFromSol( vx );
				}
			}			
		}
	}
}
   
bool SolucaoTaticoInicial::existeSolInicial()
{ 
	return mapAulas.size();
}
   
std::map< int /*campusId*/, std::map< Disciplina*, std::map< int /*turma*/, GGroup<Aula*, LessPtr<Aula>> >,
	   LessPtr<Disciplina> > >* SolucaoTaticoInicial::getPtMapAulas()
{
	std::map< int /*campusId*/, std::map< Disciplina*, std::map< int /*turma*/, GGroup<Aula*, LessPtr<Aula>> >,
	   LessPtr<Disciplina> > >* pt = & mapAulas;

	return pt;
}

std::map< int /*campusId*/, std::map< Disciplina*, std::map< int /*turma*/, GGroup<AlunoDemanda*, LessPtr<AlunoDemanda>> >,
	   LessPtr<Disciplina> > >* SolucaoTaticoInicial::getPtMapTurmasAlsDem()
{
	std::map< int /*campusId*/, std::map< Disciplina*, std::map< int /*turma*/, GGroup<AlunoDemanda*, LessPtr<AlunoDemanda>> >,
	   LessPtr<Disciplina> > >* pt = & mapCampusDiscTurma_AlunosDemanda;

	return pt;
}

GGroup<Aula*, LessPtr<Aula>> SolucaoTaticoInicial::getAulas( int campusId, Disciplina* disciplina, int turma )
{
	GGroup<Aula*, LessPtr<Aula>> aulas;

	std::map< int /*campusId*/, std::map< Disciplina*, std::map< int /*turma*/, GGroup<Aula*, LessPtr<Aula>> >,
		LessPtr<Disciplina> > >::iterator itMap1 = mapAulas.find( campusId );
	if ( itMap1 != mapAulas.end() )
	{
		std::map< Disciplina*, std::map< int /*turma*/, GGroup<Aula*, LessPtr<Aula>> >,
			LessPtr<Disciplina> > *map2 = & itMap1->second;

		std::map< Disciplina*, std::map< int /*turma*/, GGroup<Aula*, LessPtr<Aula>> >,
			LessPtr<Disciplina> >::iterator itMap2 = (*map2).find( disciplina );
		if ( itMap2 != (*map2).end() )
		{
			std::map< int /*turma*/, GGroup<Aula*, LessPtr<Aula>> > *map3 = & itMap2->second;

			std::map< int /*turma*/, GGroup<Aula*, LessPtr<Aula>> >::iterator itMap3 = (*map3).find( turma );
			if ( itMap3 != (*map3).end() )
			{
				aulas = itMap3->second;
			}			
		}
	}
	return aulas;
}

Aula* SolucaoTaticoInicial::getAula( int campusId, Disciplina* disciplina, int turma, int diaSemana, Sala* sala )
{
	Aula* aula=NULL;

	std::map< int /*campusId*/, std::map< Disciplina*, std::map< int /*turma*/, GGroup<Aula*, LessPtr<Aula>> >,
		LessPtr<Disciplina> > >::iterator itMap1 = mapAulas.find( campusId );
	if ( itMap1 != mapAulas.end() )
	{
		std::map< Disciplina*, std::map< int /*turma*/, GGroup<Aula*, LessPtr<Aula>> >,
			LessPtr<Disciplina> > *map2 = & itMap1->second;

		std::map< Disciplina*, std::map< int /*turma*/, GGroup<Aula*, LessPtr<Aula>> >,
			LessPtr<Disciplina> >::iterator itMap2 = (*map2).find( disciplina );
		if ( itMap2 != (*map2).end() )
		{
			std::map< int /*turma*/, GGroup<Aula*, LessPtr<Aula>> > *map3 = & itMap2->second;

			std::map< int /*turma*/, GGroup<Aula*, LessPtr<Aula>> >::iterator itMap3 = (*map3).find( turma );
			if ( itMap3 != (*map3).end() )
			{
				ITERA_GGROUP_LESSPTR( itMap4, itMap3->second, Aula )
				{
					if ( itMap4->getDiaSemana() == diaSemana && 
						 itMap4->getSala() == sala )
					{
						aula = *itMap4;
						return aula;
					}
				}
			}			
		}
	}
	return aula;
}

GGroup<AlunoDemanda*, LessPtr<AlunoDemanda>>* SolucaoTaticoInicial::getAlunosDemanda( int campusId, Disciplina* disciplina, int turma )
{
	GGroup<AlunoDemanda*, LessPtr<AlunoDemanda>> *alunosDemanda;

	std::map< int /*campusId*/, std::map< Disciplina*, std::map< int /*turma*/, GGroup<AlunoDemanda*, LessPtr<AlunoDemanda>> >,
		LessPtr<Disciplina> > >::iterator itMap1 = mapCampusDiscTurma_AlunosDemanda.find( campusId );
	if ( itMap1 != mapCampusDiscTurma_AlunosDemanda.end() )
	{
		std::map< Disciplina*, std::map< int /*turma*/, GGroup<AlunoDemanda*, LessPtr<AlunoDemanda>> >,
			LessPtr<Disciplina> > *map2 = & itMap1->second;

		std::map< Disciplina*, std::map< int /*turma*/, GGroup<AlunoDemanda*, LessPtr<AlunoDemanda>> >,
			LessPtr<Disciplina> >::iterator itMap2 = (*map2).find( disciplina );
		if ( itMap2 != (*map2).end() )
		{
			std::map< int /*turma*/, GGroup<AlunoDemanda*, LessPtr<AlunoDemanda>> > *map3 = & itMap2->second;

			std::map< int /*turma*/, GGroup<AlunoDemanda*, LessPtr<AlunoDemanda>> >::iterator itMap3 = (*map3).find( turma );
			if ( itMap3 != (*map3).end() )
			{
				alunosDemanda = & itMap3->second;
			}			
		}
	}
	return alunosDemanda;
}
  
int SolucaoTaticoInicial::getSolNumTurmas( int campusId, Disciplina *disciplina )
{ 
	int num = 0;
	std::map< int /*campusId*/, std::map< Disciplina*, std::map< int /*turma*/, GGroup<Aula*, LessPtr<Aula>> >,
		LessPtr<Disciplina> > >::iterator itMap1 = mapAulas.find( campusId );
	if ( itMap1 != mapAulas.end() )
	{
		std::map< Disciplina*, std::map< int /*turma*/, GGroup<Aula*, LessPtr<Aula>> >,
			LessPtr<Disciplina> > *map2 = & itMap1->second;

		std::map< Disciplina*, std::map< int /*turma*/, GGroup<Aula*, LessPtr<Aula>> >,
			LessPtr<Disciplina> >::iterator itMap2 = (*map2).find( disciplina );
		if ( itMap2 != (*map2).end() )
		{
			num = itMap2->second.size();
		}
	}
	return num;   
}

Sala* SolucaoTaticoInicial::getSala( int campusId, Disciplina* disciplina, int turma, bool &fixar )
{	   
	Sala *sala=NULL;

	GGroup<Aula*, LessPtr<Aula>> aulas = getAulas(campusId, disciplina, turma);
	   
	if ( aulas.size() != 0 )
	{
		Aula *aula = *( aulas.begin() );
		sala = aula->getSala();
		fixar = aula->fixaSala();			
	}
	   
	return sala;
}

int SolucaoTaticoInicial::getTurma( Aluno* aluno, int campusId, Disciplina *disciplina, bool &fixar )
{
	int turma = -3;
	fixar = false;

	std::map< Aluno*, 
		std::map< int /*campusId*/, 
			std::map< Disciplina*, 
				std::pair< int/*turma*/, bool /*fixar*/ >, 
					LessPtr<Disciplina> > > , LessPtr<Aluno> >::iterator itMap = mapAluno_CampusTurmaDisc.find(aluno);
	if ( itMap != mapAluno_CampusTurmaDisc.end() ) 
	{
		std::map< int /*campusId*/, std::map< Disciplina*, std::pair< int/*turma*/, bool /*fixar*/ >, 
			LessPtr<Disciplina> > > *mapCp = & itMap->second;

		std::map< int /*campusId*/, std::map< Disciplina*, std::pair< int/*turma*/, bool /*fixar*/ >, 
			LessPtr<Disciplina> > >::iterator itCp = (*mapCp).find( campusId );
		if ( itCp != (*mapCp).end() )
		{
			std::map< Disciplina*, std::pair< int/*turma*/, bool /*fixar*/ >,
				LessPtr<Disciplina> > *mapDisc = & itCp->second;
			std::map< Disciplina*, std::pair< int/*turma*/, bool /*fixar*/ >,
				LessPtr<Disciplina> >::iterator itDisc = (*mapDisc).find( disciplina );
			if ( itDisc != (*mapDisc).end() )
			{
				turma = itDisc->second.first;
				fixar = itDisc->second.second;
			}
		}
	}

	return turma;
}

bool SolucaoTaticoInicial::existeTurma( int campusId, Disciplina *disciplina, int turma, bool &fixar )
{
	bool existe = false;
		
	GGroup<Aula*, LessPtr<Aula>> aulas = getAulas(campusId, disciplina, turma);
	   
	if ( aulas.size() != 0 )
	{
		existe = true;
		fixar = aulas.begin()->fixaAbertura();
	}
	   
	return existe;
}

void SolucaoTaticoInicial::removeFromSol( VariablePre v )
{
	if ( v.getType() == VariablePre::V_PRE_ALOCA_ALUNO_TURMA_DISC )
		preSol_S.erase(v);

	else if ( v.getType() == VariablePre::V_PRE_OFERECIMENTO )
		preSol_O.erase(v);
}

void SolucaoTaticoInicial::removeFromSol( VariableTatInt v )
{
	if ( v.getType() == VariableTatInt::V_ALOCA_ALUNO_TURMA_DISC )
		tatSol_S.erase(v);

	else if ( v.getType() == VariableTatInt::V_CREDITOS )
		tatSol_X.erase(v);
}

void SolucaoTaticoInicial::addInSol( VariablePre v )
{
	if ( v.getType() == VariablePre::V_PRE_ALOCA_ALUNO_TURMA_DISC )
		preSol_S.insert(v);

	else if ( v.getType() == VariablePre::V_PRE_OFERECIMENTO )
		preSol_O.insert(v);
}

void SolucaoTaticoInicial::addInSol( VariableTatInt v )
{
	if ( v.getType() == VariableTatInt::V_ALOCA_ALUNO_TURMA_DISC )
		tatSol_S.insert(v);

	else if ( v.getType() == VariableTatInt::V_CREDITOS )
		tatSol_X.insert(v);
}

bool SolucaoTaticoInicial::isInSol( VariablePre v )
{
	bool fixar = false;

	if ( v.getType() == VariablePre::V_PRE_ALOCA_ALUNO_TURMA_DISC )
	{
		int turma = getTurma( v.getAluno(), v.getCampus()->getId(), v.getDisciplina(), fixar );
		if ( v.getTurma() == turma )
		{
			return true;
		}
		return false;
	}

	if ( v.getType() == VariablePre::V_PRE_OFERECIMENTO )
	{
		Sala *sala = getSala( v.getUnidade()->getIdCampus(), v.getDisciplina(), v.getTurma(), fixar );
		
		if ( sala != NULL )
		if ( v.getSubCjtSala()->salas.begin()->second == sala )
		{
			return true;
		}
		return false;
	}

	std::cout<<"\nError: Type Not Expected! " << v.toString() << endl;
	return false;
}

bool SolucaoTaticoInicial::isInSol( VariableTatInt v )
{	
	bool fixar = false;

	if ( v.getType() == VariableTatInt::V_ALOCA_ALUNO_TURMA_DISC )
	{
		int turma = getTurma( v.getAluno(), v.getCampus()->getId(), v.getDisciplina(), fixar );
		if ( v.getTurma() == turma )
		{
			return true;
		}
		return false;
	}

	if ( v.getType() == VariableTatInt::V_CREDITOS )
	{
		if ( tatSol_X.find(v) != tatSol_X.end() )
		{
			GGroup<Aula*, LessPtr<Aula>> aulas = getAulas( v.getUnidade()->getIdCampus(), v.getDisciplina(), v.getTurma() );
			ITERA_GGROUP_LESSPTR( itAula, aulas, Aula )
			{
				if ( itAula->getDiaSemana() == v.getDia() && 
					 itAula->getHorarioAulaInicial() == v.getHorarioAulaInicial() &&
					 itAula->getHorarioAulaFinal() == v.getHorarioAulaFinal() &&
					 itAula->getSala() == v.getSubCjtSala()->salas.begin()->second )
					fixar = true;
			}
			return true;
		}
		return false;
	}
	
	std::cout<<"\nError: Type Not Expected! " << v.toString() << endl;
	return false;
}

bool SolucaoTaticoInicial::isInSol( VariableTatico v)
{		
	VariableTatInt x = inToVariableTatInt(v);
	return isInSol( x );
}

VariableTatInt SolucaoTaticoInicial::inToVariableTatInt( VariableTatico v )
{	
	VariableTatInt x;
	x.reset();

	if ( v.getType() == VariableTatico::V_CREDITOS )
	{
		x.setType( VariableTatInt::V_CREDITOS );
		x.setTurma( v.getTurma() );								// i
		x.setDisciplina( v.getDisciplina() );					// d
		x.setUnidade( v.getUnidade() );							// u
		x.setSubCjtSala( v.getSubCjtSala() );					// tps  
		x.setDia( v.getDia() );									// t
		x.setHorarioAulaInicial( v.getHorarioAulaInicial() );	// hi
		x.setHorarioAulaFinal( v.getHorarioAulaFinal() );		// hf
		x.setDateTimeInicial( v.getDateTimeInicial() );			// dti
		x.setDateTimeFinal( v.getDateTimeFinal() );				// dtf
	
		return x;
	}

	if ( v.getType() == VariableTatico::V_ABERTURA )
	{
		x.setType( VariableTatInt::V_ABERTURA );
		x.setTurma( v.getTurma() );								// i
		x.setDisciplina( v.getDisciplina() );					// d
		x.setCampus( v.getCampus() );							// cp
	
		return x;
	}
		
	std::cout<<"\nError: Type Not Expected! " << v.toString() << endl;
	return x;		
}

bool SolucaoTaticoInicial::criar( VariableTatico x, bool &fixar )
{	
	VariableTatInt v = inToVariableTatInt(x);
	fixar = false;
	
	if ( v.getType() == VariableTatInt::V_ALOCA_ALUNO_TURMA_DISC )
	{
		int turma = getTurma( v.getAluno(), v.getCampus()->getId(), v.getDisciplina(), fixar );
		if ( v.getTurma() != turma && turma != -1 && fixar ) // aluno alocado e fixo em outra turma
			return false;
		return true;
	}

	if ( v.getType() == VariableTatInt::V_CREDITOS )
	{
		int numTotalCreds = v.getDisciplina()->getTotalCreditos();
		int numCredsFixos = 0;
		GGroup<Aula*, LessPtr<Aula>> aulas = getAulas( v.getUnidade()->getIdCampus(), v.getDisciplina(), v.getTurma() );
		ITERA_GGROUP_LESSPTR( itAula, aulas, Aula )
		{
			if ( itAula->fixaDia() && itAula->fixaHi() && itAula->fixaHf() )
			{
				numCredsFixos += itAula->getTotalCreditos();

				if ( itAula->getDiaSemana() == v.getDia() && 
					 itAula->getHorarioAulaInicial() == v.getHorarioAulaInicial() &&
					 itAula->getHorarioAulaFinal() == v.getHorarioAulaFinal() )
				{
					 fixar = true;					 
					 return true;
				}
			}
		}
		if ( numCredsFixos == numTotalCreds )
			return false;
		
		return true;
	}

	if ( v.getType() == VariableTatInt::V_ABERTURA )
	{
		GGroup<Aula*, LessPtr<Aula>> aulas = getAulas( v.getCampus()->getId(), v.getDisciplina(), v.getTurma());
		ITERA_GGROUP_LESSPTR( itAula, aulas, Aula )
		{
			if ( itAula->fixaAbertura() ){
				fixar = true; break;
			}
		}
	   	return true;
	}
	
	std::cout<<"\nError: Type Not Expected! " << v.toString() << endl;
	return false;
}

bool SolucaoTaticoInicial::criar( VariablePre v, bool &fixar )
{	
	fixar = false;
	
	if ( v.getType() == VariablePre::V_PRE_ALOCA_ALUNO_TURMA_DISC )
	{
		int turma = getTurma( v.getAluno(), v.getCampus()->getId(), v.getDisciplina(), fixar );
		if ( v.getTurma() != turma && turma != -1 && fixar )
			return false;	// aluno alocado e fixo em outra turma
		return true;
	}

	if ( v.getType() == VariablePre::V_PRE_OFERECIMENTO )
	{
		bool aulaFixa = false;
		Sala *sala = v.getSubCjtSala()->salas.begin()->second;
		GGroup<Aula*, LessPtr<Aula>> aulas = getAulas( v.getUnidade()->getIdCampus(), v.getDisciplina(), v.getTurma() );
		ITERA_GGROUP_LESSPTR( itAula, aulas, Aula )
		{
			if ( itAula->fixaSala() )
			{
				aulaFixa = true;

				if ( itAula->getSala() == sala )
				{
					 fixar = true;					 
					 return true;
				}
			}
		}
		if ( aulaFixa ) return false;

		return true;
	}
	
	std::cout<<"\nError: Type Not Expected! " << v.toString() << endl;
	return false;
}

void SolucaoTaticoInicial::calculaNroTurmasAbertas( int campusId )
{
	std::map< int /*campusId*/, std::map< Disciplina*, std::map< int /*turma*/, GGroup<Aula*, LessPtr<Aula>> >,
		LessPtr<Disciplina> > >::iterator itMap1 = mapAulas.begin();
	for ( ; itMap1 != mapAulas.end(); itMap1++ )
	{
		int cpId = itMap1->first;

		if ( cpId != campusId ) continue;

		nroTurmasAbertas[cpId] = 0;

		std::map< Disciplina*, std::map< int /*turma*/, GGroup<Aula*, LessPtr<Aula>> >,
			LessPtr<Disciplina> > *map2 = & itMap1->second;

		std::map< Disciplina*, std::map< int /*turma*/, GGroup<Aula*, LessPtr<Aula>> >,
			LessPtr<Disciplina> >::iterator itMap2 = (*map2).begin();
		for ( ; itMap2 != (*map2).end(); itMap2++ )
		{
			std::map< int /*turma*/, GGroup<Aula*, LessPtr<Aula>> > *map3 = & itMap2->second;

			std::map< int /*turma*/, GGroup<Aula*, LessPtr<Aula>> >::iterator itMap3 = (*map3).begin();			
			for ( ; itMap3 != (*map3).end(); itMap3++ )
			{
				GGroup<Aula*, LessPtr<Aula>> *aulas = & itMap3->second;
				ITERA_GGROUP_LESSPTR ( itGGroup, (*aulas), Aula )
				{
					if ( itGGroup->fixaAbertura() )
					{
						nroTurmasAbertas[cpId]++;
						break;
					}
				}
			}
		}
	}
	
}

void SolucaoTaticoInicial::confereCompletudeAulas()
{
	std::cout << "\nConfere completude das aulas...";

	std::map< int /*campusId*/, std::map< Disciplina*, std::map< int /*turma*/, GGroup<Aula*, LessPtr<Aula>> >,
		LessPtr<Disciplina> > >::iterator itMap1 = mapAulas.begin( );
	for ( ; itMap1 != mapAulas.end(); itMap1++ )
	{
		int campusId = itMap1->first;

		std::map< Disciplina*, std::map< int /*turma*/, GGroup<Aula*, LessPtr<Aula>> >,
			LessPtr<Disciplina> > *map2 = & itMap1->second;

		std::map< Disciplina*, std::map< int /*turma*/, GGroup<Aula*, LessPtr<Aula>> >,
			LessPtr<Disciplina> >::iterator itMap2 = (*map2).begin( );
		for ( ; itMap2 != (*map2).end(); itMap2++ )
		{
			Disciplina *disc = itMap2->first;

			std::map< int /*turma*/, GGroup<Aula*, LessPtr<Aula>> > *map3 = & itMap2->second;

			std::map< int /*turma*/, GGroup<Aula*, LessPtr<Aula>> >::iterator itMap3 = (*map3).begin( );
			for ( ; itMap3 != (*map3).end(); itMap3++ )
			{
				int turma = itMap3->first;

				int fixaAbertura = 0;
				int fixaSala = 0;
				int fixaDia = 0;
				int fixaHi = 0;
				int fixaHf = 0;

				int nCreds = 0;
				GGroup<Aula*, LessPtr<Aula>> *ptAulas = & itMap3->second;
				ITERA_GGROUP_LESSPTR( itAula, *ptAulas, Aula )
				{
					nCreds += itAula->getTotalCreditos();

					fixaAbertura += (int) itAula->fixaAbertura();
					fixaSala += (int) itAula->fixaSala();
					fixaDia += (int) itAula->fixaDia();
					fixaHi += (int) itAula->fixaHi();
					fixaHf += (int) itAula->fixaHf();

					bool ok = disc->inicioTerminoValidos( 
						itAula->getHorarioAulaInicial(), itAula->getHorarioAulaFinal(), itAula->getDiaSemana() );
					if ( !ok )
					{
						stringstream msg;
						msg << "Aula " << itAula->toString() << " criada sem respeitar Disciplina::inicioTerminoValidos(hi,hf,dia).";
						CentroDados::printError( "void SolucaoTaticoInicial::confereCompletudeAulas()",	msg.str() );
					}
				}

				if ( fixaAbertura>0 && fixaAbertura!=ptAulas->size() )
				{
					std::cout<<"\nAtencao em tags de fixacao de abertura em solucao inicial. A turma "
						<< turma << " da disciplina " << disc->getId() << " campus " << campusId
						<< " possui as aulas com abertura parcialmente fixada."
						<< " A abertura de todas as aulas dessa turma serao fixadas.";
					ITERA_GGROUP_LESSPTR( itAula, *ptAulas, Aula )
					{
						itAula->fixaAbertura(true);
					}
				}
				if ( fixaSala>0 && fixaSala!=ptAulas->size() )
				{
					std::cout<<"\nAtencao em tags de fixacao de sala em solucao inicial. A turma "
						<< turma << " da disciplina " << disc->getId() << " campus " << campusId
						<< " possui as aulas com sala parcialmente fixada. Todas as aulas dessa turma serao fixadas."
						<< " As salas de todas as aulas dessa turma serao fixadas.";
					ITERA_GGROUP_LESSPTR( itAula, *ptAulas, Aula )
					{
						itAula->fixaSala(true);
					}
				}
				if ( fixaDia>0 && fixaDia!=ptAulas->size() )
				{
					std::cout<<"\nAtencao em tags de fixacao de dias em solucao inicial. A turma "
						<< turma << " da disciplina " << disc->getId() << " campus " << campusId
						<< " possui as aulas com dias parcialmente fixados. Todas as aulas dessa turma serao fixadas."
						<< " Os dias de todas as aulas dessa turma serao fixados.";
					ITERA_GGROUP_LESSPTR( itAula, *ptAulas, Aula )
					{
						itAula->fixaDia(true);
					}
				}
				if ( (fixaHi>0 && fixaHi!=ptAulas->size()) || (fixaHf>0 && fixaHf!=ptAulas->size()) )
				{
					std::cout<<"\nAtencao em tags de fixacao de horarios em solucao inicial. A turma "
						<< turma << " da disciplina " << disc->getId() << " campus " << campusId
						<< " possui as aulas com horarios parcialmente fixados. Todas as aulas dessa turma serao fixadas."
						<< " Os horarios de todas as aulas dessa turma serao fixados.";
					ITERA_GGROUP_LESSPTR( itAula, *ptAulas, Aula )
					{
						itAula->fixaHi(true);
						itAula->fixaHf(true);
					}
				}

				if ( nCreds != disc->getTotalCreditos() )
				{
					std::cout << "\nATENCAO: possivel erro. Solucao tatica inicial com nro de creditos incompletos para a turma "
						<< turma << " da disciplina id " << disc->getId() << " no campus " << campusId
						<< "\nTotal de creditos da disciplina: " << disc->getTotalCreditos()
						<< "\nTotal de creditos das aulas: " << nCreds << endl;
				}
			}
		}
	}
}

void SolucaoTaticoInicial::confereCorretudeAlocacoes()
{
	std::cout << "\nConfere corretude das alocacoes...";

   auto itMapCp = mapCampusDiscTurma_AlunosDemanda.begin();
   for ( ; itMapCp != mapCampusDiscTurma_AlunosDemanda.end(); itMapCp++ )
   {
	   auto itMapDisc = itMapCp->second.begin();
	   for ( ; itMapDisc != itMapCp->second.end(); itMapDisc++ )
	   {
		   Disciplina *discAlocada = itMapDisc->first;
		   
		 // std::cout << "\ndiscAlocada="<<discAlocada->getId();

		   auto itMapTurma = itMapDisc->second.begin();
		   for ( ; itMapTurma != itMapDisc->second.end(); itMapTurma++ )
		   {
			//   std::cout << "\n\tTurma="<< itMapTurma->first;

			   ITERA_GGROUP_LESSPTR( itAlDem, itMapTurma->second, AlunoDemanda )
			   {
				//   std::cout << "\n\t\titAlDem = " << itAlDem->getId() << " discAlocada="<<discAlocada->getId();

				   bool usouEquiv = abs(itAlDem->demanda->disciplina->getId()) != abs( discAlocada->getId() );

				   if ( itAlDem->getExigeEquivalenciaForcada() && !usouEquiv )
				   {
						if ( itAlDem->demandaOriginal == nullptr )
						{
							// Equivalencia NÃO usada							
							if ( itAlDem->demanda->disciplina->discEquivSubstitutas.size() > 0 )
							{
								stringstream msg;
								msg << "Equivalencia forcada nao respeitada. Aluno " 
									<< itAlDem->getAlunoId() << " e Disciplina " << discAlocada->getId();
								CentroDados::printError( "void SolucaoTaticoInicial::confereCorretudeAlocacoes()", msg.str() );
							}
							else
							{
								stringstream msg;
								msg << "Disciplina sem equivalentes encontradas. Equivalencia forcada nao respeitada. Aluno " 
									<< itAlDem->getAlunoId() << " e Disciplina " << discAlocada->getId();
								CentroDados::printError( "void SolucaoTaticoInicial::confereCorretudeAlocacoes()", msg.str() );							   
							}
						}
						else
						{
							// Equivalencia usada
							if ( ! CentroDados::getProblemData()->alocacaoEquivViavel( itAlDem->demandaOriginal, discAlocada ) )
							{
								stringstream msg;
								msg << "Equivalencia forcada nao respeitada. Aluno " 
									<< itAlDem->getAlunoId() << " e Disciplina " << discAlocada->getId();
								CentroDados::printError( "void SolucaoTaticoInicial::confereCorretudeAlocacoes()", msg.str() );
							}
						}
				   }
				   
				   if ( usouEquiv )
				   {
					    // Equivalencia usada
					    if ( itAlDem->demandaOriginal != nullptr )
						if ( ! CentroDados::getProblemData()->alocacaoEquivViavel( itAlDem->demandaOriginal, discAlocada ) )
						{
							stringstream msg;
							msg << "Equivalencia inviavel usada. Aluno " 
								<< itAlDem->getAlunoId() << ", Curso " << itAlDem->demandaOriginal->oferta->getCursoId()
								<< " e Disciplina " << discAlocada->getId();
							CentroDados::printError( "void SolucaoTaticoInicial::confereCorretudeAlocacoes()", msg.str() );
						}						
				   }
			   }
		   }
	   }
   }
}

void SolucaoTaticoInicial::confereSolucao()
{
	confereCompletudeAulas();
	confereCorretudeAlocacoes();
}

void SolucaoTaticoInicial::imprimeAulas( std::string fileName )
{
	ofstream outFile;
//	string fileName("aulasSolInicial.txt");
	outFile.open( fileName, ofstream::out );

	if ( !outFile )
	{
		std::cout << "\nErro em void SolucaoTaticoInicial::imprimeAulas():"
			<< " o arquivo " << fileName << " nao pode ser aberto.";
		return;
	}

	std::map< int /*campusId*/, std::map< Disciplina*, std::map< int /*turma*/, GGroup<Aula*, LessPtr<Aula>> >,
		LessPtr<Disciplina> > >::iterator itMap1 = mapAulas.begin( );
	for ( ; itMap1 != mapAulas.end(); itMap1++ )
	{
		int campusId = itMap1->first;

		std::map< Disciplina*, std::map< int /*turma*/, GGroup<Aula*, LessPtr<Aula>> >,
			LessPtr<Disciplina> > *map2 = & itMap1->second;

		std::map< Disciplina*, std::map< int /*turma*/, GGroup<Aula*, LessPtr<Aula>> >,
			LessPtr<Disciplina> >::iterator itMap2 = (*map2).begin( );
		for ( ; itMap2 != (*map2).end(); itMap2++ )
		{
			Disciplina *disc = itMap2->first;

			std::map< int /*turma*/, GGroup<Aula*, LessPtr<Aula>> > *map3 = & itMap2->second;

			std::map< int /*turma*/, GGroup<Aula*, LessPtr<Aula>> >::iterator itMap3 = (*map3).begin( );
			for ( ; itMap3 != (*map3).end(); itMap3++ )
			{
				int turma = itMap3->first;
				
				outFile << "\n\n=========================================================================" 
					<< "\nCampus " << campusId << ", Turma " << turma << ", Disciplina " << disc->getId()
					<< "  -  " << disc->getTotalCreditos() << " creditos";

				outFile << "\n--------";
				outFile << "\nAlunos: ";
				GGroup<AlunoDemanda*, LessPtr<AlunoDemanda>> *alunosDemanda 
					= this->getAlunosDemanda(campusId, disc, turma);
				ITERA_GGROUP_LESSPTR( itAlDem, (*alunosDemanda), AlunoDemanda )
				{
					outFile << itAlDem->getAlunoId() << "; ";
				}
				outFile << "\n--------";
				
				int nCreds = 0;
				GGroup<Aula*, LessPtr<Aula>> *ptAulas = & itMap3->second;
				ITERA_GGROUP_LESSPTR( itAula, *ptAulas, Aula )
				{
					outFile << "\nDia " << itAula->getDiaSemana();					
					outFile << ", Sala " << itAula->getSala()->getId();
					
					if ( itAula->getHorarioAulaInicial() != NULL )
						outFile << ", Hi " << itAula->getHorarioAulaInicial()->getInicio().getHour() << ":"
								<< itAula->getHorarioAulaInicial()->getInicio().getMinute()
								<< " (id" << itAula->getHorarioAulaInicial()->getId() << ")";
					if ( itAula->getHorarioAulaFinal() != NULL )
						outFile << ", Hf " << itAula->getHorarioAulaFinal()->getInicio().getHour() << ":"
								<< itAula->getHorarioAulaFinal()->getInicio().getMinute() << " (id" 
								<< itAula->getHorarioAulaFinal()->getId() << ")";
					
					outFile << "; " << itAula->getTotalCreditos() << " creditos";
				}
			}
		}
	}
	outFile.close();
}

void SolucaoTaticoInicial::imprimeSolucaoX( std::string fileName )
{
	ofstream outFile;
	outFile.open( fileName, ofstream::out );

	if ( !outFile )
	{
		std::cout << "\nErro em void SolucaoTaticoInicial::imprimeSolucaoX():"
			<< " o arquivo " << fileName << " nao pode ser aberto.";
		return;
	}

	std::set< VariableTatInt >::iterator itX = tatSol_X.begin();	
	for ( ; itX != tatSol_X.end(); itX++ )
	{
		VariableTatInt x = (*itX);
		outFile << x.toString() << "\n";
	}
	outFile.close();
}

void SolucaoTaticoInicial::imprimeSolucaoS( std::string fileName )
{
	ofstream outFile;
	outFile.open( fileName, ofstream::out );

	if ( !outFile )
	{
		std::cout << "\nErro em void SolucaoTaticoInicial::imprimeSolucaoS():"
			<< " o arquivo " << fileName << " nao pode ser aberto.";
		return;
	}

	std::set< VariableTatInt >::iterator itS = tatSol_S.begin();	
	for ( ; itS != tatSol_S.end(); itS++ )
	{
		VariableTatInt s = (*itS);
		outFile << "\n" << s.toString();
	}
	outFile.close();
}

void SolucaoTaticoInicial::imprimeSolucaoNaoAtendimentos( std::string fileName )
{
	ofstream outFile;
	outFile.open( fileName, ofstream::out );

	if ( !outFile )
	{
		std::cout << "\nErro em void SolucaoTaticoInicial::imprimeSolucaoNaoAtendimentos():"
			<< " o arquivo " << fileName << " nao pode ser aberto.";
		return;
	}
	
	outFile << "Nao-atendimentos na solucao inicial:";

	ProblemData* pdata = CentroDados::getProblemData();	
	ITERA_GGROUP_LESSPTR( itAlDem, pdata->alunosDemanda, AlunoDemanda )
	{
		bool fixar;
		if ( this->getTurma( itAlDem->getAluno(), itAlDem->getCampus()->getId(), itAlDem->demanda->disciplina, fixar ) < 0 )
		{
			outFile << "\nAlunoDemanda " << itAlDem->getId()
				<< ", Aluno " << itAlDem->getAlunoId()
				<< ", Disc " << itAlDem->demanda->getDisciplinaId()
				<< ", P " << itAlDem->getPrioridade();
		}
	}

	outFile.close();
}