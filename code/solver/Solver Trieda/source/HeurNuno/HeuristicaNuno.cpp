#include "HeuristicaNuno.h"
#include "../ProblemData.h"
#include "DadosHeuristica.h"
#include "../Professor.h"
#include "../Disciplina.h"
#include "TurmaHeur.h"
#include "../Aluno.h"
#include "AlunoHeur.h"
#include "SolucaoHeur.h"
#include "../ProblemSolution.h"
#include "ParametrosHeuristica.h"
#include "../CentroDados.h"
#include <iostream>
#include <fstream>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>


int HeuristicaNuno::nrLinhasLog_ = 0;
char HeuristicaNuno::path [1024];
char HeuristicaNuno::instance [1024];
int HeuristicaNuno::inputId = 0;
unordered_map<int, unordered_map<int, unordered_set<TurnoIES*>>> HeuristicaNuno::indispExtraSalas;

ProblemData* HeuristicaNuno::probData = nullptr;
SolucaoHeur* HeuristicaNuno::solucaoHeur = nullptr;
SolucaoHeur* HeuristicaNuno::solutionLoaded = nullptr;
bool HeuristicaNuno::improveSolucao = false;
bool HeuristicaNuno::soRealocSalas = false;

// main log
std::ofstream* HeuristicaNuno::logFile_ = new std::ofstream("heuristica.log");

// logs falta de input
std::ofstream* HeuristicaNuno::logDiscSemDiv_ = new std::ofstream("disciplinas_sem_divisao.log");
unordered_map<int, unordered_set<bool>> HeuristicaNuno::discSemDiv_;
std::ofstream* HeuristicaNuno::logDiscSemSala_ = new std::ofstream("disciplinas_sem_sala.log");
unordered_map<int, unordered_set<bool>> HeuristicaNuno::discSemSala_;
std::ofstream* HeuristicaNuno::logCheckEquiv_ = new std::ofstream("prob_com_equivalencias.log");

// logs sugestões solução fixada
std::ofstream* HeuristicaNuno::logFecharTurmas_ = nullptr;
std::ofstream* HeuristicaNuno::logMudancaAlunos_ = nullptr;
std::ofstream* HeuristicaNuno::logNovaSala_ = nullptr;

// log para analise da ordem de resolucao dos atendimentos
std::ofstream* HeuristicaNuno::logOrdemOfertas_ = nullptr;


HeuristicaNuno::HeuristicaNuno(void)
{
}

HeuristicaNuno::~HeuristicaNuno(void)
{
}

