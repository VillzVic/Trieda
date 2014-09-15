#include "Indicadores.h"
#include <sstream>
#include <iostream>
#include <iomanip>

using std::setw;
using std::string;
using std::stringstream;

// tatico columns sizes
int cs[] = { 27, 12, 15, 13, 10, 12, 10, 10, 10 };

std::string Indicadores::indicadorName_("");
bool Indicadores::printTaticoHeader_ = true;

Indicadores::Indicadores()
{
}

Indicadores::~Indicadores()
{
	
}

void Indicadores::setIndicadorFileName( std::string fileName )
{
	indicadorName_ = fileName;

	Indicadores::printSeparator(4);
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

void Indicadores::printIndicador( std::string str )
{
	std::ofstream outFile(getFileName(), std::ios::app);
	if ( outFile )
	{
		outFile << str.c_str();
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