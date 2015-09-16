#include "OfertaDisciplina.h"
#include "GGroup.h"
#include "../ProblemData.h"
#include "../HorarioAula.h"
#include "../Aluno.h"
#include "../AlunoDemanda.h"
#include "../Calendario.h"
#include "../ConjUnidades.h"
#include "TurmaHeur.h"
#include "AlunoHeur.h"
#include "ProfessorHeur.h"
#include "SalaHeur.h"
#include "TurmaPotencial.h"
#include "SolucaoHeur.h"
#include "DadosHeuristica.h"
#include "HeuristicaNuno.h"
#include "ParametrosHeuristica.h"
#include "GeradorCombsDiv.h"

size_t OfertaDisciplina::globalIdCount_ = 0;

OfertaDisciplina::OfertaDisciplina(SolucaoHeur* const solucao, Disciplina* const disciplinaTeorica, Disciplina* const disciplinaPratica, Campus* const campus)
	: solucao_(solucao), disciplinaTeorica_(disciplinaTeorica), disciplinaPratica_(disciplinaPratica), campus_(campus),
	temDivs_(true), temSalas_(true), nrLabsAssoc_(0), turmaId_(0), globalId_(++OfertaDisciplina::globalIdCount_)
{
	// turmas teóricas
	unordered_map<int, TurmaHeur*> mapT;
	pair<bool, unordered_map<int, TurmaHeur*>> parT (true, mapT);
	turmas_.insert(parT);

	// turmas práticas
	unordered_map<int, TurmaHeur*> mapP;
	pair<bool, unordered_map<int, TurmaHeur*>> parP (false, mapP);
	turmas_.insert(parP);

	// max alunos
	maxAlunosTipoAula[true] = 0;
	maxAlunosTipoAula[false] = 0;
	if(disciplinaTeorica_ != nullptr)
		maxAlunosTipoAula[true] = disciplinaTeorica_->getMaxAlunosT();
	if(disciplinaPratica_ != nullptr)
		maxAlunosTipoAula[false] = disciplinaPratica_->getMaxAlunosP();

	if(maxAlunosTipoAula.at(true) <= 0)
		maxAlunosTipoAula[true] = 10000;
	if(maxAlunosTipoAula.at(false) <= 0)
		maxAlunosTipoAula[false] = 10000;

	// verificar se teorica tem divisoes
	if(disciplinaTeorica_ != nullptr && disciplinaTeorica_->combinacao_divisao_creditos.size() == 0)
	{
		HeuristicaNuno::logDiscSemDivisao(disciplinaTeorica_->getId(), true, false);
		temDivs_ = false;
	}
	// verificar se pratica tem divisoes
	if(disciplinaPratica_ != nullptr && disciplinaPratica_->combinacao_divisao_creditos.size() == 0)
	{
		HeuristicaNuno::logDiscSemDivisao(disciplinaPratica_->getId(), false, (disciplinaTeorica_ != nullptr));
		temDivs_ = false;
	}

	// contabilizar numero de laboratorios associados a parte pratica
	setLabsAssoc();

	// preencher map com disponibilidades originais comuns entre os profs e a disciplina principal
	preencheMapDisponibComumProf();
	// preencher map com salas associadas
	preencheSalasAssoc();
}

// com cursos associados!
OfertaDisciplina::OfertaDisciplina(SolucaoHeur* const solucao, Disciplina* const disciplinaTeorica, Disciplina* const disciplinaPratica, Campus* const campus,
					set<Curso*> const &curs)
	: solucao_(solucao), disciplinaTeorica_(disciplinaTeorica), disciplinaPratica_(disciplinaPratica), campus_(campus), cursos(curs),
	temDivs_(true), temSalas_(true), nrLabsAssoc_(0), turmaId_(0), globalId_(++OfertaDisciplina::globalIdCount_)
{
	// turmas teóricas
	unordered_map<int, TurmaHeur*> mapT;
	pair<bool, unordered_map<int, TurmaHeur*>> parT (true, mapT);
	turmas_.insert(parT);

	// turmas práticas
	unordered_map<int, TurmaHeur*> mapP;
	pair<bool, unordered_map<int, TurmaHeur*>> parP (false, mapP);
	turmas_.insert(parP);

	// max alunos
	maxAlunosTipoAula[true] = 0;
	maxAlunosTipoAula[false] = 0;
	if(disciplinaTeorica_ != nullptr)
		maxAlunosTipoAula[true] = disciplinaTeorica_->getMaxAlunosT();
	if(disciplinaPratica_ != nullptr)
		maxAlunosTipoAula[false] = disciplinaPratica_->getMaxAlunosP();

	if(maxAlunosTipoAula.at(true) <= 0)
		maxAlunosTipoAula[true] = 10000;
	if(maxAlunosTipoAula.at(false) <= 0)
		maxAlunosTipoAula[false] = 10000;

	// verificar se teorica tem divisoes
	if(disciplinaTeorica_ != nullptr && disciplinaTeorica_->combinacao_divisao_creditos.size() == 0)
	{
		HeuristicaNuno::logDiscSemDivisao(disciplinaTeorica_->getId(), true, false);
		temDivs_ = false;
	}
	// verificar se pratica tem divisoes
	if(disciplinaPratica_ != nullptr && disciplinaPratica_->combinacao_divisao_creditos.size() == 0)
	{
		HeuristicaNuno::logDiscSemDivisao(disciplinaPratica_->getId(), false, (disciplinaTeorica_ != nullptr));
		temDivs_ = false;
	}

	// contabilizar numero de laboratorios associados a parte pratica
	setLabsAssoc();
}

OfertaDisciplina::~OfertaDisciplina(void)
{
	// limpar para poder remover os alunos sem problema.
	keepAlunos_.clear();

	// fechar turmas
	auto turmasCopy = turmas_;
	for(auto it = turmasCopy.begin(); it != turmasCopy.end(); ++it)
	{
		for(auto itTurma = it->second.begin(); itTurma != it->second.end();)
		{
			TurmaHeur* turma = itTurma->second;
			itTurma = it->second.erase(itTurma);
			fecharTurma(turma);
		}
	}

	turmas_.clear();
	alunosTurma_.clear();
	alunosIncompleto_.clear();
}

int OfertaDisciplina::getNextTurmaId(bool teorica)
{
	/*auto turmas = turmas_.find(teorica);
	for(int i=1; ; ++i)
	{
		if(turmas->second.find(i) == turmas->second.end())
		{
			return i;
		}
	}
	return -1;*/

	return ++turmaId_;
}

// gera divisoes caso uma disciplina não as tenha
void OfertaDisciplina::gerarDivs(void)
{
	// para teorica
	if(disciplinaTeorica_ != nullptr)
	{
		GeradorCombsDiv geraDivs (disciplinaTeorica_);
		geraDivs.gerarDivisoes();
	}
	// para pratica
	if(disciplinaPratica_ != nullptr)
	{
		GeradorCombsDiv geraDivs (disciplinaPratica_);
		geraDivs.gerarDivisoes();
	}
	temDivs_ = true;
}

// define número de demandas não atendidas (por prioridade) que podem ser satisfeitas por esta oferta
// também actualiza os indicadores de demanda das salas (se activado)
void OfertaDisciplina::setDemandasNaoAtend(unordered_map<int, unordered_set<AlunoDemanda *>> const &demandas, bool equiv) 
{
	// diminuir indice demanda das salas associadas, removendo o valor antigo com o qual a OfertaDisciplina contribuía
	if(ParametrosHeuristica::indicDemSalas && nrDemandasNaoAtend_.size() > 0)
		decIncIndicDem_(false, equiv);
	
	// Atualiza o nro de nao atendimentos da OfertaDisciplina
	nrDemandasNaoAtend_.clear();
	for(auto itPrior = demandas.begin(); itPrior != demandas.end(); ++itPrior)
	{
		nrDemandasNaoAtend_[itPrior->first] = (int)itPrior->second.size();
	}

	// aumentar indice demanda das salas associadas, acrescentando o valor atualizado com o qual a OfertaDisciplina contribui
	if(ParametrosHeuristica::indicDemSalas)
		decIncIndicDem_(true, equiv);
}
void OfertaDisciplina::setDemandasNaoAtend(unordered_map<int, int> const &demandasPrior, bool equiv)
{
	// diminuir indice demanda das salas associadas
	if(ParametrosHeuristica::indicDemSalas && nrDemandasNaoAtend_.size() > 0)
		decIncIndicDem_(false, equiv);

	nrDemandasNaoAtend_.clear();
	for(auto itPrior = demandasPrior.begin(); itPrior != demandasPrior.end(); ++itPrior)
	{
		nrDemandasNaoAtend_[itPrior->first] = itPrior->second;
	}

	// aumentar indice demanda das salas associadas
	if(ParametrosHeuristica::indicDemSalas)
		decIncIndicDem_(true, equiv);
}
// get nrDemandas nao atendidas para uma prioridade
int OfertaDisciplina::getNrDemandasNaoAtend(int prioridade, bool equiv) const
{
	int nr = 0;
	auto it = nrDemandasNaoAtend_.find(prioridade);
	if(it != nrDemandasNaoAtend_.end())
		nr += it->second;

	if(equiv)
	{
		auto itEq = nrDemandasNaoAtend_.find(-prioridade);
		if(itEq != nrDemandasNaoAtend_.end())
			nr += itEq->second;
	}
	
	return nr;
}
// retorna o valor para comparação entre ofertas disciplina
double OfertaDisciplina::valorDemandasNaoAtendP1Equiv(void) const
{
	double valor = 0;

	valor = getNrDemandasNaoAtend(1) + getNrDemandasNaoAtend(-1);

	return valor;
}
// só considera prioridade 1 (s/ equiv) e coloca peso no credito de alunos.
double OfertaDisciplina::valorDemandasNaoAtendV2(int prioridade) const
{
	if(prioridade == 0)
		return valorDemandasNaoAtendP1Equiv()*getNrCreds();

	auto it = nrDemandasNaoAtend_.find(prioridade);
	if(it == nrDemandasNaoAtend_.end())
		return 0;

	double valor = it->second * (this->getNrCreds());

	return valor;
}

Disciplina* OfertaDisciplina::getDisciplina (void) const 
{ 
	Disciplina* ptr = nullptr;
	//HeuristicaNuno::logMsg("get disciplina", 3);
	if(disciplinaTeorica_ != nullptr)
		ptr = disciplinaTeorica_;
	else
	{
		if(disciplinaPratica_->getId() < 0)
			HeuristicaNuno::warning("OfertaDisciplina::getDisciplina", "Disciplina 100% Prática devia ter ID positivo!");
		ptr =  disciplinaPratica_;
	}

	if(ptr == nullptr)
		HeuristicaNuno::excepcao("OfertaDisciplina:: getDisciplina", "Pointer to disciplina não encontrado");

	return ptr;
}

void OfertaDisciplina::getAlunos(unordered_set<AlunoHeur*>& alunos) const
{
	for(auto itAlunos = alunosTurma_.begin(); itAlunos != alunosTurma_.end(); ++itAlunos)
		alunos.insert(itAlunos->first);
}

bool OfertaDisciplina::getTeoricoCompSec(bool compSec) const
{
	if(compSec) 
		return false; 
	else 
		return disciplinaTeorica_ != nullptr;
}

bool OfertaDisciplina::tipoTurmaFromTag(bool tag) const
{
	if(disciplinaTeorica_ == nullptr && disciplinaPratica_ == nullptr)
		HeuristicaNuno::excepcao("TurmaHeur::tipoTurmaFromTag", "As duas disciplinas sao nulas");

	// se só tiver uma retornar essa
	if(disciplinaTeorica_ == nullptr)
		return (disciplinaPratica_->getCredTeoricos() > 0);
	else if(disciplinaPratica_ == nullptr)
		return (disciplinaTeorica_->getCredTeoricos() > 0);
	else
	{
		return ((disciplinaTeorica_->getCredTeoricos() > 0) && tag);
	}
}

