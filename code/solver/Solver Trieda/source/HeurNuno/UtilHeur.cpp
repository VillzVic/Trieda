#include "UtilHeur.h"
#include "../Calendario.h"
#include "../HorarioAula.h"
#include "../AlunoDemanda.h"
#include "../Aluno.h"
#include "HeuristicaNuno.h"
#include "../ProblemData.h"
#include "../ParametrosPlanejamento.h"
#include "DateTime.h"
#include "AulaHeur.h"
#include "AlunoHeur.h"
#include "ParametrosHeuristica.h"
#include "../CentroDados.h"
#include "../Disciplina.h"
#include <iostream>
#include <string>
#include <stdio.h>
#include <time.h>
#include <iomanip>

#ifdef PRINT_MEMORY
#include <Psapi.h>
#include <Windows.h>
#endif

// init
unordered_map<int, AulaHeur> UtilHeur::aulasHeurId_;
unordered_map<size_t, int> UtilHeur::hashesToIdAulasReg_;
int UtilHeur::idAulas = 0;

unordered_map<Calendario*, unordered_map<int, unordered_map<int, vector<AulaHeur*>>>> UtilHeur::aulasCalendario;
// registar dominância de calendários
unordered_map<int, unordered_set<int>> UtilHeur::dominanciaCalendarios_;
unordered_map<int, unordered_set<int>> UtilHeur::calendarioTurnos_;
unordered_map<int, unordered_set<int>> UtilHeur::calTurnosNot_;
// registar abrangência
unordered_map<int, unordered_map<int, unordered_set<int>>> UtilHeur::turnosAbrangem;
unordered_map<int, unordered_map<int, unordered_set<int>>> UtilHeur::turnosNaoAbrangem;
unordered_map<int, unordered_map<int, unordered_set<int>>> UtilHeur::calendariosAbrangem;
unordered_map<int, unordered_map<int, unordered_set<int>>> UtilHeur::calendariosNaoAbrangem;
unordered_map<int,unordered_map<int, unordered_map<int, unordered_set<int>>>> UtilHeur::calendariosAbrangemNoTurno;
unordered_map<int,unordered_map<int, unordered_map<int, unordered_set<int>>>> UtilHeur::calendariosNaoAbrangemNoTurno;

UtilHeur::UtilHeur(void)
{
}

UtilHeur::~UtilHeur(void)
{
}

// verifica se um calendario contem um horario
bool UtilHeur::calendarioAbrangeNoTurno(Calendario* const calendario, 
								TurnoIES* const turno, int dia, HorarioAula* const horario)
{
	if(calendario == nullptr || calendario == NULL)
		HeuristicaNuno::excepcao("UtilHeur::calendarioAbrangeNoTurno", "calendario nulo!");
	
	if(turno == nullptr || turno == NULL)
		HeuristicaNuno::excepcao("UtilHeur::calendarioAbrangeNoTurno", "turno nulo!");

	int calendarioId = calendario->getId();
	int turnoId = turno->getId();

	// verificar se ja foi registado
	int check = checkRegAbrange(calendarioId, turnoId, dia, 
						horario, calendariosAbrangemNoTurno, calendariosNaoAbrangemNoTurno);
	if(check > 0)
		return true;
	else if(check < 0)
		return false;

	auto horariosDia = calendario->retornaHorariosDisponiveis(dia,turnoId);
	// verifica se o calendario tem um horário igual no turno
	for(auto it = horariosDia.cbegin(); it != horariosDia.cend(); ++it)
	{
		if((*it)->inicioFimIguais(horario))
		{
			// registar abrangência
			regAbrangencia(calendarioId, turnoId, dia, horario, calendariosAbrangemNoTurno);
			return true;
		}
	}

	// registar não abrangência
	regAbrangencia(calendarioId, turnoId, dia, horario, calendariosNaoAbrangemNoTurno);

	return false;
}

bool UtilHeur::calendarioAbrange(Calendario* const calendario, int dia, HorarioAula* const horario)
{
	if(calendario == nullptr || calendario == NULL)
		HeuristicaNuno::excepcao("UtilHeur::calendarioAbrange", "calendário nulo!");

	int calendarioId = calendario->getId();
	// verificar se ja foi registado
	int check = checkRegAbrange(calendarioId, dia, horario, calendariosAbrangem, calendariosNaoAbrangem);
	if(check > 0)
		return true;
	else if(check < 0)
		return false;

	auto horariosDia = calendario->retornaHorariosDisponiveisNoDia(dia);
	// verifica se o turno tem um horário igual
	for(auto it = horariosDia.begin(); it != horariosDia.end(); ++it)
	{
		// verificar dia semana (nao precisa)
		//if(it->dias_semana.find(dia) == it->dias_semana.end())
		//	continue;

		if(it->inicioFimIguais(horario))
		{
			// registar abrangência
			regAbrangencia(calendarioId, dia, horario, calendariosAbrangem);
			return true;
		}
	}

	// registar não abrangência
	regAbrangencia(calendarioId, dia, horario, calendariosNaoAbrangem);

	return false;
}
bool UtilHeur::calendarioAbrange(Calendario* const calendario, int dia, set<HorarioAula*> const &horarios)
{
	for(auto it = horarios.begin(); it != horarios.end(); ++it)
	{
		if(!UtilHeur::calendarioAbrange(calendario, dia, *it))
			return false;
	}
	return true;
}
bool UtilHeur::turnoAbrange(TurnoIES* const turno, int dia, HorarioAula* const horario)
{
	if(turno == nullptr || turno == NULL)
		HeuristicaNuno::excepcao("UtilHeur::turnoAbrange", "turno nulo!");

	int turnoId = turno->getId();
	// verificar se ja foi registado
	int check = checkRegAbrange(turnoId, dia, horario, turnosAbrangem, turnosNaoAbrangem);
	if(check > 0)
		return true;
	else if(check < 0)
		return false;
	
	// verifica se o turno tem um horário igual
	for(auto it = turno->horarios_aula.begin(); it != turno->horarios_aula.end(); ++it)
	{
		// verificar dia semana
		if(it->dias_semana.find(dia) == it->dias_semana.end())
			continue;

		if(it->inicioFimIguais(horario))
		{
			// registar abrangência
			regAbrangencia(turnoId, dia, horario, turnosAbrangem);
			return true;
		}
	}
	
	// registar não abrangência
	regAbrangencia(turnoId, dia, horario, turnosNaoAbrangem);

	return false;
}

