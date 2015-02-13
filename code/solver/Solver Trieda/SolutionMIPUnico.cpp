#include "SolutionMIPUnico.h"

#include "GoalStatus.h"
#include "Disciplina.h"
#include "Campus.h"
#include "Aluno.h"
#include "AlunoDemanda.h"
#include "DateTime.h"
#include "HorarioAula.h"
#include "Professor.h"
#include "CentroDados.h"
#include "MIPUnicoParametros.h"


// -----------------------------------------------------------------------------------------------

SolutionMIPUnico::SolutionMIPUnico(void)
	: mapDiscAlunosDemanda_(nullptr)
{}

SolutionMIPUnico::~SolutionMIPUnico(void)
{
	deleteGoalStatus();
}

// -----------------------------------------------------------------------------------------------
// Data

void SolutionMIPUnico::setMapDiscAlunosDemanda(std::map<Disciplina*, std::set<AlunoDemanda*>> & mapDemandas)
{
	mapDiscAlunosDemanda_ = & mapDemandas;
}

// -----------------------------------------------------------------------------------------------

void SolutionMIPUnico::deleteGoalStatus()
{
   for (auto it = goalStats_.begin(); it != goalStats_.end();)
   {
	   if (*it) it = goalStats_.erase(it);
	   else it++;
   }
   goalStats_.clear();
}

void SolutionMIPUnico::clearMapsSolution()
{
	solAlocProfTurma_.clear();
	solAlocTurmaProf_.clear();
	solAlocAlunoDiscTurmaDiaDti_.clear();
	solAlocAlunoDiaDti_.clear();
}

void SolutionMIPUnico::addSolAlocProfTurma(Professor* const p, Campus * const cp, Disciplina * const d, int turma)
{
	if (p) solAlocProfTurma_[p][cp][d].insert(turma);
	solAlocTurmaProf_[cp][d][turma] = p;
}

void SolutionMIPUnico::addSolAlocAlunoTurma(Aluno* const a, Disciplina * const d, int turma, int dia, DateTime dti)
{
	solAlocAlunoDiscTurmaDiaDti_[a][d].first = turma;
	solAlocAlunoDiscTurmaDiaDti_[a][d].second[dia].insert(dti);
	solAlocAlunoDiaDti_[a][dia].insert(dti);
}

GoalStatus* SolutionMIPUnico::getAddNewGoal(int fase)
{
	int id = goalStats_.size() + 1;
	string name = MIPUnicoParametros::getGoalToString(fase);
	GoalStatus *goal = new GoalStatus(id,name);
	goalStats_.push_back(goal);
	return goal;
}

// -----------------------------------------------------------------------------------------------
// Consulta à solução corrente

bool SolutionMIPUnico::haDemandaNaoAtendida(Disciplina* const disc)
{
	if (!mapDiscAlunosDemanda_) return false;

	auto finder = this->mapDiscAlunosDemanda_->find(disc);
	if (finder != this->mapDiscAlunosDemanda_->end())
	{
		for (auto itAlDem=finder->second.begin(); itAlDem!=finder->second.end(); itAlDem++)
		{
			if (!alunoAlocDisc((*itAlDem)->getAluno(), (*itAlDem)->demanda->disciplina))
				return true;
		}
	}
	return false;
}

bool SolutionMIPUnico::demandaTodaAtendidaPorReal(Disciplina* const disc)
{	
	if (!mapDiscAlunosDemanda_) return false;

	auto finder = this->mapDiscAlunosDemanda_->find(disc);
	if (finder != this->mapDiscAlunosDemanda_->end())
	{
		for (auto itAlDem=finder->second.begin(); itAlDem!=finder->second.end(); itAlDem++)
		{
			int turma = alunoAlocDisc((*itAlDem)->getAluno(),disc);
			if (!turma)
				return false;

			Professor* prof=nullptr;
			getProfAlocNaTurma(prof, (*itAlDem)->getCampus(),disc,turma);

			if (!prof)
				return false;
			if (prof->eVirtual())
				return false;
		}
	}
	return true;
}

bool SolutionMIPUnico::existeTurmaAtendida(Campus* const campus, Disciplina* const disc, int turma) const
{
	auto finderAluno = solAlocTurmaProf_.find(campus);
	if (finderAluno != solAlocTurmaProf_.cend())
	{
		auto finderDia = finderAluno->second.find(disc);
		if (finderDia != finderAluno->second.cend())
		{
			auto finderDti = finderDia->second.find(turma);
			if (finderDti != finderDia->second.cend())
			{
				return true;
			}
		}
	}
	return false;
}