// set input info
void HeuristicaNuno::setInputInfo(char* inpPath, char* instName, int inpId)
{
	strcpy(path, inpPath);
	strcpy(instance, instName);
	inputId = inpId;

	auto oldLog = HeuristicaNuno::logFile_;
	HeuristicaNuno::logFile_ = nullptr;
	delete oldLog;
	stringstream ss;
	ss << "heuristica_id" << inputId << ".log";
	HeuristicaNuno::logFile_ = new std::ofstream(ss.str());
}
// Setup do problema
void HeuristicaNuno::setup(ProblemData* const problemData)
{
	HeuristicaNuno::probData = problemData;

	// Setup parâmetros
	ParametrosHeuristica::setup(problemData);

	// Preparar os dados da heurística
	DadosHeuristica::prepararDados();

	closeLog_(logCheckEquiv_);

	// rand
	time_t t = time(0);   // get time now
    struct tm * now = localtime( & t );
	srand(now->tm_sec);
}
// Run
void HeuristicaNuno::run(void)
{
	logMsg("", 1);
	HeuristicaNuno::logMsg("-----------%%%%%%%%% HEURISTICA %%%%%%-----------", 1);
	logMsg("", 1);

	// nenhuma solução fixada carregada. gerar solução inicial
	if(solutionLoaded == nullptr)
	{
		HeuristicaNuno::runNewSolucao_();
	}
	// tentar melhorar a partir da solucao já carregada
	else 
	{
		if(ParametrosHeuristica::initSolLoadMode == ParametrosHeuristica::LoadInitSolAndImprove)
			HeuristicaNuno::runImprovSolucao_();
		else if(ParametrosHeuristica::initSolLoadMode == ParametrosHeuristica::LoadInitSolAndComplete)
			HeuristicaNuno::runCompleteSolucao_();
	}

	// Teste: destruir parte da solução para testar load!
	if(ParametrosHeuristica::finalDestroy)
		solucaoHeur->desalocarAlunosRandom(0.30);

	logMsg("Heurística terminada!", 1);
}
// run solução do zero
void HeuristicaNuno::runNewSolucao_(void)
{
	logMsg("", 1);
	logMsg(">>> Solucao inicial nao encontrada. Gerar uma solução do zero <<<", 1);
	HeuristicaNuno::improveSolucao = false;
	clock_t begin = clock();
	int it = 0;
	do
	{
		++it;
		logMsgInt(">> GERAR SOLUCAO NR. ", it, 1);
		SolucaoHeur* const sol = SolucaoHeur::gerarSolucaoInicial();
		// primeira
		if(HeuristicaNuno::solucaoHeur == nullptr)
		{
			HeuristicaNuno::solucaoHeur = sol;
		}
		else if(sol->ehMelhor(HeuristicaNuno::solucaoHeur))
		{
			SolucaoHeur* formSol = HeuristicaNuno::solucaoHeur;
			HeuristicaNuno::solucaoHeur = sol;
			delete formSol;
		}
		else
			delete sol;
	}
	while(it < HeuristicaNuno::limIterHeur && ((double(clock() - begin) / CLOCKS_PER_MINUTE) < HeuristicaNuno::limMinHeur));
}
// run improve solução
void HeuristicaNuno::runImprovSolucao_(void)
{
	HeuristicaNuno::improveSolucao = true;
	abrirLogsSugestao_();	// abrir os logs para registar sugestão de alterações

	// verificar se tem conflitos
	if(!solutionLoaded->checkConflitos())
		excepcao("HeuristicaNuno::run", "Solução carregada tem conflitos!");

	logMsg(">>> Solucao inicial encontrada. Tentar melhora-la respeitando regras de alteracao <<<", 1);

	if(!soRealocSalas)
	{
		// Atenção: solutionLoaded modificada!
		SolucaoHeur::improveSolucaoFixada(solutionLoaded);
		solucaoHeur = solutionLoaded;
	}
	// só realocar salas!
	else
	{
		SolucaoHeur::realocSalasSolucaoFix(solutionLoaded);
		solucaoHeur = solutionLoaded;
	}
}
// run solução do zero
void HeuristicaNuno::runCompleteSolucao_(void)
{
	// verificar se tem conflitos
	if(!HeuristicaNuno::solutionLoaded)
		excepcao("HeuristicaNuno::run", "Solução carregada eh null!");
	else if(!HeuristicaNuno::solutionLoaded->checkConflitos())
		excepcao("HeuristicaNuno::run", "Solução carregada tem conflitos!");

	HeuristicaNuno::solucaoHeur = HeuristicaNuno::solutionLoaded;
	ProblemSolution * const partialSol = HeuristicaNuno::solutionLoaded->getProblemSolution();

	logMsg("", 1);
	logMsg(">>> Solucao inicial carregada. Gerar uma solução completa com o atendimento inicial fixado <<<", 1);
	HeuristicaNuno::improveSolucao = false;
	clock_t begin = clock();
	int it = 0;

	do
	{
		++it;
		logMsgInt(">> GERAR SOLUCAO NR. ", it, 1);
		SolucaoHeur* const sol = SolucaoHeur::gerarSolucaoInicial(partialSol);

		if(sol->ehMelhor(HeuristicaNuno::solucaoHeur))
		{
			SolucaoHeur* formSol = HeuristicaNuno::solucaoHeur;
			HeuristicaNuno::solucaoHeur = sol;
			delete formSol;
		}
		else
			delete sol;
	}
	while(it < HeuristicaNuno::limIterHeur && ((double(clock() - begin) / CLOCKS_PER_MINUTE) < HeuristicaNuno::limMinHeur));

	delete partialSol;
}


// Clean memory
void HeuristicaNuno::clean(void)
{
	// Solução primeiro que Dados Heurística!!!
	cout << "limpar solucao heur" << endl;
	delete solucaoHeur;
	solucaoHeur = nullptr;

	cout << "limpar prof virtual" << endl;
	ParametrosHeuristica::clean();

	cout << "fechar logs" << endl;
	closeLog_(logFile_);
	closeLog_(logDiscSemDiv_);
	closeLog_(logDiscSemSala_);
	
	// log solução fixada
	closeLog_(logFecharTurmas_);
	closeLog_(logMudancaAlunos_);
	closeLog_(logNovaSala_);

	closeLog_(logOrdemOfertas_);
}

