#include "TurmaHeur.h"
#include "UtilHeur.h"
#include "AulaHeur.h"
#include "SalaHeur.h"
#include "OfertaDisciplina.h"
#include "ProfessorHeur.h"
#include "../Aluno.h"
#include "EntidadeAlocavel.h"
#include "TurmaPotencial.h"
#include "HeuristicaNuno.h"
#include "DadosHeuristica.h"
#include "SolucaoHeur.h"
#include "StatsSolucaoHeur.h"
#include "../ProblemData.h"
#include "../AlunoDemanda.h"
#include "../Demanda.h"
#include "../ParametrosPlanejamento.h"
#include "MIPAlocarAlunos.h"
#include "ParametrosHeuristica.h"
#include <stddef.h>

unsigned int TurmaHeur::globalIdCount_ = 0;

// construtor com base em turma potencial
TurmaHeur::TurmaHeur(const TurmaPotencial* &turmaPot, int turmaId)
	: ofertaDisc(turmaPot->ofertaDisc), calendario(turmaPot->calendario), id(turmaId), tipoAula(turmaPot->tipoAula), 
	aulas_(turmaPot->aulas), sala_(turmaPot->sala), professor_(turmaPot->professor), nrAlunos_(0), nrFormandos_(0), nrCoRequisito_(0),
	carregada(false), globalId_(++TurmaHeur::globalIdCount_), salaLoaded(nullptr), keep_(false), mustAbrirMIP_(false),
	profFixado_(false), salaFixada_(false)
{
	// NOTA! só se pode adicionar a turma ao prof/sala fora do construtor
	// NOTA! Alunos adicionados em OfertaDisciplina::abrirTurma
}

// construtor usado no carregamento de solução apartir da problem solution
TurmaHeur::TurmaHeur(OfertaDisciplina* const oferta, bool teorico, int turmaId, SalaHeur* const sala, 
	ProfessorHeur* const professor, const AtendFixacao &fixacoes)
	: ofertaDisc(oferta), tipoAula(teorico), id(turmaId), sala_(sala), salaLoaded(sala), professor_(professor), nrAlunos_(0), nrFormandos_(0),
	nrCoRequisito_(0), carregada(true), globalId_(++TurmaHeur::globalIdCount_), keep_(false), mustAbrirMIP_(false),
	profFixado_(false), salaFixada_(false),
	capacidadeRestante_(10000)
{
	int maxAlunosDisc = ofertaDisc->getMaxAlunos(tipoAula);
	int capacidade = min(maxAlunosDisc, sala->getSala()->getCapacidade());
	capacidadeRestante_ = capacidade - nrAlunos_;

	if(capacidadeRestante_ < 0)
		HeuristicaNuno::excepcao("TurmaHeur::TurmaHeur", "Capacidade restante menor que zero!");

	salaFixada_ = fixacoes.fixaSala;
	profFixado_ = fixacoes.fixaProf;
	setMustAbrirMIP(fixacoes.fixaAbertura);
}

TurmaHeur::~TurmaHeur(void)
{
}

bool TurmaHeur::ehTeoricaTag(void) const
{
	Disciplina* const disc = ofertaDisc->getDisciplina(tipoAula);
	if(disc == nullptr)
		HeuristicaNuno::excepcao("TurmaHeur::ehTeoricaTag", "Oferta nao tem disciplina para esse tipo de aula!");

	return (disc->getCredTeoricos() > 0);
}

bool TurmaHeur::discEhLab(void) const
{
	Disciplina* const disc = ofertaDisc->getDisciplina(tipoAula);
	if(disc == nullptr)
		HeuristicaNuno::excepcao("TurmaHeur::discEhLab", "Disciplina e nula");

	return disc->eLab();
}

bool TurmaHeur::getAulaDia(int dia, AulaHeur* &aula) const
{
	auto it = aulas_.find(dia);
	if(it == aulas_.end())
	{
		aula = nullptr;
		return false;
	}
	
	aula = it->second;
	return true;
}

