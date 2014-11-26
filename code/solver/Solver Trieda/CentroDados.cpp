#include "CentroDados.h"
#include "ProblemData.h"
#include "AlunoDemanda.h"

ProblemData* CentroDados::problemData = nullptr;

bool CentroDados::READ_SOLUTION = false;
bool CentroDados::PRINT_LOGS = false;

std::ofstream* CentroDados::fOutTest = nullptr;
std::ofstream* CentroDados::fOutWarn = nullptr;
std::ofstream* CentroDados::fOutError = nullptr;

static CPUTimer timerTemp;
CPUTimer CentroDados::timer = timerTemp;

CentroDados::CentroDados(void)
{
}

CentroDados::~CentroDados(void)
{
}

// Get Problem data (lança excepção se nenhum estiver setado!)
ProblemData* CentroDados::getProblemData(void) 
{ 
	if(problemData == nullptr)
		throw "[EXC: CentroDado::getProblemData] ProblemData ainda nao foi carregado para o Centro de Dados!";

	return problemData; 
}

// Set Problem data (evitar que pointer seja mudado por engano)
void CentroDados::setProblemData(ProblemData* const probData)
{
	if(CentroDados::problemData != nullptr)
		clearProblemData();

	CentroDados::problemData = probData;
}

// libertar a memória de problemData e set nullptr
void CentroDados::clearProblemData(void)
{
	ProblemData* data = CentroDados::problemData;
	CentroDados::problemData = nullptr;
	delete data;
}

void CentroDados::openErrorFile()
{			
	if(CentroDados::fOutError)
	{
		delete CentroDados::fOutError;
		CentroDados::fOutError = nullptr;
	}

	CentroDados::fOutError = new ofstream("errors.txt",ios::app);
	(*CentroDados::fOutError) << "\n===========================================================================" << endl
		<< "Input " << problemData->getInputFileName() << " - id " << problemData->getInputId() << endl;
}

void CentroDados::openWarnFile()
{
	if(CentroDados::fOutWarn)
	{
		delete CentroDados::fOutWarn;
		CentroDados::fOutWarn = nullptr;
	}

	CentroDados::fOutWarn = new ofstream("warnings.txt",ios::app);
	(*CentroDados::fOutWarn) << "\n===========================================================================" << endl
		<< "Input " << problemData->getInputFileName() << " - id " << problemData->getInputId() << endl;
}

void CentroDados::openTestFile()
{
	if(CentroDados::fOutTest)
	{
		delete CentroDados::fOutTest;
		CentroDados::fOutTest = nullptr;
	}

	CentroDados::fOutTest = new ofstream("test.txt",ios::app);
	(*CentroDados::fOutTest) << "\n===========================================================================" << endl
		<< "Input " << problemData->getInputFileName() << " - id " << problemData->getInputId() << endl;
}

void CentroDados::closeFiles()
{
	closeTestFile();
	closeWarnFile();
	closeErrorFile();
}

void CentroDados::closeTestFile()
{
	if(CentroDados::fOutTest) 
	{
		(*CentroDados::fOutTest) << "===========================================================================" << endl;
		CentroDados::fOutTest->close();
		delete CentroDados::fOutTest;
		CentroDados::fOutTest = nullptr;
	}
}

void CentroDados::closeWarnFile()
{
	if(CentroDados::fOutWarn) 
	{
		(*CentroDados::fOutWarn) << "===========================================================================" << endl;
		CentroDados::fOutWarn->close();
		delete CentroDados::fOutWarn;
		CentroDados::fOutWarn = nullptr;
	}
}

void CentroDados::closeErrorFile()
{
	if(CentroDados::fOutError) 
	{
		(*CentroDados::fOutError) << "===========================================================================" << endl;
		CentroDados::fOutError->close();
		delete CentroDados::fOutError;
		CentroDados::fOutError = nullptr;
	}
}

// Print test message
void CentroDados::printTest( std::string method, std::string msg )
{
	if ( ! CentroDados::fOutTest )
		openTestFile();

	if ( CentroDados::fOutTest )
	{
		(*CentroDados::fOutTest) << endl << method << ":"
			<< endl << "\t" << msg << endl;
		CentroDados::fOutTest->flush();
	}
}

// Print warning message
void CentroDados::printWarning( std::string method, std::string msg )
{
	if ( ! CentroDados::fOutWarn )
		openWarnFile();

	if ( CentroDados::fOutWarn )
	{
		(*CentroDados::fOutWarn) << endl << method << ":"
			<< endl << "\t" << msg << endl;
		CentroDados::fOutWarn->flush();
	}
}
	
// Print error message
void CentroDados::printError( std::string method, std::string msg )
{
	if ( ! CentroDados::fOutError )
		openErrorFile();

	if ( CentroDados::fOutError )
	{
		(*CentroDados::fOutError) << endl << method << ":"
			<< endl << "\t" << msg << endl;
		CentroDados::fOutError->flush();
	}
}