bool OfertaDisciplina::podeAbrir(void)
{
	// divisões
	if(!temDivs_ || !temSalas_)
		return false;
		
	// verificar se algum dos componentes da turma não tem sala
	if(disciplinaTeorica_ != nullptr)
	{
		unordered_set<SalaHeur*> salasTeor;
		getSalasAssociadas(salasTeor, true);
		if(salasTeor.size() == 0)
		{
			HeuristicaNuno::logDiscSemSala(disciplinaTeorica_->getId(), true, false);
			HeuristicaNuno::warning("AbridorTurmas::tentarAbrirTurmas_", "Oferta disciplina nao pode ser aberta por nao ter salas teoricas associadas");
			temSalas_ = false;
			return false;
		}
	}
	if(disciplinaPratica_ != nullptr)
	{
		unordered_set<SalaHeur*> salasPrat;
		getSalasAssociadas(salasPrat, false);
		if(salasPrat.size() == 0)
		{
			HeuristicaNuno::logDiscSemSala(disciplinaPratica_->getId(), false, this->temCompTeorica());
			HeuristicaNuno::warning("AbridorTurmas::tentarAbrirTurmas_", "Oferta disciplina nao pode ser aberta por nao ter laboratorios praticos associados");
			temSalas_ = false;
			return false;
		}
	}

	return true;
}

// abrir/fechar turmas
TurmaHeur* OfertaDisciplina::abrirTurma(const TurmaPotencial* &turmaPot)
{
	if(turmaPot->ofertaDisc->getGlobalId() != globalId_)
	{
		HeuristicaNuno::excepcao("OfertaDisciplina::abrirTurma", "Turma potencial nao pertence a oferta!!");
		return nullptr;
	}

	int id = getNextTurmaId(turmaPot->tipoAula);
	HeuristicaNuno::logMsg("criar turma", 3);

	if(turmaPot->sala == nullptr)
		HeuristicaNuno::excepcao("OfertaDisciplina::abrirTurma", "Sala da turma potencial é nula!");
	if(turmaPot->professor == nullptr)
		HeuristicaNuno::excepcao("OfertaDisciplina::abrirTurma", "Professor da turma potencial é nula!");

	TurmaHeur* ptrTurma = new TurmaHeur(turmaPot, id);
	HeuristicaNuno::logMsg("turma criada", 3);
	ptrTurma->addTurmaProfSala();

	// registar a turma em 'turmas_' e atualizar estatísticas
	regAberturaTurma(ptrTurma);

	// adicionar alunos à turma
	for(auto itAlunos = turmaPot->alunos.begin(); itAlunos != turmaPot->alunos.end(); ++itAlunos)
		addAlunoTurma(*itAlunos, ptrTurma, "OfertaDisciplina::abrirTurma");

	return ptrTurma;
}

// abrir turma com um determinado id e numa determinada sala [Load solução]. Se ja houver uma com o mesmo tipo e id retornar essa.
// OBS: sala e professor já carregados mas associados à turma no fim do carregamento da solução
TurmaHeur* OfertaDisciplina::abrirTurma(bool teorico, int id, SalaHeur* const &sala, ProfessorHeur* const &professor,
	const AtendFixacao &fixacoes)
{
	// verificar primeiro se existe
	auto itTipo = turmas_.find(teorico);
	if(itTipo != turmas_.end())
	{
		auto itId = itTipo->second.find(id);
		if(itId != itTipo->second.end())
			return itId->second;
	}

	// Se não existe criar
	TurmaHeur* const turma = new TurmaHeur(this, teorico, id, sala, professor, fixacoes);

	// registar a turma em 'turmas_' e atualizar estatísticas
	regAberturaTurma(turma);

	if(id > turmaId_)
		turmaId_ = id;

	return turma;
}

// registrar abertura de turma
void OfertaDisciplina::regAberturaTurma(TurmaHeur* const turma)
{
	// inserir global id
	turmasGlobalIds_.insert(turma->getGlobalId());

	// verificar nr créditos
	if(!solucao_->isLoading())
		checkCredsTurma(turma, "OfertaDisciplina::regAberturaTurma");

	// teorico
	auto itTipo = turmas_.find(turma->tipoAula);
	if(itTipo == turmas_.end())
	{
		unordered_map<int, TurmaHeur*> emptyMap;
		auto parTipo = turmas_.insert(make_pair(turma->tipoAula, emptyMap));
		if(!parTipo.second)
			HeuristicaNuno::excepcao("OfertaDisciplina::regAberturaTurma", "Tipo de turma nao registado em turmas!");
		itTipo = parTipo.first;
	}
	if(itTipo->second.find(turma->id) != itTipo->second.end())
		HeuristicaNuno::excepcao("OfertaDisciplina::regAberturaTurma", "Turma do mesmo tipo ja registada com o mesmo id!!");
	auto par = itTipo->second.insert(make_pair(turma->id, turma));
	if(!par.second)
		HeuristicaNuno::excepcao("OfertaDisciplina::regAberturaTurma", "Turma nao inserida com sucesso em turmas_!");

	// adicionar estrutura alunosTurma
	unordered_set<AlunoHeur*> alunosTurmaEmpty;
	turmasAlunos_[turma] = alunosTurmaEmpty;

	// adicionar turma a turmasPorDisc_
	auto itDisc = solucao_->turmasPorDisc_.find(turma->ofertaDisc->getDisciplina());
	if(itDisc == solucao_->turmasPorDisc_.end())
	{
		unordered_set<TurmaHeur*> emptySet;
		itDisc = solucao_->turmasPorDisc_.insert(make_pair(turma->ofertaDisc->getDisciplina(), emptySet)).first;
		HeuristicaNuno::excepcao("OfertaDisciplina::regAberturaTurma", "Disciplina não encontrada em turmasPorDisc_.");
	}
	itDisc->second.insert(turma);

	// update stats abrir turma
	solucao_->stats_->nrTurmasAbertas_++;
	if(turma->ehCompSec())
		solucao_->stats_->nrTurmasAbertasCompSec_++;
	if(!turma->getProfessor()->ehVirtual())
		solucao_->stats_->nrTurmasProfReal_++;
	else
		solucao_->stats_->nrCreditosProfsVirtuais_+= turma->getNrCreditos();
	solucao_->stats_->nrCreditosProfessores_ += turma->getNrCreditos();

	Disciplina* const disc = getDisciplina();
	int ant = disc->getNumTurmas();
	disc->setNumTurmas(ant + 1);
}
// verificar se o numero de créditos da turma bate com o número de créditos da componente.  lança excepção caso contrario
void OfertaDisciplina::checkCredsTurma(TurmaHeur* const turma, std::string const &method) const
{
	int nrCreds = getNrCreds(turma->tipoAula);
	int nrCredsTurma = turma->getNrCreditos();
	if(nrCreds != nrCredsTurma)
	{
		int discId = turma->ofertaDisc->getDisciplina()->getId();
		HeuristicaNuno::logMsg("", 1);
		stringstream ssd;
		ssd << "Disciplina id: " << discId << " ; T: " << getNrCreds(true) << " P: " << getNrCreds(false);
		HeuristicaNuno::logMsg(ssd.str(), 1);

		stringstream sst;
		sst << "Tipo turma: " << turma->tipoAula << "; real tipo: " << turma->ehTeoricaTag() << " ; nr creds: " << nrCredsTurma;
		HeuristicaNuno::logMsg(sst.str(), 1);
		HeuristicaNuno::excepcao(method, "Numero de creditos da turma esta errado!");
	}
}

// fechar turma
void OfertaDisciplina::fecharTurma(TurmaHeur* &turma, bool remAssoc)
{
	HeuristicaNuno::logMsg("fechar turma", 2);

	auto itGlobalId = turmasGlobalIds_.find(turma->getGlobalId());
	if(itGlobalId ==  turmasGlobalIds_.end())
	{
		HeuristicaNuno::warning("OfertaDisciplina::fecharTurma", "Turma nao encontrada na disciplina");
		return;
	}

	// se carregada registar o fechamento e guardar
	if(turma->carregada)
	{
		turma->setKeep(true);
		solucao_->loadedTurmasClosed.insert(turma);
	}

	// update fechar turma
	solucao_->stats_->nrTurmasAbertas_--;
	if(turma->ehCompSec())
		solucao_->stats_->nrTurmasAbertasCompSec_--;
	if(!turma->getProfessor()->ehVirtual())
		solucao_->stats_->nrTurmasProfReal_--;
	solucao_->stats_->nrCreditosProfessores_ -= turma->getNrCreditos();

	// remover turma de turmasPorDisc_
	auto itDisc = solucao_->turmasPorDisc_.find(turma->ofertaDisc->getDisciplina());
	if(itDisc == solucao_->turmasPorDisc_.end())
		HeuristicaNuno::excepcao("OfertaDisciplina::fecharTurma", "Disciplina não encontrada em turmasPorDisc_.");
	else
	{
		auto itT = itDisc->second.find(turma);
		if(itT == itDisc->second.end())
			HeuristicaNuno::excepcao("OfertaDisciplina::fecharTurma", "Turma nao encontrada em turmasPorDisc_.");
		else
			itDisc->second.erase(itT);
	}

	// remover cada aluno da turma
	HeuristicaNuno::logMsg("removeAlunosTurma", 3);
	auto itAlunosT = turmasAlunos_.find(turma);
	if(itAlunosT == turmasAlunos_.end())
		HeuristicaNuno::excepcao("OfertaDisciplina::fecharTurma", "Alunos da turma nao encontrados");
	unordered_set<AlunoHeur*> alunos = itAlunosT->second;
	for(auto itAlunos = alunos.begin(); itAlunos != alunos.end(); ++itAlunos)
		removeAlunoTurma(*itAlunos, turma, true);
	HeuristicaNuno::logMsg("done", 3);

	// apagar turma da estrutura turmasAluno
	turmasAlunos_.erase(itAlunosT);

	// remover associações da turma
	if(remAssoc)
	{
		if(ParametrosHeuristica::relacPraticas != ParametrosHeuristica::abrePraticas::N_N)
			removeAssocTurma_(turma);
	}

	// apagar a turma da estrutura turmas_
	auto itTurmas = turmas_.find(turma->tipoAula);
	auto it = itTurmas->second.find(turma->id);
	if(it == itTurmas->second.end())
		HeuristicaNuno::excepcao("OfertaDisciplina::fecharTurma", "Turma não encontrada");
	itTurmas->second.erase(it);

	// finalmente fechar a turma, removendo da sala e professor
	HeuristicaNuno::logMsg("turma->fecharTurma_", 3);
	turma->fecharTurma_();
	HeuristicaNuno::logMsg("done", 3);

	// remover global id
	turmasGlobalIds_.erase(itGlobalId);

	// decrease
	Disciplina* const disc = getDisciplina();
	int ant = disc->getNumTurmas();
	disc->setNumTurmas(ant - 1);

	// free memory
	// obs: em local search deletar manualmente as turmas
	if(!turma->keep())
	{
		HeuristicaNuno::logMsg("delete turma", 3);
		delete turma;
		turma = nullptr;
		HeuristicaNuno::logMsg("done", 3);
	}
}
// fechar all turmas
void OfertaDisciplina::fecharAllTurmas(bool remAssoc)
{
	unordered_set<TurmaHeur*> turmas;
	getTurmas(turmas);
	for(auto it = turmas.begin(); it != turmas.end();)
	{
		TurmaHeur* turma = *it;
		fecharTurma(turma, remAssoc);
		turmas.erase(it++);
	}
}


// vai removendo incompletos e fechando turmas até não haver mais nada para remover
bool OfertaDisciplina::acertarOferta(double relaxMin, bool fixados)
{
	bool anyMudou = false;
	bool mudou = true;
	while(mudou)
	{
		mudou = false;
		if(alunosIncompleto_.size() > 0)
		{
			removerAlunosIncompleto(fixados);
			mudou = true;
			anyMudou = true;
		}

		if(fecharTurmasInv(relaxMin, fixados))
		{
			mudou = true;
			anyMudou = true;
		}
	}
	return anyMudou;
}
// relaxando o mínimo
bool OfertaDisciplina::fecharTurmasInv(double relaxMin, bool fixadas)
{
	if(relaxMin < 0)
	{
		HeuristicaNuno::warning("OfertaDisciplina::fecharTurmasInv", "RelaxMin menor que zero!");
		return false;
	}

	unordered_set<TurmaHeur*> allTurmas;
	getTurmas(allTurmas);
	if(allTurmas.size() == 0)
		return false;

	bool fechou = false;
	for(auto it = allTurmas.begin(); it != allTurmas.end();)
	{
		TurmaHeur* turma = *it;
		it = allTurmas.erase(it);
		if(!fixadas && (turma->getNrAlunosFix() > 0))
			continue;

		// se não for ilegal continuaar
		if(!turma->ehIlegal(relaxMin))
			continue;

		fecharTurma(turma);
		fechou = true;
	}
	return fechou;
}

// [INSERE / REMOVE ALUNOS]