// setSala
void TurmaHeur::setSala(SalaHeur* const sala)
{
	if(sala == nullptr)
		HeuristicaNuno::excepcao("TurmaHeur::setSala", "Sala nula!");

	if(sala_->getId() == sala->getId())
		return;

	// remover turma
	sala_->removeTurma(this);
	if(sala_->mesmoLocal(sala) == false)
	{
		unordered_map<int, AulaHeur*> newAulas;
		SalaHeur::getNovasAulas(aulas_, sala, newAulas);
		setNewAulas(newAulas);
	}

	// set nova sala
	sala_ = sala;
	sala->addTurma(this);

	int maxAlunosDisc = ofertaDisc->getMaxAlunos(tipoAula);
	int capacidade = min(maxAlunosDisc, sala->getSala()->getCapacidade());
	capacidadeRestante_ = capacidade - nrAlunos_;
	
	if(capacidadeRestante_ < 0)
		HeuristicaNuno::excepcao("TurmaHeur::setSala", "Número de alunos excede capacidade da turma!");
}
void TurmaHeur::setProfessor(ProfessorHeur* const professor)
{
	// remove anterior
	if(professor_ != nullptr)
	{
		professor_->removeTurma(this);
		if(!professor_->ehVirtual()) 
			ofertaDisc->incProfReal(-1);
		else
			ofertaDisc->solucao_->stats_->nrCreditosProfsVirtuais_ -= getNrCreditos();
	}

	// set novo professor
	professor_ = professor;
	if(professor != nullptr)
	{
		professor->addTurma(this);
		if(!professor->ehVirtual()) 
			ofertaDisc->incProfReal(1);
		else
			ofertaDisc->solucao_->stats_->nrCreditosProfsVirtuais_ += getNrCreditos();
	}
}

int TurmaHeur::unidadeId(void) const { if(sala_ == nullptr) return 1; else return sala_->unidadeId(); }
double TurmaHeur::getOcupacao()
{ 
	auto itOft = ofertaDisc->turmasAlunos_.find(this);
	if(itOft  == ofertaDisc->turmasAlunos_.end())
		HeuristicaNuno::excepcao("TurmaHeur::getOcupacao", "Turma nao encontrada em turmasAlunos_");

	return (double(itOft->second.size())/sala_->getCapacidade());
}

// add turma to prof e sala
void TurmaHeur::addTurmaProfSala(void)
{
	sala_->addTurma(this);
	professor_->addTurma(this);

	// definir capacidade restante
	int maxAlunosDisc = ofertaDisc->getMaxAlunos(tipoAula);
	if(maxAlunosDisc <= 0)
		maxAlunosDisc = 1000;
	int capacidade = sala_->getSala()->getCapacidade();
	int max = (maxAlunosDisc < capacidade) ? maxAlunosDisc : capacidade;
	capacidadeRestante_ = max - nrAlunos_;
	
	if(capacidadeRestante_ < 0)
	{
		HeuristicaNuno::logMsgInt("capacidade sala: ", capacidade, 1);
		HeuristicaNuno::logMsgInt("max alunos disc: ", maxAlunosDisc, 1);
		HeuristicaNuno::logMsgInt("nr alunos: ", nrAlunos_, 1);
		HeuristicaNuno::excepcao("TurmaHeur::setSala", "Número de alunos excede capacidade da turma!");
	}
}