bool UtilHeur::horariosAbrangem(gGroupHorarios const &horarios, int dia, HorarioAula* const horario)
{
	if(horarios.size() == 0)
		return false;

	for(auto it = horarios.begin(); it != horarios.end(); ++it)
	{
		HorarioAula* const outro = (*it);
		if(outro->dias_semana.find(dia) == outro->dias_semana.end())
			continue;

		if(outro->inicioFimIguais(horario))
			return true;
	}
	return false;
}

// verifica se o calendario tem algum horario do turno
bool UtilHeur::calendarioContemAnyHorarioTurno(Calendario* const calendario, TurnoIES* const turno)
{
	int calId = calendario->getId();
	int turnoId = turno->getId();

	// verificar se já foi registado que está contido
	auto itReg = calendarioTurnos_.find(calId);
	if(itReg != calendarioTurnos_.end() && itReg->second.find(turnoId) != itReg->second.end())
		return true;

	// verificar se já foi registado que não está contido
	auto itRegNot = calTurnosNot_.find(calId);
	if(itRegNot != calTurnosNot_.end() && itRegNot->second.find(turnoId) != itRegNot->second.end())
		return false;

	// verificar se o calendário tem todos os horários do turno
	bool tem = false;
	for(auto itHors = turno->horarios_aula.begin(); itHors != turno->horarios_aula.end(); ++itHors)
	{
		for(auto itHorsCal = calendario->horarios_aula.begin(); itHorsCal != calendario->horarios_aula.end(); ++itHorsCal)
		{
			if(itHors->inicioFimIguais(*itHors))
			{
				tem = true;
				break;
			}
		}
		if(tem)
			break;
	}

	// registar
	if(tem)
	{
		if(itReg == calendarioTurnos_.end())
		{
			unordered_set<int> vazio;
			itReg = calendarioTurnos_.insert(make_pair(calId, vazio)).first;
		}
		itReg->second.insert(turnoId);
	}
	else
	{
		if(itRegNot == calTurnosNot_.end())
		{
			unordered_set<int> vazio;
			itRegNot = calTurnosNot_.insert(make_pair(calId, vazio)).first;
		}
		itRegNot->second.insert(turnoId);
	}

	return tem;
}

void UtilHeur::initAulasCalendario(void)
{
	unordered_map<int, unordered_map<int, vector<AulaHeur*>>> emptyDia;
	unordered_map<int, vector<AulaHeur*>> emptyCreds;
	vector<AulaHeur*> aulasDiaCreds;
	for(auto itCal = HeuristicaNuno::probData->calendarios.begin(); itCal != HeuristicaNuno::probData->calendarios.end(); ++itCal)
	{
		Calendario* const calendario = *itCal;
		UtilHeur::aulasCalendario[calendario] = emptyDia;

		for(int dia = 2; dia <=8; ++dia)
		{
			auto itDia = calendario->mapDiaDateTime.find(dia);
			if(itDia == calendario->mapDiaDateTime.end())
				continue;

			UtilHeur::aulasCalendario[calendario][dia] = emptyCreds;
			int maxCreds = itDia->second.size();
			if(maxCreds == 0)
				continue;

			for(int creds = 1; creds <= maxCreds; ++creds)
			{
				aulasDiaCreds.clear();
				UtilHeur::getAulasPossiveisDia(dia, creds, calendario, -1, -1, aulasDiaCreds);
				UtilHeur::aulasCalendario[calendario][dia][creds] = aulasDiaCreds;
			}
		}
	}
}

void UtilHeur::getAulasCalDiaCreds(Calendario* const calendario, int dia, int nrCreds, vector<AulaHeur*> &aulas)
{
	aulas.clear();
	auto itCal = aulasCalendario.find(calendario);
	if(itCal == aulasCalendario.end())
		return;

	auto itDia = itCal->second.find(dia);
	if(itDia == itCal->second.end())
		return;

	auto itCreds = itDia->second.find(nrCreds);
	if(itCreds == itDia->second.end())
		return;

	aulas = itCreds->second;
}

// check formandos
void UtilHeur::temFormandoCoReq(unordered_set<AlunoDemanda*> const &setAlunos, bool &temFormando, bool &temCoReq)
{
	temFormando = false;
	temCoReq = false;
	for(auto it = setAlunos.cbegin(); it != setAlunos.cend(); ++it)
	{
		if(!temFormando && (*it)->getAluno()->ehFormando())
			temFormando = true;

		//pode ser false signal
		if(!temCoReq && (*it)->demanda->oferta->curriculo->possuiCorrequisito((*it)->demanda->getDisciplinaId()))
			temCoReq = true;
	}
}
void UtilHeur::temFormandoCoReq(OfertaDisciplina* const &ofertaDisc, unordered_set<AlunoHeur*> const &setAlunos, bool &temFormando, bool &temCoReq)
{
	temFormando = false;
	temCoReq = false;
	for(auto it = setAlunos.cbegin(); it != setAlunos.cend(); ++it)
	{
		if(!temFormando && (*it)->ehFormando())
			temFormando = true;

		if(!temCoReq && (*it)->temCoRequisito(ofertaDisc))
			temCoReq = true;

		if(temFormando && temCoReq)
			return;
	}
}

const std::string UtilHeur::currentDateTime() {
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);

    strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);

    return buf;
}

const std::string UtilHeur::horarioAulaToString(HorarioAula const &horario)
{
	DateTime inicio = horario.getInicio();
	DateTime final = horario.getFinal();
	std::stringstream ss;
	char horarios[16];
	sprintf(horarios, "[%.2i:%.2i - %.2i:%.2i]", inicio.getHour(), inicio.getMinute(), final.getHour(), final.getMinute());
	ss << horarios;

	//ss << "[" << inicio.getHour() << ":" << inicio.getMinute() << " - ";
	//ss << final.getHour() << ":" << final.getMinute() << "]";
	
	return ss.str();
}