// load solução
void HeuristicaNuno::loadSolucao(ProblemSolution* const probSolution)
{
	logMsg(">>> Carregar solucao inicial para a heurística...", 1);
	HeuristicaNuno::solutionLoaded = SolucaoHeur::carregarSolucao(probSolution);
	logMsg("solucao carregada!", 1);
	HeuristicaNuno::solutionLoaded->printStats();

	HeuristicaNuno::improveSolucao = true;
}
// output solucao
void HeuristicaNuno::outputSolucao(SolucaoHeur* const solucao, char* append)
{
	ProblemSolution* solution = solucao->getProblemSolution();

	// temporary output
	char tempOutput[ 1024 ];
	tempOutput[0] = '\0';
	strcat( tempOutput, path );
	strcat( tempOutput, "partialSolution_" );
	strcat( tempOutput, append );
	strcat( tempOutput, ".xml" );
	std::string tempOutputFile = tempOutput;

	// Output file name
	char outputFile[ 1024 ];
	outputFile[0] = '\0';
	strcat( outputFile, path );
	strcat( outputFile, "output" );
	strcat( outputFile, instance );
	strcat( outputFile, "_" );
	strcat( outputFile, append );
	// incluir parametro
	char sufixLimCred[1024];
	sufixLimCred[0] = '\0';
	std::sprintf(sufixLimCred, "%2.1f", ParametrosHeuristica::minProdutividadeCred);
	strcat( outputFile, "_lim_" );
	strcat( outputFile, sufixLimCred);
	// incluir id se existir
	if(inputId != 0)
	{
		stringstream ss;
		ss << "_id" << inputId;
		strcat( outputFile, ss.str().c_str());
	}

	// write output
	writeOutput(solution, outputFile, tempOutput);
}

// get solução
ProblemSolution* HeuristicaNuno::getSolucao(void)
{
	if(solucaoHeur == nullptr)
		excepcao("HeuristicaNuno::getSolucao", "Nenhuma solucaoo encontrada!");

	return solucaoHeur->getProblemSolution();
}

// limpar logs

// Log
void HeuristicaNuno::logMsg(std::string msg, int lvl)
{
	if(lvl > logLevel_ && lvl > consoleLogLevel_)
		return;

	if(cutLog_ && nrLinhasLog_ == cutRows_)
	{
		// destruir antigo
		closeLog_(logFile_);

		// criar novo igual
		logFile_ = new std::ofstream("heuristica.log");
		nrLinhasLog_ = 0;
	}

	std::stringstream ss;
	ss << "[" << UtilHeur::currentDateTime() << "] " << msg << std::endl;
	std::string logmsg = ss.str();

	// log no text
	if(lvl <= logLevel_)
	{
		(*logFile_) << logmsg;
		logFile_->flush();
	}

	// log na consola
	if(lvl <= consoleLogLevel_)
		std::cout << logmsg;
	
	nrLinhasLog_++;
}
void HeuristicaNuno::logMsgInt(std::string msg, int nr, int lvl)
{
	if(lvl > logLevel_ && lvl > consoleLogLevel_)
		return;

	std::stringstream sstm;
	sstm << msg << nr;
	logMsg(sstm.str(), lvl);
}

void HeuristicaNuno::logMsgLong(std::string msg, long nr, int lvl)
{
	if(lvl > logLevel_ && lvl > consoleLogLevel_)
		return;

	std::stringstream sstm;
	sstm << msg << nr;
	logMsg(sstm.str(), lvl);
}
void HeuristicaNuno::logMsgDouble(std::string msg, double nr, int lvl)
{
	if(lvl > logLevel_ && lvl > consoleLogLevel_)
		return;

	std::stringstream sstm;
	sstm << msg << nr;
	logMsg(sstm.str(), lvl);
}
void HeuristicaNuno::logHorario(std::string msg, HorarioAula const &horario, int lvl)
{
	if(lvl > logLevel_ && lvl > consoleLogLevel_)
		return;

	std::string horarioStr = UtilHeur::horarioAulaToString(horario);
	std::stringstream ss;
	ss << msg << horarioStr;
	HeuristicaNuno::logMsg(ss.str(), lvl);
}

void HeuristicaNuno::closeLog_(std::ofstream* logFile)
{
	if(logFile == nullptr)
		return;

	logFile->close();
	std::ofstream* ptr = logFile;
	logFile = nullptr;
	delete ptr;
}
void HeuristicaNuno::excepcao(std::string metodo, std::string msg)
{
	std::stringstream ss;
	ss << "[EXC: " << metodo << "] " << msg;
	logMsg(ss.str(), 0);
	throw msg;
}
void HeuristicaNuno::warning(std::string metodo, std::string msg)
{
	std::stringstream ss;
	ss << "[WARN: " << metodo << "] " << msg;
	logMsg(ss.str(), 0);
}