// adicionar aluno à turma
void OfertaDisciplina::addAlunoTurma(AlunoHeur* const aluno, TurmaHeur* const turma, string metodo, bool fix)
{
	if(turmasGlobalIds_.find(turma->getGlobalId()) == turmasGlobalIds_.end())
	{
		HeuristicaNuno::warning("OfertaDisciplina::addAlunoTurma", "Turma nao e da oferta disciplina");
		return;
	}
	// adicionar à estrutura alunosTurma. se for novo apagar demanda nao atendida
	if(addToAlunosTurma_(aluno, turma, metodo))
	{
		// adicionar a oferta às demandas atendidas do aluno e apagar nao atendida
		aluno->addOferta(this);
		HeuristicaNuno::logMsg("apagar demanda nao atend", 3);
		solucao_->apagarDemandaNaoAtendEquiv_(campus_, getDisciplina(), aluno);
		HeuristicaNuno::logMsg("out. apagar demanda nao atend", 3);
	}

	// adicionar à turma
	addToTurmasAluno_(aluno, turma);
	turma->addAluno(aluno, fix);
	solucao_->stats_->nrCreditosAlunos_ += turma->getNrCreditos();

	// verificar a associação de turma
	if(ParametrosHeuristica::relacPraticas != ParametrosHeuristica::abrePraticas::N_N
		&& (nrTiposAula() == 2))
	{
		addAlunoCheckAssocTurmas_(aluno, turma);
	}

	// se ele estava nos incompletos e já está em 2 turmas remover de lá
	auto itAluno = alunosTurma_.find(aluno);
	if(itAluno == alunosTurma_.end())
		HeuristicaNuno::excepcao("OfertaDisciplina::addAlunoTurma","Aluno não adicionado a alunosTurma_");

	HeuristicaNuno::logMsg("check alunos incompleto", 3);
	int nrTurmas = itAluno->second.size();
	if(nrTurmas == nrTiposAula())
	{
		auto itIncomp = alunosIncompleto_.find(aluno);
		if(itIncomp != alunosIncompleto_.end())
		{
			HeuristicaNuno::logMsg("aluno completo.", 3);
			alunosIncompleto_.erase(itIncomp);
		}
	}
}
// adicionar à estrutura alunos turma
bool OfertaDisciplina::addToAlunosTurma_(AlunoHeur* const aluno, TurmaHeur* const turma, string metodo)
{
	bool novo = false;
	auto itAluno = alunosTurma_.find(aluno);
	if(itAluno == alunosTurma_.end())
	{
		novo = true;
		std::unordered_map<bool, TurmaHeur*> dict;
		itAluno = alunosTurma_.insert(make_pair(aluno, dict)).first;

		// adicionar o aluno aos alunos incompleto caso esta disciplina tenha 2 componentes
		if(nrTiposAula() == 2)
			alunosIncompleto_.insert(aluno);
	}
	else if(itAluno->second.find(turma->tipoAula) != itAluno->second.end())
	{
		stringstream ss;
		ss << "Aluno já está inserido numa turma desse tipo de aula. Metodo: " << metodo;
		HeuristicaNuno::excepcao("OfertaDisciplina::addToAlunosTurma_", ss.str());
	}

	// inserir na estrutura
	itAluno->second[turma->tipoAula] = turma;

	// return true se primeiro insert
	return novo;
}
// verificar se o aluno tem demanda equivalente
void OfertaDisciplina::addToTurmasAluno_(AlunoHeur* const aluno, TurmaHeur* const turma)
{
	auto itT = turmasAlunos_.find(turma);
	if(itT == turmasAlunos_.end())
		HeuristicaNuno::excepcao("OfertaDisciplina::addToTurmasAluno_", "Alunos da turma nao encontrados");

	if(!itT->second.insert(aluno).second)
		HeuristicaNuno::excepcao("OfertaDisciplina::addToTurmasAluno_", "Aluno nao adicionado");
}
	

// remove o aluno da oferta disciplina e todas as turmas e estruturas
void OfertaDisciplina::removeAluno(AlunoHeur* const aluno, bool fixado)
{
	if(!fixado && ehFixado(aluno))
	{
		HeuristicaNuno::warning("OfertaDisciplina::removeAluno", "Tentativa de remover um aluno fixado");
		return;
	}

	if(aluno == nullptr)
	{
		HeuristicaNuno::warning("OfertaDisciplina::removeAluno", "Aluno é nulo!");
		return;
	}

	// remove o aluno das turmas
	auto itAluno = alunosTurma_.find(aluno);
	if(itAluno == alunosTurma_.end())
	{
		HeuristicaNuno::warning("OfertaDisciplina::removeAluno", "Aluno nao esta em alunosTurma_!");
	}
	else
	{
		// turma teorica
		if(disciplinaTeorica_ != nullptr)
		{
			HeuristicaNuno::logMsg("check remove turma teorica", 3);
			TurmaHeur* turmaTeor = nullptr;
			if(getTurmaAlunoTipo(aluno, true, turmaTeor))
			{
				if(turmaTeor != nullptr)
					removeAlunoTurma(aluno, turmaTeor, fixado, true);
			}
		}

		// turma pratica
		if(disciplinaPratica_ != nullptr)
		{
			HeuristicaNuno::logMsg("check remove turma pratica", 3);
			TurmaHeur* turmaPrat = nullptr;
			if(getTurmaAlunoTipo(aluno, false, turmaPrat))
			{
				if(turmaPrat != nullptr)
					removeAlunoTurma(aluno, turmaPrat, fixado, true);
			}
		}
		
		// remover dos alunosTurma_
		HeuristicaNuno::logMsg("remove alunosTurma", 3);
		alunosTurma_.erase(itAluno);
	}

	// remover dos alunosIncompleto_
	HeuristicaNuno::logMsg("remove alunosIncomp", 3);
	auto itInc = alunosIncompleto_.find(aluno);
	if(itInc != alunosIncompleto_.end())
		alunosIncompleto_.erase(itInc);

	HeuristicaNuno::logMsg("aluno->removeOferta", 3);
	aluno->removeOferta(this);
	HeuristicaNuno::logMsg("removed", 3);

	// repor demanda não atendida
	HeuristicaNuno::logMsg("add demanda nao atend", 3);
	solucao_->addDemandaNaoAtendEquiv_(campus_, getDisciplina(), aluno);
	HeuristicaNuno::logMsg("out. add demanda nao atend", 3);
}
// remove aluno da turma
void OfertaDisciplina::removeAlunoTurma(AlunoHeur* const aluno, TurmaHeur* const turma, bool fixado, bool removeAlunoTot)
{
	if(!fixado && turma->ehAlunoFixado(aluno->getId()))
	{
		HeuristicaNuno::warning("OfertaDisciplina::removeAlunoTurma", "Tentativa de remover um aluno fixado");
		return;
	}
	// manter aluno
	if(keepAluno(aluno->getId()))
	{
		HeuristicaNuno::warning("OfertaDisciplina::removeAlunoTurma", "Tentativa de remover um aluno que deve ser mantido!");
		return;
	}

	if(turma == nullptr)
	{
		HeuristicaNuno::warning("OfertaDisciplina::removeAlunoTurma", "Turma nula");
		return;
	}
	if(aluno == nullptr)
	{
		HeuristicaNuno::warning("OfertaDisciplina::removeAlunoTurma", "Aluno nulo");
		return;
	}

	if(turmasGlobalIds_.find(turma->getGlobalId()) == turmasGlobalIds_.end())
	{
		HeuristicaNuno::warning("OfertaDisciplina::removeAlunoTurma", "Turma nao e da oferta disciplina");
		return;
	}

	// remove da turma
	HeuristicaNuno::logMsg("removeFromTurmasAluno_",3);
	removeFromTurmasAluno_(aluno, turma);
	HeuristicaNuno::logMsg("turma->removeAluno", 3);
	turma->removeAluno(aluno);
	HeuristicaNuno::logMsg("done", 3);
	solucao_->stats_->nrCreditosAlunos_ -= turma->getNrCreditos();

	// se a remoção nao for total, i.e. não tiver sido chamado por removeAluno()
	if(!removeAlunoTot)
	{
		HeuristicaNuno::logMsg("removeFromAlunosTurma_", 3);
		removeFromAlunosTurma_(aluno, turma);
		HeuristicaNuno::logMsg("done", 3);

		auto itAluno = alunosTurma_.find(aluno);
		if(itAluno != alunosTurma_.end())
		{
			int nrTurmas = itAluno->second.size();
			// Se aluno já não estiver em nenhuma turma remover de todas as estruturas
			HeuristicaNuno::logMsg("removeAluno", 3);
			if(nrTurmas == 0)
			{
				removeAluno(aluno, fixado);
				HeuristicaNuno::logMsg("done", 3);
			}
			// se o aluno estiver numa turma teórica mas não estiver numa prática ou vice-versa
			else if(nrTurmas == 1 && nrTiposAula() == 2)
			{
				HeuristicaNuno::logMsg("alunosIncompleto_.insert", 3);
				if(alunosIncompleto_.find(aluno) == alunosIncompleto_.end())
					alunosIncompleto_.insert(aluno);
				else
					HeuristicaNuno::warning("OfertaDisciplina::removeAlunoTurma", "Aluno ja tava nos incompletos");
				HeuristicaNuno::logMsg("done", 3);
			}
		}
	}
}
// remove da estrutura alunos turma
void OfertaDisciplina::removeFromAlunosTurma_(AlunoHeur* const aluno, TurmaHeur* const turma)
{
	if(turma == nullptr)
	{
		HeuristicaNuno::warning("OfertaDisciplina::removeFromAlunosTurma_", "Turma e nula!");
		return;
	}

	auto itAluno = alunosTurma_.find(aluno);
	if(itAluno == alunosTurma_.end())
	{
		HeuristicaNuno::excepcao("OfertaDisciplina::removeFromAlunosTurma", "Aluno não se encontrava inserido nesta oferta disciplina");
		return;
	}
	auto itTipo = itAluno->second.find(turma->tipoAula);
	if(itTipo == itAluno->second.end())
	{
		HeuristicaNuno::excepcao("OfertaDisciplina::removeFromAlunosTurma", "Aluno não se encontrava inserido em nenhuma turma desse tipo.");
		return;
	}

	if(itTipo->second->id != turma->id)
	{
		HeuristicaNuno::excepcao("OfertaDisciplina::removeFromAlunosTurma", "Aluno não se encontrava inserido nessa turma.");
		return;
	}
	itAluno->second.erase(itTipo);
}
// remover from turmas aluno
void OfertaDisciplina::removeFromTurmasAluno_(AlunoHeur* const aluno, TurmaHeur* const turma)
{
	auto itT = turmasAlunos_.find(turma);
	if(itT == turmasAlunos_.end())
		HeuristicaNuno::excepcao("OfertaDisciplina::removeFromTurmasAluno_", "Alunos da turma nao encontrados");

	auto itAluno = itT->second.find(aluno);
	if(itAluno == itT->second.end())
		HeuristicaNuno::excepcao("OfertaDisciplina::removeFromTurmasAluno_", "Aluno nao adicionado");

	itT->second.erase(itAluno);
}

// [END]

// esse aluno já tá numa turma desse tipo ?
bool OfertaDisciplina::temAlunoComp(AlunoHeur* const aluno, bool teorico) const
{ 
	auto it = alunosTurma_.find(aluno);
	if(it == alunosTurma_.end())
		return false;

	return (it->second.find(teorico) != it->second.end());
}

// tem aluno de um curso na turma
bool OfertaDisciplina::temAlunoCurso(Curso* const curso, TurmaHeur* const turma) const
{
	if(curso == nullptr || turma == nullptr)
		return false;

	// check
	if(turmasGlobalIds_.find(turma->getGlobalId()) == turmasGlobalIds_.end())
		HeuristicaNuno::excepcao("OfertaDisciplina::temAlunoCurso", "Turma nao pertence a oferta disciplina");

	auto itAlunos = turmasAlunos_.find(turma);
	for(auto it = itAlunos->second.cbegin(); it != itAlunos->second.cend(); ++it)
	{
		if((*it)->getCurso() == curso)
			return true;
	}
	
	return false;
}

