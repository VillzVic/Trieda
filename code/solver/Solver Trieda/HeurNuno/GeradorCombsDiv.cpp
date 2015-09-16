#include "GeradorCombsDiv.h"
#include "HeuristicaNuno.h"
#include "../Disciplina.h"
#include "../Calendario.h"
#include "ParametrosHeuristica.h"

int GeradorCombsDiv::maxCombs = 0;
int GeradorCombsDiv::maxCombsNrCreds = 0;

GeradorCombsDiv::GeradorCombsDiv(Disciplina* const disciplina)
	: disciplina_(disciplina), nrCreds_(disciplina->getTotalCreditos()), nrCombs_(0), set_(false)
{
}

GeradorCombsDiv::~GeradorCombsDiv(void)
{
}

// gera combinações de divisões
void GeradorCombsDiv::gerarDivisoes(void)
{
	// verificar se a turma tem creditos associados
	if(disciplina_->getTotalCreditos() == 0)
	{
		HeuristicaNuno::warning("GeradorCombsDiv::gerarDivisoes", "disciplina tem total de zero creditos!");
		return;
	}

	// set créditos permitidos
	bool gerarAll = (ParametrosHeuristica::gerarAllCombsCreds || disciplina_->combinacao_divisao_creditos.size() == 0);
	setCredsPermitidos_(gerarAll);

	// limpar divisões
	HeuristicaNuno::logMsgInt("# cred: ", nrCreds_, 2);
	HeuristicaNuno::logMsgInt("# divs (antes): ", (int)disciplina_->combinacao_divisao_creditos.size(), 2);
	disciplina_->combinacao_divisao_creditos.clear();

	// Gerar combinações
	unordered_map<int, int> combIni;
	fixaGeraCombs(2, 0, 0, combIni, disciplina_->combinacao_divisao_creditos);
	HeuristicaNuno::logMsgInt("# divs (depois): ", (int)disciplina_->combinacao_divisao_creditos.size(), 2);
	if(nrCombs_ > maxCombs)
	{
		maxCombs = nrCombs_;
		maxCombsNrCreds = nrCreds_;
	}
}

// print maximo numero de combinações encontrado e para quantos créditos foi
void GeradorCombsDiv::printMaxCombsInfo(void)
{
	stringstream ss;
	ss << "max divs creds: " << maxCombs << " (" << maxCombsNrCreds << " creds)";
	HeuristicaNuno::logMsg(ss.str(), 1);
}

// setup
void GeradorCombsDiv::setCredsPermitidos_(bool all)
{
	set_ = true;

	// init
	maxDias_ = 0;
	minCredsDia_ = 0;

	// inicializar max creds dia
	for(int dia = 2; dia <= 8; ++dia)
		maxCredsDia_.insert(make_pair(dia, 0));

	// set max creditos com base no numero de horarios dos calendarios
	setMaxCredsCalendarios_(!all);
	// set créditos permitidos com base nas combinacoes de divisoes
	if(!all)
		setCredsPermDivisoes_();
	// set créditos permitidos se não for baseado nas combinações de divisões
	else
		setCredsPermMinMax_();
}

void GeradorCombsDiv::setMaxCredsCalendarios_(bool div)
{
	// definir o max de creditos de cada dia com base nos horarios disponiveis dos calendarios
	if(disciplina_->calendariosReduzidos.size() == 0)
	{
		HeuristicaNuno::warning("GeradorCombsDiv::setMaxCredsDia_", "Disciplina nao associada a nenhum calendario");
		return;
	}

	for(auto itCal = disciplina_->calendariosReduzidos.begin(); itCal != disciplina_->calendariosReduzidos.end(); ++itCal)
	{
		if(itCal->mapDiaDateTime.size() == 0)
		{
			HeuristicaNuno::warning("GeradorCombsDiv::setMaxCredsDia_", "MapDiaDateTime vazio!");
			continue;
		}

		for(int dia = 2; dia <= 8; ++dia)
		{
			int max = maxCredsDia_.at(dia);
			if(max == nrCreds_)
				continue;

			int nrHors = 0;
			auto diaCal = itCal->mapDiaDateTime.find(dia);
			if(diaCal != itCal->mapDiaDateTime.end())
				nrHors = (int)diaCal->second.size();

			if(nrHors > nrCreds_)
				maxCredsDia_[dia] = nrCreds_;
			else if(nrHors > max)
				maxCredsDia_[dia] = nrHors;
		}
	}

	// se nao nos basearmos nas divisoes
	if(!div)
	{
		// max dias
		if(maxDias_ == 0)
			maxDias_ = 5;

		minCredsDia_ = 1;
	}
}