// relatórios erros
void HeuristicaNuno::logDiscSemDivisao(int id, bool teorico, bool compSec)
{
	int posId = std::abs(id);

	// ver se já foi registado
	auto itDisc = discSemDiv_.find(posId);
	if(itDisc == discSemDiv_.end())
	{
		unordered_set<bool> setComps;
		itDisc = discSemDiv_.insert(make_pair(posId, setComps)).first;
	}
	auto itComp = itDisc->second.find(teorico);
	if(itComp == itDisc->second.end())
		itDisc->second.insert(teorico);
	else
		return;	// já impresso

	std::stringstream ss;
	ss << "Disc id: " << posId;
	if(teorico)
		ss << " [T]";
	else if(!compSec)
		ss << " [P]";
	else
		ss << " [parte P]";

	ss << endl;

	(*logDiscSemDiv_) << ss.str();
	logDiscSemDiv_->flush();
}
void HeuristicaNuno::logDiscSemSala(int id, bool teorico, bool compSec)
{
	int posId = std::abs(id);

	// ver se já foi registado
	auto itDisc = discSemSala_.find(posId);
	if(itDisc == discSemSala_.end())
	{
		unordered_set<bool> setComps;
		itDisc = discSemSala_.insert(make_pair(posId, setComps)).first;
	}
	auto itComp = itDisc->second.find(teorico);
	if(itComp == itDisc->second.end())
		itDisc->second.insert(teorico);
	else
		return;	// já impresso

	std::stringstream ss;
	ss << "Disc id: " << posId;
	if(teorico)
		ss << " [T]";
	else if(!compSec)
		ss << " [P]";
	else
		ss << " [parte P]";

	ss << endl;

	(*logDiscSemSala_) << ss.str();
	logDiscSemSala_->flush();
}
// check equivalencias
void HeuristicaNuno::logEquivMultiOrig(int alunoId, int equiv, unordered_set<int> origs)
{
	stringstream ss;
	ss << "[MULTI-ORIG] O Aluno " << alunoId << " tem demanda P1 pelas Disciplinas ";
	for(auto it = origs.begin(); it != origs.end(); ++it)
		ss << (*it) << ", ";

	ss << "que têm como equivalente a Disciplina " << equiv << endl;

	(*logCheckEquiv_) << ss.str();
	logCheckEquiv_->flush();
}
void HeuristicaNuno::logDemandasEquiv(int alunoId, int dem, int equiv)
{
	stringstream ss;
	ss << "[DEM-EQUIV] O Aluno " << alunoId << " tem demanda P1 pelas Disciplinas " << dem << " e " << equiv;
	ss << " que são equivalentes" << endl;

	(*logCheckEquiv_) << ss.str();
	logCheckEquiv_->flush();
}

// write output
void HeuristicaNuno::writeOutput( ProblemSolution * solution, char * outputFile, char * tempOutput )
{
	// Write output
   remove( tempOutput );
   std::ofstream file;
   file.open( tempOutput );
   file << ( *solution );
   file.flush();
   file.close();

   remove( outputFile );
   rename( tempOutput, outputFile );
}

// abrir logs de sugestão de mudanças
void HeuristicaNuno::abrirLogsSugestao_(void)
{
	// logFecharTurmas
	if(HeuristicaNuno::logFecharTurmas_ != nullptr)
	{
		delete HeuristicaNuno::logFecharTurmas_;
		HeuristicaNuno::logFecharTurmas_ = nullptr;
	}

	HeuristicaNuno::logFecharTurmas_ = new ofstream("fechar_turmas.csv");
	(*HeuristicaNuno::logFecharTurmas_) << "Campus;Disciplina;Turma_ID;Tipo;ID_Turma_Unico;Nr_Alunos_Inicial" << endl;

	// logMudancaAlunos
	if(HeuristicaNuno::logMudancaAlunos_ != nullptr)
	{
		delete HeuristicaNuno::logMudancaAlunos_;
		HeuristicaNuno::logMudancaAlunos_ = nullptr;
	}
	HeuristicaNuno::logMudancaAlunos_ = new ofstream("mudar_alunos.csv");
	(*HeuristicaNuno::logMudancaAlunos_) << "Aluno_ID; Aluno_Nome;Turma_Antiga;Campus_Nova_Turma;Disc_Nova_Turma;ID_Nova_Turma" << endl;

	// logNovaSala
	if(HeuristicaNuno::logNovaSala_ != nullptr)
	{
		delete HeuristicaNuno::logNovaSala_;
		HeuristicaNuno::logNovaSala_ = nullptr;
	}
	HeuristicaNuno::logNovaSala_ = new ofstream("mudar_salas.csv");
	(*HeuristicaNuno::logNovaSala_) << "Campus;Disciplina;Turma_ID;Tipo;Sala_Anterior;Capacidade;Lab;Nova_Sala;Capacidade;Lab" << endl;
}