// set novas aulas. só mudar alunos e prof porque a sala é mudada quando se faz set sala. caso contrário é um teste
void TurmaHeur::setNewAulas(unordered_map<int, AulaHeur*> &aulas)
{
	HeuristicaNuno::logMsg("set new aulas", 1);

	auto itOft = ofertaDisc->turmasAlunos_.find(this);
	if(itOft  == ofertaDisc->turmasAlunos_.end())
		HeuristicaNuno::excepcao("TurmaHeur::setNewAulas", "Turma nao encontrada em turmasAlunos_");

	// remover as aulas antigas
	for(auto it = itOft->second.begin(); it != itOft->second.end(); ++it)
		(*it)->removeAulasTurma(this);

	if(professor_ != nullptr && !professor_->ehVirtualUnico())
		professor_->removeAulasTurma(this);

	// setar as novas
	aulas_ = aulas;

	// adicionar aos alunos
	for(auto it = itOft->second.begin(); it != itOft->second.end(); ++it)
		(*it)->addAulasTurma(this);

	if(!professor_->ehVirtualUnico() && professor_ != nullptr)
		professor_->addAulasTurma(this);
}

// não chamar directamente, usar oferta disciplina!
void TurmaHeur::fecharTurma_()
{
	if(nrAlunos_ > 0)
		HeuristicaNuno::excepcao("TurmaHeur::fecharTurma_", "Alunos ja deviam ter sido todos removidos");

	// desmarcar aulas prof e sala
	if(professor_ != nullptr)
		professor_->removeTurma(this);
	if(sala_ != nullptr)
		sala_->removeTurma(this);
}

// alunos
void TurmaHeur::getAlunos (unordered_set<AlunoHeur*> &alunos) 
{ 
	auto itOft = ofertaDisc->turmasAlunos_.find(this);
	if(itOft  == ofertaDisc->turmasAlunos_.end())
		HeuristicaNuno::excepcao("TurmaHeur::getAlunos", "Turma nao encontrada em turmasAlunos_");

	alunos = itOft->second;
}
void TurmaHeur::getDemandasAlunos(unordered_map<Demanda*, set<AlunoDemanda*>>& demandas)
{
	auto itOft = ofertaDisc->turmasAlunos_.find(this);
	if(itOft  == ofertaDisc->turmasAlunos_.end())
		HeuristicaNuno::excepcao("TurmaHeur::getDemandasAlunos", "Turma nao encontrada em turmasAlunos_");

	set<AlunoDemanda*> conj;
	for(auto itAlunos = itOft->second.cbegin(); itAlunos != itOft->second.cend(); ++itAlunos)
	{
		AlunoHeur* const aluno = *itAlunos;
		AlunoDemanda* const alunoDem = aluno->getDemandaOriginal(ofertaDisc->getCampus(), ofertaDisc->getDisciplina());
		Demanda* const demanda =  alunoDem->demanda;
		
		auto itDem = demandas.find(demanda);
		if(itDem == demandas.end())
			itDem = demandas.insert(make_pair(demanda, conj)).first;

		itDem->second.insert(alunoDem);
	}
}

// turma tem que ficar aberta?
bool TurmaHeur::mustAbrirMIP(void) const 
{ 
	if(mustAbrirMIP_)
		return true;

	if(carregada && !ParametrosHeuristica::fecharTurmasCarregadas)
		return true;

	return false;
}

bool TurmaHeur::ehCompSec(void) const { return (!tipoAula && ofertaDisc->temCompTeorica()); }

// nr créditos
int TurmaHeur::getNrCreditos() const
{
	int nrCreditos = 0;
	for(auto itAulas = aulas_.cbegin(); itAulas != aulas_.cend(); ++itAulas)
	{
		nrCreditos += itAulas->second->nrCreditos();
	}

	return nrCreditos;
}
bool TurmaHeur::temAluno(int alunoId)
{
	auto itOft = ofertaDisc->turmasAlunos_.find(this);
	if(itOft  == ofertaDisc->turmasAlunos_.end())
		HeuristicaNuno::excepcao("TurmaHeur::temAluno(int)", "Turma nao encontrada em turmasAlunos_");

	for(auto it = itOft->second.begin(); it!= itOft->second.end(); it++)
	{
		if((*it)->getId() == alunoId)
			return true;
	}
	return false;
}
bool TurmaHeur::temAluno(AlunoHeur* const aluno)
{
	auto itOft = ofertaDisc->turmasAlunos_.find(this);
	if(itOft  == ofertaDisc->turmasAlunos_.end())
		HeuristicaNuno::excepcao("TurmaHeur::temAluno", "Turma nao encontrada em turmasAlunos_");

	return itOft->second.find(aluno) != itOft->second.end();
}
// verifica se ainda tem o numero de alunos abaixo do máximo para a disciplina
bool TurmaHeur::podeTerMaisAlunos (void) const
{
	return nrAlunos_ < ofertaDisc->getMaxAlunos(tipoAula);
}