bool SolutionMIPUnico::haDemandaPossivelNoDiaHor(Disciplina* const disc, int dia, HorarioAula* const ha)
{
	if (!mapDiscAlunosDemanda_) return false;

	auto finder = this->mapDiscAlunosDemanda_->find(disc);
	if (finder != this->mapDiscAlunosDemanda_->end())
	{
		for (auto itAlDem=finder->second.begin(); itAlDem!=finder->second.end(); itAlDem++)
		{
			if (permitirAlunoDiscNoHorDia(*itAlDem, dia, ha->getInicio()))
				return true;
		}
	}
	return false;
}

bool SolutionMIPUnico::permitirAlunoDiscNoHorDia(AlunoDemanda* const alDem, int dia, DateTime dti) const
{
	if (alDem->podeNoHorario(dti,dia))
	{
		bool alunoAlocadoNaDisc = alunoAlocDisc(alDem->getAluno(), alDem->demanda->disciplina);
			
		// aluno alocado na disciplina no horario-dia
		if (alunoAlocadoNaDisc &&
			alunoAlocDiscNoHorDia(alDem->getAluno(), alDem->demanda->disciplina, dia, dti))
			return true;

		bool horDiaLivre = alunoHorVazioNoDia(alDem->getAluno(), dia, dti);

		// aluno não alocado com o horario vazio
		if (!alunoAlocadoNaDisc && horDiaLivre)
			return true;

		bool alunoAlocIncompleto = alunoAlocIncompNaDisc(alDem->getAluno(), alDem->demanda->disciplina);

		// aluno alocado incompleto com o horario vazio
		if (alunoAlocIncompleto && horDiaLivre)
			return true;
	}

	return false;
}

bool SolutionMIPUnico::permitirAlunoNaTurma(Aluno* const aluno, Disciplina* const disciplina, int turma) const
{
	int turmaAlocada = alunoAlocDisc(aluno, disciplina);
	if (turmaAlocada && turmaAlocada!=turma)
		return false;
	return true;
}

bool SolutionMIPUnico::permitirTurma(Campus* const campus, Disciplina* const disciplina, int turma)
{
	if (haDemandaNaoAtendida(disciplina) ||
		existeTurmaAtendida(campus, disciplina, turma))
		return true;
	return false;
}

bool SolutionMIPUnico::alunoHorVazioNoDia(Aluno* const aluno, int dia, DateTime dti) const
{
	auto finderAluno = solAlocAlunoDiaDti_.find(aluno);
	if (finderAluno != solAlocAlunoDiaDti_.end())
	{
		auto finderDia = finderAluno->second.find(dia);
		if (finderDia != finderAluno->second.end())
		{
			auto finderDti = finderDia->second.find(dti);
			if (finderDti != finderDia->second.end())
			{
				return false;
			}
		}
	}
	return true;
}

bool SolutionMIPUnico::alunoAlocNaTurma(Aluno* const aluno, Disciplina* const disciplina, int turma) const
{
	int turmaAlocada = alunoAlocDisc(aluno, disciplina);
	if (turmaAlocada && turmaAlocada==turma)
		return true;
	return false;
}

int SolutionMIPUnico::alunoAlocDisc(Aluno* const aluno, Disciplina* const disc) const
{
	auto finderAluno = solAlocAlunoDiscTurmaDiaDti_.find(aluno);
	if (finderAluno != solAlocAlunoDiscTurmaDiaDti_.end())
	{
		auto finderDisc = finderAluno->second.find(disc);
		if (finderDisc != finderAluno->second.end())
		{
			return finderDisc->second.first; // retorna a turma
		}
	}
	return 0;
}

bool SolutionMIPUnico::alunoAlocDiscNoHorDia(Aluno* const aluno, Disciplina* const disc, int dia, DateTime dti) const
{
	auto finderAluno = solAlocAlunoDiscTurmaDiaDti_.find(aluno);
	if (finderAluno != solAlocAlunoDiscTurmaDiaDti_.end())
	{
		auto finderDisc = finderAluno->second.find(disc);
		if (finderDisc != finderAluno->second.end())
		{
			auto ptDiaHors = & finderDisc->second.second;
			
			auto finderDia = ptDiaHors->find(dia);
			if (finderDia != ptDiaHors->end())
			{
				auto finderDti = finderDia->second.find(dti);
				if (finderDti != finderDia->second.end())
				{
					return true;
				}
			}
		}
	}
	return false;
}

bool SolutionMIPUnico::alunoAlocIncompNaDisc(Aluno* const aluno, Disciplina* const disc) const
{
	int n=0;

	auto finderAluno = solAlocAlunoDiscTurmaDiaDti_.find(aluno);
	if (finderAluno != solAlocAlunoDiscTurmaDiaDti_.end())
	{
		auto finderDisc = finderAluno->second.find(disc);
		if (finderDisc != finderAluno->second.end())
		{
			auto ptDiaHors = & finderDisc->second.second;
			
			auto itDia = ptDiaHors->cbegin();
			for (; itDia != ptDiaHors->cend(); itDia++)
			{
				n += itDia->second.size();
			}
		}
	}

	return (n > 0) && (n < disc->getTotalCreditos());
}