// calcula o intervalo entre dois horários. se se intersectam retorna um valor negativo (minutos!)
int UtilHeur::intervaloHorarios(const HorarioAula* const anterior, const HorarioAula* const posterior)
{
	// se anterior nao for anterior
	if(posterior->getInicio().earlierTime(anterior->getInicio()))
		return intervaloHorarios(posterior, anterior);

	int interMin = posterior->getInicio().timeMin() - anterior->getFinal().timeMin();

	return interMin;
}

// total créditos da divisão
int UtilHeur::totalCreds(std::vector<std::pair<int, int>> const &divisao)
{
	int creds = 0;
	auto it = divisao.begin();
	for(; it != divisao.end(); ++it)
	{
		creds += it->second;
	}
	
	return creds;
}

// get id de aula (hash value)
AulaHeur* UtilHeur::saveAula(AulaHeur const &aula)
{
	// POR HASHVALUE
	size_t hash = aula.hashValue();
	AulaHeur* ptrAula = nullptr;
	auto it = hashesToIdAulasReg_.find(hash);
	// aula ainda não registada
	if(it == hashesToIdAulasReg_.end())
	{
		ptrAula = addAula_(aula, hash);
	}
	else
	{
		// verificar se a aula registada é a mesma!
		auto itReg = aulasHeurId_.find(it->second);
		if(itReg == aulasHeurId_.end())
			HeuristicaNuno::excepcao("UtilHeur::saveAula", "Aula ja registada nao encontrada");
		
		if(itReg->second != aula)
		{
			ptrAula = addAula_(aula, hash);
			//HeuristicaNuno::warning("UtilHeur::saveAula", "Aula diferente da registada com mesmo hash!");
		}
		else
		{
			// aula é a mesma, retornar ptr
			ptrAula = &itReg->second;
		}
	}

	if(ptrAula == nullptr)
		HeuristicaNuno::excepcao("UtilHeur::saveAula", "Ops! Algum erro no id!");

	return ptrAula;
}
// get aula form hash value
AulaHeur* UtilHeur::getAula(int id)
{
	auto it = aulasHeurId_.find(id);
	if(it == aulasHeurId_.end())
	{
		HeuristicaNuno::logMsgInt("id aula: ", id, 1);
		//HeuristicaNuno::logMsgInt("# aulas reg: ", (int)aulasHeurHash_.size(), 1);
		HeuristicaNuno::excepcao("UtilHeur::getAula", "Aula com este id não encontrada!");
	}

	return &it->second;
}

// devolve novas aulas com nova unidade
void UtilHeur::aulasUnidade(unordered_map<int, AulaHeur*> const &aulas, int unidadeId, unordered_map<int, AulaHeur*> &newAulas)
{
	for(auto itOld = aulas.begin(); itOld != aulas.end(); ++itOld)
	{
		AulaHeur* const aula = itOld->second;
		AulaHeur newAula (aula->calendarioId, aula->campusId, unidadeId, aula->horarios);
		AulaHeur* ptrNewAula = UtilHeur::saveAula(newAula);
		if(!newAulas.insert(make_pair(itOld->first, ptrNewAula)).second)
			HeuristicaNuno::excepcao("UtilHeur::aulasUnidade", "nova aula nao inserida");
	}
}
void UtilHeur::aulasCampusUnid(vector<AulaHeur*> const &aulas, int campusId, int unidadeId, vector<AulaHeur*> &newAulas)
{
	for(auto itOld = aulas.begin(); itOld != aulas.end(); ++itOld)
	{
		AulaHeur* const aula = (*itOld);
		AulaHeur newAula (aula->calendarioId, campusId, unidadeId, aula->horarios);
		AulaHeur* ptrNewAula = UtilHeur::saveAula(newAula);
		newAulas.push_back(ptrNewAula);
	}
}

void UtilHeur::printDivisao(std::vector< std::pair< int, int> > const &divisao, int lvl, bool total)
{
	std::stringstream ss;
	ss << "div: ";
	std::vector< std::pair< int, int> >::const_iterator itDiv = divisao.begin();
	for(; itDiv!= divisao.end(); itDiv++)
	{
		if(itDiv->second > 0 || total)
		{
			ss << itDiv->first << " (" << itDiv->second << "); ";
		}
	}
	HeuristicaNuno::logMsg(ss.str(), lvl);
}
void UtilHeur::printDivisao(unordered_map<int, int> const &divisao, int lvl, bool total)
{
	std::stringstream ss;
	ss << "div: ";
	for(auto itDiv = divisao.begin(); itDiv != divisao.end(); itDiv++)
	{
		if(itDiv->second > 0 || total)
		{
			ss << itDiv->first << " (" << itDiv->second << "); ";
		}
	}
	HeuristicaNuno::logMsg(ss.str(), lvl);
}
// imprime array
void UtilHeur::printArray(int* arr, int size, int lvl)
{
	std::stringstream ss;
	ss << "array: ";
	for(int i=0; i<size; ++i)
	{
		ss << i << ": " << arr[i] << "); ";
	}
	HeuristicaNuno::logMsg(ss.str(), lvl);
}

void UtilHeur::printMemoryUsed()
{	
	//std::cout << (AulaHeur.nrBuild - AulaHeur.nrDestroy) * sizeof(AulaHeur);
	int nrAulas = aulasHeurId_.size();
	std::cout << "\nSize of aulasHeurId_ = " << (nrAulas) * (sizeof(AulaHeur) + sizeof(int));

	int size = hashesToIdAulasReg_.size();
	std::cout << "\nSize of hashesToIdAulasReg_ = " << (size) * (sizeof(size_t) + sizeof(int));

}