// relatorios mudanças em solução fixada
void HeuristicaNuno::logFecharTurmaLoad(const SolucaoHeur* const solucao, TurmaHeur* const turma)
{
	if(!turma->carregada)
	{
		warning("HeuristicaNuno::logFecharTurma", "Turma nao foi carregada!");
		return;
	}

	if((logFecharTurmas_ == nullptr) || (logMudancaAlunos_ == nullptr))
	{
		HeuristicaNuno::warning("HeuristicaNuno::logFecharTurma", "Logs nao abertos!");
		return;
	}

	// logFecharTurmas [Campus;Disciplina;Turma_ID;Tipo;ID_Turma_Unico;Nr_Alunos_Inicial]
	stringstream ss;
	ss << turma->ofertaDisc->getCampus()->getCodigo() << ";";      // Campus
	ss << turma->ofertaDisc->getDisciplina()->getCodigo() << ";";  // Disciplina
	ss << turma->id << ";";
	if(turma->ehTeoricaTag())
		ss << "Teorica;";
	else
		ss << "Pratica;";
	
	// id unico
	ss << "T" << turma->getGlobalId() << ";";
	// nr alunos inicial
	ss << turma->getNrAlunosFix();
	ss << endl;

	(*logFecharTurmas_) << ss.str();
	logFecharTurmas_->flush();


	// log mudanças de alunos
	unordered_set<int> alunosFix;
	turma->getAlunosFix(alunosFix);
	for(auto it = alunosFix.cbegin(); it != alunosFix.cend(); ++it)
	{
		logMudancaAlunoFix(solucao, *it, turma);
	}
}
void HeuristicaNuno::logMudancaAlunoFix(const SolucaoHeur* const solucao, int alunoId, TurmaHeur* const oldTurma)
{
	auto itAluno = solucao->alunosHeur.find(alunoId);
	if(itAluno == solucao->alunosHeur.end())
		excepcao("HeuristicaNuno::logMudancaAluno", "Aluno nao encontrado!");

	AlunoHeur* const alunoHeur = itAluno->second;
	Aluno* const aluno = alunoHeur->getAluno();

	// logMudancaAluno [Aluno_ID; Aluno_Nome;ID_Turma_Antiga;Campus_Nova_Turma;Disc_Nova_Turma;ID_Nova_Turma]
	stringstream ss;
	ss << aluno->getAlunoId() << ";";
	ss << aluno->getNomeAluno() << ";";
	ss << "T" << oldTurma->getGlobalId() << ";";

	// get nova turma da mesma oferta ou equivalente
	TurmaHeur* newTurma = nullptr;
	alunoHeur->getTurmaTipoEquiv(oldTurma->ofertaDisc, oldTurma->ehTeoricaTag(), newTurma);
	if(newTurma != nullptr)
	{
		ss << newTurma->ofertaDisc->getCampus()->getCodigo() << ";";
		ss << newTurma->ofertaDisc->getDisciplina()->getCodigo() << ";";
		ss << newTurma->id << ";";
	}
	else
	{
		ss << ";;;";
	}
	//else
	//	warning("HeuristicaNuno::logMudancaAluno", "Aluno nao alocado em turma nova da mesma disciplina (equiv)!!");

	ss << endl;
	(*HeuristicaNuno::logMudancaAlunos_) << ss.str();
}
void HeuristicaNuno::logNovaSala(TurmaHeur* const turma, SalaHeur* const oldSala, SalaHeur* const newSala)
{
	if(!turma->carregada)
	{
		warning("HeuristicaNuno::logFecharTurma", "Turma nao foi carregada!");
		return;
	}

	if(logNovaSala_ == nullptr)
	{
		HeuristicaNuno::warning("HeuristicaNuno::logNovaSala", "Log nao aberto!");
		return;
	}

	// Campus;Disciplina;Turma_ID;Tipo;Sala_Anterior;Capacidade;Nova_Sala;Capacidade
	stringstream ss;
	ss << turma->ofertaDisc->getCampus()->getCodigo() << ";";      // Campus
	ss << turma->ofertaDisc->getDisciplina()->getCodigo() << ";";  // Disciplina
	ss << turma->id << ";";
	if(turma->ehTeoricaTag())
		ss << "Teorica;";
	else
		ss << "Pratica;";
	ss << oldSala->getSala()->getCodigo() << ";";
	ss << oldSala->getCapacidade() << ";";
	ss << oldSala->ehLab() << ";";
	ss << newSala->getSala()->getCodigo() << ";";
	ss << newSala->getCapacidade() << ";";
	ss << newSala->ehLab();
	ss << endl;

	(*logNovaSala_) << ss.str();
	logNovaSala_->flush();
}

