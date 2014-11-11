#include <iomanip>

#include "Operacional.h"
#include "AlocacaoProfVirtual.h"
#include "AtendimentoHorarioAula.h"
#include "AtendimentoTurno.h"
#include "Indicadores.h"
#include "NaoAtendimento.h"
#include "CentroDados.h"

#include "CPUTimerWin.h"


#define PRINT_cria_variaveis
#define PRINT_cria_restricoes


//bool ordenaAtendimentosBaseHorarioAula(
//   AtendimentoBase at1 , AtendimentoBase at2 )
//{
//   if ( at1.horario_aula == NULL
//      || at2.horario_aula == NULL )
//   {
//      return false;
//   }
//
//   if ( at1.horario_aula->getInicio() < at2.horario_aula->getInicio() ) return true;
//   if ( at1.horario_aula->getInicio() > at2.horario_aula->getInicio() ) return false;
//
//   return false;
//}




Operacional::Operacional( ProblemData * aProblemData, bool *endCARREGA_SOLUCAO, 
						  GGroup< VariableOp *, LessPtr<VariableOp> > * asolVarsOp,
						  ProblemSolution *aproblemSolution )
				: Solver( aProblemData )
{   
   CARREGA_SOLUCAO = endCARREGA_SOLUCAO;
   
   solVarsOp = asolVarsOp;
   problemSolution = aproblemSolution;

   FIXA_HOR_SOL_TATICO = true;

   try
   {
#ifdef SOLVER_CPLEX
	   lp = new OPT_CPLEX; 
#endif
#ifdef SOLVER_GUROBI
	   lp = new OPT_GUROBI; 
#endif
   }
   catch(...)
   {
   }
}

Operacional::~Operacional(void)
{
   if ( lp != NULL )
   {
      delete lp;
   } 
}

int Operacional::solve(){return 1;}

std::string Operacional::getOpLpFileName( int etapa )
{
	std::string lpName;
	lpName += "SolverOperacional";
	lpName += problemData->inputIdToString();
	 
   if ( this->getRodada() == OP_VIRTUAL_PERFIL )
   {	   
		stringstream ss;
		ss << "_PV"; 
		lpName += ss.str();
   }  
   if ( etapa >= 0 )
   {	   
		 stringstream ss;
		 ss << etapa;
		 lpName += "_"; 
		 lpName += ss.str();
   }
      
   return lpName;
}

std::string Operacional::getSolOpBinFileName( int etapa, int fase )
{
	std::string solName;
	solName += "solOp";
	solName +=problemData->inputIdToString();

   if ( this->getRodada() == OP_VIRTUAL_PERFIL )
   {	   
		stringstream ss;
		ss << "_PV"; 
		solName += ss.str();
   }
   if ( etapa >= 0 )
   {	   
		 stringstream ss;
		 ss << etapa;
		 solName += "_"; 
		 solName += ss.str();
   }
   if ( fase > 0 )
   {	   
		 stringstream ss;
		 ss << fase;
		 solName += "_fase"; 
		 solName += ss.str();
   }
   solName += ".bin";
      
   return solName;
}

std::string Operacional::getSolucaoOpFileName( int etapa, int fase )
{
	std::string solName;
	solName += "solucaoOperacional";
	solName +=problemData->inputIdToString();

   if ( this->getRodada() == OP_VIRTUAL_PERFIL )
   {	   
		stringstream ss;
		ss << "_PV"; 
		solName += ss.str();
   }
   if ( etapa >= 0 )
   {	   
		 stringstream ss;
		 ss << etapa;
		 solName += "_"; 
		 solName += ss.str();
   }
   if ( fase > 0 )
   {	   
		 stringstream ss;
		 ss << fase;
		 solName += "_fase"; 
		 solName += ss.str();
   }

   solName += ".txt";
      
   return solName;
}

void Operacional::writeOpSolBin( int type, double *xSol, int fase )
{
	#ifndef PRINT_LOGS
		return;
	#endif

	char solName[1024]="\0";

	switch (type)
	{
		case (OP_BIN):
			strcpy( solName, getSolOpBinFileName( -1, fase ).c_str() );
			break;
		case (OP_BIN0):
			strcat( solName, getSolOpBinFileName( 0, fase ).c_str() );
			break;
		case (OP_BIN1):
			strcat( solName, getSolOpBinFileName( 1, fase ).c_str() );
			break;
		case (OP_BIN2):
			strcat( solName, getSolOpBinFileName( 2, fase ).c_str() );
			break;
		case (OP_BIN3):
			strcat( solName, getSolOpBinFileName( 3, fase ).c_str() );
			break;
	}

	// WRITES SOLUTION
		
	FILE * fout = fopen( solName, "wb" );
	if ( fout == NULL )
	{
		std::cout << "\nErro em SolverMIP::writeOpSolBin( int etapa, int type, double *xSol ):"
				<< "\nArquivo " << solName << " nao pode ser aberto.\n";
	}
	else
	{
		int nCols = lp->getNumCols();

		fwrite( &nCols, sizeof( int ), 1, fout );
		for ( int i = 0; i < lp->getNumCols(); i++ )
		{
			fwrite( &( xSol[ i ] ), sizeof( double ), 1, fout );
		}

		fclose( fout );
	}
}

int Operacional::readOpSolBin( int type, double *xSol, int fase )
{
	char solName[1024]="\0";

	switch (type)
	{
		case (OP_BIN):
			strcpy( solName, getSolOpBinFileName( -1, fase ).c_str() );
			break;
		case (OP_BIN0):
			strcat( solName, getSolOpBinFileName( 0, fase ).c_str() );
			break;
		case (OP_BIN1):
			strcat( solName, getSolOpBinFileName( 1, fase ).c_str() );
			break;
		case (OP_BIN2):
			strcat( solName, getSolOpBinFileName( 2, fase ).c_str() );
			break;
		case (OP_BIN3):
			strcat( solName, getSolOpBinFileName( 3, fase ).c_str() );
			break;
	}

	// READS THE SOLUTION
		
	cout<<"====================> carregando solucao " <<solName <<endl; fflush(NULL);
	FILE* fin = fopen( solName,"rb");

	if ( fin == NULL )
	{
		std::cout << "<============ Arquivo " << solName << " nao encontrado. Fim do carregamento de solucao.\n\n"; fflush(NULL);
		return (0);
	}

	int nCols = 0;
    int nroColsLP = lp->getNumCols();

	fread(&nCols,sizeof(int),1,fin);
   
	if ( nCols == nroColsLP )
	{
		for (int i =0; i < nCols; i++)
		{
			double auxDbl;
			fread(&auxDbl,sizeof(double),1,fin);
			(xSol)[i] = auxDbl;
		}
	}
	else
	{
		std::cout << "\nErro em readOpSolBin( int etapa, int type, double *xSol ): "
					<< " \nNumero diferente de variaveis: " << nCols << " != " << nroColsLP; fflush(NULL);
		return (0);
	}
	fclose(fin);
	
	return (1);
}

void Operacional::writeOpSolTxt( int type, double *xSol, int fase )
{
	#ifndef PRINT_LOGS
		return;
	#endif

	char solName[1024]="\0";

	switch (type)
	{
		case (OP_BIN):
			strcpy( solName, getSolucaoOpFileName( -1, fase ).c_str() );
			break;
		case (OP_BIN0):
			strcat( solName, getSolOpBinFileName( 0, fase ).c_str() );
			break;
		case (OP_BIN1):
			strcat( solName, getSolucaoOpFileName( 1, fase ).c_str() );
			break;
		case (OP_BIN2):
			strcat( solName, getSolucaoOpFileName( 2 , fase).c_str() );
			break;
		case (OP_BIN3):
			strcat( solName, getSolucaoOpFileName( 3, fase ).c_str() );
			break;
	}

	// WRITES SOLUTION
		
	ofstream fout( solName, ios_base::out );
	if ( fout == NULL )
	{
		std::cout << "\nErro em SolverMIP::writeOpSolTxt( int type, double *xSol ):"
				<< "\nArquivo " << solName << " nao pode ser aberto.\n";
	}
	else
	{		
		VariableOpHash::iterator vit = vHashOp.begin();
		while ( vit != vHashOp.end() )
		{
				VariableOp v = vit->first;
				int col = vit->second;
				double value = xSol[ col ];
		  
				fout << v.toString() << " = " << value << endl;
				  
				vit++;
		}
		fout.close();		
	}
}

bool Operacional::inSolution( VariableOp v )
{		
	GGroup< VariableOp *, LessPtr<VariableOp> >::iterator itSol = (*this->solVarsOp).begin();
	for ( ; itSol != (*this->solVarsOp).end(); itSol++ )
    {
		fflush(NULL);
		if ( **itSol == v )
			return true;
	}
	return false;
}

#ifdef HEURISTICA_MARCIO
int Operacional::solveOperacional()
{
   solveOperacionalEtapas();
   // Criando uma solução inicial
   SolucaoInicialOperacional solIni( *problemData );

   std::cout << "Gerando uma solucao inicial para o modelo operacional" << std::endl;
   SolucaoOperacional & solucaoOperacional = solIni.geraSolucaoInicial();

   std::cout << "Solucao inicial gerada." << std::endl;

   /*bool solucao_valida = solucaoOperacional.validaSolucao( "Validando a solucao inicial gerada" );
   if ( solucao_valida )
   {
      std::cout << "Solucao Inicial VALIDA gerada." << std::endl;
   }
   else
   {
      std::cout << "Solucao Inicial NAO-VALIDA gerada." << std::endl;
   }*/

   solucaoOperacional.toString2();

   // Avaliador
   Avaliador avaliador;
   std::cout << "Avaliando solucao" << std::endl;
   avaliador.avaliaSolucao( solucaoOperacional, true );
   std::cout << "Solucao avaliada." << std::endl;

   // Estruturas de Vizinhança
   //NSSeqSwapEqBlocks nsSeqSwapEqBlocks ( *problemData );
   //NSSwapEqSchedulesBlocks nsSwapEqSchedulesBlocks ( *problemData );
   //NSSwapEqTeachersBlocks nsSwapEqTeachersBlocks ( *problemData );
   NSShift nsShift( *problemData );

   // Heurísticas de Busca Local - Descida Randômica
   //RandomDescentMethod rdmSeqSwapEqBlocks ( avaliador, nsSeqSwapEqBlocks, 300 );
   //RandomDescentMethod rdmSwapEqSchedulesBlocks ( avaliador, nsSwapEqSchedulesBlocks, 300 );
   //RandomDescentMethod rdmSwapEqTeachersBlocks ( avaliador, nsSwapEqTeachersBlocks, 300 );
   RandomDescentMethod rdmShift ( avaliador, nsShift, 300 );

   //rdmSeqSwapEqBlocks.exec( solucaoOperacional, 30, 0 );
   //rdmSwapEqSchedulesBlocks.exec( solucaoOperacional, 30, 0 );
   //rdmSwapEqTeachersBlocks.exec( solucaoOperacional, 30, 0 );
   rdmShift.exec( solucaoOperacional, 30, 0 );

   // Mecanismo de perturbação
   //ILSLPerturbationLPlus2 ilslPerturbationPlus2 ( avaliador, -1, nsSeqSwapEqBlocks );
   //ilslPerturbationPlus2.add_ns( nsSeqSwapEqBlocks );
   //ilslPerturbationPlus2.add_ns( nsSwapEqSchedulesBlocks );
   //ilslPerturbationPlus2.add_ns( nsSwapEqTeachersBlocks );
   //ilslPerturbationPlus2.add_ns( nsShift );

   // RVND
   std::vector< Heuristic * > heuristicasBuscaLocal;
   //heuristicasBuscaLocal.push_back( &rdmSeqSwapEqBlocks );
   //heuristicasBuscaLocal.push_back( &rdmSwapEqSchedulesBlocks );
   //heuristicasBuscaLocal.push_back( &rdmSwapEqTeachersBlocks );
   heuristicasBuscaLocal.push_back( &rdmShift );

   RVND rvnd( avaliador, heuristicasBuscaLocal );

   // Busca Local Iterada por Níveis
   //IteratedLocalSearchLevels ilsl ( avaliador, rvnd, ilslPerturbationPlus2, 20 , 4 );

   // Parâmetro 2 : tempo ( em segundos )
   //ilsl.exec( solucaoOperacional, 30, 0 );

   // Avaliação final
   avaliador.avaliaSolucao( solucaoOperacional, true );
   /*solucao_valida = solucaoOperacional.validaSolucao( "Validando a solucao final" );
   if ( solucao_valida )
   {
      std::cout << "Solucao final viavel." << std::endl;
   }
   else
   {
      std::cout << "A solucao final NAO ALOCOU todas as aulas." << std::endl;
   }*/

   // Armazena a solução operacional no problem solution
   problemSolution->solucao_operacional = &( solucaoOperacional );
   return 1;
}
#endif

int Operacional::solveOperacionalEtapas()
{
	bool status=true;
	    
	std::cout<<"\n\n\n------------------------------Operacional------------------------------\n\n";
	
    cout <<"\n\nOperacional::solveOperacionalEtapas";
    cout<<"\nthis->CARREGA_SOLUCAO = " << *this->CARREGA_SOLUCAO << "\n";

	std::cout<<"\n============================================================\n";

	// ------------------------------
	// Professores virtuais a estimar
	this->setRodada( Operacional::OP_VIRTUAL_PERFIL );

	std::cout<<"\nSolving MIP-Operacional com profs virtuais a estimar...\n\n";
	status = status && solveOperacionalMIP();
	std::cout<<"\nCarregando solucao...\n";
	carregaSolucaoOperacional();
	

	std::cout<<"\n============================================================\n";

    // ------------------------------
	// Cria o máximo de professores virtuais necessários e os insere infiltrados
	// na lista de professores (até então, reais). Um prof virtual terá sempre id negativo.
	// Limpa a lista de profs virtuais estimados inicialmente.

	std::cout<<"\nGerando profs virtuais...\n";
    geraMinimoDeProfessoresVirtuaisMIP();
	this->setRodada( Operacional::OP_VIRTUAL_INDIVIDUAL );


	std::cout<<"\n============================================================\n";

	// ------------------------------
	// Solução com profs reais fixada
	// Professores virtuais estimados e infiltrados no meio dos profs reais
	// Sem estimar novos profs virtuais
	std::cout<<"\nSolving MIP-Operacional com prof virtuais estimados...\n";
	status = status && solveOperacionalMIP();
	carregaSolucaoOperacional();


	std::cout<<"\n============================================================\n";

	// ------------------------------
	// Retira os profs virtuais infiltrados na lista de reais e os coloca em professores_virtuais
		
	std::cout<<"\nReferenciando profs virtuais corretamente...\n";
	separaProfsVirtuais();

	std::cout<<"\n============================================================\n";
		
	  
	std::cout<<"\nGetting solution...\n";
	getSolutionOperacionalMIP();

	
	std::cout<<"\n============================================================\n";
	testaTrocarProfVirtualPorReal();
	
	std::cout<<"\nPreenche output...\n";
	preencheOutputOperacionalMIP();
		
	std::cout<<"\n============================================================\n";

	return status;
}


void Operacional::testaTrocarProfVirtualPorReal()
{
	/*
		1) Algoritmo para, a partir de uma solução do modo Operacional do Trieda, determinar que professor 
		real poderia substituir um professor virtual, supondo total disponibilidade de dias e horários do 
		professor na semana. Isto é, somente a disponibilidade do professor é alterada (considerada total),
		o professor ainda tem que ser habilitado na disciplina e sua alocação na aula não pode violar
		restrições fortes.

		Se um professor real pode, sem qualquer alteração em sua disponibilidade de dias e horários na semana,
		substituir um professor virtual em uma aula sem violar nenhuma restrição forte, então isso indica 
		possível falha na solução.
	*/

	/*
		A solução completa (isto é, todas as variaveis não nulas)
	 fica armazenada em "GGroup< VariableOp *, LessPtr<VariableOp> > *solVarsOp;",
	 aonde a classe GGroup é somente uma redefinição de um std::set.

	 Para esse algoritmo, entretanto, basta olhar as variáveis do tipo "V_X_PROF_AULA_HOR",
	 pois elas já determinam todas as alocações dos professores.
	
	 Tais variáveis são binárias chamadas de x_{p, a, hi}, aonde os índices significam:
	
	 p = professor (pode ser virtual ou não)
	 a = aula, definida no modo tático, que corresponde a uma turma i, de uma disciplina d,
			em uma sala s, em um dia t, com n créditos.
	 hi = horário inicial. Como o número n de créditos da aula já está definido, a escolha de um horário inicial hi
			implica automaticamente em um horário final hf (hf = hi + n).
	*/

	//Hash map que guarda a informação dos horarios ja alocados pelos professores reais
	//na solução do modo operacional
	std::map< int /*profId*/, std::map<int/*dia*/, vector<HorarioDia>> > horariosAlocadosProfessoresReais;

	//hash map para agrupar todas as VariableOp do tipo V_X_PROF_AULA_HOR que possuem professores virtuais deu uma mesma disciplina e turma
	std::map< Campus*, std::map< Disciplina*, std::map<int/*idTurma*/, std::vector<VariableOp*>>,
		LessPtr<Disciplina> >, LessPtr<Campus> > variaveisProfVirtuais;
	
	//Hash map para armazenar o numero de creditos associados a um professor
	std::map<int/*idProf*/, int/*quantidade de creditos*/> quantCredProfs;

	GGroup< Professor *, LessPtr<Professor> > allRealProfs = problemData->getProfessores();
	GGroup< Professor *, LessPtr<Professor> >::iterator itProf = allRealProfs.begin();	
	for (; itProf != allRealProfs.end(); itProf++)
	{
		quantCredProfs[ (*itProf)->getId() ] = 0;
	}


    //Nesta parte do codigo são agrupadas todas as VariablesOp de acordo com a turma e a disciplina. Também é obtida a informação
    // a respeito dos horarios já alocados para os professores reais assim como os creditos a eles já associados. Essas informações são
	//necessárias para conferir se não haverá violação na restrições fortes do problema.

	ITERA_GGROUP_LESSPTR( itOp, (*solVarsOp), VariableOp )
   {
      VariableOp * v = *itOp;
	
	  if ( v->getType() != VariableOp::V_X_PROF_AULA_HOR )
      {
         continue;
      }

	  int dia = v->getAula()->getDiaSemana();
	  int idProf = v->getProfessor()->getId();
	  std::map< int, std::map<int, vector<HorarioDia>> >::iterator itDiaHorariosAlocados = horariosAlocadosProfessoresReais.find(idProf);
	  std::map<int, vector<HorarioDia>>::iterator itHorariosAlocados;
	  if( itDiaHorariosAlocados != horariosAlocadosProfessoresReais.end() )
	  {
		  itHorariosAlocados = itDiaHorariosAlocados->second.find( dia );
	  }

	  if(v->getProfessor()->eVirtual()){//Se o professor empregado é virtual
		  //É usado valor absoluto do id da disciplina devido à restrição de que em algumas disciplinas o mesmo professor
		  //deve ministrar as aulas teoricas e praticas, dessa forma as duas partes(teorica e pratica) estarao sempre no mesmo bloco.
		  //Então somente se um professor poder atender essas duas partes(teorica e pratica) será possivel trocar ou alterar a solução.

		  Campus *cp = problemData->refCampus[ v->getAula()->getUnidade()->getIdCampus() ];
		  int turma = v->getTurma();
		  Disciplina *disc = v->getDisciplina();
		  
		  //Essa forma não garante que as partes teorica e pratica vao ser indicadas como alvo de uma troca simultanea,
		  //somente quando exigir um mesmo professor
		  if((v->getDisciplina()->getId()<0) && (v->getDisciplina()->getProfUnicoDiscPT())){
			  disc = problemData->retornaDisciplina( std::abs(v->getDisciplina()->getId()) );
		  }else{
			  disc = v->getDisciplina();
		  }
		  
		  // ---------------------------------------------
		  // Agrupando as VariablesOp dos professores virtuais pelo pair<idDisciplina,idTurma>:
		  std::vector<VariableOp*> listaVariaveisOp;

		  std::map< Campus*, std::map< Disciplina*, std::map<int/*idTurma*/, std::vector<VariableOp*>>,
			  LessPtr<Disciplina> >, LessPtr<Campus> >::iterator itFinder1 = variaveisProfVirtuais.find(cp);
		  if ( itFinder1 != variaveisProfVirtuais.end() )
		  {
			  std::map< Disciplina*, std::map<int/*idTurma*/, std::vector<VariableOp*>>,
				  LessPtr<Disciplina> >::iterator itFinder2 = itFinder1->second.find( disc );
			  if ( itFinder2 != itFinder1->second.end() )
			  {
				  std::map<int/*idTurma*/, std::vector<VariableOp*>>::iterator itFinder3 = itFinder2->second.find( turma );
				  if ( itFinder3 != itFinder2->second.end() )
				  {
					  // Se já existe alguma variavel desse trio no hash map
					  listaVariaveisOp = itFinder3->second;
				  }
			  }
		  }
		  		
		  listaVariaveisOp.push_back(v);
		  variaveisProfVirtuais[cp][disc][turma] = listaVariaveisOp;
		  
	  }else{// Se o professor empregado é real
		  // Levantando os horarios já alocados para os professores reais:		  
		  
		  // ---------------------------------------------
		  std::vector<HorarioDia> horarios;

		  if( itDiaHorariosAlocados != horariosAlocadosProfessoresReais.end() )
		  if( itHorariosAlocados != itDiaHorariosAlocados->second.end() )
		  {
			  // Se o idProf já constar no hash map: atualiza o vector dos horarios alocados
			  horarios = itHorariosAlocados->second;
		  }	  
		  
		  HorarioAula* horarioAulaV = v->getHorarioAula();
		  int nCredDia = v->getAula()->getTotalCreditos();

		  for(int k = 0; k != nCredDia; k++){
			HorarioDia* horario = this->problemData->getHorarioDiaCorrespondente( horarioAulaV,dia );
			horarios.push_back(*horario);
			horarioAulaV = horarioAulaV->getCalendario()->getProximoHorario(horarioAulaV);
		  }
		  horariosAlocadosProfessoresReais[idProf][ dia ] = horarios;
		  		  
		  // ---------------------------------------------
		  // Incrementando a informação de quantos creditos já estão associados a cada professor:		  	  
		  quantCredProfs[idProf] += v->getAula()->getTotalCreditos() *
									v->getAula()->getDisciplina()->getTempoCredSemanaLetiva();		
	  }
  }
   // Flags para a escrita dos arquivos de saida
   bool escrevePossiveisTrocas = false;
   bool escreveAlocacoesAlteradas = false;
   bool escreveMotivos = false;

   ofstream saidaTrocasFile;
   char saidaTrocasFilename[] = "saidaTestaTrocarProfVirtualPorReal_PossiveisTrocas.txt";
   saidaTrocasFile.open(saidaTrocasFilename, ios::out);

   ofstream saidaAlteracoesFile;
   char saidaAlteracoesFilename[] = "saidaTestaTrocarProfVirtualPorReal_Alteracoes.txt";
   saidaAlteracoesFile.open(saidaAlteracoesFilename, ios::out);

   ofstream saidaMotivosCriacaoProfVirtuaisFile;
   char saidaMotivosCriacaoProfVirtuaisFileFilename[] = "saidaTestaTrocarProfVirtualPorReal_MotivoCriacaoProfVirtuais.txt";
   saidaMotivosCriacaoProfVirtuaisFile.open(saidaMotivosCriacaoProfVirtuaisFileFilename, ios::out);

   bool DETALHAR_TODOS_OS_MOTIVOS = true;	// Não pára no primeiro motivo encontrado. Percorre todos.
   bool DETALHAR_TODOS_PROFS = false;		// Detalha até mesmo os profs sem magisterio na disciplina ou nao cadastrados no campus
   bool IMPRIMIR_SOMENTE_RESUMO = true;

   //Para cada bloco de variaveis de mesmo trio<Campus,Disciplina,turma>
   std::map< Campus*, std::map< Disciplina*, std::map<int/*idTurma*/, std::vector<VariableOp*>>,
		LessPtr<Disciplina> >, LessPtr<Campus> >::iterator itFinder1 = variaveisProfVirtuais.begin();
   for ( ; itFinder1 != variaveisProfVirtuais.end(); itFinder1++ )
   {
	    Campus* cp = itFinder1->first;
	    int cpId = cp->getId();

		std::map< Disciplina*, std::map<int/*idTurma*/, std::vector<VariableOp*>>,
			LessPtr<Disciplina> >::iterator itFinder2 = itFinder1->second.begin();
		for ( ; itFinder2 != itFinder1->second.end(); itFinder2++ )
		{
			Disciplina* disciplina = itFinder2->first;
			int discId = disciplina->getId();

			std::map<int/*idTurma*/, std::vector<VariableOp*>>::iterator itFinder3 = itFinder2->second.begin();
			for ( ; itFinder3 != itFinder2->second.end(); itFinder3++ )
			{
			   int turma = itFinder3->first;
			   std::vector<VariableOp*> listaVarDoPar = itFinder3->second;

			   std::vector<std::pair<VariableOp*, Professor*> >possiveisTrocasParcial;
			   std::vector<std::pair<VariableOp, VariableOp> > alocacoesAlteradasParcial;

			   std::vector<std::pair<VariableOp, int> > variableOpsConflitoHorarioIdsHorAulas;
			   std::map<std::pair<int, int>, std::pair<VariableOp, int> > variableOpsNaoDisponiveisHorarioIdsHorAulas;
			   
			   // Flags para motivos comuns a todos os professores reais
			   int flagFaltaCadastroNoCampus = -1;
			   int flagFaltaMagistrado = -1;
			   int flagFaltaMestreDoutor = -1;
			   int flagFaltaCargaHoraria = -1;
			   int flagFaltaHorarioConflito = -1;
			   int flagFaltaHorarioDisponivel = -1;
			   int flagFaltaMaxDiasSemana = -1;
			   int flagFaltaMinCredsDiarios = -1;
			   int flagFaltaInterjornada = -1;

			   // Flags para resumo
			   int flagNroFaltaCadastroNoCampus = 0;
			   int flagNroFaltaMagistrado = 0;
			   int flagNroFaltaMestreDoutor = 0;
			   int flagNroFaltaCargaHoraria = 0;
			   int flagNroFaltaHorarioConflito = 0;
			   int flagNroFaltaHorarioDisponivel = 0;
			   int flagNroFaltaMaxDiasSemana = 0;
			   int flagNroFaltaMinCredsDiarios = 0;
			   int flagNroFaltaInterjornada = 0;

			   VariableOp* v0 = listaVarDoPar[0];
			   
			   bool ehPrat = (bool) v0->getAula()->getCreditosPraticos();

			   if ( turma == 1 && discId == 13039 )
				   std::cout<<"\nAqui\n";

			   ProfessorVirtualOutput *pvo = problemSolution->getProfVirtualOutput( v0->getProfessor()->getId() );
			   if ( pvo == NULL ){
					std::cout<<"\nERRO!!! PROFESSOR VIRTUAL" << v0->getProfessor()->getId() << "NAO ENCONTRADO";
				    continue;
			   }

			   AlocacaoProfVirtual *alocacao = pvo->getAlocacao( abs( discId ), turma, cpId, ehPrat );
			   if ( alocacao == NULL ){
					std::cout<<"\nERRO!!! ALOCACAO NAO ENCONTRADA:\tdisc " 
						<< abs( discId ) <<", turma "<< turma <<", cp "<< cpId <<", pratica "<< ehPrat;
				    continue;
			   }
			   
			   #pragma region CONFERE TODOS OS PROFESSORES REAIS POSSÍVEIS PARA A TURMA

			   //Para cada aula confere todos os professores
			   GGroup< Professor *, LessPtr<Professor> >::iterator itProf = allRealProfs.begin();	
			   for (; itProf != allRealProfs.end(); itProf++)
			   {
					Professor* p = *itProf;
					int idProfReal = p->getId();
					
					// Flags por professor real
				    int flagProfSemCadastroNoCampus = -1;
				    int flagProfSemMagisterio = -1;
					int flagProfSemTituloMinimo = -1;
					int flagProfCHMaxima = -1;
				    int flagProfHorarioConflito = 0;
				    int flagProfFaltaHorarioDisponivel = 0;
					int flagProfViolaInterjornada = 0;
				    int flagProfViolaMaxDias = 0;
				    int flagProfViolaMinCredsDiarios = 0;

					//Flag para indicar se o professor atual p pode assumir a turma do professor virtual considerando
					//disposição total de horarios daquele e sem violar restricões fortes.
					int flagPossivelTroca = 1;

					//Flag para indicar que possivel o professor p assumir a turma de um professor virtual nao violando restrições fortes
					//e não violando a disposição de horarios daquele. (Indica possivel falha no modelo matematico)
					int flagAltera = 1;

					//// 1. No algoritmo primeiro é verificado se é possivel trocar o professor virtual responsavel pelo trio<Disciplina,turma,campus> pelo professor real
					//para isso ele confere se o professor real é magistrado para aquela disciplina, depois confere se ao atribuir essa disciplina/turma
					//não irá ultrapassar a carga horaria maxima semanal do professor. Feito isso o algoritmo checa os horarios. Primeiramente o dia, depois o turno
					//e em segida se não há conflito nos horarios ja alocados para o professor real. Se todas essa restrições forem atendidas a flag de que pode-se
					//trocar o professor é setada parcialmente em 1.

					//// 2. Na segunda etapa se é possivel fazer uma troca entre os professores então o algoritmo checa se é possivel alterar dentro da disponibilidade
					//do professor real. Para isso é checado se há disponibilidade do professor no dia, turno e enfim no horario seguindo essa ordem. Se existir a
					//flag de alteração é ativada.
				
					flagProfSemCadastroNoCampus = ! ( cp->professores.find(p) != cp->professores.end() );						

					flagFaltaCadastroNoCampus = flagFaltaCadastroNoCampus && flagProfSemCadastroNoCampus;
					
					flagNroFaltaCadastroNoCampus += (flagProfSemCadastroNoCampus? 1 : 0);

					if ( !flagProfSemCadastroNoCampus || DETALHAR_TODOS_PROFS )
					{
						flagProfSemMagisterio = ! ( p->possuiMagisterioEm(v0->getDisciplina()) );

						flagFaltaMagistrado = flagFaltaMagistrado && flagProfSemMagisterio;

						flagNroFaltaMagistrado += (flagProfSemMagisterio? 1 : 0);

						if( !flagProfSemMagisterio || DETALHAR_TODOS_PROFS ) // Se possui magisterio
						{
							flagProfSemTituloMinimo = ! ( p->getTitulacao() >= v0->getProfessor()->getTitulacao() );
							
							flagFaltaMestreDoutor = flagFaltaMestreDoutor && flagProfSemTituloMinimo;

							flagNroFaltaMestreDoutor += (flagProfSemTituloMinimo? 1 : 0);

							if( !flagProfSemTituloMinimo || DETALHAR_TODOS_OS_MOTIVOS ) // Se a titulação for maior ou igual
							{			
								int acrescimo = v0->getAula()->getDisciplina()->getTotalCreditos() *
												v0->getAula()->getDisciplina()->getTempoCredSemanaLetiva();

								flagProfCHMaxima = ! ( quantCredProfs[p->getId()] + acrescimo <= p->getChMax()*60 );
								
								flagFaltaCargaHoraria = flagFaltaCargaHoraria && flagProfCHMaxima;

								flagNroFaltaCargaHoraria += (flagProfCHMaxima? 1 : 0);

								if( !flagProfCHMaxima || DETALHAR_TODOS_OS_MOTIVOS ) // Se a carga horaria maxima desse professor nao é violada
								{
									std::map< int, std::map<int, vector<HorarioDia>> >::iterator 
										itDiaHorsAlocados = horariosAlocadosProfessoresReais.find(p->getId());
							
									// -------------------------------------------------
									// Percorre todas as aulas do trio e confere
									for(int i=0; i<listaVarDoPar.size(); i++)
									{
										#pragma region CONFERE A I-ÉSIMA AULA DA TURMA

										if( !flagPossivelTroca && !DETALHAR_TODOS_OS_MOTIVOS ){
											break;
										}

										VariableOp* v = listaVarDoPar[i];
						
										HorarioAula* horarioAulaV = v->getHorarioAula();
										int nCredDia = v->getAula()->getTotalCreditos();
						
										// Horarios ocupados no dia do professor
										std::vector<HorarioDia> horariosAlocadosNoDia;
										if( itDiaHorsAlocados != horariosAlocadosProfessoresReais.end() )
										{
											int nroDiasUsados = itDiaHorsAlocados->second.size();

											std::map<int, vector<HorarioDia>>::iterator 
												itHorsAlocados = itDiaHorsAlocados->second.find( v->getAula()->getDiaSemana() );
											if ( itHorsAlocados != itDiaHorsAlocados->second.end() )
											{
												horariosAlocadosNoDia = itHorsAlocados->second;
											}
											else 
											{
												if ( nroDiasUsados + 1 > p->getMaxDiasSemana() )
												{
													flagProfViolaMaxDias++;
													flagAltera = 0;
												}

												flagFaltaMaxDiasSemana = flagFaltaMaxDiasSemana && flagProfViolaMaxDias;

												if ( nCredDia < p->getMinCredsDiarios() )
												{
													int gapViolado = p->getMinCredsDiarios() - nCredDia;
													if ( flagProfViolaMinCredsDiarios < gapViolado )
														flagProfViolaMinCredsDiarios = gapViolado;
													flagAltera = 0;
												}
												
												flagFaltaMinCredsDiarios = flagFaltaMinCredsDiarios && flagProfViolaMinCredsDiarios;
											}
										}

										// Confere para todos os horarios da turma no dia da VariableOp v
										for(int k = 1; k <= nCredDia; k++)
										{
											#pragma region CONFERE CONFLITOS DO HORARIO COM OUTRAS ALOCACOES DO PROFESSOR
											// Confere se existe conflito entre os horarios da VariavelOp e os horarios alocados ao professor
											if( horariosAlocadosNoDia.size() > 0 )
											{
												for(int l=0; l<horariosAlocadosNoDia.size(); l++)
												{
													HorarioDia h = horariosAlocadosNoDia[l];
													HorarioAula* hAula = h.getHorarioAula();
																								
													if( (hAula->getInicio() >= horarioAulaV->getInicio()) && (hAula->getInicio() < horarioAulaV->getFinal()) ||
														(hAula->getFinal() > horarioAulaV->getInicio()) && (hAula->getFinal() < horarioAulaV->getFinal()) )
													{
														// Há conflito
														// Seta as flags para zero
														flagAltera = 0;
														flagPossivelTroca = 0;

														flagProfHorarioConflito = 1;
														std::pair<VariableOp,int> pair(*v,horarioAulaV->getId());
														variableOpsConflitoHorarioIdsHorAulas.push_back(pair);
													}
												}
											}else{ // Se o professor não tem nenhum horario alocado então confere se existe algum horario diponivel que de certo
												flagPossivelTroca = 1;
											}
											
											#pragma endregion

											#pragma region CONFERE SE O HORARIO É HABILITADO NO CADASTRO DO PROFESSOR
											int flagEncontrouHorario = 0;
									
											// Procura se o HorarioDia está nos horários de disponibilidade do professor
											GGroup< HorarioDia *, LessPtr< HorarioDia > >::iterator
												itrHorDispProf = p->horariosDia.begin();									
											for(; itrHorDispProf!=p->horariosDia.end(); itrHorDispProf++)
											{
												HorarioAula* horarioDisp = itrHorDispProf->getHorarioAula();

												if(v->getDia()==itrHorDispProf->getDia()){				// Se for no mesmo dia
													if(horarioAulaV->inicioFimIguais(*horarioDisp)){	// Se for no mesmo horario
														flagEncontrouHorario = 1;
														break;
													}
												}									
											}
											flagAltera = flagAltera && flagEncontrouHorario;

											if( !flagEncontrouHorario )
											{											
												flagProfFaltaHorarioDisponivel = 1;

												std::pair<int, int> chaveMap(v->getDia(), horarioAulaV->getId());
												std::map<std::pair<int, int>, std::pair<VariableOp, int> >::iterator itrHorariosNaoDisponiveis; 

												if(variableOpsNaoDisponiveisHorarioIdsHorAulas.find(chaveMap) == variableOpsNaoDisponiveisHorarioIdsHorAulas.end()){										
													std::pair<VariableOp, int> pairVHorario(*v, horarioAulaV->getId());
													variableOpsNaoDisponiveisHorarioIdsHorAulas[chaveMap] = pairVHorario;
												}
											}
											#pragma endregion

											if ( k < nCredDia )
												horarioAulaV = horarioAulaV->getCalendario()->getProximoHorario(horarioAulaV);
										}

										#pragma region CONFERE INTERJORNADA MINIMA DO PROFESSOR
										// Confere interjornada
 										if ( problemData->parametros->considerarDescansoMinProf )
										{
											double tempoMinimoDescanso = problemData->parametros->descansoMinProfValue;	// em horas
											tempoMinimoDescanso *= 60;													// em minutos

											// Dia seguinte
											DateTime dtf_maisDescanso = horarioAulaV->getFinal();				
											dtf_maisDescanso.addMinutes( tempoMinimoDescanso );
											if ( horarioAulaV->getFinal().getDay() != dtf_maisDescanso.getDay() )
											{
												// Verifica se o professor esta alocado em algum horario h no dia seguinte
												// tal que h < dtf_maisDescanso
												if( itDiaHorsAlocados != horariosAlocadosProfessoresReais.end() )
												{
													std::map<int, vector<HorarioDia>>::iterator 
														itHorsDiaSeg = itDiaHorsAlocados->second.find( v->getAula()->getDiaSemana() + 1 );
													if ( itHorsDiaSeg != itDiaHorsAlocados->second.end() )
													{
														for ( int at = 0; at < itHorsDiaSeg->second.size(); at++ )
														{
															HorarioDia hd = itHorsDiaSeg->second[at];
															if ( less_than( hd.getHorarioAula()->getInicio(), dtf_maisDescanso ) )
															{
																// Viola interjornada mínima
																flagProfViolaInterjornada = 1;
																flagAltera = 0;
																flagPossivelTroca = 0;
																break;
															}
														}
													}
												}
											}

											// Dia anterior
											DateTime dti_menosDescanso = horarioAulaV->getInicio();				
											dti_menosDescanso.subMinutes( tempoMinimoDescanso );
											if ( horarioAulaV->getFinal().getDay() != dti_menosDescanso.getDay() )
											{
												// Verifica se o professor esta alocado em algum horario h no dia anterior
												// tal que h > dti_menosDescanso									
												if( itDiaHorsAlocados != horariosAlocadosProfessoresReais.end() )
												{
													std::map<int, vector<HorarioDia>>::iterator 
														itHorsDiaAnt = itDiaHorsAlocados->second.find( v->getAula()->getDiaSemana() - 1 );
													if ( itHorsDiaAnt != itDiaHorsAlocados->second.end() )
													{
														for ( int at = 0; at < itHorsDiaAnt->second.size(); at++ )
														{
															HorarioDia hd = itHorsDiaAnt->second[at];
															if ( less_than( dti_menosDescanso, hd.getHorarioAula()->getFinal() ) )
															{
																// Viola interjornada mínima
																flagProfViolaInterjornada = 1;
																flagAltera = 0;
																flagPossivelTroca = 0;
																break;
															}
														}
													}
												}
											}
										}
										#pragma endregion

										if( flagPossivelTroca && flagProfFaltaHorarioDisponivel ){
											possiveisTrocasParcial.push_back(std::pair<VariableOp*, Professor*>(v, p));
										}
										if( flagAltera ){
											VariableOp vAntiga = *v;
											VariableOp vAlterada = *v;
											vAlterada.setProfessor(p);
							
											alocacoesAlteradasParcial.push_back(std::pair<VariableOp, VariableOp>(vAntiga, vAlterada));							
										}
										#pragma endregion
									// -------------------------------------------------
									}
									
									flagFaltaHorarioConflito = flagFaltaHorarioConflito && flagProfHorarioConflito;
									flagFaltaHorarioDisponivel = flagFaltaHorarioDisponivel && flagProfFaltaHorarioDisponivel;
									flagFaltaInterjornada = flagFaltaInterjornada && flagProfViolaInterjornada;

									flagNroFaltaHorarioConflito += (flagProfHorarioConflito? 1 : 0);
									flagNroFaltaHorarioDisponivel += (flagProfFaltaHorarioDisponivel? 1 : 0);
									flagNroFaltaInterjornada += (flagProfViolaInterjornada? 1 : 0);
								}
							}
						}
					}

					if ( !IMPRIMIR_SOMENTE_RESUMO )
					{
						if ( flagProfSemCadastroNoCampus == 1 )
						{
							alocacao->addDescricaoDoMotivo( idProfReal, "Não possui cadastro no campus da turma." );													
							flagAltera = 0;
							flagPossivelTroca = 0;						
						}
						if ( flagProfSemMagisterio == 1 )
						{
							alocacao->addDescricaoDoMotivo( idProfReal, "Não possui habilitação nesta disciplina." );													
							flagAltera = 0;
							flagPossivelTroca = 0;						
						}
						if ( flagProfSemTituloMinimo == 1 )
						{
							stringstream ss;
							ss << "Titulação do professor não suficiente. Mínimo título necessário: "
								<< v0->getProfessor()->titulacao->getNome() << ".";
							alocacao->addDescricaoDoMotivo( idProfReal, ss.str() );
							flagAltera = 0;
							flagPossivelTroca = 0;
						}
						if ( flagProfCHMaxima == 1 )
						{
							alocacao->addDescricaoDoMotivo( idProfReal, "Ultrapassaria a carga horária máxima." );
							flagAltera = 0;
							flagPossivelTroca = 0;
						}
						if ( flagProfHorarioConflito == 1 )
						{
							alocacao->addDescricaoDoMotivo( idProfReal, "Haveria conflito nos horários da turma com outras alocações do professor." );
						}
						if ( flagProfFaltaHorarioDisponivel == 1 )
						{
							alocacao->addDescricaoDoMotivo( idProfReal, "Horários da turma não presentes na disponibilidade do professor." );
						}
						if ( flagProfViolaInterjornada == 1 )
						{															
							alocacao->addDescricaoDoMotivo( idProfReal, "Violaria interjornada mínima do professor." );
						}
						if ( flagProfViolaMaxDias > 0 )
						{
							alocacao->addDescricaoDoMotivo( idProfReal, "Violaria o máximo de dias na semana para o professor." );					
						}
						if ( flagProfViolaMinCredsDiarios > 0 )
						{
							alocacao->addDescricaoDoMotivo( idProfReal, "Violaria o mínimo de créditos diários do professor." );					
						}
					}

					if( flagPossivelTroca )
					{
						if(!escrevePossiveisTrocas){
							escrevePossiveisTrocas = true;
							cout<<"\n================================> ";
							cout<<"DICAS PARA TROCAS DE PROFESSORES VIRTUAIS POR REAIS FORAM ENCONTRADAS!" <<endl;				
										
							if (!saidaTrocasFile) {
								cout << "Can't open output file " << saidaTrocasFilename << endl;
								exit(1);
					
							}		
							saidaTrocasFile << "-------------------------------------------------------------------------------------------------"<<endl;
							saidaTrocasFile << "----------------------DICAS PARA TROCAS ENTRE DE PROFESSORES VIRTUAIS POR REAIS-----------------------------"<<endl;
							saidaTrocasFile << "--Considerando diponibilidade total do professor real e respeitando as demais restrições fortes--"<<endl;
							saidaTrocasFile << "-------------------------------------------------------------------------------------------------\n"<<endl;
							saidaTrocasFile << "=================================================================================================\n"<<endl;
						}	
						
						if ( possiveisTrocasParcial.size() > 0 )
						{
							stringstream ss;
							int turnoIESid = possiveisTrocasParcial[0].first->getHorarioAula()->getTurnoIESId();
							TurnoIES* turno = problemData->refTurnos[ turnoIESid ];

							ss << "Incluir na disponibilidade do professor os horários:";

							for(int i=0; i<possiveisTrocasParcial.size();i++){
								VariableOp* v = possiveisTrocasParcial[i].first;
								int hour1 = v->getHorarioAula()->getInicio().getHour();
								int min1 = v->getHorarioAula()->getInicio().getMinute();
								int ncreds = v->getAula()->getTotalCreditos();
								int tempoAula = v->getDisciplina()->getTempoCredSemanaLetiva();

								DateTime final = v->getHorarioAula()->getInicio();
								final.addMinutes( ncreds*tempoAula );
								int hour2 = final.getHour();
								int min2 = final.getMinute();
								ss << " de " << hour1 << ":" << (min1<10 ? "0":"") << min1;
								ss << " a " << hour2 << ":" << (min2<10 ? "0":"") << min2;
								ss << " n" << problemData->getDiaEmString( v->getDia(), true ) << ";";
							}
							alocacao->addDicaEliminacao( possiveisTrocasParcial[0].second->getId(), ss.str() );
						}
						if ( flagProfViolaMaxDias )
						{
							stringstream ss;
							ss << "Aumentar o máximo de dias na semana do professor para " 
								<< p->getMaxDiasSemana() + flagProfViolaMaxDias << ".";
							alocacao->addDicaEliminacao( p->getId(), ss.str() );
						}
						if ( flagProfViolaMinCredsDiarios )
						{
							stringstream ss;
							ss << "Alterar o mínimo de créditos no dia do professor para " 
								<< p->getMinCredsDiarios() - flagProfViolaMinCredsDiarios << ".";
							alocacao->addDicaEliminacao( p->getId(), ss.str() );
						}

						for(int i=0; i<possiveisTrocasParcial.size();i++){
							std::pair<VariableOp*,Professor*> troca = possiveisTrocasParcial[i];	

							saidaTrocasFile << "VariableOp IDENTIFICADA:" <<endl;
							saidaTrocasFile << troca.first->toString() <<endl <<endl;
							saidaTrocasFile << "PROFESSOR REAL POSSIVEL PARA A TROCA:" <<endl;
							saidaTrocasFile << "ID: " << troca.second->getId() <<endl;
							saidaTrocasFile << "NOME: " << troca.second->getNome() <<endl;
							saidaTrocasFile << "CPF: " << troca.second->getCpf() <<endl;
							saidaTrocasFile << "TITULO: " << troca.second->titulacao->getNome() <<endl;
							saidaTrocasFile << "VALOR CREDITO: " << troca.second->getValorCredito() <<endl <<endl;
							if ( flagProfViolaMaxDias )
								saidaTrocasFile << "VIOLA " << flagProfViolaMaxDias << " dias no maximo de dias na semana";
							if ( flagProfViolaMinCredsDiarios )
								saidaTrocasFile << "VIOLA " << flagProfViolaMinCredsDiarios << " no creditos no minimo de creditos diario";

							saidaTrocasFile << "=================================================================================================\n"<<endl;
						}								
					}													  
					possiveisTrocasParcial.clear();					  
			
					if( flagAltera )
					{
						flagFaltaHorarioDisponivel = 0;
						variableOpsNaoDisponiveisHorarioIdsHorAulas.clear();

						if(!escreveAlocacoesAlteradas){
							escreveAlocacoesAlteradas = true;
							cout<<"\n================================> ";
							cout<<"ALTERACOES NAS ALOCACOES DAS VariablesOP ENCONTRADAS(POSSIVEL ERRO NO MODELO)!"<<endl;
					
							saidaAlteracoesFile << "-------------------------------------------------------------------------------------------------"<<endl;
							saidaAlteracoesFile << "------------------------ALTERACOES ENTRE DE PROFESSORES VIRTUAIS POR REAIS-----------------------"<<endl;
							saidaAlteracoesFile << "---------Considerando todas as restrições fortes, ou seja, indica possivel erro no modelo--------"<<endl;
							saidaAlteracoesFile << "-------------------------------------------------------------------------------------------------\n"<<endl;
							saidaAlteracoesFile << "=================================================================================================\n"<<endl;

							if (!saidaAlteracoesFile) {
								cout << "Can't open output file " << saidaAlteracoesFilename << endl;
								exit(1);						
							}					
						}
						for(int i=0; i<alocacoesAlteradasParcial.size();i++){
							std::pair<VariableOp, VariableOp> alteracao = alocacoesAlteradasParcial[i];

							saidaAlteracoesFile << "VariableOp IDENTIFICADA:" <<endl;
							saidaAlteracoesFile << alteracao.first.toString() <<endl <<endl;
							saidaAlteracoesFile << "PROFESSOR REAL POSSIVEL PARA ALTERACAO:" <<endl;
							saidaAlteracoesFile << "ID: " << alteracao.second.getProfessor()->getId() <<endl;
							saidaAlteracoesFile << "NOME: " << alteracao.second.getProfessor()->getNome() <<endl;
							saidaAlteracoesFile << "CPF: " << alteracao.second.getProfessor()->getCpf() <<endl;
							saidaAlteracoesFile << "TITULO: " << alteracao.second.getProfessor()->titulacao->getNome() <<endl;
							saidaAlteracoesFile << "VALOR CREDITO: " << alteracao.second.getProfessor()->getValorCredito() <<endl <<endl;
							saidaAlteracoesFile << "=================================================================================================\n"<<endl;
						}				
					}
					alocacoesAlteradasParcial.clear();						
			   }

			   #pragma endregion

			   
			   #pragma region IMPRIME CAUSAS E O NRO DE PROFESSORES ASSOCIADOS À CAUSA
			   if ( IMPRIMIR_SOMENTE_RESUMO )
			   {
				   //Gera o relatorio de motivos da criação de professores virtuais
				   if(!escreveMotivos)
				   {
						escreveMotivos = true;
									
						if (!saidaMotivosCriacaoProfVirtuaisFile) {
							cout << "Can't open output file " << saidaMotivosCriacaoProfVirtuaisFileFilename << endl;
							exit(1);
					
						}		
						saidaMotivosCriacaoProfVirtuaisFile << "-------------------------------------------------------------------------------------------------"<<endl;
						saidaMotivosCriacaoProfVirtuaisFile << "--------------------------MOTIVOS DA CRIACAO DE PROFESSORES VIRTUAIS-----------------------------"<<endl;
						saidaMotivosCriacaoProfVirtuaisFile << "-------------------------------------------------------------------------------------------------\n"<<endl;
						saidaMotivosCriacaoProfVirtuaisFile << "=================================================================================================\n"<<endl;
				   }

				   if( flagNroFaltaCadastroNoCampus ){
					    stringstream ss;
						if ( flagNroFaltaCadastroNoCampus == 1 )
							ss << flagNroFaltaCadastroNoCampus << " professor não cadastrado no campus dessa turma.";
						else
							ss << flagNroFaltaCadastroNoCampus << " professores não cadastrados no campus dessa turma.";
						alocacao->addDescricaoDoMotivo( -1, ss.str() );

						saidaMotivosCriacaoProfVirtuaisFile << "PROFESSOR VIRTUAL: " << v0->getProfessor()->getId() <<endl;
						saidaMotivosCriacaoProfVirtuaisFile << "DISCIPLINA: " << v0->getDisciplina()->getId() <<endl;
						saidaMotivosCriacaoProfVirtuaisFile << "TURMA: " << v0->getTurma() <<endl<<endl;
						saidaMotivosCriacaoProfVirtuaisFile << "MOTIVO: " << flagNroFaltaCadastroNoCampus << " professores nao cadastrados no campus dessa turma" <<endl;
						saidaMotivosCriacaoProfVirtuaisFile << "=================================================================================================\n"<<endl;
				   }			   
				   if( flagNroFaltaMagistrado ){
					    stringstream ss;
						if ( flagNroFaltaMagistrado == 1 )
							ss << flagNroFaltaMagistrado << " professor não possui habilitação nesta disciplina.";
						else
							ss << flagNroFaltaMagistrado << " professores não possuem habilitação nesta disciplina.";
						alocacao->addDescricaoDoMotivo( -1, ss.str() );

						saidaMotivosCriacaoProfVirtuaisFile << "PROFESSOR VIRTUAL: " << v0->getProfessor()->getId() <<endl;
						saidaMotivosCriacaoProfVirtuaisFile << "DISCIPLINA: " << v0->getDisciplina()->getId() <<endl;
						saidaMotivosCriacaoProfVirtuaisFile << "TURMA: " << v0->getTurma() <<endl<<endl;
						saidaMotivosCriacaoProfVirtuaisFile << "MOTIVO: " << flagNroFaltaMagistrado << " professores sem MAGISTERIO nesta disciplina" <<endl;
						saidaMotivosCriacaoProfVirtuaisFile << "=================================================================================================\n"<<endl;
				   }			   
				   if( flagNroFaltaMestreDoutor ){
						stringstream ss;
						if ( flagNroFaltaMestreDoutor == 1 )
							ss << flagNroFaltaMestreDoutor << " professor não possui titulação necessária para manter o mínimo de mestres e doutores no curso atendido pela turma.";
						else
							ss << flagNroFaltaMestreDoutor << " professores não possuem titulação necessária para manter o mínimo de mestres e doutores no curso atendido pela turma.";
						alocacao->addDescricaoDoMotivo( -1, ss.str() );

						saidaMotivosCriacaoProfVirtuaisFile << "PROFESSOR VIRTUAL: " << v0->getProfessor()->getId() <<endl;
						saidaMotivosCriacaoProfVirtuaisFile << "DISCIPLINA: " << v0->getDisciplina()->getId() <<endl;
						saidaMotivosCriacaoProfVirtuaisFile << "TURMA: " << v0->getTurma() <<endl<<endl;
						saidaMotivosCriacaoProfVirtuaisFile << "MOTIVO: " << flagNroFaltaMestreDoutor << " professores sem TITULACAO NECESSARIA para manter o MINIMO DE MESTRE E DOUTORES" <<endl<<endl;
						saidaMotivosCriacaoProfVirtuaisFile << "ID TITULACAO NECESSARIA: " << v0->getProfessor()->titulacao->getNome() <<endl;
						saidaMotivosCriacaoProfVirtuaisFile << "=================================================================================================\n"<<endl;
				   }
				   if( flagNroFaltaCargaHoraria ){
						stringstream ss;
						if ( flagNroFaltaCargaHoraria == 1 )
							ss << flagNroFaltaCargaHoraria << " professor excederia a carga horária máxima semanal.";
						else
							ss << flagNroFaltaCargaHoraria << " professores excederiam a carga horária máxima semanal.";
						alocacao->addDescricaoDoMotivo( -1, ss.str() );

						saidaMotivosCriacaoProfVirtuaisFile << "PROFESSOR VIRTUAL: " << v0->getProfessor()->getId() <<endl;
						saidaMotivosCriacaoProfVirtuaisFile << "DISCIPLINA :" << v0->getDisciplina()->getId() <<endl;
						saidaMotivosCriacaoProfVirtuaisFile << "TURMA: " << v0->getTurma() <<endl<<endl;
						saidaMotivosCriacaoProfVirtuaisFile << "MOTIVO: " << flagNroFaltaCargaHoraria << " professores excederiam a sua CARGA HORARIA" << endl <<"         MAXIMA SEMANAL" <<endl<<endl;
						saidaMotivosCriacaoProfVirtuaisFile << "CARGA HORARIA LIVRE NECESSARIA: " << v0->getDisciplina()->getTotalCreditos() <<endl;
						saidaMotivosCriacaoProfVirtuaisFile << "=================================================================================================\n"<<endl;
				   }
				   if( flagNroFaltaHorarioConflito ){				    
						stringstream ss;
						ss << "Para ";
						if ( flagNroFaltaHorarioConflito == 1 )
							ss << flagNroFaltaHorarioConflito << " professor há conflito entre os horários da turma com os horários de outras turmas alocadas.";
						else
							ss << flagNroFaltaHorarioConflito << " professores há conflito entre os horários da turma com os horários de outras turmas alocadas.";
						alocacao->addDescricaoDoMotivo( -1, ss.str() );

	        			saidaMotivosCriacaoProfVirtuaisFile << "PROFESSOR VIRTUAL: " << v0->getProfessor()->getId() <<endl;
						saidaMotivosCriacaoProfVirtuaisFile << "DISCIPLINA: " << v0->getDisciplina()->getId() <<endl;
						saidaMotivosCriacaoProfVirtuaisFile << "TURMA: " << v0->getTurma() <<endl<<endl;
						saidaMotivosCriacaoProfVirtuaisFile << "MOTIVO: HA CONFLITO entre os horarios da turma com os HORARIOS JA ALOCADOS a " << flagNroFaltaHorarioConflito << " professores" <<endl<<endl;
						saidaMotivosCriacaoProfVirtuaisFile << "HorarioAulas COM CONFLITO:" <<endl;

						for(int i=0; i<variableOpsConflitoHorarioIdsHorAulas.size(); i++){
							std::pair<VariableOp, int> elem = variableOpsConflitoHorarioIdsHorAulas[i];
							saidaMotivosCriacaoProfVirtuaisFile << "DIA: " << elem.first.getDia();
							saidaMotivosCriacaoProfVirtuaisFile << "   ID: " << elem.second <<endl;
						}
						saidaMotivosCriacaoProfVirtuaisFile << "=================================================================================================\n"<<endl;
					}
					if( flagNroFaltaHorarioDisponivel && (!variableOpsNaoDisponiveisHorarioIdsHorAulas.empty()) ){
						stringstream ss;
						if ( flagNroFaltaHorarioDisponivel == 1 )
							ss << flagNroFaltaHorarioDisponivel << " professor não possui horários disponíveis para esta turma.";
						else
							ss << flagNroFaltaHorarioDisponivel << " professores não possuem horários disponíveis para esta turma.";
						alocacao->addDescricaoDoMotivo( -1, ss.str() );

						saidaMotivosCriacaoProfVirtuaisFile << "PROFESSOR VIRTUAL: " << v0->getProfessor()->getId() <<endl;
						saidaMotivosCriacaoProfVirtuaisFile << "DISCIPLINA: " << v0->getDisciplina()->getId() <<endl;
						saidaMotivosCriacaoProfVirtuaisFile << "TURMA: " << v0->getTurma() <<endl<<endl;
						saidaMotivosCriacaoProfVirtuaisFile << "MOTIVO: " << flagNroFaltaHorarioDisponivel 
							<< " professores nao possuem horarios disponiveis para esta disciplina" <<endl;
						saidaMotivosCriacaoProfVirtuaisFile << "HorarioAulas NAO DISPONIVEIS:" <<endl;

						std::map<std::pair<int, int>, std::pair<VariableOp, int> >::iterator itrHorDisponiveis =  variableOpsNaoDisponiveisHorarioIdsHorAulas.begin();
						for(;itrHorDisponiveis != variableOpsNaoDisponiveisHorarioIdsHorAulas.end(); itrHorDisponiveis++){
							std::pair<VariableOp, int> elem = itrHorDisponiveis->second;
							saidaMotivosCriacaoProfVirtuaisFile << "DIA: " << elem.first.getDia();
							saidaMotivosCriacaoProfVirtuaisFile << "   ID: " << elem.second <<endl;
						}
						saidaMotivosCriacaoProfVirtuaisFile << "=================================================================================================\n"<<endl;
					}
					if ( flagNroFaltaMaxDiasSemana ){
						stringstream ss;
						if ( flagNroFaltaMaxDiasSemana == 1 )
							ss << flagNroFaltaMaxDiasSemana << " professor já atingiu o máximo de dias da semana.";
						else
							ss << flagNroFaltaMaxDiasSemana << " professores já atingiram o máximo de dias da semana.";
						alocacao->addDescricaoDoMotivo( -1, ss.str() );

						saidaMotivosCriacaoProfVirtuaisFile << "PROFESSOR VIRTUAL: " << v0->getProfessor()->getId() <<endl;
						saidaMotivosCriacaoProfVirtuaisFile << "DISCIPLINA :" << v0->getDisciplina()->getId() <<endl;
						saidaMotivosCriacaoProfVirtuaisFile << "TURMA: " << v0->getTurma() <<endl<<endl;
						saidaMotivosCriacaoProfVirtuaisFile << "MOTIVO: " << flagNroFaltaMaxDiasSemana << " professores ja atingiram o maximo de dias da semana." <<endl<<endl;
						saidaMotivosCriacaoProfVirtuaisFile << "=================================================================================================\n"<<endl;
					}
					if ( flagNroFaltaMinCredsDiarios ){
						stringstream ss;
						ss << "O mínimo de créditos nos dias das aulas da turma não foi atingido para ";
						if ( flagNroFaltaMinCredsDiarios == 1 )
							ss << flagNroFaltaMinCredsDiarios << " professor.";
						else
							ss << flagNroFaltaMinCredsDiarios << " professores.";
						alocacao->addDescricaoDoMotivo( -1, ss.str() );

						saidaMotivosCriacaoProfVirtuaisFile << "PROFESSOR VIRTUAL: " << v0->getProfessor()->getId() <<endl;
						saidaMotivosCriacaoProfVirtuaisFile << "DISCIPLINA :" << v0->getDisciplina()->getId() <<endl;
						saidaMotivosCriacaoProfVirtuaisFile << "TURMA: " << v0->getTurma() <<endl<<endl;
						saidaMotivosCriacaoProfVirtuaisFile << "MOTIVO: O minimo de creditos nos dias das aulas da turma nao foi atingido para " 
							<< flagNroFaltaMinCredsDiarios << " professor." <<endl<<endl;
						saidaMotivosCriacaoProfVirtuaisFile << "=================================================================================================\n"<<endl;
					}
					if ( flagNroFaltaInterjornada ){
						stringstream ss;
						if ( flagNroFaltaInterjornada == 1 )
							ss << flagNroFaltaInterjornada << " professor já atingiu a interjornada mínima.";
						else
							ss << flagNroFaltaInterjornada << " professores já atingiram a interjornada mínima.";
						alocacao->addDescricaoDoMotivo( -1, ss.str() );

						saidaMotivosCriacaoProfVirtuaisFile << "PROFESSOR VIRTUAL: " << v0->getProfessor()->getId() <<endl;
						saidaMotivosCriacaoProfVirtuaisFile << "DISCIPLINA :" << v0->getDisciplina()->getId() <<endl;
						saidaMotivosCriacaoProfVirtuaisFile << "TURMA: " << v0->getTurma() <<endl<<endl;
						saidaMotivosCriacaoProfVirtuaisFile << "MOTIVO: " << flagNroFaltaInterjornada << " professores ja atingiram a interjornada minima." <<endl<<endl;
						saidaMotivosCriacaoProfVirtuaisFile << "=================================================================================================\n"<<endl;
					}
				}
				#pragma endregion

			   #pragma region IMPRIME CAUSAS COMUNS A TODOS OS PROFESSORES
			   if ( ! IMPRIMIR_SOMENTE_RESUMO )
			   {
				   //Gera o relatorio de motivos da criação de professores virtuais
				   if(!escreveMotivos)
				   {
						escreveMotivos = true;
									
						if (!saidaMotivosCriacaoProfVirtuaisFile) {
							cout << "Can't open output file " << saidaMotivosCriacaoProfVirtuaisFileFilename << endl;
							exit(1);
					
						}		
						saidaMotivosCriacaoProfVirtuaisFile << "-------------------------------------------------------------------------------------------------"<<endl;
						saidaMotivosCriacaoProfVirtuaisFile << "--------------------------MOTIVOS DA CRIACAO DE PROFESSORES VIRTUAIS-----------------------------"<<endl;
						saidaMotivosCriacaoProfVirtuaisFile << "-------------------------------------------------------------------------------------------------\n"<<endl;
						saidaMotivosCriacaoProfVirtuaisFile << "=================================================================================================\n"<<endl;
				   }

				   if(flagFaltaCadastroNoCampus == 1){				   
						alocacao->addDescricaoDoMotivo( -1, "Não existe professor cadastrado no campus dessa turma." );

						saidaMotivosCriacaoProfVirtuaisFile << "PROFESSOR VIRTUAL: " << v0->getProfessor()->getId() <<endl;
						saidaMotivosCriacaoProfVirtuaisFile << "DISCIPLINA: " << v0->getDisciplina()->getId() <<endl;
						saidaMotivosCriacaoProfVirtuaisFile << "TURMA: " << v0->getTurma() <<endl<<endl;
						saidaMotivosCriacaoProfVirtuaisFile << "MOTIVO: NAO EXISTE professor no campus dessa turma" <<endl;
						saidaMotivosCriacaoProfVirtuaisFile << "=================================================================================================\n"<<endl;
				   }			   
				   if(flagFaltaMagistrado == 1){				  
					 //   string eh;
						//char ch = 130;
						//eh.push_back( ch );

						alocacao->addDescricaoDoMotivo( -1, "Não existe professor com habilitação nesta disciplina." );

						saidaMotivosCriacaoProfVirtuaisFile << "PROFESSOR VIRTUAL: " << v0->getProfessor()->getId() <<endl;
						saidaMotivosCriacaoProfVirtuaisFile << "DISCIPLINA: " << v0->getDisciplina()->getId() <<endl;
						saidaMotivosCriacaoProfVirtuaisFile << "TURMA: " << v0->getTurma() <<endl<<endl;
						saidaMotivosCriacaoProfVirtuaisFile << "MOTIVO: NAO EXISTE professor com MAGISTERIO nesta disciplina" <<endl;
						saidaMotivosCriacaoProfVirtuaisFile << "=================================================================================================\n"<<endl;
				   }			   
				   if(flagFaltaMestreDoutor == 1){
						alocacao->addDescricaoDoMotivo( -1, "Não existe professor com titulação necessária para manter o mínimo de mestres e doutores no curso atendido pela turma." );

						saidaMotivosCriacaoProfVirtuaisFile << "PROFESSOR VIRTUAL: " << v0->getProfessor()->getId() <<endl;
						saidaMotivosCriacaoProfVirtuaisFile << "DISCIPLINA: " << v0->getDisciplina()->getId() <<endl;
						saidaMotivosCriacaoProfVirtuaisFile << "TURMA: " << v0->getTurma() <<endl<<endl;
						saidaMotivosCriacaoProfVirtuaisFile << "MOTIVO: NAO EXISTE professor com TITULACAO NECESSARIA para manter o MINIMO DE MESTRE E DOUTORES" <<endl<<endl;
						saidaMotivosCriacaoProfVirtuaisFile << "ID TITULACAO NECESSARIA: " << v0->getProfessor()->titulacao->getNome() <<endl;
						saidaMotivosCriacaoProfVirtuaisFile << "=================================================================================================\n"<<endl;
				   }
				   if(flagFaltaCargaHoraria == 1){
						alocacao->addDescricaoDoMotivo( -1, "Não existe professor capaz de ministrar esta turma sem exceder a sua carga horaria maxima semanal." );

						saidaMotivosCriacaoProfVirtuaisFile << "PROFESSOR VIRTUAL: " << v0->getProfessor()->getId() <<endl;
						saidaMotivosCriacaoProfVirtuaisFile << "DISCIPLINA :" << v0->getDisciplina()->getId() <<endl;
						saidaMotivosCriacaoProfVirtuaisFile << "TURMA: " << v0->getTurma() <<endl<<endl;
						saidaMotivosCriacaoProfVirtuaisFile << "MOTIVO: NAO EXISTE professor capaz de ministrar esta turma sem exceder a sua CARGA HORARIA" << endl <<"         MAXIMA SEMANAL" <<endl<<endl;
						saidaMotivosCriacaoProfVirtuaisFile << "CARGA HORARIA LIVRE NECESSARIA: " << v0->getDisciplina()->getTotalCreditos() <<endl;
						saidaMotivosCriacaoProfVirtuaisFile << "=================================================================================================\n"<<endl;
				   }
				   if(flagFaltaHorarioConflito == 1){
						alocacao->addDescricaoDoMotivo( -1, "Há conflito entre os horários da turma com os horários de outras turmas alocadas aos professores." );

	        			saidaMotivosCriacaoProfVirtuaisFile << "PROFESSOR VIRTUAL: " << v0->getProfessor()->getId() <<endl;
						saidaMotivosCriacaoProfVirtuaisFile << "DISCIPLINA: " << v0->getDisciplina()->getId() <<endl;
						saidaMotivosCriacaoProfVirtuaisFile << "TURMA: " << v0->getTurma() <<endl<<endl;
						saidaMotivosCriacaoProfVirtuaisFile << "MOTIVO: HA CONFLITO entre os horarios da turma com os HORARIOS JA ALOCADOS aos professores" <<endl<<endl;
						saidaMotivosCriacaoProfVirtuaisFile << "HorarioAulas COM CONFLITO:" <<endl;

						for(int i=0; i<variableOpsConflitoHorarioIdsHorAulas.size(); i++){
							std::pair<VariableOp, int> elem = variableOpsConflitoHorarioIdsHorAulas[i];
							saidaMotivosCriacaoProfVirtuaisFile << "DIA: " << elem.first.getDia();
							saidaMotivosCriacaoProfVirtuaisFile << "   ID: " << elem.second <<endl;
						}
						saidaMotivosCriacaoProfVirtuaisFile << "=================================================================================================\n"<<endl;
					}
					if( (flagFaltaHorarioDisponivel == 1) && (!variableOpsNaoDisponiveisHorarioIdsHorAulas.empty()) ){
						alocacao->addDescricaoDoMotivo( -1, "Não há horarios disponiveis para esta turma." );

						saidaMotivosCriacaoProfVirtuaisFile << "PROFESSOR VIRTUAL: " << v0->getProfessor()->getId() <<endl;
						saidaMotivosCriacaoProfVirtuaisFile << "DISCIPLINA: " << v0->getDisciplina()->getId() <<endl;
						saidaMotivosCriacaoProfVirtuaisFile << "TURMA: " << v0->getTurma() <<endl<<endl;
						saidaMotivosCriacaoProfVirtuaisFile << "MOTIVO: NAO HA HORARIOS DISPONIVEIS para esta disciplina, ou seja, horarios "<< endl <<"      sem inicio e termino iguais" <<endl<<endl;
						saidaMotivosCriacaoProfVirtuaisFile << "HorarioAulas NAO DISPONIVEIS:" <<endl;

						std::map<std::pair<int, int>, std::pair<VariableOp, int> >::iterator itrHorDisponiveis =  variableOpsNaoDisponiveisHorarioIdsHorAulas.begin();
						for(;itrHorDisponiveis != variableOpsNaoDisponiveisHorarioIdsHorAulas.end(); itrHorDisponiveis++){
							std::pair<VariableOp, int> elem = itrHorDisponiveis->second;
							saidaMotivosCriacaoProfVirtuaisFile << "DIA: " << elem.first.getDia();
							saidaMotivosCriacaoProfVirtuaisFile << "   ID: " << elem.second <<endl;
						}
						saidaMotivosCriacaoProfVirtuaisFile << "=================================================================================================\n"<<endl;
					}
					if ( flagFaltaMaxDiasSemana > 0 ){
						alocacao->addDescricaoDoMotivo( -1, "Todos os professores já atingiram o máximo de dias da semana." );

						saidaMotivosCriacaoProfVirtuaisFile << "PROFESSOR VIRTUAL: " << v0->getProfessor()->getId() <<endl;
						saidaMotivosCriacaoProfVirtuaisFile << "DISCIPLINA :" << v0->getDisciplina()->getId() <<endl;
						saidaMotivosCriacaoProfVirtuaisFile << "TURMA: " << v0->getTurma() <<endl<<endl;
						saidaMotivosCriacaoProfVirtuaisFile << "MOTIVO: Todos os professores ja atingiram o maximo de dias da semana." <<endl<<endl;
						saidaMotivosCriacaoProfVirtuaisFile << "=================================================================================================\n"<<endl;
					}
					if ( flagFaltaMinCredsDiarios > 0 ){
						alocacao->addDescricaoDoMotivo( -1, "O mínimo de créditos nos dias das aulas da turma não foi atingido para nenhum professor." );

						saidaMotivosCriacaoProfVirtuaisFile << "PROFESSOR VIRTUAL: " << v0->getProfessor()->getId() <<endl;
						saidaMotivosCriacaoProfVirtuaisFile << "DISCIPLINA :" << v0->getDisciplina()->getId() <<endl;
						saidaMotivosCriacaoProfVirtuaisFile << "TURMA: " << v0->getTurma() <<endl<<endl;
						saidaMotivosCriacaoProfVirtuaisFile << "MOTIVO: O minimo de creditos nos dias das aulas da turma nao foi atingido para nenhum professor." <<endl<<endl;
						saidaMotivosCriacaoProfVirtuaisFile << "=================================================================================================\n"<<endl;
					}
					if ( flagFaltaInterjornada == 1 ){
						alocacao->addDescricaoDoMotivo( -1, "Todos os professores já atingiram a interjornada mínima." );

						saidaMotivosCriacaoProfVirtuaisFile << "PROFESSOR VIRTUAL: " << v0->getProfessor()->getId() <<endl;
						saidaMotivosCriacaoProfVirtuaisFile << "DISCIPLINA :" << v0->getDisciplina()->getId() <<endl;
						saidaMotivosCriacaoProfVirtuaisFile << "TURMA: " << v0->getTurma() <<endl<<endl;
						saidaMotivosCriacaoProfVirtuaisFile << "MOTIVO: Todos os professores ja atingiram a interjornada minima." <<endl<<endl;
						saidaMotivosCriacaoProfVirtuaisFile << "=================================================================================================\n"<<endl;
					}
				}
				#pragma endregion
			}
		}
	}
		
	if ( saidaTrocasFile )
		saidaTrocasFile.close();
	if ( saidaAlteracoesFile )
		saidaAlteracoesFile.close();
	if ( saidaMotivosCriacaoProfVirtuaisFile )
		saidaMotivosCriacaoProfVirtuaisFile.close();

}


void Operacional::separaProfsVirtuais()
{
	// Retira os profs virtuais infiltrados na lista de reais e os coloca em professores_virtuais
	
	ITERA_GGROUP_LESSPTR( itCp, problemData->campi, Campus )
	{
		GGroup<Professor*, LessPtr<Professor>> removeList;

		ITERA_GGROUP_LESSPTR( itProf, itCp->professores, Professor )
		{
			if ( itProf->eVirtual() ) 
			{
				removeList.add( *itProf );
			}
		}

		ITERA_GGROUP_LESSPTR( itProf, removeList, Professor )
		{
			itCp->professores.remove( *itProf );
		}
	}
}

void Operacional::polishOperacional(double *xSol, double maxTime, int percIni, int percMin, double maxTempoSemMelhora)
{
   // Adiciona restrição de local branching
   int status = 0;
   int nIter = 0;
   int * idxSol = new int[ lp->getNumCols() ];
   double *ubVars = new double[ lp->getNumCols() ];
   double *lbVars = new double[ lp->getNumCols() ];

   for ( int i = 0; i < lp->getNumCols(); i++ )
   {
      idxSol[ i ] = i;
   }
   
   double objAtual = 100000000000.0;
   double okIter = true;

   CPUTimer tempoPol;
   tempoPol.reset();
   tempoPol.start();

   CPUTimer tempoSemMelhora;
   tempoSemMelhora.reset();
   tempoSemMelhora.start();

   srand(123);

   lp->getUB(0,lp->getNumCols()-1,ubVars);
   lp->getLB(0,lp->getNumCols()-1,lbVars);

   int tempoIter = 300;
   int perc = percIni;
   int *idxs = new int[lp->getNumCols()*2];
   double *vals = new double[lp->getNumCols()*2];
   BOUNDTYPE *bds = new BOUNDTYPE[lp->getNumCols()*2];
   int nBds = 0;

// ------------------------------------------------------------
// Procura rapidamente a solução exata, caso já se esteja perto do ótimo

#ifdef SOLVER_CPLEX
      lp->updateLP();
      lp->setNodeLimit( 100000000 );
      lp->setTimeLimit( 1800 );
      lp->setMIPRelTol( 0.01 );
      lp->setPolishAfterNode(1);
      lp->setPolishAfterTime(100000000);
      lp->setMIPEmphasis( 4 );
      lp->setNoCuts();
      lp->setVarSel(4);
      lp->setNodeLimit(200);
      lp->setHeurFrequency( 1.0 );
#endif
#ifdef SOLVER_GUROBI
      lp->updateLP();
      lp->setNodeLimit( 100000000 );
      lp->setTimeLimit( 1800 );
      lp->setMIPRelTol( 0.1 );
      lp->setMIPEmphasis( 0 );
      lp->setNoCuts();
      lp->setVarSel(4);
      lp->setNodeLimit(200);
      lp->setHeurFrequency( 1.0 );
      lp->updateLP();
#endif

      lp->copyMIPStartSol( lp->getNumCols(), idxSol, xSol );
      lp->updateLP();
	  
      status = lp->optimize( METHOD_MIP );	  
      lp->getX( xSol );
	  
	  if ( lp->getMIPGap() * 100 < 0.01 )
	  {
		  okIter = false;
		  cout << "\nPolish desnecessario, gap =" << lp->getMIPGap() * 100 << std::endl;
	  }

// ------------------------------------------------------------


   while (okIter)
   {
      VariableOpHash::iterator vit = vHashOp.begin();
      vit = vHashOp.begin();

      nBds = 0;
      while ( vit != vHashOp.end() )
      {
         if ( vit->first.getType() == VariableOp::V_X_PROF_AULA_HOR )
         {
               if ( rand() % 100 >= perc  )
               {
                  vit++;
                  continue;
               }

               if ( xSol[vit->second] > 0.1 )
               {
                  idxs[nBds] = vit->second;
                  vals[nBds] = (int)(xSol[vit->second]+0.5);
                  bds[nBds] = BOUNDTYPE::BOUND_UPPER;
                  nBds++;
                  idxs[nBds] = vit->second;
                  vals[nBds] = (int)(xSol[vit->second]+0.5);
                  bds[nBds] = BOUNDTYPE::BOUND_LOWER;
                  nBds++;
               }
               else
               {
                  idxs[nBds] = vit->second;
                  vals[nBds] = 0.0;
                  bds[nBds] = BOUNDTYPE::BOUND_UPPER;
                  nBds++;
                  lp->chgUB(vit->second,0.0);
               }
         }

         vit++;
      }

      lp->chgBds(nBds,idxs,bds,vals);
      lp->updateLP();

#ifdef SOLVER_CPLEX
      lp->updateLP();
      lp->setNodeLimit( 100000000 );
      lp->setTimeLimit( tempoIter );
      lp->setMIPRelTol( 0.01 );
      lp->setPolishAfterNode(1);
      lp->setPolishAfterTime(100000000);
      lp->setMIPEmphasis( 4 );
      lp->setNoCuts();
      lp->setVarSel(4);
      lp->setNodeLimit(200);
      lp->setHeurFrequency( 1.0 );
#endif
#ifdef SOLVER_GUROBI
      lp->updateLP();
      lp->setNodeLimit( 100000000 );
      lp->setTimeLimit( tempoIter );
      lp->setMIPRelTol( 0.1 );
      lp->setMIPEmphasis( 0 );
      lp->setNoCuts();
      lp->setVarSel(4);
      lp->setNodeLimit(200);
      lp->setHeurFrequency( 1.0 );
      lp->updateLP();
#endif
      lp->copyMIPStartSol( lp->getNumCols(), idxSol, xSol );
      lp->updateLP();

      printf("POLISH COM PERC = %i, TEMPOITER = %i\n",perc,tempoIter);
      fflush(0);

      status = lp->optimize( METHOD_MIP );

      double objN = lp->getObjVal();

      lp->getX( xSol );

      if ( fabs(objN-objAtual) < 0.0001 && perc < percMin )
      {
            perc = percIni-40;
            tempoIter = 500;
      }
      else if ( fabs(objN-objAtual) < 0.0001 && perc >= percMin)
      {
            perc -= 5;
            lp->setCuts(1);
            tempoIter += 50;
      }

	  if ( fabs(objN-objAtual) < 0.0001 )
	  {
		  /* no improvement */
		  tempoSemMelhora.stop();
		  double tempoAtual = tempoSemMelhora.getCronoTotalSecs();
		  tempoSemMelhora.start();
		  if ( tempoAtual >= maxTempoSemMelhora )
		  {
			 /* if there is too much time without any improvement, then quit */
			 okIter = false;
			 tempoSemMelhora.stop();
			 tempoPol.stop();
			 cout << "Abort by timeWithoutChange. Limit of time without improvement" << tempoAtual << ", BestObj " << objN;
		  }
	  }
	  else
	  {
		  tempoSemMelhora.stop();
		  tempoSemMelhora.reset();
		  tempoSemMelhora.start();		
	  }

      objAtual = objN;
	  
	  if ( okIter )
      {
		  tempoPol.stop();
		  double tempoAtual = tempoPol.getCronoTotalSecs();
		  tempoPol.start();
		  if ( tempoAtual >= maxTime )
		  {
			 okIter = false;
			 tempoPol.stop();
			 tempoSemMelhora.stop();
		  }
	  }
	  
      // Volta bounds
      vit = vHashOp.begin();

      nBds = 0;
      while ( vit != vHashOp.end() )
      {
         if ( vit->first.getType() == VariableOp::V_X_PROF_AULA_HOR )
         {
            idxs[nBds] = vit->second;
            vals[nBds] = ubVars[vit->second];
            bds[nBds] = BOUNDTYPE::BOUND_UPPER;
            nBds++;
            idxs[nBds] = vit->second;
            vals[nBds] = lbVars[vit->second];
            bds[nBds] = BOUNDTYPE::BOUND_LOWER;
            nBds++;
         }
         vit++;
      }
	  
      lp->chgBds(nBds,idxs,bds,vals);
      lp->updateLP();

#ifdef SOLVER_CPLEX
      lp->setPolishAfterNode(1);
      lp->setMIPRelTol( 0.0 );
#endif
   }


	// -------------------------------------------------------------
    // Garante que não dará erro se houver um getX depois desse polish,
    // já que o lp sobre alteração nos bounds no final.
    lp->copyMIPStartSol( lp->getNumCols(), idxSol, xSol );
	lp->setTimeLimit( 50 );
	status = lp->optimize( METHOD_MIP );
	lp->getX(xSol);
	// -------------------------------------------------------------


   delete [] idxSol;
   delete [] ubVars;
   delete [] lbVars;
   delete [] idxs;
   delete [] vals;
   delete [] bds;
}

void Operacional::clearModel()
{	
	vHashOp.clear();
	cHashOp.clear();
	vars_prof_aula2.clear();
	vars_prof_dia_fase_dt.clear();
	varsProfDiaFaseHiHf.clear();
	varsProfFolgaGap.clear();
}

void Operacional::setOptLogFile(std::ofstream &logMip, string name, bool clear)
{
   stringstream ss;
   ss << name << ".log";
   
   if (!clear)
	   logMip.open(ss.str(),ios::app);
   else
	   logMip.open(ss.str(),ios::out);
   
   if (lp)
	   lp->setLogFile((char*)ss.str().c_str());
}

int Operacional::solveOperacionalMIP()
{
   int varNum = 0;
   int constNum = 0;
   
   cout <<"\n\nOperacional::solveOperacionalMIP";
   cout<<"\nthis->CARREGA_SOLUCAO = " << *this->CARREGA_SOLUCAO << "\n";

   bool CARREGA_SOL_PARCIAL = * this->CARREGA_SOLUCAO;
   
   if ( * this->CARREGA_SOLUCAO )
   {
	   char solName[1024];
	   strcpy( solName, getSolOpBinFileName( -1, 0 ).c_str() );
	   FILE* fin = fopen( solName,"rb");
	   if ( fin == NULL )
	   {
		   std::cout << "\nA partir de " << solName << " , nao foram lidas mais solucoes.\n"; fflush(NULL);
		   *CARREGA_SOLUCAO = false;
	   }
	   else
	   {
		  std::cout << "\nArquivo-solucao " << solName << " encontrado.\n"; fflush(NULL);
		  fclose(fin);
	   }
   }

   if ( lp != NULL )
   {
      lp->freeProb();
      delete lp;
#ifdef SOLVER_CPLEX
	   lp = new OPT_CPLEX; 
#endif
#ifdef SOLVER_GUROBI
	   lp = new OPT_GUROBI; 
#endif
   }

    #pragma region LOG FILE
   // set log file
   static int id=0;
   id++;
   stringstream ss;
   ss << "mipOp" << id;
   ofstream opFile;
   setOptLogFile(opFile,ss.str());
	#pragma endregion

   clearModel();

   lp->createLP( "SolverOperacional",
      OPTSENSE_MINIMIZE, PROB_MIP );

#ifdef DEBUG
   cout << "Creating LP...\n";
#endif

   relacionaProfessoresDisciplinas();

   // Variable creation
   varNum = criaVariaveisOperacional();

	#ifdef PRINT_cria_variaveis
   cout << "Total of Variables: " << varNum << "\n\n";
	#endif
    
    int status = 1;

	if ( ! (* this->CARREGA_SOLUCAO) )
	{
		// Constraint creation
		constNum = criaRestricoesOperacional();

		#ifdef PRINT_cria_restricoes
		cout << "Total of Constraints: " << constNum << "\n\n";
		#endif

		#ifdef DEBUG
		 //  lp->writeProbLP( "SolverOperacional" );
		#endif
		   
		const int nCols = lp->getNumCols();

		double *x = new double[ nCols ];
		int *idxN = new int[ nCols ];
		double *obj = new double[ nCols ];
				   
		lp->getObj( 0, nCols-1, obj );
				
		VariableOpHash::iterator vit;
		for ( vit = vHashOp.begin(); vit != vHashOp.end(); vit++ )
		{   				   
			idxN[ vit->second ] = vit->second;
		}

		// RODADA 1 NÃO POSSUI FOLGA DE DEMANDA => OBRIGA TOTAL ATENDIMENTO

		bool SEM_FOLGA_DEMANDA = ( this->getRodada() == Operacional::OP_VIRTUAL_PERFIL )? true : false;
			
		// --------------------------------------------------------------
		// Garantia de solucao

		 std::cout<<"\n------------------------------------";
		 std::cout<<"\nGarantia de solucao\n\n"; fflush(NULL);

		 if(opFile){
			opFile.seekp(0, ios::end);
			opFile<<"\n------------------------------------";
			opFile<<"\nGarantia de solucao\n\n";
			opFile.flush();
		 }

         if ( SEM_FOLGA_DEMANDA )		// primeira rodada
         {
			 int success = solveGaranteTotalAtendHorInicial( CARREGA_SOL_PARCIAL, x );

			 if ( !success )
			 {
				double *newObj = new double[lp->getNumCols()];
				int *newIdx = new int[lp->getNumCols()];
				for (int i=0; i < lp->getNumCols(); i++)
				{
				   newObj[i] = 0;
				   newIdx[i] = i;
				}

				lp->chgObj(lp->getNumCols(),newIdx,newObj);
				lp->setNumIntSols(1);

				status = lp->optimize( METHOD_MIP );
				lp->getX(x);

				lp->chgObj(lp->getNumCols(),newIdx,obj);

				lp->copyMIPStartSol(lp->getNumCols(),newIdx,x);	
            
				delete[] newObj;
				delete[] newIdx;
			 }
         }
		 else							// segunda rodada
         {			 
			 // Atendimento com horarios da solucao tatica
			 int success=false;

			 if ( FIXA_HOR_SOL_TATICO )
				success = solveGaranteTotalAtendHorInicial( CARREGA_SOL_PARCIAL, x );

			 if ( !success )
			 {
				// ======================================================================	
				// Atendimento zerado
			  
				 #pragma region GARANTIA DE SOLUCAO: ATENDIMENTO ZERADO
				 if(opFile){
					 opFile.seekp(0, ios::end);
					 opFile<<"\n------------------------------------";
					 opFile<<"\nAtendimento minimon\n";
					 opFile.flush();
				 }
				 std::cout<<"\nAtendimento minimo\n\n"; fflush(NULL);

				double *origBounds = new double[lp->getNumCols()];
				int *idx = new int[lp->getNumCols()];
				BOUNDTYPE *bType = new BOUNDTYPE[lp->getNumCols()];
				int nChgs = 0;

				vit = vHashOp.begin();
				for (; vit != vHashOp.end(); vit++ )
				{
				   VariableOp v = vit->first;
				   int col = vit->second;
				   int lb = (int) (lp->getLB(col) + 0.5);
				   int ub = (int) (lp->getUB(col) + 0.5);			       
			   			   			
				   if( v.getType() == VariableOp::V_X_PROF_AULA_HOR && lb != ub  )
				   {
					   origBounds[nChgs] = ub;
					   bType[nChgs] = BOUNDTYPE::BOUND_UPPER;				   
					   idx[nChgs] = vit->second;
					   nChgs++;

					   lp->chgUB(vit->second,0.0);
				   }
				}
				lp->updateLP();   

				lp->setNumIntSols(1);

				#ifdef PRINT_LOGS
					lp->writeProbLP( this->getOpLpFileName(0).c_str() );
				#endif            
            
				if ( CARREGA_SOL_PARCIAL )
				{
				   // procura e carrega solucao parcial
				   int statusReadBin = readOpSolBin( OutPutFileType::OP_BIN0, x, 0 );
				   if ( !statusReadBin )
				   {
					  CARREGA_SOL_PARCIAL=false;
				   }
				   else writeOpSolTxt( OutPutFileType::OP_BIN0, x, 0 );
				}
				if ( !CARREGA_SOL_PARCIAL )
				{
				   // GENERATES SOLUTION
				   status = lp->optimize( METHOD_MIP );
				   lp->getX(x);
				   fflush(NULL);

				   writeOpSolBin( OutPutFileType::OP_BIN0, x, 0 );
				   writeOpSolTxt( OutPutFileType::OP_BIN0, x, 0 );
				}		   		

				// Volta as variáveis que haviam sido fixadas
				lp->chgBds( nChgs, idx, bType, origBounds );			
			
				lp->copyMIPStartSol(lp->getNumCols(),idxN,x);            
				lp->updateLP();

				delete [] origBounds;
				delete [] bType;

				#pragma endregion
			
				// ======================================================================	
				// Solução inicial da etapa anterior

				#pragma region ATENDIMENTO COM PROFS REAIS DA PRIMEIRA ETAPA

				if(opFile){
				 opFile.seekp(0, ios::end);
				 opFile<<"\n------------------------------------";
				 opFile<<"\nAtendimento com profs reais da etapa anterior\n";
				 opFile.flush();
				}
				std::cout<<"\nAtendimento com profs reais da etapa anterior\n\n"; fflush(NULL);
			
				nChgs = 0;
				double *values = new double[lp->getNumCols()];
				vit = vHashOp.begin();
				for (; vit != vHashOp.end(); vit++ )
				{
				   VariableOp v = vit->first;
				   int col = vit->second;
				   int lb = (int) (lp->getLB(col) + 0.5);
				   int ub = (int) (lp->getUB(col) + 0.5);			       

				   if ( v.getType() == VariableOp::V_X_PROF_AULA_HOR )
				   if ( ! v.getProfessor()->eVirtual() && this->inSolution( v ) )
				   {
					   values[nChgs] = 1.0;			   
					   idx[nChgs] = vit->second;
					   nChgs++;		
				   }
				}
			
				lp->setAdvance(2);
				int cpySolStatus = lp->copyMIPStartSol( nChgs, idx, values );
            
				delete [] values;
				delete [] idx;

				if ( !cpySolStatus )
					std::cout<<"\n\ncpySolStatus = error" << endl;
				else
					std::cout<<"\n\ncpySolStatus = successful" << endl;
				fflush(NULL);

				#pragma endregion 

			   // ======================================================================	
			   // Maximo atendimento
		   
				if ( !FIXA_HOR_SOL_TATICO && 0 )
				{
					solveMaxAtendPorFasesDoDia( CARREGA_SOL_PARCIAL, x );
					lp->copyMIPStartSol(lp->getNumCols(),idxN,x);
				}
				else
				{
				   #pragma region MAXIMO ATENDIMENTO UNICO
				   std::cout<<"\n------------------------------------";
				   std::cout<<"\nMaximo atendimento\n\n"; fflush(NULL);
		
				   if(opFile){
					opFile.flush();
					opFile<<"\n------------------------------------";
					opFile<<"\nMaximo atendimento\n";
				   }

				   // Função Objetivo somente com folga de demanda
				   vit = vHashOp.begin();
				   for (; vit != vHashOp.end(); vit++ )
				   {
						VariableOp v = vit->first;
						if(v.getType() != VariableOp::V_FOLGA_DEMANDA)
						{
						   lp->chgObj(vit->second,0.0);
						}
						else
						{
						   lp->chgObj(vit->second,1.0);
						}
				   }
		   
					lp->setMIPEmphasis(1);
					lp->setCuts(0);
					lp->setNodeLimit(10000000000);
					lp->setPolishAfterTime( this->getTimeLimit(Solver::OP2) / 2 );
					lp->setTimeLimit( this->getTimeLimit(Solver::OP2) );
					lp->setNumIntSols(0);
			
					#if defined SOLVER_GUROBI && defined USAR_CALLBACK
					cb_data.timeLimit = this->getMaxTimeNoImprov(Solver::OP2);		
					cb_data.gapMax = 60;
					lp->setCallbackFunc( &timeWithoutChangeCallback, &cb_data );
					#endif

					lp->updateLP();

					#ifdef PRINT_LOGS
					lp->writeProbLP( this->getOpLpFileName(2).c_str() );		
					#endif
	

					if ( CARREGA_SOL_PARCIAL )
					{
						// procura e carrega solucao parcial
						int statusReadBin = readOpSolBin( OutPutFileType::OP_BIN2, x, 0 );
						if ( !statusReadBin )
						{
							CARREGA_SOL_PARCIAL=false;
						}
						else writeOpSolTxt( OutPutFileType::OP_BIN2, x, 0 );
					}
					if ( !CARREGA_SOL_PARCIAL )
					{
						// GENERATES SOLUTION
						status = lp->optimize( METHOD_MIP );
						lp->getX(x);
						fflush(NULL);
		
						writeOpSolBin( OutPutFileType::OP_BIN2, x, 0 );
						writeOpSolTxt( OutPutFileType::OP_BIN2, x, 0 );
					}		   
			
					// Fixa maximo atendimento
					vit = vHashOp.begin();
					for (; vit != vHashOp.end(); vit++ )
					{
						VariableOp v = vit->first;
						if(v.getType() == VariableOp::V_FOLGA_DEMANDA  && x[vit->second] < 0.1)
							lp->chgUB(vit->second, 0.0);
					}
						
					lp->copyMIPStartSol(lp->getNumCols(),idxN,x);
					#pragma endregion			
				}

			}

			#pragma region MIN VIRTUAIS

			std::cout<<"\n------------------------------------";
			std::cout<<"\nUso mínimo de professores virtuais\n\n"; fflush(NULL);
		
			if(opFile){
				opFile.seekp(0, ios::end);
				opFile<<"\n------------------------------------";
				opFile<<"\nUso mínimo de professores virtuais\n";
				opFile.flush();
			}

			// Função Objetivo somente com variaveis de professor virtual
			vit = vHashOp.begin();
			for (; vit != vHashOp.end(); vit++ )
			{
				VariableOp v = vit->first;
				if(v.getType() == VariableOp::V_PROF_VIRTUAL)
				{
					lp->chgObj( vit->second, 1.0 );
				}
				else
				{
					lp->chgObj( vit->second, 0.0 );
				}
			}
		
			lp->updateLP();

			#ifdef PRINT_LOGS
				lp->writeProbLP( this->getOpLpFileName(3).c_str() );
			#endif
						
			if ( CARREGA_SOL_PARCIAL )
			{
				// procura e carrega solucao parcial
				int statusReadBin = readOpSolBin( OutPutFileType::OP_BIN3, x, 0 );
				if ( !statusReadBin )
				{
					CARREGA_SOL_PARCIAL=false;
				}
				else writeOpSolTxt( OutPutFileType::OP_BIN3, x, 0 );
			}
			if ( !CARREGA_SOL_PARCIAL )
			{				
				lp->setMIPEmphasis(1);
				lp->setCuts(0);
				lp->setNodeLimit(10000000000);
				lp->setNumIntSols(0);
				lp->setTimeLimit( this->getTimeLimit(Solver::OP3) );
				lp->setPolishAfterTime( this->getTimeLimit(Solver::OP3)/2 );			
				#if defined SOLVER_GUROBI && defined USAR_CALLBACK
				cb_data.timeLimit = this->getMaxTimeNoImprov(Solver::OP3);		
				cb_data.gapMax = 50;
				lp->setCallbackFunc( &timeWithoutChangeCallback, &cb_data );
				#endif

				if ( !FIXA_HOR_SOL_TATICO )
				{
					#if !defined (DEBUG) && !defined (TESTE)
						//polishOperacional(x, 3600, 90, 15, 1800);
						//writeOpSolBin( OutPutFileType::OP_BIN3, x, 0 );
						//writeOpSolTxt( OutPutFileType::OP_BIN3, x, 0 );
					#endif

					lp->setTimeLimit( 3600 );
					lp->setPolishAfterTime( 2000 );
					status = lp->optimize( METHOD_MIP );
					lp->getX(x);
				}
				else
				{
					// GENERATES SOLUTION
					lp->setTimeLimit( 1000 );
					lp->setPolishAfterTime( 100000000 );
					status = lp->optimize( METHOD_MIP );
					lp->getX(x);

					#if !defined (DEBUG) && !defined (TESTE)
					//	polishOperacional(x, 3600, 80, 20, 1800);
					//	writeOpSolBin( OutPutFileType::OP_BIN3, x, 0 );
					//	writeOpSolTxt( OutPutFileType::OP_BIN3, x, 0 );
					#endif

					// GENERATES SOLUTION
					lp->setTimeLimit( 2000 );
					lp->setPolishAfterTime( 1000 );
					status = lp->optimize( METHOD_MIP );
					lp->getX(x);
				}
				fflush(NULL);

				writeOpSolBin( OutPutFileType::OP_BIN3, x, 0 );
				writeOpSolTxt( OutPutFileType::OP_BIN3, x, 0 );
			}

			// Fixa máximo de profs virtuais usados e máximo de aulas alocadas com profs virtuais
			vit = vHashOp.begin();
			for (; vit != vHashOp.end(); vit++ )
			{
				VariableOp v = vit->first;
				if(v.getType() == VariableOp::V_PROF_VIRTUAL && x[vit->second] < 0.1)
					lp->chgUB(vit->second, 0.0);
				if(v.getType() == VariableOp::V_Y_PROF_DISCIPLINA  && x[vit->second] < 0.1)
					if (v.getProfessor()->eVirtual())
						lp->chgUB(vit->second, 0.0);
			}
			
			// Volta pesos originais
			lp->chgObj( nCols, idxN, obj );
					
			lp->copyMIPStartSol(lp->getNumCols(),idxN,x);
			
			#pragma endregion

			// -------------------------------------------------------------
		}
		
		std::cout<<"\n------------------------------------";
		if ( SEM_FOLGA_DEMANDA )	// RODADA 1
			std::cout<<"\nFO original\n\n";
		else						// RODADA 2
			std::cout<<"\nMaximo atendimento e Minimo prof virtual fixados e FO original\n\n";
		
		if(opFile)
		{
			opFile.seekp(0, ios::end);
			opFile<<"\n------------------------------------";
			if ( SEM_FOLGA_DEMANDA )	// RODADA 1
				opFile<<"\nFO original\n\n";
			else
				opFile<<"\nMaximo atendimento e Minimo prof virtual fixados e FO original\n\n";
			opFile.flush();
		}

		fflush(NULL);
		
		lp->updateLP();

		#ifdef PRINT_LOGS
			lp->writeProbLP( this->getOpLpFileName(-1).c_str() );
		#endif		
		
		lp->setMIPEmphasis(0);
        lp->setCuts(0);
		if ( SEM_FOLGA_DEMANDA ){
			lp->setMIPEmphasis(0);
			lp->setTimeLimit( 3000 );
			lp->setPolishAfterTime(1500);
		}
		else{
			lp->setTimeLimit( this->getTimeLimit(Solver::OP) );
			lp->setPolishAfterTime(3600);
		}
		lp->setNumIntSols(0);
      lp->setHeurFrequency(0.5);
      lp->setPreSolveIntensity(OPT_LEVEL2);
		lp->setNodeLimit(1000000000);   
      lp->setCuts(-1);
		
		#if defined SOLVER_GUROBI && defined USAR_CALLBACK
		//cb_data.timeLimit = this->getMaxTimeNoImprov(Solver::OP);		
      cb_data.timeLimit = 300;
		cb_data.gapMax = 15;
		lp->setCallbackFunc( &timeWithoutChangeCallback, &cb_data );
		#endif
				
		if ( CARREGA_SOL_PARCIAL )
		{
			// procura e carrega solucao parcial
			int statusReadBin = readOpSolBin( OutPutFileType::OP_BIN, x, 0 );
			if ( !statusReadBin )
			{
				CARREGA_SOL_PARCIAL=false;
			}
			else{ 
				writeOpSolTxt( OutPutFileType::OP_BIN, x, 0 );
				
				//// marreta: sol inicial, continua a otimizar
				//CARREGA_SOL_PARCIAL=false;

				//int *idxN = new int[ lp->getNumCols() ];
				//vit = vHashOp.begin();
				//for (; vit != vHashOp.end(); vit++ )
				//{
				//   idxN[ vit->second ] = vit->second;
				//}
				//int cpySolStatus = lp->copyMIPStartSol( lp->getNumCols(), idxN, x );
				//if ( !cpySolStatus )
				//	std::cout<<"\n\nfinal cpySolStatus = error" << endl;
				//delete[] idxN;
			}
		}
		if ( !CARREGA_SOL_PARCIAL )
		{
			// GENERATES SOLUTION
			status = lp->optimize( METHOD_MIP );
			lp->getX(x);
			double mipGap = lp->getMIPGap() * 100;
			fflush(NULL);
		
			#if !defined (DEBUG) && !defined (TESTE)
				// polish aqui só para 1 rodada, aonde se estimam os virtuais
				if ( SEM_FOLGA_DEMANDA && mipGap > 20)
				{
					polishOperacional(x, 2000, 80, 20, 1000);
				}
			#endif

			writeOpSolBin( OutPutFileType::OP_BIN, x, 0 );
			writeOpSolTxt( OutPutFileType::OP_BIN, x, 0 );
		}

		
		delete [] obj;
		delete [] idxN;
		delete [] x;
   }
      
   return status;
}

int Operacional::solveGaranteTotalAtendHorInicial( bool& CARREGA_SOL_PARCIAL, double *xS )
{
	std::cout << "\n==========================>>>>";
	std::cout << "\n======>> Garantia de atendimento com horarios iniciais" << endl;
	
	int status = 0;		
	
	// -------------------------------------------------------------------
	// Salvando função objetivo original

	int *idxN = new int[lp->getNumCols()];
	double *objOrig = new double[lp->getNumCols()];
	lp->getObj(0,lp->getNumCols()-1,objOrig);

	int *idxFO = new int[lp->getNumCols()*2];
	double *valFO = new double[lp->getNumCols()*2];
		
	#pragma region FO para max atendimento
	int nChgFO = 0;
	for ( VariableOpHash::iterator vit = vHashOp.begin(); vit != vHashOp.end(); vit++ )
	{
		double coef=0.0;
		VariableOp v = vit->first;
		
		idxN[vit->second] = vit->second;

		if(v.getType() != VariableOp::V_FOLGA_DEMANDA)
		{
			coef = 0.0;
		}
		else
		{
			coef = 1.0;
		}

		idxFO[nChgFO] = vit->second;
		valFO[nChgFO] = coef;
		nChgFO++;
	}
	lp->chgObj(nChgFO,idxFO,valFO);
	#pragma endregion

	// -------------------------------------------------------------------

    char lpName[1024];
	strcpy( lpName, this->getOpLpFileName(1).c_str() );
	
	int *idxUB_X = new int[lp->getNumCols()*2];
	double *valUB_X = new double[lp->getNumCols()*2];
	double *valOrigUB_X = new double[lp->getNumCols()*2];
	BOUNDTYPE *btsUB_X = new BOUNDTYPE[lp->getNumCols()*2];
					
	#pragma region Fixa os não-horarios de atendimentos das aulas
	int nChgX = 0;	
	for ( VariableOpHash::iterator vit = vHashOp.begin(); vit != vHashOp.end(); vit++ )
	{
		VariableOp v = vit->first;
									
		if ( v.getType() == VariableOp::V_X_PROF_AULA_HOR )
		{			
			double ub = (int)( lp->getUB( vit->second ) + 0.5 );
			double lb = (int)( lp->getLB( vit->second ) + 0.5 );

			if ( lb != ub )
			{
				HorarioAula* hSolInic = v.getAula()->getHorarioAulaInicial();
				HorarioAula *ha = v.getHorarioAula();
			
				if ( hSolInic != NULL )
				if ( hSolInic->getInicio() != ha->getInicio() )
				{
					valOrigUB_X[nChgX] = lp->getUB( vit->second );
					idxUB_X[nChgX] = vit->second;
					valUB_X[nChgX] = 0.0;
					btsUB_X[nChgX] = BOUNDTYPE::BOUND_UPPER;
					nChgX++;
				}
			}
		}
	}
	lp->chgBds( nChgX, idxUB_X, btsUB_X, valUB_X );		// Fixa x = 0
	#pragma endregion

	std::string lpNameGeral;
	lpNameGeral += "atendInic_";
	lpNameGeral += string(lpName);

	#ifdef PRINT_LOGS
	lp->writeProbLP( lpNameGeral.c_str() );
	#endif
				  				
	lp->updateLP();
		
	#pragma region OTIMIZA OU CARREGA SOLUÇÃO
	if ( CARREGA_SOL_PARCIAL )
	{
		// procura e carrega solucao parcial
		int statusReadBin = readOpSolBin( OutPutFileType::OP_BIN1, xS, 0 );
		if ( !statusReadBin )
		{
			CARREGA_SOL_PARCIAL=false;
		}
		else{
			status=1;
			writeOpSolTxt( OutPutFileType::OP_BIN1, xS, 0 );
		}
	}
	if ( !CARREGA_SOL_PARCIAL )
	{
		lp->setMIPEmphasis(1);
		lp->setCuts(0);
		lp->setNodeLimit(10000000000);
		lp->setPolishAfterTime( this->getTimeLimit(Solver::OP1) / 2 );
		lp->setTimeLimit( this->getTimeLimit(Solver::OP1) );
		lp->setNumIntSols(0);
		lp->setMIPRelTol(0.005);

		#if defined SOLVER_GUROBI && defined USAR_CALLBACK
		cb_data.timeLimit = this->getMaxTimeNoImprov(Solver::OP1);		
		cb_data.gapMax = 60;
		lp->setCallbackFunc( &timeWithoutChangeCallback, &cb_data );
		#endif

		lp->updateLP();
			
		// GENERATES SOLUTION
		OPTSTAT optStatus = lp->optimize( METHOD_MIP );
		lp->getX(xS);

		if ( optStatus != OPTSTAT_MIPOPTIMAL && 
			 optStatus != OPTSTAT_FEASIBLE && 
			 optStatus != OPTSTAT_LPOPTIMAL )
			status = 0;
		else 
			status = 1;

		fflush(NULL);
		
		writeOpSolBin( OutPutFileType::OP_BIN1, xS, 0 );
		writeOpSolTxt( OutPutFileType::OP_BIN1, xS, 0 );
	}		  
	#pragma endregion	
	
	#pragma region Verifica não-atendimentos das aulas e fixa completo atendimento em caso de sucesso completo
	int *idxUB_FD = new int[lp->getNumCols()];
	double *valUB_FD = new double[lp->getNumCols()*2];
	BOUNDTYPE *btsUB_FD = new BOUNDTYPE[lp->getNumCols()*2];
	bool fail = false;
	int nBdsFD=0;
	for ( VariableOpHash::iterator vit = vHashOp.begin(); vit != vHashOp.end(); vit++ )
	{
		VariableOp v = vit->first;
									
		if ( v.getType() == VariableOp::V_FOLGA_DEMANDA )
		{		
			double value = (int) (xS[vit->second] + 0.5);
			if ( value == 1.0 )
			{
				fail = true;	// Folga de Atendimento
				break;
			}
			else
			{
				idxUB_FD[nBdsFD] = vit->second;
				valUB_FD[nBdsFD] = 0.0;
				btsUB_FD[nBdsFD] = BOUNDTYPE::BOUND_UPPER;
				nBdsFD++;			
			}
		}
	}
	status = status && !fail;		// Sucesso se houver total atendimento
	if ( status )
		lp->chgBds( nBdsFD, idxUB_FD, btsUB_FD, valUB_FD );
	#pragma endregion
	

	// -------------------------------------------------------------------
	// Volta bounds originais de variaveis x_{i,d,s,t,hi,hf}
	lp->chgBds( nChgX, idxUB_X, btsUB_X, valOrigUB_X );
	
	// -------------------------------------------------------------------
	// Volta com a função objetivo original
	lp->chgObj( lp->getNumCols(),idxN,objOrig );
    
    lp->updateLP();
	
	std::cout << "\n================================================================================";
	
	delete[] idxUB_FD;
	delete[] valUB_FD;
	delete[] btsUB_FD;

	delete[] idxFO;
	delete[] valFO;
	delete[] idxN;
	delete[] objOrig;

	delete[] idxUB_X;
	delete[] valUB_X;
	delete[] btsUB_X;
	delete[] valOrigUB_X;
	
	return status;
}


int Operacional::solveMaxAtendPorFasesDoDia( bool& CARREGA_SOL_PARCIAL, double *xS )
{
	std::cout << "\n==========================>>>>";
	std::cout << "\nGarantindo maximo atendimento por fases do dia...\n"; fflush(NULL);
		
	int status = 0;	
	
	// -------------------------------------------------------------------

    char lpName[1024];
	strcpy( lpName, this->getOpLpFileName(2).c_str() );
	
	// -------------------------------------------------------------------
	// Salvando função objetivo original

	int *idxN = new int[lp->getNumCols()];
	double *objOrig = new double[lp->getNumCols()];
	lp->getObj(0,lp->getNumCols()-1,objOrig);
				
	// -------------------------------------------------------------------
	// Modificando coeficientes na função objetivo

	int *idxFO = new int[lp->getNumCols()*2];
	double *valFO = new double[lp->getNumCols()*2];        // FO para todos os AlunoDemanda
		

	// =====================================================================================
	// CALCULA MÁXIMO ATENDIMENTO POR PARTES DO DIA

	int *idxUB = new int[lp->getNumCols()*2];
	double *valUB = new double[lp->getNumCols()*2];
	BOUNDTYPE *btsUB = new BOUNDTYPE[lp->getNumCols()*2];

	int *idxUB_X = new int[lp->getNumCols()*2];
	double *valUB_X = new double[lp->getNumCols()*2];
	double *valOrigUB_X = new double[lp->getNumCols()*2];
	BOUNDTYPE *btsUB_X = new BOUNDTYPE[lp->getNumCols()*2];

	int *idxLB_Q = new int[lp->getNumCols()*2];
	double *valLB_Q = new double[lp->getNumCols()*2];
	double *valOrigLB_Q = new double[lp->getNumCols()*2];
	BOUNDTYPE *btsLB_Q = new BOUNDTYPE[lp->getNumCols()*2];
	
	
	int nChgLB_Q = 0;
	for ( map< int, std::pair<DateTime,DateTime> >::reverse_iterator itFase = ( problemData->fasesDosTurnos ).rbegin(); 
		itFase != ( problemData->fasesDosTurnos ).rend(); ++itFase )
	{		 
		int fase = itFase->first;

		std::cout << "\n======>> Maximizacao de atendimento da Fase " << fase << endl;
				
		#pragma region FO para max atendimento
		int nChgFO = 0;
		for ( VariableOpHash::iterator vit = vHashOp.begin(); vit != vHashOp.end(); vit++ )
		{
			double coef=0.0;
			VariableOp v = vit->first;
			if(v.getType() != VariableOp::V_FOLGA_DEMANDA)
			{
				coef = 0.0;
			}
			else
			{
				coef = 1.0;
			}

			idxFO[nChgFO] = vit->second;
			valFO[nChgFO] = coef;
			nChgFO++;
		}
		lp->chgObj(nChgFO,idxFO,valFO);
		#pragma endregion
		
		#pragma region Fixa não-atendimentos de fases à frente
		int nChgBdsFaseAdiante = 0;
		for ( VariableOpHash::iterator vit = vHashOp.begin(); vit != vHashOp.end(); vit++ )
		{
			VariableOp v = vit->first;
									
			if ( v.getType() == VariableOp::V_X_PROF_AULA_HOR )
			{
				HorarioAula *ha = v.getHorarioAula();
				int turno = problemData->getFaseDoDia( ha->getInicio() );
				int dia = v.getDia();

				if ( turno < fase && dia!=7 )
				//if ( turno > fase && dia!=7 )
				{
					double ub = (int)( lp->getUB( vit->second ) + 0.5 );
					double lb = (int)( lp->getLB( vit->second ) + 0.5 );

					if ( lb != ub )
					{
						valOrigUB_X[nChgBdsFaseAdiante] = lp->getUB( vit->second );
						idxUB_X[nChgBdsFaseAdiante] = vit->second;
						valUB_X[nChgBdsFaseAdiante] = 0.0;
						btsUB_X[nChgBdsFaseAdiante] = BOUNDTYPE::BOUND_UPPER;
						nChgBdsFaseAdiante++;
					}
				}
			}
		}
		lp->chgBds( nChgBdsFaseAdiante, idxUB_X, btsUB_X, valUB_X );
		#pragma endregion

		stringstream ss;
		ss << fase;
		std::string sf = ss.str();

		std::string lpNameGeral;
		lpNameGeral += "maxAtend" + sf + "_";
		lpNameGeral += string(lpName);

		#ifdef PRINT_LOGS
		lp->writeProbLP( lpNameGeral.c_str() );
		#endif
				  				
		lp->updateLP();
		
		#pragma region OTIMIZA OU CARREGA SOLUÇÃO PARA TODOS ALUNOS
		if ( CARREGA_SOL_PARCIAL )
		{
			// procura e carrega solucao parcial
			int statusReadBin = readOpSolBin( OutPutFileType::OP_BIN2, xS, fase );
			if ( !statusReadBin )
			{
				CARREGA_SOL_PARCIAL=false;
			}
			else writeOpSolTxt( OutPutFileType::OP_BIN2, xS, fase );
		}
		if ( !CARREGA_SOL_PARCIAL )
		{
			lp->setMIPEmphasis(1);
			lp->setCuts(0);
			lp->setNodeLimit(10000000000);
			lp->setPolishAfterTime( this->getTimeLimit(Solver::OP2) / 2 );
			lp->setTimeLimit( this->getTimeLimit(Solver::OP2) );
			lp->setNumIntSols(0);
			
			#if defined SOLVER_GUROBI && defined USAR_CALLBACK
			cb_data.timeLimit = this->getMaxTimeNoImprov(Solver::OP2);		
			cb_data.gapMax = 60;
			lp->setCallbackFunc( &timeWithoutChangeCallback, &cb_data );
			#endif

			lp->updateLP();
			
			// GENERATES SOLUTION
			status = lp->optimize( METHOD_MIP );
			lp->getX(xS);
			fflush(NULL);
		
			writeOpSolBin( OutPutFileType::OP_BIN2, xS, fase );
			writeOpSolTxt( OutPutFileType::OP_BIN2, xS, fase );
		}		  
		#pragma endregion
				
		#pragma region FIXA ATENDIMENTO fd_{d,i,cp} = 0  e  z_{i,d,cp,h} = 1
		int nAtendsTotal = 0;
		int nChgUB = 0;
		for ( VariableOpHash::iterator vit = vHashOp.begin(); vit != vHashOp.end(); vit++ )
		{
			VariableOp v = vit->first;
			double value = (int) ( xS[vit->second] + 0.5 );

			if(v.getType() == VariableOp::V_FOLGA_DEMANDA && value == 0.0 )
			{
				nAtendsTotal++;
				double ub = (int)( lp->getUB( vit->second ) + 0.5 );
				double lb = (int)( lp->getLB( vit->second ) + 0.5 );

				if ( lb != ub )
				{
					idxUB[nChgUB] = vit->second;
					valUB[nChgUB] = value;
					btsUB[nChgUB] = BOUNDTYPE::BOUND_UPPER;
					nChgUB++;		
				}
			}
			if(v.getType() == VariableOp::V_Z_DISCIPLINA_HOR && value == 1.0 )
			{
				HorarioAula *ha = v.getHorarioAula();
				int turno = problemData->getFaseDoDia( ha->getInicio() );
				
				if ( turno >= fase )
				//if ( turno <= fase )
				{
					double ub = (int)( lp->getUB( vit->second ) + 0.5 );
					double lb = (int)( lp->getLB( vit->second ) + 0.5 );
					if ( lb != ub )
					{
						valOrigLB_Q[nChgLB_Q] = lp->getLB( vit->second );
						idxLB_Q[nChgLB_Q] = vit->second;
						valLB_Q[nChgLB_Q] = value;
						btsLB_Q[nChgLB_Q] = BOUNDTYPE::BOUND_LOWER;
						nChgLB_Q++;
					}
				}
			}
		}
		lp->chgBds( nChgUB, idxUB, btsUB, valUB );			// Fixa fd = 0
		lp->chgBds( nChgLB_Q, idxLB_Q, btsLB_Q, valLB_Q );	// Fixa z = 1
		std::cout << "\n---> Numero de turmas abertas ao fim da fase " << fase << ": "<< nAtendsTotal <<endl<<endl;
		#pragma endregion
		
		
		// Volta bounds originais de variaveis x_{i,d,s,t,hi,hf} de fases à frente que foram fixadas como NÃO atendidas.
		lp->chgBds( nChgBdsFaseAdiante, idxUB_X, btsUB_X, valOrigUB_X );

	}

	// Volta lower bounds originais de variaveis z
	lp->chgBds( nChgLB_Q, idxLB_Q, btsLB_Q, valOrigLB_Q );

	// Mantem as fixações fd_{i,d,cp} = 0 				            
	

	// -------------------------------------------------------------------
	// Volta com a função objetivo original
	lp->chgObj( lp->getNumCols(),idxN,objOrig );
    lp->updateLP();
	
	std::cout << "\n================================================================================";
	
	
	delete[] idxLB_Q;
	delete[] valLB_Q;
	delete[] btsLB_Q;
	delete[] valOrigLB_Q;

	delete[] idxUB;
	delete[] valUB;
	delete[] btsUB;

	delete[] idxUB_X;
	delete[] valUB_X;
	delete[] btsUB_X;
	delete[] valOrigUB_X;
	
	delete[] idxFO;
	delete[] valFO;
	delete[] idxN;
	delete[] objOrig;

	return status;
}

int Operacional::solveMinPVPorFasesDoDia( bool& CARREGA_SOL_PARCIAL, double *xS )
{				
	int status = 0;	
	
	// -------------------------------------------------------------------

    char lpName[1024];
	strcpy( lpName, this->getOpLpFileName(3).c_str() );
	
	// -------------------------------------------------------------------
	// Salvando função objetivo original

	int *idxN = new int[lp->getNumCols()];
	double *objOrig = new double[lp->getNumCols()];
	lp->getObj(0,lp->getNumCols()-1,objOrig);
				
	// -------------------------------------------------------------------
	// Modificando coeficientes na função objetivo

	int *idxFO = new int[lp->getNumCols()*2];
	double *valFO = new double[lp->getNumCols()*2];        // FO para todos os AlunoDemanda
		

	// =====================================================================================
	// CALCULA MÁXIMO ATENDIMENTO POR PARTES DO DIA
	
	int *idxUB_X = new int[lp->getNumCols()*2];
	double *valUB_X = new double[lp->getNumCols()*2];
	double *valOrigUB_X = new double[lp->getNumCols()*2];
	BOUNDTYPE *btsUB_X = new BOUNDTYPE[lp->getNumCols()*2];

	int *idxLB_Q = new int[lp->getNumCols()*2];
	double *valLB_Q = new double[lp->getNumCols()*2];
	double *valOrigLB_Q = new double[lp->getNumCols()*2];
	BOUNDTYPE *btsLB_Q = new BOUNDTYPE[lp->getNumCols()*2];
	
	
	// -------------------------------------------------------------------
	
	#pragma region FO para min prof virtual
	int nChgFO = 0;	
	for ( VariableOpHash::iterator vit = vHashOp.begin(); vit != vHashOp.end(); vit++ )
	{
		double coef=0.0;
		VariableOp v = vit->first;
		if(v.getType() != VariableOp::V_PROF_VIRTUAL)
		{
			coef = 0.0;
		}
		else
		{
			coef = 1.0;
		}

		idxFO[nChgFO] = vit->second;
		valFO[nChgFO] = coef;
		nChgFO++;
	}
	lp->chgObj(nChgFO,idxFO,valFO);
	lp->updateLP();
	#pragma endregion
	
	// -------------------------------------------------------------------

	int nChgLB_Q = 0;
	for( auto itFase = problemData->fasesDosTurnos.begin(); itFase != problemData->fasesDosTurnos.end(); itFase++ )
	{		 
		int fase = itFase->first;
			
		std::cout << "\n======>> Minimizacao de profs virtuais da Fase " << fase << endl;

		
		#pragma region Fixa horarios não usados em atendimentos de fases à frente na solução atual
		int nChgBdsFaseAdiante = 0;
		for ( VariableOpHash::iterator vit = vHashOp.begin(); vit != vHashOp.end(); vit++ )
		{
			VariableOp v = vit->first;
									
			if ( v.getType() == VariableOp::V_Z_DISCIPLINA_HOR )
			{
				HorarioAula *ha = v.getHorarioAula();
				int turno = problemData->getFaseDoDia( ha->getInicio() );

				if ( turno > fase )
				{
					double value = (int) ( xS[vit->second] + 0.5 );
					double ub = (int)( lp->getUB( vit->second ) + 0.5 );
					double lb = (int)( lp->getLB( vit->second ) + 0.5 );

					if ( lb != ub  &&  value == 0.0 )
					{
						valOrigUB_X[nChgBdsFaseAdiante] = lp->getUB( vit->second );
						idxUB_X[nChgBdsFaseAdiante] = vit->second;
						valUB_X[nChgBdsFaseAdiante] = 0.0;
						btsUB_X[nChgBdsFaseAdiante] = BOUNDTYPE::BOUND_UPPER;
						nChgBdsFaseAdiante++;
					}
				}
			}
		}
		lp->chgBds( nChgBdsFaseAdiante, idxUB_X, btsUB_X, valUB_X );
		lp->updateLP();
		#pragma endregion

		stringstream ss;
		ss << fase;
		std::string sf = ss.str();

		std::string lpNameGeral;
		lpNameGeral += "minPV" + sf + "_";
		lpNameGeral += string(lpName);

		#ifdef PRINT_LOGS
		lp->writeProbLP( lpNameGeral.c_str() );
		#endif
							
		#pragma region OTIMIZA OU CARREGA SOLUÇÃO PARA TODOS ALUNOS
		if ( CARREGA_SOL_PARCIAL )
		{
			// procura e carrega solucao parcial
			int statusReadBin = readOpSolBin( OutPutFileType::OP_BIN3, xS, fase );
			if ( !statusReadBin )
			{
				CARREGA_SOL_PARCIAL=false;
			}
			else writeOpSolTxt( OutPutFileType::OP_BIN3, xS, fase );
		}
		if ( !CARREGA_SOL_PARCIAL )
		{
			lp->setMIPEmphasis(1);
			lp->setCuts(0);
			lp->setNodeLimit(10000000000);
			lp->setPolishAfterTime( this->getTimeLimit(Solver::OP3) / 2 );
			lp->setTimeLimit( this->getTimeLimit(Solver::OP3) );
			lp->setNumIntSols(0);
			
			#if defined SOLVER_GUROBI && defined USAR_CALLBACK
			cb_data.timeLimit = this->getMaxTimeNoImprov(Solver::OP3);		
			cb_data.gapMax = 60;
			lp->setCallbackFunc( &timeWithoutChangeCallback, &cb_data );
			#endif

			lp->updateLP();
			
			// GENERATES SOLUTION
			status = lp->optimize( METHOD_MIP );
			lp->getX(xS);
			fflush(NULL);
		
			writeOpSolBin( OutPutFileType::OP_BIN3, xS, fase );
			writeOpSolTxt( OutPutFileType::OP_BIN3, xS, fase );
		}		  
		#pragma endregion
				
		#pragma region FIXA ATENDIMENTO z_{i,d,h,cp} = 1
		int nroPV = 0;
		for ( VariableOpHash::iterator vit = vHashOp.begin(); vit != vHashOp.end(); vit++ )
		{
			VariableOp v = vit->first;
			double value = (int) ( xS[vit->second] + 0.5 );

			if(v.getType() == VariableOp::V_Z_DISCIPLINA_HOR && value == 1.0 )
			{
				HorarioAula *ha = v.getHorarioAula();
				int turno = problemData->getFaseDoDia( ha->getInicio() );
				
				if ( turno <= fase )
				{
					double ub = (int)( lp->getUB( vit->second ) + 0.5 );
					double lb = (int)( lp->getLB( vit->second ) + 0.5 );
					if ( lb != ub )
					{
						valOrigLB_Q[nChgLB_Q] = lp->getLB( vit->second );
						idxLB_Q[nChgLB_Q] = vit->second;
						valLB_Q[nChgLB_Q] = value;
						btsLB_Q[nChgLB_Q] = BOUNDTYPE::BOUND_LOWER;
						nChgLB_Q++;
					}
				}
			}
			if(v.getType() == VariableOp::V_PROF_VIRTUAL && value == 1.0 )
			{
				nroPV++;
			}
		}
		lp->chgBds( nChgLB_Q, idxLB_Q, btsLB_Q, valLB_Q );	// Fixa z = 1
		std::cout << "\n---> Numero de profs virtuais usados ao fim da fase " << fase << ": "<< nroPV <<endl<<endl;
		#pragma endregion

		
		// Volta bounds originais de variaveis x_{i,d,s,t,hi,hf} de fases à frente que foram fixadas.
		lp->chgBds( nChgBdsFaseAdiante, idxUB_X, btsUB_X, valOrigUB_X );

	}

	// Volta lower bounds originais de variaveis z
	lp->chgBds( nChgLB_Q, idxLB_Q, btsLB_Q, valOrigLB_Q );


	// -------------------------------------------------------------------
	// Volta com a função objetivo original
	lp->chgObj( lp->getNumCols(),idxN,objOrig );
    lp->updateLP();
	
	std::cout << "\n================================================================================";
	
	
	delete[] idxLB_Q;
	delete[] valLB_Q;
	delete[] btsLB_Q;
	delete[] valOrigLB_Q;

	delete[] idxUB_X;
	delete[] valUB_X;
	delete[] btsUB_X;
	delete[] valOrigUB_X;
	
	delete[] idxFO;
	delete[] valFO;
	delete[] idxN;
	delete[] objOrig;

	return status;
}


void Operacional::carregaSolucaoOperacional()
{	
	// Limpa solução antiga
   ITERA_GGROUP_LESSPTR( itSol, (*solVarsOp), VariableOp )
   {
      VariableOp * v = *itSol;
	  delete *itSol;
   }
   solVarsOp->clear();

   double * xSol = NULL;
   
   int nroColsLP = lp->getNumCols();
   xSol = new double[ nroColsLP ];
	
	#pragma region Carrega solucao xSol
	if ( (*this->CARREGA_SOLUCAO) )
	{
		int status = readOpSolBin( OutPutFileType::OP_BIN, xSol, 0 );
		if ( !status )
		{
		   std::cout << "\nErro em Operacional::carregaSolucaoOperacional(): arquivo"
					" nao encontrado.\n";
			exit(0);
		}
	}
	else
	{
		lp->getX( xSol );
	}
	#pragma endregion

	// ----------------------------------------------------------------------------------------------------

	#pragma region Carrega solução a partir do hash de variáveis

	GGroup< Trio< int /*campusId*/, int /*turma*/, Disciplina* > > naoAtendimentos; // <cp,i,d>

	ofstream fout;
	fout.open( this->getSolucaoOpFileName( -1, 0 ).c_str(), std::ios_base::out );

	GGroup<int> profsReais;
	int nroCredsProfReal=0;
	int nroCredsProfVirt=0;
	double chProfReal=0;
	double chProfVirt=0;
	int nProfsVirtuais=0;
	int nTurmasT=0;
	int nTurmasVirtT=0;
	int nTurmasRealT=0;
	int nTurmasP=0;
	int nTurmasVirtP=0;
	int nTurmasRealP=0;

	VariableOpHash::iterator vit = vHashOp.begin();
	for (; vit != vHashOp.end(); vit++ )
	{
		VariableOp v = vit->first;
		int value = (int)( xSol[ vit->second ] + 0.5 );

		if ( value > 0.01 )
		{
			VariableOp * newVar = new VariableOp( v );

			newVar->setValue( value );
			if ( fout )
			{
				fout << v.toString().c_str() << " = " << value << "\n";
			}

			solVarsOp->add( newVar );

			if ( v.getType() == VariableOp::V_FOLGA_DEMANDA )
			{
				Trio< int /*campusId*/, int /*turma*/, Disciplina* > t;
				int campusId = v.getAula()->getSala()->getIdCampus();
				int turma = v.getAula()->getTurma();
				Disciplina* disc = v.getAula()->getDisciplina();
				t.set( campusId, turma, disc );
				naoAtendimentos.add(t);
			}
			else if ( v.getType() == VariableOp::V_X_PROF_AULA_HOR )
			{
				if ( v.getProfessor()->eVirtual() )
				{
					nroCredsProfVirt += v.getAula()->getTotalCreditos();
					chProfVirt += v.getAula()->getDisciplina()->getTempoCredSemanaLetiva() * v.getAula()->getTotalCreditos();
				}
				else
				{
					nroCredsProfReal += v.getAula()->getTotalCreditos();
					chProfReal += v.getAula()->getDisciplina()->getTempoCredSemanaLetiva() * v.getAula()->getTotalCreditos();
					profsReais.add( v.getProfessor()->getId() );
				}
			}
			else if ( v.getType() == VariableOp::V_PROF_VIRTUAL && this->getRodada() == OP_VIRTUAL_INDIVIDUAL )
			{
				nProfsVirtuais++;
			}
			else if ( v.getType() == VariableOp::V_NRO_PROFS_VIRTUAIS_CURSO && this->getRodada() == OP_VIRTUAL_PERFIL )
			{
				nProfsVirtuais += value;
			}
			else if ( v.getType() == VariableOp::V_Y_PROF_DISCIPLINA )
			{
				if ( v.getDisciplina()->getId()>0 )	// teorica
				{
					nTurmasT++;
					if ( v.getProfessor()->eVirtual() )
						nTurmasVirtT++;
					else
						nTurmasRealT++;
				}
				else								// pratica
				{
					nTurmasP++;
					if ( v.getProfessor()->eVirtual() )
						nTurmasVirtP++;
					else
						nTurmasRealP++;
				}
			}
		}
	}

	delete [] xSol;
	
	fout << "\n\n" << naoAtendimentos.size() << " turmas não atendidas.";
	
	if ( fout )
	{
		fout.close();
	}

	#pragma endregion
	
	// ----------------------------------------------------------------------------------------------------
	
	#pragma region Inclui motivos de não atendimento tático em caso de não turmas não atendidas no operacional
	if ( this->getRodada() == Rodada::OP_VIRTUAL_INDIVIDUAL )
	{
		GGroup< Trio<int, int, Disciplina*> >::iterator itNaoAtend = naoAtendimentos.begin();
		for ( ; itNaoAtend != naoAtendimentos.end(); itNaoAtend++ )
		{
			Trio< int, int, Disciplina* > cpTurmaDisc = (*itNaoAtend);

			GGroup<AlunoDemanda*, LessPtr<AlunoDemanda>> *alunosDemNaTurma = 
				& problemData->mapCampusTurmaDisc_AlunosDemanda[cpTurmaDisc];
			ITERA_GGROUP_LESSPTR( itAlDem, *alunosDemNaTurma, AlunoDemanda )
			{
				NaoAtendimento* naoAtendimento = new NaoAtendimento( (*itAlDem)->getId() );
	
				stringstream ss;
				ss << "Turma do aluno não foi atendida no operacional.";
				naoAtendimento->addMotivo( ss.str() );
				
				problemSolution->nao_atendimentos->add( naoAtendimento );
			}
		}
	}
	#pragma endregion

	// ----------------------------------------------------------------------------------------------------
	
	#pragma region INDICADORES DE QUALIDADE DA SOLUÇÃO OPERACIONAL
	Indicadores::printSeparator(3);
	if ( this->getRodada() == OP_VIRTUAL_PERFIL )
	{
		Indicadores::printIndicador( "\nNumero total de professores usados: ", (int)(nProfsVirtuais + profsReais.size()) );
		Indicadores::printIndicador( "\nNumero de professores virtuais usados: ", nProfsVirtuais );	
		Indicadores::printIndicador( "\nNumero de professores reais usados: ", (int) profsReais.size() );	
		Indicadores::printSeparator(1);
		Indicadores::printIndicador( "\nNumero total de creditos alocados a professores: ", (int) (nroCredsProfVirt + nroCredsProfReal) );
		Indicadores::printIndicador( "\nNumero de creditos alocados a professores reais: ", nroCredsProfReal );
		Indicadores::printIndicador( "\nNumero de creditos alocados a professores virtuais: ", nroCredsProfVirt );
		Indicadores::printSeparator(1);
	
		Indicadores::printIndicador( "\nCarga horaria alocada a professores (h): ", (chProfReal+chProfVirt) / 60, 2 );
		Indicadores::printIndicador( "\nCarga horaria alocada a professores reais (h): ", chProfReal/60, 2 );
		Indicadores::printIndicador( "\nCarga horaria alocada a professores virtuais (h): ", chProfVirt/60, 2 );
		Indicadores::printSeparator(1);
		Indicadores::printIndicador( "\nNumero de turmas atendidas: ", nTurmasT );
		Indicadores::printIndicador( "\nNumero de turmas atendidas por professores reais: ", nTurmasRealT );
		Indicadores::printIndicador( "\nNumero de turmas atendidas por professores virtuais: ", nTurmasVirtT );
		Indicadores::printIndicador( "\nNumero de turmas praticas (com divisao PT) atendidas: ", nTurmasP );
		Indicadores::printIndicador( "\nNumero de turmas praticas (com divisao PT) atendidas por professores reais: ", nTurmasRealP );
		Indicadores::printIndicador( "\nNumero de turmas praticas (com divisao PT) atendidas por professores virtuais: ", nTurmasVirtP );
		Indicadores::printIndicador( "\nNumero de aulas nao-atendidas: ", (int) (naoAtendimentos.size()) );
	}
	#pragma endregion
}


void Operacional::relacionaProfessoresDisciplinas()
{
   problemData->mapProfessorDisciplinas.clear();

   GGroup< Professor *, LessPtr< Professor > > professores
      = problemData->campi.begin()->professores;

   // Informa as aulas alocadas para cada professor
   ITERA_GGROUP_LESSPTR( it_prof, professores, Professor )
   {
      Professor * professor = ( *it_prof );

      ITERA_GGROUP_LESSPTR( it_aula, problemData->aulas, Aula )
      {
         Aula * aula = ( *it_aula );
         Disciplina * disciplina = aula->getDisciplina();

         // TODO -- como recuperar o professor que foi alocado à aula ???
         std::pair< int, int > professor_disciplina(
            professor->getId(), disciplina->getId() );

         // Se o professor e a disciplina da aula em questão se relacionarem
         if ( problemData->prof_Disc_Dias.find( professor_disciplina )
               == problemData->prof_Disc_Dias.end() )
         {
            continue;
         }

         std::pair< Aula *, Disciplina * > aula_disciplina
            = std::make_pair( aula, disciplina );

         problemData->mapProfessorDisciplinas[ professor ].add( aula_disciplina );
      }
   }
}


void Operacional::getSolutionOperacionalMIP()
{
	std::cout<<"\nGetting solution operacional MIP... "; fflush(NULL);

   // Procura variaveis que usem professor virtual
   ITERA_GGROUP_LESSPTR( itOp, (*solVarsOp), VariableOp )
   {
      VariableOp * v = *itOp;

      if ( v->getType() != VariableOp::V_X_PROF_AULA_HOR )
      {
         continue;
      }

      if ( v->getProfessor() == NULL )
      {
		  std::cout<<"\nErro, prof null\n"; fflush(NULL);
         exit(1);
      }

      if ( !v->getProfessor()->eVirtual() )
      {
         continue;
      }

      // Procura virtual
      bool achou = false;
	  ProfessorVirtualOutput * professor_virtual = problemSolution->getProfVirtualOutput( v->getProfessor()->getId() );
	  if ( professor_virtual != NULL ){
            achou = true;
            professor_virtual->disciplinas.add( abs( v->getAula()->getDisciplina()->getId() ) );
	  }

      if ( !achou )
      {
         ProfessorVirtualOutput * professor_virtual = new ProfessorVirtualOutput();

         professor_virtual->setId( v->getProfessor()->getId() );
         professor_virtual->setChMin( v->getProfessor()->getChMin() );
         professor_virtual->setChMax( v->getProfessor()->getChMax() );
         professor_virtual->setTitulacaoId( v->getProfessor()->getTitulacaoId() );
         professor_virtual->setAreaTitulacaoId( v->getProfessor()->getAreaId() );
		 professor_virtual->setContratoId( v->getProfessor()->getTipoContratoId() );

         professor_virtual->disciplinas.add( abs( v->getAula()->getDisciplina()->getId() ) );
		 
		 professor_virtual->setCursoAssociado( v->getProfessor()->getCursoAssociado()->getId() );
         
		 AlocacaoProfVirtual *alocacao = new AlocacaoProfVirtual( 
			 abs( v->getAula()->getDisciplina()->getId() ),
			 v->getAula()->getTurma(), 
			 v->getAula()->getUnidade()->getIdCampus(),
			 (bool) v->getAula()->getCreditosPraticos() );
		 
		 professor_virtual->alocacoes.add( alocacao );

         problemSolution->professores_virtuais->add( professor_virtual );
      }
	  else
	  {
		  AlocacaoProfVirtual *alocacao = new AlocacaoProfVirtual( 
			abs( v->getAula()->getDisciplina()->getId() ), 
			v->getAula()->getTurma(), 
			v->getAula()->getUnidade()->getIdCampus(),
			(bool) v->getAula()->getCreditosPraticos() );
		  		  
		  if ( professor_virtual->alocacoes.find( alocacao ) == professor_virtual->alocacoes.end() )
		  {		 
			  professor_virtual->alocacoes.add( alocacao );	  
		  }
		  else
		  {
			  delete alocacao;
		  }
	  }

   }
   std::cout<<" done!\n"; fflush(NULL);
}


void Operacional::preencheOutputOperacionalMIP()
{
	std::cout<<"\nFilling in Output Operacional MIP..."; fflush(NULL);

   Campus * campus = nullptr;
   Unidade * unidade = nullptr;
   Sala * sala = nullptr;
   int dia_semana = 0;
   
	// ---------------------------------------------------------------------------------------------
	// ---------------------------------------------------------------------------------------------
	// Agrupa variáveis de atendimento
      
	std::map< VariableOp*,
		std::map< int /*turnoIdIES*/,
			std::map< HorarioAula*,
				std::map< Professor*,
					std::map< Oferta*,
						std::map< Disciplina*, 
							GGroup<int /*alDemId*/> > > > > > > mapSolucaoCompleta;

	ITERA_GGROUP_LESSPTR( itOp, (*solVarsOp), VariableOp )
	{
		VariableOp * const v = *itOp;
				  
        if ( v->getType() != VariableOp::V_X_PROF_AULA_HOR )
            continue;
                  
		if ( v->getAula() == nullptr )
        {
			std::cout<<"\nERRO, aula NULL. Var " << v->toString(); fflush(NULL);
            exit(1);
        }

		Aula * const a = v->getAula();
		Professor * const professor = v->getProfessor();

		auto *mapSolVariavel = & mapSolucaoCompleta[v];

		// --------------------------------------------
		// Alunos-Demanda atendidos na aula
		std::map<Oferta*, std::map<Disciplina*, GGroup<int /*alDemOrig*/ >, 
			LessPtr<Disciplina>>, LessPtr<Oferta>> *mapOftDiscorigAlsDem = a->getMapOftDiscorigAlsDem();

		if ( mapOftDiscorigAlsDem->size()==0 )
			 CentroDados::printError( "void Operacional::preencheOutputOperacionalMIP()",
				 "\nErro ao preencher o output operacional. Turma vazia ou alunos-demanda nao encontrados.");

		// --------------------------------------------
		// Percorre os horários da aula
		HorarioAula * h = v->getHorarioAula();
		Calendario * const c = h->getCalendario();  
		int nCreds = 1;
		while ( h!=nullptr && nCreds <= a->getTotalCreditos() )
		{
			 DateTime dti = h->getInicio();
			 DateTime dtf = h->getFinal();
			 
			 // --------------------------------------------
			 // Alunos-Demanda classificados por oferta
			 auto itOft = mapOftDiscorigAlsDem->begin();
			 for ( ; itOft != mapOftDiscorigAlsDem->end(); itOft++ )
			 {			
				 Oferta* oferta = itOft->first;
				 int turnoIdIES = oferta->getTurnoId();
				 HorarioAula* horAula = nullptr;
				 
				 // --------------------------------------------
				 // Recupera id do horario do turno correto
				 TurnoIES* turnoIES = problemData->refTurnos[turnoIdIES];
				 ITERA_GGROUP_LESSPTR( itHorAula, turnoIES->horarios_aula, HorarioAula )	// TODO: usar map com dia/dti ao inves de set
				 {
					 if ( itHorAula->dias_semana.find(a->getDiaSemana()) != itHorAula->dias_semana.end() )
					 if ( itHorAula->getInicio() == dti && itHorAula->getFinal() == dtf )
					 {
						 horAula = *itHorAula;
						 break;
					 }
				 }
				 if ( horAula==nullptr )
				 {
					 stringstream msg;
					 msg << "Horario " << dti.hourMinToStr() 
						 << "-" << dtf.hourMinToStr() << " nao encontrado no turno " << turnoIdIES;
					 CentroDados::printError( "void Operacional::preencheOutputOperacionalMIP()", msg.str() );
					 exit(1);
				 }
				 
				 // --------------------------------------------
				 // Insere o atendimento-horario para a oferta				 
				 auto *mapSolOft = & (*mapSolVariavel)[turnoIdIES][horAula][professor][oferta];

				 // --------------------------------------------				 
				 // Agrupa os alunos-demanda pela disciplina original
				 auto itDiscOrig = itOft->second.begin();
				 for ( ; itDiscOrig != itOft->second.end(); itDiscOrig++ )
				 {							 
					Disciplina *discOriginal = itDiscOrig->first;

					auto *mapSolDiscOrig = & (*mapSolOft)[discOriginal];

					auto itAlDemId = itDiscOrig->second.begin();
					for ( ; itAlDemId != itDiscOrig->second.end(); itAlDemId++ )
					{
						int alDemId = *itAlDemId;

						(*mapSolDiscOrig).add(alDemId);
					}										 
				 }
			 }
					 
			 h = c->getProximoHorario( h );
			 nCreds++;
		}

		nCreds--;
		if ( nCreds != a->getTotalCreditos() )
		{
			stringstream msg;
			msg << "Nao foi possivel encontrar os horarios completos da aula " << v->toString();
			CentroDados::printError( "void Operacional::preencheOutputOperacionalMIP()", msg.str() );
		}
		fflush(NULL);
	}
	
	// ---------------------------------------------------------------------------------------------
	// ---------------------------------------------------------------------------------------------
	// Preenche problemSolution

   ITERA_GGROUP( it_At_Campus,
      ( *problemSolution->atendimento_campus ), AtendimentoCampus )
   {
      // Campus do atendimento
      campus = it_At_Campus->campus;

      ITERA_GGROUP_LESSPTR( it_At_Unidade,
         ( *it_At_Campus->atendimentos_unidades ), AtendimentoUnidade )
      {
         // Unidade do atendimento
         unidade = problemData->refUnidade[ it_At_Unidade->getId() ];

         ITERA_GGROUP_LESSPTR( it_At_Sala,
            ( *it_At_Unidade->atendimentos_salas ), AtendimentoSala )
         {
            // Sala do atendimento
            sala = problemData->refSala[ it_At_Sala->getId() ];
			
            ITERA_GGROUP_LESSPTR( it_At_DiaSemana,
               ( *it_At_Sala->atendimentos_dias_semana ), AtendimentoDiaSemana )
            {
				// Dia da semana do atendimento
                dia_semana = it_At_DiaSemana->getDiaSemana();
			   			   
				if ( it_At_DiaSemana->atendimentos_turno == nullptr )
					it_At_DiaSemana->atendimentos_turno = new GGroup< AtendimentoTurno*, LessPtr<AtendimentoTurno> >();

				// Variavel
				auto itMapVariavel = mapSolucaoCompleta.begin();
				for ( ; itMapVariavel != mapSolucaoCompleta.end(); itMapVariavel++ )
				{
					VariableOp *v = itMapVariavel->first;
					Aula *aula = v->getAula();

					// Filtra dia e sala da aula
					if ( aula->getDiaSemana() != dia_semana ||
						 aula->getSala() != sala )
						continue;
					
					// Turno
					auto itMapTurno = itMapVariavel->second.begin();
					for ( ; itMapTurno != itMapVariavel->second.end(); itMapTurno++ )
					{
						int turnoIdIES = itMapTurno->first;

						// ---------------------------
						// Cria ou recupera AtendimentoTurno
						
						AtendimentoTurno * atendimento_turno = nullptr;
						ITERA_GGROUP_LESSPTR( it, ( *it_At_DiaSemana->atendimentos_turno ), AtendimentoTurno )
						{
							 if ( it->getTurnoId() == turnoIdIES )
							 {
								atendimento_turno = ( *it );
								break;
							 }
						}
						if ( atendimento_turno == nullptr )
						{
							 atendimento_turno = new AtendimentoTurno(
								this->problemSolution->getIdAtendimentos() );

							 atendimento_turno->setId( turnoIdIES );
							 atendimento_turno->setTurnoId( turnoIdIES );

							 it_At_DiaSemana->atendimentos_turno->add( atendimento_turno );
						}

						// ---------------------------

						// Horario Aula
						auto itMapHorAula = itMapTurno->second.begin();
						for ( ; itMapHorAula != itMapTurno->second.end(); itMapHorAula++ )
						{
							HorarioAula* horAula = itMapHorAula->first;

							// Professor
							auto itMapProf = itMapHorAula->second.begin();
							for ( ; itMapProf != itMapHorAula->second.end(); itMapProf++ )
							{
								Professor * professor = itMapProf->first;

								// ---------------------------
								// Cria AtendimentoHorarioAula
								AtendimentoHorarioAula * atendimento_horario_aula = new AtendimentoHorarioAula(
									this->problemSolution->getIdAtendimentos() );

								atendimento_horario_aula->setId( horAula->getId() );
								atendimento_horario_aula->setHorarioAulaId( horAula->getId() );
								atendimento_horario_aula->setProfessorId( professor->getId() );
								atendimento_horario_aula->setProfVirtual( professor->eVirtual() );
								atendimento_horario_aula->setCreditoTeorico( aula->getCreditosTeoricos() > 0 );	
								atendimento_horario_aula->horario_aula = horAula;
								atendimento_horario_aula->professor = professor;
								// ---------------------------

								// Oferta
								auto itMapOft = itMapProf->second.begin();
								for ( ; itMapOft != itMapProf->second.end(); itMapOft++ )
								{
									Oferta* oferta = itMapOft->first;
									stringstream oftStr;
									oftStr << oferta->getId();

									// Disciplina Original
									auto itMapDiscOrig = itMapOft->second.begin();
									for ( ; itMapDiscOrig != itMapOft->second.end(); itMapDiscOrig++ )
									{
										Disciplina *discOriginal = itMapDiscOrig->first;
							 							
										AtendimentoOferta * atendimento_oferta = new AtendimentoOferta(
										this->problemSolution->getIdAtendimentos() );
												
										bool EQUIV;

										if ( problemData->parametros->considerar_equivalencia_por_aluno &&
											discOriginal != aula->getDisciplina() )
										{
											atendimento_oferta->setDisciplinaSubstitutaId( aula->getDisciplina()->getId() );
											atendimento_oferta->setDisciplinaId( discOriginal->getId() );
											atendimento_oferta->disciplina = discOriginal;								
											EQUIV = true;
										}
										else
										{
											atendimento_oferta->setDisciplinaId( aula->getDisciplina()->getId() );
											atendimento_oferta->disciplina = aula->getDisciplina();
											EQUIV = false;
						 				}

										atendimento_oferta->setTurma( aula->getTurma() );	
										atendimento_oferta->alunosDemandasAtendidas.add( itMapDiscOrig->second );
							 
										int n = itMapDiscOrig->second.size();											
										 
										atendimento_oferta->setQuantidade( n );										 
										atendimento_oferta->setOfertaCursoCampiId( oftStr.str() );
										atendimento_oferta->oferta = oferta;
										 
										atendimento_horario_aula->atendimentos_ofertas->add( atendimento_oferta );
									}
								}

								atendimento_turno->atendimentos_horarios_aula->add( atendimento_horario_aula );
							}			
						}
					}
				}
            }
         }
      }
   }
	std::cout<<" done!\n"; fflush(NULL);
}


void Operacional::preencheOutputOperacionalMIP_antigo()
{
	std::cout<<"\nFilling in Output Operacional MIP..."; fflush(NULL);

   Campus * campus = nullptr;
   Unidade * unidade = nullptr;
   Sala * sala = nullptr;
   int dia_semana = 0;

	// ---------------------------------------------------------------------------------------------
	// Preenche problemSolution

   ITERA_GGROUP( it_At_Campus,
      ( *problemSolution->atendimento_campus ), AtendimentoCampus )
   {
      // Campus do atendimento
      campus = it_At_Campus->campus;

      ITERA_GGROUP_LESSPTR( it_At_Unidade,
         ( *it_At_Campus->atendimentos_unidades ), AtendimentoUnidade )
      {
         // Unidade do atendimento
         unidade = problemData->refUnidade[ it_At_Unidade->getId() ];

         ITERA_GGROUP_LESSPTR( it_At_Sala,
            ( *it_At_Unidade->atendimentos_salas ), AtendimentoSala )
         {
            // Sala do atendimento
            sala = problemData->refSala[ it_At_Sala->getId() ];

            ITERA_GGROUP_LESSPTR( it_At_DiaSemana,
               ( *it_At_Sala->atendimentos_dias_semana ), AtendimentoDiaSemana )
            {
               // Dia da semana do atendimento
               dia_semana = it_At_DiaSemana->getDiaSemana();

	//		   std::cout<<"\nAtendimentoDiaSemana id" << it_At_DiaSemana->getId(); fflush(NULL);
			   
			   if ( it_At_DiaSemana->atendimentos_turno == nullptr )
				   it_At_DiaSemana->atendimentos_turno = new GGroup< AtendimentoTurno*, LessPtr<AtendimentoTurno> >();

			   ITERA_GGROUP_LESSPTR( itOp, (*solVarsOp), VariableOp )
			   {
				  VariableOp * v = *itOp;
				  
                  if ( v->getType() != VariableOp::V_X_PROF_AULA_HOR )
                  {
                     continue;
                  }
                  
				  if ( v->getAula() == nullptr )
                  {
					  std::cout<<"\nERRO, aula NULL. Var " << v->toString(); fflush(NULL);
                      exit(1);
                  }

                  if ( v->getAula()->getSala() != sala ||
					   v->getAula()->getDiaSemana() != dia_semana )
                  {
                     continue;
                  }

	//			  std::cout<<"\nv="<<v->toString()<<"..."; fflush(NULL);

                  Aula * aula = v->getAula();

                  // Procura o turno da aula
                  Oferta * temp = ( *( aula->ofertas.begin() ) );
				  if (temp==NULL){ std::cout<<"\nErro, aula sem oferta."; fflush(NULL); continue;}
                  int turnoIdIES = temp->getTurnoId();

                  AtendimentoTurno * atendimento_turno = nullptr;
                  ITERA_GGROUP_LESSPTR( it, ( *it_At_DiaSemana->atendimentos_turno ), AtendimentoTurno )
                  {
                     if ( it->getTurnoId() == turnoIdIES )
                     {
                        atendimento_turno = ( *it );
                        break;
                     }
                  }

                  if ( atendimento_turno == nullptr )
                  {
                     atendimento_turno = new AtendimentoTurno(
                        this->problemSolution->getIdAtendimentos() );

                     atendimento_turno->setId( turnoIdIES );
                     atendimento_turno->setTurnoId( turnoIdIES );
                    // atendimento_turno->turno = temp->turno;

                     it_At_DiaSemana->atendimentos_turno->add( atendimento_turno );
                  }
				  
				  HorarioAula * h = v->getHorarioAula();
				  Calendario * c = h->getCalendario();

	//			  std::cout<<"\nCriando AtendimentoHorarioAula para " << nCreds << " creds..."; fflush(NULL);
				  
				  int nCreds = 1;
				  while ( h!=NULL && nCreds <= aula->getTotalCreditos() )
				  {
	//				  std::cout<<"\nH = "<<h->getId(); fflush(NULL);

                     AtendimentoHorarioAula * atendimento_horario_aula = new AtendimentoHorarioAula(
                        this->problemSolution->getIdAtendimentos() );
					                      
					 Professor * professor = v->getProfessor();

                     atendimento_horario_aula->setId( h->getId() );
                     atendimento_horario_aula->setHorarioAulaId( h->getId() );
                     atendimento_horario_aula->setProfessorId( professor->getId() );
                     atendimento_horario_aula->setProfVirtual( professor->eVirtual() );
                     atendimento_horario_aula->setCreditoTeorico( aula->getCreditosTeoricos() > 0 );
                     atendimento_horario_aula->horario_aula = h;
                     atendimento_horario_aula->professor = professor;
					 					 
					 int counterAlunos = 0;

					 // ----- alunosDemandasAtendidas -----
					 Trio< int /*campusId*/, int /*turma*/, Disciplina* > trio;
					 trio.set( campus->getId(), aula->getTurma(), aula->getDisciplina() );
					 GGroup<AlunoDemanda*, LessPtr< AlunoDemanda >> *alunosDemanda =
						& problemData->mapCampusTurmaDisc_AlunosDemanda[ trio ];

					 std::map<Oferta*, std::map<Disciplina*, GGroup<int /*alDemOrig*/ >, 
						 LessPtr<Disciplina>>, LessPtr<Oferta>> *mapOftDiscorigAlsDem = aula->getMapOftDiscorigAlsDem();
					 std::map<Oferta*, std::map<Disciplina*, GGroup<int /*alDemOrig*/ >, 
						 LessPtr<Disciplina>>, LessPtr<Oferta>>::iterator itOft = mapOftDiscorigAlsDem->begin();
					 for ( ; itOft != mapOftDiscorigAlsDem->end(); itOft++ )
					 {						
						 Oferta* oferta = itOft->first;

						 bool db=false;
						 if ( trio.second == 4 && trio.third->getId() == 14671 && oferta->getId()==457 )
						 {
							 //db=true; 
						 }
						 
						 std::map<Disciplina*, GGroup<int /*alDemOrig*/ >, LessPtr<Disciplina>> *mapDiscOrig = & itOft->second;
						 std::map<Disciplina*, GGroup<int /*alDemOrig*/ >, LessPtr<Disciplina>>::iterator itDiscOrig = mapDiscOrig->begin();
						 for ( ; itDiscOrig != mapDiscOrig->end(); itDiscOrig++ )
						 {							 
							 Disciplina *discOriginal = itDiscOrig->first;

							 if (db)
								std::cout<<"\n\t\tDisc orig " << discOriginal->getId();
							 
							 AtendimentoOferta * atendimento_oferta = new AtendimentoOferta(
							   this->problemSolution->getIdAtendimentos() );
												
							 bool EQUIV;

							 if ( problemData->parametros->considerar_equivalencia_por_aluno && discOriginal != aula->getDisciplina() )
							 {
								atendimento_oferta->setDisciplinaSubstitutaId( aula->getDisciplina()->getId() );
								atendimento_oferta->setDisciplinaId( discOriginal->getId() );
								atendimento_oferta->disciplina = discOriginal;								
								EQUIV = true;
							 }
							 else
							 {
								atendimento_oferta->setDisciplinaId( aula->getDisciplina()->getId() );
								atendimento_oferta->disciplina = aula->getDisciplina();
								EQUIV = false;
						 	 }

							 atendimento_oferta->setTurma( aula->getTurma() );

							 int n = itDiscOrig->second.size();
												
							 atendimento_oferta->alunosDemandasAtendidas.add( itDiscOrig->second );
							 if ( db )
							 {
								 std::cout<<"\natendimento_oferta id=" << atendimento_oferta->getId();
								std::cout<<"\nAlunos em turma == 4 && disciplina->getId() == 14671 && oferta->getId()==457";
								for ( GGroup< int >::iterator it = atendimento_oferta->alunosDemandasAtendidas.begin();
									it!=atendimento_oferta->alunosDemandasAtendidas.end(); it++ )	 
								{
									std::cout<<" " << *it;
								}
							 }

							if ( n == 0 )
								std::cout << "\nErro ao preencher o output operacional. Turma vazia ou alunos-demanda nao encontrados.";

							counterAlunos += atendimento_oferta->alunosDemandasAtendidas.size();

							atendimento_oferta->setQuantidade( n );
							stringstream str;
							str << oferta->getId();
							atendimento_oferta->setOfertaCursoCampiId( str.str() );
							atendimento_oferta->oferta = oferta;

							if(db)
							{
								std::cout<<"\nBefore:";
								ITERA_GGROUP_LESSPTR( itAO, *atendimento_horario_aula->atendimentos_ofertas, AtendimentoOferta )
								{
									std::cout<<" " << itAO->getId();
								}
							}

							atendimento_horario_aula->atendimentos_ofertas->add( atendimento_oferta ); 
							
							if(db)
							{
								std::cout<<"\nAfter:";
								ITERA_GGROUP_LESSPTR( itAO, *atendimento_horario_aula->atendimentos_ofertas, AtendimentoOferta )
								{
									std::cout<<" " << itAO->getId();
								}
							}
						}
	//					std::cout<<"\n\t => fim oferta!"; fflush(NULL);
                     }
					 

					 if ( counterAlunos != alunosDemanda->size() )
						 std::cout	<< "\nErro! Numero de alunos na turma diferente do numero de alunos impressos no output."
						 << "\nTurma " << aula->getTurma() << " Disc " << aula->getDisciplina()->getId() << " Campus " << campus->getId()
						 << "\nNro de alunos impressos: " << counterAlunos << "\tTotal na turma: " << alunosDemanda->size(); fflush(NULL);


                     bool achouAtHA = false;
                     for (GGroup< AtendimentoHorarioAula*, LessPtr<AtendimentoHorarioAula> >::iterator itGHA =  atendimento_turno->atendimentos_horarios_aula->begin();
                        itGHA != atendimento_turno->atendimentos_horarios_aula->end();
                        itGHA++)
                     {
                        AtendimentoHorarioAula *aHA = *itGHA;

                        if ( atendimento_horario_aula->getHorarioAulaId() == aHA->getHorarioAulaId() &&
                             atendimento_horario_aula->getProfessorId() == aHA->getProfessorId() &&
                             atendimento_horario_aula->getCreditoTeorico() == aHA->getCreditoTeorico() &&
                             atendimento_horario_aula->profVirtual() == aHA->profVirtual() )
                        {
                           achouAtHA = true;
                           break;
                        }
                     }

                     if ( !achouAtHA )
                        atendimento_turno->atendimentos_horarios_aula->add( atendimento_horario_aula );
					 else std::cout<<"\nErro, atendimento duplo. Variavel " << v->toString() << endl;

	//				 std::cout<<"\nFim H" << h->getId(); fflush(NULL);

					 // itera
					 h = c->getProximoHorario( h );
					 nCreds++;
                  }

				  nCreds--;
				  if ( nCreds != aula->getTotalCreditos() )
					  std::cout<<"\nErro. Nao foi possivel encontrar os horarios completos da aula: " << aula->toString(); 
				  fflush(NULL);
               }
            }
         }
      }
   }
	std::cout<<" done!\n"; fflush(NULL);
}


void Operacional::criaProfessoresVirtuaisPorCurso( int n, TipoTitulacao *titulacao, TipoContrato *contrato, Curso* curso, GGroup<Campus*, LessPtr<Campus>> campi )
{	
	int idProf = - 1 * (int) problemData->professores_virtuais.size();
	
	for ( int p=1; p <= n; p++ )
	{
		Professor * professor = new Professor( true );   
		idProf--;
		professor->setId( idProf );
		professor->setTitulacaoId( titulacao->getId() );
		professor->titulacao = titulacao;
		std::string nome = professor->getNome();
		stringstream ss; ss << idProf;
		nome += ss.str();
		professor->setNome( nome );
		professor->setCursoAssociado( curso );

		professor->tipo_contrato = contrato;
		professor->setTipoContratoId( contrato->getId() );

		GGroup<Horario*, LessPtr<Horario>> horariosDiscs;

		ITERA_GGROUP_LESSPTR( itCurr, curso->curriculos, Curriculo )
		{
			map < Disciplina*, int, LessPtr< Disciplina > >::iterator
				itMapDisc = itCurr->disciplinas_periodo.begin();
			for ( ; itMapDisc != itCurr->disciplinas_periodo.end(); itMapDisc++ )
			{
				Disciplina* disc = itMapDisc->first;

				GGroup<Disciplina*, LessPtr<Disciplina>> discsCurr;
				discsCurr.add(disc);

				if ( problemData->parametros->considerar_equivalencia_por_aluno )
				ITERA_GGROUP_LESSPTR( itDiscEq, disc->discEquivSubstitutas, Disciplina )
				{
					if ( problemData->ehSubstituivel( disc->getId(), itDiscEq->getId(), curso ) )
						discsCurr.add( *itDiscEq ); 
				}

				ITERA_GGROUP_LESSPTR( itDisciplina, discsCurr, Disciplina )
				{
					Disciplina *disciplina = *itDisciplina;

					if ( ! professor->possuiMagisterioEm( disciplina ) )
					{
						Magisterio * mag = new Magisterio();
						mag->setId( 1 );
						mag->disciplina = ( *itDisciplina );
						mag->setDisciplinaId( disciplina->getId() );
						mag->setNota( 10 );
						mag->setPreferencia( 1 );
						professor->magisterio.add( mag );

						problemData->mapProfessorDisciplinasAssociadas[ professor ].add( disciplina );

						ITERA_GGROUP_LESSPTR( it_h, disciplina->horarios, Horario )
						{							
							Horario * h = new Horario( **it_h );
							h->horario_aula->setCalendario( h->horario_aula->getCalendario() );
							
							GGroup< Horario * >::iterator itHor = professor->horarios.find( h );
							if ( itHor == professor->horarios.end() ) // HorarioAula ainda nao existe para o prof
							{
								professor->horarios.add( h );
								ITERA_GGROUP_N_PT( it_dia, it_h->dias_semana, int )
								{				
									HorarioDia * hd = problemData->getHorarioDiaCorrespondente( h->horario_aula, *it_dia );
									professor->horariosDia.add( hd );
								}
							}
							else	// HorarioAula ja existe para o prof. Adiciona possivelmente mais dias
							{
								delete h;
								ITERA_GGROUP_N_PT( it_dia, it_h->dias_semana, int )
								{	
									GGroup< int >::iterator itDias = itHor->dias_semana.find( *it_dia );
									if ( itDias == itHor->dias_semana.end() )
									{
										itHor->dias_semana.add( *it_dia );

										HorarioDia * hd = problemData->getHorarioDiaCorrespondente( itHor->horario_aula, *it_dia );
										professor->horariosDia.add( hd );
									}
								}
							}

						}

						GGroup< int >::iterator itDias = disciplina->diasLetivos.begin();
						std::pair< int, int > ids_Prof_Disc( professor->getId(), disciplina->getId() );
						for(; itDias != disciplina->diasLetivos.end(); itDias++ )
						{
							problemData->prof_Disc_Dias[ ids_Prof_Disc ].add( *itDias );		
						}
					}
				}
			}
		}

		ITERA_GGROUP_LESSPTR( it_cp, campi, Campus )
		{
			it_cp->professores.add( professor );
		}				

		problemData->professores_virtuais.push_back( professor );

	}

    return;
}

void Operacional::geraMinimoDeProfessoresVirtuaisMIP()
{
	if ( problemData->professores_virtuais.size() > 0 ){
		std::cout<<"\n\nProblemData->professores_virtuais foi esvaziado.\n\n";
		problemData->professores_virtuais.clear();
	}

	// Cria quantos professores virtuais foram estimados necessários para cada curso com a titulação adequada
   ITERA_GGROUP_LESSPTR( itOp, (*solVarsOp), VariableOp )
   {
      VariableOp * v = *itOp;

	  // Limpa a referencia para o professor virtual da solução.
	  // A solucao de profs reais dessa rodada poderá ser usada depois
	  // como solução inicial para a proxima rodada
	  if ( v->getProfessor() != NULL )
	  {
		  if ( v->getProfessor()->eVirtual() )
		  {
			  Professor *p = NULL;
			  v->setProfessor( p );
		  }
	  }
	  
	  TipoContrato *contrato = NULL;

	  TipoTitulacao *titMin = problemData->retornaTipoTitulacaoMinimo();
	  int tit = titMin->getTitulacao();
	  if ( problemData->parametros->min_mestres || problemData->parametros->min_doutores )
	  {
		  if ( v->getType() == VariableOp::V_NRO_PROFS_VIRTUAIS_MEST_CURSO )
		  {
			  tit = TipoTitulacao::Mestre;
		  }
		  else if ( v->getType() == VariableOp::V_NRO_PROFS_VIRTUAIS_DOUT_CURSO )
		  {
			  tit = TipoTitulacao::Doutor;		  
		  }
		  else if ( v->getType() == VariableOp::V_NRO_PROFS_VIRTUAIS_GERAIS_CURSO )
		  {
			  TipoTitulacao *titMin = problemData->retornaTipoTitulacaoMinimo();
			  tit = titMin->getTitulacao();
		  }
		  else continue;
		  
		  contrato = v->getContrato();
	  }
	  else
	  {
		  if ( v->getType() == VariableOp::V_NRO_PROFS_VIRTUAIS_CURSO )
		  {
			  contrato = v->getContrato();

			  TipoTitulacao *titMin = problemData->retornaTipoTitulacaoMinimo();
			  tit = titMin->getTitulacao();
		  }
		  else continue;
	  }

	  TipoTitulacao *titulacao = problemData->retornaTitulacao( tit );	  
	  if ( titulacao==NULL )std::cout<<"\nErro, titulacao "<<tit<< " nao encontrada.";

	  GGroup<Campus*, LessPtr<Campus>> campi;

	  ITERA_GGROUP_LESSPTR( itCp, problemData->campi, Campus )
	  {
		  if ( itCp->cursos.find( v->getCurso() ) != itCp->cursos.end() )
			  campi.add( *itCp );
	  }

	  criaProfessoresVirtuaisPorCurso( (int)(v->getValue() + 0.5) + 2, titulacao, contrato, v->getCurso(), campi );

   }

   // Limpa a lista de profs virtuais estimados a priori.
   // Esta lista só é usada na primeira rodada, quando profs virtuais estão sendo estimados e não têm restrição por horário.
   // Na segunda rodada, professores virtuais têm restrição de horário e estão "infiltrados" na lista de professores reais.
	problemData->profsVirtuais.deleteElements();
	problemData->profsVirtuais.clear();

}

//int Operacional::alteraHorarioAulaAtendimento(
//   const int id_novo_horario_aula, const int id_at_horario )
//{
//   return 1; // ?? TODO
//
//   ITERA_GGROUP( it_at_campi,
//      ( *this->problemSolution->atendimento_campus ), AtendimentoCampus )
//   {
//      Campus * campus = it_at_campi->campus;
//
//      ITERA_GGROUP_LESSPTR( it_at_unidade,
//         ( *it_at_campi->atendimentos_unidades ), AtendimentoUnidade )
//      {
//         Unidade * unidade = it_at_unidade->unidade;
//
//         ITERA_GGROUP_LESSPTR( it_at_sala,
//            ( *it_at_unidade->atendimentos_salas ), AtendimentoSala )
//         {
//            Sala * sala = it_at_sala->sala;
//
//            ITERA_GGROUP_LESSPTR( it_at_dia,
//               ( *it_at_sala->atendimentos_dias_semana ), AtendimentoDiaSemana )
//            {
//               int dia_semana = it_at_dia->getDiaSemana();
//
//               ITERA_GGROUP_LESSPTR( it_at_turno,
//                  ( *it_at_dia->atendimentos_turno ), AtendimentoTurno )
//               {
//                  TurnoIES * turno = it_at_turno->turno;
//
//                  ITERA_GGROUP_LESSPTR( it_at_horario,
//                     ( *it_at_turno->atendimentos_horarios_aula ), AtendimentoHorarioAula )
//                  {
//                     AtendimentoHorarioAula * at_h = ( *it_at_horario );
//
//                     if ( at_h->getId() == id_novo_horario_aula )
//                     {
//                        int id_horario_aula = at_h->getHorarioAulaId();
//
//                        HorarioAula * novo_horario_aula
//                           = problemData->refHorarioAula.find( id_novo_horario_aula )->second;
//
//                        at_h->horario_aula = novo_horario_aula;
//                        at_h->setHorarioAulaId( id_novo_horario_aula );
//
//                        return id_horario_aula;
//                     }
//                  }
//               }
//            }
//         }
//      }
//   }
//
//   return -1;
//}
//
//
//int Operacional::calculaDeslocamentoUnidades(
//   const int id_prof, const int dia )
//{
//   std::vector< AtendimentoBase > atendimentos;
//
//   ITERA_GGROUP( it_at_campi,
//      ( *this->problemSolution->atendimento_campus ), AtendimentoCampus )
//   {
//      Campus * campus = it_at_campi->campus;
//
//      ITERA_GGROUP_LESSPTR( it_at_unidade,
//         ( *it_at_campi->atendimentos_unidades ), AtendimentoUnidade )
//      {
//         Unidade * unidade = it_at_unidade->unidade;
//
//         ITERA_GGROUP_LESSPTR( it_at_sala,
//            ( *it_at_unidade->atendimentos_salas ), AtendimentoSala )
//         {
//            Sala * sala = it_at_sala->sala;
//
//            ITERA_GGROUP_LESSPTR( it_at_dia,
//               ( *it_at_sala->atendimentos_dias_semana ), AtendimentoDiaSemana )
//            {
//               int dia_semana = it_at_dia->getDiaSemana();
//
//               ITERA_GGROUP_LESSPTR( it_at_turno,
//                  ( *it_at_dia->atendimentos_turno ), AtendimentoTurno )
//               {
//                  TurnoIES * turno = it_at_turno->turno;
//
//                  ITERA_GGROUP_LESSPTR( it_at_horario,
//                     ( *it_at_turno->atendimentos_horarios_aula ), AtendimentoHorarioAula )
//                  {
//                     AtendimentoHorarioAula * at_h = ( *it_at_horario );
//
//                     HorarioAula * horario_aula = it_at_horario->horario_aula;
//                     Professor * professor = it_at_horario->professor;
//
//                     if ( dia_semana == dia
//                        && at_h->getProfessorId() == id_prof )
//                     {
//                        AtendimentoBase atendimento;
//
//                        atendimento.unidade = unidade;
//                        atendimento.horario_aula = horario_aula;
//                        atendimento.horario_aula = horario_aula;
//
//                        atendimentos.push_back( atendimento );
//                     }
//                  }
//               }
//            }
//         }
//      }
//   }
//
//   int contDeslocamentos = 0;
//
//   std::sort( atendimentos.begin(), atendimentos.end(),
//      ordenaAtendimentosBaseHorarioAula );
//
//   for ( int i = 0; i < (int)atendimentos.size() - 1; i++ )
//   {
//      Unidade * unidade1 = atendimentos.at( i ).unidade;
//      Unidade * unidade2 = atendimentos.at( i + 1 ).unidade;
//
//      if ( unidade1->getId()  != unidade2->getId() )
//      {
//         contDeslocamentos++;
//      }
//   }
//
//   return contDeslocamentos;
//}
//
//void Operacional::buscaLocalTempoDeslocamentoSolucao()
//{
//   if ( this->problemData->parametros->modo_otimizacao != "OPERACIONAL" )
//   {
//      return;
//   }
//
//   // Dado um turno e um dia da semana, temos
//   // a lista de atendimentos de cada professor
//   std::map< Professor *, std::map< int, GGroup< AtendimentoBase *,
//      LessPtr< AtendimentoBase > > >, LessPtr< Professor > > mapProfessorDiaAtendimentos;
//
//   ITERA_GGROUP( it_at_campi,
//      ( *this->problemSolution->atendimento_campus ), AtendimentoCampus )
//   {
//      Campus * campus = this->problemData->refCampus[ it_at_campi->getId() ];
//
//      ITERA_GGROUP_LESSPTR( it_at_unidade,
//         ( *it_at_campi->atendimentos_unidades ), AtendimentoUnidade )
//      {
//         Unidade * unidade = this->problemData->refUnidade[ it_at_unidade->getId() ];
//
//         ITERA_GGROUP_LESSPTR( it_at_sala,
//            ( *it_at_unidade->atendimentos_salas ), AtendimentoSala )
//         {
//            Sala * sala = this->problemData->refSala[ it_at_sala->getId() ];
//
//            ITERA_GGROUP_LESSPTR( it_at_dia,
//               ( *it_at_sala->atendimentos_dias_semana ), AtendimentoDiaSemana )
//            {
//               int dia_semana = it_at_dia->getDiaSemana();
//
//               ITERA_GGROUP_LESSPTR( it_at_turno,
//                  ( *it_at_dia->atendimentos_turno ), AtendimentoTurno )
//               {
//                 // TurnoIES * turno = this->problemData->findTurno( it_at_turno->getTurnoId() );
//
//                  ITERA_GGROUP_LESSPTR( it_at_horario,
//                     ( *it_at_turno->atendimentos_horarios_aula ), AtendimentoHorarioAula )
//                  {
//                     HorarioAula * horario_aula = this->problemData->findHorarioAula(
//                        it_at_horario->getHorarioAulaId() );
//
//                     Professor * professor = this->problemData->findProfessor(
//                        it_at_horario->getProfessorId() );
//
//                     AtendimentoBase * atendimento = new AtendimentoBase();
//
//                     atendimento->campus = campus;
//                     atendimento->unidade = unidade;
//                     atendimento->sala = sala;
//                     atendimento->dia_semana = dia_semana;
//                   //  atendimento->turno = turno;
//                     atendimento->horario_aula = horario_aula;
//                     atendimento->professor = professor;
//                     atendimento->idAtHorario = it_at_horario->getId();
//
//                     mapProfessorDiaAtendimentos[ professor ][ dia_semana ].add( atendimento );
//                  } // Horário da Aula
//               } // TurnoIES
//            } // Dia da semana
//         } // Sala
//      } // Unidade
//   } // Campus
//
//   std::map< Professor *, std::map< int, GGroup< AtendimentoBase *,
//      LessPtr< AtendimentoBase > > >, LessPtr< Professor > >::iterator
//      it_map = mapProfessorDiaAtendimentos.begin();
//
//   for (; it_map != mapProfessorDiaAtendimentos.end();
//          it_map++ )
//   {
//      Professor * professor = it_map->first;
//
//      std::map< int, GGroup< AtendimentoBase *,
//         LessPtr< AtendimentoBase > > > professorDia = it_map->second;
//
//      std::map< int, GGroup< AtendimentoBase *,
//         LessPtr< AtendimentoBase > > >::iterator
//         it_prof_dia = professorDia.begin();
//
//      for (; it_prof_dia != professorDia.end();
//             it_prof_dia++ )
//      {
//         int dia_semana = it_prof_dia->first;
//
//         // Para haver possibilidade de alterarmos o deslocamento
//         // entre unidades, deve existir pelo menos três atendimentos
//         // para o professor no mesmo dia, e em no mínimo duas unidades distintas
//
//         // Verifica se há pelo menos 3 atendimentos
//         GGroup< AtendimentoBase *,
//            LessPtr< AtendimentoBase > > atendimentos = it_prof_dia->second;
//
//         if ( atendimentos.size() <= 2 )
//         {
//            continue;
//         }
//
//         // Verifica se há pelo menos 2 unidades
//         GGroup< Unidade *, LessPtr< Unidade > > unidadesDistintas;
//
//         GGroup< AtendimentoBase *, LessPtr< AtendimentoBase > >::iterator
//            it_at = atendimentos.begin();
//
//         for (; it_at != atendimentos.end();
//                it_at++ )
//         {
//            unidadesDistintas.add( it_at->unidade );
//         }
//
//         if ( unidadesDistintas.size() <= 1 )
//         {
//            continue;
//         }
//
//         std::vector< AtendimentoBase > vectorAtendimentos;
//         ITERA_GGROUP_LESSPTR( it_at, atendimentos, AtendimentoBase )
//         {
//            vectorAtendimentos.push_back( **it_at );
//         }
//
//         // Armazena todas as combinações possíveis dos
//         // atendimentos nos horários de aula do turno atual
//         std::vector< std::vector< HorarioAula > > arranjosHorariosDia;
//
//         std::vector< HorarioAula > horarios;
//         for ( int i = 0; i < (int)problemData->horarios_aula_ordenados.size(); i++ )
//         {
//            horarios.push_back( ( *problemData->horarios_aula_ordenados.at( i ) ) );
//         }
//
//         Combinatoria< HorarioAula >::arranjos(
//            horarios, (int)atendimentos.size(),  arranjosHorariosDia );
//
//         std::vector< std::vector< HorarioAula > >::iterator
//            it_arranjosHorariosDia = arranjosHorariosDia.begin();
//
//         for (; it_arranjosHorariosDia != arranjosHorariosDia.end();
//                it_arranjosHorariosDia++ )
//         {
//            std::vector< HorarioAula > horarios = ( *it_arranjosHorariosDia );
//            std::list< int > ids_horarios_antigos;
//            ids_horarios_antigos.clear();
//
//            int deslocamentoAnterior = calculaDeslocamentoUnidades(
//               professor->getId(), dia_semana );
//
//            // Realiza a troca de horários
//            for ( int i = 0; i < (int)horarios.size(); i++ )
//            {
//               AtendimentoBase atendimento_base = vectorAtendimentos.at( i );
//               HorarioAula horario_aula = horarios.at( i );
//
//               int horario_antigo = alteraHorarioAulaAtendimento(
//                  horario_aula.getId(), atendimento_base.idAtHorario );
//
//               ids_horarios_antigos.push_back( horario_antigo );
//            }
//
//            int deslocamentoPosterior = calculaDeslocamentoUnidades(
//               professor->getId(), dia_semana );
//
//            bool solucaoValida = validateSolution->checkSolution(
//               this->problemData, this->problemSolution );
//
//            bool melhorouSolucao = ( deslocamentoPosterior < deslocamentoAnterior );
//
//            // Desfaz a troca, caso seja inviável ou
//            // caso não tenha diminuido o deslocamento
//            if ( !solucaoValida || !melhorouSolucao )
//            {
//               for ( int i = 0; i < (int)horarios.size(); i++ )
//               {
//                  AtendimentoBase atendimento_base = vectorAtendimentos.at( i );
//                  int horario_antigo = ids_horarios_antigos.front();
//                  ids_horarios_antigos.pop_front();
//
//                  int horario_antigo_alterado = alteraHorarioAulaAtendimento(
//                     horario_antigo, atendimento_base.idAtHorario );
//               }
//            }
//         }
//      }
//   }
//}


void Operacional::retornaHorariosPossiveis( Professor * prof,
   Aula * aula, std::list< HorarioDia * > & listaHor )
{  
	listaHor.clear();

    // Verifica-se a disponibilidade do
    // professor, da disciplina e da sala
    Disciplina * disc = aula->getDisciplina();
    Sala * sala = aula->getSala();

    int diaS = aula->getDiaSemana();
    int nCredAula = aula->getTotalCreditos();

	int campusId = problemData->retornaCampus( sala->getIdUnidade() )->getId();


	bool debugando = false;
	//if ( prof != NULL )
	if ( //prof->getId() == 1671 && 
		aula->getTurma() == 1 &&
		aula->getDisciplina()->getId() == 13539 &&
		aula->getDiaSemana() == 4 )
	{
		//debugando = true;
		//std::cout<<"\n----retornaHorariosPossiveis";
		//std::cout << "\nAula " << aula->toString();
		//if ( prof != NULL ) std::cout << "   Prof " << prof->getId() << endl;
	}


	// Caso a aula já tenha horarios pre-fixados
	if ( false ) // Por enquanto não tem sentido isso. Só terá sentido quando for pra fixar horários no op.
	if ( aula->getHorariosAulaId().size() > 0 )
	{
		bool disponivel = true;
		
		if ( debugando )
			std::cout<<"\nHors pre-fixados";

		GGroup<int> horsId = aula->getHorariosAulaId();
		ITERA_GGROUP_N_PT( itHorAulaId, horsId, int )
		{
			HorarioAula *ha=NULL;

			if ( debugando )
				std::cout<<"\n\tHor id" << *itHorAulaId;

			ITERA_GGROUP( itSalaHorDia, sala->horariosDia, HorarioDia )
			{
				if ( itSalaHorDia->getHorarioAulaId() == *itHorAulaId &&
					 itSalaHorDia->getDia() == diaS )
					ha = itSalaHorDia->getHorarioAula();
			}
			if ( ha!=NULL )
				disponivel = problemData->verificaDisponibilidadeHorario( ha, diaS, sala, prof, disc );
			else
				disponivel = false;

			if ( debugando )
				std::cout<<"\t\tDisponivel = " << disponivel;


			if (!disponivel) break;
		}	

		if ( disponivel )
		{
			if ( debugando )
			{
				std::cout<<"\n\tDisponivel";
			}

			HorarioDia *horarioDia = problemData->getHorarioDiaCorrespondente( aula->getHorarioAulaInicial(), diaS );		
			listaHor.push_back( horarioDia );
			return;
		}		
	}

	// Caso a aula não tenha horarios pre-fixados
	
	Trio< int /*campusId*/, int /*turma*/, int /*discId*/ > trio;
	trio.set( campusId, aula->getTurma(), disc->getId() );

	GGroup< HorarioDia*, LessPtr< HorarioDia > > horariosDia = problemData->mapTurma_HorComunsAosTurnosIES[trio];
	
	ITERA_GGROUP_LESSPTR( itHor, horariosDia, HorarioDia )
	{		
		if ( itHor->getDia() != diaS ) continue;

		if ( debugando )
			std::cout<<"\n--------\nHor:   dia " << itHor->getDia() << ",  "<< itHor->getHorarioAula()->getInicio() << ", id " << itHor->getHorarioAulaId();

		HorarioAula * hi = (*itHor)->getHorarioAula();	 
		Calendario *calendario = hi->getCalendario();
		HorarioAula *hf = hi;

		GGroup< HorarioAula*, LessPtr< HorarioAula > > hor_intermed;
		bool disponivel = true;
		int i = 1;
		for ( ; i <= nCredAula; i++ )
		{
			if ( hf==NULL )
			{ 
				disponivel=false; 
				break;
			}
				
			if ( hi->getFaseDoDia() != hf->getFaseDoDia() )
			{
				disponivel=false; 
				break;
			}

			disponivel = problemData->verificaDisponibilidadeHorario( hf, diaS, sala, prof, disc );
				
			if ( !disponivel )
			{
				if ( debugando )
					std::cout<<"\n Indisponivel 1 = "<< hf->getId() << " , " << hf->getInicio();

				break;
			}

			HorarioDia *hd = problemData->getHorarioDiaCorrespondente( hf, diaS );

			//disponivel = disponivel && ( problemData->mapTurma_HorComunsAosTurnosIES[trio].find( hd ) !=
			//								problemData->mapTurma_HorComunsAosTurnosIES[trio].end() );	 

			bool found=false;
			GGroup< HorarioDia*, LessPtr<HorarioDia> > *horsComunsTurma = & problemData->mapTurma_HorComunsAosTurnosIES[trio];
			ITERA_GGROUP_LESSPTR( itHD, *horsComunsTurma, HorarioDia )
			{
				if ( itHD->getDia() == hd->getDia() &&
					itHD->getHorarioAula()->getInicio() == hd->getHorarioAula()->getInicio() &&
					itHD->getHorarioAula()->getFinal() == hd->getHorarioAula()->getFinal() )
				{
					found = true;
				}
			}
			if ( !found ) disponivel = false;


			if ( !disponivel )
			{
				if ( debugando )
					std::cout<<"\n Indisponivel 2 = "<< hf->getId() << " , " << hf->getInicio();
				break;
			}

			hor_intermed.add( hf );
				
			if ( debugando )
				std::cout<<"\n    Add = "<< hf->getId() << " , " << hf->getInicio();

		 	if ( i < nCredAula )
				hf = calendario->getProximoHorario( hf );			
		}	 

		if ( debugando )
			std::cout<<"\n    i = "<< i << " nCredAula=" << nCredAula;
		if ( debugando && i<= nCredAula && hf == NULL ) std::cout<<", hf null!";

		if ( !disponivel ) continue;

		if ( debugando )
			std::cout<<"  disponivel ";

		//if ( ! disc->inicioTerminoValidos( hi, hf, diaS, hor_intermed ) ) continue;
		if ( ! disc->inicioTerminoValidos( hi, hf, diaS ) ) continue;

		if ( debugando )
			std::cout<<"   hihf validos, horario IN";

		listaHor.push_back( *itHor );
	}

}



/********************************************************************************************************************
**						        CRIAÇÃO DE VARIÁVEIS DO OPERACIONAL												  **
/********************************************************************************************************************/


bool Operacional::criarVariavelOp( VariableOp v, double &lb )
{
	ITERA_GGROUP_LESSPTR( itSol, (*solVarsOp), VariableOp )
	{
		VariableOp vSol = **itSol;

		if( v.getType() == vSol.getType() )
		{
			if ( vSol.getProfessor() != NULL && 
				!vSol.getProfessor()->eVirtual() )
			{
				if ( vSol == v )
					lb = 1.0;
			}
		}	
	}
	return true;
}

int Operacional::criaVariaveisOperacional()
{
   int numVars = 0;

#ifdef PRINT_cria_variaveis
   int numVarsAnterior = 0;
#endif
   
	CPUTimer timer;
	double dif = 0.0;
	
	std::cout << std::endl;
	
	timer.start();
    if (FIXA_HOR_SOL_TATICO)
		numVars += criaVariavelProfessorAulaHorario_HorsFixoTat();
	else
		numVars += criaVariavelProfessorAulaHorario();
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_variaveis
   std::cout << "numVars V_X_PROF_AULA_HOR: "
             << ( numVars - numVarsAnterior )  <<" " <<dif <<"seg" <<std::endl; fflush(NULL);
   numVarsAnterior = numVars;
#endif

   lp->updateLP();
   timer.start();
   numVars += criaVariavelProfessorDisciplina(); 
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_variaveis
   std::cout << "numVars V_Y_PROF_DISCIPLINA: "
             << ( numVars - numVarsAnterior )  <<" " <<dif <<"seg" <<std::endl; fflush(NULL);
   numVarsAnterior = numVars;
#endif

   lp->updateLP();
   timer.start();
   numVars += criaVariavelDisciplinaHorario__(); 
//   numVars += criaVariavelDisciplinaHorario(); 
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_variaveis
   std::cout << "numVars V_Z_DISCIPLINA_HOR: "
             << ( numVars - numVarsAnterior )  <<" " <<dif <<"seg" <<std::endl; fflush(NULL);
   numVarsAnterior = numVars;
#endif

   lp->updateLP();
   timer.start();
   numVars += criaVariavelProfessorCurso();
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_variaveis
   std::cout << "numVars V_PROF_CURSO: "
             << ( numVars - numVarsAnterior )  <<" " <<dif <<"seg" <<std::endl; fflush(NULL);
   numVarsAnterior = numVars;
#endif

   lp->updateLP();
   timer.start();
   numVars += criaVariavelDiasProfessoresMinistramAulas();
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_variaveis
   std::cout << "numVars V_DIAS_PROF_MINISTRA_AULAS: "
             << ( numVars - numVarsAnterior )  <<" " <<dif <<"seg" <<std::endl; fflush(NULL);
   numVarsAnterior = numVars;
#endif

   lp->updateLP();
   timer.start();
   numVars += criaVariavelDiscProfCurso();
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_variaveis
   std::cout << "numVars V_DISC_PROF_CURSO: "
             << ( numVars - numVarsAnterior )  <<" " <<dif <<"seg" <<std::endl; fflush(NULL);
   numVarsAnterior = numVars;
#endif
   
   lp->updateLP();
   timer.start();
   numVars += criaVariavelDiscProfOferta();
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_variaveis
   std::cout << "numVars V_DISC_PROF_OFERTA: "
             << ( numVars - numVarsAnterior )  <<" " <<dif <<"seg" <<std::endl; fflush(NULL);
   numVarsAnterior = numVars;
#endif


// Restrição agora é forte!
//   lp->updateLP();
//	timer.start();
//   numVars += criaVariavelFolgaMaxDiscProfCurso();
//	timer.stop();
//	dif = timer.getCronoCurrSecs();
//
//#ifdef PRINT_cria_variaveis
//   std::cout << "numVars V_F_MAX_DISC_PROF_CURSO: "
//             << ( numVars - numVarsAnterior )  <<" " <<dif <<"seg" <<std::endl;
//   numVarsAnterior = numVars;
//#endif

   lp->updateLP();
   timer.start();
   numVars += criaVariavelFolgaCargaHorariaMinimaProfessor();
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_variaveis
   std::cout << "numVars V_F_CARGA_HOR_MIN_PROF: "
             << ( numVars - numVarsAnterior )  <<" " <<dif <<"seg" <<std::endl; fflush(NULL);
   numVarsAnterior = numVars;
#endif

   /*

   lp->updateLP();
   numVars += criaVariavelFolgaCargaHorariaMinimaProfessorSemana();

#ifdef PRINT_cria_variaveis
   std::cout << "numVars V_F_CARGA_HOR_MIN_PROF_SEMANA: "
             << ( numVars - numVarsAnterior ) << std::endl;
   numVarsAnterior = numVars;
#endif

   lp->updateLP();
   numVars += criaVariavelFolgaCargaHorariaMaximaProfessorSemana();

#ifdef PRINT_cria_variaveis
   std::cout << "numVars V_F_CARGA_HOR_MAX_PROF_SEMANA: "
             << ( numVars - numVarsAnterior ) << std::endl;
   numVarsAnterior = numVars;
#endif
   */

   lp->updateLP();
   timer.start();
   numVars += criaVariavelFolgaUltimaPrimeiraAulas();
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_variaveis
   std::cout << "numVars V_F_ULTIMA_PRIMEIRA_AULA_PROF: "
             << ( numVars - numVarsAnterior )  <<" " <<dif <<"seg" <<std::endl; fflush(NULL);
   numVarsAnterior = numVars;
#endif

   lp->updateLP();
   if ( this->getRodada() == Operacional::OP_VIRTUAL_INDIVIDUAL ) // Não cria folga de demanda para a rodada de estimativa de professores virtuais (rodada 1)
   {
	   timer.start();
		numVars += criaVariavelFolgaDemanda();
	   timer.stop();
	   dif = timer.getCronoCurrSecs();
	#ifdef PRINT_cria_variaveis
	   std::cout << "numVars V_FOLGA_DEMANDA: "
             << ( numVars - numVarsAnterior )  <<" " <<dif <<"seg" <<std::endl; fflush(NULL);
		numVarsAnterior = numVars;
	#endif   

   }

   lp->updateLP();
   timer.start();
   numVars += criaVariavelFolgaDisciplinaTurmaHorario();
   	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_variaveis
   std::cout << "numVars V_FOLGA_DISC_TURMA_HOR: "
             << ( numVars - numVarsAnterior )  <<" " <<dif <<"seg" <<std::endl; fflush(NULL);
   numVarsAnterior = numVars;
#endif
   
   lp->updateLP();
   timer.start();
   numVars += criaVariavelNroProfsReaisAlocadosCurso();
   	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_variaveis
   std::cout << "numVars V_NRO_PROFS_CURSO: "
             << ( numVars - numVarsAnterior )  <<" " <<dif <<"seg" <<std::endl; fflush(NULL);
   numVarsAnterior = numVars;
#endif   

   lp->updateLP();
   timer.start();
   numVars += criaVariavelNroProfsVirtuaisAlocadosCurso();
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_variaveis
   std::cout << "numVars V_NRO_PROFS_VIRTUAIS_CURSO: "
             << ( numVars - numVarsAnterior )  <<" " <<dif <<"seg" <<std::endl; fflush(NULL);
   numVarsAnterior = numVars;
#endif   

   lp->updateLP();
   timer.start();
   numVars += criaVariavelNroProfsVirtuaisMestresAlocadosCurso();
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_variaveis
   std::cout << "numVars V_NRO_PROFS_VIRTUAIS_MEST_CURSO: "
             << ( numVars - numVarsAnterior )  <<" " <<dif <<"seg" <<std::endl; fflush(NULL);
   numVarsAnterior = numVars;
#endif   


   lp->updateLP();
   timer.start();
   numVars += criaVariavelNroProfsVirtuaisDoutoresAlocadosCurso();
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_variaveis
   std::cout << "numVars V_NRO_PROFS_VIRTUAIS_DOUT_CURSO: "
             << ( numVars - numVarsAnterior )  <<" " <<dif <<"seg" <<std::endl; fflush(NULL);
   numVarsAnterior = numVars;
#endif   
   

   lp->updateLP();
   timer.start();
   numVars += criaVariavelNroProfsVirtuaisGeraisAlocadosCurso();
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_variaveis
   std::cout << "numVars V_NRO_PROFS_VIRTUAIS_GERAIS_CURSO: "
             << ( numVars - numVarsAnterior )  <<" " <<dif <<"seg" <<std::endl; fflush(NULL);
   numVarsAnterior = numVars;
#endif   


   lp->updateLP();
   timer.start();
   numVars += criaVariavelProfessorVirtual();
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_variaveis
   std::cout << "numVars V_PROF_VIRTUAL: "
             << ( numVars - numVarsAnterior ) <<" " <<dif <<"seg" <<std::endl; fflush(NULL);
   numVarsAnterior = numVars;
#endif   
   

   lp->updateLP();
   timer.start();
   numVars += criaVariavelFolgaMinimoDemandaPorAluno();
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_variaveis
   std::cout << "numVars V_FOLGA_ALUNO_MIN_ATEND: "
             << ( numVars - numVarsAnterior ) <<" " <<dif <<"seg" <<std::endl; fflush(NULL);
   numVarsAnterior = numVars;
#endif   
   

//   lp->updateLP();
//   timer.start();
//   numVars += criaVariavelProfessorDiaHorarioIF();
//	timer.stop();
//	dif = timer.getCronoCurrSecs();
//
//#ifdef PRINT_cria_variaveis
//   std::cout << "numVars V_HI_PROFESSORES e V_HF_PROFESSORES: "
//             << ( numVars - numVarsAnterior )  <<" " <<dif <<"seg" <<std::endl; fflush(NULL);
//   numVarsAnterior = numVars;
//#endif


   lp->updateLP();
   timer.start();
   numVars += criaVariaveisHiHfProfFaseDoDiaAPartirDeX();
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_variaveis
   std::cout << "numVars V_HI_PROFESSORES e V_HF_PROFESSORES: "
             << ( numVars - numVarsAnterior )  <<" " <<dif <<"seg" <<std::endl; fflush(NULL);
   numVarsAnterior = numVars;
#endif
   

   lp->updateLP();
   timer.start();
   numVars += criaVariavelFolgaGapProfAPartirDeK();
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_variaveis
   std::cout << "numVars V_FOLGA_GAP_PROF: "
             << ( numVars - numVarsAnterior )  <<" " <<dif <<"seg" <<std::endl; fflush(NULL);
   numVarsAnterior = numVars;
#endif
   
   


   lp->updateLP(); 

   return numVars;
}

// x
int Operacional::criaVariavelProfessorAulaHorario_HorsFixoTat( void )
{
   int num_vars = 0;
   double coeff = 0.0;
   
   GGroup< Professor *, LessPtr< Professor > > professores
     = problemData->getProfessores();
		
	// ============================================================================================================
	 std::cout<<"\nMapeando horarios dos Professores..."; fflush(NULL);

	std::map<Professor*, std::map< int /*dia*/, std::map< DateTime, std::map<DateTime, std::map<int /*turnoIES*/, 
		GGroup<HorarioAula*, LessPtr<HorarioAula>> > > > >, LessPtr<Professor> > mapProfessorDtiDtfHorAula;
	
	ITERA_GGROUP_LESSPTR( itProf, professores, Professor )
	{
		GGroup< Horario * >::iterator itHor = itProf->horarios.begin();
		for( ; itHor != itProf->horarios.end(); itHor++ )
		{
			HorarioAula *ha = itHor->horario_aula;

			ITERA_GGROUP_N_PT( itDia, itHor->dias_semana, int )
			{
				int dia = *itDia;
				mapProfessorDtiDtfHorAula[ *itProf ][ dia ][ ha->getInicio() ][ ha->getFinal() ][ ha->getTurnoIESId() ].add( ha );
			}
		}
	}

	

	std::cout<<"\nMapeando horarios das turmas..."; fflush(NULL);
	
	std::map<Disciplina*, std::map< int /*turma*/, std::map< Aula*, 
				std::map< DateTime, std::map<Calendario*, std::map<int /*turnoIES*/, 
						HorarioAula* >, LessPtr<Calendario> > >, LessPtr<Aula> > > , LessPtr<Disciplina> > mapDiscTurmaDiaDtCalendTurnoHorAula;

    ITERA_GGROUP_LESSPTR( itAula, problemData->aulas, Aula )
    {
		HorarioAula *hi = itAula->getHorarioAulaInicial();
		HorarioAula *hf = itAula->getHorarioAulaFinal();
		Disciplina * discAula = itAula->getDisciplina();
		int turma = itAula->getTurma();
		int dia = itAula->getDiaSemana();
		int campus = itAula->getSala()->getIdCampus();
		int nroCred = itAula->getTotalCreditos();

		if ( hi == NULL || hf == NULL )
			std::cout << "\nErro! Sem horarios nas aulas. Esse metodo pressupoe que exista horario para ser fixado.";

		bool debug=false;
		if ( discAula->getId() == 13747 && turma==1 )
			debug=true;

		int n = 1;
		HorarioAula* h = hi;	
		while ( h!=NULL && n <= nroCred )
		{
			if(debug) std::cout<<"\nDia " << dia <<"  H = " << h->getId() << "  " << h->getInicio();

			Calendario* c = h->getCalendario();
			int turnoId = h->getTurnoIESId();
			DateTime dt = h->getInicio();
			mapDiscTurmaDiaDtCalendTurnoHorAula[discAula][turma][*itAula][dt][c][turnoId] = h;
			h = c->getProximoHorario(h);
			n++;
		}
	}

	// ============================================================================================================
		
	
	#pragma region Filtrando horarios hi-hf das turmas
	std::map<Disciplina*, std::map< int /*turma*/, std::map< Aula*, 
				std::map< DateTime, std::map<Calendario*, std::map<int /*turnoIES*/, 
						HorarioAula* >, LessPtr<Calendario> > >, LessPtr<Aula> > > , LessPtr<Disciplina> >
		 mapDiscTurmaAulaDtCalendTurnoHorAulaInit;
	
	std::cout<<"\nFiltrando horarios hi-hf das turmas..."; fflush(NULL);

	std::map<Disciplina*, std::map< int /*turma*/, std::map< Aula*, 
				std::map< DateTime, std::map<Calendario*, std::map<int /*turnoIES*/, 
						HorarioAula* >, LessPtr<Calendario> > >, LessPtr<Aula> > > , LessPtr<Disciplina> >::iterator
			itDisc = mapDiscTurmaDiaDtCalendTurnoHorAula.begin();
	for( ; itDisc != mapDiscTurmaDiaDtCalendTurnoHorAula.end(); itDisc++ )
	{
		Disciplina* disc = itDisc->first;

		std::map< int /*turma*/, std::map< Aula*, std::map< DateTime, std::map<Calendario*, std::map<int /*turnoIES*/, 
			HorarioAula* >, LessPtr<Calendario> > >, LessPtr<Aula> > >::iterator
			itTurma = itDisc->second.begin();
		for( ; itTurma != itDisc->second.end(); itTurma++ )
		{
			int turma = itTurma->first;

			bool debug=false;
			if ( disc->getId() == 13747 && turma==1 )
				debug=true;

			std::map< Aula*, std::map< DateTime, std::map<Calendario*, std::map<int /*turnoIES*/, 
				HorarioAula* >, LessPtr<Calendario> > >, LessPtr<Aula> >::iterator
				itAula = itTurma->second.begin();
			for( ; itAula != itTurma->second.end(); itAula++ )
			{
				Aula* aula = itAula->first;
				int dia = aula->getDiaSemana();
				int nroCred = aula->getTotalCreditos();

				std::map< DateTime, std::map<Calendario*, std::map<int /*turnoIES*/, 
					HorarioAula* >, LessPtr<Calendario> > >::iterator
					itDti = itAula->second.begin();
				for( ; itDti != itAula->second.end(); itDti++ )
				{
					DateTime dti = itDti->first;							
					int faseDoDiaI = problemData->getFaseDoDia( dti );

					std::map<Calendario*, std::map<int /*turnoIES*/, 
						HorarioAula* >, LessPtr<Calendario> >::iterator
						itCalend = itDti->second.begin();
					for( ; itCalend != itDti->second.end(); itCalend++ )
					{
						Calendario* calend = itCalend->first;

						std::map<int /*turnoIES*/, HorarioAula* >::iterator
							itTurnoIES = itCalend->second.begin();
						for( ; itTurnoIES != itCalend->second.end(); itTurnoIES++ )
						{
							int turnoIES = itTurnoIES->first;
							HorarioAula* hi = itTurnoIES->second;
							
							// -----------------------------------------------------------------
							// Percorre DateTime maior ou igual com mesmo dia, calendario e turno
							
							std::map< DateTime, std::map<Calendario*, std::map<int /*turnoIES*/, 
								HorarioAula* >, LessPtr<Calendario> > >::iterator
								itDtf = itDti;
							for( ; itDtf != itAula->second.end(); itDtf++ )
							{
								// DateTime maior ou igual
								DateTime dtf = itDtf->first;
								
								int faseDoDiaF = problemData->getFaseDoDia( dtf );

								// Impede aulas em fases diferentes do dia (manha, tarde, noite)
								if ( faseDoDiaI != faseDoDiaF )
								{									
									if(debug) 
										std::cout<<"\nNao valido por fase diferente: Dia " << dia
										<< " i " << dti
										 << " f " << dtf;
									continue;
								}

								std::map<Calendario*, std::map<int /*turnoIES*/, 
									HorarioAula* >, LessPtr<Calendario> >::iterator
									itCalendF = itDtf->second.find( calend );
								if( itCalendF != itDtf->second.end() )
								{
									// Mesmo calendario
									Calendario *c = itCalendF->first;

									std::map<int /*turnoIES*/, HorarioAula* >::iterator
										itTurnoF = itCalendF->second.find( turnoIES );
									if( itTurnoF != itCalendF->second.end() )
									{
										// Mesmo turnoIES
										int turnoIES = itTurnoF->first;
										HorarioAula* hf = itTurnoF->second;
																				
										int nroCredEntreHor = c->retornaNroCreditosEntreHorarios( hi,hf );

										if ( nroCredEntreHor == nroCred )
										{											
											if ( disc->inicioTerminoValidos( hi, hf, dia ) )
											{
												mapDiscTurmaAulaDtCalendTurnoHorAulaInit[disc][turma][aula][dti][c][turnoIES] = hi;
											}
											else
											{											
												if(debug) 
													std::cout<<"\nNao valido: Dia " << dia <<"  Hi = " << hi->getId() << "  " << hi->getInicio()
													<< "  Hf = " << hf->getId() << "  " << hf->getInicio();
											}
										}
									}
								}
							}	
							// -----------------------------------------------------------------						
						}
					}
				}
				
				mapDiscTurmaAulaDtCalendTurnoHorAulaInit[disc][turma][aula];

				if ( mapDiscTurmaAulaDtCalendTurnoHorAulaInit[disc][turma][aula].size() == 0 )
					std::cout << "\nErro: aula sem horarios possiveis. Disc " << disc->getId() 
					<< ", turma " << turma << " dia " << aula->getDiaSemana() ;
			}
		}
	}

	mapDiscTurmaDiaDtCalendTurnoHorAula.clear();
	#pragma endregion
	
	std::cout<<"\nCriando vars para profs reais..."; fflush(NULL);

   ITERA_GGROUP_LESSPTR( itProfessor, professores, Professor )
   {
	   Professor * prof = *itProfessor;

        ITERA_GGROUP_LESSPTR( itAula, problemData->aulas, Aula )
        {
			Disciplina * discAula = itAula->getDisciplina();
			int turma = itAula->getTurma();
			int dia = itAula->getDiaSemana();
			int nroCredsAula = itAula->getTotalCreditos();

	//		std::cout << "\nProf " << prof->getId() << " aula " << itAula->toString();	

			std::pair< int, int > prof_disc( itProfessor->getId(), discAula->getId() );
			
			std::map< DateTime, std::map<Calendario*, std::map<int /*turnoIES*/, HorarioAula* >, LessPtr<Calendario> > >
				*mapDtTurma = & mapDiscTurmaAulaDtCalendTurnoHorAulaInit[discAula][turma][*itAula];

			bool debugando = false;
			if ( itAula->getTurma() == 1 &&
				 itAula->getDisciplina()->getId() == -12962 )
			{/*
				debugando = true;
				std::cout<<"\n\n\n---------------------------\nDEBUGANDO!!\n";
				std::cout << "\nAula " << itAula->toString();
				if ( *itProfessor != NULL ) std::cout << "\nProf " << itProfessor->getId() << endl;	*/	
			}
			
			// Se o professor e a disciplina da aula em questão não se relacionarem, pula.
			if ( problemData->prof_Disc_Dias.find( prof_disc )
					== problemData->prof_Disc_Dias.end() )
			{
				if ( debugando ) std::cout << "\nProf e disc nao se relacionam" << endl;	
				continue;
			}
			if ( debugando ) std::cout << "\nProf e disc se relacionam" << endl;	
			

			std::list< HorarioAula* > listaHorarios;

			// --------------------------------------------
			// Horarios habilitados do prof no dia

			std::map< DateTime, std::map<DateTime, std::map<int /*turnoIES*/, 
						GGroup<HorarioAula*, LessPtr<HorarioAula>> > > > *mapDtProf=nullptr;
			std::map<Professor*, std::map< int /*dia*/, std::map< DateTime, std::map<DateTime, std::map<int /*turnoIES*/, 
				GGroup<HorarioAula*, LessPtr<HorarioAula>> > > > >, LessPtr<Professor> >::iterator
				itMapProf = mapProfessorDtiDtfHorAula.find( prof );
			if ( itMapProf != mapProfessorDtiDtfHorAula.end() )
			{
				std::map< int /*dia*/, std::map< DateTime, std::map<DateTime, std::map<int /*turnoIES*/, 
					GGroup<HorarioAula*, LessPtr<HorarioAula>> > > > > *mapDia = &itMapProf->second;

				std::map< int /*dia*/, std::map< DateTime, std::map<DateTime, std::map<int /*turnoIES*/, 
					GGroup<HorarioAula*, LessPtr<HorarioAula>> > > > >::iterator itDia = (*mapDia).find( dia );
				if ( itDia != (*mapDia).end() )
				{
					mapDtProf = & itDia->second;				
				}				
			}

			if( mapDtProf==nullptr ) continue;

			// --------------------------------------------
			// Interseção dos horarios habilitados do prof no dia com os possiveis pra turma

			std::map< DateTime, std::map<Calendario*, std::map<int /*turnoIES*/, 
				HorarioAula* >, LessPtr<Calendario> > >::iterator
				itDtiTurma = mapDtTurma->begin();
			for( ; itDtiTurma != mapDtTurma->end(); itDtiTurma++ )
			{
				// dti: inicio da aula
				DateTime dti = itDtiTurma->first;
				
	//			std::cout << "\nDti " << dti;

				// dti no PROF
				std::map< DateTime, std::map<DateTime, std::map<int /*turnoIES*/, 
					GGroup<HorarioAula*, LessPtr<HorarioAula>> > > >::iterator itDtiProf;
				itDtiProf = (*mapDtProf).find( dti );
				if ( itDtiProf == (*mapDtProf).end() )
					continue;
				
	//			std::cout << "ok " << dti;

				std::map<Calendario*, std::map<int /*turnoIES*/, 
					HorarioAula* >, LessPtr<Calendario> >::iterator
					itCalend = itDtiTurma->second.begin();
				for( ; itCalend != itDtiTurma->second.end(); itCalend++ )
				{
					Calendario* calend = itCalend->first;
					
	//				std::cout << "\tcalend " << calend->getId();

					// dtf: fim do 1o horario da aula
					DateTime dtf = dti;
					dtf.addMinutes( calend->getTempoAula() );
					
	//				std::cout << "\tDtf " << dtf;

					// dtf no PROF
					std::map<DateTime, std::map<int /*turnoIES*/, 
						GGroup<HorarioAula*, LessPtr<HorarioAula>> > >::iterator 
						itDtfProf = itDtiProf->second.find( dtf );
					if ( itDtfProf == itDtiProf->second.end() )
						continue;
				

					std::map<int /*turnoIES*/, HorarioAula* >::iterator
						itTurnoIES = itCalend->second.begin();
					for( ; itTurnoIES != itCalend->second.end(); itTurnoIES++ )
					{
						int turnoIES = itTurnoIES->first;
						HorarioAula* hi = itTurnoIES->second;
							
	//					std::cout << "\t hiId=" << hi->getId() << "  nroCredsAula="<<nroCredsAula;

						// VERIFICA OS HORARIOS ENTRE HI E HF
						bool DISPON=true;
						int n = 1;
						HorarioAula* h=hi;
						while ( h!=NULL && DISPON && n<=nroCredsAula )
						{
		//					std::cout << "\n\tn=" << n;

							// dti de h intermediario no PROF
							std::map< DateTime, std::map<DateTime, std::map<int /*turnoIES*/, 
								GGroup<HorarioAula*, LessPtr<HorarioAula>> > > >::iterator itDtiIntermProf;
							itDtiIntermProf = (*mapDtProf).find( h->getInicio() );
							if ( itDtiIntermProf == (*mapDtProf).end() )
							{
								DISPON=false; break;
							}
		//					std::cout << "  dti ok " << DISPON;

							// dtf de h intermediario no PROF
							std::map<DateTime, std::map<int /*turnoIES*/, 
								GGroup<HorarioAula*, LessPtr<HorarioAula>> > >::iterator 
								itDtfIntermProf = itDtiIntermProf->second.find( h->getFinal() );
							if ( itDtfIntermProf == itDtiIntermProf->second.end() )
							{
								DISPON=false; break;
							}
		//					std::cout << "  dtf ok " << DISPON;

							h = calend->getProximoHorario(h);

		//					std::cout << "  pegou prox h";
							n++;
						}
							
		//				std::cout << "\n\tDISPON=" << DISPON;

						if ( DISPON )
							listaHorarios.push_back( hi );
					}
				}
			}					
						
			if ( listaHorarios.size() == 0 )
			{
				if ( debugando ) std::cout << "\nSem horarios possiveis" << endl;	
				continue;
			}

			auto *mapVarProfDia = & vars_prof_aula2[prof][dia];

			for ( std::list< HorarioAula * >::iterator itHor = listaHorarios.begin();
					itHor != listaHorarios.end(); itHor++ )
			{
				HorarioAula * horarioAula = ( *itHor );
				HorarioDia * horarioDia = problemData->getHorarioDiaCorrespondente( horarioAula, dia );
				
				VariableOp v;
				v.reset();
				v.setType( VariableOp::V_X_PROF_AULA_HOR );

				Aula * aula = ( *itAula );

				v.setAula( aula ); 
				v.setProfessor( *itProfessor );
				v.setHorario( horarioDia );
				v.setHorarioAula( horarioDia->getHorarioAula() );
				v.setDia( horarioDia->getDia() );
				v.setDisciplina( aula->getDisciplina() );
				v.setTurma( aula->getTurma() );
				v.setSala( aula->getSala() );
				v.setUnidade( problemData->refUnidade[ aula->getSala()->getIdUnidade() ] );

				if ( vHashOp.find( v ) == vHashOp.end() )
				{
					int colNr = lp->getNumCols();

					vHashOp[ v ] = colNr;
										
					(*mapVarProfDia)[horarioAula->getInicio()][aula->getCampus()][discAula][turma] 
						= make_pair( colNr, v );
												
					double lb = 0.0;

					OPT_COL col( OPT_COL::VAR_BINARY, coeff, lb, 1.0,
						( char * )v.toString().c_str() );

					lp->newCol( col );
					num_vars++;				  
				}
			}
        }	  
   }
   
   
   if ( problemData->profsVirtuais.size() > 0 )
		std::cout<<"\nCriando vars para profs virtuais..."; fflush(NULL);


   // Professores virtuais para as aulas
   ITERA_GGROUP_LESSPTR( itPV, problemData->profsVirtuais, Professor )
   {
	   Professor *pv = *itPV;   
	   
	   ITERA_GGROUP_LESSPTR( it_aula, problemData->aulas, Aula )
	   {
		   Aula * aula = ( *it_aula );
		   Disciplina * discAula = aula->getDisciplina();
		   int dia = aula->getDiaSemana();
		   int turma = aula->getTurma();

		   std::map< DateTime, std::map<Calendario*, std::map<int /*turnoIES*/, HorarioAula* >, LessPtr<Calendario> > >
			*mapDtTurma = & mapDiscTurmaAulaDtCalendTurnoHorAulaInit[discAula][turma][aula];
		  
		   // lista de horarios possiveis para a aula e sala
		   std::list< HorarioAula * > listaHorarios;


			// --------------------------------------------
			//  horarios possiveis pra turma

			std::map< DateTime, std::map<Calendario*, std::map<int /*turnoIES*/, 
				HorarioAula* >, LessPtr<Calendario> > >::iterator
				itDtiTurma = mapDtTurma->begin();
			for( ; itDtiTurma != mapDtTurma->end(); itDtiTurma++ )
			{
				DateTime dti = itDtiTurma->first;
				
				std::map<Calendario*, std::map<int /*turnoIES*/, 
					HorarioAula* >, LessPtr<Calendario> >::iterator
					itCalend = itDtiTurma->second.begin();
				for( ; itCalend != itDtiTurma->second.end(); itCalend++ )
				{
					Calendario* calend = itCalend->first;
					
					std::map<int /*turnoIES*/, HorarioAula* >::iterator
						itTurnoIES = itCalend->second.begin();
					for( ; itTurnoIES != itCalend->second.end(); itTurnoIES++ )
					{
						int turnoIES = itTurnoIES->first;
						HorarioAula* hi = itTurnoIES->second;
							
						listaHorarios.push_back( hi );
					}
				}
			}
		  
		  if ( listaHorarios.size() == 0 )
		  {
				std::cout<<"\nErro! Nao ha horarios suficientes para alocacao da aula. Verificar se essa solucao tatica esta correta."					
					<< "\nAula: disc " << discAula->getId() << " turma " 
					<< aula->getTurma() << " cp " << aula->getSala()->getIdCampus();
				continue;
		  }

		  auto *mapVarProfDia = & vars_prof_aula2[pv][dia];

		  for ( std::list< HorarioAula * >::iterator itHor = listaHorarios.begin();
				itHor != listaHorarios.end(); itHor++ )
		  {
			  HorarioAula* horarioAula = ( *itHor );
			  HorarioDia * horarioDia = problemData->getHorarioDiaCorrespondente( horarioAula, dia );
			 
			 VariableOp v;
			 v.reset();
			 v.setType( VariableOp::V_X_PROF_AULA_HOR );
			 v.setAula( aula ); 
			 v.setProfessor( pv );
			 v.setHorario( horarioDia );
			 v.setHorarioAula( horarioDia->getHorarioAula() );
			 v.setDia( horarioDia->getDia() );
			 v.setDisciplina( aula->getDisciplina() );
			 v.setTurma( aula->getTurma() );
			 v.setSala( aula->getSala() );
			 v.setUnidade( problemData->refUnidade[ aula->getSala()->getIdUnidade() ] );

			 double coeff = 750.0 * aula->getTotalCreditos();

			 if ( vHashOp.find( v ) == vHashOp.end() )
			 {
				int colNr = lp->getNumCols();

				vHashOp[ v ] = colNr;
										
				(*mapVarProfDia)[horarioAula->getInicio()][aula->getCampus()][discAula][turma] 
					= make_pair( colNr, v );
					
				OPT_COL col( OPT_COL::VAR_BINARY, coeff, 0.0, 1.0,
				   ( char * ) v.toString().c_str() );

				lp->newCol( col );
				num_vars++;
			 }
		  }
	   }
   }


   // ---------------------------------------
   #pragma region Preenche vars_prof_dia_fase_dt

   for ( auto itProf = vars_prof_aula2.begin(); itProf != vars_prof_aula2.end(); itProf++ )
   {
	   Professor *prof = itProf->first;
	   
	   auto *pMapProf = &vars_prof_dia_fase_dt[prof];

	   for ( auto itDia = itProf->second.begin(); itDia != itProf->second.end(); itDia++ )
	   {
		   int dia = itDia->first;

		   auto *pMapDia = &(*pMapProf)[dia];

		   for ( auto itDt = itDia->second.begin(); itDt != itDia->second.end(); itDt++ )
		   {
			   DateTime inicio = itDt->first;
			   
			   pair<DateTime /*dti*/, DateTime /*dtf*/> *pairDtiDtf = nullptr;
			   int fase = CentroDados::getFaseDoDia( inicio );
			   auto finderMapFase = pMapDia->find(fase);
			   if ( finderMapFase != pMapDia->end() )
				   pairDtiDtf = & finderMapFase->second;
			   
			   for ( auto itCp = itDt->second.begin(); itCp != itDt->second.end(); itCp++ )
			   {
				   for ( auto itDisc = itCp->second.begin(); itDisc != itCp->second.end(); itDisc++ )
				   {
					   for ( auto itTurma = itDisc->second.begin(); itTurma != itDisc->second.end(); itTurma++ )
					   {
						    VariableOp v = itTurma->second.second;

							HorarioAula *hInicial = v.getHorarioAula();
							int nCred = v.getAula()->getTotalCreditos();
							Calendario *calendAula = hInicial->getCalendario();
							HorarioAula* hFinal = calendAula->getHorarioMaisNCreds( hInicial, nCred-1 );
							DateTime fim( hFinal->getFinal() );
							
							if (pairDtiDtf==nullptr)
							{
								pairDtiDtf = & (*pMapDia)[fase];
								(*pairDtiDtf) = make_pair(inicio, fim);
							}
							else
							{
								(*pairDtiDtf) = make_pair( min( (*pairDtiDtf).first, inicio ),
														max( (*pairDtiDtf).second, fim ) );
							}
					   }
				   }
			   }
		    }
		}
   }
   #pragma endregion



   return num_vars;
}


// x
int Operacional::criaVariavelProfessorAulaHorario( void )
{
   int num_vars = 0;
   double coeff = 0.0;
   
   GGroup< Professor *, LessPtr< Professor > > professores
     = problemData->getProfessores();

   bool PROF_FULL = false;
		
	// ============================================================================================================
	 std::cout<<"\nMapeando horarios dos Professores..."; fflush(NULL);

	std::map<Professor*, std::map< int /*dia*/, std::map< DateTime, std::map<DateTime, std::map<int /*turnoIES*/, 
		GGroup<HorarioAula*, LessPtr<HorarioAula>> > > > >, LessPtr<Professor> > mapProfessorDtiDtfHorAula;
	
	ITERA_GGROUP_LESSPTR( itProf, professores, Professor )
	{
		GGroup< Horario * >::iterator itHor = itProf->horarios.begin();
		for( ; itHor != itProf->horarios.end(); itHor++ )
		{
			HorarioAula *ha = itHor->horario_aula;

			ITERA_GGROUP_N_PT( itDia, itHor->dias_semana, int )
			{
				int dia = *itDia;
				mapProfessorDtiDtfHorAula[ *itProf ][ dia ][ ha->getInicio() ][ ha->getFinal() ][ ha->getTurnoIESId() ].add( ha );
			}
		}
	}

	

	std::cout<<"\nMapeando horarios das turmas..."; fflush(NULL);
	
	std::map<Disciplina*, std::map< int /*turma*/, std::map< Aula*, 
				std::map< DateTime, std::map<Calendario*, std::map<int /*turnoIES*/, 
						HorarioAula* >, LessPtr<Calendario> > >, LessPtr<Aula> > > , LessPtr<Disciplina> > mapDiscTurmaDiaDtCalendTurnoHorAula;

    ITERA_GGROUP_LESSPTR( itAula, problemData->aulas, Aula )
    {
		Disciplina * discAula = itAula->getDisciplina();
		int turma = itAula->getTurma();
		int dia = itAula->getDiaSemana();
		int campus = itAula->getSala()->getIdCampus();
		int nroCred = itAula->getTotalCreditos();

		Trio< int /*campusId*/, int /*turma*/, Disciplina* > trio ( campus, turma, discAula );

		// TODO: MARRETADO
		AlunoDemanda* alDem = * problemData->mapCampusTurmaDisc_AlunosDemanda[trio].begin();
		TurnoIES* turno = alDem->demanda->oferta->turno;

		ITERA_GGROUP_LESSPTR( itHorAula, turno->horarios_aula, HorarioAula )
		{
			DateTime dt = itHorAula->getInicio();
			Calendario *c = itHorAula->getCalendario();

			if ( c->getTempoAula() == discAula->getTempoCredSemanaLetiva() )
			if ( itHorAula->dias_semana.find( dia ) != itHorAula->dias_semana.end() )
				mapDiscTurmaDiaDtCalendTurnoHorAula[discAula][turma][*itAula][dt][c][turno->getId()] = *itHorAula;
		}
	}

	// ============================================================================================================
		

	#pragma region Filtrando horarios hi-hf das turmas
	std::map<Disciplina*, std::map< int /*turma*/, std::map< Aula*, 
				std::map< DateTime, std::map<Calendario*, std::map<int /*turnoIES*/, 
						HorarioAula* >, LessPtr<Calendario> > >, LessPtr<Aula> > > , LessPtr<Disciplina> >
		 mapDiscTurmaAulaDtCalendTurnoHorAulaInit;
	
	std::cout<<"\nFiltrando horarios hi-hf das turmas..."; fflush(NULL);

	std::map<Disciplina*, std::map< int /*turma*/, std::map< Aula*, 
				std::map< DateTime, std::map<Calendario*, std::map<int /*turnoIES*/, 
						HorarioAula* >, LessPtr<Calendario> > >, LessPtr<Aula> > > , LessPtr<Disciplina> >::iterator
			itDisc = mapDiscTurmaDiaDtCalendTurnoHorAula.begin();
	for( ; itDisc != mapDiscTurmaDiaDtCalendTurnoHorAula.end(); itDisc++ )
	{
		Disciplina* disc = itDisc->first;

		std::map< int /*turma*/, std::map< Aula*, std::map< DateTime, std::map<Calendario*, std::map<int /*turnoIES*/, 
			HorarioAula* >, LessPtr<Calendario> > >, LessPtr<Aula> > >::iterator
			itTurma = itDisc->second.begin();
		for( ; itTurma != itDisc->second.end(); itTurma++ )
		{
			int turma = itTurma->first;

			std::map< Aula*, std::map< DateTime, std::map<Calendario*, std::map<int /*turnoIES*/, 
				HorarioAula* >, LessPtr<Calendario> > >, LessPtr<Aula> >::iterator
				itAula = itTurma->second.begin();
			for( ; itAula != itTurma->second.end(); itAula++ )
			{
				Aula* aula = itAula->first;
				int dia = aula->getDiaSemana();
				int nroCred = aula->getTotalCreditos();

				std::map< DateTime, std::map<Calendario*, std::map<int /*turnoIES*/, 
					HorarioAula* >, LessPtr<Calendario> > >::iterator
					itDti = itAula->second.begin();
				for( ; itDti != itAula->second.end(); itDti++ )
				{
					DateTime dti = itDti->first;							
					int faseDoDiaI = problemData->getFaseDoDia( dti );

					std::map<Calendario*, std::map<int /*turnoIES*/, 
						HorarioAula* >, LessPtr<Calendario> >::iterator
						itCalend = itDti->second.begin();
					for( ; itCalend != itDti->second.end(); itCalend++ )
					{
						Calendario* calend = itCalend->first;

						std::map<int /*turnoIES*/, HorarioAula* >::iterator
							itTurnoIES = itCalend->second.begin();
						for( ; itTurnoIES != itCalend->second.end(); itTurnoIES++ )
						{
							int turnoIES = itTurnoIES->first;
							HorarioAula* hi = itTurnoIES->second;
							
							// -----------------------------------------------------------------
							// Percorre DateTime maior ou igual com mesmo dia, calendario e turno
							
							std::map< DateTime, std::map<Calendario*, std::map<int /*turnoIES*/, 
								HorarioAula* >, LessPtr<Calendario> > >::iterator
								itDtf = itDti;
							for( ; itDtf != itAula->second.end(); itDtf++ )
							{
								// DateTime maior ou igual
								DateTime dtf = itDtf->first;
								
								int faseDoDiaF = problemData->getFaseDoDia( dtf );

								// Impede aulas em fases diferentes do dia (manha, tarde, noite)
								if ( faseDoDiaI != faseDoDiaF )
									continue;

								std::map<Calendario*, std::map<int /*turnoIES*/, 
									HorarioAula* >, LessPtr<Calendario> >::iterator
									itCalendF = itDtf->second.find( calend );
								if( itCalendF != itDtf->second.end() )
								{
									// Mesmo calendario
									Calendario *c = itCalendF->first;

									std::map<int /*turnoIES*/, HorarioAula* >::iterator
										itTurnoF = itCalendF->second.find( turnoIES );
									if( itTurnoF != itCalendF->second.end() )
									{
										// Mesmo turnoIES
										int turnoIES = itTurnoF->first;
										HorarioAula* hf = itTurnoF->second;
																				
										int nroCredEntreHor = c->retornaNroCreditosEntreHorarios( hi,hf );

										if ( nroCredEntreHor == nroCred )
										{											
											if ( disc->inicioTerminoValidos( hi, hf, dia ) )
											{
												mapDiscTurmaAulaDtCalendTurnoHorAulaInit[disc][turma][aula][dti][c][turnoIES] = hi;
											}											
										}
									}
								}
							}	
							// -----------------------------------------------------------------						
						}
					}
				}
				
				mapDiscTurmaAulaDtCalendTurnoHorAulaInit[disc][turma][aula];

				if ( mapDiscTurmaAulaDtCalendTurnoHorAulaInit[disc][turma][aula].size() == 0 )
					std::cout << "\nErro: aula sem horarios possiveis. Disc " << disc->getId() 
					<< ", turma " << turma << " dia " << aula->getDiaSemana() ;
			}
		}
	}

	mapDiscTurmaDiaDtCalendTurnoHorAula.clear();
	#pragma endregion
	
	std::cout<<"\nCriando vars para profs reais..."; fflush(NULL);


   ITERA_GGROUP_LESSPTR( itProfessor, professores, Professor )
   {
	   Professor * prof = *itProfessor;

        ITERA_GGROUP_LESSPTR( itAula, problemData->aulas, Aula )
        {
			Disciplina * discAula = itAula->getDisciplina();
			int turma = itAula->getTurma();
			int dia = itAula->getDiaSemana();
			int nroCredsAula = itAula->getTotalCreditos();

			std::pair< int, int > prof_disc( itProfessor->getId(), discAula->getId() );
			
			std::map< DateTime, std::map<Calendario*, std::map<int /*turnoIES*/, HorarioAula* >, LessPtr<Calendario> > >
				*mapDtTurma = & mapDiscTurmaAulaDtCalendTurnoHorAulaInit[discAula][turma][*itAula];

			bool debugando = false;
			if ( itAula->getTurma() == 1 &&
				 itAula->getDisciplina()->getId() == -12962 )
			{/*
				debugando = true;
				std::cout<<"\n\n\n---------------------------\nDEBUGANDO!!\n";
				std::cout << "\nAula " << itAula->toString();
				if ( *itProfessor != NULL ) std::cout << "\nProf " << itProfessor->getId() << endl;	*/	
			}
			
			// Se o professor e a disciplina da aula em questão se relacionarem:
			if ( problemData->prof_Disc_Dias.find( prof_disc )
					== problemData->prof_Disc_Dias.end() )
			{
				if ( debugando ) std::cout << "\nProf e disc nao se relacionam" << endl;	
				continue;
			}
			if ( debugando ) std::cout << "\nProf e disc se relacionam" << endl;	
			

			std::list< HorarioAula* > listaHorarios;

			// --------------------------------------------
			// Horarios habilitados do prof no dia

			std::map< DateTime, std::map<DateTime, std::map<int /*turnoIES*/, 
						GGroup<HorarioAula*, LessPtr<HorarioAula>> > > > *mapDtProf=NULL;
			std::map<Professor*, std::map< int /*dia*/, std::map< DateTime, std::map<DateTime, std::map<int /*turnoIES*/, 
				GGroup<HorarioAula*, LessPtr<HorarioAula>> > > > >, LessPtr<Professor> >::iterator
				itMapProf = mapProfessorDtiDtfHorAula.find( prof );
			if ( itMapProf != mapProfessorDtiDtfHorAula.end() )
			{
				std::map< int /*dia*/, std::map< DateTime, std::map<DateTime, std::map<int /*turnoIES*/, 
					GGroup<HorarioAula*, LessPtr<HorarioAula>> > > > > *mapDia = &itMapProf->second;

				std::map< int /*dia*/, std::map< DateTime, std::map<DateTime, std::map<int /*turnoIES*/, 
					GGroup<HorarioAula*, LessPtr<HorarioAula>> > > > >::iterator itDia = (*mapDia).find( dia );
				if ( itDia != (*mapDia).end() )
				{
					mapDtProf = & itDia->second;				
				}				
			}
			
			if( mapDtProf==NULL ) continue;

			// --------------------------------------------
			// Interseção dos horarios habilitados do prof no dia com os possiveis pra turma

			std::map< DateTime, std::map<Calendario*, std::map<int /*turnoIES*/, 
				HorarioAula* >, LessPtr<Calendario> > >::iterator
				itDtiTurma = mapDtTurma->begin();
			for( ; itDtiTurma != mapDtTurma->end(); itDtiTurma++ )
			{
				DateTime dti = itDtiTurma->first;
				
				// dti no PROF
				std::map< DateTime, std::map<DateTime, std::map<int /*turnoIES*/, 
					GGroup<HorarioAula*, LessPtr<HorarioAula>> > > >::iterator itDtiProf;				
				if ( !PROF_FULL )
				{
					itDtiProf = (*mapDtProf).find( dti );
					if ( itDtiProf == (*mapDtProf).end() )
						continue;
				}

				std::map<Calendario*, std::map<int /*turnoIES*/, 
					HorarioAula* >, LessPtr<Calendario> >::iterator
					itCalend = itDtiTurma->second.begin();
				for( ; itCalend != itDtiTurma->second.end(); itCalend++ )
				{
					Calendario* calend = itCalend->first;
					
					if ( !PROF_FULL )
					{
						DateTime dtf = dti;
						dtf.addMinutes( calend->getTempoAula() );

						// dtf no PROF
						std::map<DateTime, std::map<int /*turnoIES*/, 
							GGroup<HorarioAula*, LessPtr<HorarioAula>> > >::iterator 
							itDtfProf = itDtiProf->second.find( dtf );
						if ( itDtfProf == itDtiProf->second.end() )
							continue;
					}

					std::map<int /*turnoIES*/, HorarioAula* >::iterator
						itTurnoIES = itCalend->second.begin();
					for( ; itTurnoIES != itCalend->second.end(); itTurnoIES++ )
					{
						int turnoIES = itTurnoIES->first;
						HorarioAula* hi = itTurnoIES->second;
							
						// VERIFICA OS HORARIOS ENTRE HI E HF
						bool DISPON=true;
						int n = 1;
						HorarioAula* h=hi;

						if ( !PROF_FULL )
						while ( h!=NULL && DISPON && n<=nroCredsAula )
						{
							// dti de h intermediario no PROF
							std::map< DateTime, std::map<DateTime, std::map<int /*turnoIES*/, 
								GGroup<HorarioAula*, LessPtr<HorarioAula>> > > >::iterator itDtiIntermProf;
							itDtiIntermProf = (*mapDtProf).find( h->getInicio() );
							if ( itDtiIntermProf == (*mapDtProf).end() )
							{
								DISPON=false; break;
							}

							// dtf de h intermediario no PROF
							std::map<DateTime, std::map<int /*turnoIES*/, 
								GGroup<HorarioAula*, LessPtr<HorarioAula>> > >::iterator 
								itDtfIntermProf = itDtiIntermProf->second.find( h->getFinal() );
							if ( itDtfIntermProf == itDtiIntermProf->second.end() )
							{
								DISPON=false; break;
							}

							h = calend->getProximoHorario(h);
							n++;
						}
							
						if ( DISPON )
							listaHorarios.push_back( hi );
					}
				}
			}
						

			if ( listaHorarios.size() == 0 )
			{
				if ( debugando ) std::cout << "\nSem horarios possiveis" << endl;	
				continue;
			}

			auto *mapVarProfDia = & vars_prof_aula2[prof][dia];

			for ( std::list< HorarioAula * >::iterator itHor = listaHorarios.begin();
					itHor != listaHorarios.end(); itHor++ )
			{
				HorarioAula * horarioAula = ( *itHor );
				HorarioDia * horarioDia = problemData->getHorarioDiaCorrespondente( horarioAula, dia );
				
				VariableOp v;
				v.reset();
				v.setType( VariableOp::V_X_PROF_AULA_HOR );

				Aula * aula = ( *itAula );

				v.setAula( aula ); 
				v.setProfessor( *itProfessor );
				v.setHorario( horarioDia );
				v.setHorarioAula( horarioDia->getHorarioAula() );
				v.setDia( horarioDia->getDia() );
				v.setDisciplina( aula->getDisciplina() );
				v.setTurma( aula->getTurma() );
				v.setSala( aula->getSala() );
				v.setUnidade( problemData->refUnidade[ aula->getSala()->getIdUnidade() ] );

				if ( vHashOp.find( v ) == vHashOp.end() )
				{
					int colNr = lp->getNumCols();

					vHashOp[ v ] = colNr;
										
					(*mapVarProfDia)[horarioAula->getInicio()][aula->getCampus()][discAula][turma] 
						= make_pair( colNr, v );

					double lb = 0.0;

					OPT_COL col( OPT_COL::VAR_BINARY, coeff, lb, 1.0,
						( char * )v.toString().c_str() );

					lp->newCol( col );
					num_vars++;				  
				}
			}
        }	  
   }
   

   
   if ( problemData->profsVirtuais.size() > 0 )
		std::cout<<"\nCriando vars para profs virtuais..."; fflush(NULL);


   // Professores virtuais para as aulas
   ITERA_GGROUP_LESSPTR( itPV, problemData->profsVirtuais, Professor )
   {
	   Professor *pv = *itPV;   
	   
	   ITERA_GGROUP_LESSPTR( it_aula, problemData->aulas, Aula )
	   {
		    Aula * aula = ( *it_aula );
		    Disciplina * discAula = aula->getDisciplina();
		    int dia = aula->getDiaSemana();
		    int turma = aula->getTurma();

		    std::map< DateTime, std::map<Calendario*, std::map<int /*turnoIES*/, HorarioAula* >, LessPtr<Calendario> > >
			 *mapDtTurma = & mapDiscTurmaAulaDtCalendTurnoHorAulaInit[discAula][turma][aula];
		  
		    // lista de horarios possiveis para a aula e sala
		    std::list< HorarioAula * > listaHorarios;


			// --------------------------------------------
			//  horarios possiveis pra turma

			std::map< DateTime, std::map<Calendario*, std::map<int /*turnoIES*/, 
				HorarioAula* >, LessPtr<Calendario> > >::iterator
				itDtiTurma = mapDtTurma->begin();
			for( ; itDtiTurma != mapDtTurma->end(); itDtiTurma++ )
			{
				DateTime dti = itDtiTurma->first;
				
				std::map<Calendario*, std::map<int /*turnoIES*/, 
					HorarioAula* >, LessPtr<Calendario> >::iterator
					itCalend = itDtiTurma->second.begin();
				for( ; itCalend != itDtiTurma->second.end(); itCalend++ )
				{
					Calendario* calend = itCalend->first;
					
					std::map<int /*turnoIES*/, HorarioAula* >::iterator
						itTurnoIES = itCalend->second.begin();
					for( ; itTurnoIES != itCalend->second.end(); itTurnoIES++ )
					{
						int turnoIES = itTurnoIES->first;
						HorarioAula* hi = itTurnoIES->second;
							
						listaHorarios.push_back( hi );
					}
				}
			}
		  
			if ( listaHorarios.size() == 0 )
			{
				std::cout<<"\nErro! Nao ha horarios suficientes para alocacao da aula. Verificar se essa solucao tatica esta correta."					
					<< "\nAula: disc " << discAula->getId() << " turma " 
					<< aula->getTurma() << " cp " << aula->getSala()->getIdCampus();
				continue;
			}
		  
			auto *mapVarProfDia = & vars_prof_aula2[pv][dia];

			for ( std::list< HorarioAula * >::iterator itHor = listaHorarios.begin();
				itHor != listaHorarios.end(); itHor++ )
			{
				HorarioAula* horarioAula = ( *itHor );
				HorarioDia * horarioDia = problemData->getHorarioDiaCorrespondente( horarioAula, dia );
			 
				VariableOp v;
				v.reset();
				v.setType( VariableOp::V_X_PROF_AULA_HOR );
				v.setAula( aula ); 
				v.setProfessor( pv );
				v.setHorario( horarioDia );
				v.setHorarioAula( horarioDia->getHorarioAula() );
				v.setDia( horarioDia->getDia() );
				v.setDisciplina( aula->getDisciplina() );
				v.setTurma( aula->getTurma() );
				v.setSala( aula->getSala() );
				v.setUnidade( problemData->refUnidade[ aula->getSala()->getIdUnidade() ] );

				double coeff = 750.0 * aula->getTotalCreditos();

				if ( vHashOp.find( v ) == vHashOp.end() )
				{
					int colNr = lp->getNumCols();

					vHashOp[ v ] = colNr;
										
					(*mapVarProfDia)[horarioAula->getInicio()][aula->getCampus()][discAula][turma] 
						= make_pair( colNr, v );
				
					OPT_COL col( OPT_COL::VAR_BINARY, coeff, 0.0, 1.0,
						( char * ) v.toString().c_str() );

					lp->newCol( col );
					num_vars++;
				}
			}
	   }
   }
   

   // ---------------------------------------
   #pragma region Preenche vars_prof_dia_fase_dt

   for ( auto itProf = vars_prof_aula2.begin(); itProf != vars_prof_aula2.end(); itProf++ )
   {
	   Professor *prof = itProf->first;
	   
	   auto *pMapProf = &vars_prof_dia_fase_dt[prof];

	   for ( auto itDia = itProf->second.begin(); itDia != itProf->second.end(); itDia++ )
	   {
		   int dia = itDia->first;

		   auto *pMapDia = &(*pMapProf)[dia];

		   for ( auto itDt = itDia->second.begin(); itDt != itDia->second.end(); itDt++ )
		   {
			   DateTime inicio = itDt->first;
			   
			   pair<DateTime /*dti*/, DateTime /*dtf*/> *pairDtiDtf = nullptr;
			   int fase = CentroDados::getFaseDoDia( inicio );
			   auto finderMapFase = pMapDia->find(fase);
			   if ( finderMapFase != pMapDia->end() )
				   pairDtiDtf = & finderMapFase->second;
			   
			   for ( auto itCp = itDt->second.begin(); itCp != itDt->second.end(); itCp++ )
			   {
				   for ( auto itDisc = itCp->second.begin(); itDisc != itCp->second.end(); itDisc++ )
				   {
					   for ( auto itTurma = itDisc->second.begin(); itTurma != itDisc->second.end(); itTurma++ )
					   {
						    VariableOp v = itTurma->second.second;

							HorarioAula *hInicial = v.getHorarioAula();
							int nCred = v.getAula()->getTotalCreditos();
							Calendario *calendAula = hInicial->getCalendario();
							HorarioAula* hFinal = calendAula->getHorarioMaisNCreds( hInicial, nCred-1 );
							DateTime fim( hFinal->getFinal() );
							
							if (pairDtiDtf==nullptr)
							{
								pairDtiDtf = & (*pMapDia)[fase];
								(*pairDtiDtf) = make_pair(inicio, fim);
							}
							else
							{
								(*pairDtiDtf) = make_pair( min( (*pairDtiDtf).first, inicio ),
														max( (*pairDtiDtf).second, fim ) );
							}
					   }
				   }
			   }
		    }
		}
   }
   #pragma endregion


   return num_vars;
}

// y
int Operacional::criaVariavelProfessorDisciplina()
{
   int num_vars = 0;

   double coeff = 0.0;
   double pesoNota = 1;
   double pesoPreferencia = 1;

   if ( !problemData->parametros->considerar_preferencia_prof )
   {
		pesoPreferencia = 0;
   }
   if ( !problemData->parametros->considerar_desempenho_prof )
   {
		pesoNota = 0;
   }

   GGroup< Professor *, LessPtr< Professor > > professores
      = problemData->getProfessores();

   ITERA_GGROUP_LESSPTR( itProfessor, professores, Professor )
   {
	  double custoCredito = (*itProfessor)->getValorCredito();

      ITERA_GGROUP_LESSPTR( itMagisterio, itProfessor->magisterio, Magisterio )
      {
         Disciplina * discMinistradaProf = itMagisterio->disciplina;

         int nota = itMagisterio->getNota();
         int preferencia = itMagisterio->getPreferencia();
		 
         ITERA_GGROUP_LESSPTR( itAula, problemData->aulas, Aula )
         {
			Aula *aula = *itAula;
            Disciplina * discAula = aula->getDisciplina();

            if ( discMinistradaProf != discAula )
            {
               continue;
            }

            std::pair< int, int > prof_disc ( itProfessor->getId(),
               discMinistradaProf->getId() );

            // Se o professor e a disciplina da aula em questão se relacionarem:
            if ( problemData->prof_Disc_Dias.find( prof_disc )
                  == problemData->prof_Disc_Dias.end() )
            {
               continue;
            }
			
			Campus *campus = problemData->retornaCampus( itAula->getSala()->getIdUnidade() );

			if ( campus->professores.find( *itProfessor ) == campus->professores.end() )
				continue;

            VariableOp v;
            v.reset();
            v.setType( VariableOp::V_Y_PROF_DISCIPLINA );

            v.setDisciplina( discAula ); 
            v.setProfessor( ( *itProfessor ) );
            v.setTurma( itAula->getTurma() );
			v.setCampus( campus );

            //------------------------------------------------------
            // Preferência:
            // 10  --> Maior preferência
            // 1 --> Menor preferência
            //------------------------------------------------------
            // Nota de desempenho:
            // 1  --> Menor desempenho
            // 10 --> Maior desempenho
            //------------------------------------------------------
            // Assim, o 'peso' da preferência e da nota
            // de desempenho na função objetivo variam
            // entre 0 e 9, sendo 0 o MELHOR CASO e 9 o PIOR CASO
            //------------------------------------------------------
            coeff = ( ( 10 - nota ) * ( pesoNota )
               + ( preferencia ) * ( pesoPreferencia )
			   + custoCredito * aula->getDisciplina()->getTotalCreditos() );

            if ( vHashOp.find( v ) == vHashOp.end() )
            {
               vHashOp[ v ] = lp->getNumCols();

               OPT_COL col( OPT_COL::VAR_BINARY, coeff, 0.0, 1.0,
                  ( char * ) v.toString().c_str() );

               lp->newCol( col );
               num_vars++;
            }
         }
      }
   }

   int nota = 1;
   int preferencia = 1;
   double custoCredito = 100; // todo

   // Professores virtuais para as aulas
   ITERA_GGROUP_LESSPTR( itPV, problemData->profsVirtuais, Professor )
   {
	   Professor *pv = *itPV;   
	   
	   ITERA_GGROUP_LESSPTR( it_aula, problemData->aulas, Aula )
	   {
		    Aula * aula = ( *it_aula );
		    Disciplina * discAula = aula->getDisciplina();

		    Campus *campus = problemData->retornaCampus( aula->getSala()->getIdUnidade() );
		  
			VariableOp v;
			v.reset();
			v.setType( VariableOp::V_Y_PROF_DISCIPLINA );
			v.setDisciplina( discAula ); 
            v.setProfessor( pv );
            v.setTurma( aula->getTurma() );
			v.setCampus( campus );

			//------------------------------------------------------
			// Preferência:
			// 10  --> Maior preferência
			// 1 --> Menor preferência
			//------------------------------------------------------
			// Nota de desempenho:
			// 1  --> Menor desempenho
			// 10 --> Maior desempenho
			//------------------------------------------------------
			// Assim, o 'peso' da preferência e da nota
			// de desempenho na função objetivo variam
			// entre 0 e 9, sendo 0 o MELHOR CASO e 9 o PIOR CASO
		 	//------------------------------------------------------
			coeff = ( ( 10 - nota ) * ( pesoNota )
			+ ( preferencia ) * ( pesoPreferencia )
			+ custoCredito * aula->getDisciplina()->getTotalCreditos() );

			if ( vHashOp.find( v ) == vHashOp.end() )
			{
				vHashOp[ v ] = lp->getNumCols();

				OPT_COL col( OPT_COL::VAR_BINARY, coeff, 0.0, 1.0,
					( char * ) v.toString().c_str() );

				lp->newCol( col );
				num_vars++;
			}
	   }
   }
   return num_vars;
}

// z
int Operacional::criaVariavelDisciplinaHorario__()
{
   int num_vars = 0;
   double coeff = 0.0;


	//std::cout<<"\nMapeando horarios das turmas...";
	
	std::map<Disciplina*, std::map< int /*turma*/, std::map< Aula*, 
				std::map< DateTime, std::map<Calendario*, std::map<int /*turnoIES*/, 
						HorarioAula* >, LessPtr<Calendario> > >, LessPtr<Aula> > > , LessPtr<Disciplina> > mapDiscTurmaDiaDtCalendTurnoHorAula;

    ITERA_GGROUP_LESSPTR( itAula, problemData->aulas, Aula )
    {
		Disciplina * discAula = itAula->getDisciplina();
		int turma = itAula->getTurma();
		int dia = itAula->getDiaSemana();
		int campus = itAula->getSala()->getIdCampus();
		int nroCred = itAula->getTotalCreditos();

		Trio< int /*campusId*/, int /*turma*/, Disciplina* > trio ( campus, turma, discAula );

		// MARRETADO
		AlunoDemanda* alDem = * problemData->mapCampusTurmaDisc_AlunosDemanda[trio].begin();
		TurnoIES* turno = alDem->demanda->oferta->turno;

		ITERA_GGROUP_LESSPTR( itHorAula, turno->horarios_aula, HorarioAula )
		{
			DateTime dt = itHorAula->getInicio();
			Calendario *c = itHorAula->getCalendario();
			
			if ( c->getTempoAula() == discAula->getTempoCredSemanaLetiva() )
			if ( itHorAula->dias_semana.find( dia ) != itHorAula->dias_semana.end() )
				mapDiscTurmaDiaDtCalendTurnoHorAula[discAula][turma][*itAula][dt][c][turno->getId()] = *itHorAula;
		}
	}

	// ============================================================================================================
		

	std::map<Disciplina*, std::map< int /*turma*/, std::map< Aula*, 
				std::map< DateTime, std::map<Calendario*, std::map<int /*turnoIES*/, 
						HorarioAula* >, LessPtr<Calendario> > >, LessPtr<Aula> > > , LessPtr<Disciplina> >
		 mapDiscTurmaAulaDtCalendTurnoHorAulaInit;
	
	//std::cout<<"\nFiltrando horarios hi-hf das turmas...\n";

	std::map<Disciplina*, std::map< int /*turma*/, std::map< Aula*, 
				std::map< DateTime, std::map<Calendario*, std::map<int /*turnoIES*/, 
						HorarioAula* >, LessPtr<Calendario> > >, LessPtr<Aula> > > , LessPtr<Disciplina> >::iterator
			itDisc = mapDiscTurmaDiaDtCalendTurnoHorAula.begin();
	for( ; itDisc != mapDiscTurmaDiaDtCalendTurnoHorAula.end(); itDisc++ )
	{
		Disciplina* disc = itDisc->first;

		std::map< int /*turma*/, std::map< Aula*, std::map< DateTime, std::map<Calendario*, std::map<int /*turnoIES*/, 
			HorarioAula* >, LessPtr<Calendario> > >, LessPtr<Aula> > >::iterator
			itTurma = itDisc->second.begin();
		for( ; itTurma != itDisc->second.end(); itTurma++ )
		{
			int turma = itTurma->first;

			std::map< Aula*, std::map< DateTime, std::map<Calendario*, std::map<int /*turnoIES*/, 
				HorarioAula* >, LessPtr<Calendario> > >, LessPtr<Aula> >::iterator
				itAula = itTurma->second.begin();
			for( ; itAula != itTurma->second.end(); itAula++ )
			{
				Aula* aula = itAula->first;
				int dia = aula->getDiaSemana();
				int nroCred = aula->getTotalCreditos();

				std::map< DateTime, std::map<Calendario*, std::map<int /*turnoIES*/, 
					HorarioAula* >, LessPtr<Calendario> > >::iterator
					itDti = itAula->second.begin();
				for( ; itDti != itAula->second.end(); itDti++ )
				{
					DateTime dti = itDti->first;							
					int faseDoDiaI = problemData->getFaseDoDia( dti );

					std::map<Calendario*, std::map<int /*turnoIES*/, 
						HorarioAula* >, LessPtr<Calendario> >::iterator
						itCalend = itDti->second.begin();
					for( ; itCalend != itDti->second.end(); itCalend++ )
					{
						Calendario* calend = itCalend->first;

						std::map<int /*turnoIES*/, HorarioAula* >::iterator
							itTurnoIES = itCalend->second.begin();
						for( ; itTurnoIES != itCalend->second.end(); itTurnoIES++ )
						{
							int turnoIES = itTurnoIES->first;
							HorarioAula* hi = itTurnoIES->second;
							
							// -----------------------------------------------------------------
							// Percorre DateTime maior ou igual com mesmo dia, calendario e turno
							
							std::map< DateTime, std::map<Calendario*, std::map<int /*turnoIES*/, 
								HorarioAula* >, LessPtr<Calendario> > >::iterator
								itDtf = itDti;
							for( ; itDtf != itAula->second.end(); itDtf++ )
							{
								// DateTime maior ou igual
								DateTime dtf = itDtf->first;
								
								int faseDoDiaF = problemData->getFaseDoDia( dtf );

								// Impede aulas em fases diferentes do dia (manha, tarde, noite)
								if ( faseDoDiaI != faseDoDiaF )
									continue;

								std::map<Calendario*, std::map<int /*turnoIES*/, 
									HorarioAula* >, LessPtr<Calendario> >::iterator
									itCalendF = itDtf->second.find( calend );
								if( itCalendF != itDtf->second.end() )
								{
									// Mesmo calendario
									Calendario *c = itCalendF->first;

									std::map<int /*turnoIES*/, HorarioAula* >::iterator
										itTurnoF = itCalendF->second.find( turnoIES );
									if( itTurnoF != itCalendF->second.end() )
									{
										// Mesmo turnoIES
										int turnoIES = itTurnoF->first;
										HorarioAula* hf = itTurnoF->second;

										// TODO: CONFERIR SE OS HORARIOS INTERMEDIARIOS ESTAO PRESENTES NA DISICPLINA

										int nroCredEntreHor = c->retornaNroCreditosEntreHorarios( hi,hf );

										if ( nroCredEntreHor == nroCred )
										if ( disc->inicioTerminoValidos( hi, hf, dia ) )
										{
											mapDiscTurmaAulaDtCalendTurnoHorAulaInit[disc][turma][aula][dti][c][turnoIES] = hi;
										}
									}
								}
							}	
							// -----------------------------------------------------------------						
						}
					}
				}
				
				mapDiscTurmaAulaDtCalendTurnoHorAulaInit[disc][turma][aula];

				if ( mapDiscTurmaAulaDtCalendTurnoHorAulaInit[disc][turma][aula].size() == 0 )
					std::cout << "\nErro: aula sem horarios possiveis. Disc " << disc->getId() 
					<< ", turma " << turma << " dia " << aula->getDiaSemana() ;
			}
		}
	}

	mapDiscTurmaDiaDtCalendTurnoHorAula.clear();
	


   ITERA_GGROUP_LESSPTR( itAula, problemData->aulas, Aula )
   {
		Aula * aula = ( *itAula );
	    Disciplina * discAula = itAula->getDisciplina();
	    int turma = itAula->getTurma();
		int dia = itAula->getDiaSemana();

		std::map< DateTime, std::map<Calendario*, std::map<int /*turnoIES*/, HorarioAula* >, LessPtr<Calendario> > >
		*mapDtTurma = & mapDiscTurmaAulaDtCalendTurnoHorAulaInit[discAula][turma][*itAula];
		  
		// lista de horarios possiveis para a aula e sala
		std::list< HorarioAula * > listaHorarios;


		// --------------------------------------------
		//  horarios possiveis pra turma

		std::map< DateTime, std::map<Calendario*, std::map<int /*turnoIES*/, 
			HorarioAula* >, LessPtr<Calendario> > >::iterator
			itDtiTurma = mapDtTurma->begin();
		for( ; itDtiTurma != mapDtTurma->end(); itDtiTurma++ )
		{
			DateTime dti = itDtiTurma->first;
				
			std::map<Calendario*, std::map<int /*turnoIES*/, 
				HorarioAula* >, LessPtr<Calendario> >::iterator
				itCalend = itDtiTurma->second.begin();
			for( ; itCalend != itDtiTurma->second.end(); itCalend++ )
			{
				Calendario* calend = itCalend->first;
					
				std::map<int /*turnoIES*/, HorarioAula* >::iterator
					itTurnoIES = itCalend->second.begin();
				for( ; itTurnoIES != itCalend->second.end(); itTurnoIES++ )
				{
					int turnoIES = itTurnoIES->first;
					HorarioAula* hi = itTurnoIES->second;
							
					listaHorarios.push_back( hi );
				}
			}
		}
		  
		if ( listaHorarios.size() == 0 )
		{
			std::cout<<"\nErro! Nao ha horarios suficientes para alocacao da aula. Verificar se essa solucao tatica esta correta."					
				<< "\nAula: disc " << discAula->getId() << " turma " 
				<< aula->getTurma() << " cp " << aula->getSala()->getIdCampus();
		}

		for ( std::list< HorarioAula * >::iterator itHor = listaHorarios.begin();
			itHor != listaHorarios.end(); itHor++ )
		{
			  HorarioAula* horarioAula = ( *itHor );
			  HorarioDia * horarioDia = problemData->getHorarioDiaCorrespondente( horarioAula, dia );
			  

			 VariableOp v;
			 v.reset();
			 v.setType( VariableOp::V_Z_DISCIPLINA_HOR );

			 Campus *campus = problemData->retornaCampus( aula->getSala()->getIdUnidade() );

			 v.setHorarioAula( horarioDia->getHorarioAula() );
			 v.setDisciplina( aula->getDisciplina() );
			 v.setTurma( aula->getTurma() );
			 v.setCampus( campus );

			 if ( vHashOp.find( v ) == vHashOp.end() )
			 {
				vHashOp[ v ] = lp->getNumCols();

				OPT_COL col( OPT_COL::VAR_BINARY, coeff, 0.0, 1.0,
				   ( char * ) v.toString().c_str() );

				lp->newCol( col );
				num_vars++;
			 }
		 }
   }

   return num_vars;
}


// z
int Operacional::criaVariavelDisciplinaHorario()
{
   int num_vars = 0;
   double coeff = 0.0;

   ITERA_GGROUP_LESSPTR( itAula, problemData->aulas, Aula )
   {
      Disciplina * discAula = itAula->getDisciplina();

      // Retorna lista de horarios possiveis para a aula e a sala
      std::list< HorarioDia * > listaHorarios;

      retornaHorariosPossiveis( NULL, *itAula, listaHorarios );

      std::list< HorarioDia * >::iterator
         itHor = listaHorarios.begin();

      for (; itHor != listaHorarios.end(); itHor++ )
      {
         HorarioDia * horarioDia = ( *itHor );

         VariableOp v;
         v.reset();
         v.setType( VariableOp::V_Z_DISCIPLINA_HOR );

         Aula * aula = ( *itAula );

		 Campus *campus = problemData->retornaCampus( aula->getSala()->getIdUnidade() );

         v.setHorarioAula( horarioDia->getHorarioAula() );
         v.setDisciplina( aula->getDisciplina() );
         v.setTurma( aula->getTurma() );
		 v.setCampus( campus );

         if ( vHashOp.find( v ) == vHashOp.end() )
         {
            vHashOp[ v ] = lp->getNumCols();

            OPT_COL col( OPT_COL::VAR_BINARY, coeff, 0.0, 1.0,
               ( char * ) v.toString().c_str() );

            lp->newCol( col );
            num_vars++;
         }
      }
   }

   return num_vars;
}

// w
int Operacional::criaVariavelProfessorCurso()
{
   int num_vars = 0;

   GGroup< Professor *, LessPtr< Professor > > professores
      = problemData->getProfessores();

   ITERA_GGROUP_LESSPTR( itProfessor, professores, Professor )
   {
         ITERA_GGROUP_LESSPTR( it_aula, problemData->aulas, Aula )
         {
            Aula * aula = *it_aula;
            Disciplina * discAula = aula->getDisciplina();

            std::pair< int, int > professor_disciplina (
               itProfessor->getId(), discAula->getId() );

            // Se o professor e a disciplina da aula em questão se relacionarem:
            if ( problemData->prof_Disc_Dias.find( professor_disciplina )
                  == problemData->prof_Disc_Dias.end() )
            {
               continue;
            }

            // Como o professor em questão está alocado nessa aula, então dizemos que
            // ele leciona disciplinas de cada curso que essa aula está relacionada.
            // Para encontrar os cursos que a aula atende, basta verificar suas ofertas
            ITERA_GGROUP_LESSPTR( itOferta, aula->ofertas, Oferta )
            {
               Oferta * oferta = ( *itOferta );
               Curso * curso = oferta->curso;

               // Cria a variavel 'professor/curso' w_{p,c}
               VariableOp v;
               v.reset();
               v.setType( VariableOp::V_PROF_CURSO );

               v.setProfessor( *itProfessor ); 
               v.setCurso( curso );

               double coeff = 0.0;

               if ( vHashOp.find( v ) == vHashOp.end() )
               {
                  vHashOp[ v ] = lp->getNumCols();

                  OPT_COL col( OPT_COL::VAR_BINARY, coeff, 0.0, 1.0,
                     ( char * )v.toString().c_str() );

                  lp->newCol( col );
                  num_vars++;
               }
            }
         }      
   }
      
   ITERA_GGROUP_LESSPTR( itPV, problemData->profsVirtuais, Professor )
   {
       ITERA_GGROUP_LESSPTR( it_aula, problemData->aulas, Aula )
       {
            ITERA_GGROUP_LESSPTR( itOferta, it_aula->ofertas, Oferta )
            {
                Oferta * oferta = ( *itOferta );
                Curso * curso = oferta->curso;

				// Cria a variavel 'professor virtual/curso' w_{pv,c}
				VariableOp v;
				v.reset();
				v.setType( VariableOp::V_PROF_CURSO );

				v.setProfessor( *itPV ); 
				v.setCurso( curso );

				double coeff = 0.0;

				if ( vHashOp.find( v ) == vHashOp.end() )
				{
					v.setValue( 1.0 );

					vHashOp[ v ] = lp->getNumCols();

					OPT_COL col( OPT_COL::VAR_BINARY, coeff, 0.0, 1.0,
						( char * )v.toString().c_str() );

					lp->newCol( col );
					num_vars++;
				}
			}
	   }
   }
   
   return num_vars;
}


int Operacional::criaVariavelDiasProfessoresMinistramAulas()
{
   int num_vars = 0;

   if ( problemData->parametros->carga_horaria_semanal_prof == ParametrosPlanejamento::CHS::INDIFERENTE &&
	   !problemData->parametros->considerarMaxDiasPorProf &&
	   !problemData->parametros->minCredsDiariosPorProf )
   {
	   return num_vars;
   }
   
   double coeff = 0.0;
   if ( problemData->parametros->carga_horaria_semanal_prof == ParametrosPlanejamento::CHS::EQUILIBRAR )
   {
	   coeff = -10.0;
   }
   else if ( problemData->parametros->carga_horaria_semanal_prof == ParametrosPlanejamento::CHS::MINIMIZAR_DIAS )
   {
	   coeff = 10.0;
   }
   else
   {
	   coeff = 0.0; // só para considerarMaxDiasPorProf ou minCredsDiariosPorProf
   }

   GGroup< Professor *, LessPtr< Professor > > professores
      = problemData->getProfessores();

   // Dias em que cada professor pode dar aula
   std::map< Professor *, GGroup< int > > mapProfessorDias;

   ITERA_GGROUP_LESSPTR( it_prof, professores, Professor )
   {
      Professor * professor = ( *it_prof );

      ITERA_GGROUP_LESSPTR( it_horario_dia, professor->horariosDia, HorarioDia )
      {
         mapProfessorDias[ professor ].add( it_horario_dia->getDia() );
      }
   }

   // Criando as variáveis apenas para os dias possíveis de cada professor
   ITERA_GGROUP_LESSPTR( it_prof, professores, Professor )
   {
      Professor * professor = ( *it_prof );
      GGroup< int > dias_professor = mapProfessorDias[ professor ];

      ITERA_GGROUP_N_PT( it_dia, dias_professor, int )
      {
         VariableOp v;
         v.reset();

         v.setType( VariableOp::V_DIAS_PROF_MINISTRA_AULAS );
         v.setProfessor( *it_prof );
         v.setDia( *it_dia );

         if ( vHashOp.find( v ) == vHashOp.end() )
         {
            vHashOp[ v ] = lp->getNumCols();

            OPT_COL col( OPT_COL::VAR_BINARY, coeff, 0.0, 1.0,
               ( char * )v.toString().c_str() );

            lp->newCol( col );
            num_vars++;
         }
      }
   }

   return num_vars;
}

// dpc_{p,c,d}:			 Só para disciplina d teorica
int Operacional::criaVariavelDiscProfCurso()
{
   int num_vars = 0;
   double coeff = 0.0;

   GGroup< Professor *, LessPtr< Professor > > professores
      = problemData->getProfessores();

   ITERA_GGROUP_LESSPTR( it_prof, professores, Professor )
   {
      Professor * professor = ( *it_prof );

      GGroup< Disciplina *, LessPtr< Disciplina > > disciplinasProf
         = problemData->mapProfessorDisciplinasAssociadas[ professor ];

	   ITERA_GGROUP_LESSPTR( it_aula, problemData->aulas, Aula )
	   {
		     Aula * aula = ( *it_aula );
		     Disciplina * discAula = aula->getDisciplina();

			 if ( discAula->getId() < 0 )
				continue;

			 if ( disciplinasProf.find( discAula ) == disciplinasProf.end() )
				 continue;

			ITERA_GGROUP_LESSPTR( it_oft, aula->ofertas, Oferta )
			{		 
				 Curso *curso = it_oft->curso;
				 
				VariableOp v;
				v.reset();
				v.setType( VariableOp::V_DISC_PROF_CURSO );

				v.setProfessor( professor );
				v.setDisciplina( discAula );
				v.setCurso( curso );

				if ( vHashOp.find( v ) == vHashOp.end() )
				{
				   v.setValue( coeff );

				   vHashOp[ v ] = lp->getNumCols();

				   OPT_COL col( OPT_COL::VAR_BINARY, coeff, 0.0, 1.0,
					  ( char * )v.toString().c_str() );

				   lp->newCol( col );
				   num_vars++;
				}
			}
	   }
   }

   // Não está sendo usada para virtuais
   /* 
   // Professores virtuais
   ITERA_GGROUP_LESSPTR( itPV, problemData->profsVirtuais, Professor )
   {
	   Professor *pv = *itPV;   
	   
	   ITERA_GGROUP_LESSPTR( it_aula, problemData->aulas, Aula )
	   {
		     Aula * aula = ( *it_aula );
		     Disciplina * discAula = aula->getDisciplina();

			 if ( discAula->getId() < 0 )
				continue;

			ITERA_GGROUP_LESSPTR( it_oft, aula->ofertas, Oferta )
			{		 
				 Curso *curso = it_oft->curso;

				 VariableOp v;
				 v.reset();
				 v.setType( VariableOp::V_DISC_PROF_CURSO );
				 v.setCurso( curso );
				 v.setProfessor( pv );
				 v.setDisciplina( aula->getDisciplina() );
			 
				 double coeff = 0.0;

				 if ( vHashOp.find( v ) == vHashOp.end() )
				 {
					v.setValue( coeff );

					vHashOp[ v ] = lp->getNumCols();

					OPT_COL col( OPT_COL::VAR_BINARY, coeff, 0.0, 1.0,
						( char * )v.toString().c_str() );

					lp->newCol( col );
					num_vars++;
				 }
			}
	   }
   }
   */

   return num_vars;
}


// dpo_{p,oft,d}:			 Só para disciplina d teorica
int Operacional::criaVariavelDiscProfOferta()
{
   int num_vars = 0;
   double coeff = 0.0;

   GGroup< Professor *, LessPtr< Professor > > professores
      = problemData->getProfessores();

   ITERA_GGROUP_LESSPTR( it_prof, professores, Professor )
   {
      Professor * professor = ( *it_prof );

      GGroup< Disciplina *, LessPtr< Disciplina > > disciplinasProf
         = problemData->mapProfessorDisciplinasAssociadas[ professor ];

	   ITERA_GGROUP_LESSPTR( it_aula, problemData->aulas, Aula )
	   {
		     Aula * aula = ( *it_aula );
		     Disciplina * discAula = aula->getDisciplina();

			 if ( discAula->getId() < 0 )
				continue;

			 if ( disciplinasProf.find( discAula ) == disciplinasProf.end() )
				 continue;

			ITERA_GGROUP_LESSPTR( it_oft, aula->ofertas, Oferta )
			{		 
				 Oferta *oferta = *it_oft;
				 
				 if ( oferta->curso->getMaisDeUma() )
					 continue;

				VariableOp v;
				v.reset();
				v.setType( VariableOp::V_DISC_PROF_OFERTA );

				v.setProfessor( professor );
				v.setDisciplina( discAula );
				v.setOferta( oferta );

				if ( vHashOp.find( v ) == vHashOp.end() )
				{
				   v.setValue( coeff );

				   vHashOp[ v ] = lp->getNumCols();

				   OPT_COL col( OPT_COL::VAR_BINARY, coeff, 0.0, 1.0,
					  ( char * )v.toString().c_str() );

				   lp->newCol( col );
				   num_vars++;
				}
			}
	   }
   }

   return num_vars;
}


int Operacional::criaVariavelFolgaMaxDiscProfCurso()
{
   int num_vars = 0;
   double coeff = 10.0;

   GGroup< Professor *, LessPtr< Professor > > professores
      = problemData->getProfessores();

   ITERA_GGROUP_LESSPTR( it_prof, professores, Professor )
   {
      Professor * professor = ( *it_prof );

      GGroup< Disciplina *, LessPtr< Disciplina > > disciplinasProf
         = problemData->mapProfessorDisciplinasAssociadas[ professor ];

      ITERA_GGROUP_LESSPTR( it_disc, disciplinasProf, Disciplina )
      {
         Disciplina * disciplina = ( *it_disc );

         ITERA_GGROUP_LESSPTR( it_curso, problemData->cursos, Curso )
         {
            Curso * curso = ( *it_curso );

			if ( !curso->possuiDisciplina( disciplina ) )
				continue;

            VariableOp v;
            v.reset();
            v.setType( VariableOp::V_F_MAX_DISC_PROF_CURSO );
            v.setProfessor( professor );
            v.setCurso( curso );

            if ( vHashOp.find( v ) == vHashOp.end() )
            {
               v.setValue( coeff );

               vHashOp[ v ] = lp->getNumCols();

			   OPT_COL col( OPT_COL::VAR_INTEGRAL, coeff, 0.0, OPT_INF,
                  ( char * )v.toString().c_str() );

               lp->newCol( col );
               num_vars++;
			   break;
            }
         }
      }
   }

   // Professores virtuais
   ITERA_GGROUP_LESSPTR( itPV, problemData->profsVirtuais, Professor )
   {
	   Professor *pv = *itPV;   
	   
	   ITERA_GGROUP_LESSPTR( it_aula, problemData->aulas, Aula )
	   {
		     Aula * aula = ( *it_aula );
		     Disciplina * discAula = aula->getDisciplina();

			ITERA_GGROUP_LESSPTR( it_oft, aula->ofertas, Oferta )
			{		 
				 Curso *curso = it_oft->curso;

				 VariableOp v;
				 v.reset();
				 v.setType( VariableOp::V_F_MAX_DISC_PROF_CURSO );
				 v.setCurso( curso );
				 v.setProfessor( pv );
			 				 
				 if ( vHashOp.find( v ) == vHashOp.end() )
				 {
					v.setValue( coeff );

					vHashOp[ v ] = lp->getNumCols();

					OPT_COL col( OPT_COL::VAR_INTEGRAL, coeff, 0.0, OPT_INF,
						( char * )v.toString().c_str() );

					lp->newCol( col );
					num_vars++;
				 }
			}
	   }
   }

   return num_vars;
}

int Operacional::criaVariavelFolgaCargaHorariaMinimaProfessor()
{
   int num_vars = 0;
   double coeff = 10.0;
   int ub = 168; // Total de horas da semana

   GGroup< Professor *, LessPtr< Professor > > professores
      = problemData->getProfessores();

   ITERA_GGROUP_LESSPTR( it_prof, professores, Professor )
   {
      VariableOp v;
      v.reset();

      v.setType( VariableOp::V_F_CARGA_HOR_MIN_PROF );
      v.setProfessor( *it_prof );

      if ( vHashOp.find( v ) == vHashOp.end() )
      {
         v.setValue( 1.0 );
         vHashOp[ v ] = lp->getNumCols();

         OPT_COL col( OPT_COL::VAR_INTEGRAL, coeff, 0.0, ub,
            ( char * )v.toString().c_str() );

         lp->newCol( col );
         num_vars++;
      }
   }

   return num_vars;
}

int Operacional::criaVariavelFolgaCargaHorariaMinimaProfessorSemana()
{
   int num_vars = 0;
   double coeff = 10.0;
   int ub = 168; // Total de horas da semana

   GGroup< Professor *, LessPtr< Professor > > professores
      = problemData->getProfessores();

   ITERA_GGROUP_LESSPTR( it_prof, professores, Professor )
   {
      VariableOp v;
      v.reset();

      v.setType( VariableOp::V_F_CARGA_HOR_MIN_PROF_SEMANA );
      v.setProfessor( *it_prof );

      if ( vHashOp.find( v ) == vHashOp.end() )
      {
         v.setValue( 1.0 );
         vHashOp[ v ] = lp->getNumCols();

         OPT_COL col( OPT_COL::VAR_INTEGRAL, coeff, 0.0, ub,
            ( char * )v.toString().c_str() );

         lp->newCol( col );
         num_vars++;
      }
   }

   return num_vars;
}

int Operacional::criaVariavelFolgaCargaHorariaMaximaProfessorSemana()
{
   int num_vars = 0;
   double coeff = 1000.0;
   int ub = 168; // Total de horas da semana

   GGroup< Professor *, LessPtr< Professor > > professores
      = problemData->getProfessores();

   ITERA_GGROUP_LESSPTR( it_prof, professores, Professor )
   {
      VariableOp v;
      v.reset();

      v.setType( VariableOp::V_F_CARGA_HOR_MAX_PROF_SEMANA );
      v.setProfessor( *it_prof );

      if ( vHashOp.find( v ) == vHashOp.end() )
      {
         v.setValue( 1.0 );
         vHashOp[ v ] = lp->getNumCols();

         OPT_COL col( OPT_COL::VAR_INTEGRAL, coeff, 0.0, ub,
            ( char * )v.toString().c_str() );

         lp->newCol( col );
         num_vars++;
      }
   }

   return num_vars;
}

int Operacional::criaVariavelFolgaUltimaPrimeiraAulas()
{
   int num_vars = 0;

   if ( ! problemData->parametros->evitar_prof_ultimo_primeiro_hr )
   {
		return num_vars;
   }

   double coeff = 100.0;
   double alfa = 100.0;

   GGroup< Professor *, LessPtr< Professor > > professores
      = problemData->getProfessores();

   ITERA_GGROUP_LESSPTR( it_prof, professores, Professor )
   {
      Professor * professor = ( *it_prof );

      ITERA_GGROUP_LESSPTR( it_h1,
         professor->horariosDia, HorarioDia )
      {
            HorarioDia * h1 = ( *it_h1 );
         
			VariableOp v;
			v.reset();
			v.setType( VariableOp::V_F_ULTIMA_PRIMEIRA_AULA_PROF );

			v.setProfessor( professor );
			v.setDia( h1->getDia() );

			if ( vHashOp.find( v ) == vHashOp.end() )
			{
				vHashOp[ v ] = lp->getNumCols();

				OPT_COL col( OPT_COL::VAR_BINARY, coeff, 0.0, 1.0,
					( char * ) v.toString().c_str() );

				lp->newCol( col );
				num_vars++;
			}         
       }
   }

   return num_vars;
}


int Operacional::criaVariavelFolgaDemanda( void )
{
   int num_vars = 0;

   ITERA_GGROUP_LESSPTR( it_aula, problemData->aulas, Aula )
   {
		Aula * aula = ( *it_aula );

		VariableOp v;
		v.reset();
		v.setType( VariableOp::V_FOLGA_DEMANDA );
		v.setAula( aula );
		
		if ( vHashOp.find( v ) == vHashOp.end() )
		{
			double coeff = 0.0;

			int campusId = problemData->retornaCampus( v.getAula()->getSala()->getIdUnidade() )->getId();
			int turma = v.getAula()->getTurma();
			Disciplina* disc = v.getAula()->getDisciplina();

			std::map< Trio< int /*campusId*/, int /*turma*/, Disciplina* >, GGroup< AlunoDemanda*, LessPtr< AlunoDemanda > > >::iterator itMapAtend;

			Trio< int, int, Disciplina* > trio;
			trio.set( campusId, turma, disc );

			itMapAtend = problemData->mapCampusTurmaDisc_AlunosDemanda.find( trio );
			if ( itMapAtend != problemData->mapCampusTurmaDisc_AlunosDemanda.end() )
			{
				GGroup< AlunoDemanda*, LessPtr< AlunoDemanda > > *alunosDemanda = &(*itMapAtend).second;
				coeff = alunosDemanda->size() * 100000.0;
			}

			if ( coeff == 0 )
				std::cout<<"\nErro! Turma nao encontrada ou vazia: v = " << v.toString();

			vHashOp[ v ] = lp->getNumCols();

			OPT_COL col( OPT_COL::VAR_BINARY, coeff, 0.0, 1.0,
				( char * ) v.toString().c_str() );

			lp->newCol( col );
			num_vars++;
		}
   }

   return num_vars;

}

int Operacional::criaVariavelFolgaDisciplinaTurmaHorario( void )
{
	int num_vars = 0;
	
   ITERA_GGROUP_LESSPTR( itAula, problemData->aulas, Aula )
   {      
	    Disciplina * disciplina = (*itAula)->getDisciplina();
		int turma = (*itAula)->getTurma();
		Campus* campus = problemData->retornaCampus( (*itAula)->getSala()->getIdUnidade() );

		VariableOp v;
		v.reset();
		v.setType( VariableOp::V_FOLGA_DISCIPLINA_HOR );
		v.setDisciplina( disciplina );
		v.setTurma( turma );
		v.setCampus( campus );

		if ( vHashOp.find( v ) == vHashOp.end() )
		{
			double coeff = 1000.0;

			vHashOp[ v ] = lp->getNumCols();

			int nHor = disciplina->horarios.size();

			if ( nHor <= 0 )
			{
				std::cout << "\nAtencao na funcao Operacional::criaVariavelFolgaDisciplinaTurmaHorario:"
				<< " nao ha horarios disponiveis para a disciplina "<<disciplina->getId();
			}

			OPT_COL col( OPT_COL::VAR_INTEGRAL, coeff, 0.0, nHor,
						( char* ) v.toString().c_str() );

			lp->newCol( col );
			num_vars++;
		}			
	}

	return num_vars;
}


int Operacional::criaVariavelProfessorDiaHorarioIF()
{
   int num_vars = 0;

   if ( !problemData->parametros->minimizar_horarios_vazios_professor )
   {
	   return num_vars;
   }

   double coeff = 0.0;
   double peso = 10;

   GGroup< Professor *, LessPtr< Professor > > professores
      = problemData->getProfessores();

   ITERA_GGROUP_LESSPTR( itProfessor, professores, Professor )
   {
	   GGroup<int> dias;
	   ITERA_GGROUP_LESSPTR( itHor, itProfessor->horariosDia, HorarioDia )
	   {
		    dias.add( itHor->getDia() );
	   }

	   ITERA_GGROUP_N_PT( itDia, dias, int )
	   {
		    int dia = *itDia;

			HorarioAula *h = itProfessor->getPrimeiroHorarioDisponivelDia( dia );
			double lb = h->getInicio().getDateMinutes();
			h = itProfessor->getUltimoHorarioDisponivelDia( dia );						
			double ub = h->getInicio().getDateMinutes();
			
            VariableOp v;
            v.reset();
            v.setProfessor( *itProfessor );
            v.setDia( dia );
			
			v.setType( VariableOp::V_HI_PROFESSORES );			
						
			if ( problemData->parametros->funcao_objetivo == 0 ) // max
			{
				coeff = 1.0;
			}
			else if ( problemData->parametros->funcao_objetivo == 1 ) // min
			{
				coeff = -1.0;
			}              

            if ( vHashOp.find( v ) == vHashOp.end() )
            {
               vHashOp[ v ] = lp->getNumCols();
			   
               OPT_COL col( OPT_COL::VAR_INTEGRAL, peso*coeff, lb, ub,
                  ( char * ) v.toString().c_str() );

               lp->newCol( col );
               num_vars++;
            }

            v.setType( VariableOp::V_HF_PROFESSORES );
			
			if ( problemData->parametros->funcao_objetivo == 0 ) // max
			{
				coeff = -1.0;
			}
			else if ( problemData->parametros->funcao_objetivo == 1 ) // min
			{
				coeff = 1.0;
			}     

            if ( vHashOp.find( v ) == vHashOp.end() )
            {
               vHashOp[ v ] = lp->getNumCols();

               OPT_COL col( OPT_COL::VAR_INTEGRAL, peso*coeff, lb, ub,
                  ( char * ) v.toString().c_str() );

               lp->newCol( col );
               num_vars++;
            }
	   }
   }

   return num_vars;
}


int Operacional::criaVariavelNroProfsReaisAlocadosCurso()
{
   int num_vars = 0;
      
  // if ( ! problemData->parametros->min_mestres &&
	 //   ! problemData->parametros->min_doutores )
  // {
		//return num_vars;
  // }

   ITERA_GGROUP_LESSPTR( itCurso, problemData->cursos, Curso )
   {
        Curso *curso = ( *itCurso );
		
		// Se o curso tem aula associada
		bool DEMANDA = false;
		ITERA_GGROUP_LESSPTR( it_aula, problemData->aulas, Aula )
		{
			Aula * aula = ( *it_aula );			
			ITERA_GGROUP_LESSPTR( it_oferta, aula->ofertas, Oferta )
			{
				if ( it_oferta->getCursoId() == curso->getId() )
				{
					DEMANDA = true;
					break;
				}
			}
		}
		if ( DEMANDA )
		{
			VariableOp v;
			v.reset();
			v.setType( VariableOp::V_NRO_PROFS_CURSO );		
			v.setCurso( curso );

			if ( vHashOp.find( v ) == vHashOp.end() )
			{
				vHashOp[ v ] = lp->getNumCols();
			   
				int ub = 100;

				OPT_COL col( OPT_COL::VAR_INTEGRAL, 0.0, 0.0, ub,
					( char * ) v.toString().c_str() );

				lp->newCol( col );
				num_vars++;
			}
		} 
   }
   return num_vars;
}

int Operacional::criaVariavelNroProfsVirtuaisAlocadosCurso()
{
   int num_vars = 0;
      
   if ( this->getRodada() == Operacional::OP_VIRTUAL_PERFIL )
   {
	   ITERA_GGROUP_LESSPTR( itCurso, problemData->cursos, Curso )
	   {
			Curso *curso = ( *itCurso );
		
			// Se o curso tem aula associada
			bool DEMANDA = false;
			ITERA_GGROUP_LESSPTR( it_aula, problemData->aulas, Aula )
			{
				Aula * aula = ( *it_aula );			
				ITERA_GGROUP_LESSPTR( it_oferta, aula->ofertas, Oferta )
				{
					if ( it_oferta->getCursoId() == curso->getId() )
					{
						DEMANDA = true;
						break;
					}
				}
			}
			if ( DEMANDA )
			{
				ITERA_GGROUP_LESSPTR( it_contr, problemData->tipos_contrato, TipoContrato )
				{
					if ( ( ! problemData->parametros->minContratoIntegral && 
						  it_contr->getContrato() == TipoContrato::Integral ) ||
						  ( ! problemData->parametros->minContratoParcial && 
						 it_contr->getContrato() == TipoContrato::Parcial ) )
					{
						continue;
					}

					VariableOp v;
					v.reset();
					v.setType( VariableOp::V_NRO_PROFS_VIRTUAIS_CURSO );		
					v.setCurso( curso );
					v.setContrato( *it_contr );

					if ( vHashOp.find( v ) == vHashOp.end() )
					{
						vHashOp[ v ] = lp->getNumCols();
			   
						int ub = 100;

						OPT_COL col( OPT_COL::VAR_INTEGRAL, 1.0, 0.0, ub,
							( char * ) v.toString().c_str() );

						lp->newCol( col );
						num_vars++;
					}
				}
			} 
	   }
   }
   else if ( this->getRodada() == Operacional::OP_VIRTUAL_INDIVIDUAL )
   {
	   for( int at = 0; at < problemData->professores_virtuais.size(); at++ )
	   {
		    Professor *prof = problemData->professores_virtuais[at];

			VariableOp v;
			v.reset();
			v.setType( VariableOp::V_NRO_PROFS_VIRTUAIS_CURSO );		
			v.setCurso( prof->getCursoAssociado() );
			v.setContrato( prof->tipo_contrato );

			if ( vHashOp.find( v ) == vHashOp.end() )
			{
				vHashOp[ v ] = lp->getNumCols();
			   
				int ub = 100;

				OPT_COL col( OPT_COL::VAR_INTEGRAL, 1.0, 0.0, ub,
					( char * ) v.toString().c_str() );

				lp->newCol( col );
				num_vars++;
			}
	   }
   }

   return num_vars;
}

int Operacional::criaVariavelNroProfsVirtuaisMestresAlocadosCurso()
{
   int num_vars = 0;
      
   if ( ! problemData->parametros->min_mestres )
   {
		return num_vars;
   }

   if ( this->getRodada() == Operacional::OP_VIRTUAL_PERFIL )
   {
	   ITERA_GGROUP_LESSPTR( itCurso, problemData->cursos, Curso )
	   {
			Curso *curso = ( *itCurso );
		
			// Se o curso tem aula associada
			bool DEMANDA = false;
			ITERA_GGROUP_LESSPTR( it_aula, problemData->aulas, Aula )
			{
				Aula * aula = ( *it_aula );			
				ITERA_GGROUP_LESSPTR( it_oferta, aula->ofertas, Oferta )
				{
					if ( it_oferta->getCursoId() == curso->getId() )
					{
						DEMANDA = true;
						break;
					}
				}
			}
			if ( DEMANDA )
			{
				ITERA_GGROUP_LESSPTR( it_contr, problemData->tipos_contrato, TipoContrato )
				{
					if ( ( ! problemData->parametros->minContratoIntegral && 
						  it_contr->getContrato() == TipoContrato::Integral ) ||
						  ( ! problemData->parametros->minContratoParcial && 
						 it_contr->getContrato() == TipoContrato::Parcial ) )
					{
						continue;
					}

					VariableOp v;
					v.reset();
					v.setType( VariableOp::V_NRO_PROFS_VIRTUAIS_MEST_CURSO );		
					v.setCurso( curso );
					v.setContrato( *it_contr );

					if ( vHashOp.find( v ) == vHashOp.end() )
					{
						vHashOp[ v ] = lp->getNumCols();
			   
						int ub = 100;
						double coef = 5000;

						OPT_COL col( OPT_COL::VAR_INTEGRAL, coef, 0.0, ub,
							( char * ) v.toString().c_str() );

						lp->newCol( col );
						num_vars++;
					}
				}
			} 
	   }
   }
   else if ( this->getRodada() == Operacional::OP_VIRTUAL_INDIVIDUAL )
   {
	   for( int at = 0; at < problemData->professores_virtuais.size(); at++ )
	   {
		    Professor *prof = problemData->professores_virtuais[at];

			if ( prof->getTitulacao() != TipoTitulacao::Mestre )
			   continue;

			VariableOp v;
			v.reset();
			v.setType( VariableOp::V_NRO_PROFS_VIRTUAIS_MEST_CURSO );		
			v.setCurso( prof->getCursoAssociado() );
			v.setContrato( prof->tipo_contrato );

			if ( vHashOp.find( v ) == vHashOp.end() )
			{
				vHashOp[ v ] = lp->getNumCols();
			   
				int ub = 100;
				double coef = 5000;

				OPT_COL col( OPT_COL::VAR_INTEGRAL, coef, 0.0, ub,
					( char * ) v.toString().c_str() );

				lp->newCol( col );
				num_vars++;
			}
	   }
   }
   
   return num_vars;
}

int Operacional::criaVariavelNroProfsVirtuaisDoutoresAlocadosCurso()
{
   int num_vars = 0;
      
   if ( ! problemData->parametros->min_doutores )
   {
		return num_vars;
   }

   if ( this->getRodada() == Operacional::OP_VIRTUAL_PERFIL )
   {
	   ITERA_GGROUP_LESSPTR( itCurso, problemData->cursos, Curso )
	   {
			Curso *curso = ( *itCurso );
		
			// Se o curso tem aula associada
			bool DEMANDA = false;
			ITERA_GGROUP_LESSPTR( it_aula, problemData->aulas, Aula )
			{
				Aula * aula = ( *it_aula );			
				ITERA_GGROUP_LESSPTR( it_oferta, aula->ofertas, Oferta )
				{
					if ( it_oferta->getCursoId() == curso->getId() )
					{
						DEMANDA = true;
						break;
					}
				}
			}
			if ( DEMANDA )
			{
				ITERA_GGROUP_LESSPTR( it_contr, problemData->tipos_contrato, TipoContrato )
				{
					if ( ( ! problemData->parametros->minContratoIntegral && 
						  it_contr->getContrato() == TipoContrato::Integral ) ||
						  ( ! problemData->parametros->minContratoParcial && 
						 it_contr->getContrato() == TipoContrato::Parcial ) )
					{
						continue;
					}

					VariableOp v;
					v.reset();
					v.setType( VariableOp::V_NRO_PROFS_VIRTUAIS_DOUT_CURSO );		
					v.setCurso( curso );
					v.setContrato( *it_contr );

					if ( vHashOp.find( v ) == vHashOp.end() )
					{
						vHashOp[ v ] = lp->getNumCols();
			   
						int ub = 100;
						double coef = 5000;

						OPT_COL col( OPT_COL::VAR_INTEGRAL, coef, 0.0, ub,
							( char * ) v.toString().c_str() );

						lp->newCol( col );
						num_vars++;
					}
				}
			} 
	   }
   }
   else if ( this->getRodada() == Operacional::OP_VIRTUAL_INDIVIDUAL )
   {
	   for( int at = 0; at < problemData->professores_virtuais.size(); at++ )
	   {
		    Professor *prof = problemData->professores_virtuais[at];

			if ( prof->getTitulacao() != TipoTitulacao::Doutor )
			   continue;

			VariableOp v;
			v.reset();
			v.setType( VariableOp::V_NRO_PROFS_VIRTUAIS_DOUT_CURSO );		
			v.setCurso( prof->getCursoAssociado() );
			v.setContrato( prof->tipo_contrato );

			if ( vHashOp.find( v ) == vHashOp.end() )
			{
				vHashOp[ v ] = lp->getNumCols();
			   
				int ub = 100;
				double coef = 5000;

				OPT_COL col( OPT_COL::VAR_INTEGRAL, coef, 0.0, ub,
					( char * ) v.toString().c_str() );

				lp->newCol( col );
				num_vars++;
			}
	   }
   }
  
   
   return num_vars;
}

int Operacional::criaVariavelNroProfsVirtuaisGeraisAlocadosCurso()
{
   int num_vars = 0;
      
   if ( ! problemData->parametros->min_doutores &&
	    ! problemData->parametros->min_mestres )
   {
		return num_vars;
   }

   if ( this->getRodada() == Operacional::OP_VIRTUAL_PERFIL )
   {
	   ITERA_GGROUP_LESSPTR( itCurso, problemData->cursos, Curso )
	   {
			Curso *curso = ( *itCurso );
		
			// Se o curso tem aula associada
			bool DEMANDA = false;
			ITERA_GGROUP_LESSPTR( it_aula, problemData->aulas, Aula )
			{
				Aula * aula = ( *it_aula );			
				ITERA_GGROUP_LESSPTR( it_oferta, aula->ofertas, Oferta )
				{
					if ( it_oferta->getCursoId() == curso->getId() )
					{
						DEMANDA = true;
						break;
					}
				}
			}
			if ( DEMANDA )
			{
				ITERA_GGROUP_LESSPTR( it_contr, problemData->tipos_contrato, TipoContrato )
				{
					if ( ( ! problemData->parametros->minContratoIntegral && 
						  it_contr->getContrato() == TipoContrato::Integral ) ||
						  ( ! problemData->parametros->minContratoParcial && 
						 it_contr->getContrato() == TipoContrato::Parcial ) )
					{
						continue;
					}

					VariableOp v;
					v.reset();
					v.setType( VariableOp::V_NRO_PROFS_VIRTUAIS_GERAIS_CURSO );		
					v.setCurso( curso );
					v.setContrato( *it_contr );

					if ( vHashOp.find( v ) == vHashOp.end() )
					{
						vHashOp[ v ] = lp->getNumCols();
			   
						int ub = 100;
						double coef = 5000;

						OPT_COL col( OPT_COL::VAR_INTEGRAL, coef, 0.0, ub,
							( char * ) v.toString().c_str() );

						lp->newCol( col );
						num_vars++;
					}
				}
			} 
	   }
   }
   else if ( this->getRodada() == Operacional::OP_VIRTUAL_INDIVIDUAL )
   {
	   for( int at = 0; at < problemData->professores_virtuais.size(); at++ )
	   {
		    Professor *prof = problemData->professores_virtuais[at];

		    if ( prof->getTitulacao() == TipoTitulacao::Mestre ||
			     prof->getTitulacao() == TipoTitulacao::Doutor )
			   continue;

			VariableOp v;
			v.reset();
			v.setType( VariableOp::V_NRO_PROFS_VIRTUAIS_GERAIS_CURSO );		
			v.setCurso( prof->getCursoAssociado() );
			v.setContrato( prof->tipo_contrato );

			if ( vHashOp.find( v ) == vHashOp.end() )
			{
				vHashOp[ v ] = lp->getNumCols();
			   
				int ub = 100;
				double coef = 5000;

				OPT_COL col( OPT_COL::VAR_INTEGRAL, coef, 0.0, ub,
					( char * ) v.toString().c_str() );

				lp->newCol( col );
				num_vars++;
			}
	   }
   }
   
   
   return num_vars;
}

int Operacional::criaVariavelProfessorVirtual()
{
   int num_vars = 0;
   double coeff = 0.0;

   GGroup< Professor *, LessPtr< Professor > > professores
      = problemData->getProfessores();

   ITERA_GGROUP_LESSPTR( itProfessor, professores, Professor )
   {
	   if ( ! itProfessor->eVirtual() )
		   continue;

        VariableOp v;
        v.reset();
        v.setType( VariableOp::V_PROF_VIRTUAL );
        v.setProfessor( ( *itProfessor ) );
            
        coeff = 500000;

        if ( vHashOp.find( v ) == vHashOp.end() )
        {
            vHashOp[ v ] = lp->getNumCols();

            OPT_COL col( OPT_COL::VAR_BINARY, coeff, 0.0, 1.0,
                ( char * ) v.toString().c_str() );

            lp->newCol( col );
            num_vars++;
        }
   }

   return num_vars;
}

// fmd_{a}
int Operacional::criaVariavelFolgaMinimoDemandaPorAluno()
{
    int num_vars = 0;
 
   if ( ! problemData->parametros->considerarMinPercAtendAluno )
   {
		return num_vars;
   }
   
	double coef = 100000;	// TODO

	ITERA_GGROUP_LESSPTR( itAluno, problemData->alunos, Aluno )
	{
		Aluno *aluno = *itAluno;

		VariableOp v;
		v.reset();
		v.setType( VariableOp::V_FOLGA_ALUNO_MIN_ATEND );
		v.setAluno( aluno );
		
		if ( vHashOp.find( v ) == vHashOp.end() )
		{
			vHashOp[v] = lp->getNumCols();

			double ub = (double) aluno->getNroCredsOrigRequeridosP1();

			OPT_COL col( OPT_COL::VAR_INTEGRAL, coef, 0.0, ub, ( char * )v.toString().c_str() );
			lp->newCol(col);
				
			num_vars++;
		}			
	}

	return num_vars;

}

// hip_{p,t,f} e hfp_{p,t,f}
int Operacional::criaVariaveisHiHfProfFaseDoDiaAPartirDeX(void)
{
	// Cria vars inteiras que indicam o primeiro e o último horário
	// da fase do dia (M/T/N) usados pelo professor
	
	int numVars = 0;
		
	if ( problemData->parametros->proibirProfGapMTN ==
		 ParametrosPlanejamento::ConstraintLevel::Off )
		return numVars;

	// min dti e max dtf para cada prof/dia => obtido a partir de variaveis x_{p,aula,h}
	for(auto itProf = vars_prof_dia_fase_dt.begin(); itProf != vars_prof_dia_fase_dt.end(); ++itProf)
	{
		if ( itProf->first->eVirtual() && !problemData->parametros->proibirProfVirtualGapMTN )
			continue;

		for(auto itDia = itProf->second.begin(); itDia != itProf->second.end(); ++itDia)
		{
			for(auto itFase = itDia->second.begin(); itFase != itDia->second.end(); ++itFase)
			{
				DateTime dti = itFase->second.first;
				DateTime dtf = itFase->second.second;
				int dtiMinutes = dti.getDateMinutes();
				int dtfMinutes = dtf.getDateMinutes();

				double coef = 0.0;
				int lbi = dtiMinutes;
				int ubf = dtfMinutes;
				int ubi = ubf;
				int lbf = lbi;
				
				VariableOp var;
				var.reset();
				var.setProfessor(itProf->first);
				var.setDia(itDia->first);
				var.setFaseDoDia(itFase->first);
				
				// -----------------------------------------------
				// varProfHi
				var.setType( VariableOp::V_HI_PROFESSORES );				
				
				int colNrHi = lp->getNumCols();

				vHashOp[ var ] = colNrHi;
				varsProfDiaFaseHiHf[itProf->first][itDia->first][itFase->first].first = colNrHi;

				OPT_COL colHi( OPT_COL::VAR_INTEGRAL, coef, lbi, ubi,
					( char * ) var.toString().c_str() );

				lp->newCol( colHi );
				numVars++;
				
				// -----------------------------------------------
				// varProfHf
				var.setType( VariableOp::V_HF_PROFESSORES );
				
				int colNrHf = lp->getNumCols();

				vHashOp[ var ] = colNrHf;
				varsProfDiaFaseHiHf[itProf->first][itDia->first][itFase->first].second = colNrHf;

				OPT_COL colHf( OPT_COL::VAR_INTEGRAL, coef, lbf, ubf,
					( char * ) var.toString().c_str() );

				lp->newCol( colHf );
				numVars++;
			}
		}
	}
	return numVars;
}

// fpgap_{p,t,f}
int Operacional::criaVariavelFolgaGapProfAPartirDeK(void)
{
	// Cria var inteira de folga que indica o número de minutos que
	// violaram a restrição de gap na fase do dia de um dia do professor
	
	int numVars = 0;
		
	if ( problemData->parametros->proibirProfGapMTN !=
		 ParametrosPlanejamento::ConstraintLevel::Weak )
		return numVars;

	// min dti e max dtf para cada prof/dia => obtido a partir de variaveis k_{p,i,d,cp,t,h}
	for(auto itProf = vars_prof_dia_fase_dt.begin(); itProf != vars_prof_dia_fase_dt.end(); ++itProf)
	{
		if ( itProf->first->eVirtual() && !problemData->parametros->proibirProfVirtualGapMTN )
			continue;

		for(auto itDia = itProf->second.begin(); itDia != itProf->second.end(); ++itDia)
		{
			for(auto itFase = itDia->second.begin(); itFase != itDia->second.end(); ++itFase)
			{
				DateTime dti = itFase->second.first;
				DateTime dtf = itFase->second.second;
				const int dtiMinutes = dti.getDateMinutes();
				const int dtfMinutes = dtf.getDateMinutes();

				const double coef = 2.0;
				const int lb = 0;
				const int ub = dtfMinutes - dtiMinutes;
			
				VariableOp var;
				var.reset();
				var.setType( VariableOp::V_FOLGA_GAP_PROF );	// fpgap_{p,t,f}
				var.setProfessor(itProf->first);
				var.setDia(itDia->first);
				var.setFaseDoDia(itFase->first);

				const int colNr = lp->getNumCols();

				vHashOp[ var ] = colNr;
				varsProfFolgaGap[itProf->first][itDia->first][itFase->first] = colNr;

				OPT_COL col( OPT_COL::VAR_INTEGRAL, coef, lb, ub,
					( char * ) var.toString().c_str() );

				lp->newCol( col );
				numVars++;	
			}
		}
	}
	return numVars;
}


/********************************************************************************************************************
**						        CRIAÇÃO DE RESTRIÇÕES DO OPERACIONAL											  **
/********************************************************************************************************************/


int Operacional::criaRestricoesOperacional()
{

	CPUTimer timer;
	double dif = 0.0;

	int restricoes = 0;

#ifdef PRINT_cria_restricoes
	int numRestAnterior = 0;
#endif

	if ( problemData->parametros->otimizarPor == "ALUNO" )
	{
		lp->updateLP();
		timer.start();
		restricoes += criaRestricaoAlunoHorario__2();	
		timer.stop();
		dif = timer.getCronoCurrSecs();

		#ifdef PRINT_cria_restricoes
		std::cout << "numRest C_ALUNO_HORARIO: "
			<< ( restricoes - numRestAnterior ) <<" " <<dif <<"seg" <<std::endl; fflush(NULL);
		numRestAnterior = restricoes;
		#endif	
	}

	timer.start();
	restricoes += criaRestricaoSalaHorario();
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest C_SALA_HORARIO: "
		<< ( restricoes - numRestAnterior ) <<" " <<dif <<"seg" <<std::endl; fflush(NULL);
	numRestAnterior = restricoes;
#endif

	lp->updateLP();
	timer.start();
	restricoes += criaRestricaoProfessorHorario();
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest C_PROFESSOR_HORARIO: "
		<< ( restricoes - numRestAnterior ) <<" " <<dif <<"seg" <<std::endl; fflush(NULL);
	numRestAnterior = restricoes;
#endif

	lp->updateLP();
	timer.start();
	restricoes += criaRestricaoAlocAula();
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest C_ALOC_AULA: "
		<< ( restricoes - numRestAnterior ) <<" " <<dif <<"seg" <<std::endl; fflush(NULL);
	numRestAnterior = restricoes;
#endif

	lp->updateLP();
	timer.start();
	restricoes += criaRestricaoAtendimentoCompleto();
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest C_ATEND_COMPLETO: "
		<< ( restricoes - numRestAnterior ) <<" " <<dif <<"seg" <<std::endl; fflush(NULL);
	numRestAnterior = restricoes;
#endif
		

	lp->updateLP();
	timer.start();
	restricoes += criaRestricaoProfessorDisciplina();
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest C_PROF_DISC: "
		<< ( restricoes - numRestAnterior ) <<" " <<dif <<"seg" <<std::endl; fflush(NULL);
	numRestAnterior = restricoes;
#endif

	lp->updateLP();
	timer.start();
	restricoes += criaRestricaoProfessorDisciplinaUnico();
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest C_PROF_DISC_UNI: "
		<< ( restricoes - numRestAnterior ) <<" " <<dif <<"seg" <<std::endl; fflush(NULL);
	numRestAnterior = restricoes;
#endif

	lp->updateLP();
	timer.start();
	restricoes += criaRestricaoProfessorDisciplinaUnicoPT();
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest C_PROF_DISC_PT_UNICO: "
		<< ( restricoes - numRestAnterior ) <<" " <<dif <<"seg" <<std::endl; fflush(NULL);
	numRestAnterior = restricoes;
#endif
	
	lp->updateLP();
	timer.start();
	restricoes += criaRestricaoDisciplinaMesmoHorario();
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest C_DISC_HORARIO: "
		<< ( restricoes - numRestAnterior ) <<" " <<dif <<"seg" <<std::endl; fflush(NULL);
	numRestAnterior = restricoes;
#endif

	lp->updateLP();
	timer.start();
	restricoes += criaRestricaoDisciplinaHorarioUnico();
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest C_DISC_HORARIO_UNICO: "
		<< ( restricoes - numRestAnterior ) <<" " <<dif <<"seg" <<std::endl; fflush(NULL);
	numRestAnterior = restricoes;
#endif


	lp->updateLP();
	timer.start();
	restricoes += criaRestricaoDeslocamentoViavel();
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest C_DESLOC_VIAVEL: "
		<< ( restricoes - numRestAnterior ) <<" " <<dif <<"seg" <<std::endl; fflush(NULL);
	numRestAnterior = restricoes;
#endif

	lp->updateLP();
	timer.start();
	restricoes += criaRestricaoDeslocamentoProfessor();
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest C_DESLOC_PROF: "
		<< ( restricoes - numRestAnterior ) <<" " <<dif <<"seg" <<std::endl; fflush(NULL);
	numRestAnterior = restricoes;
#endif
	
	/*
	lp->updateLP();
	timer.start();
	restricoes += criaRestricaoAvaliacaoCorpoDocente();
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest C_AVALIACAO_CORPO_DOCENTE: "
		<< ( restricoes - numRestAnterior ) <<" " <<dif <<"seg" <<std::endl;
	numRestAnterior = restricoes;
#endif*/

	/*lp->updateLP();
	timer.start();
	restricoes += criaRestricaoCustoCorpoDocente();
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest C_CUSTO_CORPO_DOCENTE: "
		<< ( restricoes - numRestAnterior ) <<" " <<dif <<"seg" <<std::endl;
	numRestAnterior = restricoes;
#endif*/

	lp->updateLP();
	timer.start();
	restricoes += criaRestricaoCalculaDiasProfMinistra_Min();
	restricoes += criaRestricaoCalculaDiasProfMinistra_Max();
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest C_DIAS_PROF_MINISTRA_AULA: "
		<< ( restricoes - numRestAnterior ) <<" " <<dif <<"seg" <<std::endl; fflush(NULL);
	numRestAnterior = restricoes;
#endif

	lp->updateLP();
	timer.start();
	restricoes += criaRestricaoUltimaPrimeiraAulas();
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest C_ULTIMA_PRIMEIRA_AULA_PROF: "
		<< ( restricoes - numRestAnterior ) <<" " <<dif <<"seg" <<std::endl; fflush(NULL);
	numRestAnterior = restricoes;
#endif

	lp->updateLP();
	timer.start();
	restricoes += criaRestricaoAlocacaoProfessorCurso();
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest C_ALOC_PROF_CURSO: "
		<< ( restricoes - numRestAnterior ) <<" " <<dif <<"seg" <<std::endl; fflush(NULL);
	numRestAnterior = restricoes;
#endif

	lp->updateLP();
	timer.start();
	restricoes += criaRestricaoCargaHorariaMinimaProfessor();
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest C_CARGA_HOR_MIN_PROF: "
		<< ( restricoes - numRestAnterior ) <<" " <<dif <<"seg" <<std::endl; fflush(NULL);
	numRestAnterior = restricoes;
#endif

	lp->updateLP();
	timer.start();
	restricoes += criaRestricaoDiscProfCurso( );
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest C_DISC_PROF_CURSO1 e C_DISC_PROF_CURSO2: "
		<< ( restricoes - numRestAnterior ) <<" " <<dif <<"seg" <<std::endl; fflush(NULL);
	numRestAnterior = restricoes;
#endif

	lp->updateLP();
	timer.start();
	restricoes += criaRestricaoMaxDiscProfCurso();
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest C_MAX_DISC_PROF_CURSO: "
		<< ( restricoes - numRestAnterior ) <<" " <<dif <<"seg" <<std::endl; fflush(NULL);
	numRestAnterior = restricoes;
#endif


	lp->updateLP();
	timer.start();
	restricoes += criaRestricaoDiscProfOferta( );
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest C_DISC_PROF_OFERTA1 e C_DISC_PROF_OFERTA2: "
		<< ( restricoes - numRestAnterior ) <<" " <<dif <<"seg" <<std::endl; fflush(NULL);
	numRestAnterior = restricoes;
#endif


	lp->updateLP();
	timer.start();
	restricoes += criaRestricaoMaxDiscProfPeriodoOferta();
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest C_MAX_DISC_PROF_OFERTA_PERIODO: "
		<< ( restricoes - numRestAnterior ) <<" " <<dif <<"seg" <<std::endl; fflush(NULL);
	numRestAnterior = restricoes;
#endif
	

	/* // TODO: REVER A MODELAGEM. NÃO ESTÁ CERTO USAR X

	lp->updateLP();
	timer.start();
	restricoes += criaRestricaoCargaHorariaMinimaProfessorSemana();
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest C_CARGA_HOR_MIN_PROF_SEMANA: "
		<< ( restricoes - numRestAnterior ) <<" " <<dif <<"seg" <<std::endl;
	numRestAnterior = restricoes;
#endif
	*/

	lp->updateLP();
	timer.start();
	restricoes += criaRestricaoCargaHorariaMaximaProfessorSemana();
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest C_CARGA_HOR_MAX_PROF_SEMANA: "
		<< ( restricoes - numRestAnterior ) <<" " <<dif <<"seg" <<std::endl; fflush(NULL);
	numRestAnterior = restricoes;
#endif
	
	lp->updateLP();
	timer.start();
//	restricoes += criaRestricaoProfHorarioMultiUnid();
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest C_PROF_HORARIO_MULTIUNID: "
		<< ( restricoes - numRestAnterior ) <<" " <<dif <<"seg" <<std::endl; fflush(NULL);
	numRestAnterior = restricoes;
#endif


	lp->updateLP();
	timer.start();
	restricoes += criaRestricaoCalculaNroProfsAlocadosCurso();
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest C_CALCULA_NRO_PROFS_CURSO: "
		<< ( restricoes - numRestAnterior ) <<" " <<dif <<"seg" <<std::endl; fflush(NULL);
	numRestAnterior = restricoes;
#endif
	

	lp->updateLP();
	timer.start();
	restricoes += criarRestricaoProfHiHf_();
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest C_PROF_HOR_INIC_LB, C_PROF_HOR_INIC_UB, C_PROF_HOR_FIN_LB, C_PROF_HOR_FIN_UB, C_PROF_GAP: "
		<< ( restricoes - numRestAnterior ) <<" " <<dif <<"seg" <<std::endl; fflush(NULL);
	numRestAnterior = restricoes;
#endif
	

	/*
	//	NÃO FAZ SENTIDO, DADO QUE TEREMOS ATENDIMENTO COMPLETO USANDO PROF VIRTUAL

	lp->updateLP();
	timer.start();
	restricoes += criaRestricaoAlocMinAluno();
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest C_ALOC_MIN_ALUNO: "
		<< ( restricoes - numRestAnterior ) <<" " <<dif <<"seg" <<std::endl;
	numRestAnterior = restricoes;
#endif	
	*/

	if ( this->getRodada() == Operacional::OP_VIRTUAL_PERFIL ) // RODADA 1
	{

		lp->updateLP();
		timer.start();
		restricoes += criaRestricaoGaranteMinProfsPorCurso();
		timer.stop();
		dif = timer.getCronoCurrSecs();

	#ifdef PRINT_cria_restricoes
		std::cout << "numRest C_GARANTE_MIN_PROFS_CURSO: "
			<< ( restricoes - numRestAnterior ) <<" " <<dif <<"seg" <<std::endl; fflush(NULL);
		numRestAnterior = restricoes;
	#endif
	

		lp->updateLP();
		timer.start();
		restricoes += criaRestricaoEstimaNroProfsVirtuaisAlocadosCurso();
		timer.stop();
		dif = timer.getCronoCurrSecs();

	#ifdef PRINT_cria_restricoes
		std::cout << "numRest C_ESTIMA_NRO_PROFS_VIRTUAIS_CURSO: "
			<< ( restricoes - numRestAnterior ) <<" " <<dif <<"seg" <<std::endl; fflush(NULL);
		numRestAnterior = restricoes;
	#endif


		lp->updateLP();
		timer.start();
		restricoes += criaRestricaoEstimaNroProfsVirtuaisMestresAlocadosCurso();
		timer.stop();
		dif = timer.getCronoCurrSecs();

	#ifdef PRINT_cria_restricoes
		std::cout << "numRest C_ESTIMA_NRO_PROFS_VIRTUAIS_MEST_CURSO: "
			<< ( restricoes - numRestAnterior ) <<" " <<dif <<"seg" <<std::endl; fflush(NULL);
		numRestAnterior = restricoes;
	#endif


		lp->updateLP();
		timer.start();
		restricoes += criaRestricaoEstimaNroProfsVirtuaisDoutoresAlocadosCurso();
		timer.stop();
		dif = timer.getCronoCurrSecs();

	#ifdef PRINT_cria_restricoes
		std::cout << "numRest C_ESTIMA_NRO_PROFS_VIRTUAIS_DOUT_CURSO: "
			<< ( restricoes - numRestAnterior ) <<" " <<dif <<"seg" <<std::endl; fflush(NULL);
		numRestAnterior = restricoes;
	#endif
	

		lp->updateLP();
		timer.start();
		restricoes += criaRestricaoEstimaNroProfsVirtuaisGeraisAlocadosCurso();
		timer.stop();
		dif = timer.getCronoCurrSecs();

	#ifdef PRINT_cria_restricoes
		std::cout << "numRest C_ESTIMA_NRO_PROFS_VIRTUAIS_GERAIS_CURSO: "
			<< ( restricoes - numRestAnterior ) <<" " <<dif <<"seg" <<std::endl; fflush(NULL);
		numRestAnterior = restricoes;
	#endif

   
		lp->updateLP();
		timer.start();
		restricoes += criaRestricaoEstimaNroProfsVirtuaisPorContratoCurso();
		timer.stop();
		dif = timer.getCronoCurrSecs();

	#ifdef PRINT_cria_restricoes
		std::cout << "numRest C_ESTIMA_NRO_PROFS_VIRTUAIS_CONTRATO_CURSO: "
			<< ( restricoes - numRestAnterior ) <<" " <<dif <<"seg" <<std::endl; fflush(NULL);
		numRestAnterior = restricoes;
	#endif


	}
	else // RODADA 2
	{
		lp->updateLP();
		timer.start();
		restricoes += criaRestricaoCalculaNroProfsVirtAlocadosCurso();
		timer.stop();
		dif = timer.getCronoCurrSecs();

	#ifdef PRINT_cria_restricoes
		std::cout << "numRest C_CALCULA_NRO_PROFS_VIRTUAIS_CURSO: "
			<< ( restricoes - numRestAnterior ) <<" " <<dif <<"seg" <<std::endl; fflush(NULL);
		numRestAnterior = restricoes;
	#endif


		lp->updateLP();
		timer.start();
		restricoes += criaRestricaoCalculaNroProfsVirtMestAlocadosCurso();
		timer.stop();
		dif = timer.getCronoCurrSecs();

	#ifdef PRINT_cria_restricoes
		std::cout << "numRest C_CALCULA_NRO_PROFS_VIRTUAIS_MEST_CURSO: "
			<< ( restricoes - numRestAnterior ) <<" " <<dif <<"seg" <<std::endl; fflush(NULL);
		numRestAnterior = restricoes;
	#endif


		lp->updateLP();
		timer.start();
		restricoes += criaRestricaoCalculaNroProfsVirtDoutAlocadosCurso();
		timer.stop();
		dif = timer.getCronoCurrSecs();

	#ifdef PRINT_cria_restricoes
		std::cout << "numRest C_CALCULA_NRO_PROFS_VIRTUAIS_DOUT_CURSO: "
			<< ( restricoes - numRestAnterior ) <<" " <<dif <<"seg" <<std::endl; fflush(NULL);
		numRestAnterior = restricoes;
	#endif
	

		lp->updateLP();
		timer.start();
		restricoes += criaRestricaoCalculaNroProfsVirtGeraisAlocadosCurso();
		timer.stop();
		dif = timer.getCronoCurrSecs();

	#ifdef PRINT_cria_restricoes
		std::cout << "numRest C_CALCULA_NRO_PROFS_VIRTUAIS_GERAIS_CURSO: "
			<< ( restricoes - numRestAnterior ) <<" " <<dif <<"seg" <<std::endl; fflush(NULL);
		numRestAnterior = restricoes;
	#endif	
	

		lp->updateLP();
		timer.start();
		restricoes += criaRestricaoUsaProfVirtual();
		timer.stop();
		dif = timer.getCronoCurrSecs();

	#ifdef PRINT_cria_restricoes
		std::cout << "numRest C_USA_PROF_VIRTUAL: "
			<< ( restricoes - numRestAnterior ) <<" " <<dif <<"seg" <<std::endl; fflush(NULL);
		numRestAnterior = restricoes;
	#endif
	

		lp->updateLP();
		timer.start();
		restricoes += criaRestricaoCalculaNroProfsVirtPorContratoAlocadosCurso();
		timer.stop();
		dif = timer.getCronoCurrSecs();

	#ifdef PRINT_cria_restricoes
		std::cout << "numRest C_CALCULA_NRO_PROFS_VIRTUAIS_CONTRATO_CURSO: "
			<< ( restricoes - numRestAnterior ) <<" " <<dif <<"seg" <<std::endl; fflush(NULL);
		numRestAnterior = restricoes;
	#endif


	}

	lp->updateLP();
	timer.start();
	restricoes += criaRestricaoSomaNroProfsVirtuaisAlocadosCurso();
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest C_SOMA_NRO_PROFS_VIRTUAIS_CURSO: "
		<< ( restricoes - numRestAnterior ) <<" " <<dif <<"seg" <<std::endl; fflush(NULL);
	numRestAnterior = restricoes;
#endif


	lp->updateLP();
	timer.start();
	restricoes += criaRestricaoMaximoNaoMestresCurso();
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest C_MAX_NAO_MEST_CURSO: "
		<< ( restricoes - numRestAnterior ) <<" " <<dif <<"seg" <<std::endl; fflush(NULL);
	numRestAnterior = restricoes;
#endif


	lp->updateLP();
	timer.start();
	restricoes += criaRestricaoMaximoNaoDoutoresCurso();
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest C_MAX_NAO_DOUT_CURSO: "
		<< ( restricoes - numRestAnterior ) <<" " <<dif <<"seg" <<std::endl; fflush(NULL);
	numRestAnterior = restricoes;
#endif

	lp->updateLP();
	timer.start();
	restricoes += criaRestricaoMinimoMestresCurso();
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest C_MIN_MEST_CURSO: "
		<< ( restricoes - numRestAnterior ) <<" " <<dif <<"seg" <<std::endl; fflush(NULL);
	numRestAnterior = restricoes;
#endif

	lp->updateLP();
	timer.start();
	restricoes += criaRestricaoMinimoDoutoresCurso();
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest C_MIN_DOUT_CURSO: "
		<< ( restricoes - numRestAnterior ) <<" " <<dif <<"seg" <<std::endl; fflush(NULL);
	numRestAnterior = restricoes;
#endif

	lp->updateLP();
	timer.start();
	restricoes += criaRestricaoMaximoProfSemContratoCurso();
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest C_MAX_SEM_CONTRATO_CURSO: "
		<< ( restricoes - numRestAnterior ) <<" " <<dif <<"seg" <<std::endl; fflush(NULL);
	numRestAnterior = restricoes;
#endif
	
	lp->updateLP();
	timer.start();
	restricoes += criaRestricaoMinimoContratoCurso();
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest C_MIN_CONTRATO_CURSO: "
		<< ( restricoes - numRestAnterior ) <<" " <<dif <<"seg" <<std::endl; fflush(NULL);
	numRestAnterior = restricoes;
#endif
	
	lp->updateLP();
	timer.start();
	restricoes += criaRestricaoDiscPTAulasContinuas();
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest C_AULA_PT_SEQUENCIAL: "
		<< ( restricoes - numRestAnterior ) <<" " <<dif <<"seg" <<std::endl; fflush(NULL);
	numRestAnterior = restricoes;
#endif
		
	
	lp->updateLP();
	timer.start();
	restricoes += criaRestricaoMaxDiasSemanaProf();
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest C_MAX_DIAS_SEMANA_PROF: "
		<< ( restricoes - numRestAnterior ) <<" " <<dif <<"seg" <<std::endl; fflush(NULL);
	numRestAnterior = restricoes;
#endif	
		
	
	lp->updateLP();
	timer.start();
	restricoes += criaRestricaoMinCredDiariosProf();
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest C_MIN_CREDS_DIARIOS_PROF: "
		<< ( restricoes - numRestAnterior ) <<" " <<dif <<"seg" <<std::endl; fflush(NULL);
	numRestAnterior = restricoes;
#endif	
		
	
	lp->updateLP();
	timer.start();
	restricoes += criaRestricaoProfDescansoMinimo();
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest C_PROF_MIN_DESCANSO: "
		<< ( restricoes - numRestAnterior ) <<" " <<dif <<"seg" <<std::endl; fflush(NULL);
	numRestAnterior = restricoes;
#endif	
	
	lp->updateLP();	

	return restricoes;
}

void Operacional::chgCoeffList(
   std::vector< std::pair< int, int > > cL,
   std::vector< double > cLV )
{
   lp->updateLP();

   int * rList = new int[ cL.size() ];
   int * cList = new int[ cL.size() ];
   double * vList = new double[ cLV.size() ];

   for ( int i = 0; i < (int) cL.size(); i++ )
   {
      rList[ i ] = cL[ i ].first;
      cList[ i ] = cL[ i ].second;
      vList[ i ] = cLV[ i ];
   }

   lp->chgCoefList( (int) cL.size(), rList, cList, vList );
   lp->updateLP();

   delete [] rList;
   delete [] cList;
   delete [] vList;
}


int Operacional::criaRestricaoSalaParHorario()
{
	// equivale a criaRestricaoSalaHorario, porem criaRestricaoSalaHorario é mais forte (clique)!

   int restricoes = 0;
   char name[ 200 ];
   int nnz = ( this->problemData->aulas.size() * this->problemData->horarios_aula_ordenados.size() );
   
   ConstraintOp c;
   VariableOpHash::iterator vit;
   ConstraintOpHash::iterator cit;   

   std::map< Sala*, std::map< int, std::map< DateTime, std::map< DateTime, std::map< Aula*, 
	   GGroup<std::pair<VariableOp, int>>, LessPtr<Aula> > > > >, LessPtr<Sala> > mapSalaDiaHiAulaVar;
      
   for( vit = vHashOp.begin(); vit != vHashOp.end(); vit++ )
   {
	   VariableOp vOp = ( vit->first );

	   if ( vOp.getType() != VariableOp::V_X_PROF_AULA_HOR )
	   {
		   continue;
	   }

	   Sala *sala = vOp.getAula()->getSala();
	   std::pair<VariableOp, int> var_id ( vOp, vit->second );
	   DateTime dti = vOp.getHorarioAula()->getInicio();
	   int dia = vOp.getAula()->getDiaSemana();
	   int nCred = vOp.getAula()->getTotalCreditos();
	   int duracao = vOp.getAula()->getDisciplina()->getTempoCredSemanaLetiva();
	   DateTime dtf = dti;
	   dtf.addMinutes( duracao*nCred );

	   mapSalaDiaHiAulaVar[sala][dia][dti][dtf][vOp.getAula()].add( var_id );
   }


   std::map< Sala*, std::map< int, std::map< DateTime, std::map< DateTime, std::map< Aula*, 
	   GGroup<std::pair<VariableOp, int>>, LessPtr<Aula> > > > >, LessPtr<Sala> >::iterator
	   itSala = mapSalaDiaHiAulaVar.begin();

   for ( ; itSala != mapSalaDiaHiAulaVar.end(); itSala++)
   {
	   Sala *sala = itSala->first;

	   std::map< int, std::map< DateTime, std::map< DateTime, std::map< Aula*, 
		GGroup<std::pair<VariableOp, int>>, LessPtr<Aula> > > > > *mapDia = & itSala->second;
	   std::map< int, std::map< DateTime, std::map< DateTime, std::map< Aula*, 
		GGroup<std::pair<VariableOp, int>>, LessPtr<Aula> > > > >::iterator itDia = mapDia->begin();

		for ( ; itDia != mapDia->end(); itDia++)
		{
			int dia = itDia->first;

		   std::map< DateTime, std::map< DateTime, std::map< Aula*, 
				GGroup<std::pair<VariableOp, int>>, LessPtr<Aula> > > > *mapDti1 = & itDia->second;
		   std::map< DateTime, std::map< DateTime, std::map< Aula*, 
				GGroup<std::pair<VariableOp, int>>, LessPtr<Aula> > > >::iterator itDti1 = mapDti1->begin();
			for ( ; itDti1 != mapDti1->end(); itDti1++)
			{
				DateTime dti1 = itDti1->first;

			   std::map< DateTime, std::map< Aula*, GGroup<std::pair<VariableOp, int>>,
				   LessPtr<Aula> > > *mapDtf1 = & itDti1->second;
			   std::map< DateTime, std::map< Aula*, GGroup<std::pair<VariableOp, int>>,
				   LessPtr<Aula> > >::iterator itDtf1 = mapDtf1->begin();
			   for ( ; itDtf1 != mapDtf1->end(); itDtf1++)
			   {
				   DateTime dtf1 = itDtf1->first;

				   std::map< Aula*, GGroup<std::pair<VariableOp, int>>,
					   LessPtr<Aula> > *mapAula1 = &itDtf1->second;
				   std::map< Aula*, GGroup<std::pair<VariableOp, int>>,
					   LessPtr<Aula> >::iterator itAula1 = mapAula1->begin();
			       for ( ; itAula1 != mapAula1->end(); itAula1++)
				   {
					    Aula* aula1 = itAula1->first;

						GGroup<std::pair<VariableOp, int>> *ggroupVars1 = & itAula1->second;
						GGroup<std::pair<VariableOp, int>>::iterator itVars1 = ggroupVars1->begin();
						for ( ; itVars1 != ggroupVars1->end(); itVars1++ )
						{
							VariableOp vOp1 = (*itVars1).first;
							int vId1 = (*itVars1).second;
													   
							HorarioAula *hor1 = vOp1.getHorarioAula();
						
							// --------------------------------
							// --------------------------------

						    std::map< DateTime, std::map< DateTime, std::map< Aula*, 
								GGroup<std::pair<VariableOp, int>>, LessPtr<Aula> > > > *mapDti2 = & itDia->second;
						    std::map< DateTime, std::map< DateTime, std::map< Aula*, 
								GGroup<std::pair<VariableOp, int>>, LessPtr<Aula> > > >::iterator itDti2 = mapDti2->begin();
							for ( ; itDti2 != mapDti2->end(); itDti2++)
							{
								DateTime dti2 = itDti2->first;

								if ( !( dti2 <= dtf1 ) )
								{
									break;
								}


							   std::map< DateTime, std::map< Aula*, GGroup<std::pair<VariableOp, int>>,
								   LessPtr<Aula> > > *mapDtf2 = & itDti2->second;
							   std::map< DateTime, std::map< Aula*, GGroup<std::pair<VariableOp, int>>,
								   LessPtr<Aula> > >::reverse_iterator itDtf2 = mapDtf2->rbegin();
							   for ( ; itDtf2 != mapDtf2->rend(); itDtf2++)
							   {
								   DateTime dtf2 = itDtf2->first;

								   if ( !( dtf2 > dti1 ) )
								   {
										break;
								   }

								   std::map< Aula*, GGroup<std::pair<VariableOp, int>>,
									   LessPtr<Aula> > *mapAula2 = &itDtf2->second;
								   std::map< Aula*, GGroup<std::pair<VariableOp, int>>,
									   LessPtr<Aula> >::iterator itAula2 = mapAula2->begin();
								   for ( ; itAula2 != mapAula2->end(); itAula2++)
								   {
										Aula* aula2 = itAula2->first;

										GGroup<std::pair<VariableOp, int>> *ggroupVars2 = & itAula2->second;
										GGroup<std::pair<VariableOp, int>>::iterator itVars2 = ggroupVars2->begin();
										for ( ; itVars2 != ggroupVars2->end(); itVars2++ )
										{
											VariableOp vOp2 = (*itVars2).first;
											int vId2 = (*itVars2).second;
													   
											HorarioAula *hor2 = vOp2.getHorarioAula();			

											c.reset();
											c.setType( ConstraintOp::C_SALA_HORARIO );
											c.setPar1AulaHor( aula1, hor1 );
											c.setPar2AulaHor( aula2, hor2 );
											c.setSala( sala );

											cit = cHashOp.find(c);
											if ( cit == cHashOp.end() )
											{
												cHashOp[ c ] = lp->getNumRows();

												sprintf( name, "%s", c.toString().c_str() );
												OPT_ROW row( nnz, OPT_ROW::LESS , 1.0, name );
												row.insert( vId1, 1.0 );
												row.insert( vId2, 1.0 );
												
												lp->addRow( row );
												restricoes++;
											}
											else
											{
												lp->chgCoef(cit->second, vId1, 1.0);
												lp->chgCoef(cit->second, vId2, 1.0);
											}
											
										}
								   }
							   }
							}
							// --------------------------------
							// --------------------------------
						}
					}
				}
			}
		}

   }

   return restricoes;
}


int Operacional::criaRestricaoSalaHorario()
{
   int restricoes = 0;
   char name[ 200 ];
   int nnz = ( this->problemData->aulas.size() * this->problemData->horarios_aula_ordenados.size() );

   GGroup< Sala *, LessPtr< Sala > > salas = this->problemData->getSalas();

   ConstraintOp c;
   VariableOpHash::iterator vit;
   ConstraintOpHash::iterator cit;

   vit = vHashOp.begin();

   while(vit != vHashOp.end())
   {
	   VariableOp vOp = ( vit->first );

	   if (vOp.getType() != VariableOp::V_X_PROF_AULA_HOR)
	   {
		   vit++;
		   continue;
	   }

	   Sala *sala = vOp.getSala();
	   ITERA_GGROUP_LESSPTRPTR( it_horario_dia,
		   sala->horariosDia, HorarioDia )
	   {
		   HorarioDia * horario_dia = ( *it_horario_dia );

		   int dia_semana = horario_dia->getDia();

		   if(vOp.getDia() != dia_semana)
			   continue;

		   HorarioAula * horario_aula = horario_dia->getHorarioAula();

		   DateTime inicio = horario_aula->getInicio();
		   DateTime fim = horario_aula->getFinal();

		   int nCred2 = vOp.getAula()->getTotalCreditos();
		   int duracao = vOp.getDisciplina()->getTempoCredSemanaLetiva();			
		   DateTime dt2Inicio = vOp.getHorarioAula()->getInicio();
		   HorarioAula *horarioAulaFim = vOp.getHorarioAula();
		   for (int k = 1; k < nCred2; k++)
           {
			   horarioAulaFim = horarioAulaFim->getCalendario()->getProximoHorario(horarioAulaFim);
		   }
		   DateTime dt2Fim = horarioAulaFim->getFinal();

		   if (  ( vOp.getHorarioAula() != horario_aula ) &&				    
			   !( ( dt2Inicio <= inicio ) && ( dt2Fim > inicio ) ) )
		   {
			   continue;
		   }

		   c.reset();
		   c.setType( ConstraintOp::C_SALA_HORARIO );
		   c.setSala( sala );
		   c.setHorario( horario_dia );
		   c.setHorarioAula( horario_aula );
		   c.setDia( dia_semana );

		   cit = cHashOp.find(c);
		   if ( cit == cHashOp.end() )
		   {
			   cHashOp[ c ] = lp->getNumRows();

			   sprintf( name, "%s", c.toString().c_str() );
			   OPT_ROW row( nnz, OPT_ROW::LESS , 1.0, name );
			   row.insert( vit->second, 1.0 );
			   lp->addRow( row );
			   restricoes++;
		   }
		   else
		   {
			   lp->chgCoef(cit->second, vit->second, 1.0);
		   }
	   }
	   vit++;
   }

   return restricoes;
}

int Operacional::criaRestricaoProfessorParHorario()
{
	// equivale a criaRestricaoProfessorHorario, porem criaRestricaoProfessorHorario é mais forte (clique)!

   int restricoes = 0;
   char name[ 200 ];
   int nnz = ( this->problemData->aulas.size() * this->problemData->horarios_aula_ordenados.size() );
   
   ConstraintOp c;
   VariableOpHash::iterator vit;
   ConstraintOpHash::iterator cit;   

   std::map< Professor*, std::map< int, std::map< DateTime, std::map< DateTime, std::map< Aula*, 
	   GGroup<std::pair<VariableOp, int>>, LessPtr<Aula> > > > >, LessPtr<Professor> > mapProfDiaHiAulaVar;

   vit = vHashOp.begin();
   for( ; vit != vHashOp.end(); vit++ )
   {
	   VariableOp vOp = ( vit->first );

	   if ( vOp.getType() != VariableOp::V_X_PROF_AULA_HOR ||
		  ( vOp.getProfessor()->eVirtual() && this->getRodada() == OP_VIRTUAL_PERFIL ) )
	   {
		   continue;
	   }

	   std::pair<VariableOp, int> var_id ( vOp, vit->second );
	   DateTime dti = vOp.getHorarioAula()->getInicio();
	   int dia = vOp.getAula()->getDiaSemana();
	   int nCred = vOp.getAula()->getTotalCreditos();
	   int duracao = vOp.getAula()->getDisciplina()->getTempoCredSemanaLetiva();
	   DateTime dtf = dti;
	   dtf.addMinutes( duracao*nCred );

	   mapProfDiaHiAulaVar[vOp.getProfessor()][dia][dti][dtf][vOp.getAula()].add( var_id );
   }


   std::map< Professor*, std::map< int, std::map< DateTime, std::map< DateTime, std::map< Aula*, 
	   GGroup<std::pair<VariableOp, int>>, LessPtr<Aula> > > > >, LessPtr<Professor> >::iterator
	   itProf = mapProfDiaHiAulaVar.begin();

   for ( ; itProf != mapProfDiaHiAulaVar.end(); itProf++)
   {
	   Professor *prof = itProf->first;

	   std::map< int, std::map< DateTime, std::map< DateTime, std::map< Aula*, 
		GGroup<std::pair<VariableOp, int>>, LessPtr<Aula> > > > > *mapDia = & itProf->second;
	   std::map< int, std::map< DateTime, std::map< DateTime, std::map< Aula*, 
		GGroup<std::pair<VariableOp, int>>, LessPtr<Aula> > > > >::iterator itDia = mapDia->begin();

		for ( ; itDia != mapDia->end(); itDia++)
		{
			int dia = itDia->first;

		   std::map< DateTime, std::map< DateTime, std::map< Aula*, 
				GGroup<std::pair<VariableOp, int>>, LessPtr<Aula> > > > *mapDti1 = & itDia->second;
		   std::map< DateTime, std::map< DateTime, std::map< Aula*, 
				GGroup<std::pair<VariableOp, int>>, LessPtr<Aula> > > >::iterator itDti1 = mapDti1->begin();
			for ( ; itDti1 != mapDti1->end(); itDti1++)
			{
				DateTime dti1 = itDti1->first;

			   std::map< DateTime, std::map< Aula*, GGroup<std::pair<VariableOp, int>>,
				   LessPtr<Aula> > > *mapDtf1 = & itDti1->second;
			   std::map< DateTime, std::map< Aula*, GGroup<std::pair<VariableOp, int>>,
				   LessPtr<Aula> > >::iterator itDtf1 = mapDtf1->begin();
			   for ( ; itDtf1 != mapDtf1->end(); itDtf1++)
			   {
				   DateTime dtf1 = itDtf1->first;

				   std::map< Aula*, GGroup<std::pair<VariableOp, int>>,
					   LessPtr<Aula> > *mapAula1 = &itDtf1->second;
				   std::map< Aula*, GGroup<std::pair<VariableOp, int>>,
					   LessPtr<Aula> >::iterator itAula1 = mapAula1->begin();
			       for ( ; itAula1 != mapAula1->end(); itAula1++)
				   {
					    Aula* aula1 = itAula1->first;

						GGroup<std::pair<VariableOp, int>> *ggroupVars1 = & itAula1->second;
						GGroup<std::pair<VariableOp, int>>::iterator itVars1 = ggroupVars1->begin();
						for ( ; itVars1 != ggroupVars1->end(); itVars1++ )
						{
							VariableOp vOp1 = (*itVars1).first;
							int vId1 = (*itVars1).second;
													   
							HorarioAula *hor1 = vOp1.getHorarioAula();
						
							// --------------------------------
							// --------------------------------

						    std::map< DateTime, std::map< DateTime, std::map< Aula*, 
								GGroup<std::pair<VariableOp, int>>, LessPtr<Aula> > > > *mapDti2 = & itDia->second;
						    std::map< DateTime, std::map< DateTime, std::map< Aula*, 
								GGroup<std::pair<VariableOp, int>>, LessPtr<Aula> > > >::iterator itDti2 = mapDti2->begin();
							for ( ; itDti2 != mapDti2->end(); itDti2++)
							{
								DateTime dti2 = itDti2->first;

								if ( !( dti2 <= dtf1 ) )
								{
									break;
								}

							   //std::map< DateTime, std::map< Aula*, GGroup<std::pair<VariableOp, int>>,
								  // LessPtr<Aula> > > *mapDtf2 = & itDti2->second;
							   //std::map< DateTime, std::map< Aula*, GGroup<std::pair<VariableOp, int>>,
								  // LessPtr<Aula> > >::iterator itDtf2 = mapDtf2->begin();
							   //for ( ; itDtf2 != mapDtf2->end(); itDtf2++)
							   //{
								  // DateTime dtf2 = itDtf2->first;

								  // if ( !( dtf2 > dti1 ) )
								  // {
										//continue;
								  // }

							   std::map< DateTime, std::map< Aula*, GGroup<std::pair<VariableOp, int>>,
								   LessPtr<Aula> > > *mapDtf2 = & itDti2->second;
							   std::map< DateTime, std::map< Aula*, GGroup<std::pair<VariableOp, int>>,
								   LessPtr<Aula> > >::reverse_iterator itDtf2 = mapDtf2->rbegin();
							   for ( ; itDtf2 != mapDtf2->rend(); itDtf2++)
							   {
								   DateTime dtf2 = itDtf2->first;

								   if ( !( dtf2 > dti1 ) )
								   {
										break;
								   }

								   std::map< Aula*, GGroup<std::pair<VariableOp, int>>,
									   LessPtr<Aula> > *mapAula2 = &itDtf2->second;
								   std::map< Aula*, GGroup<std::pair<VariableOp, int>>,
									   LessPtr<Aula> >::iterator itAula2 = mapAula2->begin();
								   for ( ; itAula2 != mapAula2->end(); itAula2++)
								   {
										Aula* aula2 = itAula2->first;

										GGroup<std::pair<VariableOp, int>> *ggroupVars2 = & itAula2->second;
										GGroup<std::pair<VariableOp, int>>::iterator itVars2 = ggroupVars2->begin();
										for ( ; itVars2 != ggroupVars2->end(); itVars2++ )
										{
											VariableOp vOp2 = (*itVars2).first;
											int vId2 = (*itVars2).second;
													   
											HorarioAula *hor2 = vOp2.getHorarioAula();			

											c.reset();
											c.setType( ConstraintOp::C_PROFESSOR_HORARIO );
											c.setPar1AulaHor( aula1, hor1 );
											c.setPar2AulaHor( aula2, hor2 );
											c.setProfessor( prof );

											cit = cHashOp.find(c);
											if ( cit == cHashOp.end() )
											{
												cHashOp[ c ] = lp->getNumRows();

												sprintf( name, "%s", c.toString().c_str() );
												OPT_ROW row( nnz, OPT_ROW::LESS , 1.0, name );
												row.insert( vId1, 1.0 );
												row.insert( vId2, 1.0 );
												
												lp->addRow( row );
												restricoes++;
											}
											else
											{
												lp->chgCoef(cit->second, vId1, 1.0);
												lp->chgCoef(cit->second, vId2, 1.0);
											}
											
										}
								   }
							   }
							}
							// --------------------------------
							// --------------------------------
						}
					}
				}
			}
		}

   }

   return restricoes;
}

int Operacional::criaRestricaoProfessorHorario()
{
   int restricoes = 0;
   char name[ 200 ];
   int nnz = ( this->problemData->aulas.size() * this->problemData->horarios_aula_ordenados.size() );
   
   ConstraintOp c;
   VariableOpHash::iterator vit;
   ConstraintOpHash::iterator cit;

   vit = vHashOp.begin();

   while(vit != vHashOp.end())
   {
	   VariableOp vOp = ( vit->first );

	   if ( vOp.getType() != VariableOp::V_X_PROF_AULA_HOR ||
		  ( vOp.getProfessor()->eVirtual() && this->getRodada() == OP_VIRTUAL_PERFIL ) )
	   {
		   vit++;
		   continue;
	   }

	   Professor *prof = vOp.getProfessor();
	   ITERA_GGROUP_LESSPTR( it_horario_dia, prof->horariosDia, HorarioDia )
      {
		   HorarioDia * horario_dia = ( *it_horario_dia );

		   int dia = horario_dia->getDia();

		   if(vOp.getDia() != dia)
			   continue;
		   
		   HorarioAula * horario_aula = horario_dia->getHorarioAula();
		   DateTime inicio = horario_aula->getInicio();

		   int nCredAula = vOp.getAula()->getTotalCreditos(); 
		   HorarioAula *hAulaFim = vOp.getHorarioAula()->getCalendario()->getHorarioMaisNCreds( vOp.getHorarioAula(), nCredAula-1 );
		   
		   DateTime dtInicioAula = vOp.getHorarioAula()->getInicio();
		   DateTime dtFimAula = hAulaFim->getFinal();
		   
		   if ( !( dtInicioAula.earlierTime(inicio, true) && inicio.earlierTime(dtFimAula) ) )
		   {
			   continue;
		   }

		   c.reset();
		   c.setType( ConstraintOp::C_PROFESSOR_HORARIO );
		   c.setHorarioAula( horario_aula );
		   c.setDia( dia );
		   c.setProfessor( prof );

		   cit = cHashOp.find(c);
		   if ( cit == cHashOp.end() )
		   {
			   cHashOp[ c ] = lp->getNumRows();

			   sprintf( name, "%s", c.toString().c_str() );
			   OPT_ROW row( nnz, OPT_ROW::LESS , 1.0, name );
			   row.insert( vit->second, 1.0 );
			   lp->addRow( row );
			   restricoes++;
		   }
		   else
		   {
			   lp->chgCoef(cit->second, vit->second, 1.0);
		   }
	   }
	   vit++;
   }

   return restricoes;
}

int Operacional::criaRestricaoAlunoHorario__2( void )
{
   int restricoes = 0;
   int nnz;
   char name[ 200 ];

   ConstraintOp c;
   VariableOpHash::iterator vit;
   ConstraintOpHash::iterator cit;

   std::vector< std::pair< int, int > > coeffList;
   std::vector< double > coeffListVal;
   std::pair< int, int > auxCoef;

	// ============================================================================================================
	 std::cout<<"\nMapeando horarios do problemData..."; fflush(NULL);

	std::map< int /*dia*/, std::map< DateTime, HorarioDia* > > mapDiaDtHorAula;
	ITERA_GGROUP_LESSPTR( itHd, problemData->horariosDia, HorarioDia )
	{
		HorarioAula *ha = itHd->getHorarioAula();
		int dia = itHd->getDia();
				  
		mapDiaDtHorAula[ dia ][ ha->getInicio() ] = *itHd;
	}

	
	// ============================================================================================================
	 std::cout<<"\nMapeando horarios das variaveis...";fflush(NULL);
	 	
	std::map< Campus*, std::map<Disciplina*, std::map< int /*turma*/, std::map< int /*dia*/, 
		std::map< DateTime, GGroup< std::pair<VariableOp,int> > > > > , LessPtr<Disciplina> >, LessPtr<Campus> >  
				mapCpDiscTurmaDiaDt;
	
   vit = vHashOp.begin();
   for (; vit != vHashOp.end(); vit++ )
   {
      VariableOp v = vit->first;

      if ( v.getType() != VariableOp::V_X_PROF_AULA_HOR )
      {
         continue;
      }

	  Campus* campus = problemData->retornaCampus( v.getAula()->getSala()->getIdUnidade() );
	  int turma = v.getAula()->getTurma();
	  Disciplina* disc = v.getAula()->getDisciplina();
	  int dia = v.getDia();
	  DateTime dti = v.getHorarioAula()->getInicio();

	  mapCpDiscTurmaDiaDt[campus][disc][turma][dia][dti].add( std::make_pair( v, vit->second ) );
   }
   
	// ============================================================================================================
	
   // PERCORRE HORARIOS POSSIVEIS DE TODAS AS AULAS
	std::cout<<"\nPercorrendo horarios de todas as aulas...";fflush(NULL);

   std::map< Campus*, std::map<Disciplina*, std::map< int /*turma*/, std::map< int /*dia*/, 
	   std::map< DateTime, GGroup< std::pair<VariableOp,int> > > > > , LessPtr<Disciplina> >, LessPtr<Campus> >::iterator
			itMapCp = mapCpDiscTurmaDiaDt.begin();   
   for (; itMapCp != mapCpDiscTurmaDiaDt.end(); itMapCp++ )
   {
	   Campus* campus = itMapCp->first;

	   std::map<Disciplina*, std::map< int /*turma*/, std::map< int /*dia*/, 
		   std::map< DateTime, GGroup< std::pair<VariableOp,int> > > > > , LessPtr<Disciplina> >
		   *mapDisc = & itMapCp->second;
	   std::map<Disciplina*, std::map< int /*turma*/, std::map< int /*dia*/, 
		   std::map< DateTime, GGroup< std::pair<VariableOp,int> > > > > , LessPtr<Disciplina> >::iterator
		   itDisc = mapDisc->begin();   
	   for (; itDisc != mapDisc->end(); itDisc++ )
	   {
		   Disciplina* disc = itDisc->first;

		   std::map< int /*turma*/, std::map< int /*dia*/, 
			   std::map< DateTime, GGroup< std::pair<VariableOp,int> > > > > *mapTurma = &itDisc->second;
		   std::map< int /*turma*/, std::map< int /*dia*/, 
			   std::map< DateTime, GGroup< std::pair<VariableOp,int> > > > >::iterator 
			   itMapTurma = mapTurma->begin();		
		   for (; itMapTurma != mapTurma->end(); itMapTurma++ )
		   {
			   int turma = itMapTurma->first;
			   
			   // -----------------------
			   // Acha alunos da turma
				Trio< int, int, Disciplina* > trio;
				trio.set( campus->getId(), turma, disc );

				std::map< Trio< int /*campusId*/, int /*turma*/, Disciplina* >, 
					GGroup< AlunoDemanda*, LessPtr< AlunoDemanda > > >::iterator itMapAtend;						   
				itMapAtend = problemData->mapCampusTurmaDisc_AlunosDemanda.find( trio );
				if ( itMapAtend == problemData->mapCampusTurmaDisc_AlunosDemanda.end() )
				{
					std::cout<<"\nErro: turma nao encontrada.";
					continue;
				}
				
				GGroup< AlunoDemanda*, LessPtr< AlunoDemanda > > *alunosDemanda = & itMapAtend->second;				
			   // -----------------------

			   std::map< int /*dia*/, std::map< DateTime, GGroup< std::pair<VariableOp,int> > > > *mapDia = &itMapTurma->second;
			   std::map< int /*dia*/, std::map< DateTime, GGroup< std::pair<VariableOp,int> > > >::iterator
				   itMapDia = mapDia->begin();		   
			   for (; itMapDia != mapDia->end(); itMapDia++ )		   
			   {
				   int dia = itMapDia->first;

				   std::map< DateTime, GGroup< std::pair<VariableOp,int> > > *mapDt = & itMapDia->second;
				   std::map< DateTime, GGroup< std::pair<VariableOp,int> > >::iterator
					   itMapDt = mapDt->begin(); 
				   for (; itMapDt != mapDt->end(); itMapDt++ )		   
				   {		
					   DateTime dtiAula = itMapDt->first;

					   std::map< DateTime, HorarioDia* >
						   *mapDtiDoDiaNoCp = & mapDiaDtHorAula[dia];
					   std::map< DateTime, HorarioDia* >::iterator
						   itDtiDoDiaNoCp = mapDtiDoDiaNoCp->find( dtiAula );
					   if ( itDtiDoDiaNoCp == mapDtiDoDiaNoCp->end() )
					   {
							std::cout<<"\nErro: Datetime nao encontrado no campus. Dt " << dtiAula << ", Dia " << dia;
							continue;
					   }

					   GGroup< std::pair<VariableOp,int> > *ggroupVar = & itMapDt->second;
					   GGroup< std::pair<VariableOp,int> >::iterator itVar = ggroupVar->begin();	   
					   for ( ; itVar != ggroupVar->end(); itVar++ )		   
					   {
						   VariableOp v = (*itVar).first;
						   int varId = (*itVar).second;
						   								
						   // --------------------
						   // ACHA O FIM DA AULA
						   int nCred = v.getAula()->getTotalCreditos();
						   int duracao = v.getDisciplina()->getTempoCredSemanaLetiva();			
						   DateTime vInicio = v.getHorarioAula()->getInicio();
						   HorarioAula *horarioAulaFim = v.getHorarioAula();

						   for (int k = 1; k < nCred; k++)
						   {
							   horarioAulaFim = horarioAulaFim->getCalendario()->getProximoHorario(horarioAulaFim);
						   }
						   DateTime vFim = horarioAulaFim->getFinal();
						   
						   // --------------------
						   // PROCURA TODOS OS HORARIOS DE INICIO NO DIA QUE CORTAM A AULA
						   std::map< DateTime, HorarioDia* >::iterator itDt = itDtiDoDiaNoCp;
						   for ( ; itDt != mapDtiDoDiaNoCp->end(); itDt++ )
						   {
							    DateTime inicio = itDt->first;

							    if ( !( ( vInicio <= inicio ) && ( vFim > inicio ) ) )
							    {
								   break;
							    }

								HorarioDia *horario_dia = itDt->second;
								   
								// Para cada aluno alocado na aula
								ITERA_GGROUP_LESSPTR( itAlunoDem, (*alunosDemanda), AlunoDemanda )
								{
									int alunoId = ( *itAlunoDem )->getAlunoId();
									Aluno *aluno = problemData->retornaAluno( alunoId );								 

									c.reset();
									c.setType( ConstraintOp::C_ALUNO_HORARIO );
									c.setAluno( aluno );
									c.setHorario( horario_dia );
									c.setDia( dia );
									c.setHorarioAula( horario_dia->getHorarioAula() );

									cit = cHashOp.find( c );

									if ( cit != cHashOp.end() )
									{
										//lp->chgCoef( cit->second, varId, 1.0 );
										auxCoef.first = cit->second;
										auxCoef.second = varId;

										coeffList.push_back( auxCoef );
										coeffListVal.push_back( 1.0 );
									}
									else
									{
										sprintf( name, "%s", c.toString().c_str() );
										nnz = 100;

										OPT_ROW row( nnz, OPT_ROW::LESS , 1.0, name );

										row.insert( varId, 1.0 );
										cHashOp[ c ] = lp->getNumRows();

										lp->addRow( row );
										restricoes++;
									}  
								}							  
						   }
					   }
				   }
				}
			}
		}
   }
   
   std::cout<<"\nPercorrendo horarios de todas as aulas...";fflush(NULL);

   chgCoeffList( coeffList, coeffListVal );

   return restricoes;
}



int Operacional::criaRestricaoAlunoHorario( void )
{
   int restricoes = 0;
   int nnz;
   char name[ 200 ];

   ConstraintOp c;
   VariableOpHash::iterator vit;
   ConstraintOpHash::iterator cit;

   std::vector< std::pair< int, int > > coeffList;
   std::vector< double > coeffListVal;
   std::pair< int, int > auxCoef;

   vit = vHashOp.begin();

   for (; vit != vHashOp.end(); vit++ )
   {
      VariableOp v = vit->first;

      if ( v.getType() != VariableOp::V_X_PROF_AULA_HOR )
      {
         continue;
      }

	  int campusId = problemData->retornaCampus( v.getAula()->getSala()->getIdUnidade() )->getId();
	  int turma = v.getAula()->getTurma();
	  Disciplina* disc = v.getAula()->getDisciplina();
      
	  std::map< Trio< int /*campusId*/, int /*turma*/, Disciplina* >, GGroup< AlunoDemanda*, LessPtr< AlunoDemanda > > >::iterator itMapAtend;

	  Trio< int, int, Disciplina* > trio;
	  trio.set( campusId, turma, disc );

	  itMapAtend = problemData->mapCampusTurmaDisc_AlunosDemanda.find( trio );
	  if ( itMapAtend != problemData->mapCampusTurmaDisc_AlunosDemanda.end() )
	  {
		  GGroup< AlunoDemanda*, LessPtr< AlunoDemanda > > alunosDemanda = (*itMapAtend).second;

		  // Para cada aluno alocado na aula
		  ITERA_GGROUP_LESSPTR( itAlunoDem, alunosDemanda, AlunoDemanda )
		  {
			    int alunoId = ( *itAlunoDem )->getAlunoId();
			    Aluno *aluno = problemData->retornaAluno( alunoId );

				Sala *sala = v.getSala();
				ITERA_GGROUP_LESSPTRPTR( it_horario_dia, sala->horariosDia, HorarioDia )
				{
					HorarioDia * horario_dia = ( *it_horario_dia );

					int dia = horario_dia->getDia();

					if ( v.getDia() != dia )
						continue;

					HorarioAula * horario_aula = horario_dia->getHorarioAula();

					DateTime inicio = horario_aula->getInicio();

					int nCred = v.getAula()->getTotalCreditos();
					int duracao = v.getDisciplina()->getTempoCredSemanaLetiva();			
					DateTime vInicio = v.getHorarioAula()->getInicio();
					HorarioAula *horarioAulaFim = v.getHorarioAula();
					for (int k = 1; k < nCred; k++)
           			{
						horarioAulaFim = horarioAulaFim->getCalendario()->getProximoHorario(horarioAulaFim);
					}
					DateTime vFim = horarioAulaFim->getFinal();

					if ( !( ( vInicio <= inicio ) && ( vFim > inicio ) ) )
					{
						continue;
					}         

					c.reset();
					c.setType( ConstraintOp::C_ALUNO_HORARIO );
					c.setAluno( aluno );
					c.setHorario( horario_dia );
					c.setDia( dia );
					c.setHorarioAula( horario_aula );

					cit = cHashOp.find( c );

					if ( cit != cHashOp.end() )
					{
						lp->chgCoef( cit->second, vit->second, 1.0 );

						//auxCoef.first = cit->second;
						//auxCoef.second = vit->second;

						//coeffList.push_back( auxCoef );
						//coeffListVal.push_back( 1.0 );
					}
					else
					{
						sprintf( name, "%s", c.toString().c_str() );
						nnz = 100;

						OPT_ROW row( nnz, OPT_ROW::LESS , 1.0, name );

						row.insert( vit->second, 1.0 );
						cHashOp[ c ] = lp->getNumRows();

						lp->addRow( row );
						restricoes++;
					}   
				} 
		  }
       }
   }

  // chgCoeffList( coeffList, coeffListVal );

   return restricoes;
}

int Operacional::criaRestricaoAlocAula()
{
   int restricoes = 0;

   int totalHorariosAula = ( (int)( this->problemData->horarios_aula_ordenados.size() ) );
   int totalProfessores = ( this->problemData->getProfessores().size() + 1 );
   int nnz = ( totalProfessores * totalHorariosAula );
   char name[ 200 ];

   ConstraintOp c;
   VariableOpHash::iterator vit;
   ConstraintOpHash::iterator cit;

   std::vector< std::pair< int, int > > coeffList;
   std::vector< double > coeffListVal;
   std::pair< int, int > auxCoef;

   vit = vHashOp.begin();

   for (; vit != vHashOp.end(); vit++ )
   {
      VariableOp v = vit->first;

      if ( v.getType() != VariableOp::V_X_PROF_AULA_HOR &&
		   v.getType() != VariableOp::V_FOLGA_DEMANDA )
      {
         continue;
      }

      c.reset();
      c.setType( ConstraintOp::C_ALOC_AULA );
      c.setAula( v.getAula() );

      cit = cHashOp.find( c );

      if ( cit != cHashOp.end() )
      {
         auxCoef.first = cit->second;
         auxCoef.second = vit->second;

         coeffList.push_back( auxCoef );
         coeffListVal.push_back( 1.0 );
      }
      else
      {
         sprintf( name, "%s", c.toString().c_str() );

         OPT_ROW row( nnz, OPT_ROW::EQUAL , 1.0, name );

         row.insert( vit->second, 1.0 );
         cHashOp[ c ] = lp->getNumRows();

         lp->addRow( row );
         restricoes++;
      }
   }

   chgCoeffList( coeffList, coeffListVal );

   return restricoes;
}

/*
	Obriga o total atendimento das aulas de uma turma ou nenhum atendimento (impede atendimento parcial).

	sum[a] fd_{a} = nroAulas_{i,d,cp} * ( 1 - sum[p] y_{p,i,d,cp} )   Para toda turma i, disciplina d, campus cp

	aonde a = aula da turma i, disciplina d, campus cp
	e nroAulas_{a} = numero total de aulas do trio (i,d,cp). Uma aula por dia somente (uma aula é uma sequência de créditos).

*/
int Operacional::criaRestricaoAtendimentoCompleto( void )
{
   int restricoes = 0;
   int nnz;
   char name[ 200 ];

   ConstraintOp c;
   VariableOpHash::iterator vit;
   ConstraintOpHash::iterator cit;
   
   std::map< int /*turma*/, std::map< Disciplina*, std::map< Campus*, std::map<int, VariableOp>, 
	   LessPtr<Campus> >, LessPtr<Disciplina> > > varsY;
   std::map< int /*turma*/, std::map< Disciplina*, std::map< Campus*, std::map<int, VariableOp>, 
	   LessPtr<Campus> >, LessPtr<Disciplina> > > varsFd;

   vit = vHashOp.begin();
   for (; vit != vHashOp.end(); vit++ )
   {
      VariableOp v = vit->first;	  
      if ( v.getType() == VariableOp::V_Y_PROF_DISCIPLINA )
      {
		  Disciplina *disciplina = v.getDisciplina();
		  int turma = v.getTurma();
		  Campus *campus = v.getCampus();

		  varsY[turma][disciplina][campus][vit->second] = v;
      }
	  else if ( v.getType() == VariableOp::V_FOLGA_DEMANDA )
      {
		  Disciplina *disciplina = v.getAula()->getDisciplina();
		  int turma = v.getAula()->getTurma();
		  Campus *campus = problemData->refCampus[ v.getAula()->getSala()->getIdCampus() ]; 
		  
		  varsFd[turma][disciplina][campus][vit->second] = v;
      }
   }

   std::map< int, std::map< Disciplina*, std::map< Campus*, std::map<int, VariableOp>, 
	   LessPtr<Campus> >, LessPtr<Disciplina> > >::iterator itTurmaFd;

   std::map< Disciplina*, std::map< Campus*, std::map<int, VariableOp>, 
		   LessPtr<Campus> >, LessPtr<Disciplina> >::iterator itDiscFd;

   std::map< Campus*, std::map<int, VariableOp>, LessPtr<Campus> >::iterator itCpFd;     
      
   std::map<int, VariableOp>::iterator itFd;
   std::map<int, VariableOp>::iterator itY;

   for ( itTurmaFd = varsFd.begin(); itTurmaFd != varsFd.end(); itTurmaFd++ )
   {
      int turma = itTurmaFd->first;
   
	   std::map< Disciplina*, std::map< Campus*, std::map<int, VariableOp>, 
		   LessPtr<Campus> >, LessPtr<Disciplina> > mapDiscFd = itTurmaFd->second;

	   for ( itDiscFd = mapDiscFd.begin(); itDiscFd != mapDiscFd.end(); itDiscFd++ )
	   {
		   Disciplina* disciplina = itDiscFd->first;

		   std::map< Campus*, std::map<int, VariableOp>, LessPtr<Campus> > mapCpFd = itDiscFd->second;

		   for ( itCpFd = mapCpFd.begin(); itCpFd != mapCpFd.end(); itCpFd++ )
		   {
				Campus* campus = itCpFd->first;

				std::map<int, VariableOp> mapFd = itCpFd->second;
				int nroAulas = mapFd.size();
			   
				std::map<int, VariableOp> mapY = varsY[turma][disciplina][campus];
				int nroY = mapY.size();

				c.reset();
				c.setType( ConstraintOp::C_PROF_ATEND_COMPLETO );
				c.setDisciplina( disciplina );
				c.setTurma( turma );
				c.setCampus( campus );

				cit = cHashOp.find( c );

				if ( cit == cHashOp.end() )
				{					
					sprintf( name, "%s", c.toString().c_str() );
					nnz = nroAulas + nroY;

					OPT_ROW row( nnz, OPT_ROW::EQUAL, nroAulas, name );

					double coefFd = 1.0;
					double coefY = nroAulas;
			   
					for ( itFd = mapFd.begin(); itFd != mapFd.end(); itFd++ )
					{
						row.insert( itFd->first, coefFd );
					}
					for ( itY = mapY.begin(); itY != mapY.end(); itY++ )
					{
						row.insert( itY->first, coefY );
					}

					cHashOp[ c ] = lp->getNumRows();

					lp->addRow( row );
					restricoes++;					
				}

		   }
	   }
   }
   
   return restricoes;
}


int Operacional::criaRestricaoProfessorDisciplinaUnico()
{
   int restricoes = 0;
   int nnz;
   char name[ 200 ];

   ConstraintOp c;
   VariableOpHash::iterator vit;
   ConstraintOpHash::iterator cit;

   std::vector< std::pair< int, int > > coeffList;
   std::vector< double > coeffListVal;
   std::pair< int, int > auxCoef;

   vit = vHashOp.begin();

   for (; vit != vHashOp.end(); vit++ )
   {
      VariableOp v = vit->first;

      if ( v.getType() != VariableOp::V_Y_PROF_DISCIPLINA )
      {
         continue;
      }

      c.reset();
      c.setType( ConstraintOp::C_PROF_DISC_UNI );
      c.setDisciplina( v.getDisciplina() );
      c.setTurma( v.getTurma() );
	  c.setCampus( v.getCampus() );

      cit = cHashOp.find( c );

      if ( cit != cHashOp.end() )
      {
         auxCoef.first = cit->second;
         auxCoef.second = vit->second;

         coeffList.push_back( auxCoef );
         coeffListVal.push_back( 1.0 );
      }
      else
      {
         sprintf( name, "%s", c.toString().c_str() );
         nnz = 100;

         OPT_ROW row( nnz, OPT_ROW::LESS , 1.0, name );

         row.insert( vit->second, 1.0 );
         cHashOp[ c ] = lp->getNumRows();

         lp->addRow( row );
         restricoes++;
      }
   }

   chgCoeffList( coeffList, coeffListVal );

   return restricoes;
}

int Operacional::criaRestricaoProfessorDisciplinaUnicoPT()
{
   int restricoes = 0;
   int nnz;
   char name[ 200 ];

   ConstraintOp c;
   VariableOpHash::iterator vit;
   ConstraintOpHash::iterator cit;

   std::vector< std::pair< int, int > > coeffList;
   std::vector< double > coeffListVal;
   std::pair< int, int > auxCoef;

   std::map< Professor*, std::map< Disciplina*, std::map<int, VariableOp>, LessPtr<Disciplina> >, LessPtr<Professor> > mapProfDiscVars;

   vit = vHashOp.begin();
   for (; vit != vHashOp.end(); vit++ )
   {
      VariableOp v = vit->first;
      if ( v.getType() != VariableOp::V_Y_PROF_DISCIPLINA )
      {
         continue;
      }

	  Disciplina *disciplina = v.getDisciplina();
	  
	  // Restrição só para disciplinas com divisão pratica/teorica
	  if ( disciplina->getId() > 0 )
		  if ( problemData->getDisciplinaTeorPrat( disciplina ) == NULL )
		  	  continue;
	     
	  if ( disciplina->getProfUnicoDiscPT() )
	  {
		  mapProfDiscVars[v.getProfessor()][disciplina][vit->second] = v;
	  }
   }

   std::map< Professor*, std::map< Disciplina*, std::map<int, VariableOp>, LessPtr<Disciplina> >, LessPtr<Professor> >::iterator
	   itMapProfDiscVars = mapProfDiscVars.begin();
   for ( ; itMapProfDiscVars != mapProfDiscVars.end(); itMapProfDiscVars++ )
   {
	   Professor *professor = itMapProfDiscVars->first;

	   std::map< Disciplina*, std::map<int, VariableOp>, LessPtr<Disciplina> > mapDiscVars = itMapProfDiscVars->second;
	   std::map< Disciplina*, std::map<int, VariableOp>, LessPtr<Disciplina> >::iterator
	   itMapDiscVars = mapDiscVars.begin();
	   for ( ; itMapDiscVars != mapDiscVars.end(); itMapDiscVars++ )
	   {
		   if ( itMapDiscVars->first->getId() < 0 ) continue;

		   Disciplina* discT = itMapDiscVars->first;;
		   Disciplina* discP = problemData->getDisciplinaTeorPrat( discT );
	   
		   std::map<int, VariableOp> varsT = itMapDiscVars->second;
		   std::map<int, VariableOp> varsP = mapProfDiscVars[professor][discP];

		   std::map<int, VariableOp>::iterator itVarT = varsT.begin();
		   for ( ; itVarT != varsT.end(); itVarT++ )
		   {
			   int ind_t = itVarT->first;
			   VariableOp y_t = itVarT->second;

			   std::map<int, VariableOp>::iterator itVarP = varsP.begin();
			   for ( ; itVarP != varsP.end(); itVarP++ )
			   {
				   int ind_p = itVarP->first;
				   VariableOp y_p = itVarP->second;

				   // TODO: campus
				   if ( ! problemData->possuiAlunosEmComum( y_t.getTurma(), y_t.getDisciplina(), y_t.getCampus(), y_p.getTurma(), y_p.getDisciplina(), y_p.getCampus() ) )
					   continue;

				   c.reset();
				   c.setType( ConstraintOp::C_PROF_DISC_PT_UNICO );
				   c.setDisciplina( discT );
				   c.setTurmaT( y_t.getTurma() );
				   c.setTurmaP( y_p.getTurma() );
				   c.setCampusT( y_t.getCampus() );
				   c.setCampusP( y_p.getCampus() );
				   c.setProfessor( professor );
			   
				   cit = cHashOp.find( c );
				   if ( cit == cHashOp.end() )
				   {
						 sprintf( name, "%s", c.toString().c_str() );
						 nnz = 100;

						 OPT_ROW row( nnz, OPT_ROW::EQUAL , 0.0, name );

						 row.insert( ind_t, 1.0 );
						 row.insert( ind_p, -1.0 );

						 cHashOp[ c ] = lp->getNumRows();

						 lp->addRow( row );
						 restricoes++;
				   }
			   }
		   }
	   }
   }
   
   return restricoes;
}


int Operacional::criaRestricaoProfessorDisciplina()
{
   int restricoes = 0;
   int nnz;
   char name[ 200 ];

   ConstraintOp c;
   VariableOpHash::iterator vit;
   ConstraintOpHash::iterator cit;

   std::vector< std::pair< int, int > > coeffList;
   std::vector< double > coeffListVal;
   std::pair< int, int > auxCoef;

   vit = vHashOp.begin();

   for (; vit != vHashOp.end(); vit++ )
   {
      VariableOp v = vit->first;

	  Campus *campus;

      if ( v.getType() == VariableOp::V_Y_PROF_DISCIPLINA )
      {
		  campus = v.getCampus();         
      }
	  else if ( v.getType() == VariableOp::V_X_PROF_AULA_HOR  )
	  {
		  campus = problemData->refCampus[ v.getUnidade()->getIdCampus() ];         
	  }
	  else
	  {
		  continue;
	  }

      if ( v.getType() == VariableOp::V_Y_PROF_DISCIPLINA )
      {
         c.reset();
         c.setType( ConstraintOp::C_PROF_DISC );
         c.setProfessor( v.getProfessor() );
         c.setDisciplina( v.getDisciplina() );
         c.setTurma( v.getTurma() );
		 c.setCampus( campus );

         cit = cHashOp.find( c );
		 		 
		 int nroAulas = problemData->getMapAulas( campus->getId(), v.getTurma(), v.getDisciplina() ).size();
			 
		 if ( cit != cHashOp.end() )
         {
            auxCoef.first = cit->second;
            auxCoef.second = vit->second;

            coeffList.push_back( auxCoef );
            coeffListVal.push_back( -nroAulas );
         }
         else
         {
            sprintf( name, "%s", c.toString().c_str() );
            nnz = 100;

            OPT_ROW row( nnz, OPT_ROW::EQUAL , 0.0, name );

            row.insert( vit->second, -nroAulas );
            cHashOp[ c ] = lp->getNumRows();

            lp->addRow( row );
            restricoes++;
         }   
      }
      else
      {
         c.reset();
         c.setType( ConstraintOp::C_PROF_DISC );
         c.setProfessor( v.getProfessor() );
         c.setDisciplina( v.getAula()->getDisciplina() );
         c.setTurma( v.getAula()->getTurma() );
		 c.setCampus( campus );

         cit = cHashOp.find( c );

         if ( cit != cHashOp.end() )
         {
            auxCoef.first = cit->second;
            auxCoef.second = vit->second;

            coeffList.push_back( auxCoef );
            coeffListVal.push_back( 1.0 );
         }
         else
         {
            sprintf( name, "%s", c.toString().c_str() );
            nnz = 100;

            OPT_ROW row( nnz, OPT_ROW::EQUAL , 0.0, name );

            row.insert( vit->second, 1.0 );
            cHashOp[ c ] = lp->getNumRows();

            lp->addRow( row );
            restricoes++;
         }   
      }
   }

   chgCoeffList( coeffList, coeffListVal );

   return restricoes;
}

/*
	sum[p] sum[pv] sum[u] sum[s] x_{p,i,d,u,s,t,h} + x_{pv,i,d,u,s,t,h} <= z_{i,d,h}

	forall d \in D
	forall i \in I_{d}
	forall t \in T
	forall h \in H
*/
int Operacional::criaRestricaoDisciplinaMesmoHorario()
{
   int restricoes = 0;
   int nnz;
   char name[ 200 ];

   ConstraintOp c;
   VariableOpHash::iterator vit;
   ConstraintOpHash::iterator cit;

   std::vector< std::pair< int, int > > coeffList;
   std::vector< double > coeffListVal;
   std::pair< int, int > auxCoef;

   vit = vHashOp.begin();

   for (; vit != vHashOp.end(); vit++ )
   {
      VariableOp v = vit->first;

      if ( v.getType() != VariableOp::V_X_PROF_AULA_HOR )
      {
         continue;
      }

      c.reset();
      c.setType( ConstraintOp::C_DISC_HORARIO );
      c.setDisciplina( v.getDisciplina() );
      c.setTurma( v.getTurma() );
      c.setHorarioAula( v.getHorario()->getHorarioAula() );
      c.setDia( v.getHorario()->getDia() );
	  c.setCampus( problemData->refCampus[ v.getUnidade()->getIdCampus() ] );

      cit = cHashOp.find( c );

      if ( cit != cHashOp.end() )
      {
         auxCoef.first = cit->second;
         auxCoef.second = vit->second;
         coeffList.push_back( auxCoef );
         coeffListVal.push_back( 1.0 );
      }
      else
      {
         sprintf( name, "%s", c.toString().c_str() );
         nnz = 100;

         OPT_ROW row( nnz, OPT_ROW::LESS , 0.0, name );

         row.insert( vit->second, 1.0 );
         cHashOp[ c ] = lp->getNumRows();

         lp->addRow( row );
         restricoes++;
      }   
   }

   vit = vHashOp.begin();

   for (; vit != vHashOp.end(); vit++ )
   {
      VariableOp v = vit->first;

      if ( v.getType() == VariableOp::V_Z_DISCIPLINA_HOR )
      {
         c.reset();
         c.setType( ConstraintOp::C_DISC_HORARIO );
         c.setDisciplina( v.getDisciplina() );
         c.setTurma( v.getTurma() );
         c.setHorarioAula( v.getHorarioAula() );
		 c.setCampus( v.getCampus() );

         for ( int dia = 0; dia <= 7; dia++ )
         {
            c.setDia( dia );

            cit = cHashOp.find( c );

            if ( cit != cHashOp.end() )
            {
               auxCoef.first = cit->second;
               auxCoef.second = vit->second;

               coeffList.push_back( auxCoef );
               coeffListVal.push_back( -1.0 );
            }
         }
      }
   }

   chgCoeffList( coeffList, coeffListVal );

   return restricoes;
}

/*
	Usada quando o parametro carga_horaria_semanal_prof é CONCENTRAR a carga horária durante semana,
	ou seja, MINIMIZAR o número de dias de aulas do professor.
*/
int Operacional::criaRestricaoCalculaDiasProfMinistra_Min()
{
	int restricoes = 0;
   
	if ( problemData->parametros->carga_horaria_semanal_prof != ParametrosPlanejamento::CHS::MINIMIZAR_DIAS &&
		!problemData->parametros->considerarMaxDiasPorProf &&
		!problemData->parametros->minCredsDiariosPorProf )
    {
	    return restricoes;
    }
   
	int nnz = 100;
	char name[ 200 ];

	ConstraintOp c;
	VariableOpHash::iterator vit;
	ConstraintOpHash::iterator cit;

	map< ConstraintOp, int > numVars;

	vit = vHashOp.begin();

	while(vit != vHashOp.end())
	{
		VariableOp vOp = ( vit->first );

		if (vOp.getType() != VariableOp::V_X_PROF_AULA_HOR)
		{
			vit++;
			continue;
		}

		if ( vOp.getProfessor()->eVirtual() && this->getRodada() == OP_VIRTUAL_PERFIL )
        {
           vit++;
           continue;
        }

		Professor *professor = vOp.getProfessor();
		int dia = vOp.getHorario()->getDia();

		c.reset();
		c.setType( ConstraintOp::C_DIAS_PROF_MINISTRA_AULA_MIN );
		c.setProfessor( professor );
		c.setDia( dia );

		cit = cHashOp.find(c);
		if ( cit == cHashOp.end() )
		{
			cHashOp[ c ] = lp->getNumRows();

			sprintf( name, "%s", c.toString().c_str() );
			OPT_ROW row( nnz, OPT_ROW::LESS , 0.0, name );
			row.insert( vit->second, 1.0 );
			lp->addRow( row );
			restricoes++;
			numVars[ c ] = 1;
		}
		else
		{
			lp->chgCoef(cit->second, vit->second, 1.0);
			numVars[c]++;
		}

		vit++;
	}

	vit = vHashOp.begin();

	while(vit != vHashOp.end())
	{
		VariableOp vOp = ( vit->first );

		if (vOp.getType() != VariableOp::V_DIAS_PROF_MINISTRA_AULAS)
		{
			vit++;
			continue;
		}
		
		if ( vOp.getProfessor()->eVirtual() && this->getRodada() == OP_VIRTUAL_PERFIL )
        {
           vit++;
           continue;
        }

		Professor *professor = vOp.getProfessor();
		int dia = vOp.getDia();

		c.reset();
		c.setType( ConstraintOp::C_DIAS_PROF_MINISTRA_AULA_MIN );
		c.setProfessor( professor );
		c.setDia( dia );

		cit = cHashOp.find(c);
		if ( cit != cHashOp.end() )
		{
			lp->chgCoef(cit->second, vit->second, -1*numVars[ c ]);
		}

		vit++;
	}

   return restricoes;
}

/*
	Usada quando o parametro carga_horaria_semanal_prof é DISTRIBUIR as aulas durante a semana,
	ou seja, maximizar o número de dias de aulas do professor.
*/
int Operacional::criaRestricaoCalculaDiasProfMinistra_Max()
{
	int restricoes = 0;
   
	if ( problemData->parametros->carga_horaria_semanal_prof != ParametrosPlanejamento::CHS::EQUILIBRAR )
    {
	    return restricoes;
    }
   
	int nnz = 100;
	char name[ 200 ];

	ConstraintOp c;
	VariableOpHash::iterator vit;
	ConstraintOpHash::iterator cit;

	map< ConstraintOp, int > numVars;

	vit = vHashOp.begin();

	while(vit != vHashOp.end())
	{
		VariableOp vOp = ( vit->first );

		if (vOp.getType() != VariableOp::V_X_PROF_AULA_HOR)
		{
			vit++;
			continue;
		}

		if ( vOp.getProfessor()->eVirtual() && this->getRodada() == OP_VIRTUAL_PERFIL )
        {
           vit++;
           continue;
        }

		Professor *professor = vOp.getProfessor();
		int dia = vOp.getHorario()->getDia();

		c.reset();
		c.setType( ConstraintOp::C_DIAS_PROF_MINISTRA_AULA_MAX );
		c.setProfessor( professor );
		c.setDia( dia );

		cit = cHashOp.find(c);
		if ( cit == cHashOp.end() )
		{
			cHashOp[ c ] = lp->getNumRows();

			sprintf( name, "%s", c.toString().c_str() );
			OPT_ROW row( nnz, OPT_ROW::GREATER , 0.0, name );
			row.insert( vit->second, 1.0 );
			lp->addRow( row );
			restricoes++;
			numVars[ c ] = 1;
		}
		else
		{
			lp->chgCoef(cit->second, vit->second, 1.0);
			numVars[c]++;
		}

		vit++;
	}

	vit = vHashOp.begin();

	while(vit != vHashOp.end())
	{
		VariableOp vOp = ( vit->first );

		if (vOp.getType() != VariableOp::V_DIAS_PROF_MINISTRA_AULAS)
		{
			vit++;
			continue;
		}
		
		if ( vOp.getProfessor()->eVirtual() && this->getRodada() == OP_VIRTUAL_PERFIL )
        {
           vit++;
           continue;
        }

		Professor *professor = vOp.getProfessor();
		int dia = vOp.getDia();

		c.reset();
		c.setType( ConstraintOp::C_DIAS_PROF_MINISTRA_AULA_MAX );
		c.setProfessor( professor );
		c.setDia( dia );

		cit = cHashOp.find(c);
		if ( cit != cHashOp.end() )
		{
			lp->chgCoef(cit->second, vit->second, -1);
		}

		vit++;
	}

   return restricoes;
}


int Operacional::criaRestricaoMaxDiasSemanaProf()
{
	int restricoes = 0;
   
	if ( problemData->parametros->carga_horaria_semanal_prof != ParametrosPlanejamento::CHS::MINIMIZAR_DIAS &&
		!problemData->parametros->considerarMaxDiasPorProf )
    {
	    return restricoes;
    }
   
	int nnz = 100;
	char name[ 200 ];

	ConstraintOp c;
	VariableOpHash::iterator vit;
	ConstraintOpHash::iterator cit;

	vit = vHashOp.begin();
	while(vit != vHashOp.end())
	{
		VariableOp vOp = ( vit->first );

		if (vOp.getType() != VariableOp::V_DIAS_PROF_MINISTRA_AULAS)
		{
			vit++;
			continue;
		}

		if ( vOp.getProfessor()->eVirtual() && this->getRodada() == OP_VIRTUAL_PERFIL )
        {
           vit++;
           continue;
        }

		Professor *professor = vOp.getProfessor();
		
		c.reset();
		c.setType( ConstraintOp::C_MAX_DIAS_SEMANA_PROF );
		c.setProfessor( professor );
		
		cit = cHashOp.find(c);
		if ( cit == cHashOp.end() )
		{
			double rhs = professor->getMaxDiasSemana();

			cHashOp[ c ] = lp->getNumRows();

			sprintf( name, "%s", c.toString().c_str() );
			OPT_ROW row( nnz, OPT_ROW::LESS , rhs, name );
			row.insert( vit->second, 1.0 );
			lp->addRow( row );
			restricoes++;
		}
		else
		{
			lp->chgCoef(cit->second, vit->second, 1.0);
		}

		vit++;
	}

   return restricoes;
}


int Operacional::criaRestricaoMinCredDiariosProf()
{
	int restricoes = 0;
   
	if ( ! problemData->parametros->minCredsDiariosPorProf )
    {
	    return restricoes;
    }
   
	int nnz = 100;
	char name[ 200 ];

	ConstraintOp c;
	VariableOpHash::iterator vit;
	ConstraintOpHash::iterator cit;
	
	vit = vHashOp.begin();
	while(vit != vHashOp.end())
	{
		VariableOp vOp = ( vit->first );
		
		double coef = 0.0;

		if ( vOp.getType() == VariableOp::V_X_PROF_AULA_HOR )
		{
			coef = vOp.getAula()->getTotalCreditos();
		}
		else if ( vOp.getType() == VariableOp::V_DIAS_PROF_MINISTRA_AULAS )
		{
			coef = - vOp.getProfessor()->getMinCredsDiarios();
		}
		else
		{ 
			vit++; continue; 
		}

		if ( vOp.getProfessor()->eVirtual() )
        {
           vit++;
           continue;
        }

		Professor *professor = vOp.getProfessor();
		int dia = vOp.getDia();

		c.reset();
		c.setType( ConstraintOp::C_MIN_CREDS_DIARIOS_PROF );
		c.setProfessor( professor );
		c.setDia( dia );

		cit = cHashOp.find(c);
		if ( cit == cHashOp.end() )
		{
			cHashOp[ c ] = lp->getNumRows();

			sprintf( name, "%s", c.toString().c_str() );
			OPT_ROW row( nnz, OPT_ROW::GREATER , 0.0, name );
			row.insert( vit->second, coef );
			lp->addRow( row );
			restricoes++;
		}
		else
		{
			lp->chgCoef(cit->second, vit->second, coef);
		}

		vit++;
	}

   return restricoes;
}

int Operacional::criaRestricaoDisciplinaHorarioUnico()
{
   int restricoes = 0;
   int nnz;
   char name[ 200 ];

   ConstraintOp c;
   VariableOpHash::iterator vit;
   ConstraintOpHash::iterator cit;

   std::vector< std::pair< int, int > > coeffList;
   std::vector< double > coeffListVal;
   std::pair< int, int > auxCoef;

   vit = vHashOp.begin();

   for (; vit != vHashOp.end(); vit++ )
   {
      VariableOp v = vit->first;

      if ( v.getType() != VariableOp::V_Z_DISCIPLINA_HOR &&
		   v.getType() != VariableOp::V_FOLGA_DISCIPLINA_HOR )
      {
         continue;
      }

      c.reset();
      c.setType( ConstraintOp::C_DISC_HORARIO_UNICO );
      c.setDisciplina( v.getDisciplina() );
      c.setTurma( v.getTurma() );
	  c.setCampus( v.getCampus() );

      cit = cHashOp.find( c );

      if ( cit != cHashOp.end() )
      {
         auxCoef.first = cit->second; //constraint
         auxCoef.second = vit->second; //variable

         coeffList.push_back( auxCoef );

		 if (  v.getType() == VariableOp::V_Z_DISCIPLINA_HOR )
		 {
			coeffListVal.push_back( 1.0 );
		 }

		 else if (v.getType() == VariableOp::V_FOLGA_DISCIPLINA_HOR)
		 {
			coeffListVal.push_back( -1.0 );
		 }
      }
      else
      {
         sprintf( name, "%s", c.toString().c_str() );
         nnz = 100;

         OPT_ROW row( nnz, OPT_ROW::LESS , 1.0, name );

		 if (  v.getType() == VariableOp::V_Z_DISCIPLINA_HOR )
		 {
			row.insert( vit->second, 1.0 );		 
		 }

		 else if (v.getType() == VariableOp::V_FOLGA_DISCIPLINA_HOR)
		 {
			row.insert( vit->second, -1.0 );
		 }
		 
         cHashOp[ c ] = lp->getNumRows();

         lp->addRow( row );
         restricoes++;
      }   
   }

   chgCoeffList( coeffList, coeffListVal );

   return restricoes;
}

// RODADAS 1 E 2
int Operacional::criaRestricaoMinimoMestresCurso()
{
   int restricoes = 0;
      
   if ( ! problemData->parametros->min_mestres )
   {
		return restricoes;
   }

   int nnz;
   double rhs;
   char name[ 200 ];

   ConstraintOp c;
   VariableOpHash::iterator vit;
   ConstraintOpHash::iterator cit;

   GGroup< Professor *, LessPtr< Professor > > professores
      = problemData->getProfessores();

   // Agrupando os professores que ministram disciplinas de cada curso
   std::map< Curso *, GGroup< Professor *,
      LessPtr< Professor > > > mapCursoProfessores;

   ITERA_GGROUP_LESSPTR( it_aula, problemData->aulas, Aula )
   {
         Aula * aula = ( *it_aula );
         Disciplina * disciplina = aula->getDisciplina();
		 int dia = aula->getDiaSemana();

         ITERA_GGROUP_LESSPTR( it_oferta, aula->ofertas, Oferta )
         {
            Oferta * oferta = ( *it_oferta );

			if ( oferta->curso->regra_min_mestres.second > 0 )
			{
				 // Garante que vai haver a restrição para o curso, mesmo que não haja professor real para o mesmo
				 GGroup< Professor *, LessPtr< Professor > > vazio;				 
				 std::map< Curso*, GGroup< Professor*, LessPtr< Professor > > >::iterator
					 itFinder = mapCursoProfessores.find( oferta->curso );
				 if ( itFinder == mapCursoProfessores.end() )
				 {
					mapCursoProfessores[ oferta->curso ] = vazio;
				 }

				 // Adiciona os professores reais aptos a ministrar a disciplina
				 ITERA_GGROUP_LESSPTR( it_prof, professores, Professor )
				 {
					 Professor * professor = ( *it_prof );
	  
					 if (professor->eVirtual()) continue;
			 
					 std::pair< int, int > professor_disciplina ( professor->getId(), disciplina->getId() );

					 // Se o professor e a disciplina da aula em questão se relacionarem
					 std::map< std::pair< int /*idProf*/, int /*idDisc*/>, GGroup< int > /*Dias*/ >::iterator
						 ifFinderDD = problemData->prof_Disc_Dias.find( professor_disciplina );
					 if ( ifFinderDD != problemData->prof_Disc_Dias.end() )
					 if ( ifFinderDD->second.find( dia ) != ifFinderDD->second.end() )
					 {
						 mapCursoProfessores[ oferta->curso ].add( professor );
					 }
				 }
			}
        }
   }

   std::map< Curso *, GGroup< Professor *, LessPtr< Professor > > >::iterator
      it_map = mapCursoProfessores.begin();

   for (; it_map != mapCursoProfessores.end();
          it_map++ )
   {
        Curso * curso = it_map->first;

		c.reset();
		c.setType( ConstraintOp::C_MIN_MEST_CURSO );
		c.setCurso( curso );

		cit = cHashOp.find( c );

		if ( cit == cHashOp.end() )
		{
			GGroup< Professor *, LessPtr< Professor > > professores_curso
				= it_map->second;

			sprintf( name, "%s", c.toString().c_str() );
			nnz = professores_curso.size();
			rhs = 0.0;

			OPT_ROW row( nnz, OPT_ROW::GREATER, rhs, name );
				  
			// professores reais
			ITERA_GGROUP_LESSPTR( it_prof, professores_curso, Professor )
			{
				Professor * professor = ( *it_prof );
				
				// Recupera os professores que estão associados ao curso, e que são no minimo mestres
				VariableOpHash::iterator vit_find = vHashOp.begin();

				for (; vit_find != vHashOp.end(); vit_find++ )
				{
				   VariableOp v_find = vit_find->first;

				   if ( v_find.getType() == VariableOp::V_PROF_CURSO
					  && v_find.getCurso() == curso
					  && v_find.getProfessor() == professor
					  && ( professor->getTitulacao() == TipoTitulacao::Mestre || professor->getTitulacao() == TipoTitulacao::Doutor ) )
				   {
					  row.insert( vit_find->second, 1.0 );
				   }
				}
			}

			// Variavel npvm_{c,r}
			ITERA_GGROUP_LESSPTR( it_contr, problemData->tipos_contrato, TipoContrato )
			{
				VariableOp v;
				v.reset();
				v.setType( VariableOp::V_NRO_PROFS_VIRTUAIS_MEST_CURSO );
				v.setContrato( *it_contr );
				v.setCurso( curso );

				vit = vHashOp.find( v );
				if ( vit != vHashOp.end() )
				{
					row.insert( vit->second, 1.0 );
				}
			}

			// Variavel npvd_{c,r}
			ITERA_GGROUP_LESSPTR( it_contr, problemData->tipos_contrato, TipoContrato )
			{
				VariableOp v;
				v.reset();
				v.setType( VariableOp::V_NRO_PROFS_VIRTUAIS_DOUT_CURSO );
				v.setContrato( *it_contr );
				v.setCurso( curso );

				vit = vHashOp.find( v );
				if ( vit != vHashOp.end() )
				{
					row.insert( vit->second, 1.0 );
				}
			}
						
			double minPercMestre = ( curso->regra_min_mestres.second / 100.0 );

			// Variavel np_{c}			
			VariableOp v;
			v.reset();
			v.setType( VariableOp::V_NRO_PROFS_CURSO );
			v.setCurso( curso );
			vit = vHashOp.find( v );

			if ( vit != vHashOp.end() )
			{
				row.insert( vit->second, -minPercMestre );
			}

			// Variavel npv_{c,r}
			ITERA_GGROUP_LESSPTR( it_contr, problemData->tipos_contrato, TipoContrato )
			{
				VariableOp v;
				v.reset();
				v.setType( VariableOp::V_NRO_PROFS_VIRTUAIS_CURSO );
				v.setContrato( *it_contr );
				v.setCurso( curso );

				vit = vHashOp.find( v );
				if ( vit != vHashOp.end() )
				{
					row.insert( vit->second, -minPercMestre );
				}
			}

			if ( row.getnnz() != 0 )
			{
				cHashOp[ c ] = lp->getNumRows();
				lp->addRow( row );
				restricoes++;
			}
        }
   }

   return restricoes;
}

// RODADAS 1 E 2
int Operacional::criaRestricaoMinimoDoutoresCurso()
{
   int restricoes = 0;
      
   if ( ! problemData->parametros->min_doutores )
   {
		return restricoes;
   }

   int nnz;
   double rhs;
   char name[ 200 ];

   ConstraintOp c;
   VariableOpHash::iterator vit;
   ConstraintOpHash::iterator cit;

   GGroup< Professor *, LessPtr< Professor > > professores
      = problemData->getProfessores();

   // Agrupando os professores que ministram disciplinas de cada curso
   std::map< Curso *, GGroup< Professor *,
      LessPtr< Professor > > > mapCursoProfessores;

   ITERA_GGROUP_LESSPTR( it_aula, problemData->aulas, Aula )
   {
         Aula * aula = ( *it_aula );
         Disciplina * disciplina = aula->getDisciplina();
		 int dia = aula->getDiaSemana();

         ITERA_GGROUP_LESSPTR( it_oferta, aula->ofertas, Oferta )
         {
            Oferta * oferta = ( *it_oferta );

			if ( oferta->curso->regra_min_doutores.second > 0 )
			{
				 // Garante que vai haver a restrição para o curso, mesmo que não haja professor real para o mesmo
				 GGroup< Professor *, LessPtr< Professor > > vazio;				 
				 std::map< Curso*, GGroup< Professor*, LessPtr< Professor > > >::iterator
					 itFinder = mapCursoProfessores.find( oferta->curso );
				 if ( itFinder == mapCursoProfessores.end() )
				 {
					mapCursoProfessores[ oferta->curso ] = vazio;
				 }

				 // Adiciona os professores reais aptos a ministrar a disciplina
				 ITERA_GGROUP_LESSPTR( it_prof, professores, Professor )
				 {
					 Professor * professor = ( *it_prof );
	  
					 if (professor->eVirtual()) continue;
			 
					 std::pair< int, int > professor_disciplina ( professor->getId(), disciplina->getId() );

					 // Se o professor e a disciplina da aula em questão se relacionarem
					 std::map< std::pair< int /*idProf*/, int /*idDisc*/>, GGroup< int > /*Dias*/ >::iterator
						 ifFinderDD = problemData->prof_Disc_Dias.find( professor_disciplina );
					 if ( ifFinderDD != problemData->prof_Disc_Dias.end() )
					 if ( ifFinderDD->second.find( dia ) != ifFinderDD->second.end() )
					 {
						 mapCursoProfessores[ oferta->curso ].add( professor );
					 }
				 }
			}
        }
   }

   std::map< Curso *, GGroup< Professor *, LessPtr< Professor > > >::iterator
      it_map = mapCursoProfessores.begin();

   for (; it_map != mapCursoProfessores.end();
          it_map++ )
   {
        Curso * curso = it_map->first;

		c.reset();
		c.setType( ConstraintOp::C_MIN_DOUT_CURSO );
		c.setCurso( curso );

		cit = cHashOp.find( c );

		if ( cit == cHashOp.end() )
		{			
			GGroup< Professor *, LessPtr< Professor > > professores_curso
				= it_map->second;

			sprintf( name, "%s", c.toString().c_str() );
			nnz = professores_curso.size();
			rhs = 0;

			OPT_ROW row( nnz, OPT_ROW::GREATER, rhs, name );

			// professores reais
			ITERA_GGROUP_LESSPTR( it_prof, professores_curso, Professor )
			{
				Professor * professor = ( *it_prof );

				// Recupera os professores que estão associados ao curso, e que são doutores
				VariableOpHash::iterator vit_find = vHashOp.begin();

				for (; vit_find != vHashOp.end(); vit_find++ )
				{
				   VariableOp v_find = vit_find->first;

				   if ( v_find.getType() == VariableOp::V_PROF_CURSO
					  && v_find.getCurso() == curso
					  && v_find.getProfessor() == professor
					  && professor->getTitulacao() >= TipoTitulacao::Doutor )
				   {
					  row.insert( vit_find->second, 1.0 );
				   }
				}
			}

			// Variavel npvd_{c,r}
			ITERA_GGROUP_LESSPTR( it_contr, problemData->tipos_contrato, TipoContrato )
			{
				VariableOp v;
				v.reset();
				v.setType( VariableOp::V_NRO_PROFS_VIRTUAIS_DOUT_CURSO );
				v.setContrato( *it_contr );
				v.setCurso( curso );

				vit = vHashOp.find( v );
				if ( vit != vHashOp.end() )
				{
					row.insert( vit->second, 1.0 );
				}
			}
			
			double minPercDoutor = ( curso->regra_min_doutores.second / 100.0 );
        
			// Variavel np_{c}
			VariableOp v;
			v.reset();
			v.setType( VariableOp::V_NRO_PROFS_CURSO );
			v.setCurso( curso );
			vit = vHashOp.find( v );

			if ( vit != vHashOp.end() )
			{
				row.insert( vit->second, -minPercDoutor );
			}	

			// Variavel npv_{c,r}
			ITERA_GGROUP_LESSPTR( it_contr, problemData->tipos_contrato, TipoContrato )
			{
				VariableOp v;
				v.reset();
				v.setType( VariableOp::V_NRO_PROFS_VIRTUAIS_CURSO );
				v.setContrato( *it_contr );
				v.setCurso( curso );

				vit = vHashOp.find( v );
				if ( vit != vHashOp.end() )
				{
					row.insert( vit->second, -minPercDoutor );
				}
			}

			
			if ( row.getnnz() != 0 )
			{
				cHashOp[ c ] = lp->getNumRows();
				lp->addRow( row );
				restricoes++;
			}
		}
   }

   return restricoes;
}

// RODADAS 1 E 2
int Operacional::criaRestricaoMaximoNaoMestresCurso()
{
   int restricoes = 0;
      
   if ( ! problemData->parametros->min_mestres )
   {
		return restricoes;
   }

   int nnz;
   double rhs;
   char name[ 200 ];

   ConstraintOp c;
   VariableOpHash::iterator vit;
   ConstraintOpHash::iterator cit;

   GGroup< Professor *, LessPtr< Professor > > professores
      = problemData->getProfessores();

   // Agrupando os professores que ministram disciplinas de cada curso
   std::map< Curso *, GGroup< Professor *,
      LessPtr< Professor > > > mapCursoProfessores;
   
   ITERA_GGROUP_LESSPTR( it_aula, problemData->aulas, Aula )
   {
         Aula * aula = ( *it_aula );
         Disciplina * disciplina = aula->getDisciplina();
		 int dia = aula->getDiaSemana();

         ITERA_GGROUP_LESSPTR( it_oferta, aula->ofertas, Oferta )
         {
            Oferta * oferta = ( *it_oferta );

			if ( oferta->curso->regra_min_mestres.second > 0 )
			{
				 // Garante que vai haver a restrição para o curso, mesmo que não haja professor real para o mesmo
				 GGroup< Professor *, LessPtr< Professor > > vazio;				 
				 std::map< Curso*, GGroup< Professor*, LessPtr< Professor > > >::iterator
					 itFinder = mapCursoProfessores.find( oferta->curso );
				 if ( itFinder == mapCursoProfessores.end() )
				 {
					mapCursoProfessores[ oferta->curso ] = vazio;
				 }

				 // Adiciona os professores reais aptos a ministrar a disciplina
				 ITERA_GGROUP_LESSPTR( it_prof, professores, Professor )
				 {
					 Professor * professor = ( *it_prof );
	  
					 if (professor->eVirtual()) continue;
			 
					 std::pair< int, int > professor_disciplina ( professor->getId(), disciplina->getId() );
			 
					 // Se o professor e a disciplina da aula em questão se relacionarem
					 std::map< std::pair< int /*idProf*/, int /*idDisc*/>, GGroup< int > /*Dias*/ >::iterator
						 ifFinderDD = problemData->prof_Disc_Dias.find( professor_disciplina );
					 if ( ifFinderDD != problemData->prof_Disc_Dias.end() )
					 if ( ifFinderDD->second.find( dia ) != ifFinderDD->second.end() )
					 {
						 mapCursoProfessores[ oferta->curso ].add( professor );
					 }
				 }
			}
        }
   }


   std::map< Curso *, GGroup< Professor *, LessPtr< Professor > > >::iterator
      it_map = mapCursoProfessores.begin();

   for (; it_map != mapCursoProfessores.end();
          it_map++ )
   {
        Curso * curso = it_map->first;
		
		bool debuging = false;
		if ( curso->getId()==1100 )
			debuging = false;
		
		if ( debuging )
		std::cout << "\nCurso " << curso->getId();

		c.reset();
		c.setType( ConstraintOp::C_MAX_NAO_MEST_CURSO );
		c.setCurso( curso );

		cit = cHashOp.find( c );

		if ( cit == cHashOp.end() )
		{
			GGroup< Professor *, LessPtr< Professor > > professores_curso
				= it_map->second;

			sprintf( name, "%s", c.toString().c_str() );
			nnz = professores_curso.size();
			rhs = 0.0;

			OPT_ROW row( nnz, OPT_ROW::LESS, rhs, name );
				  
			if ( debuging )
			std::cout << "\n\tCriando restricao";


			ITERA_GGROUP_LESSPTR( it_prof, professores_curso, Professor )
			{
				Professor * professor = ( *it_prof );
				
				if ( debuging )
		std::cout << "\n\t\tProfessor " << professor->getId();

				// Recupera os professores que estão associados ao curso, e que são menos que mestres
				VariableOpHash::iterator vit_find = vHashOp.begin();

				for (; vit_find != vHashOp.end(); vit_find++ )
				{
				   VariableOp v_find = vit_find->first;
				   
				   if ( debuging )
				   std::cout << "\n\t\t\t v = " << v_find.toString();

				   if ( debuging )
				   if ( v_find.getProfessor() != NULL )   	{			   
					   std::cout << "\n\t\t\t prof_V_tit = "; fflush(0);
					   if ( v_find.getProfessor()->titulacao != NULL ){
						   std::cout << v_find.getProfessor()->getTitulacao(); fflush(0);}
					   else
						   std::cout <<"\tTitulacao null!!!!!!";
				   }

				   if ( v_find.getType() == VariableOp::V_PROF_CURSO
					  && v_find.getCurso() == curso
					  && v_find.getProfessor() == professor
					  && ( professor->getTitulacao() < TipoTitulacao::Mestre ) )
				   {
					  row.insert( vit_find->second, 1.0 );
				   }
				    if ( debuging )
				   std::cout << "\n\t\t\tend v";
				}
				 if ( debuging )
				  std::cout << "end prof";
			}

			// Variavel npvg_{c,r}
			ITERA_GGROUP_LESSPTR( it_contr, problemData->tipos_contrato, TipoContrato )
			{
				VariableOp v;
				v.reset();
				v.setType( VariableOp::V_NRO_PROFS_VIRTUAIS_GERAIS_CURSO );
				v.setContrato( *it_contr );
				v.setCurso( curso );

				vit = vHashOp.find( v );
				if ( vit != vHashOp.end() )
				{
					row.insert( vit->second, 1.0 );
				}
			}			

			double minPercMestre = ( curso->regra_min_mestres.second / 100.0 );
        
			// Variavel np_{c}
			VariableOp v;
			v.reset();
			v.setType( VariableOp::V_NRO_PROFS_CURSO );
			v.setCurso( curso );
			vit = vHashOp.find( v );

			if ( vit != vHashOp.end() )
			{
				row.insert( vit->second, -(1-minPercMestre) );
			}
			
			if ( debuging )
		std::cout << "\n\tInseri np_{c} ";
			
			// Variavel npv_{c,r}
			ITERA_GGROUP_LESSPTR( it_contr, problemData->tipos_contrato, TipoContrato )
			{
				VariableOp v;
				v.reset();
				v.setType( VariableOp::V_NRO_PROFS_VIRTUAIS_CURSO );
				v.setContrato( *it_contr );
				v.setCurso( curso );

				vit = vHashOp.find( v );
				if ( vit != vHashOp.end() )
				{
					row.insert( vit->second, -(1-minPercMestre) );
				}
			}

			if ( row.getnnz() > 2 )
			{
				cHashOp[ c ] = lp->getNumRows();
				lp->addRow( row );
				restricoes++;
			}
			
			if ( debuging )
		std::cout << "\n\tInseri npv_{c} ";

        }
   }
   return restricoes;
}

// RODADAS 1 E 2
int Operacional::criaRestricaoMaximoNaoDoutoresCurso()
{
   int restricoes = 0;
      
   if ( ! problemData->parametros->min_doutores )
   {
		return restricoes;
   }

   int nnz;
   double rhs;
   char name[ 200 ];

   ConstraintOp c;
   VariableOpHash::iterator vit;
   ConstraintOpHash::iterator cit;

   GGroup< Professor *, LessPtr< Professor > > professores
      = problemData->getProfessores();

   // Agrupando os professores que ministram disciplinas de cada curso
   std::map< Curso *, GGroup< Professor *,
      LessPtr< Professor > > > mapCursoProfessores;

   ITERA_GGROUP_LESSPTR( it_aula, problemData->aulas, Aula )
   {
         Aula * aula = ( *it_aula );
         Disciplina * disciplina = aula->getDisciplina();
		 int dia = aula->getDiaSemana();

         ITERA_GGROUP_LESSPTR( it_oferta, aula->ofertas, Oferta )
         {
            Oferta * oferta = ( *it_oferta );

			if ( oferta->curso->regra_min_doutores.second > 0 )
			{
				 // Garante que vai haver a restrição para o curso, mesmo que não haja professor real para o mesmo
				 GGroup< Professor *, LessPtr< Professor > > vazio;				 
				 std::map< Curso*, GGroup< Professor*, LessPtr< Professor > > >::iterator
					 itFinder = mapCursoProfessores.find( oferta->curso );
				 if ( itFinder == mapCursoProfessores.end() )
				 {
					mapCursoProfessores[ oferta->curso ] = vazio;
				 }

				 // Adiciona os professores reais aptos a ministrar a disciplina
				 ITERA_GGROUP_LESSPTR( it_prof, professores, Professor )
				 {
					 Professor * professor = ( *it_prof );
	  
					 if (professor->eVirtual()) continue;
			 
					 std::pair< int, int > professor_disciplina ( professor->getId(), disciplina->getId() );
			 
					 // Se o professor e a disciplina da aula em questão se relacionarem
					 std::map< std::pair< int /*idProf*/, int /*idDisc*/>, GGroup< int > /*Dias*/ >::iterator
						 ifFinderDD = problemData->prof_Disc_Dias.find( professor_disciplina );
					 if ( ifFinderDD != problemData->prof_Disc_Dias.end() )
					 if ( ifFinderDD->second.find( dia ) != ifFinderDD->second.end() )
					 {
						 mapCursoProfessores[ oferta->curso ].add( professor );
					 }
				 }
			}
        }
   }


   std::map< Curso *, GGroup< Professor *, LessPtr< Professor > > >::iterator
      it_map = mapCursoProfessores.begin();

   for (; it_map != mapCursoProfessores.end();
          it_map++ )
   {
        Curso * curso = it_map->first;

		c.reset();
		c.setType( ConstraintOp::C_MAX_NAO_DOUT_CURSO );
		c.setCurso( curso );

		cit = cHashOp.find( c );

		if ( cit == cHashOp.end() )
		{
			GGroup< Professor *, LessPtr< Professor > > professores_curso
				= it_map->second;

			sprintf( name, "%s", c.toString().c_str() );
			nnz = professores_curso.size();
			rhs = 0.0;

			OPT_ROW row( nnz, OPT_ROW::LESS, rhs, name );
				  
			ITERA_GGROUP_LESSPTR( it_prof, professores_curso, Professor )
			{
				Professor * professor = ( *it_prof );
				
				// Recupera os professores que estão associados ao curso, e que são menos que doutores
				VariableOpHash::iterator vit_find = vHashOp.begin();
				for (; vit_find != vHashOp.end(); vit_find++ )
				{
				   VariableOp v_find = vit_find->first;

				   if ( v_find.getType() == VariableOp::V_PROF_CURSO
					  && v_find.getCurso() == curso
					  && v_find.getProfessor() == professor
					  && professor->getTitulacao() < TipoTitulacao::Doutor )
				   {
					  row.insert( vit_find->second, 1.0 );
				   }
				}
			}

			// Variavel npvm_{c,r}
			ITERA_GGROUP_LESSPTR( it_contr, problemData->tipos_contrato, TipoContrato )
			{
				VariableOp v;
				v.reset();
				v.setType( VariableOp::V_NRO_PROFS_VIRTUAIS_MEST_CURSO );
				v.setContrato( *it_contr );
				v.setCurso( curso );

				vit = vHashOp.find( v );
				if ( vit != vHashOp.end() )
				{
					row.insert( vit->second, 1.0 );
				}
			}

			// Variavel npvg_{c,r}
			ITERA_GGROUP_LESSPTR( it_contr, problemData->tipos_contrato, TipoContrato )
			{
				VariableOp v;
				v.reset();
				v.setType( VariableOp::V_NRO_PROFS_VIRTUAIS_GERAIS_CURSO );
				v.setContrato( *it_contr );
				v.setCurso( curso );

				vit = vHashOp.find( v );
				if ( vit != vHashOp.end() )
				{
					row.insert( vit->second, 1.0 );
				}
			}
			

			double minPercDout = ( curso->regra_min_doutores.second / 100.0 );
        
			// Variavel np_{c}
			VariableOp v;
			v.reset();
			v.setType( VariableOp::V_NRO_PROFS_CURSO );
			v.setCurso( curso );
			vit = vHashOp.find( v );

			if ( vit != vHashOp.end() )
			{
				row.insert( vit->second, -(1-minPercDout) );
			}

			// Variavel npv_{c,r}
			ITERA_GGROUP_LESSPTR( it_contr, problemData->tipos_contrato, TipoContrato )
			{
				VariableOp v;
				v.reset();
				v.setType( VariableOp::V_NRO_PROFS_VIRTUAIS_CURSO );
				v.setContrato( *it_contr );
				v.setCurso( curso );

				vit = vHashOp.find( v );
				if ( vit != vHashOp.end() )
				{
					row.insert( vit->second, -(1-minPercDout) );
				}
			}
						
			if ( row.getnnz() > 0 )
			{
				cHashOp[ c ] = lp->getNumRows();
				lp->addRow( row );
				restricoes++;
			}
        }
   }

   return restricoes;
}


/*
	Seta variavel w_{p,c}

	1)  sum[d E D_{c}] sum[i E I_{d,c}] y_{p,d,i} <= M*w_{p,c}		Para todo p, c

	&

	2)  sum[d E D_{c}] sum[i E I_{d,c}] y_{p,d,i} >= w_{p,c}		Para todo p, c
*/
int Operacional::criaRestricaoAlocacaoProfessorCurso()
{
	int restricoes = 0;
	char name[ 200 ];
	double bigM = 999;

	ConstraintOp c;
	VariableOpHash::iterator vit;
	VariableOpHash::iterator vit_f;
	ConstraintOpHash::iterator cit;

	vit = vHashOp.begin();
	for (; vit != vHashOp.end(); vit++ )
	{
		VariableOp v = ( vit->first );

		double coef=0.0;

		if (v.getType() == VariableOp::V_PROF_CURSO)					// w_{p,c}
		{
			// CONSTRAINT 1 -----------------------------

			coef=-bigM;
			
			c.reset();
			c.setType( ConstraintOp::C_ALOC_PROF_CURSO1 );
			c.setProfessor( v.getProfessor() );
			c.setCurso( v.getCurso() );

			cit = cHashOp.find(c);
			if ( cit == cHashOp.end() )
			{
				cHashOp[ c ] = lp->getNumRows();

				sprintf( name, "%s", c.toString().c_str() );
				OPT_ROW row( 100, OPT_ROW::LESS, 0.0, name );

				row.insert( vit->second, coef );

				lp->addRow( row );
				restricoes++;
			}
			else
			{
				lp->chgCoef(cit->second, vit->second, coef);
			}

			// CONSTRAINT 2 -----------------------------

			coef=-1.0;
		
			c.reset();
			c.setType( ConstraintOp::C_ALOC_PROF_CURSO2 );
			c.setProfessor( v.getProfessor() );
			c.setCurso( v.getCurso() );

			cit = cHashOp.find(c);
			if ( cit == cHashOp.end() )
			{
				cHashOp[ c ] = lp->getNumRows();

				sprintf( name, "%s", c.toString().c_str() );
				OPT_ROW row( 100, OPT_ROW::GREATER, 0.0, name );

				row.insert( vit->second, coef );

				lp->addRow( row );
				restricoes++;
			}
			else
			{
				lp->chgCoef(cit->second, vit->second, coef);
			}

		}
		else if(v.getType() == VariableOp::V_Y_PROF_DISCIPLINA)			// y_{p,d,i}
		{
			coef = 1.0;

			GGroup< Curso*, LessPtr<Curso> > cursosAtendidos = 
				problemData->cursosAtendidosNaTurma( v.getTurma(), v.getDisciplina(), v.getCampus() );

			ITERA_GGROUP_LESSPTR( it_curso, cursosAtendidos, Curso )
			{
				Curso * curso = *it_curso;
												
				// CONTRAINT 1 -----------------------

				c.reset();
				c.setType( ConstraintOp::C_ALOC_PROF_CURSO1 );
				c.setProfessor( v.getProfessor() );
				c.setCurso( curso );

				cit = cHashOp.find(c);
				if ( cit == cHashOp.end() )
				{
					cHashOp[ c ] = lp->getNumRows();

					sprintf( name, "%s", c.toString().c_str() );
					OPT_ROW row( 100, OPT_ROW::LESS, 0.0, name );

					row.insert( vit->second, coef );

					lp->addRow( row );
					restricoes++;
				}
				else
				{
					lp->chgCoef(cit->second, vit->second, 1.0);
				}
				
				// CONSTRAINT 2 -----------------------------
								
				c.reset();
				c.setType( ConstraintOp::C_ALOC_PROF_CURSO2 );
				c.setProfessor( v.getProfessor() );
				c.setCurso( curso );

				cit = cHashOp.find(c);
				if ( cit == cHashOp.end() )
				{
					cHashOp[ c ] = lp->getNumRows();

					sprintf( name, "%s", c.toString().c_str() );
					OPT_ROW row( 100, OPT_ROW::GREATER, 0.0, name );

					row.insert( vit->second, coef );

					lp->addRow( row );
					restricoes++;
				}
				else
				{
					lp->chgCoef(cit->second, vit->second, coef);
				}
			}
		}

	}

	return restricoes;
}

int Operacional::criaRestricaoCargaHorariaMinimaProfessor()
{
   int restricoes = 0;
   int nnz = problemData->aulas.size();
   double rhs;
   char name[ 200 ];

   if ( nnz == 0 || !problemData->parametros->evitar_reducao_carga_horaria_prof )
   {
      return restricoes;
   }

   double percMaxReducaoCHP = problemData->parametros->perc_max_reducao_CHP;

   ConstraintOp c;
   VariableOpHash::iterator vit;
   ConstraintOpHash::iterator cit;

   GGroup< Professor *, LessPtr< Professor > > professores
      = problemData->getProfessores();

   ITERA_GGROUP_LESSPTR( it_prof, professores, Professor )
   {
      Professor * professor = ( *it_prof );
	        
      c.reset();
      c.setType( ConstraintOp::C_CARGA_HOR_MIN_PROF );
      c.setProfessor( professor );

      cit = cHashOp.find( c );

      if ( cit == cHashOp.end() )
      {
         sprintf( name, "%s", c.toString().c_str() );
         rhs = professor->getChAnterior() * ( 1 - percMaxReducaoCHP/100 );

         OPT_ROW row( nnz, OPT_ROW::GREATER, rhs, name );
         
         vit = vHashOp.begin();

         for (; vit != vHashOp.end(); vit++ )
         {
            VariableOp v = vit->first;

            if ( v.getType() == VariableOp::V_Y_PROF_DISCIPLINA
               && v.getProfessor() == professor )
            {
				double val = v.getDisciplina()->getTotalCreditos();
                row.insert( vit->second, val );
            }
			else if ( v.getType() == VariableOp::V_F_CARGA_HOR_MIN_PROF
					&& v.getProfessor() == professor )
			{   
				row.insert( vit->second, 1.0 ); // Insere a variável de folga na restrição
			}
         }
         
		 if ( row.getnnz() != 0 )
		 {
			cHashOp[ c ] = lp->getNumRows();
			lp->addRow( row );
		    restricoes++;
		 }
      }
   }

   return restricoes;
}

int Operacional::criaRestricaoCargaHorariaMinimaProfessorSemana()
{
   int restricoes = 0;
   int nnz = problemData->aulas.size();
   double rhs;
   char name[ 200 ];

   if ( nnz == 0 )
   {
      return restricoes;
   }

   ConstraintOp c;
   VariableOpHash::iterator vit;
   ConstraintOpHash::iterator cit;

   GGroup< Professor *, LessPtr< Professor > > professores
      = problemData->getProfessores();

   ITERA_GGROUP_LESSPTR( it_prof, professores, Professor )
   {
      Professor * professor = ( *it_prof );

      c.reset();
      c.setType( ConstraintOp::C_CARGA_HOR_MIN_PROF_SEMANA );
      c.setProfessor( professor );

      cit = cHashOp.find( c );

      if ( cit == cHashOp.end() )
      {
         sprintf( name, "%s", c.toString().c_str() );
         rhs = ( professor->getChMin() > 0 ? professor->getChMin() : 1 );

         OPT_ROW row( nnz, OPT_ROW::GREATER, rhs, name );

         vit = vHashOp.begin();

         for (; vit != vHashOp.end(); vit++ )
         {
            VariableOp v = vit->first;

            if ( v.getType() == VariableOp::V_X_PROF_AULA_HOR
                  && v.getProfessor() == professor )
            {
               // Verifica se o professor está alocado a essa disciplina
               bool encontrou = false;

               GGroup< std::pair< Aula *, Disciplina * > >::iterator
                  it_disciplinas = problemData->mapProfessorDisciplinas[ professor ].begin();

               for (; it_disciplinas != problemData->mapProfessorDisciplinas[ professor ].end();
                      it_disciplinas++ )
               {
                  if ( ( *it_disciplinas ).second->getId() == v.getDisciplina()->getId() )
                  {
                     encontrou = true;
                     break;
                  }
               }

               if ( encontrou )
               {
                  double totalCreditos = ( v.getAula()->getTotalCreditos() );

                  row.insert( vit->second, totalCreditos );
               }
               ///////
            }
         }

         // Insere a variável de folga na restrição
         VariableOp v;
         v.reset();
         v.setType( VariableOp::V_F_CARGA_HOR_MIN_PROF_SEMANA );
         v.setProfessor( professor );

         vit = vHashOp.find( v );

         if ( vit != vHashOp.end() )
         {
            row.insert( vit->second, 1.0 );
         }
         ///////

         cHashOp[ c ] = lp->getNumRows();
         lp->addRow( row );

         restricoes++;
      }
   }

   return restricoes;
}

int Operacional::criaRestricaoCargaHorariaMaximaProfessorSemana()
{
    int restricoes = 0;
    int nnz = 0;
    double rhs;
    char name[ 200 ];

    if ( problemData->aulas.size() == 0 )
    {
		return restricoes;
    }

    VariableOpHash::iterator vit = vHashOp.begin();
	for (; vit != vHashOp.end(); vit++ )
	{
		VariableOp v = vit->first;

		if ( v.getType() == VariableOp::V_Y_PROF_DISCIPLINA )
		{
			if ( v.getProfessor()->eVirtual() && this->getRodada() == Operacional::OP_VIRTUAL_PERFIL )
			{
				continue;
			}
			
			ConstraintOp c;
			c.reset();
			c.setType( ConstraintOp::C_CARGA_HOR_MAX_PROF_SEMANA );
			c.setProfessor( v.getProfessor() );
			
			double ch = v.getDisciplina()->getTotalCreditos() *
						v.getDisciplina()->getTempoCredSemanaLetiva();
			  
			ConstraintOpHash::iterator cit = cHashOp.find( c );
			if ( cit == cHashOp.end() )
			{
				sprintf( name, "%s", c.toString().c_str() );
				rhs = v.getProfessor()->getChMax() * 60;
				nnz = v.getProfessor()->magisterio.size();

				OPT_ROW row( nnz, OPT_ROW::LESS, rhs, name );

				row.insert( vit->second, ch );
				
				// Insere a variável de folga na restrição
				VariableOp v;
				v.reset();
				v.setType( VariableOp::V_F_CARGA_HOR_MAX_PROF_SEMANA );
				v.setProfessor( v.getProfessor() );

				VariableOpHash::iterator vitin = vHashOp.begin();
				vitin = vHashOp.find( v );
				if ( vitin != vHashOp.end() )
				{
					row.insert( vitin->second, -1.0 );
				}

				cHashOp[ c ] = lp->getNumRows();
				lp->addRow( row );

				restricoes++;
			}				  
			else
			{
				lp->chgCoef( cit->second, vit->second, ch );
			}
		}
	}

   return restricoes;
}


/*
	Seta variavel dpc_{p,c,d}

	1) sum[i] y_{p,d,i} <= nTurmas_{d} * dpc_{p,c,d}		Para todo prof p, curso c, disc d E D_{teoricas}

		i pertencente a I_{d,c} U I_{-d,c} ( contem alunos do curso c )


	2) sum[i] y_{p,d,i} >= dpc_{p,c,d}		Para todo prof p, curso c, disc d

		i pertencente a I_{d,c} U I_{-d,c} ( contem alunos do curso c )
*/
int Operacional::criaRestricaoDiscProfCurso()
{
   int restricoes = 0;
   int nnz = 0;
   double rhs = 0.0;
   char name[ 200 ];
   
   ConstraintOp c;
   VariableOp v_find;
   VariableOpHash::iterator vit;
   ConstraintOpHash::iterator cit;

	vit = vHashOp.begin();

	for( ; vit != vHashOp.end(); vit++ )
	{
		VariableOp v = ( vit->first );

		if (v.getType() != VariableOp::V_DISC_PROF_CURSO &&
			v.getType() != VariableOp::V_Y_PROF_DISCIPLINA)
		{
			continue;
		}

		if ( v.getProfessor()->eVirtual() && this->getRodada() == Operacional::OP_VIRTUAL_PERFIL )
		{
			continue;
		}

		if(v.getType() == VariableOp::V_DISC_PROF_CURSO)			// dpc_{d,p,c}
		{
			// Restrições só por disciplina teorica.
			if ( v.getDisciplina()->getId() < 0 )
			{
				std::cout << "\nAtencao: variavel dpc a principio deveria ser somente para disc teorica."
					<< " Var " << v.toString();
				continue;
			}

			// CONTRAINT 1 -----------------------

			c.reset();
			c.setType( ConstraintOp::C_DISC_PROF_CURSO1 );
			c.setProfessor( v.getProfessor() );
			c.setCurso( v.getCurso() );
			c.setDisciplina( v.getDisciplina() );

			double M = v.getDisciplina()->getNumTurmas();
			
			Disciplina *discPrat = problemData->getDisciplinaTeorPrat( v.getDisciplina() );

			if ( discPrat != NULL )
			{
				M += discPrat->getNumTurmas();
			}

			cit = cHashOp.find(c);
			if ( cit == cHashOp.end() )
			{
				cHashOp[ c ] = lp->getNumRows();
				sprintf( name, "%s", c.toString().c_str() );
				OPT_ROW row( 100, OPT_ROW::LESS, 0.0, name );
								
				row.insert( vit->second, -M );

				lp->addRow( row );
				restricoes++;
			}
			else
			{
				lp->chgCoef(cit->second, vit->second, -M);
			}		

			// CONTRAINT 2 -----------------------

			c.reset();
			c.setType( ConstraintOp::C_DISC_PROF_CURSO2 );
			c.setProfessor( v.getProfessor() );
			c.setCurso( v.getCurso() );
			c.setDisciplina( v.getDisciplina() );
			
			cit = cHashOp.find(c);
			if ( cit == cHashOp.end() )
			{
				cHashOp[ c ] = lp->getNumRows();
				sprintf( name, "%s", c.toString().c_str() );
				OPT_ROW row( 100, OPT_ROW::GREATER, 0.0, name );
								
				row.insert( vit->second, -1.0 );

				lp->addRow( row );
				restricoes++;
			}
			else
			{
				lp->chgCoef(cit->second, vit->second, -1.0);
			}
		}
		else if(v.getType() == VariableOp::V_Y_PROF_DISCIPLINA)			// y_{p,d,i}
		{
			Disciplina *disciplina = v.getDisciplina();

			if ( disciplina->getId() < 0 )
			{
				// Restrições só por disciplina teorica.
				disciplina = problemData->refDisciplinas[ abs( disciplina->getId() ) ];
			}

			GGroup< Curso*, LessPtr<Curso> > cursosAtendidos = 
				problemData->cursosAtendidosNaTurma( v.getTurma(), v.getDisciplina(), v.getCampus() );

			ITERA_GGROUP_LESSPTR( it_curso, cursosAtendidos, Curso )
			{
				Curso * curso = *it_curso;
								
				// CONTRAINT 1 -----------------------

				c.reset();
				c.setType( ConstraintOp::C_DISC_PROF_CURSO1 );
				c.setProfessor( v.getProfessor() );
				c.setCurso( curso );
				c.setDisciplina( disciplina );

				cit = cHashOp.find(c);
				if ( cit == cHashOp.end() )
				{
					cHashOp[ c ] = lp->getNumRows();

					sprintf( name, "%s", c.toString().c_str() );
					OPT_ROW row( 100, OPT_ROW::LESS, 0.0, name );

					row.insert( vit->second, 1.0 );

					lp->addRow( row );
					restricoes++;
				}
				else
				{
					lp->chgCoef(cit->second, vit->second, 1.0);
				}

				// CONTRAINT 2 -----------------------

				c.reset();
				c.setType( ConstraintOp::C_DISC_PROF_CURSO2 );
				c.setProfessor( v.getProfessor() );
				c.setCurso( curso );
				c.setDisciplina( disciplina );

				cit = cHashOp.find(c);
				if ( cit == cHashOp.end() )
				{
					cHashOp[ c ] = lp->getNumRows();

					sprintf( name, "%s", c.toString().c_str() );
					OPT_ROW row( 100, OPT_ROW::GREATER, 0.0, name );

					row.insert( vit->second, 1.0 );

					lp->addRow( row );
					restricoes++;
				}
				else
				{
					lp->chgCoef(cit->second, vit->second, 1.0);
				}
			}
		}		
	}
	
	return restricoes;
}


int Operacional::criaRestricaoMaxDiscProfCurso()
{
   int restricoes = 0;

   if ( ! problemData->parametros->maximoDisciplinasDeUmProfessorPorCurso )
   {
	   return restricoes;
   }

   int nnz = 0;
   double rhs = 0.0;
   char name[ 200 ];

   ConstraintOp c;
   VariableOp v_find;
   VariableOpHash::iterator vit;
   ConstraintOpHash::iterator cit;

	vit = vHashOp.begin();

	for (; vit != vHashOp.end(); vit++)
	{
		VariableOp v = ( vit->first );
				
		if (v.getType() != VariableOp::V_DISC_PROF_CURSO) // dpc_{p,c,d}
		{
			continue;
		}

		// Restrições só por disciplina teorica.
		if ( v.getDisciplina()->getId() < 0 )
		{
			std::cout << "\nAtencao em Operacional::criaRestricaoMaxDiscProfCurso: "
				<< "variavel dpc a principio deveria ser somente para disc teorica."
				<< " Var " << v.toString();
			continue;
		}

		if ( v.getProfessor()->eVirtual() && this->getRodada() == Operacional::OP_VIRTUAL_PERFIL )
		{
			continue;
		}
						
		double coef = 1.0;

		c.reset();
		c.setType( ConstraintOp::C_MAX_DISC_PROF_CURSO );
		c.setProfessor( v.getProfessor() );
		c.setCurso( v.getCurso() );

		cit = cHashOp.find(c);
		if ( cit == cHashOp.end() )
		{
			cHashOp[ c ] = lp->getNumRows();

			sprintf( name, "%s", c.toString().c_str() );
			
			rhs = v.getCurso()->getQtdMaxProfDisc();
			nnz = v.getCurso()->getNumTotalDisciplinasNoCurso();

			OPT_ROW row( nnz, OPT_ROW::LESS, rhs, name );

			row.insert( vit->second, coef );

			lp->addRow( row );
			restricoes++;
		}
		else
		{
			lp->chgCoef(cit->second, vit->second, coef );
		}
	}

	return restricoes;
}

/*
	Seta variavel dpo_{p,o,d}

	1) sum[i] y_{p,d,i} <= nTurmas_{d} * dpo_{p,o,d}		Para todo prof p, oferta o, disc d E D_{teoricas}

		i pertencente a I_{d,o} U I_{-d,o} ( contem alunos da oferta p )


	2) sum[i] y_{p,d,i} >= dpo_{p,o,d}		Para todo prof p, oferta o, disc d

		i pertencente a I_{d,o} U I_{-d,o} ( contem alunos da oferta p )
*/
int Operacional::criaRestricaoDiscProfOferta()
{
   int restricoes = 0;
   int nnz = 0;
   double rhs = 0.0;
   char name[ 200 ];
   
   ConstraintOp c;
   VariableOp v_find;
   VariableOpHash::iterator vit;
   ConstraintOpHash::iterator cit;

	vit = vHashOp.begin();

	for( ; vit != vHashOp.end(); vit++ )
	{
		VariableOp v = ( vit->first );

		if (v.getType() != VariableOp::V_DISC_PROF_OFERTA &&
			v.getType() != VariableOp::V_Y_PROF_DISCIPLINA)
		{
			continue;
		}

		if ( v.getProfessor()->eVirtual() && this->getRodada() == Operacional::OP_VIRTUAL_PERFIL )
		{
			continue;
		}
		
		if(v.getType() == VariableOp::V_DISC_PROF_OFERTA)			// dpo_{d,p,oft}
		{
			// Restrições só por disciplina teorica.
			if ( v.getDisciplina()->getId() < 0 )
			{
				std::cout << "\nAtencao: variavel dpc a principio deveria ser somente para disc teorica."
					<< " Var " << v.toString();
				continue;
			}

			// CONTRAINT 1 -----------------------

			c.reset();
			c.setType( ConstraintOp::C_DISC_PROF_OFERTA1 );
			c.setProfessor( v.getProfessor() );
			c.setOferta( v.getOferta() );
			c.setDisciplina( v.getDisciplina() );

			double M = v.getDisciplina()->getNumTurmas();
			
			cit = cHashOp.find(c);
			if ( cit == cHashOp.end() )
			{
				cHashOp[ c ] = lp->getNumRows();
				sprintf( name, "%s", c.toString().c_str() );
				OPT_ROW row( 100, OPT_ROW::LESS, 0.0, name );
								
				row.insert( vit->second, -M );

				lp->addRow( row );
				restricoes++;
			}
			else
			{
				lp->chgCoef(cit->second, vit->second, -M);
			}		

			// CONTRAINT 2 -----------------------

			c.reset();
			c.setType( ConstraintOp::C_DISC_PROF_OFERTA2 );
			c.setProfessor( v.getProfessor() );
			c.setOferta( v.getOferta() );
			c.setDisciplina( v.getDisciplina() );
			
			cit = cHashOp.find(c);
			if ( cit == cHashOp.end() )
			{
				cHashOp[ c ] = lp->getNumRows();
				sprintf( name, "%s", c.toString().c_str() );
				OPT_ROW row( 100, OPT_ROW::GREATER, 0.0, name );
								
				row.insert( vit->second, -1.0 );

				lp->addRow( row );
				restricoes++;
			}
			else
			{
				lp->chgCoef(cit->second, vit->second, -1.0);
			}
		}
		else if(v.getType() == VariableOp::V_Y_PROF_DISCIPLINA)			// y_{p,d,i}
		{
			Disciplina *disciplina = v.getDisciplina();
			
			if ( disciplina->getId() < 0 )
			{
				// Restrições só por disciplina teorica.
				disciplina = problemData->refDisciplinas[ abs( disciplina->getId() ) ];
			}

			GGroup< Oferta*, LessPtr<Oferta> > ofertasAtendidas = 
				problemData->ofertasAtendidasNaTurma( v.getTurma(), v.getDisciplina(), v.getCampus() );

			ITERA_GGROUP_LESSPTR( it_oft, ofertasAtendidas, Oferta )
			{
				Oferta *oferta = *it_oft;
				
				if ( oferta->curso->getMaisDeUma() )
				{
					continue;
				}

				// CONTRAINT 1 -----------------------

				c.reset();
				c.setType( ConstraintOp::C_DISC_PROF_OFERTA1 );
				c.setProfessor( v.getProfessor() );
				c.setOferta( oferta );
				c.setDisciplina( disciplina );

				cit = cHashOp.find(c);
				if ( cit == cHashOp.end() )
				{
					cHashOp[ c ] = lp->getNumRows();

					sprintf( name, "%s", c.toString().c_str() );
					OPT_ROW row( 100, OPT_ROW::LESS, 0.0, name );

					row.insert( vit->second, 1.0 );

					lp->addRow( row );
					restricoes++;
				}
				else
				{
					lp->chgCoef(cit->second, vit->second, 1.0);
				}

				// CONTRAINT 2 -----------------------

				c.reset();
				c.setType( ConstraintOp::C_DISC_PROF_OFERTA2 );
				c.setProfessor( v.getProfessor() );
				c.setOferta( oferta );
				c.setDisciplina( disciplina );

				cit = cHashOp.find(c);
				if ( cit == cHashOp.end() )
				{
					cHashOp[ c ] = lp->getNumRows();

					sprintf( name, "%s", c.toString().c_str() );
					OPT_ROW row( 100, OPT_ROW::GREATER, 0.0, name );

					row.insert( vit->second, 1.0 );

					lp->addRow( row );
					restricoes++;
				}
				else
				{
					lp->chgCoef(cit->second, vit->second, 1.0);
				}
			}
		}		
	}
	
	return restricoes;
}


int Operacional::criaRestricaoMaxDiscProfPeriodoOferta()
{
   int restricoes = 0;
   int nnz = 0;
   double rhs = 0.0;
   char name[ 200 ];

   ConstraintOp c;
   VariableOp v_find;
   VariableOpHash::iterator vit;
   ConstraintOpHash::iterator cit;

	vit = vHashOp.begin();

	for (; vit != vHashOp.end(); vit++)
	{
		VariableOp v = ( vit->first );
				
		if (v.getType() != VariableOp::V_DISC_PROF_OFERTA) // dpo_{p,oft,d}
		{
			continue;
		}

		// Restrições só por disciplina teorica.
		if ( v.getDisciplina()->getId() < 0 )
		{
			std::cout << "\nAtencao em Operacional::criaRestricaoMaxDiscProfCurso: "
				<< "variavel dpc a principio deveria ser somente para disc teorica."
				<< " Var " << v.toString();
			continue;
		}

		if ( v.getProfessor()->eVirtual() && this->getRodada() == Operacional::OP_VIRTUAL_PERFIL )
		{
			continue;
		}
						
		double coef = 1.0;

		Disciplina *disciplina = v.getDisciplina();
		Professor *professor = v.getProfessor();
		Oferta* oferta = v.getOferta();

		int periodo = oferta->curriculo->getPeriodo( disciplina );
		
		c.reset();
		c.setType( ConstraintOp::C_MAX_DISC_PROF_OFERTA_PERIODO );
		c.setProfessor( professor );
		c.setOferta( oferta );
		c.setPeriodo( periodo );

		cit = cHashOp.find(c);
		if ( cit == cHashOp.end() )
		{
			cHashOp[ c ] = lp->getNumRows();

			sprintf( name, "%s", c.toString().c_str() );
			
			rhs = oferta->curso->getQtdMaxDiscNoPeriodoPorProf();
			nnz = oferta->curso->getNumTotalDisciplinasNoCurso();

			OPT_ROW row( nnz, OPT_ROW::LESS, rhs, name );

			row.insert( vit->second, coef );

			lp->addRow( row );
			restricoes++;
		}
		else
		{
			lp->chgCoef(cit->second, vit->second, coef );
		}
	}

	return restricoes;
}

// SÓ PARA RODADA 1
int Operacional::criaRestricaoGaranteMinProfsPorCurso()
{
   int restricoes = 0;

   if ( ! problemData->parametros->maximoDisciplinasDeUmProfessorPorCurso )
   {
	   return restricoes;
   }

   int nnz;
   double rhs;
   char name[ 200 ];

   ConstraintOp c;
   VariableOpHash::iterator vit;
   ConstraintOpHash::iterator cit;
   VariableOp v;
   
	ITERA_GGROUP_LESSPTR( itCurso, problemData->cursos, Curso )
	{
		Curso *curso = *itCurso;
	   	   
		c.reset();
		c.setType( ConstraintOp::C_GARANTE_MIN_PROFS_CURSO );
		c.setCurso( curso );

		cit = cHashOp.find( c );

		if ( cit == cHashOp.end() )
		{
			sprintf( name, "%s", c.toString().c_str() );
			nnz = problemData->cursos.size() * 2;
			rhs = (double) problemData->getNroDiscsAtendidasNoCurso( curso );

			double coef = (double) curso->getQtdMaxProfDisc();

			OPT_ROW row( nnz, OPT_ROW::GREATER, rhs, name );

			// Variavel np_{c}
			v.reset();
			v.setType( VariableOp::V_NRO_PROFS_CURSO );
			v.setCurso( curso );

			vit = vHashOp.find( v );
			if ( vit != vHashOp.end() )
			{
				row.insert( vit->second, coef );
			}				

			// Variavel npv_{c,r}
		    ITERA_GGROUP_LESSPTR( itContr, problemData->tipos_contrato, TipoContrato )
		    {
				v.reset();
				v.setType( VariableOp::V_NRO_PROFS_VIRTUAIS_CURSO );
				v.setCurso( curso );
				v.setContrato( *itContr );

				vit = vHashOp.find( v );
				if ( vit != vHashOp.end() )
				{
					row.insert( vit->second, coef );
				}
		    }

			// Insere restrição
			if ( row.getnnz() > 0 )
			{
				cHashOp[ c ] = lp->getNumRows();
				lp->addRow( row );
				restricoes++;
			}
		}

   }

   return restricoes;
}

int Operacional::criaRestricaoDeslocamentoProfessor()
{
   int restricoes = 0;
   int nnz = 2;
   double rhs = 1.0;
   char name[ 200 ];

   if ( problemData->tempo_campi.size() == 0
      || problemData->tempo_unidades.size() == 0 )
   {
      return restricoes;
   }

   ConstraintOp c;
   VariableOpHash::iterator vit1;
   VariableOpHash::iterator vit2;

   // Hash que armazena apenas as variáveis 'Xpah'
   VariableOpHash hashX;
   vit1 = vHashOp.begin();

   for (; vit1 != vHashOp.end(); vit1++ )
   {
      if ( vit1->first.getType() == VariableOp::V_X_PROF_AULA_HOR )
      {
		  if ( ! vit1->first.getProfessor()->eVirtual() )
			 hashX[ vit1->first ] = vit1->second;
      }
   }

   GGroup< Professor *, LessPtr< Professor > > professores
      = problemData->getProfessores();

   ITERA_GGROUP_LESSPTR( it_prof, professores, Professor )
   {
      Professor * professor = ( *it_prof );

      c.reset();
      c.setType( ConstraintOp::C_DESLOC_PROF );
      c.setProfessor( professor );

      if ( cHashOp.find( c ) == cHashOp.end() )
      {
         sprintf( name, "%s", c.toString().c_str() );

         vit1 = hashX.begin();
         for (; vit1 != hashX.end(); vit1++ )
         {
            VariableOp v1 = vit1->first;
            int idUnidade1 = v1.getSala()->getIdUnidade();

            vit2 = hashX.begin();
            for (; vit2 != hashX.end(); vit2++ )
            {
               VariableOp v2 = vit2->first;
               int idUnidade2 = v2.getSala()->getIdUnidade();

               if ( v1 == v2 || idUnidade1 == idUnidade2
                  || v1.getHorario()->getDia() != v2.getHorario()->getDia()
                  || v1.getProfessor() != professor
                  || v2.getProfessor() != professor )
               {
                  continue;
               }

               Unidade * unidade1 = problemData->refUnidade[ idUnidade1 ];
               Campus * campus1 = problemData->refCampus[ unidade1->getIdCampus() ];

               Unidade * unidade2 = problemData->refUnidade[ idUnidade2 ];
               Campus * campus2 = problemData->refCampus[ unidade2->getIdCampus() ];

               int tempo_minimo = problemData->calculaTempoEntreCampusUnidades(
                  campus1, campus2, unidade1, unidade2 );

               int tempo_disponivel = problemData->minutosIntervalo(
                  v1.getHorario()->getHorarioAula()->getInicio(),
                  v2.getHorario()->getHorarioAula()->getInicio() );

               if ( tempo_minimo > tempo_disponivel )
               {
                  // Cria a restrição 'Xpah1 + Xpah2 <= 1'
                  // --> Aloca no máximo uma das aulas ao professor
                  OPT_ROW row( nnz, OPT_ROW::LESS, rhs, name );

                  row.insert( vit1->second, 1.0 );
                  row.insert( vit2->second, 1.0 );

                  lp->addRow( row );
               }
            }
         }

         cHashOp[ c ] = lp->getNumRows();
         restricoes++;
      }
   }

   return restricoes;
}

int Operacional::criaRestricaoDeslocamentoViavel()
{
   int restricoes = 0;
   int nnz = 2;
   double rhs = 1.0;
   char name[ 200 ];

   if ( problemData->tempo_campi.size() == 0
      || problemData->tempo_unidades.size() == 0 )
   {
      return restricoes;
   }

   ConstraintOp c;
   VariableOpHash::iterator vit1;
   VariableOpHash::iterator vit2;

   // Hash que armazena apenas as variáveis 'Xpah'
   VariableOpHash hashX;
   vit1 = vHashOp.begin();

   for (; vit1 != vHashOp.end(); vit1++ )
   {
      if ( vit1->first.getType() == VariableOp::V_X_PROF_AULA_HOR )
      {
		  if ( ! vit1->first.getProfessor()->eVirtual() )
			 hashX[ vit1->first ] = vit1->second;
      }
   }

    GGroup< Professor *, LessPtr< Professor > > professores
      = problemData->getProfessores();

   vit1 = hashX.begin();

   for (; vit1 != hashX.end(); vit1++ )
   {
      VariableOp v1 = vit1->first;
      int idUnidade1 = v1.getSala()->getIdUnidade();
      Aula * aula1 = v1.getAula();

      c.reset();
      c.setType( ConstraintOp::C_DESLOC_VIAVEL );
      c.setAula( aula1 );

      if ( cHashOp.find( c ) == cHashOp.end() )
      {
         sprintf( name, "%s", c.toString().c_str() );

         vit2 = hashX.begin();
         for (; vit2 != hashX.end(); vit2++ )
         {
            VariableOp v2 = vit2->first;
            int idUnidade2 = v2.getSala()->getIdUnidade();
            Aula * aula2 = v2.getAula();

            if ( v1 == v2 || idUnidade1 == idUnidade2
               || v1.getHorario()->getDia() != v2.getHorario()->getDia() )
            {
               continue;
            }

            ITERA_GGROUP_LESSPTR( it_oferta1, aula1->ofertas, Oferta )
            {
               Oferta * oferta1 = ( *it_oferta1 );
               Curriculo * curriculo1 = oferta1->curriculo;
            
               ITERA_GGROUP_LESSPTR( it_oferta2, aula2->ofertas, Oferta )
               {
                  Oferta * oferta2 = ( *it_oferta2 );
                  Curriculo * curriculo2 = oferta2->curriculo;

                  if ( curriculo1 == curriculo2 )
                  {
                     continue;
                  }

                  Unidade * unidade1 = problemData->refUnidade[ idUnidade1 ];
                  Campus * campus1 = problemData->refCampus[ unidade1->getIdCampus() ];

                  Unidade * unidade2 = problemData->refUnidade[ idUnidade2 ];
                  Campus * campus2 = problemData->refCampus[ unidade2->getIdCampus() ];

                  int tempo_minimo = problemData->calculaTempoEntreCampusUnidades(
                     campus1, campus2, unidade1, unidade2 );

                  int tempo_disponivel = problemData->minutosIntervalo(
                     v1.getHorario()->getHorarioAula()->getInicio(),
                     v2.getHorario()->getHorarioAula()->getInicio() );

                  if ( tempo_minimo > tempo_disponivel )
                  {
                     // Cria a restrição 'Xpah1 + Xpah2 <= 1'
                     // --> Aloca no máximo uma das aulas ao professor
                     OPT_ROW row( nnz, OPT_ROW::LESS, rhs, name );

                     row.insert( vit1->second, 1.0 );
                     row.insert( vit2->second, 1.0 );

                     lp->addRow( row );
                  }
               }
            }
         }

         cHashOp[ c ] = lp->getNumRows();
         restricoes++;
      }
   }

   return restricoes;
}

int Operacional::criaRestricaoUltimaPrimeiraAulas()
{
   int restricoes = 0;

   if ( ! problemData->parametros->evitar_prof_ultimo_primeiro_hr )
   {
		return restricoes;
   }

   int nnz = 100;
   double rhs = 1.0;
   char name[ 200 ];

   ConstraintOp c;
   VariableOpHash::iterator vit1;
   VariableOpHash::iterator vit2;
   VariableOpHash::iterator vitSlack;

   map< Professor*, map< HorarioDia*, vector< VariableOpHash::iterator > , LessPtr < HorarioDia > >, LessPtr< Professor > > mapPrimeirasAulas;
   map< Professor*, map< HorarioDia*, vector< VariableOpHash::iterator > , LessPtr < HorarioDia > >, LessPtr< Professor > > mapUltimassAulas;
   
   vit1 = vHashOp.begin();

   for (; vit1 != vHashOp.end(); vit1++ )
   {
	   if ( vit1->first.getType() == VariableOp::V_X_PROF_AULA_HOR && !vit1->first.getProfessor()->eVirtual() )
      {
		  VariableOp v = vit1->first;

		  if( problemData->verificaPrimeiraAulas(v.getHorario()) )
			  mapPrimeirasAulas[v.getProfessor()][v.getHorario()].push_back(vit1);
		  else
		  {
			  int nCreds = v.getAula()->getTotalCreditos();
			  HorarioAula *hai = v.getHorarioAula();
			  HorarioAula *haf = hai->getCalendario()->getHorarioMaisNCreds( hai, nCreds-1 );
			  HorarioDia *ha = problemData->getHorarioDiaCorrespondente( haf, v.getDia() );
			  if( problemData->verificaUltimaAulas( ha ) )
				mapUltimassAulas[v.getProfessor()][v.getHorario()].push_back(vit1);
		  }
      }
   }

   map< Professor*, map< HorarioDia*, vector< VariableOpHash::iterator > , LessPtr < HorarioDia > >, LessPtr< Professor > >::iterator itP1 = mapPrimeirasAulas.begin();
   for(; itP1 != mapPrimeirasAulas.end(); itP1++)
   {
	   Professor *professor = itP1->first;

	   map< HorarioDia*, vector< VariableOpHash::iterator > , LessPtr < HorarioDia > >::iterator itP2 = itP1->second.begin();
	   for(; itP2 != itP1->second.end(); itP2++)
	   {
		   HorarioDia *horario1 = itP2->first; // primeiro horario do segundo dia

		   map< HorarioDia*, vector< VariableOpHash::iterator > , LessPtr < HorarioDia > > mapTemp = mapUltimassAulas[professor];
		   map< HorarioDia*, vector< VariableOpHash::iterator > , LessPtr < HorarioDia > >::iterator itU1 = mapTemp.begin();
		   for(; itU1 != mapTemp.end(); itU1++)
		   {
			   HorarioDia *horario2 = itU1->first; // ultimo horario do primeiro dia

			   if ( horario1->getDia() - horario2->getDia() != 1 )
				   continue;

			   c.reset();
			   c.setType( ConstraintOp::C_ULTIMA_PRIMEIRA_AULA_PROF );
			   c.setProfessor( professor );
			   c.setHorarioDiaD( horario1 );
			   c.setHorarioDiaD1( horario2 );

			   if ( cHashOp.find( c ) != cHashOp.end() )
				   continue;

			   sprintf( name, "%s", c.toString().c_str() );

			   cHashOp[ c ] = lp->getNumRows();
			   restricoes++;

			   OPT_ROW row( nnz, OPT_ROW::LESS, rhs, name );

			   vector< VariableOpHash::iterator > vars = itP2->second;

			   for(vector< VariableOpHash::iterator >::iterator itV = vars.begin();
				   itV != vars.end();
				   itV++)
			   {
				   VariableOpHash::iterator vit = *itV;
				   row.insert(vit->second, 1.0);
			   }

			   vars = itU1->second;

			   for(vector< VariableOpHash::iterator >::iterator itV = vars.begin();
				   itV != vars.end();
				   itV++)
			   {
				   VariableOpHash::iterator vit = *itV;
				   row.insert(vit->second, 1.0);
			   }


			   // Adiciona a variável de folga
			   VariableOp vSlack;
			   vSlack.reset();
			   vSlack.setType( VariableOp::V_F_ULTIMA_PRIMEIRA_AULA_PROF );

			   vSlack.setProfessor( professor );
			   vSlack.setDia( horario2->getDia() ); // primeiro dia

			   vitSlack = vHashOp.find( vSlack );

			   if ( vitSlack != vHashOp.end() )
			   {
				   row.insert( vitSlack->second, -1.0 );
			   }

			   lp->addRow( row );
		   }

	   }
   }

   return restricoes;
}


/*
	Garante um intervalo de tempo mínimo entre aulas de um mesmo professor
	em um mesmo dia e em unidades distintas.

*/
int Operacional::criaRestricaoProfHorarioMultiUnid( void )
{
	// todo: COMPARACAO DE HORARIO-DIA ESTA ERRADO! USAR DATETIME

	int restricoes = 0;
	int nnz;
	char name[ 200 ];

	ConstraintOp c;
	VariableOpHash::iterator vit;
	ConstraintOpHash::iterator cit;

	map< Professor*, map< int, map< HorarioDia*, map< Aula*, vector< VariableOpHash::iterator >, 
															 LessPtr< Aula > >,
							                     LessPtr< HorarioDia > > >,
					 LessPtr< Professor > > mapVariaveis;

	vit = vHashOp.begin();
	while( vit != vHashOp.end() )
	{
		if( vit->first.getType() == VariableOp::V_X_PROF_AULA_HOR && !vit->first.getProfessor()->eVirtual() )
		{
			VariableOp v = vit->first;
			mapVariaveis[v.getProfessor()][v.getDia()][v.getHorario()][v.getAula()].push_back(vit);
		}

		vit++;
	}

	map< Professor*, map< int, map< HorarioDia*, map< Aula*, vector< VariableOpHash::iterator >, 
		LessPtr< Aula > >, LessPtr< HorarioDia > > >, LessPtr< Professor > >::iterator it1 = mapVariaveis.begin();

	for(; it1 != mapVariaveis.end(); it1++)
	{
		Professor *professor = it1->first;

		map< int, map< HorarioDia*, map< Aula*, vector< VariableOpHash::iterator >, 
				LessPtr< Aula > >, LessPtr< HorarioDia > > >::iterator it2 = it1->second.begin();

		for(; it2 != it1->second.end(); it2++)
		{
			int dia = it2->first;

			map< HorarioDia*, map< Aula*, vector< VariableOpHash::iterator >, 
					LessPtr< Aula > >, LessPtr< HorarioDia > >::iterator it3 = it2->second.begin();

			for(; it3 != it2->second.end(); it3++)
			{
				HorarioDia *horControle = it3->first;
				DateTime inicioControle = horControle->getHorarioAula()->getInicio();

				map< HorarioDia*, map< Aula*, vector< VariableOpHash::iterator >, 
							LessPtr< Aula > >, LessPtr< HorarioDia > >::iterator it4 = it2->second.begin();

				for(; it4 != it2->second.end(); it4++)
				{
					HorarioDia *hor2 = it4->first;
					DateTime inicio2 = hor2->getHorarioAula()->getInicio();

					map< Aula*, vector< VariableOpHash::iterator >, 
								LessPtr< Aula > >::iterator it5 = it3->second.begin();

					for(; it5 != it3->second.end(); it5++)
					{
						Aula *aula = it5->first;

						Unidade *unid = problemData->refUnidade[ aula->getSala()->getIdUnidade() ];
						Campus *cp = problemData->retornaCampus( unid->getId() );

						int tempoCred = aula->getDisciplina()->getTempoCredSemanaLetiva();
						int nCred = aula->getTotalCreditos();
						int duracaoAulaControle = tempoCred * nCred;

						c.reset();
						c.setType( ConstraintOp::C_PROF_HORARIO_MULTIUNID );
						c.setCampus( cp );
						c.setUnidade( unid );
						c.setProfessor( professor );
						c.setDia( dia );
						c.setHorarioAula( horControle->getHorarioAula() ); // Controle
						c.setDuracaoAula( duracaoAulaControle ); // Controle
						c.setH2( hor2->getHorarioAula() );

						cit = cHashOp.find( c );

						if ( cHashOp.find( c ) == cHashOp.end() )
						{
							sprintf( name, "%s", c.toString().c_str() );
							nnz = 100;
							OPT_ROW row( nnz, OPT_ROW::LESS, 1.0, name );

							bool inseriuUnidControle = false;
							bool inseriuUnidDiferente = false;

							map< Aula*, vector< VariableOpHash::iterator >, 
										LessPtr< Aula > >::iterator it6 = it3->second.begin();

							for(; it6 != it3->second.end(); it6++)
							{
								Aula *aula = it6->first;

								Unidade *unidAula = problemData->refUnidade[ aula->getSala()->getIdUnidade() ];
								Campus *cpAula = problemData->retornaCampus( unidAula->getId() );

								if ( cpAula->getId() == cp->getId() &&
									unidAula->getId() == unid->getId() )
								{
									Trio< int /*campusId*/, int /*turma*/, int /*discId*/ > trio;
									trio.set( cpAula->getId(), aula->getTurma(), aula->getDisciplina()->getId() );
									GGroup< HorarioDia*, LessPtr< HorarioDia > > horariosDiaAula = problemData->mapTurma_HorComunsAosTurnosIES[trio];	
									
									if ( aula->getDiaSemana() != dia ||
										horariosDiaAula.find( horControle ) == horariosDiaAula.end() )
									{
										continue;
									}

									int nCred = aula->getTotalCreditos();
									int tempoCred = aula->getDisciplina()->getTempoCredSemanaLetiva();							
									int aulaDuracao = tempoCred * nCred;

									if ( aulaDuracao != duracaoAulaControle )
									{
										continue;
									}

									vector< VariableOpHash::iterator >::iterator it7 = it6->second.begin();
									for(; it7 != it6->second.end(); it7++)
									{
										VariableOpHash::iterator vit = *it7;
										row.insert(vit->second, 1.0);
										inseriuUnidControle = true;
									}
								}
							}

							it6 = it4->second.begin();

							for(; it6 != it4->second.end(); it6++)
							{
								Aula *aula = it6->first;

								Unidade *unidAula = problemData->refUnidade[ aula->getSala()->getIdUnidade() ];
								Campus *cpAula = problemData->retornaCampus( unidAula->getId() );

								if(! ( cpAula->getId() == cp->getId() &&
									unidAula->getId() == unid->getId() ))
								{
									Trio< int /*campusId*/, int /*turma*/, int /*discId*/ > trio;
									trio.set( cpAula->getId(), aula->getTurma(), aula->getDisciplina()->getId() );
									GGroup< HorarioDia*, LessPtr< HorarioDia > > horariosDiaAula = problemData->mapTurma_HorComunsAosTurnosIES[trio];	
									
									if ( horariosDiaAula.find( hor2 ) == horariosDiaAula.end() ) // todo: nao precisa olhar 'dia' igual anteriormente?
									{
										continue;
									}

									int tempoCred = aula->getDisciplina()->getTempoCredSemanaLetiva();
									int nCred = aula->getTotalCreditos();
									int duracaoAula = tempoCred * nCred;

									int tempoMinDesloc = problemData->calculaTempoEntreCampusUnidades( cp, cpAula, unid, unidAula );

									DateTime fimControle = inicioControle;
									fimControle.addMinutes( duracaoAulaControle + tempoMinDesloc );

									DateTime fim2 = inicio2;
									fim2.addMinutes( duracaoAula + tempoMinDesloc );

									if ( ( ( fim2 > inicioControle ) &&
										( inicio2 < fimControle ) )										
										||
										( ( fimControle > inicio2 ) &&
										( fimControle < fim2 ) ) )		// TODO: tem esse teste pro fim tb??
									{
										vector< VariableOpHash::iterator >::iterator it7 = it6->second.begin();
										for(; it7 != it6->second.end(); it7++)
										{
											VariableOpHash::iterator vit = *it7;
											row.insert(vit->second, 1.0);
											inseriuUnidDiferente = true;
										}
									}			
								}
							}

							if ( inseriuUnidControle && inseriuUnidDiferente )
							{
								cHashOp[ c ] = lp->getNumRows();
								lp->addRow( row );
								restricoes++;
							}
						}
					}
				}
			}
		}
	}

	if (restricoes)
		std::cout << "\nATENCAO: Conferir essa restricao de multi-unidade. Comparacao de horarios ta ok?\n";

	return restricoes;
}

// ANTIGA! PODE DELETAR DEPOIS QUE criarRestricaoProfHiHf_ FUNCIONAR
int Operacional::criaRestricaoGapsHorariosProfessores()
{
   int restricoes = 0;

   if ( !problemData->parametros->minimizar_horarios_vazios_professor )
   {
	   return restricoes;
   }

   VariableOpHash::iterator vit;
   VariableOpHash::iterator vit_x;
   VariableOpHash::iterator vit_h;
   
   int nnz = 100; // TODO

   double rhs = 0.0;
   char name[ 200 ];

   if ( nnz == 0 )
   {
      return 0;
   }

   map< Professor*, map< int, vector< VariableOpHash::iterator > >, LessPtr< Professor > > variaveisHashHorariosIFProfessores;
   map< Professor*, map< int, vector< VariableOpHash::iterator > >, LessPtr< Professor > > variaveisHashX;

   vit = vHashOp.begin();

   for (; vit != vHashOp.end(); vit++ )
   {
      VariableOp v = ( vit->first );

	  if ( ( v.getType() == VariableOp::V_HI_PROFESSORES ) ||
		   ( v.getType() == VariableOp::V_HF_PROFESSORES ) )
      {
		  if ( !v.getProfessor()->eVirtual() )
			variaveisHashHorariosIFProfessores[v.getProfessor()][v.getDia()].push_back( vit );
      }
      else if ( v.getType() == VariableOp::V_X_PROF_AULA_HOR )
      {
		  if ( !v.getProfessor()->eVirtual() )
			variaveisHashX[v.getProfessor()][v.getDia()].push_back( vit );
      }
   }

   // Para cada professor
   map< Professor*, map< int, vector< VariableOpHash::iterator > >, LessPtr< Professor > >::iterator it1 = variaveisHashHorariosIFProfessores.begin();
   for(; it1 != variaveisHashHorariosIFProfessores.end(); it1++)
   {
	   Professor *professor = it1->first;
		
	   // Para cada dia
	   map< int, vector< VariableOpHash::iterator > >::iterator it2 = it1->second.begin();
	   for(; it2 != it1->second.end(); it2++)
	   {
		   int dia = it2->first;

		   // --------------- C_GAPS_PROFESSORES_I_F ---------------------------------------
		    
		   ConstraintOp c2;
		   c2.reset();
		   c2.setType( ConstraintOp::C_GAPS_PROFESSORES_I_F );
		   c2.setProfessor( professor );
		   c2.setDia( dia );
				
		   if ( cHashOp.find( c2 ) != cHashOp.end() )
		   {
			   continue;
		   }

		   sprintf( name, "%s", c2.toString().c_str() );

		   OPT_ROW row2( nnz, OPT_ROW::LESS, 0.0, name );

		   // Par cada variavel Hi_{p,t} ou Hf_{p,t}
		   vector< VariableOpHash::iterator >::iterator it3 = it2->second.begin();
		   for(; it3 != it2->second.end(); it3++)
		   {
			    vit_h = *it3;
			    VariableOp vh = vit_h->first;

				// --------------- insere variaveis em C_GAPS_PROFESSORES_I_F ----------------------
				
				// Insere Hi_{p,t}
				if ( vh.getType() == VariableOp::V_HI_PROFESSORES )
				{
					row2.insert( vit_h->second, 1.0 );		
				}
				// Insere Hf_{p,t}
				else
				{
					row2.insert( vit_h->second, -1.0 );
				}

				// ------------------------------------------------------------------------------

			   // Para cada horario no dia
			   ITERA_GGROUP_LESSPTR( itHor, professor->horariosDia, HorarioDia )
			   {
				   if ( itHor->getDia() != dia )
					   continue;

				   HorarioAula * h = itHor->getHorarioAula();
				
				   int horEmMinutos = h->getInicio().getDateMinutes();

				   // Variável Hi_{p,t}
				   if ( vh.getType() == VariableOp::V_HI_PROFESSORES )
				   {
					   // --------------- C_GAPS_PROFESSORES_I ---------------------------------------
					    ConstraintOp c;
						c.reset();
						c.setType( ConstraintOp::C_GAPS_PROFESSORES_I );
						c.setProfessor( professor );
						c.setDia( dia );
						c.setHorarioAula( h );

						double bigM = professor->getUltimoHorarioDisponivelDia( dia )->getInicio().getDateMinutes() + h->getTempoAula();						
						
						rhs = (double) horEmMinutos;
						rhs += bigM;

						if ( cHashOp.find( c ) == cHashOp.end() )
						{
							sprintf( name, "%s", c.toString().c_str() );

							OPT_ROW row( nnz, OPT_ROW::LESS, rhs, name );

							// Insere Hi_{p,t}
							row.insert( vit_h->second, 1.0 );					   	   

							bool inseriuVariavel = false;

							vector< VariableOpHash::iterator > vars = variaveisHashX[professor][dia];
							vector< VariableOpHash::iterator >::iterator it4 = vars.begin();

							for(; it4 != vars.end(); it4++)
							{
								vit_x = *it4;
								VariableOp v_x = vit_x->first;

								if ( v_x.getHorario()->getHorarioAula()->getId() == h->getId() )
								{
									row.insert( vit_x->second, bigM );			   
									inseriuVariavel = true;
								}
							}

							if ( inseriuVariavel )
							{
								cHashOp[ c ] = lp->getNumRows();
								lp->addRow( row );
								restricoes++;
							}
						}
						// --------------------------------------------------------------------
				   }

				   // Variável Hf_{p,t}
				   else if ( vh.getType() == VariableOp::V_HF_PROFESSORES )
				   {
					   // --------------- C_GAPS_PROFESSORES_F ---------------------------------------
						ConstraintOp c;
					    c.reset();
						c.setType( ConstraintOp::C_GAPS_PROFESSORES_F );
						c.setProfessor( professor );
						c.setDia( dia );
						c.setHorarioAula( h );

						rhs = 0.0;

						if ( cHashOp.find( c ) == cHashOp.end() )
						{
							sprintf( name, "%s", c.toString().c_str() );

							OPT_ROW row( nnz, OPT_ROW::GREATER, rhs, name );

							// Insere Hf_{p,t}
							row.insert( vit_h->second, 1.0 );

							bool inseriuVariavel = false;

							vector< VariableOpHash::iterator > vars = variaveisHashX[professor][dia];
							vector< VariableOpHash::iterator >::iterator it4 = vars.begin();

							for(; it4 != vars.end(); it4++)
							{
								vit_x = *it4;
								VariableOp v_x = vit_x->first;

								if ( v_x.getHorario()->getHorarioAula()->getId() == h->getId() )
								{
									row.insert( vit_x->second, -horEmMinutos );							   
									inseriuVariavel = true;
								}
							}

							if ( inseriuVariavel )
							{
								cHashOp[ c ] = lp->getNumRows();
								lp->addRow( row );
								restricoes++;
							}
						}
					   // ------------------------------------------------------------------
				   }			  
			   }
		   }

		   if ( row2.getnnz() > 0 )
		   {
				cHashOp[ c2 ] = lp->getNumRows();
				lp->addRow( row2 );
				restricoes++;
		   }
		    // --------------------------------------------------------------------------------
	   }
   }

   return restricoes;
}

int Operacional::criaRestricaoCalculaNroProfsAlocadosCurso()
{
   int restricoes = 0;  
      
  // if ( ! problemData->parametros->min_mestres &&
	 //   ! problemData->parametros->min_doutores )
  // {
		//return restricoes;
  // }

   int nnz;
   double rhs;
   char name[ 200 ];

   ConstraintOp c;
   VariableOpHash::iterator vit;
   ConstraintOpHash::iterator cit;
   VariableOp v;

   GGroup< Professor *, LessPtr< Professor > > professores
      = problemData->getProfessores();

   // Agrupando os professores que ministram disciplinas de cada curso
   std::map< Curso *, GGroup< Professor *,
      LessPtr< Professor > > > mapCursoProfessores;

   ITERA_GGROUP_LESSPTR( it_prof, professores, Professor )
   {
      Professor * professor = ( *it_prof );

	  if ( professor->eVirtual() ) continue;

      ITERA_GGROUP_LESSPTR( it_aula, problemData->aulas, Aula )
      {
         Aula * aula = ( *it_aula );
         Disciplina * disciplina = aula->getDisciplina();

         std::pair< int, int > professor_disciplina (
            professor->getId(), disciplina->getId() );

         // Se o professor e a disciplina da aula em questão se relacionarem
         if ( problemData->prof_Disc_Dias.find( professor_disciplina )
            == problemData->prof_Disc_Dias.end() )
         {
            continue;
         }

         ITERA_GGROUP_LESSPTR( it_oferta, aula->ofertas, Oferta )
         {
            Oferta * oferta = ( *it_oferta );

            mapCursoProfessores[ oferta->curso ].add( professor );
         }
      }
   }


   ITERA_GGROUP_LESSPTR( itCurso, problemData->cursos, Curso )
   {
	   Curso *curso = *itCurso;
	   	   
	   c.reset();
	   c.setType( ConstraintOp::C_CALCULA_NRO_PROFS_CURSO );
	   c.setCurso( curso );

	   cit = cHashOp.find( c );

		if ( cit == cHashOp.end() )
		{		
			GGroup< Professor *, LessPtr< Professor > > profs;

			std::map< Curso *, GGroup< Professor *, LessPtr< Professor > > >::iterator
				itCursoProfs = mapCursoProfessores.find(curso);		
			if ( itCursoProfs != mapCursoProfessores.end() )
			{
				profs = itCursoProfs->second;
			}

			sprintf( name, "%s", c.toString().c_str() );
			nnz = profs.size() + 1;
			rhs = 0;

			OPT_ROW row( nnz, OPT_ROW::EQUAL, rhs, name );

			// Itera todos os professores hábeis para o curso
			ITERA_GGROUP_LESSPTR( it_prof, profs, Professor )
			{
				Professor * professor = ( *it_prof );

				// Recupera os professores que estão associados ao curso

				// Variavel w_{p,c}
				v.reset();
				v.setType( VariableOp::V_PROF_CURSO );
				v.setProfessor( professor );
				v.setCurso( curso );
						
				vit = vHashOp.find( v );

				if ( vit != vHashOp.end() )
				{
					row.insert( vit->second, -1.0 );
				}
			}      
					
			// Variavel np_{c}
			v.reset();
			v.setType( VariableOp::V_NRO_PROFS_CURSO );
			v.setCurso( curso );
			vit = vHashOp.find( v );

			if ( vit != vHashOp.end() )
			{
				row.insert( vit->second, 1.0 );
			}

			// Insere restrição
			if ( row.getnnz() != 0 )
			{
				cHashOp[ c ] = lp->getNumRows();
				lp->addRow( row );
				restricoes++;
			}
		}
   }


   return restricoes;
}

// SÓ PARA RODADA 2
int Operacional::criaRestricaoCalculaNroProfsVirtGeraisAlocadosCurso()
{
  int restricoes = 0;  
      
   if ( ! problemData->parametros->min_doutores &&
	    ! problemData->parametros->min_mestres )
   {
		return restricoes;
   }

   int nnz;
   double rhs;
   char name[ 200 ];

   ConstraintOp c;
   VariableOpHash::iterator vit;
   ConstraintOpHash::iterator cit;
   VariableOp v;
      
   vit = vHashOp.begin();

	for (; vit != vHashOp.end(); vit++)
	{
		VariableOp v = ( vit->first );
				
		double coef = 0.0;
		TipoContrato* contrato;

		if (v.getType() == VariableOp::V_PROF_CURSO) // w_{p,c}
		{
			if ( ! v.getProfessor()->eVirtual() ) continue;
			if ( v.getProfessor()->getTitulacao() == TipoTitulacao::Doutor ||
				 v.getProfessor()->getTitulacao() == TipoTitulacao::Mestre ) continue;
			coef = 1.0;
			contrato = v.getProfessor()->tipo_contrato;
		}
		else if(v.getType() == VariableOp::V_NRO_PROFS_VIRTUAIS_GERAIS_CURSO) // npvg_{c,r}
		{
			coef = -1.0;
			contrato = v.getContrato();
		}
		else
		{
			continue;
		}

		c.reset();
		c.setType( ConstraintOp::C_CALCULA_NRO_PROF_VIRT_GERAIS_CURSO );
		c.setCurso( v.getCurso() );
		c.setContrato( contrato );

		cit = cHashOp.find(c);
		if ( cit == cHashOp.end() )
		{
			cHashOp[ c ] = lp->getNumRows();

			sprintf( name, "%s", c.toString().c_str() );
			rhs = 0;
			OPT_ROW row( 50, OPT_ROW::EQUAL, rhs, name );

			row.insert( vit->second, coef );

			lp->addRow( row );
			restricoes++;
		}
		else
		{
			lp->chgCoef(cit->second, vit->second, coef );
		}
	}

   return restricoes;
}

// SÓ PARA RODADA 2
int Operacional::criaRestricaoCalculaNroProfsVirtDoutAlocadosCurso()
{
  int restricoes = 0;  
      
   if ( ! problemData->parametros->min_doutores )
   {
		return restricoes;
   }

   int nnz;
   double rhs;
   char name[ 200 ];

   ConstraintOp c;
   VariableOpHash::iterator vit;
   ConstraintOpHash::iterator cit;
   VariableOp v;
      
   vit = vHashOp.begin();

	for (; vit != vHashOp.end(); vit++)
	{
		VariableOp v = ( vit->first );
				
		double coef = 0.0;
		TipoContrato* contrato;

		if (v.getType() == VariableOp::V_PROF_CURSO) // w_{p,c}
		{
			if ( ! v.getProfessor()->eVirtual() ) continue;
			if ( v.getProfessor()->getTitulacao() != TipoTitulacao::Doutor ) continue;
			coef = 1.0;
			contrato = v.getProfessor()->tipo_contrato;
		}
		else if(v.getType() == VariableOp::V_NRO_PROFS_VIRTUAIS_DOUT_CURSO) // npvd_{c}
		{
			coef = -1.0;
			contrato = v.getContrato();
		}
		else
		{
			continue;
		}

		c.reset();
		c.setType( ConstraintOp::C_CALCULA_NRO_PROF_VIRT_DOUT_CURSO );
		c.setCurso( v.getCurso() );
		c.setContrato( contrato );

		cit = cHashOp.find(c);
		if ( cit == cHashOp.end() )
		{
			cHashOp[ c ] = lp->getNumRows();

			sprintf( name, "%s", c.toString().c_str() );
			rhs = 0;
			OPT_ROW row( 50, OPT_ROW::EQUAL, rhs, name );

			row.insert( vit->second, coef );

			lp->addRow( row );
			restricoes++;
		}
		else
		{
			lp->chgCoef(cit->second, vit->second, coef );
		}
	}

   return restricoes;
}

// SÓ PARA RODADA 2
int Operacional::criaRestricaoCalculaNroProfsVirtMestAlocadosCurso()
{
   int restricoes = 0;  
      
   if ( ! problemData->parametros->min_mestres )
   {
		return restricoes;
   }

   int nnz;
   double rhs;
   char name[ 200 ];

   ConstraintOp c;
   VariableOpHash::iterator vit;
   ConstraintOpHash::iterator cit;
   VariableOp v;
      
   vit = vHashOp.begin();

	for (; vit != vHashOp.end(); vit++)
	{
		VariableOp v = ( vit->first );
		TipoContrato* contrato;

		double coef = 0.0;

		if (v.getType() == VariableOp::V_PROF_CURSO) // w_{p,c}
		{
			if ( ! v.getProfessor()->eVirtual() ) continue;
			if ( v.getProfessor()->getTitulacao() != TipoTitulacao::Mestre ) continue;
			coef = 1.0;
			contrato = v.getProfessor()->tipo_contrato;
		}
		else if(v.getType() == VariableOp::V_NRO_PROFS_VIRTUAIS_MEST_CURSO) // npvm_{c}
		{
			coef = -1.0;
			contrato = v.getContrato();
		}
		else
		{
			continue;
		}

		c.reset();
		c.setType( ConstraintOp::C_CALCULA_NRO_PROF_VIRT_MEST_CURSO );
		c.setCurso( v.getCurso() );
		c.setContrato( contrato );

		cit = cHashOp.find(c);
		if ( cit == cHashOp.end() )
		{
			cHashOp[ c ] = lp->getNumRows();

			sprintf( name, "%s", c.toString().c_str() );
			rhs = 0;
			OPT_ROW row( 50, OPT_ROW::EQUAL, rhs, name );

			row.insert( vit->second, coef );

			lp->addRow( row );
			restricoes++;
		}
		else
		{
			lp->chgCoef(cit->second, vit->second, coef );
		}
	}

   return restricoes;
}

// SÓ PARA RODADA 2
int Operacional::criaRestricaoCalculaNroProfsVirtAlocadosCurso()
{
   int restricoes = 0;  
      
  // if ( ! problemData->parametros->min_mestres && ! problemData->parametros->min_doutores )
  // {
		//return restricoes;
  // }

   int nnz;
   double rhs;
   char name[ 200 ];

   ConstraintOp c;
   VariableOpHash::iterator vit;
   ConstraintOpHash::iterator cit;
   VariableOp v;
      
   vit = vHashOp.begin();

	for (; vit != vHashOp.end(); vit++)
	{
		VariableOp v = ( vit->first );
				
		double coef = 0.0;
		TipoContrato* contrato;

		if (v.getType() == VariableOp::V_PROF_CURSO) // w_{p,c}
		{
			if ( ! v.getProfessor()->eVirtual() ) continue;
			coef = 1.0;
			contrato = v.getProfessor()->tipo_contrato;
		}
		else if(v.getType() == VariableOp::V_NRO_PROFS_VIRTUAIS_CURSO) // npv_{c,r}
		{
			coef = -1.0;
			contrato = v.getContrato();
		}
		else
		{
			continue;
		}

		c.reset();
		c.setType( ConstraintOp::C_CALCULA_NRO_PROF_VIRT_CURSO );
		c.setCurso( v.getCurso() );
		c.setContrato( contrato );

		cit = cHashOp.find(c);
		if ( cit == cHashOp.end() )
		{
			cHashOp[ c ] = lp->getNumRows();

			sprintf( name, "%s", c.toString().c_str() );
			rhs = 0;
			OPT_ROW row( 50, OPT_ROW::EQUAL, rhs, name );

			row.insert( vit->second, coef );

			lp->addRow( row );
			restricoes++;
		}
		else
		{
			lp->chgCoef(cit->second, vit->second, coef );
		}
	}

   return restricoes;
}

// SÓ PARA RODADA 1
int Operacional::criaRestricaoEstimaNroProfsVirtuaisAlocadosCurso()
{
   int restricoes = 0;  
   
   int nnz;
   double rhs;
   char name[ 200 ];

   ConstraintOp c;
   VariableOpHash::iterator vit;
   ConstraintOpHash::iterator cit;

   // TODO: usar esse map por questao de eficiencia
 //  std::map< Curso*,  std::map< TipoContrato*, std::map< int /*dia*/, std::map< DateTime /*dti*/, 
	//   std::map< DateTime /*dtf*/, VariableOpHash::iterator > > >, LessPtr<TipoContrato> >, LessPtr<Curso> > mapCursoContrDiaDtiDtf;

	//for ( vit = vHashOp.begin(); vit != vHashOp.end(); vit++ )
	//{
	//	VariableOp v = vit->first;
	//	
	//	if ( v.getType() != VariableOp::V_X_PROF_AULA_HOR )
	//		continue;	
	//	if ( ! v.getProfessor()->eVirtual() )
	//		continue;
	//	
	//	TipoContrato* contrato = v.getProfessor()->tipo_contrat;
	//	std::map< Curso*, int > *mapCursoAtendidos = v.getAula()->getMapCursosAtendidos();
	//	int dia = v.getDia();
	//	HorarioAula *hi = v.getHorarioAula();

	//	int nCreds = v.getAula()->getTotalCreditos();
	//	Calendario *calendario = hai->getCalendario();
	//	HorarioAula *hf = calendario->getHorarioMaisNCreds(hai, nCreds-1);
	//	DateTime *dtf = problemData->getDateTimeFinal( hf->getFinal() );
	//	if ( dtf==NULL )
	//	{
	//		std::cout << "\nErro: dtf Null para final = " << hf->getFinal()
	//				<< " e horario aula id = " << hf->getId();
	//		continue;
	//	}
	//	
	//	std::map< Curso*, int >::iterator itCurso = mapCursoAtendidos->begin();
	//	for( ; itCurso != mapCursoAtendidos->end(); itCurso++ )
	//	{	
	//		Curso *curso = itCurso->first;

	//		mapCursoContrDiaDtiDtf[curso][contrato][dia][hi->getInicio()][hf->getFinal()] = vit;
	//	}
	//} 


   ITERA_GGROUP_LESSPTR( itHorDia, problemData->horariosDia, HorarioDia )
   {
	   int dia = (*itHorDia)->getDia();
	   HorarioAula *horarioAula = (*itHorDia)->getHorarioAula();

	   ITERA_GGROUP_LESSPTR( itCurso, problemData->cursos, Curso )
	   {
		   Curso *curso = *itCurso;

		   ITERA_GGROUP_LESSPTR( itContr, problemData->tipos_contrato, TipoContrato )
		   {
			   TipoContrato *contrato = *itContr;
	   	   
			   c.reset();
			   c.setType( ConstraintOp::C_ESTIMA_NRO_PROFS_VIRTUAIS_CURSO );
			   c.setCurso( curso );
			   c.setDia( dia );
			   c.setHorarioAula( horarioAula );
			   c.setContrato( contrato );

			   cit = cHashOp.find( c );

				if ( cit == cHashOp.end() )
				{
					sprintf( name, "%s", c.toString().c_str() );
					nnz = problemData->profsVirtuais.size() * problemData->horariosDia.size() + 1;
					rhs = 0;

					OPT_ROW row( nnz, OPT_ROW::LESS, rhs, name );
					   				  
					for ( vit = vHashOp.begin(); vit != vHashOp.end(); vit++ )
					{
						VariableOp v;
						v.reset();
						v = ( vit->first );
						if ( v.getType() == VariableOp::V_X_PROF_AULA_HOR &&
							 v.getProfessor()->eVirtual() &&
							 v.getProfessor()->tipo_contrato == contrato &&
							 v.getDia() == dia &&
							 v.getAula()->atendeAoCurso( curso->getId() ) )
						{	
							HorarioAula *h = v.getHorarioAula();
							Calendario *sl = h->getCalendario();
							for ( int i=1; i<=v.getAula()->getTotalCreditos(); i++ )
							{
								if ( h->sobrepoe( *horarioAula ) )
								{
									row.insert( vit->second, 1.0 );
									break;
								}
								h = sl->getProximoHorario( h );
							}
						}					
					}      

					if ( problemData->parametros->considerarDescansoMinProf )
					{
						double tempoMinimoDescanso = problemData->parametros->descansoMinProfValue;	// em horas

						if ( tempoMinimoDescanso > 24 ) 
							std::cout<<"TODO: a comparacao de dias de DateTime nao "
								<<"esta considerando interjornada de mais de 24hs. Consertar.";

					    tempoMinimoDescanso *= 60;													// em minutos
						
						DateTime dtf_max = horarioAula->getInicio();						
						dtf_max.subMinutes( tempoMinimoDescanso );
						
						// A restrição é somente para a ultima aula de um dia e a primeira aula do dia SEGUINTE
						if ( dtf_max.getDay() == horarioAula->getInicio().getDay() )
							continue;
			
						//std::cout<<"\n\nComecei dia "<<dia;
						//std::cout<<"\ndtf_max="<<dtf_max;
						//std::cout<<"\ndia anterior+1 = "<< dtf_max.getDay() + 1;
						//std::cout<<"\ndia seguinte = "<<horarioAula->getInicio().getDay();

						for ( vit = vHashOp.begin(); vit != vHashOp.end(); vit++ )
						{
							VariableOp v;
							v.reset();
							v = ( vit->first );											
						
							if ( v.getType() == VariableOp::V_X_PROF_AULA_HOR &&
								 v.getProfessor()->eVirtual() &&
								 v.getProfessor()->tipo_contrato == contrato &&
								 v.getDia() == dia-1 &&
								 v.getAula()->atendeAoCurso( curso->getId() ) )
							{	
								HorarioAula *h = v.getHorarioAula();
								Calendario *sl = h->getCalendario();
								
								//std::cout<<"\n" << v.toString();
								//std::cout<<"\th " << h->getInicio();
								//std::cout<<"\tnumCred " << v.getAula()->getTotalCreditos();
								HorarioAula *hf = h;
								DateTime dtfAula;
								int n = 1;
								while ( hf!=NULL && n<=v.getAula()->getTotalCreditos() )
								{
									dtfAula = hf->getFinal();
									hf = sl->getProximoHorario( hf );
									n++;
								}	
								n--;

								if( n!=v.getAula()->getTotalCreditos() )
								{
									std::cout<<"\nErro, nro de horarios nao suficientes.";
									continue;
								}
																
								//std::cout<<"\tdtfAula "<<dtfAula;

								if (  dtf_max.earlierTime(dtfAula) )
								{
									row.insert( vit->second, 1.0 );
								}
							}										
						}
					}

					// Variavel npv_{c}
					VariableOp v;
					v.reset();
					v.setType( VariableOp::V_NRO_PROFS_VIRTUAIS_CURSO );
					v.setCurso( curso );
					v.setContrato( contrato );
					vit = vHashOp.find( v );
					if ( vit != vHashOp.end() )
					{
						row.insert( vit->second, -1.0 );
					}

					// Insere restrição
					if ( row.getnnz() > 1 )
					{
						cHashOp[ c ] = lp->getNumRows();
						lp->addRow( row );
						restricoes++;
					}
				}
		   }
	   }

   }

   return restricoes;
}

// SÓ PARA RODADA 1
int Operacional::criaRestricaoEstimaNroProfsVirtuaisDoutoresAlocadosCurso()
{
   int restricoes = 0;  
      
   if ( ! problemData->parametros->min_doutores )
   {
		return restricoes;
   }

   int nnz;
   double rhs;
   char name[ 200 ];

   ConstraintOp c;
   VariableOpHash::iterator vit;
   ConstraintOpHash::iterator cit;
   VariableOp v;

   ITERA_GGROUP_LESSPTR( itHorDia, problemData->horariosDia, HorarioDia )
   {
	   int dia = (*itHorDia)->getDia();
	   HorarioAula *horarioAula = (*itHorDia)->getHorarioAula();

	   ITERA_GGROUP_LESSPTR( itCurso, problemData->cursos, Curso )
	   {
		   Curso *curso = *itCurso;

		   ITERA_GGROUP_LESSPTR( itContr, problemData->tipos_contrato, TipoContrato )
		   {
			   TipoContrato *contrato = *itContr; 	   
	   	   
			   c.reset();
			   c.setType( ConstraintOp::C_ESTIMA_NRO_PROFS_VIRTUAIS_DOUT_CURSO );
			   c.setCurso( curso );
			   c.setDia( dia );
			   c.setHorarioAula( horarioAula );
			   c.setContrato( contrato );

			   cit = cHashOp.find( c );

				if ( cit == cHashOp.end() )
				{
					sprintf( name, "%s", c.toString().c_str() );
					nnz = problemData->profsVirtuais.size() * problemData->horariosDia.size() + 1;
					rhs = 0;

					OPT_ROW row( nnz, OPT_ROW::LESS, rhs, name );
					   				  
					for ( vit = vHashOp.begin(); vit != vHashOp.end(); vit++ )
					{
						v.reset();
						v = ( vit->first );
						if ( v.getType() == VariableOp::V_X_PROF_AULA_HOR &&
							 v.getProfessor()->eVirtual() &&
							 v.getProfessor()->tipo_contrato == contrato &&
							 v.getProfessor()->getTitulacao() == TipoTitulacao::Doutor &&
							 v.getDia() == dia &&
							 v.getAula()->atendeAoCurso( curso->getId() ) )
						{	
							HorarioAula *h = v.getHorarioAula();
							Calendario *sl = h->getCalendario();
							for ( int i=1; i<=v.getAula()->getTotalCreditos(); i++ )
							{
								if ( h->sobrepoe( *horarioAula ) )
								{
									row.insert( vit->second, 1.0 );
									break;
								}
								h = sl->getProximoHorario( h );
							}
						}					
					}      

					if ( problemData->parametros->considerarDescansoMinProf )
					{
						double tempoMinimoDescanso = problemData->parametros->descansoMinProfValue;	// em horas
					    tempoMinimoDescanso *= 60;													// em minutos
					
						DateTime dtf_max = horarioAula->getInicio();					
						dtf_max.subMinutes( tempoMinimoDescanso );
						
						// A restrição é somente para a ultima aula de um dia e a primeira aula do dia SEGUINTE
						if ( dtf_max.getDay() == horarioAula->getInicio().getDay() )
							continue;
			
						for ( vit = vHashOp.begin(); vit != vHashOp.end(); vit++ )
						{
							VariableOp v;
							v.reset();
							v = ( vit->first );											
						
							if ( v.getType() == VariableOp::V_X_PROF_AULA_HOR &&
								 v.getProfessor()->eVirtual() &&
								 v.getProfessor()->tipo_contrato == contrato &&
								 v.getProfessor()->getTitulacao() == TipoTitulacao::Doutor &&
								 v.getDia() == dia-1 &&
								 v.getAula()->atendeAoCurso( curso->getId() ) )
							{	
								HorarioAula *h = v.getHorarioAula();
								Calendario *sl = h->getCalendario();								
								HorarioAula *hf = h;
								DateTime dtfAula;
								int n = 1;
								while ( hf!=NULL && n<=v.getAula()->getTotalCreditos() )
								{
									dtfAula = hf->getFinal();
									hf = sl->getProximoHorario( hf );
									n++;
								}	
								n--;

								if( n!=v.getAula()->getTotalCreditos() )
								{
									std::cout<<"\nErro, nro de horarios nao suficientes.";
									continue;
								}
								
								if (  dtf_max.earlierTime(dtfAula) )
								{
									row.insert( vit->second, 1.0 );
								}
							}										
						}
					}
					
					// Variavel npvd_{c,r}
					v.reset();
					v.setType( VariableOp::V_NRO_PROFS_VIRTUAIS_DOUT_CURSO );
					v.setCurso( curso );
					v.setContrato( contrato );
					vit = vHashOp.find( v );

					if ( vit != vHashOp.end() )
					{
						row.insert( vit->second, -1.0 );
					}

					// Insere restrição
					if ( row.getnnz() > 1 )
					{
						cHashOp[ c ] = lp->getNumRows();
						lp->addRow( row );
						restricoes++;
					}
				}
		   }
	   }

   }

   return restricoes;
}

// SÓ PARA RODADA 1
int Operacional::criaRestricaoEstimaNroProfsVirtuaisMestresAlocadosCurso()
{
   int restricoes = 0;  
      
   if ( ! problemData->parametros->min_mestres )
   {
		return restricoes;
   }

   int nnz;
   double rhs;
   char name[ 200 ];

   ConstraintOp c;
   VariableOpHash::iterator vit;
   ConstraintOpHash::iterator cit;
   VariableOp v;

   ITERA_GGROUP_LESSPTR( itHorDia, problemData->horariosDia, HorarioDia )
   {
	   int dia = (*itHorDia)->getDia();
	   HorarioAula *horarioAula = (*itHorDia)->getHorarioAula();

	   ITERA_GGROUP_LESSPTR( itCurso, problemData->cursos, Curso )
	   {
		   Curso *curso = *itCurso;
	   	   
		   ITERA_GGROUP_LESSPTR( itContr, problemData->tipos_contrato, TipoContrato )
		   {
			   TipoContrato *contrato = *itContr;

			   c.reset();
			   c.setType( ConstraintOp::C_ESTIMA_NRO_PROFS_VIRTUAIS_MEST_CURSO );
			   c.setCurso( curso );
			   c.setDia( dia );
			   c.setHorarioAula( horarioAula );
			   c.setContrato( contrato );

			   cit = cHashOp.find( c );

				if ( cit == cHashOp.end() )
				{
					sprintf( name, "%s", c.toString().c_str() );
					nnz = problemData->profsVirtuais.size() * problemData->horariosDia.size() + 1;
					rhs = 0;

					OPT_ROW row( nnz, OPT_ROW::LESS, rhs, name );
					   				  
					for ( vit = vHashOp.begin(); vit != vHashOp.end(); vit++ )
					{
						v.reset();
						v = ( vit->first );
						if ( v.getType() == VariableOp::V_X_PROF_AULA_HOR &&
							 v.getProfessor()->eVirtual() &&
							 v.getProfessor()->tipo_contrato == contrato &&
							 v.getProfessor()->getTitulacao() == TipoTitulacao::Mestre &&
							 v.getDia() == dia &&
							 v.getAula()->atendeAoCurso( curso->getId() ) )
						{	
							HorarioAula *h = v.getHorarioAula();
							Calendario *sl = h->getCalendario();
							for ( int i=1; i<=v.getAula()->getTotalCreditos(); i++ )
							{
								if ( h->sobrepoe( *horarioAula ) )
								{
									row.insert( vit->second, 1.0 );
									break;
								}
								h = sl->getProximoHorario( h );
							}
						}					
					}      

					if ( problemData->parametros->considerarDescansoMinProf )
					{
						double tempoMinimoDescanso = problemData->parametros->descansoMinProfValue;	// em horas
					    tempoMinimoDescanso *= 60;													// em minutos
					
						DateTime dtf_max = horarioAula->getInicio();					
						dtf_max.subMinutes( tempoMinimoDescanso );
						
						// A restrição é somente para a ultima aula de um dia e a primeira aula do dia SEGUINTE
						if ( dtf_max.getDay() == horarioAula->getInicio().getDay() )
							continue;
			
						for ( vit = vHashOp.begin(); vit != vHashOp.end(); vit++ )
						{
							VariableOp v;
							v.reset();
							v = ( vit->first );											
						
							if ( v.getType() == VariableOp::V_X_PROF_AULA_HOR &&
								 v.getProfessor()->eVirtual() &&
								 v.getProfessor()->tipo_contrato == contrato &&
								 v.getProfessor()->getTitulacao() == TipoTitulacao::Mestre &&
								 v.getDia() == dia-1 &&
								 v.getAula()->atendeAoCurso( curso->getId() ) )
							{	
								HorarioAula *h = v.getHorarioAula();
								Calendario *sl = h->getCalendario();								
								HorarioAula *hf = h;
								DateTime dtfAula;
								int n = 1;
								while ( hf!=NULL && n<=v.getAula()->getTotalCreditos() )
								{
									dtfAula = hf->getFinal();
									hf = sl->getProximoHorario( hf );
									n++;
								}	
								n--;

								if( n!=v.getAula()->getTotalCreditos() )
								{
									std::cout<<"\nErro, nro de horarios nao suficientes.";
									continue;
								}
								
								if (  dtf_max.earlierTime(dtfAula) )
								{
									row.insert( vit->second, 1.0 );
								}
							}										
						}
					}
				
					// Variavel npvm_{c,r}
					v.reset();
					v.setType( VariableOp::V_NRO_PROFS_VIRTUAIS_MEST_CURSO );
					v.setCurso( curso );
					v.setContrato( contrato );
					vit = vHashOp.find( v );

					if ( vit != vHashOp.end() )
					{
						row.insert( vit->second, -1.0 );
					}

					// Insere restrição
					if ( row.getnnz() > 1 )
					{
						cHashOp[ c ] = lp->getNumRows();
						lp->addRow( row );
						restricoes++;
					}
				}
		   }
	   }

   }

   return restricoes;
}

// SÓ PARA RODADA 1
int Operacional::criaRestricaoEstimaNroProfsVirtuaisGeraisAlocadosCurso()
 {
    int restricoes = 0;  
      
   if ( ! problemData->parametros->min_mestres &&
	    ! problemData->parametros->min_doutores )
   {
		return restricoes;
   }

   int nnz;
   double rhs;
   char name[ 200 ];

   ConstraintOp c;
   VariableOpHash::iterator vit;
   ConstraintOpHash::iterator cit;
   VariableOp v;

   ITERA_GGROUP_LESSPTR( itHorDia, problemData->horariosDia, HorarioDia )
   {
	   int dia = (*itHorDia)->getDia();
	   HorarioAula *horarioAula = (*itHorDia)->getHorarioAula();

	   ITERA_GGROUP_LESSPTR( itCurso, problemData->cursos, Curso )
	   {
		   Curso *curso = *itCurso;

		   ITERA_GGROUP_LESSPTR( itContr, problemData->tipos_contrato, TipoContrato )
		   {
			   TipoContrato *contrato = *itContr;
	   	   
				c.reset();
				c.setType( ConstraintOp::C_ESTIMA_NRO_PROFS_VIRTUAIS_GERAIS_CURSO );
				c.setCurso( curso );
				c.setDia( dia );
				c.setHorarioAula( horarioAula );
				c.setContrato( contrato );

				cit = cHashOp.find( c );

				if ( cit == cHashOp.end() )
				{
					sprintf( name, "%s", c.toString().c_str() );
					nnz = problemData->profsVirtuais.size() * problemData->horariosDia.size() + 1;
					rhs = 0;

					OPT_ROW row( nnz, OPT_ROW::LESS, rhs, name );
					   				  
					for ( vit = vHashOp.begin(); vit != vHashOp.end(); vit++ )
					{
						v.reset();
						v = ( vit->first );
						if ( v.getType() == VariableOp::V_X_PROF_AULA_HOR &&
								v.getProfessor()->eVirtual() &&
								v.getProfessor()->tipo_contrato == contrato &&
								v.getProfessor()->getTitulacao() < TipoTitulacao::Mestre &&
								v.getDia() == dia &&
								v.getAula()->atendeAoCurso( curso->getId() ) )
						{	
							HorarioAula *h = v.getHorarioAula();
							Calendario *sl = h->getCalendario();
							for ( int i=1; i<=v.getAula()->getTotalCreditos(); i++ )
							{
								if ( h->sobrepoe( *horarioAula ) )
								{
									row.insert( vit->second, 1.0 );
									break;
								}
								h = sl->getProximoHorario( h );
							}
						}					
					}      
					
					if ( problemData->parametros->considerarDescansoMinProf )
					{
						double tempoMinimoDescanso = problemData->parametros->descansoMinProfValue;	// em horas
					    tempoMinimoDescanso *= 60;													// em minutos
					
						DateTime dtf_max = horarioAula->getInicio();					
						dtf_max.subMinutes( tempoMinimoDescanso );
						
						// A restrição é somente para a ultima aula de um dia e a primeira aula do dia SEGUINTE
						if ( dtf_max.getDay() == horarioAula->getInicio().getDay() )
							continue;
			
						for ( vit = vHashOp.begin(); vit != vHashOp.end(); vit++ )
						{
							VariableOp v;
							v.reset();
							v = ( vit->first );											
						
							if ( v.getType() == VariableOp::V_X_PROF_AULA_HOR &&
								 v.getProfessor()->eVirtual() &&
								 v.getProfessor()->tipo_contrato == contrato &&
 								 v.getProfessor()->getTitulacao() < TipoTitulacao::Mestre &&
								 v.getDia() == dia-1 &&
								 v.getAula()->atendeAoCurso( curso->getId() ) )
							{	
								HorarioAula *h = v.getHorarioAula();
								Calendario *sl = h->getCalendario();								
								HorarioAula *hf = h;
								DateTime dtfAula;
								int n = 1;
								while ( hf!=NULL && n<=v.getAula()->getTotalCreditos() )
								{
									dtfAula = hf->getFinal();
									hf = sl->getProximoHorario( hf );
									n++;
								}	
								n--;

								if( n!=v.getAula()->getTotalCreditos() )
								{
									std::cout<<"\nErro, nro de horarios nao suficientes.";
									continue;
								}
								
								if (  dtf_max.earlierTime(dtfAula) )
								{
									row.insert( vit->second, 1.0 );
								}
							}										
						}
					}

					// Variavel npvg_{c,r}
					v.reset();
					v.setType( VariableOp::V_NRO_PROFS_VIRTUAIS_GERAIS_CURSO );
					v.setCurso( curso );
					v.setContrato( contrato );
					vit = vHashOp.find( v );

					if ( vit != vHashOp.end() )
					{
						row.insert( vit->second, -1.0 );
					}

					// Insere restrição
					if ( row.getnnz() > 1 )
					{
						cHashOp[ c ] = lp->getNumRows();
						lp->addRow( row );
						restricoes++;
					}
				}
		   }
	   }

   }

   return restricoes;
 }

// RODADAS 1 E 2
int Operacional::criaRestricaoSomaNroProfsVirtuaisAlocadosCurso()
 {
    int restricoes = 0;  
      
	if ( ! problemData->parametros->min_mestres &&
		 ! problemData->parametros->min_doutores )
	{
		return restricoes;
	}

	int nnz;
	double rhs;
	char name[ 200 ];

	ConstraintOp c;
	VariableOpHash::iterator vit;
	ConstraintOpHash::iterator cit;
	VariableOp v;

	ITERA_GGROUP_LESSPTR( itCurso, problemData->cursos, Curso )
	{
		Curso *curso = *itCurso;

		ITERA_GGROUP_LESSPTR( itContr, problemData->tipos_contrato, TipoContrato )
		{
			TipoContrato *contrato = *itContr;

			c.reset();
			c.setType( ConstraintOp::C_SOMA_NRO_PROFS_VIRTUAIS_CURSO );
			c.setCurso( curso );
			c.setContrato( contrato );

			cit = cHashOp.find( c );

			if ( cit == cHashOp.end() )
			{
				sprintf( name, "%s", c.toString().c_str() );
				nnz = problemData->profsVirtuais.size() * problemData->horariosDia.size() + 1;
				rhs = 0;

				OPT_ROW row( nnz, OPT_ROW::EQUAL, rhs, name );				   				  

				// Variavel npvg_{c,r}
				v.reset();
				v.setType( VariableOp::V_NRO_PROFS_VIRTUAIS_GERAIS_CURSO );
				v.setCurso( curso );
				v.setContrato( contrato );
				vit = vHashOp.find( v );

				if ( vit != vHashOp.end() )
				{
					row.insert( vit->second, 1.0 );
				}

				// Variavel npvm_{c,r}
				v.reset();
				v.setType( VariableOp::V_NRO_PROFS_VIRTUAIS_MEST_CURSO );
				v.setCurso( curso );
				v.setContrato( contrato );
				vit = vHashOp.find( v );

				if ( vit != vHashOp.end() )
				{
					row.insert( vit->second, 1.0 );
				}

				// Variavel npvd_{c,r}
				v.reset();
				v.setType( VariableOp::V_NRO_PROFS_VIRTUAIS_DOUT_CURSO );
				v.setCurso( curso );
				v.setContrato( contrato );
				vit = vHashOp.find( v );

				if ( vit != vHashOp.end() )
				{
					row.insert( vit->second, 1.0 );
				}

				// Variavel npv_{c,r}
				v.reset();
				v.setType( VariableOp::V_NRO_PROFS_VIRTUAIS_CURSO );
				v.setCurso( curso );
				v.setContrato( contrato );
				vit = vHashOp.find( v );

				if ( vit != vHashOp.end() )
				{
					row.insert( vit->second, -1.0 );
				}

				// Insere restrição
				if ( row.getnnz() != 0 )
				{
					cHashOp[ c ] = lp->getNumRows();
					lp->addRow( row );
					restricoes++;
				}
			}
		}
	}
	   
   return restricoes;
 }

// SÓ PARA RODADA 1
int Operacional::criaRestricaoEstimaNroProfsVirtuaisPorContratoCurso()
{
   int restricoes = 0;  

   int nnz;
   double rhs;
   char name[ 200 ];  

   ITERA_GGROUP_LESSPTR( itHorDia, problemData->horariosDia, HorarioDia )
   {
	   int dia = (*itHorDia)->getDia();
	   HorarioAula *horarioAula = (*itHorDia)->getHorarioAula();

	   ITERA_GGROUP_LESSPTR( itCurso, problemData->cursos, Curso )
	   {
		   Curso *curso = *itCurso;
	   
		   ITERA_GGROUP_LESSPTR( itContrato, problemData->tipos_contrato, TipoContrato )
		   {
			   TipoContrato *contrato = *itContrato;

			   ConstraintOp c;
			   c.reset();
			   c.setType( ConstraintOp::C_ESTIMA_NRO_PROFS_VIRTUAIS_CONTRATO_CURSO );
			   c.setCurso( curso );
			   c.setDia( dia );
			   c.setHorarioAula( horarioAula );
			   c.setContrato( contrato );

			   ConstraintOpHash::iterator cit;
			   cit = cHashOp.find( c );

				if ( cit == cHashOp.end() )
				{
					sprintf( name, "%s", c.toString().c_str() );
					nnz = problemData->profsVirtuais.size() * problemData->horariosDia.size() + 1;
					rhs = 0;

					OPT_ROW row( nnz, OPT_ROW::LESS, rhs, name );
					   				  
					for (VariableOpHash::iterator vit = vHashOp.begin(); vit != vHashOp.end(); vit++ )
					{
						VariableOp v;
						v.reset();
						v = ( vit->first );
						if ( v.getType() == VariableOp::V_X_PROF_AULA_HOR &&
							 v.getProfessor()->eVirtual() &&
							 v.getProfessor()->tipo_contrato == contrato &&
							 v.getDia() == dia &&
							 v.getAula()->atendeAoCurso( curso->getId() ) )
						{	
							HorarioAula *h = v.getHorarioAula();
							Calendario *sl = h->getCalendario();
							for ( int i=1; i<=v.getAula()->getTotalCreditos(); i++ )
							{
								if ( h->sobrepoe( *horarioAula ) )
								{
									row.insert( vit->second, 1.0 );
									break;
								}
								h = sl->getProximoHorario( h );
							}
						}					
					}      
					
					// Variavel npv_{c,r}
					VariableOp v;
					v.reset();
					v.setType( VariableOp::V_NRO_PROFS_VIRTUAIS_CURSO );
					v.setCurso( curso );
					v.setContrato( contrato );
					
					VariableOpHash::iterator vit;
					vit = vHashOp.find( v );
					
					if ( vit != vHashOp.end() )
					{
						row.insert( vit->second, -1.0 );
					}
					else if ( row.getnnz() > 1 ){
						std::cout << "\nErro! Variavel " << v.toString() << " nao encontrada, mas existe x associado!";
					}

					// Insere restrição
					if ( row.getnnz() > 1 )
					{
						cHashOp[ c ] = lp->getNumRows();
						lp->addRow( row );
						restricoes++;
					}
				}
		   }
	   }

   }

   return restricoes;
}

// RODADAS 1 E 2
int Operacional::criaRestricaoMaximoProfSemContratoCurso()
{
   int restricoes = 0;
      
   if ( ! problemData->parametros->minContratoParcial &&
	    ! problemData->parametros->minContratoIntegral )
   {
		return restricoes;
   }

   int nnz;
   double rhs;
   char name[ 200 ];

   ConstraintOp c;
   VariableOpHash::iterator vit;
   ConstraintOpHash::iterator cit;

   // Agrupando as variaveis por curso e contrato
   std::map< Curso*, std::map< TipoContrato*, GGroup< std::pair<int, double> >,
	   LessPtr<TipoContrato> >, LessPtr<Curso> > mapCursoContratoProfessores;
   
	VariableOpHash::iterator vit_find = vHashOp.begin();
	for (; vit_find != vHashOp.end(); vit_find++ )
	{
		VariableOp v = vit_find->first;
			   
		if ( v.getType() == VariableOp::V_PROF_CURSO ) // w_{p,c}
		{
			if ( v.getProfessor()->eVirtual() )
				continue;

			int contrato_var;
			
			Professor* p = v.getProfessor();
			contrato_var = p->tipo_contrato->getContrato();

			ITERA_GGROUP_LESSPTR( it_contrato, problemData->tipos_contrato, TipoContrato )
			{
				TipoContrato *contrato = *it_contrato;

				if ( v.getCurso()->getMinPercContrato( contrato ) == 0 ) continue;

				if ( ( contrato->getContrato() == TipoContrato::Parcial && 
					   problemData->parametros->minContratoParcial ) ||
					 ( contrato->getContrato() == TipoContrato::Integral &&
					   problemData->parametros->minContratoIntegral ) )
				{
					if ( contrato_var < contrato->getContrato() )
					{
						mapCursoContratoProfessores[ v.getCurso() ][ contrato ].add( std::make_pair( vit_find->second, 1.0 ) );
					}
				}
			}
		}
		else if ( v.getType() == VariableOp::V_NRO_PROFS_VIRTUAIS_CURSO ) // npv_{c,r}
		{
			int contrato_var = v.getContrato()->getContrato();

			ITERA_GGROUP_LESSPTR( it_contrato, problemData->tipos_contrato, TipoContrato )
			{
				TipoContrato *contrato = *it_contrato;
				
				if ( v.getCurso()->getMinPercContrato( contrato ) == 0 ) continue;

				if ( ( contrato->getContrato() == TipoContrato::Parcial && 
					   problemData->parametros->minContratoParcial ) ||
					 ( contrato->getContrato() == TipoContrato::Integral &&
					   problemData->parametros->minContratoIntegral ) )
				{
					if ( contrato_var < contrato->getContrato() )
					{
						// aparece no lado esquerdo e direito
						double minPerc = v.getCurso()->getMinPercContrato( contrato );
						double coef = 1.0 -(1-minPerc);
						mapCursoContratoProfessores[ v.getCurso() ][ contrato ].add( std::make_pair( vit_find->second, coef ) );
					}
					else
					{
						// aparece só no lado direito
						double minPerc = v.getCurso()->getMinPercContrato( contrato );
						double coef = -(1-minPerc);
						mapCursoContratoProfessores[ v.getCurso() ][ contrato ].add( std::make_pair( vit_find->second, coef ) );
					}
				}
			}
		}
		else if ( v.getType() == VariableOp::V_NRO_PROFS_CURSO ) // np_{c}
		{		
			ITERA_GGROUP_LESSPTR( it_contrato, problemData->tipos_contrato, TipoContrato )
			{
				TipoContrato *contrato = *it_contrato;
				
				if ( v.getCurso()->getMinPercContrato( contrato ) == 0 ) continue;

				if ( ( contrato->getContrato() == TipoContrato::Parcial && 
					   problemData->parametros->minContratoParcial ) ||
					 ( contrato->getContrato() == TipoContrato::Integral &&
					   problemData->parametros->minContratoIntegral ) )
				{
					double minPerc = v.getCurso()->getMinPercContrato( contrato );
					double coef = -(1-minPerc);
					mapCursoContratoProfessores[ v.getCurso() ][ contrato ].add( std::make_pair( vit_find->second, coef ) );
				}
			}
		}

	}

   std::map< Curso*, std::map< TipoContrato*, GGroup< std::pair<int, double> >,
	   LessPtr<TipoContrato> >, LessPtr<Curso> >::iterator
      it_map_curso = mapCursoContratoProfessores.begin();

   for (; it_map_curso != mapCursoContratoProfessores.end();
          it_map_curso++ )
   {
        Curso * curso = it_map_curso->first;
				
	    std::map< TipoContrato*, GGroup< std::pair<int, double> >,
		   LessPtr<TipoContrato> >::iterator it_map_contrato = it_map_curso->second.begin();
   
	    for (; it_map_contrato != it_map_curso->second.end(); it_map_contrato++ )
	    {
			TipoContrato *contrato = it_map_contrato->first;
			
			c.reset();
			c.setType( ConstraintOp::C_MAX_SEM_CONTRATO_CURSO );
			c.setCurso( curso );
			c.setContrato( contrato );

			cit = cHashOp.find( c );

			if ( cit == cHashOp.end() )
			{
				GGroup< std::pair<int, double> > variaveis = it_map_contrato->second;

				sprintf( name, "%s", c.toString().c_str() );
				nnz = variaveis.size();
				rhs = 0.0;

				OPT_ROW row( nnz, OPT_ROW::LESS, rhs, name );
				
				GGroup< std::pair<int, double> >::iterator itVars = variaveis.begin();
				for ( ; itVars != variaveis.end(); itVars++ )
				{
					row.insert( (*itVars).first, (*itVars).second );
				}
			
				if ( row.getnnz() > 0 )
				{
					cHashOp[ c ] = lp->getNumRows();
					lp->addRow( row );
					restricoes++;
				}			
			}
		}
   }
   return restricoes;
}

// RODADAS 1 E 2
int Operacional::criaRestricaoMinimoContratoCurso()
{
   int restricoes = 0;
      
   if ( ! problemData->parametros->minContratoParcial &&
	    ! problemData->parametros->minContratoIntegral )
   {
		return restricoes;
   }

   int nnz;
   double rhs;
   char name[ 200 ];

   ConstraintOp c;
   VariableOpHash::iterator vit;
   ConstraintOpHash::iterator cit;

   // Agrupando as variaveis por curso e contrato
   std::map< Curso*, std::map< TipoContrato*, GGroup< std::pair<int, double> >,
	   LessPtr<TipoContrato> >, LessPtr<Curso> > mapCursoContratoProfessores;

  	VariableOpHash::iterator vit_find = vHashOp.begin();
	for (; vit_find != vHashOp.end(); vit_find++ )
	{
		VariableOp v = vit_find->first;
			   
		if ( v.getType() == VariableOp::V_PROF_CURSO ) // w_{p,c}
		{
			if ( v.getProfessor()->eVirtual() )
				continue;

			int contrato_var;
			
			Professor* p = v.getProfessor();
			contrato_var = p->tipo_contrato->getContrato();

			ITERA_GGROUP_LESSPTR( it_contrato, problemData->tipos_contrato, TipoContrato )
			{
				TipoContrato *contrato = *it_contrato;
								
				if ( v.getCurso()->getMinPercContrato( contrato ) == 0 ) continue;

				if ( ( contrato->getContrato() == TipoContrato::Parcial && 
					   problemData->parametros->minContratoParcial ) ||
					 ( contrato->getContrato() == TipoContrato::Integral &&
					   problemData->parametros->minContratoIntegral ) )
				{
					if ( contrato_var >= contrato->getContrato() )
					{
						mapCursoContratoProfessores[ v.getCurso() ][ contrato ].add( std::make_pair( vit_find->second, 1.0 ) );
					}
				}
			}
		}
		else if ( v.getType() == VariableOp::V_NRO_PROFS_VIRTUAIS_CURSO ) // npv_{c,r}
		{
			int contrato_var = v.getContrato()->getContrato();

			ITERA_GGROUP_LESSPTR( it_contrato, problemData->tipos_contrato, TipoContrato )
			{
				TipoContrato *contrato = *it_contrato;
								
				if ( v.getCurso()->getMinPercContrato( contrato ) == 0 ) continue;

				if ( ( contrato->getContrato() == TipoContrato::Parcial && 
					   problemData->parametros->minContratoParcial ) ||
					 ( contrato->getContrato() == TipoContrato::Integral &&
					   problemData->parametros->minContratoIntegral ) )
				{
					if ( contrato_var >= contrato->getContrato() )
					{
						// aparece no lado esquerdo e direito
						double minPerc = v.getCurso()->getMinPercContrato( contrato );
						double coef = 1.0 -minPerc;
						mapCursoContratoProfessores[ v.getCurso() ][ contrato ].add( std::make_pair( vit_find->second, coef ) );
					}
					else
					{
						// aparece só no lado direito
						double minPerc = v.getCurso()->getMinPercContrato( contrato );
						double coef = -minPerc;
						mapCursoContratoProfessores[ v.getCurso() ][ contrato ].add( std::make_pair( vit_find->second, coef ) );
					}
				}
			}
		}
		else if ( v.getType() == VariableOp::V_NRO_PROFS_CURSO ) // np_{c}
		{		
			ITERA_GGROUP_LESSPTR( it_contrato, problemData->tipos_contrato, TipoContrato )
			{
				TipoContrato *contrato = *it_contrato;
								
				if ( v.getCurso()->getMinPercContrato( contrato ) == 0 ) continue;

				if ( ( contrato->getContrato() == TipoContrato::Parcial && 
					   problemData->parametros->minContratoParcial ) ||
					 ( contrato->getContrato() == TipoContrato::Integral &&
					   problemData->parametros->minContratoIntegral ) )
				{
					double minPerc = v.getCurso()->getMinPercContrato( contrato );
					double coef = -minPerc;
					mapCursoContratoProfessores[ v.getCurso() ][ contrato ].add( std::make_pair( vit_find->second, coef ) );
				}
			}
		}
	} 


   std::map< Curso*, std::map< TipoContrato*, GGroup< std::pair<int, double> >,
	   LessPtr<TipoContrato> >, LessPtr<Curso> >::iterator
      it_map_curso = mapCursoContratoProfessores.begin();

   for (; it_map_curso != mapCursoContratoProfessores.end();
          it_map_curso++ )
   {
        Curso * curso = it_map_curso->first;
				
	    std::map< TipoContrato*, GGroup< std::pair<int, double> >,
		   LessPtr<TipoContrato> >::iterator it_map_contrato = it_map_curso->second.begin();
   
	    for (; it_map_contrato != it_map_curso->second.end(); it_map_contrato++ )
	    {
			TipoContrato *contrato = it_map_contrato->first;
			
			c.reset();
			c.setType( ConstraintOp::C_MIN_CONTRATO_CURSO );
			c.setCurso( curso );
			c.setContrato( contrato );

			cit = cHashOp.find( c );

			if ( cit == cHashOp.end() )
			{
				GGroup< std::pair<int, double> > variaveis = it_map_contrato->second;

				sprintf( name, "%s", c.toString().c_str() );
				nnz = variaveis.size();
				rhs = 0.0;

				OPT_ROW row( nnz, OPT_ROW::GREATER, rhs, name );
				
				GGroup< std::pair<int, double> >::iterator itVars = variaveis.begin();
				for( ; itVars != variaveis.end(); itVars++ )
				{
					row.insert( (*itVars).first, (*itVars).second );
				}
			
				if ( row.getnnz() > 0 )
				{
					cHashOp[ c ] = lp->getNumRows();
					lp->addRow( row );
					restricoes++;
				}			
			}
		}
   }
   return restricoes;
}

// SÓ PARA RODADA 2
int Operacional::criaRestricaoCalculaNroProfsVirtPorContratoAlocadosCurso()
{
  int restricoes = 0;  
      
  if ( ! problemData->parametros->minContratoIntegral &&
	   ! problemData->parametros->minContratoParcial )
   {
		return restricoes;
   }

   int nnz;
   double rhs;
   char name[ 200 ];

   ConstraintOp c;
   VariableOpHash::iterator vit;
   ConstraintOpHash::iterator cit;
   VariableOp v;
      
   vit = vHashOp.begin();

	for (; vit != vHashOp.end(); vit++)
	{
		VariableOp v = ( vit->first );
				
		double coef = 0.0;
		TipoContrato *contrato = NULL;

		if (v.getType() == VariableOp::V_PROF_CURSO) // w_{p,c}
		{
			if ( ! v.getProfessor()->eVirtual() ) continue;
			
			coef = 1.0;
			contrato = v.getProfessor()->tipo_contrato;
		}
		else if(v.getType() == VariableOp::V_NRO_PROFS_VIRTUAIS_CURSO) // npv_{c, r}
		{
			coef = -1.0;
			contrato = v.getContrato();
		}
		else
		{
			continue;
		}

		c.reset();
		c.setType( ConstraintOp::C_CALCULA_NRO_PROF_VIRT_CONTRATO_CURSO );
		c.setCurso( v.getCurso() );
		c.setContrato( contrato );

		cit = cHashOp.find(c);
		if ( cit == cHashOp.end() )
		{
			cHashOp[ c ] = lp->getNumRows();

			sprintf( name, "%s", c.toString().c_str() );
			rhs = 0;
			OPT_ROW row( 50, OPT_ROW::EQUAL, rhs, name );

			row.insert( vit->second, coef );

			lp->addRow( row );
			restricoes++;
		}
		else
		{
			lp->chgCoef(cit->second, vit->second, coef );
		}
	}

   return restricoes;
}

// SÓ PARA RODADA 2
int Operacional::criaRestricaoUsaProfVirtual()
{
   int restricoes = 0;  
   int nnz;
   double rhs;
   char name[ 200 ];

   ConstraintOp c;
   VariableOpHash::iterator vit;
   ConstraintOpHash::iterator cit;
   VariableOp v;

   int M = problemData->cursos.size();
      
   for (vit = vHashOp.begin(); vit != vHashOp.end(); vit++)
   {
		VariableOp v = ( vit->first );
				
		double coef = 0.0;

		if (v.getType() == VariableOp::V_PROF_CURSO) // w_{p,c}
		{
			if ( ! v.getProfessor()->eVirtual() ) continue;
			coef = -1.0;
		}
		else if(v.getType() == VariableOp::V_PROF_VIRTUAL) // pv_{p}
		{
			if ( ! v.getProfessor()->eVirtual() ) continue;
			coef = M;
		}
		else
		{
			continue;
		}

		c.reset();
		c.setType( ConstraintOp::C_USA_PROF_VIRT );
		c.setProfessor( v.getProfessor() );

		cit = cHashOp.find(c);
		if ( cit == cHashOp.end() )
		{
			cHashOp[ c ] = lp->getNumRows();

			sprintf( name, "%s", c.toString().c_str() );
			rhs = 0;
			OPT_ROW row( 50, OPT_ROW::GREATER, rhs, name );

			row.insert( vit->second, coef );

			lp->addRow( row );
			restricoes++;
		}
		else
		{
			lp->chgCoef(cit->second, vit->second, coef );
		}
	}

   return restricoes;
}

/*
Aulas de disciplinas pratica/teorica continuas

sum[p]sum[s] x_{p,it,dt,s,t,hi,hf} <= sum[p]sum[h]sum[s] x_{p,ip,dp,s,t,h,hi-1} + sum[p]sum[h]sum[s] x_{p,ip,dp,s,t,hf+1,h}

Para toda disciplina d=(dt,dp) sendo d com obrigação de créditos contínuos
Para toda turma it \in I_{dt}, ip \in I_{dp} sendo que (it,dt) tem aluno comum com (ip,dp)
Para todo dia t
Para todo par de horários hi,hf
	
*/
int Operacional::criaRestricaoDiscPTAulasContinuas()
{
	int restricoes = 0;
	int nnz;
	char name[ 100 ];

	VariableOp v;
	ConstraintOp c;
	VariableOpHash::iterator vit;
	ConstraintOpHash::iterator cit;

   std::map<Disciplina*, std::map< int /*turma*/, std::map< int /*dia*/, std::map< DateTime, VariableOp > > >, LessPtr<Disciplina>> mapDiscDiaHiVar;

	vit = vHashOp.begin();
	while(vit != vHashOp.end())
	{
		if( vit->first.getType() == VariableOp::V_X_PROF_AULA_HOR )
		{
			VariableOp v = vit->first;

			if ( v.getDisciplina()->getId() > 0 )
			if ( problemData->getDisciplinaTeorPrat( v.getDisciplina() ) == NULL )
			{
				vit++; continue;
			}
				
			if ( v.getDisciplina()->aulasContinuas() )
			{	
				mapDiscDiaHiVar[ v.getDisciplina() ][ v.getTurma() ][ v.getDia() ][ v.getHorarioAula()->getInicio() ] = v;
			}
		}
		vit++;
	}

	std::map<Disciplina*, std::map< int /*turma*/, std::map< int /*dia*/, std::map<DateTime, VariableOp> > >, LessPtr<Disciplina>>::iterator
		itMapDisc = mapDiscDiaHiVar.begin();
	for ( ; itMapDisc != mapDiscDiaHiVar.end(); itMapDisc++ )
	{	
		if ( itMapDisc->first->getId() < 0 ) continue;

		Disciplina *disciplinaTeor = itMapDisc->first;

		std::map< int /*turma*/, std::map< int /*dia*/, std::map< DateTime, VariableOp > > >::iterator
			itTurma = itMapDisc->second.begin();
		for ( ; itTurma != itMapDisc->second.end(); itTurma++ )
		{
			int turmaTeor = itTurma->first;

			std::map< int /*dia*/, std::map< DateTime, VariableOp > >::iterator
				itMapDia = itTurma->second.begin();

			for ( ; itMapDia != itTurma->second.end(); itMapDia++ )
			{	
				int dia = itMapDia->first;

				std::map< DateTime, VariableOp >::iterator
					itMapDateTime = itMapDia->second.begin();
				for ( ; itMapDateTime != itMapDia->second.end(); itMapDateTime++ )
				{	
					DateTime dt = itMapDateTime->first;
					VariableOp v_t = itMapDateTime->second;
				
					Aula *aula = v_t.getAula();
					HorarioAula *hi = v_t.getHorarioAula();

					int nCred = aula->getTotalCreditos();				
					HorarioAula *hf = hi;
					for (int k = 1; k < nCred; k++)
           			{
						hf = hf->getCalendario()->getProximoHorario(hf);
						if ( hf == NULL ) std::cout<<"\nErro, horario final nao encontrado.\n";
					}

					int turmaTeor = v_t.getTurma();
					Campus * campus = problemData->refCampus[ v_t.getUnidade()->getIdCampus() ];

					DateTime *dti = problemData->horarioAulaDateTime[hi->getId()].first;
					DateTime *dtf = problemData->horarioAulaDateTime[hf->getId()].first;

					c.reset();
					c.setType( ConstraintOp::C_AULA_PT_SEQUENCIAL );
					c.setCampus( campus );
					c.setTurma( turmaTeor );
					c.setDisciplina( disciplinaTeor );
					c.setDia( dia );
					c.setDateTimeInicial( dti );
					c.setDateTimeFinal( dtf );
					
					sprintf( name, "%s", c.toString().c_str() );
					OPT_ROW row( 100, OPT_ROW::LESS , 0.0 , name );
				
					// --------------------
					// teórica
					row.insert( vHashOp[v_t], 1.0 );
				
					Disciplina * disciplinaPrat = problemData->getDisciplinaTeorPrat( disciplinaTeor );
					Calendario * calendario = hf->getCalendario();
					int nCredsPrat = disciplinaPrat->getTotalCreditos();

					// --------------------
					// antes da teórica
					HorarioAula *hi_p1 = hi;
					for ( int i = 1; i <= nCredsPrat; i++ )
					{
						 hi_p1 = calendario->getHorarioAnterior( hi_p1 );
						 if ( hi_p1==NULL ) break;
					}
					if ( hi_p1 != NULL )
					{
						DateTime dti_p1 = hi_p1->getInicio();
					
						std::map<Disciplina*, std::map< int /*turma*/, 
							std::map< int /*dia*/, std::map< DateTime, VariableOp > > >, LessPtr<Disciplina>>::iterator
						itDisc = mapDiscDiaHiVar.find( disciplinaPrat );
						if ( itDisc != mapDiscDiaHiVar.end() )
						{
							std::map< int /*turma*/, std::map< int /*dia*/, std::map< DateTime, VariableOp > > >::iterator
							itTurma = itDisc->second.find( turmaTeor );
							if ( itTurma != itDisc->second.end() )
							{
								std::map< int /*dia*/, std::map< DateTime, VariableOp > >::iterator
								itDia = itTurma->second.find( dia );
								if ( itDia != itTurma->second.end() )
								{
									std::map< DateTime, VariableOp >::iterator
									itDt = itDia->second.find( dti_p1 );
									if ( itDt != itDia->second.end() )
									{
										VariableOp v_p1 = itDt->second;
										row.insert( vHashOp[v_p1], -1.0 );
									}
								}
							}
						}										
					}

					// --------------------
					// após a teórica
					HorarioAula *hi_p2 = calendario->getProximoHorario( hf );
					if ( hi_p2 != NULL )
					{
						DateTime dti_p2 = hi_p2->getInicio();
									
						std::map<Disciplina*, std::map< int /*turma*/, 
							std::map< int /*dia*/, std::map< DateTime, VariableOp > > >, LessPtr<Disciplina>>::iterator
						itDisc = mapDiscDiaHiVar.find( disciplinaPrat );
						if ( itDisc != mapDiscDiaHiVar.end() )
						{
							std::map< int /*turma*/, std::map< int /*dia*/, std::map< DateTime, VariableOp > > >::iterator
							itTurma = itDisc->second.find( turmaTeor );
							if ( itTurma != itDisc->second.end() )
							{
								std::map< int /*dia*/, std::map< DateTime, VariableOp > >::iterator
								itDia = itTurma->second.find( dia );
								if ( itDia != itTurma->second.end() )
								{
									std::map< DateTime, VariableOp >::iterator
									itDt = itDia->second.find( dti_p2 );
									if ( itDt != itDia->second.end() )
									{
										VariableOp v_p2 = itDt->second;
										row.insert( vHashOp[v_p2], -1.0 );
									}
								}
							}
						}
					}

					// --------------------
					if( row.getnnz() > 0 )
					{
						cHashOp[ c ] = lp->getNumRows();
						lp->addRow( row );
						restricoes++;
					}
				}
			}
		}
	}

	if ( restricoes > 0 && !problemData->parametros->discPratTeor1x1 )
	{
		std::cout << "\nERRO OP: ha obrigatoriedade de aulas continuas para disciplinas praticas e teoricas"
			<< " mesmo permitindo-se a mistura de alunos entre turmas praticas e teoricas. Esse metodo nao"
			<< " esta preparado para isso. Assume-se aqui que um aluno está na turma 1 da disciplina teorica " 
			<< " se e somente se ele esta na turma 1 da pratica.\n";
	}

	return restricoes;
}


/*
	Alocação minima de demanda por aluno

	sum[d] nCreds_{d} * (sum[p] y_{p,d,i}) >= MinAtendPerc * TotalDemanda_{a} - fmd_{a}		, para cada aluno a
	
	min sum[a] fmd{a}
*/
int Operacional::criaRestricaoAlocMinAluno()
{
   int restricoes = 0;
 
   if ( ! problemData->parametros->considerarMinPercAtendAluno )
   {
		return restricoes;
   }

   char name[ 200 ];
   int nnz=0;

   ConstraintOp c;
   ConstraintOpHash::iterator cit;
   VariableOp v;
   VariableOpHash::iterator vit;   
      
	vit = vHashOp.begin();
	for ( ; vit != vHashOp.end(); vit++ )
	{
		VariableOp v = vit->first;
			
		double coef;
				
		if( v.getType() == VariableOp::V_Y_PROF_DISCIPLINA ) // y_{p,d,i,cp}
		{
			coef = v.getDisciplina()->getTotalCreditos();

			Trio< int /*campusId*/, int /*turma*/, Disciplina* > trio(v.getCampus()->getId(), v.getTurma(), v.getDisciplina() );

			GGroup< AlunoDemanda*, LessPtr< AlunoDemanda > > alunosDem = problemData->mapCampusTurmaDisc_AlunosDemanda[trio];
			ITERA_GGROUP_LESSPTR( itAlDem, alunosDem, AlunoDemanda )
			{
				Aluno *aluno = problemData->retornaAluno( itAlDem->getAlunoId() );
						
				c.reset();
				c.setType( ConstraintOp::C_ALOC_MIN_ALUNO );
				c.setAluno( aluno );
		
				sprintf( name, "%s", c.toString().c_str() ); 

				cit = cHashOp.find( c ); 
				if ( cit != cHashOp.end() )
				{
					lp->chgCoef( cit->second, vit->second, coef );
				}
				else
				{
					double rhs = problemData->parametros->minAtendPercPorAluno * aluno->getNroCredsOrigRequeridosP1();
		
					nnz += aluno->demandas.size() + 1;
					OPT_ROW row( nnz, OPT_ROW::GREATER, rhs , name );
		
					row.insert( vit->second, coef );
					cHashOp[ c ] = lp->getNumRows();
			
					lp->addRow( row );
					restricoes++;
				}
			}
		}
		else if( v.getType() == VariableOp::V_FOLGA_ALUNO_MIN_ATEND ) // fmd_{a}
		{ 
			coef = 1.0;

			Aluno *aluno = v.getAluno();
						
			c.reset();
			c.setType( ConstraintOp::C_ALOC_MIN_ALUNO );
			c.setAluno( aluno );
		
			sprintf( name, "%s", c.toString().c_str() ); 

			cit = cHashOp.find( c ); 
			if ( cit != cHashOp.end() )
			{
				lp->chgCoef( cit->second, vit->second, coef );
			}
			else
			{
				double rhs = problemData->parametros->minAtendPercPorAluno * aluno->getNroCredsOrigRequeridosP1();
		
				nnz += aluno->demandas.size() + 1;
				OPT_ROW row( nnz, OPT_ROW::GREATER, rhs , name );
		
				row.insert( vit->second, coef );
				cHashOp[ c ] = lp->getNumRows();
			
				lp->addRow( row );
				restricoes++;
			}
		}
	}

	return restricoes;
}

/*
	Garante o tempo de descanso mínimo entre o ultimo horario de aula que o professor
	ministra no dia t e o primeiro horario de aula que este ministra no dia t+1

	Para cada professor p e par [ (hf,t) e (hi,t+1) ] tal que o tempo entre (hf,t) e (hi,t+1)
	é menor do que o tempo mínimo de descanso:

	sum[a E At] x_{p,a,hf} + sum[a E At+1] x_{p,a,hi} <= 1

*/
int Operacional::criaRestricaoProfDescansoMinimo()
{
   int restricoes = 0;

   if ( ! problemData->parametros->considerarDescansoMinProf )
   {
		return restricoes;
   }

   double tempoMinimoDescanso = problemData->parametros->descansoMinProfValue;	// em horas
   
	if ( tempoMinimoDescanso > 24 ) 
		std::cout<<"TODO: a comparacao de dias de DateTime nao "
			<<"esta considerando interjornada de mais de 24hs. Consertar.";

   tempoMinimoDescanso *= 60;													// em minutos

   int nnz;
   char name[ 200 ];

   ConstraintOp c;
   VariableOpHash::iterator vit;
   ConstraintOpHash::iterator cit;
   
   // Aulas organizadas em relação aos DateTimes de INÍCIO da aula
   std::map<Professor*, std::map< int /*dia*/, 
	   std::map<DateTime*, GGroup<int> > >, LessPtr<Professor>> mapProfDiaHiVarId;

   // Aulas organizadas em relação aos DateTimes de TÉRMIDO da aula
   std::map<Professor*, std::map< int /*dia*/, 
	   std::map<DateTime*, GGroup<int> > >, LessPtr<Professor>> mapProfDiaHfVarId;


	vit = vHashOp.begin();
	for ( ; vit != vHashOp.end(); vit++ )
	{
		if ( vit->first.getType() == VariableOp::V_X_PROF_AULA_HOR )
		{	
			VariableOp v = vit->first;
			
			if ( v.getProfessor()->eVirtual() && this->getRodada() == OP_VIRTUAL_PERFIL )
				continue;

			HorarioAula *hai = v.getHorarioAula();					
			DateTime *dti = problemData->horarioAulaDateTime[ hai->getId() ].first;
			mapProfDiaHiVarId[ v.getProfessor() ][ v.getDia() ][ dti ].add( vit->second );	
			
			int nCreds = v.getAula()->getTotalCreditos();
			Calendario *calendario = hai->getCalendario();
			HorarioAula *haf = calendario->getHorarioMaisNCreds(hai, nCreds-1);
			DateTime *dtf = problemData->getDateTimeFinal( haf->getFinal() );
			if ( dtf==NULL )
			{
				std::cout << "\nErro: dtf Null para final = " << haf->getFinal()
						<< " e horario aula id = " << haf->getId();
				continue;
			}
			mapProfDiaHfVarId[ v.getProfessor() ][ v.getDia() ][ dtf ].add( vit->second );	
		}
	}


   std::map<Professor*, std::map< int /*dia*/, 
	   std::map<DateTime*, GGroup<int> > >, LessPtr<Professor>>::iterator
	   itProfDiaDtf = mapProfDiaHfVarId.begin();
	for ( ; itProfDiaDtf != mapProfDiaHfVarId.end(); itProfDiaDtf++ )
	{		
		Professor *professor = itProfDiaDtf->first;

		std::map< int /*dia*/, std::map<DateTime*, GGroup<int> > >::iterator
			itDiaDtf = itProfDiaDtf->second.begin();	
		for ( ; itDiaDtf != itProfDiaDtf->second.end(); itDiaDtf++ )
		{		
			int dia = itDiaDtf->first;

			// Verifica se há aulas iniciando no dia seguinte
			std::map< int /*dia*/, std::map<DateTime*, GGroup<int> > >::iterator
				itDiaSeguinte = mapProfDiaHiVarId[ professor ].find( dia+1 );	
			if ( itDiaSeguinte == mapProfDiaHiVarId[ professor ].end() )
			{
				continue;
			}
			std::map<DateTime*, GGroup<int> > mapDtiVar = itDiaSeguinte->second;


			// Varre os tempos finais das aulas no dia corrente
			std::map<DateTime*, GGroup<int> >::iterator
				itDtf = itDiaDtf->second.begin();	
			for ( ; itDtf != itDiaDtf->second.end(); itDtf++ )
			{		
				DateTime *dtf = itDtf->first;

				GGroup<int> varsX_diaT = itDtf->second;

				DateTime dtfCopy = *dtf;				
				dtfCopy.addMinutes( tempoMinimoDescanso );
				DateTime dtf_maisDescanso = dtfCopy;

				// A restrição é somente para a ultima aula de um dia e a primeira aula do dia SEGUINTE
				if ( dtf_maisDescanso.getDay() == dtf->getDay() )
					continue;

				GGroup<HorarioDia*, LessPtr<HorarioDia> > horsIncompat = 
					professor->getHorariosAnterioresDisponivelDia( dtf_maisDescanso, dia+1 );

				ITERA_GGROUP_LESSPTR( itHorDia, horsIncompat, HorarioDia )
				{
					HorarioAula *hai = itHorDia->getHorarioAula();					
					DateTime *dti = problemData->horarioAulaDateTime[ hai->getId() ].first;
					
					// Procura aulas no dia seguinte do professor que comecem no horario ha					
					std::map<DateTime*, GGroup<int>>::iterator itDti = mapDtiVar.find( dti );
					if ( itDti == mapDtiVar.end() )
					{
						continue;
					}
					
					GGroup<int> varsX_diaSeguinte = itDti->second;

					c.reset();
					c.setType( ConstraintOp::C_PROF_MIN_DESCANSO );
					c.setProfessor( professor );
					c.setDia( dia );
					c.setDateTimeFinal( dtf );
					c.setDateTimeInicial( dti );
					
					double coef = 1.0;


				
					cit = cHashOp.find( c ); 
					if ( cit == cHashOp.end() )
					{
						nnz = varsX_diaT.size() + varsX_diaSeguinte.size();
						
						sprintf( name, "%s", c.toString().c_str() );
						OPT_ROW row( 100, OPT_ROW::LESS, 1.0, name );
						
						ITERA_GGROUP_N_PT( itVarOp, varsX_diaT, int )
						{
							row.insert( *itVarOp, coef );
						}
						ITERA_GGROUP_N_PT( itVarOp, varsX_diaSeguinte, int )
						{
							row.insert( *itVarOp, coef );
						}
						
						cHashOp[ c ] = lp->getNumRows();
			
						lp->addRow( row );
						restricoes++;
					}
					else
					{
						ITERA_GGROUP_N_PT( itVarOp, varsX_diaT, int )
						{
							lp->chgCoef( cit->second, *itVarOp, coef );
						}
						ITERA_GGROUP_N_PT( itVarOp, varsX_diaSeguinte, int )
						{
							lp->chgCoef( cit->second, *itVarOp, coef );
						}
						
					}
				
				}

			}
		
		}
	}

   return restricoes;
}



// --------------------------------------------------------------

// criar restrições que impedem gaps nos horários do professor em uma mesma fase de um dia

int Operacional::criarRestricaoProfHiHf_()
{	
	int numRestr = 0;
	int numRestAnterior = numRestr;

	if ( problemData->parametros->proibirProfGapMTN ==
		 ParametrosPlanejamento::ConstraintLevel::Off )
		return numRestr;

	// Par: ( DateTimeInicial -> set<(col,coef)> ), ( DateTimeFinal -> set<(col,coef)> ) 
	typedef pair< map<DateTime, set< pair<int,double> >>, 
				  map<DateTime, set< pair<int,double> >> > ParMapDtiDtf;

	// Prof -> Dia -> Fase Do Dia -> Par-DateTime
	unordered_map<Professor*, unordered_map<int, unordered_map<int,ParMapDtiDtf>>> mapProfDiaFaseHiHf;
	
	// Prof -> Dia -> Fase do dia -> <(col,coef)>
	map<Professor*, map<int, map< int, set<pair<int,double>> > >> mapProfDiaFase;
	
	unordered_map<int, unordered_set<int>> mapDiasFases;

	// -------------------------------------------------------------------------------------------------
	#pragma region Agrupa variáveis em mapProfDiaFaseHiHf e mapProfDiaFase
	
	// Percorre variaveis k_{p,i,d,t,hi}	
	for(auto itProf = vars_prof_aula2.begin(); itProf != vars_prof_aula2.end(); ++itProf)
	{
		Professor* const professor = itProf->first;
		
		if ( professor->eVirtual() && !problemData->parametros->proibirProfVirtualGapMTN )
		{
			continue;
		}

		auto *mapProf1 = &mapProfDiaFaseHiHf[professor];
		auto *mapProf2 = &mapProfDiaFase[professor];

		for(auto itDia = itProf->second.begin(); itDia != itProf->second.end(); ++itDia)
		{
			const int dia = itDia->first;
			auto *mapProfDia1 = &(*mapProf1)[dia];
			auto *mapProfDia2 = &(*mapProf2)[dia];

			for(auto itDti = itDia->second.begin(); itDti != itDia->second.end(); ++itDti)
			{		
				const DateTime dti = itDti->first;
				const int faseDoDia = CentroDados::getFaseDoDia(dti);	
							
				ParMapDtiDtf *mapProfDiaFase1 = &(*mapProfDia1)[faseDoDia];
				auto *mapProfDiaFase2 = &(*mapProfDia2)[faseDoDia];
				
				mapDiasFases[dia].insert(faseDoDia);

				for(auto itCp = itDti->second.begin(); itCp != itDti->second.end(); ++itCp)
				{
					for(auto itDisc = itCp->second.begin(); itDisc != itCp->second.end(); ++itDisc)
					{
						for(auto itTurma = itDisc->second.begin(); itTurma != itDisc->second.end(); ++itTurma)
						{
							const int col = itTurma->second.first;
							const VariableOp var = itTurma->second.second;
		
							// ----------- Recupera datetime final da aula

							Calendario *calend = var.getHorarioAula()->getCalendario();
							HorarioAula *h = calend->getHorarioMaisNCreds( var.getHorarioAula(), var.getAula()->getTotalCreditos() - 1 );							
							DateTime dtf = h->getFinal();	

							// ----------- mapProfDiaFaseHiHf

							DateTime maiorDt = CentroDados::getFimDaFase(faseDoDia);
							double bigM = maiorDt.getDateMinutes();
							double coefXiUB = bigM;
							((*mapProfDiaFase1).first)[dti].insert( make_pair(col,coefXiUB) );

							double coefXfLB = - dtf.getDateMinutes();
							((*mapProfDiaFase1).second)[dtf].insert( make_pair(col,coefXfLB) );

							// ----------- mapProfDiaFase
				
							double duracaoAula = (dtf - dti).getDateMinutes();
							(*mapProfDiaFase2).insert( make_pair(col,duracaoAula) );							
						}
					}
				}
			}
		}
	}
	#pragma endregion
	
	// -------------------------------------------------------------------------------------------------
	// Prof -> Dia -> Fase Do Dia -> Par-DateTime

	unordered_map<Professor*, unordered_map<int, unordered_map<int,ParMapDtiDtf>>> mapProfDiaFaseHilbHfub;

	// -------------------------------------------------------------------------------------------------
	// - Restrições para setar ub de hip e lb de hfp de cada dia-faseDoDia do professor
	// - Construção de mapProfDiaFaseHilbHfub
	#pragma region Restrições para setar ub de hip e lb de hfp de cada dia-faseDoDia do professor & Construção de mapProfDiaFaseHilbHfub

	for(auto itProf = mapProfDiaFaseHiHf.begin(); itProf != mapProfDiaFaseHiHf.end(); ++itProf)
	{
		Professor *prof = itProf->first;

		auto itProfHiHf = varsProfDiaFaseHiHf.find(prof);
		if ( itProfHiHf == varsProfDiaFaseHiHf.end() )
		{
			stringstream ss;
			ss << "var hip-hfp nao encontrada para prof " << prof->getId();
			CentroDados::printError("MIPUnico::criarRestProfHiHf_", ss.str());
			exit(1);
		}
		
		unordered_map<int, unordered_map<int,ParMapDtiDtf>> *mapProf3 = &mapProfDiaFaseHilbHfub[prof];

		for(auto itDia = itProf->second.begin(); itDia != itProf->second.end(); ++itDia)
		{
			int dia = itDia->first;

			auto itDiaHiHf = itProfHiHf->second.find(dia);
			if ( itDiaHiHf == itProfHiHf->second.end() )
			{
				stringstream ss;
				ss << "var hip-hfp nao encontrada para dia " << dia;
				CentroDados::printError("MIPUnico::criarRestProfHiHf_", ss.str());
				exit(1);
			}
			
			unordered_map<int,ParMapDtiDtf> *mapDia3 = &(*mapProf3)[dia];

			for(auto itFase = itDia->second.begin(); itFase != itDia->second.end(); ++itFase)
			{
				int faseDoDia = itFase->first;

				auto itFaseHiHf = itDiaHiHf->second.find(faseDoDia);
				if ( itFaseHiHf == itDiaHiHf->second.end() )
				{
					stringstream ss;
					ss << "var hip-hfp nao encontrada para fase " << faseDoDia;
					CentroDados::printError("MIPUnico::criarRestProfHiHf_", ss.str());
					exit(1);
				}
				
				// Procura vars hip e hfp
				int colHi = -1, colHf = -1;
				colHi = itFaseHiHf->second.first;
				colHf = itFaseHiHf->second.second;

				if ((colHi < 0) || (colHi >= lp->getNumCols()))
				{
					CentroDados::printError("MIPUnico::criarRestProfHiHf_", "index de coluna hip invalido!");
					exit(1);
				}
				if ((colHf < 0) || (colHf >= lp->getNumCols()))
				{
					CentroDados::printError("MIPUnico::criarRestProfHiHf_", "index de coluna hfp invalido!");
					exit(1);
				}
				
				map<DateTime, set< pair<int,double> >> *mapEqualDti = &itFase->second.first;
				map<DateTime, set< pair<int,double> >> *mapEqualDtf = &itFase->second.second;

				numRestr += criarRestricaoProfHiUB_( prof, dia, faseDoDia, colHi, *mapEqualDti );
				numRestr += criarRestricaoProfHfLB_( prof, dia, faseDoDia, colHf, *mapEqualDtf );

				
				// ----------------------------- preenche mapProf3 (pointer para parte de mapProfDiaFaseHilbHfub)
				
				DateTime maiorDt = CentroDados::getFimDaFase(faseDoDia);
				double bigM = maiorDt.getDateMinutes();

				ParMapDtiDtf *parMapFase3 = &(*mapDia3)[faseDoDia];
				map<DateTime, set< pair<int,double> >> *mapLessDti3 = &parMapFase3->first;
				map<DateTime, set< pair<int,double> >> *mapGreaterDtf3 = &parMapFase3->second;

				// preenche map com vars para setar lower bound de hi:  percorre do menor para o maior dt
				for ( auto itDti_2 = mapEqualDti->begin(); itDti_2 != mapEqualDti->end(); itDti_2++ )
				{
					DateTime dti_2 = itDti_2->first;
					double coefXiLB = dti_2.getDateMinutes();

					// Adiciona todas as vars x cujo tempo de inicio é MENOR que dti_2
					for ( auto itDti_1 = mapEqualDti->begin(); itDti_1 != itDti_2; itDti_1++ )
					{
						// Percore vars
						for ( auto itVars_1 = itDti_1->second.begin(); itVars_1 != itDti_1->second.end(); itVars_1++ )
						{
							int col1 = itVars_1->first;
							
							(*mapLessDti3)[dti_2].insert( make_pair(col1,coefXiLB) );
						}
					}
				}
				
				// preenche map com vars para setar upper bound de hf:  percorre do maior para o menor dt
				for ( auto itDtf_1 = mapEqualDtf->rbegin(); itDtf_1 != mapEqualDtf->rend(); itDtf_1++ )
				{
					DateTime dtf_1 = itDtf_1->first;
					double coefXfUB = - bigM;

					// Adiciona todas as vars x cujo tempo de fim é MAIOR que dtf_2
					for ( auto itDtf_2 = mapEqualDtf->rbegin(); itDtf_2 != itDtf_1; itDtf_2++ )
					{
						// Percore vars
						for ( auto itVars_2 = itDtf_2->second.begin(); itVars_2 != itDtf_2->second.end(); itVars_2++ )
						{
							int col2 = itVars_2->first;
							
							(*mapGreaterDtf3)[dtf_1].insert( make_pair(col2,coefXfUB) );
						}
					}
				}			
				
				// -----------------------------
			}
		}
	}

	#ifdef PRINT_cria_restricoes
		std::cout << std::endl << "numRest criarRestricaoProfHiUB_ e criarRestricaoProfHfLB_: " 
			<< numRestr-numRestAnterior << std::endl;
		numRestAnterior = numRestr;
	#endif
	#pragma endregion

	// -------------------------------------------------------------------------------------------------
	#pragma region Restrições para setar lb de hip e ub de hfp de cada dia-faseDoDia do professor
	for(auto itProf = mapProfDiaFaseHilbHfub.begin(); itProf != mapProfDiaFaseHilbHfub.end(); ++itProf)
	{
		Professor *prof = itProf->first;

		auto itProfHiHf = varsProfDiaFaseHiHf.find(prof);
		if ( itProfHiHf == varsProfDiaFaseHiHf.end() )
		{
			stringstream ss;
			ss << "var hip-hfp nao encontrada para prof " << prof->getId();
			CentroDados::printError("MIPUnico::criarRestProfHiHf_", ss.str());
		}

		for(auto itDia = itProf->second.begin(); itDia != itProf->second.end(); ++itDia)
		{
			int dia = itDia->first;

			auto itDiaHiHf = itProfHiHf->second.find(dia);
			if ( itDiaHiHf == itProfHiHf->second.end() )
			{
				stringstream ss;
				ss << "var hip-hfp nao encontrada para dia " << dia;
				CentroDados::printError("MIPUnico::criarRestProfHiHf_", ss.str());
			}

			for(auto itFase = itDia->second.begin(); itFase != itDia->second.end(); ++itFase)
			{
				int faseDoDia = itFase->first;
				auto itFaseHiHf = itDiaHiHf->second.find(faseDoDia);
				if ( itFaseHiHf == itDiaHiHf->second.end() )
				{
					stringstream ss;
					ss << "var hip-hfp nao encontrada para fase " << faseDoDia;
					CentroDados::printError("MIPUnico::criarRestProfHiHf_", ss.str());
				}
				
				// Procura vars hip e hfp
				int colHi = -1, colHf = -1;
				colHi = itFaseHiHf->second.first;
				colHf = itFaseHiHf->second.second;

				if ((colHi < 0) || (colHi >= lp->getNumCols()))
				{
					CentroDados::printError("MIPUnico::criarRestProfHiHf_", "index de coluna hip invalido!");
				}
				if ((colHf < 0) || (colHf >= lp->getNumCols()))
				{
					CentroDados::printError("MIPUnico::criarRestProfHiHf_", "index de coluna hfp invalido!");
				}

				numRestr += criarRestricaoProfHiLB_( prof, dia, faseDoDia, colHi, itFase->second.first );
				numRestr += criarRestricaoProfHfUB_( prof, dia, faseDoDia, colHf, itFase->second.second );
			}
		}
	}
	#ifdef PRINT_cria_restricoes
		std::cout << "numRest criarRestricaoProfHiLB_ e criarRestricaoProfHfUB_: " 
			<< numRestr-numRestAnterior << std::endl;
		numRestAnterior = numRestr;
	#endif
	#pragma endregion

	// -------------------------------------------------------------------------------------------------
	#pragma region Calcula os valores que serão usados como rhs das restrições de gap (a seguir)
	map< int, map< int, int > > somaTempoIntervDiaFase;
	
	for ( auto itDia = mapDiasFases.begin(); itDia != mapDiasFases.end(); itDia++ )
	{
		for ( auto itFase = itDia->second.begin(); itFase != itDia->second.end(); itFase++ )
		{
			somaTempoIntervDiaFase[itDia->first][*itFase] = 
				CentroDados::getProblemData()->getCalendariosMaxSomaInterv( itDia->first, *itFase );
		}
	}
	#pragma endregion

	// -------------------------------------------------------------------------------------------------
	#pragma region Restrições para impedir gap nos horários de mesma fase do dia do professor

	for(auto itProf = mapProfDiaFase.begin(); itProf != mapProfDiaFase.end(); ++itProf)
	{
		Professor *prof = itProf->first;
		auto itProfHiHf = varsProfDiaFaseHiHf.find(prof);

		for(auto itDia = itProf->second.begin(); itDia != itProf->second.end(); ++itDia)
		{
			int dia = itDia->first;
			auto itDiaHiHf = itProfHiHf->second.find(dia);

			for(auto itFase = itDia->second.begin(); itFase != itDia->second.end(); ++itFase)
			{
				int faseDoDia = itFase->first;
				auto itFaseHiHf = itDiaHiHf->second.find(faseDoDia);
				
				// Procura vars hip e hfp
				int colHi = -1, colHf = -1;
				colHi = itFaseHiHf->second.first;
				colHf = itFaseHiHf->second.second;

				// Right hand side é (-) o máximo de tempo ocioso permitido dentro de uma mesma fase f do dia t
				int delta = somaTempoIntervDiaFase[dia][faseDoDia];
				int rhs = - delta;

				numRestr += criarRestricaoProfGapMTN_( prof, dia, faseDoDia, rhs, colHi, colHf, itFase->second );
			}
		}
	}
	#ifdef PRINT_cria_restricoes
		std::cout << "numRest criarRestricaoProfGapMTN_: " 
			<< numRestr-numRestAnterior << std::endl;
		numRestAnterior = numRestr;
	#endif
	#pragma endregion

	return numRestr;
}

int Operacional::criarRestricaoProfHiUB_( Professor* const prof, const int dia, const int fase,
	const int colHi, map<DateTime, set< pair<int,double> >> const &mapDtiVars )
{
	/*
		Restrição que limita superiormente a variável hip.

		Para todo professor p, todo dia t, todo fase do dia f, todo DateTime dt:

		hip_{p,t,f} <= m(dt) + M*( 1 - sum[x t.q. dti==dt] x_{p,t,h} )

		aonde:
		m(dt)		= datetime dt em número de minutos. Por ex: m(7:30) = 7*60 + 30
		x_{p,t,h}	= variável binária indicando se o prof p dá aula no dia t iniciando no horário h
		hip_{p,t,f}	= variável inteira indicando o primeiro horário (valor em número de minutos) 
					  da fase f do dia t em que prof p dá aula.
		
		"x t.q. dti==dt" significa as variáveis x tais que o DateTime de fim da aula dti é igual ao dt da restrição
	*/
	int numRestr = 0;

	for(auto itDateTime = mapDtiVars.begin(); itDateTime != mapDtiVars.end(); ++itDateTime)
	{					
		DateTime dti = itDateTime->first;
		int dtMin = dti.getDateMinutes();

		double bigM = 9999999;
		
		DateTime *pDti = problemData->getDateTimeInicial(dti);

		ConstraintOp cons;
		cons.reset();
		cons.setType(ConstraintOp::C_PROF_HOR_INIC_UB);
		cons.setProfessor(prof);
		cons.setDia(dia);
		cons.setFaseDoDia(fase);
		cons.setDateTimeInicial(pDti);

		OPT_ROW rowHi (OPT_ROW::ROWSENSE::LESS, 0.0, (char*) cons.toString().c_str() );

		// Insere vars ProfTurma
		for ( auto itVar = itDateTime->second.begin(); itVar != itDateTime->second.end(); itVar++ )
		{
			rowHi.insert(itVar->first,itVar->second);

			bigM = itVar->second;
		}

		// Corrige rhs
		double rhs = dtMin + bigM;
		rowHi.setRhs(rhs);	

		// Insere var hip
		rowHi.insert(colHi,1.0);
					
		lp->addRow(rowHi);
		numRestr++;
	}

	return numRestr;
}

int Operacional::criarRestricaoProfHiLB_( Professor* const prof, const int dia, const int fase,
	const int colHi, map<DateTime, set< pair<int,double> >> const &mapDtiVars )
{
	/*
		Restrição que limita inferiormente a variável hip.

		Para todo professor p, todo dia t, todo fase do dia f, todo DateTime dt:

		hip_{p,t,f} >= m(dt) * ( 1 - sum[x t.q. dti<dt] x_{p,t,dti} )

		aonde:
		m(dt) = datetime dt em número de minutos. Por ex: m(7:30) = 7*60 + 30
		x_{p,t,dti}	= variável binária indicando se o prof p dá aula no dia t iniciando no DateTime dti
		hip_{p,t,f}	= variável inteira indicando o primeiro horário (valor em número de minutos) 
					  da fase f do dia t em que prof p dá aula.
		
		"x t.q. dti<dt" significa as variáveis x tais que o DateTime dti de inicio da aula é menor ao dt da restrição
	*/
	
	int numRestr = 0;
	
	for(auto itDateTime = mapDtiVars.begin(); itDateTime != mapDtiVars.end(); ++itDateTime)
	{					
		DateTime dti= itDateTime->first;
		int dtMin = dti.getDateMinutes();
		
		DateTime *pDti = problemData->getDateTimeInicial(dti);

		ConstraintOp cons;
		cons.reset();
		cons.setType(ConstraintOp::C_PROF_HOR_INIC_LB);
		cons.setProfessor(prof);
		cons.setDia(dia);
		cons.setFaseDoDia(fase);
		cons.setDateTimeInicial(pDti);

		int rhs = dtMin;
		OPT_ROW rowHi (OPT_ROW::ROWSENSE::GREATER, rhs, (char*) cons.toString().c_str());

		// Insere vars ProfTurma
		for ( auto itVar = itDateTime->second.begin(); itVar != itDateTime->second.end(); itVar++ )
		{
			rowHi.insert(itVar->first,itVar->second);

			if ( itVar->second != rhs )
				CentroDados::printError("MIPUnico::criarRestProfHiLB_()", "Coef de x diferente do rhs.");
		}

		// Insere var hfi
		rowHi.insert(colHi,1.0);
					
		lp->addRow(rowHi);
		numRestr++;
	}
	return numRestr;
}

int Operacional::criarRestricaoProfHfLB_( Professor* const prof, const int dia, const int fase,
	const int colHf, map<DateTime, set< pair<int,double> >> const &mapDtfVars )
{
	/*
		Para todo professor p, todo dia t, todo fase do dia f, todo DateTime dt:

		hfp_{p,t,f} >= sum[x t.q. dtf==dt] m(dt) * x_{p,t,h}

		aonde:
		m(dt) = datetime dt em número de minutos. Por ex: m(7:30) = 7*60 + 30
		x_{p,t,h}	= variável binária indicando se o prof p dá aula no dia t iniciando no horário h
		hfp_{p,t,f}	= variável inteira indicando o último horário (valor em número de minutos) 
					  da fase f do dia t em que prof p dá aula.
		
		"x t.q. dtf==dt" significa as variáveis x tais que o DateTime de fim da aula dtf é igual ao dt da restrição
	*/
	
	int numRestr = 0;
	
	for(auto itDateTime = mapDtfVars.begin(); itDateTime != mapDtfVars.end(); ++itDateTime)
	{					
		DateTime dtf= itDateTime->first;
		int dtMin = dtf.getDateMinutes();
		
		DateTime *pDtf = problemData->getDateTimeFinal(dtf);

		ConstraintOp cons;
		cons.reset();
		cons.setType(ConstraintOp::C_PROF_HOR_FIN_LB);
		cons.setProfessor(prof);
		cons.setDia(dia);
		cons.setFaseDoDia(fase);
		cons.setDateTimeFinal(pDtf);

		int rhs = 0.0;
		OPT_ROW rowHf (OPT_ROW::ROWSENSE::GREATER, rhs, (char*) cons.toString().c_str());

		// Insere vars ProfTurma
		for ( auto itVar = itDateTime->second.begin(); itVar != itDateTime->second.end(); itVar++ )
		{
			rowHf.insert(itVar->first,itVar->second);										
		}
										
		// Insere var hfp
		rowHf.insert(colHf,1.0);
					
		lp->addRow(rowHf);
		numRestr++;
	}
	return numRestr;
}

int Operacional::criarRestricaoProfHfUB_( Professor* const prof, const int dia, const int fase,
	const int colHf, map<DateTime, set< pair<int,double> >> const &mapDtfVars )
{
	/*
		Para todo professor p, todo dia t, todo fase do dia f, todo DateTime dt:

		hfp_{p,t,f} <= m(dt) + M*( sum[x t.q. dtf>dt] x_{p,t,h} )

		aonde:
		m(dt)		= datetime dt em número de minutos. Por ex: m(7:30) = 7*60 + 30
		x_{p,t,h}	= variável binária indicando se o prof p dá aula no dia t que inicia no horario h
		hfp_{p,t,f}	= variável inteira indicando o último horário (valor em número de minutos) 
					  da fase f do dia t em que prof p dá aula.
		
		"x t.q. dtf>dt" significa as variáveis x tais que o DateTime dtf de fim da aula é maior que o dt da restrição
	*/
	
	int numRestr = 0;
	
	for(auto itDateTime = mapDtfVars.begin(); itDateTime != mapDtfVars.end(); ++itDateTime)
	{					
		DateTime dtf = itDateTime->first;
		int dtMax = dtf.getDateMinutes();
		double rhs = dtMax;
		
		DateTime *pDtf = problemData->getDateTimeFinal(dtf);

		ConstraintOp cons;
		cons.reset();
		cons.setType(ConstraintOp::C_PROF_HOR_FIN_UB);
		cons.setProfessor(prof);
		cons.setDia(dia);
		cons.setFaseDoDia(fase);
		cons.setDateTimeFinal(pDtf);

		OPT_ROW rowHf (OPT_ROW::ROWSENSE::LESS, rhs, (char*) cons.toString().c_str());

		// Insere vars ProfTurma
		for ( auto itVar = itDateTime->second.begin(); itVar != itDateTime->second.end(); itVar++ )
		{
			rowHf.insert(itVar->first,itVar->second);
		}
		
		// Insere var hfp
		rowHf.insert(colHf,1.0);
					
		lp->addRow(rowHf);
		numRestr++;
	}

	return numRestr;
}

int Operacional::criarRestricaoProfGapMTN_( Professor* const prof, const int dia, const int fase,
	const int rhs, const int colHi, const int colHf, set< pair<int,double> > const &varsColCoef )
{
	/*
		Para todo professor p, todo dia t, todo fase do dia f:

		sum[x t.q. h e Hf] tempo_{x} * x_{p,t,h} + delta_{f,t} + fpgap_{p,t,f} >= hfp_{p,t,f} - hip_{p,t,f}

		aonde:
		duracao_{x}		= tempo de duração (min) da aula representada por x
		delta_{f,t}		= máximo de tempo ocioso permitido dentro de uma mesma fase f do dia t,
						entre a primeira e a ultima aula do professor na fase. Conta, basicamente,
						o tempo do intervalo entre aulas.
		x_{p,t,h}		= variável binária indicando se o prof p dá aula no dia t que inicia no horário h
		hfp_{p,t,f}		= variável inteira indicando o último horário (valor em número de minutos) 
						da fase f do dia t em que prof p dá aula.
		hip_{p,t,f}		= variável inteira indicando o primeiro horário (valor em número de minutos) 
						da fase f do dia t em que prof p dá aula.
		fpgap_{p,t,f}	= variável inteira de folga.

		"x t.q. h e Hf" significa as variáveis x tais que o horário h de inicio da aula 
			pertence aos horários do turno f
	*/
	
	int numRestr = 0;
	
	ConstraintOp cons;
	cons.reset();
	cons.setType(ConstraintOp::C_PROF_GAP);
	cons.setProfessor(prof);
	cons.setDia(dia);
	cons.setFaseDoDia(fase);

	OPT_ROW row (OPT_ROW::ROWSENSE::GREATER, rhs, (char*) cons.toString().c_str());

	for(auto itVars = varsColCoef.begin(); itVars != varsColCoef.end(); ++itVars)
	{					
		int col = itVars->first;
		double coef = itVars->second;
		
		// Insere vars ProfTurma
		row.insert(col,coef);				
	}		

	// Insere var hip
	row.insert(colHi,1.0);
	// Insere var hfp
	row.insert(colHf,-1.0);
	
	// Insere var fpgap
	if ( problemData->parametros->proibirProfGapMTN ==
		ParametrosPlanejamento::ConstraintLevel::Weak )
	{
		auto itFolgaProf = varsProfFolgaGap.find(prof);
		if ( itFolgaProf != varsProfFolgaGap.end() )
		{
			auto itFolgaDia = itFolgaProf->second.find(dia);
			if ( itFolgaDia != itFolgaProf->second.end() )
			{
				auto itFolgaFase = itFolgaDia->second.find(fase);
				if ( itFolgaFase != itFolgaDia->second.end() )
				{
					int colFpgap = itFolgaFase->second;
					row.insert(colFpgap,1.0);
				}
			}
		}
	}

	lp->addRow(row);
	numRestr++;

	return numRestr;
}