bool SolutionMIPUnico::profAlocNaTurma(Professor* const prof, Campus* const campus, Disciplina* const disciplina, int turma) const
{
	auto finderProf = solAlocProfTurma_.find(prof);
	if (finderProf != solAlocProfTurma_.cend())
	{
		auto finderCp = finderProf->second.find(campus);
		if (finderCp != finderProf->second.cend())
		{
			auto finderDisc = finderCp->second.find(disciplina);
			if (finderDisc != finderCp->second.cend())
			{
				auto finderDia = finderDisc->second.find(turma);
				if (finderDia != finderDisc->second.cend())
				{
					return true;
				}
			}
		}
	}
	return false;
}

bool SolutionMIPUnico::getProfAlocNaTurma(Professor* &professor, Campus* const campus, Disciplina* const disciplina, int turma) const
{
	auto finderCp = solAlocTurmaProf_.find(campus);
	if (finderCp != solAlocTurmaProf_.cend())
	{
		auto finderDisc = finderCp->second.find(disciplina);
		if (finderDisc != finderCp->second.cend())
		{
			auto finderTurma = finderDisc->second.find(turma);
			if (finderTurma != finderDisc->second.cend())
			{
				professor = finderTurma->second;
				return true;
			}
		}
	}
	professor = nullptr;
	return false;
}



// -----------------------------------------------------------------------------------------------
// Imprime

void SolutionMIPUnico::imprimeTurmaProf( int campusId, int prioridade )
{
	if (!CentroDados::getPrintLogs())
		return;
	
	std::cout<<"\nImprimindo assignment de turma a prof...\n";

	stringstream fileName;
	fileName << "solTurmaProf";
	fileName << "_" << CentroDados::getInputFileName();
	fileName << "_id" << CentroDados::inputIdToString() << ".txt";

	ofstream outTurmaProf(fileName.str(), ios::out);	
	if ( !outTurmaProf ) 
	{
		std::cout << "\nErro!! Arquivo nao pode ser aberto " << fileName.str();
		return;
	}
	
	outTurmaProf << "CampusId " << campusId << "  -  Prioridade " << prioridade << endl;
	for ( auto itCp = solAlocTurmaProf_.begin(); itCp != solAlocTurmaProf_.end(); itCp++ )
	{
		for ( auto itDisc = itCp->second.begin(); itDisc != itCp->second.end(); itDisc++ )
		{
			for ( auto itTurma = itDisc->second.begin(); itTurma != itDisc->second.end(); itTurma++ )
			{				
				outTurmaProf << "\n\nCp " << itCp->first->getId()
					<< " Disc " << itDisc->first->getId()
					<< " Turma " << itTurma->first << ":";

				outTurmaProf << " Prof " << itTurma->second->getId();				
			}
		}
	}
	outTurmaProf.close();
}

void SolutionMIPUnico::imprimeProfTurmas( int campusId, int prioridade )
{
	if (!CentroDados::getPrintLogs())
		return;
	
	std::cout<<"\nImprimindo assignment de prof e turmas...\n";

	stringstream fileName;
	fileName << "solProfTurmas";
	fileName << "_" << CentroDados::getInputFileName();
	fileName << "_id" << CentroDados::inputIdToString() << ".txt";

	ofstream outProfTurmas(fileName.str(), ios::out);	
	if ( !outProfTurmas ) 
	{
		std::cout << "\nErro!! Arquivo nao pode ser aberto " << fileName.str();
		return;
	}
	
	outProfTurmas << "CampusId " << campusId << "  -  Prioridade " << prioridade << endl;
	for ( auto itProf = solAlocProfTurma_.begin(); itProf != solAlocProfTurma_.end(); itProf++ )
	{
		outProfTurmas << "\n\nProf " << itProf->first->getId() << ": ";

		for ( auto itCp = itProf->second.begin(); itCp != itProf->second.end(); itCp++ )
		{
			for ( auto itDisc = itCp->second.begin(); itDisc != itCp->second.end(); itDisc++ )
			{
				for ( auto itTurma = itDisc->second.begin(); itTurma != itDisc->second.end(); itTurma++ )
				{				
					outProfTurmas << " (Cp" << itCp->first->getId()
						<< "Disc" << itDisc->first->getId()
						<< "Turma" << *itTurma << ")";
				}
			}
		}
	}
	outProfTurmas.close();
}
