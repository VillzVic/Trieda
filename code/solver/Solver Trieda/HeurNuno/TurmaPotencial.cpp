#include "TurmaPotencial.h"
#include "UtilHeur.h"
#include "ProfessorHeur.h"
#include "ParametrosHeuristica.h"
#include "AulaHeur.h"
#include "DadosHeuristica.h"
#include "../ProblemData.h"
#include "../ParametrosPlanejamento.h"
#include "HeuristicaNuno.h"

int TurmaPotencial::nrBuild = 0;
int TurmaPotencial::nrDestroy = 0;
unsigned int TurmaPotencial::globalIdCount_ = 0;

TurmaPotencial::TurmaPotencial(int turmaId, OfertaDisciplina* const oft, Calendario * const cal, bool tipo, unordered_map<int, AulaHeur*> aul,
			  ProfessorHeur* const prof, SalaHeur* const sal, set<AlunoHeur*> aluns, int alunDisp, bool algPotProf)
			  : id(turmaId), ofertaDisc(oft), calendario(cal), tipoAula(tipo), aulas(aul), professor(prof), sala(sal), 
			  alunos(aluns), algumPotencialProf(algPotProf), ok(true), globalId_(++TurmaPotencial::globalIdCount_)
{
	calcularValor_(aluns.size());
}

TurmaPotencial::TurmaPotencial(void)
	: ok(false), globalId_(++TurmaPotencial::globalIdCount_)
{
}

TurmaPotencial::~TurmaPotencial(void)
{
}

// assignment operator
void TurmaPotencial::operator= (const TurmaPotencial &other)
{
	id = other.id;
	ofertaDisc = other.ofertaDisc;
	calendario = other.calendario;
	tipoAula = other.tipoAula;
	aulas = other.aulas;
	professor = other.professor;
	sala = other.sala;
	alunos = other.alunos;
	algumPotencialProf = other.algumPotencialProf;
	valor_ = other.getValor();
	ok = (alunos.size() > 0);
}

int TurmaPotencial::getTipoTurma(void) const 
{
	int idx = 2;
	if(!professor->ehVirtual())
		idx = 0;
	else if(algumPotencialProf)
		idx = 1;
	return idx;
}

void TurmaPotencial::calcularValor_(int alunosDisp)
{
	valor_ = 0;

	// nr de alunos
	valor_ = alunos.size();
	
	// taxa ocupação da sala
	//double capUsada = double(alunos_.size()) / sala_->getCapacidade();
	//valor_ += capUsada;

	// penalizar indice demanda da sala
	double indicSala = sala->getIndicDem();
	if(ParametrosHeuristica::indicDemSalas)
		valor_ -= (indicSala / 10000);

	// pequeno incentivo a turmas com prof real e horarios disponiveis
	if(algumPotencialProf)
		valor_ += 0.001;
	if(!professor->ehVirtual())
		valor_ += 0.01;
}

// verifica se esta turma domina outra
bool TurmaPotencial::domina(const TurmaPotencial& outra) const
{
	// se a disponibilidade de prof for menor, não domina
	if(getTipoTurma() > outra.getTipoTurma())
		return false;
	// se o valor for menor não domina
	if(getValor() < outra.getValor())
		return false;

	// verificar se uma usa sabado e outra não
	bool sab = usaSabado();
	bool othSab = outra.usaSabado();
	if(sab != othSab)
		return othSab;

	// verificar se uma tem menos dias que a outra
	int diffDias = aulas.size() - outra.aulas.size();
	if(diffDias != 0)
		return (diffDias < 0);

	// verificar aulas
	for(auto itAulas = aulas.cbegin(); itAulas != aulas.cend(); ++itAulas)
	{
		int dia = itAulas->first;
		
		auto itAulaOutra = outra.aulas.find(dia);
		if(itAulaOutra == outra.aulas.end())
			return false;	// divisões de créditos diferentes

		HorarioAula* const primHor = *(itAulas->second->horarios.begin());
		HorarioAula* const primHorOutra = *(itAulaOutra->second->horarios.begin());
		
		// aula de outra turma é mais cedo
		if(primHorOutra->getInicio() < primHor->getInicio())
			return false;
	}

	return true;
}
bool TurmaPotencial::domina(const TurmaPotencial* const outra) const
{
	return domina(*outra);
}