// valor da demanda
// get valor de demanda
double UtilHeur::getValDemPrior(int prioridade)
{
	int absPri = std::abs(prioridade);
	double coef = 1.0 / std::powf(absPri, 3.0);
	// equivalencia
	if(prioridade < 0)
		coef = coef * ParametrosHeuristica::coefEquivalenciaMIP;

	return coef;
}

// retorna as horas do dia como um double
double UtilHeur::horasDouble(DateTime const &time)
{
	double horas = time.getHour();
	horas += double(time.getMinute())/ 60;

	return horas;
}
// imprime as horas do dia
std::string UtilHeur::strHoras(DateTime const &time)
{
	std::stringstream ssHor, ssMin, ssTime;
	ssHor << setfill('0') << setw(2) << time.getHour();
	ssMin << setfill('0') << setw(2) << time.getMinute();

	ssTime << ssHor.str() << ":" << ssMin.str();

	return ssTime.str();
}
void UtilHeur::printHoras(DateTime const &time, int lvl)
{
	HeuristicaNuno::logMsg(strHoras(time), 1);
}

// string to char*
char* UtilHeur::stringToChar(std::string str)
{
	char* writable = new char[str.size() + 1];
	std::copy(str.begin(), str.end(), writable);
	writable[str.size()] = '\0';

	return writable;
}

// numero de combinacoes de horarios de uma divisao num calendario
long UtilHeur::nrCombHorarios(Calendario* const calendario, std::vector<std::pair<int, int>> &divisao)
{
	long nr = 1;
	for(auto it = divisao.begin(); it != divisao.end(); ++it)
	{
		int dia = it->first;
		int creds = it->second;
		if(creds == 0)
			continue;

		auto horariosDia = calendario->retornaHorariosDisponiveisNoDia(dia);
		long nrCombDia = horariosDia.size() - creds + 1;
		nr = nr * nrCombDia;
	}
	return nr;
}
// numero de combinacoes de horarios de uma divisao num turno
long UtilHeur::nrCombHorarios(TurnoIES* const turno, std::vector<std::pair<int, int>> &divisao)
{
	long nr = 1;
	for(auto it = divisao.begin(); it != divisao.end(); ++it)
	{
		int dia = it->first;
		int creds = it->second;
		if(creds <= 0)
			continue;

		gGroupHorarios horariosDia;
		turno->retornaHorariosDisponiveisNoDia(dia, horariosDia);
		long nrCombDia = horariosDia.size() - creds + 1;
		nr = nr * nrCombDia;
	}
	return nr;
}
// numero de combinacoes possiveis aulas
int UtilHeur::nrCombAulas(const unordered_map<int, std::vector<AulaHeur*>> &possiveisAulas)
{
	int nrCombs = 1;
	for(auto it = possiveisAulas.begin(); it != possiveisAulas.end(); ++it)
	{
		int n = it->second.size();
		if(n > 0)
			nrCombs = nrCombs*n;
	}
	return nrCombs;
}

// VERIFICACAO

// verifica se a aula tem creditos suficientes
void UtilHeur::checkCreds(AulaHeur* const aula, int creditos, std::string metodo)
{
	int credsAula = aula->nrCreditos();
	if(aula->nrCreditos() != creditos)
	{
		HeuristicaNuno::logMsgInt("creds aula: ", credsAula, 1);
		HeuristicaNuno::logMsgInt("creditos: ", creditos, 1);
		HeuristicaNuno::excepcao(metodo, "Numero de creditos incorrecto!");
	}
}
void UtilHeur::checkAulaId(int id, std::string metodo)
{
	if(id > 0)
		return;

	HeuristicaNuno::logMsgInt("aula id: ", id, 1);
	HeuristicaNuno::excepcao(metodo, "id aula negativo!");
}

// verificar se não há turmas potenciais
bool UtilHeur::turmasPotenciaisVazia(unordered_map<int, set<TurmaPotencial*>> const &turmasPotenciais)
{
	int nrTurmas = 0;

	auto it = turmasPotenciais.begin();
	for(; it != turmasPotenciais.end(); ++it)
	{
		nrTurmas += it->second.size();
	}

	return nrTurmas == 0;
}
// unir sets turmas potenciais
void UtilHeur::unirSetsTurmasPot(set<const TurmaPotencial* const> const &set1, set<const TurmaPotencial* const> const &set2, set<const TurmaPotencial* const> &final)
{
	// insere elementos do set 1
	for(auto it1 = set1.cbegin(); it1 != set1.cend(); ++it1)
		final.insert(*it1);

	// insere elementos do set 2
	for(auto it2 = set2.cbegin(); it2 != set2.cend(); ++it2)
		final.insert(*it2);
}