// compatibilidade
bool TurmaHeur::ehIncompativel(TurmaHeur* const outra, bool deslocavel) const
{
	// verificar se ha aulas coincidentes
	for(auto itDiasUm = aulas_.cbegin(); itDiasUm != aulas_.cend(); itDiasUm++)
	{
		int dia = itDiasUm -> first;
		// aulas de cada turma marcadas no dia em questão
		AulaHeur* aulasDiaDois = nullptr;
		if(!outra->getAulaDia(dia, aulasDiaDois))
			continue;

		// verificar se aulas são compatíveis
		if(!EntidadeAlocavel::aulasCompativeis(itDiasUm->second, aulasDiaDois, deslocavel))
			return true;
	}
	return false;
}
bool TurmaHeur::ehIncompativel(unordered_set<TurmaHeur*> const &outras, bool deslocavel) const
{
	for(auto it = outras.begin(); it != outras.end(); ++it)
	{
		if(ehIncompativel(*it, deslocavel))
			return true;
	}
	return false;
}
bool TurmaHeur::ehIncompativel(int dia, HorarioAula* const horario, bool deslocavel, int campId, int unidId) const
{
	auto itDia = aulas_.find(dia);
	if(itDia == aulas_.end())
		return false;

	return itDia->second->intersectaHorario(horario, deslocavel, campId, unidId);
}
bool TurmaHeur::ehIncompativel(int dia, AulaHeur* const aula, bool deslocavel) const
{
	auto it = aulas_.find(dia);
	if(it == aulas_.end())
		return false;

	return !EntidadeAlocavel::aulasCompativeis(it->second, aula, deslocavel);
}

// [LOAD Solução] add aula
void TurmaHeur::addAula(int dia, AulaHeur* const aula)
{
	if(nrAlunos_ > 0)
		HeuristicaNuno::excepcao("TurmaHeur::addAula", "Ja ha alunos na turma!");

	auto itDia = aulas_.find(dia);
	if(itDia != aulas_.end())
		HeuristicaNuno::excepcao("TurmaHeur::addAula", "Ja ha uma aula registada nesse dia!");

	aulas_[dia] = aula;
}

// verificação
bool TurmaHeur::alocacaoCompleta() const 
{
	int nrCreds = ofertaDisc->getDisciplina(tipoAula)->getTotalCreditos();
	int credsAlocados = 0;
	for(auto itAulas = aulas_.cbegin(); itAulas != aulas_.cend(); ++itAulas)
	{
		credsAlocados += itAulas->second->nrCreditos();
	}

	if(credsAlocados > nrCreds)
		HeuristicaNuno::excepcao("TurmaHeur::alocacaoCompleta", "Alocados mais créditos que os necessários");

	return nrCreds == credsAlocados;
}
bool TurmaHeur::ehIlegal(double relaxMin) const
{
	if(nrAlunos_ == 0)
		return true;

	// check capacidade
	if(sala_ != nullptr && nrAlunos_ > sala_->getCapacidade())
		HeuristicaNuno::excepcao("TurmaHeur::ehIlegal()", "Turma excede capacidade da sala!!!");

	// tem pelo menos um formando
	if(HeuristicaNuno::probData->parametros->violar_min_alunos_turmas_formandos && nrFormandos_ > 0)
		return false;

	// tem pelo menos um aluno com co-requisito
	if(HeuristicaNuno::probData->parametros->considerarCoRequisitos && nrCoRequisito_ > 0)
		return false;

	// componente secundaria
	double minD = double(ParametrosHeuristica::getMinAlunos(discEhLab()) * relaxMin);
	int min = int(std::ceil(minD));

	return (nrAlunos_ < min);
}