void OfertaDisciplina::preencheSalasAssoc()
{
	preencheSalasAssoc(true);
	preencheSalasAssoc(false);
}
void OfertaDisciplina::preencheSalasAssoc(bool teorico)
{
	Disciplina* const disciplina = teorico ? disciplinaTeorica_ : disciplinaPratica_;
	if(disciplina == nullptr) return;
		
	// Limpa
	if ( salasAssoc_.find(teorico) != salasAssoc_.end() )
		salasAssoc_.erase(teorico);

	unordered_set<SalaHeur*> empty;
	auto itSalaAssoc = salasAssoc_.insert( pair<bool, unordered_set<SalaHeur*>> (teorico, empty) ); 
	if ( !itSalaAssoc.second )
	{
		HeuristicaNuno::warning("OfertaDisciplina::preencheSalasAssoc(bool)", "Par <bool,empty> nao inserido!");
		return;
	}	
	auto setSalasAssoc = &itSalaAssoc.first->second;
	
	// Preenche
	int nrLabs = 0;
	for(auto it = disciplina->cjtSalasAssociados.begin(); it!= disciplina->cjtSalasAssociados.end(); ++it)
	{
		for(auto itSalas = it->second->salas.begin(); itSalas != it->second->salas.end(); ++itSalas)
		{
			if(itSalas->second->getIdCampus() != campus_->getId())
				continue;

			auto salaHeur = solucao_->salasHeur.find(itSalas->second->getId());
			if(salaHeur == solucao_->salasHeur.end())
			{
				HeuristicaNuno::warning("OfertaDisciplina::getSalasAssociadas", "SalaHeur nao encontrada!");
				continue;
			}
			setSalasAssoc->insert(salaHeur->second);
			if(salaHeur->second->ehLab())
				nrLabs++;
		}
	}
	
	// Se não houve associação, associar a todas salas do campus desse tipo
	if(setSalasAssoc->size() == 0)
	{
		stringstream ss;
		ss << "Disciplina id: " << disciplina->getId() << " sem salas associadas. ";
		if(disciplina->eLab())
			ss << "Associar todos os labs, pois ela usa lab!" << endl;
		else
			ss << "Associar todas as salas (nao lab)!" << endl;
		for(auto itUnid = campus_->unidades.begin(); itUnid != campus_->unidades.end(); ++itUnid)
		{
			for(auto itSala = (*itUnid)->salas.begin(); itSala != (*itUnid)->salas.end(); ++itSala)
			{
				if(disciplina->eLab() != (*itSala)->ehLab())
					continue;

				auto salaHeur = solucao_->salasHeur.find(itSala->getId());
				if(salaHeur == solucao_->salasHeur.end())
				{
					HeuristicaNuno::warning("OfertaDisciplina::getSalasAssociadas", "SalaHeur nao encontrada!");
					continue;
				}
				setSalasAssoc->insert(salaHeur->second);
			}
		}
	}
}

void OfertaDisciplina::getSalasAssociadas(unordered_set<SalaHeur*> &set, bool teorico) const
{
	Disciplina* const disciplina = teorico ? disciplinaTeorica_ : disciplinaPratica_;
	if(disciplina == nullptr) return;
		
	int nrLabs = 0;
	for(auto it = disciplina->cjtSalasAssociados.begin(); it!= disciplina->cjtSalasAssociados.end(); ++it)
	{
		for(auto itSalas = it->second->salas.begin(); itSalas != it->second->salas.end(); ++itSalas)
		{
			if(itSalas->second->getIdCampus() != campus_->getId())
				continue;

			auto salaHeur = solucao_->salasHeur.find(itSalas->second->getId());
			if(salaHeur == solucao_->salasHeur.end())
			{
				HeuristicaNuno::warning("OfertaDisciplina::getSalasAssociadas", "SalaHeur nao encontrada!");
				continue;
			}
			set.insert(salaHeur->second);
			if(salaHeur->second->ehLab())
				nrLabs++;
		}
	}
	/*if(teorico && nrLabs > 0)
	{
		HeuristicaNuno::logMsgInt("disciplina id: ", disciplina->getId(), 1);
		HeuristicaNuno::warning("OfertaDisciplina::getSalasAssociadas", "Componente teorica com laboratorios associados");
	}*/

	// associar a todas salas do campus desse tipo
	if(set.size() == 0)
	{
		stringstream ss;
		ss << "Disciplina id: " << disciplina->getId() << " sem salas associadas. ";
		if(disciplina->eLab())
			ss << "Associar todos os labs, pois ela usa lab!" << endl;
		else
			ss << "Associar todas as salas (nao lab)!" << endl;
		for(auto itUnid = campus_->unidades.begin(); itUnid != campus_->unidades.end(); ++itUnid)
		{
			for(auto itSala = (*itUnid)->salas.begin(); itSala != (*itUnid)->salas.end(); ++itSala)
			{
				if(disciplina->eLab() != (*itSala)->ehLab())
					continue;

				auto salaHeur = solucao_->salasHeur.find(itSala->getId());
				if(salaHeur == solucao_->salasHeur.end())
				{
					HeuristicaNuno::warning("OfertaDisciplina::getSalasAssociadas", "SalaHeur nao encontrada!");
					continue;
				}
				set.insert(salaHeur->second);
			}
		}
	}
}
void OfertaDisciplina::getSalasAssociadas (set<SalaHeur*> &conj, bool teorico) const
{
	Disciplina* const disciplina = teorico ? disciplinaTeorica_ : disciplinaPratica_;
	if(disciplina == nullptr) return;
		
	int nrLabs = 0;
	for(auto it = disciplina->cjtSalasAssociados.begin(); it!= disciplina->cjtSalasAssociados.end(); ++it)
	{
		for(auto itSalas = it->second->salas.begin(); itSalas != it->second->salas.end(); ++itSalas)
		{
			if(itSalas->second->getIdCampus() != campus_->getId())
				continue;

			auto salaHeur = solucao_->salasHeur.find(itSalas->second->getId());
			if(salaHeur == solucao_->salasHeur.end())
			{
				HeuristicaNuno::warning("OfertaDisciplina::getSalasAssociadas", "SalaHeur nao encontrada!");
				continue;
			}
			conj.insert(salaHeur->second);
			if(salaHeur->second->ehLab())
				nrLabs++;
		}
	}
	/*if(teorico && nrLabs > 0)
	{
		HeuristicaNuno::logMsgInt("disciplina id: ", disciplina->getId(), 1);
		HeuristicaNuno::warning("OfertaDisciplina::getSalasAssociadas", "Componente teorica com laboratorios associados");
	}*/

	// associar a todas salas do campus desse tipo
	if(conj.size() == 0)
	{
		stringstream ss;
		ss << "Disciplina id: " << disciplina->getId() << " sem salas associadas. ";
		if(disciplina->eLab())
			ss << "Associar todos os labs, pois ela usa lab!" << endl;
		else
			ss << "Associar todas as salas (nao lab)!" << endl;
		for(auto itUnid = campus_->unidades.begin(); itUnid != campus_->unidades.end(); ++itUnid)
		{
			for(auto itSala = (*itUnid)->salas.begin(); itSala != (*itUnid)->salas.end(); ++itSala)
			{
				if(disciplina->eLab() != (*itSala)->ehLab())
					continue;

				auto salaHeur = solucao_->salasHeur.find(itSala->getId());
				if(salaHeur == solucao_->salasHeur.end())
				{
					HeuristicaNuno::warning("OfertaDisciplina::getSalasAssociadas", "SalaHeur nao encontrada!");
					continue;
				}
				conj.insert(salaHeur->second);
			}
		}
	}
}
void OfertaDisciplina::getProfessoresAssociados (unordered_set<ProfessorHeur*> &profsAssoc) const
{
	// reais
	auto it = HeuristicaNuno::probData->mapProfessorDisciplinasAssociadas.begin();
	for(; it!= HeuristicaNuno::probData->mapProfessorDisciplinasAssociadas.end(); it++)
	{
		int id = it->first->getId();
		// verificar se professor pode leccionar esta disciplina, neste campus
		if(it->second.find(getDisciplina()) != it->second.end()
			&& campus_->professores.find(it->first) != campus_->professores.end())
		{
			auto itHeur = solucao_->professoresHeur.find(id);
			if(itHeur == solucao_->professoresHeur.end())
				continue;

			profsAssoc.insert(itHeur->second);
		}
	}
}
bool OfertaDisciplina::haProfessorAssociado () const
{
	// reais
	auto it = HeuristicaNuno::probData->mapProfessorDisciplinasAssociadas.begin();
	for(; it!= HeuristicaNuno::probData->mapProfessorDisciplinasAssociadas.end(); it++)
	{
		int id = it->first->getId();
		// verificar se professor pode leccionar esta disciplina, neste campus
		if(it->second.find(getDisciplina()) != it->second.end()
			&& campus_->professores.find(it->first) != campus_->professores.end())
		{
			auto itHeur = solucao_->professoresHeur.find(id);
			if(itHeur == solucao_->professoresHeur.end())
				continue;

			return true;
		}
	}
	return false;
}


int OfertaDisciplina::getNroCredsLivresProfsEstimados() const
{ 
	int nr=0;
	
	int minCred = 0;
	if (duasComp()) minCred = min<int>(getNrCreds(true),getNrCreds(false));
	else minCred = getNrCreds();

	auto itProf = mapProfDiaHorComum_.cbegin();
	for(; itProf!= mapProfDiaHorComum_.cend(); itProf++)
	{
		int nCredProf = itProf->first->nroCredsLivresEstimados();
		if (nCredProf >= minCred)
			nr += nCredProf;
	}
	return nr;
}

void OfertaDisciplina::preencheMapDisponibComumProf()
{
	bool mesmoProfPratTeor = this->getDisciplina()->getProfUnicoDiscPT();
	const int nroCredsT = this->getNrCreds(true);
	const int nroCredsP = this->getNrCreds(false);

	unordered_set<ProfessorHeur*> profsAssoc;
	getProfessoresAssociados(profsAssoc);

	auto itProf = profsAssoc.cbegin();
	for(; itProf!= profsAssoc.cend(); itProf++)
	{
		ProfessorHeur* const prof = *itProf;

		std::map< int/*dia*/, std::map<DateTime, std::set<DateTime>> > mapDiaHorComum;
		const int nrHorComuns = getTempoComumProf(prof, mapDiaHorComum);

		bool enoughP = (nrHorComuns >= nroCredsP);
		bool enoughT = (nrHorComuns >= nroCredsT);
		bool enough = (enoughP && enoughT) || (!mesmoProfPratTeor && (enoughP || enoughT));
		if (enough)
		{
			mapProfDiaHorComum_[prof] = mapDiaHorComum;
		}	
	}
}

int OfertaDisciplina::getTempoComumProf( ProfessorHeur* const prof,
	std::map< int/*dia*/, std::map<DateTime, std::set<DateTime>> > &mapDiaHorComum ) const
{
	Disciplina * const discTeor = this->getDisciplina();
	GGroup<Horario*,LessPtr<Horario>> * const horsDisc = &discTeor->horarios;
		
	// Armazena os horarios comuns da disponib do prof e da disc
	for( auto itHorDisc = horsDisc->begin();
		 itHorDisc!= horsDisc->end(); itHorDisc++)
	{
		HorarioAula * const hi = (*itHorDisc)->horario_aula;
		HorarioAula * const hf = hi;
		DateTime const dti = hi->getInicio();
		DateTime const dtf = hf->getFinal();

		for ( auto itDia = itHorDisc->dias_semana.begin();
			  itDia != itHorDisc->dias_semana.end(); itDia++ )
		{
			const int dia = *itDia;
			if ( prof->getProfessor()->possuiHorariosNoDia(hi,hf,dia) )
			{
				std::map<DateTime, std::set<DateTime>> empty1;
				auto itDayInser = mapDiaHorComum.insert(
					pair<int/*dia*/,std::map<DateTime, std::set<DateTime>>> (dia,empty1) ).first;
				
				std::set<DateTime> empty2;
				auto itDtiInser = itDayInser->second.insert(
					pair<DateTime, std::set<DateTime>> (dti,empty2) ).first;

				itDtiInser->second.insert(dtf);
			}
		}
	}
	
	// Soma nro de creditos comuns e sem sobreposicao (supondo que a disc tem sempre cred de msm duração)
	// na disponib do prof e da disc
	int credsLivres=0;
	for( auto it = mapDiaHorComum.cbegin(); it!= mapDiaHorComum.cend(); it++)
	{
		credsLivres += (int) it->second.size();
	}

	return credsLivres;
}

int OfertaDisciplina::getDisponibProfDisc() const
{
	int nr = 0;

	Disciplina * const discTeor = this->getDisciplina();
	const int nroCredsT = this->getNrCreds(true);
	const int nroCredsP = this->getNrCreds(false);
	bool mesmoProfPratTeor = discTeor->getProfUnicoDiscPT();
	
	auto itProf = mapProfDiaHorComum_.cbegin();
	for(; itProf!= mapProfDiaHorComum_.cend(); itProf++)
	{
		ProfessorHeur* const prof = itProf->first;

		std::map< int/*dia*/, map<DateTime, std::set<DateTime>> >
			const * const mapDiaHorComum = & itProf->second;

		const int nrHorComuns = getNroCredsLivresProf(prof, mapDiaHorComum);
		bool enoughP = (nrHorComuns >= nroCredsP);
		bool enoughT = (nrHorComuns >= nroCredsT);
		bool enough = (enoughP && enoughT) || (!mesmoProfPratTeor && (enoughP || enoughT));
		if (enough)
			nr += nrHorComuns;
	}

	return nr;
}