// load indisponibilidades extra das salas
void HeuristicaNuno::loadIndispExtraSalas(char* const fullPath)
{
	HeuristicaNuno::logMsg(">> Tentar carregar indisponibilidades extra das salas", 1);
	std::ifstream file (fullPath);
	if(!file.is_open())
	{
		HeuristicaNuno::warning("HeuristicaNuno::loadIndispExtraSalas", "Nao foi possible abrir CSV com indisponibilidades extra das salas!");
		return;
	}

	// limpar
	indispExtraSalas.clear();

	string line;
	vector<string> strs;
	// get primeira linha com os titulos
	getline(file,line);

	while(getline(file,line))
	{
		boost::split(strs, line, boost::is_any_of(";,"));
		if(strs.size() != 3)
			continue;

		int salaId = std::atoi(strs.at(0).c_str());
		int dia = std::atoi(strs.at(1).c_str());
		int turnoId = std::atoi(strs.at(2).c_str());

		// check sala
		Sala* sala = nullptr;
		sala = CentroDados::getSala(salaId);
		if(sala == nullptr)
		{
			stringstream swarn;
			swarn << "Sala id " << salaId << " nao encontrada!";
			HeuristicaNuno::warning("HeuristicaNuno::loadIndispExtraSalas", swarn.str());
			continue;
		}

		// check dia da semana
		if(dia < 2 || dia > 8)
		{
			stringstream swarn;
			swarn << "Dia " << dia << " fora dos limites! [Seg = 2; Dom = 8]";
			HeuristicaNuno::warning("HeuristicaNuno::loadIndispExtraSalas", swarn.str());
			continue;
		}

		// get turno
		TurnoIES* turno = nullptr;
		turno = CentroDados::getTurno(turnoId);
		if(turno == nullptr)
		{
			stringstream swarn;
			swarn << "Turno id " << turnoId << " nao encontrado!";
			HeuristicaNuno::warning("HeuristicaNuno::loadIndispExtraSalas", swarn.str());
			continue;
		}

		// check print
		/*stringstream ss;
		ss << salaId << ";" << dia << ";" << turnoId;
		HeuristicaNuno::logMsg(ss.str(), 1);*/

		// registar
		auto itSala = indispExtraSalas.find(salaId);
		if(itSala == indispExtraSalas.end())
		{
			unordered_map<int, unordered_set<TurnoIES*>> emptyMapDia;
			auto par = indispExtraSalas.insert(make_pair(salaId, emptyMapDia));
			if(!par.second)
			{
				HeuristicaNuno::warning("HeuristicaNuno::loadIndispExtraSalas", "Map indisp da sala nao inserido");
				continue;
			}
			itSala = par.first;
		}

		auto itDia = itSala->second.find(dia);
		if(itDia == itSala->second.end())
		{
			unordered_set<TurnoIES*> setTurnos;
			auto par = itSala->second.insert(make_pair(dia, setTurnos));
			if(!par.second)
			{
				HeuristicaNuno::warning("HeuristicaNuno::loadIndispExtraSalas", "Map indisp dia da sala nao inserido");
				continue;
			}
			itDia = par.first;
		}

		itDia->second.insert(turno);
	}
}

// log de ordenação das ofertas-disciplinas
void HeuristicaNuno::logOrdemOfertas( std::string msg )
{
	if(HeuristicaNuno::logOrdemOfertas_ == nullptr)
	{
		HeuristicaNuno::logOrdemOfertas_ = new ofstream("logOrdemOfertas_.log");
	}	

	(*logOrdemOfertas_) << msg;
	logOrdemOfertas_->flush();
}