// check divisão disciplina
bool UtilHeur::checkDivisaoDisc(std::vector<std::pair<int, int>> const &divisao, Disciplina* const disciplina)
{
	if(divisao.size() == 0)
	{
		HeuristicaNuno::warning("UtilHeur::checkDivisaoDisc", "Divisao size 0!");
		return false;
	}

	int creds = UtilHeur::totalCreds(divisao);
	int credsDisc = disciplina->getTotalCreditos();

	if(creds != credsDisc)
	{
		/*UtilHeur::printDivisao(divisao, 1);
		HeuristicaNuno::logMsgInt("disciplina id: ", disciplina->getId(), 1);
		HeuristicaNuno::logMsgInt("creds teoricos: ", disciplina->getCredTeoricos(), 1);
		HeuristicaNuno::logMsgInt("creds praticos: ", disciplina->getCredPraticos(), 1);
		HeuristicaNuno::logMsgInt("nr creds turma divisão: ", creds, 1);
		HeuristicaNuno::logMsgInt("nr creds disciplina: ", credsDisc, 1);
		HeuristicaNuno::warning("UtilHeur::checkDivisaoDisc", "Número de créditos da divisão é diferente do necessário!!!");*/
		return false;
	}

	for(auto itDiv = divisao.cbegin(); itDiv != divisao.cend(); ++itDiv)
	{
		int dia = itDiv->first;
		if(dia <= 1 || dia >= 9)
		{
			UtilHeur::printDivisao(divisao, 1, true);
			HeuristicaNuno::logMsgInt("dia: ", dia, 1);
			HeuristicaNuno::warning("AbridorTurmas::checkDivisaoDisc", "Valor do dia invalido!!!");
		}
	}

	return true;
}
// check aulas possíveis
void UtilHeur::checkAulasPoss(std::vector<std::pair<int, int>> const &divisao, unordered_map<int, std::vector<AulaHeur*>> const &aulas)
{
	for(auto itDiv = divisao.begin(); itDiv != divisao.end(); ++itDiv)
	{
		int dia = itDiv->first;
		int creds = itDiv->second;

		if(creds == 0)
			continue;

		auto itAulas = aulas.find(dia);
		if(itAulas == aulas.end())
		{
			UtilHeur::printDivisao(divisao, 1);
			HeuristicaNuno::excepcao("UtilHeur::checkAulasPoss", "Aulas nao encontradas para o dia!");
		}

		for(auto itAulasDia = itAulas->second.begin(); itAulasDia != itAulas->second.end(); ++itAulasDia)
		{
			UtilHeur::checkCreds(*itAulasDia, creds, "UtilHeur::checkAulasPoss");
		}
	}
}
// nr creditos total das aulas
int UtilHeur::nrCredsAulas(unordered_map<int, AulaHeur*>const &aulas)
{
	int nrCreds = 0;
	for(auto itAulas = aulas.begin(); itAulas != aulas.end(); ++itAulas)
	{
		nrCreds += itAulas->second->nrCreditos();
	}
	return nrCreds;
}

// get disciplinas de um curso
void UtilHeur::getDisciplinasCurso(Curso* const curso, unordered_set<Disciplina*> &disciplinas, bool equiv)
{
	ProblemData* const probData = CentroDados::getProblemData();
	for(auto itCurr = curso->curriculos.begin(); itCurr != curso->curriculos.end(); ++itCurr)
	{
		for(auto itMapDisc = itCurr->disciplinas_periodo.begin(); itMapDisc != itCurr->disciplinas_periodo.end(); ++itMapDisc)
		{
			Disciplina* const disc = itMapDisc->first;
			disciplinas.insert(disc);

			if ( equiv && probData->parametros->considerar_equivalencia_por_aluno )
			{
				for(auto itDiscEq = disc->discEquivSubstitutas.begin(); itDiscEq != disc->discEquivSubstitutas.end(); ++itDiscEq)
				{
					// ja inserida
					if(disciplinas.find(*itDiscEq) != disciplinas.end())
						continue;

					if ( probData->ehSubstituivel( disc->getId(), itDiscEq->getId(), curso ) )
						disciplinas.insert( *itDiscEq ); 
				}
			}
		}
	}
}



// ABRIDOR TURMAS

// retorna as aulas com 'nrCreditos' possiveis num determinado dia do calendario
bool UtilHeur::getAulasPossiveis(Calendario * const calendario, std::vector<std::pair<int,int>> const &divisao, int campusId, 
										int unidadeId, unordered_map<int, std::vector<AulaHeur*>> &aulas)
{
	HeuristicaNuno::logMsg("get aulas possiveis...", 2);

	for(auto itComb = divisao.cbegin(); itComb != divisao.cend(); ++itComb)
	{
		int dia = itComb->first;
		int creditos = itComb->second;
		if(creditos == 0)
			continue;

		if(dia >= 9 || dia <= 1)
		{
			HeuristicaNuno::logMsgInt("dia: ", dia, 1);
			HeuristicaNuno::excepcao("UtilHeur::getAulasPossiveis_", "valor dia invalido!");
		}

		std::vector<AulaHeur*> aulasDia;
		if(!getAulasPossiveisDia(dia, creditos, calendario, campusId, unidadeId, aulasDia))
			return false;

		// double check
		if(aulasDia.size() == 0)
			return false;

		// verificar aulas dia feito no final!
		/*auto itAulasDia = aulasDia.begin();
		for(; itAulasDia != aulasDia.end(); ++itAulasDia)
		{
			int aulaId = *itAulasDia;
			UtilHeur::checkAulaId(aulaId, "AbridorTurmas::getAulasPossiveis_");

			AulaHeur* aula = UtilHeur::getAula(aulaId);
			UtilHeur::checkCreds(aula, creditos, "AbridorTurmas::getAulasPossiveis_");
		}*/

		std::pair<int, vector<AulaHeur*>> par (dia, aulasDia);
		if(!aulas.insert(par).second)
			HeuristicaNuno::excepcao("UtilHeur::getAulasPossiveis_", "Aulas dia nao inseridas!");
	}

	// check
	checkAulasPoss(divisao, aulas);

	return aulas.size() != 0;
}

// com base no calendário e na divisão selecciona as aulas possiveis
bool UtilHeur::getAulasPossiveisDia(int dia, int nrCreditos, Calendario * const  calendario, int campusId, int unidadeId,
											std::vector<AulaHeur*>& result)
{
	if(nrCreditos == 0)
		return false;

	// verificar se já está registado
	vector<AulaHeur*> aulas;
	getAulasCalDiaCreds(calendario, dia, nrCreditos, aulas);
	if(aulas.size() > 0)
	{
		if(campusId < 0 || unidadeId < 0)
			result = aulas;
		else
			aulasCampusUnid(aulas, campusId, unidadeId, result);

		return true;
	}

	HeuristicaNuno::logMsgInt("get aulas possiveis dia... ", (int) dia, 3);
	gGroupHorarios horariosDia = calendario->retornaHorariosDisponiveisNoDia(dia);

	if(horariosDia.size() < nrCreditos)
		return false;
	
	bool got = false;
	got = getAulasPossiveisDiaHors(dia, nrCreditos, horariosDia, calendario->getId(), campusId, unidadeId, result);

	// registar
	if(got)
	{
		vector<AulaHeur*> aulasSemCampUnid;
		aulasCampusUnid(result, -1, -1, aulasSemCampUnid);

		auto itCal = aulasCalendario.find(calendario);
		if(itCal == aulasCalendario.end())
		{
			unordered_map<int, unordered_map<int, vector<AulaHeur*>>> emptyDay;
			aulasCalendario.insert(make_pair(calendario, emptyDay));
		}
		auto itDia = itCal->second.find(dia);
		if(itDia == itCal->second.end())
		{
			unordered_map<int, vector<AulaHeur*>> emptyCreds;
			itCal->second.insert(make_pair(dia, emptyCreds));
		}
		itDia->second[nrCreditos] = aulasSemCampUnid;
	}

	return got; 
}