int OfertaDisciplina::getNroCredsLivresProf( ProfessorHeur* const prof, 
		std::map< int/*dia*/, std::map<DateTime, std::set<DateTime>> > const * const & mapDiaHorComum ) const
{
	int credsLivres=0;
	
	for( auto itDia = mapDiaHorComum->cbegin();
		 itDia!= mapDiaHorComum->cend(); itDia++)
	{
		const int dia = itDia->first;
		for( auto itDti = itDia->second.cbegin();
			itDti!= itDia->second.cend(); itDti++)
		{
			const DateTime dti = itDti->first;
			for( auto itDtf = itDti->second.cbegin();
				itDtf!= itDti->second.cend(); itDtf++)
			{			
				const DateTime dtf = *itDtf;
				if ( prof->estaDisponivel(dia,dti,dtf) )
					credsLivres++;
			}
		}
	}
	
	return credsLivres;
}

int OfertaDisciplina::getDisponibProfSalaDisc() const
{
	int nr = 0;

	Disciplina * const discTeor = this->getDisciplina();
	const int nroCredsT = this->getNrCreds(true);
	const int nroCredsP = this->getNrCreds(false);
	bool mesmoProfPratTeor = discTeor->getProfUnicoDiscPT();
	
	unordered_set<SalaHeur*> const * const salas = getSalasAssocRef(true);
	unordered_set<SalaHeur*> const * const labs = getSalasAssocRef(false);

	int nrHorComuns=9999999999;
	int nrHorComunsTeor;
	int nrHorComunsPrat;
	if (salas)
	{
		nrHorComunsTeor = getDisponibProfSalaDisc(*salas,true);
		nrHorComuns = min(nrHorComunsTeor,nrHorComuns);
	}
	if (labs)
	{
		nrHorComunsPrat = getDisponibProfSalaDisc(*labs,true);
		nrHorComuns = min(nrHorComunsPrat,nrHorComuns);
	}
	if ( !salas && !labs ) nrHorComuns = 0;

	return nrHorComuns;
}

// Retorna o nro de creditos livres em comum da disciplina e o cjt de salas,
// considerando os horarios das turmas ja alocadas, e para os quais há professor real disponivel
int OfertaDisciplina::getDisponibProfSalaDisc( unordered_set<SalaHeur*> const &salas, bool teorico ) const
{
	int nr = 0;
	const int nroCreds = this->getNrCreds(teorico);
	
	auto itProf = mapProfDiaHorComum_.cbegin();
	for(; itProf!= mapProfDiaHorComum_.cend(); itProf++)
	{
		ProfessorHeur* const prof = itProf->first;
		
		std::map< int/*dia*/, map<DateTime, std::set<DateTime>> >
			const * const mapDiaHorComum = & itProf->second;
	
		auto itSala = salas.cbegin();
		for(; itSala!= salas.cend(); itSala++)
		{
			const int nrHorComuns = getNroCredsLivresProfSala(prof, *itSala, mapDiaHorComum);
			bool enough = (nrHorComuns >= nroCreds);
			if (enough)
				nr += nrHorComuns;
		}
	}

	return nr;
}

// Retorna o nro de creditos livres em comum da disciplina, com o professor e a sala,
// dentro do map de horarios especificado, considerando os horarios das turmas ja alocadas
int OfertaDisciplina::getNroCredsLivresProfSala( ProfessorHeur* const prof, SalaHeur* const sala, 
		std::map< int/*dia*/, std::map<DateTime, std::set<DateTime>> > const * const & mapDiaHorComum ) const
{
	int credsLivres=0;
	
	for( auto itDia = mapDiaHorComum->cbegin();
		 itDia!= mapDiaHorComum->cend(); itDia++)
	{
		const int dia = itDia->first;
		for( auto itDti = itDia->second.cbegin();
			itDti!= itDia->second.cend(); itDti++)
		{
			const DateTime dti = itDti->first;
			for( auto itDtf = itDti->second.cbegin();
				itDtf!= itDti->second.cend(); itDtf++)
			{			
				const DateTime dtf = *itDtf;
				if ( prof->estaDisponivel(dia,dti,dtf) )
				if ( sala->estaDisponivel(dia,dti,dtf) )
					credsLivres++;
			}
		}
	}
	
	return credsLivres;
}

// Retorna pointer para o cjt de salas associadas da OfertaDisciplina.
unordered_set<SalaHeur*> const * OfertaDisciplina::getSalasAssocRef(bool teorico) const
{
	unordered_set<SalaHeur*> const * salas = nullptr;
	auto finder = salasAssoc_.find(teorico);
	if (finder != salasAssoc_.end())
		salas = &(finder->second);
	return salas;
}

// set nrLabsAssoc_
void OfertaDisciplina::setLabsAssoc(void)
{
	nrLabsAssoc_ = 0;
	int nrLabs = 0;

	// pratica 
	if(disciplinaPratica_ != nullptr)
	{
		int nrSalas = 0;
		for(auto it = disciplinaPratica_->cjtSalasAssociados.begin(); it!= disciplinaPratica_->cjtSalasAssociados.end(); ++it)
		{
			for(auto itSalas = it->second->salas.begin(); itSalas != it->second->salas.end(); ++itSalas)
			{
				if(itSalas->second->getIdCampus() != campus_->getId())
					continue;

				auto salaHeur = solucao_->salasHeur.find(itSalas->second->getId());
				if(salaHeur == solucao_->salasHeur.end())
				{
					HeuristicaNuno::warning("OfertaDisciplina::getSalasAssociadas", "SalaHeur nao encontrada!");
					continue;
				}
				nrSalas++;
				if(salaHeur->second->ehLab())
					nrLabs++;
			}
		}
		if(nrSalas == 0)
		{
			HeuristicaNuno::logMsgInt("disciplina [P] id: ", std::abs(disciplinaPratica_->getId()), 1);
			HeuristicaNuno::warning("OfertaDisciplina::setLabsAssoc", "Disciplina nao tem ambientes associados a componente pratica");
		}
	}
	nrLabsAssoc_ = nrLabs;
}

bool OfertaDisciplina::algumaSalaCluster(const ConjUnidades* const cluster, bool teorico) const
{
	unordered_set<SalaHeur*> salas;
	getSalasAssociadas(salas, teorico);
	if(salas.size() == 0)
		return false;

	for(auto it = salas.begin(); it != salas.end(); ++it)
	{
		if(cluster->temUnidade((*it)->unidadeId()))
			return true;
	}
	return false;
}

int OfertaDisciplina::getNrCreds(void) const
{
	int nrCreds = 0;

	if(disciplinaTeorica_ != nullptr)
		nrCreds += disciplinaTeorica_->getTotalCreditos();
	if(disciplinaPratica_ != nullptr)
		nrCreds += disciplinaPratica_->getTotalCreditos();

	return nrCreds;
}
int OfertaDisciplina::getNrCreds(bool teorico) const
{
	if(teorico)
		return (disciplinaTeorica_ == nullptr) ? 0 : disciplinaTeorica_->getTotalCreditos();
	else
		return (disciplinaPratica_ == nullptr) ? 0 : disciplinaPratica_->getTotalCreditos();
}

void OfertaDisciplina::getAlunosIncompleto(unordered_set<AlunoHeur*>& alunosInc) const 
{
	//HeuristicaNuno::logMsg("get alunos incompletos", 3);
	for(auto it = alunosIncompleto_.begin(); it != alunosIncompleto_.end(); ++it)
	{
		alunosInc.insert(*it);
	}
}
// remove todos os alunos que estão alocados numa componente mas não noutra, e.g. tão numa turma teórica mas não numa prática
void OfertaDisciplina::removerAlunosIncompleto(unordered_set<AlunoHeur*> &alunosRemovidos, bool fixados)
{
	// remove alunos da estrutura alunos incompletos. (Nota: copiar pois estrutura é alterada)
	alunosRemovidos = alunosIncompleto_;

	// remover alunos
	for (auto itInc = alunosRemovidos.begin(); itInc != alunosRemovidos.end(); ++itInc)
	{
		if(!fixados && ehFixado(*itInc))
			continue;

		removeAluno(*itInc, fixados);
	}
}
void OfertaDisciplina::removerAlunosIncompleto(bool fixados)
{
	auto alunosRemovidos = alunosIncompleto_;
	for (auto itInc = alunosRemovidos.begin(); itInc != alunosRemovidos.end(); ++itInc)
	{
		if(!fixados && ehFixado(*itInc))
			continue;

		removeAluno(*itInc, fixados);
	}
}
void OfertaDisciplina::removerTodosAlunos(unordered_set<AlunoHeur*> &alunosRemovidos, bool fixados)
{
	alunosRemovidos.clear();
	// recolher alunos anteriormente alocados (nao incluir fixados excepto se especificado)
	for(auto it = alunosTurma_.begin(); it != alunosTurma_.end(); ++it)
	{
		// verificar se é incompleto. se for é sempre removido
		bool incompleto = this->ehIncompleto(it->first);

		// se for de prioridade maior nao remover
		if(!incompleto && (keepAlunos_.find(it->first->getId()) != keepAlunos_.end()))
			continue;
		// check se e fixado
		if(!incompleto && !fixados && ehFixado(it->first))
			continue;

		alunosRemovidos.insert(it->first);
	}

	// remover todos da oferta
	for (auto itInc = alunosRemovidos.begin(); itInc != alunosRemovidos.end(); ++itInc)
		removeAluno(*itInc, fixados);
}
void OfertaDisciplina::removerTodosAlunos(bool teorico, unordered_set<AlunoHeur*> &alunosRemovidos, bool fixados)
{
	HeuristicaNuno::logMsg("remove todos os alunos", 2);
	auto itTurmasSec = turmas_.find(teorico);
	if(itTurmasSec == turmas_.end())
		return;

	int nr = 0;
	for(auto itTurma = itTurmasSec->second.begin(); itTurma != itTurmasSec->second.end(); ++itTurma)
	{
		TurmaHeur* const turma = itTurma->second;
		if(turma->tipoAula != teorico)
			HeuristicaNuno::excepcao("OfertaDisciplina::removerTodosAlunos", "Turma com tipo diferente do esperado");
		auto alunos = turmasAlunos_.at(turma);
		for(auto itAluno = alunos.begin(); itAluno != alunos.end(); ++itAluno)
		{
			AlunoHeur* const aluno = *itAluno;
			// verificar se é incompleto. se for é sempre removido
			bool incompleto = this->ehIncompleto(aluno);

			// se for de prioridade maior nao remover
			if(!incompleto && (keepAlunos_.find(aluno->getId()) != keepAlunos_.end()))
				continue;
			// check se é fixado
			if(!fixados && turma->ehAlunoFixado((*itAluno)->getId()))
				continue;

			removeAlunoTurma(aluno, turma, fixados, false);
			alunosRemovidos.insert(aluno);
			nr++;
		}
	}
}

// verifica se está fixado a alguma das turmas a que está alocado
bool OfertaDisciplina::ehFixado(AlunoHeur* const aluno) const
{
	auto it = alunosTurma_.find(aluno);
	if(it == alunosTurma_.end())
		return false;

	for(auto itT = it->second.begin(); itT != it->second.end(); ++itT)
	{
		if(itT->second->ehAlunoFixado(aluno->getId()))
			return true;
	}

	return false;
}

// manter alunos completos atuais. não remover para o MIP.
void OfertaDisciplina::setKeepAlunos(void)
{
	for(auto it = alunosTurma_.cbegin(); it != alunosTurma_.cend(); ++it)
	{
		if(!ehIncompleto(it->first))
			keepAlunos_.insert(it->first->getId());
	}
}