void TurmaHeur::checkCreditos(void) const
{
	int nrCredsTurma = getNrCreditos();
	int nrCredsDisc = ofertaDisc->getDisciplina(tipoAula)->getTotalCreditos();

	if(nrCredsDisc != nrCredsTurma)
	{
		HeuristicaNuno::logMsgInt("nr creds turma: ", nrCredsTurma, 1);
		HeuristicaNuno::logMsgInt("nr creds disciplina: ", nrCredsDisc, 1);
		HeuristicaNuno::excepcao("TurmaHeur::checkCreditos", "Número de créditos da turma é diferente do necessário!!!");
	}
}

// add/remove aluno
void TurmaHeur::addAluno(AlunoHeur* const aluno, bool fixado)
{
	if(aluno == nullptr)
	{
		HeuristicaNuno::warning("TurmaHeur::addAluno","Aluno nulo!");
		return;
	}

	nrAlunos_++;
	capacidadeRestante_--;
	if(capacidadeRestante_ < 0 || (nrAlunos_ > sala_->getCapacidade()))
		HeuristicaNuno::excepcao("TurmaHeur::addAluno", "Número de alunos excede capacidade da turma!");

	// check se é formando
	if(aluno->getAluno()->ehFormando())
		nrFormandos_++;

	// check se é co-requisito
	if(aluno->temCoRequisito(this->ofertaDisc))
		nrCoRequisito_++;

	aluno->addTurma(this);

	if(fixado)
		alunosFixados_.insert(aluno->getId());
}
void TurmaHeur::removeAluno(AlunoHeur* const aluno)
{
	if(aluno == nullptr)
	{
		HeuristicaNuno::warning("TurmaHeur::removeAluno","Aluno nulo!");
		return;
	}

	nrAlunos_--;
	capacidadeRestante_++;
	if(capacidadeRestante_ > sala_->getCapacidade())
		HeuristicaNuno::excepcao("TurmaHeur::removeAluno", "Capacidade restante excede a capacidade da sala!");

	// check se é formando
	if(aluno->getAluno()->ehFormando())
		nrFormandos_--;

	// check se é co-requisito
	if(aluno->temCoRequisito(ofertaDisc))
		nrCoRequisito_--;

	aluno->removeTurma(this);

	// não apagar fixados! manter sempre essa informação. ùtil para o mip
	/*auto it = alunosFixados_.find(aluno->getId());
	if(it != alunosFixados_.end())
		alunosFixados_.erase(it);*/
}

// check
bool TurmaHeur::checkLink (TurmaHeur* const turma)
{
	bool ok = true;

	// professor
	if(!turma->getProfessor()->ehVirtualUnico() && !turma->getProfessor()->temTurma(turma))
	{
		ok = false;
		HeuristicaNuno::warning("TurmaHeur::checkLink", "Professor e turma não linkados!");
	}

	// sala
	if(!turma->getSala()->temTurma(turma))
	{
		ok = false;
		HeuristicaNuno::warning("TurmaHeur::checkLink", "Sala e turma não linkados!");
	}

	// alunos
	unordered_set<AlunoHeur*> alunos;
	turma->getAlunos(alunos);
	for(auto itAlunos = alunos.cbegin(); itAlunos != alunos.cend(); ++itAlunos)
	{
		if(!(*itAlunos)->temTurma(turma))
		{
			ok = false;
			HeuristicaNuno::warning("TurmaHeur::checkLink", "Aluno e turma não linkados!");
		}
	}

	return ok;
}