// retorna as aulas com 'nrCreditos' possiveis num determinado dia do calendario
bool UtilHeur::getAulasPossiveis(TurnoIES * const turno, int calendarioId, std::vector<std::pair<int,int>> const &divisao, int campusId, 
										int unidadeId, unordered_map<int, std::vector<AulaHeur*>> &aulas)
{
	HeuristicaNuno::logMsg("get aulas possiveis...", 2);

	for(auto itComb = divisao.cbegin(); itComb != divisao.cend(); ++itComb)
	{
		int dia = itComb->first;
		int creditos = itComb->second;
		if(creditos == 0)
			continue;

		if(dia >= 9 || dia <= 1)
		{
			HeuristicaNuno::logMsgInt("dia: ", dia, 1);
			HeuristicaNuno::excepcao("UtilHeur::getAulasPossiveis_", "valor dia invalido!");
		}

		std::vector<AulaHeur*> aulasDia;
		if(!getAulasPossiveisDia(dia, creditos, turno, calendarioId, campusId, unidadeId, aulasDia))
			return false;

		// double check
		if(aulasDia.size() == 0)
			return false;

		std::pair<int, vector<AulaHeur*>> par (dia, aulasDia);
		if(!aulas.insert(par).second)
			HeuristicaNuno::excepcao("UtilHeur::getAulasPossiveis_", "Aulas dia nao inseridas!");
	}

	// check
	checkAulasPoss(divisao, aulas);

	return aulas.size() != 0;
}

// com base no calendário e na divisão selecciona as aulas possiveis
bool UtilHeur::getAulasPossiveisDia(int dia, int nrCreditos, TurnoIES * const turno, int calendarioId, int campusId, int unidadeId,
											std::vector<AulaHeur*>& result)
{
	if(nrCreditos == 0)
		return false;

	HeuristicaNuno::logMsgInt("get aulas possiveis dia... ", (int) dia, 3);
	gGroupHorarios horariosDia;
	turno->retornaHorariosDisponiveisNoDia(dia, horariosDia);

	if(horariosDia.size() < nrCreditos)
		return false;
	
	bool got = false;
	got = getAulasPossiveisDiaHors(dia, nrCreditos, horariosDia, calendarioId, campusId, unidadeId, result);

	return got; 
}

// retorna as aulas possiveis para um número de creditos e um conjunto de horarios
bool UtilHeur::getAulasPossiveisDiaHors(int dia, int nrCreditos, gGroupHorarios &horariosDia, int calendarioId, int campusId, int unidadeId, 
							std::vector<AulaHeur*>& result)
{
	if(horariosDia.size() < nrCreditos)
		return false;
	
	HeuristicaNuno::logMsgInt("numero de créditos: ", nrCreditos, 3);
	HeuristicaNuno::logMsgInt("numero de horários calendario/turno dia: ", (int)horariosDia.size(), 3);

	Calendario* calendario = CentroDados::getCalendario(calendarioId);

	for(auto itHor = horariosDia.begin(); itHor != horariosDia.end();)
	{
		set<HorarioAula*> horariosAula;
		horariosAula.insert(*itHor);
		HorarioAula* lastHor = *itHor;		// ultimo horario adicionado (para calcular intervalo)
		++itHor;
		for(auto itHor2 = itHor; itHor2 != horariosDia.end(); ++itHor2)
		{
			if(horariosAula.size() == nrCreditos)
				break;

			// verificar se há um intervalo grande no meio das aulas
			int inter = UtilHeur::intervaloHorarios(lastHor, *itHor2);
			// interseccção!
			if(inter < 0)
				continue;
			if((inter != 0) && (nrCreditos == 2))
				break;
			if(inter > ParametrosHeuristica::maxIntervAulas && 
				calendario->restringeAulaComIntervalo())
				break;

			horariosAula.insert(*itHor2);
			lastHor = *itHor2;
		}
		if(horariosAula.size() == nrCreditos)
		{
			AulaHeur aula (calendarioId, campusId, unidadeId, horariosAula);
			//UtilHeur::checkCreds(aula, nrCreditos, "AbridorTurmas::getAulasPossiveisDia_");

			AulaHeur* aulaPtr = UtilHeur::saveAula(aula);
			//UtilHeur::checkAulaId(aulaId, "AbridorTurmas::getAulasPossiveisDia_");
			result.push_back(aulaPtr); 
		}
	}

	HeuristicaNuno::logMsgInt("numero de aulas possiveis dia: ", result.size(), 3);

	return result.size() != 0;
}