// preenche mapa com a alocacao atual
void OfertaDisciplina::getAlocacaoAtual(unordered_map<TurmaHeur*, pair<unordered_set<AlunoHeur*>, SalaHeur*>>  &alocOft) const
{
	unordered_set<AlunoHeur*> mapaVazio;
	for(auto itTurma = turmasAlunos_.begin(); itTurma != turmasAlunos_.end(); ++itTurma)
	{
		pair<unordered_set<AlunoHeur*>, SalaHeur*> parTurma (mapaVazio, itTurma->first->getSala());
		auto parIns = alocOft.insert(make_pair(itTurma->first, parTurma));
		if(!parIns.second)
		{
			HeuristicaNuno::warning("OfertaDisciplina::getAlocacaoAtual", "Par turma, alunos-sala nao inserido!");
			continue;
		}
		auto itAlocT = parIns.first;
		for(auto itAluno = itTurma->second.begin(); itAluno != itTurma->second.end(); ++itAluno)
			itAlocT->second.first.insert(*itAluno);
	}
}
void OfertaDisciplina::getAlocacaoAtual(unordered_map<TurmaHeur*, pair<unordered_set<AlunoHeur*>, SalaHeur*>>* const alocOft) const
{
	unordered_set<AlunoHeur*> mapaVazio;
	for(auto itTurma = turmasAlunos_.begin(); itTurma != turmasAlunos_.end(); ++itTurma)
	{
		pair<unordered_set<AlunoHeur*>, SalaHeur*> parTurma (mapaVazio, itTurma->first->getSala());
		auto parIns = alocOft->insert(make_pair(itTurma->first, parTurma));
		if(!parIns.second)
		{
			HeuristicaNuno::warning("OfertaDisciplina::getAlocacaoAtual", "Par turma, alunos-sala nao inserido!");
			continue;
		}
		auto itAlocT = parIns.first;
		for(auto itAluno = itTurma->second.begin(); itAluno != itTurma->second.end(); ++itAluno)
			itAlocT->second.first.insert(*itAluno);
	}
}

// Get turmas
void OfertaDisciplina::getTurmas(unordered_set<TurmaHeur*>& turmas)
{
	for(auto itTeor = turmas_.cbegin(); itTeor != turmas_.cend(); ++itTeor)
	{
		for(auto itId = itTeor->second.cbegin(); itId != itTeor->second.cend(); ++itId)
		{
			turmas.insert(itId->second);
		}
	}
}
unordered_map<int, TurmaHeur*> OfertaDisciplina::getTurmasTipo(bool teorico) const
{ 
	unordered_map<int, TurmaHeur*> mapa;
	auto it = turmas_.find(teorico);
	if(it == turmas_.end())
	{
		HeuristicaNuno::warning("OfertaDisciplina::getTurmasTipo", "Turmas do tipo nao encontradas!");
		return mapa;
	}

	return turmas_.at(teorico);
}
void OfertaDisciplina::getTurmasTipo(bool teorico, unordered_set<TurmaHeur*> &turmas) const
{
	auto it = turmas_.find(teorico);
	if(it == turmas_.end())
		return;

	for(auto itTurma = it->second.begin(); itTurma != it->second.end(); ++itTurma)
		turmas.insert(itTurma->second);
}

// procura turma por id e tipo
TurmaHeur* OfertaDisciplina::getTurma(bool teorico, int id) const
{
	auto itTipo = turmas_.find(teorico);
	if(itTipo == turmas_.end())
		return nullptr;

	auto itId = itTipo->second.find(id);
	if(itId == itTipo->second.end())
		return nullptr;

	return itId->second;
}

// verifica se tem turmas ilegais
bool OfertaDisciplina::temTurmasIlegais(void) const
{
	for(auto itTeor = turmas_.cbegin(); itTeor != turmas_.cend(); itTeor++)
	{
		for(auto itId = itTeor->second.cbegin(); itId != itTeor->second.cend(); itId++)
		{
			if(itId->second->ehIlegal())
				return true;
		}
	}
	return false;
}

// retorna turmas a qual esta turma está associada
void OfertaDisciplina::getTurmasAssoc(TurmaHeur* const turma, unordered_set<TurmaHeur*> &turmasAssoc) const
{
	if(nrTiposAula() != 2)
		HeuristicaNuno::excepcao("OfertaDisciplina::getTurmasAssoc", "Esta oferta nao tem duas componentes!");

	// se NxN retornar todas as turmas do tipo inverso
	if(ParametrosHeuristica::relacPraticas == ParametrosHeuristica::abrePraticas::N_N)
	{
		getTurmasTipo(!turma->tipoAula, turmasAssoc);
	}
	// retornar turmas associadas
	else
	{
		auto it = turmasAssoc_.find(turma);
		if(it != turmasAssoc_.end())
			turmasAssoc = it->second;
	}
}
bool OfertaDisciplina::temAssoc(TurmaHeur* const turmaUm, TurmaHeur* const turmaDois) const
{
	auto it = turmasAssoc_.find(turmaUm);
	if(it == turmasAssoc_.end())
		return false;

	return (it->second.find(turmaDois) != it->second.end());
}
bool OfertaDisciplina::temAnyAssoc(TurmaHeur* const turma) const
{
	auto it = turmasAssoc_.find(turma);
	if(it == turmasAssoc_.end())
		return false;

	return (it->second.size() > 0);
}
// limpar associações de turmas (exceto as que devem ser abertas pelo MIP)
void OfertaDisciplina::limparTurmasAssocPreMIP(void)
{
	for(auto it = turmasAssoc_.begin(); it != turmasAssoc_.end();)
	{
		// pode ser fechada, remover associações
		if(!it->first->mustAbrirMIP())
		{
			turmasAssoc_.erase(it++);
			continue;
		}

		// verificar as outras turmas
		for(auto itOth = it->second.begin(); itOth != it->second.end(); )
		{
			// pode ser fechada, remover associação
			if(!(*itOth)->mustAbrirMIP())
			{
				it->second.erase(itOth++);
				continue;
			}
			else
				++itOth;
		}

		++it;
	}
}

int OfertaDisciplina::nrTurmas(bool teorico) const
{
	auto it = turmas_.find(teorico);
	if(it == turmas_.end())
		return 0;

	return it->second.size();
}
bool OfertaDisciplina::temTurmaTipo(bool teorico) const
{
	auto it = turmas_.find(teorico);
	if(it == turmas_.end())
		return false;

	return it->second.size() > 0;
}

int OfertaDisciplina::nrTurmasVirt(bool soProfVirtual) const
{
	int nr = 0;
	for(auto it = turmasAlunos_.cbegin(); it != turmasAlunos_.cend(); ++it)
	{
		if(it->first->getProfessor()->ehVirtual() || !soProfVirtual)
			nr++;
	}
	return nr;
}

// retorna a aula do tipo a que o aluno está alocado. Se não encontrar retorna false;
bool OfertaDisciplina::getTurmaAlunoTipo(AlunoHeur* const aluno, bool teorico, TurmaHeur* &turma) const
{
	turma = nullptr;

	auto itAluno = alunosTurma_.find(aluno);
	if(itAluno == alunosTurma_.end())
		return false;

	auto itTipo = itAluno->second.find(teorico);
	if(itTipo == itAluno->second.end())
		return false;

	turma = itTipo->second;
	return true;
}

// retorna as turmas a que o aluno está alocado
void OfertaDisciplina::getTurmasAluno(AlunoHeur* const aluno, vector<TurmaHeur*> &turmas) const
{
	auto itAluno = alunosTurma_.find(aluno);
	if(itAluno == alunosTurma_.end())
		return;

	for(auto it = itAluno->second.begin(); it != itAluno->second.end(); ++it)
		turmas.push_back(it->second);
}

// get todas as associações turma aluno
void OfertaDisciplina::getTurmasSalasAlunos(unordered_map<TurmaHeur*, pair<SalaHeur*, unordered_set<AlunoHeur*> > > &turmasSalasAlunos) const
{
	turmasSalasAlunos.clear();
	for(auto it = turmasAlunos_.begin(); it != turmasAlunos_.end(); ++it)
	{
		TurmaHeur* const turma = it->first;
		// para não ser deletado o objeto
		turma->setKeep(true);
		if(!turmasSalasAlunos.insert(make_pair(turma, make_pair(turma->getSala(), it->second))).second)
			HeuristicaNuno::excepcao("OfertaDisciplina::getTurmasSalasAlunos", "Alunos e sala nao guardados!");
	}
}

int OfertaDisciplina::nrTiposAula() const 
{
	int n = 0;
	if(disciplinaTeorica_ != nullptr && disciplinaTeorica_ != NULL)
		n++;
	if(disciplinaPratica_ != nullptr && disciplinaPratica_ != NULL)
		n++;

	if(n == 0)
		HeuristicaNuno::excepcao("OfertaDisciplina::nrTiposAula", "Retornando zero!");

	return n;
}
int OfertaDisciplina::nrDivisoes(bool teorico) const
{
	Disciplina* disc = getDisciplina(teorico);
	if(disc == nullptr)
		return 0;

	return disc->combinacao_divisao_creditos.size();
}

int OfertaDisciplina::nrAlunosCompleto(void) const
{
	int nrCompleto = alunosTurma_.size() - alunosIncompleto_.size();
	if(nrCompleto < 0)
		HeuristicaNuno::warning("OfertaDisciplina::nrAlunosCompleto", "Nr alunos completo negativo!");
	
	return nrCompleto;
}

// print numero de combinacoes previstas
void OfertaDisciplina::printNrCombPrev(void)
{
	if(disciplinaTeorica_ != nullptr)
	{
		HeuristicaNuno::logMsgLong("# comb estimado teorico: ", nrCombinacoesPrev(true, false), 1);
		//HeuristicaNuno::logMsgLong("# comb estimado teorico (p/turno): ", nrCombinacoesPrev(true, true), 1);
	}
	if(disciplinaPratica_ != nullptr)
	{
		HeuristicaNuno::logMsgLong("# comb estimado pratico: ", nrCombinacoesPrev(false, false), 1);
		//HeuristicaNuno::logMsgLong("# comb estimado pratico (p/turno): ", nrCombinacoesPrev(false, true), 1);
	}
}

// numero de combinacoes previstas para analise na abertura de turmas de um tipo
long OfertaDisciplina::nrCombinacoesPrev(bool teorico, bool porTurno)
{
	Disciplina* disciplina = getDisciplina(teorico);
	auto calendarios = disciplina->getCalendarios();
	auto divisoes = disciplina->combinacao_divisao_creditos;

	// nr unidades a considerar
	int nrUnid = 0;
	auto clusters = campus_->getClustersUnids();
	auto itUnid = clusters.begin();
	for(; itUnid != clusters.end(); ++itUnid)
	{
		if(algumaSalaCluster(*itUnid, teorico))
			nrUnid++;

	}
	if(nrUnid == 0)
		return 0;

	long nrCombs = 0;
	for(auto itCal = calendarios.begin(); itCal != calendarios.end(); ++itCal)
	{
		// check cal
		if((*itCal) == nullptr)
			continue;

		for(auto itDiv = divisoes.begin(); itDiv != divisoes.end(); ++itDiv)
		{
			// check divisao
			if(!UtilHeur::checkDivisaoDisc(*itDiv, disciplina))
				continue;

			// cross check divisao e calendario
			unordered_map<int, std::vector<AulaHeur*>> aulas;
			if(!UtilHeur::getAulasPossiveis(*itCal, *itDiv, -1, -1, aulas))
				continue;

			if(!porTurno)
			{
				//nrCombs += UtilHeur::nrCombHorarios(*itCal, *itDiv);
				nrCombs += UtilHeur::nrCombAulas(aulas);
				continue;
			}

			for(auto itTurno = (*itCal)->turnosIES.begin(); itTurno != (*itCal)->turnosIES.end(); ++itTurno)
			{
				nrCombs += UtilHeur::nrCombHorarios(*itTurno, *itDiv);
			}
		}
	}

	return nrCombs*nrUnid;
}

// increase stat prof real
void OfertaDisciplina::incProfReal(int val)
{
	solucao_->stats_->nrTurmasProfReal_ += val;
}

