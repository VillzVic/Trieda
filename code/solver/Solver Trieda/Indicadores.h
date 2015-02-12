#ifndef _INDICADORES_
#define _INDICADORES_

#include <fstream>


/*
 *	Indicadores de qualidade da solução
 */
class Indicadores
{
public:
	Indicadores();
	~Indicadores(void);

	// classe abstrata
	virtual void foo(void) = 0;

	/*
	*	Seta o nome do arquivo de indicadores.
	*/	
	static void setIndicadorFileName (std::string fileName);
	static void setIndicadorFileName( std::string inputName, int id );

	/*
	*	Inicializa o arquivo de saída com eventual cabeçalho/identificacao.
	*/
	static bool firstTimePrintTaticoHeader();

	/*
	*	Inicializa o arquivo de saída com eventual cabeçalho/identificacao.
	*/
	static void printTaticoHeader();	
	static void printTaticoIndicadores( std::string rowName, int totalAtendsSemDivPT_P1, int totalAtendsSemDivPT_P2, int totalCreditos, 
		int totalCreditosPagos, int totalTurmas, int totalCusto, int totalReceita, std::string runtime );

	static void printEscolaHeader();
	static void printEscolaIndicadores(std::string rowName, double value, double gap, bool opt, std::string runtime);


	/*
	*	Retorna o nome do arquivo de saída
	*/			
	static std::string getFileName();
	
	/*
	*	Imprime no arquivo de saída uma mensagem contendo algum indicador
	*/	
	static void printIndicador( std::string str );

	/*
	*	Imprime no arquivo de saída uma mensagem e valor de indicador a ser concatenado na mensagem
	*/	
	static void printIndicador( std::string str, int value );

	/*
	*	Imprime no arquivo de saída uma mensagem e valor de indicador a ser concatenado na mensagem
	*/	
	static void printIndicador( std::string str, double value, int precision = 4 );

	/*
	*	Retorna um string separador.
	*   int size : indica o tamanho do separador. Separador minimo = 1, separador máximo = 4.
	*/
	static void printSeparator(int size=2);
	
private:

	static bool printTaticoHeader_;

	static std::string indicadorName_;
	
};

#endif