// obter coeficiente de abertura de turma com base no nr de alunos que mantém
double TurmaHeur::getCoefMIPAbrir(void) const
{
	// só considerar as demandas atendidas no principal para nao contabilizar demandas atendidas duas vezes!
	if(nrAlunos_ == 0 || ehCompSec())
		return 0.0;

	// get alunos
	unordered_set<AlunoHeur*> alunos;
	for(auto itOft = ofertaDisc->turmasAlunos_.cbegin(); itOft != ofertaDisc->turmasAlunos_.cend(); ++itOft)
	{
		if(itOft->first->getGlobalId() == globalId_)
		{
			alunos = itOft->second;
			break;
		}
	}

	double coefTotal = 0.0;
	for(auto it = alunos.cbegin(); it != alunos.cend(); ++it)
	{
		AlunoDemanda* const demanda = (*it)->getDemandaOriginal(ofertaDisc, false);
		double coef = ParametrosHeuristica::coefDemAtendP1;	// default

		// demanda não encontrada. P1 default
		if(demanda != nullptr)
		{
			coef = MIPAlocarAlunos::getCoefVarAlunoOft_(ofertaDisc, *it, demanda->getPrioridade());
		}

		coefTotal += coef;
	}

	return coefTotal;
}

// nr formandos
bool TurmaHeur::temFormandos(unordered_set<AlunoHeur*> const &alunos)
{
	for(auto itAlunos = alunos.cbegin(); itAlunos != alunos.cend(); ++itAlunos)
	{
		if((*itAlunos)->getAluno()->ehFormando())
			return true;
	}

	return false;
}


// Operadores
bool TurmaHeur::operator ==(const TurmaHeur &other) const
{
	/*if(*ofertaDisc != *(other.ofertaDisc))
		return false;

	if(tipoAula != other.tipoAula)
		return false;

	return id == other.id;*/

	return globalId_ == other.globalId_;
}

bool TurmaHeur::operator < (const TurmaHeur &other) const
{
	if(nrDisp_ < other.getNrDisp())
		return true;
	else if(nrDisp_ > other.getNrDisp())
		return false;

	if(ofertaDisc < other.ofertaDisc)
		return true;
	else if(ofertaDisc > other.ofertaDisc)
		return false;

	// teórica primeiro, convenção
	if(tipoAula && !other.tipoAula)
		return true;

	return id < other.id;
}

// Comparadores

bool compTurmaOcup::operator() (TurmaHeur* const first, TurmaHeur* const second)
{
	// prioridade a menor ocupação
	double fstOcup = first->getOcupacao();
	double scdOcup = second->getOcupacao();
	double diff = fstOcup - scdOcup;
	if(std::abs(diff) > 0.00001)
		return diff < 0;

	// prioridade a maior indice demanda.
	int indFst = first->getSala()->getIndicDem();
	int secFst = second->getSala()->getIndicDem();
	int diffInd = indFst - secFst;
	if(diffInd != 0)
		return diffInd > 0;

	return first->id < second->id;
}

bool compNrAlunos::operator() (TurmaHeur* const first, TurmaHeur* const second)
{
	int fstNrTA = first->ofertaDisc->nrTiposAula();
	int scdNrTA = second->ofertaDisc->nrTiposAula();
	int diff = fstNrTA - scdNrTA;
	if(diff != 0)
		return diff < 0;

	int fstAlunos = first->getNrAlunos();
	int scdAlunos = second->getNrAlunos();
	diff = fstAlunos - scdAlunos;
	if(diff != 0)
		return diff < 0;

	int fstCreds = first->getNrCreditos();
	int scdCreds = second->getNrCreditos();
	diff = fstCreds - scdCreds;
	if(diff != 0)
		return diff > 0;

	return first < second;
}