// verificar se a estrutura está consistente
bool OfertaDisciplina::checkConsistencia(void)
{
	// obter todos os alunos registados nas turmas
	bool ok = true;
	unordered_set<AlunoHeur*> alunosTurmas;
	unordered_set<TurmaHeur*> turmas;
	getTurmas(turmas);
	for(auto itTurmas = turmas.cbegin(); itTurmas != turmas.cend(); ++itTurmas)
	{
		bool tipoAula = (*itTurmas)->tipoAula;
		auto& alunosTurma = turmasAlunos_.at(*itTurmas);
		for(auto itAlunos = alunosTurma.cbegin(); itAlunos != alunosTurma.cend(); ++itAlunos)
		{
			alunosTurmas.insert(*itAlunos);
			TurmaHeur* turmaAluno = nullptr;

			if(!getTurmaAlunoTipo(*itAlunos, tipoAula, turmaAluno))
			{
				ok = false;
				if(tipoAula)
					HeuristicaNuno::warning("OfertaDisciplina::checkConsistencia", "Aluno na turma teorica mas nao registado em alunosTurma_");
				else
					HeuristicaNuno::warning("OfertaDisciplina::checkConsistencia", "Aluno na turma pratica mas nao registado em alunosTurma_");
			}
			else if(turmaAluno != (*itTurmas))
			{
				ok = false;
				if(tipoAula)
					HeuristicaNuno::warning("OfertaDisciplina::checkConsistencia", "Aluno registado em alunosTurma_ numa turma teorica diferente");
				else
					HeuristicaNuno::warning("OfertaDisciplina::checkConsistencia", "Aluno registado em alunosTurma_ numa turma pratica diferente");
			}
		}
	}

	int nrAlunosOfDisc = alunosTurma_.size();
	int nrAlunosTurmas = alunosTurmas.size();
	if(nrAlunosOfDisc != nrAlunosTurmas)
	{
		HeuristicaNuno::logMsgInt("Nr alunos na oferta: ", nrAlunosOfDisc, 1);
		HeuristicaNuno::logMsgInt("Nr alunos nas turmas: ", nrAlunosTurmas, 1);
		HeuristicaNuno::warning("OfertaDisciplina::checkConsistencia", "Número de alunos na oferta e nas turmas é diferente!");
		ok = false;
	}

	return ok;
}
bool OfertaDisciplina::checkCompletos(void)
{
	bool ok = true;
	for(auto it = alunosTurma_.cbegin(); it != alunosTurma_.cend(); ++it)
	{
		bool incompleto = (alunosIncompleto_.find(it->first) != alunosIncompleto_.end());
		int nrTurmas = it->second.size();
		// verificar se tem zero turmas
		if(nrTurmas == 0)
		{
			ok = false;
			HeuristicaNuno::warning("OfertaDisciplina::checkCompletos", "Aluno em alunosTurma_ mas zero associado a zero turmas na estrutura");
		}
		// verificar se incompleto esta mal registado
		if((nrTurmas != 1) && incompleto)
		{
			ok = false;
			HeuristicaNuno::warning("OfertaDisciplina::checkCompletos", "Incompleto mas nao esta associado a so uma turma");
		}
		// verificar se ha incompletos nao registados
		if((nrTiposAula() == 2) && (nrTurmas != 2) && !incompleto)
		{
			ok = false;
			HeuristicaNuno::warning("OfertaDisciplina::checkCompletos", "E incompleto mas nao esta registado");
		}
		// verificar se esta mesmo alocado a essas turmas
		int realTurmas = 0;
		for(auto itTurma = it->second.begin(); itTurma != it->second.end(); ++itTurma)
		{
			if(itTurma->second->temAluno(it->first))
				realTurmas++;
		}
		if(realTurmas != nrTurmas)
		{
			ok = false;
			HeuristicaNuno::warning("OfertaDisciplina::checkCompletos", "Turmas registadas em alunosTurma_ estao incorretas");
		}
	}
	return ok;
}


// [1x1 ou 1xN] verificar se 1x1 ou 1xN é satisfeito
bool OfertaDisciplina::checkRelacTeoricasPraticas(void) const
{
	if(ParametrosHeuristica::relacPraticas == ParametrosHeuristica::abrePraticas::N_N
		|| this->nrTiposAula() != 2)
	{
		return true;
	}

	if(!checkRelacTipo(false))
	{
		HeuristicaNuno::warning("OfertaDisciplina::checkRelacTeoricasPraticas", "Relacionamento teoricas x praticas nao respeitado [Praticas]");
		return false;
	}
	if(ParametrosHeuristica::relacPraticas == ParametrosHeuristica::abrePraticas::UM_UM)
	{
		if(!checkRelacTipo(true))
		{
			HeuristicaNuno::warning("OfertaDisciplina::checkRelacTeoricasPraticas", "Relacionamento 1 x 1 nao respeitado [Teoricas]");
			return false;
		}
	}

	return true;
}
bool OfertaDisciplina::checkRelacTipo(bool teorico) const
{
	// só verificar Teorico quando é 1x1
	if(teorico && ParametrosHeuristica::relacPraticas != ParametrosHeuristica::abrePraticas::UM_UM)
		return true;

	auto it = turmas_.find(teorico);
	if(it == turmas_.end())
		return true;

	for(auto itT = it->second.begin(); itT != it->second.end(); ++itT)
	{
		if(!checkRelacTurma(itT->second))
			return false;
	}

	return true;
}
// verificar se todos os alunos da turma prática estão na mesma disciplina teórica
bool OfertaDisciplina::checkRelacTurma(TurmaHeur* const turma) const
{
	// só verificar Teorico quando é 1x1
	if(turma->tipoAula && ParametrosHeuristica::relacPraticas != ParametrosHeuristica::abrePraticas::UM_UM)
		return true;

	TurmaHeur* turmaCompOpp = nullptr;
	auto& alunosTurma = turmasAlunos_.at(turma);
	for(auto it = alunosTurma.begin(); it != alunosTurma.end(); ++it)
	{
		AlunoHeur* const aluno = *it;
		TurmaHeur* turmaOppAluno = nullptr;
		// verificar qual a turma do outro tipo que o aluno está alocado
		if(!getTurmaAlunoTipo(aluno, !turma->tipoAula, turmaOppAluno))
			continue;

		if(turmaCompOpp == nullptr)
			turmaCompOpp = turmaOppAluno;
		else if(turmaCompOpp != turmaOppAluno)
			return false;
	}

	return true;
}

// faz associação de turmas e verifica se é violada a regra de associação de turmas
void OfertaDisciplina::addAlunoCheckAssocTurmas_(AlunoHeur* const aluno, TurmaHeur* const turma)
{
	if(ParametrosHeuristica::relacPraticas == ParametrosHeuristica::abrePraticas::N_N || nrTiposAula() != 2)
	{
		return;
	}

	TurmaHeur* turmaOppAluno = nullptr;
	this->getTurmaAlunoTipo(aluno, !turma->tipoAula, turmaOppAluno);

	// so associar a teorica para reduzir espaço usado
	auto itAssoc = turmasAssoc_.find(turma);
	if(itAssoc == turmasAssoc_.end())
	{
		unordered_set<TurmaHeur*> assoc;
		if(!turmasAssoc_.insert(make_pair(turma, assoc)).second)
			HeuristicaNuno::excepcao("OfertaDisciplina::addAlunoCheckAssocTurmas_", "Associacao nao inserida");
	}

	// não dá para associar ainda
	if(turmaOppAluno == nullptr)
		return;

	addCheckAssocTurmas_(turma, turmaOppAluno);
}

// faz associação de turmas e verifica se é violada a regra de associação de turmas
void OfertaDisciplina::addCheckAssocTurmas_(TurmaHeur* const teorica, TurmaHeur* const pratica)
{
	if(ParametrosHeuristica::relacPraticas == ParametrosHeuristica::abrePraticas::N_N  || nrTiposAula() != 2)
		return;

	if(teorica == nullptr || pratica == nullptr)
	{
		HeuristicaNuno::warning("OfertaDisciplina::addCheckAssocTurmas_", "turma nullptr");
		return;
	}

	bool primTeor = teorica->tipoAula;
	bool segTeor = pratica->tipoAula;

	// verificar se sao do mesmo tipo
	if(primTeor == segTeor)
		HeuristicaNuno::excepcao("OfertaDisciplina::addCheckAssocTurmas_", "Duas turmas do mesmo tipo!");

	// respeitar a ordem
	if(!primTeor && segTeor)
		return addCheckAssocTurmas_(pratica, teorica);

	// Verificar primeiro a teorica
	auto itT = turmasAssoc_.find(teorica);
	if(itT == turmasAssoc_.end())
		HeuristicaNuno::excepcao("OfertaDisciplina::addCheckAssocTurmas_", "Associacao a turma teorica nao encontrada");

	if(ParametrosHeuristica::relacPraticas == ParametrosHeuristica::abrePraticas::UM_UM && itT->second.size() > 0)
	{
		if(pratica->getGlobalId() != (*itT->second.begin())->getGlobalId())
			HeuristicaNuno::excepcao("OfertaDisciplina::addCheckAssocTurmas_", "[1x1] Turma teorica ja associada a outra turma pratica!");
	}

	// associar prática à teórica
	itT->second.insert(pratica);


	// Verificar a prática
	auto itP = turmasAssoc_.find(pratica);
	if(itP == turmasAssoc_.end())
		HeuristicaNuno::excepcao("OfertaDisciplina::addCheckAssocTurmas_", "Associacao a turma pratica nao encontrada");

	if(itP->second.size() > 0 && (teorica->getGlobalId() != (*itP->second.begin())->getGlobalId()))
	{
		stringstream ss;
		ss << "[1x1 / 1xN] Turma pratica " << pratica->id << " da disciplina "
			<< this->getDisciplina()->getId() << " ja associada aa turma teorica " 
			<< (*itP->second.begin())->id << ". Tentativa de associa-la tb aa " 
			<< teorica->id;
		
		HeuristicaNuno::excepcao("OfertaDisciplina::addCheckAssocTurmas_", ss.str());
	}

	// associar teórica à prática
	itP->second.insert(teorica);
}

// remover associacoes a uma turma
void OfertaDisciplina::removeAssocTurma_(TurmaHeur* const turma)
{
	if(ParametrosHeuristica::relacPraticas == ParametrosHeuristica::abrePraticas::N_N || this->nrTiposAula() == 1)
		return;

	if(turma->ofertaDisc->getDisciplina()->getId()==14658)
		std::cout<<"\nremoveAssocTurma_: 14658";

	auto it = turmasAssoc_.find(turma);
	if(it == turmasAssoc_.end())
	{
		HeuristicaNuno::warning("OfertaDisciplina::removeAssocTurma_", "Associacoes da turma nao encontradas");
		return;
	}

	// remover associações à turma
	for(auto itAssoc = it->second.begin(); itAssoc != it->second.end(); ++itAssoc)
	{
		TurmaHeur* const turmaAssoc = *itAssoc;
		auto itOutra = turmasAssoc_.find(turmaAssoc);
		if(itOutra == turmasAssoc_.end())
		{
			HeuristicaNuno::warning("OfertaDisciplina::removeAssocTurma_", "Associacoes da turma associada nao encontradas");
			return;
		}

		itOutra->second.erase(turma);
	}

	// remover associações da turma
	turmasAssoc_.erase(it);
}

bool OfertaDisciplina::assocAulaContValida(TurmaHeur* const teorica, TurmaHeur* const pratica) const
{
	if (this->nrTiposAula() != 2) return true;
	if (!this->getDisciplina()->aulasContinuas()) return true;

	unordered_map<int, AulaHeur*> aulast;
	teorica->getAulas(aulast);

	unordered_map<int, AulaHeur*> aulasp;
	pratica->getAulas(aulasp);

	return checkAulasContinuas(aulasp,aulast);
}

bool OfertaDisciplina::checkAulasContinuas(unordered_map<int, AulaHeur*> const &aulasp, 
									unordered_map<int, AulaHeur*> const &aulast) const
{
	// verifica se toda aula pratica esta imediatamente após alguma aula teorica
	// atenção: aulas devem ser todas da mesma disciplina e de turmas associadas (1xN).

	bool ok=true;
	for(auto itAulap = aulasp.begin(); itAulap != aulasp.end(); ++itAulap)
	{
		int diap = itAulap->first;
		AulaHeur* aulap = itAulap->second;
		DateTime dti;
		aulap->getPrimeiroHor(dti);

		auto findDiaT = aulast.find(diap);
		if(findDiaT == aulast.end())
		{
			ok = false;
		}
		else
		{
			DateTime dtf;
			findDiaT->second->getLastHor(dtf);
			if (dti<dtf || (dti-dtf).timeMin() > ParametrosHeuristica::maxIntervAulas)
			{
				ok = false;
			}
		}
	}
	return ok;
}