// função auxiliar usada para conseguir obter as várias combinações de aulas possiveis para um calendário e uma divisão
bool UtilHeur::incrementArray(int arrayComb[9], const unordered_map<int, std::vector<AulaHeur*>> &possiveisAulas)
{
	for(auto itIdx = possiveisAulas.cbegin(); itIdx != possiveisAulas.cend(); itIdx++)
	{
		if(itIdx->second.size() == 0)
			continue;

		int dia = itIdx->first;
		if(dia >= 9 || dia <= 1)
		{
			HeuristicaNuno::logMsgInt("dia: ", dia, 1);
			HeuristicaNuno::excepcao("UtilHeur::incrementArray_", "Indice dia maior que array!");
		}

		int idx = arrayComb[dia];
		int size = itIdx->second.size();
		if(idx < (size - 1))
		{
			arrayComb[dia] = idx + 1;
			return true;
		}
		else
		{
			arrayComb[dia] = 0;
		}
	}
	return false;
}
// retorna um conjunto de aulas para um calendário e uma divisão
void UtilHeur::getAulasFromArray(int arrayComb[9], unordered_map<int, std::vector<AulaHeur*>> const &possiveisAulas, 
										unordered_map<int, AulaHeur*> &result)
{
	for(auto itAulas = possiveisAulas.cbegin(); itAulas != possiveisAulas.cend(); ++itAulas)
	{
		if(itAulas->second.size() == 0)
			continue;

		int dia = itAulas->first;
		if(dia >= 9 || dia <= 1)
		{
			HeuristicaNuno::logMsgInt("dia: ", dia, 1);
			HeuristicaNuno::excepcao("UtilHeur::getAulasFromArray_", "Indice dia maior que array!");
		}
		int idx = arrayComb[dia];
		int size = itAulas->second.size();
		if(idx >= size || idx < 0)
		{
			HeuristicaNuno::logMsgInt("idx: ", idx, 1);
			HeuristicaNuno::logMsgInt("size: ", size, 1);
			UtilHeur::printArray(arrayComb, 9, 1);
			HeuristicaNuno::excepcao("UtilHeur::getAulasFromArray_", "Aula nao encontrada. Indice invalido!");
		}

		result[dia] = itAulas->second[idx];
	}
}

// analisa todas as combinações de créditos e obtem o minimo de maximo de creditos por dia.
void UtilHeur::credsPossiveisDias(vector<vector<pair<int,int>>> const &divisoesCreditos, unordered_map<int, unordered_set<int> > &credsPossiveisDias)
{
	unordered_set<int> emptySet;
	for(auto it = divisoesCreditos.cbegin(); it != divisoesCreditos.cend(); ++it)
	{
		for(auto itDiv = it->cbegin(); itDiv != it->cend(); ++itDiv)
		{
			int dia = itDiv->first;
			int creds = itDiv->second;

			auto itDia = credsPossiveisDias.find(dia);
			if(itDia == credsPossiveisDias.end())
				itDia = credsPossiveisDias.insert(make_pair(dia, emptySet)).first;

			if(itDia->second.find(creds) == itDia->second.end())
				itDia->second.insert(creds);
		}
	}
}

// intersecta sets
void UtilHeur::intersectSetsAlunos(OfertaDisciplina* const ofertaDisc, unordered_set<AlunoHeur*> const &setUm, 
									unordered_set<AlunoHeur*> const &setDois,  unordered_set<AlunoHeur*> &result, 
									bool &temFormando, bool &temCoReq)
{
	result.clear();
	temFormando = false;
	temCoReq = false;

	int minSize = (setUm.size() < setDois.size()) ? setUm.size() : setDois.size();
	if(minSize == 0)
		return;

	for(auto it = setUm.cbegin(); it != setUm.cend(); ++it)
	{
		if(setDois.find(*it) != setDois.end())
		{
			result.insert(*it);
			if(!temFormando && (*it)->ehFormando())
				temFormando = true;
			if(!temCoReq && (*it)->temCoRequisito(ofertaDisc))
				temCoReq = true;
		}

		if(result.size() == minSize)
			return;
	}
}

// add aula. retorna id
AulaHeur* UtilHeur::addAula_(AulaHeur const &aula, size_t hash)
{
	int id = ++UtilHeur::idAulas;
	auto par = aulasHeurId_.insert(make_pair(id, aula));
	if(!par.second)
		HeuristicaNuno::excepcao("UtilHeur::saveAula", "Aula nao adicionada a aulasHeurId_!");

	hashesToIdAulasReg_[hash] = id;
	par.first->second.setId(id);

	return &par.first->second;
}

// print nr creditos e nr divisoes
void UtilHeur::printCredsNrDivs(Disciplina* const disciplina, int nrDiv, int lvl)
{
	stringstream ss;
	ss << "# creds: " << disciplina->getTotalCreditos() << " ; # divs: " << nrDiv;
	ss << " ; # cals: " << disciplina->calendariosReduzidos.size();
	HeuristicaNuno::logMsg(ss.str(), lvl);
}

// verifica se um calendario abrange outro, i.e. se tem todos os horarios do outro (inicio e fim igual)
bool UtilHeur::calendarioDomina(Calendario* const calendario, Calendario* const dominado)
{
	if(calendario->mapDiaDateTime.size() < dominado->mapDiaDateTime.size())
		return false;

	int calId = calendario->getId();
	int domId = dominado->getId();
	if(calId == domId)
		return false;

	// verificar se já foi registado
	auto itCalReg = dominanciaCalendarios_.find(calId);
	if(itCalReg != dominanciaCalendarios_.end() && (itCalReg->second.find(domId) != itCalReg->second.end()))
		return true;

	// se o inverso já foi registado ignorar
	auto itDomReg = dominanciaCalendarios_.find(domId);
	if(itDomReg != dominanciaCalendarios_.end() && (itDomReg->second.find(calId) != itDomReg->second.end()))
		return false;
		
	if(calendario->mapDiaDateTime.size() == 0 || dominado->mapDiaDateTime.size() == 0)
		HeuristicaNuno::excepcao("UtilHeur::calendarioDomina", "calendario->mapDiaDateTime vazio!");

	// verificar horarios
	for(auto itDomDia = dominado->mapDiaDateTime.begin(); itDomDia != dominado->mapDiaDateTime.end(); ++itDomDia)
	{
		int dia = itDomDia->first;
		// calendario nao tem esse dia
		auto itCalDia = calendario->mapDiaDateTime.find(dia);
		if(itCalDia == calendario->mapDiaDateTime.end())
			return false;

		// menos horarios nesse dia que o suposto calendario dominado
		if(itCalDia->second.size() < itDomDia->second.size())
			return false;

		// agora para cada horario do calendario supostamente dominado, para esse dia
		for(auto itDomHor = itDomDia->second.begin(); itDomHor != itDomDia->second.end(); ++itDomHor)
		{
			auto itCalHor = itCalDia->second.find(itDomHor->first);
			if(itCalHor == itCalDia->second.end())
				return false;
			else if(!itCalHor->second->inicioFimIguais(itDomHor->second))
				return false;
		}
	}

	// registar dominancia
	if(itCalReg == dominanciaCalendarios_.end())
	{
		unordered_set<int> dominadosId;
		auto par = dominanciaCalendarios_.insert(make_pair(calId, dominadosId));
		if(par.second == false)
			HeuristicaNuno::excepcao("UtilHeur::calendarioDomina", "Dominancia nao inserida corretamente.");

		itCalReg = par.first;
	}
	itCalReg->second.insert(domId);

	return true;
}