void GeradorCombsDiv::setCredsPermDivisoes_(void)
{
	for(auto itDiv = disciplina_->combinacao_divisao_creditos.begin(); itDiv != disciplina_->combinacao_divisao_creditos.end(); ++itDiv)
	{
		if(!UtilHeur::checkDivisaoDisc(*itDiv, disciplina_))
			continue;

		int dias = 0;
		for(auto it = itDiv->begin(); it != itDiv->end(); ++it)
		{
			int dia = it->first;
			int creds = it->second;
			if(creds <= 0)
				continue;

			// registar creds permitidos
			if(nrCredsPermitidos_.find(creds) == nrCredsPermitidos_.end())
				nrCredsPermitidos_.insert(creds);

			dias++;
		}
		if(dias > maxDias_)
			maxDias_ = dias;
	}
}

void GeradorCombsDiv::setCredsPermMinMax_(void)
{
	int maxCreds = -1;
	// mudar min creds dia se for maior que maximo
	for(int dia = 2; dia <= 8; ++dia)
	{
		int mxdia = maxCredsDia_[dia];
		if(mxdia > 1 && mxdia < minCredsDia_)
			minCredsDia_ = mxdia;

		if(mxdia > maxCreds)
			maxCreds = mxdia;
	}

	nrCredsPermitidos_.clear();
	for(int creds = minCredsDia_; creds <= maxCreds; ++creds)
		nrCredsPermitidos_.insert(creds);
}

// funcoes
void GeradorCombsDiv::fixaGeraCombs(int dia, int sumCreds, int nrDias, unordered_map<int, int> const &comb, 
									vector<vector<pair<int, int>>> &combinacoes)
{
	if(dia > 8)
		return;

	int proxDia = dia + 1;

	// fixar dia com zero creditos
	if(dia < 8 && sumCreds < nrCreds_)
	{
		unordered_map<int, int> copyComb (comb);
		fixaGeraCombs(proxDia, sumCreds, nrDias, copyComb, combinacoes);
	}
	// se máximo for zero retornar pq opção com zero já foi testada
	int maxDia = maxCredsDia_.at(dia);
	if(maxDia == 0)
		return;

	// verificar se máximo de dias é excedido
	if(nrDias + 1 > maxDias_)
		return;

	// fixar dia com numero de creditos entre o min e max de creditos!
	for(auto it = nrCredsPermitidos_.begin(); it != nrCredsPermitidos_.end(); ++it)
	{
		int creds = *it;
		if(creds < 0)
			HeuristicaNuno::excepcao("GeradorCombsDiv::fixaGeraCombs", "creditos negativos");

		if(creds == 0)
			continue;

		// excede o máxido do dia
		if(creds > maxDia)
			continue;

		// se já tiver o numero de creditos necessario adicionar comb, se nao fixar e continuar
		int newSum = sumCreds + creds;
		if(newSum > nrCreds_)
			break;
		else 
		{
			// copiar comb
			unordered_map<int, int> copyComb (comb);
			copyComb[dia] = creds;

			if(newSum == nrCreds_)
			{
				addComb_(combinacoes, copyComb);
				break;
			}
			else // (newSum < nrCreds_)
			{
				fixaGeraCombs(proxDia, newSum, nrDias + 1,  copyComb, combinacoes);
			}
		}
	}
}

void GeradorCombsDiv::addComb_(vector<vector<pair<int, int>>> &combinacoes, unordered_map<int, int> const &comb)
{
	// dia -> nrCreds
	std::vector<std::pair<int, int>> divisao;

	int nrCreds = 0;	// check
	for(auto itDia = comb.begin(); itDia != comb.end(); ++itDia)
	{
		if(itDia->second < 0)
			HeuristicaNuno::excepcao("GeradorCombsDiv::addComb_", "Numero de creditos do dia menor que zero!");

		// inserir só quando tem créditos!
		if(itDia->second > 0)
		{
			divisao.push_back(make_pair(itDia->first, itDia->second));
			nrCreds += itDia->second;
		}
	}

	if(nrCreds != nrCreds_)
		HeuristicaNuno::excepcao("GeradorCombsDiv::addComb_", "Numero de creditos errado!");

	combinacoes.push_back(divisao);
	nrCombs_++;
}


void GeradorCombsDiv::printMaxCreds_(void)
{
	std::stringstream ss;
	ss << "div: ";
	unordered_map<int, int>::const_iterator itDia = maxCredsDia_.begin();
	for(; itDia != maxCredsDia_.end(); itDia++)
	{
		ss << itDia->first << " (" << itDia->second << "); ";
	}
	HeuristicaNuno::logMsg(ss.str(), 1);
}