void CentroDados::startTimer()
{
	CentroDados::timer.start();
}
double CentroDados::getLastRunTime()
{
	CentroDados::timer.stop();
	double dif = CentroDados::timer.getCronoCurrSecs();
	CentroDados::timer.start();
	return dif;
}
double CentroDados::stopTimer()
{
	CentroDados::timer.stop();
	return CentroDados::timer.getCronoCurrSecs();
}

// ------------------------------------------------------------------------------------------------------------
// [PROCURAR PTRS OBJETOS]

// campus
Campus* CentroDados::getCampus(int id)
{
	if(problemData == nullptr)
		throw "[EXC: CentroDado::getCampus] ProblemData ainda nao foi carregado para o Centro de Dados!";

	auto itFind = problemData->refCampus.find(id);
	if(itFind == problemData->refCampus.end())
		return nullptr;

	return itFind->second;
}
// unidade
Unidade* CentroDados::getUnidade(int id)
{
	if(problemData == nullptr)
		throw "[EXC: CentroDado::getUnidade] ProblemData ainda nao foi carregado para o Centro de Dados!";

	auto itFind = problemData->refUnidade.find(id);
	if(itFind == problemData->refUnidade.end())
		return nullptr;

	return itFind->second;
}
// sala
Sala* CentroDados::getSala(int id)
{
	if(problemData == nullptr)
		throw "[EXC: CentroDado::getSala] ProblemData ainda nao foi carregado para o Centro de Dados!";

	auto itFind = problemData->refSala.find(id);
	if(itFind == problemData->refSala.end())
		return nullptr;

	return itFind->second;
}
// turno
TurnoIES* CentroDados::getTurno(int id)
{
	if(problemData == nullptr)
		throw "[EXC: CentroDado::getTurno] ProblemData ainda nao foi carregado para o Centro de Dados!";

	auto itFind = problemData->refTurnos.find(id);
	if(itFind == problemData->refTurnos.end())
		return nullptr;

	return itFind->second;
}
// horario aula
HorarioAula* CentroDados::getHorarioAula(int id)
{
	if(problemData == nullptr)
		throw "[EXC: CentroDado::getHorarioAula] ProblemData ainda nao foi carregado para o Centro de Dados!";

	auto itFind = problemData->refHorarioAula.find(id);
	if(itFind == problemData->refHorarioAula.end())
		return nullptr;

	return itFind->second;
}
// professor [USAR refProfessores]
Professor* CentroDados::getProfessor(int id)
{
	if(problemData == nullptr)
		throw "[EXC: CentroDado::getProfessor] ProblemData ainda nao foi carregado para o Centro de Dados!";

	auto prof = problemData->findProfessor(id);
	if(prof == NULL)
		prof = nullptr;

	return prof;
}
// oferta
Oferta* CentroDados::getOferta(int id)
{
	if(problemData == nullptr)
		throw "[EXC: CentroDado::getOferta] ProblemData ainda nao foi carregado para o Centro de Dados!";

	auto itFind = problemData->refOfertas.find(id);
	if(itFind == problemData->refOfertas.end())
		return nullptr;

	return itFind->second;
}
// disciplina
Disciplina* CentroDados::getDisciplina(int id)
{
	if(problemData == nullptr)
		throw "[EXC: CentroDado::getDisciplina] ProblemData ainda nao foi carregado para o Centro de Dados!";

	auto itFind = problemData->refDisciplinas.find(id);
	if(itFind == problemData->refDisciplinas.end())
		return nullptr;

	return itFind->second;
}
// calendario
Calendario* CentroDados::getCalendario(int id)
{
	if(problemData == nullptr)
		throw "[EXC: CentroDado::getCalendario] ProblemData ainda nao foi carregado para o Centro de Dados!";

	auto itFind = problemData->calendarios.begin();
	for(; itFind != problemData->calendarios.end(); ++itFind)
	{
		if(itFind->getId() == id)
			break;
	}
	if(itFind == problemData->calendarios.end())
		return nullptr;

	return (*itFind);
}

// get aluno from aluno demanda id [USAR futuro ref alunosDemanda]
Aluno* CentroDados::getAlunoIdFromAlunoDemanda(int demandaId)
{
	if(problemData == nullptr)
		throw "[EXC: CentroDado::getAlunoIdFromAlunoDemanda] ProblemData ainda nao foi carregado para o Centro de Dados!";

	Aluno* aluno = nullptr;
	for(auto it = problemData->alunosDemandaTotal.begin(); it != problemData->alunosDemandaTotal.end(); ++it)
	{
		if(it->getId() == demandaId)
		{
			aluno = it->getAluno();
			break;
		}
	}

	return aluno;
}

// ------------------------------------------------------------------------------------------------------------
// [PROCURAR INFO]

DateTime CentroDados::getFimDaFase(int fase)
{
	if(problemData == nullptr)
		throw "[EXC: CentroDado::getFaseDoDia] ProblemData ainda nao foi carregado para o Centro de Dados!";

	return problemData->getFimDaFase(fase);
}

// fase do dia
int CentroDados::getFaseDoDia(DateTime dt)
{
	if(problemData == nullptr)
		throw "[EXC: CentroDado::getFaseDoDia] ProblemData ainda nao foi carregado para o Centro de Dados!";

	return problemData->getFaseDoDia(dt);
}