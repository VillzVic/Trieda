#include "AlunoSolution.h"
#include "Aluno.h"
#include "AlunoDemanda.h"
#include "Aula.h"

AlunoSolution::AlunoSolution( Aluno* aluno )
{
	aluno_ = aluno;
	alunoId_ = aluno->getAlunoId();

	mapDiaDiscAulas_.clear();
    mapAlDemDiscTurmaCp_.clear();

	nroCredsAtendidos_=0;
	nroDiscAtendidas_=0;
	chAtendida_=0;
}

AlunoSolution::~AlunoSolution()
{
	mapDiaDiscAulas_.clear();	
    mapAlDemDiscTurmaCp_.clear();
}

void AlunoSolution::addTurma( AlunoDemanda *ad, Disciplina* discReal, int turma, Campus* campus )
{ 
	mapAlDemDiscTurmaCp_[ad][discReal] = make_pair(turma, campus);
	addDiscIdAtendida( abs(discReal->getId()) );
}

int AlunoSolution::getTurma( AlunoDemanda *ad, bool teorica )
{ 
	auto itFinderAlDem = mapAlDemDiscTurmaCp_.find(ad);
	if ( itFinderAlDem != mapAlDemDiscTurmaCp_.end() )
	{
		auto itFinderDisc = itFinderAlDem->second.begin();
		for ( ; itFinderDisc != itFinderAlDem->second.end(); itFinderDisc++ )
		{
			bool discTeor = (itFinderDisc->first->getId() > 0);

			if ( (discTeor && teorica) || (!discTeor && !teorica) )
				return itFinderDisc->second.first;
		}
	}

	return -1;
}

void AlunoSolution::addDiaDiscAula( int dia, Disciplina* discReal, Aula *aula )
{
	mapDiaDiscAulas_[dia][aula->getHorarioAulaInicial()->getInicio()][aula->getHorarioAulaFinal()->getFinal()][discReal] = aula;
}

void AlunoSolution::mapAlDemDiscTurmaCpToStream( std::ostream & outFile )
{      
	std::stringstream ss1;
	std::stringstream ss2;

	ss1 << "\n\n-----------------------------------------------------------------------"
		<< "\nAluno " << this->alunoId_ << "\t" << this->aluno_->getNomeAluno();
		
	double ch = 0;
	for( auto itAlDem = this->mapAlDemDiscTurmaCp_.begin(); itAlDem != this->mapAlDemDiscTurmaCp_.end(); itAlDem++ )
	{
		for( auto itDisc = itAlDem->second.begin(); itDisc != itAlDem->second.end(); itDisc++ )
		{
			Disciplina *disciplina = itDisc->first;
			pair<int/*turma*/, Campus*> parTurmaCp = itDisc->second;

			ch += disciplina->getTotalCreditos() * disciplina->getTempoCredSemanaLetiva();

			ss2 << " (Disc " << disciplina->getId()
				<< "_i" << parTurmaCp.first << "_Cp" << parTurmaCp.second->getId() << ")";
		}
	}

	outFile << ss1.str();
	outFile << "\n\t" << 100 * ch / this->aluno_->getCargaHorariaOrigRequeridaP1() << "\% de atendimento\n\t";
	outFile << ss2.str();
}


void AlunoSolution::mapDiaDiscAulasToStream( std::ostream & outFile )
{
	outFile << "\n\n-----------------------------------------------------------------------"
			<< "\nAluno " << this->alunoId_ << "\t" << this->aluno_->getNomeAluno();
		
	double ch = 0;
	for( auto itDia = this->mapDiaDiscAulas_.begin(); itDia != this->mapDiaDiscAulas_.end(); itDia++ )
	{
		int dia = itDia->first;
			
		outFile << "\n\tDia " << dia;
		for( auto itDti = itDia->second.begin(); itDti != itDia->second.end(); itDti++ )
		{
			for( auto itDtf = itDti->second.begin(); itDtf != itDti->second.end(); itDtf++ )
			{
				for( auto itDisc = itDtf->second.begin(); itDisc != itDtf->second.end(); itDisc++ )
				{
					Aula *aula = itDisc->second;
				
					outFile << "\n\t\tAula " << aula->toString();
				}
			}
		}
	}
}

map< DateTime /*dti*/, map< DateTime /*dtf*/, map< Disciplina*, Aula*, 
		LessPtr<Disciplina> > > >* AlunoSolution::getMapAulasNoDia( int dia )
{
	map< DateTime /*dti*/, map< DateTime /*dtf*/, map< Disciplina*, Aula*, 
		LessPtr<Disciplina> > > > *mapAulas=nullptr;

	auto finderDia = mapDiaDiscAulas_.find(dia);
	if ( finderDia != mapDiaDiscAulas_.end() )
	{
		mapAulas = & finderDia->second;
	}

	return mapAulas;
}

