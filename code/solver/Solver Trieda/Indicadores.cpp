#include "Indicadores.h"
#include <sstream>
#include <iostream>
#include <iomanip>

using std::setw;
using std::string;
using std::stringstream;

// tatico columns sizes
int cs[] = { 27, 12, 15, 13, 10, 12, 10, 10, 10 };
int cse[] = { 27, 12, 15, 13, 13, 10, 12 };

std::string Indicadores::indicadorName_("");
bool Indicadores::printTaticoHeader_ = true;
bool Indicadores::printGoalsHeader_ = true;

Indicadores::Indicadores()
{
}

Indicadores::~Indicadores()
{
	
}

void Indicadores::setIndicadorFileName( std::string fileName )
{
	indicadorName_ = fileName;
}

void Indicadores::setIndicadorFileName( std::string inputName, int id )
{
	std::stringstream fileName;   
	fileName << "indicadores_" << inputName;
	if ( id!=0 ) fileName << "_id" << id;

	indicadorName_ = fileName.str();
}

std::string Indicadores::getFileName()
{
	std::stringstream name;
	name << indicadorName_.c_str()<< ".txt";
	return (name.str());
}

bool Indicadores::firstTimePrintTaticoHeader()
{
	return printTaticoHeader_;
}

void Indicadores::printTaticoHeader()
{
	printTaticoHeader_=false;

	std::stringstream header;
	header << std::left << setw(cs[0]) << "\n\nEtapa-Otimizacao"
		<< std::left << setw(cs[1]) << "AlDem P1+P2"
		<< std::left << setw(cs[1]) << "AlDem P1"
		<< std::left << setw(cs[1]) << "AlDem P2"
		<< std::left << setw(cs[2]) << "Creds x alunos"
		<< std::left << setw(cs[3]) << "Creds pagos"
		<< std::left << setw(cs[4]) << "Turmas"
		<< std::left << setw(cs[5]) << "Custo C"
		<< std::left << setw(cs[6]) << "Receita R"
		<< std::left << setw(cs[7]) << "C/R (%)"
		<< std::left << setw(cs[8]) << "Runtime\n\n";

	Indicadores::printIndicador( header.str() );
}

void Indicadores::printTaticoIndicadores( std::string rowName, int totalAtendsSemDivPT_P1, int totalAtendsSemDivPT_P2, int totalCreditos, 
		int totalCreditosPagos, int totalTurmas, int totalCusto, int totalReceita, std::string runtime )
{
	if ( firstTimePrintTaticoHeader() )
		printTaticoHeader();
	
	std::stringstream row;
	row << std::endl << std::left << setw(cs[0]) << rowName
		<< std::left << setw(cs[1]) << totalAtendsSemDivPT_P1 + totalAtendsSemDivPT_P2
		<< std::left << setw(cs[1]) << totalAtendsSemDivPT_P1
		<< std::left << setw(cs[1]) << totalAtendsSemDivPT_P2
		<< std::left << setw(cs[2]) << totalCreditos
		<< std::left << setw(cs[3]) << totalCreditosPagos
		<< std::left << setw(cs[4]) << totalTurmas
		<< std::left << setw(cs[5]) << totalCusto
		<< std::left << setw(cs[6]) << totalReceita
		<< std::left << setw(cs[7]) << ( totalReceita != 0 ? (double) 100*totalCusto/totalReceita : 0 )
		<< std::left << setw(cs[8]) << runtime;
	Indicadores::printIndicador( row.str() );
}

void Indicadores::printEscolaHeader()
{
	printTaticoHeader_=false;

	std::stringstream header;
	header << std::left << setw(cse[0]) << "\n\nPhase"
		<< std::left << setw(cse[1]) << "FO value"
		<< std::left << setw(cse[2]) << "Gap"
		<< std::left << setw(cse[3]) << "IsOptimal"
		<< std::left << setw(cse[4]) << "PercFixed"
		<< std::left << setw(cse[5]) << "Runtime"
		<< std::left << setw(cse[6]) << "StopCriteria\n\n";

	Indicadores::printIndicador( header.str() );
}

void Indicadores::printEscolaIndicadores(std::string rowName, double value, double gap, bool opt, 
	int percFixed, std::string runtime, std::string stopCriteria)
{
	if ( firstTimePrintTaticoHeader() )
		printEscolaHeader();
	
	std::stringstream row;
	row << std::endl << std::left << setw(cse[0]) << rowName
		<< std::left << setw(cse[1]) << value
		<< std::left << setw(cse[2]) << gap
		<< std::left << setw(cse[3]) << (opt? "true":"false")
		<< std::left << setw(cse[4]) << percFixed
		<< std::left << setw(cse[5]) << runtime
		<< std::left << setw(cse[6]) << stopCriteria;
	Indicadores::printIndicador( row.str() );
}


bool Indicadores::firstTimePrintGoalsHeader()
{
	return printGoalsHeader_;
}

void Indicadores::printEscolaGoalsHeader()
{
	printGoalsHeader_ = false;

	std::stringstream header;
	header << std::left << setw(cse[0]) << "\n\nPhase"
		<< std::left << setw(cse[1]) << "Atend"
		<< std::left << setw(cse[2]) << "Turmas"
		<< std::left << setw(cse[3]) << "Desloc"
		<< std::left << setw(cse[4]) << "Fases&Dias"
		<< std::left << setw(cse[5]) << "Gaps";

	Indicadores::printIndicador( header.str() );
}

void Indicadores::printEscolaGoalsIndicadores(std::string rowName, double atend, double turmas, double desloc, 
	double fasesDias, double gaps)
{
	if ( firstTimePrintGoalsHeader() )
		printEscolaGoalsHeader();
	
	std::stringstream row;
	row << std::endl << std::left << setw(cse[0]) << rowName
		<< std::left << setw(cse[1]) << atend
		<< std::left << setw(cse[2]) << turmas
		<< std::left << setw(cse[3]) << desloc
		<< std::left << setw(cse[4]) << fasesDias
		<< std::left << setw(cse[5]) << gaps;
	Indicadores::printIndicador( row.str() );
}


void Indicadores::printIndicador( std::string str )
{
	std::ofstream outFile(getFileName(), std::ios::app);
	if ( outFile )
	{
		outFile << str.c_str();
		outFile.flush();
		outFile.close();	
	}
	else
	{
		std::cout << "Error: Can't open output file " << getFileName() << std::endl;
		return;		
	}
}

void Indicadores::printIndicador( std::string str, int value )
{
	std::stringstream ssMsg;
	ssMsg << str << value;
	Indicadores::printIndicador( ssMsg.str() );
}

void Indicadores::printIndicador( std::string str, double value, int precision )
{
	std::stringstream ssMsg;
	ssMsg << str << std::fixed << std::setprecision( precision ) << value;
	Indicadores::printIndicador( ssMsg.str() );
}

void Indicadores::printSeparator(int size)
{
	if ( size <= 1 )
		printIndicador( "\n-----------" );
	else if ( size <= 2 )
		printIndicador( "\n------------------------------------" );
	else if ( size <= 3 )
		printIndicador( "\n\n-----------------------------------------------------------------------------------------------------------------------\n" );
	else
		printIndicador( "\n\n+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n" );
}