// increase/decrease indicadores demanda de salas associadas!
void OfertaDisciplina::decIncIndicDem_(bool increase, bool equiv) const
{
	// parte teórica
	if(temCompTeorica())
		decIncIndicDemComp_(increase, true, equiv);

	// parte prática
	if(temCompPratica())
		decIncIndicDemComp_(increase, false, equiv);
}
void OfertaDisciplina::decIncIndicDemComp_(bool increase, bool teorico, bool equiv) const
{
	double credsAlunos = 0;
	int nrCreds = getNrCreds(teorico);
	if(nrCreds == 0 || nrDemandasNaoAtend_.size() == 0)
		return;

	for(auto itPrior = nrDemandasNaoAtend_.begin(); itPrior != nrDemandasNaoAtend_.end(); itPrior++)
	{
		if(itPrior->first == 1 || (equiv && itPrior->first == -1))
			credsAlunos += itPrior->second*nrCreds;
	}

	if(credsAlunos == 0)
		return;

	// dimuinuir indicadores
	unordered_set<SalaHeur*> salas;
	getSalasAssociadas(salas, teorico);
	if(salas.size() == 0)
		return;

	// dividir pelo numero de salas a que ta associado
	credsAlunos = (credsAlunos / salas.size());

	for(auto itSala = salas.begin(); itSala != salas.end(); ++itSala)
	{
		if(increase)
			(*itSala)->incIndicDem(credsAlunos);
		else
		{
			double ind = (*itSala)->decIndicDem(credsAlunos);
			if(ind < -0.0001)
			{
				HeuristicaNuno::logMsgDouble("indicDem = ", ind, 1);
				HeuristicaNuno::warning("OfertaDisciplina::decIncIndicDemComp_", "indice demanda negativo!");
			}
		}
	}
}

// escrever alocacoes para ficheiro text
void OfertaDisciplina::exportarAlocs(std::ofstream &file) const
{
	HeuristicaNuno::excepcao("OfertaDisciplina::exportarAlocs", "Nao implementado");
}

// print info
void OfertaDisciplina::printInfo(void) const
{
	stringstream ss;
	ss << "[OftDisc] Disc id: " << getDisciplina()->getId() << " ; Campus id: " << getCampus()->getId();
	HeuristicaNuno::logMsg(ss.str(), 1);

	stringstream ssInfo;
	ssInfo << "nr creds: " << getNrCreds() << " ; T: " << getNrCreds(true) << " ; P: " << getNrCreds(false);
	HeuristicaNuno::logMsg(ssInfo.str(), 1);

	stringstream ssDem;
	ssDem << "dem P1: " << getNrDemandasNaoAtend(1) << " ; dem P1 + equiv: " << getNrDemandasNaoAtend(1, true);
	HeuristicaNuno::logMsg(ssDem.str(), 1);
}

// [COMPARADORES]

// valor demandas nao atend >> nr creds >> nr tipos de aula >> nr creditos praticos >> nr labs
bool OfertaDisciplina::compOfDisc(OfertaDisciplina* const ofertaUm, OfertaDisciplina* const ofertaDois, int prioridade)
{
	// valor de demandas nao atendidas
	bool veredito = true;
	bool check = compDemNaoAtendV2(ofertaUm, ofertaDois, prioridade, veredito);
	if(check)
		return veredito;

	// dar prioridade a disciplinas com mais creditos
	int difCred = ofertaUm->getNrCreds() - ofertaDois->getNrCreds();
	if(difCred != 0)
		return difCred > 0;

	// nr de tipos de aula
	int frstTipos = ofertaUm->nrTiposAula();
	int scdTipos = ofertaDois->nrTiposAula();
	// dar prioridade a ofertas com duas componentes
	if(frstTipos != scdTipos)
		return frstTipos > scdTipos;

	// prioridade a ofertas com + creditos praticos
	int credsPrat = ofertaUm->getNrCreds(false);
	int credsPratOutro = ofertaDois->getNrCreds(false);
	if(credsPrat != credsPratOutro)
		return credsPrat > credsPratOutro;

	// prioridade a oferta com menos labs associados
	int nrLabsUm = ofertaUm->nrLabsAssoc();
	int nrLabsDois = ofertaDois->nrLabsAssoc();
	if(nrLabsUm != nrLabsDois)
		return nrLabsUm < nrLabsDois;

	return ofertaUm < ofertaDois;
}

// nr creds >> nr tipos de aula >> nr creditos praticos >> valor demandas nao atend >> nr labs
bool OfertaDisciplina::compOfDiscII(OfertaDisciplina* const ofertaUm, OfertaDisciplina* const ofertaDois, int prioridade)
{
	// dar prioridade a disciplinas com mais creditos
	int difCred = ofertaUm->getNrCreds() - ofertaDois->getNrCreds();
	if(difCred != 0)
		return difCred > 0;

	// nr de tipos de aula
	int frstTipos = ofertaUm->nrTiposAula();
	int scdTipos = ofertaDois->nrTiposAula();
	// dar prioridade a ofertas com duas componentes
	if(frstTipos != scdTipos)
		return frstTipos > scdTipos;

	// prioridade a ofertas com + creditos praticos
	int credsPrat = ofertaUm->getNrCreds(false);
	int credsPratOutro = ofertaDois->getNrCreds(false);
	if(credsPrat != credsPratOutro)
		return credsPrat > credsPratOutro;

	// valor de demandas nao atendidas
	bool veredito = true;
	bool check = compDemNaoAtendV2(ofertaUm, ofertaDois, prioridade, veredito);
	if(check)
		return veredito;

	// prioridade a oferta com menos labs associados
	int nrLabsUm = ofertaUm->nrLabsAssoc();
	int nrLabsDois = ofertaDois->nrLabsAssoc();
	if(nrLabsUm != nrLabsDois)
		return nrLabsUm < nrLabsDois;


	return ofertaUm < ofertaDois;
}

// tem pratico >> nr labs >> nr creditos praticos >> nr tipos de aula >> nr creds >> valor demandas nao atend
bool OfertaDisciplina::compOfDiscIII(OfertaDisciplina* const ofertaUm, OfertaDisciplina* const ofertaDois, int prioridade)
{
	// tem pratica
	bool pratUm = ofertaUm->temCompPratica();
	bool pratDois = ofertaDois->temCompPratica();
	if(pratUm != pratDois)
		return pratUm;

	// nr creditos praticos
	int credsPrat = ofertaUm->getNrCreds(false);
	int credsPratOutro = ofertaDois->getNrCreds(false);
	if(credsPrat != credsPratOutro)
		return credsPrat > credsPratOutro;

	// prioridade a oferta com menos labs associados
	int nrLabsUm = ofertaUm->nrLabsAssoc();
	int nrLabsDois = ofertaDois->nrLabsAssoc();
	if(nrLabsUm != nrLabsDois)
		return nrLabsUm < nrLabsDois;

	// dar prioridade a disciplinas com mais creditos
	int difCred = ofertaUm->getNrCreds() - ofertaDois->getNrCreds();
	if(difCred != 0)
		return difCred > 0;

	// valor de demandas nao atendidas
	bool veredito = true;
	bool check = compDemNaoAtendV2(ofertaUm, ofertaDois, prioridade, veredito);
	if(check)
		return veredito;

	return ofertaUm < ofertaDois;
}

// nr demandas não atendidas P1 orig >> nr creditos >> valor demandas nao atend
bool OfertaDisciplina::compOfDiscIV(OfertaDisciplina* const ofertaUm, OfertaDisciplina* const ofertaDois, int prioridade)
{
	// prioridade à oferta com mais demandas p (s/equiv)
	int diff = ofertaUm->getNrDemandasNaoAtend(prioridade) - ofertaDois->getNrDemandasNaoAtend(prioridade);
	if(diff != 0)
		return diff > 0;

	// prioridade à oferta com mais demandas p (c/equiv)
	diff = ofertaUm->getNrDemandasNaoAtend(prioridade, true) - ofertaDois->getNrDemandasNaoAtend(prioridade, true);
	if(diff != 0)
		return diff > 0;

	// dar prioridade às disciplinas com mais creditos
	diff = ofertaUm->getNrCreds() - ofertaDois->getNrCreds();
	if(diff != 0)
		return diff > 0;

	// dar prioridade às disciplinas com menos profs disponiveis
	diff = ofertaUm->getDisponibProfDisc() - ofertaDois->getDisponibProfDisc();
	if(diff != 0)
		return diff < 0;

	return ofertaUm < ofertaDois;
}

// nr demandas não atendidas P1 orig >> nr creditos >> prof real disponiv >> valor demandas nao atend
bool OfertaDisciplina::compOfDiscV(OfertaDisciplina* const ofertaUm, OfertaDisciplina* const ofertaDois, int prioridade)
{
	// prioridade à oferta com mais demandas p (s/equiv)
	int diff = ofertaUm->getNrDemandasNaoAtend(prioridade) - ofertaDois->getNrDemandasNaoAtend(prioridade);
	if(diff != 0)
		return diff > 0;

	// prioridade à oferta com mais demandas p (c/equiv)
	diff = ofertaUm->getNrDemandasNaoAtend(prioridade, true) - ofertaDois->getNrDemandasNaoAtend(prioridade, true);
	if(diff != 0)
		return diff > 0;

	// dar prioridade às disciplinas com mais creditos
	diff = ofertaUm->getNrCreds() - ofertaDois->getNrCreds();
	if(diff != 0)
		return diff > 0;

	// dar prioridade às disciplinas com menos profs disponiveis
	diff = ofertaUm->getNroCredsLivresProfsEstimados() - ofertaDois->getNroCredsLivresProfsEstimados();
	if(diff != 0)
		return diff < 0;

	return ofertaUm < ofertaDois;
}

// nr demandas não atendidas P1 orig >> nr creditos >> dispProf >> dem/profSala
bool OfertaDisciplina::compOfDiscVI(OfertaDisciplina* const ofertaUm, OfertaDisciplina* const ofertaDois, int prioridade)
{
	// prioridade à oferta com mais demandas p (s/equiv)
	int demUm = ofertaUm->getNrDemandasNaoAtend(prioridade);
	int demDois = ofertaDois->getNrDemandasNaoAtend(prioridade);
	int diff = demUm - demDois;
	if(diff != 0)
		return diff > 0;

	// prioridade à oferta com mais demandas p (c/equiv)
	diff = ofertaUm->getNrDemandasNaoAtend(prioridade, true) - ofertaDois->getNrDemandasNaoAtend(prioridade, true);
	if(diff != 0)
		return diff > 0;

	// dar prioridade às disciplinas com mais creditos
	diff = ofertaUm->getNrCreds() - ofertaDois->getNrCreds();
	if(diff != 0)
		return diff > 0;

	// dar prioridade às disciplinas com menos profs disponiveis
	int profUm = ofertaUm->getDisponibProfDisc();
	int profDois = ofertaDois->getDisponibProfDisc();
	diff = profUm - profDois;
	if(diff != 0)
		return diff < 0;
	
	// Prioridade à oferta com maior indice dem/prof
	if (profUm || profDois)
	{
		int profSalaUm = ofertaUm->getDisponibProfSalaDisc();
		int profSalaDois = ofertaDois->getDisponibProfSalaDisc();
		double divUm = (profSalaUm? (double) demUm/profSalaUm : demUm);
		double divDois = (profSalaDois? (double) demDois/profSalaDois : demDois);
		double diff = divUm - divDois;
		if(diff != 0)
			return diff > 0;
	}

	return ofertaUm < ofertaDois;
}


// Comparadores singulares
bool OfertaDisciplina::compDemNaoAtendV2(OfertaDisciplina* const ofertaUm, OfertaDisciplina* const ofertaDois, int prioridade, bool &veredito)
{
	// valor de demandas nao atendidas
	double val1 = ofertaUm->valorDemandasNaoAtendV2(prioridade);
	double val2 = ofertaDois->valorDemandasNaoAtendV2(prioridade);
	double diff = val1 - val2;
	if(std::abs(diff) > 0.0001)
	{
		veredito = val1 > val2;
		return true;
	}

	return false;
}



std::ostringstream& operator<< ( std::ostringstream &out, OfertaDisciplina const &ofDisc )
{
	out << "Id=" << ofDisc.getGlobalId()
		<< " DiscId=" << ofDisc.getDisciplina()->getId()
		<< " PT=" << (ofDisc.temCompTeorica() && ofDisc.temCompPratica())
		<< " nAtendP1=" << ofDisc.getNrDemandasNaoAtend(1)
		<< " nAtendP1+equiv=" << ofDisc.getNrDemandasNaoAtend(1,true)
		<< " NrCreds=" << ofDisc.getNrCreds()
		<< " DispProf=" << ofDisc.getNroCredsLivresProfsEstimados();
		//<< " DispProf=" << ofDisc.getDisponibProfDisc();
		
	//int profSala = ofDisc.getDisponibProfSalaDisc();
	//int dem = ofDisc.getNrDemandasNaoAtend(1);
	//double div = (profSala? (double)dem/profSala : dem);
	//out << " Dem/ProfSala=" << div;

	return out;
}