// procura a demanda de um aluno quando ela não é encontrada em alunosDemandaTotal
AlunoDemanda* UtilHeur::findDemanda(Aluno* const aluno, Disciplina* const disciplina)
{
	// verificar se está cadastrada diretamente
	for(auto it = HeuristicaNuno::probData->alunosDemandaTotal.begin(); it != HeuristicaNuno::probData->alunosDemandaTotal.end(); ++it)
	{
		if(aluno->getAlunoId() != (*it)->getAlunoId())
			continue;

		if(disciplina->getId() != (*it)->demanda->getDisciplinaId())
			continue;

		return (*it);
	}

	// verificar equivalencias
	/*for(auto it = HeuristicaNuno::probData->alunosDemandaTotal.begin(); it != HeuristicaNuno::probData->alunosDemandaTotal.end(); ++it)
	{
		if(aluno->getAlunoId() != (*it)->getAlunoId())
			continue;

		if ( (*it)->demanda->disciplina->discEquivSubstitutas.find( disciplina )!=
			 (*it)->demanda->disciplina->discEquivSubstitutas.end() )
		{
			AlunoDemanda* const alDemTeor = (*it);
			int idOrig = alDemTeor->demanda->disciplina->getId();

			if ( disciplina->getId() > 0)
				return alDemTeor;
			else
			{
				AlunoDemanda* const alDemPrat = getAlunoDemanda( -idOrig );
				if ( alDemPrat!=NULL )
					return alDemPrat;
				else
					return alDemTeor;
			}
		}
	}*/

	return nullptr;
}

// (<0) -> nao abrange / (>0) -> abrange / (==0) unknown
int UtilHeur::checkRegAbrange(int id, int dia, HorarioAula* const horario,
								unordered_map<int, unordered_map<int, unordered_set<int>>> const &registoAbrange,
								unordered_map<int, unordered_map<int, unordered_set<int>>> const &registoNaoAbrange)
{
	// check se abrange
	auto itSim = registoAbrange.find(id);
	if(itSim != registoAbrange.end())
	{
		auto itDia = itSim->second.find(dia);
		if(itDia != itSim->second.end())
		{
			if(itDia->second.find(horario->getId()) != itDia->second.end())
				return 1;
		}
	}

	// check se nao abrange
	auto itNao = registoNaoAbrange.find(id);
	if(itNao != registoNaoAbrange.end())
	{
		auto itDia = itNao->second.find(dia);
		if(itDia != itNao->second.end())
		{
			if(itDia->second.find(horario->getId()) != itDia->second.end())
				return -1;
		}
	}

	return 0;
}

int UtilHeur::checkRegAbrange(int idCalend, int idTurno, int dia, HorarioAula* const horario,
				unordered_map<int, unordered_map<int, unordered_map<int, unordered_set<int>>>> const &registoAbrange,
				unordered_map<int, unordered_map<int, unordered_map<int, unordered_set<int>>>> const &registoNaoAbrange)
{
	// check se abrange
	auto itSim = registoAbrange.find(idCalend);
	if(itSim != registoAbrange.end())
	{
		auto itTurno = itSim->second.find(idTurno);
		if(itTurno != itSim->second.end())
		{
			auto itDia = itTurno->second.find(dia);
			if(itDia != itTurno->second.end())
			{
				if(itDia->second.find(horario->getId()) != itDia->second.end())
					return 1;
			}
		}
	}

	// check se nao abrange
	auto itNao = registoNaoAbrange.find(idCalend);
	if(itNao != registoNaoAbrange.end())
	{
		auto itTurno = itNao->second.find(idTurno);
		if(itTurno != itNao->second.end())
		{
			auto itDia = itTurno->second.find(dia);
			if(itDia != itTurno->second.end())
			{
				if(itDia->second.find(horario->getId()) != itDia->second.end())
					return -1;
			}
		}
	}

	return 0;
}

void UtilHeur::regAbrangencia(int id, int dia, HorarioAula* const horario, 
							unordered_map<int, unordered_map<int, unordered_set<int>>> &registo)
{
	auto itTurno = registo.find(id);
	if(itTurno == registo.end())
	{
		unordered_map<int, unordered_set<int>> emptyMap;
		itTurno = registo.insert(make_pair(id, emptyMap)).first; 
	}

	auto itDia = itTurno->second.find(dia);
	if(itDia == itTurno->second.end())
	{
		unordered_set<int> emptySet;
		itDia = itTurno->second.insert(make_pair(dia, emptySet)).first;
	}
	itDia->second.insert(horario->getId());
}

void UtilHeur::regAbrangencia(int idCalend, int idTurno, int dia, HorarioAula* const horario, 
				unordered_map<int, unordered_map<int, unordered_map<int, unordered_set<int>>>> &registo)
{
	auto itCalend = registo.find(idCalend);
	if(itCalend == registo.end())
	{
		unordered_map<int, unordered_map<int, unordered_set<int>>> emptyMap;
		itCalend = registo.insert(make_pair(idCalend, emptyMap)).first; 
	}

	auto itTurno = itCalend->second.find(idTurno);
	if(itTurno == itCalend->second.end())
	{
		unordered_map<int, unordered_set<int>> emptyMap;
		itTurno = itCalend->second.insert(make_pair(idTurno, emptyMap)).first; 
	}

	auto itDia = itTurno->second.find(dia);
	if(itDia == itTurno->second.end())
	{
		unordered_set<int> emptySet;
		itDia = itTurno->second.insert(make_pair(dia, emptySet)).first;
	}
	itDia->second.insert(horario->getId());
}