void AlunoSolution::procuraChoqueDeHorariosAluno(
	vector< map<int /*dia*/, GGroup<HorarioAula*, LessPtr<HorarioAula>> > > &opcoes,
	map< int, GGroup< Trio<int,int,Disciplina*> > > &turmas_choque_horarios )
{
	/*
		- Recebe em 'opcoes' um vetor com combinacoes de horários-dia, e um aluno.
		- Verifica para cada combinacao se há choque de horários com as turmas já alocadas do aluno.
		- Retorna, para cada combinacao em que houve choque, as turmas do aluno que possuem choques.
	*/

	// Verifica conflitos entre horários-dia alocados do aluno e os horários-dia em 'opcoes'
	for ( int at = 0; at != opcoes.size(); at++ )
	{
		map<int /*dia*/, GGroup<HorarioAula*, LessPtr<HorarioAula>> > *opcao = & opcoes[at];
		auto itDiaHors = (*opcao).begin();
		for ( ; itDiaHors != (*opcao).end(); itDiaHors++ )
		{
			int dia = itDiaHors->first;

			ITERA_GGROUP_LESSPTR( itHorAula, itDiaHors->second, HorarioAula )
			{
				DateTime dti = itHorAula->getInicio();
				DateTime dtf = itHorAula->getFinal();

				auto itDiaDoAluno = mapDiaDiscAulas_.find(dia);
				if ( itDiaDoAluno != mapDiaDiscAulas_.end() )
				{
					// encontrou alocação do aluno no mesmo dia
					auto itDtiAulaAluno = itDiaDoAluno->second.begin();
					for( ; itDtiAulaAluno != itDiaDoAluno->second.end(); itDtiAulaAluno++ )
					{
						DateTime inicio_aluno = itDtiAulaAluno->first;

						if( inicio_aluno >= dtf )
							break;

						auto itDtfAulaAluno = itDtiAulaAluno->second.begin();
						for( ; itDtfAulaAluno != itDtiAulaAluno->second.end(); itDtfAulaAluno++ )
						{
							DateTime fim_aluno = itDtfAulaAluno->first;
							
							if( (inicio_aluno >= dti && inicio_aluno < dtf) ||
								(dti >= inicio_aluno && dti < fim_aluno ) )
							{
								auto itDiscAulaDoAluno = itDtfAulaAluno->second.begin();
								for( ; itDiscAulaDoAluno != itDtfAulaAluno->second.end(); itDiscAulaDoAluno++ )
								{
									Disciplina* disc_aluno = itDiscAulaDoAluno->first;
									Aula *aula = itDiscAulaDoAluno->second;
									int campus_aluno = aula->getCampus()->getId();
									int turma_aluno = aula->getTurma();

									Trio< int , int , Disciplina* > trio_aluno (campus_aluno, turma_aluno, disc_aluno);
							
									turmas_choque_horarios[at].add(trio_aluno);
								}
							}
						}
					}
				}
			}
		}
	}
}


void AlunoSolution::procuraChoqueDeHorariosAluno( int dia, DateTime dti, DateTime dtf, GGroup< Aula* > & aulas_choque )
{
	/*
		- Recebe o dia, o horario inicial dti e o horario final dtf;
		- Retorna as aulas do aluno que possuem choques com esse trio.
	*/
		
	auto itDiaDoAluno = mapDiaDiscAulas_.find(dia);
	if ( itDiaDoAluno != mapDiaDiscAulas_.end() )
	{
		// encontrou alocação do aluno no mesmo dia
		auto itDtiAulaAluno = itDiaDoAluno->second.begin();
		for( ; itDtiAulaAluno != itDiaDoAluno->second.end(); itDtiAulaAluno++ )
		{
			DateTime inicio_aluno = itDtiAulaAluno->first;

			if( inicio_aluno >= dtf )
				break;

			auto itDtfAulaAluno = itDtiAulaAluno->second.begin();
			for( ; itDtfAulaAluno != itDtiAulaAluno->second.end(); itDtfAulaAluno++ )
			{
				DateTime fim_aluno = itDtfAulaAluno->first;
							
				if( (inicio_aluno >= dti && inicio_aluno < dtf) ||
					(dti >= inicio_aluno && dti < fim_aluno ) )
				{
					auto itDiscAulaDoAluno = itDtfAulaAluno->second.begin();
					for( ; itDiscAulaDoAluno != itDtfAulaAluno->second.end(); itDiscAulaDoAluno++ )
					{
						Aula *aula = itDiscAulaDoAluno->second;

						aulas_choque.add(aula);
					}
				}
			}
		}
	}
}