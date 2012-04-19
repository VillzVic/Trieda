#include "SolverMIP.h"
#include <math.h>

#ifdef SOLVER_CPLEX
#include "opt_cplex.h"
#include "opt_cplex.cpp"
#endif

#ifdef SOLVER_GUROBI
#include "opt_gurobi.h"
#include "opt_gurobi.cpp"
#endif

/*==================================================================/
%DocBegin TRIEDA_LOAD_MODEL
%Title M�dulo T�tico

%ProbSense MIN

%Set CP 
%Desc 
Conjunto de campus. Os elementos desse conjunto s�o denotados por $cp$.

%Set U
%Desc 
Conjunto de unidades. Os elementos desse conjunto s�o denotados por $u$.

%Set S_{u}
%Desc 
Conjunto de salas da unidade $u$. Os elementos desse conjunto s�o 
denotados por $s$.

%Set SCAP_{u}
%Desc 
Conjunto de salas da unidade $u$ classificadas de acordo com as suas 
capacidades. Os elementos desse conjunto s�o denotados por $tps$.

%Set T 
%Desc 
Conjunto de dias letivos da semana. Os elementos desse conjunto 
s�o denotados por $t$.

%Set C
%Desc 
Conjunto de cursos. Os elementos desse conjunto s�o denotados por $c$.

%Set CC
%Desc 
Conjunto de cursos compat�veis.

%Set D
%Desc 
Conjunto de disciplinas. Os elementos desse conjunto s�o denotados por $d$.

%Set B
%Desc 
Conjunto de blocos curriculares. Os elementos desse conjunto s�o 
denotados por $bc$.

%Set D_{bc}
%Desc 
Conjunto de disciplinas que pertencem ao bloco curricular $bc$. 
Os elementos desse conjunto s�o denotados por $d_{bc}$.

%Set I_{bc}
%Desc 
Conjunto de turmas de um bloco curricular $bc$. 
Os elementos desse conjunto s�o denotados por $i_{bc}$.

%Set O
%Desc 
Conjunto de ofertas de cursos. Os elementos desse conjunto s�o 
denotados por $oft$.

%Set D_{oft}
%Desc 
Conjunto de disciplinas de uma oferta $oft$. 
Os elementos desse conjunto s�o denotados por $d_{oft}$.

%Set O_{d}
%Desc 
Conjunto de ofertas de uma uma disciplina $d$. 
Os elementos desse conjunto s�o denotados por $oft_{d}$.

%Set K_{d}
%Desc 
Conjunto de combina��es poss�veis de divis�o de cr�ditos de uma uma disciplina $d$. 
Os elementos desse conjunto s�o denotados por $k$.

%DocEnd
/===================================================================*/

/*==================================================================/
%DocBegin TRIEDA_LOAD_MODEL

%Data A_{u,s}
%Desc
capacidade da sala $s$ da unidade $u$.


%Data C_{d}
%Desc
Total de cr�ditos da disciplina $d$.

%Data \overline{H_{d}}
%Desc 
m�ximo de cr�ditos di�rios da disciplina $d$.

%Data \underline{H_{d}}
%Desc 
m�nimo de cr�ditos di�rios da disciplina $d$.

%Data I_{d}
%Desc 
m�ximo de turmas que podem ser abertas da disciplina $d$.

%Data P_{d,c,cp}
%Desc 
demanda da disciplina $d$ no campus $cp$ para o curso $c$.

%Data P_{d,oft}
%Desc
demanda da disciplina $d$ da oferta $oft$.

%Data Pmax_{d}
%Desc 
maior demanda da disciplina $d$.

%Data H_{t}
%Desc
m�ximo de cr�ditos permitidos por dia $t$.

%Data A_{u,s}
%Desc 
capacidade da sala $s$ da unidade $u$.

%Data HTPS_{t,tps}
%Desc 
m�ximo de cr�ditos permitidos por dia $t$ para o conjunto de salas do tipo (capacidade) $tps$.

%Data A_{u,tps}
%Desc 
capacidade total das salas de um conjunto de salas do tipo (capacidade) $tps$ da unidade $u$.

%Data O_{cp}
%Desc
conjunto de ofertas de um campus $cp$.

%Data FC_{d,t}
%Desc 
n�mero de cr�ditos fixados para a disciplina $d$ no dia $t$.

%Data N_{d,k,t}
%Desc 
n�mero de cr�ditos determinados para a disciplina $d$ no dia $t$ na combina��o de divis�o de cr�dito $k$.

%Data M
%Desc 
big $M$.

%Data \alpha
%Desc 
peso associado a fun��o objetivo.
%Data \beta
%Desc 
peso associado a fun��o objetivo.
%Data \gamma
%Desc 
peso associado a fun��o objetivo.
%Data \delta
%Desc 
peso associado a fun��o objetivo.
%Data \lambda
%Desc 
peso associado a fun��o objetivo.
%Data \rho
%Desc 
peso associado a fun��o objetivo.
%Data \xi
%Desc 
pesos associados a cada item da fun��o objetivo.
%Data \psi
%Desc 
peso associado a fun��o objetivo.
%Data \theta
%Desc 
peso associado a fun��o objetivo.
%Data \omega
%Desc 
peso associado a fun��o objetivo.
%Data \tau
%Desc 
peso associado a fun��o objetivo.
%Data \eta
%Desc 
peso associado a fun��o objetivo.

%DocEnd
/===================================================================*/

#ifndef WIN32
struct ordenaDiscSalas {
   bool operator()(const std::pair< int /*Disc id*/, int /*Qtd salas associadas*/ > & left,
                   const std::pair< int /*Disc id*/, int /*Qtd salas associadas*/ > & right) const
   {
      return ( left.second < right.second );
   }
};
#else
bool ordenaDiscSalas( std::pair< int /*Disc id*/, int /*Qtd salas associadas*/ > & left,
                      std::pair< int /*Disc id*/, int /*Qtd salas associadas*/ > & right )
{
   return ( left.second < right.second );
}
#endif

bool ordenaVarsX( Variable * left, Variable * right )
{
   if ( left->getUnidade() > right->getUnidade() )
   {
      return false;
   }
   else
   {
      if ( left->getTurma() > right->getTurma() )
      {
         return false;
      }
      else
      {
         if ( left->getDia() > right->getDia() )
         {
            return false;
         }
      }
   }

   return true;
}

SolverMIP::SolverMIP( ProblemData * aProblemData,
  ProblemSolution * _ProblemSolution, ProblemDataLoader * _problemDataLoader )
   : Solver( aProblemData )
{
   problemSolution = _ProblemSolution;
   problemDataLoader = _problemDataLoader;

   alpha = 5.0;
   beta = 10.0;
   gamma = 0;
   delta = 1.0;
   lambda = 10.0;
   epsilon = 1.0;
   M = 1.0;
   rho = 5;

   // Verificar o valor
   psi = 5.0;
   tau = 1.0;

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

   solVars.clear();
}

SolverMIP::~SolverMIP()
{
   int i;
   if ( lp != NULL )
   {
      delete lp;
   }

   for ( i = 0; i < (int)solVars.size(); i++ )
   {
      if ( solVars[ i ] != NULL )
      {
         delete solVars[ i ];
      }
   }

   solVars.clear();
}

bool SolverMIP::SolVarsPreFound( VariablePre v )
{
	std::set< VariablePre  >::iterator it = solVarsPre.find(v);
	if(it != solVarsPre.end() )
		return true;
	else
		return false;
}

void SolverMIP::carregaVariaveisSolucaoTatico( int campusId )
{
   double * xSol = NULL;
   VariableHash::iterator vit;

   SolutionLoader sLoader( problemData, problemSolution );

   xSol = new double[ lp->getNumCols() ];

#ifndef READ_SOLUTION_TATICO_BIN
   lp->getX( xSol );
#endif

#ifdef READ_SOLUTION_TATICO_BIN
   FILE* fin = fopen("solBin.bin","rb");

   int nCols = 0;

   fread(&nCols,sizeof(int),1,fin);

   if ( nCols == lp->getNumCols() )
   {
      for (int i =0; i < nCols; i++)
      {
         double auxDbl;
         fread(&auxDbl,sizeof(double),1,fin);
         xSol[i] = auxDbl;
      }
   }

   fclose(fin);
#endif

   vit = vHash.begin();
   
   char solFilename[1024], id[100];
   strcpy( solFilename, "solucaoTatico" );
   _itoa_s( campusId, id, 100, 10 );
   strcat( solFilename, id );
   strcat( solFilename, ".txt" );

   FILE * fout = fopen( solFilename, "wt" );

   while ( vit != vHash.end() )
   {
      Variable * v = new Variable( vit->first );
      int col = vit->second;
      v->setValue( xSol[ col ] );

      if ( v->getValue() > 0.00001 )
      {
         //#ifdef DEBUG
         char auxName[100];
         lp->getColName( auxName, col, 100 );
         fprintf( fout, "%s = %f\n", auxName, v->getValue() );
         //#endif
         switch( v->getType() )
         {
         case Variable::V_ERROR:
            std::cout << "Vari�vel inv�lida " << std::endl;
            break;
         case Variable::V_CREDITOS:
            std::cout << "Oferta de " << v->getValue()
                      << " creditos da disciplina " << v->getDisciplina()->getCodigo()
                      << " para a turma " << v->getTurma()
                      << " no dia " << v->getDia()
                      << " para alguma de sala do conjunto de salas " << v->getSubCjtSala()->getId()
                      << std::endl << std::endl;
            vars_x.push_back(v);
            break;
         case Variable::V_OFERECIMENTO: break;
         case Variable::V_ABERTURA: break;
         case Variable::V_ALUNOS:
		 {
            std::cout << "Oferecimento de " << v->getValue()
                      << " vagas da disciplina " << v->getDisciplina()->getCodigo()
                      << " para a turma " << v->getTurma()
                      << " do curso " << v->getOferta()->curso->getCodigo()
                      << std::endl << std::endl;
            vars_a[ std::make_pair( v->getTurma(), v->getDisciplina() ) ].push_back( v );
			break;
		 }
         case Variable::V_ALOC_ALUNO: break;
         case Variable::V_N_SUBBLOCOS: break;
         case Variable::V_DIAS_CONSECUTIVOS: break;
         case Variable::V_MIN_CRED_SEMANA: break;
         case Variable::V_MAX_CRED_SEMANA: break;
         case Variable::V_ALOC_DISCIPLINA: break;
         case Variable::V_N_ABERT_TURMA_BLOCO: break;
         case Variable::V_SLACK_DIST_CRED_DIA_SUPERIOR: break;
         case Variable::V_SLACK_DIST_CRED_DIA_INFERIOR: break;
         }
      }

      vit++;
   }

   vit = vHash.begin();

   for (; vit != vHash.end(); ++vit )
   {
      Variable * v = new Variable( vit->first );
      int col = vit->second;
      v->setValue( xSol[ col ] );

      if ( v->getValue() > 0.00001 )
      {
         sLoader.setFolgas( v );
      }

      delete v;
   }

   //#ifdef DEBUG
   if ( fout )
   {
      fclose( fout );
   }
   //#endif

   if ( xSol )
   {
      delete [] xSol;
   }

   // ---------------------------------------
   // Imprimindo as vari�veis x_{i,d,u,tps,t} coletadas.

   std::cout << "x\t\ti\td\t\tu\ttps\tt\n";

   ITERA_VECTOR( it_Vars_x, vars_x, Variable )
   {
      std::cout << ( *it_Vars_x )->getValue() << "\t\t"
                << ( *it_Vars_x )->getTurma() << "\t"
                << ( *it_Vars_x )->getDisciplina()->getCodigo() << "\t\t"
                << ( *it_Vars_x )->getUnidade()->getCodigo() << "\t"
                << ( *it_Vars_x )->getSubCjtSala()->getId() << "\t"
                << ( *it_Vars_x )->getDia() << "\n";
   }

   std::cout << "\n\n\n";

   // Imprimindo as vari�veis a_{i,d,o} coletadas.
   std::cout << "a\t\ti\td\to\tCod.Curso\n";

   vars__A___i_d_o::iterator it_Vars_a = vars_a.begin();

   for (; it_Vars_a != vars_a.end(); ++it_Vars_a )
   {
      ITERA_VECTOR( it_Vars_a_TEMP, it_Vars_a->second, Variable )
      {
         std::cout << ( *it_Vars_a_TEMP )->getValue() << "\t\t"
                   << ( *it_Vars_a_TEMP )->getTurma() << "\t"
                   << ( *it_Vars_a_TEMP )->getDisciplina()->getCodigo() << "\t"
                   << ( *it_Vars_a_TEMP )->getOferta()->getId() << "\t"
                   << ( *it_Vars_a_TEMP )->getOferta()->curso->getCodigo() << "\n";
      }
   }
}

void SolverMIP::carregaVariaveisSolucaoTaticoPorAluno( int campusId, int prioridade )
{
   double * xSol = NULL;
   VariableTaticoHash::iterator vit;

   SolutionLoader sLoader( problemData, problemSolution );

   xSol = new double[ lp->getNumCols() ];

#ifndef READ_SOLUTION_TATICO_BIN
   lp->getX( xSol );
#endif

#ifdef READ_SOLUTION_TATICO_BIN
   FILE* fin = fopen("solBin.bin","rb");

   int nCols = 0;

   fread(&nCols,sizeof(int),1,fin);

   if ( nCols == lp->getNumCols() )
   {
      for (int i =0; i < nCols; i++)
      {
         double auxDbl;
         fread(&auxDbl,sizeof(double),1,fin);
         xSol[i] = auxDbl;
      }
   }

   fclose(fin);
#endif

   std::map< std::pair<Disciplina*, Oferta*>, int > mapSlackDemanda;
   std::map< std::pair<Disciplina*, Oferta*>, int >::iterator itMapSlackDemanda;

   vars_xh.clear();

   vit = vHashTatico.begin();
   
   char solFilename[1024], id[100];
   strcpy( solFilename, "solucaoTatico" );
   _itoa_s( campusId, id, 100, 10 );
   strcat( solFilename, id );
   _itoa_s( prioridade, id, 100, 10 );
   strcat( solFilename, "_P" );
   strcat( solFilename, id );
   strcat( solFilename, ".txt" );

   FILE * fout = fopen( solFilename, "wt" );

   while ( vit != vHashTatico.end() )
   {
      VariableTatico * v = new VariableTatico( vit->first );
      int col = vit->second;
      v->setValue( xSol[ col ] );

      if ( v->getValue() > 0.00001 )
      {
         //#ifdef DEBUG
         char auxName[100];
         lp->getColName( auxName, col, 100 );
         fprintf( fout, "%s = %f\n", auxName, v->getValue() );
         //#endif

		 Trio< int, int, Disciplina* > trio;

         switch( v->getType() )
         {
			 case VariableTatico::V_ERROR:
				std::cout << "Vari�vel inv�lida " << std::endl;
				break;
			 case VariableTatico::V_CREDITOS:				 					 
				 
				 trio.set( v->getUnidade()->getIdCampus(), v->getTurma(), v->getDisciplina() );

				 std::cout << problemData->mapCampusTurmaDisc_AlunosDemanda[trio].size() << " vagas para a oferta"
				   		   << " de " << v->getDisciplina()->getCalendario()->retornaNroCreditosEntreHorarios
															( v->getHorarioAulaInicial(), v->getHorarioAulaFinal() )
						  << " creditos da disciplina " << v->getDisciplina()->getCodigo()
						  << " para a turma " << v->getTurma()
						  << " no dia " << v->getDia()
						  << " no horario " << v->getHorarioAulaInicial()->getId()
						  << " para a sala " << v->getSubCjtSala()->salas.begin()->first
						  << std::endl << std::endl;
				 vars_xh.add(v);
				break;
			 case VariableTatico::V_ABERTURA: break;
			 case VariableTatico::V_DIAS_CONSECUTIVOS: break;
			 case VariableTatico::V_MIN_CRED_SEMANA: break;
			 case VariableTatico::V_MAX_CRED_SEMANA: break;
			 case VariableTatico::V_SLACK_DIST_CRED_DIA_SUPERIOR: break;
			 case VariableTatico::V_SLACK_DIST_CRED_DIA_INFERIOR: break;

			 case VariableTatico::V_SLACK_DEMANDA:

				 Disciplina *d = v->getDisciplina();		 
				 Oferta *oft = v->getAluno()->getOferta();
				 int alunoId = v->getAluno()->getAlunoId();

				 AlunoDemanda* ad = problemData->procuraAlunoDemanda( d->getId(), alunoId );
				 problemData->listSlackDemandaAluno.add( ad );

				 itMapSlackDemanda = mapSlackDemanda.find( std::make_pair( d, oft ) );
				 if ( itMapSlackDemanda != mapSlackDemanda.end() )
					 itMapSlackDemanda->second = itMapSlackDemanda->second + 1;
				 else
					mapSlackDemanda[ std::make_pair( d, oft ) ] = 1;

				 break;
         }
      }
	  else
	  {
		 delete v;
	  }

      vit++;
   }
	std::cout << std::endl;

   // Escreve um resumo do n�o-atendimento das demandas, juntando
   // as demandas n�o atendidas por oferta.
	itMapSlackDemanda = mapSlackDemanda.begin();
	for ( ; itMapSlackDemanda != mapSlackDemanda.end() ; itMapSlackDemanda++ )
	{
		fprintf( fout, "FD_{_Disc%d_Oft%d} = %d\n",
			itMapSlackDemanda->first.first->getId(), 
			itMapSlackDemanda->first.second->getId(), 
			itMapSlackDemanda->second );
	}

	std::cout << std::endl;

	// -----------------------------------------------------------------------------
	// Retira dos maps mapAluno_CampusTurmaDisc e mapCampusTurmaDisc_AlunosDemanda
	// os atendimentos incompletos ( atendeu s� disc pratica ou s� teorica ), e os
	// acrescenta em listSlackDemandaAluno.
	// Deleta os atendimentos em vars_xh que n�o tiverem nenhum aluno alocado.
    ITERA_GGROUP_LESSPTR( itSlack, problemData->listSlackDemandaAluno, AlunoDemanda )
	{		
		Aluno *aluno = problemData->retornaAluno( itSlack->getAlunoId() );
		int campusId = itSlack->demanda->oferta->getCampusId();
		int discId = - itSlack->demanda->getDisciplinaId();

		// Se existir a disciplina teorica/pratica correspondente
		if ( problemData->refDisciplinas.find( discId ) != 
			 problemData->refDisciplinas.end() )
		{
			Disciplina *disciplina = problemData->refDisciplinas[ discId ];

			// Se o aluno tiver alocado em alguma turma da disciplina
			// retira-o, eliminando atendimento parcial
			int turma = problemData->retornaTurmaDiscAluno( aluno, disciplina );
			if ( turma != -1 )
			{
				AlunoDemanda *ad = problemData->procuraAlunoDemanda( discId, aluno->getAlunoId() );

				Trio< int /*campusId*/, int /*turma*/, Disciplina* > trio;
				trio.set( campusId, turma, disciplina );

				problemData->mapAluno_CampusTurmaDisc[aluno].remove( trio );
				
				problemData->mapCampusTurmaDisc_AlunosDemanda[ trio ].remove( ad );
				 
				problemData->listSlackDemandaAluno.add( ad );

				int nroAlunos = problemData->mapCampusTurmaDisc_AlunosDemanda[ trio ].size();

				if ( nroAlunos == 0 )
				{
					GGroup< VariableTatico *> remover;
					GGroup< VariableTatico *> todos;

					ITERA_GGROUP( itVarXh, vars_xh, VariableTatico )
					{
						VariableTatico *v = *itVarXh;
						
						todos.add( v );

						if ( v->getTurma() == turma &&
							 v->getDisciplina() == disciplina &&
							 problemData->retornaCampus( v->getUnidade()->getId() )->getId() == campusId )
						{
							remover.add( v );
						}
					}
					vars_xh.clear();
					ITERA_GGROUP( itVarXh, todos, VariableTatico ) // o erase do vector n�o funcionou!! pq?? por isso usei clear e add
					{
						bool inserir = true;
						ITERA_GGROUP( itRemover, remover, VariableTatico )
						{
							if ( *itRemover == *itVarXh )
								inserir = false;
						}
						if ( inserir )
							vars_xh.add( *itVarXh );
					}

					problemData->mapCampusTurmaDisc_AlunosDemanda.erase( trio );
				}
			}
		}
	}
	// -----------------------------------------------------------------------------

   vit = vHashTatico.begin();
   for (; vit != vHashTatico.end(); ++vit )
   {
      VariableTatico * v = new VariableTatico( vit->first );
      int col = vit->second;
      v->setValue( xSol[ col ] );

      if ( v->getValue() > 0.00001 )
      {
         sLoader.setFolgas( v );
      }

      delete v;
   }

   //#ifdef DEBUG
   if ( fout )
   {
      fclose( fout );
   }
   //#endif

   if ( xSol )
   {
      delete [] xSol;
   }

}


int SolverMIP::solveTatico( int campusId )
{
   int varNum = 0;
   int constNum = 0;

   if ( problemData->parametros->funcao_objetivo == 0
      || problemData->parametros->funcao_objetivo == 2 )
   {
      lp->createLP( "Solver Trieda", OPTSENSE_MAXIMIZE, PROB_MIP );
   }
   else if( problemData->parametros->funcao_objetivo == 1 )
   {
      lp->createLP( "Solver Trieda", OPTSENSE_MINIMIZE, PROB_MIP );
   }

#ifdef DEBUG
   printf( "Creating LP...\n" );
#endif

   // Variable creation
   varNum = cria_variaveis( campusId );

   lp->updateLP();

#ifdef PRINT_cria_variaveis
   printf( "Total of Variables: %i\n\n", varNum );
#endif

   // Constraint creation
   constNum = cria_restricoes( campusId );

   lp->updateLP();

#ifdef PRINT_cria_restricoes
   printf( "Total of Constraints: %i\n\n", constNum );
#endif

    lp->writeProbLP( "Solver Trieda" );
#ifdef DEBUG
   //lp->writeProbLP( "Solver Trieda" );
#endif

   int status = 0;

#ifndef READ_SOLUTION_TATICO_BIN

   // Muda FO para considerar somente atendimento
   double * objOrig = new double[ lp->getNumCols() ];

   lp->getObj( 0, lp->getNumCols()-1, objOrig );
   double * objNova = new double[ lp->getNumCols() ];
   int * idxNova = new int[ lp->getNumCols() ];

   for ( int i = 0; i < lp->getNumCols(); i++ )
   {
      objNova[ i ] = 0;
      idxNova[ i ] = i;
   }

   VariableHash::iterator vit = vHash.begin();

   for (; vit != vHash.end(); vit++ )
   {
      if ( vit->first.getType() == Variable::V_SLACK_DEMANDA )
      {
         objNova[ vit->second ] = 1.0;
      }
      
   }

   lp->chgObj( lp->getNumCols(), idxNova, objNova );

   //lp->setHeurFrequency( 1.0 );

#ifdef DEBUG
   lp->setTimeLimit( 3600 );
#else
   lp->setTimeLimit( 600 );
#endif

   lp->setMIPRelTol( 0.02 );
   //lp->setMIPStartAlg( METHOD_PRIMAL );
   lp->setMIPEmphasis( 0 );
   lp->setMIPScreenLog( 4 );
   // lp->setNoCuts();
   // lp->setNodeLimit( 1 );
   lp->setPreSolve( OPT_TRUE );

   // Resolve problema olhando somente atendimento
   status = lp->optimize( METHOD_MIP );

   // Passa solucao inicial obtida e fixa atendimento
   double * xSolInic = new double[ lp->getNumCols() ];
   lp->getX( xSolInic );

   double lbAtend = lp->getBestObj();
   double ubAtend = lp->getObjVal();

   OPT_ROW rowLB( 100, OPT_ROW::GREATER , lbAtend , "LBATEND" );
   OPT_ROW rowUB( 100, OPT_ROW::LESS , ubAtend , "UBATEND" );

   vit = vHash.begin();

   for (; vit != vHash.end(); vit++ )
   {
      if ( vit->first.getType() == Variable::V_SLACK_DEMANDA )
      {
         rowLB.insert( vit->second, 1.0 );
         rowUB.insert( vit->second, 1.0 );
      }

#ifdef SOLVER_GUROBI
      if ( vit->first.getType() == Variable::V_N_SUBBLOCOS )
      {
         xSolInic[ vit->second ] = GRB_UNDEFINED;
      }
#endif
   }

   lp->addRow( rowLB );
   lp->addRow( rowUB );
   lp->updateLP();

   lp->setHeurFrequency( 1.0 );
   lp->setTimeLimit( 600 );
   lp->setMIPRelTol( 0.02 );
   // lp->setMIPStartAlg( METHOD_PRIMAL );
   lp->setMIPEmphasis( 1 );
   lp->setMIPScreenLog( 4 );
   // lp->setNoCuts();
   // lp->setNodeLimit( 1 );
   lp->setPreSolve( OPT_TRUE );
   lp->copyMIPStartSol( lp->getNumCols(), idxNova, xSolInic );
   lp->chgObj( lp->getNumCols(), idxNova, objOrig );

   status = localBranching( xSolInic, 1200.0 );

   delete [] objNova;
   delete [] objOrig;
   delete [] idxNova;
   delete [] xSolInic;
#endif

#ifdef WRITE_SOLUTION_TATICO_BIN
   double * xSol = NULL;
   xSol = new double[ lp->getNumCols() ];
   lp->getX( xSol );
   FILE * fout = fopen( "solBin.bin", "wb" );
   int nCols = lp->getNumCols();

   fwrite( &nCols, sizeof( int ), 1, fout );
   for ( int i = 0; i < lp->getNumCols(); i++ )
   {
      fwrite( &( xSol[i] ), sizeof( double ), 1, fout );
   }

   fclose( fout );

   delete [] xSol;
#endif

   return status;
}


/*
	Retorna o valor minimo a ser assumido pela variavel em sua cria��o.
	Fun��o usada a partir da itera��o de prioridade 2 de demandas, a fim de
	considerar a otimiza��o do tatico da itera��o anterior.
*/
int SolverMIP::fixaLimiteInferiorVariavelPre( VariablePre *v )
{
#ifndef PRE_TATICO
	return 0;
#endif

#ifdef PRE_TATICO
			   
	return 0;

	switch( v->getType() )
	{
		
		 case VariablePre::V_ERROR:
		 {
			 return true;
		 }
		 case VariablePre::V_PRE_CREDITOS:  //  x_{i,d,u,s} 
		 {
			 // x_{i,d,u,s,hi,hf,t}
			 ITERA_GGROUP( it_Vars_x, vars_xh, VariableTatico )
			 {
				 VariableTatico *vSol = *it_Vars_x;
				 if ( vSol->getTurma() == v->getTurma() &&
					  vSol->getDisciplina() == v->getDisciplina() &&
					  vSol->getUnidade() == v->getUnidade() &&
					  vSol->getSubCjtSala() == v->getSubCjtSala() )
				 {
					 HorarioAula *hf = vSol->getHorarioAulaFinal();
					 HorarioAula *hi = vSol->getHorarioAulaInicial();

					 int nCreditos = vSol->getDisciplina()->getCalendario()->retornaNroCreditosEntreHorarios( hi, hf );

					 return nCreditos; 
				 }
				
			 }

			 return 0;

		 }
		 case VariablePre::V_PRE_OFERECIMENTO:  //  o_{i,d,u,s}
		 {
			 // x_{i,d,u,s,hi,hf,t}
			 ITERA_GGROUP( it_Vars_x, vars_xh, VariableTatico )
			 {
				 VariableTatico *vSol = *it_Vars_x;
				 if ( vSol->getTurma() == v->getTurma() &&
					  vSol->getDisciplina() == v->getDisciplina() &&
					  vSol->getUnidade() == v->getUnidade() &&
					  vSol->getSubCjtSala() == v->getSubCjtSala() )
					  return 1;
			 }

			 return 0;
		 }
		case VariablePre::V_PRE_ABERTURA: // z_{i,d,cp}
		 {
			 // x_{i,d,u,s,hi,hf,t}
			 ITERA_GGROUP( it_Vars_x, vars_xh, VariableTatico )
			 {
				 VariableTatico *vSol = *it_Vars_x;
				 if ( vSol->getTurma() == v->getTurma() &&
					  vSol->getDisciplina() == v->getDisciplina() &&
					  problemData->retornaCampus( vSol->getUnidade()->getId() ) == v->getCampus() )
					  return 1;
			 }

			 return 0;
		 }
		 case VariablePre::V_PRE_ALUNOS:  //  a_{i,d,s,oft}
		 {
			 int ofertaId = v->getOferta()->getId();
			 int discId = v->getDisciplina()->getId();
			 int turma = v->getTurma();

			 int nroDeAtendimentos = problemData->atendeTurmaDiscOferta( turma, discId, ofertaId );

			 if ( ! nroDeAtendimentos )
			 {
				return 0;
			 } 

			 // x_{i,d,u,s,hi,hf,t}
			 ITERA_GGROUP( it_Vars_x, vars_xh, VariableTatico )
			 {
				 VariableTatico *vSol = *it_Vars_x;
				 if ( vSol->getTurma() == v->getTurma() &&
					  vSol->getDisciplina() == v->getDisciplina() &&
					  vSol->getUnidade() == v->getUnidade() &&
					  vSol->getSubCjtSala() == v->getSubCjtSala() )
				 {					 
					return nroDeAtendimentos;
				 }
			 }
			 return 0;
		 }
		 case VariablePre::V_PRE_ALOC_ALUNO:  //  b_{i,d,c}
		 {
			 Curso *curso = v->getCurso();

			 ITERA_GGROUP_LESSPTR( itOft, problemData->ofertas, Oferta )
			 {
				 if ( itOft->curso == curso )
				 {
					 int ofertaId = itOft->getId();
					 int discId = v->getDisciplina()->getId();
					 int turma = v->getTurma();

					 if ( problemData->atendeTurmaDiscOferta( turma, discId, ofertaId ) )
					 {
						return 1;
					 } 					
				 }
			 }
			 return 0;
		 }
		 case VariablePre::V_PRE_SLACK_DEMANDA: // fd_{d,a}
		 {	
			 return 0;
		 } 		 
		 case VariablePre::V_PRE_SLACK_COMPARTILHAMENTO:  // fc_{i,d,c1,c2}
		 {
			 Curso* curso1 = v->getParCursos().first;
			 Curso* curso2 = v->getParCursos().second;
			 int discId = v->getDisciplina()->getId();
			 int turma = v->getTurma();

			 ITERA_GGROUP_LESSPTR( itOft1, problemData->ofertas, Oferta )
			 {
				 if ( itOft1->curso == curso1 )
				 {
					int ofertaId = itOft1->getId();

					if ( problemData->atendeTurmaDiscOferta( turma, discId, ofertaId ) )
					{
						 ITERA_GGROUP_INIC_LESSPTR( itOft2, itOft1, problemData->ofertas, Oferta )
						 {
							 if ( itOft2->curso == curso2 )
							 {
								 int ofertaId = itOft2->getId();

								 if ( problemData->atendeTurmaDiscOferta( turma, discId, ofertaId ) )
								 {
									return 1;
								 } 
							 }
						 }
					} 
				 }
			 }

			 return 0;
		 }		 
		 case VariablePre::V_PRE_SLACK_SALA:  //  fs_{d,s,oft}
		 {
			 int ofertaId = v->getOferta()->getId();
			 int discId = v->getDisciplina()->getId();
			 int n = 0;

			 // x_{i,d,u,s,hi,hf,t}
			 ITERA_GGROUP( it_Vars_x, vars_xh, VariableTatico )
			 {
				 VariableTatico *vSol = *it_Vars_x;
				 if ( vSol->getDisciplina() == v->getDisciplina() &&
					  vSol->getUnidade() == v->getUnidade() &&
					  vSol->getSubCjtSala() == v->getSubCjtSala() )
				 {			
					 int turma = vSol->getTurma();
					 if ( problemData->atendeTurmaDiscOferta( turma, discId, ofertaId ) )
					 {
						n++;
					 }
				 }
			 }

 			 if ( n <= 1 )
				return 0;
			 else
				 return n-1;
		 }
		 case VariablePre::V_PRE_LIM_SUP_CREDS_SALA:  //  Hs_{cp}
		 {
			  return 0;
		 }
		 case VariablePre::V_PRE_ALOC_ALUNO_OFT:  //  c_{i,d,u,s,oft}
		 {
			 int ofertaId = v->getOferta()->getId();
			 int discId = v->getDisciplina()->getId();
			 int turma = v->getTurma();

			 if ( ! problemData->atendeTurmaDiscOferta( turma, discId, ofertaId ) )
			 {
				return 0;
			 } 

			 // x_{i,d,u,s,hi,hf,t}
			 ITERA_GGROUP( it_Vars_x, vars_xh, VariableTatico )
			 {
				 VariableTatico *vSol = *it_Vars_x;
				 if ( vSol->getTurma() == v->getTurma() &&
					  vSol->getDisciplina() == v->getDisciplina() &&
					  vSol->getUnidade() == v->getUnidade() &&
					  vSol->getSubCjtSala() == v->getSubCjtSala() )
				 {					 
					return 1;
				 }
			 }
			 return 0;
		 }
		 case VariablePre::V_PRE_ALOCA_ALUNO_TURMA_DISC:  // s_{a,i,d,cp}
		 {
			 Aluno* aluno = v->getAluno();
			 int campusId = v->getCampus()->getId();
			 Disciplina *disciplina = v->getDisciplina();
			 int turma = v->getTurma();

		 	 std::map< Aluno*, GGroup< Trio< int /*campusId*/, int /*turma*/, Disciplina* > > >::iterator itMap =
				problemData->mapAluno_CampusTurmaDisc.find( aluno );
			 
			 if ( itMap == problemData->mapAluno_CampusTurmaDisc.end() )
				 return 0;

			 Trio< int, int, Disciplina* > trio;
			 trio.set( campusId, turma, disciplina );

			 GGroup< Trio< int , int, Disciplina* > >::iterator itGGroup =
				 itMap->second.find( trio );
			 
			 if ( itGGroup != itMap->second.end() )
			 {
				  return 1;
			 }

			 return 0;
		 }

		 default:
		 {
			return 0;
			break;
		 }
	}

#endif

}


bool SolverMIP::criaVariavelTatico( VariableTatico *v )
{
#ifndef PRE_TATICO
	return true;
#endif

#ifdef PRE_TATICO

	VariablePre preV;
		   
	switch( v->getType() )
	{
		
		 case VariableTatico::V_ERROR:
		 {
			 return true;
		 }
		 case VariableTatico::V_CREDITOS:  //  x_{i,d,u,s,hi,hf,t} 
		 {
			 preV.reset();
			 preV.setType( VariablePre::V_PRE_CREDITOS ); // x_{i,d,u,s}
			 preV.setTurma( v->getTurma() );
			 preV.setDisciplina( v->getDisciplina() );
			 preV.setUnidade( v->getUnidade() );
			 preV.setSubCjtSala( v->getSubCjtSala() );

			 if ( SolVarsPreFound(preV) )
				return true;
			 else
				return false;
		 }
		case VariableTatico::V_ABERTURA: // z_{i,d,cp}
		 {
			 preV.reset();
			 preV.setType( VariablePre::V_PRE_ABERTURA ); // z_{i,d,cp}
			 preV.setTurma( v->getTurma() );
			 preV.setDisciplina( v->getDisciplina() );
			 preV.setCampus( v->getCampus() );
		 		 
			 if ( SolVarsPreFound(preV) )
				return true;
			 else
				return false;
		 }
		 case VariableTatico::V_DIAS_CONSECUTIVOS: // c_{i,d,t}
		 {
			 preV.reset();
			 preV.setType( VariablePre::V_PRE_ABERTURA ); // z_{i,d,cp}
			 preV.setTurma( v->getTurma() );
			 preV.setDisciplina( v->getDisciplina() );

			 ITERA_GGROUP_LESSPTR( itCp, problemData->campi, Campus )
			 {
				 preV.setCampus( *itCp );
				 if ( SolVarsPreFound( preV ) )
					 return true;
			 }
			 return false;
		 }
		 case VariableTatico::V_MAX_CRED_SEMANA: // H_{a}
		 {
			 Aluno *aluno = v->getAluno();

			 preV.reset();
			 preV.setType( VariablePre::V_PRE_ALOCA_ALUNO_TURMA_DISC ); //s_{i,d,a,cp}
			 preV.setAluno( aluno );
			 		 	
			 ITERA_GGROUP_LESSPTR( itCampi, problemData->campi, Campus )
			 {
				 preV.setCampus( *itCampi );
				 ITERA_GGROUP_LESSPTR( itAlDemanda, aluno->demandas, AlunoDemanda )
				 {
					 Disciplina* d = (*itAlDemanda)->demanda->disciplina;
					 preV.setDisciplina( d );
				 
					 for ( int turma = 0; turma < d->getNumTurmas(); turma++ )
					 {
						preV.setTurma( turma );
						if ( SolVarsPreFound( preV ) )
							return true;
					 }
				 }
			 }
			 return false;
		 }
		 case VariableTatico::V_MIN_CRED_SEMANA: // h_{a}
		 {
			 Aluno *aluno = v->getAluno();

			 preV.reset();
			 preV.setType( VariablePre::V_PRE_ALOCA_ALUNO_TURMA_DISC ); //s_{i,d,a,cp}
			 preV.setAluno( aluno );
			 		 	
			 ITERA_GGROUP_LESSPTR( itCampi, problemData->campi, Campus )
			 {
				 preV.setCampus( *itCampi );
				 ITERA_GGROUP_LESSPTR( itAlDemanda, aluno->demandas, AlunoDemanda )
				 {
					 Disciplina* d = itAlDemanda->demanda->disciplina;
					 preV.setDisciplina( d );
				 
					 for ( int turma = 0; turma < d->getNumTurmas(); turma++ )
					 {
						preV.setTurma( turma );
						if ( SolVarsPreFound( preV ) )
							return true;
					 }						 
				 }
			 }
			 return false;
		 }
		 case VariableTatico::V_ALUNO_UNID_DIA:// y_{a,u,t}
		 {
			 Aluno *aluno = v->getAluno();
			 Unidade *unid = v->getUnidade();
			 Campus *cp = problemData->retornaCampus( unid->getId() );
			 
			 ITERA_GGROUP_LESSPTR( itAlDemanda, aluno->demandas, AlunoDemanda )
			 {
				 Disciplina* d = itAlDemanda->demanda->disciplina;
				 				 
				 for ( int turma = 0; turma < d->getNumTurmas(); turma++ )
				 {
					preV.reset();
					preV.setType( VariablePre::V_PRE_ALOCA_ALUNO_TURMA_DISC ); //s_{i,d,a,cp}
					preV.setAluno( aluno );
					preV.setTurma( turma );
					preV.setDisciplina( d );
					preV.setCampus( cp );

					if ( SolVarsPreFound( preV ) )
					{
						ITERA_GGROUP_LESSPTR( itCjtSala, unid->conjutoSalas, ConjuntoSala )
						{
							preV.reset();
							preV.setType( VariablePre::V_PRE_CREDITOS ); //x_{i,d,u,s}
							preV.setTurma( turma );
							preV.setDisciplina( d );
							preV.setUnidade( unid );
							preV.setSubCjtSala( *itCjtSala );

							if ( SolVarsPreFound( preV ) )
								return true;
						}
					}
				 }						 
			 }
			 return false;
		 }
		 case VariableTatico::V_SLACK_DIST_CRED_DIA_SUPERIOR: // fcp_{i,d,s,t}
		 {
			 preV.reset();
			 preV.setType( VariablePre::V_PRE_OFERECIMENTO );	// o_{i,d,s}
			 preV.setTurma( v->getTurma() );
			 preV.setDisciplina( v->getDisciplina() );
			 preV.setUnidade( v->getUnidade() );
			 preV.setSubCjtSala( v->getSubCjtSala() );

			 if ( SolVarsPreFound(preV) )
				return true;
			 else
				return false;
		 }
		 case VariableTatico::V_SLACK_DIST_CRED_DIA_INFERIOR: // fcm_{i,d,s,t}
		 {
			 preV.reset();
			 preV.setType( VariablePre::V_PRE_OFERECIMENTO );	// o_{i,d,s}
			 preV.setTurma( v->getTurma() );
			 preV.setDisciplina( v->getDisciplina() );
			 preV.setUnidade( v->getUnidade() );
			 preV.setSubCjtSala( v->getSubCjtSala() );

			 if ( SolVarsPreFound(preV) )
				return true;
			 else
				return false;
		 }
		 case VariableTatico::V_SLACK_DEMANDA: // fd_{d,a}
		 {	
			 return true;			
		 }
		 case VariableTatico::V_COMBINACAO_DIVISAO_CREDITO: // m{i,d,k}
		 {
			 preV.reset();
			 preV.setType( VariablePre::V_PRE_ABERTURA ); // z_{i,d,cp}
			 preV.setTurma( v->getTurma() );
			 preV.setDisciplina( v->getDisciplina() );

			 ITERA_GGROUP_LESSPTR( itCp, problemData->campi, Campus )
			 {
				 preV.setCampus( *itCp );
				 if ( SolVarsPreFound( preV ) )
					 return true;
			 }
			 return false;
		 }
		 case VariableTatico::V_SLACK_COMBINACAO_DIVISAO_CREDITO_M: // fkm{i,d,k}
		 {
			 preV.reset();
			 preV.setType( VariablePre::V_PRE_ABERTURA ); // z_{i,d,cp}
			 preV.setTurma( v->getTurma() );
			 preV.setDisciplina( v->getDisciplina() );

			 ITERA_GGROUP_LESSPTR( itCp, problemData->campi, Campus )
			 {
				 preV.setCampus( *itCp );
				 if ( SolVarsPreFound( preV ) )
					 return true;
			 }
			 return false;
		 }
		 case VariableTatico::V_SLACK_COMBINACAO_DIVISAO_CREDITO_P: // fkp{i,d,k}
		 {
			 preV.reset();
			 preV.setType( VariablePre::V_PRE_ABERTURA ); // z_{i,d,cp}
			 preV.setTurma( v->getTurma() );
			 preV.setDisciplina( v->getDisciplina() );

			 ITERA_GGROUP_LESSPTR( itCp, problemData->campi, Campus )
			 {
				 preV.setCampus( *itCp );
				 if ( SolVarsPreFound( preV ) )
					 return true;
			 }
			 return false;
		 }
		 case VariableTatico::V_ABERTURA_COMPATIVEL: // zc_{d,t,cp}
		 {
			 preV.reset();
			 preV.setType( VariablePre::V_PRE_ABERTURA ); // z_{i,d,cp}
			 preV.setDisciplina( v->getDisciplina() );
			 preV.setCampus( v->getCampus() );

			 for ( int turma = 0; turma < v->getDisciplina()->getNumTurmas(); turma++ )
			 {
				preV.setTurma( turma );
				if	( SolVarsPreFound( preV ) )
				return true;
			 }
			 
			 return false;
		 }		 		 
		 case VariableTatico::V_ALUNO_VARIAS_UNID_DIA: // w_{a,t}
		 {
			 Aluno *aluno = v->getAluno();

			 preV.reset();
			 preV.setType( VariablePre::V_PRE_ALOCA_ALUNO_TURMA_DISC ); //s_{i,d,a,cp}
			 preV.setAluno( aluno );

			 ITERA_GGROUP_LESSPTR( itCampi, problemData->campi, Campus )
			 {
				 preV.setCampus( *itCampi );
				 ITERA_GGROUP_LESSPTR( itAlDemanda, aluno->demandas, AlunoDemanda )
				 {
					 Disciplina* d = itAlDemanda->demanda->disciplina;
					 preV.setDisciplina( d );
				 
					 for ( int turma = 0; turma < d->getNumTurmas(); turma++ )
					 {
						preV.setTurma( turma );
						if ( SolVarsPreFound( preV ) )
							return true;
					 }						 
				 }
			 }
			 return false;
		 }
		
		 default:
		 {
			return true;
			break;
		 }
	}

#endif

}


bool SolverMIP::criaVariavelTatico( Variable *v )
{
#ifndef PRE_TATICO
	return true;
#endif

#ifdef PRE_TATICO
	
	VariablePre preV;
	
	switch( v->getType() )
	{
	 case Variable::V_ABERTURA: // z_{i,d,cp}
	 {
		 preV.reset();
		 preV.setType( VariablePre::V_PRE_ABERTURA ); // z_{i,d,cp}
		 preV.setTurma( v->getTurma() );
		 preV.setDisciplina( v->getDisciplina() );
		 preV.setCampus( v->getCampus() );
		 		 
		 if ( SolVarsPreFound(preV) )
			return true;
		 else
			return false;
	 }
     case Variable::V_ALUNOS:	// a_{i,d,oft}
	 {
		 preV.reset();
		 preV.setType( VariablePre::V_PRE_ALUNOS );  // a_{i,d,u,s,oft}
		 preV.setTurma( v->getTurma() );
		 preV.setDisciplina( v->getDisciplina() );
		 preV.setOferta( v->getOferta() );
		 
		ITERA_GGROUP_LESSPTR( itUnid, v->getOferta()->campus->unidades, Unidade )
		{
			ITERA_GGROUP_LESSPTR( itCjtSala, itUnid->conjutoSalas, ConjuntoSala )
			{
				 preV.setSubCjtSala( *itCjtSala );
				 preV.setUnidade( *itUnid );
				 if ( SolVarsPreFound( preV ) )
					 return true;
			}
		}
		return false;
	 }
     case Variable::V_CREDITOS:  // x_{i,d,u,s,t}
	 {
		 preV.reset();
		 preV.setType( VariablePre::V_PRE_CREDITOS ); // x_{i,d,u,s}
		 preV.setTurma( v->getTurma() );
		 preV.setDisciplina( v->getDisciplina() );
		 preV.setUnidade( v->getUnidade() );
		 preV.setSubCjtSala( v->getSubCjtSala() );

		 if ( SolVarsPreFound(preV) )
			return true;
		 else
			return false;
	 }
     case Variable::V_DIAS_CONSECUTIVOS: // c_{i,d,t}
	 {
		 preV.reset();
		 preV.setType( VariablePre::V_PRE_ABERTURA ); // z_{i,d,cp}
		 preV.setTurma( v->getTurma() );
		 preV.setDisciplina( v->getDisciplina() );

		 ITERA_GGROUP_LESSPTR( itCp, problemData->campi, Campus )
		 {
			 preV.setCampus( *itCp );
			 if ( SolVarsPreFound( preV ) )
				 return true;
		 }
		 return false;
	 }
     case Variable::V_ERROR:
	 {
		 return true;
	 }
     case Variable::V_MAX_CRED_SEMANA: // H_{bc,i}
	 {
		 Oferta *oft = v->getBloco()->oferta;

		 preV.reset();
		 preV.setType( VariablePre::V_PRE_ALOC_ALUNO_OFT ); // c_{i,d,s,oft}
		 preV.setTurma( v->getTurma() );
		 preV.setOferta( oft );
		 	
		 ITERA_GGROUP_LESSPTR( itUnid, oft->campus->unidades, Unidade )
		 {
			preV.setUnidade( *itUnid );
			ITERA_GGROUP_LESSPTR( itCjtSala, (*itUnid)->conjutoSalas, ConjuntoSala )
			{
				 preV.setSubCjtSala( *itCjtSala );
				 ITERA_GGROUP_LESSPTR( itDisc, (*itCjtSala)->disciplinas_associadas, Disciplina )
				 {				 
					preV.setDisciplina( *itDisc );
					if ( SolVarsPreFound( preV ) )
						return true;
				 }
			}			 
		 }
		 return false;
	 }
     case Variable::V_MIN_CRED_SEMANA: // h_{bc,i}
	 {
		 Oferta *oft = v->getBloco()->oferta;

		 preV.reset();
		 preV.setType( VariablePre::V_PRE_ALOC_ALUNO_OFT ); // c_{i,d,s,oft}
		 preV.setTurma( v->getTurma() );
		 preV.setOferta( oft );
		 	
		 ITERA_GGROUP_LESSPTR( itUnid, oft->campus->unidades, Unidade )
		 {
			preV.setUnidade( *itUnid );
			ITERA_GGROUP_LESSPTR( itCjtSala, (*itUnid)->conjutoSalas, ConjuntoSala )
			{
				 preV.setSubCjtSala( *itCjtSala );
				 ITERA_GGROUP_LESSPTR( itDisc, (*itCjtSala)->disciplinas_associadas, Disciplina )
				 {				 
					preV.setDisciplina( *itDisc );
					if ( SolVarsPreFound( preV ) )
						return true;
				 }
			}			 
		 }
		 return false;
	 }
     case Variable::V_OFERECIMENTO: // o_{i,d,u,s,t}
	 {
		 preV.reset();
		 preV.setType( VariablePre::V_PRE_OFERECIMENTO ); // o_{i,d,s}
		 preV.setTurma( v->getTurma() );
		 preV.setDisciplina( v->getDisciplina() );
		 preV.setUnidade( v->getUnidade() );
		 preV.setSubCjtSala( v->getSubCjtSala() );

		 if ( SolVarsPreFound(preV) )
			return true;
		 else
			return false;
	 }
     case Variable::V_N_SUBBLOCOS:
	 {
		 return true;
	 }
     case Variable::V_ALOC_ALUNO:// b_{i,d,c,cp}
	 {
		 preV.reset();
		 preV.setType( VariablePre::V_PRE_ALOC_ALUNO );		 // b_{i,d,c}
		 preV.setTurma( v->getTurma() );
		 preV.setDisciplina( v->getDisciplina() );
		 preV.setCurso( v->getCurso() );

		 if ( SolVarsPreFound(preV) )
			return true;
		 else
			return false;
	 }
     case Variable::V_ALOC_DISCIPLINA:// y_{i,d,tps,u}
	 {
		 preV.reset();
		 preV.setType( VariablePre::V_PRE_OFERECIMENTO );	// o_{i,d,s}
		 preV.setTurma( v->getTurma() );
		 preV.setDisciplina( v->getDisciplina() );
		 preV.setUnidade( v->getUnidade() );
		 preV.setSubCjtSala( v->getSubCjtSala() );

		 if ( SolVarsPreFound(preV) )
			return true;
		 else
			return false;
	 }
     case Variable::V_N_ABERT_TURMA_BLOCO: // v_{bc,t}
	 {
		 return true;
	 }
     case Variable::V_SLACK_DIST_CRED_DIA_SUPERIOR: // fcp_{i,d,tps,t}
	 {
		 preV.reset();
		 preV.setType( VariablePre::V_PRE_OFERECIMENTO );	// o_{i,d,s}
		 preV.setTurma( v->getTurma() );
		 preV.setDisciplina( v->getDisciplina() );
		 preV.setUnidade( v->getUnidade() );
		 preV.setSubCjtSala( v->getSubCjtSala() );

		 if ( SolVarsPreFound(preV) )
			return true;
		 else
			return false;
	 }
     case Variable::V_SLACK_DIST_CRED_DIA_INFERIOR: // fcm_{i,d,tps,t}
	 {
		 preV.reset();
		 preV.setType( VariablePre::V_PRE_OFERECIMENTO );	// o_{i,d,s}
		 preV.setTurma( v->getTurma() );
		 preV.setDisciplina( v->getDisciplina() );
		 preV.setUnidade( v->getUnidade() );
		 preV.setSubCjtSala( v->getSubCjtSala() );

		 if ( SolVarsPreFound(preV) )
			return true;
		 else
			return false;
	 }
     case Variable::V_ABERTURA_SUBBLOCO_DE_BLC_DIA_CAMPUS: // r_{bc,t,cp}
	 {
		 return true;
	 }
     case Variable::V_SLACK_ALOC_ALUNOS_CURSO_INCOMPAT: // bs_{i,d,c,c',cp}
	 {
		 preV.reset();
		 preV.setType( VariablePre::V_PRE_SLACK_ALOC_ALUNOS_CURSO_INCOMPAT );	 // bs_{i,d,c1,c2}
		 preV.setTurma( v->getTurma() );
		 preV.setDisciplina( v->getDisciplina() );
		 preV.setParCursos( v->getParCursos() );

		 if ( SolVarsPreFound(preV) )
			return true;
		 else
			return false;
	 }
     case Variable::V_SLACK_DEMANDA: // fd_{d,oft}
	 {	return true;
		 /*
		 preV.reset();
		 preV.setType( VariablePre::V_PRE_SLACK_DEMANDA );	 // fd_{d,oft}
		 preV.setDisciplina( v->getDisciplina() );
		 preV.setOferta( v->getOferta() );

		 if ( SolVarsPreFound(preV) )
			return true;
		 else
			return false;
		*/
	 }
	 case Variable::V_COMBINACAO_DIVISAO_CREDITO: // m{i,d,k}
	 {
		 preV.reset();
		 preV.setType( VariablePre::V_PRE_ABERTURA ); // z_{i,d,cp}
		 preV.setTurma( v->getTurma() );
		 preV.setDisciplina( v->getDisciplina() );

		 ITERA_GGROUP_LESSPTR( itCp, problemData->campi, Campus )
		 {
			 preV.setCampus( *itCp );
			 if ( SolVarsPreFound( preV ) )
				 return true;
		 }
		 return false;
	 }
     case Variable::V_SLACK_COMBINACAO_DIVISAO_CREDITO_M: // fkm{i,d,k}
	 {
		 preV.reset();
		 preV.setType( VariablePre::V_PRE_ABERTURA ); // z_{i,d,cp}
		 preV.setTurma( v->getTurma() );
		 preV.setDisciplina( v->getDisciplina() );

		 ITERA_GGROUP_LESSPTR( itCp, problemData->campi, Campus )
		 {
			 preV.setCampus( *itCp );
			 if ( SolVarsPreFound( preV ) )
				 return true;
		 }
		 return false;
	 }
     case Variable::V_SLACK_COMBINACAO_DIVISAO_CREDITO_P: // fkp{i,d,k}
	 {
		 preV.reset();
		 preV.setType( VariablePre::V_PRE_ABERTURA ); // z_{i,d,cp}
		 preV.setTurma( v->getTurma() );
		 preV.setDisciplina( v->getDisciplina() );

		 ITERA_GGROUP_LESSPTR( itCp, problemData->campi, Campus )
		 {
			 preV.setCampus( *itCp );
			 if ( SolVarsPreFound( preV ) )
				 return true;
		 }
		 return false;
	 }
     case Variable::V_CREDITOS_MODF: // xm_{d,t}
	 {
		 return true;
	 }
     case Variable::V_ABERTURA_COMPATIVEL: // zc_{d,t,cp}
	 {
		 return true;
	 }
	 case Variable::V_ABERTURA_BLOCO_MESMO_TPS: // n_{bc,tps}
	 {
		 return true; // TODO
	 }
	 case Variable::V_SLACK_ABERTURA_BLOCO_MESMO_TPS: //fn_{bc,tps}
	 {
		 return true; // TODO
	 }
	 case Variable::V_SLACK_COMPARTILHAMENTO: // fc_{i,d,c1,c2,cp}
	 {
		 preV.reset();
		 preV.setType( VariablePre::V_PRE_SLACK_COMPARTILHAMENTO );	 // fc_{i,d,c1,c2}
		 preV.setTurma( v->getTurma() );
		 preV.setDisciplina( v->getDisciplina() );
		 preV.setParCursos( v->getParCursos() );

		 if ( SolVarsPreFound(preV) )
			return true;
		 else
			return false;
	 }
 	 case Variable::V_ALOC_ALUNOS_OFT: // e_{i,d,oft} 
	 {
		 preV.reset();
		 preV.setType( VariablePre::V_PRE_ALUNOS );	 // a_{i,d,s,oft}
		 preV.setTurma( v->getTurma() );
		 preV.setDisciplina( v->getDisciplina() );
		 preV.setOferta( v->getOferta() );

		 ITERA_GGROUP_LESSPTR( itCp, problemData->campi, Campus )
		 {
			ITERA_GGROUP_LESSPTR( itUnid, itCp->unidades, Unidade )
			{
				preV.setUnidade( *itUnid );
				ITERA_GGROUP_LESSPTR( itCjtSala, itUnid->conjutoSalas, ConjuntoSala )
				{
					preV.setSubCjtSala( *itCjtSala );
					if ( SolVarsPreFound( preV ) )
						return true;
				}
			}
		 }
		 return false;
	 }
	 case Variable::V_CREDITOS_OFT: // q_{i,d,oft,u,s,t}
	 {		 
		 preV.reset();
		 preV.setType( VariablePre::V_PRE_ALUNOS );	// a_{i,d,s,oft}
		 preV.setTurma( v->getTurma() );
		 preV.setUnidade( v->getUnidade() );
		 preV.setSubCjtSala( v->getSubCjtSala() );
		 preV.setDisciplina( v->getDisciplina() );
		 preV.setOferta( v->getOferta() );

		 if ( SolVarsPreFound(preV) )
		 {
			return true;
		 }
		 else
			return false;
	 }
	 case Variable::V_CREDITOS_PAR_OFT: // p_{i,d,oft1,oft2,u,s,t}
	 {
		 preV.reset();
		 preV.setType( VariablePre::V_PRE_ALUNOS );	 // a_{i,d,s,oft}
		 preV.setTurma( v->getTurma() );
		 preV.setUnidade( v->getUnidade() );
		 preV.setSubCjtSala( v->getSubCjtSala() );
		 preV.setDisciplina( v->getDisciplina() );
		 preV.setOferta( v->getParOfertas().first );

		 if ( SolVarsPreFound(preV) )
		 {
			 preV.setOferta( v->getParOfertas().second );

			 if ( SolVarsPreFound(preV) )
			 {
				 preV.reset();
				 preV.setType( VariablePre::V_PRE_CREDITOS );  // x_{i,d,s}
				 preV.setTurma( v->getTurma() );
				 preV.setDisciplina( v->getDisciplina() );
				 preV.setUnidade( v->getUnidade() );
				 preV.setSubCjtSala( v->getSubCjtSala() );

				 if ( SolVarsPreFound(preV) )
				 {
					 return true;
				 }
				 else
					 return false;
			 }
			 else
				return false;
		 }
		 else
			return false;
	 }
	 case Variable::V_ALOC_ALUNOS_PAR_OFT: // of_{i,d,oft1,oft2}
	 {
		 preV.reset();
		 preV.setType( VariablePre::V_PRE_ALUNOS );	// a_{i,d,s,oft1}
		 preV.setTurma( v->getTurma() );
		 preV.setDisciplina( v->getDisciplina() );
		 preV.setOferta( v->getParOfertas().first );

		 ITERA_GGROUP_LESSPTR( itCp, problemData->campi, Campus )
		 {
			ITERA_GGROUP_LESSPTR( itUnid, itCp->unidades, Unidade )
			{
				preV.setUnidade( *itUnid );
				ITERA_GGROUP_LESSPTR( itCjtSala, itUnid->conjutoSalas, ConjuntoSala )
				{
					preV.setSubCjtSala( *itCjtSala );
					if ( SolVarsPreFound( preV ) )
					{
						preV.setOferta( v->getParOfertas().second );	// a_{i,d,s,oft2}

						if ( SolVarsPreFound( preV ) )
						{
							return true;
						}
						else
						{
							return false; //TODO: conferir se est� certo retornar falso aqui.
						}
					}
				}
			}
		 }
		 return false;
	 }
	 case Variable::V_MIN_HOR_DISC_OFT_DIA: // g_{d,oft,t}
	 {
		 preV.reset();
		 preV.setType( VariablePre::V_PRE_ALUNOS );	// a_{i,d,s,oft}
		 preV.setDisciplina( v->getDisciplina() );
		 preV.setOferta( v->getOferta() );
		 
		 for ( int turma = 0; turma < v->getDisciplina()->getNumTurmas(); turma++ )
		 { 
			 preV.setTurma( turma );
			 ITERA_GGROUP_LESSPTR( itCp, problemData->campi, Campus )
			 {
				ITERA_GGROUP_LESSPTR( itUnid, itCp->unidades, Unidade )
				{
					preV.setUnidade( *itUnid );
					ITERA_GGROUP_LESSPTR( itCjtSala, itUnid->conjutoSalas, ConjuntoSala )
					{
						preV.setSubCjtSala( *itCjtSala );
						if ( SolVarsPreFound( preV ) )
							return true;
					}
				}
			 }
		 }
		 return false;

	 }
	 case Variable::V_COMBINA_SL_SALA:  // cs_{s,t,k}
	 {
		 return true;
	 }	
	 case Variable::V_COMBINA_SL_BLOCO: // cbc_{cb,t,k}
	 {
		 return true;
	 }
		
     default:
	 {
		return true;
		break;
	 }
	}

#endif

}

void SolverMIP::carregaVariaveisSolucaoPreTatico( int campusId, int prioridade )
{
   double * xSol = NULL;
   VariablePreHash::iterator vit;

 //  SolutionLoader sLoader( problemData, problemSolution );

   xSol = new double[ lp->getNumCols() ];

#ifndef READ_SOLUTION_TATICO_BIN
   lp->getX( xSol );
#endif

#ifdef READ_SOLUTION_TATICO_BIN
   FILE* fin = fopen("solBinPre.bin","rb");

   int nCols = 0;

   fread(&nCols,sizeof(int),1,fin);

   if ( nCols == lp->getNumCols() )
   {
      for (int i =0; i < nCols; i++)
      {
         double auxDbl;
         fread(&auxDbl,sizeof(double),1,fin);
         xSol[i] = auxDbl;
      }
   }

   fclose(fin);
#endif

   vit = vHashPre.begin();

   char solFilename[1024], id[100];
   strcpy( solFilename, "solucaoPreTatico" );
   _itoa_s( campusId, id, 100, 10 );
   strcat( solFilename, id );
   _itoa_s( prioridade, id, 100, 10 );
   strcat( solFilename, "_P" );
   strcat( solFilename, id );
   strcat( solFilename, ".txt" );

   FILE * fout = fopen( solFilename, "wt" );

   while ( vit != vHashPre.end() )
   {
      VariablePre * v = new VariablePre( vit->first );
      int col = vit->second;
      v->setValue( xSol[ col ] );

      if ( v->getValue() > 0.00001 )
      {
		 solVarsPre.insert( *v );
         
		 char auxName[100];
         lp->getColName( auxName, col, 100 );
         fprintf( fout, "%s = %f\n", auxName, v->getValue() );

         switch( v->getType() )
         {
			 case VariablePre::V_ERROR:
				std::cout << "Vari�vel inv�lida " << std::endl;
				break;
			 case VariablePre::V_PRE_CREDITOS:
				std::cout << "Oferta de " << v->getValue()
						  << " creditos da disciplina " << v->getDisciplina()->getCodigo()
						  << " para a turma " << v->getTurma()
						  << " para alguma de sala do conjunto de salas " << v->getSubCjtSala()->getId()
						  << std::endl << std::endl;
				break;
			 case VariablePre::V_PRE_OFERECIMENTO: break;
			 case VariablePre::V_PRE_ABERTURA: break;
			 case VariablePre::V_PRE_ALUNOS:
				std::cout << "Oferecimento de " << v->getValue()
						  << " vagas da disciplina " << v->getDisciplina()->getCodigo()
						  << " para a turma " << v->getTurma()
						  << " do curso " << v->getOferta()->curso->getCodigo()
						  << std::endl << std::endl;
				break;
			 case VariablePre::V_PRE_ALOC_ALUNO: break;
			 case VariablePre::V_PRE_SLACK_DEMANDA: break;			 
			 case VariablePre::V_PRE_SLACK_COMPARTILHAMENTO: break;
			 case VariablePre::V_PRE_SLACK_ALOC_ALUNOS_CURSO_INCOMPAT: break;			 
         }
      }
	  else
	  {
		  delete v;
	  }

      vit++;
   }

   //#ifdef DEBUG
   if ( fout )
   {
      fclose( fout );
   }
   //#endif

   if ( xSol )
   {
      delete [] xSol;
   }

}

void SolverMIP::preencheMapAtendimentoAluno()
{
	// Limpa a solu��o obtida na itera��o de prioridade de demanda anterior, caso exista
	problemData->mapAluno_CampusTurmaDisc.clear();
	problemData->mapCampusTurmaDisc_AlunosDemanda.clear();

	// Resultado da aloca��o de alunos no pre-modelo:
	std::set< VariablePre >::iterator itSol = solVarsPre.begin();

	for ( ; itSol != solVarsPre.end(); itSol++ )
	{
		if ( itSol->getType() == VariablePre::V_PRE_ALOCA_ALUNO_TURMA_DISC )
		{
			Aluno *aluno = itSol->getAluno();
			int turma = itSol->getTurma();
			Disciplina *disciplina = itSol->getDisciplina();
			Campus *cp = itSol->getCampus();

			Trio< int /*campusId*/, int /*turma*/, Disciplina* > trio;
			trio.set( cp->getId(), turma, disciplina );
					
			problemData->mapAluno_CampusTurmaDisc[ aluno ].add( trio );

			AlunoDemanda *alunoDemanda = problemData->procuraAlunoDemanda( disciplina->getId(), aluno->getAlunoId() );
			problemData->mapCampusTurmaDisc_AlunosDemanda[ trio ].add( alunoDemanda );
		}
	}
	
}


int SolverMIP::solvePreTatico( int campusId, int prioridade )
{   
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
   
   if ( vHashPre.size() > 0 )
   {
	   vHashPre.clear();
   }
   if ( cHashPre.size() > 0 )
   {
	   cHashPre.clear();
   }

   int varNum = 0;
   int constNum = 0;

   char lpName[1024], id[100];
   strcpy( lpName, "SolverTriedaPreTatico" );
   _itoa_s( campusId, id, 100, 10 );
   strcat( lpName, id );
   _itoa_s( prioridade, id, 100, 10 );
   strcat( lpName, "_P" );
   strcat( lpName, id );

   if ( problemData->parametros->funcao_objetivo == 0
      || problemData->parametros->funcao_objetivo == 2 )
   {
      lp->createLP( lpName, OPTSENSE_MAXIMIZE, PROB_MIP );
   }
   else if( problemData->parametros->funcao_objetivo == 1 )
   {
      lp->createLP( lpName, OPTSENSE_MINIMIZE, PROB_MIP );
   }

#ifdef DEBUG
   printf( "Creating LP...\n" );
#endif

   // Variable creation
   varNum = cria_preVariaveis( campusId, prioridade );

   lp->updateLP();

#ifdef PRINT_cria_variaveis
   printf( "Total of Variables: %i\n\n", varNum );
#endif

   // Constraint creation
   constNum = cria_preRestricoes( campusId, prioridade );

   lp->updateLP();

#ifdef PRINT_cria_restricoes
   printf( "Total of Constraints: %i\n\n", constNum );
#endif

   lp->writeProbLP( lpName );
#ifdef DEBUG
  // lp->writeProbLP( lpName );
#endif

   int status = 0;
   lp->setTimeLimit( 9000 );
   lp->setMIPRelTol( 0.01 );
   lp->setPreSolve(OPT_TRUE);
   lp->setHeurFrequency(1.0);
   lp->setMIPEmphasis(0);
   lp->setSymetry(0);
   lp->setPolishAfterTime(7200);
   //lp->setNoCuts();
   lp->setMIPScreenLog( 4 );

   lp->setPreSolve(OPT_TRUE);

#ifndef READ_SOLUTION_TATICO_BIN
   status = lp->optimize( METHOD_MIP );
#endif

#ifdef WRITE_SOLUTION_TATICO_BIN
   double * xSol = NULL;
   xSol = new double[ lp->getNumCols() ];
   lp->getX( xSol );
   FILE * fout = fopen( "solBinPre.bin", "wb" );
   int nCols = lp->getNumCols();

   fwrite( &nCols, sizeof( int ), 1, fout );
   for ( int i = 0; i < lp->getNumCols(); i++ )
   {
      fwrite( &( xSol[ i ] ), sizeof( double ), 1, fout );
   }

   fclose( fout );

   delete [] xSol;
#endif

   return status;
}


int SolverMIP::solveTaticoBasico( int campusId, int prioridade  )
{
   int varNum = 0;
   int constNum = 0;
   
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

   if ( vHash.size() > 0 )
   {
	   vHash.clear();
   }
   if ( cHash.size() > 0 )
   {
	   cHash.clear();
   }
   if ( vHashTatico.size() > 0 )
   {
	   vHashTatico.clear();
   }
   if ( cHashTatico.size() > 0 )
   {
	   cHashTatico.clear();
   }

   char lpName[1024], id[100];
   strcpy( lpName, "SolverTrieda" );
   _itoa_s( campusId, id, 100, 10 ); // converte o valor em campusId para string, armazenando-o em id
   strcat( lpName, id );
   _itoa_s( prioridade, id, 100, 10 );
   strcat( lpName, "_P" );
   strcat( lpName, id );

   if ( problemData->parametros->funcao_objetivo == 0
      || problemData->parametros->funcao_objetivo == 2 )
   {
      lp->createLP( lpName, OPTSENSE_MAXIMIZE, PROB_MIP );
   }
   else if( problemData->parametros->funcao_objetivo == 1 )
   {
      lp->createLP( lpName, OPTSENSE_MINIMIZE, PROB_MIP );
   }

#ifdef DEBUG
   printf( "Creating LP...\n" );
#endif

// ---------------------------------------------------------------
// Tatico por bloco curricular:

   if ( problemData->parametros->otimizarPor == "BLOCOCURRICULAR" )
   {
	   // Variable creation
	   varNum = cria_variaveis( campusId );

	   lp->updateLP();

		#ifdef PRINT_cria_variaveis
	   printf( "Total of Variables: %i\n\n", varNum );
		#endif

	   // Constraint creation
	   constNum = cria_restricoes( campusId );

	   lp->updateLP();

		#ifdef PRINT_cria_restricoes
	   printf( "Total of Constraints: %i\n\n", constNum );
		#endif

   }

// ---------------------------------------------------------------
// Tatico por aluno:

   else if ( problemData->parametros->otimizarPor == "ALUNO" )
   {
	   // Variable creation
	   varNum = criaVariaveisTatico( campusId );

	   lp->updateLP();

		#ifdef PRINT_cria_variaveis
	   printf( "Total of Variables: %i\n\n", varNum );
		#endif

	   // Constraint creation
	   constNum = criaRestricoesTatico( campusId );

	   lp->updateLP();

		#ifdef PRINT_cria_restricoes
	   printf( "Total of Constraints: %i\n\n", constNum );
		#endif
   }
// ---------------------------------------------------------------

   else
   {
		std::cerr<<"\nErro: Parametro otimizarPor deve ser ALUNO ou BLOCOCURRICULAR!\n";
		exit(0);
   }  

   lp->writeProbLP( lpName ); 
#ifdef DEBUG
  // lp->writeProbLP( lpName );
#endif

   int status = 0;
   lp->setTimeLimit( 14000 );
   //lp->setTimeLimit( 3600 );
   //lp->setMIPRelTol( 0.01 );
   lp->setPreSolve(OPT_TRUE);
   lp->setHeurFrequency(1.0);
   lp->setMIPScreenLog( 4 );
   lp->setMIPEmphasis(0);
   lp->setPolishAfterNode(1);
   lp->setSymetry(0);
   //lp->setNoCuts();
   lp->setCuts(3);
   lp->writeProbLP( lpName );
   
   lp->setPreSolve(OPT_TRUE);

   lp->updateLP();

#ifndef READ_SOLUTION_TATICO_BIN
   status = lp->optimize( METHOD_MIP );
#endif

#ifdef WRITE_SOLUTION_TATICO_BIN
   double * xSol = NULL;
   xSol = new double[ lp->getNumCols() ];
   lp->getX( xSol );
   FILE * fout = fopen( "solBin.bin", "wb" );
   int nCols = lp->getNumCols();

   fwrite( &nCols, sizeof( int ), 1, fout );
   for ( int i = 0; i < lp->getNumCols(); i++ )
   {
      fwrite( &( xSol[ i ] ), sizeof( double ), 1, fout );
   }

   fclose( fout );

   delete [] xSol;
#endif

   return status;
}

void SolverMIP::mudaCjtSalaParaSala()
{
	if ( problemData->parametros->otimizarPor == "BLOCOCURRICULAR" )
	{		
		   ITERA_VECTOR( it_Vars_x, vars_x, Variable )
		   {
			  if ( ( *it_Vars_x )->getSubCjtSala()->salas.size() > 0 )
			  {
				 Sala *auxSala = (( *it_Vars_x )->getSubCjtSala()->salas.begin())->second;
				 ( *it_Vars_x )->setSala(auxSala);
			  }
		   }
		   // Imprimindo as vari�veis x_{i,d,u,s,t} convertidas.

		   std::cout << "\n\n\n";
		   std::cout << "x\t\ti\td\tu\ts\tt\n";

		   ITERA_VECTOR( it_Vars_x, vars_x, Variable )
		   {
			  if ( ( *it_Vars_x )->getSala() == NULL )
			  {
				 printf( "\nOPA. Variavel x (x_i(%d)_d(%d)_u(%d)_tps(%d)_t(%d))nao convertida.\n\n",
						 ( *it_Vars_x )->getTurma(),
						 ( *it_Vars_x )->getDisciplina()->getId(),
						 ( *it_Vars_x )->getUnidade()->getId(),
						 ( *it_Vars_x )->getSubCjtSala()->getId(),
						 ( *it_Vars_x )->getDia() );

				 exit( 1 );
			  }

			  std::cout << (*it_Vars_x)->getValue() << "\t\t"
						<< ( *it_Vars_x )->getTurma() << "\t"
						<< ( *it_Vars_x )->getDisciplina()->getCodigo() << "\t"
						<< ( *it_Vars_x )->getUnidade()->getCodigo() << "\t"
						<< ( *it_Vars_x )->getSala()->getCodigo() << "\t"
						<< ( *it_Vars_x )->getDia() << "\n";
		   }

	}
	else if ( problemData->parametros->otimizarPor == "ALUNO" )
	{
		   ITERA_GGROUP( it_Vars_x, vars_xh, VariableTatico )
		   {
			  if ( ( *it_Vars_x )->getSubCjtSala()->salas.size() > 0 )
			  {
				 Sala *auxSala = (( *it_Vars_x )->getSubCjtSala()->salas.begin())->second;
				 ( *it_Vars_x )->setSala(auxSala);
			  }
		   }
		   // Imprimindo as vari�veis x_{i,d,u,s,hi,hf,t} convertidas.

		   std::cout << "\n\n\n";
		   std::cout << "x\t\ti\td\tu\ts\thi\thf\tt\n";

		   ITERA_GGROUP( it_Vars_x, vars_xh, VariableTatico )
		   {
			  if ( ( *it_Vars_x )->getSala() == NULL )
			  {
				 printf( "\nOPA. Variavel x (x_i(%d)_d(%d)_u(%d)_tps(%d)_t(%d))nao convertida.\n\n",
						 ( *it_Vars_x )->getTurma(),
						 ( *it_Vars_x )->getDisciplina()->getId(),
						 ( *it_Vars_x )->getUnidade()->getId(),
						 ( *it_Vars_x )->getSubCjtSala()->getId(),
						 ( *it_Vars_x )->getHorarioAulaInicial()->getId(),
						 ( *it_Vars_x )->getHorarioAulaFinal()->getId(),
						 ( *it_Vars_x )->getDia() );

				 exit( 1 );
			  }

			  std::cout << (*it_Vars_x)->getValue() << "\n\t\t"
						<< ( *it_Vars_x )->getTurma() << "\t"
						<< ( *it_Vars_x )->getDisciplina()->getCodigo() << "\t"
						<< ( *it_Vars_x )->getUnidade()->getCodigo() << "\t"
						<< ( *it_Vars_x )->getSala()->getCodigo() << "\t"
						<< ( *it_Vars_x )->getHorarioAulaInicial()->getId() << "\t"
						<< ( *it_Vars_x )->getHorarioAulaFinal()->getId() << "\t"
						<< ( *it_Vars_x )->getDia() << "\n\n";
		   }	
	}
}


void SolverMIP::converteCjtSalaEmSala()
{
   // ---------------------------------------

   // POS PROCESSAMENTO
   // Convertendo as vari�veis x_{i,d,u,tps,t} para x_{i,d,u,s,t}.

   // PASSO 1: Criando uma estrutura que ir� gerenciar o tempo livre para cada sala.
   std::map< Sala *, std::vector< std::pair< int /*dia*/, int/*minutos Livres*/ > > > tempo_Livre_Sala;

   // Inicializando a estrutura criada acima
   ITERA_GGROUP_LESSPTR( it_Campus, problemData->campi, Campus )
   {
      ITERA_GGROUP_LESSPTR( it_Unidade, it_Campus->unidades, Unidade )
      {
         ITERA_GGROUP_LESSPTR( it_Sala, it_Unidade->salas, Sala )
         {
            ITERA_GGROUP_N_PT( it_Dia, it_Sala->diasLetivos, int )
			{
				int dia = *it_Dia;
				int minutosLivres = it_Sala->getTempoDispPorDia(dia);
                
				tempo_Livre_Sala[ *it_Sala ].push_back( std::make_pair( dia, minutosLivres ) );
            }
         }
      }
   }

   // PASSO 2: Ordenando os vetores de Salas para cada disciplina da estrutura <disc_Salas_Pref>.

   // As disciplinas que possuem o menor n�mero de
   // prefer�ncias, acima de 0, ser�o consideradas primeiro.
   // Em seguida, a ordem das demais disciplinas ser� mantida.

   // Vetor de disciplinas respons�vel por informar a ordem em que a 
   // heur�stica ir� alocar as disicplinas.
   std::vector< std::pair< int/*Disc id*/, int/*Qtd salas associadas*/ > > disc_Salas_Cont;

   // Adicionando as disciplinas que possuem alguma associa��o.
   std::map< Disciplina *, GGroup< Sala *, LessPtr< Sala > >, LessPtr< Disciplina > >::iterator 
      it_Disc_Salas_Pref = problemData->disc_Salas_Pref.begin();

   for(; it_Disc_Salas_Pref != problemData->disc_Salas_Pref.end();
         it_Disc_Salas_Pref++ )
   {
      disc_Salas_Cont.push_back(
         std::make_pair( it_Disc_Salas_Pref->first->getId(),
                         it_Disc_Salas_Pref->second.size() ) );
   }

   // Ordenando.
#ifndef WIN32
   ordenaDiscSalas comparador;
   std::sort( disc_Salas_Cont.begin(), disc_Salas_Cont.end(), comparador );
#else
   std::sort( disc_Salas_Cont.begin(), disc_Salas_Cont.end(), ordenaDiscSalas );
#endif

   // Adicionando as demais disciplinas (as que n�o possuem nenhuma associa��o).
   std::vector< std::pair< int /*Disc id*/, int /*Qtd salas associadas*/ > > disc_Salas_TEMP;
   ITERA_GGROUP_LESSPTR( it_Disciplina, problemData->disciplinas, Disciplina )
   {
      // Estrutura: Dado o 'id' de uma disciplina,
      // retorna-se quantas salas est�o associadas a essa disciplina
      std::pair< int, int > chave( it_Disciplina->getId(),
         problemData->disc_Salas_Pref[ *it_Disciplina ].size() );

      // Verificando para disciplina n�o adicionar a mesma disciplina v�rias vezes.
      // Estrutura dos itens do vector: Dado o 'id' de uma disciplina,
      // retorna-se quantas salas est�o associadas a essa disciplina
      std::vector< std::pair< int, int > >::iterator it_Disc_Salas
         = std::find( disc_Salas_Cont.begin(), disc_Salas_Cont.end(), chave );

      if ( it_Disc_Salas == disc_Salas_Cont.end() )
      {
         disc_Salas_TEMP.push_back( chave );
      }
   }

   // Concatenando os 2 vetores.
   disc_Salas_Cont.insert( disc_Salas_Cont.end(),
      disc_Salas_TEMP.begin(), disc_Salas_TEMP.end() );

   // Para cada disciplina da estrutura <disc_Salas> realiza-se alguma aloca��o
   std::vector< std::pair< int /*Disc id*/, int /*Qtd salas associadas*/ > >::iterator
      it_Disc_Salas = disc_Salas_Cont.begin();

   for (; it_Disc_Salas != disc_Salas_Cont.end(); ++it_Disc_Salas )
   {
      // Obtendo uma referencia para a disciplina em quest�o.
      std::map< int /*Id Disc*/, Disciplina * >::iterator 
         it_Ref_Disciplinas = problemData->refDisciplinas.find( it_Disc_Salas->first );

      if ( it_Ref_Disciplinas == problemData->refDisciplinas.end() )
      {
         std::cout << "\nOpa. Disciplina inexistente."
                   << "\n(SolverMIP::converteCjtSalaEmSala()) !!!"
                   << "\n\nSaindo." << std::endl;

         exit( 1 );
      }

      Disciplina * disciplina = it_Ref_Disciplinas->second;

      // Estrutura respons�vel por armazenar as
      // variaveis "x" para a disciplina em quest�o.
      std::vector< Variable * > vars_x_Disc;

      // Listando todas as variaveis "x" para a disciplina em quest�o.
      ITERA_VECTOR( it_Vars_x, vars_x, Variable )
      {
         if ( ( *it_Vars_x )->getDisciplina() == disciplina )
         {
            vars_x_Disc.push_back( *it_Vars_x );
         }
      }

      // Ordenando as variaveis coletadas segundo
      // a ordem dos crit�rios estabelecidos:
      // 1 - unidade
      // 2 - turma
      // 3 - dia
      std::sort( vars_x_Disc.begin(),
                 vars_x_Disc.end(), ordenaVarsX );

      // Estrutura que armazena separadamente, por unidade,
      // turma e tipo de sala, as variaveis x coletadas.
      // --> Unidade, TPS, Turma
      std::map< std::vector< int >,
         std::vector< Variable * > > vars_x_Disc_Und_TPS_Turma;

      ITERA_VECTOR( it_Vars_x_Disc, vars_x_Disc, Variable )
      {
         // Unidade, TPS, Turma
         std::vector< int > chave;

         chave.push_back( ( *it_Vars_x_Disc )->getUnidade()->getId() );
         chave.push_back( ( *it_Vars_x_Disc )->getSubCjtSala()->getId() );
         chave.push_back( ( *it_Vars_x_Disc )->getTurma() );

         vars_x_Disc_Und_TPS_Turma[ chave ].push_back( *it_Vars_x_Disc );
      }

      // Estrutura que armazena as salas na ordem em
      // que se deve tentar alocar a disciplina em quest�o.

      std::vector< Sala * > salas_Ordenadas;

      // Ordenando a estrutura <discSalas> do problemData
      // de acordo com os seguintes crit�rios:
      // 1 - Salas preferenciais sugeridas pelo usu�rio.
      // 2 - Outras salas da mesma unidade em que todas,
      // ou a maioria, das salas estavam associadas.
      // 3 - Qualquer outra sala.
      // OBS.: Por eqto tento fazer o passo 1 e depois o 3.
      // O 2 talvez nem precise pq agt ja esta convertendo
      // a lista de cursos predios para associacoes de disciplinas a salas.

      // Adicionando as salas que foram associadas pelo usuario.
      std::map< Disciplina *, GGroup< Sala *, LessPtr< Sala > >, LessPtr< Disciplina > >::iterator
         it_Disc_Salas_Pref = problemData->disc_Salas_Pref.find( disciplina );

      if ( it_Disc_Salas_Pref != problemData->disc_Salas_Pref.end() )
      {
         // Iterando sobre as salas preferenciais para a disciplina em quest�o.
         ITERA_GGROUP_LESSPTR( it_Sala, it_Disc_Salas_Pref->second, Sala )
         {
            salas_Ordenadas.push_back( *it_Sala );
         }
      }

      // Adicionando as demais salas associadas � disciplina em quest�o.
      std::map< Disciplina *, std::vector< Sala * >, LessPtr< Disciplina > >::iterator
         it_Disc_Demais_Salas = problemData->discSalas.find( disciplina );

      if ( it_Disc_Demais_Salas != problemData->discSalas.end() )
      {
         ITERA_VECTOR( it_Sala, it_Disc_Demais_Salas->second, Sala )
         {
            // Para n�o adicionar repetidas
            if ( std::find( salas_Ordenadas.begin(),
               salas_Ordenadas.end(), ( *it_Sala ) ) == salas_Ordenadas.end() )
            {
               salas_Ordenadas.push_back( *it_Sala );
            }
         }
      }

      // Estrutura do vector: Unidade, TPS, Turma
      std::map< std::vector< int >, std::vector< Variable * > >::iterator
         it_Vars_x_Disc_Und_TPS_Turma = vars_x_Disc_Und_TPS_Turma.begin();

      // Iterando em cada conjunto de variaveis
      // da estrutura <vars_x_Disc_Und_TPS_Turma>
      for(; it_Vars_x_Disc_Und_TPS_Turma != vars_x_Disc_Und_TPS_Turma.end();
            it_Vars_x_Disc_Und_TPS_Turma++ )
      {
         bool alocou = false;

         // Iterando sobre as salas ordenadas para a disciplina em quest�o.
         ITERA_VECTOR( it_Salas_Ordenadas, salas_Ordenadas, Sala )
         {
            // Checando se a sala em quest�o pertence ao TPS especificado pelo solver
            if ( it_Vars_x_Disc_Und_TPS_Turma->second.front()->getSubCjtSala()->salas.find(
               ( *it_Salas_Ordenadas )->getId() ) != 
               it_Vars_x_Disc_Und_TPS_Turma->second.front()->getSubCjtSala()->salas.end() )
            {
               std::map< Sala *, std::vector< std::pair< int /*dia*/, int /*creds. Livres*/ > > >::iterator
                  it_Tempo_Livre_Sala = tempo_Livre_Sala.find( *it_Salas_Ordenadas );

               if ( it_Tempo_Livre_Sala == tempo_Livre_Sala.end() )
               {
                  std::cout << "Opa. Sala nao encontrada na estrutura\n"
                            << "<tempo_Livre_Sala> (SolverMIP::converteCjtSalaEmSala()) !!!"
                            << "\n\nSaindo." << std::endl;

                  exit( 1 );
               }

               // Indica se os dias demandados pelas vars x s�o
               // compat�veis com os dias dispon�veis da sala.
               bool dias_Sala_Compativeis = true;

               // Iterando em cada variavel X armazenada para o conjunto em quest�o
               ITERA_VECTOR( it_Dias_Demandados_Vars_x, it_Vars_x_Disc_Und_TPS_Turma->second, Variable )
               {
                  // Iterando nos dias disponiveis da sala
                  std::vector< std::pair< int /*dia*/, int /*creds. Livres*/ > >::iterator
                     it_Dia = it_Tempo_Livre_Sala->second.begin();

                  for (; it_Dia != it_Tempo_Livre_Sala->second.end(); ++it_Dia )
                  {
                     // Se encontrei o dia, testo se tem o tempo livre necessario. Caso
                     // nao possua, posso parar de tentar alocar nessa sala.
                     if ( it_Dia->first == ( *it_Dias_Demandados_Vars_x )->getDia() )
                     {
						int tempoCredSL = ( *it_Dias_Demandados_Vars_x )->getDisciplina()->getTempoCredSemanaLetiva();
						int nCreds = (int) ( *it_Dias_Demandados_Vars_x )->getValue();

                        if ( it_Dia->second >= nCreds*tempoCredSL )
                        {
                           // Nao fa�o nada aqui. A busca pelos outros dias continua.
                           // Apenas dou um break por efici�ncia
                           break;
                        }
                        else
                        {
                           dias_Sala_Compativeis = false;

                           // J� que o dia � invi�vel, n�o
                           // faz sentido buscar os outros dias.
                           break;
                        }
                     }
                  }

                  if ( !dias_Sala_Compativeis )
                  {
                     // Parando o iterador <it_Dias_Demandados_Vars_x>.
                     // J� se sabe que a sala n�o � compat�vel para o dia em quest�o. Portanto
                     // paro a busca pelos demais dias livres que a sala pode ter.
                     break;
                  }
               }

               // Teste para saber se posso alocar na sala em quest�o.
               if ( dias_Sala_Compativeis )
               {
                  // Iterando em cada variavel X armazenada
                  ITERA_VECTOR( it_Dias_Demandados_Vars_x,
                     it_Vars_x_Disc_Und_TPS_Turma->second, Variable )
                  {
                     if ( ( *it_Dias_Demandados_Vars_x )->getSala() != NULL )
                     {
                        std::cout << "Opa. Fui setar a sala para uma\n"
                                  << "var x__i_u_tps_t e ja estava setada.\n"
                                  << "(// J� que o dia e invi�vel, n�o faz sentido\n"
                                  << "buscar os outros dias.) !!!"
                                  << "\n\nSaindo." << std::endl;

                        exit( 1 );
                     }

                     // Setando a sala na(s) variavel(eis)
                     ( *it_Dias_Demandados_Vars_x )->setSala( *it_Salas_Ordenadas );

                     // Iterando nos dias disponiveis da sala
                     std::vector< std::pair< int /*dia*/, int /*creds. Livres*/ > >::iterator
                        it_Dia = it_Tempo_Livre_Sala->second.begin();

                     for (; it_Dia != it_Tempo_Livre_Sala->second.end(); it_Dia++ )
                     {
                        // Quando encontro o dia correto, atualizo
                        // a estrutura que armazena os cr�ditos livres.
                        if ( it_Dia->first == ( *it_Dias_Demandados_Vars_x )->getDia() )
                        {
							int tempoCredSL = ( *it_Dias_Demandados_Vars_x )->getDisciplina()->getTempoCredSemanaLetiva();
							int nCreds = (int) ( *it_Dias_Demandados_Vars_x )->getValue();

                           it_Dia->second -= (tempoCredSL*nCreds);

                           // Apenas por efici�ncia
                           break;
                        }
                     }
                  }

                  // Setando a FLAG que indica se alocou ou n�o
                  alocou = true;

                  // Parando a busca de salas.
                  break;
               }
               else
               {
                  // Nao fa�o nada. Deixo continuar
                  // tentando alocar nas outras salas associadas.
               }
            }
            else
            {
               // Nao fa�o nada. Deixo continuar
               // tentando alocar nas outras salas associadas.
            }
         }

         // Checando se a aloca��o foi realizada.
         // Abaixo, devemos tratar da poss�vel causa da aloca��o da turma da disciplina em quest�o
         // n�o ter sido realizada.
         // 1 - Quando a disciplina foi dividida em mais de 1 dia letivo para realizar um atendimento,
         // pode ser que n�o se tenha mais nenhuma sala com os dias letivos demandados com cr�ditos livres
         // o suficiente.
         // Solu��o: Por eqto, atendo em quantas salas diferentes forem necess�rias, sem um padr�o de
         // escolha das salas.
         // Tinha outras causas, mas como mudei a heur�stica pra poder rodar pra todas as salas
         // associadas (pelo usu�rio e pelo solver), acredito que este seja o �nico erro.

         if ( !alocou )
         {
            // Estrutura que armazena separadamente, por unidade,
            // turma, tipo de sala e DIA, as variaveis x coletadas.

            std::map< std::vector< int /*Unidade, TPS, Turma, dia*/ >, Variable * > 
               vars_x_Disc_Und_TPS_Turma_DIA;

            // Armazenando os dados na estrutura
            ITERA_VECTOR( it_Dias_Demandados_Vars_x,
               it_Vars_x_Disc_Und_TPS_Turma->second, Variable )
            {
               std::vector< int /*Unidade, TPS, Turma, Dia*/ > chave;

               chave.push_back( ( *it_Dias_Demandados_Vars_x )->getUnidade()->getId() );
               chave.push_back( ( *it_Dias_Demandados_Vars_x )->getSubCjtSala()->getId() );
               chave.push_back( ( *it_Dias_Demandados_Vars_x )->getTurma() );
               chave.push_back( ( *it_Dias_Demandados_Vars_x )->getDia() );

               if ( vars_x_Disc_Und_TPS_Turma_DIA.find( chave )
                  != vars_x_Disc_Und_TPS_Turma_DIA.end() )
               {
                  std::cout << "Opa. Fui add um dado na estrutura\n"
                            << "<vars_x_Disc_Und_TPS_Turma_DIA> e iria\n"
                            << "sobrescrever um existente. VOLTAR PARA VECTOR.\n"
                            << "(SolverMIP::converteCjtSalaEmSala()) !!!"
                            << "\n\nSaindo." << std::endl;

                  exit( 1 );
               }

               vars_x_Disc_Und_TPS_Turma_DIA[ chave ] = ( *it_Dias_Demandados_Vars_x );
            }

            std::map< std::vector< int /*Unidade, TPS, Turma, Dia */ >, Variable * >::iterator
               it_Vars_x_Disc_Und_TPS_Turma_DIA = vars_x_Disc_Und_TPS_Turma_DIA.begin();

            // Iterando em cada variavel da estrutura <vars_x_Disc_Und_TPS_Turma_DIA>
            for(; it_Vars_x_Disc_Und_TPS_Turma_DIA != vars_x_Disc_Und_TPS_Turma_DIA.end(); 
                  it_Vars_x_Disc_Und_TPS_Turma_DIA++ )
            {
               bool continuaBusca = false;

               // Iterando sobre as salas ordenadas para a disciplina em quest�o.
               ITERA_VECTOR( it_Salas_Ordenadas, salas_Ordenadas, Sala )
               {
                  // Checando se a sala em quest�o pertence ao TPS especificado pelo solver
                  if ( it_Vars_x_Disc_Und_TPS_Turma_DIA->second->getSubCjtSala()->salas.find(
                     ( *it_Salas_Ordenadas )->getId() ) != 
                     it_Vars_x_Disc_Und_TPS_Turma_DIA->second->getSubCjtSala()->salas.end() )
                  {
                     std::map< Sala *, std::vector< std::pair< int /*dia*/, int /*minutos Livres*/ > > >::iterator
                        it_Tempo_Livre_Sala = tempo_Livre_Sala.find( *it_Salas_Ordenadas );

                     if ( it_Tempo_Livre_Sala == tempo_Livre_Sala.end() )
                     {
                        std::cout << "Opa. Sala nao encontrada na estrutura <tempo_Livre_Sala>\n"
                                  << "(SolverMIP::converteCjtSalaEmSala()) !!!\n"
                                  << "\n\nSaindo." << std::endl;

                        exit( 1 );
                     }

                     // Indica se o dia demandado pela var x em
                     // quest�o � compat�vel com o dia dispon�vel da sala.

                     // Iterando nos dias disponiveis da sala
                     std::vector< std::pair< int /*dia*/, int /*minutos Livres*/ > >::iterator
                        it_Dia = it_Tempo_Livre_Sala->second.begin();

                     for (; it_Dia != it_Tempo_Livre_Sala->second.end(); it_Dia++ )
                     {
                        // Se encontrei o dia, testo se tem o tempo livre necessario. Caso
                        // nao possua, posso parar de tentar alocar nessa sala.

                        if ( it_Dia->first == it_Vars_x_Disc_Und_TPS_Turma_DIA->second->getDia() )
                        {
							int tempoCredSL = it_Vars_x_Disc_Und_TPS_Turma_DIA->second->getDisciplina()->getTempoCredSemanaLetiva();
							int nCreds = (int) it_Vars_x_Disc_Und_TPS_Turma_DIA->second->getValue();

                           if ( it_Dia->second >= tempoCredSL*nCreds )
                           {
                              // J� posso alocar. Pois trata-se de apenas um dia.

                              // Setando a sala na variavel
                              it_Vars_x_Disc_Und_TPS_Turma_DIA->second->setSala( *it_Salas_Ordenadas );

                              // Atualizo a estrutura que armazena o tempo livre.
                              it_Dia->second -= tempoCredSL*nCreds;
                              it_Salas_Ordenadas = salas_Ordenadas.begin();

                              alocou = true;

                              // Apenas dou um break por efici�ncia
                              break;
                           }
                           else
                           {
                              // Continuo a busca em outras salas.
                              continuaBusca = true;

                              // Dia � invi�vel.
                              break;
                           }
                        }
                     }

                     if ( alocou )
                     {
                        alocou = false;
                        break;
                     }
                  }
                  else
                  {
                     // Nao fa�o nada. Deixo continuar
                     // tentando alocar nas outras salas associadas.
                  }
               }

               // TRIEDA-715
               // Ainda assim, a vari�vel x em quest�o pode n�o ter sido convertida.
               // Pode acontecer o caso em que a vari�vel x tem valor maior que 1 e todas as salas
               // do TPS especificado pela vari�vel em quest�o possuem apenas 1 cred livre. Desse modo,
               // deve-se dividir os cr�ditos da disciplina, alocando-os separadamente por sala.

               if ( continuaBusca && !alocou )
               {
                  // Referenciando a vari�vel em quest�o.
                  Variable * pt_Var_x = ( it_Vars_x_Disc_Und_TPS_Turma_DIA->second );

                  // A ideia aqui � pegar a vari�vel em quest�o e sair tentando alocar os cr�ditos
                  // separadamente. Eles ser�o separados de acordo com a disponibilidade de cada
                  // sala.
                  // Quando a vari�vel for totalmente alocada, aponta-se para NULL e finaliza a aloca��o da mesma.
                  // IMPORTANTE: Como estou dividindo os cr�ditos, tenho que criar novas vari�veis. Lembrar de adiciona-las
                  // � estrutura <vars_x> que armazena todas as vari�veis do tipo x.

                  while ( pt_Var_x )
                  {
                     // Iterando sobre as salas ordenadas para a disciplina em quest�o.
                     ITERA_VECTOR( it_Salas_Ordenadas, salas_Ordenadas, Sala )
                     {
                        // Checando se a sala em quest�o pertence ao TPS especificado pelo solver
                        if ( pt_Var_x->getSubCjtSala()->salas.find( ( *it_Salas_Ordenadas )->getId() ) != 
                             pt_Var_x->getSubCjtSala()->salas.end() )
                        {
                           std::map< Sala *, std::vector< std::pair< int /*dia*/, int /*minutos Livres*/ > > >::iterator
                              it_Tempo_Livre_Sala = tempo_Livre_Sala.find( *it_Salas_Ordenadas );

                           if ( it_Tempo_Livre_Sala == tempo_Livre_Sala.end() )
                           {
                              std::cout << "Opa. Sala nao encontrada na estrutura\n"
                                        << "<tempo_Livre_Sala>\n"
                                        << "(SolverMIP::converteCjtSalaEmSala()) !!!"
                                        << "\n\nSaindo." << std::endl;

                              exit( 1 );
                           }

                           // Iterando nos dias disponiveis da sala
                           std::vector< std::pair< int /*dia*/, int /*minutos Livres*/ > >::iterator
                              it_Dia = ( it_Tempo_Livre_Sala->second.begin() );

                           for (; it_Dia != it_Tempo_Livre_Sala->second.end(); it_Dia++ )
                           {
                              // Se encontrei o dia, testo se tem tempo
                              // livre necessario. Caso nao possua, posso parar de tentar alocar nessa sala.
                              if ( it_Dia->first == pt_Var_x->getDia() )
                              {
                                 if ( it_Dia->second > 0 )
                                 {
                                    // J� posso alocar. Pois trata-se de apenas um dia.
							
									int tempoCredSL = pt_Var_x->getDisciplina()->getTempoCredSemanaLetiva();
									int nCreds = (int) pt_Var_x->getValue();

                                    // C�lculo do total de tempo a ser alocado.
                                    int tempo_Alocar = (int)( tempoCredSL*nCreds - it_Dia->second );

                                    // Criando uma nova vari�vel para uma divis�o da vari�vel em quest�o.
                                    Variable * var_x_NAO_ALOCADA = NULL;

                                    // Se o total de cr�ditos a serem alocados for maior do que a 
                                    // o total de cr�dtios livres da sala (tempo_Alocar > 0), devo
                                    // criar uma c�pia da vari�vel 'x' em quest�o e atualizar o seu
                                    // valor com a diferen�a entre o total de cr�ditos da var subtraido
                                    // do total de cred. livre da sala. J� no caso em que o total de
                                    // cr�ditos alocados � menor ou igual ao total de cr�ditos livres
                                    // da sala em quest�o (tempo_Alocar <= 0), devo apenas alocar.

                                    if ( tempo_Alocar > 0 )
                                    {
									   // Numero de creditos em excesso, que nao poderao ser alocados
									   int excessoCreds = (int) ceil( (double) tempo_Alocar/tempoCredSL );

                                       // Criando uma nova vari�vel para a divis�o da vari�vel em quest�o.
                                       var_x_NAO_ALOCADA = new Variable( *pt_Var_x );

                                       // Atualizando a quantidade de cr�ditos n�o alocada.
                                       var_x_NAO_ALOCADA->setValue( excessoCreds ); //TODO: conferir o valor, originalmente estava diferente

                                       // Adicionando a nova vari�vel � estrutura <vars_x>
                                       vars_x.push_back( var_x_NAO_ALOCADA );

                                       // Atualizando o valor da vari�vel a ser atendida.
                                       pt_Var_x->setValue( nCreds - excessoCreds ); //TODO: conferir o valor, originalmente estava diferente
                                    }

                                    // Setando a sala na variavel
                                    pt_Var_x->setSala( *it_Salas_Ordenadas );

                                    // Atualizo a estrutura que armazena o tempo livre.
                                    it_Dia->second -= (int) nCreds;

                                    // Checando se ainda existe algum cr�dito para alocar.
                                    if ( var_x_NAO_ALOCADA )
                                    {
                                       pt_Var_x = var_x_NAO_ALOCADA;
                                    }
                                    else
                                    {
                                       // Condi��o para sair do loop.
                                       pt_Var_x = NULL;
                                    }

                                    alocou = true;

                                    // Apenas dou um break por efici�ncia
                                    break;
                                 }
                                 else
                                 {
                                    // Dia � invi�vel.
                                    break;
                                 }
                              }
                           }

                           if ( alocou )
                           {
                              alocou = false;
                              break;
                           }
                        }
                        else
                        {
                           // Nao fa�o nada. Deixo continuar tentando
                           // alocar nas outras salas associadas.
                        }
                     }
                  }
               }
            }
         }
      }
   }

   // ---------------------------------------
   // Imprimindo as vari�veis x_{i,d,u,s,t} convertidas.

   std::cout << "\n\n\n";
   std::cout << "x\t\ti\td\tu\ts\tt\n";

   ITERA_VECTOR( it_Vars_x, vars_x, Variable )
   {
      if ( ( *it_Vars_x )->getSala() == NULL )
      {
         printf( "\nOPA. Variavel x (x_i(%d)_d(%d)_u(%d)_tps(%d)_t(%d))nao convertida.\n\n",
                 ( *it_Vars_x )->getTurma(),
                 ( *it_Vars_x )->getDisciplina()->getId(),
                 ( *it_Vars_x )->getUnidade()->getId(),
                 ( *it_Vars_x )->getSubCjtSala()->getId(),
                 ( *it_Vars_x )->getDia() );

         exit( 1 );
      }

      std::cout << (*it_Vars_x)->getValue() << "\t\t"
                << ( *it_Vars_x )->getTurma() << "\t"
                << ( *it_Vars_x )->getDisciplina()->getCodigo() << "\t"
                << ( *it_Vars_x )->getUnidade()->getCodigo() << "\t"
                << ( *it_Vars_x )->getSala()->getCodigo() << "\t"
                << ( *it_Vars_x )->getDia() << "\n";
   }
}

void SolverMIP::getSolutionTaticoPorAluno()
{
   // POVOANDO AS CLASSES DE SAIDA

   int at_Tatico_Counter = 0;

   // Iterando sobre as vari�veis do tipo x.
   ITERA_GGROUP( it_Vars_x, vars_xh, VariableTatico )
   {
	  int dia = ( *it_Vars_x )->getDia();
	  Disciplina *d = ( *it_Vars_x )->getDisciplina();
	  int turma = ( *it_Vars_x )->getTurma();
	  HorarioAula *hi = ( *it_Vars_x )->getHorarioAulaInicial();
	  HorarioAula *hf = ( *it_Vars_x )->getHorarioAulaFinal();
  	  int nCreds = d->getCalendario()->retornaNroCreditosEntreHorarios( hi, hf );
	  Sala *sala = ( *it_Vars_x )->getSala();
      Unidade * unidade = ( *it_Vars_x )->getUnidade();

      // Descobrindo qual Campus a vari�vel x em quest�o pertence.
      Campus * campus = problemData->refCampus[ ( *it_Vars_x )->getUnidade()->getIdCampus() ];
	
      bool novo_Campus = true;
      ITERA_GGROUP( it_At_Campus, ( *problemSolution->atendimento_campus ), AtendimentoCampus )
      {
         if ( it_At_Campus->getId() == campus->getId() )
         {
            if ( it_At_Campus->atendimentos_unidades->size() == 0 )
            {
               std::cout << "Achei que nao era pra cair aqui <dbg1>" << std::endl;

               // NOVA UNIDADE
               // exit( 1 );
            }
            else
            {
               bool nova_Unidade = true;
               ITERA_GGROUP( it_At_Unidade, ( *it_At_Campus->atendimentos_unidades ), AtendimentoUnidade )
               {
                  if ( it_At_Unidade->getId() == unidade->getId() )
                  {
                     if ( it_At_Unidade->atendimentos_salas->size() == 0 )
                     {
                        std::cout << "Achei que nao era pra cair aqui <dbg2>" << std::endl;

                        // NOVA SALA
                        // exit( 1 );
                     }
                     else
                     {
                        bool nova_Sala = true;
                        ITERA_GGROUP( it_At_Sala, ( *it_At_Unidade->atendimentos_salas ), AtendimentoSala )
                        {
                           if ( it_At_Sala->getId() == sala->getId() )
                           {
							    AtendimentoDiaSemana *at_Dia_Semana = NULL;
								bool novo_Dia = false;

                                if ( it_At_Sala->atendimentos_dias_semana->size() == 0 )
                                {
                                   std::cout << "Achei que nao era pra cair aqui <dbg3>" << std::endl;

                                   // NOVO DIA SEMANA
                                   // exit( 1 );
                                }
                                else
                                {
									// Verifica se o AtendimentoDiaSemana j� existe
									ITERA_GGROUP( it_At_Dia, ( *it_At_Sala->atendimentos_dias_semana ), AtendimentoDiaSemana )
									{
										if ( it_At_Dia->getDiaSemana() == dia )
										{
											if ( it_At_Dia->atendimentos_tatico->size() == 0 )
											{
												std::cout << "Achei que nao era pra cair aqui <dbg4>" << std::endl;
												// NOVO ATENDIMENTO
												// exit( 1 );
											}

											at_Dia_Semana = *it_At_Dia;
											break;
										}
									}

									if ( at_Dia_Semana == NULL )
									{
										novo_Dia = true;

										// Cadastrando o dia da semana
										at_Dia_Semana = new AtendimentoDiaSemana(
											this->problemSolution->getIdAtendimentos() );

										at_Dia_Semana->setDiaSemana( dia );
									}

									#pragma region CADASTRO DE ATENDIMENTO TATICO

									Trio<int,int,Disciplina*> trio;
									trio.set(campus->getId(), turma, d );

									std::map< Trio<int,int,Disciplina*>, GGroup< AlunoDemanda* > >::iterator
										itMap = problemData->mapCampusTurmaDisc_AlunosDemanda.find( trio );
							
									if ( itMap == problemData->mapCampusTurmaDisc_AlunosDemanda.end() )
									{
										std::cout << "\nvoid SolverMIP::getSolutionTatico() <dbg5>" << std::endl;
										std::cout << "\nNao era pra cair aqui <dbg5>" << std::endl;
										std::cout << "\nNao encontrado AlunosDemanda em: Disciplina: " << d->getId();
										std::cout << "\tTurma: " << turma;
										continue;
									}

									// Todas as ofertas atendidas
									GGroup< Oferta* > ofertas;
									ITERA_GGROUP( itAlunoDemanda, itMap->second, AlunoDemanda )
									{
										ofertas.add( itAlunoDemanda->demanda->oferta );									
									}

									ITERA_GGROUP( itOferta, ofertas, Oferta )
									{										
										Oferta *oferta = *itOferta;

										AtendimentoTatico * at_Tatico = new AtendimentoTatico(
										this->problemSolution->getIdAtendimentos(),
										this->problemSolution->getIdAtendimentos() );

										// Verificando se a disciplina � de carater pr�tico ou te�rico.
										if ( d->getId() > 0 && d->getCredTeoricos() > 0 )
										{
											at_Tatico->setQtdCreditosTeoricos( nCreds );
										}
										else
										{
											at_Tatico->setQtdCreditosPraticos( nCreds );
										}

										AtendimentoOferta * at_Oferta = new AtendimentoOferta( this->problemSolution->getIdAtendimentos() );
										
										ITERA_GGROUP( itAlunoDemanda, itMap->second, AlunoDemanda )
										{
											if ( itAlunoDemanda->demanda->oferta != oferta )
												continue;

											if ( itAlunoDemanda->demanda->getDisciplinaId() < 0 )
											{
												int alunoId = itAlunoDemanda->getAlunoId();
												int discId = - itAlunoDemanda->demanda->getDisciplinaId();

												// Disciplina pratica que teve seu AlunoDemanda criado internamente, pelo solver.
												// Deve-se passar o AlunoDemanda original, que corresponde ao da disciplina teorica.
												AlunoDemanda* alunoDemanda = problemData->procuraAlunoDemanda( discId, alunoId );
												if ( alunoDemanda != NULL )
												{
													at_Oferta->alunosDemandasAtendidas.add( alunoDemanda->getId() );
												}
											}
											else
											{
												at_Oferta->alunosDemandasAtendidas.add( itAlunoDemanda->getId() );
											}
										}
																																	 
										stringstream str;
										str << oferta->getId();
										at_Oferta->setOfertaCursoCampiId( str.str() );

										int id_disc = d->getId();
											 
										std::pair< Curso *, Curriculo * > parCursoCurr = std::make_pair( oferta->curso, oferta->curriculo );
										Disciplina *discOriginal = problemData->ehSubstitutaDe( d, parCursoCurr );
										if ( discOriginal != NULL )
										{
											at_Oferta->setDisciplinaSubstitutaId( id_disc );
											at_Oferta->setDisciplinaId( discOriginal->getId() );
											at_Oferta->disciplina = d;
										}
										else
										{
											at_Oferta->setDisciplinaId( id_disc );
											at_Oferta->disciplina = d;
										}

										at_Oferta->setQuantidade( at_Oferta->alunosDemandasAtendidas.size() );
										at_Oferta->setTurma( turma );
										at_Oferta->oferta = oferta;

										at_Tatico->atendimento_oferta = at_Oferta;

										at_Dia_Semana->atendimentos_tatico->add( at_Tatico );

										++at_Tatico_Counter;

									}

									#pragma endregion
                                }

                                if ( novo_Dia )
                                {
                                    it_At_Sala->atendimentos_dias_semana->add( at_Dia_Semana );
                                }
                               
								nova_Sala = false;
                                break;
                           }
                        }

                        if ( nova_Sala )
                        {
                            // Cadastrando a Sala
                            AtendimentoSala * at_Sala = new AtendimentoSala(
                              this->problemSolution->getIdAtendimentos() );

                            at_Sala->setId( sala->getId() );
                            at_Sala->setSalaId( sala->getCodigo() );
                            at_Sala->sala = sala;

                            // Cadastrando o dia da semana
                            AtendimentoDiaSemana * at_Dia_Semana = new AtendimentoDiaSemana(
                              this->problemSolution->getIdAtendimentos() );

                            at_Dia_Semana->setDiaSemana( dia );
							
							#pragma region CADASTRO DE ATENDIMENTO TATICO PARA NOVA SALA

							Trio<int,int,Disciplina*> trio;
							trio.set(campus->getId(), turma, d );

							std::map< Trio<int,int,Disciplina*>, GGroup< AlunoDemanda* > >::iterator
								itMap = problemData->mapCampusTurmaDisc_AlunosDemanda.find( trio );
							
							if ( itMap == problemData->mapCampusTurmaDisc_AlunosDemanda.end() )
							{
								std::cout << "\nvoid SolverMIP::getSolutionTatico() <dbg5>" << std::endl;
								std::cout << "\nNao era pra cair aqui <dbg5>" << std::endl;
								std::cout << "\nNao encontrado AlunosDemanda em: Disciplina: " << d->getId();
								std::cout << "\tTurma: " << turma;
								continue;
							}

							// Todas as ofertas atendidas
							GGroup< Oferta* > ofertas;
							ITERA_GGROUP( itAlunoDemanda, itMap->second, AlunoDemanda )
							{
								ofertas.add( itAlunoDemanda->demanda->oferta );									
							}

							ITERA_GGROUP( itOferta, ofertas, Oferta )
							{										
								Oferta *oferta = *itOferta;

								AtendimentoTatico * at_Tatico = new AtendimentoTatico(
								this->problemSolution->getIdAtendimentos(),
								this->problemSolution->getIdAtendimentos() );

								// Verificando se a disciplina � de carater pr�tico ou te�rico.
								if ( d->getId() > 0 && d->getCredTeoricos() > 0 )
								{
									at_Tatico->setQtdCreditosTeoricos( nCreds );
								}
								else
								{
									at_Tatico->setQtdCreditosPraticos( nCreds );
								}

								AtendimentoOferta * at_Oferta = new AtendimentoOferta( this->problemSolution->getIdAtendimentos() );
											
								ITERA_GGROUP( itAlunoDemanda, itMap->second, AlunoDemanda )
								{
									if ( itAlunoDemanda->demanda->oferta != oferta )
										continue;

									if ( itAlunoDemanda->demanda->getDisciplinaId() < 0 )
									{
										int alunoId = itAlunoDemanda->getAlunoId();
										int discId = - itAlunoDemanda->demanda->getDisciplinaId();

										// Disciplina pratica que teve seu AlunoDemanda criado internamente, pelo solver.
										// Deve-se passar o AlunoDemanda original, que corresponde ao da disciplina teorica.
										AlunoDemanda* alunoDemanda = problemData->procuraAlunoDemanda( discId, alunoId );
										if ( alunoDemanda != NULL )
										{
											at_Oferta->alunosDemandasAtendidas.add( alunoDemanda->getId() );
										}
									}
									else
									{
										at_Oferta->alunosDemandasAtendidas.add( itAlunoDemanda->getId() );
									}
								}
																																	 
								stringstream str;
								str << oferta->getId();
								at_Oferta->setOfertaCursoCampiId( str.str() );

								int id_disc = d->getId();
											 
								std::pair< Curso *, Curriculo * > parCursoCurr = std::make_pair( oferta->curso, oferta->curriculo );
								Disciplina *discOriginal = problemData->ehSubstitutaDe( d, parCursoCurr );
								if ( discOriginal != NULL )
								{
									at_Oferta->setDisciplinaSubstitutaId( id_disc );
									at_Oferta->setDisciplinaId( discOriginal->getId() );
									at_Oferta->disciplina = d;
								}
								else
								{
									at_Oferta->setDisciplinaId( id_disc );
									at_Oferta->disciplina = d;
								}

								at_Oferta->setQuantidade( at_Oferta->alunosDemandasAtendidas.size() );
								at_Oferta->setTurma( turma );
								at_Oferta->oferta = oferta;

								at_Tatico->atendimento_oferta = at_Oferta;

								at_Dia_Semana->atendimentos_tatico->add( at_Tatico );

								++at_Tatico_Counter;
							
							}

							#pragma endregion

                           at_Sala->atendimentos_dias_semana->add( at_Dia_Semana );
                           it_At_Unidade->atendimentos_salas->add( at_Sala );
                        }
                     }

                     nova_Unidade = false;
                     break;

                  }
               }

               if ( nova_Unidade )
               {
				   // Cadastrando a Unidade
                   AtendimentoUnidade * at_Unidade = new AtendimentoUnidade(
                     this->problemSolution->getIdAtendimentos() );

                   at_Unidade->setId( unidade->getId() );
                   at_Unidade->setCodigoUnidade( unidade->getCodigo() );
                   at_Unidade->unidade = unidade;

                   // Cadastrando a Sala
                   AtendimentoSala * at_Sala = new AtendimentoSala(
                     this->problemSolution->getIdAtendimentos() );

                   at_Sala->setId( sala->getId() );
                   at_Sala->setSalaId( sala->getCodigo() );
                   at_Sala->sala = sala;

                   // Cadastrando o dia da semana
                   AtendimentoDiaSemana * at_Dia_Semana = new AtendimentoDiaSemana(
                     this->problemSolution->getIdAtendimentos() );

                   at_Dia_Semana->setDiaSemana( dia );
 					
					#pragma region CADASTRO DE ATENDIMENTO TATICO PARA NOVA UNIDADE
				   
					Trio<int,int,Disciplina*> trio;
					trio.set(campus->getId(), turma, d );

					std::map< Trio<int,int,Disciplina*>, GGroup< AlunoDemanda* > >::iterator
						itMap = problemData->mapCampusTurmaDisc_AlunosDemanda.find( trio );
							
					if ( itMap == problemData->mapCampusTurmaDisc_AlunosDemanda.end() )
					{
						std::cout << "\nvoid SolverMIP::getSolutionTatico() <dbg5>" << std::endl;
						std::cout << "\nNao era pra cair aqui <dbg5>" << std::endl;
						std::cout << "\nNao encontrado AlunosDemanda em: Disciplina: " << d->getId();
						std::cout << "\tTurma: " << turma;
						continue;
					}

					// Todas as ofertas atendidas
					GGroup< Oferta* > ofertas;
					ITERA_GGROUP( itAlunoDemanda, itMap->second, AlunoDemanda )
					{
						ofertas.add( itAlunoDemanda->demanda->oferta );									
					}

					ITERA_GGROUP( itOferta, ofertas, Oferta )
					{										
						Oferta *oferta = *itOferta;

						AtendimentoTatico * at_Tatico = new AtendimentoTatico(
						this->problemSolution->getIdAtendimentos(),
						this->problemSolution->getIdAtendimentos() );

						// Verificando se a disciplina � de carater pr�tico ou te�rico.
						if ( d->getId() > 0 && d->getCredTeoricos() > 0 )
						{
							at_Tatico->setQtdCreditosTeoricos( nCreds );
						}
						else
						{
							at_Tatico->setQtdCreditosPraticos( nCreds );
						}

						AtendimentoOferta * at_Oferta = new AtendimentoOferta( this->problemSolution->getIdAtendimentos() );
									
						ITERA_GGROUP( itAlunoDemanda, itMap->second, AlunoDemanda )
						{
							if ( itAlunoDemanda->demanda->oferta != oferta )
								continue;

							if ( itAlunoDemanda->demanda->getDisciplinaId() < 0 )
							{
								int alunoId = itAlunoDemanda->getAlunoId();
								int discId = - itAlunoDemanda->demanda->getDisciplinaId();

								// Disciplina pratica que teve seu AlunoDemanda criado internamente, pelo solver.
								// Deve-se passar o AlunoDemanda original, que corresponde ao da disciplina teorica.
								AlunoDemanda* alunoDemanda = problemData->procuraAlunoDemanda( discId, alunoId );
								if ( alunoDemanda != NULL )
								{
									at_Oferta->alunosDemandasAtendidas.add( alunoDemanda->getId() );
								}
							}
							else
							{
								at_Oferta->alunosDemandasAtendidas.add( itAlunoDemanda->getId() );
							}
						}
																																	 
						stringstream str;
						str << oferta->getId();
						at_Oferta->setOfertaCursoCampiId( str.str() );

						int id_disc = d->getId();
											 
						std::pair< Curso *, Curriculo * > parCursoCurr = std::make_pair( oferta->curso, oferta->curriculo );
						Disciplina *discOriginal = problemData->ehSubstitutaDe( d, parCursoCurr );
						if ( discOriginal != NULL )
						{
							at_Oferta->setDisciplinaSubstitutaId( id_disc );
							at_Oferta->setDisciplinaId( discOriginal->getId() );
							at_Oferta->disciplina = d;
						}
						else
						{
							at_Oferta->setDisciplinaId( id_disc );
							at_Oferta->disciplina = d;
						}

						at_Oferta->setQuantidade( at_Oferta->alunosDemandasAtendidas.size() );
						at_Oferta->setTurma( turma );
						at_Oferta->oferta = oferta;

						at_Tatico->atendimento_oferta = at_Oferta;

						at_Dia_Semana->atendimentos_tatico->add( at_Tatico );

						++at_Tatico_Counter;
					}
					#pragma endregion

                    at_Sala->atendimentos_dias_semana->add( at_Dia_Semana );
                    at_Unidade->atendimentos_salas->add( at_Sala );
                    it_At_Campus->atendimentos_unidades->add( at_Unidade );
               }
            }

            novo_Campus = false;
            break;
         }
      }

      if ( novo_Campus )
      {
			AtendimentoCampus * at_Campus = new AtendimentoCampus(
			this->problemSolution->getIdAtendimentos() );

			at_Campus->setId( campus->getId() );
			at_Campus->setCampusId( campus->getCodigo() );
			at_Campus->campus = campus;

			// Cadastrando a Unidade
			AtendimentoUnidade * at_Unidade = new AtendimentoUnidade(
			this->problemSolution->getIdAtendimentos() );

			at_Unidade->setId( unidade->getId() );
			at_Unidade->setCodigoUnidade( unidade->getCodigo() );
			at_Unidade->unidade = unidade;

			// Cadastrando a Sala
			AtendimentoSala * at_Sala = new AtendimentoSala(
			this->problemSolution->getIdAtendimentos() );

			at_Sala->setId( sala->getId() );
			at_Sala->setSalaId( sala->getCodigo() );
			at_Sala->sala = sala;

			// Cadastrando o dia da semana
			AtendimentoDiaSemana * at_Dia_Semana = new AtendimentoDiaSemana(
			this->problemSolution->getIdAtendimentos() );

			at_Dia_Semana->setDiaSemana( dia );

			#pragma region CADASTRO DE ATENDIMENTO TATICO PARA NOVO CAMPUS

			Trio<int,int,Disciplina*> trio;
			trio.set(campus->getId(), turma, d );

			std::map< Trio<int,int,Disciplina*>, GGroup< AlunoDemanda* > >::iterator
				itMap = problemData->mapCampusTurmaDisc_AlunosDemanda.find( trio );
							
			if ( itMap == problemData->mapCampusTurmaDisc_AlunosDemanda.end() )
			{
				std::cout << "\nvoid SolverMIP::getSolutionTatico() <dbg5>" << std::endl;
				std::cout << "\nNao era pra cair aqui <dbg5>" << std::endl;
				std::cout << "\nNao encontrado AlunosDemanda em: Disciplina: " << d->getId();
				std::cout << "\tTurma: " << turma;
				continue;
			}

			// Todas as ofertas atendidas
			GGroup< Oferta* > ofertas;
			ITERA_GGROUP( itAlunoDemanda, itMap->second, AlunoDemanda )
			{
				ofertas.add( itAlunoDemanda->demanda->oferta );									
			}

			ITERA_GGROUP( itOferta, ofertas, Oferta )
			{										
				Oferta *oferta = *itOferta;
				
				AtendimentoTatico * at_Tatico = new AtendimentoTatico(
				this->problemSolution->getIdAtendimentos(),
				this->problemSolution->getIdAtendimentos() );

				// Verificando se a disciplina � de carater pr�tico ou te�rico.
				if ( d->getId() > 0 && d->getCredTeoricos() > 0 )
				{
					at_Tatico->setQtdCreditosTeoricos( nCreds );
				}
				else
				{
					at_Tatico->setQtdCreditosPraticos( nCreds );
				}

				AtendimentoOferta * at_Oferta = new AtendimentoOferta( this->problemSolution->getIdAtendimentos() );
								
				ITERA_GGROUP( itAlunoDemanda, itMap->second, AlunoDemanda )
				{
					if ( itAlunoDemanda->demanda->oferta != oferta )
						continue;

					if ( itAlunoDemanda->demanda->getDisciplinaId() < 0 )
					{
						int alunoId = itAlunoDemanda->getAlunoId();
						int discId = - itAlunoDemanda->demanda->getDisciplinaId();

						// Disciplina pratica que teve seu AlunoDemanda criado internamente, pelo solver.
						// Deve-se passar o AlunoDemanda original, que corresponde ao da disciplina teorica.
						AlunoDemanda* alunoDemanda = problemData->procuraAlunoDemanda( discId, alunoId );
						if ( alunoDemanda != NULL )
						{
							at_Oferta->alunosDemandasAtendidas.add( alunoDemanda->getId() );
						}
					}
					else
					{
						at_Oferta->alunosDemandasAtendidas.add( itAlunoDemanda->getId() );
					}
				}
																																	 
				stringstream str;
				str << oferta->getId();
				at_Oferta->setOfertaCursoCampiId( str.str() );

				int id_disc = d->getId();
											 
				std::pair< Curso *, Curriculo * > parCursoCurr = std::make_pair( oferta->curso, oferta->curriculo );
				Disciplina *discOriginal = problemData->ehSubstitutaDe( d, parCursoCurr );
				if ( discOriginal != NULL )
				{
					at_Oferta->setDisciplinaSubstitutaId( id_disc );
					at_Oferta->setDisciplinaId( discOriginal->getId() );
					at_Oferta->disciplina = d;
				}
				else
				{
					at_Oferta->setDisciplinaId( id_disc );
					at_Oferta->disciplina = d;
				}

				at_Oferta->setQuantidade( at_Oferta->alunosDemandasAtendidas.size() );
				at_Oferta->setTurma( turma );
				at_Oferta->oferta = oferta;

				at_Tatico->atendimento_oferta = at_Oferta;

				at_Dia_Semana->atendimentos_tatico->add( at_Tatico );

				++at_Tatico_Counter;
			}
			#pragma endregion
         
			at_Sala->atendimentos_dias_semana->add( at_Dia_Semana );
			at_Unidade->atendimentos_salas->add( at_Sala );
			at_Campus->atendimentos_unidades->add( at_Unidade );

			problemSolution->atendimento_campus->add( at_Campus );
      }
   }
}

void SolverMIP::getSolutionTatico()
{
   // POVOANDO AS CLASSES DE SAIDA

   int at_Tatico_Counter = 0;

   // Iterando sobre as vari�veis do tipo x.
   ITERA_VECTOR( it_Vars_x, vars_x, Variable )
   {
      // Descobrindo qual Campus a vari�vel x em quest�o pertence.
      Campus * campus = problemData->refCampus[ ( *it_Vars_x )->getUnidade()->getIdCampus() ];
	
      bool novo_Campus = true;
      ITERA_GGROUP( it_At_Campus, ( *problemSolution->atendimento_campus ),
                    AtendimentoCampus )
      {
         if ( it_At_Campus->getId() == campus->getId() )
         {
            if ( it_At_Campus->atendimentos_unidades->size() == 0 )
            {
               std::cout << "Achei que nao era pra cair aqui <dbg1>" << std::endl;

               // NOVA UNIDADE
               // exit( 1 );
            }
            else
            {
               Unidade * unidade = ( *it_Vars_x )->getUnidade();

               bool nova_Unidade = true;
               ITERA_GGROUP( it_At_Unidade, ( *it_At_Campus->atendimentos_unidades ),
                             AtendimentoUnidade )
               {
                  if ( it_At_Unidade->getId() == unidade->getId() )
                  {
                     if ( it_At_Unidade->atendimentos_salas->size() == 0 )
                     {
                        std::cout << "Achei que nao era pra cair aqui <dbg2>" << std::endl;

                        // NOVA SALA
                        // exit( 1 );
                     }
                     else
                     {
                        Sala * sala = ( *it_Vars_x )->getSala();

                        bool nova_Sala = true;
                        ITERA_GGROUP( it_At_Sala, ( *it_At_Unidade->atendimentos_salas ),
                                      AtendimentoSala )
                        {
                           if ( it_At_Sala->getId() == sala->getId() )
                           {
                              if ( it_At_Sala->atendimentos_dias_semana->size() == 0 )
                              {
                                 std::cout << "Achei que nao era pra cair aqui <dbg3>" << std::endl;

                                 // NOVO DIA SEMANA
                                 // exit( 1 );
                              }
                              else
                              {
                                 int dia = ( *it_Vars_x )->getDia();
								 Disciplina *d = ( *it_Vars_x )->getDisciplina();
								 int turma = ( *it_Vars_x )->getTurma();

                                 bool novo_Dia = true;
                                 ITERA_GGROUP( it_At_Dia, ( *it_At_Sala->atendimentos_dias_semana ), AtendimentoDiaSemana )
                                 {
                                    if ( it_At_Dia->getDiaSemana() == dia )
                                    {
                                       if ( it_At_Dia->atendimentos_tatico->size() == 0 )
                                       {
                                          std::cout << "Achei que nao era pra cair aqui <dbg4>" << std::endl;

                                          // NOVO ATENDIMENTO
                                          // exit( 1 );
                                       }
                                       else
                                       {
                                          // CADASTRO DE ATENDIMENTO TATICO

                                          // Para cada variavel a__i_d_o existem para a variavel x__i_d_u_s_t em quest�o.
										  std::map< std::pair< int /*turma*/, Disciplina * >,
													std::vector< Variable * > >::iterator
													itMap = vars_a.find( std::make_pair( turma, d ) );
							
										  if ( itMap == vars_a.end() )
										  {
											std::cout << "\nvoid SolverMIP::getSolutionTatico() <dbg5>" << std::endl;
											std::cout << "\nNao era pra cair aqui <dbg5>" << std::endl;
											std::cout << "\nNao encontrada variavel 'a' com: Disciplina: " << d->getId();
											std::cout << "\tTurma: " << turma;
											continue;
										  }

										  for ( std::vector< Variable* >::iterator it_Vars_a = itMap->second.begin();
												it_Vars_a != itMap->second.end(); ++it_Vars_a )
										  {
                                             AtendimentoTatico * at_Tatico = new AtendimentoTatico(
                                                this->problemSolution->getIdAtendimentos(),
                                                this->problemSolution->getIdAtendimentos() );

                                             // Verificando se a disciplina � de carater pr�tico ou te�rico.
                                             if ( ( *it_Vars_x )->getDisciplina()->getId() > 0
                                                && ( *it_Vars_x )->getDisciplina()->getCredTeoricos() > 0 )
                                             {
                                                at_Tatico->setQtdCreditosTeoricos( (int)( ( *it_Vars_x )->getValue() ) );
                                             }
                                             else
                                             {
                                                at_Tatico->setQtdCreditosPraticos( (int)( ( *it_Vars_x )->getValue() ) );
                                             }

                                             AtendimentoOferta * at_Oferta = new AtendimentoOferta( this->problemSolution->getIdAtendimentos() );

											 Oferta *oferta = ( *it_Vars_a )->getOferta();
											 
                                             stringstream str;
                                             str << oferta->getId();
                                             at_Oferta->setOfertaCursoCampiId( str.str() );

                                             int id_disc = d->getId();
											 
											 std::pair< Curso *, Curriculo * > parCursoCurr = std::make_pair( oferta->curso, oferta->curriculo );
											 Disciplina *discOriginal = problemData->ehSubstitutaDe( d, parCursoCurr );
											 if ( discOriginal != NULL )
											 {
												 at_Oferta->setDisciplinaSubstitutaId( id_disc );
												 at_Oferta->setDisciplinaId( discOriginal->getId() );
												 at_Oferta->disciplina = d;
											 }
											 else
											 {
												at_Oferta->setDisciplinaId( id_disc );
												at_Oferta->disciplina = d;
											 }

                                             at_Oferta->setQuantidade( (int)( ( *it_Vars_a )->getValue() ) );
                                             at_Oferta->setTurma( turma );
                                             at_Oferta->oferta = ( *it_Vars_a )->getOferta();

											 at_Tatico->atendimento_oferta = at_Oferta;

                                             it_At_Dia->atendimentos_tatico->add( at_Tatico );

                                             ++at_Tatico_Counter;
                                          }
                                       }
                                       novo_Dia = false;
                                       break;
                                    }
                                 }

                                 if ( novo_Dia )
                                 {
									//Disciplina *d = ( *it_Vars_x )->getDisciplina();
									//int turma = ( *it_Vars_x )->getTurma();

                                    // Cadastrando o dia da semana
                                    AtendimentoDiaSemana * at_Dia_Semana = new AtendimentoDiaSemana(
                                       this->problemSolution->getIdAtendimentos() );

                                    at_Dia_Semana->setDiaSemana( ( *it_Vars_x )->getDia() );

                                    // Para cada variavel x__i_d_u_s_t existe uma variavel a__i_d_o em quest�o.

									std::map< std::pair< int /*turma*/, Disciplina * >,
											  std::vector< Variable * > >::iterator
											  itMap = vars_a.find( std::make_pair( turma, d ) );
							
									if ( itMap == vars_a.end() )
									{
										std::cout << "\nvoid SolverMIP::getSolutionTatico() <dbg5>" << std::endl;
										std::cout << "\nNao era pra cair aqui <dbg5>" << std::endl;
										std::cout << "\nNao encontrada variavel 'a' com: Disciplina: " << d->getId();
										std::cout << "\tTurma: " << turma;
										continue;
									}

									for ( std::vector< Variable* >::iterator it_Vars_a = itMap->second.begin();
										  it_Vars_a != itMap->second.end(); ++it_Vars_a )
                                    {
                                       AtendimentoTatico * at_Tatico = new AtendimentoTatico(
                                          this->problemSolution->getIdAtendimentos(),
                                          this->problemSolution->getIdAtendimentos() );

                                       // Verificando se a disicplina � de carater pr�tico ou te�rico.
                                       if (  ( *it_Vars_x )->getDisciplina()->getId() > 0
                                          && ( *it_Vars_x )->getDisciplina()->getCredTeoricos() > 0 )
                                       {
                                          at_Tatico->setQtdCreditosTeoricos( (int)( ( *it_Vars_x )->getValue() ) );
                                       }
                                       else
                                       {
                                          at_Tatico->setQtdCreditosPraticos( (int)( ( *it_Vars_x )->getValue() ) );
                                       }
											 
									   Oferta *oferta = ( *it_Vars_a )->getOferta();
									   
                                       AtendimentoOferta * at_Oferta = new AtendimentoOferta(
                                          this->problemSolution->getIdAtendimentos() );

                                       stringstream str;
                                       str << oferta->getId();
                                       at_Oferta->setOfertaCursoCampiId( str.str() );

                                       int id_disc = d->getId();
											 
										std::pair< Curso *, Curriculo * > parCursoCurr = std::make_pair( oferta->curso, oferta->curriculo );
										Disciplina *discOriginal = problemData->ehSubstitutaDe( d, parCursoCurr );
										if ( discOriginal != NULL )
										{
											at_Oferta->setDisciplinaSubstitutaId( id_disc );
											at_Oferta->setDisciplinaId( discOriginal->getId() );
											at_Oferta->disciplina = d;
										}
										else
										{
											at_Oferta->setDisciplinaId( id_disc );
											at_Oferta->disciplina = d;
										}

                                       at_Oferta->setQuantidade( (int)( ( *it_Vars_a )->getValue() ) );
                                       at_Oferta->setTurma( ( *it_Vars_a )->getTurma() );
                                       at_Oferta->oferta = oferta;
                                       at_Tatico->atendimento_oferta = at_Oferta;

                                       at_Dia_Semana->atendimentos_tatico->add( at_Tatico );

                                       ++at_Tatico_Counter;
                                    }

                                    it_At_Sala->atendimentos_dias_semana->add( at_Dia_Semana );
                                 }
                              }
                              nova_Sala = false;
                              break;
                           }
                        }

                        if ( nova_Sala )
                        {
							Sala *s = ( *it_Vars_x )->getSala();
							Disciplina *d = ( *it_Vars_x )->getDisciplina();
							int turma = ( *it_Vars_x )->getTurma();

                           // Cadastrando a Sala
                           AtendimentoSala * at_Sala = new AtendimentoSala(
                              this->problemSolution->getIdAtendimentos() );

                           at_Sala->setId( s->getId() );
                           at_Sala->setSalaId( s->getCodigo() );
                           at_Sala->sala = s;

                           // Cadastrando o dia da semana
                           AtendimentoDiaSemana * at_Dia_Semana = new AtendimentoDiaSemana(
                              this->problemSolution->getIdAtendimentos() );

                           at_Dia_Semana->setDiaSemana( ( *it_Vars_x )->getDia() );

                           // Para cada variavel x__i_d_u_s_t existe uma variavel a__i_d_o em quest�o.
							std::map< std::pair< int /*turma*/, Disciplina * >,
									  std::vector< Variable * > >::iterator
									  itMap = vars_a.find( std::make_pair( turma, d ) );
							
							if ( itMap == vars_a.end() )
							{
								std::cout << "\nvoid SolverMIP::getSolutionTatico() <dbg5>" << std::endl;
								std::cout << "\nNao era pra cair aqui <dbg5>" << std::endl;
								std::cout << "\nNao encontrada variavel 'a' com: Disciplina: " << d->getId();
								std::cout << "\tTurma: " << turma;
								continue;
							}

							for ( std::vector< Variable* >::iterator it_Vars_a = itMap->second.begin();
								  it_Vars_a != itMap->second.end(); ++it_Vars_a )
                            {
                              AtendimentoTatico * at_Tatico = new AtendimentoTatico(
                                 this->problemSolution->getIdAtendimentos(),
                                 this->problemSolution->getIdAtendimentos() );

                              // Verificando se a disicplina � de carater pr�tico ou te�rico.
                              if (  d->getId() > 0 && d->getCredTeoricos() > 0 )
                              {
                                 at_Tatico->setQtdCreditosTeoricos( (int)( ( *it_Vars_x )->getValue() ) );
                              }
                              else
                              {
                                 at_Tatico->setQtdCreditosPraticos( (int)( ( *it_Vars_x )->getValue() ) );
                              }
							  
							  Oferta *oferta = ( *it_Vars_a )->getOferta();

                              AtendimentoOferta * at_Oferta = new AtendimentoOferta(
                                 this->problemSolution->getIdAtendimentos() );

                              stringstream str;
                              str << oferta->getId();
                              at_Oferta->setOfertaCursoCampiId( str.str() );

                              int id_disc = d->getId();
								
							  std::pair< Curso *, Curriculo * > parCursoCurr = std::make_pair( oferta->curso, oferta->curriculo );
							  Disciplina *discOriginal = problemData->ehSubstitutaDe( d, parCursoCurr );
							  if ( discOriginal != NULL )
							  {
									at_Oferta->setDisciplinaSubstitutaId( id_disc );	// substituta
									at_Oferta->setDisciplinaId( discOriginal->getId() );// original
									at_Oferta->disciplina = d;							// substituta
							  }
							  else
						      {
									at_Oferta->setDisciplinaId( id_disc );  // original
									at_Oferta->disciplina = d;				// original
							  }

                              at_Oferta->setQuantidade( (int)( ( *it_Vars_a )->getValue() ) );
                              at_Oferta->setTurma( (*it_Vars_a)->getTurma() );
                              at_Oferta->oferta = oferta;
                              at_Tatico->atendimento_oferta = at_Oferta;

                              at_Dia_Semana->atendimentos_tatico->add( at_Tatico );
                              ++at_Tatico_Counter;
                           }
                           at_Sala->atendimentos_dias_semana->add( at_Dia_Semana );
                           it_At_Unidade->atendimentos_salas->add( at_Sala );
                        }
                     }

                     nova_Unidade = false;
                     break;
                  }
               }

               if ( nova_Unidade )
               {
				   Disciplina *d = ( *it_Vars_x )->getDisciplina();
				   int turma = ( *it_Vars_x )->getTurma();

				   // Cadastrando a Unidade
                  AtendimentoUnidade * at_Unidade = new AtendimentoUnidade(
                     this->problemSolution->getIdAtendimentos() );

                  at_Unidade->setId( ( *it_Vars_x )->getUnidade()->getId() );
                  at_Unidade->setCodigoUnidade( ( *it_Vars_x )->getUnidade()->getCodigo() );
                  at_Unidade->unidade = ( *it_Vars_x )->getUnidade();

                  // Cadastrando a Sala
                  AtendimentoSala * at_Sala = new AtendimentoSala(
                     this->problemSolution->getIdAtendimentos() );

                  at_Sala->setId( ( *it_Vars_x )->getSala()->getId() );
                  at_Sala->setSalaId( ( *it_Vars_x )->getSala()->getCodigo() );
                  at_Sala->sala = ( *it_Vars_x )->getSala();

                  // Cadastrando o dia da semana
                  AtendimentoDiaSemana * at_Dia_Semana = new AtendimentoDiaSemana(
                     this->problemSolution->getIdAtendimentos() );

                  at_Dia_Semana->setDiaSemana( ( *it_Vars_x )->getDia() );

                  // Para cada variavel a__i_d_o existem para a variavel x__i_d_u_s_t em quest�o.

				  std::map< std::pair< int /*turma*/, Disciplina * >,
							std::vector< Variable * > >::iterator
							itMap = vars_a.find( std::make_pair( turma, d ) );
							
				  if ( itMap == vars_a.end() )
				  {
					std::cout << "\nvoid SolverMIP::getSolutionTatico() <dbg6>" << std::endl;
					std::cout << "\nNao era pra cair aqui <dbg6>" << std::endl;
					std::cout << "\nNao encontrada variavel 'a' com: Disciplina: " << d->getId();
					std::cout << "\tTurma: " << turma;
					continue;
				  }

				  for ( std::vector< Variable* >::iterator it_Vars_a = itMap->second.begin();
						it_Vars_a != itMap->second.end(); ++it_Vars_a )
				  {
                     AtendimentoTatico * at_Tatico = new AtendimentoTatico(
                        this->problemSolution->getIdAtendimentos(),
                        this->problemSolution->getIdAtendimentos() );

                     // Verificando se a disciplina � de carater pr�tico ou te�rico.
                     if (  ( *it_Vars_x )->getDisciplina()->getId() > 0
                        && ( *it_Vars_x )->getDisciplina()->getCredTeoricos() > 0 )
                     {
                        at_Tatico->setQtdCreditosTeoricos( (int)( ( *it_Vars_x )->getValue() ) );
                     }
                     else
                     {
                        at_Tatico->setQtdCreditosPraticos( (int)( ( *it_Vars_x )->getValue() ) );
                     }
					 
					 Oferta *oferta = ( *it_Vars_a )->getOferta();

                     AtendimentoOferta * at_Oferta = new AtendimentoOferta(
                        this->problemSolution->getIdAtendimentos() );

                     stringstream str;
                     str << ( *it_Vars_a )->getOferta()->getId();
                     at_Oferta->setOfertaCursoCampiId( str.str() );

                     int id_disc = ( *it_Vars_a )->getDisciplina()->getId();

 					 std::pair< Curso *, Curriculo * > parCursoCurr = std::make_pair( oferta->curso, oferta->curriculo );
					 Disciplina *discOriginal = problemData->ehSubstitutaDe( d, parCursoCurr );
					 if ( discOriginal != NULL )
					 {
						at_Oferta->setDisciplinaSubstitutaId( id_disc );
						at_Oferta->setDisciplinaId( discOriginal->getId() );
						at_Oferta->disciplina = d;
					 }
					 else
					 {
						at_Oferta->setDisciplinaId( id_disc );
						at_Oferta->disciplina = d;
					 }

                     at_Oferta->setQuantidade( (int)( ( *it_Vars_a )->getValue() ) );
                     at_Oferta->setTurma( ( *it_Vars_a )->getTurma() );
                     at_Oferta->oferta = ( *it_Vars_a )->getOferta();
                     at_Tatico->atendimento_oferta = at_Oferta;

                     at_Dia_Semana->atendimentos_tatico->add( at_Tatico );
                     ++at_Tatico_Counter;
                  }

                  at_Sala->atendimentos_dias_semana->add( at_Dia_Semana );
                  at_Unidade->atendimentos_salas->add( at_Sala );
                  it_At_Campus->atendimentos_unidades->add( at_Unidade );
               }
            }

            novo_Campus = false;
            break;
         }
      }

      if ( novo_Campus )
      {
		 Disciplina *d = ( *it_Vars_x )->getDisciplina();
		 int turma = ( *it_Vars_x )->getTurma();

         AtendimentoCampus * at_Campus = new AtendimentoCampus(
            this->problemSolution->getIdAtendimentos() );

         at_Campus->setId( campus->getId() );
         at_Campus->setCampusId( campus->getCodigo() );
         at_Campus->campus = campus;

         // Cadastrando a Unidade
         AtendimentoUnidade * at_Unidade = new AtendimentoUnidade(
            this->problemSolution->getIdAtendimentos() );

         at_Unidade->setId( ( *it_Vars_x )->getUnidade()->getId() );
         at_Unidade->setCodigoUnidade( ( *it_Vars_x )->getUnidade()->getCodigo() );
         at_Unidade->unidade = ( *it_Vars_x )->getUnidade();

         // Cadastrando a Sala
         AtendimentoSala * at_Sala = new AtendimentoSala(
            this->problemSolution->getIdAtendimentos() );

         at_Sala->setId( ( *it_Vars_x )->getSala()->getId() );
         at_Sala->setSalaId( ( *it_Vars_x )->getSala()->getCodigo() );
         at_Sala->sala = ( *it_Vars_x )->getSala();

         // Cadastrando o dia da semana
         AtendimentoDiaSemana * at_Dia_Semana = new AtendimentoDiaSemana(
            this->problemSolution->getIdAtendimentos() );

         at_Dia_Semana->setDiaSemana( ( *it_Vars_x )->getDia() );

         // Para cada variavel a__i_d_o existem para a variavel x__i_d_u_s_t em quest�o.
		std::map< std::pair< int /*turma*/, Disciplina * >,
					std::vector< Variable * > >::iterator
					itMap = vars_a.find( std::make_pair( turma, d ) );
							
		if ( itMap == vars_a.end() )
		{
			std::cout << "\nvoid SolverMIP::getSolutionTatico() <dbg7>" << std::endl;
			std::cout << "\nNao era pra cair aqui <dbg7>" << std::endl;
			std::cout << "\nNao encontrada variavel 'a' com: Disciplina: " << d->getId();
			std::cout << "\tTurma: " << turma;
			continue;
		}

		for ( std::vector< Variable* >::iterator it_Vars_a = itMap->second.begin();
				it_Vars_a != itMap->second.end(); ++it_Vars_a )
        {
            AtendimentoTatico * at_Tatico = new AtendimentoTatico(
               this->problemSolution->getIdAtendimentos(),
			   this->problemSolution->getIdAtendimentos() );

            // Verificando se a disciplina � de carater pr�tico ou te�rico.
            if ( ( *it_Vars_x )->getDisciplina()->getId() > 0
               && ( *it_Vars_x )->getDisciplina()->getCredTeoricos() > 0 )
            {
               at_Tatico->setQtdCreditosTeoricos( (int)( ( *it_Vars_x )->getValue() ) );
            }
            else
            {
               at_Tatico->setQtdCreditosPraticos( (int)( ( *it_Vars_x )->getValue() ) );
            }
					 
			Oferta *oferta = ( *it_Vars_a )->getOferta();

            AtendimentoOferta * at_Oferta = new AtendimentoOferta(
            this->problemSolution->getIdAtendimentos() );

            stringstream str;
            str << ( *it_Vars_a )->getOferta()->getId();
            at_Oferta->setOfertaCursoCampiId( str.str() );

            int id_disc = ( *it_Vars_a )->getDisciplina()->getId();

 			std::pair< Curso *, Curriculo * > parCursoCurr = std::make_pair( oferta->curso, oferta->curriculo );
			Disciplina *discOriginal = problemData->ehSubstitutaDe( d, parCursoCurr );
			if ( discOriginal != NULL )
			{
				at_Oferta->setDisciplinaSubstitutaId( id_disc );
				at_Oferta->setDisciplinaId( discOriginal->getId() );
				at_Oferta->disciplina = d;
			}
			else
			{
				at_Oferta->setDisciplinaId( id_disc );
				at_Oferta->disciplina = d;
			}

            at_Oferta->setQuantidade( (int)( ( *it_Vars_a )->getValue() ) );
            at_Oferta->setTurma( ( *it_Vars_a )->getTurma() );
            at_Oferta->oferta = ( *it_Vars_a )->getOferta();
            at_Tatico->atendimento_oferta = at_Oferta;

            at_Dia_Semana->atendimentos_tatico->add( at_Tatico );
            ++at_Tatico_Counter;
         }
         at_Sala->atendimentos_dias_semana->add( at_Dia_Semana );
         at_Unidade->atendimentos_salas->add( at_Sala );
         at_Campus->atendimentos_unidades->add( at_Unidade );

         problemSolution->atendimento_campus->add( at_Campus );
      }
   }
}

int SolverMIP::solveOperacional()
{
   solveOperacionalMIP();
   // Criando uma solu��o inicial
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

   // Estruturas de Vizinhan�a
   //NSSeqSwapEqBlocks nsSeqSwapEqBlocks ( *problemData );
   //NSSwapEqSchedulesBlocks nsSwapEqSchedulesBlocks ( *problemData );
   //NSSwapEqTeachersBlocks nsSwapEqTeachersBlocks ( *problemData );
   NSShift nsShift( *problemData );

   // Heur�sticas de Busca Local - Descida Rand�mica
   //RandomDescentMethod rdmSeqSwapEqBlocks ( avaliador, nsSeqSwapEqBlocks, 300 );
   //RandomDescentMethod rdmSwapEqSchedulesBlocks ( avaliador, nsSwapEqSchedulesBlocks, 300 );
   //RandomDescentMethod rdmSwapEqTeachersBlocks ( avaliador, nsSwapEqTeachersBlocks, 300 );
   RandomDescentMethod rdmShift ( avaliador, nsShift, 300 );

   //rdmSeqSwapEqBlocks.exec( solucaoOperacional, 30, 0 );
   //rdmSwapEqSchedulesBlocks.exec( solucaoOperacional, 30, 0 );
   //rdmSwapEqTeachersBlocks.exec( solucaoOperacional, 30, 0 );
   rdmShift.exec( solucaoOperacional, 30, 0 );

   // Mecanismo de perturba��o
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

   // Busca Local Iterada por N�veis
   //IteratedLocalSearchLevels ilsl ( avaliador, rvnd, ilslPerturbationPlus2, 20 , 4 );

   // Par�metro 2 : tempo ( em segundos )
   //ilsl.exec( solucaoOperacional, 30, 0 );

   // Avalia��o final
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

   // Armazena a solu��o operacional no problem solution
   problemSolution->solucao_operacional = &( solucaoOperacional );
   return 1;
}

int SolverMIP::solveOperacionalMIP()
{
   int varNum = 0;
   int constNum = 0;

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

   lp->createLP( "SolverOperacional",
      OPTSENSE_MINIMIZE, PROB_MIP );

#ifdef DEBUG
   printf( "Creating LP...\n" );
#endif

   relacionaProfessoresDisciplinas();

   // Variable creation
   varNum = criaVariaveisOperacional();

#ifdef PRINT_cria_variaveis
   printf( "Total of Variables: %i\n\n", varNum );
#endif

   // Constraint creation
   constNum = criaRestricoesOperacional();

#ifdef PRINT_cria_restricoes
   printf( "Total of Constraints: %i\n\n", constNum );
#endif

   lp->writeProbLP( "SolverOperacional" );

#ifdef DEBUG
 //  lp->writeProbLP( "SolverOperacional" );
#endif

   int status = 0;

#ifdef DEBUG
   lp->setTimeLimit( 3600 );
#else
   lp->setTimeLimit( 600 );
#endif

   //lp->setMIPRelTol( 0.01 );
   lp->setMIPEmphasis(0);
   lp->setVarSel(4);
   lp->setCuts(5);
   lp->setMIPScreenLog( 4 );
   lp->setTimeLimit(7200);
   //lp->setPolishAfterNode(1);
   
   lp->setPreSolve(OPT_TRUE);

   status = lp->optimize( METHOD_MIP );

   double * x = new double[ lp->getNumCols() ];

   lp->getX( x );

   VariableOpHash::iterator vit;
   FILE * fout = fopen( "solucaoOp.txt", "wt" );
   solVarsOp.clear();

   vit = vHashOp.begin();

   for (; vit != vHashOp.end(); vit++ )
   {
      VariableOp v = vit->first;

      if ( x[ vit->second ] > 0.01 )
      {
         VariableOp * newVar = new VariableOp( v );

         newVar->setValue( x[ vit->second ] );
         fprintf( fout, "%s = %f\n",
            v.toString().c_str(), x[ vit->second ] );

         solVarsOp.push_back( newVar );
      }
   }

   fclose( fout );
   delete [] x;

   geraProfessoresVirtuaisMIP();

   return status;
}

void SolverMIP::relacionaProfessoresDisciplinas()
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

         // TODO -- como recuperar o professor que foi alocado � aula ???
         std::pair< int, int > professor_disciplina(
            professor->getId(), disciplina->getId() );

         // Se o professor e a disciplina da aula em quest�o se relacionarem
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

void SolverMIP::getSolutionOperacional()
{
   int i = 0, j = 0, id_operacional = 0;

   Aula * aula = NULL;
   Professor * professor = NULL;
   ProfessorVirtualOutput * professor_virtual = NULL;

   // Preenche a lista de professores virtuais do output
   MatrizSolucao * matriz_solucao
      = problemSolution->solucao_operacional->getMatrizAulas();

   for (; i < (int)this->problemData->professores_virtuais.size(); i++ )
   {
      // Devo apenas analisar os professores virtuais
      professor = this->problemData->professores_virtuais.at( i );

      // Inclui um novo professor na
      // lista de professores virtuais
      professor_virtual = new ProfessorVirtualOutput();

      professor_virtual->setId( professor->getId() );
      professor_virtual->setChMin( professor->getChMin() );
      professor_virtual->setChMax( professor->getChMax() );
      professor_virtual->setTitulacaoId( professor->getTitulacaoId() );
      professor_virtual->setAreaTitulacaoId( professor->getAreaId() );

      // Procura pelas disciplinas que o professor dever� ministrar
      id_operacional = professor->getIdOperacional();
      for (; j < (int)matriz_solucao->at( id_operacional )->size(); j++ )
      {
         aula = matriz_solucao->at( id_operacional )->at( j );

         if ( aula != NULL && aula->eVirtual() == false )
         {
            // Adiciona uma disciplina a mais ao professor virtual
            professor_virtual->disciplinas.add(
               aula->getDisciplina()->getId() );
         }
      }

      this->problemSolution->professores_virtuais->add( professor_virtual );
   }
}

Professor * SolverMIP::criaProfessorVirtual( HorarioDia * horario, int cred,
   std::set< std::pair< Professor *, HorarioDia * > > & profVirtualList )
{
   // Procura primeiro professor virtual livre no horario
   Professor * prof = NULL;
   int nCreds;
   int dia = horario->getDia();

   for ( int i = 0; i < (int)problemData->professores_virtuais.size(); i++ )
   {
      std::pair< Professor *, HorarioDia * > auxPair;
	  
      // Tenta professor livre para todos os creditos
      bool profOK = true;
	  
	  nCreds = 1;
	  Calendario* c = horario->getHorarioAula()->getCalendario();
	  HorarioAula* h = horario->getHorarioAula();
	 
	  while ( h != NULL && nCreds <= cred )
      {
		  DateTime inicio = h->getInicio();
		  DateTime fim = h->getFinal();

		  std::set< std::pair< Professor *, HorarioDia * > >::iterator itProfV = profVirtualList.begin();
		  for ( ; itProfV != profVirtualList.end(); itProfV++ )
		  {
			  // Verifica todos os hor�rios j� alocados para o professor i no dia
			  // e marca profOK=false caso este j� possua aula em algum horario que
			  // sobreponha h

			  if ( itProfV->first != problemData->professores_virtuais[ i ] ||
				   itProfV->second->getDia() != dia )
			  {
				  continue;
			  }

			  HorarioDia* hd = itProfV->second;
			  			  
			  DateTime dt2Inicio = hd->getHorarioAula()->getInicio();			   
			  DateTime dt2Fim = hd->getHorarioAula()->getFinal();

			  if ( ( hd->getHorarioAula() == h ) ||				    
				   ( ( dt2Inicio <= inicio ) && ( dt2Fim > inicio ) ) ||
				   ( ( dt2Inicio >= inicio ) && ( dt2Inicio < fim ) ) )
			  {
				  profOK = false; // prof j� possui horario que sobrep�e h no dia
				  break;
			  }
		  }

		  // itera
		  nCreds++;
		  h = c->getProximoHorario( h );
      }

      if ( profOK )
      {
         prof = problemData->professores_virtuais[ i ];

		 nCreds = 1;
		 Calendario* c = horario->getHorarioAula()->getCalendario();
		 HorarioAula* h = horario->getHorarioAula();
	     		 
		 while ( h!=NULL && nCreds <= cred )
		 {
            auxPair.first = prof;
            auxPair.second = problemData->getHorarioDiaCorrespondente( h, dia );

            profVirtualList.insert( auxPair );

			// itera
			nCreds++;
			h = c->getProximoHorario( h );
         }

         break;
      }
   }

   if ( prof != NULL )
   {
      return prof;
   }

   // Criando um professor virtual.
   prof = new Professor( true );
   int idProf = -1 * (int)( problemData->professores_virtuais.size() );
   idProf--;
   prof->setId( idProf );

   // Setando alguns dados para o novo professor
   prof->tipo_contrato = ( *problemData->tipos_contrato.begin() );

   // Temporariamente vou setar os ids
   // dos magist�rios com valores negativos.
   int idMag = 0;

   ITERA_GGROUP_LESSPTR( itDisciplina,
      problemData->disciplinas, Disciplina )
   {
	  #pragma region Equivalencias
	  if ( problemData->mapDiscSubstituidaPor.find( *itDisciplina ) !=
		   problemData->mapDiscSubstituidaPor.end() )
	  {
		  continue;
	  }
	  #pragma endregion

      Magisterio * mag = new Magisterio();

      mag->setId( --idMag );
      mag->disciplina = ( *itDisciplina );
      mag->setDisciplinaId( itDisciplina->getId() );
      mag->setNota( 10 );
      mag->setPreferencia( 1 );

      prof->magisterio.add( mag );
   }

   problemData->campi.begin()->professores.add( prof );
   prof->horarios = problemData->campi.begin()->horarios;

   problemData->professores_virtuais.push_back( prof );

   std::pair< Professor *, HorarioDia * > auxPair;
   
	nCreds = 1;
	Calendario* c = horario->getHorarioAula()->getCalendario();
	HorarioAula* h = horario->getHorarioAula();

	while ( h!=NULL && nCreds <= cred )
	{
       auxPair.first = prof;
       auxPair.second = problemData->getHorarioDiaCorrespondente( h, dia );

       profVirtualList.insert( auxPair );

	   // itera
	   nCreds++;
	   h = c->getProximoHorario( h );
    }

   return prof;
}

void SolverMIP::geraProfessoresVirtuaisMIP()
{
	std::set<std::pair< Professor *, HorarioDia * > > profVirtualList;

   // Procura variaveis que usem professor virtual
   for ( int i = 0; i < (int) solVarsOp.size(); i++ )
   {
      VariableOp * v = solVarsOp[ i ];

      if ( v->getType() != VariableOp::V_X_PROF_AULA_HOR )
      {
         continue;
      }

      if ( v->getProfessor() != NULL )
      {
         continue;
      }

      int nCred = v->getAula()->getTotalCreditos();

      Professor * profVirtual = criaProfessorVirtual(
        v->getHorario(), nCred, profVirtualList );

      v->setProfessor( profVirtual );
   }
}

void SolverMIP::getSolutionOperacionalMIP()
{
   // Procura variaveis que usem professor virtual
   for ( int i = 0; i < (int) solVarsOp.size(); i++ )
   {
      VariableOp * v = solVarsOp[ i ];

      if ( v->getType() != VariableOp::V_X_PROF_AULA_HOR )
      {
         continue;
      }

      if ( v->getProfessor() == NULL
         || !v->getProfessor()->eVirtual() )
      {
         continue;
      }

      // Procura virtual
      bool achou = false;
      ITERA_GGROUP( itProf, ( *problemSolution->professores_virtuais ), ProfessorVirtualOutput )
      {
         ProfessorVirtualOutput * professor_virtual = ( *itProf );

         if ( professor_virtual->getId() == v->getProfessor()->getId() )
         {
            achou = true;

            professor_virtual->disciplinas.add(
               v->getAula()->getDisciplina()->getId() );

            break;
         }
      }

      if ( !achou )
      {
         ProfessorVirtualOutput * professor_virtual = new ProfessorVirtualOutput();

         professor_virtual->setId( v->getProfessor()->getId() );
         professor_virtual->setChMin( v->getProfessor()->getChMin() );
         professor_virtual->setChMax( v->getProfessor()->getChMax() );
         professor_virtual->setTitulacaoId( v->getProfessor()->getTitulacaoId() );
         professor_virtual->setAreaTitulacaoId( v->getProfessor()->getAreaId() );

         professor_virtual->disciplinas.add( v->getAula()->getDisciplina()->getId() );

         problemSolution->professores_virtuais->add( professor_virtual );
      }
   }
}


int SolverMIP::solveTaticoPorCampus()
{
	int statusPre, statusTatico, status = 1;

    ITERA_GGROUP_LESSPTR( itCampus, problemData->campi, Campus )
    {
		solVarsPre.clear();

		int campusId = ( *itCampus )->getId();

		std::cout<<"\n\n--------------------------------------------------------------------------------";		
		std::cout<<"\n-------------------------- Campus " << campusId << "----------------------------\n";

		int n_prioridades = problemData->nPrioridadesDemanda[campusId];

		for( int P = 1; P <= n_prioridades; P++ )
		{
			std::cout<<"\n-------------------------- Prioridade " << P << "---------------------------\n";

			std::cout<<"\n------------------------------Pre-modelo------------------------------\n";

			statusPre = solvePreTatico( campusId, P );    
			carregaVariaveisSolucaoPreTatico( campusId, P );
			preencheMapAtendimentoAluno();

			std::cout<<"\n------------------------------Tatico------------------------------\n";

			statusTatico = solveTaticoBasico( campusId, P );

			if ( problemData->parametros->otimizarPor == "BLOCOCURRICULAR" )
				carregaVariaveisSolucaoTatico( campusId );
			if ( problemData->parametros->otimizarPor == "ALUNO" )		
				carregaVariaveisSolucaoTaticoPorAluno( campusId, P );

			mudaCjtSalaParaSala();
			
			statusTatico = ( statusTatico && statusPre );

			if ( problemData->listSlackDemandaAluno.size() == 0 )
			{
				break;
			}
			else if ( P + 1 <= n_prioridades )
			{
				std::cout << "\nAtualizacao de demandas de prioridade " << P + 1 << "...\n";

				problemData->atualizaDemandas( P+1, campusId );
			}
		}

		problemData->listSlackDemandaAluno.clear(); // Caso seja util ter isso depois, entao tem que fazer um map com campus

        
		// Preenchendo a estrutura "atendimento_campus" com a sa�da.
		if ( problemData->parametros->otimizarPor == "BLOCOCURRICULAR" )
			 getSolutionTatico();
		else if ( problemData->parametros->otimizarPor == "ALUNO" )
			 getSolutionTaticoPorAluno();


		status = ( status && statusTatico );
	}

	return (status);
}

int SolverMIP::solve()
{
   int status = 0;

   if ( problemData->parametros->modo_otimizacao == "TATICO"
         && problemData->atendimentosTatico == NULL )
   {
	  status = solveTaticoPorCampus();

   }
   else if ( problemData->parametros->modo_otimizacao == "OPERACIONAL" )
   {
	  std::cout<<"\n------------------------------Operacional------------------------------\n";

      problemSolution->atendimento_campus;

      if ( problemData->atendimentosTatico != NULL
            && problemData->atendimentosTatico->size() > 0 )
      {
         ITERA_GGROUP( itAtTat, ( *problemData->atendimentosTatico ), AtendimentoCampusSolucao )
         { 
            Campus * campus = problemData->refCampus[ itAtTat->getCampusId() ];

            AtendimentoCampus * atCampus = new AtendimentoCampus( this->problemSolution->getIdAtendimentos() );

            atCampus->setId( campus->getId() );
            atCampus->setCampusId( campus->getCodigo() );
            atCampus->campus = campus;

            ITERA_GGROUP( itAtUnd, itAtTat->atendimentosUnidades, AtendimentoUnidadeSolucao )
            {
               Unidade * unidade = problemData->refUnidade[ itAtUnd->getUnidadeId() ];

               AtendimentoUnidade * atUnidade = new AtendimentoUnidade(
                  this->problemSolution->getIdAtendimentos() );

               atUnidade->setId( unidade->getId() );
               atUnidade->setCodigoUnidade( unidade->getCodigo() );
               atUnidade->unidade = unidade;

               ITERA_GGROUP( itAtSala,
                  itAtUnd->atendimentosSalas, AtendimentoSalaSolucao )
               {
                  Sala * sala = problemData->refSala[ itAtSala->getSalaId() ];

                  AtendimentoSala * atSala = new AtendimentoSala(
                     this->problemSolution->getIdAtendimentos() );

                  atSala->setId( sala->getId() );
                  atSala->setSalaId( sala->getCodigo() );
                  atSala->sala = sala;

                  ITERA_GGROUP( itAtDiaSemana,
                     itAtSala->atendimentosDiasSemana, AtendimentoDiaSemanaSolucao )
                  {
                     AtendimentoDiaSemana * atDiaSemana = new AtendimentoDiaSemana(
                        this->problemSolution->getIdAtendimentos() );

                     atDiaSemana->setDiaSemana( itAtDiaSemana->getDiaSemana() );

                     atSala->atendimentos_dias_semana->add( atDiaSemana );
                  }

                  atUnidade->atendimentos_salas->add( atSala );
               }

               atCampus->atendimentos_unidades->add( atUnidade );
            }

            problemSolution->atendimento_campus->add( atCampus );
         }

         // Resolvendo o modelo operacional
         status = solveOperacionalMIP();

         // Preenche as classes do output operacional
         preencheOutputOperacionalMIP( problemSolution );
      }
      else
      {
         // Neste caso, primeiro deve-se gerar uma sa�da para
         // o modelo t�tico. Em seguida, deve-se resolver o
         // modelo operacional com base na sa�da do modelo t�tico gerada.

	     status = solveTaticoPorCampus();
		 
         // Preenchendo a estrutura "atendimento_campus" com a sa�da.
		 if ( problemData->parametros->otimizarPor == "BLOCOCURRICULAR" )
			 getSolutionTatico();
		 else if ( problemData->parametros->otimizarPor == "ALUNO" )
			 getSolutionTaticoPorAluno();

         // Preenchendo a estrutura "atendimentosTatico".
         problemData->atendimentosTatico
               = new GGroup< AtendimentoCampusSolucao * >();

         ITERA_GGROUP( it_At_Campus,
            ( *problemSolution->atendimento_campus ), AtendimentoCampus )
         {
            problemData->atendimentosTatico->add(
               new AtendimentoCampusSolucao( **it_At_Campus ) );
         }

         // Remove a refer�ncia para os atendimentos t�tico (que pertencem ao output t�tico)
         ITERA_GGROUP( it_At_Campus,
            ( *problemSolution->atendimento_campus ), AtendimentoCampus )
         {
            ITERA_GGROUP( it_At_Unidade,
               ( *it_At_Campus->atendimentos_unidades ), AtendimentoUnidade )
            {
               ITERA_GGROUP( it_At_Sala,
                  ( *it_At_Unidade->atendimentos_salas ), AtendimentoSala )
               {
                  ITERA_GGROUP( it_At_DiaSemana,
                     ( *it_At_Sala->atendimentos_dias_semana ), AtendimentoDiaSemana )
                  {
                     GGroup< AtendimentoTatico * > * atendimentos_tatico
                           = it_At_DiaSemana->atendimentos_tatico;

                     atendimentos_tatico->clear();
                  }
               }
            }
         }
	    
		 std::cout<<"\n------------------------------Operacional------------------------------\n";

         // Criando as aulas que ser�o utilizadas
         // para resolver o modelo operacional
         problemDataLoader->criaAulas();

         // Resolvendo o modelo operacional
         status = solveOperacionalMIP();

         // Preenche as classes do output operacional
         preencheOutputOperacionalMIP( problemSolution );
      }
	   
	  getSolutionOperacionalMIP();

   }
   
   relacionaAlunosDemandas();

   //buscaLocalTempoDeslocamentoSolucao();

   return status;
}

// M�todo que relaciona cada demanda atendida aos
// correspondentes alunos que assistir�o as aulas 
void SolverMIP::relacionaAlunosDemandas()
{
   Campus * campus = NULL;
   Unidade * unidade = NULL;
   Sala * sala = NULL;
   int dia_semana = 0;

   // Lendo os atendimentos oferta da solu��o
   GGroup< AtendimentoOferta * > atendimentosOferta;

   ITERA_GGROUP( it_At_Campus,
      ( *problemSolution->atendimento_campus ), AtendimentoCampus )
   {
      // Campus do atendimento
      campus = it_At_Campus->campus;

      ITERA_GGROUP( it_At_Unidade,
         ( *it_At_Campus->atendimentos_unidades ), AtendimentoUnidade )
      {
         // Unidade do atendimento
         unidade = problemData->refUnidade[ it_At_Unidade->getId() ];

         ITERA_GGROUP( it_At_Sala,
            ( *it_At_Unidade->atendimentos_salas ), AtendimentoSala )
         {
            // Sala do atendimento
            sala = problemData->refSala[ it_At_Sala->getId() ];

            ITERA_GGROUP( it_At_DiaSemana,
               ( *it_At_Sala->atendimentos_dias_semana ), AtendimentoDiaSemana )
            {
               // Dia da semana do atendimento
               dia_semana = it_At_DiaSemana->getDiaSemana();

               // Modelo T�tico
               if ( it_At_DiaSemana->atendimentos_tatico != NULL
                  && it_At_DiaSemana->atendimentos_tatico->size() > 0 )
               {
                  ITERA_GGROUP( it_at_tatico,
                     ( *it_At_DiaSemana->atendimentos_tatico ), AtendimentoTatico )
                  {
                     AtendimentoTatico * at_tatico = ( *it_at_tatico );

                     atendimentosOferta.add( at_tatico->atendimento_oferta );
                  }
               }
               // Modelo Operacional
               else if ( it_At_DiaSemana->atendimentos_turno != NULL
                  && it_At_DiaSemana->atendimentos_turno->size() > 0 )
               {
                  ITERA_GGROUP( it_at_turno,
                     ( *it_At_DiaSemana->atendimentos_turno ), AtendimentoTurno )
                  {
                     AtendimentoTurno * at_turno = ( *it_at_turno );

                     ITERA_GGROUP( it_horario_aula,
                        ( *at_turno->atendimentos_horarios_aula ), AtendimentoHorarioAula )
                     {
                        AtendimentoHorarioAula * horario_aula = ( *it_horario_aula );

                        ITERA_GGROUP( it_oferta,
                           ( *it_horario_aula->atendimentos_ofertas ), AtendimentoOferta )
                        {
                           AtendimentoOferta * oferta = ( *it_oferta );

                           atendimentosOferta.add( oferta );
                        }
                     }
                  }
               }
            }
         }
      }
   }
   // Fim da leitura dos atendimentos oferta

   // Armazenando a informa��o de quantos alunos
   // de cada demanda foram atendidos pela solu��o
   std::map< Demanda *, int > quantidadeAlunosAtendidosDemanda;

   ITERA_GGROUP( it_at_oferta, atendimentosOferta, AtendimentoOferta )
   {
      AtendimentoOferta * at_oferta = ( *it_at_oferta );

      int id_oferta = atoi( at_oferta->getOfertaCursoCampiId().c_str() );
      int id_disciplina = at_oferta->getDisciplinaId();

      Demanda * demanda = this->problemData->buscaDemanda( id_oferta, id_disciplina );

      quantidadeAlunosAtendidosDemanda[ demanda ] += at_oferta->getQuantidade();
   }

   // Preenchendo alunosDemanda com cada demanda atendida de cada aluno
   if ( problemData->parametros->otimizarPor == "ALUNO" )
   {
	   ITERA_GGROUP_LESSPTR( it_alunosDemanda, problemData->alunosDemanda, AlunoDemanda )
	   {
		   AlunoDemanda * aluno_demanda = ( *it_alunosDemanda );

		   Disciplina * disc = aluno_demanda->demanda->disciplina;
		   int alunoId = aluno_demanda->getAlunoId();

		   Aluno* a = problemData->retornaAluno( alunoId );

		   GGroup< Trio<int, int, Disciplina*> > campusTurmaDiscAluno = problemData->mapAluno_CampusTurmaDisc[a];
		   for ( GGroup< Trio<int, int, Disciplina*> >::iterator it_at_aluno = campusTurmaDiscAluno.begin();
				 it_at_aluno != campusTurmaDiscAluno.end(); it_at_aluno++ )
		   {
			   if ( (*it_at_aluno).third == disc )
			   {
					this->problemSolution->alunosDemanda->add( aluno_demanda );
			   }
		   }
	   }
   }

   // Preenchendo alunosDemanda de acordo com o numero de demandas atendidas
   // (Escolhe os alunos que foram atendidos 'aleatoriamente' )
   else if ( problemData->parametros->otimizarPor == "BLOCOCURRICULAR" )
   {
	   // De acordo com o total de alunos de cada demanda que foram atendidos,
	   // adicionamos os alunos na lista de atendidos da solu��o
	   std::map< Demanda *, int >::iterator it_alunos_atendidos =
		  quantidadeAlunosAtendidosDemanda.begin();

	   for (; it_alunos_atendidos != quantidadeAlunosAtendidosDemanda.end();
			  it_alunos_atendidos++ )
	   {
		  Demanda * demanda = it_alunos_atendidos->first;
		  int alunos_atendidos = it_alunos_atendidos->second;

		  GGroup< AlunoDemanda *, LessPtr< AlunoDemanda > > alunosDemanda =
			 this->problemData->mapDemandaAlunos[ demanda ];

		  int cont = 0;

		  ITERA_GGROUP_LESSPTR( it_atendidos, alunosDemanda, AlunoDemanda )
		  {
			 // Mais um aluno dessa demanda foi atendido
			 cont++;

			 AlunoDemanda * aluno_demanda = ( *it_atendidos );
			 this->problemSolution->alunosDemanda->add( aluno_demanda );

			 if ( cont == alunos_atendidos )
			 {
				break;
			 }
		  }
	   }
   }
}

void SolverMIP::preencheOutputOperacionalMIP( ProblemSolution * solution )
{
   Campus * campus = NULL;
   Unidade * unidade = NULL;
   Sala * sala = NULL;
   int dia_semana = 0;

   ITERA_GGROUP( it_At_Campus,
      ( *problemSolution->atendimento_campus ), AtendimentoCampus )
   {
      // Campus do atendimento
      campus = it_At_Campus->campus;

      ITERA_GGROUP( it_At_Unidade,
         ( *it_At_Campus->atendimentos_unidades ), AtendimentoUnidade )
      {
         // Unidade do atendimento
         unidade = problemData->refUnidade[ it_At_Unidade->getId() ];

         ITERA_GGROUP( it_At_Sala,
            ( *it_At_Unidade->atendimentos_salas ), AtendimentoSala )
         {
            // Sala do atendimento
            sala = problemData->refSala[ it_At_Sala->getId() ];

            ITERA_GGROUP( it_At_DiaSemana,
               ( *it_At_Sala->atendimentos_dias_semana ), AtendimentoDiaSemana )
            {
               // Dia da semana do atendimento
               dia_semana = it_At_DiaSemana->getDiaSemana();

               it_At_DiaSemana->atendimentos_turno = new GGroup< AtendimentoTurno * >();

               for ( int i = 0; i < (int)solVarsOp.size(); i++ )
               {
                  VariableOp * v = solVarsOp[ i ];

                  if ( v->getType() != VariableOp::V_X_PROF_AULA_HOR )
                  {
                     continue;
                  }

                  if ( v->getAula()->getSala() != sala )
                  {
                     continue;
                  }

                  if ( v->getAula()->getDiaSemana() != dia_semana )
                  {
                     continue;
                  }

                  Aula * aula = v->getAula();

                  // Procura o turno da aula
                  Oferta * temp = ( *( aula->ofertas.begin() ) );
                  int turno = temp->getTurnoId();

                  AtendimentoTurno * atendimento_turno = NULL;
                  ITERA_GGROUP( it, ( *it_At_DiaSemana->atendimentos_turno ), AtendimentoTurno )
                  {
                     if ( it->getTurnoId() == turno )
                     {
                        atendimento_turno = ( *it );
                        break;
                     }
                  }

                  if ( atendimento_turno == NULL )
                  {
                     atendimento_turno = new AtendimentoTurno(
                        this->problemSolution->getIdAtendimentos() );

                     atendimento_turno->setId( turno );
                     atendimento_turno->setTurnoId( turno );
                     atendimento_turno->turno = temp->turno;

                     it_At_DiaSemana->atendimentos_turno->add( atendimento_turno );
                  }
				  
				  HorarioAula * h = v->getHorarioAula();
				  Calendario * c = v->getDisciplina()->getCalendario();
				  int nCreds = 1;

				  while ( h!=NULL && nCreds <= aula->getTotalCreditos() )
				  {
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

                     GGroup< Oferta *, LessPtr< Oferta > >::iterator 
                        it_oferta = aula->ofertas.begin();

                     for (; it_oferta != aula->ofertas.end(); ++it_oferta )
                     {
                        Oferta * oferta = ( *it_oferta );
                        AtendimentoOferta * atendimento_oferta = new AtendimentoOferta(
                           this->problemSolution->getIdAtendimentos() );

 						std::pair< Curso *, Curriculo * > parCursoCurr = std::make_pair( oferta->curso, oferta->curriculo );
						Disciplina *discOriginal = problemData->ehSubstitutaDe( aula->getDisciplina(), parCursoCurr );
						if ( discOriginal != NULL )
						{
							atendimento_oferta->setDisciplinaSubstitutaId( aula->getDisciplina()->getId() );
							atendimento_oferta->setDisciplinaId( discOriginal->getId() );
							atendimento_oferta->disciplina = discOriginal;
						}
						else
						{
							atendimento_oferta->setDisciplinaId( aula->getDisciplina()->getId() );
							atendimento_oferta->disciplina = aula->getDisciplina();
						}
						
                        atendimento_oferta->setId( oferta->getId() );
                        atendimento_oferta->setTurma( aula->getTurma() );
                        atendimento_oferta->setQuantidade( aula->getQuantidadePorOft(oferta) );

						// ----- alunosDemandasAtendidas -----
						Trio< int /*campusId*/, int /*turma*/, Disciplina* > trio;
						trio.set( campus->getId(), aula->getTurma(), aula->getDisciplina() );

						GGroup<AlunoDemanda*> alunosDemanda =
							problemData->mapCampusTurmaDisc_AlunosDemanda[ trio ];

						ITERA_GGROUP( itAlunoDemanda, alunosDemanda, AlunoDemanda )
						{
							if ( itAlunoDemanda->demanda->getDisciplinaId() < 0 )
							{
								int alunoId = itAlunoDemanda->getAlunoId();
								int discId = - itAlunoDemanda->demanda->getDisciplinaId();

								// Disciplina pratica que teve seu AlunoDemanda criado internamente, pelo solver.
								// Deve-se passar o AlunoDemanda original, que corresponde ao da disciplina teorica.
								AlunoDemanda* alunoDemanda = problemData->procuraAlunoDemanda( discId, alunoId );
								if ( alunoDemanda != NULL )
								{
									atendimento_oferta->alunosDemandasAtendidas.add( alunoDemanda->getId() );
									oferta = alunoDemanda->demanda->oferta; // TODO !!!!!!!!!!!!!!
								}
							}
							else
							{
								atendimento_oferta->alunosDemandasAtendidas.add( itAlunoDemanda->getId() );
								oferta = itAlunoDemanda->demanda->oferta; // TODO !!!!!!!!!!!!!!
							}
						}
						// -----
						
						stringstream str;
                        str << oferta->getId();
                        atendimento_oferta->setOfertaCursoCampiId( str.str() );
                        atendimento_oferta->oferta = oferta;

                        atendimento_horario_aula->atendimentos_ofertas->add( atendimento_oferta );                        
                     }

                     atendimento_turno->atendimentos_horarios_aula->add( atendimento_horario_aula );

					 // itera
					 h = c->getProximoHorario( h );
					 nCreds++;
                  }
               }
            }
         }
      }
   }
}

void SolverMIP::preencheOutputOperacional( ProblemSolution * solution )
{
   /*
   Campus * campus = NULL;
   Unidade * unidade = NULL;
   Sala * sala = NULL;
   int dia_semana = 0;

   int total_horarios = solution->solucao_operacional->getTotalHorarios();
   MatrizSolucao * matriz_aulas = solution->solucao_operacional->getMatrizAulas();

   ITERA_GGROUP( it_At_Campus, ( *problemSolution->atendimento_campus ), AtendimentoCampus )
   {
      // Campus do atendimento
      campus = it_At_Campus->campus;

      ITERA_GGROUP( it_At_Unidade, ( *it_At_Campus->atendimentos_unidades ), AtendimentoUnidade )
      {
         // Unidade do atendimento
         unidade = problemData->refUnidade[ it_At_Unidade->getId() ];

         ITERA_GGROUP( it_At_Sala, ( *it_At_Unidade->atendimentos_salas ), AtendimentoSala )
         {
            // Sala do atendimento
            sala = problemData->refSala[ it_At_Sala->getId() ];

            ITERA_GGROUP( it_At_DiaSemana, ( *it_At_Sala->atendimentos_dias_semana ), AtendimentoDiaSemana )
            {
               // Dia da semana do atendimento
               dia_semana = it_At_DiaSemana->getDiaSemana();

               it_At_DiaSemana->atendimentos_turno = new GGroup< AtendimentoTurno * >();

               std::vector< std::vector< Aula * > * >::iterator
                  it_matriz_aulas = matriz_aulas->begin();

               for (; it_matriz_aulas != matriz_aulas->end(); it_matriz_aulas++ )
               {
                  int linha_professor = std::distance( matriz_aulas->begin(), it_matriz_aulas );

                  std::vector< Aula * > * aulas = ( *it_matriz_aulas );
                  std::vector< Aula * >::iterator it_aula = aulas->begin();

                  for (; it_aula != aulas->end(); it_aula++ )
                  {
                     Aula * aula = ( *it_aula );
                     if ( aula == NULL || aula->eVirtual() == true )
                     {
                        continue;
                     }

                     // Verifica se a aula est� alocada na sala e dia da semana atuais
                     if ( !aulaAlocada( aula, campus, unidade, sala, dia_semana ) )
                     {
                        continue;
                     }

                     int horario_aula_id = ( std::distance( aulas->begin(), it_aula ) % total_horarios );

                     // Procura o turno da aula
                     Oferta * temp = ( *( aula->ofertas.begin() ) );
                     int turno = temp->getTurnoId();

                     //-------------------------------------------------------------------------------------
                     AtendimentoTurno * atendimento_turno = NULL;
                     ITERA_GGROUP( it, ( *it_At_DiaSemana->atendimentos_turno ), AtendimentoTurno )
                     {
                        if ( it->getTurnoId() == turno )
                        {
                           atendimento_turno = ( *it );
                           break;
                        }
                     }

                     if ( atendimento_turno == NULL )
                     {
                        atendimento_turno = new AtendimentoTurno();

                        atendimento_turno->setId( turno );
                        atendimento_turno->setTurnoId( turno );
                        atendimento_turno->turno = temp->turno;

                        it_At_DiaSemana->atendimentos_turno->add( atendimento_turno );
                     }
                     //-------------------------------------------------------------------------------------

                     //-------------------------------------------------------------------------------------
                     AtendimentoHorarioAula * atendimento_horario_aula = new AtendimentoHorarioAula();

                     HorarioAula * horario_aula = problemData->horarios_aula_ordenados[ horario_aula_id ];
                     Professor * professor = solution->solucao_operacional->getProfessorMatriz( linha_professor );

                     atendimento_horario_aula->setId( horario_aula->getId() );
                     atendimento_horario_aula->setHorarioAulaId( horario_aula->getId() );
                     atendimento_horario_aula->setProfessorId( professor->getId() );
                     atendimento_horario_aula->setProfVirtual( professor->eVirtual() );
                     atendimento_horario_aula->setCreditoTeorico( aula->getCreditosTeoricos() > 0 );
                     atendimento_horario_aula->horario_aula = horario_aula;
                     atendimento_horario_aula->professor = professor;

                     GGroup< Oferta *, LessPtr< Oferta > >::iterator 
                        it_oferta = aula->ofertas.begin();

                     for (; it_oferta != aula->ofertas.end(); ++it_oferta )
                     {
                        Oferta * oferta = ( *it_oferta );
                        AtendimentoOferta * atendimento_oferta = new AtendimentoOferta();

                        atendimento_oferta->setId( oferta->getId() );
                        atendimento_oferta->disciplina = aula->getDisciplina();
                        atendimento_oferta->setDisciplinaId( aula->getDisciplina()->getId() );
                        atendimento_oferta->setTurma( aula->getTurma() );
                        atendimento_oferta->setQuantidade( aula->getQuantidade() );

                        char id_oferta_char[ 200 ];
                        sprintf( id_oferta_char, "%d", oferta->getId() );
                        std::string id_oferta_str = std::string( id_oferta_char );
                        atendimento_oferta->setOfertaCursoCampiId( id_oferta_str );
                        atendimento_oferta->oferta = oferta;

                        atendimento_horario_aula->atendimentos_ofertas->add( atendimento_oferta );                        
                     }

                     atendimento_turno->atendimentos_horarios_aula->add( atendimento_horario_aula );
                     //-------------------------------------------------------------------------------------
                  }
               }
            }
         }
      }
   }
   */
}

bool SolverMIP::aulaAlocada( Aula * aula, Campus * campus,
   Unidade * unidade, Sala * sala, int dia_semana ) 
{
   if ( aula == NULL || aula->eVirtual() )
   {
      return false;
   }

   bool aula_alocada = true;

   int id_unidade_aula = aula->getSala()->getIdUnidade();
   int id_campus_aula = problemData->refUnidade[ id_unidade_aula ]->getIdCampus();
   int id_sala_aula = aula->getSala()->getId();
   int dia_semana_aula = aula->getDiaSemana();

   if ( id_campus_aula != campus->getId() || id_unidade_aula != unidade->getId()
      || id_sala_aula != sala->getId() || dia_semana_aula != dia_semana )
   {
      aula_alocada = false;
   }

   return aula_alocada;
}

void SolverMIP::separaDisciplinasEquivalentes()
{
   // Primeiramente, devo criar as vari�veis de aloca��o de alunos ( vari�veis 'a' )
   // para as disciplinas que foram substitu�das. A aloca��o � feita utilizando o total
   // de alunos que foram alocados para a disciplina que as substituiu
   criaVariaveisAlunosDisciplinasSubstituidas();

   // Ap�s criadas as vari�veis de aloca��o de alunos, ent�o devo criar
   // as vari�veis de cr�ditos ( vari�veis 'x' )para as disciplinas substitu�das
   criaVariaveisCreditosDisciplinasSubstituidas();
}

void SolverMIP::criaVariaveisAlunosDisciplinasSubstituidas()
{
   Curso * curso = NULL;
   Curriculo * curriculo = NULL;
   Disciplina * disciplina_substituta = NULL;
   Disciplina * disciplina_equivalente = NULL;

   std::map< std::pair< Curso *, Curriculo * >,
             std::map< Disciplina *, GGroup< Disciplina *, LessPtr< Disciplina > > > >::iterator
             it_disc_substituidas = problemData->mapGroupDisciplinasSubstituidas.begin();

   // Procura criar vari�veis 'a' para as disciplinas que foram substitu�das
   for (; it_disc_substituidas != problemData->mapGroupDisciplinasSubstituidas.end();
          it_disc_substituidas++ )
   {
      curso = it_disc_substituidas->first.first;
      curriculo = it_disc_substituidas->first.second;

		// Cria vari�veis para cada uma das disciplinas substitu�das
      std::map< Disciplina *, GGroup< Disciplina *, LessPtr< Disciplina > > >::iterator
         it_conjunto_disc = it_disc_substituidas->second.begin();

      for (; it_conjunto_disc != it_disc_substituidas->second.end();
             it_conjunto_disc++ )
      {
         // Disciplina para a qual as vari�veis foram criadas
         disciplina_substituta = ( it_conjunto_disc->first );

         // Vari�veis criadas para 'disciplina_substituta' em cursos compat�veis
         // com os cursos das disciplinas que foram substitu�das por ela
         std::vector< Variable * > variaveis_alunos
            = variaveisAlunosAtendidos( curso, disciplina_substituta );

         // Total de alunos que foram atendidos
         int alunos_atendidos = 0;

         for ( int i = 0; i < (int)variaveis_alunos.size(); i++ )
         {
            Variable * v = variaveis_alunos[i];

            alunos_atendidos += (int)( v->getValue() );
         }

         // Primeiramente, devo atender toda a demanda da disciplina
         // 'disciplina_substituta', e em seguida alocar a demanda de
         // alunos das suas disciplinas equivalentes, enquanto for poss�vel
         Demanda * demanda_substituta
            = problemData->buscaDemanda( curso, disciplina_substituta );

         // Demanda da disciplina que substituiu as demais
         int alunos_disciplina_substituta = demanda_substituta->getQuantidade();

         Variable * v_disc_substituta = NULL;
         if ( alunos_disciplina_substituta <= alunos_atendidos )
         {
            // Atende a disciplina 'disciplina_substituta'
            ITERA_VECTOR( it_v_substituta, variaveis_alunos, Variable )
            {
               v_disc_substituta = ( *it_v_substituta );

               // Atende ESPECIFICAMENTE � disciplina 'disciplina_substituta'
               if ( v_disc_substituta->getOferta()->curso == curso
                  && v_disc_substituta->getOferta()->curriculo == curriculo )
               {
                  if ( v_disc_substituta->getValue() <= alunos_disciplina_substituta )
                  {
                     alunos_atendidos -= (int)( v_disc_substituta->getValue() );
                     alunos_disciplina_substituta -= (int)( v_disc_substituta->getValue() );
                  }
                  else
                  {
                     v_disc_substituta->setValue( alunos_disciplina_substituta );
                     alunos_atendidos -= alunos_disciplina_substituta;
                     break;
                  }
               }

               // J� atendi � demanda de disciplina 'disciplina_substituta'
               if ( alunos_disciplina_substituta <= 0 )
               {
                  alunos_disciplina_substituta = 0;
                  break;
               }
            }
         }
         else
         {
            // Assim, temos que s� a disciplina 'disciplina_substituta' ser�
            // atendida, n�o sendo criada nenhuma nova vari�vel para as suas
            // disciplinas equivalentes, pois as mesmas n�o foram atendidas
            continue;
         }

         // Enquanto for poss�vel, criamos vari�veis referentes ao atendimento da demanda
         // das disciplinas substitu�das. OBS.: Partimos do princ�pio qu\e a escolha de qual
         // disciplina deve ser atendida prioritariamente � INDIFERENTE para a solu��o
         ITERA_GGROUP_LESSPTR( it_disc_equi, it_conjunto_disc->second, Disciplina )
         {
            // N�o pode atender mais disciplinas
            if ( alunos_atendidos == 0 )
            {
               break;
            }

            disciplina_equivalente = ( *it_disc_equi );

            // Vari�veis criadas para 'disciplina_equivalente'
            std::vector< Variable > variaveis_alunos_equivalente
               = filtraVariaveisAlunos( mapVariaveisDisciplinasEquivalentes[ disciplina_equivalente ] );

            // Alunos atendidos da disciplina
            std::vector< Variable >::iterator it_variable
               = variaveis_alunos_equivalente.begin();
            for (; it_variable != variaveis_alunos_equivalente.end(); it_variable++ )
            {
               // 'v_temp' representa a vari�vel do modelo que estava
               // associada � disciplina 'disciplina_substituta', mas que
               // na verdade foi criada para representar a disciplina 'disciplina_equivalente'
               Variable v_temp = ( *it_variable );

               // Criando uma nova  vari�vel 'a' (alunos)
               Campus * campus_alunos = v_temp.getCampus();
               Unidade * unidade_alunos = v_temp.getUnidade();
               ConjuntoSala * cjtSala_alunos = v_temp.getSubCjtSala();
               Sala * sala_alunos = v_temp.getSala();
               int dia_alunos = v_temp.getDia();
               Oferta * oferta_alunos = v_temp.getOferta();
               Curso * curso_alunos = v_temp.getCurso();
               Disciplina * disc_alunos = v_temp.getDisciplina();
				   int turma_alunos = v_temp.getTurma();

               Variable * v = criaVariavelAlunos( campus_alunos, unidade_alunos, cjtSala_alunos, sala_alunos,
                                                  dia_alunos, oferta_alunos, curso_alunos, disc_alunos, turma_alunos );

               v->setValue( v_temp.getValue() );

					std::pair< int, Disciplina * > turma_disciplina
                  = std::make_pair( v->getTurma(), v->getDisciplina() );

					// Adiciono uma vari�vel 'a' a mais para o getSolutionTatico()
               vars_a[ turma_disciplina ].push_back( v );

               alunos_atendidos -= (int)( v_temp.getValue() );

               // Remove a vari�vel anterior da lista de vari�veis
               // a serem utilizadas no m�todo getSolutionTatico()
               for ( int i = 0; i < (int)( vars_a[ turma_disciplina ].size() ); i++ )                  
               {
                  if ( ( *vars_a[ turma_disciplina ][i] ) == v_temp )
                  {
                     vars_a[ turma_disciplina ].erase( vars_a[ turma_disciplina ].begin() + i );
                     break;
                  }
               }
            }

            // Diz que a disciplina equivalente foi atendida, ou seja,
            // posso posteriormente criar vari�vel de cr�ditos para essa disciplina
            problemData->disciplinasSubstituidasAtendidas[ disciplina_equivalente ] = true;
         }
      }
   }
}

void SolverMIP::criaVariaveisCreditosDisciplinasSubstituidas()
{
   Curso * curso = NULL;
   Curriculo * curriculo = NULL;
   Disciplina * disciplina_substituta = NULL;
   Disciplina * disciplina_equivalente = NULL;

   std::map< std::pair< Curso *, Curriculo * >,
             std::map< Disciplina *, GGroup< Disciplina *, LessPtr< Disciplina > > > >::iterator
             it_disc_substituidas = problemData->mapGroupDisciplinasSubstituidas.begin();

	// Procura criar vari�veis 'x' para as disciplinas que foram substitu�das
   for (; it_disc_substituidas != problemData->mapGroupDisciplinasSubstituidas.end();
          it_disc_substituidas++ )
   {
      curso = it_disc_substituidas->first.first;
      curriculo = it_disc_substituidas->first.second;

		// Cria vari�veis para cada uma das disciplinas substitu�das
      std::map< Disciplina *, GGroup< Disciplina *, LessPtr< Disciplina > > >::iterator
         it_conjunto_disc = it_disc_substituidas->second.begin();

      for (; it_conjunto_disc != it_disc_substituidas->second.end();
             it_conjunto_disc++ )
      {
         disciplina_substituta = ( it_conjunto_disc->first );

		 // Busca pelas ofertas da disciplina 'disciplina_substituta'
		 // GGroup< Oferta *, LessPtr< Oferta > > ofertas_disc
		 // 	= problemData->ofertasDisc[ disciplina_substituta->getId() ];

		 // Procura pelas vari�veis cr�ditos que foram
		 // criadas para atender a 'disciplina_substituta'
         std::vector< Variable * > variaveis_creditos
            = variaveisCreditosAtendidos( disciplina_substituta );

         // Enquanto for poss�vel, criamos vari�veis referentes
         // ao atendimento da demanda das disciplinas substitu�das.
         // OBS.: Partimos do princ�pio que a escolha de qual disciplina
         // deve ser atendida prioritariamente � INDIFERENTE para a solu��o
         ITERA_GGROUP_LESSPTR( it_disc_equi, it_conjunto_disc->second, Disciplina )
         {
            disciplina_equivalente = ( *it_disc_equi );

            // Verifica se a disciplina equivalente
				// teve pelo menos um aluno atendido na solu��o
            bool disciplina_atendida = false;

			std::map< Disciplina *, bool >::iterator it_find_disciplina
            = problemData->disciplinasSubstituidasAtendidas.find( disciplina_equivalente );

			if ( it_find_disciplina != problemData->disciplinasSubstituidasAtendidas.end() )
			{
				disciplina_atendida = it_find_disciplina->second;
			}

            if ( disciplina_atendida )
            {
			   // Procura pelas vari�veis cr�ditos que foram
			   // criadas para atender a 'disciplina_substituta'
               std::vector< Variable > variaveis_creditos_equivalente
                  = filtraVariaveisCreditos( mapVariaveisDisciplinasEquivalentes[ disciplina_equivalente ] );

               std::vector< Variable >::iterator it_variable
                  = variaveis_creditos_equivalente.begin();
               for (; it_variable != variaveis_creditos_equivalente.end(); it_variable++ )
               {
                  // 'v_temp' representa a vari�vel do modelo que estava
                  // associada � disciplina 'disciplina_substituta', mas que
                  // na verdade foi criada para representar a disciplina 'disciplina_equivalente'
                  Variable v_temp = ( *it_variable );

                  // Criando uma nova vari�vel 'x' (cr�ditos) para a disciplina equivalente
                  Campus * campus_creditos = v_temp.getCampus();
                  Unidade * unidade_creditos = v_temp.getUnidade();
                  ConjuntoSala * cjtSala_creditos = v_temp.getSubCjtSala();
                  Sala * sala_creditos = v_temp.getSala();
                  int dia_creditos = v_temp.getDia();
                  Oferta * oferta_creditos = v_temp.getOferta();
                  Curso * curso_creditos = v_temp.getCurso();
					   Disciplina * disc_creditos = v_temp.getDisciplina();
                  int turma_creditos = v_temp.getTurma();
                  BlocoCurricular * bloco_curricular = v_temp.getBloco();

                  Variable * v = criaVariavelCreditos( campus_creditos, unidade_creditos, cjtSala_creditos,
                                                       sala_creditos, dia_creditos, oferta_creditos, curso_creditos,
                                                       disc_creditos, turma_creditos, bloco_curricular );

                  v->setValue( v_temp.getValue() );
                  vars_x.push_back( v );

                  // Remove a vari�vel anterior da lista de vari�veis
                  // a serem utilizadas no m�todo getSolutionTatico()
                  for ( int i = 0; i < (int)( vars_x.size() ); i++ )                  
                  {
                     if ( ( *vars_x[i] ) == v_temp )
                     {
                        vars_x.erase( vars_x.begin() + i );
                        break;
                     }
                  }
               }
            }
         }
      }
   }
}

std::vector< Variable > SolverMIP::filtraVariaveisAlunos(
   std::vector< Variable > variables )
{
   std::vector< Variable > result;

   std::vector< Variable >::iterator
      it_variable = variables.begin();

   for (; it_variable != variables.end(); it_variable++ )
   {
      Variable v = ( *it_variable );

      if ( v.getType() == Variable::V_ALUNOS )
      {
         result.push_back( v );
      }
   }

   return result;
}

std::vector< Variable > SolverMIP::filtraVariaveisCreditos(
   std::vector< Variable > variables )
{
   std::vector< Variable > result;
   
   std::vector< Variable >::iterator
      it_variable = variables.begin();

   for (; it_variable != variables.end(); it_variable++ )
   {
      Variable v = ( *it_variable );

      if ( v.getType() == Variable::V_CREDITOS
            || v.getType() == Variable::V_CREDITOS_MODF )
      {
         result.push_back( v );
      }
   }

   return result;
}

// Retorna as vari�veis de alunos referentes � essa disciplina,
// em qualquer dos cursos que sejam compat�veis com o curso dessa da disciplina
std::vector< Variable * > SolverMIP::variaveisAlunosAtendidos(
   Curso * curso, Disciplina * disciplina )
{
   std::vector< Variable * > variaveis;

   vars__A___i_d_o::iterator it_a = vars_a.begin();

   for (; it_a != vars_a.end(); it_a++ )
   {
      std::vector< Variable * >::iterator it_variable = it_a->second.begin();

      for (; it_variable != it_a->second.end(); it_variable++ )
      {
         Variable * v = ( *it_variable );

         if ( abs( v->getDisciplina()->getId() ) == abs( disciplina->getId() )
            && problemData->cursosCompativeis( v->getOferta()->curso, curso ) )
         {
            variaveis.push_back( v );
         }
      }
   }

   return variaveis;
}

// Retorna as vari�veis de alunos referentes � essa disciplina
std::vector< Variable * > SolverMIP::variaveisCreditosAtendidos( Disciplina * disciplina )
{
   std::vector< Variable * > variaveis;

   for ( int i = 0; i < (int)vars_x.size(); i++ )
   {
      Variable * v = vars_x[ i ];

      if ( abs( v->getDisciplina()->getId() ) == abs( disciplina->getId() ) )
      {
         variaveis.push_back( v );
      }
   }

   return variaveis;
}

Variable * SolverMIP::criaVariavelAlunos(
   Campus * campus, Unidade * unidade, ConjuntoSala * cjtSala,
   Sala * sala, int dia_semana, Oferta * oferta, Curso * curso,
   Disciplina * disciplina, int turma )
{
   Variable * v = new Variable();

   v->reset();
   v->setType( Variable::V_ALUNOS );
   v->setCampus( campus );
   v->setUnidade( unidade );
   v->setSubCjtSala( cjtSala );
   v->setSala( sala );
   v->setDia( dia_semana );
   v->setOferta( oferta );
   v->setCurso( curso );
   v->setDisciplina( disciplina );
   v->setTurma( turma );

   return v;
}

Variable * SolverMIP::criaVariavelCreditos(
   Campus * campus, Unidade * unidade, ConjuntoSala * cjtSala,
   Sala * sala, int dia_semana, Oferta * oferta, Curso * curso,
   Disciplina * disciplina, int turma, BlocoCurricular * bloco_curricular )
{
   Variable * v = new Variable();

   v->reset();
   v->setType( Variable::V_CREDITOS_MODF );
   v->setCampus( campus );
   v->setUnidade( unidade );
   v->setSubCjtSala( cjtSala );
   v->setSala( sala );
   v->setDia( dia_semana );
   v->setOferta( oferta );
   v->setCurso( curso );
   v->setDisciplina( disciplina );
   v->setTurma( turma );
   v->setBloco( bloco_curricular );

   return v;
}

int SolverMIP::localBranching(
   double * xSol, double maxTime )
{
   // Adiciona restri��o de local branching
   int status = 0;
   int nIter = 0;
   int * idxSol = new int[ lp->getNumCols() ];

   for ( int i = 0; i < lp->getNumCols(); i++ )
   {
      idxSol[ i ] = i;
   }

   for (; nIter < 3; nIter++ )
   {
      VariableHash::iterator vit = vHash.begin();

      OPT_ROW nR( 100, OPT_ROW::GREATER, 0.0, "LOCBRANCH" );
      double rhsLB = -5000;

      while ( vit != vHash.end() )
      {
         if ( vit->first.getType() == Variable::V_OFERECIMENTO )
         {
            if ( xSol[vit->second] > 0.1 )
            {
               rhsLB += 1.0;
               nR.insert( vit->second,1.0 );
            }
            else
            {
               nR.insert( vit->second, -1.0 );
            }
         }

         vit++;
      }

      nR.setRhs( rhsLB );

      lp->addRow( nR );
      lp->updateLP();
      lp->setNodeLimit( 100000000 );
      lp->setTimeLimit( 3600 );
      lp->setMIPRelTol( 0.02 );
      lp->setNodeLimit( 1 );
      lp->setMIPEmphasis( 1 );
      lp->setHeurFrequency( 1.0 );
      lp->copyMIPStartSol( lp->getNumCols(), idxSol, xSol );

      status = lp->optimize( METHOD_MIP );

      if ( nIter == 2 )
      {
         break;
      }

      lp->getX( xSol );

      int * idxs = new int[ 1 ];
      idxs[ 0 ] = lp->getNumRows() - 1;
      lp->delSetRows( 1, idxs );
      lp->updateLP();
      delete [] idxs;
   }

   delete [] idxSol;

   return status;
}

void SolverMIP::getSolution( ProblemSolution * problem_solution )
{
   // Input TATICO
   if ( problemData->parametros->modo_otimizacao == "TATICO"
      && problemData->atendimentosTatico == NULL )
   {
	   if ( problemData->parametros->otimizarPor == "BLOCOCURRICULAR" )
	   {
			getSolutionTatico();
	   }
	   else if ( problemData->parametros->otimizarPor == "ALUNO" )
	   {
			getSolutionTaticoPorAluno();
	   }
   }
   // Input OPERACIONAL
   else if ( problemData->parametros->modo_otimizacao == "OPERACIONAL" )
   {
      if ( problemData->atendimentosTatico == NULL )
      {
		   if ( problemData->parametros->otimizarPor == "BLOCOCURRICULAR" )
		   {
				getSolutionTatico();
		   }
		   else if ( problemData->parametros->otimizarPor == "ALUNO" )
		   {
				getSolutionTaticoPorAluno();
		   }
      }

      // getSolutionOperacional();
      getSolutionOperacionalMIP();
   }

   relacionaAlunosDemandas();
}

/********************************************************************
**                      Variaveis do pre-tatico                    **
*********************************************************************/

int SolverMIP::cria_preVariaveis(  int campusId, int prioridade )
{
	int num_vars = 0;
	CPUTimer timer;
	double dif = 0.0;

#ifdef PRINT_cria_variaveis
	int numVarsAnterior = 0;
#endif

	timer.start();
	num_vars += cria_preVariavel_creditos( campusId );   // x_{i,d,s}
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_variaveis
	std::cout << "numVars \"x\": " << (num_vars - numVarsAnterior)  <<" "<<dif <<" sec" << std::endl;
	numVarsAnterior = num_vars;
#endif

	timer.start();
	num_vars += cria_preVariavel_oferecimentos( campusId ); // o_{i,d,s}
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_variaveis
	std::cout << "numVars \"o\": " << (num_vars - numVarsAnterior)  <<" "<<dif <<" sec" << std::endl;
	numVarsAnterior = num_vars;
#endif

	timer.start();
	num_vars += cria_preVariavel_abertura( campusId );   // z_{i,d}
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_variaveis
	std::cout << "numVars \"z\": " << (num_vars - numVarsAnterior)  <<" "<<dif <<" sec" << std::endl;
	numVarsAnterior = num_vars;
#endif

	timer.start();
	num_vars += cria_preVariavel_alunos( campusId );  // a_{i,d,oft,s}
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_variaveis
	std::cout << "numVars \"a\": " << (num_vars - numVarsAnterior)  <<" "<<dif <<" sec" << std::endl;
	numVarsAnterior = num_vars;
#endif

	timer.start();
	num_vars += cria_preVariavel_aloc_alunos( campusId );   // b_{i,d,c}
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_variaveis
	std::cout << "numVars \"a\": " << (num_vars - numVarsAnterior)  <<" "<<dif <<" sec" << std::endl;
	numVarsAnterior = num_vars;
#endif

	timer.start();
	num_vars += cria_preVariavel_folga_compartilhamento_incomp( campusId ); // bs_{d,oft}
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_variaveis
	std::cout << "numVars \"bs\": " << (num_vars - numVarsAnterior)  <<" "<<dif <<" sec" << std::endl;
	numVarsAnterior = num_vars;
#endif

	timer.start();
	num_vars += cria_preVariavel_folga_proibe_compartilhamento( campusId ); // fc_{d,oft}
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_variaveis
	std::cout << "numVars \"fc\": " << (num_vars - numVarsAnterior)  <<" "<<dif <<" sec" << std::endl;
	numVarsAnterior = num_vars;
#endif

	timer.start();
	num_vars += cria_preVariavel_folga_turma_mesma_disc_sala_dif( campusId ); // fs_{d,s}
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_variaveis
	std::cout << "numVars \"fs\": " << (num_vars - numVarsAnterior)  <<" "<<dif <<" sec" << std::endl;
	numVarsAnterior = num_vars;
#endif

	timer.start();
	num_vars += cria_preVariavel_limite_sup_creds_sala( campusId ); // Hs
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_variaveis
	std::cout << "numVars \"Hs\": " << (num_vars - numVarsAnterior)  <<" "<<dif <<" sec" << std::endl;
	numVarsAnterior = num_vars;
#endif

	timer.start();
	num_vars += cria_preVariavel_aloca_alunos_oferta( campusId ); // c
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_variaveis
	std::cout << "numVars \"c\": " << (num_vars - numVarsAnterior)  <<" "<<dif <<" sec" << std::endl;
	numVarsAnterior = num_vars;
#endif	


   if ( problemData->parametros->otimizarPor == "BLOCOCURRICULAR" )
   {
		timer.start();
		num_vars += cria_preVariavel_folga_demanda_disciplina_oft( campusId ); // fd_{d,oft}
		timer.stop();
		dif = timer.getCronoCurrSecs();

	#ifdef PRINT_cria_variaveis
		std::cout << "numVars \"fd\": " << (num_vars - numVarsAnterior)  <<" "<<dif <<" sec" << std::endl;
		numVarsAnterior = num_vars;
	#endif
   }

   else if ( problemData->parametros->otimizarPor == "ALUNO" )
   {
		timer.start();
		num_vars += cria_preVariavel_folga_demanda_disciplina_aluno( campusId ); // fd_{d,a}
		timer.stop();
		dif = timer.getCronoCurrSecs();

	#ifdef PRINT_cria_variaveis
		std::cout << "numVars \"fd\": " << (num_vars - numVarsAnterior)  <<" "<<dif <<" sec" << std::endl;
		numVarsAnterior = num_vars;
	#endif

		timer.start();
		num_vars += cria_preVariavel_aloca_aluno_turma_disc( campusId ); // s
		timer.stop();
		dif = timer.getCronoCurrSecs();

	#ifdef PRINT_cria_variaveis
		std::cout << "numVars \"s\": " << (num_vars - numVarsAnterior)  <<" "<<dif <<" sec" << std::endl;
		numVarsAnterior = num_vars;
	#endif

		timer.start();
		num_vars += cria_preVariavel_folga_prioridade_inf( campusId, prioridade ); // fpi
		timer.stop();
		dif = timer.getCronoCurrSecs();

	#ifdef PRINT_cria_variaveis
		std::cout << "numVars \"fpi\": " << (num_vars - numVarsAnterior)  <<" "<<dif <<" sec" << std::endl;
		numVarsAnterior = num_vars;
	#endif
		

		timer.start();
		num_vars += cria_preVariavel_folga_prioridade_sup( campusId, prioridade ); // fps
		timer.stop();
		dif = timer.getCronoCurrSecs();

	#ifdef PRINT_cria_variaveis
		std::cout << "numVars \"fps\": " << (num_vars - numVarsAnterior)  <<" "<<dif <<" sec" << std::endl;
		numVarsAnterior = num_vars;
	#endif
		

   }	

	return num_vars;
}

// x_{i,d,u,s}
int SolverMIP::cria_preVariavel_creditos( int campusId )
{
	int num_vars = 0;

   Disciplina * disciplina = NULL;

   ITERA_GGROUP_LESSPTR( itCampus, problemData->campi, Campus )
   {
	  Campus *cp = *itCampus;

	  if ( cp->getId() != campusId )
	  {
		  continue;
	  }

      ITERA_GGROUP_LESSPTR( itUnidade, cp->unidades, Unidade )
      {
         ITERA_GGROUP_LESSPTR( itCjtSala, itUnidade->conjutoSalas, ConjuntoSala )
         {
            ITERA_GGROUP_LESSPTR( it_disc, itCjtSala->disciplinas_associadas, Disciplina )
            {
               disciplina = ( *it_disc );

			   #pragma region Equivalencias
			   if ( problemData->mapDiscSubstituidaPor.find(disciplina) !=
				    problemData->mapDiscSubstituidaPor.end() )
			   {
				   continue;
			   }
			   #pragma endregion

               for ( int turma = 0; turma < disciplina->getNumTurmas(); turma++ )
               {
                     VariablePre v;
                     v.reset();
                     v.setType( VariablePre::V_PRE_CREDITOS );

                     v.setTurma( turma );            // i
                     v.setDisciplina( disciplina );  // d
                     v.setUnidade( *itUnidade );     // u
                     v.setSubCjtSala( *itCjtSala );  // tps

                     if ( vHashPre.find( v ) == vHashPre.end() )
                     {
						 double coef = 0.0;

						 if ( problemData->parametros->funcao_objetivo == 0 )
                         {							 
							 coef = -1.0;
						 }
						 else if ( problemData->parametros->funcao_objetivo == 1 )
                         {
							 coef = 1.0;
						 }
						 
						 int upperBound = disciplina->getCredTeoricos() + disciplina->getCredPraticos();
						 
						 double lowerBound = this->fixaLimiteInferiorVariavelPre( &v ) - 0.001;

                         vHashPre[ v ] = lp->getNumCols();
						   						 
                         OPT_COL col( OPT_COL::VAR_INTEGRAL, coef, lowerBound, upperBound, ( char * )v.toString().c_str() );
                          
						 lp->newCol( col );						 

						 num_vars++;						  
                     }
                }
            }
         }
      }
   }

	return num_vars;
}

// o_{i,d,s}
int SolverMIP::cria_preVariavel_oferecimentos( int campusId )
{
	int num_vars = 0;

   Disciplina * disciplina = NULL;

   Curso * curso = NULL;
   Curriculo * curriculo = NULL;

   ITERA_GGROUP_LESSPTR( itCampus, problemData->campi, Campus )
   {
	  Campus *cp = *itCampus;

	  if ( cp->getId() != campusId )
	  {
		  continue;
	  }

      ITERA_GGROUP_LESSPTR( itUnidade, cp->unidades, Unidade )
      {
         ITERA_GGROUP_LESSPTR( itCjtSala, itUnidade->conjutoSalas, ConjuntoSala )
         {
            ITERA_GGROUP_LESSPTR( it_disc, itCjtSala->disciplinas_associadas, Disciplina )
            {
               disciplina = ( *it_disc );

			   #pragma region Equivalencias
			   if ( problemData->mapDiscSubstituidaPor.find(disciplina) !=
				    problemData->mapDiscSubstituidaPor.end() )
			   {
				   continue;
			   }
			   #pragma endregion

               for ( int turma = 0; turma < disciplina->getNumTurmas(); turma++ )
               {
                    // Dada uma disciplina, recupera os seus dias letivos, observando as fixa��es
                    GGroup< int > dias_letivos = itCjtSala->dias_letivos_disciplinas[ ( disciplina ) ];

                    GGroup< int >::iterator itDiscSala_Dias = dias_letivos.begin();

                     VariablePre v;
                     v.reset();
                     v.setType( VariablePre::V_PRE_OFERECIMENTO );

                     v.setTurma( turma );            // i
                     v.setDisciplina( disciplina );  // d
                     v.setUnidade( *itUnidade );     // u
                     v.setSubCjtSala( *itCjtSala );  // tps

                     if ( vHashPre.find( v ) == vHashPre.end() )
                     {
                        double custo = 0.0;

                        vHashPre[ v ] = lp->getNumCols();
						 
						double lowerBound = this->fixaLimiteInferiorVariavelPre( &v ) - 0.001;

                        OPT_COL col( OPT_COL::VAR_BINARY, custo, lowerBound, 1.0, ( char* )v.toString().c_str() );
                          
						lp->newCol( col );

						num_vars++;						  
                     }
                }
            }
         }
      }
   }

	return num_vars;
}

// z_{i,d,cp}
int SolverMIP::cria_preVariavel_abertura( int campusId )
{
	int num_vars = 0;
	
   Curso * curso = NULL;
   Curriculo * curriculo = NULL;

   std::map< int /*Id Campus*/, GGroup< int > /*Id Discs*/ >::iterator it_CpDisc = problemData->cp_discs.begin();

   for ( ; it_CpDisc != problemData->cp_discs.end(); it_CpDisc++ )
   {
	  Campus *cp = problemData->refCampus[ it_CpDisc->first ];

	  if ( cp->getId() != campusId )
	  {
		  continue;
	  }

      ITERA_GGROUP_N_PT( it_disciplina, it_CpDisc->second, int )
      {
		 Disciplina *disciplina = problemData->refDisciplinas[ *it_disciplina ];
		 
		 #pragma region Equivalencias
		 if ( problemData->mapDiscSubstituidaPor.find(disciplina) !=
			 problemData->mapDiscSubstituidaPor.end() )
		 {
		 	continue;
		 }
		 #pragma endregion

         for ( int turma = 0; turma < disciplina->getNumTurmas(); turma++ )
         {
            VariablePre v;
            v.reset();
            v.setType( VariablePre::V_PRE_ABERTURA );

            v.setTurma( turma );            // i
            v.setDisciplina( disciplina );  // d
            v.setCampus( cp );	    // cp

            std::pair< int, int > dc
               = std::make_pair( disciplina->getId(), cp->getId() );

            if ( problemData->demandas_campus.find( dc )
               == problemData->demandas_campus.end() )
            {
               problemData->demandas_campus[ dc ] = 0;
            }

			/*
            double ratioDem = ( disciplina->getDemandaTotal() - 
               problemData->demandas_campus[ dc ] ) 
               / (1.0 * disciplina->getDemandaTotal() );

            double coeff = alpha + gamma * ratioDem;

            int numCreditos = ( disciplina->getCredTeoricos() + disciplina->getCredPraticos() );
            double valorCredito = 1600.0;
            double coef_FO_1_2 = ( numCreditos * valorCredito );
			*/
            if ( vHashPre.find(v) == vHashPre.end() )
            {
               lp->getNumCols();
               vHashPre[v] = lp->getNumCols();

			   /*
               if ( problemData->parametros->funcao_objetivo == 0 )
               {
                  OPT_COL col( OPT_COL::VAR_BINARY, coeff, 0.0, 1.0,
                     ( char * )v.toString().c_str() );

                  lp->newCol( col );
               }
               else if ( problemData->parametros->funcao_objetivo == 1 )
               {
                  OPT_COL col( OPT_COL::VAR_BINARY, -coef_FO_1_2, 0.0, 1.0,
                     ( char * )v.toString().c_str() );

                  lp->newCol( col );
               }
               else if ( problemData->parametros->funcao_objetivo == 2 )
               {
                  OPT_COL col( OPT_COL::VAR_BINARY, coef_FO_1_2, 0.0, 1.0,
                     ( char * )v.toString().c_str() );

                  lp->newCol( col );
               }*/

			    double coef = 0.0;

				if ( problemData->parametros->funcao_objetivo == 0 )
				{
					double custo = cp->getCusto();
							 
					coef = -10 * custo * disciplina->getTotalCreditos();
				}
				else if ( problemData->parametros->funcao_objetivo == 1 )
				{
					double custo = cp->getCusto();

					coef = custo * disciplina->getTotalCreditos();
				}
						                
				double lowerBound = this->fixaLimiteInferiorVariavelPre( &v ) - 0.001;

				OPT_COL col( OPT_COL::VAR_BINARY, coef, lowerBound, 1.0,
                     ( char * )v.toString().c_str() );

                lp->newCol( col ); 
                
				num_vars++;
            }
         }
      }
   }

	return num_vars;
}

// a_{i,d,oft,s}
int SolverMIP::cria_preVariavel_alunos( int campusId )
{
	int num_vars = 0;
	
    Curso * curso = NULL;
    Curriculo * curriculo = NULL;
	
	ITERA_GGROUP_LESSPTR( itCampus, problemData->campi, Campus )
	{
		Campus* cp = *itCampus;
	  
	    if ( cp->getId() != campusId )
	    {
		    continue;
	    }

		ITERA_GGROUP_LESSPTR( itUnidade, cp->unidades, Unidade )
		{
			ITERA_GGROUP_LESSPTR( itCjtSala, itUnidade->conjutoSalas, ConjuntoSala )
			{
				ITERA_GGROUP_LESSPTR( it_disc, itCjtSala->disciplinas_associadas, Disciplina )
				{
					Disciplina* disciplina = *it_disc;

				    #pragma region Equivalencias
				    if ( problemData->mapDiscSubstituidaPor.find(disciplina) !=
					 	problemData->mapDiscSubstituidaPor.end() )
				    {
					    continue;
				    }
				    #pragma endregion

					// Listando todas as ofertas que cont�m uma disciplina especificada.
					GGroup< Oferta *, LessPtr< Oferta > > ofertas = problemData->ofertasDisc[ it_disc->getId() ];

					ITERA_GGROUP_LESSPTR( itOferta, ofertas, Oferta )
					{
							 Oferta* oft = *itOferta;

							 if ( oft->campus != cp )
								 continue;

							 // Calculando P_{d,o}
							 int qtdDem = 0;
							 ITERA_GGROUP_LESSPTR( itDem, problemData->demandas, Demanda )
							 {
								if ( itDem->disciplina->getId() == disciplina->getId() &&
								   itDem->getOfertaId() == itOferta->getId() )
								{
								   qtdDem += itDem->getQuantidade();
								}
							 }

							 if ( qtdDem <= 0 )
							 {
								continue;
							 }

							 for ( int turma = 0; turma < disciplina->getNumTurmas(); turma++ )
							 {
								VariablePre v;
								v.reset();
								v.setType( VariablePre::V_PRE_ALUNOS );

								v.setTurma( turma );               // i
								v.setDisciplina( disciplina );     // d
								v.setOferta( *itOferta );          // oft
								v.setUnidade( *itUnidade );		   // u
								v.setSubCjtSala( *itCjtSala );	   // s
								
								if ( vHashPre.find(v) == vHashPre.end() )
								{
									vHashPre[v] = lp->getNumCols();
									
									double coef = 0.0;

									if ( problemData->parametros->funcao_objetivo == 0 )
									{
										double valorCredito = itOferta->getReceita();
										int numCreditos = ( disciplina->getCredTeoricos() + disciplina->getCredPraticos() );
				
										coef = 100 * numCreditos * valorCredito;
									}
									else if ( problemData->parametros->funcao_objetivo == 1 )
									{
										coef = 0.0;
									}

									double lowerBound = this->fixaLimiteInferiorVariavelPre( &v ) - 0.001;

									OPT_COL col( OPT_COL::VAR_INTEGRAL, coef, lowerBound, qtdDem,
										( char * )v.toString().c_str() );

									lp->newCol( col );

									num_vars += 1;
								}
							}
				      }
				}
			}
		}
   }

	return num_vars;
}

// b_{i,d,c}
int SolverMIP::cria_preVariavel_aloc_alunos( int campusId )
{
	int num_vars = 0;
		
	ITERA_GGROUP_LESSPTR( itDisc, problemData->disciplinas, Disciplina )
	{		
		Disciplina* disciplina = *itDisc;

		#pragma region Equivalencias
		if ( problemData->mapDiscSubstituidaPor.find(disciplina) !=
			problemData->mapDiscSubstituidaPor.end() )
		{
			continue;
		}
		#pragma endregion

		if ( problemData->cp_discs[campusId].find( disciplina->getId() ) ==
			 problemData->cp_discs[campusId].end() )
		{
			continue;
		}

		ITERA_GGROUP_LESSPTR( itCurso, problemData->cursos, Curso )
		{
			Curso *curso = *itCurso;

			if ( !curso->possuiDisciplina( disciplina ) )
				continue;

			// Calculando P_{d,o}
			int qtdDem = 0;
			ITERA_GGROUP_LESSPTR( itDem, problemData->demandas, Demanda )
			{
				if ( itDem->disciplina->getId() == disciplina->getId() &&
					 itDem->oferta->getCursoId() == curso->getId() &&
					 itDem->oferta->getCampusId() == campusId )
				{
					qtdDem += itDem->getQuantidade();
				}
			}

			if ( qtdDem <= 0 )
			{
				continue;
			}

			for ( int turma = 0; turma < disciplina->getNumTurmas(); turma++ )
			{
				VariablePre v;
				v.reset();
				v.setType( VariablePre::V_PRE_ALOC_ALUNO );

				v.setTurma( turma );               // i
				v.setDisciplina( disciplina );     // d
				v.setCurso( curso );			   // c
				
				if ( vHashPre.find(v) == vHashPre.end() )
				{
					double coef = 0.0;
					
					vHashPre[v] = lp->getNumCols();

					if ( problemData->parametros->funcao_objetivo == 0 )
					{
						coef = -1.0;
					}
					else if ( problemData->parametros->funcao_objetivo == 1 )
					{						
						coef = 1.0;
					}
													
					double lowerBound = this->fixaLimiteInferiorVariavelPre( &v ) - 0.001;
					
					OPT_COL col( OPT_COL::VAR_BINARY, coef, lowerBound, 1.0, ( char * )v.toString().c_str() );
												
					lp->newCol( col );
					
					num_vars += 1;
				}
			}
		}
   }

	return num_vars;
}

// fd_{d,a}
int SolverMIP::cria_preVariavel_folga_demanda_disciplina_aluno( int campusId )
{
   int num_vars = 0;

	Campus *cp = problemData->refCampus[campusId];

	ITERA_GGROUP_LESSPTR( itAluno, problemData->alunos, Aluno )
	{
		Aluno *aluno = *itAluno;

		if ( aluno->getOferta()->getCampusId() != campusId )
		{
			continue;
		}

		ITERA_GGROUP_LESSPTR( itAlDemanda, aluno->demandas, AlunoDemanda )
		{
			Disciplina *disciplina = (*itAlDemanda)->demanda->disciplina;

			VariablePre v;
			v.reset();
			v.setType( VariablePre::V_PRE_SLACK_DEMANDA );
			v.setAluno( aluno );
			v.setDisciplina( disciplina );

			// Coeficiente na funcao objetivo
			double coef = 0.0;
			if ( itAlDemanda->getPrioridade() > 1 )
			{
				coef = 0.0;
			}
			else
			{
				if ( problemData->parametros->funcao_objetivo == 0 )
				{
					coef = 0.0;
				}
				else if( problemData->parametros->funcao_objetivo == 1 )
				{
					coef = 10.0 * cp->getCusto() * disciplina->getTotalCreditos();
				}
			}

			// Limite superior da variavel
			double ub = 1.0;

			GGroup< Trio< int /*campusId*/, int /*turma*/, Disciplina* > > atendimentosAluno =
					problemData->mapAluno_CampusTurmaDisc[aluno];

			for ( int turma = 0; turma < disciplina->getNumTurmas(); turma++ )
			{
				Trio< int, int, Disciplina* > trio;
				trio.set( campusId, turma, disciplina);

				if ( atendimentosAluno.find( trio ) != atendimentosAluno.end() )
					ub = 0.0;
			}

			// Limite inferior da variavel
			double lowerBound = this->fixaLimiteInferiorVariavelPre( &v ) - 0.001;

			if ( vHashPre.find( v ) == vHashPre.end() )
			{
				vHashPre[v] = lp->getNumCols();

				OPT_COL col( OPT_COL::VAR_BINARY, coef, lowerBound, ub, ( char * )v.toString().c_str() );
				lp->newCol(col);
				
				num_vars++;
			}
		}
	}

	return num_vars;
}


/*====================================================================/
%DocBegin TRIEDA_LOAD_MODEL

%Var fd_{d,oft} 

%Desc 
vari�vel de folga para a restri��o "Capacidade alocada tem que 
permitir atender demanda da disciplina".

%ObjCoef
\omega \cdot \sum\limits_{oft \in O} \sum\limits_{d \in D_{oft}} fd_{d,oft}

%DocEnd
/====================================================================*/

int SolverMIP::cria_preVariavel_folga_demanda_disciplina_oft( int campusId )
{
   int num_vars = 0;

   Disciplina * disciplina = NULL;
   
   ITERA_GGROUP_LESSPTR( itOferta, problemData->ofertas, Oferta )
   {
	   if ( itOferta->getCampusId() != campusId )
	   {
		   continue;
	   }

      map < Disciplina*, int, LessPtr< Disciplina > >::iterator itPrdDisc = 
         itOferta->curriculo->disciplinas_periodo.begin();

      for (; itPrdDisc != itOferta->curriculo->disciplinas_periodo.end(); itPrdDisc++ )
      {
         // Calculando P_{d,o}
         int qtdDem = 0;

		 disciplina = itPrdDisc->first;

         ITERA_GGROUP_LESSPTR( itDem, problemData->demandas, Demanda )
         {
            if ( itDem->disciplina->getId() == disciplina->getId() &&
                 itDem->getOfertaId() == itOferta->getId() )
            {
               qtdDem += itDem->getQuantidade();
            }
         }

         if ( qtdDem <= 0 )
         {
            continue;
         }

         VariablePre v;
         v.reset();
         v.setType( VariablePre::V_PRE_SLACK_DEMANDA );

         v.setDisciplina( disciplina ); // d
         v.setOferta( *itOferta );      // o

         if ( vHashPre.find( v ) == vHashPre.end() )
         {
            vHashPre[ v ] = lp->getNumCols();

            double ub = qtdDem;

			double coef = 0.0;

			if ( problemData->parametros->funcao_objetivo == 0 )
			{
				coef = 0.0;
			}
			else if ( problemData->parametros->funcao_objetivo == 1 )
			{
				coef = 10 * itOferta->campus->getCusto() * disciplina->getTotalCreditos();
			}

			OPT_COL col( OPT_COL::VAR_INTEGRAL, coef, 0.0, ub, ( char * )v.toString().c_str() );

            lp->newCol( col );

            num_vars++;
         }
      }
   }

   return num_vars;
}



// bs_{i,d,c1,c2}
int SolverMIP::cria_preVariavel_folga_compartilhamento_incomp( int campusId )
{
	int num_vars = 0;

   ITERA_GGROUP_LESSPTR( itCampus, problemData->campi, Campus )
   {
		Campus* cp = *itCampus;
	  
	    if ( cp->getId() != campusId )
	    {
		    continue;
	    }

	   ITERA_GGROUP_LESSPTR( itCurso1, cp->cursos, Curso )
	   {
		   Curso* c1 = *itCurso1;
		   
		   ITERA_GGROUP_INIC_LESSPTR( itCurso2, itCurso1, cp->cursos, Curso )
		   {
				Curso* c2 = *itCurso2;
			    
			    if ( problemData->cursosCompativeis(c1, c2) || c1 == c2 )
				    continue;

				ITERA_GGROUP_LESSPTR( itDisc, problemData->disciplinas, Disciplina )
				{
					Disciplina *disciplina = *itDisc;

					#pragma region Equivalencias
					if ( problemData->mapDiscSubstituidaPor.find( disciplina ) !=
						problemData->mapDiscSubstituidaPor.end() )
					{
						continue;
					}
					#pragma endregion

					// A disciplina deve ser ofertada no campus especificado
					if ( problemData->cp_discs[campusId].find( disciplina->getId() ) ==
						 problemData->cp_discs[campusId].end() )
					{
						continue;
					}

					// A disciplina deve pertencer aos dois cursos c1 e c2
					if ( !c1->possuiDisciplina(disciplina) || !c2->possuiDisciplina(disciplina) )
						continue;

					for ( int turma = 0; turma < disciplina->getNumTurmas(); turma++ )
					{
					   VariablePre v;
					   v.reset();
					   v.setType( VariablePre::V_PRE_SLACK_ALOC_ALUNOS_CURSO_INCOMPAT );
					   v.setTurma( turma );							// i
					   v.setDisciplina( disciplina );   			// d
					   v.setParCursos( std::make_pair(c1, c2) );	// c1 c2

					   if ( vHashPre.find( v ) == vHashPre.end() )
					   {
						  vHashPre[v] = lp->getNumCols();
						  
						  double coef = 0.0;

						  if ( problemData->parametros->funcao_objetivo == 0 )
						  {
							  coef = -cp->getCusto();
						  }
						  else if( problemData->parametros->funcao_objetivo == 1 )
						  {
							  coef = cp->getCusto();
						  }
						  
						  double lowerBound = this->fixaLimiteInferiorVariavelPre( &v ) - 0.001;

						  OPT_COL col( OPT_COL::VAR_BINARY, coef, lowerBound, 1.0, ( char * )v.toString().c_str() );

						  lp->newCol( col );
						  num_vars++;
					   }
					}
				}
		    }
		}
   }

	return num_vars;
}


// fc_{i,d,c1,c2}
int SolverMIP::cria_preVariavel_folga_proibe_compartilhamento( int campusId )
{
	int num_vars = 0;

   if ( problemData->parametros->permite_compartilhamento_turma_sel )
   {
		return num_vars;
   }

   ITERA_GGROUP_LESSPTR( itCampus, problemData->campi, Campus )
   {
	   Campus* cp = *itCampus;
	  
	   if ( cp->getId() != campusId )
	   {
		   continue;
	   }

	   ITERA_GGROUP_LESSPTR( itCurso1, cp->cursos, Curso )
	   {
		   Curso* c1 = *itCurso1;
		   
		   ITERA_GGROUP_INIC_LESSPTR( itCurso2, itCurso1, cp->cursos, Curso )
		   {
				Curso* c2 = *itCurso2;
			    
				// A variavel de folga s� � criada para cursos compativeis e diferentes entre si
				// Ofertas para mesmo curso sempre poder�o compartilhar
				// Ofertas de cursos distintos s� poder�o compartilhar se forem compativeis e o compartilhamento estiver permitido
			    if ( c1 == c2 || !problemData->cursosCompativeis(c1, c2) )
				    continue;

				ITERA_GGROUP_LESSPTR( itDisc, problemData->disciplinas, Disciplina )
				{
					Disciplina *disciplina = *itDisc;

					#pragma region Equivalencias
					if ( problemData->mapDiscSubstituidaPor.find( disciplina ) !=
						problemData->mapDiscSubstituidaPor.end() )
					{
						continue;
					}
					#pragma endregion
					
					// A disciplina deve ser ofertada no campus especificado
					if ( problemData->cp_discs[campusId].find( disciplina->getId() ) ==
						 problemData->cp_discs[campusId].end() )
					{
						continue;
					}

					// A disciplina deve pertencer aos dois cursos c1 e c2
					if ( !c1->possuiDisciplina(disciplina) || !c2->possuiDisciplina(disciplina) )
						continue;

					for ( int turma = 0; turma < disciplina->getNumTurmas(); turma++ )
					{
					   VariablePre v;
					   v.reset();
					   v.setType( VariablePre::V_PRE_SLACK_COMPARTILHAMENTO );
					   v.setTurma( turma );							// i
					   v.setDisciplina( disciplina );   			// d
					   v.setParCursos( std::make_pair(c1, c2) );	// c1 c2

					   if ( vHashPre.find( v ) == vHashPre.end() )
					   {
						  vHashPre[v] = lp->getNumCols();
						  
						  double coef = 0.0;

						  if ( problemData->parametros->funcao_objetivo == 0 )
						  {
							  coef = -cp->getCusto()/4;
						  }
						  else if( problemData->parametros->funcao_objetivo == 1 )
						  {
							  coef = cp->getCusto()/4;
						  }

						  double lowerBound = this->fixaLimiteInferiorVariavelPre( &v ) - 0.001;
						  
						  OPT_COL col( OPT_COL::VAR_BINARY, coef, lowerBound, 1.0, ( char * )v.toString().c_str() );

						  lp->newCol( col );
						  num_vars++;
					   }
					}
				}
		    }
		}
   }

	return num_vars;
}


// fs_{d,s,oft}
int SolverMIP::cria_preVariavel_folga_turma_mesma_disc_sala_dif( int campusId )
{
	int num_vars = 0;

    Disciplina * disciplina_equivalente = NULL;

    Curso * curso = NULL;
    Curriculo * curriculo = NULL;

	ITERA_GGROUP_LESSPTR( itCampus, problemData->campi, Campus )
	{
		Campus* cp = *itCampus;

		if ( cp->getId() != campusId )
	    {
		    continue;
	    }
		
		ITERA_GGROUP_LESSPTR( itUnidade, cp->unidades, Unidade )
		{
			ITERA_GGROUP_LESSPTR( itCjtSala, itUnidade->conjutoSalas, ConjuntoSala )
			{
				ITERA_GGROUP_LESSPTR( itDisc, itCjtSala->disciplinas_associadas, Disciplina )
				{
					Disciplina* disciplina = *itDisc;

					#pragma region Equivalencias
					if ( problemData->mapDiscSubstituidaPor.find( disciplina ) !=
						problemData->mapDiscSubstituidaPor.end() )
					{
						continue;
					}
					#pragma endregion

					// Listando todas as ofertas que contem uma disciplina especificada.
					GGroup< Oferta *, LessPtr< Oferta > > ofertas = problemData->ofertasDisc[ itDisc->getId() ];

					ITERA_GGROUP_LESSPTR( itOferta, ofertas, Oferta )
					{
						Oferta* oft = *itOferta;

						if ( oft->campus != cp )
							continue;

						// Calculando P_{d,o}
						int qtdDem = 0;
						ITERA_GGROUP_LESSPTR( itDem, problemData->demandas, Demanda )
						{
							if ( itDem->disciplina->getId() == disciplina->getId() &&
								 itDem->getOfertaId() == oft->getId() )
							{
								qtdDem += itDem->getQuantidade();
							}
						}

						if ( qtdDem <= 0 )
						{
							continue;
						}

						if ( itDisc->getNumTurmas() > 1 )
						{
							VariablePre v;
							v.reset();
							v.setType( VariablePre::V_PRE_SLACK_SALA);
							v.setDisciplina( *itDisc );
							v.setUnidade( *itUnidade );
							v.setSubCjtSala( *itCjtSala );
							v.setOferta( oft );

							if ( vHashPre.find( v ) == vHashPre.end() )
							{
								vHashPre[v] = lp->getNumCols();

								double coef = 0.0;

								if ( problemData->parametros->funcao_objetivo == 0 )
								{
									coef = -cp->getCusto()/2;
								}
								else if( problemData->parametros->funcao_objetivo == 1 )
								{
									coef = cp->getCusto()/2;
								}
						  
								double upperbound = itDisc->getNumTurmas();
								double lowerBound = this->fixaLimiteInferiorVariavelPre( &v ) - 0.001;

								OPT_COL col( OPT_COL::VAR_INTEGRAL, coef, lowerBound, upperbound, ( char * )v.toString().c_str() );

								lp->newCol( col );
								num_vars++;
							}
						}
					}
				}
			}
		}
	}
	return num_vars;
}


// Hs_{cp}
int SolverMIP::cria_preVariavel_limite_sup_creds_sala( int campusId )
{
	int num_vars = 0;

	ITERA_GGROUP_LESSPTR( itCampus, problemData->campi, Campus )
	{
		Campus *cp = *itCampus;

	    if ( cp->getId() != campusId )
	    {
		   continue;
	    }

		VariablePre v;
		v.reset();
		v.setType( VariablePre::V_PRE_LIM_SUP_CREDS_SALA);
		v.setCampus( cp );

		if ( vHashPre.find( v ) == vHashPre.end() )
		{
			vHashPre[v] = lp->getNumCols();

			double upperbound = 0;

			ITERA_GGROUP_N_PT( itDisc, problemData->cp_discs[ cp->getId() ], int )
			{
				Disciplina* disciplina = problemData->refDisciplinas[ *itDisc ];
			
				#pragma region Equivalencias
				if ( problemData->mapDiscSubstituidaPor.find( disciplina ) !=
					problemData->mapDiscSubstituidaPor.end() )
				{
					continue;
				}
				#pragma endregion

				int nCreds = disciplina->getCredTeoricos() + disciplina->getCredPraticos();

				upperbound += nCreds * disciplina->getNumTurmas();				
			}

			double coef = 0.0;

			if ( problemData->parametros->funcao_objetivo == 0 )
			{
				coef = -cp->getCusto()/2;
			}
			else if( problemData->parametros->funcao_objetivo == 1 )
			{
				coef = cp->getCusto()/2;
			}
			
			double lowerBound = this->fixaLimiteInferiorVariavelPre( &v ) - 0.001;

			OPT_COL col( OPT_COL::VAR_INTEGRAL, coef, lowerBound, upperbound, ( char * )v.toString().c_str() );

			lp->newCol( col );
			num_vars++;
		}
	}

	return num_vars;
}


// c_{i,d,s,oft}
int SolverMIP::cria_preVariavel_aloca_alunos_oferta( int campusId )
{
	int num_vars = 0;
	
	ITERA_GGROUP_LESSPTR( itCampus, problemData->campi, Campus )
	{
		Campus* cp = *itCampus;
	    
		if ( cp->getId() != campusId )
	    {
		   continue;
	    }

		ITERA_GGROUP_LESSPTR( itUnidade, cp->unidades, Unidade )
		{
			ITERA_GGROUP_LESSPTR( itCjtSala, itUnidade->conjutoSalas, ConjuntoSala )
			{
				ITERA_GGROUP_LESSPTR( it_disc, itCjtSala->disciplinas_associadas, Disciplina )
				{
					Disciplina* disciplina = *it_disc;
					
					#pragma region Equivalencias
					if ( problemData->mapDiscSubstituidaPor.find( disciplina ) !=
						problemData->mapDiscSubstituidaPor.end() )
					{
						continue;
					}
					#pragma endregion

					// Listando todas as ofertas que contem uma disciplina especificada.
					GGroup< Oferta *, LessPtr< Oferta > > ofertas = problemData->ofertasDisc[ disciplina->getId() ];

					ITERA_GGROUP_LESSPTR( itOferta, ofertas, Oferta )
					{
							 Oferta* oft = *itOferta;

							 if ( oft->campus != cp )
								 continue;

							 // Calculando P_{d,o}
							 int qtdDem = 0;
							 ITERA_GGROUP_LESSPTR( itDem, problemData->demandas, Demanda )
							 {
								if ( itDem->disciplina->getId() == disciplina->getId() &&
								   itDem->getOfertaId() == itOferta->getId() )
								{
								   qtdDem += itDem->getQuantidade();
								}
							 }

							 if ( qtdDem <= 0 )
							 {
								continue;
							 }

							 for ( int turma = 0; turma < disciplina->getNumTurmas(); turma++ )
							 {
								VariablePre v;
								v.reset();
								v.setType( VariablePre::V_PRE_ALOC_ALUNO_OFT );

								v.setTurma( turma );               // i
								v.setDisciplina( disciplina );     // d
								v.setOferta( *itOferta );          // oft
								v.setUnidade( *itUnidade );		   // u
								v.setSubCjtSala( *itCjtSala );	   // s
								
								if ( vHashPre.find(v) == vHashPre.end() )
								{
									vHashPre[v] = lp->getNumCols();
									
									double coef = 0.0;
									if ( problemData->parametros->funcao_objetivo == 0 )
									{
										coef = -1.0;
									}
									else if ( problemData->parametros->funcao_objetivo == 1 )
									{
										coef = 1.0;
									}
									
									double lowerBound = this->fixaLimiteInferiorVariavelPre( &v ) - 0.001;
									
									OPT_COL col( OPT_COL::VAR_BINARY, coef, lowerBound, 1.0,
										( char * )v.toString().c_str() );

									lp->newCol( col );

									num_vars += 1;
								}
							}
				     }
				}
			}
		}
   }

	return num_vars;
}

// s_{i,d,a,cp}
int SolverMIP::cria_preVariavel_aloca_aluno_turma_disc( int campusId )
{
	int num_vars = 0;

	Campus *cp = problemData->refCampus[campusId];

	ITERA_GGROUP_LESSPTR( itAluno, problemData->alunos, Aluno )
	{
		Aluno *aluno = *itAluno;

		if ( aluno->getOferta()->getCampusId() != campusId )
		{
			continue;
		}

		ITERA_GGROUP_LESSPTR( itAlDemanda, aluno->demandas, AlunoDemanda )
		{
			Disciplina *disciplina = (*itAlDemanda)->demanda->disciplina;

			for ( int turma = 0; turma < disciplina->getNumTurmas(); turma++ )
			{
				VariablePre v;
				v.reset();
				v.setType( VariablePre::V_PRE_ALOCA_ALUNO_TURMA_DISC );
				v.setAluno( aluno );
				v.setDisciplina( disciplina );
				v.setTurma( turma );
				v.setCampus( cp );

				if ( vHashPre.find( v ) == vHashPre.end() )
				{
					vHashPre[v] = lp->getNumCols();

					double coef = 0.0;
					double lowerBound = this->fixaLimiteInferiorVariavelPre( &v ) - 0.001;

					OPT_COL col( OPT_COL::VAR_BINARY, coef, lowerBound, 1.0, ( char * )v.toString().c_str() );

					lp->newCol( col );
					num_vars++;
				}
			}
		}
	}

	return num_vars;
}



// fpi_{a}
int SolverMIP::cria_preVariavel_folga_prioridade_inf( int campusId, int prior )
{
	int num_vars = 0;

	if ( prior < 2 )
	   return num_vars;

	Campus *cp = problemData->refCampus[campusId];

	ITERA_GGROUP_LESSPTR( itAluno, problemData->alunos, Aluno )
	{
		Aluno *aluno = *itAluno;

		if ( aluno->getOferta()->getCampusId() != campusId )
		{
			continue;
		}

		VariablePre v;
		v.reset();
		v.setType( VariablePre::V_PRE_SLACK_PRIOR_INF );
		v.setAluno( aluno );
		v.setCampus( cp );

		if ( vHashPre.find( v ) == vHashPre.end() )
		{
			vHashPre[v] = lp->getNumCols();					

			double lowerBound = 0.0;
			double upperBound = 0.0;
			int totalCreditos = 0;

			ITERA_GGROUP_LESSPTR( itAlunoDem, aluno->demandas, AlunoDemanda )
			{
				if ( itAlunoDem->getPrioridade() != prior )
					continue;

				int nCreds = itAlunoDem->demanda->disciplina->getTotalCreditos();
				int tempo = itAlunoDem->demanda->disciplina->getTempoCredSemanaLetiva();
				totalCreditos += nCreds;

				upperBound += nCreds*tempo;
			}
			
			double coef = 0.0;
			if ( problemData->parametros->funcao_objetivo == 0 )
			{
				double custo = cp->getCusto();
							 
				coef = -50 * custo * totalCreditos;
			}
			else if ( problemData->parametros->funcao_objetivo == 1 )
			{
				double custo = cp->getCusto();

				coef = 5 * custo * totalCreditos;
			}	

			OPT_COL col( OPT_COL::VAR_INTEGRAL, coef, lowerBound, upperBound, ( char * )v.toString().c_str() );

			lp->newCol( col );
			num_vars++;
		}
	}

	return num_vars;
}

// fps_{a}
int SolverMIP::cria_preVariavel_folga_prioridade_sup( int campusId, int prior )
{
	int num_vars = 0;
	
	if ( prior < 2 )
	   return num_vars;

	Campus *cp = problemData->refCampus[campusId];

	ITERA_GGROUP_LESSPTR( itAluno, problemData->alunos, Aluno )
	{
		Aluno *aluno = *itAluno;

		if ( aluno->getOferta()->getCampusId() != campusId )
		{
			continue;
		}

		VariablePre v;
		v.reset();
		v.setType( VariablePre::V_PRE_SLACK_PRIOR_SUP );
		v.setAluno( aluno );
		v.setCampus( cp );

		if ( vHashPre.find( v ) == vHashPre.end() )
		{
			vHashPre[v] = lp->getNumCols();
				
			double lowerBound = 0.0;
			double upperBound = 0.0;
			int totalCreditos = 0;

			double cargaHorariaNaoAtendida = 0.0;
			ITERA_GGROUP_LESSPTR( itAlDemanda, problemData->listSlackDemandaAluno, AlunoDemanda )
			{
				if ( itAlDemanda->getAlunoId() == aluno->getAlunoId() &&
					 itAlDemanda->getPrioridade() == prior-1 )
				{
					int nCreds = itAlDemanda->demanda->disciplina->getTotalCreditos();
					int duracaoCred = itAlDemanda->demanda->disciplina->getTempoCredSemanaLetiva();
					totalCreditos += nCreds;

					cargaHorariaNaoAtendida += nCreds*duracaoCred;
				}
			}
						
			double coef = 0.0;
			if ( problemData->parametros->funcao_objetivo == 0 )
			{
				double custo = cp->getCusto();
							 
				coef = -100 * custo * totalCreditos;
			}
			else if ( problemData->parametros->funcao_objetivo == 1 )
			{
				double custo = cp->getCusto();

				coef = 10 * custo * totalCreditos;
			}	

			upperBound = cargaHorariaNaoAtendida;

			OPT_COL col( OPT_COL::VAR_INTEGRAL, coef, lowerBound, upperBound, ( char * )v.toString().c_str() );

			lp->newCol( col );
			num_vars++;
		}
	}

	return num_vars;
}

/********************************************************************
**                    Restri��es do pre-Tatico                     **
*********************************************************************/

int SolverMIP::cria_preRestricoes( int campusId, int prioridade )
{
	int restricoes = 0;
	CPUTimer timer;
	double dif = 0.0;

#ifdef PRINT_cria_restricoes
	int numRestAnterior = 0;
#endif

	timer.start();
	restricoes += cria_preRestricao_carga_horaria( campusId );					// Restri��o 1.1
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest \"1.1\": " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
#endif

	timer.start();
	restricoes += cria_preRestricao_max_cred_sala_sl( campusId );				// Restri��o 1.2
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest \"1.2\": " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
#endif

	timer.start();
	restricoes += cria_preRestricao_ativacao_var_o( campusId );				// Restri��o 1.3
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest \"1.3\": " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
#endif

	timer.start();
	restricoes += cria_preRestricao_evita_mudanca_de_sala( campusId );			// Restri��o 1.4
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest \"1.4\": " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
#endif

	timer.start();
	restricoes += cria_preRestricao_aluno_curso_disc( campusId );				// Restri��o 1.6
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest \"1.6\": " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
#endif

	timer.start();
	restricoes += cria_preRestricao_cap_sala( campusId );						// Restri��o 1.7
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest \"1.7\": " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
#endif   

	timer.start();
	restricoes += cria_preRestricao_compartilhamento_incompat( campusId );		// Restri��o 1.8
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest \"1.8\": " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
#endif    

	timer.start();
	restricoes += cria_preRestricao_proibe_compartilhamento( campusId );		// Restri��o 1.9
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest \"1.9\": " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
#endif   

	timer.start();
	restricoes += cria_preRestricao_ativacao_var_z( campusId );				// Restri��o 1.10
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest \"1.10\": " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
#endif   

	timer.start();
	restricoes += cria_preRestricao_evita_turma_disc_camp_d( campusId );		// Restri��o 1.11
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest \"1.11\": " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
#endif   

	timer.start();
	restricoes += cria_preRestricao_limita_abertura_turmas( campusId );		// Restri��o 1.12
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest \"1.12\": " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
#endif   

	timer.start();
	restricoes += cria_preRestricao_abre_turmas_em_sequencia( campusId );		// Restri��o 1.13
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest \"1.13\": " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
#endif   

	timer.start();
	restricoes += cria_preRestricao_turma_mesma_disc_sala_dif( campusId );		// Restri��o 1.14
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest \"1.14\": " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
#endif

	timer.start();
	restricoes += cria_preRestricao_limite_sup_creds_sala( campusId );		// Restri��o 1.15
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest \"1.15\": " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
#endif   

	timer.start();
	restricoes += cria_preRestricao_ativa_var_aloc_aluno_oft( campusId );		// Restri��o 1.16
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest \"1.16\": " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
#endif   

   if ( problemData->parametros->otimizarPor == "BLOCOCURRICULAR" )
   {
		timer.start();
		restricoes += cria_preRestricao_cap_aloc_dem_disc_oft( campusId );				// Restri��o 1.5
		timer.stop();
		dif = timer.getCronoCurrSecs();

	   #ifdef PRINT_cria_restricoes
		std::cout << "numRest \"1.5\": " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
		numRestAnterior = restricoes;
	   #endif


		timer.start();
		restricoes += cria_preRestricao_fixa_nao_compartilhamento( campusId );				// Restri��o 1.17
		timer.stop();
		dif = timer.getCronoCurrSecs();

		#ifdef PRINT_cria_restricoes
	   std::cout << "numRest \"1.17\": " << (restricoes - numRestAnterior) << std::endl;
	   numRestAnterior = restricoes;
		#endif   
   }

   else if ( problemData->parametros->otimizarPor == "ALUNO" )
   {
		timer.start();
		restricoes += cria_preRestricao_cap_aloc_dem_disc_aluno( campusId );				// Restri��o 1.5
		timer.stop();
		dif = timer.getCronoCurrSecs();

	   #ifdef PRINT_cria_restricoes
		std::cout << "numRest \"1.5\": " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
		numRestAnterior = restricoes;
	   #endif

		timer.start();
		restricoes += cria_preRestricao_atendimento_aluno( campusId );				// Restri��o 1.18
		timer.stop();
		dif = timer.getCronoCurrSecs();

		#ifdef PRINT_cria_restricoes
	   std::cout << "numRest \"1.18\": " << (restricoes - numRestAnterior) << std::endl;
	   numRestAnterior = restricoes;
		#endif   

	   /*
	   restricoes += cria_preRestricao_aluno_unica_turma_disc( campusId );		// Restri��o 1.19

		#ifdef PRINT_cria_restricoes
	   std::cout << "numRest \"1.19\": " << (restricoes - numRestAnterior) << std::endl;
	   numRestAnterior = restricoes;
		#endif
		*/

		timer.start();
		restricoes += cria_preRestricao_aluno_discPraticaTeorica( campusId );		// Restri��o 1.20
		timer.stop();
		dif = timer.getCronoCurrSecs();

		#ifdef PRINT_cria_restricoes
	   std::cout << "numRest \"1.20\": " << (restricoes - numRestAnterior) << std::endl;
	   numRestAnterior = restricoes;
		#endif	   


		timer.start();
		restricoes += cria_preRestricao_prioridadesDemanda( campusId, prioridade );		// Restri��o 1.21
		timer.stop();
		dif = timer.getCronoCurrSecs();

		#ifdef PRINT_cria_restricoes
	   std::cout << "numRest \"1.21\": " << (restricoes - numRestAnterior) << std::endl;
	   numRestAnterior = restricoes;
		#endif
	   

   }

	return restricoes;
}

// Restri��o 1.1
int SolverMIP::cria_preRestricao_carga_horaria( int campusId )
{
   int restricoes = 0;
   int nnz;
   char name[ 100 ];

   VariablePre v;
   ConstraintPre c;
   VariablePreHash::iterator it_v;

   Disciplina * disciplina = NULL;

   ITERA_GGROUP_LESSPTR( itCampus, problemData->campi, Campus )
   {
	   if ( itCampus->getId() != campusId )
	   {
			continue;
	   }

      ITERA_GGROUP_LESSPTR( it_disciplina, problemData->disciplinas, Disciplina )
      {
         disciplina = ( *it_disciplina );

		 #pragma region Equivalencias
		 if ( problemData->mapDiscSubstituidaPor.find( disciplina ) !=
		 	  problemData->mapDiscSubstituidaPor.end() )
		 {
			 continue;
		 }
		 #pragma endregion

         for ( int turma = 0; turma < disciplina->getNumTurmas(); turma++ )
         {
            c.reset();
            c.setType( ConstraintPre::C_PRE_CARGA_HORARIA );

            c.setCampus( *itCampus );
            c.setTurma( turma );
            c.setDisciplina( disciplina );

            sprintf( name, "%s", c.toString().c_str() );

            if ( cHashPre.find( c ) != cHashPre.end() )
            {
               continue;
            }

			nnz = disciplina->getNSalasAptas();

            OPT_ROW row( nnz + 1, OPT_ROW::EQUAL , 0 , name );

            // ---

            ITERA_GGROUP_LESSPTR( itUnidade, itCampus->unidades, Unidade )
            {
               ITERA_GGROUP_LESSPTR( itCjtSala, itUnidade->conjutoSalas, ConjuntoSala )
               {
				    if ( itCjtSala->disciplinas_associadas.find( disciplina ) ==
					     itCjtSala->disciplinas_associadas.end() )
					{
						continue;
				    }

					v.reset();
					v.setType( VariablePre::V_PRE_CREDITOS );

					v.setTurma( turma );
					v.setDisciplina( disciplina );
					v.setUnidade( *itUnidade );
					v.setSubCjtSala( *itCjtSala );             

					it_v = vHashPre.find( v );
					if ( it_v != vHashPre.end() )
					{
						row.insert( it_v->second, 1.0 );
					}
               }
            }

            // ---

            v.reset();
            v.setType( VariablePre::V_PRE_ABERTURA );

            v.setCampus( *itCampus );
            v.setDisciplina( disciplina );
            v.setTurma( turma );

            it_v = vHashPre.find( v );
            if( it_v != vHashPre.end() )
            {
               row.insert( it_v->second, -( disciplina->getCredPraticos() + 
											disciplina->getCredTeoricos() ) );
            }

            // ---

            if ( row.getnnz() != 0 )
            {
               cHashPre[ c ] = lp->getNumRows();

               lp->addRow( row );
               restricoes++;
            }
         }
      }
   }

	return restricoes;
}

// Restri��o 1.2
int SolverMIP::cria_preRestricao_max_cred_sala_sl( int campusId )
{
   int restricoes = 0;
   int nnz;
   char name[ 100 ];

   VariablePre v;
   ConstraintPre c;
   VariablePreHash::iterator it_v;

   Disciplina * disciplina = NULL;

   ITERA_GGROUP_LESSPTR( itCampus, problemData->campi, Campus )
   {
	   if ( itCampus->getId() != campusId )
	   {
			continue;
	   }

      ITERA_GGROUP_LESSPTR( itUnidade, itCampus->unidades, Unidade )
      {
         ITERA_GGROUP_LESSPTR( itCjtSala, itUnidade->conjutoSalas, ConjuntoSala )
         {
			ConstraintPre c;
			c.reset();
			c.setType( ConstraintPre::C_PRE_MAX_CRED_SALA );
			
			c.setUnidade( *itUnidade );
			c.setSubCjtSala( *itCjtSala );

			sprintf( name, "%s", c.toString().c_str() ); 
			if ( cHashPre.find( c ) != cHashPre.end() )
			{
				continue;
			}

			double htps = itCjtSala->maxTempoPermitidoNaSemana( problemData->mapDiscSubstituidaPor ); 

			nnz = itCjtSala->disciplinas_associadas.size() * 5; // estimando em media 5 turmas por disciplina

			OPT_ROW row( nnz, OPT_ROW::LESS , htps, name );

            ITERA_GGROUP_LESSPTR( it_disc, itCjtSala->disciplinas_associadas, Disciplina )
            {
               disciplina = ( *it_disc );
			   
			   #pragma region Equivalencias
			   if ( problemData->mapDiscSubstituidaPor.find( disciplina ) !=
					problemData->mapDiscSubstituidaPor.end() )
			   {
					continue;
			   }
				#pragma endregion

               for ( int turma = 0; turma < disciplina->getNumTurmas(); turma++ )
               {
                     VariablePre v;
                     v.reset();
                     v.setType( VariablePre::V_PRE_CREDITOS );

                     v.setTurma( turma );            // i
                     v.setDisciplina( disciplina );  // d
                     v.setUnidade( *itUnidade );     // u
                     v.setSubCjtSala( *itCjtSala );  // tps

					 it_v = vHashPre.find( v );
					 if ( it_v != vHashPre.end() )
					 {
						 row.insert( it_v->second, disciplina->getTempoCredSemanaLetiva() );
					 }       
                }
            }

			if ( row.getnnz() != 0 )
			{
				cHashPre[ c ] = lp->getNumRows();

				lp->addRow( row );
				restricoes++;
			}
         }
      }
   }

	return restricoes;
}

// Restri��o 1.3
int SolverMIP::cria_preRestricao_ativacao_var_o( int campusId )
{
   int restricoes = 0;
   char name[ 100 ];
   int nnz;

   VariablePre v;
   ConstraintPre c;
   VariablePreHash::iterator it_v;

   Disciplina * disciplina = NULL;

   ITERA_GGROUP_LESSPTR( itCampus, problemData->campi, Campus )
   {
	   if ( itCampus->getId() != campusId )
	   {
			continue;
	   }

      ITERA_GGROUP_LESSPTR( itUnidade, itCampus->unidades, Unidade )
      {
         ITERA_GGROUP_LESSPTR( itCjtSala, itUnidade->conjutoSalas, ConjuntoSala )
         {
            ITERA_GGROUP_LESSPTR( it_disc, itCjtSala->disciplinas_associadas, Disciplina )
            {
                disciplina = ( *it_disc );

				#pragma region Equivalencias
				if ( problemData->mapDiscSubstituidaPor.find( disciplina ) !=
					problemData->mapDiscSubstituidaPor.end() )
				{
					continue;
				}
				#pragma endregion

                for ( int turma = 0; turma < disciplina->getNumTurmas(); turma++ )
                {
					c.reset();
					c.setType( ConstraintPre::C_PRE_VAR_O );

					c.setUnidade( *itUnidade );
					c.setSubCjtSala( *itCjtSala );
					c.setDisciplina( disciplina );
					c.setTurma( turma );

					sprintf( name, "%s", c.toString().c_str() ); 
					if ( cHashPre.find( c ) != cHashPre.end() )
					{
						continue;
					}

					nnz = 2;

					OPT_ROW row( nnz, OPT_ROW::LESS , 0.0, name );

					v.reset();
					v.setType( VariablePre::V_PRE_OFERECIMENTO );

					v.setTurma( turma );
					v.setDisciplina( disciplina );
					v.setUnidade( *itUnidade );
					v.setSubCjtSala( *itCjtSala );

					it_v = vHashPre.find( v );
					if( it_v != vHashPre.end() )
					{
						row.insert( it_v->second, - ( disciplina->getCredPraticos() + 
													  disciplina->getCredTeoricos() ) );
					}

					v.reset();
					v.setType( VariablePre::V_PRE_CREDITOS );

					v.setTurma( turma );
					v.setDisciplina( disciplina );
					v.setUnidade( *itUnidade );
					v.setSubCjtSala( *itCjtSala );

					it_v = vHashPre.find( v );
					if ( it_v != vHashPre.end() )
					{
						row.insert( it_v->second, 1.0 );
					}

					if ( row.getnnz() != 0 )
					{
						cHashPre[ c ] = lp->getNumRows();

						lp->addRow( row );
						restricoes++;
					}
				}
            }
         }
      }
   }

	return restricoes;
}

// Restri��o 1.4
int SolverMIP::cria_preRestricao_evita_mudanca_de_sala( int campusId )
{
   int restricoes = 0;
   int nnz;
   char name[ 100 ];

   VariablePre v;
   ConstraintPre c;
   VariablePreHash::iterator it_v;

   Disciplina * disciplina = NULL;

   ITERA_GGROUP_LESSPTR( itCampus, problemData->campi, Campus )
   {
	   if ( itCampus->getId() != campusId )
	   {
			continue;
	   }

      ITERA_GGROUP_LESSPTR( it_disciplina, problemData->disciplinas, Disciplina )
      {
         disciplina = ( *it_disciplina );

		 #pragma region Equivalencias
		 if ( problemData->mapDiscSubstituidaPor.find( disciplina ) !=
			  problemData->mapDiscSubstituidaPor.end() )
		 {
		 	 continue;
		 }
		 #pragma endregion

         for ( int turma = 0; turma < disciplina->getNumTurmas(); turma++ )
         {
            c.reset();
            c.setType( ConstraintPre::C_EVITA_MUDANCA_DE_SALA );

            c.setCampus( *itCampus );
            c.setTurma( turma );
            c.setDisciplina( disciplina );

            sprintf( name, "%s", c.toString().c_str() );

            if ( cHashPre.find( c ) != cHashPre.end() )
            {
               continue;
            }

			nnz = disciplina->getNSalasAptas();

            OPT_ROW row( nnz, OPT_ROW::LESS , 1.0 , name );
			
            ITERA_GGROUP_LESSPTR( itUnidade, itCampus->unidades, Unidade )
            {
               ITERA_GGROUP_LESSPTR( itCjtSala, itUnidade->conjutoSalas, ConjuntoSala )
               {
				    if ( itCjtSala->disciplinas_associadas.find( disciplina ) ==
					     itCjtSala->disciplinas_associadas.end() )
					{
						continue;
				    }

					v.reset();
					v.setType( VariablePre::V_PRE_OFERECIMENTO );

					v.setTurma( turma );
					v.setDisciplina( disciplina );
					v.setUnidade( *itUnidade );
					v.setSubCjtSala( *itCjtSala );             

					it_v = vHashPre.find( v );
					if ( it_v != vHashPre.end() )
					{
						row.insert( it_v->second, 1.0 );
					}
               }
            }
			
            if ( row.getnnz() != 0 )
            {
               cHashPre[ c ] = lp->getNumRows();

               lp->addRow( row );
               restricoes++;
            }
         }
      }
   }

   return restricoes;
}

// Restri��o 1.5
/*
	Aloca��o de demanda por oferta
*/
int SolverMIP::cria_preRestricao_cap_aloc_dem_disc_oft( int campusId )
{
    int restricoes = 0;
    char name[ 100 ];
    int nnz = 0;

    ConstraintPre c;
    VariablePre v;
    VariablePreHash::iterator it_v;

    Disciplina * disciplina = NULL;

   ITERA_GGROUP_LESSPTR( itOferta, problemData->ofertas, Oferta )
   {
	   if ( itOferta->getCampusId() != campusId )
	   {
			continue;
	   }

      map < Disciplina*, int, LessPtr< Disciplina > >::iterator itPrdDisc = 
         itOferta->curriculo->disciplinas_periodo.begin();

      for (; itPrdDisc != itOferta->curriculo->disciplinas_periodo.end(); itPrdDisc++ )
      {
		  disciplina = itPrdDisc->first;

         c.reset();
         c.setType( ConstraintPre::C_PRE_CAP_ALOC_DEM_DISC );

         c.setOferta( *itOferta );
         c.setDisciplina( disciplina );

         sprintf( name, "%s", c.toString().c_str() ); 
         if ( cHashPre.find( c ) != cHashPre.end() )
         {
            continue;
         }

         if ( disciplina->getNumTurmas() <= 0 )
         {
            continue;
         }

         nnz = disciplina->getNumTurmas();
         int rhs = 0;

         // Calculando P_{d,o}
         ITERA_GGROUP_LESSPTR( itDem, problemData->demandas, Demanda )
         {
            if ( itDem->disciplina->getId() == disciplina->getId()
               && itDem->getOfertaId() == itOferta->getId() )
            {
               rhs += itDem->getQuantidade();
            }
         }

         OPT_ROW row( nnz , OPT_ROW::EQUAL, rhs , name );

		 ITERA_GGROUP_LESSPTR( itCampus, problemData->campi, Campus )
		 {
			if ( itCampus->getId() != campusId )
			{
				continue;
			}

			Campus* cp = *itCampus;
		
			ITERA_GGROUP_LESSPTR( itUnidade, itCampus->unidades, Unidade )
			{
				ITERA_GGROUP_LESSPTR( itCjtSala, itUnidade->conjutoSalas, ConjuntoSala )
				{
				    if ( itCjtSala->disciplinas_associadas.find( disciplina ) ==
					     itCjtSala->disciplinas_associadas.end() )
					{
						continue;
				    }
					
					for ( int turma = 0; turma < disciplina->getNumTurmas(); turma++ )
					{
						v.reset();
						v.setType( VariablePre::V_PRE_ALUNOS );

						v.setTurma( turma );
						v.setDisciplina( disciplina );
						v.setOferta( *itOferta );
						v.setUnidade( *itUnidade );
						v.setSubCjtSala( *itCjtSala );

						it_v = vHashPre.find( v );
						if ( it_v != vHashPre.end() )
						{
							row.insert( it_v->second, 1.0 );
						}
					}
				}
			}
		 }

         v.reset();
         v.setType( VariablePre::V_PRE_SLACK_DEMANDA );
         v.setDisciplina( disciplina );
         v.setOferta( *itOferta );

         it_v = vHashPre.find( v );
         if( it_v != vHashPre.end() )
         {
            row.insert( it_v->second, 1.0 );
         }

         if ( row.getnnz() != 0 )
         {
            cHashPre[ c ] = lp->getNumRows();

            lp->addRow( row );
            restricoes++;
         }
      }
   }

	return restricoes;
}

/*
	Aloca��o de demanda por aluno
*/
int SolverMIP::cria_preRestricao_cap_aloc_dem_disc_aluno( int campusId )
{
   int restricoes = 0;
      
   char name[ 200 ];
   int nnz;

   ConstraintPre c;
   VariablePre v;
   VariablePreHash::iterator it_v;   

   Campus *campus = problemData->refCampus[campusId];

	ITERA_GGROUP_LESSPTR( itAluno, problemData->alunos, Aluno )
	{
		Aluno *aluno = *itAluno;

		if ( aluno->getOferta()->getCampusId() != campusId )
		{
			continue;
		}

		ITERA_GGROUP_LESSPTR( itAlDemanda, aluno->demandas, AlunoDemanda )
		{
			Disciplina *disciplina = itAlDemanda->demanda->disciplina;

			c.reset();
			c.setType( ConstraintPre::C_PRE_CAP_ALOC_DEM_DISC );
			c.setAluno( aluno );
			c.setDisciplina( disciplina );
			c.setCampus( campus );

			sprintf( name, "%s", c.toString().c_str() ); 

			if ( cHashPre.find( c ) != cHashPre.end() )
			{
				continue;
			}

			nnz = disciplina->getNumTurmas() + 1;

			OPT_ROW row( nnz, OPT_ROW::EQUAL , 1.0 , name );

			for ( int turma = 0; turma < disciplina->getNumTurmas(); turma++ )
			{
				VariablePre v;
				v.reset();
				v.setType( VariablePre::V_PRE_ALOCA_ALUNO_TURMA_DISC );
				v.setAluno( aluno );
				v.setDisciplina( disciplina );
				v.setTurma( turma );
				v.setCampus( campus );

				it_v = vHashPre.find( v );
				if( it_v != vHashPre.end() )
				{
					row.insert( it_v->second, 1.0 );
				}
			}

			v.reset();
			v.setType( VariablePre::V_PRE_SLACK_DEMANDA );
			v.setDisciplina( disciplina );
			v.setAluno( aluno );

			it_v = vHashPre.find( v );
			if( it_v != vHashPre.end() )
			{
				row.insert( it_v->second, 1.0 );
			}

			if ( row.getnnz() != 0 )
			{
				cHashPre[ c ] = lp->getNumRows();
				lp->addRow( row );
				restricoes++;
			}
		}
	}

	return restricoes;
}


// Restri��o 1.6
int SolverMIP::cria_preRestricao_aluno_curso_disc( int campusId )
{
	int restricoes = 0;

    char name[ 100 ];
    int nnz = 0;

    ConstraintPre c;
    VariablePre v;
    VariablePreHash::iterator it_v;

    Disciplina * disciplina = NULL;

	ITERA_GGROUP_LESSPTR( itDisc, problemData->disciplinas, Disciplina )
	{		
		Disciplina* disciplina = *itDisc;

		if ( problemData->cp_discs[ campusId ].find( disciplina->getId() ) ==
			 problemData->cp_discs[ campusId ].end() )
		{
			continue;
		}

		#pragma region Equivalencias
		if ( problemData->mapDiscSubstituidaPor.find( disciplina ) !=
			problemData->mapDiscSubstituidaPor.end() )
		{
			continue;
		}
		#pragma endregion
										
		ITERA_GGROUP_LESSPTR( itCurso, problemData->cursos, Curso )
		{
			Curso *curso = *itCurso;

			if ( !curso->possuiDisciplina( disciplina ) )
				continue;

			// Calculando P_{d,o}
			int qtdDem = 0;
			ITERA_GGROUP_LESSPTR( itDem, problemData->demandas, Demanda )
			{
				if ( itDem->disciplina->getId() == disciplina->getId() &&
					 itDem->oferta->getCursoId() == curso->getId() &&
					 itDem->oferta->getCampusId() == campusId )
				{
					qtdDem += itDem->getQuantidade();
				}
			}

			if ( qtdDem <= 0 )
			{
				continue;
			}

			for ( int turma = 0; turma < disciplina->getNumTurmas(); turma++ )
			{
				// -------------------------------------------------
				c.reset();
				c.setType( ConstraintPre::C_ALUNO_OFT_DISC );

				c.setCurso( curso );
				c.setDisciplina( disciplina );
				c.setTurma( turma );

				sprintf( name, "%s", c.toString().c_str() ); 
				if ( cHashPre.find( c ) != cHashPre.end() )
				{
					continue;
				}
						 
				nnz = 100;

				OPT_ROW row( nnz , OPT_ROW::LESS, 0.0 , name );

				v.reset();
				v.setType( VariablePre::V_PRE_ALOC_ALUNO ); // b_{i,d,c}
				v.setTurma( turma );
				v.setDisciplina( disciplina );
				v.setCurso( curso );

				it_v = vHashPre.find(v);
				if( it_v != vHashPre.end() )
				{
					row.insert( it_v->second, -qtdDem );
				}

				GGroup< Oferta*, LessPtr<Oferta> > ofertas = problemData->ofertasDisc[ disciplina->getId() ];

				ITERA_GGROUP_LESSPTR( itOferta, ofertas, Oferta )
				{
					if ( itOferta->getCursoId() != curso->getId() )
						continue;

					if ( itOferta->campus->getId() != campusId )
						continue;

					ITERA_GGROUP_LESSPTR( itUnidade, itOferta->campus->unidades, Unidade )
					{
						ITERA_GGROUP_LESSPTR( itCjtSala, itUnidade->conjutoSalas, ConjuntoSala )
						{
							 if ( itCjtSala->disciplinas_associadas.find( disciplina ) ==
							 	  itCjtSala->disciplinas_associadas.end() )
							 {
								 continue;
							 }
						 
							 v.reset();
							 v.setType( VariablePre::V_PRE_ALUNOS ); // a_{i,d,oft,s}
							 v.setTurma( turma );
							 v.setDisciplina( disciplina );
							 v.setOferta( *itOferta );
							 v.setUnidade( *itUnidade );
							 v.setSubCjtSala( *itCjtSala );

							 it_v = vHashPre.find( v );
							 if ( it_v != vHashPre.end() )
							 {
						 		 row.insert( it_v->second, 1.0 );
							 }
						}
					}
				}

				if ( row.getnnz() != 0 )
				{
					cHashPre[ c ] = lp->getNumRows();

					lp->addRow( row );
					restricoes++;
				}				
				// -------------------------------------------------
		   }
       }
   }

	return restricoes;
}

// Restri��o 1.7
int SolverMIP::cria_preRestricao_cap_sala( int campusId )
{
	int restricoes = 0;

    char name[ 100 ];
    int nnz = 0;

    ConstraintPre c;
    VariablePre v;
    VariablePreHash::iterator it_v;

    Disciplina * disciplina = NULL;

	ITERA_GGROUP_LESSPTR( itCampus, problemData->campi, Campus )
	{
		Campus* cp = *itCampus;
			
		if ( cp->getId() != campusId )
		{
			continue;
		}

		GGroup< int > disciplinas = problemData->cp_discs[ cp->getId() ];
		
		// Para cada disciplina do campus
		ITERA_GGROUP_N_PT( itDisc, disciplinas, int )
		{
			 Disciplina *disciplina = problemData->refDisciplinas[ *itDisc ];

			 #pragma region Equivalencias
			 if ( problemData->mapDiscSubstituidaPor.find( disciplina ) !=
				  problemData->mapDiscSubstituidaPor.end() )
			 {
			 	 continue;
			 }
			 #pragma endregion

			 ITERA_GGROUP_LESSPTR( itUnidade, itCampus->unidades, Unidade )
			 {
				// Para cada sala associada � disciplina
				ITERA_GGROUP_LESSPTR( itCjtSala, itUnidade->conjutoSalas, ConjuntoSala )
				{
					if ( itCjtSala->disciplinas_associadas.find( disciplina ) ==
						itCjtSala->disciplinas_associadas.end() )
					{
						continue;
					}

					// Para cada turma da disciplina
					for ( int turma = 0; turma < disciplina->getNumTurmas(); turma++ )
					{
						c.reset();
						c.setType( ConstraintPre::C_CAP_SALA );

						c.setDisciplina( disciplina );
						c.setTurma( turma );
						c.setUnidade( *itUnidade );
						c.setSubCjtSala( *itCjtSala );

						sprintf( name, "%s", c.toString().c_str() ); 
						if ( cHashPre.find( c ) != cHashPre.end() )
						{
							continue;
						}
						 
						nnz = disciplina->getNumTurmas();

						OPT_ROW row( nnz , OPT_ROW::LESS, 0.0 , name );

						// Para cada oferta que cont�m a disciplina
						GGroup< Oferta*, LessPtr<Oferta> > ofertas = problemData->ofertasDisc[ disciplina->getId() ];

						ITERA_GGROUP_LESSPTR( itOferta, ofertas, Oferta )
						{							
							 v.reset();
							 v.setType( VariablePre::V_PRE_ALUNOS );

							 v.setTurma( turma );
							 v.setDisciplina( disciplina );
							 v.setOferta( *itOferta );
							 v.setUnidade( *itUnidade );
							 v.setSubCjtSala( *itCjtSala );

							 it_v = vHashPre.find( v );
							 if ( it_v != vHashPre.end() )
							 {
						 		 row.insert( it_v->second, 1.0 );
							 }

						}						

						v.reset();
						v.setType( VariablePre::V_PRE_OFERECIMENTO );

						v.setTurma( turma );
						v.setDisciplina( disciplina );
						v.setSubCjtSala( *itCjtSala );
						v.setUnidade( *itUnidade ); 

						it_v = vHashPre.find(v);
						if( it_v != vHashPre.end() )
						{
							row.insert( it_v->second, -itCjtSala->capTotalSalas() );
						}

						if ( row.getnnz() != 0 )
						{
							cHashPre[ c ] = lp->getNumRows();

							lp->addRow( row );
							restricoes++;
						}
					}
				}
			}
		}
   }

	return restricoes;
}

// Restri��o 1.8
int SolverMIP::cria_preRestricao_compartilhamento_incompat( int campusId )
{
       int restricoes = 0;
       char name[ 100 ];
       int nnz;

	   ConstraintPre c;
	   VariablePre v;
	   VariablePreHash::iterator it_v;	
	   Campus *cp = NULL;

	   cp = problemData->refCampus[ campusId ];

	   if ( cp==NULL )
	   {
		   std::cout<<"\nATENCAO: SolverMIP::cria_preRestricao_compartilhamento_incompat( int campusId ): ";
		   std::cout<<"\nCampus" << campusId << "nao encontrado.\n";
		   return restricoes;
	   }

	   ITERA_GGROUP_LESSPTR( itCurso1, cp->cursos, Curso )
	   {
		   Curso* c1 = *itCurso1;

		   ITERA_GGROUP_INIC_LESSPTR( itCurso2, itCurso1, cp->cursos, Curso )
		   {
				Curso* c2 = *itCurso2;
				
			    if ( problemData->cursosCompativeis(c1, c2) || c1 == c2 )
				    continue;

				ITERA_GGROUP_LESSPTR( itDisc, problemData->disciplinas, Disciplina )
				{
					Disciplina *disciplina = *itDisc;

					if ( problemData->cp_discs[ campusId ].find( disciplina->getId() ) ==
						 problemData->cp_discs[ campusId ].end() )
					{
						continue;
					}

					#pragma region Equivalencias
					if ( problemData->mapDiscSubstituidaPor.find( disciplina ) !=
						problemData->mapDiscSubstituidaPor.end() )
					{
						continue;
					}
					#pragma endregion
										
					if ( !c1->possuiDisciplina( disciplina ) ||
						 !c2->possuiDisciplina( disciplina ) )
						continue;

					for ( int turma = 0; turma < disciplina->getNumTurmas(); turma++ )
					{
						c.reset();
						c.setType( ConstraintPre::C_PRE_ALUNOS_CURSOS_INCOMP );
						c.setParCursos( std::make_pair( c1, c2 ) );
						c.setDisciplina( disciplina );
						c.setTurma( turma );

						sprintf( name, "%s", c.toString().c_str() ); 

						if ( cHashPre.find( c ) != cHashPre.end() )
						{
							continue;
						}

						nnz = 3;

						OPT_ROW row( nnz, OPT_ROW::LESS , 1.0 , name );

						v.reset();
						v.setType( VariablePre::V_PRE_ALOC_ALUNO );
						v.setTurma( turma );
						v.setDisciplina( disciplina );
						v.setCurso( c1 );

						it_v = vHashPre.find( v );
						if( it_v != vHashPre.end() )
						{
							row.insert( it_v->second, 1 );
						}

						v.reset();
						v.setType( VariablePre::V_PRE_ALOC_ALUNO );
						v.setTurma( turma );
						v.setDisciplina( disciplina );
						v.setCurso( c2 );

						it_v = vHashPre.find( v );
						if( it_v != vHashPre.end() )
						{
							row.insert(it_v->second, 1);
						}

						VariablePre v;
						v.reset();
						v.setType( VariablePre::V_PRE_SLACK_ALOC_ALUNOS_CURSO_INCOMPAT );
						v.setTurma( turma );				
						v.setDisciplina( disciplina );  
						v.setParCursos( std::make_pair(c1, c2) );	

						it_v = vHashPre.find( v );
						if ( it_v != vHashPre.end() )
						{
							row.insert( it_v->second, -1 );
						}

						if ( row.getnnz() != 0 )
						{
							cHashPre[ c ] = lp->getNumRows();
							lp->addRow( row );
							restricoes++;
						}
					}
				}
			}
		}

		return restricoes;
}

// Restri��o 1.9
int SolverMIP::cria_preRestricao_proibe_compartilhamento( int campusId )
{
	int restricoes = 0;

    if ( problemData->parametros->permite_compartilhamento_turma_sel )
    {
	 	return restricoes;
    }

    char name[ 100 ];
    int nnz;

    ConstraintPre c;
    VariablePre v;
    VariablePreHash::iterator it_v;
	   
	Campus *cp = NULL;

	cp = problemData->refCampus[ campusId ];

	if ( cp==NULL )
	{
		 std::cout<<"\nATENCAO: SolverMIP::cria_preRestricao_compartilhamento_incompat( int campusId ): ";
		 std::cout<<"\nCampus" << campusId << "nao encontrado.\n";
		 return restricoes;
	}
	
	   ITERA_GGROUP_LESSPTR( itCurso1, cp->cursos, Curso )
	   {
		   Curso* c1 = *itCurso1;
		   
		   ITERA_GGROUP_INIC_LESSPTR( itCurso2, itCurso1, cp->cursos, Curso )
		   {
				Curso* c2 = *itCurso2;
			    
			    if ( !problemData->cursosCompativeis(c1, c2) || c1 == c2 )
				    continue;

				ITERA_GGROUP_LESSPTR( itDisc, problemData->disciplinas, Disciplina )
				{
					Disciplina *disciplina = *itDisc;

					if ( problemData->cp_discs[ campusId ].find( disciplina->getId() ) ==
						 problemData->cp_discs[ campusId ].end() )
					{
						continue;
					}

					#pragma region Equivalencias
					if ( problemData->mapDiscSubstituidaPor.find( disciplina ) !=
						problemData->mapDiscSubstituidaPor.end() )
					{
						continue;
					}
					#pragma endregion
										
					if ( !c1->possuiDisciplina( disciplina ) ||
						 !c2->possuiDisciplina( disciplina ) )
						continue;
							
					for ( int turma = 0; turma < disciplina->getNumTurmas(); turma++ )
					{
						c.reset();
						c.setType( ConstraintPre::C_PRE_PROIBE_COMPARTILHAMENTO );
						c.setParCursos( std::make_pair( c1, c2 ) );
						c.setDisciplina( disciplina );
						c.setTurma( turma );

						sprintf( name, "%s", c.toString().c_str() ); 

						if ( cHashPre.find( c ) != cHashPre.end() )
						{
							continue;
						}

						nnz = 3;

						OPT_ROW row( nnz, OPT_ROW::LESS , 1.0 , name );

						v.reset();
						v.setType( VariablePre::V_PRE_ALOC_ALUNO );
						v.setTurma( turma );
						v.setDisciplina( disciplina );
						v.setCurso( c1 );

						it_v = vHashPre.find( v );
						if( it_v != vHashPre.end() )
						{
							row.insert( it_v->second, 1 );
						}

						v.reset();
						v.setType( VariablePre::V_PRE_ALOC_ALUNO );
						v.setTurma( turma );
						v.setDisciplina( disciplina );
						v.setCurso( c2 );

						it_v = vHashPre.find( v );
						if( it_v != vHashPre.end() )
						{
							row.insert(it_v->second, 1);
						}

						VariablePre v;
						v.reset();
						v.setType( VariablePre::V_PRE_SLACK_COMPARTILHAMENTO );
						v.setTurma( turma );				
						v.setDisciplina( disciplina );  
						v.setParCursos( std::make_pair(c1, c2) );	

						it_v = vHashPre.find( v );
						if ( it_v != vHashPre.end() )
						{
							row.insert( it_v->second, -1 );
						}

						if ( row.getnnz() != 0 )
						{
							cHashPre[ c ] = lp->getNumRows();
							lp->addRow( row );
							restricoes++;
						}
					}
				}
			}
		}

		return restricoes;
}

// Restricao 1.10
int SolverMIP::cria_preRestricao_ativacao_var_z( int campusId )
{
	int restricoes = 0;
    char name[ 100 ];
    int nnz;

    ConstraintPre c;
    VariablePre v;
    VariablePreHash::iterator it_v;

    Disciplina * disciplina = NULL;

   std::map< int /*Id Campus*/, GGroup< int > /*Id Discs*/ >::iterator it_CpDisc = problemData->cp_discs.begin();

   for ( ; it_CpDisc != problemData->cp_discs.end(); it_CpDisc++ )
   {
	  Campus *cp = problemData->refCampus[ it_CpDisc->first ];

	  if ( cp->getId() != campusId )
	  {
		  continue;
	  }

      ITERA_GGROUP_N_PT( it_disciplina, it_CpDisc->second, int )
      {
		 Disciplina *disciplina = problemData->refDisciplinas[ *it_disciplina ];

		 #pragma region Equivalencias
		 if ( problemData->mapDiscSubstituidaPor.find( disciplina ) !=
			  problemData->mapDiscSubstituidaPor.end() )
		 {
		 	 continue;
		 }
		 #pragma endregion
		
		 double bigM = disciplina->getNSalasAptas();

         for ( int turma = 0; turma < disciplina->getNumTurmas(); turma++ )
         {
			c.reset();
			c.setType( ConstraintPre::C_PRE_ATIVA_Z );
			c.setCampus( cp );
			c.setDisciplina( disciplina );
			c.setTurma( turma );

			sprintf( name, "%s", c.toString().c_str() ); 

			if ( cHashPre.find( c ) != cHashPre.end() )
			{
				continue;
			}

			nnz = disciplina->getNSalasAptas() + 1;

			OPT_ROW row( nnz, OPT_ROW::LESS , 0.0 , name );

            VariablePre v;
            v.reset();
            v.setType( VariablePre::V_PRE_ABERTURA );
            v.setTurma( turma );            // i
            v.setDisciplina( disciplina );  // d
            v.setCampus( cp );				// cp

			it_v = vHashPre.find( v );
			if ( it_v != vHashPre.end() )
			{
				row.insert( it_v->second, -bigM );
			}
			
			ITERA_GGROUP_LESSPTR( itUnidade, cp->unidades, Unidade )
			{
				ITERA_GGROUP_LESSPTR( itCjtSala, itUnidade->conjutoSalas, ConjuntoSala )
				{							
					if ( itCjtSala->disciplinas_associadas.find( disciplina ) ==
						itCjtSala->disciplinas_associadas.end() )
					{
						continue;
					}

                    VariablePre v;
                    v.reset();
                    v.setType( VariablePre::V_PRE_OFERECIMENTO );
                    v.setTurma( turma );            // i
                    v.setDisciplina( disciplina );  // d
                    v.setUnidade( *itUnidade );     // u
                    v.setSubCjtSala( *itCjtSala );  // tps

					it_v = vHashPre.find( v );
					if ( it_v != vHashPre.end() )
					{
						row.insert( it_v->second, 1.0 );
					}
				}
			}

			if ( row.getnnz() != 0 )
			{
				cHashPre[ c ] = lp->getNumRows();
				lp->addRow( row );
				restricoes++;
			}
            
         }
      }
   }

   return restricoes;

}
   

// Restricao 1.11
int SolverMIP::cria_preRestricao_evita_turma_disc_camp_d( int campusId )
{
   int restricoes = 0;
   char name[ 100 ];
   int nnz;

   ConstraintPre c;
   VariablePre v;
   VariablePreHash::iterator it_v;

   Disciplina * disciplina = NULL;

   ITERA_GGROUP_LESSPTR( it_disciplina, problemData->disciplinas, Disciplina )
   {
      disciplina = ( *it_disciplina );

	  if ( problemData->cp_discs[ campusId ].find( disciplina->getId() ) ==
		   problemData->cp_discs[ campusId ].end() )
	  {
		  continue;
 	  }

	  #pragma region Equivalencias
	  if ( problemData->mapDiscSubstituidaPor.find( disciplina ) !=
		   problemData->mapDiscSubstituidaPor.end() )
	  {
		  continue;
	  }
	  #pragma endregion

      for ( int i = 0; i < disciplina->getNumTurmas(); ++i )
      {
         c.reset();
         c.setType( ConstraintPre::C_PRE_EVITA_TURMA_DISC_CAMP_D );

         c.setDisciplina( disciplina );
         c.setTurma( i );

         sprintf( name, "%s", c.toString().c_str() ); 
         if ( cHashPre.find( c ) != cHashPre.end() )
         {
            continue;
         }

         nnz = problemData->totalSalas;
         OPT_ROW row( nnz, OPT_ROW::LESS , 1.0, name );

         ITERA_GGROUP_LESSPTR( it_campus, problemData->campi, Campus )
         {
			if ( it_campus->getId() != campusId )
			{
				continue;
			}

            v.reset();
            v.setType( VariablePre::V_PRE_ABERTURA );

            v.setTurma( i );
            v.setDisciplina( disciplina );
            v.setCampus( *it_campus );

            it_v = vHashPre.find( v );
            if( it_v != vHashPre.end() )
            {
               row.insert( it_v->second, 1.0 );
            }
         }

         if ( row.getnnz() != 0 )
         {
            cHashPre[ c ] = lp->getNumRows();

            lp->addRow( row );
            restricoes++;
         }
      }
   }

   return restricoes;
}   
   
// Restricao 1.12
int SolverMIP::cria_preRestricao_limita_abertura_turmas( int campusId )
{
   int restricoes = 0;
   char name[ 100 ];
   int nnz;
   int min;

   VariablePre v;
   ConstraintPre c;
   VariablePreHash::iterator it_v;

   Disciplina * disciplina = NULL;
   
   if ( problemData->parametros->min_alunos_abertura_turmas )
   {
		min = problemData->parametros->min_alunos_abertura_turmas_value;
		if ( min <= 0 ) min = 1;
   }
   else
   {
	   min = 1;
   }

   ITERA_GGROUP_LESSPTR( itCampus, problemData->campi, Campus )
   {
	  if ( itCampus->getId() != campusId )
	  {
		  continue;
	  }

      ITERA_GGROUP_LESSPTR( it_disciplina, problemData->disciplinas, Disciplina )
      {
         disciplina = ( *it_disciplina );
	  
		 if ( problemData->cp_discs[ campusId ].find( disciplina->getId() ) ==
			  problemData->cp_discs[ campusId ].end() )
		 {
			 continue;
 		 }

		 #pragma region Equivalencias
		 if ( problemData->mapDiscSubstituidaPor.find( disciplina ) !=
		      problemData->mapDiscSubstituidaPor.end() )
		 {
			  continue;
		 }
		 #pragma endregion

         for ( int turma = 0; turma < disciplina->getNumTurmas(); turma++ )
         {
            c.reset();
            c.setType( ConstraintPre::C_PRE_LIMITA_ABERTURA_TURMAS );

            c.setCampus( *itCampus );
            c.setDisciplina( disciplina );
            c.setTurma( turma );

            sprintf( name, "%s", c.toString().c_str() );

            if ( cHashPre.find( c ) != cHashPre.end() )
            {
               continue;
            }

			nnz = 1 + disciplina->getNSalasAptas() * problemData->ofertasDisc[ disciplina->getId() ].size();
            OPT_ROW row( nnz, OPT_ROW::LESS , 0.0, name );

            v.reset();
            v.setType( VariablePre::V_PRE_ABERTURA );

            v.setTurma( turma );
            v.setDisciplina( disciplina );
            v.setCampus( *itCampus );

            it_v = vHashPre.find( v );
            if ( it_v != vHashPre.end() )
            {
               row.insert( it_v->second, min );
            }

            // ---

			ITERA_GGROUP_LESSPTR( itUnidade, itCampus->unidades, Unidade )
			{
				ITERA_GGROUP_LESSPTR( itCjtSala, itUnidade->conjutoSalas, ConjuntoSala )
				{							
					if ( itCjtSala->disciplinas_associadas.find( disciplina ) ==
						itCjtSala->disciplinas_associadas.end() )
					{
						continue;
					}

					GGroup< Oferta *, LessPtr< Oferta > > ofertas =
					problemData->ofertasDisc[ disciplina->getId() ];
					
					ITERA_GGROUP_LESSPTR( itOft, ofertas, Oferta )
					{
					   if ( itOft->campus->getId() == itCampus->getId() )
					   {            
						  v.reset();
						  v.setType( VariablePre::V_PRE_ALUNOS );

						  v.setTurma( turma );
						  v.setDisciplina( disciplina );
						  v.setOferta( *itOft );
						  v.setUnidade( *itUnidade );
						  v.setSubCjtSala( *itCjtSala );

						  it_v = vHashPre.find( v );
						  if ( it_v != vHashPre.end() )
						  {
							 row.insert( it_v->second, -1.0 );
						  }
					   }
					}
				}
			}

            if ( row.getnnz() != 0 )
            {
               cHashPre[ c ] = lp->getNumRows();
               lp->addRow( row );
               restricoes++;
            }
         }
      }
   }

   return restricoes;
}
   
// Restricao 1.13
int SolverMIP::cria_preRestricao_abre_turmas_em_sequencia( int campusId )
{
   int restricoes = 0;
   char name[ 100 ];
   int nnz;

   VariablePre v;
   ConstraintPre c;
   VariablePreHash::iterator it_v;

   Disciplina * disciplina = NULL;

   ITERA_GGROUP_LESSPTR( it_disciplina, problemData->disciplinas, Disciplina )
   {
      disciplina = ( *it_disciplina );

	  if ( problemData->cp_discs[ campusId ].find( disciplina->getId() ) ==
		   problemData->cp_discs[ campusId ].end() )
	  {
		  continue;
 	  }

	  #pragma region Equivalencias
	  if ( problemData->mapDiscSubstituidaPor.find( disciplina ) !=
		   problemData->mapDiscSubstituidaPor.end() )
	  {
		  continue;
	  }
	  #pragma endregion

      if ( disciplina->getNumTurmas() > 1 )
      {
         for ( int turma = 0; turma < ( disciplina->getNumTurmas() - 1 ); turma++ )
         {
            c.reset();
            c.setType( ConstraintPre::C_PRE_ABRE_TURMAS_EM_SEQUENCIA );

            c.setDisciplina( disciplina );
            c.setTurma( turma );

            sprintf( name, "%s", c.toString().c_str() ); 
            if ( cHashPre.find( c ) != cHashPre.end() )
            {
               continue;
            }

			nnz = 2*disciplina->getNSalasAptas();
            OPT_ROW row( nnz, OPT_ROW::GREATER , 0.0, name );

            ITERA_GGROUP_LESSPTR( itCampus, problemData->campi, Campus )
            {
				if ( itCampus->getId() != campusId )
				{
					continue;
				}

               ITERA_GGROUP_LESSPTR( itUnidade, itCampus->unidades, Unidade )
               {
                  ITERA_GGROUP_LESSPTR( itCjtSala, itUnidade->conjutoSalas, ConjuntoSala )
                  {
                        v.reset();
                        v.setType( VariablePre::V_PRE_OFERECIMENTO );

                        v.setTurma( turma );
                        v.setDisciplina( disciplina );
                        v.setUnidade( *itUnidade );
                        v.setSubCjtSala( *itCjtSala );

                        it_v = vHashPre.find( v );
                        if ( it_v != vHashPre.end() )
                        {
                           row.insert( it_v->second, 1.0 );
                        }

                        v.reset();
                        v.setType( VariablePre::V_PRE_OFERECIMENTO );

                        int turmaSuc = turma + 1;
                        v.setTurma(turmaSuc);
                        v.setDisciplina( disciplina );
                        v.setUnidade( *itUnidade );
                        v.setSubCjtSala( *itCjtSala );

                        it_v = vHashPre.find( v );
                        if ( it_v != vHashPre.end() )
                        {
                           row.insert( it_v->second, -1.0 );
                        }
                  }
               }
            }

            if ( row.getnnz() != 0 )
            {
               cHashPre[ c ] = lp->getNumRows();

               lp->addRow( row );
               restricoes++;
            }
         }
      }
   }

   return restricoes;
}


// Restri��o 1.14
int SolverMIP::cria_preRestricao_turma_mesma_disc_sala_dif( int campusId )
{
   int restricoes = 0;
   char name[ 100 ];
   int nnz;

   VariablePre v;
   ConstraintPre c;
   VariablePreHash::iterator it_v;

   Disciplina * disciplina = NULL;

	ITERA_GGROUP_LESSPTR( itCampus, problemData->campi, Campus )
	{
		Campus* cp = *itCampus;
				
		if ( cp->getId() != campusId )
		{
			continue;
		}

		ITERA_GGROUP_LESSPTR( itUnidade, itCampus->unidades, Unidade )
		{
			ITERA_GGROUP_LESSPTR( itCjtSala, itUnidade->conjutoSalas, ConjuntoSala )
			{
				ITERA_GGROUP_LESSPTR( itDisc, itCjtSala->disciplinas_associadas, Disciplina )
				{
					  Disciplina* disciplina = ( *itDisc );
					  
					  #pragma region Equivalencias
					  if ( problemData->mapDiscSubstituidaPor.find( disciplina ) !=
						   problemData->mapDiscSubstituidaPor.end() )
					  {
						  continue;
					  }
					  #pragma endregion
					  
					  if ( disciplina->getNumTurmas() > 1 )
					  {
							// Listando todas as ofertas que contem uma disciplina especificada.
							GGroup< Oferta *, LessPtr< Oferta > > ofertas = problemData->ofertasDisc[ disciplina->getId() ];

							ITERA_GGROUP_LESSPTR( itOferta, ofertas, Oferta )
							{
								  Oferta* oft = *itOferta;

								  if ( oft->campus != cp )
									  continue;

								  // Calculando P_{d,o}
								  int qtdDem = 0;
								  ITERA_GGROUP_LESSPTR( itDem, problemData->demandas, Demanda )
								  {
										if ( itDem->disciplina->getId() == disciplina->getId() &&
											 itDem->getOfertaId() == itOferta->getId() )
										{
											qtdDem += itDem->getQuantidade();
										}
								  }

								  if ( qtdDem <= 0 )
								  {
										continue;
								  }

								  c.reset();
								  c.setType( ConstraintPre::C_PRE_TURMA_MESMA_DISC_SALA_DIF );

								  c.setDisciplina( disciplina );
								  c.setUnidade( *itUnidade );
								  c.setSubCjtSala( *itCjtSala );
								  c.setOferta( oft );

								  sprintf( name, "%s", c.toString().c_str() ); 
								  if ( cHashPre.find( c ) != cHashPre.end() )
								  {
									  continue;
								  }

								  nnz = disciplina->getNumTurmas() + 1;

								  OPT_ROW row( nnz, OPT_ROW::LESS , 1.0, name );
						  
								  for ( int turma = 0; turma < disciplina->getNumTurmas(); turma++ )
								  {
										v.reset();
										v.setType( VariablePre::V_PRE_ALOC_ALUNO_OFT);
										v.setTurma( turma );
										v.setDisciplina( *itDisc );
										v.setUnidade( *itUnidade );
										v.setSubCjtSala( *itCjtSala );
										v.setOferta( oft );

										it_v = vHashPre.find( v );
										if ( it_v != vHashPre.end() )
										{
										   row.insert( it_v->second, 1.0 );
										}
								  }

								  v.reset();
								  v.setType( VariablePre::V_PRE_SLACK_SALA);
								  v.setDisciplina( *itDisc );
								  v.setUnidade( *itUnidade );
								  v.setSubCjtSala( *itCjtSala );
								  v.setOferta( oft );

								  it_v = vHashPre.find( v );
								  if ( it_v != vHashPre.end() )
								  {
									  row.insert( it_v->second, 1.0 );
								  }

								  if ( row.getnnz() != 0 )
								  {
									 cHashPre[ c ] = lp->getNumRows();

  									 lp->addRow( row );
									 restricoes++;
								  }
							  }
					  }
				}
			}
		}
	}

	return restricoes;
}

// Restri��o 1.15
int SolverMIP::cria_preRestricao_limite_sup_creds_sala( int campusId )
{
   int restricoes = 0;
   char name[ 100 ];
   int nnz;

   VariablePre v;
   ConstraintPre c;
   VariablePreHash::iterator it_v;

   Disciplina * disciplina = NULL;

	ITERA_GGROUP_LESSPTR( itCampus, problemData->campi, Campus )
	{
		if ( itCampus->getId() != campusId )
		{
			continue;
		}

		ITERA_GGROUP_LESSPTR( itUnidade, itCampus->unidades, Unidade )
		{
			ITERA_GGROUP_LESSPTR( itCjtSala, itUnidade->conjutoSalas, ConjuntoSala )
			{
				// --------------------------------------------------------
				c.reset();
				c.setType( ConstraintPre::C_PRE_LIM_SUP_CREDS_SALA );
				c.setCampus( *itCampus );
				c.setUnidade( *itUnidade );
				c.setSubCjtSala( *itCjtSala );

				sprintf( name, "%s", c.toString().c_str() ); 
				if ( cHashPre.find( c ) != cHashPre.end() )
				{
					continue;
				}

				nnz = 100 + 1;

				OPT_ROW row( nnz, OPT_ROW::LESS , 0.0, name );
						  
				v.reset();
				v.setType( VariablePre::V_PRE_LIM_SUP_CREDS_SALA);
				v.setCampus( *itCampus );

				it_v = vHashPre.find( v );
				if ( it_v != vHashPre.end() )
				{
					row.insert( it_v->second, -1.0 );
				}

				ITERA_GGROUP_LESSPTR( itDisc, itCjtSala->disciplinas_associadas, Disciplina )
				{
					  Disciplina* disciplina = ( *itDisc );
					  
					  #pragma region Equivalencias
					  if ( problemData->mapDiscSubstituidaPor.find( disciplina ) !=
						   problemData->mapDiscSubstituidaPor.end() )
					  {
							continue;
					  }
					  #pragma endregion
					  
					  for ( int turma = 0; turma < disciplina->getNumTurmas(); turma++ )
					  {
						  v.reset();
						  v.setType( VariablePre::V_PRE_CREDITOS);
						  v.setUnidade( *itUnidade );
						  v.setSubCjtSala( *itCjtSala );
						  v.setDisciplina( disciplina );
						  v.setTurma( turma );

						  it_v = vHashPre.find( v );
						  if ( it_v != vHashPre.end() )
						  {
							  row.insert( it_v->second, 1.0 );
						  }
					  }
				}

				if ( row.getnnz() != 0 )
				{
					cHashPre[ c ] = lp->getNumRows();

  					lp->addRow( row );
					restricoes++;
				}
				// --------------------------------------------------------
			}
		}
	}

	return restricoes;
}

// Restricao 1.16
int SolverMIP::cria_preRestricao_ativa_var_aloc_aluno_oft( int campusId )
{
    int restricoes = 0;
    char name[ 100 ];
    int nnz;

    VariablePre v;
    ConstraintPre c;
    VariablePreHash::iterator it_v;

    Disciplina * disciplina = NULL;
    
	ITERA_GGROUP_LESSPTR( itCampus, problemData->campi, Campus )
	{
		Campus* cp = *itCampus;

		if ( cp->getId() != campusId )
		{
			continue;
		}
		
		ITERA_GGROUP_LESSPTR( itUnidade, itCampus->unidades, Unidade )
		{
			ITERA_GGROUP_LESSPTR( itCjtSala, itUnidade->conjutoSalas, ConjuntoSala )
			{
				ITERA_GGROUP_LESSPTR( it_disc, itCjtSala->disciplinas_associadas, Disciplina )
				{
					Disciplina* disciplina = *it_disc;

					#pragma region Equivalencias
					if ( problemData->mapDiscSubstituidaPor.find( disciplina ) !=
						problemData->mapDiscSubstituidaPor.end() )
					{
						continue;
					}
					#pragma endregion

					// Listando todas as ofertas que contem uma disciplina especificada.
					GGroup< Oferta *, LessPtr< Oferta > > ofertas = problemData->ofertasDisc[ it_disc->getId() ];

					ITERA_GGROUP_LESSPTR( itOferta, ofertas, Oferta )
					{
							 Oferta* oft = *itOferta;

							 if ( oft->campus != cp )
								 continue;

							 // Calculando P_{d,o}
							 int qtdDem = 0;
							 ITERA_GGROUP_LESSPTR( itDem, problemData->demandas, Demanda )
							 {
								if ( itDem->disciplina->getId() == disciplina->getId() &&
								   itDem->getOfertaId() == itOferta->getId() )
								{
								   qtdDem += itDem->getQuantidade();
								}
							 }

							 if ( qtdDem <= 0 )
							 {
								continue;
							 }

							 for ( int turma = 0; turma < disciplina->getNumTurmas(); turma++ )
							 {
								// --------------------------------------------------------
								c.reset();
								c.setType( ConstraintPre::C_PRE_ATIVA_C );
								c.setCampus( *itCampus );
								c.setTurma( turma );
								c.setDisciplina( disciplina );
								c.setUnidade( *itUnidade );
								c.setSubCjtSala( *itCjtSala );
								c.setOferta( oft );

								sprintf( name, "%s", c.toString().c_str() ); 
								if ( cHashPre.find( c ) != cHashPre.end() )
								{
									continue;
								}

								nnz = 2;

								OPT_ROW row( nnz, OPT_ROW::LESS , 0.0, name );

								// variavel a_{i,d,oft,s}
								v.reset();
								v.setType( VariablePre::V_PRE_ALUNOS );
								v.setTurma( turma );               // i
								v.setDisciplina( disciplina );     // d
								v.setOferta( *itOferta );          // oft
								v.setUnidade( *itUnidade );		   // u
								v.setSubCjtSala( *itCjtSala );	   // s
								
								it_v = vHashPre.find( v );
								if ( it_v != vHashPre.end() )
								{
									row.insert( it_v->second, 1.0 );
								}

								// variavel c_{i,d,oft,s}
								v.reset();
								v.setType( VariablePre::V_PRE_ALOC_ALUNO_OFT );
								v.setTurma( turma );               // i
								v.setDisciplina( disciplina );     // d
								v.setOferta( *itOferta );          // oft
								v.setUnidade( *itUnidade );		   // u
								v.setSubCjtSala( *itCjtSala );	   // s
								
								it_v = vHashPre.find( v );
								if ( it_v != vHashPre.end() )
								{
									row.insert( it_v->second, -qtdDem );
								}

								// insere constraint
								if ( row.getnnz() != 0 )
								{
									cHashPre[ c ] = lp->getNumRows();

  									lp->addRow( row );
									restricoes++;
								}
								// --------------------------------------------------------
							}
				     }
				}
			}
		}
   }

	return restricoes;
}


// Restricao 1.17
int SolverMIP::cria_preRestricao_fixa_nao_compartilhamento( int campusId )
{
   int restricoes = 0;

   // Metodo somente utilizado quando h� 2 semanas letivas
   if ( problemData->calendarios.size() < 2 )
   {
	   return restricoes;
   }   
   
   char name[ 200 ];
   int nnz;

   ConstraintPre c;
   VariablePre v;
   VariablePreHash::iterator it_v;
   
   // para cada campus
   ITERA_GGROUP_LESSPTR( itCampus, problemData->campi, Campus )
   {
	   Campus *cp = *itCampus;

	   if ( itCampus->getId() != campusId )
	   {
			continue;
	   }

	   // para cada par de cursos compativeis
	   std::map< std::pair< Curso *, Curso * >, std::vector< int > >::iterator
               it_cursoComp_disc = problemData->cursosComp_disc.begin();
        for (; it_cursoComp_disc != problemData->cursosComp_disc.end(); it_cursoComp_disc++ )
        {
			Curso* c1 = it_cursoComp_disc->first.first;
			Curso* c2 = it_cursoComp_disc->first.second;
			
			// para cada disciplina em comum (possivel de ser compartilhada) ao par de cursos
            std::vector< int >::iterator it_discComum = it_cursoComp_disc->second.begin();
            for (; it_discComum != it_cursoComp_disc->second.end(); ++it_discComum )
            {
				Disciplina * discComum = problemData->retornaDisciplina( *it_discComum );
				  
				if (discComum == NULL) continue;

				#pragma region Equival�ncia de disciplinas
				if ( problemData->mapDiscSubstituidaPor.find( discComum ) !=
					 problemData->mapDiscSubstituidaPor.end() )
				{
					continue;
				}
				#pragma endregion
				
				GGroup<Oferta*, LessPtr<Oferta>> ofts1 = cp->retornaOfertasComCursoDisc( c1->getId(), discComum );
				GGroup<Oferta*, LessPtr<Oferta>> ofts2 = cp->retornaOfertasComCursoDisc( c2->getId(), discComum );

				// para cada oferta contendo discComum do curso c1
				ITERA_GGROUP_LESSPTR( itOft1, ofts1, Oferta )
				{
					Oferta *oft1 = *itOft1;
					int periodo1 = oft1->periodoDisciplina( discComum );

					if ( oft1->getCampusId() != campusId )
					{
						continue;
					}

					// para cada oferta contendo discComum do curso c2
					ITERA_GGROUP_LESSPTR( itOft2, ofts2, Oferta )
					{
						Oferta *oft2 = *itOft2;
						int periodo2 = oft2->periodoDisciplina( discComum );

						if ( oft2->getId() == oft1->getId() )
						{
							continue;
						}
						if ( oft2->getCampusId() != campusId )
						{
							continue;
						}

						// Cria a restri��o somente para per�odos que possuem mais de uma semana letiva, ou sl's diferentes
						if ( oft1->curriculo->retornaSemanasLetivasNoPeriodo( periodo1 ).size() == 1 &&
							 oft2->curriculo->retornaSemanasLetivasNoPeriodo( periodo2 ).size() == 1 &&
							 oft1->curriculo->retornaSemanasLetivasNoPeriodo( periodo1 ).begin()->getId() ==
							 oft2->curriculo->retornaSemanasLetivasNoPeriodo( periodo2 ).begin()->getId() )
						{
							continue;
						}

						// para cada turma da disciplina em comum
						for ( int turma = 0; turma < discComum->getNumTurmas(); turma++ )
						{
							c.reset();
							c.setType( ConstraintPre::C_PRE_FIXA_NAO_COMPARTILHAMENTO );
							c.setParOfertas( std::make_pair(oft1, oft2) );
							c.setDisciplina( discComum );
							c.setTurma( turma );

							sprintf( name, "%s", c.toString().c_str() ); 

							if ( cHashPre.find( c ) != cHashPre.end() )
							{
								continue;
							}

							nnz = 300;

							OPT_ROW row( nnz, OPT_ROW::LESS , 1.0 , name );

							ITERA_GGROUP_LESSPTR( itUnidade, cp->unidades, Unidade )
							{
								ITERA_GGROUP_LESSPTR( itCjtSala, itUnidade->conjutoSalas, ConjuntoSala )
								{
									if ( itCjtSala->disciplinas_associadas.find( discComum ) ==
										 itCjtSala->disciplinas_associadas.end() )
										continue;

									v.reset();
									v.setType( VariablePre::V_PRE_ALOC_ALUNO_OFT );
									v.setTurma( turma );
									v.setDisciplina( discComum );
									v.setOferta( oft1 );
									v.setSubCjtSala( *itCjtSala );
									v.setUnidade( *itUnidade );

									it_v = vHashPre.find( v );
									if( it_v != vHashPre.end() )
									{
										row.insert( it_v->second, 1 );
									}

									v.reset();
									v.setType( VariablePre::V_PRE_ALOC_ALUNO_OFT );
									v.setTurma( turma );
									v.setDisciplina( discComum );
									v.setOferta( oft2 );
									v.setSubCjtSala( *itCjtSala );
									v.setUnidade( *itUnidade );

									it_v = vHashPre.find( v );
									if( it_v != vHashPre.end() )
									{
										row.insert( it_v->second, 1 );
									}
								}
							}
								
							if ( row.getnnz() != 0 )
							{
								cHashPre[ c ] = lp->getNumRows();
								lp->addRow( row );
								restricoes++;
							}
						}
					}
				}
			}
		}
   }

   return restricoes;

}


// Restricao 1.18
/*
    Metodo somente utilizado para o modelo Tatico_Aluno

	Define qual aluno foi atendido em cada atendimento feito.
*/
int SolverMIP::cria_preRestricao_atendimento_aluno( int campusId )
{
    int restricoes = 0;
   
    char name[ 100 ];
    int nnz = 0;

    ConstraintPre c;
    VariablePre v;
    VariablePreHash::iterator it_v;

    Disciplina * disciplina = NULL;

   ITERA_GGROUP_LESSPTR( itOferta, problemData->ofertas, Oferta )
   {
	   Oferta *oferta = *itOferta;

	   if ( oferta->getCampusId() != campusId )
	   {
			continue;
	   }

       map < Disciplina*, int, LessPtr< Disciplina > >::iterator itPrdDisc = 
         oferta->curriculo->disciplinas_periodo.begin();

      for (; itPrdDisc != oferta->curriculo->disciplinas_periodo.end(); itPrdDisc++ )
      {
		  disciplina = itPrdDisc->first;

		  for ( int turma = 0; turma < disciplina->getNumTurmas(); turma++ )
		  {
				 c.reset();
				 c.setType( ConstraintPre::C_ATENDIMENTO_ALUNO );

				 c.setOferta( oferta );
				 c.setDisciplina( disciplina );
				 c.setTurma( turma );

				 sprintf( name, "%s", c.toString().c_str() ); 
				 if ( cHashPre.find( c ) != cHashPre.end() )
				 {
					continue;
				 }
				 
				 nnz = 50;
				 
				 OPT_ROW row( nnz , OPT_ROW::EQUAL, 0.0 , name );

				 ITERA_GGROUP_LESSPTR( itAluno, problemData->alunos, Aluno )
				 {
					 Aluno *aluno = *itAluno;

					 if ( aluno->getOferta() != oferta )
					 {
						continue;
					 }

					 if ( !aluno->demandaDisciplina( disciplina->getId() ) )
					 {
						continue;
					 }

					 v.reset();
					 v.setType( VariablePre::V_PRE_ALOCA_ALUNO_TURMA_DISC );
					 v.setTurma( turma );
					 v.setDisciplina( disciplina );
					 v.setAluno( aluno );
					 v.setCampus( oferta->campus );

					 it_v = vHashPre.find( v );
					 if ( it_v != vHashPre.end() )
					 {
						 row.insert( it_v->second, -1.0 );
					 }
				 }

				 ITERA_GGROUP_LESSPTR( itCampus, problemData->campi, Campus )
				 {
					if ( itCampus->getId() != campusId )
					{
						continue;
					}
							
					ITERA_GGROUP_LESSPTR( itUnidade, itCampus->unidades, Unidade )
					{
						ITERA_GGROUP_LESSPTR( itCjtSala, itUnidade->conjutoSalas, ConjuntoSala )
						{
							if ( itCjtSala->disciplinas_associadas.find( disciplina ) ==
								 itCjtSala->disciplinas_associadas.end() )
							{
								continue;
							}
			
							v.reset();
							v.setType( VariablePre::V_PRE_ALUNOS );
							v.setTurma( turma );
							v.setDisciplina( disciplina );
							v.setOferta( oferta );
							v.setUnidade( *itUnidade );
							v.setSubCjtSala( *itCjtSala );

							it_v = vHashPre.find( v );
							if ( it_v != vHashPre.end() )
							{
								row.insert( it_v->second, 1.0 );
							}
						}
					}
				 }

				 if ( row.getnnz() != 0 )
				 {
					cHashPre[ c ] = lp->getNumRows();

					lp->addRow( row );
					restricoes++;
				 }
		  }
      }
   }

	return restricoes;
}


// Restricao 1.19
/*
    Metodo somente utilizado para o modelo Tatico_Aluno
   
	Garante que cada aluno esteja em apenas 1 turma de uma disciplina
*/
int SolverMIP::cria_preRestricao_aluno_unica_turma_disc( int campusId )
{
   int restricoes = 0;
      
   char name[ 200 ];
   int nnz;

   ConstraintPre c;
   VariablePre v;
   VariablePreHash::iterator it_v;   

   Campus *campus = problemData->refCampus[campusId];

	ITERA_GGROUP_LESSPTR( itAluno, problemData->alunos, Aluno )
	{
		Aluno *aluno = *itAluno;

		if ( aluno->getOferta()->getCampusId() != campusId )
		{
			continue;
		}

		ITERA_GGROUP_LESSPTR( itAlDemanda, aluno->demandas, AlunoDemanda )
		{
			Disciplina *disciplina = itAlDemanda->demanda->disciplina;

			c.reset();
			c.setType( ConstraintPre::C_ALUNO_UNICA_TURMA_DISC );
			c.setAluno( aluno );
			c.setDisciplina( disciplina );
			c.setCampus( campus );

			sprintf( name, "%s", c.toString().c_str() ); 

			if ( cHashPre.find( c ) != cHashPre.end() )
			{
				continue;
			}

			nnz = disciplina->getNumTurmas();

			OPT_ROW row( nnz, OPT_ROW::LESS , 1.0 , name );

			for ( int turma = 0; turma < disciplina->getNumTurmas(); turma++ )
			{
				VariablePre v;
				v.reset();
				v.setType( VariablePre::V_PRE_ALOCA_ALUNO_TURMA_DISC );
				v.setAluno( aluno );
				v.setDisciplina( disciplina );
				v.setTurma( turma );
				v.setCampus( campus );

				it_v = vHashPre.find( v );
				if( it_v != vHashPre.end() )
				{
					row.insert( it_v->second, 1.0 );
				}
			}

			if ( row.getnnz() != 0 )
			{
				cHashPre[ c ] = lp->getNumRows();
				lp->addRow( row );
				restricoes++;
			}
		}
	}

	return restricoes;
}


// Restricao 1.20
int SolverMIP::cria_preRestricao_aluno_discPraticaTeorica( int campusId )
{
   int restricoes = 0;
      
   char name[ 200 ];
   int nnz;

   ConstraintPre c;
   VariablePre v;
   VariablePreHash::iterator it_v;   

   Campus *campus = problemData->refCampus[campusId];

	ITERA_GGROUP_LESSPTR( itAluno, problemData->alunos, Aluno )
	{
		Aluno *aluno = *itAluno;

		if ( aluno->getOferta()->getCampusId() != campusId )
		{
			continue;
		}

		ITERA_GGROUP_LESSPTR( itAlDemanda, aluno->demandas, AlunoDemanda )
		{
			Disciplina *discPratica = itAlDemanda->demanda->disciplina;
			
			// Pula disciplina teorica
			if ( discPratica->getId() > 0 )
				continue;

			Disciplina *discTeorica = problemData->refDisciplinas[ - discPratica->getId() ];

			if ( discTeorica == NULL )
			{
				std::cout<<"\nErro em cria_preRestricao_aluno_discPraticaTeorica: disciplina teorica nao encontrada.\n";
				continue;
			}
			
			c.reset();
			c.setType( ConstraintPre::C_ALUNO_DISC_PRATICA_TEORICA );
			c.setAluno( aluno );
			c.setDisciplina( discPratica );
			c.setCampus( campus );

			sprintf( name, "%s", c.toString().c_str() ); 

			if ( cHashPre.find( c ) != cHashPre.end() )
			{
				continue;
			}

			nnz = discPratica->getNumTurmas() + discTeorica->getNumTurmas() + 1;

			OPT_ROW row( nnz, OPT_ROW::EQUAL , 0.0 , name );

			for ( int turma = 0; turma < discPratica->getNumTurmas(); turma++ )
			{
				VariablePre v;
				v.reset();
				v.setType( VariablePre::V_PRE_ALOCA_ALUNO_TURMA_DISC );
				v.setAluno( aluno );
				v.setDisciplina( discPratica );
				v.setTurma( turma );
				v.setCampus( campus );

				it_v = vHashPre.find( v );
				if( it_v != vHashPre.end() )
				{
					row.insert( it_v->second, 1.0 );
				}
			}

			for ( int turma = 0; turma < discTeorica->getNumTurmas(); turma++ )
			{
				VariablePre v;
				v.reset();
				v.setType( VariablePre::V_PRE_ALOCA_ALUNO_TURMA_DISC );
				v.setAluno( aluno );
				v.setDisciplina( discTeorica );
				v.setTurma( turma );
				v.setCampus( campus );

				it_v = vHashPre.find( v );
				if( it_v != vHashPre.end() )
				{
					row.insert( it_v->second, -1.0 );
				}
			}

			if ( row.getnnz() != 0 )
			{
				cHashPre[ c ] = lp->getNumRows();
				lp->addRow( row );
				restricoes++;
			}
		}
	}

	return restricoes;

}

// Restricao 1.21
int SolverMIP::cria_preRestricao_prioridadesDemanda( int campusId, int prior )
{
   int restricoes = 0;

   if ( prior < 2 )
	   return restricoes;

   char name[ 200 ];
   int nnz;

   ConstraintPre c;
   VariablePre v;
   VariablePreHash::iterator it_v;   

   Campus *campus = problemData->refCampus[campusId];

	ITERA_GGROUP_LESSPTR( itAluno, problemData->alunos, Aluno )
	{
		Aluno *aluno = *itAluno;

		if ( aluno->getOferta()->getCampusId() != campusId )
		{
			continue;
		}
			
		c.reset();
		c.setType( ConstraintPre::C_ALUNO_PRIORIDADES_DEMANDA );
		c.setAluno( aluno );
		c.setCampus( campus );

		sprintf( name, "%s", c.toString().c_str() ); 

		if ( cHashPre.find( c ) != cHashPre.end() )
		{
			continue;
		}

		nnz = aluno->demandas.size()*3 + 2;
		
		double cargaHorariaNaoAtendida = 0.0;
		ITERA_GGROUP_LESSPTR( itAlDemanda, problemData->listSlackDemandaAluno, AlunoDemanda )
		{
			if ( itAlDemanda->getPrioridade() != prior-1 )
				continue;

			if ( itAlDemanda->getAlunoId() == aluno->getAlunoId() )
			{
				int nCreds = itAlDemanda->demanda->disciplina->getTotalCreditos();
				int duracaoCred = itAlDemanda->demanda->disciplina->getTempoCredSemanaLetiva();

				cargaHorariaNaoAtendida += nCreds*duracaoCred;
			}
		}

		OPT_ROW row( nnz, OPT_ROW::EQUAL , cargaHorariaNaoAtendida, name );
		
		ITERA_GGROUP_LESSPTR( itAlDemanda, aluno->demandas, AlunoDemanda )
		{
			Disciplina *disciplina = itAlDemanda->demanda->disciplina;

			if ( itAlDemanda->getPrioridade() != prior )
				continue;

			for ( int turma = 0; turma < disciplina->getNumTurmas(); turma++ )
			{
				VariablePre v;
				v.reset();
				v.setType( VariablePre::V_PRE_ALOCA_ALUNO_TURMA_DISC );
				v.setAluno( aluno );
				v.setDisciplina( disciplina );
				v.setTurma( turma );
				v.setCampus( campus );

				it_v = vHashPre.find( v );
				if( it_v != vHashPre.end() )
				{
					double tempo = disciplina->getTotalCreditos() * disciplina->getTempoCredSemanaLetiva();

					row.insert( it_v->second, tempo );
				}
			}
		}

		VariablePre v;
		v.reset();
		v.setType( VariablePre::V_PRE_SLACK_PRIOR_INF );
		v.setAluno( aluno );
		v.setCampus( campus );
		it_v = vHashPre.find( v );
		if( it_v != vHashPre.end() )
		{
			row.insert( it_v->second, -1.0 );
		}

		v.reset();
		v.setType( VariablePre::V_PRE_SLACK_PRIOR_SUP );
		v.setAluno( aluno );
		v.setCampus( campus );
		it_v = vHashPre.find( v );
		if( it_v != vHashPre.end() )
		{
			row.insert( it_v->second, 1.0 );
		}


		if ( row.getnnz() >= 3 )
		{
			cHashPre[ c ] = lp->getNumRows();
			lp->addRow( row );
			restricoes++;
		}
	}

	return restricoes;

}


/* ----------------------------------------------------------------------------------
	
							VARIAVEIS TATICO POR ALUNO
 ---------------------------------------------------------------------------------- */


int SolverMIP::criaVariaveisTatico( int campusId )
{
	int num_vars = 0;
	CPUTimer timer;
	double dif = 0.0;

#ifdef PRINT_cria_variaveis
	int numVarsAnterior = 0;
#endif


	timer.start();
	num_vars += criaVariavelTaticoCreditos( campusId ); // x
	timer.stop();
	dif = timer.getCronoCurrSecs();
	
#ifdef PRINT_cria_variaveis
	std::cout << "numVars \"x\": " << (num_vars - numVarsAnterior)  <<" "<<dif <<" sec" << std::endl;
	numVarsAnterior = num_vars;
#endif


	timer.start();
	num_vars += criaVariavelTaticoAbertura( campusId ); // z
	timer.stop();
	dif = timer.getCronoCurrSecs();
	
#ifdef PRINT_cria_variaveis
	std::cout << "numVars \"z\": " << (num_vars - numVarsAnterior)  <<" "<<dif <<" sec" << std::endl;
	numVarsAnterior = num_vars;
#endif


	timer.start();
	num_vars += criaVariavelTaticoConsecutivos( campusId ); // c
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_variaveis
	std::cout << "numVars \"c\": " << (num_vars - numVarsAnterior)  <<" "<<dif <<" sec" << std::endl;
	numVarsAnterior = num_vars;
#endif


	timer.start();
	num_vars += criaVariavelTaticoMinCreds( campusId ); // h
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_variaveis
	std::cout << "numVars \"h\": " << (num_vars - numVarsAnterior)  <<" "<<dif <<" sec" << std::endl;
	numVarsAnterior = num_vars;
#endif
	

	timer.start();
	num_vars += criaVariavelTaticoMaxCreds( campusId ); // H
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_variaveis
	std::cout << "numVars \"H\": " << (num_vars - numVarsAnterior)  <<" "<<dif <<" sec" << std::endl;
	numVarsAnterior = num_vars;
#endif


	timer.start();
	num_vars += criaVariavelTaticoCombinacaoDivisaoCredito( campusId ); // m_{i,d,k}
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_variaveis
	std::cout << "numVars \"m\": " << (num_vars - numVarsAnterior)  <<" "<<dif <<" sec" << std::endl;
	numVarsAnterior = num_vars;
#endif

				
	timer.start();
	num_vars += criaVariavelTaticoFolgaCombinacaoDivisaoCredito( campusId ); // fk_{i,d,k}
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_variaveis
	std::cout << "numVars \"fkp e fkm\": " << (num_vars - numVarsAnterior)  <<" "<<dif <<" sec" << std::endl;
	numVarsAnterior = num_vars;
#endif


	timer.start();
	num_vars += criaVariavelTaticoFolgaDistCredDiaSuperior( campusId ); // fcp_{d,t}
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_variaveis
	std::cout << "numVars \"fcp\": " << (num_vars - numVarsAnterior)  <<" "<<dif <<" sec" << std::endl;
	numVarsAnterior = num_vars;
#endif
   		

	timer.start();
	num_vars += criaVariavelTaticoFolgaDistCredDiaInferior( campusId ); // fcm_{d,t}
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_variaveis
	std::cout << "numVars \"fcm\": " << (num_vars - numVarsAnterior)  <<" "<<dif <<" sec" << std::endl;
	numVarsAnterior = num_vars;
#endif
	

	timer.start();
	num_vars += criaVariavelTaticoAberturaCompativel( campusId ); // zc
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_variaveis
	std::cout << "numVars \"zc\": " << (num_vars - numVarsAnterior)  <<" "<<dif <<" sec" << std::endl;
	numVarsAnterior = num_vars;
#endif


	timer.start();
	num_vars += criaVariavelTaticoFolgaDemandaDiscAluno( campusId ); // fd
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_variaveis
	std::cout << "numVars \"fd\": " << (num_vars - numVarsAnterior)  <<" "<<dif <<" sec" << std::endl;
	numVarsAnterior = num_vars;
#endif
		

	timer.start();
	num_vars += criaVariavelTaticoAlunoUnidDia( campusId ); // y_{a,u,t} 
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_variaveis
	std::cout << "numVars \"y\": " << (num_vars - numVarsAnterior)  <<" "<<dif <<" sec" << std::endl;
	numVarsAnterior = num_vars;
#endif
		

	timer.start();
	num_vars += criaVariavelTaticoAlunoUnidadesDifDia( campusId ); // w_{a,t}
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_variaveis
	std::cout << "numVars \"w\": " << (num_vars - numVarsAnterior)  <<" "<<dif <<" sec" << std::endl;
	numVarsAnterior = num_vars;
#endif
	
	return num_vars;

}

// x_{i,d,u,s,hi,hf,t}
int SolverMIP::criaVariavelTaticoCreditos( int campusId )
{
	int numVars = 0;
    
	Disciplina * disciplina = NULL;

    ITERA_GGROUP_LESSPTR( itCampus, problemData->campi, Campus )
    {
	   if ( itCampus->getId() != campusId )
	   {
		   continue;
	   }

      ITERA_GGROUP_LESSPTR( itUnidade, itCampus->unidades, Unidade )
      {
         ITERA_GGROUP_LESSPTR( itCjtSala, itUnidade->conjutoSalas, ConjuntoSala )
         {
			 if ( itCjtSala->salas.size() > 1 )
			 {
				std::cout<<"\nATENCAO em criaVariavelTaticoCreditos: conjunto sala deve ter somente 1 sala! \n";
			 }

			 int salaId = itCjtSala->salas.begin()->first;

            ITERA_GGROUP_LESSPTR( it_disciplina, itCjtSala->disciplinas_associadas, Disciplina )
            {
               disciplina = ( *it_disciplina );
			   
				#pragma region Equivalencias
				if ( problemData->mapDiscSubstituidaPor.find( disciplina ) !=
					 problemData->mapDiscSubstituidaPor.end() )
				{
					continue;
				}
				#pragma endregion
				
			   std::pair< int, int > parDiscSala = std::make_pair( disciplina->getId(), salaId );

			   std::map< std::pair< int /*idDisc*/, int /*idSala*/ >, 
						 std::map< int /*Dia*/, GGroup< HorarioAula *, LessPtr< HorarioAula > > > >::iterator

			   it_Disc_Sala_Dias_HorariosAula = problemData->disc_Salas_Dias_HorariosAula.find( parDiscSala );

			   if ( it_Disc_Sala_Dias_HorariosAula == 
				    problemData->disc_Salas_Dias_HorariosAula.end() )
			   {
				   continue;
			   }

			   for ( int turma = 0; turma < disciplina->getNumTurmas(); turma++ )
               {
				    std::map< int /*Dia*/, GGroup< HorarioAula *, LessPtr< HorarioAula > > >::iterator
						it_Dia_HorarioAula = it_Disc_Sala_Dias_HorariosAula->second.begin();
					
					for ( ; it_Dia_HorarioAula != it_Disc_Sala_Dias_HorariosAula->second.end(); it_Dia_HorarioAula++ )
					{
						int dia = it_Dia_HorarioAula->first;
					  
						ITERA_GGROUP_LESSPTR( itHorario, it_Dia_HorarioAula->second, HorarioAula )
						{
							HorarioAula *hi = *itHorario;

							ITERA_GGROUP_LESSPTR( itHorario, it_Dia_HorarioAula->second, HorarioAula )
							{
								 HorarioAula *hf = *itHorario;

								 if ( hf < hi )
								 {
							 		 continue;
								 }

								 if ( hf == hi )
								 {
									 // N�o permite que uma disciplina com nro par de creditos
									 // tenha uma aula com somente 1 credito
									 if ( disciplina->getTotalCreditos() % 2 == 0 )
									 {
										continue;
									 }
								 }

								 if ( disciplina->getCalendario()->retornaNroCreditosEntreHorarios( hi, hf ) >
									  disciplina->getTotalCreditos() )
								 {
									continue;
								 }

								 VariableTatico v;
								 v.reset();
								 v.setType( VariableTatico::V_CREDITOS );

								 v.setTurma( turma );            // i
								 v.setDisciplina( disciplina );  // d
								 v.setUnidade( *itUnidade );     // u
								 v.setSubCjtSala( *itCjtSala );  // tps  
								 v.setDia( dia );				 // t
								 v.setHorarioAulaInicial( hi );	 // hi
								 v.setHorarioAulaFinal( hf );	 // hf

								 if ( vHashTatico.find( v ) == vHashTatico.end() )
								 {
									if ( !criaVariavelTatico( &v ) )
										continue;

									vHashTatico[ v ] = lp->getNumCols();

									OPT_COL col( OPT_COL::VAR_BINARY, 0.0, 0.0, 1.0,
									   ( char * )v.toString().c_str());

									lp->newCol( col );
									numVars++;
								 }
							}
						}
					}
               }
            }
         }
      }
    }

	return numVars;
}
      
// z_{i,d,cp}
int SolverMIP::criaVariavelTaticoAbertura( int campusId )
{
	int numVars = 0;
   
   Disciplina * disciplina = NULL;
   
   ITERA_GGROUP_LESSPTR( it_campus, problemData->campi, Campus )
   {
	   if ( it_campus->getId() != campusId )
	   {
		   continue;
	   }
	     
	  GGroup< int > disciplinas = problemData->cp_discs[ campusId ];

      ITERA_GGROUP_N_PT( it_disciplina, disciplinas, int )
      {
		  Disciplina * disciplina = problemData->refDisciplinas[ *it_disciplina ];

		 #pragma region Equivalencias
		 if ( problemData->mapDiscSubstituidaPor.find( disciplina ) !=
			  problemData->mapDiscSubstituidaPor.end() )
		 {
			continue;
		 }
		 #pragma endregion

         for ( int turma = 0; turma < disciplina->getNumTurmas(); turma++ )
         {
            VariableTatico v;
            v.reset();
            v.setType( VariableTatico::V_ABERTURA );

            v.setTurma( turma );            // i
            v.setDisciplina( disciplina );  // d
            v.setCampus( *it_campus );	    // cp

            std::pair< int, int > dc
               = std::make_pair( disciplina->getId(), it_campus->getId() );

            if ( problemData->demandas_campus.find( dc )
               == problemData->demandas_campus.end() )
            {
               problemData->demandas_campus[ dc ] = 0;
            }

            if ( vHashTatico.find(v) == vHashTatico.end() )
            {
			   if ( !criaVariavelTatico( &v ) )
					continue;

               lp->getNumCols();
               vHashTatico[v] = lp->getNumCols();

			   double coef = 0.0;

               if ( problemData->parametros->funcao_objetivo == 0 )
               {
				   coef = -10 * it_campus->getCusto() * disciplina->getTotalCreditos();
               }
               else if ( problemData->parametros->funcao_objetivo == 1 )
               {
				   coef = it_campus->getCusto() * disciplina->getTotalCreditos();
               }
                  
			   OPT_COL col( OPT_COL::VAR_BINARY, coef, 0.0, 1.0, ( char * )v.toString().c_str() );

               lp->newCol( col );

               numVars++;
            }
         }
      }
   }
	
	return numVars;
}
   
/*   
// b_{i,d,c,cp}
int SolverMIP::criaVariavelTaticoCursoAlunos( int campusId )
{
	int numVars = 0;
   
	Disciplina * disciplina = NULL;
   
   ITERA_GGROUP_LESSPTR( it_Oferta, problemData->ofertas, Oferta )
   {
       Campus * pt_Campus = it_Oferta->campus;
       Curso * pt_Curso = it_Oferta->curso;

	   if ( pt_Campus->getId() != campusId )
	   {
		   continue;
	   }

       map < Disciplina*, int, LessPtr< Disciplina > >::iterator it_Prd_Disc = 
         it_Oferta->curriculo->disciplinas_periodo.begin();
      for(; it_Prd_Disc != it_Oferta->curriculo->disciplinas_periodo.end();
         it_Prd_Disc++ )
      {
		  disciplina = it_Prd_Disc->first;

		  #pragma region Equivalencias
		  if ( problemData->mapDiscSubstituidaPor.find( disciplina ) !=
			   problemData->mapDiscSubstituidaPor.end() )
		  {
			  continue;
		  }
		  #pragma endregion

         for ( int turma = 0; turma < disciplina->getNumTurmas(); turma++ )
         {
            VariableTatico v;
            v.reset();
            v.setType( VariableTatico::V_ALOC_ALUNO );

            v.setTurma( turma );           // i
            v.setDisciplina( disciplina ); // d
            v.setCurso( pt_Curso );        // c
            v.setCampus( pt_Campus );	    // cp

            if ( vHashTatico.find( v ) == vHashTatico.end() )
            {
			   if ( !criaVariavelTatico( &v ) )
					continue;

               vHashTatico[v] = lp->getNumCols();
				
			   double coef = 0.0;

				if ( problemData->parametros->funcao_objetivo == 0 )
				{
					coef = -1.0;
				}
				else if ( problemData->parametros->funcao_objetivo == 1 )
				{
					coef = 1.0;
				}

               OPT_COL col( OPT_COL::VAR_BINARY, coef, 0.0, 1.0,
                  ( char * )v.toString().c_str() );

               lp->newCol( col );
               numVars++;
            }
         }
      }
   }
	
	return numVars;
}
   
*/

/*====================================================================/
%DocBegin TRIEDA_LOAD_MODEL

%Var c_{i,d,t} 

%Desc 
indica se houve abertura de turma $i$ da disciplina $d$ em dias consecutivos.

%ObjCoef
\delta \cdot \sum\limits_{d \in D} 
\sum\limits_{i \in I_{d}} \sum\limits_{t \in T-{1}} c_{i,d,t}

%Data \delta
%Desc
peso associado a fun��o objetivo.

%DocEnd
/====================================================================*/
int SolverMIP::criaVariavelTaticoConsecutivos( int campusId )
{
	int numVars = 0;

   Disciplina * disciplina = NULL;

   ITERA_GGROUP_LESSPTR( it_campus, problemData->campi, Campus )
   {
	  if ( it_campus->getId() != campusId )
	  {
		  continue;
	  }

      ITERA_GGROUP_LESSPTR( it_unidade, it_campus->unidades, Unidade )
      {
         ITERA_GGROUP_LESSPTR( it_conjunto_sala, it_unidade->conjutoSalas, ConjuntoSala )
         {
            ITERA_GGROUP_LESSPTR( it_disciplina, it_conjunto_sala->disciplinas_associadas, Disciplina )
            {
               disciplina = ( *it_disciplina );

				#pragma region Equivalencias
				if ( problemData->mapDiscSubstituidaPor.find( disciplina ) !=
					problemData->mapDiscSubstituidaPor.end() )
				{
					continue;
				}
				#pragma endregion

               for ( int turma = 0; turma < disciplina->getNumTurmas(); turma++ )
               {
                  // Dada uma disciplina, recupera os seus dias letivos, observando as fixa��es
                  GGroup< int > dias_letivos
                     = it_conjunto_sala->dias_letivos_disciplinas[ ( disciplina ) ];

                  GGroup< int >::iterator itDiasLetDisc = dias_letivos.begin();

                  for (; itDiasLetDisc != dias_letivos.end(); itDiasLetDisc++ )
                  {
                     VariableTatico v;
                     v.reset();
                     v.setType( VariableTatico::V_DIAS_CONSECUTIVOS );

                     v.setTurma( turma );            // i
                     v.setDisciplina( disciplina );  // d
                     v.setDia( *itDiasLetDisc );     // t

                     if ( vHashTatico.find( v ) == vHashTatico.end() )
                     {
						if ( !criaVariavelTatico( &v ) )
							continue;

                        vHashTatico[v] = lp->getNumCols();
					    
						double coef = 0.0;

						if ( problemData->parametros->funcao_objetivo == 0 )
						{
							coef = -1.0;
						}
						else if ( problemData->parametros->funcao_objetivo == 1 )
						{
							coef = 1.0;
						}
                           
						OPT_COL col( OPT_COL::VAR_BINARY, coef, 0.0, 1.0, ( char * )v.toString().c_str() );
						                           
						lp->newCol( col );

                        numVars++;
                     }
                  }
               }
            }
         }
      }
   }

	return numVars;
}

// h_{a}
int SolverMIP::criaVariavelTaticoMinCreds( int campusId )
{
   int numVars = 0;
   
   if ( problemData->parametros->carga_horaria_semanal_aluno == ParametrosPlanejamento::INDIFERENTE )
   {
		return numVars;
   }

   Campus *cp = problemData->refCampus[campusId];

   ITERA_GGROUP_LESSPTR( itAluno, problemData->alunos, Aluno )
   {
	    Aluno *aluno = *itAluno;

	    if ( aluno->getOferta()->getCampusId() != campusId )
	    {
			continue;
	    }

		VariableTatico v;
		v.reset();
		v.setType( VariableTatico::V_MIN_CRED_SEMANA );
		v.setAluno( aluno );	// a

		if ( vHashTatico.find( v ) == vHashTatico.end() )
		{
			if ( !criaVariavelTatico( &v ) )
				continue;

			vHashTatico[ v ] = lp->getNumCols();

			double obj = 0.0;

            if ( problemData->parametros->funcao_objetivo == 0 )
            {
                if ( problemData->parametros->carga_horaria_semanal_aluno == ParametrosPlanejamento::EQUILIBRAR)
                {
					obj = cp->getCusto()/4;
                }
                else if ( problemData->parametros->carga_horaria_semanal_aluno == ParametrosPlanejamento::MINIMIZAR_DIAS)
                {
                    obj = cp->getCusto()/4;
                }
            }
            else if ( problemData->parametros->funcao_objetivo == 1 )
            {
                if ( problemData->parametros->carga_horaria_semanal_aluno == ParametrosPlanejamento::EQUILIBRAR)
                {
                    obj = cp->getCusto()/4;
                }
                else if ( problemData->parametros->carga_horaria_semanal_aluno == ParametrosPlanejamento::MINIMIZAR_DIAS)
                {
                    obj = cp->getCusto()/4;
                }
            }
               
			OPT_COL col( OPT_COL::VAR_INTEGRAL, obj, 0.0, 1000.0, ( char * )v.toString().c_str() );

			lp->newCol( col );
			numVars++;
		}
	}

    return numVars;
}

// H_{a}
int SolverMIP::criaVariavelTaticoMaxCreds( int campusId )
{
   int numVars = 0;
   
   if ( problemData->parametros->carga_horaria_semanal_aluno == ParametrosPlanejamento::INDIFERENTE )
   {
		return numVars;
   }

   Campus *cp = problemData->refCampus[campusId];

   ITERA_GGROUP_LESSPTR( itAluno, problemData->alunos, Aluno )
   {
	    Aluno *aluno = *itAluno;

	    if ( aluno->getOferta()->getCampusId() != campusId )
	    {
			continue;
	    }

		VariableTatico v;
		v.reset();
		v.setType( VariableTatico::V_MAX_CRED_SEMANA );

		v.setAluno( aluno );	// a

		if ( vHashTatico.find( v ) == vHashTatico.end() )
		{
			if ( !criaVariavelTatico( &v ) )
				continue;

			vHashTatico[ v ] = lp->getNumCols();

			double obj = 0.0;

            if ( problemData->parametros->funcao_objetivo == 0 )
            {
                if ( problemData->parametros->carga_horaria_semanal_aluno == ParametrosPlanejamento::EQUILIBRAR)
                {
					obj = - cp->getCusto()/4;
                }
                else if ( problemData->parametros->carga_horaria_semanal_aluno == ParametrosPlanejamento::MINIMIZAR_DIAS)
                {
                    obj = 0.0;
                }
            }
            else if ( problemData->parametros->funcao_objetivo == 1 )
            {
                if ( problemData->parametros->carga_horaria_semanal_aluno == ParametrosPlanejamento::EQUILIBRAR)
                {
                    obj = - cp->getCusto()/4;
                }
                else if ( problemData->parametros->carga_horaria_semanal_aluno == ParametrosPlanejamento::MINIMIZAR_DIAS)
                {
                    obj = 0.0;
                }
            }
               
			OPT_COL col( OPT_COL::VAR_INTEGRAL, obj, 0.0, 1000.0, ( char * )v.toString().c_str() );

			lp->newCol( col );
			numVars++;
		}
	}

    return numVars;
}


/*====================================================================/
%DocBegin TRIEDA_LOAD_MODEL

%Var fc_{i,d,c,c',cp}

%Desc 
vari�vel de folga para a restri��o em que o compartilhamento de turmas 
de alunos de cursos compativeis � proibido.

%DocEnd
/====================================================================*/
/*
int SolverMIP::criaVariavelTaticoFolgaCompartilhamento( int campusId )
{
   int numVars = 0;

   if ( problemData->parametros->permite_compartilhamento_turma_sel )
   {
		return numVars;
   }

   ITERA_GGROUP_LESSPTR( itCampus, problemData->campi, Campus )
   {
	   if ( itCampus->getId() != campusId )
	   {
		   continue;
	   }

	   std::map< std::pair< Curso *, Curso * >, std::vector< int > >::iterator
               it_cursoComp_disc = problemData->cursosComp_disc.begin();

        for (; it_cursoComp_disc != problemData->cursosComp_disc.end(); it_cursoComp_disc++ )
        {
			Curso* c1 = it_cursoComp_disc->first.first;
			Curso* c2 = it_cursoComp_disc->first.second;

			if ( itCampus->cursos.find( c1 ) == itCampus->cursos.end() ||
				 itCampus->cursos.find( c2 ) == itCampus->cursos.end() )
			{
				continue;
			}
         
			std::vector< int >::iterator it_disc = it_cursoComp_disc->second.begin();

            for (; it_disc != it_cursoComp_disc->second.end(); ++it_disc )
            {
				int discId = *it_disc;
				Disciplina * disciplina = problemData->retornaDisciplina(discId);
				
				if (disciplina == NULL) continue;

				#pragma region Equivalencias
				if ( problemData->mapDiscSubstituidaPor.find( disciplina ) !=
					problemData->mapDiscSubstituidaPor.end() )
				{
					continue;
				}
				#pragma endregion

				for ( int turma = 0; turma < disciplina->getNumTurmas(); turma++ )
				{
				   VariableTatico v;
				   v.reset();
				   v.setType( VariableTatico::V_SLACK_COMPARTILHAMENTO );
				   v.setTurma( turma );								// i
				   v.setDisciplina( disciplina );   				// d
				   v.setParCursos( std::make_pair( c1, c2 ) );		// (c, c)
				   v.setCampus( *itCampus );						// cp

				   if ( vHashTatico.find( v ) == vHashTatico.end() )
				   {
					  if ( !criaVariavelTatico( &v ) )
						  continue;

					  vHashTatico[v] = lp->getNumCols();

						double coef = 0.0;

						if ( problemData->parametros->funcao_objetivo == 0 )
						{
							coef = -itCampus->getCusto()/4;
						}
						else if ( problemData->parametros->funcao_objetivo == 1 )
						{
							coef = itCampus->getCusto()/4;
						}  

					  OPT_COL col( OPT_COL::VAR_BINARY, coef, 0.0, 1.0, ( char * )v.toString().c_str() );

					  lp->newCol( col );
					  numVars++;
				   }
				}
			}
		}
   }

   return numVars;
}
*/



/*====================================================================/
%DocBegin TRIEDA_LOAD_MODEL

%Var m_{d,i,k} 

%Desc 
vari�vel bin�ria que indica se a combina��o de divis�o de cr�ditos 
$k$ foi escolhida para a turma $i$ da disciplina $d$.

%DocEnd
/====================================================================*/

int SolverMIP::criaVariavelTaticoCombinacaoDivisaoCredito( int campusId )
{
   int numVars = 0;

   Disciplina * disciplina = NULL;
  
   GGroup<int> disciplinas = problemData->cp_discs[campusId];

   ITERA_GGROUP_N_PT( it_disciplina, disciplinas, int )
   {
	   disciplina = problemData->refDisciplinas[ *it_disciplina ];

	  #pragma region Equivalencias
	  if ( problemData->mapDiscSubstituidaPor.find( disciplina ) !=
		   problemData->mapDiscSubstituidaPor.end() )
	  {
		  continue;
	  }
	  #pragma endregion

      for ( int turma = 0; turma < disciplina->getNumTurmas(); turma++ )
      {
         for ( unsigned k = 0; k < disciplina->combinacao_divisao_creditos.size(); k++ )
         { 
            VariableTatico v;
            v.reset();
            v.setType( VariableTatico::V_COMBINACAO_DIVISAO_CREDITO );

            v.setTurma( turma );           // i
            v.setDisciplina( disciplina ); // d
            v.setK( k );	                // k

            if ( vHashTatico.find( v ) == vHashTatico.end() )
            {
			   if ( !criaVariavelTatico( &v ) )
					continue;

               vHashTatico[ v ] = lp->getNumCols();

               OPT_COL col( OPT_COL::VAR_BINARY,
                            0.0, 0.0, 1.0,
                            ( char * )v.toString().c_str() );

               lp->newCol( col );
               numVars++;
            }
         }
      }
   }

   return numVars;
}

/*====================================================================/
%DocBegin TRIEDA_LOAD_MODEL

%Var fkp_{d,i,t} 

%Desc 
vari�vel de folga superior para a restri��o de combina��o de divis�o de cr�ditos.

%ObjCoef
\psi \cdot \sum\limits_{d \in D} 
\sum\limits_{t \in T} \sum\limits_{i \in I_{d}} fkp_{d,i,k}

%Data \psi
%Desc
peso associado a fun��o objetivo.

%Var fkm_{d,i,t} 

%Desc 
vari�vel de folga inferior para a restri��o de combina��o de divis�o de cr�ditos.

%ObjCoef
\psi \cdot \sum\limits_{d \in D} 
\sum\limits_{t \in T} \sum\limits_{i \in I_{d}} fkm_{d,i,k}

%Data \psi
%Desc
peso associado a fun��o objetivo.

%DocEnd
/====================================================================*/

int SolverMIP::criaVariavelTaticoFolgaCombinacaoDivisaoCredito( int campusId )
{
   int numVars = 0;

   Disciplina * disciplina = NULL;
   
   ITERA_GGROUP_LESSPTR( it_campus, problemData->campi, Campus )
   {
	   if ( it_campus->getId() != campusId )
	   {
			continue;
	   }

      ITERA_GGROUP_LESSPTR( it_unidade, it_campus->unidades, Unidade )
      {
         ITERA_GGROUP_LESSPTR( it_conjunto_sala, it_unidade->conjutoSalas, ConjuntoSala )
         {
            ITERA_GGROUP_LESSPTR( it_disciplina, it_conjunto_sala->disciplinas_associadas, Disciplina )
            {
               disciplina = ( *it_disciplina );

				#pragma region Equivalencias
				if ( problemData->mapDiscSubstituidaPor.find( disciplina ) !=
					problemData->mapDiscSubstituidaPor.end() )
				{
					continue;
				}
				#pragma endregion

               for ( int turma = 0; turma < disciplina->getNumTurmas(); turma++ )
               {
                  // Dada uma disciplina, recupera os seus dias letivos, observando as fixa��es
                  GGroup< int > dias_letivos =
                     it_conjunto_sala->dias_letivos_disciplinas[ ( disciplina ) ];

                  GGroup< int >::iterator itDiasLetDisc = dias_letivos.begin();

                  for (; itDiasLetDisc != dias_letivos.end(); itDiasLetDisc++ )
                  {
                     VariableTatico v;
                     v.reset();
                     v.setType( VariableTatico::V_SLACK_COMBINACAO_DIVISAO_CREDITO_P );

                     v.setTurma( turma );            // i
                     v.setDisciplina( disciplina );  // d
                     v.setDia( *itDiasLetDisc );	    // t

                     if ( vHashTatico.find( v ) == vHashTatico.end() )
                     {
						if ( !criaVariavelTatico( &v ) )
							continue;

                        vHashTatico[v] = lp->getNumCols();

						double coef = 0.0;

						if ( problemData->parametros->funcao_objetivo == 0 )
						{
							coef = -it_campus->getCusto()/4;
						}
						else if ( problemData->parametros->funcao_objetivo == 1 )
						{
							coef = it_campus->getCusto()/4;
						}
                           
						OPT_COL col( OPT_COL::VAR_INTEGRAL, coef, 0.0, 10000.0,
                              ( char * )v.toString().c_str() );

                        lp->newCol( col );

                        numVars++;
                     }

                     v.reset();
                     v.setType( VariableTatico::V_SLACK_COMBINACAO_DIVISAO_CREDITO_M );

                     v.setTurma( turma );            // i
                     v.setDisciplina( disciplina );  // d
                     v.setDia( *itDiasLetDisc );	    // t

                     if ( vHashTatico.find( v ) == vHashTatico.end() )
                     {
						if ( !criaVariavelTatico( &v ) )
							continue;

                        vHashTatico[v] = lp->getNumCols();

						double coef = 0.0;

						if ( problemData->parametros->funcao_objetivo == 0 )
						{
							coef = -it_campus->getCusto()/4;
						}
						else if ( problemData->parametros->funcao_objetivo == 1 )
						{
							coef = it_campus->getCusto()/4;
						}
                           
						OPT_COL col( OPT_COL::VAR_INTEGRAL, coef, 0.0, 10000.0,
                              ( char * )v.toString().c_str() );
						                           
						lp->newCol( col );

                        numVars++;
                     }
                  }
               }
            }
         }
      }
   }

   return numVars;
}



/*====================================================================/
%DocBegin TRIEDA_LOAD_MODEL

%Var fcp_{d,t}

%Desc 
vari�vel de folga superior para a restri��o de fixa��o da distribui��o de cr�ditos por dia.
%ObjCoef
\xi \cdot \sum\limits_{d \in D} \sum\limits_{t \in T} fcp_{d,t}

%Data \xi
%Desc
peso associado a fun��o objetivo.

%DocEnd
/====================================================================*/

int SolverMIP::criaVariavelTaticoFolgaDistCredDiaSuperior( int campusId )
{
   int numVars = 0;

   Disciplina * disciplina = NULL;

   ITERA_GGROUP_LESSPTR( itCampus, problemData->campi, Campus )
   {
	  if ( itCampus->getId() != campusId )
	  {
		  continue;
	  }

      ITERA_GGROUP_LESSPTR( itUnidade, itCampus->unidades, Unidade )
      {
         ITERA_GGROUP_LESSPTR( itCjtSala, itUnidade->conjutoSalas, ConjuntoSala )
         {
            ITERA_GGROUP_LESSPTR( it_disciplina, problemData->disciplinas, Disciplina )
            {
                disciplina = ( *it_disciplina );

				// A disciplina deve ser ofertada no campus especificado
				if ( problemData->cp_discs[campusId].find( disciplina->getId() ) ==
					 problemData->cp_discs[campusId].end() )
				{
					continue;
				}

			   #pragma region Equivalencias
			   if ( problemData->mapDiscSubstituidaPor.find( disciplina ) !=
				    problemData->mapDiscSubstituidaPor.end() )
			   {
				   continue;
			   }
			   #pragma endregion

               for ( int turma = 0; turma < disciplina->getNumTurmas(); turma++ )
               {
                  // Dada uma disciplina, recupera os seus dias letivos, observando as fixa��es
                  GGroup< int > dias_letivos
                     = itCjtSala->dias_letivos_disciplinas[ ( disciplina ) ];

                  GGroup< int >::iterator itDiasLetDisc = dias_letivos.begin();

                  for (; itDiasLetDisc != dias_letivos.end(); itDiasLetDisc++ )
                  {
                     ITERA_GGROUP_LESSPTR( it_fix, problemData->fixacoes, Fixacao )
                     {
                        if ( it_fix->getDisciplinaId() == disciplina->getId() 
                           && it_fix->getDiaSemana() == ( *itDiasLetDisc ) )
                        {
                           VariableTatico v;
                           v.reset();
                           v.setType( VariableTatico::V_SLACK_DIST_CRED_DIA_SUPERIOR );

                           v.setTurma( turma );
                           v.setDisciplina( disciplina );
                           v.setDia( *itDiasLetDisc );
                           v.setSubCjtSala( *itCjtSala );

                           if ( vHashTatico.find( v ) == vHashTatico.end() )
                           {
							    if ( !criaVariavelTatico( &v ) )
									continue;

                                vHashTatico[v] = lp->getNumCols();
                                int cred_disc_dia = it_fix->disciplina->getMaxCreds();

								double coef = 0.0;

								if ( problemData->parametros->funcao_objetivo == 0 )
								{
									coef = -itCampus->getCusto()/2;
								}
								else if ( problemData->parametros->funcao_objetivo == 1 )
								{
									coef = itCampus->getCusto()/2;
								}
               
								OPT_COL col( OPT_COL::VAR_INTEGRAL, coef, 0.0, cred_disc_dia,
										( char * )v.toString().c_str() );
			                 
								lp->newCol( col );

                                numVars++;
                           }
                        }
                     }
                  }
               }
            }
         }
      }
   }

   return numVars;
}


/*====================================================================/
%DocBegin TRIEDA_LOAD_MODEL

%Var fcm_{d,t}  

%Desc 
vari�vel de folga inferior para a restri��o de fixa��o da distribui��o de cr�ditos por dia.

%ObjCoef
\xi \cdot \sum\limits_{d \in D} \sum\limits_{t \in T} fcm_{d,t}

%DocEnd
/====================================================================*/

int SolverMIP::criaVariavelTaticoFolgaDistCredDiaInferior( int campusId )
{
   int numVars = 0;

   Disciplina * disciplina = NULL;

   ITERA_GGROUP_LESSPTR( itCampus, problemData->campi, Campus )
   {
	   if ( itCampus->getId() != campusId )
	   {
		   continue;
	   }

      ITERA_GGROUP_LESSPTR( itUnidade, itCampus->unidades, Unidade )
      {
         ITERA_GGROUP_LESSPTR( itCjtSala, itUnidade->conjutoSalas, ConjuntoSala )
         {
            ITERA_GGROUP_LESSPTR( it_disciplina, problemData->disciplinas, Disciplina )
            {
               disciplina = ( *it_disciplina );

				// A disciplina deve ser ofertada no campus especificado
				if ( problemData->cp_discs[campusId].find( disciplina->getId() ) ==
					 problemData->cp_discs[campusId].end() )
				{
					continue;
				}

			   #pragma region Equivalencias
			   if ( problemData->mapDiscSubstituidaPor.find( disciplina ) !=
				    problemData->mapDiscSubstituidaPor.end() )
			   {
				  continue;
			   }
			   #pragma endregion

               for ( int turma = 0; turma < disciplina->getNumTurmas(); turma++ )
               {
                  // Dada uma disciplina, recupera os seus dias letivos, observando as fixa��es
                  GGroup< int > dias_letivos =
                     itCjtSala->dias_letivos_disciplinas[ ( disciplina ) ];

                  GGroup< int >::iterator itDiasLetDisc = dias_letivos.begin();

                  for (; itDiasLetDisc != dias_letivos.end(); itDiasLetDisc++ )
                  {
                     ITERA_GGROUP_LESSPTR( it_fix, problemData->fixacoes, Fixacao )
                     {
                        if ( it_fix->getDisciplinaId() == disciplina->getId() 
                           && it_fix->getDiaSemana() == ( *itDiasLetDisc ) )
                        {
                           VariableTatico v;
                           v.reset();
                           v.setType( VariableTatico::V_SLACK_DIST_CRED_DIA_INFERIOR );

                           v.setTurma( turma );
                           v.setDisciplina( disciplina );
                           v.setDia( *itDiasLetDisc );
                           v.setSubCjtSala( *itCjtSala );

                           if ( vHashTatico.find( v ) == vHashTatico.end() )
                           {
							    if ( !criaVariavelTatico( &v ) )
									continue;

                                vHashTatico[v] = lp->getNumCols();
                                int cred_disc_dia = it_fix->disciplina->getMinCreds();

							    double coef = 0.0;

								if ( problemData->parametros->funcao_objetivo == 0 )
								{
									coef = -itCampus->getCusto()/2;
								}
								else if ( problemData->parametros->funcao_objetivo == 1 )
								{
									coef = itCampus->getCusto()/2;
								}               

								OPT_COL col( OPT_COL::VAR_INTEGRAL, coef, 0.0, cred_disc_dia,
											( char* )v.toString().c_str() );

								lp->newCol( col );

                                numVars += 1;
                           }
                        }
                     }
                  }
               }
            }
         }
      }
   }

   return numVars;
}

/*====================================================================/
%DocBegin TRIEDA_LOAD_MODEL

%Var zc_{d,t} 

%Desc 
indica se houve abertura da disciplina $d$ no dia $t$.

%DocEnd
/====================================================================*/
int SolverMIP::criaVariavelTaticoAberturaCompativel( int campusId )
{
   int numVars = 0;

   Disciplina *disciplina = NULL;

   Campus *campus = problemData->refCampus[ campusId ];

   ITERA_GGROUP_N_PT( it_disciplina, problemData->cp_discs[campusId], int )
   {
	    disciplina = problemData->refDisciplinas[ *it_disciplina ];

		#pragma region Equivalencias
		if ( problemData->mapDiscSubstituidaPor.find( disciplina ) !=
			problemData->mapDiscSubstituidaPor.end() )
		{
			continue;
		}
		#pragma endregion

        ITERA_GGROUP_N_PT( itDiasLetDisc, disciplina->diasLetivos, int )
        {
            VariableTatico v;
            v.reset();
            v.setType( VariableTatico::V_ABERTURA_COMPATIVEL );

            v.setDisciplina( disciplina );  // d
            v.setDia( *itDiasLetDisc );     // t
			v.setCampus( campus );			// cp

            if ( vHashTatico.find( v ) == vHashTatico.end() )
            {
				if ( !criaVariavelTatico( &v ) )
					continue;

                vHashTatico[ v ] = lp->getNumCols();

				double coef = 0.0;

				if ( problemData->parametros->funcao_objetivo == 0 )
				{
					coef = -1.0;
				}
				else if ( problemData->parametros->funcao_objetivo == 1 )
				{
					coef = 1.0;
			 	}

                OPT_COL col( OPT_COL::VAR_BINARY, coef, 0.0, 1.0,
                ( char * )v.toString().c_str() );

                lp->newCol( col );
                numVars++;
            }
        }
   }

   return numVars;
}

// fd_{d,a}
int SolverMIP::criaVariavelTaticoFolgaDemandaDiscAluno( int campusId )
{
   int numVars = 0;
   
   Campus *cp = problemData->refCampus[campusId];

   ITERA_GGROUP_LESSPTR( itAluno, problemData->alunos, Aluno )
   {
	    Aluno *aluno = *itAluno;

	    if ( aluno->getOferta()->getCampusId() != campusId )
	    {
		    continue;
	    }

		ITERA_GGROUP_LESSPTR( itAlDemanda, aluno->demandas, AlunoDemanda )
		{
			Disciplina *disciplina = itAlDemanda->demanda->disciplina;

			#pragma region Equivalencias
			if ( problemData->mapDiscSubstituidaPor.find( disciplina ) !=
				 problemData->mapDiscSubstituidaPor.end() )
			{
				continue;
			}
			#pragma endregion

			VariableTatico v;
			v.reset();
			v.setType( VariableTatico::V_SLACK_DEMANDA );

			v.setDisciplina( disciplina );  // d
			v.setAluno( aluno );			// a

			if ( vHashTatico.find( v ) == vHashTatico.end() )
			{
				if ( !criaVariavelTatico( &v ) )
					continue;

				vHashTatico[ v ] = lp->getNumCols();

				double coef = 0.0;

				if ( problemData->parametros->funcao_objetivo == 0 )
				{
					coef = - 50 * disciplina->getTotalCreditos() * aluno->getOferta()->getReceita();
				}
				else if ( problemData->parametros->funcao_objetivo == 1 )
				{
					coef = 10 * disciplina->getTotalCreditos() * cp->getCusto();
			 	}

				double ub = 1.0;

				OPT_COL col( OPT_COL::VAR_BINARY, coef, 0.0, ub,
				( char * )v.toString().c_str() );

				lp->newCol( col );
				numVars++;
			}
        }
   }

   return numVars;
}

// y_{a,u,t}
int SolverMIP::criaVariavelTaticoAlunoUnidDia( int campusId )
{
   int numVars = 0;
   
   Campus *cp = problemData->refCampus[campusId];
   
   ITERA_GGROUP_LESSPTR( itUnid, cp->unidades, Unidade )
   {
	   Unidade *unid = *itUnid;

	   ITERA_GGROUP_N_PT( itDia, unid->dias_letivos, int )
	   {
			int dia = *itDia;

		   ITERA_GGROUP_LESSPTR( itAluno, problemData->alunos, Aluno )
		   {
				Aluno *aluno = *itAluno;
			
				VariableTatico v;
				v.reset();
				v.setType( VariableTatico::V_ALUNO_UNID_DIA );

				v.setUnidade( unid ); // u
				v.setAluno( aluno );  // a
				v.setDia( dia );	  // t

				if ( vHashTatico.find( v ) == vHashTatico.end() )
				{
					if ( !criaVariavelTatico( &v ) )
						continue;

					vHashTatico[ v ] = lp->getNumCols();

					double coef = 0.0;

					if ( problemData->parametros->funcao_objetivo == 0 )
					{
						coef = -1.0;
					}
					else if ( problemData->parametros->funcao_objetivo == 1 )
					{
						coef = 1.0;
			 		}

					OPT_COL col( OPT_COL::VAR_BINARY, coef, 0.0, 1.0,
					( char * )v.toString().c_str() );

					lp->newCol( col );
					numVars++;
				}
			}
	   }
   }

   return numVars;
}


// w_{a,t}
int SolverMIP::criaVariavelTaticoAlunoUnidadesDifDia( int campusId )
{
   int numVars = 0;
   
   Campus *cp = problemData->refCampus[campusId];
   
   ITERA_GGROUP_N_PT( itDia, cp->diasLetivos, int )
   {
		int dia = *itDia;
		ITERA_GGROUP_LESSPTR( itAluno, problemData->alunos, Aluno )
		{
			Aluno *aluno = *itAluno;
			
			VariableTatico v;
			v.reset();
			v.setType( VariableTatico::V_ALUNO_VARIAS_UNID_DIA );

			v.setAluno( aluno );  // a
			v.setDia( dia );	  // t

			if ( vHashTatico.find( v ) == vHashTatico.end() )
			{
				if ( !criaVariavelTatico( &v ) )
					continue;

				vHashTatico[ v ] = lp->getNumCols();

				double coef = 0.0;

				if ( problemData->parametros->funcao_objetivo == 0 )
				{
					coef = -100.0;
				}
				else if ( problemData->parametros->funcao_objetivo == 1 )
				{
					coef = 100.0;
			 	}

				OPT_COL col( OPT_COL::VAR_BINARY, coef, 0.0, 1.0,
				( char * )v.toString().c_str() );

				lp->newCol( col );
				numVars++;
			}
		}
   }

   return numVars;
}


/* ----------------------------------------------------------------------------------
	
							RESTRICOES TATICO POR ALUNO
 ---------------------------------------------------------------------------------- */


int SolverMIP::criaRestricoesTatico( int campusId )
{
	int restricoes = 0;

	CPUTimer timer;
	double dif = 0.0;

#ifdef PRINT_cria_restricoes
	int numRestAnterior = 0;
#endif

	timer.start();
	restricoes += criaRestricaoTaticoCargaHoraria( campusId );				// Restricao 1.2.2
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest \"1.2.2\": " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
#endif

	timer.start();
	restricoes += criaRestricaoTaticoUsoDeSalaParaCadaHorario( campusId );				// Restricao 1.2.3
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest \"1.2.3\": " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
#endif


	timer.start();
	restricoes += criaRestricaoTaticoAtendimentoUnicoTurmaDiscDia( campusId );				// Restricao 1.2.4
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest \"1.2.4\": " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
#endif


	timer.start();
	restricoes += criaRestricaoTaticoAtendeDemandaAluno( campusId );				// Restricao 1.2.5
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest \"1.2.5\": " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
#endif


	timer.start();
	restricoes += criaRestricaoTaticoTurmaDiscDiasConsec( campusId );				// Restricao 1.2.6
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest \"1.2.6\": " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
#endif



	timer.start();
	restricoes += criaRestricaoTaticoLimitaAberturaTurmas( campusId );			// Restricao 1.2.7
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest \"1.2.7\": " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
#endif



	timer.start();
	restricoes += criaRestricaoTaticoDivisaoCredito( campusId );			// Restricao 1.2.8
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest \"1.2.8\": " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
#endif


	timer.start();
	restricoes += criaRestricaoTaticoCombinacaoDivisaoCredito( campusId );			// Restricao 1.2.9
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest \"1.2.9\": " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
#endif
      

	timer.start();
	restricoes += criaRestricaoTaticoAtivacaoVarZC( campusId );			// Restricao 1.2.10
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest \"1.2.10\": " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
#endif


  	timer.start();
	restricoes += criaRestricaoTaticoDisciplinasIncompativeis( campusId );			// Restricao 1.2.11
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest \"1.2.11\": " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
#endif


  	timer.start();
	restricoes += criaRestricaoTaticoEvitaSobreposicaoAulaAluno( campusId );			// Restricao 1.2.12
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest \"1.2.12\": " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
#endif

	
  	timer.start();
	restricoes += criaRestricaoTaticoAtivaY( campusId );						// Restricao 1.2.13
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest \"1.2.13\": " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
#endif


  	timer.start();
	restricoes += criaRestricaoTaticoAlunoUnidadesDifDia( campusId );			// Restricao 1.2.14
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest \"1.2.14\": " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
#endif


  	timer.start();
	restricoes += criaRestricaoTaticoMinCreds( campusId );			// Restricao 1.2.15
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest \"1.2.15\": " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
#endif


  	timer.start();
	restricoes += criaRestricaoTaticoMaxCreds( campusId );			// Restricao 1.2.16
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest \"1.2.16\": " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
#endif


  	timer.start();
	restricoes += criaRestricaoTaticoAlunoDiscPraticaTeorica( campusId );	// Restricao 1.2.17
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest \"1.2.17\": " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
#endif
		   
	return restricoes;

}

int SolverMIP::criaRestricaoTaticoCargaHoraria( int campusId )
{
	int restricoes = 0;

	int nnz;
	char name[ 100 ];

	VariableTatico v;
	ConstraintTatico c;

	VariableTaticoHash::iterator vit;
	ConstraintTaticoHash::iterator cit;

	Campus *campus = NULL;

	ITERA_GGROUP_LESSPTR( itCampus, problemData->campi, Campus )
	{
		if ( itCampus->getId() != campusId )
			continue;

		campus = *itCampus;
		break;
	}

	if(!campus)
		return 0;

	vit = vHashTatico.begin();
	while(vit != vHashTatico.end())
	{
		if(vit->first.getType() != VariableTatico::V_CREDITOS && vit->first.getType() != VariableTatico::V_ABERTURA)
		{
			vit++;
			continue;
		}

		VariableTatico v = vit->first;

		if(v.getType() == VariableTatico::V_CREDITOS)
		{
			if ( problemData->cp_discs[campusId].find( v.getDisciplina()->getId() ) ==
				problemData->cp_discs[campusId].end() )
			{
				vit++;
				continue;
			}

			c.reset();
			c.setType( ConstraintTatico::C_CARGA_HORARIA );
			c.setCampus( campus );
			c.setTurma( v.getTurma() );
			c.setDisciplina( v.getDisciplina() );

			cit = cHashTatico.find(c);

			cit = cHashTatico.find(c);
			if(cit == cHashTatico.end())
			{
				sprintf( name, "%s", c.toString().c_str() );
				OPT_ROW row( 100, OPT_ROW::EQUAL , 0 , name );

				int NCH = v.getDisciplina()->getCalendario()->retornaNroCreditosEntreHorarios( v.getHorarioAulaInicial(), v.getHorarioAulaFinal());
				row.insert( vit->second, NCH);

				cHashTatico[ c ] = lp->getNumRows();
				lp->addRow( row );
				restricoes++;
			}
			else
			{
				int NCH = v.getDisciplina()->getCalendario()->retornaNroCreditosEntreHorarios( v.getHorarioAulaInicial(), v.getHorarioAulaFinal());
				lp->chgCoef(cit->second, vit->second,  NCH);
			}
		}
		else
		{
			if(campus->getId() != v.getCampus()->getId())
			{
				vit++;
				continue;
			}

			c.reset();
			c.setType( ConstraintTatico::C_CARGA_HORARIA );
			c.setCampus( campus );
			c.setTurma( v.getTurma() );
			c.setDisciplina( v.getDisciplina() );

			cit = cHashTatico.find(c);
			if(cit == cHashTatico.end())
			{
				sprintf( name, "%s", c.toString().c_str() );
				OPT_ROW row( 100, OPT_ROW::EQUAL , 0 , name );

				row.insert( vit->second, -( v.getDisciplina()->getCredPraticos() + v.getDisciplina()->getCredTeoricos() ) );

				cHashTatico[ c ] = lp->getNumRows();
				lp->addRow( row );
				restricoes++;
			}
			else
			{
				lp->chgCoef(cit->second, vit->second,  -( v.getDisciplina()->getCredPraticos() + v.getDisciplina()->getCredTeoricos() ));
			}
		}


		vit++;
	}

	return restricoes;
}

//int SolverMIP::criaRestricaoTaticoCargaHoraria( int campusId )
//{
//   int restricoes = 0;
//
//   int nnz;
//   char name[ 100 ];
//
//   VariableTatico v;
//   ConstraintTatico c;
//   VariableTaticoHash::iterator it_v;
//
//   Disciplina * disciplina = NULL;
//
//   ITERA_GGROUP_LESSPTR( itCampus, problemData->campi, Campus )
//   {
//	   if ( itCampus->getId() != campusId )
//	   {
//			continue;
//	   }
//
//      ITERA_GGROUP_LESSPTR( it_disciplina, problemData->disciplinas, Disciplina )
//      {
//         disciplina = ( *it_disciplina );
//
//		 // A disciplina deve ser ofertada no campus especificado
//		 if ( problemData->cp_discs[campusId].find( disciplina->getId() ) ==
//			  problemData->cp_discs[campusId].end() )
//		 {
//			 continue;
//		 }
//
//		 #pragma region Equivalencias
//		 if ( problemData->mapDiscSubstituidaPor.find( disciplina ) !=
//				problemData->mapDiscSubstituidaPor.end() )
//		 {
//			continue;
//		 }
//		 #pragma endregion
//
//         for ( int turma = 0; turma < disciplina->getNumTurmas(); turma++ )
//         {
//            c.reset();
//            c.setType( ConstraintTatico::C_CARGA_HORARIA );
//
//            c.setCampus( *itCampus );
//            c.setTurma( turma );
//            c.setDisciplina( disciplina );
//
//            sprintf( name, "%s", c.toString().c_str() );
//
//            if ( cHashTatico.find( c ) != cHashTatico.end() )
//            {
//               continue;
//            }
//
//            nnz = ( itCampus->getTotalSalas() * 7 );
//
//            OPT_ROW row( nnz + 1, OPT_ROW::EQUAL , 0 , name );
//
//            v.reset();
//            v.setType( VariableTatico::V_CREDITOS );
//
//            // Insere variaveis Credito (x) ---
//
//            ITERA_GGROUP_LESSPTR( itUnidade, itCampus->unidades, Unidade )
//            {
//               ITERA_GGROUP_LESSPTR( itCjtSala, itUnidade->conjutoSalas, ConjuntoSala )
//               {
//				   if ( itCjtSala->disciplinas_associadas.find( disciplina) ==
//					    itCjtSala->disciplinas_associadas.end() )
//				   {
//					   continue;
//				   }
//				   
//				   int salaId = itCjtSala->salas.begin()->first;
//					
//				   GGroup<int> dias = itCjtSala->dias_letivos_disciplinas[disciplina];
//
//				   ITERA_GGROUP_N_PT ( it_Dia, dias, int )
//				   {
//						int dia = *it_Dia;
//
//						GGroup< HorarioAula *, LessPtr< HorarioAula > > horariosEmComum = 
//							problemData->retornaHorariosEmComum( salaId, disciplina->getId(), dia );
//					  
//						ITERA_GGROUP_LESSPTR( itHorario, horariosEmComum, HorarioAula )
//						{
//							HorarioAula *hi = *itHorario;
//
//							ITERA_GGROUP_LESSPTR( itHorario, horariosEmComum, HorarioAula )
//							{
//									HorarioAula *hf = *itHorario;
//
//									if ( hf < hi )
//									{
//							 			continue;
//									}
//
//									int NCH = disciplina->getCalendario()->retornaNroCreditosEntreHorarios( hi, hf );
//									
//									v.setTurma( turma );
//									v.setDisciplina( disciplina );
//									v.setUnidade( *itUnidade );
//									v.setSubCjtSala( *itCjtSala );
//									v.setDia( dia );                   
//									v.setHorarioAulaInicial( hi );	 // hi
//									v.setHorarioAulaFinal( hf );	 // hf
//
//									it_v = vHashTatico.find( v );
//									if( it_v != vHashTatico.end() )
//									{
//										row.insert( it_v->second, NCH );
//									}
//							}
//						}
//					}                  
//                }
//            }
//
//            // Insere variaveis Abertura (z) ---
//
//            v.reset();
//            v.setType( VariableTatico::V_ABERTURA );
//
//            v.setCampus( *itCampus );
//            v.setDisciplina( disciplina );
//            v.setTurma( turma );
//
//            it_v = vHashTatico.find( v );
//            if( it_v != vHashTatico.end() )
//            {
//               row.insert( it_v->second,
//                  -( disciplina->getCredPraticos() + 
//                  disciplina->getCredTeoricos() ) );
//            }
//
//            // Insere restri��o no Hash ---
//
//            if ( row.getnnz() != 0 )
//            {
//               cHashTatico[ c ] = lp->getNumRows();
//
//               lp->addRow( row );
//               restricoes++;
//            }
//         }
//      }
//   }
//
//	return restricoes;
//}
   

// Restricao 1.2.3
int SolverMIP::criaRestricaoTaticoUsoDeSalaParaCadaHorario( int campusId )
{
	int restricoes = 0;

	int nnz;
	char name[ 100 ];

	VariableTatico v;
	ConstraintTatico c;

	VariableTaticoHash::iterator vit;
	ConstraintTaticoHash::iterator cit;

	Campus *campus = NULL;

	ITERA_GGROUP_LESSPTR( itCampus, problemData->campi, Campus )
	{
		if ( itCampus->getId() != campusId )
			continue;

		campus = *itCampus;
		break;
	}

	if(!campus)
		return 0;

	vit = vHashTatico.begin();
	while(vit != vHashTatico.end())
	{
		if(vit->first.getType() != VariableTatico::V_CREDITOS)
		{
			vit++;
			continue;
		}

		VariableTatico v = vit->first;

		if ( campus->unidades.find(v.getUnidade()) == campus->unidades.end())
		{
			vit++;
			continue;
		}

		Sala *sala = v.getSubCjtSala()->salas.begin()->second;
		ITERA_GGROUP( itHor, sala->horariosDia, HorarioDia )
		{
			HorarioAula* h = itHor->getHorarioAula();

			if ( itHor->getDia() != v.getDia() )					
				continue;

			DateTime fimF = v.getHorarioAulaFinal()->getInicio();
			fimF.addMinutes( v.getHorarioAulaFinal()->getTempoAula() );

			DateTime fimH = h->getInicio(); // controle
			fimH.addMinutes( h->getTempoAula() );
			
			if ( ( v.getHorarioAulaInicial()->getInicio() <= h->getInicio() ) && ( fimF >  h->getInicio() ) )
			{
				c.reset();
				c.setType( ConstraintTatico::C_SALA_HORARIO );
				c.setCampus( campus );
				c.setUnidade( v.getUnidade() );
				c.setSubCjtSala( v.getSubCjtSala() );
				c.setDia( v.getDia() );
				c.setHorarioAula( h );

				cit = cHashTatico.find(c);

				cit = cHashTatico.find(c);
				if(cit == cHashTatico.end())
				{
					sprintf( name, "%s", c.toString().c_str() );
					OPT_ROW row( 100, OPT_ROW::LESS , 1 , name );

					row.insert( vit->second, 1.0);

					cHashTatico[ c ] = lp->getNumRows();
					lp->addRow( row );
					restricoes++;
				}
				else
				{
					lp->chgCoef(cit->second, vit->second,  1.0);
				}
			}
		}

		vit++;
	}

	return restricoes;

}

//int SolverMIP::criaRestricaoTaticoUsoDeSalaParaCadaHorario( int campusId )
//{
//   int restricoes = 0;
//
//   int nnz;
//   char name[ 100 ];
//
//   VariableTatico v;
//   ConstraintTatico c;
//   VariableTaticoHash::iterator it_v;
//
//   Disciplina * disciplina = NULL;
//
//   ITERA_GGROUP_LESSPTR( itCampus, problemData->campi, Campus )
//   {
//	    if ( itCampus->getId() != campusId )
//	    {
//			continue;
//	    }
//
//		ITERA_GGROUP_LESSPTR( itUnidade, itCampus->unidades, Unidade )
//		{
//			ITERA_GGROUP_LESSPTR( itCjtSala, itUnidade->conjutoSalas, ConjuntoSala )
//			{
//				Sala *sala = itCjtSala->salas.begin()->second;
//
//				ITERA_GGROUP_N_PT( itDia, sala->diasLetivos, int )
//				{
//					int dia = *itDia;
//
//					ITERA_GGROUP( itHor, sala->horariosDia, HorarioDia )
//					{
//						HorarioAula* h = itHor->getHorarioAula();
//
//						if ( itHor->getDia() != dia )					
//						{
//							continue;
//						}
//
//						 CONSTRAINT --------------------------------------
//
//						c.reset();
//						c.setType( ConstraintTatico::C_SALA_HORARIO );
//
//						c.setCampus( *itCampus );
//						c.setUnidade( *itUnidade );
//						c.setSubCjtSala( *itCjtSala );
//						c.setDia( dia );
//						c.setHorarioAula( h );
//
//						sprintf( name, "%s", c.toString().c_str() );
//
//						if ( cHashTatico.find( c ) != cHashTatico.end() )
//						{
//							continue;
//						}
//
//						nnz = ( itCampus->getTotalSalas() * 7 );
//
//						OPT_ROW row( nnz + 1, OPT_ROW::LESS , 1 , name );
//
//						 Insere variaveis Credito (x) ---
//						
//						ITERA_GGROUP_LESSPTR( it_disciplina, itCjtSala->disciplinas_associadas, Disciplina )
//						{
//							disciplina = ( *it_disciplina );
//							
//							#pragma region Equivalencias
//							if ( problemData->mapDiscSubstituidaPor.find( disciplina ) !=
//								 problemData->mapDiscSubstituidaPor.end() )
//							{
//								continue;
//							}
//							#pragma endregion
//
//							for ( int turma = 0; turma < disciplina->getNumTurmas(); turma++ )
//							{			
//								GGroup< HorarioAula *, LessPtr< HorarioAula > > horariosEmComum = 
//									problemData->retornaHorariosEmComum( sala->getId(), disciplina->getId(), dia );
//					
//								ITERA_GGROUP_LESSPTR( itHorario, horariosEmComum, HorarioAula )
//								{
//									HorarioAula *hi = *itHorario;
//
//									ITERA_GGROUP_LESSPTR( itHorario, horariosEmComum, HorarioAula )
//									{
//											HorarioAula *hf = *itHorario;
//
//											if ( hf < hi )
//											{
//							 					continue;
//											}
//														   
//											DateTime fimF = hf->getInicio();
//											fimF.addMinutes( hf->getTempoAula() );
//
//											DateTime fimH = h->getInicio(); // controle
//											fimH.addMinutes( h->getTempoAula() );
//
//											 ---- S� insere variaveis com horarios (hi,hf) que possuem interse��o com h
//
//											if ( ( ( hi->getInicio() <= h->getInicio() ) && ( fimF >  h->getInicio() ) ) ||
//												 ( ( h->getInicio() <= hi->getInicio() ) && ( fimH >  hi->getInicio() ) ) )
//											{	
//												 intersecao de horarios, com hi antes de h
//												 intersecao de horarios, com h antes de hi
//
//												int NCH = disciplina->getCalendario()->retornaNroCreditosEntreHorarios( hi, hf );
//											
//												v.reset();
//												v.setType( VariableTatico::V_CREDITOS );									
//												v.setTurma( turma );
//												v.setDisciplina( disciplina );
//												v.setUnidade( *itUnidade );
//												v.setSubCjtSala( *itCjtSala );
//												v.setDia( dia );                   
//												v.setHorarioAulaInicial( hi );	 // hi
//												v.setHorarioAulaFinal( hf );	 // hf
//
//												it_v = vHashTatico.find( v );
//												if( it_v != vHashTatico.end() )
//												{
//													row.insert( it_v->second, 1.0 );
//												}												
//											
//											}
//									}
//								}
//							}
//						}
//
//						 Insere restri��o no Hash ---
//
//						if ( row.getnnz() != 0 )
//						{
//						   cHashTatico[ c ] = lp->getNumRows();
//
//						   lp->addRow( row );
//						   restricoes++;
//						}						
//					}
//				}  
//			}
//		}
//   }
//
//	return restricoes;
//
//}

// Restricao 1.2.4
int SolverMIP::criaRestricaoTaticoAtendimentoUnicoTurmaDiscDia( int campusId )
{
	int restricoes = 0;

	int nnz;
	char name[ 100 ];

	VariableTatico v;
	ConstraintTatico c;

	VariableTaticoHash::iterator vit;
	ConstraintTaticoHash::iterator cit;

	Campus *campus = NULL;

	ITERA_GGROUP_LESSPTR( itCampus, problemData->campi, Campus )
	{
		if ( itCampus->getId() != campusId )
			continue;

		campus = *itCampus;
		break;
	}

	if(!campus)
		return 0;

	vit = vHashTatico.begin();
	while(vit != vHashTatico.end())
	{
		if(vit->first.getType() != VariableTatico::V_CREDITOS)
		{
			vit++;
			continue;
		}

		VariableTatico v = vit->first;

		if ( problemData->cp_discs[campusId].find( v.getDisciplina()->getId() ) ==
			problemData->cp_discs[campusId].end() )
		{
			vit++;
			continue;
		}

		c.reset();
		c.setType( ConstraintTatico::C_UNICO_ATEND_TURMA_DISC_DIA );
		c.setCampus( campus );
		c.setTurma( v.getTurma() );
		c.setDisciplina( v.getDisciplina() );
		c.setDia( v.getDia() );

		cit = cHashTatico.find(c);

		cit = cHashTatico.find(c);
		if(cit == cHashTatico.end())
		{
			sprintf( name, "%s", c.toString().c_str() );
			OPT_ROW row( 100, OPT_ROW::LESS , 1.0 , name );

			row.insert( vit->second, 1.0);

			cHashTatico[ c ] = lp->getNumRows();
			lp->addRow( row );
			restricoes++;
		}
		else
		{
			lp->chgCoef(cit->second, vit->second,  1.0);
		}

		vit++;
	}

	return restricoes;
}

//int SolverMIP::criaRestricaoTaticoAtendimentoUnicoTurmaDiscDia( int campusId )
//{
//   int restricoes = 0;
//
//   int nnz;
//   char name[ 100 ];
//
//   VariableTatico v;
//   ConstraintTatico c;
//   VariableTaticoHash::iterator it_v;
//
//   Disciplina * disciplina = NULL;
//
//   ITERA_GGROUP_LESSPTR( itCampus, problemData->campi, Campus )
//   {
//	   if ( itCampus->getId() != campusId )
//	   {
//			continue;
//	   }
//
//      ITERA_GGROUP_LESSPTR( it_disciplina, problemData->disciplinas, Disciplina )
//      {
//         disciplina = ( *it_disciplina );
//
//		 // A disciplina deve ser ofertada no campus especificado
//		 if ( problemData->cp_discs[campusId].find( disciplina->getId() ) ==
//			  problemData->cp_discs[campusId].end() )
//		 {
//			 continue;
//		 }
//
//		 #pragma region Equivalencias
//		 if ( problemData->mapDiscSubstituidaPor.find( disciplina ) !=
//				problemData->mapDiscSubstituidaPor.end() )
//		 {
//			continue;
//		 }
//		 #pragma endregion
//
//		 ITERA_GGROUP_N_PT( itDia, disciplina->diasLetivos, int )
//		 {
//			    int dia = *itDia;
//
//				for ( int turma = 0; turma < disciplina->getNumTurmas(); turma++ )
//				{
//					c.reset();
//					c.setType( ConstraintTatico::C_UNICO_ATEND_TURMA_DISC_DIA );
//
//					c.setCampus( *itCampus );
//					c.setTurma( turma );
//					c.setDisciplina( disciplina );
//					c.setDia( dia );
//
//					sprintf( name, "%s", c.toString().c_str() );
//
//					if ( cHashTatico.find( c ) != cHashTatico.end() )
//					{
//						continue;
//					}
//
//					nnz = ( itCampus->getTotalSalas() * 7 );
//
//					OPT_ROW row( nnz + 1, OPT_ROW::LESS , 1.0 , name );
//					
//					// Insere variaveis de Credito (x) ---
//
//					ITERA_GGROUP_LESSPTR( itUnidade, itCampus->unidades, Unidade )
//					{
//						ITERA_GGROUP_LESSPTR( itCjtSala, itUnidade->conjutoSalas, ConjuntoSala )
//						{
//							if ( itCjtSala->disciplinas_associadas.find( disciplina) ==
//								itCjtSala->disciplinas_associadas.end() )
//							{
//								continue;
//							}
//				   
//							int salaId = itCjtSala->salas.begin()->first;
//
//							GGroup< HorarioAula *, LessPtr< HorarioAula > > horariosEmComum = 
//								problemData->retornaHorariosEmComum( salaId, disciplina->getId(), dia );
//							
//							ITERA_GGROUP_LESSPTR( itHorario, horariosEmComum, HorarioAula )
//							{
//								HorarioAula *hi = *itHorario;
//
//								ITERA_GGROUP_LESSPTR( itHorario, horariosEmComum, HorarioAula )
//								{
//										HorarioAula *hf = *itHorario;
//
//										if ( hf < hi )
//										{
//							 				continue;
//										}
//
//										int NCH = disciplina->getCalendario()->retornaNroCreditosEntreHorarios( hi, hf );
//
//										v.reset();
//										v.setType( VariableTatico::V_CREDITOS );									
//										v.setTurma( turma );
//										v.setDisciplina( disciplina );
//										v.setUnidade( *itUnidade );
//										v.setSubCjtSala( *itCjtSala );
//										v.setDia( dia );                   
//										v.setHorarioAulaInicial( hi );	 // hi
//										v.setHorarioAulaFinal( hf );	 // hf
//
//										it_v = vHashTatico.find( v );
//										if( it_v != vHashTatico.end() )
//										{
//											row.insert( it_v->second, 1.0 );
//										}
//								}
//							}                  
//						}
//					}
//
//					// Insere restri��o no Hash ---
//
//					if ( row.getnnz() != 0 )
//					{
//						cHashTatico[ c ] = lp->getNumRows();
//
//						lp->addRow( row );
//						restricoes++;
//					}
//				}
//          }
//      }
//   }
//
//	return restricoes;
//
//}

int SolverMIP::criaRestricaoTaticoAtendeDemandaAluno( int campusId )
{
   int restricoes = 0;
   int nnz;
   char name[ 100 ];

   VariableTatico v;
   ConstraintTatico c;
   VariableTaticoHash::iterator it_v;
   
   Campus* cp = problemData->refCampus[ campusId ];

   ITERA_GGROUP_LESSPTR( itAluno, problemData->alunos, Aluno )
   {
	    Aluno *aluno = *itAluno;

	    if ( aluno->getOferta()->getCampusId() != campusId )
	    {
	 		continue;
	    }

		ITERA_GGROUP_LESSPTR( itAlDemanda, aluno->demandas, AlunoDemanda )
		{
			Disciplina *disciplina = (*itAlDemanda)->demanda->disciplina;

			#pragma region Equivalencias
			if ( problemData->mapDiscSubstituidaPor.find( disciplina ) !=
				 problemData->mapDiscSubstituidaPor.end() )
			{
				continue;
			}
			#pragma endregion

			int turma = problemData->retornaTurmaDiscAluno( aluno, disciplina );

			c.reset();
			c.setType( ConstraintTatico::C_DEMANDA_DISC_ALUNO );

			c.setCampus( cp );
			c.setTurma( turma );
			c.setDisciplina( disciplina );
			c.setAluno( aluno );

			sprintf( name, "%s", c.toString().c_str() );

			if ( cHashTatico.find( c ) != cHashTatico.end() )
			{
				continue;
			}
			
			nnz = 2;
			OPT_ROW row( nnz, OPT_ROW::EQUAL , 1.0 , name );

			// Variavel de abertura z_{i,d,cp}

			VariableTatico v;
			v.reset();
			v.setType( VariableTatico::V_ABERTURA );

			v.setDisciplina( disciplina );  // d
			v.setTurma( turma );			// i
			v.setCampus( cp );

			it_v = vHashTatico.find( v );
			if( it_v != vHashTatico.end() )
			{
				row.insert( it_v->second, 1.0 );
			}

			// Variavel de folga fd_{d,a}

			v.reset();
			v.setType( VariableTatico::V_SLACK_DEMANDA );

			v.setDisciplina( disciplina );  // d
			v.setAluno( aluno );			// a

			it_v = vHashTatico.find( v );
			if( it_v != vHashTatico.end() )
			{
				row.insert( it_v->second, 1.0 );
			}
			
			// Insere restri��o no Hash ---

			if ( row.getnnz() != 0 )
			{
				cHashTatico[ c ] = lp->getNumRows();

				lp->addRow( row );
				restricoes++;
			}

        }
   }

	return restricoes;
}



/*====================================================================/
%DocBegin TRIEDA_LOAD_MODEL

%Constraint 
Evitar aloca��o de turmas da mesma disciplina em campus diferentes
%Desc 

%MatExp

\begin{eqnarray}
\sum\limits_{cp \in CP} z_{i,d,cp}  \leq  1  \nonumber \qquad 
\forall d \in D \quad
\forall i \in I_{d}
\end{eqnarray}

%DocEnd
/====================================================================*/
/*
int SolverMIP::criaRestricaoTaticoEvitaTurmaDiscCampiDif()
{
   int restricoes = 0;
   char name[ 100 ];
   int nnz;

   ConstraintTatico c;
   VariableTatico v;
   VariableTaticoHash::iterator it_v;

   Disciplina * disciplina = NULL;
   
   ITERA_GGROUP_LESSPTR( it_disciplina, problemData->disciplinas, Disciplina )
   {
       disciplina = ( *it_disciplina );

		#pragma region Equivalencias
		if ( problemData->mapDiscSubstituidaPor.find( disciplina ) !=
				problemData->mapDiscSubstituidaPor.end() )
		{
			continue;
		}
		#pragma endregion	

      for ( int i = 0; i < disciplina->getNumTurmas(); ++i )
      {
         c.reset();
         c.setType( ConstraintTatico::C_EVITA_TURMA_DISC_CAMP_D );

         c.setDisciplina( disciplina );
         c.setTurma( i );

         sprintf( name, "%s", c.toString().c_str() ); 
         if ( cHashTatico.find( c ) != cHashTatico.end() )
         {
            continue;
         }

		 nnz = problemData->campi.size();
         OPT_ROW row( nnz, OPT_ROW::LESS , 1.0, name );

         ITERA_GGROUP_LESSPTR( it_campus, problemData->campi, Campus )
         {
            v.reset();
            v.setType( VariableTatico::V_ABERTURA );

            v.setTurma( i );
            v.setDisciplina( disciplina );

            v.setCampus( *it_campus );

            it_v = vHashTatico.find( v );
            if( it_v != vHashTatico.end() )
            {
               row.insert( it_v->second, 1.0 );
            }
         }

         if ( row.getnnz() != 0 )
         {
            cHashTatico[ c ] = lp->getNumRows();

            lp->addRow( row );
            restricoes++;
         }
      }
   }

   return restricoes;
}
*/



// Restricao 1.2.8
/*====================================================================/
%DocBegin TRIEDA_LOAD_MODEL

%Constraint 
Contabiliza se h� turmas da mesma disciplina em dias consecutivos (*)
%Desc 

%MatExp

\begin{eqnarray}
c_{i,d,t}  \geq \sum\limits_{u \in U_{cp}} \sum\limits_{s \in S_{u}}(x_{i,d,u,s,hi,hf,t} - x_{i,d,u,s,hi,hf,t-1}) - 1  \nonumber \qquad 
\forall cp \in CP \quad
\forall d \in D \quad
\forall i \in I_{d} \quad
\forall (t \geq 2) \in T
\end{eqnarray}

%DocEnd
/====================================================================*/
int SolverMIP::criaRestricaoTaticoTurmaDiscDiasConsec( int campusId )
{	
   int restricoes = 0;

   char name[ 100 ];
   int nnz;

   ConstraintTatico c;
   VariableTatico v;
   VariableTaticoHash::iterator it_v;

   Disciplina * disciplina = NULL;
   
   ITERA_GGROUP_LESSPTR( itCampus, problemData->campi, Campus )
   {
	   if ( itCampus->getId() != campusId )
	   {
			continue;
	   }
	   
	   ITERA_GGROUP_LESSPTR( it_disciplina, problemData->disciplinas, Disciplina )
	   {
		  disciplina = ( *it_disciplina );

		  #pragma region Equivalencias
		  if ( problemData->mapDiscSubstituidaPor.find( disciplina ) !=
			   problemData->mapDiscSubstituidaPor.end() )
		  {
			  continue;
		  }
		  #pragma endregion	

		  // A disciplina deve ser ofertada no campus especificado
		  if ( problemData->cp_discs[campusId].find( disciplina->getId() ) ==
			   problemData->cp_discs[campusId].end() )
		  {
			  continue;
		  }

		  for ( int turma = 0; turma < disciplina->getNumTurmas(); turma++ )
		  {
			 GGroup< int, std::less<int> >::iterator itDiasLetDisc = disciplina->diasLetivos.begin();
			 
			 // S� cria as restri��es a partir do segundo dia
			 // J� que a estrutura � ordenada, pula o primeiro.
			 if ( itDiasLetDisc != disciplina->diasLetivos.end() )
				itDiasLetDisc++;

			 for (; itDiasLetDisc != disciplina->diasLetivos.end(); itDiasLetDisc++ )
			 {
				 int dia = *itDiasLetDisc;

				c.reset();
				c.setType( ConstraintTatico::C_TURMA_DISC_DIAS_CONSEC );

				c.setCampus( *itCampus );
				c.setDisciplina( disciplina );
				c.setTurma( turma );
				c.setDia( dia );

				sprintf( name, "%s", c.toString().c_str() ); 

				if ( cHashTatico.find( c ) != cHashTatico.end() )
				{
				   continue;
				}

				nnz = ( problemData->totalSalas * 2 + 1 );
				OPT_ROW row( nnz, OPT_ROW::GREATER , -1.0 , name );

				v.reset();
				v.setType( VariableTatico::V_DIAS_CONSECUTIVOS );
				v.setTurma( turma );
				v.setDisciplina( disciplina );
				v.setDia( dia );

				it_v = vHashTatico.find( v );
				if ( it_v != vHashTatico.end() )
				{
				   row.insert( it_v->second, 1.0 );
				}

				ITERA_GGROUP_LESSPTR( itUnidade, itCampus->unidades, Unidade )
				{
					ITERA_GGROUP_LESSPTR( itCjtSala, itUnidade->conjutoSalas, ConjuntoSala )
					{
						int salaId = itCjtSala->salas.begin()->first;

						GGroup< HorarioAula *, LessPtr< HorarioAula > > horariosEmComum = 
							problemData->retornaHorariosEmComum( salaId, disciplina->getId(), dia );

						ITERA_GGROUP_LESSPTR( itHorario, horariosEmComum, HorarioAula )
						{
							HorarioAula *hi = *itHorario;

							ITERA_GGROUP_LESSPTR( itHorario, horariosEmComum, HorarioAula )
							{
								HorarioAula *hf = *itHorario;

								if ( hf < hi )
								{
							 		continue;
								}

								v.reset();
								v.setType( VariableTatico::V_CREDITOS );

								v.setTurma( turma );
								v.setDisciplina( disciplina );
								v.setUnidade( *itUnidade );
								v.setSubCjtSala( *itCjtSala );
								v.setHorarioAulaInicial( hi );
								v.setHorarioAulaFinal( hf );
								v.setDia( dia );

								it_v = vHashTatico.find( v );
								if ( it_v != vHashTatico.end() )
								{
									row.insert( it_v->second, -1.0 );
								}

								v.setDia( dia - 1 );

								it_v = vHashTatico.find( v );
								if ( it_v != vHashTatico.end() )
								{
									row.insert( it_v->second, -1.0 );
								}
							}
						}
					}
				}

				if ( row.getnnz() != 0 )
				{
				   cHashTatico[ c ] = lp->getNumRows();

				   lp->addRow( row );
				   restricoes++;
				}
			 }
		  }
	   }
	}
 
	return restricoes;
}


/*====================================================================/
%DocBegin TRIEDA_LOAD_MODEL

%Constraint 
N�o permitir que alunos de cursos diferentes incompat�veis compartilhem turmas (*)
%Desc 

%MatExp

\begin{eqnarray} 
b_{i,d,c,cp} + b_{i,d,c',cp} \leq 1 \nonumber \qquad 
\forall d \in D \quad
\forall i \in I_{d} \quad
\forall c,c' \notin CC \quad
\forall cp \in CP
\end{eqnarray}

%DocEnd
/====================================================================*/

/*
int SolverMIP::criaRestricaoTaticoCursosIncompat( int campusId )
{
   int restricoes = 0;
   char name[ 100 ];
   int nnz;

   ConstraintTatico c;
   VariableTatico v;
   VariableTaticoHash::iterator it_v;

	ITERA_GGROUP_LESSPTR( itCampus, problemData->campi, Campus )
	{
		if ( itCampus->getId() != campusId )
		{
			continue;
		}

		ITERA_GGROUP_LESSPTR( it1Cursos, problemData->cursos, Curso )
		{
			Curso* c1 = *it1Cursos;
			
			if ( itCampus->cursos.find( c1 ) == itCampus->cursos.end() )
			{
				continue;
			}

			ITERA_GGROUP_INIC_LESSPTR( it2Cursos, it1Cursos, problemData->cursos, Curso )
			{
				Curso* c2 = *it2Cursos;
			    
				if ( itCampus->cursos.find( c2 ) == itCampus->cursos.end() )
				{
					continue;
				}
				if ( problemData->cursosCompativeis(c1, c2) )
				{
					continue;
				}

				ITERA_GGROUP_LESSPTR( itDisc, problemData->disciplinas, Disciplina )
				{
					Disciplina *disciplina = *itDisc;

					// A disciplina deve ser ofertada no campus especificado
					if ( problemData->cp_discs[campusId].find( disciplina->getId() ) ==
						 problemData->cp_discs[campusId].end() )
					{
						continue;
					}

					#pragma region Equivalencias
					if ( problemData->mapDiscSubstituidaPor.find( disciplina ) !=
						problemData->mapDiscSubstituidaPor.end() )
					{
						continue;
					}
					#pragma endregion	
										
					if ( !c1->possuiDisciplina(disciplina) || !c2->possuiDisciplina(disciplina) )
						continue;
					

					for ( int turma = 0; turma < disciplina->getNumTurmas(); turma++ )
					{
						c.reset();
						c.setType( ConstraintTatico::C_ALUNOS_CURSOS_INCOMP );
						c.setCampus( *itCampus );
						c.setParCursos( std::make_pair( c1, c2 ) );
						c.setDisciplina( disciplina );
						c.setTurma( turma );

						sprintf( name, "%s", c.toString().c_str() ); 

						if ( cHashTatico.find( c ) != cHashTatico.end() )
						{
							continue;
						}

						nnz = 3;

						OPT_ROW row( nnz, OPT_ROW::LESS , 1.0 , name );

						v.reset();
						v.setType( VariableTatico::V_ALOC_ALUNO );
						v.setTurma( turma );
						v.setDisciplina( disciplina );
						v.setCurso( c1 );
						v.setCampus( *itCampus );

						it_v = vHashTatico.find( v );
						if( it_v != vHashTatico.end() )
						{
							row.insert( it_v->second, 1 );
						}

						v.reset();
						v.setType( VariableTatico::V_ALOC_ALUNO );
						v.setTurma( turma );
						v.setDisciplina( disciplina );
						v.setCurso( c2 );
						v.setCampus( *itCampus );

						it_v = vHashTatico.find( v );
						if( it_v != vHashTatico.end() )
						{
							row.insert(it_v->second, 1);
						}

						if ( row.getnnz() != 0 )
						{
							cHashTatico[ c ] = lp->getNumRows();
							lp->addRow( row );
							restricoes++;
						}
					}
				}
			}
		}
   }

   return restricoes;
}
*/

// Restricao 1.2.11
/*====================================================================/
%DocBegin TRIEDA_LOAD_MODEL

%Constraint 
N�o permitir que alunos de cursos diferentes (mesmo que compativeis) compartilhem turmas.
%Desc 

%MatExp

\begin{eqnarray} 
b_{i,d,c,cp} + b_{i,d,c',cp} - fc_{i,d,c,c',cp} \leq 1 \nonumber \qquad 
\forall d \in D \quad
\forall i \in I_{d} \quad
\forall c,c' C \quad
\forall cp \in CP
\end{eqnarray}

%DocEnd

/====================================================================*/
/*
int SolverMIP::criaRestricaoTaticoProibeCompartilhamento( int campusId )
{
   int restricoes = 0;

   if ( problemData->parametros->permite_compartilhamento_turma_sel )
   {
	   return restricoes;
   }

   char name[ 100 ];
   int nnz;

   ConstraintTatico c;
   VariableTatico v;
   VariableTaticoHash::iterator it_v;

   ITERA_GGROUP_LESSPTR( itCampus, problemData->campi, Campus )
   {
	   if ( itCampus->getId() != campusId )
	   {
		   continue;
	   }

	   std::map< std::pair< Curso *, Curso * >, std::vector< int > >::iterator
               it_cursoComp_disc = problemData->cursosComp_disc.begin();

        for (; it_cursoComp_disc != problemData->cursosComp_disc.end(); it_cursoComp_disc++ )
        {
			Curso *c1 = it_cursoComp_disc->first.first;
			Curso *c2 = it_cursoComp_disc->first.second;

			if ( itCampus->cursos.find( c1 ) == itCampus->cursos.end() ||
				 itCampus->cursos.find( c2 ) == itCampus->cursos.end() )
			{
				continue;
			}

            std::vector< int >::iterator it_disc = it_cursoComp_disc->second.begin();

            for (; it_disc != it_cursoComp_disc->second.end(); ++it_disc )
            {
				int discId = *it_disc;
				Disciplina * disciplina = problemData->retornaDisciplina(discId);
				  
				if (disciplina == NULL) continue;

				#pragma region Equivalencias
				if ( problemData->mapDiscSubstituidaPor.find( disciplina ) !=
					problemData->mapDiscSubstituidaPor.end() )
				{
					continue;
				}
				#pragma endregion

				for ( int turma = 0; turma < disciplina->getNumTurmas(); turma++ )
				{
					c.reset();
					c.setType( ConstraintTatico::C_PROIBE_COMPARTILHAMENTO );
					c.setCampus( *itCampus );
					c.setParCursos( std::make_pair( c1, c2 ) );
					c.setDisciplina( disciplina );
					c.setTurma( turma );

					sprintf( name, "%s", c.toString().c_str() ); 

					if ( cHash.find( c ) != cHash.end() )
					{
						continue;
					}

					nnz = 3;

					OPT_ROW row( nnz, OPT_ROW::LESS , 1.0 , name );

					v.reset();
					v.setType( VariableTatico::V_ALOC_ALUNO );
					v.setTurma( turma );
					v.setDisciplina( disciplina );
					v.setCurso( c1 );
					v.setCampus( *itCampus );

					it_v = vHashTatico.find( v );
					if( it_v != vHashTatico.end() )
					{
						row.insert( it_v->second, 1 );
					}

					v.reset();
					v.setType( VariableTatico::V_ALOC_ALUNO );
					v.setTurma( turma );
					v.setDisciplina( disciplina );
					v.setCurso( c2 );
					v.setCampus( *itCampus );

					it_v = vHashTatico.find( v );
					if( it_v != vHashTatico.end() )
					{
						row.insert(it_v->second, 1);
					}

					v.reset();
					v.setType( VariableTatico::V_SLACK_COMPARTILHAMENTO );
					v.setTurma( turma );
					v.setDisciplina( disciplina );
					v.setParCursos( std::make_pair( c1, c2 ) );
					v.setCampus( *itCampus );

					it_v = vHashTatico.find( v );
					if ( it_v != vHashTatico.end() )
					{
						row.insert( it_v->second, -1 );
					}

					if ( row.getnnz() != 0 )
					{
						cHashTatico[ c ] = lp->getNumRows();
						lp->addRow( row );
						restricoes++;
					}
				}
			}
		}
   }

   return restricoes;
}
*/

// Restricao 1.2.12
int SolverMIP::criaRestricaoTaticoLimitaAberturaTurmas( int campusId )
{
   int restricoes = 0;
	
   int nnz;
   char name[ 100 ];

   VariableTatico v;
   ConstraintTatico c;
   VariableTaticoHash::iterator it_v;

   Disciplina * disciplina = NULL;

   int minAlunos;
   if ( problemData->parametros->min_alunos_abertura_turmas )
   {
		minAlunos = problemData->parametros->min_alunos_abertura_turmas_value;
		if ( minAlunos <= 0 ) minAlunos = 1;
   }
   else
   {
	   minAlunos = 1;
   }

   ITERA_GGROUP_LESSPTR( itCampus, problemData->campi, Campus )
   {
	   if ( itCampus->getId() != campusId )
	   {
			continue;
	   }

      ITERA_GGROUP_LESSPTR( it_disciplina, problemData->disciplinas, Disciplina )
      {
         disciplina = ( *it_disciplina );

		 // A disciplina deve ser ofertada no campus especificado
		 if ( problemData->cp_discs[campusId].find( disciplina->getId() ) ==
			  problemData->cp_discs[campusId].end() )
		 {
			 continue;
		 }

		 #pragma region Equivalencias
		 if ( problemData->mapDiscSubstituidaPor.find( disciplina ) !=
				problemData->mapDiscSubstituidaPor.end() )
		 {
			continue;
		 }
		 #pragma endregion

         for ( int turma = 0; turma < disciplina->getNumTurmas(); turma++ )
         {
            c.reset();
            c.setType( ConstraintTatico::C_LIMITA_ABERTURA_TURMAS );

            c.setCampus( *itCampus );
            c.setTurma( turma );
            c.setDisciplina( disciplina );

            sprintf( name, "%s", c.toString().c_str() );

            if ( cHashTatico.find( c ) != cHashTatico.end() )
            {
               continue;
            }

            nnz = ( itCampus->getTotalSalas() * 7 );

            OPT_ROW row( nnz + 1, OPT_ROW::LESS , 0 , name );

            v.reset();
            v.setType( VariableTatico::V_CREDITOS );

            // Insere variaveis Credito (x) ---

            ITERA_GGROUP_LESSPTR( itUnidade, itCampus->unidades, Unidade )
            {
               ITERA_GGROUP_LESSPTR( itCjtSala, itUnidade->conjutoSalas, ConjuntoSala )
               {
				   if ( itCjtSala->disciplinas_associadas.find( disciplina) ==
					    itCjtSala->disciplinas_associadas.end() )
				   {
					   continue;
				   }
				   
				   int salaId = itCjtSala->salas.begin()->first;
					
				   ITERA_GGROUP_N_PT ( it_Dia, itCjtSala->dias_letivos_disciplinas[disciplina], int )
				   {
						int dia = *it_Dia;

						GGroup< HorarioAula *, LessPtr< HorarioAula > > horariosEmComum = 
							problemData->retornaHorariosEmComum( salaId, disciplina->getId(), dia );
					  
						ITERA_GGROUP_LESSPTR( itHorario, horariosEmComum, HorarioAula )
						{
							HorarioAula *hi = *itHorario;

							ITERA_GGROUP_LESSPTR( itHorario, horariosEmComum, HorarioAula )
							{
									HorarioAula *hf = *itHorario;

									if ( hf < hi )
									{
							 			continue;
									}
																		
									v.setTurma( turma );
									v.setDisciplina( disciplina );
									v.setUnidade( *itUnidade );
									v.setSubCjtSala( *itCjtSala );
									v.setDia( dia );                   
									v.setHorarioAulaInicial( hi );	 // hi
									v.setHorarioAulaFinal( hf );	 // hf

									it_v = vHashTatico.find( v );
									if( it_v != vHashTatico.end() )
									{
										row.insert( it_v->second, -1.0 );
									}
							}
						}
					}                  
                }
            }

            // Insere variaveis Abertura (z) ---

            v.reset();
            v.setType( VariableTatico::V_ABERTURA );

            v.setCampus( *itCampus );
            v.setDisciplina( disciplina );
            v.setTurma( turma );

            it_v = vHashTatico.find( v );
            if( it_v != vHashTatico.end() )
            {
               row.insert( it_v->second, minAlunos );
            }

            // Insere restri��o no Hash ---

            if ( row.getnnz() != 0 )
            {
               cHashTatico[ c ] = lp->getNumRows();

               lp->addRow( row );
               restricoes++;
            }
         }
      }
   }

	return restricoes;
}



// Restricao 1.2.12
/*====================================================================/
%DocBegin TRIEDA_LOAD_MODEL

%Constraint 
Abertura sequencial de turmas
%Desc 

%MatExp

\begin{eqnarray}
\sum\limits_{u \in U_{cp}} \sum\limits_{s \in S_{u}} \sum\limits_{t \in T} \sum\limits_{hi \in H} \sum\limits_{hf \in H} x_{i,d,u,s,hi,hf,t} 
\geq \sum\limits_{u \in U_{cp}} \sum\limits_{s \in S_{u}} \sum\limits_{t \in T} \sum\limits_{hi \in H} \sum\limits_{hf \in H} x_{i',d,u,s,hi,hf,t}
\nonumber \qquad 

\forall cp \in CP \quad
\forall d \in D \quad
\forall i,i' \in I_{d}
\end{eqnarray}

%DocEnd
/====================================================================*/
/*
int SolverMIP::criaRestricaoTaticoAbreTurmasEmSequencia( int campusId )
{
   int restricoes = 0;
   char name[ 100 ];
   int nnz;

   VariableTatico v;
   ConstraintTatico c;
   VariableTaticoHash::iterator it_v;

   Disciplina * disciplina = NULL;

   ITERA_GGROUP_LESSPTR( it_disciplina, problemData->disciplinas, Disciplina )
   {
      disciplina = ( *it_disciplina );

	  // A disciplina deve ser ofertada no campus especificado
	  if ( problemData->cp_discs[campusId].find( disciplina->getId() ) ==
			problemData->cp_discs[campusId].end() )
 	  {
		  continue;
	  }

	  #pragma region Equivalencias
	  if ( problemData->mapDiscSubstituidaPor.find( disciplina ) !=
		   problemData->mapDiscSubstituidaPor.end() )
	  {
		  continue;
	  }
	  #pragma endregion	

      if ( disciplina->getNumTurmas() > 1 )
      {
         for ( int turma = 0; turma < ( disciplina->getNumTurmas() - 1 ); turma++ )
         {
            c.reset();
            c.setType( ConstraintTatico::C_ABRE_TURMAS_EM_SEQUENCIA );

			c.setCampus( problemData->refCampus[campusId] );
            c.setDisciplina( disciplina );
            c.setTurma( turma );

            sprintf( name, "%s", c.toString().c_str() ); 
            if ( cHashTatico.find( c ) != cHashTatico.end() )
            {
               continue;
            }

            nnz = 9999;
            OPT_ROW row( nnz, OPT_ROW::GREATER , 0.0, name );

            ITERA_GGROUP_LESSPTR( itCampus, problemData->campi, Campus )
            {
				if ( itCampus->getId() != campusId )
				{
					continue;
				}

                ITERA_GGROUP_LESSPTR( itUnidade, itCampus->unidades, Unidade )
                {
                    ITERA_GGROUP_LESSPTR( itCjtSala, itUnidade->conjutoSalas, ConjuntoSala )
                    {
						  int salaId = itCjtSala->salas.begin()->first;
                      
						  GGroup< int > disc_sala_dias = problemData->disc_Conjutno_Salas__Dias[ std::make_pair<int,int>
																		( disciplina->getId(), itCjtSala->getId() ) ];

						  ITERA_GGROUP_N_PT( itDia, disc_sala_dias, int )
						  {
								int dia = *itDia;

								GGroup< HorarioAula *, LessPtr< HorarioAula > > horariosEmComum = 
									problemData->retornaHorariosEmComum( salaId, disciplina->getId(), dia );

								ITERA_GGROUP_LESSPTR( itHorario, horariosEmComum, HorarioAula )
								{
									HorarioAula *hi = *itHorario;

									ITERA_GGROUP_LESSPTR( itHorario, horariosEmComum, HorarioAula )
									{
										HorarioAula *hf = *itHorario;

										if ( hf < hi )
										{
							 				continue;
										}

										v.reset();
										v.setType( VariableTatico::V_CREDITOS );

										v.setTurma( turma );
										v.setDisciplina( disciplina );
										v.setUnidade( *itUnidade );
										v.setSubCjtSala( *itCjtSala );
										v.setHorarioAulaInicial( hi );
										v.setHorarioAulaFinal( hf );
										v.setDia( dia );

										it_v = vHashTatico.find( v );
										if ( it_v != vHashTatico.end() )
										{
											row.insert( it_v->second, 1.0 );
										}

										v.reset();
										v.setType( VariableTatico::V_CREDITOS );

										int proxTurma = turma + 1;
										v.setTurma(proxTurma);

										v.setDisciplina( disciplina );
										v.setUnidade( *itUnidade );
										v.setSubCjtSala( *itCjtSala );
										v.setHorarioAulaInicial( hi );
										v.setHorarioAulaFinal( hf );
										v.setDia( dia );

										it_v = vHashTatico.find( v );
										if ( it_v != vHashTatico.end() )
										{
											row.insert( it_v->second, -1.0 );
										}
									}
								}
							}
						}
				  }
            }

            if ( row.getnnz() != 0 )
            {
               cHashTatico[ c ] = lp->getNumRows();

               lp->addRow( row );
               restricoes++;
            }
         }
      }
   }

   return restricoes;
}

*/

/*====================================================================/
%DocBegin TRIEDA_LOAD_MODEL

%Constraint 
Regra de divis�o de cr�ditos
%Desc 

%MatExp

\begin{eqnarray}
\sum\limits_{u \in U} \sum\limits_{s \in S_{u}} \sum\limits_{hi \in H} \sum\limits_{hf \in H} x_{i,d,u,s,hi,hf,t} = 
 \sum\limits_{k \in K_{d}}N_{d,k,t} \cdot m_{d,i,k} + fkp_{d,i,t} - fkm_{d,i,t} \nonumber \qquad 
\forall d \in D \quad
\forall i \in I_{d} \quad
\forall t \in T
\end{eqnarray}

%Data N_{d,k,t}
%Desc
n�mero de cr�ditos determinados para a disciplina $d$ no dia $t$ na combina��o de divis�o de cr�dito $k$.

%DocEnd
/====================================================================*/

int SolverMIP::criaRestricaoTaticoDivisaoCredito( int campusId )
{
   int restricoes = 0;
   char name[ 100 ];
   int nnz;

   ConstraintTatico c;
   VariableTatico v;
   VariableTaticoHash::iterator it_v;

   Disciplina * disciplina = NULL;

   ITERA_GGROUP_LESSPTR( it_disciplina, problemData->disciplinas, Disciplina )
   {
      disciplina = ( *it_disciplina );
	  
	  // A disciplina deve ser ofertada no campus especificado
	  if ( problemData->cp_discs[campusId].find( disciplina->getId() ) ==
			problemData->cp_discs[campusId].end() )
 	  {
		  continue;
	  }

	  #pragma region Equivalencias
	  if ( problemData->mapDiscSubstituidaPor.find( disciplina ) !=
		   problemData->mapDiscSubstituidaPor.end() )
	  {
		  continue;
	  }
	  #pragma endregion	

      if ( disciplina->divisao_creditos != NULL )
      {
         for ( int turma = 0; turma < disciplina->getNumTurmas(); turma++ )
         {
            ITERA_GGROUP_N_PT( itDiasLetDisc, disciplina->diasLetivos, int )
            {
				int dia = *itDiasLetDisc;

                c.reset();
                c.setType( ConstraintTatico::C_DIVISAO_CREDITO );

                c.setDisciplina( disciplina );
                c.setTurma( turma );
                c.setDia( dia );

                sprintf( name, "%s", c.toString().c_str() ); 

                if ( cHashTatico.find( c ) != cHashTatico.end() )
                {
                   continue;
                }

                nnz = ( problemData->totalSalas + ( (int)( disciplina->combinacao_divisao_creditos.size() ) * 2 ) );
                OPT_ROW row( nnz, OPT_ROW::EQUAL , 0.0 , name );

                ITERA_GGROUP_LESSPTR( itCampus, problemData->campi, Campus )
                {
				   if ( itCampus->getId() != campusId )
				   {
					    continue;
				   }

                   ITERA_GGROUP_LESSPTR( itUnidade, itCampus->unidades, Unidade )
                   {
                       ITERA_GGROUP_LESSPTR( itCjtSala, itUnidade->conjutoSalas, ConjuntoSala )
                       {
						   int salaId = itCjtSala->salas.begin()->first;

							GGroup< HorarioAula *, LessPtr< HorarioAula > > horariosEmComum = 
								problemData->retornaHorariosEmComum( salaId, disciplina->getId(), dia );

							ITERA_GGROUP_LESSPTR( itHorario, horariosEmComum, HorarioAula )
							{
								HorarioAula *hi = *itHorario;

								ITERA_GGROUP_LESSPTR( itHorario, horariosEmComum, HorarioAula )
								{
									HorarioAula *hf = *itHorario;

									if ( hf < hi )
									{
							 			continue;
									}

									v.reset();
									v.setType( VariableTatico::V_CREDITOS );

									v.setTurma( turma );
									v.setDisciplina( disciplina );
									v.setUnidade( *itUnidade );
									v.setSubCjtSala( *itCjtSala );
									v.setHorarioAulaInicial( hi );
									v.setHorarioAulaFinal( hf );
									v.setDia( *itDiasLetDisc );

									int nCreds = disciplina->getCalendario()->retornaNroCreditosEntreHorarios(hi,hf);

									it_v = vHashTatico.find( v );
									if ( it_v != vHashTatico.end() )
									{
									   row.insert( it_v->second, nCreds );
									}
								}
							}
                        }
                    }
                }

				for ( int k = 0; k < (int)disciplina->combinacao_divisao_creditos.size(); k++ )
				{	
					v.reset();
					v.setType( VariableTatico::V_COMBINACAO_DIVISAO_CREDITO );

					v.setDisciplina( disciplina );
					v.setTurma( turma );
					v.setK( k );
					
					// N{d,k,t}
					int numCreditos = ( disciplina->combinacao_divisao_creditos[ k ] )[ dia - 1 ].second;

					it_v = vHashTatico.find( v );
					if ( it_v != vHashTatico.end() )
					{
						row.insert( it_v->second, -numCreditos );
					}

					v.reset();
					v.setType( VariableTatico::V_SLACK_COMBINACAO_DIVISAO_CREDITO_M );

					v.setDisciplina( disciplina );
					v.setTurma( turma );
					v.setDia( dia );

					it_v = vHashTatico.find( v );
					if( it_v != vHashTatico.end() )
					{
						row.insert( it_v->second, 1.0 );
					}

					v.reset();
					v.setType( VariableTatico::V_SLACK_COMBINACAO_DIVISAO_CREDITO_P );

					v.setDisciplina( disciplina );
					v.setTurma( turma );
					v.setDia( dia );

					it_v = vHashTatico.find( v );
					if( it_v != vHashTatico.end() )
					{
						row.insert( it_v->second, -1.0 );
					}
				}

                if ( row.getnnz() != 0 )
                {
                   cHashTatico[ c ] = lp->getNumRows();

                   lp->addRow( row );
                   restricoes++;
                }
            }
         }
      }
   }

   return restricoes;
}


// Restricao 1.2.15
/*====================================================================/
%DocBegin TRIEDA_LOAD_MODEL

%Constraint 
Somente uma combina��o de regra de divis�o de cr�ditos pode ser escolhida
%Desc 

%MatExp

\begin{eqnarray}
\sum\limits_{k \in K_{d}} m_{d,i,k} \leq 1 \nonumber \qquad 
\forall d \in D \quad
\forall i \in I_{d}
\end{eqnarray}

%DocEnd
/====================================================================*/
int SolverMIP::criaRestricaoTaticoCombinacaoDivisaoCredito( int campusId )
{
   int restricoes = 0;
   char name[ 100 ];
   int nnz;

   VariableTatico v;
   ConstraintTatico c;
   VariableTaticoHash::iterator it_v;

   Disciplina * disciplina = NULL;
   
   ITERA_GGROUP_LESSPTR( it_disciplina, problemData->disciplinas, Disciplina )
   {
      disciplina = ( *it_disciplina );

	  // A disciplina deve ser ofertada no campus especificado
	  if ( problemData->cp_discs[campusId].find( disciplina->getId() ) ==
			problemData->cp_discs[campusId].end() )
 	  {
		  continue;
	  }

	  #pragma region Equivalencias
	  if ( problemData->mapDiscSubstituidaPor.find( disciplina ) !=
		   problemData->mapDiscSubstituidaPor.end() )
	  {
		  continue;
	  }
	  #pragma endregion	

      for ( int i = 0; i < disciplina->getNumTurmas(); i++ )
      {
         c.reset();
         c.setType( ConstraintTatico::C_COMBINACAO_DIVISAO_CREDITO );

         c.setDisciplina( disciplina );
         c.setTurma( i );

         sprintf( name, "%s", c.toString().c_str() ); 

         if ( cHashTatico.find( c ) != cHashTatico.end() )
         {
            continue;
         }

         nnz = (int)( disciplina->combinacao_divisao_creditos.size() );
         OPT_ROW row( nnz, OPT_ROW::LESS , 1.0, name );

         for ( int k = 0; k < (int)disciplina->combinacao_divisao_creditos.size(); k++ )
         {
            v.reset();
            v.setType( VariableTatico::V_COMBINACAO_DIVISAO_CREDITO );

            v.setTurma( i );
            v.setDisciplina( disciplina );
            v.setK( k );

            it_v = vHashTatico.find( v );

            if ( it_v != vHashTatico.end() )
            {
               row.insert( it_v->second, 1.0 );
            }
         }

         if ( row.getnnz() != 0 )
         {
            cHashTatico[ c ] = lp->getNumRows();

            lp->addRow( row );
            restricoes++;
         }
      }
   }

   return restricoes;
}


// Restricao 1.2.16
/*====================================================================/
%DocBegin TRIEDA_LOAD_MODEL

%Constraint 
Ativa��o da vari�vel zc
%Desc

%MatExp
\begin{eqnarray}
\sum\limits_{i \in I} \sum\limits_{u \in U} \sum\limits_{s \in S_{u}} \sum\limits_{hi \in H} \sum\limits_{hf \in H}
  x_{i,d,u,s,hi,hf,t} \leq zc_{d,t} \cdot N \nonumber \qquad 
\forall cp \in CP \quad
\forall d \in D \quad
\forall t \in T
\end{eqnarray}


%DocEnd
/====================================================================*/
int SolverMIP::criaRestricaoTaticoAtivacaoVarZC( int campusId )
{
   int restricoes = 0;
   char name[ 100 ];
   int nnz;

   ConstraintTatico c;
   VariableTatico v;
   VariableTaticoHash::iterator it_v;

   Disciplina * disciplina = NULL;

   ITERA_GGROUP_LESSPTR( itCampus, problemData->campi, Campus )
   {
	   if ( itCampus->getId() != campusId )
	   {
		   continue;
	   }

	   ITERA_GGROUP_LESSPTR( it_disciplina, problemData->disciplinas, Disciplina )
	   {
		  disciplina = ( *it_disciplina );
	  
		  // A disciplina deve ser ofertada no campus especificado
		  if ( problemData->cp_discs[campusId].find( disciplina->getId() ) ==
				problemData->cp_discs[campusId].end() )
 		  {
			  continue;
		  }

		  #pragma region Equivalencias
		  if ( problemData->mapDiscSubstituidaPor.find( disciplina ) !=
			   problemData->mapDiscSubstituidaPor.end() )
		  {
			   continue;
		  }
		  #pragma endregion

		  GGroup< int >::iterator itDiasLetDisc = disciplina->diasLetivos.begin();

		  for (; itDiasLetDisc != disciplina->diasLetivos.end(); itDiasLetDisc++ )
		  {
             int dia = *itDiasLetDisc;

			 c.reset();
			 c.setType( ConstraintTatico::C_VAR_ZC );

			 c.setCampus( *itCampus );
			 c.setDisciplina( disciplina );
			 c.setDia( dia );

			 sprintf( name, "%s", c.toString().c_str() ); 
			 if ( cHashTatico.find( c ) != cHashTatico.end() )
			 {
				continue;
			 }

			 nnz = 100;

			 OPT_ROW row( nnz, OPT_ROW::LESS, 0.0, name );
 
			 ITERA_GGROUP_LESSPTR( itUnidade, itCampus->unidades, Unidade )
			 {
				ITERA_GGROUP_LESSPTR( itCjtSala, itUnidade->conjutoSalas, ConjuntoSala )
				{
					int salaId = itCjtSala->salas.begin()->first;

					GGroup< HorarioAula *, LessPtr< HorarioAula > > horariosEmComum = 
						problemData->retornaHorariosEmComum( salaId, disciplina->getId(), dia );

					ITERA_GGROUP_LESSPTR( itHorario, horariosEmComum, HorarioAula )
					{
						HorarioAula *hi = *itHorario;

						ITERA_GGROUP_LESSPTR( itHorario, horariosEmComum, HorarioAula )
						{
							HorarioAula *hf = *itHorario;

							if ( hf < hi )
							{
							 	continue;
							}

							for ( int turma = 0; turma < disciplina->getNumTurmas(); turma++ )
							{
								v.reset();
								v.setType( VariableTatico::V_CREDITOS );

								v.setTurma( turma );
								v.setDisciplina( disciplina );
								v.setUnidade( *itUnidade );
								v.setSubCjtSala( *itCjtSala );
								v.setHorarioAulaInicial( hi );
								v.setHorarioAulaFinal( hf );
								v.setDia( dia );

								it_v = vHashTatico.find( v );
								if ( it_v != vHashTatico.end() )
								{
									row.insert( it_v->second, 1.0 );
								}
							}
						}
					}
				}
			 }

			 v.reset();
			 v.setType( VariableTatico::V_ABERTURA_COMPATIVEL );

			 v.setDisciplina( disciplina );
			 v.setDia( *itDiasLetDisc );
			 v.setCampus( *itCampus );

			 it_v = vHashTatico.find( v );
			 if ( it_v != vHashTatico.end() )
			 {
				 row.insert( it_v->second, - disciplina->getNumTurmas() );
			 }

			 if ( row.getnnz() != 0 )
			 {
				cHashTatico[ c ] = lp->getNumRows();

				lp->addRow( row );
				restricoes++;
			 }
		  }
	   }
   }

   return restricoes;
}



/*====================================================================/
%DocBegin TRIEDA_LOAD_MODEL

%Constraint 
Disciplinas incompat�veis
%Desc

%MatExp
\begin{eqnarray}
zc_{d_1,t} + zc_{d_2,t} \leq 1 \nonumber \qquad 
(d_1, d_2),
\forall t \in T
\end{eqnarray}

%DocEnd
/====================================================================*/

int SolverMIP::criaRestricaoTaticoDisciplinasIncompativeis( int campusId )
{
   int restricoes = 0;
   char name[ 100 ];
   int nnz;

   ConstraintTatico c;
   VariableTatico v;
   VariableTaticoHash::iterator it_v;

   Disciplina * disciplina = NULL;

   Campus *campus = problemData->refCampus[campusId];
   
   ITERA_GGROUP_LESSPTR( it_disciplina, problemData->disciplinas, Disciplina )
   {
      disciplina = ( *it_disciplina );
	  
	  // A disciplina deve ser ofertada no campus especificado
	  if ( problemData->cp_discs[campusId].find( disciplina->getId() ) ==
			problemData->cp_discs[campusId].end() )
 	  {
		  continue;
	  }

	  #pragma region Equivalencias
	  if ( problemData->mapDiscSubstituidaPor.find( disciplina ) !=
		   problemData->mapDiscSubstituidaPor.end() )
	  {
		   continue;
	  }
	  #pragma endregion

      ITERA_GGROUP_N_PT( itDiasLetDisc, disciplina->diasLetivos, int )
      {
         ITERA_GGROUP_N_PT( it_inc, disciplina->ids_disciplinas_incompativeis, int )
         {
			std::map< int, Disciplina* >::iterator it_Ref_Disc = problemData->refDisciplinas.find( *it_inc );
			if ( it_Ref_Disc == problemData->refDisciplinas.end() )
			{ 
				continue;
			}
			
			Disciplina * nova_disc = it_Ref_Disc->second;

			// A disciplina deve ser ofertada no campus especificado
			if ( problemData->cp_discs[campusId].find( nova_disc->getId() ) ==
				problemData->cp_discs[campusId].end() )
 			{
				continue;
			}

			#pragma region Equivalencias
			if ( problemData->mapDiscSubstituidaPor.find( nova_disc ) !=
				problemData->mapDiscSubstituidaPor.end() )
			{
				continue;
			}
			#pragma endregion

            c.reset();
            c.setType( ConstraintTatico::C_DISC_INCOMPATIVEIS );
			c.setParDisciplinas( std::make_pair( nova_disc, disciplina ) );
			c.setCampus( campus );
			c.setDia( *itDiasLetDisc );

            sprintf( name, "%s", c.toString().c_str() ); 

            if ( cHashTatico.find( c ) != cHashTatico.end() )
            {
               continue;
            }

            nnz = 2;
            OPT_ROW row( nnz, OPT_ROW::LESS, 1.0, name );

            v.reset();
            v.setType( VariableTatico::V_ABERTURA_COMPATIVEL );

            v.setDisciplina( disciplina );
            v.setDia( *itDiasLetDisc );
			v.setCampus( campus );

            it_v = vHashTatico.find( v );
            if ( it_v != vHashTatico.end() )
            {
               row.insert( it_v->second, 1.0 );
            }

            v.setDisciplina( nova_disc );

            it_v = vHashTatico.find( v );
			if ( it_v != vHashTatico.end() )
            {
               row.insert(it_v->second, 1.0);
            }

            if ( row.getnnz() != 0 )
            {
               cHashTatico[ c ] = lp->getNumRows();

               lp->addRow( row );
               restricoes++;
            }

         }
      }
   }

   return restricoes;
}

// Restricao 1.2.18
/*
int SolverMIP::criaRestricaoTaticoUnicaSalaParaTurmaDisc( int campusId )
{
   int restricoes = 0;

   int nnz;
   char name[ 100 ];

   VariableTatico v;
   ConstraintTatico c;
   VariableTaticoHash::iterator it_v;

   Disciplina * disciplina = NULL;

   ITERA_GGROUP_LESSPTR( itCampus, problemData->campi, Campus )
   {
	   if ( itCampus->getId() != campusId )
	   {
			continue;
	   }

      ITERA_GGROUP_LESSPTR( it_disciplina, problemData->disciplinas, Disciplina )
      {
         disciplina = ( *it_disciplina );

		 // A disciplina deve ser ofertada no campus especificado
		 if ( problemData->cp_discs[campusId].find( disciplina->getId() ) ==
			  problemData->cp_discs[campusId].end() )
		 {
			 continue;
		 }

		 #pragma region Equivalencias
		 if ( problemData->mapDiscSubstituidaPor.find( disciplina ) !=
				problemData->mapDiscSubstituidaPor.end() )
		 {
			continue;
		 }
		 #pragma endregion

         for ( int turma = 0; turma < disciplina->getNumTurmas(); turma++ )
         {
            c.reset();
            c.setType( ConstraintTatico::C_TURMA_SALA );

            c.setCampus( *itCampus );
            c.setTurma( turma );
            c.setDisciplina( disciplina );

            sprintf( name, "%s", c.toString().c_str() );

            if ( cHashTatico.find( c ) != cHashTatico.end() )
            {
               continue;
            }

            nnz = ( itCampus->getTotalSalas() * 7 );

            OPT_ROW row( nnz, OPT_ROW::LESS, 1, name );

            v.reset();
            v.setType( VariableTatico::V_CREDITOS );

            // Insere variaveis Credito (x) ---

            ITERA_GGROUP_LESSPTR( itUnidade, itCampus->unidades, Unidade )
            {
               ITERA_GGROUP_LESSPTR( itCjtSala, itUnidade->conjutoSalas, ConjuntoSala )
               {
				   if ( itCjtSala->disciplinas_associadas.find( disciplina) ==
					    itCjtSala->disciplinas_associadas.end() )
				   {
					   continue;
				   }
				   
				   int salaId = itCjtSala->salas.begin()->first;
					
				   ITERA_GGROUP_N_PT ( it_Dia, itCjtSala->dias_letivos_disciplinas[disciplina], int )
				   {
						int dia = *it_Dia;

						GGroup< HorarioAula *, LessPtr< HorarioAula > > horariosEmComum = 
							problemData->retornaHorariosEmComum( salaId, disciplina->getId(), dia );
					  
						ITERA_GGROUP_LESSPTR( itHorario, horariosEmComum, HorarioAula )
						{
							HorarioAula *hi = *itHorario;

							ITERA_GGROUP_LESSPTR( itHorario, horariosEmComum, HorarioAula )
							{
								HorarioAula *hf = *itHorario;

								if ( hf < hi )
								{
							 		continue;
								}
																		
								v.setTurma( turma );
								v.setDisciplina( disciplina );
								v.setUnidade( *itUnidade );
								v.setSubCjtSala( *itCjtSala );
								v.setDia( dia );                   
								v.setHorarioAulaInicial( hi );	 // hi
								v.setHorarioAulaFinal( hf );	 // hf

								it_v = vHashTatico.find( v );
								if( it_v != vHashTatico.end() )
								{
									row.insert( it_v->second, 1.0 );
								}
							}
						}
					}                  
                }
            }

            // Insere restri��o no Hash ---

            if ( row.getnnz() != 0 )
            {
               cHashTatico[ c ] = lp->getNumRows();

               lp->addRow( row );
               restricoes++;
            }
         }
      }
   }

	return restricoes;
}
*/

int SolverMIP::criaRestricaoTaticoEvitaSobreposicaoAulaAluno( int campusId )
{
   int restricoes = 0;
   int nnz;
   char name[ 100 ];

   //VariableTatico v;
   ConstraintTatico c;
   //VariableTaticoHash::iterator it_v;
   ConstraintTaticoHash::iterator cit;

   //Disciplina * disciplina = NULL;

   Campus *campus = NULL;

   ITERA_GGROUP_LESSPTR( itCampus, problemData->campi, Campus )
   {
	    if ( itCampus->getId() != campusId )
			continue;

		campus = *itCampus;
		break;
   }

   if(!campus)
	   return 0;

   map< int, map< Disciplina*, set< Aluno *, LessPtr< Aluno > >, LessPtr< Disciplina > > > mapAlunos;

   ITERA_GGROUP_LESSPTR( itAluno, problemData->alunos, Aluno )
   {
	   Aluno *aluno = *itAluno;
	   GGroup< Trio< int, int, Disciplina* > >::iterator itT = problemData->mapAluno_CampusTurmaDisc[ aluno ].begin();
	   for(; itT != problemData->mapAluno_CampusTurmaDisc[ aluno ].end(); itT++)
	   {
		   Trio< int, int, Disciplina* > trio = *itT;
		   if(trio.first != campusId)
			   continue;

		   mapAlunos[trio.second][trio.third].insert(aluno);
	   }
   }


   VariableTaticoHash::iterator vit;

   vit = vHashTatico.begin();
   while(vit != vHashTatico.end())
   {
	   if(vit->first.getType() == VariableTatico::V_CREDITOS)
	   {
		   VariableTatico v = vit->first;

		   HorarioAula *hi = v.getHorarioAulaInicial();
		   HorarioAula *hf = v.getHorarioAulaFinal();

		   set< Aluno *, LessPtr< Aluno > >::iterator itA = mapAlunos[v.getTurma()][v.getDisciplina()].begin();
		   for(; itA != mapAlunos[v.getTurma()][v.getDisciplina()].end(); itA++)
		   {
			   Aluno *aluno = *itA;

			   ITERA_GGROUP( itHor, campus->horarios, Horario )
			   {
				   if ( itHor->dias_semana.find( v.getDia() ) ==
					   itHor->dias_semana.end() )			
				   {
					   continue;
				   }

				   HorarioAula* h = itHor->horario_aula;

				   DateTime fimF = hf->getInicio();
				   fimF.addMinutes( hf->getTempoAula() );

				   DateTime fimH = h->getInicio(); // controle
				   fimH.addMinutes( h->getTempoAula() );

				   // ---- S� insere variaveis com horarios (hi,hf) que possuem interse��o com h

				   if ( ( ( hi->getInicio() <= h->getInicio() ) && ( fimF >  h->getInicio() ) ) ||
					   ( ( h->getInicio() <= hi->getInicio() ) && ( fimH >  hi->getInicio() ) ) )
				   {

					   c.reset();
					   c.setType( ConstraintTatico::C_EVITA_SOBREPOSICAO_TURMA_DISC_ALUNO );
					   c.setCampus( campus );
					   c.setAluno( aluno );
					   c.setDia( v.getDia() );
					   c.setHorarioAula( h );

					   cit = cHashTatico.find(c);
					   if(cit == cHashTatico.end())
					   {
						   sprintf( name, "%s", c.toString().c_str() );
						   OPT_ROW row( 100, OPT_ROW::LESS , 1 , name );

						   row.insert( vit->second, 1.0 );

						   cHashTatico[ c ] = lp->getNumRows();

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
	   }

	   vit++;
   }

   return restricoes;

}

//int SolverMIP::criaRestricaoTaticoEvitaSobreposicaoAulaAluno( int campusId )
//{
//   int restricoes = 0;
//   int nnz;
//   char name[ 100 ];
//
//   VariableTatico v;
//   ConstraintTatico c;
//   VariableTaticoHash::iterator it_v;
//
//   Disciplina * disciplina = NULL;
//
//   ITERA_GGROUP_LESSPTR( itCampus, problemData->campi, Campus )
//   {
//	    if ( itCampus->getId() != campusId )
//	    {
//			continue;
//	    }
//
//		ITERA_GGROUP_LESSPTR( itAluno, problemData->alunos, Aluno )
//		{
//			Aluno *aluno = *itAluno;
//			
//			ITERA_GGROUP_N_PT( itDia, itCampus->diasLetivos, int )
//			{
//				int dia = *itDia;
//
//				ITERA_GGROUP( itHor, itCampus->horarios, Horario )
//				{
//					HorarioAula* h = itHor->horario_aula;
//
//					if ( itHor->dias_semana.find( dia ) ==
//						 itHor->dias_semana.end() )			
//					{
//						continue;
//					}
//
//					// CONSTRAINT --------------------------------------
//
//					c.reset();
//					c.setType( ConstraintTatico::C_EVITA_SOBREPOSICAO_TURMA_DISC_ALUNO );
//
//					c.setCampus( *itCampus );
//					c.setAluno( aluno );
//					c.setDia( dia );
//					c.setHorarioAula( h );
//
//					sprintf( name, "%s", c.toString().c_str() );
//
//					if ( cHashTatico.find( c ) != cHashTatico.end() )
//					{
//						continue;
//					}
//
//					nnz = ( aluno->demandas.size() * itCampus->horarios.size() * 2 );
//
//					OPT_ROW row( nnz + 1, OPT_ROW::LESS , 1 , name );
//
//					// Insere variaveis Credito (x) ------------------------------------------------------
//
//					ITERA_GGROUP_LESSPTR( itUnidade, itCampus->unidades, Unidade )
//					{
//						ITERA_GGROUP_LESSPTR( itCjtSala, itUnidade->conjutoSalas, ConjuntoSala )
//						{				   
//							Sala *sala = itCjtSala->salas.begin()->second;
//						
//							ITERA_GGROUP_LESSPTR( it_disciplina, itCjtSala->disciplinas_associadas, Disciplina )
//							{
//								disciplina = ( *it_disciplina );
//							
//								#pragma region Equivalencias
//								if ( problemData->mapDiscSubstituidaPor.find( disciplina ) !=
//									 problemData->mapDiscSubstituidaPor.end() )
//								{
//									continue;
//								}
//								#pragma endregion
//
//								for ( int turma = 0; turma < disciplina->getNumTurmas(); turma++ )
//								{
//									// Para cada trio <campus, turma, disciplina> no qual o aluno est� alocado
//									Trio< int /*campusId*/, int /*turma*/, Disciplina* > trio;
//									trio.set( campusId, turma, disciplina );
//
//									if ( problemData->mapAluno_CampusTurmaDisc[ aluno ].find( trio ) ==
//										 problemData->mapAluno_CampusTurmaDisc[ aluno ].end() )
//									{
//										continue;
//									}
//
//									GGroup< HorarioAula *, LessPtr< HorarioAula > > horariosEmComum = 
//										problemData->retornaHorariosEmComum( sala->getId(), disciplina->getId(), dia );
//					
//									ITERA_GGROUP_LESSPTR( itHorario, horariosEmComum, HorarioAula )
//									{
//										HorarioAula *hi = *itHorario;
//
//										ITERA_GGROUP_LESSPTR( itHorario, horariosEmComum, HorarioAula )
//										{
//												HorarioAula *hf = *itHorario;
//
//												if ( hf < hi )
//												{
//							 						continue;
//												}
//														   
//												DateTime fimF = hf->getInicio();
//												fimF.addMinutes( hf->getTempoAula() );
//
//												DateTime fimH = h->getInicio(); // controle
//												fimH.addMinutes( h->getTempoAula() );
//
//												// ---- S� insere variaveis com horarios (hi,hf) que possuem interse��o com h
//
//												if ( ( ( hi->getInicio() <= h->getInicio() ) && ( fimF >  h->getInicio() ) ) ||
//													 ( ( h->getInicio() <= hi->getInicio() ) && ( fimH >  hi->getInicio() ) ) )
//												{	
//													// intersecao de horarios, com hi antes de h
//													// intersecao de horarios, com h antes de hi
//																										
//													v.reset();
//													v.setType( VariableTatico::V_CREDITOS );									
//													v.setTurma( turma );
//													v.setDisciplina( disciplina );
//													v.setUnidade( *itUnidade );
//													v.setSubCjtSala( *itCjtSala );
//													v.setDia( dia );                   
//													v.setHorarioAulaInicial( hi );	 // hi
//													v.setHorarioAulaFinal( hf );	 // hf
//
//													it_v = vHashTatico.find( v );
//													if( it_v != vHashTatico.end() )
//													{
//														row.insert( it_v->second, 1.0 );
//													}												
//											
//												}
//										}
//									}
//								}
//							}
//						}
//					}
//
//					// Insere restri��o no Hash ---
//
//					if ( row.getnnz() != 0 )
//					{
//						cHashTatico[ c ] = lp->getNumRows();
//
//						lp->addRow( row );
//						restricoes++;
//					}
//				}  
//			}
//		}
//   }
//
//   return restricoes;
//
//}

int SolverMIP::criaRestricaoTaticoAtivaY( int campusId )
{
	int restricoes = 0;
	int nnz;
	char name[ 100 ];

	//VariableTatico v;
	ConstraintTatico c;
	//VariableTaticoHash::iterator it_v;
	ConstraintTaticoHash::iterator cit;

	//Disciplina * disciplina = NULL;

	Campus *campus = NULL;

	ITERA_GGROUP_LESSPTR( itCampus, problemData->campi, Campus )
	{
		if ( itCampus->getId() != campusId )
			continue;

		campus = *itCampus;
		break;
	}

	if(!campus)
		return 0;

	map< int, map< Disciplina*, set< Aluno *, LessPtr< Aluno > >, LessPtr< Disciplina > > > mapAlunos;

	ITERA_GGROUP_LESSPTR( itAluno, problemData->alunos, Aluno )
	{
		Aluno *aluno = *itAluno;
		GGroup< Trio< int, int, Disciplina* > >::iterator itT = problemData->mapAluno_CampusTurmaDisc[ aluno ].begin();
		for(; itT != problemData->mapAluno_CampusTurmaDisc[ aluno ].end(); itT++)
		{
			Trio< int, int, Disciplina* > trio = *itT;
			if(trio.first != campusId)
				continue;

			mapAlunos[trio.second][trio.third].insert(aluno);
		}
	}


	VariableTaticoHash::iterator vit;

	vit = vHashTatico.begin();
	while(vit != vHashTatico.end())
	{
		if(vit->first.getType() != VariableTatico::V_CREDITOS && vit->first.getType() != VariableTatico::V_ALUNO_UNID_DIA)
		{
			vit++;
			continue;
		}

		VariableTatico v = vit->first;

		if(vit->first.getType() == VariableTatico::V_CREDITOS)
		{
			if(campus->unidades.find(v.getUnidade()) == campus->unidades.end())
			{
				vit++;
				continue;
			}

			set< Aluno *, LessPtr< Aluno > >::iterator itA = mapAlunos[v.getTurma()][v.getDisciplina()].begin();
			for(; itA != mapAlunos[v.getTurma()][v.getDisciplina()].end(); itA++)
			{
				Aluno *aluno = *itA;

				c.reset();
				c.setType( ConstraintTatico::C_ATIVA_Y );
				c.setCampus( campus );
				c.setAluno( aluno );
				c.setDia( v.getDia() );
				c.setUnidade( v.getUnidade() );

				cit = cHashTatico.find(c);
				if(cit == cHashTatico.end())
				{
					sprintf( name, "%s", c.toString().c_str() );
					OPT_ROW row( 100, OPT_ROW::GREATER , 0 , name );

					row.insert( vit->second, -1.0 );

					cHashTatico[ c ] = lp->getNumRows();

					lp->addRow( row );
					restricoes++;
				}
				else
				{
					lp->chgCoef(cit->second, vit->second, -1.0);
				}

			}
		}
		else
		{
			if(campus->unidades.find(v.getUnidade()) == campus->unidades.end())
			{
				vit++;
				continue;
			}

			c.reset();
			c.setType( ConstraintTatico::C_ATIVA_Y );
			c.setCampus( campus );
			c.setAluno( v.getAluno() );
			c.setDia( v.getDia() );
			c.setUnidade( v.getUnidade() );

			cit = cHashTatico.find(c);
			if(cit == cHashTatico.end())
			{
				sprintf( name, "%s", c.toString().c_str() );
				OPT_ROW row( 100, OPT_ROW::GREATER , 0 , name );

				row.insert( vit->second, v.getAluno()->demandas.size() );

				cHashTatico[ c ] = lp->getNumRows();

				lp->addRow( row );
				restricoes++;
			}
			else
			{
				lp->chgCoef(cit->second, vit->second, v.getAluno()->demandas.size());
			}

		}

		vit++;
	}

	return restricoes;
}

//int SolverMIP::criaRestricaoTaticoAtivaY( int campusId )
//{
//   int restricoes = 0;
//   int nnz;
//   char name[ 100 ];
//
//   VariableTatico v;
//   ConstraintTatico c;
//   VariableTaticoHash::iterator it_v;
//
//   Disciplina * disciplina = NULL;
//
//   ITERA_GGROUP_LESSPTR( itCampus, problemData->campi, Campus )
//   {
//	    if ( itCampus->getId() != campusId )
//	    {
//			continue;
//	    }
//
//		ITERA_GGROUP_LESSPTR( itUnidade, itCampus->unidades, Unidade )
//		{
//			ITERA_GGROUP_LESSPTR( itAluno, problemData->alunos, Aluno )
//			{
//				Aluno *aluno = *itAluno;
//			
//				ITERA_GGROUP_N_PT( itDia, itCampus->diasLetivos, int )
//				{
//					int dia = *itDia;
//
//					// CONSTRAINT --------------------------------------
//
//					c.reset();
//					c.setType( ConstraintTatico::C_ATIVA_Y );
//
//					c.setCampus( *itCampus );
//					c.setAluno( aluno );
//					c.setDia( dia );
//					c.setUnidade( *itUnidade );
//
//					sprintf( name, "%s", c.toString().c_str() );
//
//					if ( cHashTatico.find( c ) != cHashTatico.end() )
//					{
//						continue;
//					}
//
//					nnz = ( aluno->demandas.size() * itCampus->horarios.size() * 2 );
//
//					OPT_ROW row( nnz + 1, OPT_ROW::GREATER , 0 , name );
//
//					// Insere variaveis Credito (x) ------------------------------------------------------
//
//					ITERA_GGROUP_LESSPTR( itCjtSala, itUnidade->conjutoSalas, ConjuntoSala )
//					{				   
//						Sala *sala = itCjtSala->salas.begin()->second;
//						
//						ITERA_GGROUP_LESSPTR( it_disciplina, itCjtSala->disciplinas_associadas, Disciplina )
//						{
//							disciplina = ( *it_disciplina );
//							
//							#pragma region Equivalencias
//							if ( problemData->mapDiscSubstituidaPor.find( disciplina ) !=
//									problemData->mapDiscSubstituidaPor.end() )
//							{
//								continue;
//							}
//							#pragma endregion
//
//							for ( int turma = 0; turma < disciplina->getNumTurmas(); turma++ )
//							{
//								// Para cada trio <campus, turma, disciplina> no qual o aluno est� alocado
//								Trio< int /*campusId*/, int /*turma*/, Disciplina* > trio;
//								trio.set( campusId, turma, disciplina );
//
//								if ( problemData->mapAluno_CampusTurmaDisc[ aluno ].find( trio ) ==
//										problemData->mapAluno_CampusTurmaDisc[ aluno ].end() )
//								{
//									continue;
//								}
//
//								GGroup< HorarioAula *, LessPtr< HorarioAula > > horariosEmComum = 
//									problemData->retornaHorariosEmComum( sala->getId(), disciplina->getId(), dia );
//					
//								ITERA_GGROUP_LESSPTR( itHorario, horariosEmComum, HorarioAula )
//								{
//									HorarioAula *hi = *itHorario;
//
//									ITERA_GGROUP_LESSPTR( itHorario, horariosEmComum, HorarioAula )
//									{
//											HorarioAula *hf = *itHorario;
//
//											if ( hf < hi )
//											{
//							 					continue;
//											}														  
//																										
//											v.reset();
//											v.setType( VariableTatico::V_CREDITOS );									
//											v.setTurma( turma );
//											v.setDisciplina( disciplina );
//											v.setUnidade( *itUnidade );
//											v.setSubCjtSala( *itCjtSala );
//											v.setDia( dia );                   
//											v.setHorarioAulaInicial( hi );	 // hi
//											v.setHorarioAulaFinal( hf );	 // hf
//
//											it_v = vHashTatico.find( v );
//											if( it_v != vHashTatico.end() )
//											{
//												row.insert( it_v->second, -1.0 );
//											}
//									}
//								}
//							}
//						}
//					}
//
//					// Insere variavel y_{a,u,t} -------------------------------------------------------								
//					v.reset();
//					v.setType( VariableTatico::V_ALUNO_UNID_DIA );
//
//					v.setUnidade( *itUnidade ); // u
//					v.setAluno( aluno );  // a
//					v.setDia( dia );	  // t
//
//					it_v = vHashTatico.find( v );
//					if( it_v != vHashTatico.end() )
//					{
//						row.insert( it_v->second, aluno->demandas.size() );
//					}
//
//					// Insere restri��o no Hash --------------------------------------------------------
//					if ( row.getnnz() != 0 )
//					{
//						cHashTatico[ c ] = lp->getNumRows();
//
//						lp->addRow( row );
//						restricoes++;
//					}
//				}  
//			}
//		}
//   }
//
//   return restricoes;
//}
   
int SolverMIP::criaRestricaoTaticoAlunoUnidadesDifDia( int campusId )
{
   int restricoes = 0;
   int nnz;
   char name[ 100 ];

   VariableTatico v;
   ConstraintTatico c;
   VariableTaticoHash::iterator it_v;

   Disciplina * disciplina = NULL;

   ITERA_GGROUP_LESSPTR( itCampus, problemData->campi, Campus )
   {
	    if ( itCampus->getId() != campusId )
	    {
			continue;
	    }

		ITERA_GGROUP_LESSPTR( itAluno, problemData->alunos, Aluno )
		{
			Aluno *aluno = *itAluno;
			
			ITERA_GGROUP_N_PT( itDia, itCampus->diasLetivos, int )
			{
				int dia = *itDia;

				// CONSTRAINT --------------------------------------

				c.reset();
				c.setType( ConstraintTatico::C_ALUNO_VARIAS_UNIDADES_DIA );

				c.setCampus( *itCampus );
				c.setAluno( aluno );
				c.setDia( dia );

				sprintf( name, "%s", c.toString().c_str() );

				if ( cHashTatico.find( c ) != cHashTatico.end() )
				{
					continue;
				}

				nnz = ( itCampus->unidades.size() + 1 );

				OPT_ROW row( nnz, OPT_ROW::GREATER , -1.0 , name );

				ITERA_GGROUP_LESSPTR( itUnidade, itCampus->unidades, Unidade )
				{
					// Insere variavel y_{a,u,t} -------------------------------------------------------								
					v.reset();
					v.setType( VariableTatico::V_ALUNO_UNID_DIA );

					v.setUnidade( *itUnidade ); // u
					v.setAluno( aluno );  // a
					v.setDia( dia );	  // t

					it_v = vHashTatico.find( v );
					if( it_v != vHashTatico.end() )
					{
						row.insert( it_v->second, -1.0 );
					}
				}

				// Insere variavel w_{a,t} -------------------------------------------------------								
				v.reset();
				v.setType( VariableTatico::V_ALUNO_VARIAS_UNID_DIA );

				v.setAluno( aluno );  // a
				v.setDia( dia );	  // t

				it_v = vHashTatico.find( v );
				if( it_v != vHashTatico.end() )
				{
					row.insert( it_v->second, itCampus->unidades.size() );
				}

				// Insere restri��o no Hash --------------------------------------------------------
				if ( row.getnnz() != 0 )
				{
					cHashTatico[ c ] = lp->getNumRows();

					lp->addRow( row );
					restricoes++;
				}
			}
		}
   }

   return restricoes;
}   

int SolverMIP::criaRestricaoTaticoMinCreds( int campusId )
{
   int restricoes = 0;

   if ( problemData->parametros->carga_horaria_semanal_aluno == ParametrosPlanejamento::INDIFERENTE )
   {
		return restricoes;
   }

   int nnz;
   char name[ 100 ];

   VariableTatico v;
   ConstraintTatico c;
   VariableTaticoHash::iterator it_v;

   Disciplina * disciplina = NULL;

   ITERA_GGROUP_LESSPTR( itCampus, problemData->campi, Campus )
   {
	    if ( itCampus->getId() != campusId )
	    {
			continue;
	    }

		ITERA_GGROUP_LESSPTR( itAluno, problemData->alunos, Aluno )
		{
			Aluno *aluno = *itAluno;
			
			ITERA_GGROUP_N_PT( itDia, itCampus->diasLetivos, int )
			{
				int dia = *itDia;

				// CONSTRAINT --------------------------------------

				c.reset();
				c.setType( ConstraintTatico::C_MIN_CREDS_ALUNO );

				c.setCampus( *itCampus );
				c.setAluno( aluno );
				c.setDia( dia );

				sprintf( name, "%s", c.toString().c_str() );

				if ( cHashTatico.find( c ) != cHashTatico.end() )
				{
					continue;
				}

				nnz = ( aluno->demandas.size() * itCampus->horarios.size() * 2 );

				OPT_ROW row( nnz + 1, OPT_ROW::LESS , 0 , name );

				// Insere variaveis Credito (x) ------------------------------------------------------

				ITERA_GGROUP_LESSPTR( itUnidade, itCampus->unidades, Unidade )
				{
					ITERA_GGROUP_LESSPTR( itCjtSala, itUnidade->conjutoSalas, ConjuntoSala )
					{				   
						Sala *sala = itCjtSala->salas.begin()->second;
						
						ITERA_GGROUP_LESSPTR( it_disciplina, itCjtSala->disciplinas_associadas, Disciplina )
						{
							disciplina = ( *it_disciplina );
							
							#pragma region Equivalencias
							if ( problemData->mapDiscSubstituidaPor.find( disciplina ) !=
									problemData->mapDiscSubstituidaPor.end() )
							{
								continue;
							}
							#pragma endregion

							for ( int turma = 0; turma < disciplina->getNumTurmas(); turma++ )
							{
								// Para cada trio <campus, turma, disciplina> no qual o aluno est� alocado
								Trio< int /*campusId*/, int /*turma*/, Disciplina* > trio;
								trio.set( campusId, turma, disciplina );

								if ( problemData->mapAluno_CampusTurmaDisc[ aluno ].find( trio ) ==
									 problemData->mapAluno_CampusTurmaDisc[ aluno ].end() )
								{
									continue;
								}

								GGroup< HorarioAula *, LessPtr< HorarioAula > > horariosEmComum = 
									problemData->retornaHorariosEmComum( sala->getId(), disciplina->getId(), dia );
					
								ITERA_GGROUP_LESSPTR( itHorario, horariosEmComum, HorarioAula )
								{
									HorarioAula *hi = *itHorario;

									ITERA_GGROUP_LESSPTR( itHorario, horariosEmComum, HorarioAula )
									{
											HorarioAula *hf = *itHorario;

											if ( hf < hi )
											{
							 					continue;
											}	

											int NCH = disciplina->getCalendario()->retornaNroCreditosEntreHorarios(hi,hf);

											v.reset();
											v.setType( VariableTatico::V_CREDITOS );									
											v.setTurma( turma );
											v.setDisciplina( disciplina );
											v.setUnidade( *itUnidade );
											v.setSubCjtSala( *itCjtSala );
											v.setDia( dia );                   
											v.setHorarioAulaInicial( hi );	 // hi
											v.setHorarioAulaFinal( hf );	 // hf

											it_v = vHashTatico.find( v );
											if( it_v != vHashTatico.end() )
											{
												row.insert( it_v->second, -NCH );
											}
									}
								}
							}
						}
					}
				}

				// Insere variavel h_{a} -------------------------------------------------------								
				v.reset();
				v.setType( VariableTatico::V_MIN_CRED_SEMANA );

				v.setAluno( aluno );  // a
					
				it_v = vHashTatico.find( v );
				if( it_v != vHashTatico.end() )
				{
					row.insert( it_v->second, 1.0 );
				}

				// Insere restri��o no Hash --------------------------------------------------------
				if ( row.getnnz() != 0 )
				{
					cHashTatico[ c ] = lp->getNumRows();

					lp->addRow( row );
					restricoes++;
				}
			}  
		}
   }

   return restricoes;
}

int SolverMIP::criaRestricaoTaticoMaxCreds( int campusId )
{
   int restricoes = 0;

   if ( problemData->parametros->carga_horaria_semanal_aluno == ParametrosPlanejamento::INDIFERENTE )
   {
		return restricoes;
   }

   int nnz;
   char name[ 100 ];

   VariableTatico v;
   ConstraintTatico c;
   VariableTaticoHash::iterator it_v;

   Disciplina * disciplina = NULL;

   ITERA_GGROUP_LESSPTR( itCampus, problemData->campi, Campus )
   {
	    if ( itCampus->getId() != campusId )
	    {
			continue;
	    }

		ITERA_GGROUP_LESSPTR( itAluno, problemData->alunos, Aluno )
		{
			Aluno *aluno = *itAluno;
			
			ITERA_GGROUP_N_PT( itDia, itCampus->diasLetivos, int )
			{
				int dia = *itDia;

				// CONSTRAINT --------------------------------------

				c.reset();
				c.setType( ConstraintTatico::C_MAX_CREDS_ALUNO );

				c.setCampus( *itCampus );
				c.setAluno( aluno );
				c.setDia( dia );

				sprintf( name, "%s", c.toString().c_str() );

				if ( cHashTatico.find( c ) != cHashTatico.end() )
				{
					continue;
				}

				nnz = ( aluno->demandas.size() * itCampus->horarios.size() * 2 );

				OPT_ROW row( nnz + 1, OPT_ROW::GREATER , 0 , name );

				// Insere variaveis Credito (x) ------------------------------------------------------

				ITERA_GGROUP_LESSPTR( itUnidade, itCampus->unidades, Unidade )
				{
					ITERA_GGROUP_LESSPTR( itCjtSala, itUnidade->conjutoSalas, ConjuntoSala )
					{				   
						Sala *sala = itCjtSala->salas.begin()->second;
						
						ITERA_GGROUP_LESSPTR( it_disciplina, itCjtSala->disciplinas_associadas, Disciplina )
						{
							disciplina = ( *it_disciplina );
							
							#pragma region Equivalencias
							if ( problemData->mapDiscSubstituidaPor.find( disciplina ) !=
									problemData->mapDiscSubstituidaPor.end() )
							{
								continue;
							}
							#pragma endregion

							for ( int turma = 0; turma < disciplina->getNumTurmas(); turma++ )
							{
								// Para cada trio <campus, turma, disciplina> no qual o aluno est� alocado
								Trio< int /*campusId*/, int /*turma*/, Disciplina* > trio;
								trio.set( campusId, turma, disciplina );

								if ( problemData->mapAluno_CampusTurmaDisc[ aluno ].find( trio ) ==
									 problemData->mapAluno_CampusTurmaDisc[ aluno ].end() )
								{
									continue;
								}

								GGroup< HorarioAula *, LessPtr< HorarioAula > > horariosEmComum = 
									problemData->retornaHorariosEmComum( sala->getId(), disciplina->getId(), dia );
					
								ITERA_GGROUP_LESSPTR( itHorario, horariosEmComum, HorarioAula )
								{
									HorarioAula *hi = *itHorario;

									ITERA_GGROUP_LESSPTR( itHorario, horariosEmComum, HorarioAula )
									{
											HorarioAula *hf = *itHorario;

											if ( hf < hi )
											{
							 					continue;
											}	

											int NCH = disciplina->getCalendario()->retornaNroCreditosEntreHorarios(hi,hf);

											v.reset();
											v.setType( VariableTatico::V_CREDITOS );									
											v.setTurma( turma );
											v.setDisciplina( disciplina );
											v.setUnidade( *itUnidade );
											v.setSubCjtSala( *itCjtSala );
											v.setDia( dia );                   
											v.setHorarioAulaInicial( hi );	 // hi
											v.setHorarioAulaFinal( hf );	 // hf

											it_v = vHashTatico.find( v );
											if( it_v != vHashTatico.end() )
											{
												row.insert( it_v->second, -NCH );
											}
									}
								}
							}
						}
					}
				}

				// Insere variavel h_{a} -------------------------------------------------------								
				v.reset();
				v.setType( VariableTatico::V_MAX_CRED_SEMANA );

				v.setAluno( aluno );  // a
					
				it_v = vHashTatico.find( v );
				if( it_v != vHashTatico.end() )
				{
					row.insert( it_v->second, 1.0 );
				}

				// Insere restri��o no Hash --------------------------------------------------------
				if ( row.getnnz() != 0 )
				{
					cHashTatico[ c ] = lp->getNumRows();

					lp->addRow( row );
					restricoes++;
				}
			}  
		}
   }

   return restricoes;
}


// Restricao 1.2.17
int SolverMIP::criaRestricaoTaticoAlunoDiscPraticaTeorica( int campusId )
{
   int restricoes = 0;
   int nnz;
   char name[ 100 ];

   VariableTatico v;
   ConstraintTatico c;
   VariableTaticoHash::iterator it_v;
   
   Campus* cp = problemData->refCampus[ campusId ];

   ITERA_GGROUP_LESSPTR( itAluno, problemData->alunos, Aluno )
   {
	    Aluno *aluno = *itAluno;

	    if ( aluno->getOferta()->getCampusId() != campusId )
	    {
	 		continue;
	    }

		ITERA_GGROUP_LESSPTR( itAlDemanda, aluno->demandas, AlunoDemanda )
		{
			Disciplina *discPratica = (*itAlDemanda)->demanda->disciplina;

			if ( discPratica->getId() > 0 )
			{
				continue;
			}

			Disciplina *discTeorica = problemData->refDisciplinas[ - discPratica->getId() ];

			if ( discTeorica == NULL )
			{
				continue;
			}
			
			c.reset();
			c.setType( ConstraintTatico::C_ALUNO_DISC_PRATICA_TEORICA );
			c.setCampus( cp );
			c.setDisciplina( discPratica );
			c.setAluno( aluno );

			sprintf( name, "%s", c.toString().c_str() );

			if ( cHashTatico.find( c ) != cHashTatico.end() )
			{
				continue;
			}
			
			nnz = 2;
			OPT_ROW row( nnz, OPT_ROW::EQUAL , 0.0 , name );

			// Variavel de folga fd_{dp,a}
			v.reset();
			v.setType( VariableTatico::V_SLACK_DEMANDA );
			v.setDisciplina( discPratica );  // dp
			v.setAluno( aluno );			// a

			it_v = vHashTatico.find( v );
			if( it_v != vHashTatico.end() )
			{
				row.insert( it_v->second, 1.0 );
			}
			
			// Variavel de folga fd_{dt,a}
			v.reset();
			v.setType( VariableTatico::V_SLACK_DEMANDA );
			v.setDisciplina( discTeorica );  // dt
			v.setAluno( aluno );			// a

			it_v = vHashTatico.find( v );
			if( it_v != vHashTatico.end() )
			{
				row.insert( it_v->second, -1.0 );
			}
			
			// Insere restri��o no Hash ---
			if ( row.getnnz() != 0 )
			{
				cHashTatico[ c ] = lp->getNumRows();

				lp->addRow( row );
				restricoes++;
			}

        }
   }

	return restricoes;
}



// Restricao 1.2.12
/*====================================================================/
%DocBegin TRIEDA_LOAD_MODEL

%Constraint 
Fixa��o da distribui��o de cr�ditos por dia (*)
%Desc 

%MatExp

\begin{eqnarray}
x_{i,d,u,s,t} + fcp_{i,d,s,t} - fcm_{i,d,s,t} = FC_{d,s,t} \cdot z_{i,d,cp}  \nonumber \qquad 
\forall cp \in CP
\forall s \in S_{u} \quad
\forall d \in D \quad
\forall i \in I_{d} \quad
\forall t \in T
\end{eqnarray}

%DocEnd
/====================================================================*/

// TRIEDA-395
/*
int SolverMIP::criaRestricaoTaticoFixaDistribCredDia( int campusId )
{
   int restricoes = 0;

   char name[ 100 ];
   int nnz;

   ConstraintTatico c;
   VariableTatico v;
   VariableTaticoHash::iterator it_v;

   Disciplina * disciplina = NULL;
   
   ITERA_GGROUP_LESSPTR( itCampus, problemData->campi, Campus )
   {
	   if ( itCampus->getId() != campusId )
	   {
		    continue;
	   }

      ITERA_GGROUP_LESSPTR( itUnidade, itCampus->unidades, Unidade )
      {
         ITERA_GGROUP_LESSPTR( itCjtSala, itUnidade->conjutoSalas, ConjuntoSala )
         {
			 ITERA_GGROUP_LESSPTR( it_disciplina, itCjtSala->disciplinas_associadas, Disciplina )
            {
                disciplina = ( *it_disciplina );

				#pragma region Equivalencias
				if ( problemData->mapDiscSubstituidaPor.find( disciplina ) !=
					 problemData->mapDiscSubstituidaPor.end() )
				{
					continue;
				}
				#pragma endregion

               for ( int turma = 0; turma < disciplina->getNumTurmas(); turma++ )
               {
                  GGroup< int >::iterator itDiasLetDisc = disciplina->diasLetivos.begin();

                  for (; itDiasLetDisc != disciplina->diasLetivos.end(); itDiasLetDisc++ )
                  {
                     ITERA_GGROUP_LESSPTR( it_fix, problemData->fixacoes, Fixacao )
                     {
                        if ( it_fix->getDisciplinaId() == disciplina->getId() &&
                             it_fix->getDiaSemana() == ( *itDiasLetDisc ) &&
						     it_fix->sala != NULL )
                        {
						   Sala *s = (*itCjtSala)->salas.begin()->second;
						   if ( s->getId() != it_fix->sala->getId() )
						   {
							   continue;
						   }

                           c.reset();
                           c.setType( ConstraintTatico::C_FIXA_DISTRIB_CRED_DIA );

                           c.setTurma( turma );
                           c.setDisciplina( disciplina );
                           c.setDia( *itDiasLetDisc );
                           c.setSubCjtSala( *itCjtSala );

                           sprintf( name, "%s", c.toString().c_str() );
                           if ( cHashTatico.find( c ) != cHashTatico.end() )
                           {
                              continue;
                           }

                           if ( disciplina->getNumTurmas() < 0 )
                           {
                              continue;
                           }

						   nnz = problemData->horariosDia.size(); //estimativa

                           OPT_ROW row( nnz, OPT_ROW::EQUAL , 0.0 , name );

							v.reset();
							v.setType( VariableTatico::V_CREDITOS );

							v.setTurma( turma );
							v.setDisciplina( disciplina );
							v.setUnidade( *itUnidade );
							v.setSubCjtSala( *itCjtSala );
							v.setHorarioAulaInicial(  );
							v.setHorarioAulaFinal(  );
							v.setDia( *itDiasLetDisc );

							it_v = vHashTatico.find(v);
							if( it_v != vHashTatico.end() )
							{
								row.insert( it_v->second, 1.0 );
							}

							v.reset();
							v.setType( VariableTatico::V_SLACK_DIST_CRED_DIA_SUPERIOR );
							v.setTurma( turma );
							v.setDisciplina( disciplina );
							v.setDia( *itDiasLetDisc );

							it_v = vHashTatico.find( v );
							if ( it_v != vHashTatico.end() )
							{
								row.insert( it_v->second, 1.0 );
							}

							v.reset();
							v.setType( VariableTatico::V_SLACK_DIST_CRED_DIA_INFERIOR );
							v.setTurma( turma );
							v.setDisciplina( disciplina );
							v.setDia( *itDiasLetDisc );

							it_v = vHashTatico.find( v );
							if ( it_v != vHashTatico.end() )
							{
								row.insert( it_v->second, -1.0 );
							}

							v.reset();
							v.setType( VariableTatico::V_ABERTURA );

							v.setCampus( *itCampus );
							v.setDisciplina( disciplina );
							v.setTurma( turma );

							it_v = vHashTatico.find( v );
							if ( it_v != vHashTatico.end() )
							{
								row.insert( it_v->second,
									-( it_fix->disciplina->getMaxCreds() ) ); // TODO!!!! coeficiente..
							}

							if ( row.getnnz() != 0 )
							{
								cHashTatico[ c ] = lp->getNumRows();

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

   return restricoes;
}

*/



// ==============================================================
//							VARIABLES
// ==============================================================



int SolverMIP::cria_variaveis( int campusId )
{
	int num_vars = 0;
	CPUTimer timer;
	double dif = 0.0;

#ifdef PRINT_cria_variaveis
	int numVarsAnterior = 0;
#endif

	//if(!problemData->parametros->permitir_alunos_em_varios_campi)
	timer.start();
	num_vars += cria_variavel_oferecimentos( campusId ); // vari�vel 'o'
	timer.stop();
	dif = timer.getCronoCurrSecs();
	//else
	//   num_vars += cria_variavel_oferecimentos_permitir_alunos_varios_campi(); // variavel o

#ifdef PRINT_cria_variaveis
	std::cout << "numVars \"o\": " << (num_vars - numVarsAnterior)  <<" "<<dif <<" sec" << std::endl;
	numVarsAnterior = num_vars;
#endif

	//if (!problemData->parametros->permitir_alunos_em_varios_campi)
	//{
	timer.start();
	num_vars += cria_variavel_creditos( campusId ); // x
	timer.stop();
	dif = timer.getCronoCurrSecs();
	//}
	//else
	//{
	//   num_vars += cria_variavel_creditos_permitir_alunos_varios_campi(); // x
	//}

#ifdef PRINT_cria_variaveis
	std::cout << "numVars \"x\": " << (num_vars - numVarsAnterior)  <<" "<<dif <<" sec" << std::endl;
	numVarsAnterior = num_vars;
#endif

	//if(!problemData->parametros->permitir_alunos_em_varios_campi)
	timer.start();
	num_vars += cria_variavel_abertura( campusId ); // z
	timer.stop();
	dif = timer.getCronoCurrSecs();
	//else
	//   num_vars += cria_variavel_abertura_permitir_alunos_varios_campi(); // z

#ifdef PRINT_cria_variaveis
	std::cout << "numVars \"z\": " << (num_vars - numVarsAnterior)  <<" "<<dif <<" sec" << std::endl;
	numVarsAnterior = num_vars;
#endif

	timer.start();
	num_vars += cria_variavel_alunos( campusId ); // a
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_variaveis
	std::cout << "numVars \"a\": " << (num_vars - numVarsAnterior)  <<" "<<dif <<" sec" << std::endl;
	numVarsAnterior = num_vars;
#endif

	timer.start();
	num_vars += cria_variavel_consecutivos( campusId ); // c
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_variaveis
	std::cout << "numVars \"c\": " << (num_vars - numVarsAnterior)  <<" "<<dif <<" sec" << std::endl;
	numVarsAnterior = num_vars;
#endif

	timer.start();
	num_vars += cria_variavel_max_creds( campusId ); // H
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_variaveis
	std::cout << "numVars \"H\": " << (num_vars - numVarsAnterior)  <<" "<<dif <<" sec" << std::endl;
	numVarsAnterior = num_vars;
#endif

	timer.start();
	num_vars += cria_variavel_min_creds( campusId ); // h
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_variaveis
	std::cout << "numVars \"h\": " << (num_vars - numVarsAnterior)  <<" "<<dif <<" sec" << std::endl;
	numVarsAnterior = num_vars;
#endif

	//if(!problemData->parametros->permitir_alunos_em_varios_campi)
	timer.start();
	num_vars += cria_variavel_aloc_disciplina( campusId ); // y
	timer.stop();
	dif = timer.getCronoCurrSecs();
	//else
	//   num_vars += cria_variavel_aloc_disciplina_permitir_alunos_varios_campi(); // y

#ifdef PRINT_cria_variaveis
	std::cout << "numVars \"y\": " << (num_vars - numVarsAnterior)  <<" "<<dif <<" sec" << std::endl;
	numVarsAnterior = num_vars;
#endif

	timer.start();
	num_vars += cria_variavel_num_subblocos( campusId ); // w
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_variaveis
	std::cout << "numVars \"w\": " << (num_vars - numVarsAnterior)  <<" "<<dif <<" sec" << std::endl;
	numVarsAnterior = num_vars;
#endif

	timer.start();
	num_vars += cria_variavel_num_abertura_turma_bloco( campusId ); // v
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_variaveis
	std::cout << "numVars \"v\": " << (num_vars - numVarsAnterior)  <<" "<<dif <<" sec" << std::endl;
	numVarsAnterior = num_vars;
#endif

	//if (!problemData->parametros->permitir_alunos_em_varios_campi)
	timer.start();
	num_vars += cria_variavel_aloc_alunos( campusId ); // b
	timer.stop();
	dif = timer.getCronoCurrSecs();
	//else
	//   num_vars += cria_variavel_aloc_alunos_permitir_alunos_varios_campi(); // b

#ifdef PRINT_cria_variaveis
	std::cout << "numVars \"b\": " << (num_vars - numVarsAnterior)  <<" "<<dif <<" sec" << std::endl;
	numVarsAnterior = num_vars;
#endif

	//if(!problemData->parametros->permitir_alunos_em_varios_campi)
	timer.start();
	num_vars += cria_variavel_de_folga_dist_cred_dia_superior( campusId ); // fcp
	timer.stop();
	dif = timer.getCronoCurrSecs();
	//else
	//   num_vars += cria_variavel_de_folga_dist_cred_dia_superior_permitir_alunos_varios_campi(); // fcp

#ifdef PRINT_cria_variaveis
	std::cout << "numVars \"fcp\": " << (num_vars - numVarsAnterior)  <<" "<<dif <<" sec" << std::endl;
	numVarsAnterior = num_vars;
#endif

	//if(!problemData->parametros->permitir_alunos_em_varios_campi)
	timer.start();
	num_vars += cria_variavel_de_folga_dist_cred_dia_inferior( campusId ); // fcm
	timer.stop();
	dif = timer.getCronoCurrSecs();
	//else
	//   num_vars += cria_variavel_de_folga_dist_cred_dia_inferior_permitir_alunos_varios_campi(); // fcm

#ifdef PRINT_cria_variaveis
	std::cout << "numVars \"fcm\": " << (num_vars - numVarsAnterior)  <<" "<<dif <<" sec" << std::endl;
	numVarsAnterior = num_vars;
#endif

	timer.start();
	num_vars += cria_variavel_abertura_subbloco_de_blc_dia_campus( campusId ); // r
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_variaveis
	std::cout << "numVars \"r\": " << (num_vars - numVarsAnterior)  <<" "<<dif <<" sec" << std::endl;
	numVarsAnterior = num_vars;
#endif

	//if(!problemData->parametros->permitir_alunos_em_varios_campi)
	timer.start();
	num_vars += cria_variavel_de_folga_aloc_alunos_curso_incompat( campusId ); // bs
	timer.stop();
	dif = timer.getCronoCurrSecs();
	//else
	//   num_vars += cria_variavel_de_folga_aloc_alunos_curso_incompat_permitir_alunos_varios_campi(); // bs

#ifdef PRINT_cria_variaveis
	std::cout << "numVars \"bs\": " << (num_vars - numVarsAnterior)  <<" "<<dif <<" sec" << std::endl;
	numVarsAnterior = num_vars;
#endif

	timer.start();
	num_vars += cria_variavel_de_folga_demanda_disciplina( campusId ); // fd
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_variaveis
	std::cout << "numVars \"fd\": " << (num_vars - numVarsAnterior)  <<" "<<dif <<" sec" << std::endl;
	numVarsAnterior = num_vars;
#endif

	timer.start();
	num_vars += cria_variavel_combinacao_divisao_credito( campusId ); // m
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_variaveis
	std::cout << "numVars \"m\": " << (num_vars - numVarsAnterior)  <<" "<<dif <<" sec" << std::endl;
	numVarsAnterior = num_vars;
#endif

	timer.start();
	num_vars += cria_variavel_de_folga_combinacao_divisao_credito( campusId ); // fk
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_variaveis
	std::cout << "numVars \"fk\": " << (num_vars - numVarsAnterior)  <<" "<<dif <<" sec" << std::endl;
	numVarsAnterior = num_vars;
#endif
	/*
	//if(problemData->parametros->permitir_alunos_em_varios_campi)
	num_vars += cria_variavel_creditos_modificada(); // xm
	//else
	//   num_vars += cria_variavel_creditos_modificada_permitir_alunos_varios_campi(); // xm


	#ifdef PRINT_cria_variaveis
	std::cout << "numVars \"xm\": " << (num_vars - numVarsAnterior)  <<" "<<dif <<" sec" << std::endl;
	numVarsAnterior = num_vars;
	#endif
	*/
	timer.start();
	num_vars += cria_variavel_abertura_compativel( campusId ); // zc
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_variaveis
	std::cout << "numVars \"zc\": " << (num_vars - numVarsAnterior)  <<" "<<dif <<" sec" << std::endl;
	numVarsAnterior = num_vars;
#endif

	/* // "n" n�o est� sendo usado.
	//  A restri��o que a usaria tem uma msg de erro de modelagem. Pode ser que tenha sido consertado. 
	timer.start();
	num_vars += cria_variavel_abertura_bloco_mesmoTPS(); // n
	timer.stop();
	dif = timer.getCronoCurrSecs();

	#ifdef PRINT_cria_variaveis
	std::cout << "numVars \"n\": " << (num_vars - numVarsAnterior)  <<" "<<dif <<" sec" << std::endl;
	numVarsAnterior = num_vars;
	#endif

	*/
	/*
	timer.start();
	num_vars += cria_variavel_de_folga_abertura_bloco_mesmoTPS(); // fn
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_variaveis
	std::cout << "numVars \"fn\": " << (num_vars - numVarsAnterior)  <<" "<<dif <<" sec" << std::endl;
	numVarsAnterior = num_vars;
#endif
	*/
	timer.start();
	num_vars += cria_variavel_de_folga_compartilhamento( campusId ); // fc
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_variaveis
	std::cout << "numVars \"fn\": " << (num_vars - numVarsAnterior)  <<" "<<dif <<" sec" << std::endl;
	numVarsAnterior = num_vars;
#endif

	timer.start();
	num_vars += cria_variavel_aloc_alunos_oft( campusId ); // e
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_variaveis
	std::cout << "numVars \"e\": " << (num_vars - numVarsAnterior)  <<" "<<dif <<" sec" << std::endl;
	numVarsAnterior = num_vars;
#endif

	timer.start();
	num_vars += cria_variavel_creditos_oferta( campusId ); // q
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_variaveis
	std::cout << "numVars \"q\": " << (num_vars - numVarsAnterior)  <<" "<<dif <<" sec" << std::endl;
	numVarsAnterior = num_vars;
#endif

	timer.start();
	num_vars += cria_variavel_aloc_alunos_parOft( campusId ); // of
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_variaveis
	std::cout << "numVars \"of\": " << (num_vars - numVarsAnterior)  <<" "<<dif <<" sec" << std::endl;
	numVarsAnterior = num_vars;
#endif

	timer.start();
	num_vars += cria_variavel_creditos_parOferta( campusId ); // p
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_variaveis
	std::cout << "numVars \"p\": " << (num_vars - numVarsAnterior)  <<" "<<dif <<" sec" << std::endl;
	numVarsAnterior = num_vars;
#endif

	timer.start();
	num_vars += cria_variavel_min_hor_disc_oft_dia( campusId ); // g
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_variaveis
	std::cout << "numVars \"g\": " << (num_vars - numVarsAnterior)  <<" "<<dif <<" sec" << std::endl;
	numVarsAnterior = num_vars;
#endif

	timer.start();
	num_vars += cria_variavel_maxCreds_combina_sl_sala( campusId ); // cs
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_variaveis
	std::cout << "numVars \"cs\": " << (num_vars - numVarsAnterior)  <<" "<<dif <<" sec" << std::endl;
	numVarsAnterior = num_vars;
#endif   

	timer.start();
	num_vars += cria_variavel_maxCreds_combina_Sl_bloco( campusId ); // cbc
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_variaveis
	std::cout << "numVars \"cbc\": " << (num_vars - numVarsAnterior)  <<" "<<dif <<" sec" << std::endl;
	numVarsAnterior = num_vars;
#endif   

	return num_vars;
}

/*====================================================================/
%DocBegin TRIEDA_LOAD_MODEL

%Var x_{i,d,u,tps,t}

%Desc 
n�mero de cr�ditos da turma $i$ da disciplina $d$ na unidade $u$ 
em salas do tipo (capacidade) $tps$ no dia $t$. 

%ObjCoef
\theta \cdot \sum\limits_{u \in U}\sum\limits_{tps \in SCAP_{u}} 
\sum\limits_{d \in D}\sum\limits_{t \in T}
\sum\limits_{i \in I_{d}} x_{i,d,u,tps,t}

%DocEnd
/====================================================================*/

int SolverMIP::cria_variavel_creditos( int campusId )
{
   int num_vars = 0;

   Disciplina * disciplina = NULL;

   ITERA_GGROUP_LESSPTR( itCampus, problemData->campi, Campus )
   {
	   if ( itCampus->getId() != campusId )
	   {
		   continue;
	   }

      ITERA_GGROUP_LESSPTR( itUnidade, itCampus->unidades, Unidade )
      {
         ITERA_GGROUP_LESSPTR( itCjtSala, itUnidade->conjutoSalas, ConjuntoSala )
         {
            ITERA_GGROUP_LESSPTR( it_disc, itCjtSala->disciplinas_associadas, Disciplina )
            {
               disciplina = ( *it_disc );

				#pragma region Equivalencias
				if ( problemData->mapDiscSubstituidaPor.find( disciplina ) !=
					problemData->mapDiscSubstituidaPor.end() )
				{
					continue;
				}
				#pragma endregion

               for ( int turma = 0; turma < disciplina->getNumTurmas(); turma++ )
               {
                  // Dada uma disciplina, recupera os seus dias letivos, observando as fixa��es
                  GGroup< int > dias_letivos
                     = itCjtSala->dias_letivos_disciplinas[ ( disciplina ) ];

                  GGroup< int >::iterator itDiscSala_Dias = dias_letivos.begin();

                  for (; itDiscSala_Dias != dias_letivos.end(); itDiscSala_Dias++ )
                  {
                     Variable v;
                     v.reset();
                     v.setType( Variable::V_CREDITOS );

                     v.setTurma( turma );            // i
                     v.setDisciplina( disciplina );  // d
                     v.setUnidade( *itUnidade );     // u
                     v.setSubCjtSala( *itCjtSala );  // tps  
                     v.setDia( *itDiscSala_Dias );   // t

                     if ( problemData->parametros->modo_otimizacao == "OPERACIONAL" )
                     {
                        double custo = 0.0;

                        if ( problemData->usarProfDispDiscTatico )
                        {
                           if ( problemData->disc_Dias_Prof_Tatico[ v.getDisciplina()->getId() ].find( v.getDia() )
                                 == problemData->disc_Dias_Prof_Tatico[ v.getDisciplina()->getId() ].end() )
                           {
                              custo = 100.0;
                           }
                        }

                        if ( vHash.find( v ) == vHash.end() )
                        {
                           vHash[ v ] = lp->getNumCols();

                           if ( problemData->parametros->funcao_objetivo == 0 )
                           {
                              OPT_COL col( OPT_COL::VAR_INTEGRAL, custo, 0.0,
								  itCjtSala->maxCredsDiaPorSL( *itDiscSala_Dias, v.getDisciplina()->getCalendario() ),
                                 ( char * )v.toString().c_str() );

                              lp->newCol( col );
                           }
                           else if ( problemData->parametros->funcao_objetivo == 1
                              || problemData->parametros->funcao_objetivo == 2 )
                           {
                              OPT_COL col( OPT_COL::VAR_INTEGRAL, 0.0, 0.0,
								  itCjtSala->maxCredsDiaPorSL( *itDiscSala_Dias, v.getDisciplina()->getCalendario() ),
                                 ( char * )v.toString().c_str() );

                              lp->newCol( col );
                           }

                           num_vars++;
                        }
                     }
                     else
                     {
                        if ( vHash.find( v ) == vHash.end() )
                        {
						   if ( !criaVariavelTatico( &v ) )
								continue;

                           double custo = 0.0;

                           if ( problemData->usarProfDispDiscTatico )
                           {
                              if ( problemData->disc_Dias_Prof_Tatico[ v.getDisciplina()->getId() ].find( v.getDia() )
                                    == problemData->disc_Dias_Prof_Tatico[ v.getDisciplina()->getId() ].end() )
                              {
                                 custo = 100.0;
                              }
                           }

                           vHash[ v ] = lp->getNumCols();
						   
						   double coef = 0.0;

						   if ( problemData->parametros->funcao_objetivo == 0 )
						   {							
								coef = -1.0;
						   }
						   else if ( problemData->parametros->funcao_objetivo == 1 )
						   {
								coef = 1.0;
						   }

                           OPT_COL col( OPT_COL::VAR_INTEGRAL, coef, 0.0,
                                        itCjtSala->maxCredsDiaPorSL( *itDiscSala_Dias, v.getDisciplina()->getCalendario() ),
                                        ( char * )v.toString().c_str() );
                          
						   lp->newCol( col );						 
						  
                        }

                        num_vars++;
                     }
                  }
               }
            }
         }
      }
   }

   return num_vars;
}



//int SolverMIP::cria_variavel_creditos_permitir_alunos_varios_campi(void)
//{
//   int num_vars = 0;
//
//   Disciplina * disciplina = NULL;
//   Disciplina * disciplina_equivalente = NULL;
//
//   Curso * curso = NULL;
//   Curriculo * curriculo = NULL;
//
//   ITERA_GGROUP_LESSPTR( itCampus, problemData->campi, Campus )
//   {
//      ITERA_GGROUP_LESSPTR( itUnidade, itCampus->unidades, Unidade )
//      {
//         ITERA_GGROUP_LESSPTR( itCjtSala, itUnidade->conjutoSalas, ConjuntoSala )
//         {
//            ITERA_GGROUP_LESSPTR( it_discisciplina, itCjtSala->disciplinas_associadas, Disciplina )
//            {
//               for ( int turma = 0; turma < disciplina->getNumTurmas(); turma++ )
//               {
//                  disciplina = ( *it_discisciplina );
//
//                  std::pair< Curso *, Curriculo * > curso_curriculo
//                     = problemData->map_Disc_CursoCurriculo[ disciplina ];
//                  curso = curso_curriculo.first;
//                  curriculo = curso_curriculo.second;
//
//                  disciplina_equivalente = problemData->retornaDisciplinaSubstituta( curso, curriculo, disciplina );
//                  if ( disciplina_equivalente != NULL )
//                  {
//                     disciplina = disciplina_equivalente;
//                  }
//
//                  int idDisc = disciplina->getId();
//
//                  // Dada uma disciplina, recupera os seus dias letivos, observando as fixa��es
//                  GGroup< int > dias_letivos =
//                     itCjtSala->dias_letivos_disciplinas[ ( disciplina ) ];
//
//                  GGroup< int >::iterator itDiscSala_Dias = dias_letivos.begin();
//                  for (; itDiscSala_Dias != dias_letivos.end(); itDiscSala_Dias++ )
//                  {
//                     std::map< int /*Id Campus*/, GGroup< int > /*Id Discs*/ >::iterator itCPDiscs =
//                        problemData->cp_discs.begin();
//                     for (; itCPDiscs != problemData->cp_discs.end(); itCPDiscs++ )
//                     {
//                        GGroup< int >::iterator it_disc = itCPDiscs->second.begin();
//                        for (; it_disc != itCPDiscs->second.end(); it_disc++ )
//                        {
//                           if ( ( itCampus->getId() == itCPDiscs->first )
//                              && ( disciplina->getId() == ( *it_disc ) ) )
//                           {
//                              Variable v;
//                              v.reset();
//                              v.setType( Variable::V_CREDITOS );
//
//                              v.setTurma( turma );            // i
//                              v.setDisciplina( disciplina );  // d
//                              v.setUnidade( *itUnidade );     // u
//                              v.setSubCjtSala( *itCjtSala );  // tps  
//                              v.setDia( *itDiscSala_Dias );   // t
//
//                              if ( problemData->parametros->modo_otimizacao == "OPERACIONAL" )
//                              {
//                                 ITERA_GGROUP_LESSPTR( it_prof, itCampus->professores, Professor )
//                                 {
//                                    std::pair< int /*idProf*/ , int /*idDisc*/ > prof_Disc 
//                                       ( it_prof->getId(), disciplina->getId() );
//
//                                    if ( problemData->prof_Disc_Dias.find( prof_Disc )
//                                       != problemData->prof_Disc_Dias.end() )
//                                    {
//                                       eta = 0;
//                                    }
//                                    else
//                                    {
//                                       eta = 10;
//                                    }
//
//                                    if ( vHash.find( v ) == vHash.end() )
//                                    {
//                                       vHash[ v ] = lp->getNumCols();
//
//                                       if ( problemData->parametros->funcao_objetivo == 0 )
//                                       {
//                                          OPT_COL col( OPT_COL::VAR_INTEGRAL, eta, 0.0,
//                                             itCjtSala->maxCredsDia( *itDiscSala_Dias ),
//                                             ( char * )v.toString().c_str() );
//
//                                          lp->newCol( col );
//                                       }
//                                       else if ( problemData->parametros->funcao_objetivo == 1 ||
//                                          problemData->parametros->funcao_objetivo == 2 )
//                                       {
//                                          OPT_COL col( OPT_COL::VAR_INTEGRAL,0.0,0.0,
//                                             itCjtSala->maxCredsDia( *itDiscSala_Dias ),
//                                             ( char * )v.toString().c_str() );
//
//                                          lp->newCol( col );
//                                       }
//
//                                       num_vars += 1;
//                                    }
//                                 }
//                              }
//                              else
//                              {
//                                 if ( vHash.find(v) == vHash.end() )
//                                 {
//                                    vHash[v] = lp->getNumCols();
//
//                                    OPT_COL col( OPT_COL::VAR_INTEGRAL,0.0,0.0,
//                                       itCjtSala->maxCredsDia( *itDiscSala_Dias ),
//                                       ( char* )v.toString().c_str() );
//
//                                    lp->newCol( col );
//                                 }
//
//                                 num_vars += 1;
//                              }
//                           }
//                        }
//                     }
//                  }
//               }
//            }
//         }
//      }
//   }
//
//   return num_vars;
//}


/*====================================================================/
%DocBegin TRIEDA_LOAD_MODEL

%Var o_{i,d,u,tps,t} 

%Desc 
indica se a turma $i$ da disciplina $d$ foi alocada na unidade $u$ 
para alguma sala do tipo (capacidade) $tps$ no dia $t$.

%DocEnd
/====================================================================*/


int SolverMIP::cria_variavel_oferecimentos( int campusId )
{
   int num_vars = 0;

   Disciplina * disciplina = NULL;

   ITERA_GGROUP_LESSPTR( itCampus, problemData->campi, Campus )
   {
	   if ( itCampus->getId() != campusId )
	   {
		   continue;
	   }

      ITERA_GGROUP_LESSPTR( itUnidade, itCampus->unidades, Unidade )
      {
         ITERA_GGROUP_LESSPTR( itCjtSala, itUnidade->conjutoSalas, ConjuntoSala )
         {
            ITERA_GGROUP_LESSPTR( it_disciplina, itCjtSala->disciplinas_associadas, Disciplina )
            {
               disciplina = ( *it_disciplina );

				#pragma region Equivalencias
				if ( problemData->mapDiscSubstituidaPor.find( disciplina ) !=
					problemData->mapDiscSubstituidaPor.end() )
				{
					continue;
				}
				#pragma endregion

               for ( int turma = 0; turma < disciplina->getNumTurmas(); turma++ )
               {
                  // Dada uma disciplina, recupera os seus dias letivos, observando as fixa��es
                  GGroup< int > dias_letivos =
                     itCjtSala->dias_letivos_disciplinas[ ( disciplina ) ];

                  GGroup< int >::iterator itDiscSala_Dias = dias_letivos.begin();

                  for (; itDiscSala_Dias != dias_letivos.end(); itDiscSala_Dias++ )
                  {
                     Variable v;
                     v.reset();
                     v.setType( Variable::V_OFERECIMENTO );

                     v.setTurma( turma );            // i
                     v.setDisciplina( disciplina );  // d
                     v.setUnidade( *itUnidade );     // u
                     v.setSubCjtSala( *itCjtSala );  // tps  
                     v.setDia( *itDiscSala_Dias );   // t

                     if ( vHash.find( v ) == vHash.end() )
                     {
						if ( !criaVariavelTatico( &v ) )
							continue;

                        vHash[ v ] = lp->getNumCols();

                        OPT_COL col( OPT_COL::VAR_BINARY, 0.0, 0.0, 1.0,
                           ( char * )v.toString().c_str());

                        lp->newCol( col );
                        num_vars++;
                     }
                  }
               }
            }
         }
      }
   }

   return num_vars;
}

//int SolverMIP::cria_variavel_oferecimentos_permitir_alunos_varios_campi(void)
//{
//   int num_vars = 0;
//
//   Disciplina * disciplina = NULL;
//   Disciplina * disciplina_equivalente = NULL;
//
//   Curso * curso = NULL;
//   Curriculo * curriculo = NULL;
//
//   ITERA_GGROUP_LESSPTR( itCampus, problemData->campi, Campus )
//   {
//      ITERA_GGROUP_LESSPTR( itUnidade, itCampus->unidades, Unidade )
//      {
//         ITERA_GGROUP_LESSPTR( itCjtSala, itUnidade->conjutoSalas, ConjuntoSala )
//         {
//            ITERA_GGROUP_LESSPTR( it_disciplina, itCjtSala->disciplinas_associadas, Disciplina )
//            {
//               disciplina = ( *it_disciplina );
//
//               std::pair< Curso *, Curriculo * > curso_curriculo
//                  = problemData->map_Disc_CursoCurriculo[ disciplina ];
//               curso = curso_curriculo.first;
//               curriculo = curso_curriculo.second;
//
//               disciplina_equivalente = problemData->retornaDisciplinaSubstituta( curso, curriculo, disciplina );
//               if ( disciplina_equivalente != NULL )
//               {
//                  disciplina = disciplina_equivalente;
//               }
//
//               for ( int turma = 0; turma < disciplina->getNumTurmas(); turma++ )
//               {
//                  // Dada uma disciplina, recupera os seus dias letivos, observando as fixa��es
//                  GGroup< int > dias_letivos =
//                     itCjtSala->dias_letivos_disciplinas[ ( disciplina ) ];
//
//                  GGroup< int >::iterator itDiscSala_Dias = dias_letivos.begin();
//
//                  for (; itDiscSala_Dias != dias_letivos.end(); itDiscSala_Dias++ )
//                  {
//                     std::map< int /*Id Campus*/, GGroup< int > /*Id Discs*/ >::iterator
//                        itCPDiscs = problemData->cp_discs.begin();
//                     for (; itCPDiscs != problemData->cp_discs.end(); itCPDiscs++ )
//                     {
//                        GGroup< int >::iterator it_disc
//                           = itCPDiscs->second.begin();
//                        for (; it_disc != itCPDiscs->second.end(); it_disc++ )
//                        {
//                           if ( ( itCampus->getId() == itCPDiscs->first )
//                              && ( disciplina->getId() == ( *it_disc ) ) )
//                           {
//                              Variable v;
//                              v.reset();
//                              v.setType( Variable::V_OFERECIMENTO );
//
//                              v.setTurma( turma );            // i
//                              v.setDisciplina( disciplina );  // d
//                              v.setUnidade( *itUnidade );     // u
//                              v.setSubCjtSala( *itCjtSala );  // tps  
//                              v.setDia( *itDiscSala_Dias );   // t
//
//                              if ( vHash.find(v) == vHash.end() )
//                              {
//                                 vHash[v] = lp->getNumCols();
//
//                                 OPT_COL col( OPT_COL::VAR_BINARY, 0.0, 0.0, 1.0,
//                                    ( char* )v.toString().c_str() );
//
//                                 lp->newCol( col );
//
//                                 num_vars += 1;
//                              }								 
//                           }
//                        }
//                     }
//                  }
//               }
//            }
//         }
//      }
//   }
//
//   return num_vars;
//}

/*====================================================================/
%DocBegin TRIEDA_LOAD_MODEL

%Var z_{i,d,cp} 

%Desc 
indica se houve abertura da $i$-�sima turma da disciplina $d$ no campus $cp$.

%ObjCoef

\alpha \cdot \sum\limits_{d \in D} \sum\limits_{cp \in CP}\sum\limits_{i \in I_{d}}z_{i,d,cp}
+ \gamma \cdot \sum\limits_{d \in D} \sum\limits_{cp \in CP}\sum\limits_{i \in I_{d}}
\left(\frac{Pmax_{d} - \sum\limits_{c \in C}P_{d,c,cp}}{Pmax_{d}} \right) \cdot z_{i,d,cp}

%Data Pmax_{d} 
%Desc
maior demanda da disciplina $d$.

%Data P_{d,c,cp} 
%Desc
demanda da disciplina $d$ no campus $cp$ para o curso $c$.

%Data \alpha
%Desc
peso associado a fun��o objetivo.

%Data \gamma
%Desc
peso associado a fun��o objetivo.

%DocEnd
/====================================================================*/

int SolverMIP::cria_variavel_abertura( int campusId )
{
   int num_vars = 0;

   // Pode ser implementado de uma maneira melhor, listando
   // apenas as disciplinas que podem ser abertas em um campus
   // (atraves do OFERTACURSO) e criando as suas respectivas
   // variaveis. Desse modo, variaveis desnecess�rias (relacionadas
   // � disciplinas que n�o existem em outros campus) seriam evitadas.
   // VER <demandas_campus> em <ProblemData>

   Disciplina * disciplina = NULL;
   
   ITERA_GGROUP_LESSPTR( it_campus, problemData->campi, Campus )
   {
	   if ( it_campus->getId() != campusId )
	   {
		   continue;
	   }

      ITERA_GGROUP_LESSPTR( it_disciplina, problemData->disciplinas, Disciplina )
      {
         disciplina = ( *it_disciplina );

		 // A disciplina deve ser ofertada no campus especificado
		 if ( problemData->cp_discs[campusId].find( disciplina->getId() ) ==
			  problemData->cp_discs[campusId].end() )
		 {
			continue;
		 }

		 #pragma region Equivalencias
		 if ( problemData->mapDiscSubstituidaPor.find( disciplina ) !=
			  problemData->mapDiscSubstituidaPor.end() )
		 {
			continue;
		 }
		 #pragma endregion

         for ( int turma = 0; turma < disciplina->getNumTurmas(); turma++ )
         {
            Variable v;
            v.reset();
            v.setType( Variable::V_ABERTURA );

            v.setTurma( turma );            // i
            v.setDisciplina( disciplina );  // d
            v.setCampus( *it_campus );	    // cp

            std::pair< int, int > dc
               = std::make_pair( disciplina->getId(), it_campus->getId() );

            if ( problemData->demandas_campus.find( dc )
               == problemData->demandas_campus.end() )
            {
               problemData->demandas_campus[ dc ] = 0;
            }
			/*
            double ratioDem = ( disciplina->getDemandaTotal() - 
               problemData->demandas_campus[ dc ] ) 
               / (1.0 * disciplina->getDemandaTotal() );

            double coeff = alpha + gamma * ratioDem;

            int numCreditos = ( disciplina->getCredTeoricos() + disciplina->getCredPraticos() );
            double valorCredito = 1600.0;
            double coef_FO_1_2 = ( numCreditos * valorCredito );
			*/
            if ( vHash.find(v) == vHash.end() )
            {
			   if ( !criaVariavelTatico( &v ) )
					continue;

               lp->getNumCols();
               vHash[v] = lp->getNumCols();
			   /*
               if ( problemData->parametros->funcao_objetivo == 0 )
               {
                  OPT_COL col( OPT_COL::VAR_BINARY, coeff, 0.0, 1.0,
                     ( char * )v.toString().c_str() );

                  lp->newCol( col );
               }
               else if ( problemData->parametros->funcao_objetivo == 1 )
               {
                  OPT_COL col( OPT_COL::VAR_BINARY, -coef_FO_1_2, 0.0, 1.0,
                     ( char * )v.toString().c_str() );

                  lp->newCol( col );
               }
               else if ( problemData->parametros->funcao_objetivo == 2 )
               {
                  OPT_COL col( OPT_COL::VAR_BINARY, coef_FO_1_2, 0.0, 1.0,
                     ( char * )v.toString().c_str() );

                  lp->newCol( col );
               }*/

			   double coef = 0.0;

               if ( problemData->parametros->funcao_objetivo == 0 )
               {
				   coef = -10 * it_campus->getCusto() * disciplina->getTotalCreditos();
               }
               else if ( problemData->parametros->funcao_objetivo == 1 )
               {
				   coef = 10 * it_campus->getCusto() * disciplina->getTotalCreditos();
               }
                  
			   OPT_COL col( OPT_COL::VAR_BINARY, coef, 0.0, 1.0, ( char * )v.toString().c_str() );

               lp->newCol( col );

               num_vars++;
            }
         }
      }
   }

   return num_vars;
}

//int SolverMIP::cria_variavel_abertura_permitir_alunos_varios_campi()
//{
//   int num_vars = 0;
//
//   Disciplina * disciplina = NULL;
//   Disciplina * disciplina_equivalente = NULL;
//
//   Curso * curso = NULL;
//   Curriculo * curriculo = NULL;
//
//   ITERA_GGROUP_LESSPTR( it_campus, problemData->campi, Campus )
//   {
//      ITERA_GGROUP_LESSPTR( it_disciplina, problemData->disciplinas, Disciplina )
//      {
//         disciplina = ( *it_disciplina );
//
//         std::pair< Curso *, Curriculo * > curso_curriculo
//            = problemData->map_Disc_CursoCurriculo[ disciplina ];
//         curso = curso_curriculo.first;
//         curriculo = curso_curriculo.second;
//
//         disciplina_equivalente = problemData->retornaDisciplinaSubstituta( curso, curriculo, disciplina );
//         if ( disciplina_equivalente != NULL )
//         {
//            disciplina = disciplina_equivalente;
//         }
//
//         std::map< int /*Id Campus*/, GGroup< int > /*Id Discs*/ >::iterator itCPDiscs =
//            problemData->cp_discs.begin();
//         for (; itCPDiscs != problemData->cp_discs.end(); itCPDiscs++ )
//         {
//            GGroup< int >::iterator itDisc = itCPDiscs->second.begin();
//            for (; itDisc != itCPDiscs->second.end(); itDisc++ )
//            {
//               if( ( it_campus->getId() == itCPDiscs->first )
//                  && ( disciplina->getId() == ( *itDisc ) ) )
//               {
//                  for ( int turma = 0; turma < disciplina->getNumTurmas(); turma++ )
//                  {
//                     Variable v;
//                     v.reset();
//                     v.setType( Variable::V_ABERTURA );
//
//                     v.setTurma( turma );            // i
//                     v.setDisciplina( disciplina );  // d
//                     v.setCampus( *it_campus );	    // cp
//
//                     std::pair< int, int > dc = std::make_pair
//                        ( disciplina->getId(), it_campus->getId() );
//
//                     if ( problemData->demandas_campus.find( dc )
//                        == problemData->demandas_campus.end() )
//                     {
//                        problemData->demandas_campus[ dc ] = 0;
//                     }
//
//                     double ratioDem = ( disciplina->getDemandaTotal() -
//                        problemData->demandas_campus[ dc ] )
//                        / (1.0 * disciplina->getDemandaTotal() );
//
//                     double coeff = alpha + gamma * ratioDem;
//
//                     int numCreditos = ( disciplina->getCredTeoricos() + disciplina->getCredPraticos() );
//                     double valorCredito = 1600.0;
//                     double coef_FO_1_2 = numCreditos * valorCredito;
//
//                     if ( vHash.find(v) == vHash.end() )
//                     {
//                        lp->getNumCols();
//                        vHash[v] = lp->getNumCols();
//
//                        if ( problemData->parametros->funcao_objetivo == 0 )
//                        {
//                           OPT_COL col( OPT_COL::VAR_BINARY, coeff, 0.0, 1.0,
//                              ( char* )v.toString().c_str() );
//
//                           lp->newCol( col );
//                        }
//                        else if( problemData->parametros->funcao_objetivo == 1 )
//                        {
//                           OPT_COL col( OPT_COL::VAR_BINARY, -coef_FO_1_2, 0.0, 1.0,
//                              ( char* )v.toString().c_str() );
//
//                           lp->newCol( col );
//                        }
//                        else if( problemData->parametros->funcao_objetivo == 2 )
//                        {
//                           OPT_COL col( OPT_COL::VAR_BINARY, coef_FO_1_2, 0.0, 1.0,
//                              ( char* )v.toString().c_str() );
//
//                           lp->newCol( col );
//                        }
//
//                        num_vars += 1;
//                     }
//                  }
//               }
//            }
//         }
//      }
//   }
//
//   return num_vars;
//}

/*====================================================================/
%DocBegin TRIEDA_LOAD_MODEL

%Var a_{i,d,oft}

%Desc 
n�mero de alunos de uma oferta $oft$ alocados para a $i$-�sima turma da disciplina $d$.

%DocEnd
/====================================================================*/

int SolverMIP::cria_variavel_alunos( int campusId )
{
   int total_Vars = 0;
   int num_vars = 0;
   
   ITERA_GGROUP_LESSPTR( itOferta, problemData->ofertas, Oferta )
   {
	   if ( itOferta->getCampusId() != campusId )
	   {
		   continue;
	   }

      map < Disciplina*, int, LessPtr< Disciplina > >::iterator itPrdDisc = 
         itOferta->curriculo->disciplinas_periodo.begin();
      for(; itPrdDisc != itOferta->curriculo->disciplinas_periodo.end();
         itPrdDisc++ )
      {
		  Disciplina * ptDisc = itPrdDisc->first;

		  #pragma region Equivalencias
		  if ( problemData->mapDiscSubstituidaPor.find( ptDisc ) !=
			   problemData->mapDiscSubstituidaPor.end() )
		  {
			  continue;
		  }
		  #pragma endregion

         // Calculando P_{d,o}
         int qtdDem = 0;
         ITERA_GGROUP_LESSPTR( itDem, problemData->demandas, Demanda )
         {
            if ( itDem->disciplina->getId() == ptDisc->getId() &&
               itDem->getOfertaId() == itOferta->getId() )
            {
               qtdDem += itDem->getQuantidade();
            }
         }

         if ( qtdDem <= 0 )
         {
            continue;
         }

         for ( int turma = 0; turma < ptDisc->getNumTurmas(); turma++ )
         {
            Variable v;
            v.reset();
            v.setType( Variable::V_ALUNOS );

            v.setTurma( turma );               // i
            v.setDisciplina( ptDisc );         // d
            v.setOferta( *itOferta );          // oft

            if ( vHash.find(v) == vHash.end() )
            {
			    if ( !criaVariavelTatico( &v ) )
					continue;

                vHash[v] = lp->getNumCols();

				double coef = 0.0;

				if ( problemData->parametros->funcao_objetivo == 0 )
				{
					double valorCredito = itOferta->getReceita();
					int numCreditos = ptDisc->getTotalCreditos();
				
					coef = 10.0 * numCreditos * valorCredito;
				}
				else if ( problemData->parametros->funcao_objetivo == 1 )
				{
					coef = 0.0;
				}

                OPT_COL col( OPT_COL::VAR_INTEGRAL, coef, 0.0, qtdDem,
                     ( char * )v.toString().c_str() );

                lp->newCol( col );

                num_vars += 1;
            }
         }
      }
   }

   return num_vars;
}

/*====================================================================/
%DocBegin TRIEDA_LOAD_MODEL

%Var b_{i,d,c,cp} 

%Desc 
indica se algum aluno do curso $c$ foi alocado para a $i$-�sima turma da disciplina $d$ no campus $cp$.

%DocEnd
/====================================================================*/

int SolverMIP::cria_variavel_aloc_alunos( int campusId )
{
   int num_vars = 0;

   Disciplina * disciplina = NULL;
   
   ITERA_GGROUP_LESSPTR( it_Oferta, problemData->ofertas, Oferta )
   {
      Campus * pt_Campus = it_Oferta->campus;
      Curso * pt_Curso = it_Oferta->curso;

	   if ( pt_Campus->getId() != campusId )
	   {
		   continue;
	   }

      map < Disciplina*, int, LessPtr< Disciplina > >::iterator it_Prd_Disc = 
         it_Oferta->curriculo->disciplinas_periodo.begin();
      for(; it_Prd_Disc != it_Oferta->curriculo->disciplinas_periodo.end();
         it_Prd_Disc++ )
      {
		  disciplina = it_Prd_Disc->first;

		  #pragma region Equivalencias
		  if ( problemData->mapDiscSubstituidaPor.find( disciplina ) !=
			   problemData->mapDiscSubstituidaPor.end() )
		  {
			  continue;
		  }
		  #pragma endregion

         for ( int turma = 0; turma < disciplina->getNumTurmas(); turma++ )
         {
            Variable v;
            v.reset();
            v.setType( Variable::V_ALOC_ALUNO );

            v.setTurma( turma );           // i
            v.setDisciplina( disciplina ); // d
            v.setCurso( pt_Curso );        // c
            v.setCampus( pt_Campus );	    // cp

            if ( vHash.find( v ) == vHash.end() )
            {
			   if ( !criaVariavelTatico( &v ) )
					continue;

               vHash[v] = lp->getNumCols();
				
			   double coef = 0.0;

				if ( problemData->parametros->funcao_objetivo == 0 )
				{
					coef = -1.0;
				}
				else if ( problemData->parametros->funcao_objetivo == 1 )
				{
					coef = 1.0;
				}

               OPT_COL col( OPT_COL::VAR_BINARY, coef, 0.0, 1.0,
                  ( char * )v.toString().c_str() );

               lp->newCol( col );
               num_vars++;
            }
         }
      }
   }

   return num_vars;
}

//int SolverMIP::cria_variavel_aloc_alunos_permitir_alunos_varios_campi( void )
//{
//   int num_vars = 0;
//
//   Disciplina * disciplina = NULL;
//   Disciplina * disciplina_equivalente = NULL;
//
//   Curso * curso = NULL;
//   Curriculo * curriculo = NULL;
//
//   ITERA_GGROUP_LESSPTR( it_Oferta, problemData->ofertas, Oferta )
//   {
//      Campus * pt_Campus = it_Oferta->campus;
//      Curso * pt_Curso = it_Oferta->curso;
//
//      GGroup< std::pair< int, Disciplina * > >::iterator it_Prd_Disc = 
//         it_Oferta->curriculo->disciplinas_periodo.begin();
//
//      for (; it_Prd_Disc != it_Oferta->curriculo->disciplinas_periodo.end();
//         it_Prd_Disc++ )
//      {
//         disciplina = ( *it_Prd_Disc ).second;
//
//         std::pair< Curso *, Curriculo * > curso_curriculo
//            = problemData->map_Disc_CursoCurriculo[ disciplina ];
//
//         curso = curso_curriculo.first;
//         curriculo = curso_curriculo.second;
//
//         disciplina_equivalente = problemData->retornaDisciplinaSubstituta( curso, curriculo, disciplina );
//         if ( disciplina_equivalente != NULL )
//         {
//            disciplina = disciplina_equivalente;
//         }
//
//         for ( int turma = 0; turma < disciplina->getNumTurmas(); turma++ )
//         {
//            // Id do Campus --> Ids das Disciplinas
//            std::map< int , GGroup< int >  >::iterator itCPDiscs =
//               problemData->cp_discs.begin();
//
//            for (; itCPDiscs != problemData->cp_discs.end(); itCPDiscs++ )
//            {
//               GGroup< int >::iterator it_disc
//                  = itCPDiscs->second.begin();
//
//               for (; it_disc != itCPDiscs->second.end(); it_disc++ )
//               {
//                  if( ( pt_Campus->getId() == itCPDiscs->first )
//                     && ( disciplina->getId() == ( *it_disc ) ) )
//                  {
//                     Variable v;
//                     v.reset();
//                     v.setType( Variable::V_ALOC_ALUNO );
//
//                     v.setTurma( turma );           // i
//                     v.setDisciplina( disciplina ); // d
//                     v.setCurso( pt_Curso );        // c
//                     v.setCampus( pt_Campus );	   // cp
//
//                     if ( vHash.find( v ) == vHash.end() )
//                     {
//                        vHash[ v ] = lp->getNumCols();
//
//                        OPT_COL col( OPT_COL::VAR_BINARY, 0, 0.0, 1.0,
//                           ( char * )v.toString().c_str() );
//
//                        lp->newCol( col );
//                        num_vars += 1;
//                     }
//                  }
//               }
//            }
//         }
//      }
//   }
//
//   return num_vars;
//}

/*====================================================================/
%DocBegin TRIEDA_LOAD_MODEL

%Var c_{i,d,t} 

%Desc 
indica se houve abertura de turma $i$ da disciplina $d$ em dias consecutivos.

%ObjCoef
\delta \cdot \sum\limits_{d \in D} 
\sum\limits_{i \in I_{d}} \sum\limits_{t \in T-{1}} c_{i,d,t}

%Data \delta
%Desc
peso associado a fun��o objetivo.

%DocEnd
/====================================================================*/

int SolverMIP::cria_variavel_consecutivos( int campusId )
{
   int num_vars = 0;

   Disciplina * disciplina = NULL;

   ITERA_GGROUP_LESSPTR( it_campus, problemData->campi, Campus )
   {
	  if ( it_campus->getId() != campusId )
	  {
		  continue;
	  }

      ITERA_GGROUP_LESSPTR( it_unidade, it_campus->unidades, Unidade )
      {
         ITERA_GGROUP_LESSPTR( it_conjunto_sala, it_unidade->conjutoSalas, ConjuntoSala )
         {
            ITERA_GGROUP_LESSPTR( it_disciplina, it_conjunto_sala->disciplinas_associadas, Disciplina )
            {
               disciplina = ( *it_disciplina );

				#pragma region Equivalencias
				if ( problemData->mapDiscSubstituidaPor.find( disciplina ) !=
					problemData->mapDiscSubstituidaPor.end() )
				{
					continue;
				}
				#pragma endregion

               for ( int turma = 0; turma < disciplina->getNumTurmas(); turma++ )
               {
                  // Dada uma disciplina, recupera os seus dias letivos, observando as fixa��es
                  GGroup< int > dias_letivos
                     = it_conjunto_sala->dias_letivos_disciplinas[ ( disciplina ) ];

                  GGroup< int >::iterator itDiasLetDisc = dias_letivos.begin();

                  for (; itDiasLetDisc != dias_letivos.end(); itDiasLetDisc++ )
                  {
                     Variable v;
                     v.reset();
                     v.setType( Variable::V_DIAS_CONSECUTIVOS );

                     v.setTurma( turma );            // i
                     v.setDisciplina( disciplina );  // d
                     v.setDia( *itDiasLetDisc );     // t

                     if ( vHash.find( v ) == vHash.end() )
                     {
						if ( !criaVariavelTatico( &v ) )
							continue;

                        vHash[v] = lp->getNumCols();
					    
						double coef = 0.0;

						if ( problemData->parametros->funcao_objetivo == 0 )
						{
							coef = -1.0;
						}
						else if ( problemData->parametros->funcao_objetivo == 1 )
						{
							coef = 1.0;
						}
                           
						OPT_COL col( OPT_COL::VAR_BINARY, coef, 0.0, 1.0, ( char * )v.toString().c_str() );
						                           
						lp->newCol( col );

                        num_vars++;
                     }
                  }
               }
            }
         }
      }
   }

   return num_vars;
}

/*====================================================================/
%DocBegin TRIEDA_LOAD_MODEL

%Var \underline{h}_{bc,i} 

%Desc 
m�nimo de cr�ditos alocados na semana na $i$-�sima turma do bloco $bc$.
%ObjCoef
\lambda \cdot \sum\limits_{bc \in B} 
\sum\limits_{i \in I_{d}, d \in D_{bc}} \left( \overline{h}_{bc,i} - \underline{h}_{bc,i} \right)

%Data \underline{H_{d}} 
%Desc
m�nimo de cr�ditos di�rios da disciplina $d$.

%Data \lambda
%Desc
peso associado a fun��o objetivo.

%DocEnd
/====================================================================*/

int SolverMIP::cria_variavel_min_creds( int campusId )
{
   int num_vars = 0;

   Disciplina * disciplina = NULL;

   ITERA_GGROUP_LESSPTR( it_bloco, problemData->blocos, BlocoCurricular )
   {
	  if ( it_bloco->campus->getId() != campusId )
	  {
		  continue;
	  }

      ITERA_GGROUP_LESSPTR( it_disciplina, it_bloco->disciplinas, Disciplina )
      {
         disciplina = ( *it_disciplina );
		
		 #pragma region Equivalencias
		 if ( problemData->mapDiscSubstituidaPor.find( disciplina ) !=
			  problemData->mapDiscSubstituidaPor.end() )
		 {
			  continue;
		 }
		 #pragma endregion

         for ( int i = 0; i < disciplina->getNumTurmas(); i++ )
         {
            Variable v;
            v.reset();
            v.setType( Variable::V_MIN_CRED_SEMANA );

            v.setBloco( *it_bloco );
            v.setTurma( i );

            if ( vHash.find( v ) == vHash.end() )
            {
			   if ( !criaVariavelTatico( &v ) )
					continue;

               vHash[ v ] = lp->getNumCols();
                  
			   double obj = 0.0;

               if ( problemData->parametros->funcao_objetivo == 0 )
               {
                  if ( problemData->parametros->carga_horaria_semanal_aluno == ParametrosPlanejamento::EQUILIBRAR)
                  {
					  obj = it_bloco->campus->getCusto()/4;
                  }
                  else if ( problemData->parametros->carga_horaria_semanal_aluno == ParametrosPlanejamento::MINIMIZAR_DIAS)
                  {
                     obj = it_bloco->campus->getCusto()/4;
                  }
               }
               else if ( problemData->parametros->funcao_objetivo == 1 )
               {
                  if ( problemData->parametros->carga_horaria_semanal_aluno == ParametrosPlanejamento::EQUILIBRAR)
                  {
                     obj = it_bloco->campus->getCusto()/4;
                  }
                  else if ( problemData->parametros->carga_horaria_semanal_aluno == ParametrosPlanejamento::MINIMIZAR_DIAS)
                  {
                     obj = it_bloco->campus->getCusto()/4;
                  }
               }
               
			   OPT_COL col( OPT_COL::VAR_INTEGRAL, obj, 0.0, 1000.0, ( char * )v.toString().c_str() );

               lp->newCol( col );

               num_vars++;
            }
         }
      }
   }

   return num_vars;
}

/*====================================================================/
%DocBegin TRIEDA_LOAD_MODEL

%Var \overline{h}_{bc,i} 

%Desc 
m�ximo de cr�ditos alocados na semana na $i$-�sima turma do bloco $bc$.

%Data \overline{H_{d}} 
%Desc
m�ximo de cr�ditos di�rios da disciplina $d$.

%DocEnd
/====================================================================*/

int SolverMIP::cria_variavel_max_creds( int campusId )
{
   int num_vars = 0;

   Disciplina * disciplina = NULL;

   ITERA_GGROUP_LESSPTR( it_bloco, problemData->blocos, BlocoCurricular )
   {
	  if ( it_bloco->campus->getId() != campusId )
	  {
		  continue;
	  }

      ITERA_GGROUP_LESSPTR( it_disciplina, it_bloco->disciplinas, Disciplina )
      {
         disciplina = ( *it_disciplina );

		  #pragma region Equivalencias
		  if ( problemData->mapDiscSubstituidaPor.find( disciplina ) !=
			   problemData->mapDiscSubstituidaPor.end() )
		  {
			  continue;
		  }
		  #pragma endregion

         for ( int i = 0; i < disciplina->getNumTurmas(); i++ )
         {
            Variable v;
            v.reset();
            v.setType( Variable::V_MAX_CRED_SEMANA );

            v.setBloco( *it_bloco );
            v.setTurma( i );

            if ( vHash.find( v ) == vHash.end() )
            {
			   if ( !criaVariavelTatico( &v ) )
					continue;

               vHash[v] = lp->getNumCols();

			   double obj = 0.0;

               if ( problemData->parametros->funcao_objetivo == 0 )
               {
                  if ( problemData->parametros->carga_horaria_semanal_aluno == ParametrosPlanejamento::EQUILIBRAR)
                  {
					  obj = -it_bloco->campus->getCusto()/4;
                  }
                  else if ( problemData->parametros->carga_horaria_semanal_aluno == ParametrosPlanejamento::MINIMIZAR_DIAS)
                  {
                      obj = 0.0;
                  }
               }
               else if ( problemData->parametros->funcao_objetivo == 1 )
               {
                  if ( problemData->parametros->carga_horaria_semanal_aluno == ParametrosPlanejamento::EQUILIBRAR)
                  {
                      obj = -it_bloco->campus->getCusto()/4;
                  }
                  else if ( problemData->parametros->carga_horaria_semanal_aluno == ParametrosPlanejamento::MINIMIZAR_DIAS)
                  {
                      obj = 0.0;
                  }
               }
               
			   OPT_COL col( OPT_COL::VAR_INTEGRAL, obj, 0.0, 1000.0, ( char * )v.toString().c_str() );

               lp->newCol( col );

               num_vars++;
            }
         }
      }
   }
   return num_vars;
}

/*====================================================================/
%DocBegin TRIEDA_LOAD_MODEL

%Var y_{i,d,tps,u} 

%Desc 
indica que a turma $i$ da disciplina $d$ foi alocada em alguma sala do tipo $tps$ da unidade $u$.

%DocEnd
/====================================================================*/

int SolverMIP::cria_variavel_aloc_disciplina( int campusId )
{
   // Poderia criar as variaveis apenas qdo a
   // disciplina for compativel com a sala. Por
   // exemplo, n�o criar uma vari�vel qdo a disciplina
   // demanda horarios que a sala n�o disp�e.

   int num_vars = 0;

   Disciplina * disciplina = NULL;

   ITERA_GGROUP_LESSPTR( itCampus, problemData->campi, Campus )
   {
	  if ( itCampus->getId() != campusId )
	  {
		  continue;
	  }

      ITERA_GGROUP_LESSPTR( itUnidade, itCampus->unidades, Unidade )
      {
         ITERA_GGROUP_LESSPTR( itCjtSala, itUnidade->conjutoSalas, ConjuntoSala )
         {
            ITERA_GGROUP_LESSPTR( it_disciplina, itCjtSala->disciplinas_associadas, Disciplina )
            {
                disciplina = ( *it_disciplina );

				#pragma region Equivalencias
				if ( problemData->mapDiscSubstituidaPor.find( disciplina ) !=
					problemData->mapDiscSubstituidaPor.end() )
				{
					continue;
				}
				#pragma endregion

               for ( int turma = 0; turma < disciplina->getNumTurmas(); turma++ )
               {
                  Variable v;
                  v.reset();
                  v.setType( Variable::V_ALOC_DISCIPLINA );

                  v.setTurma( turma );            // i
                  v.setDisciplina( disciplina );  // d
                  v.setSubCjtSala( *itCjtSala );  // tps
                  v.setUnidade( *itUnidade );     // u

                  if ( vHash.find( v ) == vHash.end() )
                  {
					 if ( !criaVariavelTatico( &v ) )
						continue;

                     vHash[v] = lp->getNumCols();

                     OPT_COL col( OPT_COL::VAR_BINARY, 0.0, 0.0, 1.0,
                        ( char * )v.toString().c_str() );

                     lp->newCol( col );
                     num_vars++;
                  }
               }
            }
         }
      }
   }

   return num_vars;
}

//int SolverMIP::cria_variavel_aloc_disciplina_permitir_alunos_varios_campi( void )
//{
//   // Poderia criar as variaveis apenas qdo a disciplina
//   // for compativel com a sala. Por exemplo, n�o criar
//   // uma vari�vel qdo a disciplia demanda horarios que a sala n�o disp�e.
//
//   int num_vars = 0;
//
//   Disciplina * disciplina = NULL;
//   Disciplina * disciplina_equivalente = NULL;
//
//   Curso * curso = NULL;
//   Curriculo * curriculo = NULL;
//
//   ITERA_GGROUP_LESSPTR( itCampus, problemData->campi, Campus )
//   {
//      ITERA_GGROUP_LESSPTR( itUnidade, itCampus->unidades, Unidade )
//      {
//         ITERA_GGROUP_LESSPTR( itCjtSala, itUnidade->conjutoSalas, ConjuntoSala )
//         {
//            ITERA_GGROUP_LESSPTR( it_disciplina, itCjtSala->disciplinas_associadas, Disciplina )
//            {
//               disciplina = ( *it_disciplina );
//
//               std::pair< Curso *, Curriculo * > curso_curriculo
//                  = problemData->map_Disc_CursoCurriculo[ disciplina ];
//               curso = curso_curriculo.first;
//               curriculo = curso_curriculo.second;
//
//               disciplina_equivalente = problemData->retornaDisciplinaSubstituta( curso, curriculo, disciplina );
//               if ( disciplina_equivalente != NULL )
//               {
//                  disciplina = disciplina_equivalente;
//               }
//
//               for ( int turma = 0; turma < disciplina->getNumTurmas(); turma++ )
//               {
//                  std::map< int /*Id Campus*/, GGroup< int > /*Id Discs*/ >::iterator
//                     itCPDiscs = problemData->cp_discs.begin();
//
//                  for (; itCPDiscs != problemData->cp_discs.end(); itCPDiscs++ )
//                  {
//                     GGroup< int >::iterator it_disc = itCPDiscs->second.begin();
//
//                     for (; it_disc != itCPDiscs->second.end(); it_disc++ )
//                     {
//                        if ( ( itCampus->getId() == itCPDiscs->first )
//                           && ( disciplina->getId() == ( *it_disc ) ) )
//                        {
//                           Variable v;
//                           v.reset();
//                           v.setType( Variable::V_ALOC_DISCIPLINA );
//
//                           v.setTurma( turma );            // i
//                           v.setDisciplina( disciplina );  // d
//                           v.setSubCjtSala( *itCjtSala );  // tps
//                           v.setUnidade( *itUnidade );     // u
//
//                           if ( vHash.find( v ) == vHash.end() )
//                           {
//                              vHash[v] = lp->getNumCols();
//
//                              OPT_COL col( OPT_COL::VAR_BINARY, 0.0, 0.0, 1.0,
//                                 ( char * )v.toString().c_str() );
//
//                              lp->newCol( col );
//                              num_vars++;
//                           }
//                        }
//                     }
//                  }
//               }
//            }
//         }
//      }
//   }
//
//   return num_vars;
//}

/*====================================================================/
%DocBegin TRIEDA_LOAD_MODEL

%Var w_{bc,t,cp}  

%Desc 
indica o n�mero sub-blocos abertos do bloco curricular $bc$ no dia $t$ no campus $cp$.
%ObjCoef
\rho \cdot \sum\limits_{bc \in B}\sum\limits_{t \in T} (\sum\limits_{cp \in CP} w_{bc,t,cp})

%Data \rho
%Desc
peso associado a fun��o objetivo.

%DocEnd
/====================================================================*/

int SolverMIP::cria_variavel_num_subblocos( int campusId )
{
   int num_vars = 0;

   ITERA_GGROUP_LESSPTR( it_bloco, problemData->blocos, BlocoCurricular )
   {
      Campus * campus = it_bloco->campus;
	  
	  if ( campus->getId() != campusId )
	  {
		  continue;
	  }

      ITERA_GGROUP_N_PT( it_Dias_Letivos, problemData->bloco_Campus_Dias
         [ std::make_pair( it_bloco->getId(), campus->getId() ) ], int )
      {
         Variable v;
         v.reset();
         v.setType( Variable::V_N_SUBBLOCOS );

         v.setBloco( *it_bloco );
         v.setDia( *it_Dias_Letivos );
         v.setCampus( campus );

         if ( vHash.find( v ) == vHash.end() )
         {
			if ( !criaVariavelTatico( &v ) )
				continue;

            vHash[ v ] = lp->getNumCols();
			
			double coef = 0.0;

            if ( problemData->parametros->funcao_objetivo == 0 )
            {
				coef = -1.0;
            }
            else if ( problemData->parametros->funcao_objetivo == 1 )
            {
				coef = 1.0;
            }
            
			OPT_COL col( OPT_COL::VAR_INTEGRAL, coef, 0.0, 10,
               ( char * )v.toString().c_str() );
			               
			lp->newCol( col );

            num_vars++;
         }
      }
   }

   return num_vars;
}

/*====================================================================/
%DocBegin TRIEDA_LOAD_MODEL

%Var v_{bc,t} 

%Desc 
contabiliza a abertura do mesmo bloco curricular $bc$, no mesmo dia $t$, em campus distintos.
%ObjCoef
\beta \cdot \sum\limits_{bc \in B} \sum\limits_{t \in T} v_{b,t}

%Data \beta
%Desc
peso associado a fun��o objetivo.

%DocEnd
/====================================================================*/

int SolverMIP::cria_variavel_num_abertura_turma_bloco( int campusId )
{
   int num_vars = 0;

   ITERA_GGROUP_LESSPTR( it_bloco, problemData->blocos, BlocoCurricular )
   {
	  if ( it_bloco->campus->getId() != campusId )
	  {
		  continue;
	  }

      GGroup< int >::iterator itDiasLetBloco =
         it_bloco->diasLetivos.begin();

      for (; itDiasLetBloco != it_bloco->diasLetivos.end();
             itDiasLetBloco++ )
      {
         Variable v;
         v.reset();
         v.setType( Variable::V_N_ABERT_TURMA_BLOCO );

         v.setBloco( *it_bloco );
         v.setDia( *itDiasLetBloco );

         if ( vHash.find( v ) == vHash.end() )
         {
			if ( !criaVariavelTatico( &v ) )
				continue;

            vHash[v] = lp->getNumCols();
			
			double coef = 0.0;

            if ( problemData->parametros->funcao_objetivo == 0 )
            {
				coef = -1.0;
            }
            else if ( problemData->parametros->funcao_objetivo == 1 )
            {
				coef = 1.0;
            }
               
			OPT_COL col( OPT_COL::VAR_BINARY, coef, 0.0, 1.0,
                 ( char * )v.toString().c_str() );
			                 
			lp->newCol( col );
			
            num_vars++;
         }
      }
   }

   return num_vars;
}

/*====================================================================/
%DocBegin TRIEDA_LOAD_MODEL

%Var fcp_{d,t}

%Desc 
vari�vel de folga superior para a restri��o de fixa��o da distribui��o de cr�ditos por dia.
%ObjCoef
\xi \cdot \sum\limits_{d \in D} \sum\limits_{t \in T} fcp_{d,t}

%Data \xi
%Desc
peso associado a fun��o objetivo.

%DocEnd
/====================================================================*/

int SolverMIP::cria_variavel_de_folga_dist_cred_dia_superior( int campusId )
{
   int num_vars = 0;

   Disciplina * disciplina = NULL;

   ITERA_GGROUP_LESSPTR( itCampus, problemData->campi, Campus )
   {
	  if ( itCampus->getId() != campusId )
	  {
		  continue;
	  }

      ITERA_GGROUP_LESSPTR( itUnidade, itCampus->unidades, Unidade )
      {
         ITERA_GGROUP_LESSPTR( itCjtSala, itUnidade->conjutoSalas, ConjuntoSala )
         {
            ITERA_GGROUP_LESSPTR( it_disciplina, problemData->disciplinas, Disciplina )
            {
                disciplina = ( *it_disciplina );

				// A disciplina deve ser ofertada no campus especificado
				if ( problemData->cp_discs[campusId].find( disciplina->getId() ) ==
					 problemData->cp_discs[campusId].end() )
				{
					continue;
				}

			   #pragma region Equivalencias
			   if ( problemData->mapDiscSubstituidaPor.find( disciplina ) !=
				    problemData->mapDiscSubstituidaPor.end() )
			   {
				   continue;
			   }
			   #pragma endregion

               for ( int turma = 0; turma < disciplina->getNumTurmas(); turma++ )
               {
                  // Dada uma disciplina, recupera os seus dias letivos, observando as fixa��es
                  GGroup< int > dias_letivos
                     = itCjtSala->dias_letivos_disciplinas[ ( disciplina ) ];

                  GGroup< int >::iterator itDiasLetDisc = dias_letivos.begin();

                  for (; itDiasLetDisc != dias_letivos.end(); itDiasLetDisc++ )
                  {
                     ITERA_GGROUP_LESSPTR( it_fix, problemData->fixacoes, Fixacao )
                     {
                        if ( it_fix->getDisciplinaId() == disciplina->getId() 
                           && it_fix->getDiaSemana() == ( *itDiasLetDisc ) )
                        {
                           Variable v;
                           v.reset();
                           v.setType( Variable::V_SLACK_DIST_CRED_DIA_SUPERIOR );

                           v.setTurma( turma );
                           v.setDisciplina( disciplina );
                           v.setDia( *itDiasLetDisc );
                           v.setSubCjtSala( *itCjtSala );

                           if ( vHash.find( v ) == vHash.end() )
                           {
							    if ( !criaVariavelTatico( &v ) )
									continue;

                                vHash[v] = lp->getNumCols();
                                int cred_disc_dia = it_fix->disciplina->getMaxCreds();

								double coef = 0.0;

								if ( problemData->parametros->funcao_objetivo == 0 )
								{
									coef = -itCampus->getCusto()/2;
								}
								else if ( problemData->parametros->funcao_objetivo == 1 )
								{
									coef = itCampus->getCusto()/2;
								}
               
								OPT_COL col( OPT_COL::VAR_INTEGRAL, coef, 0.0, cred_disc_dia,
										( char * )v.toString().c_str() );
			                 
								lp->newCol( col );

                                num_vars++;
                           }
                        }
                     }
                  }
               }
            }
         }
      }
   }

   return num_vars;
}

//int SolverMIP::cria_variavel_de_folga_dist_cred_dia_superior_permitir_alunos_varios_campi( void )
//{
//   int num_vars = 0;
//
//   Disciplina * disciplina = NULL;
//   Disciplina * disciplina_equivalente = NULL;
//
//   Curso * curso = NULL;
//   Curriculo * curriculo = NULL;
//
//   ITERA_GGROUP_LESSPTR( itCampus, problemData->campi, Campus )
//   {
//      ITERA_GGROUP_LESSPTR( itUnidade, itCampus->unidades, Unidade )
//      {
//         ITERA_GGROUP_LESSPTR( itCjtSala, itUnidade->conjutoSalas, ConjuntoSala )
//         {
//            ITERA_GGROUP_LESSPTR( it_disciplina, problemData->disciplinas, Disciplina )
//            {
//               disciplina = ( *it_disciplina );
//
//               std::pair< Curso *, Curriculo * > curso_curriculo
//                  = problemData->map_Disc_CursoCurriculo[ disciplina ];
//               curso = curso_curriculo.first;
//               curriculo = curso_curriculo.second;
//
//               disciplina_equivalente = problemData->retornaDisciplinaSubstituta( curso, curriculo, disciplina );
//               if ( disciplina_equivalente != NULL )
//               {
//                  disciplina = disciplina_equivalente;
//               }
//
//               for ( int turma = 0; turma < disciplina->getNumTurmas(); turma++ )
//               {
//                  // Dada uma disciplina, recupera os seus dias letivos, observando as fixa��es
//                  GGroup< int > dias_letivos =
//                     itCjtSala->dias_letivos_disciplinas[ ( disciplina ) ];
//
//                  ITERA_GGROUP_N_PT( itDiasLetDisc, dias_letivos, int )
//                  {
//                     ITERA_GGROUP_LESSPTR( it_fix, problemData->fixacoes, Fixacao )
//                     {
//                        if ( it_fix->getDisciplinaId() == disciplina->getId()
//                           && it_fix->getDiaSemana() == ( *itDiasLetDisc ) )
//                        {
//                           std::map< int /*Id Campus*/, GGroup< int > /*Id Discs*/ >::iterator
//                              itCPDiscs = problemData->cp_discs.begin();
//
//                           for (; itCPDiscs != problemData->cp_discs.end(); itCPDiscs++ )
//                           {
//                              GGroup< int >::iterator itDisc = itCPDiscs->second.begin();
//
//                              for (; itDisc != itCPDiscs->second.end(); itDisc++)
//                              {
//                                 if ( ( itCampus->getId() == itCPDiscs->first )
//                                    && ( disciplina->getId() == ( *itDisc ) ) )
//                                 {
//                                    Variable v;
//                                    v.reset();
//                                    v.setType( Variable::V_SLACK_DIST_CRED_DIA_SUPERIOR );
//
//                                    v.setTurma( turma );
//                                    v.setDisciplina( disciplina );
//                                    v.setDia( *itDiasLetDisc );
//                                    v.setSubCjtSala( *itCjtSala );
//
//                                    int idDisc = disciplina->getId();
//
//                                    if ( vHash.find( v ) == vHash.end() )
//                                    {
//                                       vHash[ v ] = lp->getNumCols();
//                                       int cred_disc_dia = ( it_fix->disciplina->getMaxCreds() );
//
//                                       OPT_COL col( OPT_COL::VAR_INTEGRAL,
//                                          epsilon, 0.0, cred_disc_dia,
//                                          ( char* )v.toString().c_str() );
//
//                                       if ( problemData->parametros->funcao_objetivo == 0 )
//                                       {
//                                          OPT_COL col( OPT_COL::VAR_INTEGRAL,
//                                             epsilon, 0.0, cred_disc_dia,
//                                             ( char* )v.toString().c_str() );
//
//                                          lp->newCol( col );
//                                       }
//                                       else if( problemData->parametros->funcao_objetivo == 1
//                                          || problemData->parametros->funcao_objetivo == 2 )
//                                       {
//                                          OPT_COL col( OPT_COL::VAR_INTEGRAL,
//                                             100.0, 0.0, cred_disc_dia,
//                                             ( char* )v.toString().c_str() );
//
//                                          lp->newCol( col );
//                                       }
//
//                                       num_vars += 1;
//                                    }
//                                 }
//                              }
//                           }
//                        }
//                     }
//                  }
//               }
//            }
//         }
//      }
//   }
//
//   return num_vars;
//}

/*====================================================================/
%DocBegin TRIEDA_LOAD_MODEL

%Var fcm_{d,t}  

%Desc 
vari�vel de folga inferior para a restri��o de fixa��o da distribui��o de cr�ditos por dia.

%ObjCoef
\xi \cdot \sum\limits_{d \in D} \sum\limits_{t \in T} fcm_{d,t}

%DocEnd
/====================================================================*/

int SolverMIP::cria_variavel_de_folga_dist_cred_dia_inferior( int campusId )
{
   int num_vars = 0;

   Disciplina * disciplina = NULL;

   ITERA_GGROUP_LESSPTR( itCampus, problemData->campi, Campus )
   {
	   if ( itCampus->getId() != campusId )
	   {
		   continue;
	   }

      ITERA_GGROUP_LESSPTR( itUnidade, itCampus->unidades, Unidade )
      {
         ITERA_GGROUP_LESSPTR( itCjtSala, itUnidade->conjutoSalas, ConjuntoSala )
         {
            ITERA_GGROUP_LESSPTR( it_disciplina, problemData->disciplinas, Disciplina )
            {
               disciplina = ( *it_disciplina );

				// A disciplina deve ser ofertada no campus especificado
				if ( problemData->cp_discs[campusId].find( disciplina->getId() ) ==
					 problemData->cp_discs[campusId].end() )
				{
					continue;
				}

			   #pragma region Equivalencias
			   if ( problemData->mapDiscSubstituidaPor.find( disciplina ) !=
				    problemData->mapDiscSubstituidaPor.end() )
			   {
				  continue;
			   }
			   #pragma endregion

               for ( int turma = 0; turma < disciplina->getNumTurmas(); turma++ )
               {
                  // Dada uma disciplina, recupera os seus dias letivos, observando as fixa��es
                  GGroup< int > dias_letivos =
                     itCjtSala->dias_letivos_disciplinas[ ( disciplina ) ];

                  GGroup< int >::iterator itDiasLetDisc = dias_letivos.begin();

                  for (; itDiasLetDisc != dias_letivos.end(); itDiasLetDisc++ )
                  {
                     ITERA_GGROUP_LESSPTR( it_fix, problemData->fixacoes, Fixacao )
                     {
                        if ( it_fix->getDisciplinaId() == disciplina->getId() 
                           && it_fix->getDiaSemana() == ( *itDiasLetDisc ) )
                        {
                           Variable v;
                           v.reset();
                           v.setType( Variable::V_SLACK_DIST_CRED_DIA_INFERIOR );

                           v.setTurma( turma );
                           v.setDisciplina( disciplina );
                           v.setDia( *itDiasLetDisc );
                           v.setSubCjtSala( *itCjtSala );

                           if ( vHash.find( v ) == vHash.end() )
                           {
							    if ( !criaVariavelTatico( &v ) )
									continue;

                                vHash[v] = lp->getNumCols();
                                int cred_disc_dia = it_fix->disciplina->getMinCreds();

							    double coef = 0.0;

								if ( problemData->parametros->funcao_objetivo == 0 )
								{
									coef = -itCampus->getCusto()/2;
								}
								else if ( problemData->parametros->funcao_objetivo == 1 )
								{
									coef = itCampus->getCusto()/2;
								}               

								OPT_COL col( OPT_COL::VAR_INTEGRAL, coef, 0.0, cred_disc_dia,
											( char* )v.toString().c_str() );

								lp->newCol( col );

                                num_vars += 1;
                           }
                        }
                     }
                  }
               }
            }
         }
      }
   }

   return num_vars;
}

//int SolverMIP::cria_variavel_de_folga_dist_cred_dia_inferior_permitir_alunos_varios_campi(void)
//{
//   int num_vars = 0;
//
//   Disciplina * disciplina = NULL;
//   Disciplina * disciplina_equivalente = NULL;
//
//   Curso * curso = NULL;
//   Curriculo * curriculo = NULL;
//
//   ITERA_GGROUP_LESSPTR( itCampus, problemData->campi, Campus )
//   {
//      ITERA_GGROUP_LESSPTR( itUnidade, itCampus->unidades, Unidade )
//      {
//         ITERA_GGROUP_LESSPTR( itCjtSala, itUnidade->conjutoSalas, ConjuntoSala )
//         {
//            ITERA_GGROUP_LESSPTR( it_disciplina, problemData->disciplinas, Disciplina )
//            {
//               disciplina = ( *it_disciplina );
//
//               std::pair< Curso *, Curriculo * > curso_curriculo
//                  = problemData->map_Disc_CursoCurriculo[ disciplina ];
//               curso = curso_curriculo.first;
//               curriculo = curso_curriculo.second;
//
//               disciplina_equivalente = problemData->retornaDisciplinaSubstituta( curso, curriculo, disciplina );
//               if ( disciplina_equivalente != NULL )
//               {
//                  disciplina = disciplina_equivalente;
//               }
//
//               for ( int turma = 0; turma < disciplina->getNumTurmas(); turma++ )
//               {
//                  // Dada uma disciplina, recupera os seus dias letivos, observando as fixa��es
//                  GGroup< int > dias_letivos =
//                     itCjtSala->dias_letivos_disciplinas[ ( disciplina ) ];
//
//                  GGroup< int >::iterator itDiasLetDisc = dias_letivos.begin();
//
//                  for (; itDiasLetDisc != dias_letivos.end(); itDiasLetDisc++ )
//                  {
//                     ITERA_GGROUP_LESSPTR( it_fix, problemData->fixacoes, Fixacao )
//                     {
//                        if ( it_fix->getDisciplinaId() == disciplina->getId() 
//                           && it_fix->getDiaSemana() == ( *itDiasLetDisc ) ) 
//                        {
//                           std::map< int /*Id Campus*/, GGroup< int > /*Id Discs*/ >::iterator
//                              itCPDiscs = problemData->cp_discs.begin();
//
//                           for (; itCPDiscs != problemData->cp_discs.end(); itCPDiscs++ )
//                           {
//                              ITERA_GGROUP_N_PT( itDisc, itCPDiscs->second, int )
//                              {
//                                 if ( ( itCampus->getId() == itCPDiscs->first )
//                                    && ( disciplina->getId() == ( *itDisc ) ) )
//                                 {
//                                    Variable v;
//                                    v.reset();
//                                    v.setType( Variable::V_SLACK_DIST_CRED_DIA_INFERIOR );
//
//                                    v.setTurma( turma );
//                                    v.setDisciplina( disciplina );
//                                    v.setDia( *itDiasLetDisc );
//                                    v.setSubCjtSala( *itCjtSala );
//
//                                    if ( vHash.find( v ) == vHash.end() )
//                                    {
//                                       vHash[v] = lp->getNumCols();
//                                       int cred_disc_dia = it_fix->disciplina->getMinCreds();
//
//                                       OPT_COL col( OPT_COL::VAR_INTEGRAL,
//                                          epsilon, 0.0, cred_disc_dia,
//                                          ( char * )v.toString().c_str() );
//
//                                       if ( problemData->parametros->funcao_objetivo == 0 )
//                                       {
//                                          OPT_COL col( OPT_COL::VAR_INTEGRAL,
//                                             epsilon,0.0, cred_disc_dia,
//                                             ( char * )v.toString().c_str() );
//
//                                          lp->newCol( col );
//                                       }
//                                       else if( problemData->parametros->funcao_objetivo == 1 ||
//                                          problemData->parametros->funcao_objetivo == 2 )
//                                       {
//                                          OPT_COL col( OPT_COL::VAR_INTEGRAL,
//                                             100.0, 0.0, cred_disc_dia,
//                                             ( char * )v.toString().c_str() );
//
//                                          lp->newCol( col );
//                                       }
//
//                                       num_vars += 1;
//                                    }
//                                 }
//                              }
//                           }
//                        }
//                     }
//                  }
//               }
//            }
//         }
//      }
//   }
//
//   return num_vars;
//}

/*====================================================================/
%DocBegin TRIEDA_LOAD_MODEL

%Var r_{bc,t,cp}  

%Desc 
indica se algum sub-bloco foi aberto do bloco curricular $bc$ no dia $t$ no campus $cp$.

%DocEnd
/====================================================================*/

int SolverMIP::cria_variavel_abertura_subbloco_de_blc_dia_campus( int campusId )
{
   int num_vars = 0;

   ITERA_GGROUP_LESSPTR( it_bloco, problemData->blocos, BlocoCurricular )
   {
	   if ( it_bloco->campus->getId() != campusId )
	   {
		   continue;
	   }

      ITERA_GGROUP_N_PT( itDia, it_bloco->campus->diasLetivos, int )
      {
         Variable v;
         v.reset();
         v.setType( Variable::V_ABERTURA_SUBBLOCO_DE_BLC_DIA_CAMPUS );

         v.setBloco( *it_bloco );
         v.setDia( *itDia );
         v.setCampus( it_bloco->campus );

         if ( vHash.find( v ) == vHash.end() )
         {
			if ( !criaVariavelTatico( &v ) )
				continue;

            vHash[ v ] = lp->getNumCols();

			double coef = 0.0;

			if ( problemData->parametros->funcao_objetivo == 0 )
			{
				coef = -1.0;
			}
			else if ( problemData->parametros->funcao_objetivo == 1 )
			{
				coef = 1.0;
			}  

            OPT_COL col( OPT_COL::VAR_BINARY, coef, 0.0, 1.0,
               ( char * )v.toString().c_str( ));

            lp->newCol( col );
            num_vars++;
         }
      }
   }

   return num_vars;
}



/*====================================================================/
%DocBegin TRIEDA_LOAD_MODEL

%Var bs_{i,d,c,c',cp}

%Desc 
vari�vel de folga para a restri��o em que o compartilhamento de turmas 
de alunos de cursos incompativeis � proibido.

%DocEnd
/====================================================================*/

int SolverMIP::cria_variavel_de_folga_aloc_alunos_curso_incompat( int campusId )
{
   int num_vars = 0;

   ITERA_GGROUP_LESSPTR( itCampus, problemData->campi, Campus )
   {
	   Campus *cp = *itCampus;

	   if ( cp->getId() != campusId )
	   {
		   continue;
	   }

	   ITERA_GGROUP_LESSPTR( it1Cursos, cp->cursos, Curso )
	   {
		   Curso* c1 = *it1Cursos;
			
		   ITERA_GGROUP_LESSPTR( it2Cursos, cp->cursos, Curso )
		   {
			    Curso* c2 = *it2Cursos;
			    
			    if ( problemData->cursosCompativeis(c1, c2) )
				    continue;

				ITERA_GGROUP_LESSPTR( itDisc, problemData->disciplinas, Disciplina )
				{
					 // A disciplina deve ser ofertada no campus especificado
					 if ( problemData->cp_discs[campusId].find( itDisc->getId() ) ==
						  problemData->cp_discs[campusId].end() )
					 {
						continue;
					 }

					 #pragma region Equivalencias
					 if ( problemData->mapDiscSubstituidaPor.find( *itDisc ) !=
						  problemData->mapDiscSubstituidaPor.end() )
					 {
						  continue;
					 }
					 #pragma endregion

					if ( !c1->possuiDisciplina(*itDisc) || !c2->possuiDisciplina(*itDisc) )
						continue;

					for ( int turma = 0; turma < itDisc->getNumTurmas(); turma++ )
					{
					   Variable v;
					   v.reset();
					   v.setType( Variable::V_SLACK_ALOC_ALUNOS_CURSO_INCOMPAT );
					   v.setTurma( turma );						// i
					   v.setDisciplina( *itDisc );   			// d
					   v.setCurso( c1 );					    // c1
					   v.setCursoIncompat( c2 );				// c2
					   v.setCampus( *itCampus );				// cp

					   if ( vHash.find( v ) == vHash.end() )
					   {
						  if ( !criaVariavelTatico( &v ) )
							  continue;

						  vHash[v] = lp->getNumCols();

						  double coef = 0.0;

							if ( problemData->parametros->funcao_objetivo == 0 )
							{
								coef = -itCampus->getCusto();
							}
							else if ( problemData->parametros->funcao_objetivo == 1 )
							{
								coef = itCampus->getCusto();
							}  

						  OPT_COL col( OPT_COL::VAR_BINARY, coef, 0.0, 1.0, ( char * )v.toString().c_str() );

						  lp->newCol( col );
						  num_vars++;
					   }
					}
				}
		    }
		}
   }

   return num_vars;
}



/*====================================================================/
%DocBegin TRIEDA_LOAD_MODEL

%Var fc_{i,d,c,c',cp}

%Desc 
vari�vel de folga para a restri��o em que o compartilhamento de turmas 
de alunos de cursos compativeis � proibido.

%DocEnd
/====================================================================*/

int SolverMIP::cria_variavel_de_folga_compartilhamento( int campusId )
{
   int num_vars = 0;

   if ( problemData->parametros->permite_compartilhamento_turma_sel )
   {
		return num_vars;
   }

   ITERA_GGROUP_LESSPTR( itCampus, problemData->campi, Campus )
   {
	   if ( itCampus->getId() != campusId )
	   {
		   continue;
	   }

	   std::map< std::pair< Curso *, Curso * >, std::vector< int > >::iterator
               it_cursoComp_disc = problemData->cursosComp_disc.begin();

        for (; it_cursoComp_disc != problemData->cursosComp_disc.end(); it_cursoComp_disc++ )
        {
			Curso* c1 = it_cursoComp_disc->first.first;
			Curso* c2 = it_cursoComp_disc->first.second;

			if ( itCampus->cursos.find( c1 ) == itCampus->cursos.end() ||
				 itCampus->cursos.find( c2 ) == itCampus->cursos.end() )
			{
				continue;
			}
         
			std::vector< int >::iterator it_disc = it_cursoComp_disc->second.begin();

            for (; it_disc != it_cursoComp_disc->second.end(); ++it_disc )
            {
				int discId = *it_disc;
				Disciplina * disciplina = problemData->retornaDisciplina(discId);
				
				if (disciplina == NULL) continue;

				#pragma region Equivalencias
				if ( problemData->mapDiscSubstituidaPor.find( disciplina ) !=
					problemData->mapDiscSubstituidaPor.end() )
				{
					continue;
				}
				#pragma endregion

				for ( int turma = 0; turma < disciplina->getNumTurmas(); turma++ )
				{
				   Variable v;
				   v.reset();
				   v.setType( Variable::V_SLACK_COMPARTILHAMENTO );
				   v.setTurma( turma );								// i
				   v.setDisciplina( disciplina );   				// d
				   v.setParCursos( std::make_pair( c1, c2 ) );		// (c, c)
				   v.setCampus( *itCampus );						// cp

				   if ( vHash.find( v ) == vHash.end() )
				   {
					  if ( !criaVariavelTatico( &v ) )
						  continue;

					  vHash[v] = lp->getNumCols();

						double coef = 0.0;

						if ( problemData->parametros->funcao_objetivo == 0 )
						{
							coef = -itCampus->getCusto()/4;
						}
						else if ( problemData->parametros->funcao_objetivo == 1 )
						{
							coef = itCampus->getCusto()/4;
						}  

					  OPT_COL col( OPT_COL::VAR_BINARY, coef, 0.0, 1.0, ( char * )v.toString().c_str() );

					  lp->newCol( col );
					  num_vars++;
				   }
				}
			}
		}
   }

   return num_vars;
}

//int SolverMIP::cria_variavel_de_folga_aloc_alunos_curso_incompat_permitir_alunos_varios_campi()
//{
//   int num_vars = 0;
//
//   Disciplina * disciplina = NULL;
//   Disciplina * disciplina_equivalente = NULL;
//
//   Curso * curso = NULL;
//   Curriculo * curriculo = NULL;
//
//   ITERA_GGROUP_LESSPTR( it_Oferta_Incompat, problemData->ofertas, Oferta )
//   {
//      Curso * pt_Curso_Incompat = it_Oferta_Incompat->curso;
//      ITERA_GGROUP_LESSPTR( it_Oferta, problemData->ofertas, Oferta )
//      {
//         Campus * pt_Campus = it_Oferta->campus;
//         Curso * pt_Curso = it_Oferta->curso;
//
//         GGroup< std::pair< int, Disciplina * > >::iterator it_Prd_Disc = 
//            it_Oferta->curriculo->disciplinas_periodo.begin();
//
//         for (; it_Prd_Disc != it_Oferta->curriculo->disciplinas_periodo.end(); it_Prd_Disc++ )
//         {
//            disciplina = ( *it_Prd_Disc ).second;
//
//            std::pair< Curso *, Curriculo * > curso_curriculo
//               = problemData->map_Disc_CursoCurriculo[ disciplina ];
//            curso = curso_curriculo.first;
//            curriculo = curso_curriculo.second;
//
//            disciplina_equivalente = problemData->retornaDisciplinaSubstituta( curso, curriculo, disciplina );
//            if ( disciplina_equivalente != NULL )
//            {
//               disciplina = disciplina_equivalente;
//            }
//
//            for ( int turma = 0; turma < disciplina->getNumTurmas(); turma++ )
//            {
//               std::map< int /*Id Campus*/, GGroup< int > /*Id Discs*/ >::iterator
//                  itCPDiscs = problemData->cp_discs.begin();
//
//               for (; itCPDiscs != problemData->cp_discs.end(); itCPDiscs++ )
//               {
//                  GGroup< int >::iterator itDisc = itCPDiscs->second.begin();
//                  for (; itDisc != itCPDiscs->second.end(); itDisc++ )
//                  {
//                     if ( ( pt_Campus->getId() == itCPDiscs->first )
//                        && ( disciplina->getId() == ( *itDisc ) ) )
//                     {
//                        Variable v;
//                        v.reset();
//                        v.setType( Variable::V_SLACK_ALOC_ALUNOS_CURSO_INCOMPAT );
//
//                        v.setTurma( turma );                     // i
//                        v.setDisciplina( disciplina );			     // d
//                        v.setCurso( pt_Curso );					 // c
//                        v.setCursoIncompat( pt_Curso_Incompat );
//                        v.setCampus( pt_Campus );				 // cp
//
//                        if ( vHash.find( v ) == vHash.end() )
//                        {
//                           vHash[ v ] = lp->getNumCols();
//
//                           OPT_COL col( OPT_COL::VAR_BINARY,
//                              0, 0.0, 1.0,
//                              ( char * )v.toString().c_str() );
//
//                           lp->newCol( col );
//                           num_vars++;
//                        }
//                     }
//                  }
//               }
//            }
//         }
//      }
//   }
//
//   return num_vars;
//}

/*====================================================================/
%DocBegin TRIEDA_LOAD_MODEL

%Var fd_{d,oft} 

%Desc 
vari�vel de folga para a restri��o "Capacidade alocada tem que 
permitir atender demanda da disciplina".

%ObjCoef
\omega \cdot \sum\limits_{oft \in O} \sum\limits_{d \in D_{oft}} fd_{d,oft}

%DocEnd
/====================================================================*/

int SolverMIP::cria_variavel_de_folga_demanda_disciplina( int campusId )
{
   int num_vars = 0;

   Disciplina * disciplina = NULL;
   
   ITERA_GGROUP_LESSPTR( itOferta, problemData->ofertas, Oferta )
   {
	   if ( itOferta->getCampusId() != campusId )
	   {
		   continue;
	   }

      map < Disciplina*, int, LessPtr< Disciplina > >::iterator itPrdDisc = 
         itOferta->curriculo->disciplinas_periodo.begin();

      for (; itPrdDisc != itOferta->curriculo->disciplinas_periodo.end(); itPrdDisc++ )
      {
         // Calculando P_{d,o}
         int qtdDem = 0;

		 disciplina = itPrdDisc->first;

         ITERA_GGROUP_LESSPTR( itDem, problemData->demandas, Demanda )
         {
            if ( itDem->disciplina->getId() == disciplina->getId() &&
               itDem->getOfertaId() == itOferta->getId() )
            {
               qtdDem += itDem->getQuantidade();
            }
         }

         if ( qtdDem <= 0 )
         {
            continue;
         }

         Variable v;
         v.reset();
         v.setType( Variable::V_SLACK_DEMANDA );

         v.setDisciplina( disciplina ); // d
         v.setOferta( *itOferta );      // o

         if ( vHash.find( v ) == vHash.end() )
         {
			if ( !criaVariavelTatico( &v ) )
				continue;
			
            vHash[ v ] = lp->getNumCols();

            double ub = qtdDem;

			double coef = 0.0;

			if ( problemData->parametros->funcao_objetivo == 0 )
			{
				coef = 0.0;
			}
			else if ( problemData->parametros->funcao_objetivo == 1 )
			{
				coef = 10 * itOferta->campus->getCusto() * disciplina->getTotalCreditos();
			}

			OPT_COL col( OPT_COL::VAR_INTEGRAL, coef, 0.0, ub, ( char * )v.toString().c_str() );

            lp->newCol( col );

            num_vars++;
         }
      }
   }

   return num_vars;
}

/*====================================================================/
%DocBegin TRIEDA_LOAD_MODEL

%Var m_{d,i,k} 

%Desc 
vari�vel bin�ria que indica se a combina��o de divis�o de cr�ditos 
$k$ foi escolhida para a turma $i$ da disciplina $d$.

%DocEnd
/====================================================================*/

int SolverMIP::cria_variavel_combinacao_divisao_credito( int campusId )
{
   int num_vars = 0;

   Disciplina * disciplina = NULL;
   
   ITERA_GGROUP_LESSPTR( it_disciplina, problemData->disciplinas, Disciplina )
   {
      disciplina = ( *it_disciplina );

	  // A disciplina deve ser ofertada no campus especificado
	  if ( problemData->cp_discs[campusId].find( disciplina->getId() ) ==
		   problemData->cp_discs[campusId].end() )
	  {
		  continue;
	  }

	  #pragma region Equivalencias
	  if ( problemData->mapDiscSubstituidaPor.find( disciplina ) !=
		   problemData->mapDiscSubstituidaPor.end() )
	  {
		  continue;
	  }
	  #pragma endregion

      for ( int turma = 0; turma < disciplina->getNumTurmas(); turma++ )
      {
         for ( unsigned k = 0; k < disciplina->combinacao_divisao_creditos.size(); k++ )
         { 
            Variable v;
            v.reset();
            v.setType( Variable::V_COMBINACAO_DIVISAO_CREDITO );

            v.setTurma( turma );           // i
            v.setDisciplina( disciplina ); // d
            v.setK( k );	                // k

            if ( vHash.find( v ) == vHash.end() )
            {
			   if ( !criaVariavelTatico( &v ) )
					continue;

               vHash[ v ] = lp->getNumCols();

               OPT_COL col( OPT_COL::VAR_BINARY,
                            0.0, 0.0, 1.0,
                            ( char * )v.toString().c_str() );

               lp->newCol( col );
               num_vars++;
            }
         }
      }
   }

   return num_vars;
}

/*====================================================================/
%DocBegin TRIEDA_LOAD_MODEL

%Var fkp_{d,i,t} 

%Desc 
vari�vel de folga superior para a restri��o de combina��o de divis�o de cr�ditos.

%ObjCoef
\psi \cdot \sum\limits_{d \in D} 
\sum\limits_{t \in T} \sum\limits_{i \in I_{d}} fkp_{d,i,k}

%Data \psi
%Desc
peso associado a fun��o objetivo.

%Var fkm_{d,i,t} 

%Desc 
vari�vel de folga inferior para a restri��o de combina��o de divis�o de cr�ditos.

%ObjCoef
\psi \cdot \sum\limits_{d \in D} 
\sum\limits_{t \in T} \sum\limits_{i \in I_{d}} fkm_{d,i,k}

%Data \psi
%Desc
peso associado a fun��o objetivo.

%DocEnd
/====================================================================*/

int SolverMIP::cria_variavel_de_folga_combinacao_divisao_credito( int campusId )
{
   int num_vars = 0;

   Disciplina * disciplina = NULL;
   
   ITERA_GGROUP_LESSPTR( it_campus, problemData->campi, Campus )
   {
	   if ( it_campus->getId() != campusId )
	   {
			continue;
	   }

      ITERA_GGROUP_LESSPTR( it_unidade, it_campus->unidades, Unidade )
      {
         ITERA_GGROUP_LESSPTR( it_conjunto_sala, it_unidade->conjutoSalas, ConjuntoSala )
         {
            ITERA_GGROUP_LESSPTR( it_disciplina, it_conjunto_sala->disciplinas_associadas, Disciplina )
            {
               disciplina = ( *it_disciplina );

				#pragma region Equivalencias
				if ( problemData->mapDiscSubstituidaPor.find( disciplina ) !=
					problemData->mapDiscSubstituidaPor.end() )
				{
					continue;
				}
				#pragma endregion

               for ( int turma = 0; turma < disciplina->getNumTurmas(); turma++ )
               {
                  // Dada uma disciplina, recupera os seus dias letivos, observando as fixa��es
                  GGroup< int > dias_letivos =
                     it_conjunto_sala->dias_letivos_disciplinas[ ( disciplina ) ];

                  GGroup< int >::iterator itDiasLetDisc = dias_letivos.begin();

                  for (; itDiasLetDisc != dias_letivos.end(); itDiasLetDisc++ )
                  {
                     Variable v;
                     v.reset();
                     v.setType( Variable::V_SLACK_COMBINACAO_DIVISAO_CREDITO_P );

                     v.setTurma( turma );            // i
                     v.setDisciplina( disciplina );  // d
                     v.setDia( *itDiasLetDisc );	    // t

                     if ( vHash.find( v ) == vHash.end() )
                     {
						if ( !criaVariavelTatico( &v ) )
							continue;

                        vHash[v] = lp->getNumCols();

						double coef = 0.0;

						if ( problemData->parametros->funcao_objetivo == 0 )
						{
							coef = -it_campus->getCusto()/4;
						}
						else if ( problemData->parametros->funcao_objetivo == 1 )
						{
							coef = it_campus->getCusto()/4;
						}
                           
						OPT_COL col( OPT_COL::VAR_INTEGRAL, coef, 0.0, 10000.0,
                              ( char * )v.toString().c_str() );

                        lp->newCol( col );

						/*
                        if ( problemData->parametros->funcao_objetivo == 0 )
                        {
                           OPT_COL col( OPT_COL::VAR_INTEGRAL,
                                        psi, 0.0, 10000.0,
                                        ( char * )v.toString().c_str() );

                           lp->newCol( col );
                        }
                        else if( problemData->parametros->funcao_objetivo == 1
                                    || problemData->parametros->funcao_objetivo == 2 )
                        {
                           OPT_COL col( OPT_COL::VAR_INTEGRAL,
                                        50.0, 0.0, 10000.0,
                                        ( char * )v.toString().c_str() );

                           lp->newCol( col );
                        }*/

                        num_vars++;
                     }

                     v.reset();
                     v.setType( Variable::V_SLACK_COMBINACAO_DIVISAO_CREDITO_M );

                     v.setTurma( turma );            // i
                     v.setDisciplina( disciplina );  // d
                     v.setDia( *itDiasLetDisc );	    // t

                     if ( vHash.find( v ) == vHash.end() )
                     {
						if ( !criaVariavelTatico( &v ) )
							continue;

                        vHash[v] = lp->getNumCols();

						double coef = 0.0;

						if ( problemData->parametros->funcao_objetivo == 0 )
						{
							coef = -it_campus->getCusto()/4;
						}
						else if ( problemData->parametros->funcao_objetivo == 1 )
						{
							coef = it_campus->getCusto()/4;
						}
                           
						OPT_COL col( OPT_COL::VAR_INTEGRAL, coef, 0.0, 10000.0,
                              ( char * )v.toString().c_str() );
						                           
						lp->newCol( col );

						/*
                        if ( problemData->parametros->funcao_objetivo == 0 )
                        {
                           OPT_COL col( OPT_COL::VAR_INTEGRAL,
                                        psi, 0.0, 10000.0,
                                        ( char * )v.toString().c_str() );

                           lp->newCol( col );
                        }
                        else if( problemData->parametros->funcao_objetivo == 1
                                    || problemData->parametros->funcao_objetivo == 2 )
                        {
                           OPT_COL col( OPT_COL::VAR_INTEGRAL,
                                        50.0, 0.0, 10000.0,
                                        ( char * )v.toString().c_str() );

                           lp->newCol( col );
                        }*/

                        num_vars++;
                     }
                  }
               }
            }
         }
      }
   }

   return num_vars;
}

/*====================================================================/
%DocBegin TRIEDA_LOAD_MODEL

%Var xm_{bc,d,t}

%Desc
m�ximo de cr�ditos alocados para qualquer turma da disciplina $d$ no bloco $bc$ no dia $t$.

%DocEnd
/====================================================================*/

int SolverMIP::cria_variavel_creditos_modificada( int campusId )
{
   int num_vars = 0;

   Disciplina * disciplina = NULL;

   ITERA_GGROUP_LESSPTR( it_bloco, problemData->blocos, BlocoCurricular )
   {
	   if ( it_bloco->campus->getId() != campusId )
	   {
			continue;
	   }

      ITERA_GGROUP_LESSPTR( it_disciplina, it_bloco->disciplinas, Disciplina )
      {
         disciplina = ( *it_disciplina );

		#pragma region Equivalencias
		if ( problemData->mapDiscSubstituidaPor.find( disciplina ) !=
			problemData->mapDiscSubstituidaPor.end() )
		{
			continue;
		}
		#pragma endregion

         GGroup< int >::iterator itDiasLetBloco =
            it_bloco->diasLetivos.begin();

         for (; itDiasLetBloco != it_bloco->diasLetivos.end();
                itDiasLetBloco++ )
         {
            Variable v;
            v.reset();
            v.setType( Variable::V_CREDITOS_MODF );

            v.setDisciplina(disciplina);
            v.setBloco( *it_bloco );
            v.setDia( *itDiasLetBloco );

            if ( vHash.find( v ) == vHash.end() )
            {
			   if ( !criaVariavelTatico( &v ) )
					continue;

               vHash[v] = lp->getNumCols();

               OPT_COL col( OPT_COL::VAR_INTEGRAL,
                            0.0, 0.0, 50,
                            ( char * )v.toString().c_str() );

               lp->newCol( col );
               num_vars++;
            }
         }
      }
   }

   return num_vars;
}

//int SolverMIP::cria_variavel_creditos_modificada_permitir_alunos_varios_campi(void)
//{
//   int num_vars = 0;
//
//   Disciplina * disciplina = NULL;
//   Disciplina * disciplina_equivalente = NULL;
//
//   Curso * curso = NULL;
//   Curriculo * curriculo = NULL;
//
//   ITERA_GGROUP_LESSPTR( itCampus, problemData->campi, Campus )
//   {
//      ITERA_GGROUP_LESSPTR( itUnidade, itCampus->unidades, Unidade )
//      {
//         ITERA_GGROUP_LESSPTR( itCjtSala, itUnidade->conjutoSalas, ConjuntoSala )
//         {
//            ITERA_GGROUP_LESSPTR( it_disciplina, itCjtSala->disciplinas_associadas, Disciplina )
//            {
//               disciplina = ( *it_disciplina );
//
//               std::pair< Curso *, Curriculo * > curso_curriculo
//                  = problemData->map_Disc_CursoCurriculo[ disciplina ];
//               curso = curso_curriculo.first;
//               curriculo = curso_curriculo.second;
//
//               disciplina_equivalente = problemData->retornaDisciplinaSubstituta( curso, curriculo, disciplina );
//               if ( disciplina_equivalente != NULL )
//               {
//                  disciplina = disciplina_equivalente;
//               }
//
//               for ( int turma = 0; turma < disciplina->getNumTurmas(); turma++ )
//               {
//                  // Dada uma disciplina, recupera os seus dias letivos, observando as fixa��es
//                  GGroup< int > dias_letivos =
//                     itCjtSala->dias_letivos_disciplinas[ ( disciplina ) ];
//
//                  ITERA_GGROUP_N_PT( itDiscSala_Dias, dias_letivos, int )
//                  {
//                     std::map< int /*Id Campus*/, GGroup< int > /*Id Discs*/ >::iterator
//                        itCPDiscs = problemData->cp_discs.begin();
//
//                     for (; itCPDiscs != problemData->cp_discs.end(); itCPDiscs++ )
//                     {
//                        GGroup< int >::iterator it_disc = itCPDiscs->second.begin();
//                        for (; it_disc != itCPDiscs->second.end(); it_disc++ )
//                        {
//                           if( ( itCampus->getId() == itCPDiscs->first )
//                              && ( disciplina->getId() == ( *it_disc ) ) )
//                           {
//                              Variable v;
//                              v.reset();
//                              v.setType( Variable::V_CREDITOS_MODF );
//
//                              v.setDisciplina( disciplina );     // d
//                              v.setDia( *itDiscSala_Dias );   // t
//
//                              if ( vHash.find(v) == vHash.end() )
//                              {
//                                 vHash[v] = lp->getNumCols();
//
//                                 OPT_COL col( OPT_COL::VAR_INTEGRAL, 0.0, 0.0, 50 /*FIX-ME*/,
//                                    ( char * )v.toString().c_str() );
//
//                                 // OPT_COL col(OPT_COL::VAR_INTEGRAL,0.0,0.0,itCjtSala->maxCredsDia(*itDiscSala_Dias),
//                                 // (char*)v.toString().c_str());
//
//                                 lp->newCol( col );
//                                 num_vars += 1;
//                              }
//                           }
//                        }
//                     }
//                  }
//               }
//            }
//         }
//      }
//   }
//
//   return num_vars;
//}

/*====================================================================/
%DocBegin TRIEDA_LOAD_MODEL

%Var zc_{d,t} 

%Desc 
indica se houve abertura da disciplina $d$ no dia $t$.

%DocEnd
/====================================================================*/

int SolverMIP::cria_variavel_abertura_compativel( int campusId )
{
   int num_vars = 0;

   Disciplina * disciplina = NULL;

   ITERA_GGROUP_LESSPTR( it_campus, problemData->campi, Campus )
   {
	   if ( it_campus->getId() != campusId )
	   {
			continue;
	   }

      ITERA_GGROUP_LESSPTR( it_unidade, it_campus->unidades, Unidade )
      {
         ITERA_GGROUP_LESSPTR( it_conjunto_sala, it_unidade->conjutoSalas, ConjuntoSala )
         {
            ITERA_GGROUP_LESSPTR( it_disciplina, it_conjunto_sala->disciplinas_associadas, Disciplina )
            {
                disciplina = ( *it_disciplina );

				#pragma region Equivalencias
				if ( problemData->mapDiscSubstituidaPor.find( disciplina ) !=
					problemData->mapDiscSubstituidaPor.end() )
				{
					continue;
				}
				#pragma endregion

               // Dada uma disciplina, recupera os seus dias letivos, observando as fixa��es
               GGroup< int > dias_letivos =
                  it_conjunto_sala->dias_letivos_disciplinas[ ( disciplina ) ];

               ITERA_GGROUP_N_PT( itDiasLetDisc, dias_letivos, int )
               {
                  Variable v;
                  v.reset();
                  v.setType( Variable::V_ABERTURA_COMPATIVEL );

                  v.setDisciplina( disciplina );  // d
                  v.setDia( *itDiasLetDisc );     // t
				  v.setCampus( *it_campus );	  // cp

                  if ( vHash.find( v ) == vHash.end() )
                  {
					 if ( !criaVariavelTatico( &v ) )
						continue;

                     vHash[ v ] = lp->getNumCols();

					 double coef = 0.0;

					 if ( problemData->parametros->funcao_objetivo == 0 )
					 {
					 	coef = -1.0;
					 }
					 else if ( problemData->parametros->funcao_objetivo == 1 )
					 {
				 		coef = 1.0;
			 		 }

                     OPT_COL col( OPT_COL::VAR_BINARY, coef, 0.0, 1.0,
                        ( char * )v.toString().c_str() );

                     lp->newCol( col );
                     num_vars++;
                  }
               }
            }
         }
      }
   }

   return num_vars;
}

/*====================================================================/
%DocBegin TRIEDA_LOAD_MODEL

%Var n_{bc,tps} 

%Desc 
vari�vel bin�ria que indica se o bloco $bc$ foi alocado na sala $tps$. 

%DocEnd
/====================================================================*/

int SolverMIP::cria_variavel_abertura_bloco_mesmoTPS( int campusId )
{
   int num_vars = 0;

   ITERA_GGROUP_LESSPTR( itBloco, problemData->blocos, BlocoCurricular )
   {
	     if ( itBloco->campus->getId() != campusId )
	     {
			  continue;
	     }

         ITERA_GGROUP_LESSPTR( itUnidade, itBloco->campus->unidades, Unidade )
         {
            ITERA_GGROUP_LESSPTR( itCjtSala, itUnidade->conjutoSalas, ConjuntoSala )
            {
               Variable v;
               v.reset();
               v.setType( Variable::V_ABERTURA_BLOCO_MESMO_TPS );

               v.setSubCjtSala( *itCjtSala );
               v.setBloco( *itBloco );

               if ( vHash.find( v ) == vHash.end() )
               {
				  if ( !criaVariavelTatico( &v ) )
					 continue;

                  vHash[v] = lp->getNumCols();

                  OPT_COL col( OPT_COL::VAR_BINARY,
                               0.0, 0.0, 1.0,
                               ( char * )v.toString().c_str() );

                  lp->newCol( col );
                  num_vars += 1;
               }
            }
        }
   }

   return num_vars;
}

/*====================================================================/
%DocBegin TRIEDA_LOAD_MODEL

%Var fn_{bc,tps} 

%Desc 
vari�vel de folga para a restri��o "Evitar aloca��o do mesmo 
bloco curricular em tipos de salas diferentes".

%ObjCoef
\tau \cdot \sum\limits_{bc \in B} \sum\limits_{tps \in SCAP_{u}} fn_{bc,tps}

%Data \tau
%Desc
peso associado a fun��o objetivo.

%DocEnd
/====================================================================*/

int SolverMIP::cria_variavel_de_folga_abertura_bloco_mesmoTPS( int campusId )
{
   int num_vars = 0;

   ITERA_GGROUP_LESSPTR( itBloco, problemData->blocos, BlocoCurricular )
   {
	     if ( itBloco->campus->getId() != campusId )
	     {
			 continue;
	     }

         ITERA_GGROUP_LESSPTR( itUnidade, itBloco->campus->unidades, Unidade )
         {
            ITERA_GGROUP_LESSPTR( itCjtSala, itUnidade->conjutoSalas, ConjuntoSala )
            {
               Variable v;
               v.reset();
               v.setType( Variable::V_SLACK_ABERTURA_BLOCO_MESMO_TPS );

               v.setSubCjtSala( *itCjtSala );
               v.setBloco( *itBloco );

               if ( vHash.find( v ) == vHash.end() )
               {
				  if ( !criaVariavelTatico( &v ) )
					  continue;

                  vHash[v] = lp->getNumCols();

                  if ( problemData->parametros->funcao_objetivo == 0 )
                  {
                     OPT_COL col( OPT_COL::VAR_BINARY,
                                  tau, 0.0, 1.0,
                                  ( char * )v.toString().c_str() );

                     lp->newCol( col );
                  }
                  else if ( problemData->parametros->funcao_objetivo == 1
                     || problemData->parametros->funcao_objetivo == 2 )
                  {
                     OPT_COL col( OPT_COL::VAR_BINARY,
                                  0.0, 0.0, 1.0,
                                  ( char * )v.toString().c_str() );

                     lp->newCol( col );
                  }

                  num_vars++;
               }
            }
        }
   }

   return num_vars;
}


/*====================================================================/
%DocBegin TRIEDA_LOAD_MODEL

%Var e_{i,d,oft} 

%Desc 
vari�vel binaria, indica se houve alunos da oferta oft alocados
para a turma i da disciplina d.

%DocEnd
/====================================================================*/
int SolverMIP::cria_variavel_aloc_alunos_oft( int campusId )
{
   int num_vars = 0;
   
   ITERA_GGROUP_LESSPTR( itOft, problemData->ofertas, Oferta )
   {
	    Oferta *oft = *itOft;
	    
		if ( oft->getCampusId() != campusId )
	    {
			continue;
	    }

	    map < Disciplina*, int, LessPtr< Disciplina > >::iterator it_disc = oft->curriculo->disciplinas_periodo.begin();
	    for (; it_disc != oft->curriculo->disciplinas_periodo.end(); it_disc++ )
	    {
			Disciplina * d = it_disc->first;
			
		    for ( int turma = 0; turma < d->getNumTurmas(); turma++ )
		    {
               Variable v;
               v.reset();
               v.setType( Variable::V_ALOC_ALUNOS_OFT );
               v.setTurma(turma);
               v.setDisciplina(d);
			   v.setOferta(oft);

               if ( vHash.find( v ) == vHash.end() )
               {
				  if ( !criaVariavelTatico( &v ) )
					  continue;

                  vHash[v] = lp->getNumCols();

				  double coef = 0.0;

				  if ( problemData->parametros->funcao_objetivo == 0 )
			      {
					  coef = -1.0;
				  }
				  else if ( problemData->parametros->funcao_objetivo == 1 )
				  {
					  coef = 1.0;
				  }

                  OPT_COL col( OPT_COL::VAR_BINARY, coef, 0.0, 1.0, (char*)v.toString().c_str() );

                  lp->newCol( col );
                  num_vars++;
			   }
			}
		}
   }

   return num_vars;
}




/*====================================================================/
%DocBegin TRIEDA_LOAD_MODEL

%Var q_{i,d,oft,u,tps,t}

%Desc 
n�mero de cr�ditos da turma $i$ da disciplina $d$ para a oferta $oft$
na unidade $u$ em salas do tipo (capacidade) $tps$ no dia $t$.

%DocEnd
/====================================================================*/

int SolverMIP::cria_variavel_creditos_oferta( int campusId )
{
   int num_vars = 0;

   Disciplina * disciplina = NULL;
   
   ITERA_GGROUP_LESSPTR( itOft, problemData->ofertas, Oferta )
   {
	   Oferta *oft = *itOft;

	   Campus * cp = itOft->campus;

	   if ( cp->getId() != campusId )
	   {
			continue;
	   }

      ITERA_GGROUP_LESSPTR( itUnidade, cp->unidades, Unidade )
      {
         ITERA_GGROUP_LESSPTR( itCjtSala, itUnidade->conjutoSalas, ConjuntoSala )
         {
            ITERA_GGROUP_LESSPTR( it_disc, itCjtSala->disciplinas_associadas, Disciplina )
            {
               disciplina = ( *it_disc );
			   
				#pragma region Equivalencias
				if ( problemData->mapDiscSubstituidaPor.find( disciplina ) !=
					problemData->mapDiscSubstituidaPor.end() )
				{
					continue;
				}
				#pragma endregion

			   if ( ! oft->curriculo->possuiDisciplina( disciplina ) )
				   continue;


               for ( int turma = 0; turma < disciplina->getNumTurmas(); turma++ )
               {
                  // Dada uma disciplina, recupera os seus dias letivos, observando as fixa��es
                  GGroup< int > dias_letivos
                     = itCjtSala->dias_letivos_disciplinas[ ( disciplina ) ];

                  GGroup< int >::iterator itDiscSala_Dias = dias_letivos.begin();

                  for (; itDiscSala_Dias != dias_letivos.end(); itDiscSala_Dias++ )
                  {
						 Variable v;
						 v.reset();
						 v.setType( Variable::V_CREDITOS_OFT );

						 v.setTurma( turma );            // i
						 v.setDisciplina( disciplina );  // d
						 v.setUnidade( *itUnidade );     // u
						 v.setSubCjtSala( *itCjtSala );  // tps  
						 v.setDia( *itDiscSala_Dias );   // t
						 v.setOferta( oft );			 // oft

						if ( vHash.find( v ) == vHash.end() )
						{
						    if ( !criaVariavelTatico( &v ) )
								continue;

							vHash[ v ] = lp->getNumCols();

							OPT_COL col( OPT_COL::VAR_INTEGRAL, 0.0, 0.0,
										itCjtSala->maxCredsDiaPorSL( *itDiscSala_Dias, v.getDisciplina()->getCalendario() ),
										( char * )v.toString().c_str() );

							lp->newCol( col );
						}

						num_vars++;
                     
                  }
               }
            }
         }
      }
   }

   return num_vars;
}



/*====================================================================/
%DocBegin TRIEDA_LOAD_MODEL

%Var of_{i,d,oft1,oft2} 

%Desc 
vari�vel binaria, indica se houve alunos das ofertas oft1 e oft2 alocados
para a turma i da disciplina d.

%DocEnd
/====================================================================*/
int SolverMIP::cria_variavel_aloc_alunos_parOft( int campusId )
{
    int num_vars = 0;

	// para cada par de ofertas compativeis
	std::map< std::pair< Oferta *, Oferta * >, std::vector< int > >::iterator
            it_oftsComp_disc = problemData->oftsComp_disc.begin();

    for (; it_oftsComp_disc != problemData->oftsComp_disc.end(); it_oftsComp_disc++ )
    {
		Oferta *oft1 = it_oftsComp_disc->first.first;
		Oferta *oft2 = it_oftsComp_disc->first.second;

		Curso *c1 = oft1->curso;
		Curso *c2 = oft2->curso;
		
		if ( oft1->getCampusId() != campusId || 
			 oft2->getCampusId() != campusId )
	    {
			continue;
	    }

		if ( c1->getId() != c2->getId() && !problemData->parametros->permite_compartilhamento_turma_sel )
			continue;

		// para cada disciplina em comum (possivel de ser compartilhada) ao par de ofertas
        std::vector< int >::iterator it_discComum = it_oftsComp_disc->second.begin();
        for (; it_discComum != it_oftsComp_disc->second.end(); ++it_discComum )
        {
			Disciplina * discComum = problemData->retornaDisciplina( *it_discComum );

			if (discComum == NULL)
				continue;

			#pragma region Equivalencias
			if ( problemData->mapDiscSubstituidaPor.find( discComum ) !=
				problemData->mapDiscSubstituidaPor.end() )
			{
				continue;
			}
			#pragma endregion

			for ( int turma = 0; turma < discComum->getNumTurmas(); turma++ )
			{
				Variable v;
				v.reset();
				v.setType( Variable::V_ALOC_ALUNOS_PAR_OFT );
				v.setTurma(turma);
				v.setDisciplina(discComum);
				v.setParOfertas(oft1, oft2);

				if ( vHash.find( v ) == vHash.end() )
				{
				    if ( !criaVariavelTatico( &v ) )
						continue;

					vHash[v] = lp->getNumCols();

					double coef = 0.0;

					if ( problemData->parametros->funcao_objetivo == 0 )
					{
					 	coef = -1.0;
					}
					else if ( problemData->parametros->funcao_objetivo == 1 )
					{
				 		coef = 1.0;
			 		}

					OPT_COL col( OPT_COL::VAR_BINARY, coef, 0.0, 1.0, (char*)v.toString().c_str() );

					lp->newCol( col );
					num_vars++;
				}
			}
		}
	}
   
    return num_vars;
}


/*====================================================================/
%DocBegin TRIEDA_LOAD_MODEL

%Var p_{i,d,oft1,oft2,u,s,t} 

%Desc 
indica quantos creditos alocados para a turma i
da disciplina d atendendo as ofertas oft1 e oft2.

%DocEnd
/====================================================================*/
int SolverMIP::cria_variavel_creditos_parOferta( int campusId )
{
	ofstream outTestFile;
	char outputTestFilename[] = "outTestVariableP.txt";
	outTestFile.open(outputTestFilename, ios::out);
	if (!outTestFile) {
		cerr << "Can't open output file " << outputTestFilename << endl;
		exit(1);
	}

   int num_vars = 0;

   // para cada campus
   ITERA_GGROUP_LESSPTR( itCampus, problemData->campi, Campus )
   {
	   Campus *cp = *itCampus;

	   if ( cp->getId() != campusId )
	   {
			continue;
	   }

	   // para cada par de ofertas compativeis
	   std::map< std::pair< Oferta *, Oferta * >, std::vector< int > >::iterator
               it_oftsComp_disc = problemData->oftsComp_disc.begin();

	   outTestFile << "\nNro de par ofertas: " << problemData->oftsComp_disc.size();

        for (; it_oftsComp_disc != problemData->oftsComp_disc.end(); it_oftsComp_disc++ )
        {
			Oferta *oft1 = it_oftsComp_disc->first.first;
			Oferta *oft2 = it_oftsComp_disc->first.second;

			if ( oft1->campus != cp || oft1->campus != cp )
				continue;

			Curso *c1 = oft1->curso;
			Curso *c2 = oft2->curso;

			if ( c1->getId() != c2->getId() && !problemData->parametros->permite_compartilhamento_turma_sel )
				continue;

			outTestFile << "\nOfts: " << it_oftsComp_disc->first.first->getId() <<", " << it_oftsComp_disc->first.second->getId();
			outTestFile << "\nNro de disciplinas comuns: " << it_oftsComp_disc->second.size();

			// para cada disciplina em comum (possivel de ser compartilhada) ao par de ofertas
            std::vector< int >::iterator it_discComum = it_oftsComp_disc->second.begin();
            for (; it_discComum != it_oftsComp_disc->second.end(); ++it_discComum )
            {
				Disciplina * discComum = problemData->retornaDisciplina( *it_discComum );
				  
				if (discComum == NULL)
					continue;

				#pragma region Equivalencias
				if ( problemData->mapDiscSubstituidaPor.find( discComum ) !=
					problemData->mapDiscSubstituidaPor.end() )
				{
					continue;
				}
				#pragma endregion

				outTestFile << "\nCapacidade media de sala: " << discComum->getCapacMediaSala() << endl;

				// para cada unidade do campus de discComum
				ITERA_GGROUP_LESSPTR( itUnid, cp->unidades, Unidade )
				{
					// para cada sala aonde poder� ser ministrada a disc compartilhada
					ITERA_GGROUP_LESSPTR( itCjtSalaCompart, itUnid->conjutoSalas, ConjuntoSala )
					{   
						if ( itCjtSalaCompart->disciplinas_associadas.find( discComum ) ==
								itCjtSalaCompart->disciplinas_associadas.end() )
							continue;

						Sala* salaCompart = itCjtSalaCompart->salas.begin()->second;
										
						GGroup< int > diasLetivos = itCjtSalaCompart->dias_letivos_disciplinas[ discComum ];
									
						// Para cada dia em que discComum pode ser ministrada na salaCompart
						GGroup< int >::iterator itDia = diasLetivos.begin();
						for (; itDia != diasLetivos.end(); itDia++ )
						{
							for ( int turma = 0; turma < discComum->getNumTurmas(); turma++ )
							{
								Variable v;
								v.setType( Variable::V_CREDITOS_PAR_OFT );
								v.setTurma( turma );
								v.setDisciplina( discComum );
								v.setParOfertas( oft1, oft2 );
								v.setUnidade( *itUnid );
								v.setSubCjtSala( *itCjtSalaCompart );
								v.setDia( *itDia );

								if ( vHash.find( v ) == vHash.end() )
								{
									if ( !criaVariavelTatico( &v ) )
										continue;

									outTestFile << "P" << num_vars
										  		<< "  i=" << v.getTurma()
												<< " d=" << v.getDisciplina()->getId()
												<< " ofts=" << v.getParOfertas().first->getId() << " " << v.getParOfertas().second->getId()
												<< " u=" << v.getUnidade()
												<< " s" << v.getSubCjtSala()
												<< " t=" << v.getDia() << endl;


									vHash[v] = lp->getNumCols();

									OPT_COL col( OPT_COL::VAR_INTEGRAL, 0.0, 0.0,
											   	itCjtSalaCompart->maxCredsDiaPorSL( *itDia, v.getDisciplina()->getCalendario() ),
												(char*)v.toString().c_str() );

									lp->newCol( col );
									num_vars++;
										  
								}
							}
						}
					}
				}
			}
		}
   }
   outTestFile.close();

   return num_vars;
}


/*====================================================================/
%DocBegin TRIEDA_LOAD_MODEL

%Var g_{d,oft,t} 

%Desc 
Indica o numero minimo de creditos que a disciplina d da oferta oft
ocupa no dia t. Para isso, trata conta sobreposi��o ou n�o de salas
para turmas da disciplina

%DocEnd
/====================================================================*/
int SolverMIP::cria_variavel_min_hor_disc_oft_dia( int campusId )
{
   int num_vars = 0;
   
   ITERA_GGROUP_LESSPTR( itOft, problemData->ofertas, Oferta )
   {
	    Oferta *oft = *itOft;

		if ( oft->getCampusId() != campusId )
	    {
			continue;
	    }

	    map < Disciplina*, int, LessPtr< Disciplina > >::iterator it_disc = oft->curriculo->disciplinas_periodo.begin();
	    for (; it_disc != oft->curriculo->disciplinas_periodo.end(); it_disc++ )
	    {
			Disciplina * d = it_disc->first;
	
			GGroup< int >::iterator itDia = d->diasLetivos.begin();
			for (; itDia != d->diasLetivos.end(); itDia++ )
			{
               Variable v;
               v.reset();
               v.setType( Variable::V_MIN_HOR_DISC_OFT_DIA );
               v.setDisciplina(d);
			   v.setOferta(oft);
			   v.setDia(*itDia);

               if ( vHash.find( v ) == vHash.end() )
               {
				  if ( !criaVariavelTatico( &v ) )
					 continue;

                  vHash[v] = lp->getNumCols();
				  
				  int maxCredDia = oft->curriculo->getMaxCreds(*itDia);

				  double coef = 0.0;

				  if ( problemData->parametros->funcao_objetivo == 0 )
				  {
						coef = -1.0;
				  }
				  else if ( problemData->parametros->funcao_objetivo == 1 )
				  {
						coef = 1.0;
			 	  }

                  OPT_COL col( OPT_COL::VAR_INTEGRAL, coef, 0.0, maxCredDia, (char*)v.toString().c_str() );

                  lp->newCol( col );
                  num_vars++;
			   }
			}
		}
   }

   return num_vars;
}

// cs_{s,t,k}
int SolverMIP::cria_variavel_maxCreds_combina_sl_sala( int campusId )
{
   int num_vars = 0;

   // Metodo somente utilizado quando h� 2 semanas letivas
   if ( problemData->calendarios.size() != 2 )
   {
	   return num_vars;
   }

   ITERA_GGROUP_LESSPTR( itCampus, problemData->campi, Campus )
   {
	   if ( itCampus->getId() != campusId )
	   {
			continue;
	   }

      ITERA_GGROUP_LESSPTR( itUnidade, itCampus->unidades, Unidade )
      {
         ITERA_GGROUP_LESSPTR( itCjtSala, itUnidade->conjutoSalas, ConjuntoSala )
         {
			std::map< int, Sala * >::iterator itSala = itCjtSala->salas.begin();
			for( ; itSala != itCjtSala->salas.end(); itSala++ )
			{
				Sala *s = itSala->second;

				ITERA_GGROUP_N_PT( itDia, s->diasLetivos, int )
				{
					int dia = *itDia;

					std::map< Trio<int, int, Calendario*>, int >::iterator it_map = s->combinaCredSL.begin();
					for ( ; it_map != s->combinaCredSL.end(); it_map++ )
					{
						if ( it_map->first.first == dia )
						{
							Variable v;
							v.reset();
							v.setType( Variable::V_COMBINA_SL_SALA );
							v.setSala( s );
							v.setDia( dia );
							v.setCombinaSL( it_map->first.second );

							if ( vHash.find( v ) == vHash.end() )
							{
								if ( !criaVariavelTatico( &v ) )
									continue;

								vHash[v] = lp->getNumCols();
						   
								OPT_COL col( OPT_COL::VAR_BINARY, 0.0, 0.0, 1.0, (char*)v.toString().c_str() );

								lp->newCol( col );
								num_vars++;
							}
						}
					}
				}
			}
		 }
	  }
   }

   return num_vars;
}

// cbc_{bc,t,k}
int SolverMIP::cria_variavel_maxCreds_combina_Sl_bloco( int campusId )
{
   int num_vars = 0;

   ITERA_GGROUP_LESSPTR( itBloco, problemData->blocos, BlocoCurricular )
   {
	   if ( itBloco->campus->getId() != campusId )
	   {
			continue;
	   }

	   BlocoCurricular *bc = *itBloco;
		
	   ITERA_GGROUP_N_PT( itDia, bc->diasLetivos, int )
	   {
		   int dia = *itDia;

		   std::map< Trio< int/*dia*/, int /*k_id*/, Calendario* /*sl*/ >,
					  int/*nroCreds*/ >::iterator it_map = bc->combinaCredSL.begin();

			for ( ; it_map != bc->combinaCredSL.end(); it_map++ )
			{
				if ( it_map->first.first == dia )
				{
					Variable v;
					v.reset();
					v.setType( Variable::V_COMBINA_SL_BLOCO );

					v.setBloco( bc );
					v.setDia( dia );
					v.setCombinaSLBloco( it_map->first.second );

					if ( vHash.find( v ) == vHash.end() )
					{
						if ( !criaVariavelTatico( &v ) )
							continue;

						vHash[v] = lp->getNumCols();
						   
						OPT_COL col( OPT_COL::VAR_BINARY, 0.0, 0.0, 1.0, (char*)v.toString().c_str() );

						lp->newCol( col );
						num_vars++;
					}
				}
			}
	    }
   }

   return num_vars;
}



// ==============================================================
//							CONSTRAINTS
// ==============================================================

int SolverMIP::cria_restricoes( int campusId )
{
	int restricoes = 0;

	CPUTimer timer;
	double dif = 0.0;

#ifdef PRINT_cria_restricoes
	int numRestAnterior = 0;
#endif

	timer.start();
	restricoes += cria_restricao_carga_horaria( campusId );				// Restricao 1.2.2
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest \"1.2.2\": " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
#endif

	timer.start();
	restricoes += cria_restricao_max_tempo_sd( campusId );					// Restricao 1.2.3.a
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest \"1.2.3.a\": " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
#endif

	timer.start();
	restricoes += cria_restricao_max_tempo_s_t_SL( campusId );
	timer.stop();
	dif = timer.getCronoCurrSecs();

	timer.start();
	//   restricoes += cria_restricao_max_tempo_s_d_SL();				// Restricao 1.2.3.b
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest \"1.2.3.b\": " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
#endif

	timer.start();
	restricoes += cria_restricao_min_cred_dd( campusId );					// Restricao 1.2.4
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest \"1.2.4\": " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
#endif

	timer.start();
	restricoes += cria_restricao_ativacao_var_o( campusId );					// Restricao 1.2.5
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest \"1.2.5\": " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
#endif

	timer.start();
	restricoes += cria_restricao_evita_sobreposicao( campusId );			// Restricao 1.2.6
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest \"1.2.6\": " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
#endif

	timer.start();
	restricoes += cria_restricao_disciplina_sala( campusId );				// Restricao 1.2.7
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest \"1.2.7\": " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
#endif

	timer.start();
	restricoes += cria_restricao_turma_sala( campusId );					// Restricao 1.2.8
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest \"1.2.8\": " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
#endif
	/*
	timer.start();
	restricoes += cria_restricao_evita_turma_disc_camp_d();				// Restricao 1.2.9 N�o � usada em Otimiza��o Por Campus
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest \"1.2.9\": " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
#endif
	*/
	timer.start();
	restricoes += cria_restricao_turmas_bloco( campusId );				// Restricao 1.2.10
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest \"1.2.10\": " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
#endif
	
	timer.start();
	restricoes += cria_restricao_max_cred_disc_bloco( campusId );			// Restricao 1.2.11
	timer.stop();
	dif = timer.getCronoCurrSecs();

	#ifdef PRINT_cria_restricoes
	std::cout << "numRest \"1.2.11\": " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
	#endif
		
	timer.start();
	restricoes += cria_restricao_num_tur_bloc_dia_difunid( campusId );	// Restricao 1.2.12
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest \"1.2.12\": " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
#endif

	timer.start();
	restricoes += cria_restricao_lim_cred_diar_disc( campusId );			// Restricao 1.2.13
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest \"1.2.13\": " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
#endif

	timer.start();
	restricoes += cria_restricao_cap_aloc_dem_disc( campusId );			// Restricao 1.2.14
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest \"1.2.14\": " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
#endif

	timer.start();
	restricoes += cria_restricao_cap_sala_compativel_turma( campusId );	// Restricao 1.2.15
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest \"1.2.15\": " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
#endif

	timer.start();
	restricoes += cria_restricao_cap_sala_unidade( campusId );			// Restricao 1.2.16
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest \"1.2.16\": " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
#endif

	timer.start();
	//restricoes += cria_restricao_turma_disc_dias_consec();		// Restricao 1.2.17
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	//std::cout << "numRest \"1.2.17\": " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	std::cout << "numRest \"1.2.17\": NAO ESTA SENDO CRIADA DEVIDO A ERROS DE IMPLEMENTACAO - VER ToDo 12 (MARIO)"  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
#endif

	timer.start();
	restricoes += cria_restricao_min_creds_turm_bloco( campusId );		// Restricao 1.2.18
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest \"1.2.18\": " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
#endif

	timer.start();
	restricoes += cria_restricao_max_creds_turm_bloco( campusId );		// Restricao 1.2.19
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest \"1.2.19\": " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
#endif

	timer.start();
	restricoes += cria_restricao_aluno_curso_disc( campusId );			// Restricao 1.2.20
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest \"1.2.20\": " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
#endif

	timer.start();
	restricoes += cria_restricao_alunos_cursos_incompat( campusId );		// Restricao 1.2.21
	timer.stop();
	dif = timer.getCronoCurrSecs();
	

#ifdef PRINT_cria_restricoes
	std::cout << "numRest \"1.2.21\": " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	//std::cout << "numVars \"1.2.21\": NAO ESTA SENDO CRIADA DEVIDO A ERROS DE IMPLEMENTACAO (A Inst. UNI-BH nao precisa dessa restricao implementada)."  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
#endif


	timer.start();
	restricoes += cria_restricao_de_folga_dist_cred_dia( campusId );		// Restricao 1.2.22
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest \"1.2.22\": " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	//std::cout << "numRest \"1.2.22\": NAO ESTA SENDO CRIADA DEVIDO A NOVA MODELAGEM QUE O MARCELO FEZ."  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
#endif


	timer.start();
	restricoes += cria_restricao_ativacao_var_r( campusId );						// Restricao 1.2.23
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest \"1.2.23\": " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
#endif


	timer.start();
	restricoes += cria_restricao_limita_abertura_turmas( campusId );						// Restricao 1.2.24
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest \"1.2.24\": " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
#endif


	timer.start();
	restricoes += cria_restricao_abre_turmas_em_sequencia( campusId );						// Restricao 1.2.25
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest \"1.2.25\": " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
#endif


	timer.start();
	restricoes += cria_restricao_divisao_credito( campusId );
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest \"1.2.26\": " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
#endif


	timer.start();
	restricoes += cria_restricao_combinacao_divisao_credito( campusId );
	timer.stop();
	dif = timer.getCronoCurrSecs();
	

#ifdef PRINT_cria_restricoes
	std::cout << "numRest \"1.2.27\": " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
#endif


	timer.start();
	restricoes += cria_restricao_ativacao_var_y( campusId );
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest \"1.2.28\": " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
#endif

	/*
	timer.start();
	restricoes += cria_restricao_max_creds_disc_dia();
	timer.stop();
	dif = timer.getCronoCurrSecs();
	

	#ifdef PRINT_cria_restricoes
	std::cout << "numRest \"1.2.29\": " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
	#endif

	timer.start();
	restricoes += cria_restricao_max_creds_bloco_dia();
	timer.stop();
	dif = timer.getCronoCurrSecs();
	

	#ifdef PRINT_cria_restricoes
	std::cout << "numRest \"1.2.30\": " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
	#endif

	*/


	timer.start();
	restricoes += cria_restricao_ativacao_var_zc( campusId );
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest \"1.2.31\": " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
#endif

	timer.start();
	restricoes +=  cria_restricao_disciplinas_incompativeis( campusId );
	timer.stop();
	dif = timer.getCronoCurrSecs();
	

#ifdef PRINT_cria_restricoes
	std::cout << "numRest \"1.2.32\": " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
#endif

	timer.start();
	//restricoes +=  cria_restricao_abertura_bloco_mesmoTPS();
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	//std::cout << "numRest \"1.2.33\": " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	std::cout << "numRest \"1.2.33\": NAO ESTA SENDO CRIADA DEVIDO A ERRO DE MODELAGEM - (MARIO)"  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
#endif

	timer.start();
	//restricoes +=  cria_restricao_folga_abertura_bloco_mesmoTPS();
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	//std::cout << "numRest \"1.2.34\": " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	std::cout << "numRest \"1.2.34\": NAO ESTA SENDO CRIADA DEVIDO A ERRO DE MODELAGEM DA RESTRICAO 1.2.33 - (MARIO)"  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
#endif

	timer.start();
	restricoes +=  cria_restricao_proibe_compartilhamento( campusId );			// Restricao 1.2.35
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest \"1.2.35\": " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
#endif


	timer.start();
	restricoes +=  cria_restricao_ativacao_var_e( campusId );				// Restricao 1.2.36
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest \"1.2.36\": " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
#endif


	timer.start();
	restricoes +=  cria_restricao_evita_sobrepos_sala_por_compartilhamento( campusId );	// Restricao 1.2.37
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest \"1.2.37\": " << (restricoes - numRestAnterior) <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
#endif

	timer.start();
	restricoes +=  cria_restricao_ativacao_var_of( campusId );	// Restricao 1.2.38, 1.2.39, 1.2.40
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest \"1.2.38, 1.2.39, 1.2.40\": " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
#endif

	timer.start();
	restricoes +=  cria_restricao_ativacao_var_p( campusId );	// Restri��es 1.2.41, 1.2.42, 1.2.43
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest \"1.2.41, 1.2.42, 1.2.43\": " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
#endif

	timer.start();
	restricoes +=  cria_restricao_ativacao_var_g( campusId );	// Restricao 1.2.44
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest \"1.2.44\": " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
#endif

	timer.start();
	restricoes +=  cria_restricao_evita_sobrepos_sala_por_div_turmas( campusId );	// Restricao 1.2.45
	timer.stop();
	dif = timer.getCronoCurrSecs();
	

#ifdef PRINT_cria_restricoes
	std::cout << "numRest \"1.2.45\": " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
#endif

	timer.start();
	restricoes +=  cria_restricao_ativacao_var_q( campusId );	// Restricoes 1.2.46, 1.2.47, 1.2.48
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest \"1.2.46, 1.2.47, 1.2.48\": " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
#endif

	timer.start();
	restricoes += cria_restricao_ativacao_var_cs( campusId ); // Restricao 1.2.49
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest \"1.2.49\": " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
#endif

	timer.start();
	restricoes += cria_restricao_fixa_nao_compartilhamento( campusId ); // Restricao 1.2.50
	timer.stop();
	dif = timer.getCronoCurrSecs();
	

#ifdef PRINT_cria_restricoes
	std::cout << "numRest \"1.2.50\": " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
#endif

	timer.start();
	restricoes += cria_restricao_ativacao_var_cbc( campusId ); // Restricao 1.2.51
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest \"1.2.51\": " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
#endif

	return restricoes;
}

/*====================================================================/
%DocBegin TRIEDA_LOAD_MODEL

%Constraint 
Carga hor�ria de todas as turmas de cada disciplina

%MatExp 

\begin{eqnarray}
\sum\limits_{u \in U}\sum\limits_{tps \in SCAP_{u}}\sum\limits_{t \in T} x_{i,d,u,tps,t}  =  C_{d} \cdot z_{i,d,cp} \nonumber \qquad 
\forall d \in D \quad
\forall i \in I_{d} \quad 
\forall cp \in CP 
\end{eqnarray}

%Data C_{d}
%Desc
Total de cr�ditos da disciplina $d$.

%Data I_{d}
%Desc
M�ximo de turmas que podem ser abertas da disciplina $d$.

%DocEnd
/====================================================================*/

int SolverMIP::cria_restricao_carga_horaria( int campusId )
{
   int restricoes = 0;

   int nnz;
   char name[ 100 ];

   Variable v;
   Constraint c;
   VariableHash::iterator it_v;

   Disciplina * disciplina = NULL;
   Disciplina * disciplina_equivalente = NULL;

   Curso * curso = NULL;
   Curriculo * curriculo = NULL;

   ITERA_GGROUP_LESSPTR( itCampus, problemData->campi, Campus )
   {
	   if ( itCampus->getId() != campusId )
	   {
			continue;
	   }

      ITERA_GGROUP_LESSPTR( it_disciplina, problemData->disciplinas, Disciplina )
      {
         disciplina = ( *it_disciplina );

		 // A disciplina deve ser ofertada no campus especificado
		 if ( problemData->cp_discs[campusId].find( disciplina->getId() ) ==
			  problemData->cp_discs[campusId].end() )
		 {
			 continue;
		 }

         std::pair< Curso *, Curriculo * > curso_curriculo
            = problemData->map_Disc_CursoCurriculo[ disciplina ];
         curso = curso_curriculo.first;
         curriculo = curso_curriculo.second;

         disciplina_equivalente = problemData->retornaDisciplinaSubstituta( curso, curriculo, disciplina );
         if ( disciplina_equivalente != NULL )
         {
            disciplina = disciplina_equivalente;
         }

         for ( int turma = 0; turma < disciplina->getNumTurmas(); turma++ )
         {
            c.reset();
            c.setType( Constraint::C_CARGA_HORARIA );

            c.setCampus( *itCampus );
            c.setTurma( turma );
            c.setDisciplina( disciplina );

            sprintf( name, "%s", c.toString().c_str() );

            if ( cHash.find( c ) != cHash.end() )
            {
               continue;
            }

            nnz = ( itCampus->getTotalSalas() * 7 );

            OPT_ROW row( nnz + 1, OPT_ROW::EQUAL , 0 , name );

            v.reset();
            v.setType( Variable::V_CREDITOS );

            // ---

            ITERA_GGROUP_LESSPTR( itUnidade, itCampus->unidades, Unidade )
            {
               ITERA_GGROUP_LESSPTR( itCjtSala, itUnidade->conjutoSalas, ConjuntoSala )
               {
                  GGroup< int /*dias*/ > disc_sala_dias
                     = problemData->disc_Conjutno_Salas__Dias
                     [ std::make_pair< int, int > ( disciplina->getId(), itCjtSala->getId() ) ];

                  ITERA_GGROUP_N_PT( itDiscSala_Dias, disc_sala_dias, int )
                  {
                     v.setTurma( turma );
                     v.setDisciplina( disciplina );
                     v.setUnidade( *itUnidade );
                     v.setSubCjtSala( *itCjtSala );
                     v.setDia( *itDiscSala_Dias );                   

                     it_v = vHash.find( v );
                     if( it_v != vHash.end() )
                     {
                        row.insert( it_v->second, 1.0 );
                     }
                  }
               }
            }

            // ---

            v.reset();
            v.setType( Variable::V_ABERTURA );

            v.setCampus( *itCampus );
            v.setDisciplina( disciplina );
            v.setTurma( turma );

            it_v = vHash.find( v );
            if( it_v != vHash.end() )
            {
               row.insert( it_v->second,
                  -( disciplina->getCredPraticos() + 
                  disciplina->getCredTeoricos() ) );
            }

            // ---

            if ( row.getnnz() != 0 )
            {
               cHash[ c ] = lp->getNumRows();

               lp->addRow( row );
               restricoes++;
            }
         }
      }
   }

   return restricoes;
}



/*====================================================================/
%DocBegin TRIEDA_LOAD_MODEL

%Constraint 
M�ximo de tempo por sala e dia
%Desc 

%MatExp

\begin{eqnarray}
\sum\limits_{d \in D}\sum\limits_{i \in I_{d}} sl_{d} \cdot x_{i,d,u,tps,t} \leq  HTPS_{t,tps} \nonumber \qquad 
\forall u \in U \quad
\forall tps \in SCAP_{u} \quad
\forall t \in T
\end{eqnarray}

%Data HTPS_{t,tps}
%Desc
m�ximo de tempo permitido por dia $t$ para o conjunto de salas do tipo (capacidade) $tps$.

%DocEnd
/====================================================================*/

int SolverMIP::cria_restricao_max_tempo_sd( int campusId )
{
   int restricoes = 0;

   // Metodo somente utilizado quando h� 1 semana letiva
   if ( problemData->calendarios.size() != 1 )
   {
	   return restricoes;
   }

   int nnz;
   char name[ 100 ];

   Constraint c;
   Variable v;
   VariableHash::iterator it_v;

   Curso * curso = NULL;
   Curriculo * curriculo = NULL;
   Disciplina * disciplina = NULL;

   ITERA_GGROUP_LESSPTR( itCampus, problemData->campi, Campus )
   {
	   if ( itCampus->getId() != campusId )
	   {
		   continue;
	   }

      ITERA_GGROUP_LESSPTR( itUnidade, itCampus->unidades, Unidade )
      {
         ITERA_GGROUP_LESSPTR( itCjtSala, itUnidade->conjutoSalas, ConjuntoSala )
         {
            //////////////////////////////////////////////////////////////////////
            // Percorre cada disciplina do conjunto de salas
            std::map< Disciplina *, GGroup< int > >::iterator
               it_disc_dias = itCjtSala->dias_letivos_disciplinas.begin();

            for (; it_disc_dias != itCjtSala->dias_letivos_disciplinas.end(); it_disc_dias++ )
            {
               // Verifica se a disciplina foi substitu�da
               disciplina = it_disc_dias->first;

				#pragma region Equivalencias
				if ( problemData->mapDiscSubstituidaPor.find( disciplina ) !=
					 problemData->mapDiscSubstituidaPor.end() )
				{
					continue;
				}
				#pragma endregion

               GGroup< int >::iterator itDiasLetCjtSala =
                  it_disc_dias->second.begin();

               for (; itDiasLetCjtSala != it_disc_dias->second.end(); itDiasLetCjtSala++ )
               {
                  c.reset();
                  c.setType( Constraint::C_MAX_TEMPO_SD );

                  c.setUnidade( *itUnidade );
                  c.setSubCjtSala( *itCjtSala );
                  c.setDia( *itDiasLetCjtSala );

                  sprintf( name, "%s", c.toString().c_str() );
                  if ( cHash.find( c ) != cHash.end() )
                  {
                     continue;
                  }

                  nnz = 0;
                  ITERA_GGROUP_LESSPTR( itDisc, problemData->disciplinas, Disciplina )
                  {
					 // A disciplina deve ser ofertada no campus especificado
					 if ( problemData->cp_discs[campusId].find( itDisc->getId() ) ==
						  problemData->cp_discs[campusId].end() )
					 {
						 continue;
					 }
                     if ( itDisc->getNumTurmas() >= 0 )
                     {
                        nnz += ( itDisc->getNumTurmas() );
                     }
                  }

                  int HTPS = itCjtSala->maxTempoPermitidoPorDia( *itDiasLetCjtSala );
                  OPT_ROW row( nnz, OPT_ROW::LESS , HTPS, name );

                  ITERA_GGROUP_LESSPTR( itDisc, problemData->disciplinas, Disciplina )
                  {
					 #pragma region Equivalencias
					 if ( problemData->mapDiscSubstituidaPor.find( *itDisc ) !=
						  problemData->mapDiscSubstituidaPor.end() )
					 {
						continue;
					 }
					 #pragma endregion

					 // A disciplina deve ser ofertada no campus especificado
					 if ( problemData->cp_discs[campusId].find( itDisc->getId() ) ==
						  problemData->cp_discs[campusId].end() )
					 {
						 continue;
					 }

                     for ( int turma = 0; turma < itDisc->getNumTurmas(); turma++ )
                     {
                        v.reset();
                        v.setType( Variable::V_CREDITOS );

                        v.setTurma( turma );
                        v.setUnidade( *itUnidade );
                        v.setDisciplina( *itDisc );
                        v.setSubCjtSala( *itCjtSala );
                        v.setDia( *itDiasLetCjtSala );

                        it_v = vHash.find( v );
                        if( it_v != vHash.end() )
                        {
							row.insert( it_v->second, itDisc->getTempoCredSemanaLetiva() );
                        }
                     }
                  }

                  if ( row.getnnz() != 0 )
                  {
                     cHash[ c ] = lp->getNumRows();

                     lp->addRow( row );
                     restricoes++;
                  }
               }
            }
            //////////////////////////////////////////////////////////////////////
         }
      }
   }

   return restricoes;
}


/*====================================================================/
%DocBegin TRIEDA_LOAD_MODEL

%Constraint 
M�ximo de tempo por sala, dia e semana letiva, de acordo com a divisao
de horarios por semana letiva escolhida.
%Desc 

%MatExp

\begin{eqnarray}

\sum\limits_{d \in D_{sl}}\sum\limits_{i \in I_{d}} x_{i,d,u,s,t} \leq  \sum\limits_{k \in CS_{k}}( Q_{sl,k,s,t} \cdot cs_{s,t,k} ) \nonumber \qquad

\forall u \in U \quad
\forall s \in S_{u} \quad
\forall t \in T \quad
\forall sl \in SL
\end{eqnarray}

%Data Q_{sl,t,s,k}
%Desc
m�ximo de tempo da semana letiva $sl$ permitidos por dia $t$
para a sala $s$, na divis�o de horarios k.

%DocEnd
/====================================================================*/

int SolverMIP::cria_restricao_max_tempo_s_t_SL( int campusId )
{
   int restricoes = 0;

   // Metodo somente utilizado quando h� 2 semanas letivas
   if ( problemData->calendarios.size() != 2 )
   {
	   return restricoes;
   }

   char name[ 100 ];
   Constraint c;
   Variable v;
   VariableHash::iterator it_v;

   ITERA_GGROUP_LESSPTR( itCampus, problemData->campi, Campus )
   {
	   if ( itCampus->getId() != campusId )
	   {
		   continue;
	   }

      ITERA_GGROUP_LESSPTR( itUnidade, itCampus->unidades, Unidade )
      {
         ITERA_GGROUP_LESSPTR( itCjtSala, itUnidade->conjutoSalas, ConjuntoSala )
         {
			std::map< int, Sala * >::iterator itSala = itCjtSala->salas.begin();
			for( ; itSala != itCjtSala->salas.end(); itSala++ )
			{
				Sala *s = itSala->second;

				ITERA_GGROUP_N_PT( itDia, s->diasLetivos, int )
				{
					int dia = *itDia;
				
					ITERA_GGROUP_LESSPTR( it_Sl, problemData->calendarios, Calendario )
					{
						//  Cria restri��o
						c.reset();
						c.setType( Constraint::C_MAX_TEMPO_S_D_SL );
						c.setUnidade( *itUnidade );
						c.setSubCjtSala( *itCjtSala );
						c.setDia( dia );
						c.setSemanaLetiva( *it_Sl );

						sprintf( name, "%s", c.toString().c_str() );
						if ( cHash.find( c ) != cHash.end() )
						{
							continue;
						}

						int nnz = 100;
					
						OPT_ROW row( nnz, OPT_ROW::LESS , 0, name );

						ITERA_GGROUP( itDisc, s->disciplinasAssociadas, Disciplina )
						{
							Disciplina *disciplina = *itDisc;

							// Verifica se � a semana letiva corrente
							if ( disciplina->getCalendario() != *it_Sl )
							{
								continue;
							}

							#pragma region Equivalencias
							if ( problemData->mapDiscSubstituidaPor.find( disciplina ) !=
								problemData->mapDiscSubstituidaPor.end() )
							{
								continue;
							}
							#pragma endregion		

							for ( int turma = 0; turma < disciplina->getNumTurmas(); turma++ )
							{
								v.reset();
								v.setType( Variable::V_CREDITOS );
								v.setTurma( turma );
								v.setUnidade( *itUnidade );
								v.setDisciplina( disciplina );
								v.setSubCjtSala( *itCjtSala );
								v.setDia( dia );

								it_v = vHash.find( v );
								if( it_v != vHash.end() )
								{
									row.insert( it_v->second, 1 );
								}
							}
						}
						std::map< Trio<int, int, Calendario*>, int >::iterator it_map = s->combinaCredSL.begin();
						for ( ; it_map != s->combinaCredSL.end(); it_map++  )
						{
							if ( it_map->first.first == dia && it_map->first.third == *it_Sl )
							{
								v.reset();
								v.setType( Variable::V_COMBINA_SL_SALA );
								v.setSala( s );
								v.setDia( dia );
								v.setCombinaSL( it_map->first.second );

								it_v = vHash.find( v );
								if( it_v != vHash.end() )
								{
									int coef = s->getNroCredCombinaSL( it_map->first.second, *it_Sl, dia );

									row.insert( it_v->second, -coef );
								}
							}
						}
					
						if ( row.getnnz() != 0 )
						{
							cHash[ c ] = lp->getNumRows();
							lp->addRow( row );
							restricoes++;
						}
					}
				}
			}
         }
      }
   }

   return restricoes;
}




/*====================================================================/
%DocBegin TRIEDA_LOAD_MODEL

%Constraint 
M�nimo de cr�ditos di�rios da disciplina (*)
%Desc

%MatExp

\begin{eqnarray}
\underline{H_{d}} \cdot o_{i,d,u,tps,t}  \leq  x_{i,d,u,tps,t}  \nonumber \qquad 
\forall d \in D \quad
\forall i \in I_{d} \quad
\forall u \in U \quad
\forall tps \in SCAP_{u} \quad
\forall t \in T
\end{eqnarray}

%Data \underline{H_{d}} 
%Desc
m�nimo de cr�ditos di�rios da disciplina $d$.

%DocEnd
/====================================================================*/

// TRIEDA-405 Cont-M�nimo de cr�ditos di�rios da disciplina(*)
int SolverMIP::cria_restricao_min_cred_dd( int campusId )
{
   int restricoes = 0;
   char name[ 100 ];
   int nnz;

   Variable v;
   Constraint c;
   VariableHash::iterator it_v;

   Disciplina * disciplina = NULL;
   Disciplina * disciplina_equivalente = NULL;

   Curso * curso = NULL;
   Curriculo * curriculo = NULL;

   ITERA_GGROUP_LESSPTR( itCampus, problemData->campi, Campus )
   {
	   if ( itCampus->getId() != campusId )
	   {
		   continue;
	   }

      ITERA_GGROUP_LESSPTR( itUnidade, itCampus->unidades, Unidade )
      {
         ITERA_GGROUP_LESSPTR( itCjtSala, itUnidade->conjutoSalas, ConjuntoSala )
         {
            ITERA_GGROUP_LESSPTR( it_disciplina, itCjtSala->disciplinas_associadas, Disciplina )
            {
                disciplina = ( *it_disciplina );

				#pragma region Equivalencias
				if ( problemData->mapDiscSubstituidaPor.find( disciplina ) !=
					 problemData->mapDiscSubstituidaPor.end() )
				{
					continue;
				}
				#pragma endregion		
				
               GGroup< int /*Dias*/ > disc_sala_dias =
                  problemData->disc_Conjutno_Salas__Dias[ std::make_pair< int, int >
                  ( disciplina->getId(), itCjtSala->getId() ) ];

               ITERA_GGROUP_N_PT( itDiscSala_Dias, disc_sala_dias, int )
               {
                  for ( int turma = 0; turma < disciplina->getNumTurmas(); turma++ )
                  {
                     c.reset();
                     c.setType( Constraint::C_MIN_CREDITOS_DD );

                     c.setUnidade( *itUnidade );
                     c.setSubCjtSala( *itCjtSala );
                     c.setDia( *itDiscSala_Dias );
                     c.setDisciplina( disciplina );
                     c.setTurma( turma	);

                     sprintf( name, "%s", c.toString().c_str() );
                     if ( cHash.find( c ) != cHash.end() )
                     {
                        continue;
                     }

                     nnz = 2;

                     v.reset();
                     v.setType( Variable::V_OFERECIMENTO );

                     v.setTurma( turma );
                     v.setDisciplina( disciplina );
                     v.setUnidade( *itUnidade );
                     v.setSubCjtSala( *itCjtSala );
                     v.setDia( *itDiscSala_Dias );

                     // 1.2.4
                     // M�nimo de cr�ditos di�rios da disciplina (*)
                     int min_creditos = disciplina->getMinCreds();
                     int min_creditos_fixacao
                        = problemData->creditosFixadosDisciplinaDia( disciplina, *itDiscSala_Dias, *itCjtSala );

                     // Quando existir uma fixa��o, o m�nimo de cr�ditos
                     // dever� ser exatamente a fixa��o. Caso contr�rio,
                     // continua sendo o m�nimo de cr�ditos da disciplina
                     OPT_ROW::ROWSENSE row_sense = OPT_ROW::LESS;
                     if ( min_creditos_fixacao != 0 )
                     {
                        min_creditos = min_creditos_fixacao;
                        row_sense = OPT_ROW::EQUAL;
                     }

                     OPT_ROW row( nnz, row_sense , 0.0, name );

                     it_v = vHash.find( v );
                     if ( it_v != vHash.end() )
                     {
                        row.insert( it_v->second, min_creditos );
                     }

                     v.reset();
                     v.setType( Variable::V_CREDITOS );

                     v.setTurma( turma );
                     v.setDisciplina( disciplina );
                     v.setUnidade( *itUnidade );
                     v.setSubCjtSala( *itCjtSala );
                     v.setDia( *itDiscSala_Dias );

                     it_v = vHash.find( v );
                     if ( it_v != vHash.end() )
                     {
                        row.insert( it_v->second, -1.0 );
                     }

                     if ( row.getnnz() != 0 )
                     {
                        cHash[ c ] = lp->getNumRows();

                        lp->addRow( row );
                        restricoes++;
                     }
                  }
               }
            }
         }
      }
   }

   return restricoes;
}

/*====================================================================/
%DocBegin TRIEDA_LOAD_MODEL

%Constraint 
Ativa��o da vari�vel $o$
%Desc 

%MatExp

\begin{eqnarray}
C_{d} \cdot o_{i,d,u,tps,t}  \geq  x_{i,d,u,tps,t}  \nonumber \qquad 
\forall d \in D \quad
\forall i \in I_{d} \quad
\forall u \in U \quad
\forall tps \in SCAP_{u} \quad
\forall t \in T
\end{eqnarray}

%Data C_{d}
%Desc
Total de cr�ditos da disciplina $d$.

%DocEnd
/====================================================================*/

int SolverMIP::cria_restricao_ativacao_var_o( int campusId )
{
   int restricoes = 0;
   char name[ 100 ];
   int nnz;

   Variable v;
   Constraint c;
   VariableHash::iterator it_v;

   Disciplina * disciplina = NULL;
   Disciplina * disciplina_equivalente = NULL;

   Curso * curso = NULL;
   Curriculo * curriculo = NULL;

   ITERA_GGROUP_LESSPTR( itCampus, problemData->campi, Campus )
   {
	   if ( itCampus->getId() != campusId )
	   {
		   continue;
	   }

      ITERA_GGROUP_LESSPTR( itUnidade, itCampus->unidades, Unidade )
      {
         ITERA_GGROUP_LESSPTR( itCjtSala, itUnidade->conjutoSalas, ConjuntoSala )
         {
			ITERA_GGROUP_LESSPTR( it_disciplina, itCjtSala->disciplinas_associadas, Disciplina )
            {
               disciplina = ( *it_disciplina );
			   
				#pragma region Equivalencias
				if ( problemData->mapDiscSubstituidaPor.find( disciplina ) !=
					 problemData->mapDiscSubstituidaPor.end() )
				{
					continue;
				}
				#pragma endregion

               GGroup< int /*Dias*/ >::iterator itDiscSala_Dias = problemData->disc_Conjutno_Salas__Dias
                  [ std::make_pair< int, int > ( disciplina->getId(), itCjtSala->getId() ) ].begin();

               for (; itDiscSala_Dias != problemData->disc_Conjutno_Salas__Dias[
                      std::make_pair< int, int > ( disciplina->getId(), itCjtSala->getId() ) ].end();
                      itDiscSala_Dias++ )
                  {
                     for ( int turma = 0; turma < disciplina->getNumTurmas(); turma++ )
                     {
                        c.reset();
                        c.setType( Constraint::C_VAR_O );

                        c.setUnidade( *itUnidade );
                        c.setSubCjtSala( *itCjtSala );
                        c.setDia( *itDiscSala_Dias );
                        c.setDisciplina( disciplina );
                        c.setTurma( turma );

                        sprintf( name, "%s", c.toString().c_str() ); 
                        if ( cHash.find( c ) != cHash.end() )
                        {
                           continue;
                        }

                        nnz = 2;

                        OPT_ROW row( nnz, OPT_ROW::LESS , 0.0, name );

                        v.reset();
                        v.setType(Variable::V_OFERECIMENTO);

                        v.setTurma( turma );
                        v.setDisciplina( disciplina );
                        v.setUnidade( *itUnidade );
                        v.setSubCjtSala( *itCjtSala );
                        v.setDia( *itDiscSala_Dias );

                        it_v = vHash.find( v );
                        if( it_v != vHash.end() )
                        {
                           row.insert( it_v->second,
                              -( disciplina->getCredPraticos() + 
                              disciplina->getCredTeoricos() ) );
                        }

                        v.reset();
                        v.setType( Variable::V_CREDITOS );

                        v.setTurma( turma );
                        v.setDisciplina( disciplina );
                        v.setUnidade( *itUnidade );
                        v.setSubCjtSala( *itCjtSala );
                        v.setDia( *itDiscSala_Dias );

                        it_v = vHash.find( v );
                        if ( it_v != vHash.end() )
                        {
                           row.insert( it_v->second, 1.0 );
                        }

                        if ( row.getnnz() != 0 )
                        {
                           cHash[ c ] = lp->getNumRows();

                           lp->addRow( row );
                           restricoes++;
                        }
                     }
                  }
            }
         }
      }
   }

   return restricoes;
}

/*====================================================================/
%DocBegin TRIEDA_LOAD_MODEL

%Constraint 
Evitar sobreposi��o de turmas da mesma disciplina
%Desc 

%MatExp

\begin{eqnarray}
\sum\limits_{tps \in SCAP_{u}} o_{i,d,u,tps,t}  \leq  1  \nonumber \qquad 
\forall d \in D \quad
\forall i \in I_{d} \quad
\forall u \in U \quad
\forall t \in T
\end{eqnarray}

%DocEnd
/====================================================================*/

int SolverMIP::cria_restricao_evita_sobreposicao( int campusId )
{
   int restricoes = 0;

   char name[ 100 ];
   int nnz;

   Constraint c;
   Variable v;
   VariableHash::iterator it_v;

   Disciplina * disciplina = NULL;
   Disciplina * disciplina_equivalente = NULL;

   Curso * curso = NULL;
   Curriculo * curriculo = NULL;

   ITERA_GGROUP_LESSPTR( itCampus, problemData->campi, Campus )
   {
	   if ( itCampus->getId() != campusId )
	   {
		   continue;
	   }

      ITERA_GGROUP_LESSPTR( itUnidade, itCampus->unidades, Unidade )
      {
         GGroup< int >::iterator itDiasLetUnid =
            itUnidade->dias_letivos.begin();

         for (; itDiasLetUnid != itUnidade->dias_letivos.end(); itDiasLetUnid++ )
         {
            ITERA_GGROUP_LESSPTR( it_disciplina, problemData->disciplinas, Disciplina )
            {
                disciplina = ( *it_disciplina );
			   
				#pragma region Equivalencias
				if ( problemData->mapDiscSubstituidaPor.find( disciplina ) !=
					 problemData->mapDiscSubstituidaPor.end() )
				{
					continue;
				}
				#pragma endregion	

				// A disciplina deve ser ofertada no campus especificado
				if ( problemData->cp_discs[campusId].find( disciplina->getId() ) ==
					 problemData->cp_discs[campusId].end() )
				{
					continue;
				}

               for ( int turma = 0; turma < disciplina->getNumTurmas(); turma++ )
               {
                  c.reset();
                  c.setType( Constraint::C_EVITA_SOBREPOSICAO_TD );

                  c.setUnidade( *itUnidade );
                  c.setDia( *itDiasLetUnid );
                  c.setDisciplina( disciplina );
                  c.setTurma( turma );

                  sprintf( name, "%s", c.toString().c_str() ); 
                  if ( cHash.find( c ) != cHash.end() )
                  {
                     continue;
                  }

                  nnz = itUnidade->salas.size();

                  OPT_ROW row( nnz, OPT_ROW::LESS , 1.0, name );

                  ITERA_GGROUP_LESSPTR( itCjtSala, itUnidade->conjutoSalas, ConjuntoSala )
                  {
                     v.reset();
                     v.setType( Variable::V_OFERECIMENTO );

                     v.setTurma( turma );
                     v.setDisciplina( disciplina );
                     v.setUnidade( *itUnidade );
                     v.setSubCjtSala( *itCjtSala );
                     v.setDia( *itDiasLetUnid );

                     it_v = vHash.find( v );
                     if( it_v != vHash.end() )
                     {
                        row.insert( it_v->second, 1.0 );
                     }
                  }

                  if ( row.getnnz() != 0 )
                  {
                     cHash[ c ] = lp->getNumRows();

                     lp->addRow( row );
                     restricoes++;
                  }
               }
            }
         }
      }
   }

   return restricoes;
}

/*====================================================================/
%DocBegin TRIEDA_LOAD_MODEL

%Constraint 
Indica��o de que uma turma de uma disciplina foi alocada 
em um determinado tipo de sala (*)
%Desc 

%MatExp

\begin{eqnarray}
\sum\limits_{t \in T} o_{i,d,u,tps,t}  \leq  7 \cdot y_{i,d,tps,u}  \nonumber \qquad 
\forall d \in D \quad
\forall i \in I_{d} \quad
\forall u \in U \quad
\forall s \in S_{u}
\forall tps \in SCAP_{u}
\end{eqnarray}

%DocEnd
/====================================================================*/

int SolverMIP::cria_restricao_disciplina_sala( int campusId )
{
   int restricoes = 0;
   char name[ 100 ];
   int nnz;

   Constraint c;
   Variable v;
   VariableHash::iterator it_v;

   Disciplina * disciplina = NULL;
   Disciplina * disciplina_equivalente = NULL;

   Curso * curso = NULL;
   Curriculo * curriculo = NULL;

   ITERA_GGROUP_LESSPTR( itCampus, problemData->campi, Campus )
   {
	   if ( itCampus->getId() != campusId )
	   {
		   continue;
	   }

      ITERA_GGROUP_LESSPTR( itUnidade, itCampus->unidades, Unidade )
      {
         ITERA_GGROUP_LESSPTR( itCjtSala, itUnidade->conjutoSalas, ConjuntoSala )
         {
			 ITERA_GGROUP_LESSPTR( it_disciplina, itCjtSala->disciplinas_associadas, Disciplina )
            {
               disciplina = ( *it_disciplina );

				#pragma region Equivalencias
				if ( problemData->mapDiscSubstituidaPor.find( disciplina ) !=
					 problemData->mapDiscSubstituidaPor.end() )
				{
					continue;
				}
				#pragma endregion	

               for ( int turma = 0; turma < disciplina->getNumTurmas(); turma++ )
               {
                  c.reset();
                  c.setType( Constraint::C_TURMA_DISCIPLINA_SALA );

                  c.setUnidade( *itUnidade );
                  c.setSubCjtSala( *itCjtSala );
                  c.setDisciplina( disciplina );
                  c.setTurma( turma );

                  sprintf( name, "%s", c.toString().c_str() );
                  if ( cHash.find( c ) != cHash.end() )
                  {
                     continue;
                  }

                  nnz = 8;
                  OPT_ROW row( nnz, OPT_ROW::LESS , 0.0, name );

                  GGroup< int /*Dias*/>::iterator itDiscSala_Dias =
                     problemData->disc_Conjutno_Salas__Dias
                     [ std::make_pair< int, int > ( disciplina->getId(), itCjtSala->getId() ) ].begin();

                  for(; itDiscSala_Dias != problemData->disc_Conjutno_Salas__Dias
                     [ std::make_pair< int, int > ( disciplina->getId(), itCjtSala->getId() ) ].end(); itDiscSala_Dias++ )
                  {
                     v.reset();
                     v.setType( Variable::V_OFERECIMENTO );

                     v.setTurma( turma );
                     v.setDisciplina( disciplina );
                     v.setUnidade( *itUnidade );
                     v.setSubCjtSala( *itCjtSala );
                     v.setDia( *itDiscSala_Dias );

                     it_v = vHash.find( v );
                     if ( it_v != vHash.end() )
                     {
                        row.insert( it_v->second, 1.0 );
                     }
                  }

                  v.reset();
                  v.setType( Variable::V_ALOC_DISCIPLINA );

                  v.setTurma( turma );
                  v.setDisciplina( disciplina );
                  v.setUnidade( *itUnidade );
                  v.setSubCjtSala( *itCjtSala );

                  it_v = vHash.find( v );
                  if ( it_v != vHash.end() )
                  {
                     row.insert( it_v->second, -7.0 );
                  }

                  if ( row.getnnz() != 0 )
                  {
                     cHash[ c ] = lp->getNumRows();

                     lp->addRow( row );
                     restricoes++;
                  }
               }
            }
         }
      }
   }

   return restricoes;
}

/*====================================================================/
%DocBegin TRIEDA_LOAD_MODEL

%Constraint 
Uma turma s� pode ser alocada a um tipo de sala
%Desc 

%MatExp

\begin{eqnarray}
\sum\limits_{u \in U} \sum\limits_{tps \in SCAP_{u}} y_{i,d,tps,u}  \leq  1 \nonumber \qquad 
\forall d \in D \quad
\forall i \in I_{d} \quad
\end{eqnarray}

%DocEnd
/====================================================================*/

// TRIEDA-413 - Garante que a mesma turma tenha que ser alocada no mesmo tipo de sala em dias diferentes
int SolverMIP::cria_restricao_turma_sala( int campusId )
{
   int restricoes = 0;
   char name[100];
   int nnz;

   Constraint c;
   Variable v;
   VariableHash::iterator it_v;

   Disciplina * disciplina = NULL;

   ITERA_GGROUP_LESSPTR( it_disciplina, problemData->disciplinas, Disciplina )
   {
		disciplina = ( *it_disciplina );

		#pragma region Equivalencias
		if ( problemData->mapDiscSubstituidaPor.find( disciplina ) !=
				problemData->mapDiscSubstituidaPor.end() )
		{
			continue;
		}
		#pragma endregion	

		// A disciplina deve ser ofertada no campus especificado
		if ( problemData->cp_discs[campusId].find( disciplina->getId() ) ==
			 problemData->cp_discs[campusId].end() )
		{
			continue;
		}

		for ( int turma = 0; turma < disciplina->getNumTurmas(); turma++ )
		{
			c.reset();
			c.setType( Constraint::C_TURMA_SALA );

			c.setDisciplina( disciplina );
			c.setTurma( turma );

			sprintf( name, "%s", c.toString().c_str() ); 
			if ( cHash.find( c ) != cHash.end() )
			{
				continue;
			}

			// Pode ser menor esse valor. Na verdade, ele
			// tem que ser igual ao total de conjuntos de salas.
			nnz = problemData->totalSalas;
			OPT_ROW row( nnz, OPT_ROW::LESS , 1.0, name );

			ITERA_GGROUP_LESSPTR( itCampus, problemData->campi, Campus )
			{
				if ( itCampus->getId() != campusId )
				{
					continue;
				}

				ITERA_GGROUP_LESSPTR( itUnidade, itCampus->unidades, Unidade )
				{
					ITERA_GGROUP_LESSPTR( itCjtSala, itUnidade->conjutoSalas, ConjuntoSala )
					{
						v.reset();
						v.setType( Variable::V_ALOC_DISCIPLINA );

						v.setTurma( turma );
						v.setDisciplina( disciplina );
						v.setUnidade( *itUnidade );
						v.setSubCjtSala( *itCjtSala );

						it_v = vHash.find( v );
						if ( it_v != vHash.end() )
						{
							row.insert( it_v->second, 1.0 );
						}
					}
				}
			}

			if ( row.getnnz() != 0 )
			{
				cHash[ c ] = lp->getNumRows();

				lp->addRow( row );
				restricoes++;
			}
		}
	}

   return restricoes;
}

/*====================================================================/
%DocBegin TRIEDA_LOAD_MODEL

%Constraint 
Evitar aloca��o de turmas da mesma disciplina em campus diferentes
%Desc 

%MatExp

\begin{eqnarray}
\sum\limits_{cp \in CP} z_{i,d,cp}  \leq  1  \nonumber \qquad 
\forall d \in D \quad
\forall i \in I_{d}
\end{eqnarray}

%DocEnd
/====================================================================*/

int SolverMIP::cria_restricao_evita_turma_disc_camp_d()
{
   int restricoes = 0;
   char name[ 100 ];
   int nnz;

   Constraint c;
   Variable v;
   VariableHash::iterator it_v;

   Disciplina * disciplina = NULL;
   Disciplina * disciplina_equivalente = NULL;

   Curso * curso = NULL;
   Curriculo * curriculo = NULL;

   ITERA_GGROUP_LESSPTR( it_disciplina, problemData->disciplinas, Disciplina )
   {
       disciplina = ( *it_disciplina );

		#pragma region Equivalencias
		if ( problemData->mapDiscSubstituidaPor.find( disciplina ) !=
				problemData->mapDiscSubstituidaPor.end() )
		{
			continue;
		}
		#pragma endregion	

      for ( int i = 0; i < disciplina->getNumTurmas(); ++i )
      {
         c.reset();
         c.setType( Constraint::C_EVITA_TURMA_DISC_CAMP_D );

         c.setDisciplina( disciplina );
         c.setTurma( i );

         sprintf( name, "%s", c.toString().c_str() ); 
         if ( cHash.find( c ) != cHash.end() )
         {
            continue;
         }

         nnz = problemData->totalSalas;
         OPT_ROW row( nnz, OPT_ROW::LESS , 1.0, name );

         ITERA_GGROUP_LESSPTR( it_campus, problemData->campi, Campus )
         {
            v.reset();
            v.setType( Variable::V_ABERTURA );

            v.setTurma( i );
            v.setDisciplina( disciplina );

            v.setCampus( *it_campus );

            it_v = vHash.find( v );
            if( it_v != vHash.end() )
            {
               row.insert( it_v->second, 1.0 );
            }
         }

         if ( row.getnnz() != 0 )
         {
            cHash[ c ] = lp->getNumRows();

            lp->addRow( row );
            restricoes++;
         }
      }
   }

   return restricoes;
}

/*====================================================================/
%DocBegin TRIEDA_LOAD_MODEL

%Constraint 
Abertura de turmas de um mesmo bloco curricular
%Desc 

%MatExp

\begin{eqnarray}
\sum\limits_{d \in D_{b}} \sum\limits_{u \in U} \sum\limits_{tps \in SCAP_{u}} \sum\limits_{i \in I_{d}} o_{i,d,u,tps,t}  \leq M \cdot w_{bc,t,cp} \nonumber \qquad 
\forall bc \in B \quad
\forall cp \in CP \quad
\forall t \in T
\end{eqnarray}

%Data M 
%Desc
big $M$.

%DocEnd
/====================================================================*/

int SolverMIP::cria_restricao_turmas_bloco( int campusId )
{	
   int restricoes = 0;
   char name[ 100 ];
   int nnz;

   Constraint c;
   Variable v;
   VariableHash::iterator it_v;

   Disciplina * disciplina = NULL;
   Disciplina * disciplina_equivalente = NULL;

   Curso * curso = NULL;
   Curriculo * curriculo = NULL;

   ITERA_GGROUP_LESSPTR( itCampus, problemData->campi, Campus )
   {
	   if ( itCampus->getId() != campusId )
	   {
		   continue;
	   }

      GGroup< int >::iterator itDiasLetCampus =
         itCampus->diasLetivos.begin();

      for(; itDiasLetCampus != itCampus->diasLetivos.end();
         itDiasLetCampus++ )
      {
         ITERA_GGROUP_LESSPTR( itBloco, problemData->blocos, BlocoCurricular )
         {
			if ( itBloco->campus->getId() != campusId )
			{
				continue;
			}

            c.reset();
            c.setType( Constraint::C_TURMAS_BLOCO );

            c.setBloco( *itBloco );
            c.setDia( *itDiasLetCampus );
            c.setCampus( *itCampus );

            sprintf( name, "%s", c.toString().c_str() ); 
            if ( cHash.find( c ) != cHash.end() )
            {
               continue;
            }

            // FIX-ME
            nnz = 100;

            OPT_ROW row( ( nnz + 1 ), OPT_ROW::LESS , 0.0, name );

            ITERA_GGROUP_LESSPTR( it_disciplina, itBloco->disciplinas, Disciplina )
            {
                disciplina = ( *it_disciplina );

				#pragma region Equivalencias
				if ( problemData->mapDiscSubstituidaPor.find( disciplina ) !=
					 problemData->mapDiscSubstituidaPor.end() )
				{
					continue;
				}
				#pragma endregion	

               ITERA_GGROUP_LESSPTR( itUnidade, itCampus->unidades, Unidade )
               {
                  ITERA_GGROUP_LESSPTR( itCjtSala, itUnidade->conjutoSalas, ConjuntoSala )
                  {
                     for ( int turma = 0; turma < disciplina->getNumTurmas(); turma++ )
                     {
                        v.reset();
                        v.setType( Variable::V_OFERECIMENTO );

                        v.setTurma( turma );
                        v.setDisciplina( disciplina );
                        v.setUnidade( *itUnidade );
                        v.setSubCjtSala( *itCjtSala );
                        v.setDia( *itDiasLetCampus );

                        it_v = vHash.find( v );
                        if ( it_v != vHash.end() )
                        {
                           row.insert( it_v->second, 1.0 );
                        }
                     }
                  }
               }
            }

            v.reset();
            v.setType( Variable::V_N_SUBBLOCOS );

            v.setBloco( *itBloco );
            v.setDia( *itDiasLetCampus );
            v.setCampus( *itCampus );

            it_v = vHash.find( v );
            if( it_v != vHash.end() )
            {
               row.insert( it_v->second, -9999.0 );
            }
            // Provavelmente esta restri��o � in�til

            if ( row.getnnz() != 0 )
            {
               cHash[ c ] = lp->getNumRows();

               lp->addRow( row );
               restricoes++;
            }
         }
      }
   }

   return restricoes;
}

/*====================================================================/
%DocBegin TRIEDA_LOAD_MODEL

%Constraint 
Turmas de disciplinas de mesmo bloco n�o devem exceder m�ximo de cr�ditos por dia
%Desc 

%MatExp

\begin{eqnarray}
\sum\limits_{d \in D_{bc}} \sum\limits_{u \in U} \sum\limits_{tps \in SCAP_{u}} \sum\limits_{i \in I_{d}} q_{i,d,oft,u,tps,t} 
			\leq \sum\limits_{k} ( Nh_{bc,t,sl,k} \cdot cbc_{bc,t,k} )  \nonumber \qquad 
\forall bc \in B \quad
\forall cp \in CP
\forall t \in T \quad
\end{eqnarray}

%Data H_{t}
%Desc
m�ximo de cr�ditos permitidos por dia $t$.

%DocEnd
/====================================================================*/

int SolverMIP::cria_restricao_max_cred_disc_bloco( int campusId )
{
   int restricoes = 0;
   char name[ 100 ];
   int nnz;

   Constraint c;
   Variable v;
   VariableHash::iterator it_v;

   Disciplina * disciplina = NULL;
   Disciplina * disciplina_equivalente = NULL;

   Curso * curso = NULL;
   Curriculo * curriculo = NULL;
   
   ITERA_GGROUP_LESSPTR( itCampus, problemData->campi, Campus )
   {
	   if ( itCampus->getId() != campusId )
	   {
		   continue;
	   }

      GGroup< int >::iterator itDiasLetCampus = itCampus->diasLetivos.begin();

      ITERA_GGROUP_N_PT( itDiasLetCampus, itCampus->diasLetivos, int )
      {
		 int dia = *itDiasLetCampus;

         ITERA_GGROUP_LESSPTR( itBloco, problemData->blocos, BlocoCurricular )
         {
			BlocoCurricular *bc = *itBloco;

			if ( bc->campus->getId() != campusId )
			{
				continue;
			}

			int periodo = bc->getPeriodo();
			
			Oferta* oft = bc->oferta;

			GGroup< Calendario*, LessPtr<Calendario> > calendarios = itBloco->curriculo->retornaSemanasLetivasNoPeriodo( periodo );

			ITERA_GGROUP_LESSPTR( itSL, calendarios, Calendario )
			{
				Calendario *sl = *itSL;

				c.reset();
				c.setType( Constraint::C_MAX_CRED_DISC_BLOCO );

				c.setBloco( bc );
				c.setDia( dia );
				c.setCampus( *itCampus );
				c.setSemanaLetiva( sl );

				sprintf( name, "%s", c.toString().c_str() ); 
				if ( cHash.find( c ) != cHash.end() )
				{
				   continue;
				}

				nnz =  100; /*FIX-ME*/

				OPT_ROW row( nnz + 1, OPT_ROW::LESS , 0.0, name );
			
				// Variavel responsavel por armazenar o maximo de
				// creditos disponiveis  dentre todas as salas para um dado dia.
				//int maxCredsSalaDia = 0;
				//int maxCredsProfDia = 0;
				ITERA_GGROUP_LESSPTR( it_disciplina, itBloco->disciplinas, Disciplina )
				{
				    disciplina = ( *it_disciplina );

					#pragma region Equivalencias
					if ( problemData->mapDiscSubstituidaPor.find( disciplina ) !=
						 problemData->mapDiscSubstituidaPor.end() )
					{
						continue;
					}
					#pragma endregion	

				   // S� considera disciplinas da semana letiva corrente
				   if ( disciplina->getCalendario() != *itSL )
				   {
					   continue;
				   }

				   ITERA_GGROUP_LESSPTR( itUnidade, itCampus->unidades, Unidade )
				   {
					  ITERA_GGROUP_LESSPTR( itCjtSala, itUnidade->conjutoSalas, ConjuntoSala )
					  {
						 if ( itCjtSala->disciplinas_associadas.find( disciplina ) == itCjtSala->disciplinas_associadas.end() )
							continue;

						 for ( int turma = 0; turma < disciplina->getNumTurmas(); turma++ )
						 {
							v.reset();
							v.setType( Variable::V_CREDITOS_OFT );

							v.setTurma( turma );
							v.setDisciplina( disciplina );
							v.setUnidade( *itUnidade );
							v.setSubCjtSala( *itCjtSala );
							v.setDia( *itDiasLetCampus );
							v.setOferta( oft );

							it_v = vHash.find( v );
							if ( it_v != vHash.end() )
							{
							   row.insert( it_v->second, 1.0 );
							}
						 }
					  }
				   }
				}

				std::map< Trio<int, int, Calendario*>, int >::iterator it_map = bc->combinaCredSL.begin();
				for ( ; it_map != bc->combinaCredSL.end(); it_map++  )
				{
					if ( it_map->first.first == dia && it_map->first.third == sl )
					{
						int k = it_map->first.second;
						
						v.reset();
						v.setType( Variable::V_COMBINA_SL_BLOCO );
						v.setBloco( bc );
						v.setDia( dia );
						v.setCombinaSLBloco( k );

						it_v = vHash.find( v );
						if( it_v != vHash.end() )
						{
							int coef = bc->getNroMaxCredCombinaSL( k, sl, dia );

							row.insert( it_v->second, -coef );
						}
					}
				}

				if ( row.getnnz() != 0 )
				{
				   cHash[ c ] = lp->getNumRows();

				   lp->addRow( row );
				   restricoes++;
				}
			}
         }
      }
   }

   return restricoes;
}




/*====================================================================/
%DocBegin TRIEDA_LOAD_MODEL

%Constraint 
Contabiliza se h� turmas do mesmo bloco curricular abertas no mesmo 
dia em unidades distintas
%Desc 

%MatExp

\begin{eqnarray}
\sum\limits_{cp \in CP} r_{bc,t,cp} - 1  \leq v_{bc,t} \nonumber \qquad 
\forall bc \in B \quad
\forall t \in T
\end{eqnarray}

%DocEnd
/====================================================================*/

int SolverMIP::cria_restricao_num_tur_bloc_dia_difunid( int campusId )
{
   int restricoes = 0;
   char name[ 100 ];
   int nnz;

   Variable v;
   Constraint c;
   VariableHash::iterator it_v;

   ITERA_GGROUP_LESSPTR( it_bloco, problemData->blocos, BlocoCurricular )
   {
	   if ( it_bloco->campus->getId() != campusId )
	   {
		   continue;
	   }

      GGroup< int >::iterator itDiasLetBloco =
         it_bloco->diasLetivos.begin();

      for (; itDiasLetBloco != it_bloco->diasLetivos.end(); itDiasLetBloco++ )
      {
         c.reset();
         c.setType( Constraint::C_NUM_TUR_BLOC_DIA_DIFUNID );

         c.setBloco( *it_bloco );
         c.setDia( *itDiasLetBloco );

         sprintf( name, "%s", c.toString().c_str() ); 
         cHash[ c ] = lp->getNumRows();
         nnz = problemData->campi.size();

         OPT_ROW row( nnz + 1, OPT_ROW::LESS , 1.0 , name );

         ITERA_GGROUP_LESSPTR( it_cp, problemData->campi, Campus )
         {
            v.reset();
            v.setType( Variable::V_ABERTURA_SUBBLOCO_DE_BLC_DIA_CAMPUS );

            v.setBloco( *it_bloco );
            v.setDia( *itDiasLetBloco );
            v.setCampus( *it_cp );

            it_v = vHash.find( v );
            if ( it_v != vHash.end() )
            {
               row.insert( it_v->second, 1.0 );
            }
         }

         v.reset();
         v.setType( Variable::V_N_ABERT_TURMA_BLOCO );

         v.setBloco( *it_bloco );
         v.setDia( *itDiasLetBloco );

         it_v = vHash.find( v );
         if ( it_v != vHash.end() )
         {
            row.insert( it_v->second, -1.0 );
         }

         if ( row.getnnz() != 0 )
         {
            cHash[ c ] = lp->getNumRows();

            lp->addRow( row );
            restricoes++;
         }
      }
   }

   return restricoes;
}

/*====================================================================/
%DocBegin TRIEDA_LOAD_MODEL

%Constraint
Limite de cr�ditos di�rios de disciplina (*)
%Desc 

%MatExp

\begin{eqnarray}
x_{i,d,u,tps,t}  \leq \overline{H_{d}}  \nonumber \qquad 
\forall d \in D \quad
\forall u \in U \quad
\forall tps \in SCAP_{u} \quad
\forall t \in T \quad
\forall i \in I_{d}
\end{eqnarray}

%Data \overline{H_{d}}
%Desc
m�ximo de cr�ditos di�rios da disciplina $d$.

%DocEnd
/====================================================================*/

// TRIEDA-406 Cont-Limite de cr�ditos di�rios de disciplina
int SolverMIP::cria_restricao_lim_cred_diar_disc( int campusId )
{
   int restricoes = 0;

   char name[ 100 ];
   int nnz;

   Constraint c;
   Variable v;
   VariableHash::iterator it_v;

   Disciplina * disciplina = NULL;
   Disciplina * disciplina_equivalente = NULL;

   Curso * curso = NULL;
   Curriculo * curriculo = NULL;

   ITERA_GGROUP_LESSPTR( itCampus, problemData->campi, Campus )
   {
	   if ( itCampus->getId() != campusId )
	   {
		   continue;
	   }

      ITERA_GGROUP_LESSPTR( itUnidade, itCampus->unidades, Unidade )
      {
         ITERA_GGROUP_LESSPTR( itCjtSala, itUnidade->conjutoSalas, ConjuntoSala )
         {
			 ITERA_GGROUP_LESSPTR( it_disciplina, itCjtSala->disciplinas_associadas, Disciplina )
             {
                disciplina = ( *it_disciplina );

				#pragma region Equivalencias
				if ( problemData->mapDiscSubstituidaPor.find( disciplina ) !=
					 problemData->mapDiscSubstituidaPor.end() )
				{
					continue;
				}
				#pragma endregion	

               GGroup< int /*Dias*/ > disc_sala_dias
                  = problemData->disc_Conjutno_Salas__Dias
                  [ std::make_pair< int, int > ( disciplina->getId(), itCjtSala->getId() ) ];

               ITERA_GGROUP_N_PT( itDiscSala_Dias, disc_sala_dias, int )
               {
                  for ( int turma = 0; turma < disciplina->getNumTurmas(); turma++ )
                  {
                     c.reset();
                     c.setType( Constraint::C_LIM_CRED_DIAR_DISC );

                     c.setTurma( turma );
                     c.setDisciplina( disciplina );
                     c.setUnidade( *itUnidade );
                     c.setSubCjtSala( *itCjtSala );
                     c.setDia( *itDiscSala_Dias );

                     sprintf( name, "%s", c.toString().c_str() );
                     if ( cHash.find( c ) != cHash.end() )
                     {
                        continue;
                     }

                     nnz = 1;

                     // 1.2.13
                     // Limite de cr�ditos di�rios de disciplina (*)
                     int maximo_creditos = itCjtSala->maxCredsDiaPorSL( *itDiscSala_Dias, disciplina->getCalendario() );
                     int maximo_creditos_fixacao
                        = problemData->creditosFixadosDisciplinaDia( disciplina, *itDiscSala_Dias, *itCjtSala );

                     // Quando existir uma fixa��o, o m�ximo de cr�ditos dever�
                     // ser exatamente a fixa��o. Caso contr�rio, continua sendo
                     // o m�ximo de cr�ditos por dia do conjunto de salas
                     OPT_ROW::ROWSENSE row_sense = OPT_ROW::LESS;
                     if ( maximo_creditos_fixacao != 0 )
                     {
                        maximo_creditos = maximo_creditos_fixacao;
                        row_sense = OPT_ROW::EQUAL;
                     }

                     OPT_ROW row( 1, row_sense , maximo_creditos , name );

                     v.reset();
                     v.setType( Variable::V_CREDITOS );

                     v.setTurma( turma );
                     v.setDisciplina( disciplina );
                     v.setUnidade( *itUnidade );
                     v.setSubCjtSala( *itCjtSala );
                     v.setDia( *itDiscSala_Dias );

                     it_v = vHash.find( v );
                     if( it_v != vHash.end() )
                     {
                        row.insert( it_v->second, 1.0 );
                     }

                     if ( row.getnnz() != 0 )
                     {
                        cHash[ c ] = lp->getNumRows();

                        lp->addRow( row );
                        restricoes++;
                     }
                  }
               }
            }
         }
      }
   }

   return restricoes;
}

/*====================================================================/
%DocBegin TRIEDA_LOAD_MODEL

%Constraint 
Capacidade alocada tem que permitir atender demanda da disciplina
%Desc 

%MatExp

\begin{eqnarray}
\sum\limits_{i \in I_{d}} a_{i,d,oft} + fd_{d,oft} =  P_{d,oft}  \nonumber \qquad 
\forall oft \in O
\forall d \in D_{oft} \quad
\end{eqnarray}

%Data P_{d,oft}
%Desc
demanda da disciplina $d$ da oferta $oft$.

%DocEnd
/====================================================================*/

int SolverMIP::cria_restricao_cap_aloc_dem_disc( int campusId )
{
   int restricoes = 0;
   char name[ 100 ];
   int nnz = 0;

   Constraint c;
   Variable v;
   VariableHash::iterator it_v;

   Disciplina * disciplina = NULL;

   ITERA_GGROUP_LESSPTR( itOferta, problemData->ofertas, Oferta )
   {
	   if ( itOferta->getCampusId() != campusId )
	   {
		   continue;
	   }

       map < Disciplina*, int, LessPtr< Disciplina > >::iterator itPrdDisc = 
         itOferta->curriculo->disciplinas_periodo.begin();

      for (; itPrdDisc != itOferta->curriculo->disciplinas_periodo.end(); itPrdDisc++ )
      {
		  disciplina = itPrdDisc->first;

		  #pragma region Equivalencias
		  if ( problemData->mapDiscSubstituidaPor.find( disciplina ) !=
			   problemData->mapDiscSubstituidaPor.end() )
		  {
			  continue;
		  }
		  #pragma endregion	

         c.reset();
         c.setType( Constraint::C_CAP_ALOC_DEM_DISC );

         c.setOferta( *itOferta );
         c.setDisciplina( disciplina );

         sprintf( name, "%s", c.toString().c_str() ); 
         if ( cHash.find( c ) != cHash.end() )
         {
            continue;
         }

         if ( disciplina->getNumTurmas() <= 0 )
         {
            continue;
         }

         nnz = disciplina->getNumTurmas();
         int rhs = 0;

         // Calculando P_{d,o}
         ITERA_GGROUP_LESSPTR( itDem, problemData->demandas, Demanda )
         {
            if ( itDem->disciplina->getId() == disciplina->getId()
               && itDem->getOfertaId() == itOferta->getId() )
            {
               rhs += itDem->getQuantidade();
            }
         }

         OPT_ROW row( nnz , OPT_ROW::EQUAL, rhs , name );

         for ( int turma = 0; turma < disciplina->getNumTurmas(); turma++ )
         {
            v.reset();
            v.setType( Variable::V_ALUNOS );

            v.setTurma( turma );
            v.setDisciplina( disciplina );
            v.setOferta( *itOferta );

            it_v = vHash.find( v );
            if ( it_v != vHash.end() )
            {
               row.insert( it_v->second, 1.0 );
            }
         }

         v.reset();
         v.setType( Variable::V_SLACK_DEMANDA );
         v.setDisciplina( disciplina );
         v.setOferta( *itOferta );

         it_v = vHash.find( v );
         if( it_v != vHash.end() )
         {
            row.insert( it_v->second, 1.0 );
         }

         if ( row.getnnz() != 0 )
         {
            cHash[ c ] = lp->getNumRows();

            lp->addRow( row );
            restricoes++;
         }
      }
   }

   return restricoes;
}

/*====================================================================/
%DocBegin TRIEDA_LOAD_MODEL

%Constraint 
Se alocar um conjunto de salas para uma turma, tem que respeitar 
capacidade total das salas pertencentes ao conjunto
%Desc 

%MatExp

\begin{eqnarray}
\sum\limits_{c \in C} \sum\limits_{oft \in O_{d}} a_{i,d,oft}  \leq \sum\limits_{u \in U} \sum\limits_{tps \in SCAP_{u}} \sum\limits_{t \in T} A_{u,tps} \cdot o_{i,d,u,tps,t} 
\nonumber \qquad 
\forall d \in D \quad
\forall i \in I_{d} \quad
\forall cp \in CP
\end{eqnarray}

%Data A_{u,s}
%Desc
capacidade da sala $s$ da unidade $u$.

%DocEnd
/====================================================================*/

// TRIEDA-390
int SolverMIP::cria_restricao_cap_sala_compativel_turma( int campusId )
{
   int restricoes = 0;
   char name[ 100 ];
   int nnz;

   Constraint c;
   Variable v;
   VariableHash::iterator it_v;

   Disciplina * disciplina = NULL;

   ITERA_GGROUP_LESSPTR( itCampus, problemData->campi, Campus )
   {
	   if ( itCampus->getId() != campusId )
	   {
		    continue;
	   }

      ITERA_GGROUP_LESSPTR( it_disciplina, problemData->disciplinas, Disciplina )
      {
         disciplina = ( *it_disciplina );

		 #pragma region Equivalencias
		 if ( problemData->mapDiscSubstituidaPor.find( disciplina ) !=
				problemData->mapDiscSubstituidaPor.end() )
		 {
			continue;
		 }
		 #pragma endregion	

		 // A disciplina deve ser ofertada no campus especificado
		 if ( problemData->cp_discs[campusId].find( disciplina->getId() ) ==
			  problemData->cp_discs[campusId].end() )
		 {
			 continue;
		 }

         for ( int turma = 0; turma < disciplina->getNumTurmas(); turma++ )
         {
            c.reset();
            c.setType( Constraint::C_CAP_SALA_COMPATIVEL_TURMA );

            c.setCampus( *itCampus );
            c.setDisciplina( disciplina );
            c.setTurma( turma );

            sprintf( name, "%s", c.toString().c_str() ); 
            if ( cHash.find( c ) != cHash.end() )
            {
               continue;
            }

            nnz = problemData->ofertas.size() * problemData->cursos.size() +
               itCampus->getTotalSalas() * 7;

            OPT_ROW row( nnz , OPT_ROW::LESS , 0.0 , name );

            GGroup< Oferta *, LessPtr< Oferta > >::iterator itOferta = 
               problemData->ofertasDisc[ disciplina->getId() ].begin();

            for (; itOferta != problemData->ofertasDisc[ disciplina->getId() ].end();
                   itOferta++ )
            {
               v.reset();
               v.setType( Variable::V_ALUNOS );

               v.setTurma( turma );
               v.setDisciplina( disciplina );
               v.setOferta( *itOferta );

               it_v = vHash.find( v );
               if ( it_v != vHash.end() )
               {
                  row.insert( it_v->second, 1.0 );
               }
            }

            ITERA_GGROUP_LESSPTR( itUnidade, itCampus->unidades, Unidade )
            {
               ITERA_GGROUP_LESSPTR( itCjtSala, itUnidade->conjutoSalas, ConjuntoSala )
               {
                  GGroup< int /*Dias*/ >::iterator itDiscSala_Dias =
                     problemData->disc_Conjutno_Salas__Dias
                     [ std::make_pair< int, int >( disciplina->getId(), itCjtSala->getId() ) ].begin();

                  for (; itDiscSala_Dias != problemData->disc_Conjutno_Salas__Dias
                     [ std::make_pair< int, int >( disciplina->getId(), itCjtSala->getId() ) ].end();
                     itDiscSala_Dias++ )
                  { 
                     v.reset();
                     v.setType( Variable::V_OFERECIMENTO );

                     v.setTurma( turma );
                     v.setDisciplina( disciplina );
                     v.setUnidade( *itUnidade );
                     v.setSubCjtSala( *itCjtSala );
                     v.setDia( *itDiscSala_Dias );

                     it_v = vHash.find( v );
                     if ( it_v != vHash.end() )
                     { 
                        int tmp = ( itCjtSala->getCapacidadeRepr() > 0 ?
                           itCjtSala->getCapacidadeRepr() :
                        ( -itCjtSala->getCapacidadeRepr() ) );

                        row.insert( it_v->second, -tmp );
                     }
                  }
               }
            }

            if ( row.getnnz() != 0 )
            {
               cHash[ c ] = lp->getNumRows();

               lp->addRow( row );
               restricoes++;
            }
         }
      }
   }

   //ITERA_GGROUP(itCampus,problemData->campi,Campus)
   //{
   //   ITERA_GGROUP(itDisc,problemData->disciplinas,Disciplina)
   //   {
   //      for(int turma = 0; turma < itDisc->num_turmas; turma++)
   //      {
   //         c.reset();
   //         c.setType(Constraint::C_CAP_SALA_COMPATIVEL_TURMA);
   //         c.setCampus(*itCampus);
   //         c.setDisciplina(*itDisc);
   //         c.setTurma(turma);

   //         // >>>
   //         std::string auxCons = c.toString().c_str();
   //         auxCons += "_";
   //         auxCons += lp->getNumRows();

   //         sprintf( name, "%s", auxCons.c_str() ); 

   //         //if (cHash.find(c) != c.toString().end()) continue;
   //         if (cHash.find(c) != cHash.end()) continue;
   //         // <<<

   //         //cHash[ c ] = lp->getNumRows();

   //         nnz = problemData->ofertas.size() * problemData->cursos.size() +
   //            itCampus->totalSalas * 7;

   //         OPT_ROW row( nnz , OPT_ROW::LESS , 0.0 , name );

   //         GGroup<Oferta*>::iterator itOferta = 
   //            problemData->ofertasDisc[itDisc->getId()].begin();

   //         for(; itOferta != problemData->ofertasDisc[itDisc->getId()].end(); itOferta++)
   //         {
   //            v.reset();
   //            v.setType(Variable::V_ALUNOS);
   //            v.setTurma(turma);
   //            v.setDisciplina(*itDisc);
   //            v.setOferta(*itOferta);

   //            it_v = vHash.find(v);
   //            if( it_v != vHash.end() )
   //            { row.insert(it_v->second, 1.0); }
   //         }

   //         ITERA_GGROUP(itUnidade,itCampus->unidades,Unidade)
   //         {
   //            ITERA_GGROUP(itCjtSala,itUnidade->conjutoSalas,ConjuntoSala)
   //            {
   //               for(int dia = 0 ; dia < 7; dia++)
   //               {
   //                  v.reset();
   //                  v.setType(Variable::V_OFERECIMENTO);
   //                  v.setTurma(turma);
   //                  v.setDisciplina(*itDisc);
   //                  v.setUnidade(*itUnidade);
   //                  v.setSubCjtSala(*itCjtSala);
   //                  v.setDia(dia);

   //                  it_v = vHash.find(v);
   //                  if( it_v != vHash.end() )
   //                  { row.insert(it_v->second, -itCjtSala->capTotalSalas()); }
   //               }
   //            }
   //         }

   //         if(row.getnnz() != 0)
   //         {
   //            cHash[ c ] = lp->getNumRows();

   //            lp->addRow(row);
   //            restricoes++;
   //         }

   //         //lp->addRow(row);
   //         //++restricoes;
   //      }
   //   }
   //}

   //ITERA_GGROUP(it_campus,problemData->campi,Campus) {
   //   ITERA_GGROUP(it_disc,problemData->disciplinas,Disciplina) {
   //      for(int i=0;i<it_disc->num_turmas;++i) {

   //         c.reset();
   //         c.setType(Constraint::C_CAP_SALA_COMPATIVEL_TURMA);
   //         c.setCampus(*it_campus);
   //         c.setDisciplina(*it_disc);
   //         c.setTurma(i);

   //         sprintf( name, "%s", c.toString().c_str() ); 

   //         if (cHash.find(c) != cHash.end()) continue;

   //         cHash[ c ] = lp->getNumRows();

   //         nnz = problemData->cursos.size() +
   //            it_campus->totalSalas * 7;

   //         OPT_ROW row( nnz , OPT_ROW::LESS , 0.0 , name );

   //         ITERA_GGROUP(it_curso,problemData->cursos,Curso) {
   //            v.reset();
   //            v.setType(Variable::V_ALUNOS);
   //            v.setTurma(i);
   //            v.setDisciplina(*it_disc);
   //            v.setCampus(*it_campus);
   //            v.setCurso(*it_curso);

   //            it_v = vHash.find(v);
   //            if( it_v != vHash.end() )
   //            {
   //               row.insert(it_v->second, 1.0);
   //            }
   //         }

   //         ITERA_GGROUP(it_u,it_campus->unidades,Unidade) {
   //            ITERA_GGROUP(it_sala,it_u->salas,Sala) {
   //               for(int t=0;t<7;t++) {
   //                  v.reset();
   //                  v.setType(Variable::V_OFERECIMENTO);
   //                  v.setTurma(i);
   //                  v.setDisciplina(*it_disc);
   //                  v.setUnidade(*it_u);
   //                  v.setSala(*it_sala);
   //                  v.setDia(t);

   //                  it_v = vHash.find(v);
   //                  if( it_v != vHash.end() )
   //                  {
   //                     row.insert(it_v->second, -it_sala->capacidade);
   //                  }
   //               }
   //            }
   //         }
   //         lp->addRow(row);
   //         ++restricoes;
   //      }
   //   }
   //}

   return restricoes;
}

/*====================================================================/
%DocBegin TRIEDA_LOAD_MODEL

%Constraint 
Capacidade total de um conjunto de salas de uma unidade
%Desc

%MatExp

\begin{eqnarray}
\sum\limits_{c \in C} \sum\limits_{oft \in O_{d}} a_{i,d,oft} \leq A_{u,tps} + M \cdot (1-o_{i,d,u,tps,t}) \nonumber \qquad
\forall d \in D \quad
\forall i \in I_{d} \quad
\forall u \in U \quad
\forall tps \in SCAP_{u} \quad
\forall cp \in CP \quad
\forall t \in T
\end{eqnarray}

%Data A_{u,s}
%Desc
capacidade da sala $s$ da unidade $u$.

%Data M
%Desc
big $M$.

%DocEnd
/====================================================================*/

// TRIEDA-391 - Capacidade da sala na unidade
int SolverMIP::cria_restricao_cap_sala_unidade( int campusId )
{
   int restricoes = 0;

   char name[ 100 ];
   int nnz;

   Constraint c;
   Variable v;
   VariableHash::iterator it_v;

   Disciplina * disciplina = NULL;
   Disciplina * disciplina_equivalente = NULL;

   Curso * curso = NULL;
   Curriculo * curriculo = NULL;

   ITERA_GGROUP_LESSPTR( itCampus, problemData->campi, Campus )
   {
	   if ( itCampus->getId() != campusId )
	   {
		   continue;
	   }

      ITERA_GGROUP_LESSPTR( itUnidade, itCampus->unidades, Unidade)
      {
         ITERA_GGROUP_LESSPTR( itCjtSala, itUnidade->conjutoSalas, ConjuntoSala )
         {
			ITERA_GGROUP_LESSPTR( it_disciplina, itCjtSala->disciplinas_associadas, Disciplina )
            {
               disciplina = ( *it_disciplina );

				#pragma region Equivalencias
				if ( problemData->mapDiscSubstituidaPor.find( disciplina ) !=
					problemData->mapDiscSubstituidaPor.end() )
				{
					continue;
				}
				#pragma endregion	
				
               for ( int turma = 0; turma < disciplina->getNumTurmas(); turma++ )
               {
                  GGroup< int /*Dias*/ > disc_sala_dias =                     
                     problemData->disc_Conjutno_Salas__Dias[ std::make_pair< int, int >
                     ( disciplina->getId(), itCjtSala->getId() ) ];

                  ITERA_GGROUP_N_PT( itDiscSala_Dias, disc_sala_dias, int )
                  {
                     c.reset();
                     c.setType( Constraint::C_CAP_SALA_UNIDADE );

                     c.setUnidade( *itUnidade );
                     c.setSubCjtSala( *itCjtSala );
                     c.setDisciplina( disciplina );
                     c.setTurma( turma );
                     c.setCampus( *itCampus );
                     c.setDia( *itDiscSala_Dias );

                     sprintf( name, "%s", c.toString().c_str() ); 
                     if ( cHash.find( c ) != cHash.end() )
                     {
                        continue;
                     }

                     nnz = 200;

                     int tmp = ( itCjtSala->getCapacidadeRepr() > 0 ?
                        itCjtSala->getCapacidadeRepr() : ( -itCjtSala->getCapacidadeRepr() ) );

                     // itUnidade->maiorSala;
                     int rhs = tmp + 1000; 
                     OPT_ROW row( nnz, OPT_ROW::LESS , rhs , name );

                     // ---

                     v.reset();
                     v.setType( Variable::V_OFERECIMENTO );
                     v.setTurma( turma );
                     v.setDisciplina( disciplina );
                     v.setUnidade( *itUnidade );
                     v.setSubCjtSala( *itCjtSala );
                     v.setDia( *itDiscSala_Dias );

                     it_v = vHash.find( v );
                     if( it_v != vHash.end() )
                     { 
                        row.insert( it_v->second, 1000 );
                     }
					 else
					 {
						continue;
					 }

                     // ---

                     GGroup< Oferta *, LessPtr< Oferta > > group_ofertas
                        = problemData->ofertasDisc[ disciplina->getId() ];
                     ITERA_GGROUP_LESSPTR( itOferta, group_ofertas, Oferta )
                     {
						 if ( itOferta->getCampusId() != campusId )
						 {
							 continue;
						 }

                        v.reset();
                        v.setType( Variable::V_ALUNOS );

                        v.setTurma( turma );
                        v.setDisciplina( disciplina );
                        v.setOferta( *itOferta );

                        it_v = vHash.find( v );
                        if( it_v != vHash.end() )
                        { 
                           row.insert( it_v->second, 1.0 );
                        }
                     }

                     // ---

                     if ( row.getnnz() != 0 )
                     {
                        cHash[ c ] = lp->getNumRows();

                        lp->addRow( row );
                        restricoes++;
                     }
                  }
               }
            }
         }
      }
   }

   return restricoes;
}

/*====================================================================/
%DocBegin TRIEDA_LOAD_MODEL

%Constraint 
Contabiliza se h� turmas da mesma disciplina em dias consecutivos (*)
%Desc 

%MatExp

\begin{eqnarray}
c_{i,d,t}  \geq \sum\limits_{u \in U} \sum\limits_{tps \in SCAP_{u}}(o_{i,d,u,tps,t} - o_{i,d,u,tps,t-1}) - 1  \nonumber \qquad 
\forall d \in D \quad
\forall i \in I_{d} \quad
\forall (t \geq 2) \in T
\end{eqnarray}

%DocEnd
/====================================================================*/

// TRIEDA-392
int SolverMIP::cria_restricao_turma_disc_dias_consec( int campusId )
{
   int restricoes = 0;

   char name[ 100 ];
   int nnz;

   Constraint c;
   Variable v;
   VariableHash::iterator it_v;

   Disciplina * disciplina = NULL;
   Disciplina * disciplina_equivalente = NULL;

   Curso * curso = NULL;
   Curriculo * curriculo = NULL;

   ITERA_GGROUP_LESSPTR( it_disciplina,
      problemData->disciplinas, Disciplina )
   {
      disciplina = ( *it_disciplina );

	  #pragma region Equivalencias
	  if ( problemData->mapDiscSubstituidaPor.find( disciplina ) !=
		   problemData->mapDiscSubstituidaPor.end() )
	  {
		  continue;
	  }
	  #pragma endregion	

	  // A disciplina deve ser ofertada no campus especificado
	  if ( problemData->cp_discs[campusId].find( disciplina->getId() ) ==
		   problemData->cp_discs[campusId].end() )
	  {
		  continue;
	  }

      for ( int turma = 0; turma < disciplina->getNumTurmas(); turma++ )
      {
         GGroup< int >::iterator itDiasLetDisc =
            disciplina->diasLetivos.begin();

         for (; itDiasLetDisc != disciplina->diasLetivos.end(); itDiasLetDisc++ )
         {
            c.reset();
            c.setType( Constraint::C_TURMA_DISC_DIAS_CONSEC );

            c.setDisciplina( disciplina );
            c.setTurma( turma );
            c.setDia( *itDiasLetDisc );

            sprintf( name, "%s", c.toString().c_str() ); 

            if ( cHash.find( c ) != cHash.end() )
            {
               continue;
            }

            nnz = ( problemData->totalSalas * 2 + 1 );
            OPT_ROW row( nnz, OPT_ROW::GREATER , -1 , name );

            v.reset();
            v.setType( Variable::V_DIAS_CONSECUTIVOS );
            v.setTurma( turma );
            v.setDisciplina( disciplina );
            v.setDia( *itDiasLetDisc );

            it_v = vHash.find( v );
            if ( it_v != vHash.end() )
            {
               row.insert( it_v->second, 1.0 );
            }

            ITERA_GGROUP_LESSPTR( itCampus, problemData->campi, Campus )
            {
			   if ( itCampus->getId() != campusId )
			   {
				   continue;
			   }

               ITERA_GGROUP_LESSPTR( itUnidade, itCampus->unidades, Unidade )
               {
                  ITERA_GGROUP_LESSPTR( itCjtSala, itUnidade->conjutoSalas, ConjuntoSala )
                  {
                     v.reset();
                     v.setType( Variable::V_OFERECIMENTO );

                     v.setTurma( turma );
                     v.setDisciplina( disciplina );
                     v.setUnidade( *itUnidade );
                     v.setSubCjtSala( *itCjtSala );
                     v.setDia( *itDiasLetDisc );

                     it_v = vHash.find( v );
                     if ( it_v != vHash.end() )
                     {
                        row.insert( it_v->second, -1.0 );
                     }

                     v.setDia( ( *itDiasLetDisc ) - 1 );

                     it_v = vHash.find( v );
                     if ( it_v != vHash.end() )
                     {
                        row.insert( it_v->second, 1.0 );
                     }
                  }
               }
            }

            if ( row.getnnz() != 0 )
            {
               cHash[ c ] = lp->getNumRows();

               lp->addRow( row );
               restricoes++;
            }
         }
      }
   }

   return restricoes;
}

/*====================================================================/
%DocBegin TRIEDA_LOAD_MODEL

%Constraint 
M�nimo de cr�ditos alocados para turmas de um bloco (*)
%Desc 

%MatExp

\begin{eqnarray}
\underline{h}_{bc,i} \leq \sum\limits_{d \in D_{bc}} \sum\limits_{u \in U} \sum\limits_{tps \in SCAP_{u}} tempo_{d} \cdot q_{i,d,oft,u,tps,t} \nonumber \qquad 
\forall bc \in B \quad
\forall i \in I_{oft} \quad
\forall t \in T
\end{eqnarray}

%DocEnd
/====================================================================*/

// TRIEDA-393
int SolverMIP::cria_restricao_min_creds_turm_bloco( int campusId )
{
   int restricoes = 0;

   char name[ 100 ];
   int nnz;

   Constraint c;
   Variable v;
   VariableHash::iterator it_v;

   Disciplina * disciplina = NULL;

   ITERA_GGROUP_LESSPTR( itBloco, problemData->blocos, BlocoCurricular )
   {
	  Oferta *oft = itBloco->oferta;

	  if ( itBloco->campus->getId() != campusId )
	  {
		  continue;
	  }

      for ( int turma = 0; turma < itBloco->getTotalTurmas(); turma++ )
      {
         GGroup< int >::iterator itDiasLetivosBlocoCurric = 
            itBloco->diasLetivos.begin();

         for (; itDiasLetivosBlocoCurric != itBloco->diasLetivos.end();
            itDiasLetivosBlocoCurric++ )
         {
            c.reset();
            c.setType( Constraint::C_MIN_CREDS_TURM_BLOCO );

            c.setBloco( *itBloco );
            c.setTurma( turma );
            c.setDia( *itDiasLetivosBlocoCurric );

            sprintf( name, "%s", c.toString().c_str() ); 

            if (cHash.find(c) != cHash.end())
            {
               continue;
            }

            nnz = ( ( itBloco->disciplinas.size() * problemData->totalConjuntosSalas ) + 1 );

            OPT_ROW row( nnz, OPT_ROW::GREATER , 0.0 , name );

            ITERA_GGROUP_LESSPTR( it_disciplina, itBloco->disciplinas, Disciplina )
            {
               disciplina = ( *it_disciplina );

				#pragma region Equivalencias
				if ( problemData->mapDiscSubstituidaPor.find( disciplina ) !=
					problemData->mapDiscSubstituidaPor.end() )
				{
					continue;
				}
				#pragma endregion	

               ITERA_GGROUP_LESSPTR( itUnidade, itBloco->campus->unidades, Unidade )
               {
                  ITERA_GGROUP_LESSPTR( itCjtSala, itUnidade->conjutoSalas, ConjuntoSala )
                  {
                     v.reset();
                     v.setType( Variable::V_CREDITOS_OFT );

                     v.setTurma( turma );
                     v.setDisciplina( disciplina );
                     v.setUnidade( *itUnidade );
                     v.setSubCjtSala( *itCjtSala );
					 v.setOferta( oft );
                     v.setDia( *itDiasLetivosBlocoCurric );

                     it_v = vHash.find( v );
                     if( it_v != vHash.end() )
                     {
						 row.insert( it_v->second, disciplina->getTempoCredSemanaLetiva() );
                     }
                  }
               }
            }

            v.reset();
            v.setType( Variable::V_MIN_CRED_SEMANA );

            v.setTurma( turma );
            v.setBloco( *itBloco );

            it_v = vHash.find( v );
            if ( it_v != vHash.end() )
            {
               row.insert( it_v->second, -1.0 );
            }

            // Para evitar a cria��o da restri��o no caso em que s� a vari�vel h seja encontrada. Isso � s� uma
            // garantia. Como os dias letivos est�o sendo respeitados, n�o devemos notar erros.
            if ( row.getnnz() > 1 )
            {
               cHash[ c ] = lp->getNumRows();

               lp->addRow( row );
               restricoes++;
            }
         }
      }
   }

   return restricoes;
}

/*====================================================================/
%DocBegin TRIEDA_LOAD_MODEL

%Constraint 
M�ximo de cr�ditos alocados para turmas de um bloco (*)
%Desc 

%MatExp

\begin{eqnarray}
\overline{h}_{bc,i} \geq \sum\limits_{d \in D_{bc}} \sum\limits_{u \in U} \sum\limits_{tps \in SCAP_{u}} tempo_{d} \cdot q_{i,d,oft,u,tps,t} \nonumber \qquad 
\forall bc \in B \quad
\forall i \in I_{oft} \quad
\forall t \in T
\end{eqnarray}

%DocEnd
/====================================================================*/

// TRIEDA-394
int SolverMIP::cria_restricao_max_creds_turm_bloco( int campusId )
{
   int restricoes = 0;

   char name[ 100 ];
   int nnz;

   Constraint c;
   Variable v;
   VariableHash::iterator it_v;

   Disciplina * disciplina = NULL;
   Disciplina * disciplina_equivalente = NULL;

   Curso * curso = NULL;
   Curriculo * curriculo = NULL;

   ITERA_GGROUP_LESSPTR( itBloco, problemData->blocos, BlocoCurricular )
   {
	  Oferta *oft = itBloco->oferta;
			   
	  if ( itBloco->campus->getId() != campusId )
	  {
		  continue;
	  }

      for ( int turma = 0; turma < itBloco->getTotalTurmas(); turma++ )
      {
         GGroup< int >::iterator itDiasLetivosBlocoCurric = 
            itBloco->diasLetivos.begin();

         for(; itDiasLetivosBlocoCurric != itBloco->diasLetivos.end();
            itDiasLetivosBlocoCurric++ )
         {
            c.reset();
            c.setType( Constraint::C_MAX_CREDS_TURM_BLOCO );

            c.setBloco( *itBloco );
            c.setTurma( turma );
            c.setDia( *itDiasLetivosBlocoCurric );

            sprintf( name, "%s", c.toString().c_str() ); 

            if (cHash.find(c) != cHash.end()) continue;

            nnz = ( ( itBloco->disciplinas.size() * problemData->totalConjuntosSalas ) + 1 );

            OPT_ROW row( nnz, OPT_ROW::LESS , 0.0 , name );

            ITERA_GGROUP_LESSPTR( it_disciplina, itBloco->disciplinas, Disciplina )
            {
               disciplina = ( *it_disciplina );

				#pragma region Equivalencias
				if ( problemData->mapDiscSubstituidaPor.find( disciplina ) !=
					problemData->mapDiscSubstituidaPor.end() )
				{
					continue;
				}
				#pragma endregion	

               ITERA_GGROUP_LESSPTR( itUnidade, itBloco->campus->unidades, Unidade )
               {
                  ITERA_GGROUP_LESSPTR( itCjtSala, itUnidade->conjutoSalas, ConjuntoSala )
                  {
                     v.reset();
                     v.setType( Variable::V_CREDITOS_OFT );

                     v.setTurma( turma );
                     v.setDisciplina( disciplina );
                     v.setUnidade( *itUnidade );
                     v.setSubCjtSala( *itCjtSala );
					 v.setOferta( oft );
                     v.setDia( *itDiasLetivosBlocoCurric );

                     it_v = vHash.find( v );
                     if ( it_v != vHash.end() )
                     {
                        row.insert( it_v->second, disciplina->getTempoCredSemanaLetiva() );
                     }
                  }
               }
            }

            v.reset();
            v.setType( Variable::V_MAX_CRED_SEMANA );

            v.setTurma( turma );
            v.setBloco( *itBloco );

            it_v = vHash.find( v );
            if ( it_v != vHash.end() )
            {
               row.insert( it_v->second, -1.0 );
            }

            // Para evitar a cria��o da restri��o no caso em que s� a
            // vari�vel h seja encontrada. Isso � s� uma garantia.
            // Como os dias letivos est�o sendo respeitados, n�o devemos notar erros.
            if ( row.getnnz() > 1 )
            {
               cHash[ c ] = lp->getNumRows();

               lp->addRow( row );
               restricoes++;
            }
         }
      }
   }

   return restricoes;
}

/*====================================================================/
%DocBegin TRIEDA_LOAD_MODEL

%Constraint 
Contabiliza se houve aluno de determinado curso alocado em uma turma (*)
%Desc 

%MatExp

\begin{eqnarray}
\sum\limits_{o \in O_{d}} a_{i,d,oft} \leq M \cdot b_{i,d,c,cp} \nonumber \qquad 
\forall cp \in CP
\forall c \in C \quad
\forall d \in D \quad
\forall i \in I_{d} \quad
\end{eqnarray}

%Data M
%Desc
big $M$.

%DocEnd
/====================================================================*/

int SolverMIP::cria_restricao_aluno_curso_disc( int campusId )
{
   int restricoes = 0;

   char name[ 100 ];
   int nnz;

   Constraint c;
   Variable v;
   VariableHash::iterator it_v;

   Disciplina * disciplina = NULL;

   ITERA_GGROUP_LESSPTR( itCampus, problemData->campi, Campus )
   {
	   if ( itCampus->getId() != campusId )
	   {
		   continue;
	   }

      ITERA_GGROUP_LESSPTR( itCurso, problemData->cursos, Curso )
      {
         ITERA_GGROUP_LESSPTR( it_disciplina, problemData->disciplinas, Disciplina )
         {
            disciplina = ( *it_disciplina );

			// A disciplina deve ser ofertada no campus especificado
			if ( problemData->cp_discs[campusId].find( disciplina->getId() ) ==
				 problemData->cp_discs[campusId].end() )
			{
				continue;
			}

			#pragma region Equivalencias
			if ( problemData->mapDiscSubstituidaPor.find( disciplina ) !=
				problemData->mapDiscSubstituidaPor.end() )
			{
				continue;
			}
			#pragma endregion	

            for ( int turma = 0; turma < disciplina->getNumTurmas(); turma++ )
            {
               c.reset();
               c.setType( Constraint::C_ALUNO_CURSO_DISC );

               c.setCampus( *itCampus );
               c.setCurso( *itCurso );
               c.setDisciplina( disciplina );
               c.setTurma( turma );

               sprintf( name, "%s", c.toString().c_str() );
               if ( cHash.find( c ) != cHash.end() )
               {
                  continue;
               }

               nnz = ( problemData->ofertasDisc[ disciplina->getId() ].size() + 1 );
               OPT_ROW row( nnz, OPT_ROW::LESS , 0.0 , name );

               GGroup< Oferta *, LessPtr< Oferta > > ofertas
                  = problemData->ofertasDisc[ disciplina->getId() ];
               ITERA_GGROUP_LESSPTR( itOfertasDisc, ofertas, Oferta )
               {
                  if ( itOfertasDisc->campus == ( *itCampus ) )
                  {
                     if ( itOfertasDisc->curso == ( *itCurso ) )
                     {
                        v.reset();
                        v.setType( Variable::V_ALUNOS );

                        v.setTurma( turma );
                        v.setDisciplina( disciplina );
                        v.setOferta( *itOfertasDisc );

                        it_v = vHash.find( v );
                        if ( it_v != vHash.end() )
                        {
                           row.insert( it_v->second, 1.0 );
                        }
                     }
                  }
               }

               v.reset();
               v.setType( Variable::V_ALOC_ALUNO );

               v.setTurma( turma );
               v.setDisciplina( disciplina );
               v.setCurso( *itCurso );
               v.setCampus( *itCampus );

               it_v = vHash.find(v);
               if( it_v != vHash.end() )
               {
                  row.insert( it_v->second, -itCampus->getMaiorSala() * 100 );
               }

               if ( row.getnnz() != 0 )
               {
                  cHash[ c ] = lp->getNumRows();

                  lp->addRow( row );
                  restricoes++;
               }
            }
         }
      }
   }

   return restricoes;
}



/*====================================================================/
%DocBegin TRIEDA_LOAD_MODEL

%Constraint 
TODO: conferir o funcionamento dessa restricao
N�o permitir que alunos de cursos diferentes incompat�veis compartilhem turmas (*)
%Desc 

%MatExp

\begin{eqnarray} 
b_{i,d,c,cp} + b_{i,d,c',cp} - bs_{i,d,c,c',cp} \leq 1 \nonumber \qquad 
\forall d \in D \quad
\forall i \in I_{d} \quad
\forall c,c' \notin CC \quad
\forall cp \in CP
\end{eqnarray}

%DocEnd
/====================================================================*/

int SolverMIP::cria_restricao_alunos_cursos_incompat( int campusId )
{
   int restricoes = 0;
   char name[ 100 ];
   int nnz;

   Constraint c;
   Variable v;
   VariableHash::iterator it_v;

	ITERA_GGROUP_LESSPTR( itCampus, problemData->campi, Campus )
	{
		if ( itCampus->getId() != campusId )
		{
			continue;
		}

		ITERA_GGROUP_LESSPTR( it1Cursos, problemData->cursos, Curso )
		{
			Curso* c1 = *it1Cursos;
			
			if ( itCampus->cursos.find( c1 ) == itCampus->cursos.end() )
			{
				continue;
			}

			ITERA_GGROUP_INIC_LESSPTR( it2Cursos, it1Cursos, problemData->cursos, Curso )
			{
				Curso* c2 = *it2Cursos;
			    
				if ( itCampus->cursos.find( c2 ) == itCampus->cursos.end() )
				{
					continue;
				}
				if ( problemData->cursosCompativeis(c1, c2) )
				{
					continue;
				}

				ITERA_GGROUP_LESSPTR( itDisc, problemData->disciplinas, Disciplina )
				{
					Disciplina *disciplina = *itDisc;

					// A disciplina deve ser ofertada no campus especificado
					if ( problemData->cp_discs[campusId].find( disciplina->getId() ) ==
						 problemData->cp_discs[campusId].end() )
					{
						continue;
					}

					#pragma region Equivalencias
					if ( problemData->mapDiscSubstituidaPor.find( disciplina ) !=
						problemData->mapDiscSubstituidaPor.end() )
					{
						continue;
					}
					#pragma endregion	
										
					if ( !c1->possuiDisciplina(disciplina) || !c2->possuiDisciplina(disciplina) )
						continue;
					

					for ( int turma = 0; turma < disciplina->getNumTurmas(); turma++ )
					{
						c.reset();
						c.setType( Constraint::C_ALUNOS_CURSOS_INCOMP );
						c.setCampus( *itCampus );
						c.setParCursos( std::make_pair( c1, c2 ) );
						c.setDisciplina( disciplina );
						c.setTurma( turma );

						sprintf( name, "%s", c.toString().c_str() ); 

						if ( cHash.find( c ) != cHash.end() )
						{
							continue;
						}

						nnz = 3;

						OPT_ROW row( nnz, OPT_ROW::LESS , 1.0 , name );

						v.reset();
						v.setType( Variable::V_ALOC_ALUNO );
						v.setTurma( turma );
						v.setDisciplina( disciplina );
						v.setCurso( c1 );
						v.setCampus( *itCampus );

						it_v = vHash.find( v );
						if( it_v != vHash.end() )
						{
							row.insert( it_v->second, 1 );
						}

						v.reset();
						v.setType( Variable::V_ALOC_ALUNO );
						v.setTurma( turma );
						v.setDisciplina( disciplina );
						v.setCurso( c2 );
						v.setCampus( *itCampus );

						it_v = vHash.find( v );
						if( it_v != vHash.end() )
						{
							row.insert(it_v->second, 1);
						}

						v.reset();
						v.setType( Variable::V_SLACK_ALOC_ALUNOS_CURSO_INCOMPAT );
						v.setTurma( turma );
						v.setDisciplina( disciplina );
						v.setParCursos( std::make_pair( c1, c2 ) );
						v.setCampus( *itCampus );

						it_v = vHash.find( v );
						if ( it_v != vHash.end() )
						{
							row.insert( it_v->second, -1 );
						}

						if ( row.getnnz() != 0 )
						{
							cHash[ c ] = lp->getNumRows();
							lp->addRow( row );
							restricoes++;
						}
					}
				}
			}
		}
   }

   return restricoes;
}



/*====================================================================/
%DocBegin TRIEDA_LOAD_MODEL

%Constraint 
Fixa��o da distribui��o de cr�ditos por dia (*)
%Desc 

%MatExp

\begin{eqnarray}
x_{i,d,u,s,t} + fcp_{i,d,s,t} - fcm_{i,d,s,t} = FC_{d,s,t} \cdot z_{i,d,cp}  \nonumber \qquad 
\forall cp \in CP
\forall s \in S_{u} \quad
\forall d \in D \quad
\forall i \in I_{d} \quad
\forall t \in T
\end{eqnarray}

%DocEnd
/====================================================================*/

// TRIEDA-395
int SolverMIP::cria_restricao_de_folga_dist_cred_dia( int campusId )
{
   int restricoes = 0;

   char name[ 100 ];
   int nnz;

   Constraint c;
   Variable v;
   VariableHash::iterator it_v;

   Disciplina * disciplina = NULL;
   
   ITERA_GGROUP_LESSPTR( itCampus, problemData->campi, Campus )
   {
	   if ( itCampus->getId() != campusId )
	   {
		    continue;
	   }

      ITERA_GGROUP_LESSPTR( itUnidade, itCampus->unidades, Unidade )
      {
         ITERA_GGROUP_LESSPTR( itCjtSala, itUnidade->conjutoSalas, ConjuntoSala )
         {
			 ITERA_GGROUP_LESSPTR( it_disciplina, itCjtSala->disciplinas_associadas, Disciplina )
            {
                disciplina = ( *it_disciplina );

				#pragma region Equivalencias
				if ( problemData->mapDiscSubstituidaPor.find( disciplina ) !=
					 problemData->mapDiscSubstituidaPor.end() )
				{
					continue;
				}
				#pragma endregion

               for ( int turma = 0; turma < disciplina->getNumTurmas(); turma++ )
               {
                  GGroup< int >::iterator itDiasLetDisc = disciplina->diasLetivos.begin();

                  for (; itDiasLetDisc != disciplina->diasLetivos.end(); itDiasLetDisc++ )
                  {
                     ITERA_GGROUP_LESSPTR( it_fix, problemData->fixacoes, Fixacao )
                     {
                        if ( it_fix->getDisciplinaId() == disciplina->getId() &&
                             it_fix->getDiaSemana() == ( *itDiasLetDisc ) &&
						     it_fix->sala != NULL )
                        {
						   Sala *s = (*itCjtSala)->salas.begin()->second;
						   if ( s->getId() != it_fix->sala->getId() )
						   {
							   continue;
						   }

                           c.reset();
                           c.setType( Constraint::C_SLACK_DIST_CRED_DIA );

                           c.setTurma( turma );
                           c.setDisciplina( disciplina );
                           c.setDia( *itDiasLetDisc );
                           c.setSubCjtSala( *itCjtSala );

                           sprintf( name, "%s", c.toString().c_str() );
                           if ( cHash.find( c ) != cHash.end() )
                           {
                              continue;
                           }

                           if ( disciplina->getNumTurmas() < 0 )
                           {
                              continue;
                           }

                           nnz = 4;

                           OPT_ROW row( nnz, OPT_ROW::EQUAL , 0.0 , name );

							v.reset();
							v.setType( Variable::V_CREDITOS );

							v.setTurma( turma );
							v.setDisciplina( disciplina );
							v.setUnidade( *itUnidade );
							v.setSubCjtSala( *itCjtSala );
							v.setDia( *itDiasLetDisc );

							it_v = vHash.find(v);
							if( it_v != vHash.end() )
							{
								row.insert( it_v->second, 1.0 );
							}

							v.reset();
							v.setType( Variable::V_SLACK_DIST_CRED_DIA_SUPERIOR );
							v.setTurma( turma );
							v.setDisciplina( disciplina );
							v.setDia( *itDiasLetDisc );
							v.setSubCjtSala( *itCjtSala );

							it_v = vHash.find( v );
							if ( it_v != vHash.end() )
							{
								row.insert( it_v->second, 1.0 );
							}

							v.reset();
							v.setType( Variable::V_SLACK_DIST_CRED_DIA_INFERIOR );
							v.setTurma( turma );
							v.setDisciplina( disciplina );
							v.setDia( *itDiasLetDisc );
							v.setSubCjtSala( *itCjtSala );

							it_v = vHash.find( v );
							if ( it_v != vHash.end() )
							{
								row.insert( it_v->second, -1.0 );
							}

							v.reset();
							v.setType( Variable::V_ABERTURA );

							v.setCampus( *itCampus );
							v.setDisciplina( disciplina );
							v.setTurma( turma );

							it_v = vHash.find( v );
							if ( it_v != vHash.end() )
							{
								row.insert( it_v->second,
									-( it_fix->disciplina->getMaxCreds() ) ); // TODO!!!! coeficiente..
							}

							if ( row.getnnz() != 0 )
							{
								cHash[ c ] = lp->getNumRows();

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

   return restricoes;
}

/*====================================================================/
%DocBegin TRIEDA_LOAD_MODEL

%Constraint 
Ativa��o da vari�vel r
%Desc 

%MatExp

\begin{eqnarray}
w_{bc,t,cp} \leq M \cdot r_{bc,t,cp} \nonumber \qquad 
\forall bc \in B \quad
\forall cp \in CP \quad
\forall t \in T
\end{eqnarray}

%Data M
%Desc
big $M$.

%DocEnd
/====================================================================*/

int SolverMIP::cria_restricao_ativacao_var_r( int campusId )
{
   int restricoes = 0;
   char name[ 100 ];
   int nnz;

   Variable v;
   Constraint c;
   VariableHash::iterator it_v;

   ITERA_GGROUP_LESSPTR( it_bloco, problemData->blocos, BlocoCurricular )
   {
	   if ( it_bloco->campus->getId() != campusId )
	   {
		   continue;
	   }
		  
      GGroup< int >::iterator itDiasLetBloco = it_bloco->diasLetivos.begin();

      for (; itDiasLetBloco != it_bloco->diasLetivos.end(); itDiasLetBloco++ )
      {
         c.reset();
         c.setType( Constraint::C_VAR_R );

         c.setBloco( *it_bloco );
         c.setCampus( it_bloco->campus );
         c.setDia( *itDiasLetBloco );

         sprintf( name, "%s", c.toString().c_str() ); 

         if ( cHash.find( c ) != cHash.end() )
         {
            continue;
         }

         nnz = 2;

         OPT_ROW row( nnz, OPT_ROW::LESS , 0.0, name );

         v.reset();
         v.setType( Variable::V_N_SUBBLOCOS );

         v.setBloco( *it_bloco );
         v.setDia( *itDiasLetBloco );
         v.setCampus( it_bloco->campus );

         it_v = vHash.find( v );
         if ( it_v != vHash.end() )
         {
            row.insert( it_v->second, 1.0 );
         }

         v.reset();
         v.setType( Variable::V_ABERTURA_SUBBLOCO_DE_BLC_DIA_CAMPUS );

         v.setBloco( *it_bloco );
         v.setDia( *itDiasLetBloco );
         v.setCampus( it_bloco->campus );

         it_v = vHash.find( v );
         if ( it_v != vHash.end() )
         {
            row.insert( it_v->second, -9999.0 );
         }

         if ( row.getnnz() != 0 )
         {
            cHash[ c ] = lp->getNumRows();

            lp->addRow( row );
            restricoes++;
         }
      }
   }

   return restricoes;
}

/*====================================================================/
%DocBegin TRIEDA_LOAD_MODEL

%Constraint 
Limita a abertura de turmas
%Desc 

%MatExp

\begin{eqnarray}
z_{i,d,cp} \leq \sum\limits_{oft \in O_{cp}} a_{i,d,oft}  \nonumber \qquad 
\forall cp \in CP \quad
\forall d \in D \quad
\forall i \in I_{d}
\end{eqnarray}

%DocEnd
/====================================================================*/

int SolverMIP::cria_restricao_limita_abertura_turmas( int campusId )
{
   int restricoes = 0;
   char name[ 100 ];
   int nnz;

   Variable v;
   Constraint c;
   VariableHash::iterator it_v;

   Disciplina * disciplina = NULL;

   int min;
   if ( problemData->parametros->min_alunos_abertura_turmas )
   {
		min = problemData->parametros->min_alunos_abertura_turmas_value;
		if ( min <= 0 ) min = 1;
   }
   else
   {
	   min = 1;
   }

   ITERA_GGROUP_LESSPTR( itCampus, problemData->campi, Campus )
   {
	   if ( itCampus->getId() != campusId )
	   {
		   continue;
	   }

      ITERA_GGROUP_LESSPTR( it_disciplina, problemData->disciplinas, Disciplina )
      {
         disciplina = ( *it_disciplina );

		 // A disciplina deve ser ofertada no campus especificado
		 if ( problemData->cp_discs[campusId].find( disciplina->getId() ) ==
			  problemData->cp_discs[campusId].end() )
		 {
			continue;
		 }

		 #pragma region Equivalencias
		 if ( problemData->mapDiscSubstituidaPor.find( disciplina ) !=
			  problemData->mapDiscSubstituidaPor.end() )
		 {
			continue;
		 }
		 #pragma endregion	

         for ( int turma = 0; turma < disciplina->getNumTurmas(); turma++ )
         {
            c.reset();
            c.setType( Constraint::C_LIMITA_ABERTURA_TURMAS );

            c.setCampus( *itCampus );
            c.setDisciplina( disciplina );
            c.setTurma( turma );

            sprintf( name, "%s", c.toString().c_str() );

            if ( cHash.find( c ) != cHash.end() )
            {
               continue;
            }

            nnz = 2;
            OPT_ROW row( nnz, OPT_ROW::LESS , 0.0, name );

            v.reset();
            v.setType( Variable::V_ABERTURA );

            v.setTurma( turma );
            v.setDisciplina( disciplina );
            v.setCampus( *itCampus );

            it_v = vHash.find( v );
            if ( it_v != vHash.end() )
            {
               row.insert( it_v->second, min );
            }

            // ---

            GGroup< Oferta *, LessPtr< Oferta > > ofertas =
               problemData->ofertasDisc[ disciplina->getId() ];

            ITERA_GGROUP_LESSPTR( itOft, ofertas, Oferta )
            {
               if ( itOft->campus->getId() == itCampus->getId() )
               {            
                  v.reset();
                  v.setType( Variable::V_ALUNOS );

                  v.setTurma( turma );
                  v.setDisciplina( disciplina );
                  v.setOferta( *itOft );

                  it_v = vHash.find( v );
                  if ( it_v != vHash.end() )
                  {
                     row.insert( it_v->second, -1.0 );
                  }
               }
            }

            if ( row.getnnz() != 0 )
            {
               cHash[ c ] = lp->getNumRows();
               lp->addRow( row );
               restricoes++;
            }
         }
      }
   }

   return restricoes;
}

/*====================================================================/
%DocBegin TRIEDA_LOAD_MODEL

%Constraint 
Abertura sequencial de turmas
%Desc 

%MatExp

\begin{eqnarray}
\sum\limits_{u \in U} \sum\limits_{tps \in SCAP_{u}} \sum\limits_{t \in T} o_{i,d,u,tps,t} \geq \sum\limits_{u \in U} \sum\limits_{tps \in SCAP_{u}} \sum\limits_{t \in T} o_{i',d,u,tps,t} \nonumber \qquad 
\forall d \in D \quad
\forall i,i' \in I_{d}
\end{eqnarray}

%DocEnd
/====================================================================*/

int SolverMIP::cria_restricao_abre_turmas_em_sequencia( int campusId )
{
   int restricoes = 0;
   char name[ 100 ];
   int nnz;

   Variable v;
   Constraint c;
   VariableHash::iterator it_v;

   Disciplina * disciplina = NULL;

   ITERA_GGROUP_LESSPTR( it_disciplina, problemData->disciplinas, Disciplina )
   {
      disciplina = ( *it_disciplina );

	  // A disciplina deve ser ofertada no campus especificado
	  if ( problemData->cp_discs[campusId].find( disciplina->getId() ) ==
			problemData->cp_discs[campusId].end() )
 	  {
		  continue;
	  }

	  #pragma region Equivalencias
	  if ( problemData->mapDiscSubstituidaPor.find( disciplina ) !=
		   problemData->mapDiscSubstituidaPor.end() )
	  {
		  continue;
	  }
	  #pragma endregion	

      if ( disciplina->getNumTurmas() > 1 )
      {
         for ( int turma = 0; turma < ( disciplina->getNumTurmas() - 1 ); turma++ )
         {
            c.reset();
            c.setType( Constraint::C_ABRE_TURMAS_EM_SEQUENCIA );

            c.setDisciplina( disciplina );
            c.setTurma( turma );

            sprintf( name, "%s", c.toString().c_str() ); 
            if ( cHash.find( c ) != cHash.end() )
            {
               continue;
            }

            nnz = 9999;
            OPT_ROW row( nnz, OPT_ROW::GREATER , 0.0, name );

            ITERA_GGROUP_LESSPTR( itCampus, problemData->campi, Campus )
            {
				if ( itCampus->getId() != campusId )
				{
					continue;
				}

               ITERA_GGROUP_LESSPTR( itUnidade, itCampus->unidades, Unidade )
               {
                  ITERA_GGROUP_LESSPTR( itCjtSala, itUnidade->conjutoSalas, ConjuntoSala )
                  {
                     GGroup< int /*Dias*/ > disc_sala_dias =
                        problemData->disc_Conjutno_Salas__Dias[ std::make_pair< int, int >
                        ( disciplina->getId(), itCjtSala->getId() ) ];

                     ITERA_GGROUP_N_PT( itDiscSala_Dias, disc_sala_dias, int )
                     {
                        v.reset();
                        v.setType( Variable::V_OFERECIMENTO );

                        v.setTurma( turma );
                        v.setDisciplina( disciplina );
                        v.setUnidade( *itUnidade );
                        v.setSubCjtSala( *itCjtSala );
                        v.setDia( *itDiscSala_Dias );

                        it_v = vHash.find( v );
                        if ( it_v != vHash.end() )
                        {
                           row.insert( it_v->second, 1.0 );
                        }

                        v.reset();
                        v.setType( Variable::V_OFERECIMENTO );

                        int turmaSuc = turma + 1;
                        v.setTurma(turmaSuc);

                        v.setDisciplina( disciplina );
                        v.setUnidade( *itUnidade );
                        v.setSubCjtSala( *itCjtSala );
                        v.setDia( *itDiscSala_Dias );

                        it_v = vHash.find( v );
                        if ( it_v != vHash.end() )
                        {
                           row.insert( it_v->second, -1.0 );
                        }
                     }
                  }
               }
            }

            if ( row.getnnz() != 0 )
            {
               cHash[ c ] = lp->getNumRows();

               lp->addRow( row );
               restricoes++;
            }
         }
      }
   }

   return restricoes;
}

/*====================================================================/
%DocBegin TRIEDA_LOAD_MODEL

%Constraint 
Regra de divis�o de cr�ditos
%Desc 

%MatExp

\begin{eqnarray}
\sum\limits_{u \in U} \sum\limits_{tps \in SCAP_{u}} x_{i,d,u,tps,t} = \sum\limits_{k \in K_{d}}N_{d,k,t} \cdot m_{d,i,k} + fkp_{d,i,t} - fkm_{d,i,t} \nonumber \qquad 
\forall d \in D \quad
\forall i \in I_{d} \quad
\forall t \in T
\end{eqnarray}

%Data N_{d,k,t}
%Desc
n�mero de cr�ditos determinados para a disciplina $d$ no dia $t$ na combina��o de divis�o de cr�dito $k$.

%DocEnd
/====================================================================*/

int SolverMIP::cria_restricao_divisao_credito( int campusId )
{
   int restricoes = 0;

   char name[ 100 ];
   int nnz;

   Constraint c;
   Variable v;
   VariableHash::iterator it_v;

   Disciplina * disciplina = NULL;

   ITERA_GGROUP_LESSPTR( it_disciplina, problemData->disciplinas, Disciplina )
   {
      disciplina = ( *it_disciplina );
	  
	  // A disciplina deve ser ofertada no campus especificado
	  if ( problemData->cp_discs[campusId].find( disciplina->getId() ) ==
			problemData->cp_discs[campusId].end() )
 	  {
		  continue;
	  }

	  #pragma region Equivalencias
	  if ( problemData->mapDiscSubstituidaPor.find( disciplina ) !=
		   problemData->mapDiscSubstituidaPor.end() )
	  {
		  continue;
	  }
	  #pragma endregion	

      if ( disciplina->divisao_creditos != NULL )
      {
         for ( int turma = 0; turma < disciplina->getNumTurmas(); turma++ )
         {
            ITERA_GGROUP_N_PT( itDiasLetDisc, disciplina->diasLetivos, int )
            {
               c.reset();
               c.setType( Constraint::C_DIVISAO_CREDITO );

               c.setDisciplina( disciplina );
               c.setTurma( turma );
               c.setDia( *itDiasLetDisc );

               sprintf( name, "%s", c.toString().c_str() ); 

               if ( cHash.find( c ) != cHash.end() )
               {
                  continue;
               }

               nnz = ( problemData->totalSalas + ( (int)( disciplina->combinacao_divisao_creditos.size() ) * 2 ) );
               OPT_ROW row( nnz, OPT_ROW::EQUAL , 0.0 , name );

               ITERA_GGROUP_LESSPTR( itCampus, problemData->campi, Campus )
               {
				   if ( itCampus->getId() != campusId )
				   {
					    continue;
				   }

                  ITERA_GGROUP_LESSPTR( itUnidade, itCampus->unidades, Unidade )
                  {
                     ITERA_GGROUP_LESSPTR( itCjtSala, itUnidade->conjutoSalas, ConjuntoSala )
                     {
                        v.reset();
                        v.setType( Variable::V_CREDITOS );

                        v.setTurma( turma );
                        v.setDisciplina( disciplina );
                        v.setUnidade( *itUnidade );
                        v.setSubCjtSala( *itCjtSala );
                        v.setDia( *itDiasLetDisc );

                        it_v = vHash.find( v );
                        if ( it_v != vHash.end() )
                        {
                           row.insert( it_v->second, 1.0 );
                        }

                        for ( int k = 0; k < (int)disciplina->combinacao_divisao_creditos.size(); k++ )
                        {	
                           v.reset();
                           v.setType( Variable::V_COMBINACAO_DIVISAO_CREDITO );

                           v.setDisciplina( disciplina );
                           v.setTurma( turma );
                           v.setK( k );

                           int d = *itDiasLetDisc;

                           // N{d,k,t}
                           int numCreditos = ( disciplina->combinacao_divisao_creditos[ k ] )[ ( *itDiasLetDisc ) - 1 ].second;

                           it_v = vHash.find( v );
                           if ( it_v != vHash.end() )
                           {
                              row.insert( it_v->second, -numCreditos );
                           }

                           v.reset();
                           v.setType( Variable::V_SLACK_COMBINACAO_DIVISAO_CREDITO_M );

                           v.setDisciplina( disciplina );
                           v.setTurma( turma );
                           v.setDia( *itDiasLetDisc );

                           it_v = vHash.find( v );
                           if( it_v != vHash.end() )
                           {
                              row.insert( it_v->second, -1.0 );
                           }

                           v.reset();
                           v.setType( Variable::V_SLACK_COMBINACAO_DIVISAO_CREDITO_P );

                           v.setDisciplina( disciplina );
                           v.setTurma( turma );
                           v.setDia( *itDiasLetDisc );

                           it_v = vHash.find( v );
                           if( it_v != vHash.end() )
                           {
                              row.insert( it_v->second, 1.0 );
                           }
                        }
                     }
                  }
               }

               if ( row.getnnz() != 0 )
               {
                  cHash[ c ] = lp->getNumRows();

                  lp->addRow( row );
                  restricoes++;
               }
            }
         }
      }
   }

   return restricoes;
}

/*====================================================================/
%DocBegin TRIEDA_LOAD_MODEL

%Constraint 
Somente uma combina��o de regra de divis�o de cr�ditos pode ser escolhida
%Desc 

%MatExp

\begin{eqnarray}
\sum\limits_{k \in K_{d}} m_{d,i,k} \leq 1 \nonumber \qquad 
\forall d \in D \quad
\forall i \in I_{d}
\end{eqnarray}

%DocEnd
/====================================================================*/

int SolverMIP::cria_restricao_combinacao_divisao_credito( int campusId )
{
   int restricoes = 0;
   char name[ 100 ];
   int nnz;

   Variable v;
   Constraint c;
   VariableHash::iterator it_v;

   Disciplina * disciplina = NULL;
   
   ITERA_GGROUP_LESSPTR( it_disciplina, problemData->disciplinas, Disciplina )
   {
      disciplina = ( *it_disciplina );

	  // A disciplina deve ser ofertada no campus especificado
	  if ( problemData->cp_discs[campusId].find( disciplina->getId() ) ==
			problemData->cp_discs[campusId].end() )
 	  {
		  continue;
	  }

	  #pragma region Equivalencias
	  if ( problemData->mapDiscSubstituidaPor.find( disciplina ) !=
		   problemData->mapDiscSubstituidaPor.end() )
	  {
		  continue;
	  }
	  #pragma endregion	

      for ( int i = 0; i < disciplina->getNumTurmas(); i++ )
      {
         c.reset();
         c.setType( Constraint::C_COMBINACAO_DIVISAO_CREDITO );

         c.setDisciplina( disciplina );
         c.setTurma( i );

         sprintf( name, "%s", c.toString().c_str() ); 

         if ( cHash.find( c ) != cHash.end() )
         {
            continue;
         }

         nnz = (int)( disciplina->combinacao_divisao_creditos.size() );
         OPT_ROW row( nnz, OPT_ROW::LESS , 1.0, name );

         for ( int k = 0; k < (int)disciplina->combinacao_divisao_creditos.size(); k++ )
         {
            v.reset();
            v.setType( Variable::V_COMBINACAO_DIVISAO_CREDITO );

            v.setTurma( i );
            v.setDisciplina( disciplina );
            v.setK( k );

            it_v = vHash.find( v );

            if ( it_v != vHash.end() )
            {
               row.insert( it_v->second, 1.0 );
            }
         }

         if ( row.getnnz() != 0 )
         {
            cHash[ c ] = lp->getNumRows();

            lp->addRow( row );
            restricoes++;
         }
      }
   }

   return restricoes;
}

/*====================================================================/
%DocBegin TRIEDA_LOAD_MODEL

%Constraint 
Ativa��o da vari�vel y
%Desc 

%MatExp

\begin{eqnarray}
\sum\limits_{t \in T} o_{i,d,u,tps,t}  \geq  y_{i,d,tps,u}  \nonumber \qquad 
\forall d \in D \quad
\forall i \in I_{d} \quad
\forall u \in U \quad
\forall s \in S_{u}
\forall tps \in SCAP_{u}
\end{eqnarray}

%DocEnd
/====================================================================*/

int SolverMIP::cria_restricao_ativacao_var_y( int campusId )
{
   int restricoes = 0;
   char name[ 100 ];
   int nnz;

   Variable v;
   Constraint c;
   VariableHash::iterator it_v;

   Disciplina * disciplina = NULL;

   ITERA_GGROUP_LESSPTR( itCampus, problemData->campi, Campus )
   {
	   if ( itCampus->getId() != campusId )
	   {
		   continue;
	   }

      ITERA_GGROUP_LESSPTR( itUnidade, itCampus->unidades, Unidade )
      {
         ITERA_GGROUP_LESSPTR( itCjtSala, itUnidade->conjutoSalas, ConjuntoSala )
         {
			 ITERA_GGROUP_LESSPTR( it_disciplina, itCjtSala->disciplinas_associadas, Disciplina )
            {
               disciplina = ( *it_disciplina );

			   #pragma region Equivalencias
			   if ( problemData->mapDiscSubstituidaPor.find( disciplina ) !=
				    problemData->mapDiscSubstituidaPor.end() )
			   {
				   continue;
			   }
			   #pragma endregion

               for ( int turma = 0; turma < disciplina->getNumTurmas(); turma++ )
               {
                  c.reset();
                  c.setType( Constraint::C_VAR_Y );

                  c.setUnidade( *itUnidade );
                  c.setSubCjtSala( *itCjtSala );
                  c.setDisciplina( disciplina );
                  c.setTurma( turma );

                  sprintf( name, "%s", c.toString().c_str() ); 
                  if ( cHash.find( c ) != cHash.end() )
                  {
                     continue;
                  }

                  nnz = 8;
                  OPT_ROW row( nnz, OPT_ROW::GREATER , 0.0, name );

                  GGroup< int /*Dias*/ >::iterator itDiscSala_Dias =
                     problemData->disc_Conjutno_Salas__Dias
                     [ std::make_pair< int, int > ( disciplina->getId(), itCjtSala->getId() ) ].begin();

                  for (; itDiscSala_Dias != problemData->disc_Conjutno_Salas__Dias
                     [ std::make_pair< int, int > ( disciplina->getId(), itCjtSala->getId() ) ].end();
                     itDiscSala_Dias++ )
                  {
                     v.reset();
                     v.setType( Variable::V_OFERECIMENTO );

                     v.setTurma( turma );
                     v.setDisciplina( disciplina );
                     v.setUnidade( *itUnidade );
                     v.setSubCjtSala( *itCjtSala );
                     v.setDia( *itDiscSala_Dias );

                     it_v = vHash.find( v );
                     if ( it_v != vHash.end() )
                     {
                        row.insert( it_v->second, 1.0 );
                     }
                  }

                  v.reset();
                  v.setType( Variable::V_ALOC_DISCIPLINA );

                  v.setTurma( turma );
                  v.setDisciplina( disciplina );
                  v.setUnidade( *itUnidade );
                  v.setSubCjtSala( *itCjtSala );

                  it_v = vHash.find( v );
                  if ( it_v != vHash.end() )
                  {
                     row.insert( it_v->second, -1.0 );
                  }

                  if ( row.getnnz() != 0 )
                  {
                     cHash[ c ] = lp->getNumRows();

                     lp->addRow( row );
                     restricoes++;
                  }
               }
            }
         }
      }
   }

   return restricoes;
}

/*====================================================================/
%DocBegin TRIEDA_LOAD_MODEL

%Constraint
M�ximo de cr�ditos di�rios da disciplina
%Desc 

%MatExp

\begin{eqnarray}
\sum\limits_{u \in U} \sum\limits_{tps \in SCAP_{u}} x_{i,d,u,tps,t} - xm_{bc, d, t} \leq 0 \nonumber \qquad 
\forall bc \in BC \quad
\forall d \in D_{bc} \quad
\forall i \in I_{d} \quad
\forall t \in T
\end{eqnarray}

%DocEnd
/====================================================================*/

int SolverMIP::cria_restricao_max_creds_disc_dia( int campusId )
{
   int restricoes = 0;
   char name[ 100 ];
   int nnz;

   Constraint c;
   Variable v;
   VariableHash::iterator it_v;

   Disciplina * disciplina = NULL;

   ITERA_GGROUP_LESSPTR( it_bloco, problemData->blocos, BlocoCurricular )
   {
	   if ( it_bloco->campus->getId() != campusId )
	   {
		   continue;
	   }

      ITERA_GGROUP_LESSPTR(it_disciplina,it_bloco->disciplinas,Disciplina)
      {
         disciplina = ( *it_disciplina );

		 #pragma region Equivalencias
		 if ( problemData->mapDiscSubstituidaPor.find( disciplina ) !=
			  problemData->mapDiscSubstituidaPor.end() )
		 {
	 		continue;
		 }
		 #pragma endregion

         GGroup< int >::iterator itDiasLetBloco = it_bloco->diasLetivos.begin();
         for (; itDiasLetBloco != it_bloco->diasLetivos.end(); itDiasLetBloco++ )
         {
            for ( int turma = 0; turma < disciplina->getNumTurmas(); turma++ )
            {
               c.reset();
               c.setType( Constraint::C_MAX_CREDS_DISC_DIA );

               c.setDisciplina( disciplina );
               c.setTurma( turma );
               c.setDia( *itDiasLetBloco );
               c.setBloco(*it_bloco);

               sprintf( name, "%s", c.toString().c_str() ); 
               if (cHash.find(c) != cHash.end())
               {
                  continue;
               }

               nnz = 100;

               OPT_ROW row( nnz, OPT_ROW::LESS, 0.0, name );

               ITERA_GGROUP_LESSPTR( it_Campus, problemData->campi, Campus )
               {
				   if ( it_Campus->getId() != campusId )
				   {
					   continue;
				   }

                  ITERA_GGROUP_LESSPTR( it_Unidade, it_Campus->unidades, Unidade )
                  {
                     ITERA_GGROUP_LESSPTR( it_Cjt_Sala, it_Unidade->conjutoSalas, ConjuntoSala )
                     {
                        std::map< std::pair< int, int>, GGroup< int > >::iterator

                           it_Disc_Cjt_Salas__Dias = problemData->disc_Conjutno_Salas__Dias.find(
                           std::make_pair( disciplina->getId(), it_Cjt_Sala->getId() ) );

                        // Testando se a disciplina em questao esta associada ao cjt de salas em questao
                        if ( it_Disc_Cjt_Salas__Dias != problemData->disc_Conjutno_Salas__Dias.end() )
                        {
                           // Testando se a dia (referenciado por <*it_Dias_Letivos>) � um dia 
                           // letivo comum � disciplina e o conjunto de salas em quest�o.
                           if ( it_Disc_Cjt_Salas__Dias->second.find( *itDiasLetBloco ) != 
                              it_Disc_Cjt_Salas__Dias->second.end() )
                           {
                              v.reset();
                              v.setType( Variable::V_CREDITOS );

                              v.setTurma( turma );
                              v.setDisciplina( disciplina );
                              v.setUnidade( *it_Unidade );
                              v.setSubCjtSala( *it_Cjt_Sala );
                              v.setDia( *itDiasLetBloco );

                              it_v = vHash.find( v );
                              if ( it_v != vHash.end() )
                              {
                                 row.insert( it_v->second, 1.0 );
                              }
                           }
                        }
                     }
                  }
               }

               if ( row.getnnz() <= 0 )
               {
                  continue;
               }

               v.reset();
               v.setType( Variable::V_CREDITOS_MODF );

               v.setDisciplina( disciplina );
               v.setDia( *itDiasLetBloco );
               v.setBloco(*it_bloco);

               it_v = vHash.find( v );
               if ( it_v != vHash.end() )
               {
                  row.insert( it_v->second, -1.0 );
               }

               if ( row.getnnz() != 0 )
               {
                  cHash[ c ] = lp->getNumRows();

                  lp->addRow( row );
                  restricoes++;
               }
            }
         }
      }
   }

   return restricoes;
}

/*====================================================================/
%DocBegin TRIEDA_LOAD_MODEL

%Constraint 
M�ximo de cr�ditos di�rios do bloco
%Desc

%MatExp
\begin{eqnarray}
\sum\limits_{d \in D_{bc}} xm_{bc,d, t} \leq MAX_CRED \nonumber \qquad 
\forall bc \in B \quad
\forall t \in T
\end{eqnarray}

%DocEnd
/====================================================================*/

int SolverMIP::cria_restricao_max_creds_bloco_dia( int campusId )
{
   int restricoes = 0;
   char name[ 100 ];
   int nnz;

   Constraint c;
   Variable v;
   VariableHash::iterator it_v;

   Disciplina * disciplina = NULL;

   ITERA_GGROUP_LESSPTR( it_bloco, problemData->blocos, BlocoCurricular )
   {
	   if ( it_bloco->getId() != campusId )
	   {
		   continue;
	   }

      GGroup< int >::iterator itDiasLetBloco = it_bloco->diasLetivos.begin();
      for (; itDiasLetBloco != it_bloco->diasLetivos.end(); itDiasLetBloco++ )
      {
         c.reset();
         c.setType( Constraint::C_MAX_CREDS_BLOCO_DIA );
         c.setBloco( *it_bloco );
         c.setDia( *itDiasLetBloco );

         sprintf( name, "%s", c.toString().c_str() ); 

         if ( cHash.find(c) != cHash.end() )
         {
            continue;
         }

         nnz = 100;
         double rhs = it_bloco->getMaxCredsNoDia(*itDiasLetBloco);

         OPT_ROW row( nnz, OPT_ROW::LESS, rhs, name );

         ITERA_GGROUP_LESSPTR(it_disciplina,it_bloco->disciplinas,Disciplina)
         {
            disciplina = ( *it_disciplina );
			
			#pragma region Equivalencias
			if ( problemData->mapDiscSubstituidaPor.find( disciplina ) !=
				problemData->mapDiscSubstituidaPor.end() )
			{
				continue;
			}
			#pragma endregion

            v.reset();
            v.setType( Variable::V_CREDITOS_MODF );

            v.setBloco(*it_bloco);
            v.setDisciplina( disciplina );
            v.setDia( *itDiasLetBloco );

            it_v = vHash.find( v );
            if ( it_v != vHash.end() )
            {
               row.insert( it_v->second, 1.0 );
            }
         }
         if ( row.getnnz() != 0 )
         {
            cHash[ c ] = lp->getNumRows();

            lp->addRow( row );
            restricoes++;
         }
      }
   }

   return restricoes;
}

/*====================================================================/
%DocBegin TRIEDA_LOAD_MODEL

%Constraint 
Ativa��o da vari�vel zc
%Desc

%MatExp
\begin{eqnarray}
\sum\limits_{i \in I} \sum\limits_{u \in U} \sum\limits_{tps \in SCAP_{u}} o_{i,d,u,tps,t} \leq zc_{d,t} \cdot N \nonumber \qquad 
\forall d \in D \quad
\forall t \in T
\end{eqnarray}


%DocEnd
/====================================================================*/

int SolverMIP::cria_restricao_ativacao_var_zc( int campusId )
{
   int restricoes = 0;
   char name[ 100 ];
   int nnz;

   Constraint c;
   Variable v;
   VariableHash::iterator it_v;

   Disciplina * disciplina = NULL;

   Campus *campus = problemData->refCampus[campusId];

   ITERA_GGROUP_LESSPTR( it_disciplina, problemData->disciplinas, Disciplina )
   {
      disciplina = ( *it_disciplina );
	  
	  // A disciplina deve ser ofertada no campus especificado
	  if ( problemData->cp_discs[campusId].find( disciplina->getId() ) ==
			problemData->cp_discs[campusId].end() )
 	  {
		  continue;
	  }

	  #pragma region Equivalencias
	  if ( problemData->mapDiscSubstituidaPor.find( disciplina ) !=
		   problemData->mapDiscSubstituidaPor.end() )
	  {
		   continue;
	  }
	  #pragma endregion

      GGroup< int >::iterator itDiasLetDisc = disciplina->diasLetivos.begin();

      for (; itDiasLetDisc != disciplina->diasLetivos.end(); itDiasLetDisc++ )
      {
         c.reset();
         c.setType( Constraint::C_VAR_ZC );

         c.setDisciplina( disciplina );
         c.setDia( *itDiasLetDisc );
		 c.setCampus( campus );

         sprintf( name, "%s", c.toString().c_str() ); 
         if ( cHash.find( c ) != cHash.end() )
         {
            continue;
         }

         nnz = 100;

         OPT_ROW row( nnz, OPT_ROW::LESS, 0.0, name );
		 
		 ITERA_GGROUP_LESSPTR( itUnidade, campus->unidades, Unidade )
		 {
			ITERA_GGROUP_LESSPTR( itCjtSala, itUnidade->conjutoSalas, ConjuntoSala )
			{
				for ( int turma = 0; turma < disciplina->getNumTurmas(); turma++ )
				{
					v.reset();
					v.setType( Variable::V_OFERECIMENTO );

					v.setTurma( turma );
					v.setDisciplina( disciplina );
					v.setUnidade( *itUnidade );
					v.setSubCjtSala( *itCjtSala );
					v.setDia( *itDiasLetDisc );

					it_v = vHash.find( v );
					if ( it_v != vHash.end() )
					{
						row.insert( it_v->second, 1.0 );
					}
				}
			}
		 }

         v.reset();
         v.setType( Variable::V_ABERTURA_COMPATIVEL );

         v.setDisciplina( disciplina );
         v.setDia( *itDiasLetDisc );
		 v.setCampus( campus );

         it_v = vHash.find( v );
         if ( it_v != vHash.end() )
         {
            row.insert( it_v->second, -100.0 );
         }

         if ( row.getnnz() != 0 )
         {
            cHash[ c ] = lp->getNumRows();

            lp->addRow( row );
            restricoes++;
         }
      }
   }

   return restricoes;
}

/*====================================================================/
%DocBegin TRIEDA_LOAD_MODEL

%Constraint 
Disciplinas incompat�veis
%Desc

%MatExp
\begin{eqnarray}
zc_{d_1,t} + zc_{d_2,t} \leq 1 \nonumber \qquad 
(d_1, d_2),
\forall t \in T
\end{eqnarray}

%DocEnd
/====================================================================*/

int SolverMIP::cria_restricao_disciplinas_incompativeis( int campusId )
{
   int restricoes = 0;
   char name[ 100 ];
   int nnz;

   Constraint c;
   Variable v;
   VariableHash::iterator it_v;

   Disciplina * disciplina = NULL;
   
   Campus *campus = problemData->refCampus[ campusId ];

   ITERA_GGROUP_LESSPTR( it_disciplina, problemData->disciplinas, Disciplina )
   {
      disciplina = ( *it_disciplina );
	  
	  // A disciplina deve ser ofertada no campus especificado
	  if ( problemData->cp_discs[campusId].find( disciplina->getId() ) ==
			problemData->cp_discs[campusId].end() )
 	  {
		  continue;
	  }

	  #pragma region Equivalencias
	  if ( problemData->mapDiscSubstituidaPor.find( disciplina ) !=
		   problemData->mapDiscSubstituidaPor.end() )
	  {
		   continue;
	  }
	  #pragma endregion

      ITERA_GGROUP_N_PT( itDiasLetDisc, disciplina->diasLetivos, int )
      {
         ITERA_GGROUP_N_PT( it_inc, disciplina->ids_disciplinas_incompativeis, int )
         {
            //Disciplina * nova_disc = new Disciplina();
			std::map< int, Disciplina* >::iterator it_Ref_Disc = problemData->refDisciplinas.find( *it_inc );
			if ( it_Ref_Disc == problemData->refDisciplinas.end() )
			{ 
				continue;
			}
			
			Disciplina * nova_disc = it_Ref_Disc->second;

			// A disciplina deve ser ofertada no campus especificado
			if ( problemData->cp_discs[campusId].find( nova_disc->getId() ) ==
				problemData->cp_discs[campusId].end() )
 			{
				continue;
			}

			#pragma region Equivalencias
			if ( problemData->mapDiscSubstituidaPor.find( nova_disc ) !=
				problemData->mapDiscSubstituidaPor.end() )
			{
				continue;
			}
			#pragma endregion

            c.reset();
            c.setType( Constraint::C_DISC_INCOMPATIVEIS );

            c.setDisciplina( nova_disc );
            c.setDia( *itDiasLetDisc );
			c.setCampus( campus );

            sprintf( name, "%s", c.toString().c_str() ); 

            if ( cHash.find( c ) != cHash.end() )
            {
               continue;
            }

            nnz = 2;
            OPT_ROW row( nnz, OPT_ROW::LESS, 1.0, name );

            v.reset();
            v.setType( Variable::V_ABERTURA_COMPATIVEL );

            v.setDisciplina( disciplina );
            v.setDia( *itDiasLetDisc );
			v.setCampus( campus );

            it_v = vHash.find( v );
            if ( it_v != vHash.end() )
            {
               row.insert( it_v->second, 1.0 );
            }

            v.setDisciplina( nova_disc );

            it_v = vHash.find( v );
			if ( it_v != vHash.end() )
            {
               row.insert(it_v->second, 1.0);
            }

            if ( row.getnnz() != 0 )
            {
               cHash[ c ] = lp->getNumRows();

               lp->addRow( row );
               restricoes++;
            }

         }
      }
   }

   return restricoes;
}

/*====================================================================/
%DocBegin TRIEDA_LOAD_MODEL

%Constraint 
Ativa��o da vari�vel n
%Desc

%MatExp
\begin{eqnarray}
\sum\limits_{u \in U} \sum\limits_{i \in I} \sum\limits_{d \in D_{bc}} 
o_{i,d,u,tps,t} \leq M \cdot n_{bc,tps} \nonumber \qquad 
\forall tps \in SCAP_{u}
\forall bc \in B
\forall t \in T
\end{eqnarray}

%DocEnd
/====================================================================*/
int SolverMIP::cria_restricao_abertura_bloco_mesmoTPS( int campusId )
{
   int restricoes = 0;
   char name[ 100 ];
   int nnz;

   Constraint c;
   Variable v;
   VariableHash::iterator it_v;

   Disciplina * disciplina = NULL;
   Disciplina * disciplina_equivalente = NULL;

   Curso * curso = NULL;
   Curriculo * curriculo = NULL;

   ITERA_GGROUP_LESSPTR( itBloco, problemData->blocos, BlocoCurricular )
   {
	     Campus *cp = itBloco->campus;

         ITERA_GGROUP_LESSPTR( itUnidade, cp->unidades, Unidade )
         {
            ITERA_GGROUP_LESSPTR( itCjtSala, itUnidade->conjutoSalas, ConjuntoSala )
            {
               std::map< Disciplina *, GGroup< int > >::iterator
                  it_disc_dias = itCjtSala->dias_letivos_disciplinas.begin();

               for (; it_disc_dias != itCjtSala->dias_letivos_disciplinas.end();
                  it_disc_dias++ )
               {
                  GGroup< int >::iterator itDiasLetCjtSala =
                     it_disc_dias->second.begin();

                  for (; itDiasLetCjtSala != it_disc_dias->second.end();
                     itDiasLetCjtSala++ )
                  {
                     c.reset();
                     c.setType( Constraint::C_EVITA_BLOCO_TPS_D );

                     c.setSubCjtSala( *itCjtSala );
                     c.setBloco( *itBloco );
                     c.setDia( *itDiasLetCjtSala );

                     sprintf( name, "%s", c.toString().c_str() ); 
                     if ( cHash.find(c) != cHash.end() )
                     {
                        continue;
                     }

                     nnz = 100;
                     OPT_ROW row( nnz, OPT_ROW::LESS, 0.0, name );

                     ITERA_GGROUP_LESSPTR( it_disciplina, itBloco->disciplinas, Disciplina )
                     {
                        disciplina = ( *it_disciplina );

						#pragma region Equivalencias
						if ( problemData->mapDiscSubstituidaPor.find( disciplina ) !=
							problemData->mapDiscSubstituidaPor.end() )
						{
							continue;
						}
						#pragma endregion
							  
                        for ( int turma = 0; turma < disciplina->getNumTurmas(); turma++ )
                        {
                            v.reset();
                            v.setType( Variable::V_OFERECIMENTO );

                            v.setTurma( turma );
                            v.setDisciplina( disciplina );
                            v.setUnidade( *itUnidade );
                            v.setSubCjtSala( *itCjtSala );
                            v.setDia( *itDiasLetCjtSala );

                            it_v = vHash.find( v );
                            if ( it_v != vHash.end() )
                            {
								row.insert( it_v->second, 1.0 );
                            }

                            v.reset();
                            v.setType( Variable::V_ABERTURA_BLOCO_MESMO_TPS );

                            v.setBloco( *itBloco );
                            v.setSubCjtSala( *itCjtSala );

                            it_v = vHash.find( v );
                            if ( it_v != vHash.end() )
                            {
								int bigM = 100;
								row.insert( it_v->second, -bigM );
                            }
                        }
                     }
                      
                     if ( row.getnnz() != 0 )
                     {
                        cHash[ c ] = lp->getNumRows();

                        lp->addRow( row );
                        restricoes++;
                     }
                  }
               }
            }
         }
   }

   return restricoes;
}

/*====================================================================/
%DocBegin TRIEDA_LOAD_MODEL

%Constraint 
Evitar aloca��o do mesmo bloco curricular em tipos de salas diferentes
%Desc

%MatExp
\begin{eqnarray}
\sum\limits_{tps \in SCAP_{u}} n_{bc,tps} + fn_{bc,tps} 
\leq 1 \nonumber \qquad 
\forall bc \in B
\end{eqnarray}

%DocEnd
/====================================================================*/
int SolverMIP::cria_restricao_folga_abertura_bloco_mesmoTPS( int campusId )
{
   int restricoes = 0;
   char name[ 100 ];
   int nnz;

   Constraint c;
   Variable v;
   VariableHash::iterator it_v;

   ITERA_GGROUP_LESSPTR( itBloco, problemData->blocos, BlocoCurricular )
   {
	   if ( itBloco->campus->getId() != campusId )
	   {
		   continue;
	   }

      c.reset();
      c.setType( Constraint::C_SLACK_EVITA_BLOCO_TPS_D );
      c.setBloco( *itBloco );

      sprintf( name, "%s", c.toString().c_str() ); 

      if ( cHash.find( c ) != cHash.end() )
      {
         continue;
      }

      nnz = 100;

      OPT_ROW row( nnz, OPT_ROW::LESS, 1.0, name );

      ITERA_GGROUP_LESSPTR( itUnidade, itBloco->campus->unidades, Unidade )
      {
            ITERA_GGROUP_LESSPTR( itCjtSala, itUnidade->conjutoSalas, ConjuntoSala )
            {		
               v.reset();
               v.setType( Variable::V_ABERTURA_BLOCO_MESMO_TPS );

               v.setBloco( *itBloco );
               v.setSubCjtSala( *itCjtSala );

               it_v = vHash.find( v );
               if( it_v != vHash.end() )
               {
                  row.insert( it_v->second, 1.0 );
               }

               v.reset();
               v.setType( Variable::V_SLACK_ABERTURA_BLOCO_MESMO_TPS );

               v.setBloco( *itBloco );
               v.setSubCjtSala( *itCjtSala );

               it_v = vHash.find( v );
               if ( it_v != vHash.end() )
               {
                  row.insert( it_v->second, 1.0 );
               }
            }
      }

      if ( row.getnnz() != 0 )
      {
          cHash[ c ] = lp->getNumRows();

          lp->addRow( row );
          restricoes++;
      }
   }

   return restricoes;
}


/*====================================================================/
%DocBegin TRIEDA_LOAD_MODEL

%Constraint 
N�o permitir que alunos de cursos diferentes (mesmo que compativeis) compartilhem turmas.
%Desc 

%MatExp

\begin{eqnarray} 
b_{i,d,c,cp} + b_{i,d,c',cp} - fc_{i,d,c,c',cp} \leq 1 \nonumber \qquad 
\forall d \in D \quad
\forall i \in I_{d} \quad
\forall c,c' C \quad
\forall cp \in CP
\end{eqnarray}

%DocEnd
/====================================================================*/

int SolverMIP::cria_restricao_proibe_compartilhamento( int campusId )
{
   int restricoes = 0;

   if ( problemData->parametros->permite_compartilhamento_turma_sel )
   {
	   return restricoes;
   }

   char name[ 100 ];
   int nnz;

   Constraint c;
   Variable v;
   VariableHash::iterator it_v;

   ITERA_GGROUP_LESSPTR( itCampus, problemData->campi, Campus )
   {
	   if ( itCampus->getId() != campusId )
	   {
		   continue;
	   }

	   std::map< std::pair< Curso *, Curso * >, std::vector< int > >::iterator
               it_cursoComp_disc = problemData->cursosComp_disc.begin();

        for (; it_cursoComp_disc != problemData->cursosComp_disc.end(); it_cursoComp_disc++ )
        {
			Curso *c1 = it_cursoComp_disc->first.first;
			Curso *c2 = it_cursoComp_disc->first.second;

			if ( itCampus->cursos.find( c1 ) == itCampus->cursos.end() ||
				 itCampus->cursos.find( c2 ) == itCampus->cursos.end() )
			{
				continue;
			}

            std::vector< int >::iterator it_disc = it_cursoComp_disc->second.begin();

            for (; it_disc != it_cursoComp_disc->second.end(); ++it_disc )
            {
				int discId = *it_disc;
				Disciplina * disciplina = problemData->retornaDisciplina(discId);
				  
				if (disciplina == NULL) continue;

				#pragma region Equivalencias
				if ( problemData->mapDiscSubstituidaPor.find( disciplina ) !=
					problemData->mapDiscSubstituidaPor.end() )
				{
					continue;
				}
				#pragma endregion

				for ( int turma = 0; turma < disciplina->getNumTurmas(); turma++ )
				{
					c.reset();
					c.setType( Constraint::C_PROIBE_COMPARTILHAMENTO );
					c.setCampus( *itCampus );
					c.setParCursos( std::make_pair( c1, c2 ) );
					c.setDisciplina( disciplina );
					c.setTurma( turma );

					sprintf( name, "%s", c.toString().c_str() ); 

					if ( cHash.find( c ) != cHash.end() )
					{
						continue;
					}

					nnz = 3;

					OPT_ROW row( nnz, OPT_ROW::LESS , 1.0 , name );

					v.reset();
					v.setType( Variable::V_ALOC_ALUNO );
					v.setTurma( turma );
					v.setDisciplina( disciplina );
					v.setCurso( c1 );
					v.setCampus( *itCampus );

					it_v = vHash.find( v );
					if( it_v != vHash.end() )
					{
						row.insert( it_v->second, 1 );
					}

					v.reset();
					v.setType( Variable::V_ALOC_ALUNO );
					v.setTurma( turma );
					v.setDisciplina( disciplina );
					v.setCurso( c2 );
					v.setCampus( *itCampus );

					it_v = vHash.find( v );
					if( it_v != vHash.end() )
					{
						row.insert(it_v->second, 1);
					}

					v.reset();
					v.setType( Variable::V_SLACK_COMPARTILHAMENTO );
					v.setTurma( turma );
					v.setDisciplina( disciplina );
					v.setParCursos( std::make_pair( c1, c2 ) );
					v.setCampus( *itCampus );

					it_v = vHash.find( v );
					if ( it_v != vHash.end() )
					{
						row.insert( it_v->second, -1 );
					}

					if ( row.getnnz() != 0 )
					{
						cHash[ c ] = lp->getNumRows();
						lp->addRow( row );
						restricoes++;
					}
				}
			}
		}
   }

   return restricoes;
}




/*====================================================================/
%DocBegin TRIEDA_LOAD_MODEL

%Constraint 
Evita que, devido a compartilhamento de disciplinas entre duas ofertas,
uma sala tenha horario sobreposto.
%Desc

%MatExp
\begin{eqnarray}
 x_{i,d,u,s,t} + \sum\limits_{u' \in U} \sum\limits_{d' \in (D_{oft1} U D{oft2})} \sum\limits_{i' \in I_{d'}}( q_{i',d',oft,s',t} - p_{i',d',oft1,oft2,s',t})
\leq Q_{bc,t} + M ( 1 - of_{i,d,oft1,oft2} ) \qquad 

\forall d \in (D_{oft1} \cap D_{oft2})
\forall i \in I_{d}
\forall oft \in {oft1, oft2}
\forall u \in U
\forall s \in S_{u}
\forall s' \in S_{u'} \quad s' \ne s
\forall t \in T

\end{eqnarray}

%DocEnd
/====================================================================*/
int SolverMIP::cria_restricao_evita_sobrepos_sala_por_compartilhamento( int campusId )
{

	ofstream outTestFile;
	char outputTestFilename[] = "outTestRestricao_evita_sobrepos_sala_por_compartilhamento.txt";
	outTestFile.open(outputTestFilename, ios::out);
	if (!outTestFile) {
		cerr << "Can't open output file " << outputTestFilename << endl;
		exit(1);
	}

	int restricoes = 0;

	int nnz;
	double M = 9999.0;

	Constraint c;
	Variable v;
	VariableHash::iterator it_v;
	Disciplina * disciplina_equivalente = NULL;
	Curso * curso = NULL;
	Curriculo * curriculo = NULL;

	//map< int, map< pair< Oferta*, Oferta* >, vector<int> > > vars;

	// para cada campus
	ITERA_GGROUP_LESSPTR( itCampus, problemData->campi, Campus )
	{
		Campus *cp = *itCampus;

		if ( cp->getId() != campusId )
		{
			continue;
		}

		// para cada par de ofertas compativeis
		std::map< std::pair< Oferta *, Oferta * >, std::vector< int > >::iterator
			it_oftsComp_disc = problemData->oftsComp_disc.begin();

		for (; it_oftsComp_disc != problemData->oftsComp_disc.end(); it_oftsComp_disc++ )
		{
			Oferta *oft1 = it_oftsComp_disc->first.first;
			Oferta *oft2 = it_oftsComp_disc->first.second;

			if ( oft1->campus != cp || oft1->campus != cp )
				continue;

			Curso *c1 = oft1->curso;
			Curso *c2 = oft2->curso;

			if ( c1->getId() != c2->getId() &&
				!problemData->parametros->permite_compartilhamento_turma_sel )
			{
				continue;
			}

			// para cada disciplina em comum (possivel de ser compartilhada) ao par de ofertas
			std::vector< int >::iterator it_discComum = it_oftsComp_disc->second.begin();
			for (; it_discComum != it_oftsComp_disc->second.end(); ++it_discComum )
			{
				Disciplina * discComum = problemData->retornaDisciplina( *it_discComum );

				if (discComum == NULL)
					continue;

				#pragma region Equivalencias
				if ( problemData->mapDiscSubstituidaPor.find( discComum ) !=
					 problemData->mapDiscSubstituidaPor.end() )
				{
					continue;
				}
				#pragma endregion

				int periodo1 = oft1->periodoDisciplina( discComum );
				int periodo2 = oft2->periodoDisciplina( discComum );

				// Cria a restri��o somente para per�odos que possuem uma mesma semana letiva
				GGroup< Calendario*, LessPtr<Calendario> > sls1 = oft1->curriculo->retornaSemanasLetivasNoPeriodo( periodo1 );
				GGroup< Calendario*, LessPtr<Calendario> > sls2 = oft2->curriculo->retornaSemanasLetivasNoPeriodo( periodo2 );
				if ( sls1.size() != 1 ||
					sls2.size() != 1 ||
					sls1.begin()->getId() != sls2.begin()->getId() )
				{
					continue;
				}

				// para cada turma da disciplina em comum
				for ( int turma = 0; turma < discComum->getNumTurmas(); turma++ )
				{
					vector< pair< int, double > > cols;

#pragma region Variavel of_{i,d,oft1,oft2}
					v.reset();
					v.setType( Variable::V_ALOC_ALUNOS_PAR_OFT ); // of_{i,d,oft1,oft2}
					v.setTurma( turma );
					v.setDisciplina( discComum );
					v.setParOfertas( oft1, oft2 );

					it_v = vHash.find( v );
					if( it_v != vHash.end() )
					{
						//row.insert(it_v->second, M);
						cols.push_back(make_pair(it_v->second, M));
					}
					else
					{
						continue; // N�o cria a restri��o se n�o existe a vari�vel of_{i,d,oft1,oft2}
					}
#pragma endregion

					// para cada unidade do campus da disc compartilhada
					ITERA_GGROUP_LESSPTR( itUnid, cp->unidades, Unidade )
					{
						// para cada sala aonde poder� ser ministrada a disc compartilhada
						ITERA_GGROUP_LESSPTR( itCjtSalaCompart, itUnid->conjutoSalas, ConjuntoSala )
						{   
							// cada conjunto de salas s� pode ter 1 sala!
							if ( itCjtSalaCompart->salas.size() != 1 )
								continue;
							if ( itCjtSalaCompart->disciplinas_associadas.find( discComum ) ==
								itCjtSalaCompart->disciplinas_associadas.end() )
								continue;

							Sala* salaCompart = itCjtSalaCompart->salas.begin()->second;

							// Para cada dia em que discComum pode ser ministrada na salaCompart
							GGroup< int > diasLetivos = itCjtSalaCompart->dias_letivos_disciplinas[ discComum ];
							GGroup< int >::iterator itDia = diasLetivos.begin();
							for (; itDia != diasLetivos.end(); itDia++ )
							{
								//// para cada unidade
								//ITERA_GGROUP_LESSPTR( itUnid2, cp->unidades, Unidade )
								//{
								//	// para cada sala
								//	ITERA_GGROUP_LESSPTR( itCjtSala, itUnid2->conjutoSalas, ConjuntoSala )
								//	{  
								//		// cada conjunto de salas s� pode ter 1 sala!
								//		if ( itCjtSala->salas.size() != 1 )
								//			continue;
								//		// sala diferente da sala da discComum
								//		if ( itCjtSala->salas.begin()->first == salaCompart->getId() )
								//			continue;

								

#pragma region Variavel x_{i,d,u,s,t}
								v.reset();
								v.setType( Variable::V_CREDITOS ); // x_{i,d,u,s,t}
								v.setTurma( turma );
								v.setDisciplina( discComum );
								v.setUnidade( *itUnid );
								v.setSubCjtSala( *itCjtSalaCompart );
								v.setDia(*itDia);											

								it_v = vHash.find( v );
								if( it_v != vHash.end() )
								{
									//row.insert(it_v->second, 1);
									cols.push_back(make_pair(it_v->second, 1));
								}
								else
								{
									continue; // N�o cria a restri��o se n�o existe a vari�vel x_{i,d,u,s,t}
								}
#pragma endregion

								/*outTestFile << "R" << restricoes
								<< "  i=" << c.getTurma()
								<< " d=" << c.getDisciplina()->getId()
								<< " ofts=" << c.getParOfertas().first->getId() << " " << c.getParOfertas().second->getId()
								<< " u=" << c.getUnidade()
								<< " sC" << c.getSubCjtSalaCompart()												
								<< " s" << c.getSubCjtSala()
								<< " t=" << c.getDia() << endl;*/

								/*if(!verificou)
								{
								verificou = true;
								bool inseriu = false;*/

								// para cada unidade
								ITERA_GGROUP_LESSPTR( itUnid2, cp->unidades, Unidade )
								{
									// para cada sala
									ITERA_GGROUP_LESSPTR( itCjtSala, itUnid2->conjutoSalas, ConjuntoSala )
									{  
										// cada conjunto de salas s� pode ter 1 sala!
										if ( itCjtSala->salas.size() != 1 )
											continue;
										// sala diferente da sala da discComum
										if ( itCjtSala->salas.begin()->first == salaCompart->getId() )
											continue;

										bool inseriu = false;

										// para cada disciplina (diferente de discComum) pertencente � uniao dos
										// blocos curric que cont�m discComum das duas ofertas, que
										// pode ser dada na sala itCjtSala
										ITERA_GGROUP_LESSPTR( it_uniao_disc, problemData->disciplinas, Disciplina )
										{													
											Disciplina *disc = ( *it_uniao_disc );

											// A disciplina deve ser ofertada no campus especificado
											if ( problemData->cp_discs[campusId].find( disc->getId() ) ==
												problemData->cp_discs[campusId].end() )
 											{
												continue;
											}

											#pragma region Equivalencias
											if ( problemData->mapDiscSubstituidaPor.find( disc ) !=
												problemData->mapDiscSubstituidaPor.end() )
											{
												continue;
											}
											#pragma endregion

											if ( disc->getId() == discComum->getId() )
												continue;

											// confere se a sala � apta a receber a disciplina
											if ( itCjtSala->disciplinas_associadas.find( disc ) ==
												 itCjtSala->disciplinas_associadas.end() )
												continue;

											int p1 = oft1->periodoDisciplina( disc );
											int p2 = oft2->periodoDisciplina( disc );

											if ( p1 != periodo1 && p2 != periodo2 )
												continue;	

											Oferta *oft;
											bool ambos = false;

											if ( p1 == periodo1 && p2 == periodo2 )	ambos = true;
											else if ( p1 == periodo1 ) oft = oft1;
											else if ( p2 == periodo2 ) oft = oft2;
											
											if ( ambos )
											{														
												// para cada turma da disciplina em comum
												for ( int j = 0; j < disc->getNumTurmas(); j++ )
												{
#pragma region Primeira Variavel q_{j,d,oft1,u,s,t}
													// Primeira oferta: q_{j,d,oft1,u,s,t}
													v.reset();
													v.setType( Variable::V_CREDITOS_OFT );
													v.setTurma( j );
													v.setDisciplina( disc );
													v.setUnidade( *itUnid2 );
													v.setSubCjtSala( *itCjtSala );
													v.setDia( *itDia );
													v.setOferta( oft1 );

													it_v = vHash.find( v );
													if( it_v != vHash.end() )
													{
														//row.insert( it_v->second, 1 );
														cols.push_back(make_pair(it_v->second, 1));
														//vars.push_back(make_pair(it_v->second, 1));
														inseriu = true;
													}
#pragma endregion

#pragma region Segunda Variavel q_{j,d,oft2,u,s,t}
													// Segunda oferta: q_{j,d,oft2,u,s,t}
													v.reset();
													v.setType( Variable::V_CREDITOS_OFT );
													v.setTurma( j );
													v.setDisciplina( disc );
													v.setUnidade( *itUnid2 );
													v.setSubCjtSala( *itCjtSala );
													v.setDia( *itDia );
													v.setOferta( oft2 );

													it_v = vHash.find( v );
													if( it_v != vHash.end() )
													{
														//row.insert( it_v->second, 1 );
														cols.push_back(make_pair(it_v->second, 1));
														//vars.push_back(make_pair(it_v->second, 1));
														inseriu = true;
													}
#pragma endregion

#pragma region Variavel p_{i,d,oft1,oft2,u,tps,t}
													// Variavel p: desconto do possivel excesso
													// que foi acrescentado acima, caso a disciplina
													// disc tenha sido compartilhada entre as ofertas
													v.reset();
													v.setType( Variable::V_CREDITOS_PAR_OFT );
													v.setTurma( j );
													v.setDisciplina( disc );
													v.setUnidade( *itUnid2 );
													v.setSubCjtSala( *itCjtSala );
													v.setDia( *itDia );
													v.setParOfertas( oft1, oft2 );

													it_v = vHash.find( v );
													if( it_v != vHash.end() )
													{
														//row.insert( it_v->second, -1 );
														cols.push_back(make_pair(it_v->second, -1));
														//vars.push_back(make_pair(it_v->second, -1));
														inseriu = true;
													}									
#pragma endregion
												}
											}
											else
											{
												// para cada turma da disciplina em comum
												for ( int j = 0; j < disc->getNumTurmas(); j++ )
												{
#pragma region Variavel q_{j,d,oft,u,s,t}
													v.reset();
													v.setType( Variable::V_CREDITOS_OFT ); // q_{j,d,oft,u,s,t}
													v.setTurma( j );
													v.setDisciplina( disc );
													v.setUnidade( *itUnid2 );
													v.setSubCjtSala( *itCjtSala );
													v.setDia( *itDia );
													v.setOferta( oft );

													it_v = vHash.find( v );
													if( it_v != vHash.end() )
													{
														//row.insert( it_v->second, 1 );
														cols.push_back(make_pair(it_v->second, 1));
														//vars.push_back(make_pair(it_v->second, 1));
														inseriu = true;
													}
#pragma endregion
												}
											}
										}

										if ( !inseriu )
										{
											continue; // Se n�o tiver inserido nenhum q ou p, n�o cria a restri��o.
											//break;
										}
										/*}
										else
										{
										for(vector< pair <int,int > >::iterator itC = vars.begin();
										itC != vars.end();
										itC++)
										cols.push_back(*itC);
										}*/
										// FIM DA RESTRICAO
										//-------------------------------------------------------------------------

										if ( /*row.getnnz() != 0*/ cols.size() > 0 )
										{
											//-------------------------------------------------------------------------
											// NOVA RESTRICAO

											c.reset();
											c.setType( Constraint::C_EVITA_SOBREPOS_SALA_POR_COMPART );
											c.setParOfertas( std::make_pair( oft1, oft2 ) );
											c.setDisciplina( discComum );
											c.setTurma( turma );
											c.setSubCjtSalaCompart(*itCjtSalaCompart);
											c.setDia(*itDia);
											c.setSubCjtSala(*itCjtSala);

											if ( cHash.find( c ) != cHash.end() )
											{
												continue;
											}

											//sprintf( name, "%s", c.toString().c_str() ); 
											nnz = 30;

											// Maximo de creditos no dia para os curriculos aonde est�o
											// a disciplina compartilhada. Tanto faz pegar de Oft1 ou Oft2,
											// o valor tem que ser igual para as duas, j� que se podem compartilhar
											// � porque t�m mesma semana letiva.
											int maxCredDia = oft1->curriculo->getMaxCreds(*itDia);
											if ( oft2->curriculo->getMaxCreds(*itDia) != maxCredDia )
											{
												cerr << endl << "Erro em SolverMIP::cria_restricao_evita_sobrepos_sala_por_compartilhamento():";
												cerr << endl << "Semanas letivas devem ser iguais!" << endl;
												cerr << "curr1: " << oft1->curriculo->getId();
												cerr << "curr2: " << oft2->curriculo->getId() << endl;

												cerr << "sl1: " << oft1->curriculo->calendario->getId();
												cerr << "sl2: " << oft2->curriculo->calendario->getId() << endl;

												cerr << "oft1: " << oft1->getId();
												cerr << "oft2: " << oft2->getId() << endl;

												cerr << "max1: " << maxCredDia;
												cerr << "max2: " << oft2->curriculo->getMaxCreds(*itDia) << endl << endl;

												if ( disciplina_equivalente != NULL )
													cerr << "Houve equivalencia" << endl;

												if (oft2->curriculo->getMaxCreds(*itDia) < maxCredDia)
													maxCredDia = oft2->curriculo->getMaxCreds(*itDia);
											}

											double rhs = M + maxCredDia;
											OPT_ROW row( nnz, OPT_ROW::LESS , rhs , (char*)c.toString().c_str());

											for(vector< pair< int, double > >::iterator itC = cols.begin();
												itC != cols.end();
												itC++)
												row.insert(itC->first, itC->second);

											pair< int, double > p1 = cols[0];
											pair< int, double > p2 = cols[1];

											cols.clear();
											cols.push_back(p1);
											cols.push_back(p2);

											cHash[ c ] = lp->getNumRows();
											lp->addRow( row );
											restricoes++;
										}													
									}
								}

								cols.pop_back();
							}
						}
					}
				}
			}
		}
	}

	outTestFile.close();

	return restricoes;

}


/*====================================================================/
%DocBegin TRIEDA_LOAD_MODEL

%Constraint 
Ativa��o da vari�vel e
%Desc 

%MatExp

\begin{eqnarray}
a_{i,d,oft} \le M \cdot e_{i,d,oft} \nonumber \qquad 
\forall i \in I_{d} \quad
\forall d \in D_{oft} \quad
\forall oft \in O
\end{eqnarray}

%Data M
%Desc
big $M$.

%DocEnd
/====================================================================*/

int SolverMIP::cria_restricao_ativacao_var_e( int campusId )
{
   int restricoes = 0;
   char name[ 100 ];
   int nnz;
   double M = 9999.0;

   Variable v;
   Constraint c;
   VariableHash::iterator it_v;

   ITERA_GGROUP_LESSPTR( itOft, problemData->ofertas, Oferta )
   {
	    Oferta *oft = *itOft;

		if ( oft->getCampusId() != campusId )
		{
			continue;
		}

	    map < Disciplina*, int, LessPtr< Disciplina > >::iterator it_disc = oft->curriculo->disciplinas_periodo.begin();
	    for (; it_disc != oft->curriculo->disciplinas_periodo.end(); it_disc++ )
	    {
			Disciplina * d = it_disc->first;
			
			#pragma region Equivalencias
			if ( problemData->mapDiscSubstituidaPor.find( d ) !=
				problemData->mapDiscSubstituidaPor.end() )
			{
				continue;
			}
			#pragma endregion
			 
		    for ( int turma = 0; turma < d->getNumTurmas(); turma++ )
		    {
				c.reset();
				c.setType( Constraint::C_VAR_E );
				c.setTurma( turma );
				c.setDisciplina( d );
				c.setOferta( oft );

				sprintf( name, "%s", c.toString().c_str() ); 

				if ( cHash.find( c ) != cHash.end() )
				{
					continue;
				}

				nnz = 2;

				OPT_ROW row( nnz, OPT_ROW::LESS , 0.0, name );

				v.reset();
				v.setType( Variable::V_ALUNOS );
				v.setTurma( turma );
				v.setDisciplina( d );
				v.setOferta( oft );

				it_v = vHash.find( v );
				if ( it_v != vHash.end() )
				{
					row.insert( it_v->second, 1.0 );
				}

				v.reset();
				v.setType( Variable::V_ALOC_ALUNOS_OFT );
				v.setTurma( turma );
				v.setDisciplina( d );
				v.setOferta( oft );

				it_v = vHash.find( v );
				if ( it_v != vHash.end() )
				{
					row.insert( it_v->second, -M );
				}

				if ( row.getnnz() != 0 )
				{
					cHash[ c ] = lp->getNumRows();
					lp->addRow( row );
					restricoes++;
				}
			}
		}
   }

   return restricoes;
}



/*====================================================================/
%DocBegin TRIEDA_LOAD_MODEL

%Constraint 
Ativa��o da vari�vel of
%Desc 

%MatExp

\begin{eqnarray}
	e_{i,d,oft1} \cdot e_{i,d,oft2} \eq of_{i,d,oft1,oft2} \qquad
		 
		 =  \qquad

	e_{i,d,oft1} + e_{i,d,oft2} - 1 \le of_{i,d,oft1,oft2} \qquad
	of_{i,d,oft1,oft2} \le e_{i,d,oft2}  \qquad
	of_{i,d,oft1,oft2} \le e_{i,d,oft1}   \qquad

\forall i \in I_{d} \quad
\forall d \in D_{oft} \quad
\forall oft1 \in O_{d} \quad
\forall oft2 \in O_{d}
\end{eqnarray}

%Data M
%Desc
big $M$.

%DocEnd
/====================================================================*/

int SolverMIP::cria_restricao_ativacao_var_of( int campusId )
{
    int restricoes = 0;
    char name[ 100 ];
    int nnz;
    double M = 9999.0;

    Variable v;
    Constraint c;
    VariableHash::iterator it_v;

	// para cada par de ofertas compativeis
	std::map< std::pair< Oferta *, Oferta * >, std::vector< int > >::iterator
            it_oftsComp_disc = problemData->oftsComp_disc.begin();

    for (; it_oftsComp_disc != problemData->oftsComp_disc.end(); it_oftsComp_disc++ )
    {
		Oferta *oft1 = it_oftsComp_disc->first.first;
		Oferta *oft2 = it_oftsComp_disc->first.second;

		Curso *c1 = oft1->curso;
		Curso *c2 = oft2->curso;

		if ( c1->getId() != c2->getId() &&
			 !problemData->parametros->permite_compartilhamento_turma_sel )
		{
			continue;
		}

		if ( oft1->getCampusId() != campusId ||
			 oft2->getCampusId() != campusId )
		{
			continue;
		}

		// para cada disciplina em comum (possivel de ser compartilhada) ao par de ofertas
        std::vector< int >::iterator it_discComum = it_oftsComp_disc->second.begin();
        for (; it_discComum != it_oftsComp_disc->second.end(); ++it_discComum )
        {
			Disciplina * discComum = problemData->retornaDisciplina( *it_discComum );
				  
			if (discComum == NULL) continue;
				
			for ( int turma = 0; turma < discComum->getNumTurmas(); turma++ )
			{
				bool vNotfound = false;
				// Se pelo menos 1 das variaveis n�o existir,
				// n�o tem sentido o trio de restri��es, logo nenhuma ser� criada.

				// ----------------------------------------------------------
				// 1
				#pragma region Restricao tipo 1
				c.reset();
				c.setType( Constraint::C_VAR_OF_1 );
				c.setTurma( turma );
				c.setDisciplina( discComum);
				c.setParOfertas( std::make_pair(oft1, oft2) );

				sprintf( name, "%s", c.toString().c_str() ); 

				if ( cHash.find( c ) != cHash.end() )
				{
					continue;
				}

				nnz = 3;

				OPT_ROW row1( nnz, OPT_ROW::LESS , 1.0, name );
							
				v.reset();
				v.setType( Variable::V_ALOC_ALUNOS_PAR_OFT );
				v.setTurma( turma );
				v.setDisciplina( discComum );
				v.setParOfertas( oft1, oft2 );

				it_v = vHash.find( v );
				if ( it_v != vHash.end() )
				{
					row1.insert( it_v->second, -1.0 );
				}
				else
				{
					vNotfound = true;
					continue;
				}

				v.reset();
				v.setType( Variable::V_ALOC_ALUNOS_OFT );
				v.setTurma( turma );
				v.setDisciplina( discComum );
				v.setOferta( oft1 );
							
				it_v = vHash.find( v );
				if ( it_v != vHash.end() )
				{
					row1.insert( it_v->second, 1.0 );
				}				
				else
				{
					vNotfound = true;
					continue;
				}

				v.reset();
				v.setType( Variable::V_ALOC_ALUNOS_OFT );
				v.setTurma( turma );
				v.setDisciplina( discComum );
				v.setOferta( oft2 );
							
				it_v = vHash.find( v );
				if ( it_v != vHash.end() )
				{
					row1.insert( it_v->second, 1.0 );
				}
				else
				{
					vNotfound = true;
					continue;
				}

				#pragma endregion

				// ----------------------------------------------------------
				// 2
				#pragma region Restricao tipo 2
				c.reset();
				c.setType( Constraint::C_VAR_OF_2 );
				c.setTurma( turma );
				c.setDisciplina( discComum);
				c.setParOfertas( std::make_pair(oft1, oft2) );

				sprintf( name, "%s", c.toString().c_str() ); 

				if ( cHash.find( c ) != cHash.end() )
				{
					continue;
				}

				nnz = 2;

				OPT_ROW row2( nnz, OPT_ROW::LESS , 0.0, name );
							
				v.reset();
				v.setType( Variable::V_ALOC_ALUNOS_PAR_OFT );
				v.setTurma( turma );
				v.setDisciplina( discComum );
				v.setParOfertas( oft1, oft2 );

				it_v = vHash.find( v );
				if ( it_v != vHash.end() )
				{
					row2.insert( it_v->second, 1.0 );
				}
				else
				{
					vNotfound = true;
					continue;
				}

				v.reset();
				v.setType( Variable::V_ALOC_ALUNOS_OFT );
				v.setTurma( turma );
				v.setDisciplina( discComum );
				v.setOferta( oft1 );
							
				it_v = vHash.find( v );
				if ( it_v != vHash.end() )
				{
					row2.insert( it_v->second, -1.0 );
				}
				else
				{
					vNotfound = true;
					continue;
				}
				#pragma endregion

				// ----------------------------------------------------------
				// 3
				#pragma region Restricao tipo 3
				c.reset();
				c.setType( Constraint::C_VAR_OF_3 );
				c.setTurma( turma );
				c.setDisciplina( discComum);
				c.setParOfertas( std::make_pair(oft1, oft2) );

				sprintf( name, "%s", c.toString().c_str() ); 

				if ( cHash.find( c ) != cHash.end() )
				{
					continue;
				}

				nnz = 2;

				OPT_ROW row3( nnz, OPT_ROW::LESS , 0.0, name );
							
				v.reset();
				v.setType( Variable::V_ALOC_ALUNOS_PAR_OFT );
				v.setTurma( turma );
				v.setDisciplina( discComum );
				v.setParOfertas( oft1, oft2 );

				it_v = vHash.find( v );
				if ( it_v != vHash.end() )
				{
					row3.insert( it_v->second, 1.0 );
				}
				else
				{
					vNotfound = true;
					continue;
				}

				v.reset();
				v.setType( Variable::V_ALOC_ALUNOS_OFT );
				v.setTurma( turma );
				v.setDisciplina( discComum );
				v.setOferta( oft2 );
							
				it_v = vHash.find( v );
				if ( it_v != vHash.end() )
				{
					row3.insert( it_v->second, -1.0 );
				}
				else
				{
					vNotfound = true;
					continue;
				}

				#pragma endregion

				// Cria a restri��o tipo 1
				if ( row1.getnnz() != 0 )
				{
					cHash[ c ] = lp->getNumRows();
					lp->addRow( row1 );
					restricoes++;
				}
				// Cria a restri��o tipo 2																
				if ( row2.getnnz() != 0 )
				{
					cHash[ c ] = lp->getNumRows();
					lp->addRow( row2 );
					restricoes++;
				}
				// Cria a restri��o tipo 3
				if ( row3.getnnz() != 0 )
				{
					cHash[ c ] = lp->getNumRows();
					lp->addRow( row3 );
					restricoes++;
				}
			}
		}
	}

   return restricoes;
}


/*====================================================================/
%DocBegin TRIEDA_LOAD_MODEL

%Constraint 
Ativa��o da vari�vel p
%Desc 

%MatExp

\begin{eqnarray}
	
	 p_{i,d,oft1,oft2,u,s,t} \eq x_{i,d,u,s,t} \cdot of_{i,d,oft1,oft2} \nonumber \qquad
	  
	  \eq  \qquad

	 p_{i,d,oft1,oft2,u,s,t} \le M \cdot of_{i,d,oft1,oft2} \nonumber \qquad
	 p_{i,d,oft1,oft2,u,s,t} \ge x_{i,d,u,s,t} - M \cdot ( 1 - of_{i,d,oft1,oft2} ) \nonumber \qquad
	 x_{i,d,u,s,t} \ge p_{i,d,oft1,oft2,u,s,t} - M \cdot ( 1 - of_{i,d,oft1,oft2} ) \nonumber \qquad

\forall i \in I_{d} \quad
\forall d \in D_{oft} \quad
\forall oft1 \in O_{d} \quad
\forall oft2 \in O_{d}
\forall u \in U \quad
\forall s \in S_{u} \quad
\forall t \in T \quad
\end{eqnarray}

%Data M
%Desc
big $M$.

%DocEnd
/====================================================================*/
int SolverMIP::cria_restricao_ativacao_var_p( int campusId )
{
   int restricoes = 0;
   char name[ 100 ];
   int nnz;
   double M = 9999.0;
   
   VariableHash::iterator it_v;
   Variable v;
   
   // para cada campus
   ITERA_GGROUP_LESSPTR( itCampus, problemData->campi, Campus )
   {
	   Campus *cp = *itCampus;
	   
	   if ( cp->getId() != campusId )
	   {
		   continue;
	   }

	   // para cada par de ofertas compativeis
	   std::map< std::pair< Oferta *, Oferta * >, std::vector< int > >::iterator
               it_oftsComp_disc = problemData->oftsComp_disc.begin();

        for (; it_oftsComp_disc != problemData->oftsComp_disc.end(); it_oftsComp_disc++ )
        {
			Oferta *oft1 = it_oftsComp_disc->first.first;
			Oferta *oft2 = it_oftsComp_disc->first.second;

			if ( oft1->campus != cp || oft1->campus != cp )
				continue;

			Curso *c1 = oft1->curso;
			Curso *c2 = oft2->curso;

			if ( c1->getId() != c2->getId() && !problemData->parametros->permite_compartilhamento_turma_sel )
				continue;

			// para cada disciplina em comum (possivel de ser compartilhada) ao par de ofertas
            std::vector< int >::iterator it_discComum = it_oftsComp_disc->second.begin();
            for (; it_discComum != it_oftsComp_disc->second.end(); ++it_discComum )
            {
				Disciplina * discComum = problemData->retornaDisciplina( *it_discComum );
													
				#pragma region Equivalencias
				if ( problemData->mapDiscSubstituidaPor.find( discComum ) !=
					 problemData->mapDiscSubstituidaPor.end() )
				{
					continue;
				}
				#pragma endregion

				if (discComum == NULL) continue;
						
				// para cada unidade do campus de discComum
				ITERA_GGROUP_LESSPTR( itUnid, cp->unidades, Unidade )
				{
					// para cada sala aonde poder� ser ministrada a disc compartilhada
					ITERA_GGROUP_LESSPTR( itCjtSalaCompart, itUnid->conjutoSalas, ConjuntoSala )
					{   
						if ( itCjtSalaCompart->disciplinas_associadas.find( discComum ) ==
								itCjtSalaCompart->disciplinas_associadas.end() )
							continue;

						Sala* salaCompart = itCjtSalaCompart->salas.begin()->second;
										
						GGroup< int > diasLetivos = itCjtSalaCompart->dias_letivos_disciplinas[ discComum ];
									
						// Para cada dia em que discComum pode ser ministrada na salaCompart
						GGroup< int >::iterator itDia = diasLetivos.begin();
						for (; itDia != diasLetivos.end(); itDia++ )
						{
							for ( int turma = 0; turma < discComum->getNumTurmas(); turma++ )
							{
								bool vNotfound = false;
								// Se pelo menos 1 das variaveis n�o existir,
								// n�o tem sentido o trio de restri��es, logo nenhuma ser� criada.


								// ----------------------------------------------------------
								// 1
								#pragma region Restricao tipo 1
										
								Constraint c;

								c.reset();
								c.setType( Constraint::C_VAR_P_1 );
								c.setTurma( turma );
								c.setDisciplina( discComum);
								c.setParOfertas( std::make_pair(oft1, oft2) );
								c.setDia( *itDia );
								c.setUnidade( *itUnid );
								c.setSubCjtSala( *itCjtSalaCompart );

								sprintf( name, "%s", c.toString().c_str() ); 

								if ( cHash.find( c ) != cHash.end() )
								{
									continue;
								}

								nnz = 2;
								OPT_ROW row1( nnz, OPT_ROW::LESS , 0.0, name );

								v.reset();
								v.setType( Variable::V_CREDITOS_PAR_OFT );
								v.setTurma( turma );
								v.setDisciplina( discComum );
								v.setParOfertas( oft1, oft2 );
								v.setUnidade( *itUnid );
								v.setSubCjtSala( *itCjtSalaCompart );
								v.setDia( *itDia );

								it_v = vHash.find( v );
								if ( it_v != vHash.end() )
								{
									row1.insert( it_v->second, 1 );
								}
								else
								{
									vNotfound = true;
									continue;
								}

								v.reset();
								v.setType( Variable::V_ALOC_ALUNOS_PAR_OFT );
								v.setTurma( turma );
								v.setDisciplina( discComum );
								v.setParOfertas( oft1, oft2 );

								it_v = vHash.find( v );
								if ( it_v != vHash.end() )
								{
									row1.insert( it_v->second, -M );
								}
								else
								{
									vNotfound = true;
									continue;
								}

								#pragma endregion
										
								// ----------------------------------------------------------
								// 2
								#pragma region Restricao tipo 2
										
								c.reset();
								c.setType( Constraint::C_VAR_P_2 );
								c.setTurma( turma );
								c.setDisciplina( discComum);
								c.setParOfertas( std::make_pair(oft1, oft2) );
								c.setDia( *itDia );
								c.setUnidade( *itUnid );
								c.setSubCjtSala( *itCjtSalaCompart );

								sprintf( name, "%s", c.toString().c_str() ); 

								if ( cHash.find( c ) != cHash.end() )
								{
									continue;
								}

								nnz = 3;
								OPT_ROW row2( nnz, OPT_ROW::LESS , M, name );

								v.reset();
								v.setType( Variable::V_CREDITOS_PAR_OFT );
								v.setTurma( turma );
								v.setDisciplina( discComum );
								v.setParOfertas( oft1, oft2 );
								v.setUnidade( *itUnid );
								v.setSubCjtSala( *itCjtSalaCompart );
								v.setDia( *itDia );

								it_v = vHash.find( v );
								if ( it_v != vHash.end() )
								{
									row2.insert( it_v->second, -1 );
								}
								else
								{
									vNotfound = true;
									continue;
								}

								v.reset();
								v.setType( Variable::V_CREDITOS );
								v.setTurma( turma );
								v.setDisciplina( discComum );
								v.setUnidade( *itUnid );
								v.setSubCjtSala( *itCjtSalaCompart );
								v.setDia( *itDia );

								it_v = vHash.find( v );
								if ( it_v != vHash.end() )
								{
									row2.insert( it_v->second, 1 );
								}
								else
								{
									vNotfound = true;
									continue;
								}

								v.reset();
								v.setType( Variable::V_ALOC_ALUNOS_PAR_OFT );
								v.setTurma( turma );
								v.setDisciplina( discComum );
								v.setParOfertas( oft1, oft2 );

								it_v = vHash.find( v );
								if ( it_v != vHash.end() )
								{
									row2.insert( it_v->second, M );
								}
								else
								{
									vNotfound = true;
									continue;
								}

								#pragma endregion

								// ----------------------------------------------------------
								// 3
								#pragma region Restricao tipo 3

								c.reset();
								c.setType( Constraint::C_VAR_P_3 );
								c.setTurma( turma );
								c.setDisciplina( discComum);
								c.setParOfertas( std::make_pair(oft1, oft2) );
								c.setDia( *itDia );
								c.setUnidade( *itUnid );
								c.setSubCjtSala( *itCjtSalaCompart );

								sprintf( name, "%s", c.toString().c_str() ); 

								if ( cHash.find( c ) != cHash.end() )
								{
									continue;
								}

								nnz = 3;
								OPT_ROW row3( nnz, OPT_ROW::LESS , M, name );

								v.reset();
								v.setType( Variable::V_CREDITOS_PAR_OFT );
								v.setTurma( turma );
								v.setDisciplina( discComum );
								v.setParOfertas( oft1, oft2 );
								v.setUnidade( *itUnid );
								v.setSubCjtSala( *itCjtSalaCompart );
								v.setDia( *itDia );

								it_v = vHash.find( v );
								if ( it_v != vHash.end() )
								{
									row3.insert( it_v->second, 1 );
								}
								else
								{
									vNotfound = true;
									continue;
								}

								v.reset();
								v.setType( Variable::V_CREDITOS );
								v.setTurma( turma );
								v.setDisciplina( discComum );
								v.setUnidade( *itUnid );
								v.setSubCjtSala( *itCjtSalaCompart );
								v.setDia( *itDia );

								it_v = vHash.find( v );
								if ( it_v != vHash.end() )
								{
									row3.insert( it_v->second, -1 );
								}
								else
								{
									vNotfound = true;
									continue;
								}

								v.reset();
								v.setType( Variable::V_ALOC_ALUNOS_PAR_OFT );
								v.setTurma( turma );
								v.setDisciplina( discComum );
								v.setParOfertas( oft1, oft2 );

								it_v = vHash.find( v );
								if ( it_v != vHash.end() )
								{
									row3.insert( it_v->second, M );
								}
								else
								{
									vNotfound = true;
									continue;
								}

								#pragma endregion
								
								// Cria a restri��o tipo 1
								if ( row1.getnnz() != 0 )
								{
									cHash[ c ] = lp->getNumRows();
									lp->addRow( row1 );
									restricoes++;
								}
								// Cria a restri��o tipo 2																
								if ( row2.getnnz() != 0 )
								{
									cHash[ c ] = lp->getNumRows();
									lp->addRow( row2 );
									restricoes++;
								}
								// Cria a restri��o tipo 3
								if ( row3.getnnz() != 0 )
								{
									cHash[ c ] = lp->getNumRows();
									lp->addRow( row3 );
									restricoes++;
								}

							}
						}
					}
				}
			}
		}
   }

   return restricoes;
}


/*====================================================================/
%DocBegin TRIEDA_LOAD_MODEL

%Constraint 
Ativa��o da vari�vel g
%Desc 

%MatExp

\begin{eqnarray}
	
	 g_{d,oft,t} \ge \sum\limits_{i \in I_{d}} ( q_{i,d,u,s,t,oft} ) \nonumber \qquad

\forall d \in D_{oft} \quad
\forall oft \in O_{d}
\forall u \in U \quad
\forall s \in S_{u} \quad
\forall t \in T \quad
\end{eqnarray}

%DocEnd
/====================================================================*/
int SolverMIP::cria_restricao_ativacao_var_g( int campusId )
{
   int restricoes = 0;
   char name[ 100 ];
   int nnz;
   
   VariableHash::iterator it_v;   

   ITERA_GGROUP_LESSPTR( itOft, problemData->ofertas, Oferta )
   {
	    Oferta *oft = *itOft;
		Campus *cp = oft->campus;

		if ( cp->getId() != campusId )
		{
			continue;
		}

	    map < Disciplina*, int, LessPtr< Disciplina > >::iterator it_disc = oft->curriculo->disciplinas_periodo.begin();
	    for (; it_disc != oft->curriculo->disciplinas_periodo.end(); it_disc++ )
	    {
			Disciplina * d = it_disc->first;

			#pragma region Equivalencias
			if ( problemData->mapDiscSubstituidaPor.find( d ) !=
				problemData->mapDiscSubstituidaPor.end() )
			{
				continue;
			}
			#pragma endregion

			// para cada unidade do campus de oft
			ITERA_GGROUP_LESSPTR( itUnid, cp->unidades, Unidade )
			{
				// para cada sala aonde poder� ser ministrada a disciplina
				ITERA_GGROUP_LESSPTR( itCjtSala, itUnid->conjutoSalas, ConjuntoSala )
				{   
					if ( itCjtSala->disciplinas_associadas.find( d ) ==
						 itCjtSala->disciplinas_associadas.end() )
						continue;

					GGroup< int > diasLetivos = itCjtSala->dias_letivos_disciplinas[ d ];
									
					// Para cada dia em que d pode ser ministrada em itCjtSala
					GGroup< int >::iterator itDia = diasLetivos.begin();
					for (; itDia != diasLetivos.end(); itDia++ )
					{
						Constraint c;
						c.reset();
						c.setType( Constraint::C_VAR_G );
						c.setDisciplina( d );
						c.setOferta( oft );
						c.setDia( *itDia );
						c.setSubCjtSala( *itCjtSala );

						sprintf( name, "%s", c.toString().c_str() ); 

						if ( cHash.find( c ) != cHash.end() )
						{
							continue;
						}

						nnz = 5;
						OPT_ROW row( nnz, OPT_ROW::LESS , 0.0, name );
				
						Variable v;
						v.reset();
						v.setType( Variable::V_MIN_HOR_DISC_OFT_DIA ); // g_{d,oft,t}
						v.setDisciplina(d);
						v.setOferta(oft);
						v.setDia(*itDia);

						it_v = vHash.find( v );
						if ( it_v != vHash.end() )
						{
							row.insert( it_v->second, -1 );
						}
						else
						{
							continue;
						}
						
						bool found = false;

						#pragma region Numero de creditos da disciplina d alocados para oft, no dia t, para a sala s
						for ( int turma = 0; turma < d->getNumTurmas(); turma++ )
						{
							Variable v;
							v.reset();
							v.setType( Variable::V_CREDITOS_OFT ); // q_{i,d,oft,u,s,t}
							v.setTurma(turma);
							v.setDisciplina(d);
							v.setOferta(oft);
							v.setUnidade(*itUnid);
							v.setSubCjtSala(*itCjtSala);
							v.setDia(*itDia);

							it_v = vHash.find( v );
							if ( it_v != vHash.end() )
							{
								row.insert( it_v->second, 1 );
								found = true;
							}
						}
						#pragma endregion 

						if ( !found )
						{
							continue; // S� cria a restri��o se existir pelo menos 1 variavel q
						}

						if ( row.getnnz() != 0 )
						{
							cHash[ c ] = lp->getNumRows();
							lp->addRow( row );
							restricoes++;
						}
					}
				}
			}
		}
   }
   return restricoes;
}





/*====================================================================/
%DocBegin TRIEDA_LOAD_MODEL

%Constraint 
Evita sobreposi��o de sala por turmas de uma mesma disciplina e mesmo bloco
curricular
%Desc 

%MatExp

\begin{eqnarray}
	
	 \sum\limits_{d \in D_{bc}} ( g_{d,oft,t} ) \le MaxCred_{oft,t} \qquad

\forall bc \in oft
\forall oft \in O
\forall u \in U \quad
\forall s \in S_{u} \quad
\forall t \in T \quad
\end{eqnarray}

%DocEnd
/====================================================================*/
int SolverMIP::cria_restricao_evita_sobrepos_sala_por_div_turmas( int campusId )
{
   int restricoes = 0;
   char name[ 100 ];
   int nnz;
   
   VariableHash::iterator it_v; 

   // Para cada oferta
   ITERA_GGROUP_LESSPTR( itOft, problemData->ofertas, Oferta )
   {
	    Oferta *oft = *itOft;

		if ( oft->getCampusId() != campusId )
		{
			continue;
		}

		// Para cada dia
		GGroup< int >::iterator itDia = oft->campus->diasLetivos.begin();
		for (; itDia != oft->campus->diasLetivos.end(); itDia++ )
		{
			int dia = *itDia;

			// Para cada bloco curricular da oferta
			ITERA_GGROUP_LESSPTR( itBloco, problemData->blocos, BlocoCurricular )
			{	
				if ( itBloco->curriculo->getId() != oft->getCurriculoId() )
				{
					continue;
				}

				BlocoCurricular *bc = *itBloco;
				int periodo = bc->getPeriodo();
			
				GGroup< Calendario*, LessPtr<Calendario> > calendarios = itBloco->curriculo->retornaSemanasLetivasNoPeriodo( periodo );

				ITERA_GGROUP_LESSPTR( itSL, calendarios, Calendario )
				{
					Calendario *sl = *itSL;
				
					Constraint c;
					c.reset();
					c.setType( Constraint::C_EVITA_SOBREPOS_SALA_POR_TURMA );
					c.setOferta( oft );
					c.setBloco( bc );
					c.setDia( dia );
					c.setSemanaLetiva( sl );

					sprintf( name, "%s", c.toString().c_str() ); 

					if ( cHash.find( c ) != cHash.end() )
					{
						continue;
					}

					nnz = 10;

					OPT_ROW row( nnz, OPT_ROW::LESS , 0, name );

					// Para cada disciplina de bc
					map < Disciplina*, int, LessPtr< Disciplina > >::iterator it_disc = oft->curriculo->disciplinas_periodo.begin();
					for (; it_disc != oft->curriculo->disciplinas_periodo.end(); it_disc++ )
					{
						Disciplina * d = it_disc->first;

						// Disciplina deve pertencer � mesma semana letiva corrente
						if ( d->getCalendario() != sl )
						{
							continue;
						}
						
						// Disciplinas devem pertencer ao per�odo do bloco curricular bc
						if ( it_disc->second != bc->getPeriodo() )
						{
							continue;
						}

						Variable v;
						v.reset();
						v.setType( Variable::V_MIN_HOR_DISC_OFT_DIA );
						v.setDisciplina(d);
						v.setOferta(oft);
						v.setDia(dia);

						it_v = vHash.find( v );
						if ( it_v != vHash.end() )
						{
							row.insert( it_v->second, 1 );
						}
					}

					std::map< Trio<int, int, Calendario*>, int >::iterator it_map = bc->combinaCredSL.begin();
					for ( ; it_map != bc->combinaCredSL.end(); it_map++  )
					{
						if ( it_map->first.first == dia && it_map->first.third == sl )
						{
							int k = it_map->first.second;
							
							Variable v;
							v.reset();
							v.setType( Variable::V_COMBINA_SL_BLOCO );
							v.setBloco( bc );
							v.setDia( dia );
							v.setCombinaSLBloco( k );

							it_v = vHash.find( v );
							if( it_v != vHash.end() )
							{
								int coef = bc->getNroMaxCredCombinaSL( k, sl, dia );

								row.insert( it_v->second, -coef );
							}
						}
					}

					if ( row.getnnz() != 0 )
					{
						cHash[ c ] = lp->getNumRows();
						lp->addRow( row );
						restricoes++;
					}
				}
			}
		}
   }

   return restricoes;
}

// Restricoes 1.2.44, 1.2.45, 1.2.46
/*====================================================================/
%DocBegin TRIEDA_LOAD_MODEL

%Constraint 
Ativa��o da vari�vel q
%Desc 

%MatExp

\begin{eqnarray}
	
	 q_{i,d,oft,u,s,t} \eq x_{i,d,u,s,t} \cdot e_{i,d,oft} \nonumber \qquad
	  
	  =  \qquad

	 q_{i,d,oft,u,s,t} \le M \cdot e_{i,d,oft} \nonumber \qquad
	 q_{i,d,oft,u,s,t} \ge x_{i,d,u,s,t} - M \cdot ( 1 - e_{i,d,oft} ) \nonumber \qquad
	 x_{i,d,u,s,t} \ge q_{i,d,oft,u,s,t} - M \cdot ( 1 - e_{i,d,oft} ) \nonumber \qquad

\forall i \in I_{d} \quad
\forall d \in D_{oft} \quad
\forall oft \in O \quad
\forall u \in U \quad
\forall s \in S_{u} \quad
\forall t \in T \quad
\end{eqnarray}

%Data M
%Desc
big $M$.

%DocEnd
/====================================================================*/
int SolverMIP::cria_restricao_ativacao_var_q( int campusId )
{
   int restricoes = 0;
   char name[ 100 ];
   int nnz;
   double M = 9999.0;
   
   VariableHash::iterator it_v;
   Variable v;
   Constraint c;
   
   ITERA_GGROUP_LESSPTR( itOft, problemData->ofertas, Oferta )
   {
	    Oferta *oft = *itOft;
		Campus *cp = oft->campus;

		if ( cp->getId() != campusId )
		{
			continue;
		}

	    map < Disciplina*, int, LessPtr< Disciplina > >::iterator it_disc = oft->curriculo->disciplinas_periodo.begin();
	    for (; it_disc != oft->curriculo->disciplinas_periodo.end(); it_disc++ )
	    {
			Disciplina * d = it_disc->first;

			// para cada unidade do campus de oft
			ITERA_GGROUP_LESSPTR( itUnid, cp->unidades, Unidade )
			{
				// para cada sala aonde poder� ser ministrada a disciplina
				ITERA_GGROUP_LESSPTR( itCjtSala, itUnid->conjutoSalas, ConjuntoSala )
				{   
					if ( itCjtSala->disciplinas_associadas.find( d ) ==
						 itCjtSala->disciplinas_associadas.end() )
						continue;

					GGroup< int > diasLetivos = itCjtSala->dias_letivos_disciplinas[ d ];
									
					// Para cada dia em que d pode ser ministrada em itCjtSala
					GGroup< int >::iterator itDia = diasLetivos.begin();
					for (; itDia != diasLetivos.end(); itDia++ )
					{
						for ( int turma = 0; turma < d->getNumTurmas(); turma++ )
						{
							bool vNotfound = false;
							// Se pelo menos 1 das variaveis n�o existir,
							// n�o tem sentido o trio de restri��es, logo nenhuma ser� criada.

							// ----------------------------------------------------------
							// 1
							#pragma region Restricao tipo 1

							c.reset();
							c.setType( Constraint::C_VAR_Q_1 );
							c.setTurma( turma );
							c.setDisciplina( d );
							c.setOferta( oft );
							c.setDia( *itDia );
							c.setUnidade( *itUnid );
							c.setSubCjtSala( *itCjtSala );

							sprintf( name, "%s", c.toString().c_str() ); 

							if ( cHash.find( c ) != cHash.end() )
							{
								continue;
							}

							nnz = 2;
							OPT_ROW row1( nnz, OPT_ROW::LESS , 0.0, name );

							v.reset();
							v.setType( Variable::V_CREDITOS_OFT );
							v.setTurma( turma );
							v.setDisciplina( d );
							v.setOferta( oft );
							v.setUnidade( *itUnid );
							v.setSubCjtSala( *itCjtSala );
							v.setDia( *itDia );

							it_v = vHash.find( v );
							if ( it_v != vHash.end() )
							{
								row1.insert( it_v->second, 1 );
							}
							else
							{
								vNotfound = true;
								continue;
							}

							v.reset();
							v.setType( Variable::V_ALOC_ALUNOS_OFT );
							v.setTurma( turma );
							v.setDisciplina( d );
							v.setOferta( oft );

							it_v = vHash.find( v );
							if ( it_v != vHash.end() )
							{
								row1.insert( it_v->second, -M );
							}
							else
							{
								vNotfound = true;
								continue;
							}

							#pragma endregion
										
							// ----------------------------------------------------------
							// 2
							#pragma region Restricao tipo 2
										
							c.reset();
							c.setType( Constraint::C_VAR_Q_2 );
							c.setTurma( turma );
							c.setDisciplina( d );
							c.setOferta( oft );
							c.setDia( *itDia );
							c.setUnidade( *itUnid );
							c.setSubCjtSala( *itCjtSala );

							sprintf( name, "%s", c.toString().c_str() ); 

							if ( cHash.find( c ) != cHash.end() )
							{
								continue;
							}

							nnz = 3;
							OPT_ROW row2( nnz, OPT_ROW::LESS , M, name );

							v.reset();
							v.setType( Variable::V_CREDITOS_OFT );
							v.setTurma( turma );
							v.setDisciplina( d );
							v.setOferta( oft );
							v.setUnidade( *itUnid );
							v.setSubCjtSala( *itCjtSala );
							v.setDia( *itDia );

							it_v = vHash.find( v );
							if ( it_v != vHash.end() )
							{
								row2.insert( it_v->second, -1 );
							}
							else
							{
								vNotfound = true;
								continue;
							}

							v.reset();
							v.setType( Variable::V_CREDITOS );
							v.setTurma( turma );
							v.setDisciplina( d );
							v.setUnidade( *itUnid );
							v.setSubCjtSala( *itCjtSala );
							v.setDia( *itDia );

							it_v = vHash.find( v );
							if ( it_v != vHash.end() )
							{
								row2.insert( it_v->second, 1 );
							}
							else
							{
								vNotfound = true;
								continue;
							}

							v.reset();
							v.setType( Variable::V_ALOC_ALUNOS_OFT );
							v.setTurma( turma );
							v.setDisciplina( d );
							v.setOferta( oft );

							it_v = vHash.find( v );
							if ( it_v != vHash.end() )
							{
								row2.insert( it_v->second, M );
							}
							else
							{
								vNotfound = true;
								continue;
							}

							#pragma endregion

							// ----------------------------------------------------------
							// 3
							#pragma region Restricao tipo 3

							c.reset();
							c.setType( Constraint::C_VAR_Q_3 );
							c.setTurma( turma );
							c.setDisciplina( d );
							c.setOferta( oft );
							c.setDia( *itDia );
							c.setUnidade( *itUnid );
							c.setSubCjtSala( *itCjtSala );

							sprintf( name, "%s", c.toString().c_str() ); 

							if ( cHash.find( c ) != cHash.end() )
							{
								continue;
							}

							nnz = 3;
							OPT_ROW row3( nnz, OPT_ROW::LESS , M, name );

							v.reset();
							v.setType( Variable::V_CREDITOS_OFT );
							v.setTurma( turma );
							v.setDisciplina( d );
							v.setOferta( oft );
							v.setUnidade( *itUnid );
							v.setSubCjtSala( *itCjtSala );
							v.setDia( *itDia );

							it_v = vHash.find( v );
							if ( it_v != vHash.end() )
							{
								row3.insert( it_v->second, 1 );
							}
							else
							{
								vNotfound = true;
								continue;
							}

							v.reset();
							v.setType( Variable::V_CREDITOS );
							v.setTurma( turma );
							v.setDisciplina( d );
							v.setUnidade( *itUnid );
							v.setSubCjtSala( *itCjtSala );
							v.setDia( *itDia );

							it_v = vHash.find( v );
							if ( it_v != vHash.end() )
							{
								row3.insert( it_v->second, -1 );
							}
							else
							{
								vNotfound = true;
								continue;
							}

							v.reset();
							v.setType( Variable::V_ALOC_ALUNOS_OFT );
							v.setTurma( turma );
							v.setDisciplina( d );
							v.setOferta( oft );

							it_v = vHash.find( v );
							if ( it_v != vHash.end() )
							{
								row3.insert( it_v->second, M );
							}
							else
							{
								vNotfound = true;
								continue;
							}

							#pragma endregion

							// Cria a restri��o tipo 1
							if ( row1.getnnz() != 0 )
							{
								cHash[ c ] = lp->getNumRows();
								lp->addRow( row1 );
								restricoes++;
							}
							// Cria a restri��o tipo 2																
							if ( row2.getnnz() != 0 )
							{
								cHash[ c ] = lp->getNumRows();
								lp->addRow( row2 );
								restricoes++;
							}
							// Cria a restri��o tipo 3
							if ( row3.getnnz() != 0 )
							{
								cHash[ c ] = lp->getNumRows();
								lp->addRow( row3 );
								restricoes++;
							}
						}
					}
				}
			}
		}
   }

   return restricoes;

}


int SolverMIP::cria_restricao_ativacao_var_cs( int campusId )
{
   int restricoes = 0;

   // Metodo somente utilizado quando h� 2 semanas letivas
   if ( problemData->calendarios.size() != 2 )
   {
	   return restricoes;
   }

   char name[ 100 ];   
   VariableHash::iterator it_v;
   Variable v;
   Constraint c;

   ITERA_GGROUP_LESSPTR( itCampus, problemData->campi, Campus )
   {
	   if ( itCampus->getId() != campusId )
	   {
		   continue;
	   }

      ITERA_GGROUP_LESSPTR( itUnidade, itCampus->unidades, Unidade )
      {
         ITERA_GGROUP_LESSPTR( itCjtSala, itUnidade->conjutoSalas, ConjuntoSala )
         {
			std::map< int, Sala * >::iterator itSala = itCjtSala->salas.begin();
			for( ; itSala != itCjtSala->salas.end(); itSala++ )
			{
				Sala *s = itSala->second;

				ITERA_GGROUP_N_PT( itDia, s->diasLetivos, int )
				{
					int dia = *itDia;

					c.reset();
					c.setType( Constraint::C_VAR_CS );
					c.setUnidade( *itUnidade );
					c.setSubCjtSala( *itCjtSala );
					c.setDia( dia );

					sprintf( name, "%s", c.toString().c_str() );
					if ( cHash.find( c ) != cHash.end() )
					{
						continue;
					}

					int nnz = s->getCombinaCredSLSize()[dia];
					
					OPT_ROW row( nnz, OPT_ROW::LESS , 1, name );

					std::map< Trio<int, int, Calendario*>, int >::iterator it_map = s->combinaCredSL.begin();
					for ( ; it_map != s->combinaCredSL.end(); it_map++ )
					{
						if ( it_map->first.first == dia )
						{
							v.reset();
							v.setType( Variable::V_COMBINA_SL_SALA );
							v.setSala( s );
							v.setDia( dia );
							v.setCombinaSL( it_map->first.second );

							it_v = vHash.find( v );
							if( it_v != vHash.end() )
							{
								row.insert( it_v->second, 1 );
							}
						}
					}
					
					if ( row.getnnz() != 0 )
					{
						cHash[ c ] = lp->getNumRows();
						lp->addRow( row );
						restricoes++;
					}
				}
			}
		 }
	  }
   }
   return restricoes;
}


/*
	Impede que ofertas que possuem disciplinas de diferentes semanas letivas
	tenham turmas compartilhadas.
	Essa restri��o s� � necess�ria para que a restri��o que evita sobreposi��o
	de sala por compartilhamento de turma
	(int cria_restricao_evita_sobrepos_sala_por_div_turmas(void)) n�o falhe.
*/
int SolverMIP::cria_restricao_fixa_nao_compartilhamento( int campusId )
{
   int restricoes = 0;
   
   #pragma region Condi��es para criar as restri��es
   
   // Se o compartilhamento n�o � permitido, nenhuma restri��o � criada.
   if ( !problemData->parametros->permite_compartilhamento_turma_sel )
   {
	   return restricoes;
   }   

   // Metodo somente utilizado quando h� 2 semanas letivas
   if ( problemData->calendarios.size() != 2 )
   {
	   return restricoes;
   }
   
   #pragma endregion
   
   char name[ 200 ];
   int nnz;

   Constraint c;
   Variable v;
   VariableHash::iterator it_v;
   
   // para cada campus
   ITERA_GGROUP_LESSPTR( itCampus, problemData->campi, Campus )
   {
	   Campus *cp = *itCampus;

	   if ( cp->getId() != campusId )
	   {
		   continue;
	   }

	   // para cada par de cursos compativeis
	   std::map< std::pair< Curso *, Curso * >, std::vector< int > >::iterator
               it_cursoComp_disc = problemData->cursosComp_disc.begin();
        for (; it_cursoComp_disc != problemData->cursosComp_disc.end(); it_cursoComp_disc++ )
        {
			Curso* c1 = it_cursoComp_disc->first.first;
			Curso* c2 = it_cursoComp_disc->first.second;
			
			if ( cp->cursos.find( c1 ) == cp->cursos.end() ||
				 cp->cursos.find( c2 ) == cp->cursos.end() )
			{
				continue;
			}
			
			// para cada disciplina em comum (possivel de ser compartilhada) ao par de cursos
            std::vector< int >::iterator it_discComum = it_cursoComp_disc->second.begin();
            for (; it_discComum != it_cursoComp_disc->second.end(); ++it_discComum )
            {
				Disciplina * discComum = problemData->retornaDisciplina( *it_discComum );
				  
				if (discComum == NULL) continue;

				#pragma region Equivalencias
				if ( problemData->mapDiscSubstituidaPor.find( discComum ) !=
					problemData->mapDiscSubstituidaPor.end() )
				{
					continue;
				}
				#pragma endregion
				
				GGroup<Oferta*, LessPtr<Oferta>> ofts1 = cp->retornaOfertasComCursoDisc( c1->getId(), discComum );
				GGroup<Oferta*, LessPtr<Oferta>> ofts2 = cp->retornaOfertasComCursoDisc( c2->getId(), discComum );

				// para cada oferta contendo discComum do curso c1
				ITERA_GGROUP_LESSPTR( itOft1, ofts1, Oferta )
				{
					Oferta *oft1 = *itOft1;
					int periodo1 = oft1->periodoDisciplina( discComum );
					
					if ( oft1->getCampusId() != campusId )
					{
						continue;
					}
					
					GGroup< Calendario*, LessPtr<Calendario> > semanasLetivas1 = oft1->curriculo->retornaSemanasLetivasNoPeriodo( periodo1 );
					
					// para cada oferta contendo discComum do curso c2
					ITERA_GGROUP_LESSPTR( itOft2, ofts2, Oferta )
					{		
						Oferta *oft2 = *itOft2;
						int periodo2 = oft2->periodoDisciplina( discComum );
					
						if ( oft2->getCampusId() != campusId )
						{
							continue;
						}						

						GGroup< Calendario*, LessPtr<Calendario> > semanasLetivas2 =  oft2->curriculo->retornaSemanasLetivasNoPeriodo( periodo2 );

						// Cria a restri��o somente para per�odos que possuem mais de uma semana letiva, ou sl's diferentes
						if ( semanasLetivas1.size() == 1 &&
							 semanasLetivas2.size() == 1 &&
							 semanasLetivas1.begin()->getId() == semanasLetivas2.begin()->getId() )
						{
							continue;
						}

						// para cada turma da disciplina em comum
						for ( int turma = 0; turma < discComum->getNumTurmas(); turma++ )
						{
							c.reset();
							c.setType( Constraint::C_FIXA_NAO_COMPARTILHAMENTO );
							c.setParOfertas( std::make_pair(oft1, oft2) );
							c.setDisciplina( discComum );
							c.setTurma( turma );

							sprintf( name, "%s", c.toString().c_str() ); 

							if ( cHash.find( c ) != cHash.end() )
							{
								continue;
							}

							nnz = 1;

							OPT_ROW row( nnz, OPT_ROW::EQUAL , 0.0 , name );

							v.reset();
							v.setType( Variable::V_ALOC_ALUNOS_PAR_OFT );
							v.setTurma( turma );
							v.setDisciplina( discComum );
							v.setParOfertas( oft1, oft2 );

							it_v = vHash.find( v );
							if( it_v != vHash.end() )
							{
								row.insert( it_v->second, 1 );
							}
							
							if ( row.getnnz() != 0 )
							{
								cHash[ c ] = lp->getNumRows();
								lp->addRow( row );
								restricoes++;
							}
						}
					}
				}
			}
		}
   }
   return restricoes;
}


int SolverMIP::cria_restricao_ativacao_var_cbc( int campusId )
{
   int restricoes = 0;

   char name[ 100 ];   
   VariableHash::iterator it_v;
   Variable v;
   Constraint c;

   ITERA_GGROUP_LESSPTR( itBloco, problemData->blocos, BlocoCurricular )
   {
	   BlocoCurricular *bc = *itBloco;
		
	   if ( bc->campus->getId() != campusId )
	   {
		   continue;
	   }

	   ITERA_GGROUP_N_PT( itDia, bc->diasLetivos, int )
	   {
		   int dia = *itDia;

			c.reset();
			c.setType( Constraint::C_VAR_CBC );
			c.setBloco( *itBloco );
			c.setDia( dia );

			sprintf( name, "%s", c.toString().c_str() );
			if ( cHash.find( c ) != cHash.end() )
			{
				continue;
			}

			int nnz = bc->getCombinaCredSLSize()[dia];
					
			OPT_ROW row( nnz, OPT_ROW::LESS , 1, name );

			std::map< Trio< int/*dia*/, int /*k_id*/, Calendario* /*sl*/ >,
					  int/*nroCreds*/ >::iterator it_map = bc->combinaCredSL.begin();

			for ( ; it_map != bc->combinaCredSL.end(); it_map++ )
			{
				if ( it_map->first.first == dia )
				{
					Variable v;
					v.reset();
					v.setType( Variable::V_COMBINA_SL_BLOCO );

					v.setBloco( bc );
					v.setDia( dia );
					v.setCombinaSLBloco( it_map->first.second );
					
					it_v = vHash.find( v );
					if( it_v != vHash.end() )
					{
						row.insert( it_v->second, 1 );
					}
				}
			}

			if ( row.getnnz() != 0 )
			{
				cHash[ c ] = lp->getNumRows();
				lp->addRow( row );
				restricoes++;
			}
		}
   }

   return restricoes;
}



void SolverMIP::cria_solucao_inicial( int cnt, int * indices, double * valores )
{
   VariableHash::iterator itVHash = vHash.begin();

   for (; itVHash != vHash.end(); itVHash++ )
   {
      if ( itVHash->first.getType() != Variable::V_SLACK_DEMANDA )
      {
         indices[ cnt ] = itVHash->second;
         valores[ cnt ] = 0;
         cnt++;
      }
      else
      {
         indices[ cnt ] = itVHash->second;
         valores[ cnt ] = -1;
         cnt++;
      }
   }
}

int SolverMIP::criaVariaveisOperacional()
{
   int numVars = 0;

#ifdef PRINT_cria_variaveis
   int numVarsAnterior = 0;
#endif

   numVars += criaVariavelProfessorAulaHorario(); 

#ifdef PRINT_cria_variaveis
   std::cout << "numVars V_X_PROF_AULA_HOR: "
             << ( numVars - numVarsAnterior ) << std::endl;
   numVarsAnterior = numVars;
#endif

   lp->updateLP();
   numVars += criaVariavelProfessorDisciplina(); 

#ifdef PRINT_cria_variaveis
   std::cout << "numVars V_Y_PROF_DISCIPLINA: "
             << ( numVars - numVarsAnterior ) << std::endl;
   numVarsAnterior = numVars;
#endif

   lp->updateLP();
   //numVars += criaVariavelDisciplinaHorario(); 

#ifdef PRINT_cria_variaveis
   std::cout << "numVars V_Z_DISCIPLINA_HOR: "
             << ( numVars - numVarsAnterior ) << std::endl;
   numVarsAnterior = numVars;
#endif

   lp->updateLP();
   //numVars += criaVariavelFolgaDisciplinaHorario(); 

#ifdef PRINT_cria_variaveis
   std::cout << "numVars V_F_DISC_HOR: "
             << ( numVars - numVarsAnterior ) << std::endl;
   numVarsAnterior = numVars;
#endif

   lp->updateLP();
   numVars += criaVariavelFolgaFixProfDiscSalaDiaHor(); 

#ifdef PRINT_cria_variaveis
   std::cout << "numVars V_F_FIX_PROF_DISC_SALA_DIA_HOR: "
             << ( numVars - numVarsAnterior ) << std::endl;
   numVarsAnterior = numVars;
#endif

   lp->updateLP();
   numVars += criaVariavelFolgaFixProfDiscDiaHor(); 

#ifdef PRINT_cria_variaveis
   std::cout << "numVars V_F_FIX_PROF_DISC_DIA_HOR: "
             << ( numVars - numVarsAnterior ) << std::endl;
   numVarsAnterior = numVars;
#endif

   lp->updateLP();
   numVars += criaVariavelFolgaFixProfDisc(); 

#ifdef PRINT_cria_variaveis
   std::cout << "numVars V_F_FIX_PROF_DISC: "
             << ( numVars - numVarsAnterior ) << std::endl;
   numVarsAnterior = numVars;
#endif

   lp->updateLP();
   numVars += criaVariavelFolgaFixProfDiscSala(); 

#ifdef PRINT_cria_variaveis
   std::cout << "numVars V_F_FIX_PROF_DISC_SALA: "
             << ( numVars - numVarsAnterior ) << std::endl;
   numVarsAnterior = numVars;
#endif

   lp->updateLP();
   numVars += criaVariavelFolgaFixProfSala();

#ifdef PRINT_cria_variaveis
   std::cout << "numVars V_F_FIX_PROF_SALA: "
             << ( numVars - numVarsAnterior ) << std::endl;
   numVarsAnterior = numVars;
#endif

   lp->updateLP();
   numVars += criaVariavelProfessorCurso();

#ifdef PRINT_cria_variaveis
   std::cout << "numVars V_PROF_CURSO: "
             << ( numVars - numVarsAnterior ) << std::endl;
   numVarsAnterior = numVars;
#endif

   lp->updateLP();
   numVars += criaVariavelAvaliacaoCorpoDocente();

#ifdef PRINT_cria_variaveis
   std::cout << "numVars V_AVALIACAO_CORPO_DOCENTE: "
             << ( numVars - numVarsAnterior ) << std::endl;
   numVarsAnterior = numVars;
#endif

   lp->updateLP();
   numVars += criaVariavelCustoCorpoDocente();

#ifdef PRINT_cria_variaveis
   std::cout << "numVars V_CUSTO_CORPO_DOCENTE: "
             << ( numVars - numVarsAnterior ) << std::endl;
   numVarsAnterior = numVars;
#endif

   lp->updateLP();
   numVars += criaVariavelDiasProfessoresMinistramAulas();

#ifdef PRINT_cria_variaveis
   std::cout << "numVars V_DIAS_PROF_MINISTRA_AULAS: "
             << ( numVars - numVarsAnterior ) << std::endl;
   numVarsAnterior = numVars;
#endif

   lp->updateLP();
   numVars += criaVariavelFolgaMinimoMestresCurso();

#ifdef PRINT_cria_variaveis
   std::cout << "numVars V_F_MIN_MEST_CURSO: "
             << ( numVars - numVarsAnterior ) << std::endl;
   numVarsAnterior = numVars;
#endif

   lp->updateLP();
   numVars += criaVariavelFolgaMinimoDoutoresCurso();

#ifdef PRINT_cria_variaveis
   std::cout << "numVars V_F_MIN_DOUT_CURSO: "  
             << ( numVars - numVarsAnterior ) << std::endl;
   numVarsAnterior = numVars;
#endif

   lp->updateLP();
   numVars += criaVariavelMaxDiscProfCurso();

#ifdef PRINT_cria_variaveis
   std::cout << "numVars V_MAX_DISC_PROF_CURSO: "
             << ( numVars - numVarsAnterior ) << std::endl;
   numVarsAnterior = numVars;
#endif

   lp->updateLP();
   numVars += criaVariavelFolgaMaxDiscProfCurso();

#ifdef PRINT_cria_variaveis
   std::cout << "numVars V_F_MAX_DISC_PROF_CURSO: "
             << ( numVars - numVarsAnterior ) << std::endl;
   numVarsAnterior = numVars;
#endif

   lp->updateLP();
   numVars += criaVariavelFolgaCargaHorariaMinimaProfessor();

#ifdef PRINT_cria_variaveis
   std::cout << "numVars V_F_CARGA_HOR_MIN_PROF: "
             << ( numVars - numVarsAnterior ) << std::endl;
   numVarsAnterior = numVars;
#endif

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

   lp->updateLP();
   numVars += criaVariavelGapsProfessores();

#ifdef PRINT_cria_variaveis
   std::cout << "numVars V_GAPS_PROFESSORES: "
             << ( numVars - numVarsAnterior ) << std::endl;
   numVarsAnterior = numVars;
#endif

   lp->updateLP();
   numVars += criaVariavelFolgaUltimaPrimeiraAulas();

#ifdef PRINT_cria_variaveis
   std::cout << "numVars V_F_ULTIMA_PRIMEIRA_AULA_PROF: "
             << ( numVars - numVarsAnterior ) << std::endl;
   numVarsAnterior = numVars;
#endif

   lp->updateLP();
   numVars += criaVariavelFolgaDemanda();

#ifdef PRINT_cria_variaveis
   std::cout << "numVars V_FOLGA_DEMANDA: "
             << ( numVars - numVarsAnterior ) << std::endl;
   numVarsAnterior = numVars;
#endif   

   lp->updateLP();
   //numVars += criaVariavelFolgaDisciplinaTurmaHorario();

#ifdef PRINT_cria_variaveis
   std::cout << "numVars V_FOLGA_DISC_TURMA_HOR: "
             << ( numVars - numVarsAnterior ) << std::endl;
   numVarsAnterior = numVars;
#endif

   lp->updateLP();

   return numVars;
}

int SolverMIP::criaVariavelProfessorAulaHorario( void )
{
   int num_vars = 0;
   double coeff = 0.0;

   GGroup< Professor *, LessPtr< Professor > > professores
      = problemData->getProfessores();

   ITERA_GGROUP_LESSPTR( itProfessor, professores, Professor )
   {
      ITERA_GGROUP_LESSPTR( itMagisterio, itProfessor->magisterio, Magisterio )
      {
         Disciplina * discMinistradaProf = itMagisterio->disciplina;

         ITERA_GGROUP_LESSPTR( itAula, problemData->aulas, Aula )
         {
            Disciplina * discAula = itAula->getDisciplina();
			
            if ( discMinistradaProf != discAula )
            {
               continue;
            } 

            std::pair< int, int > prof_disc( itProfessor->getId(), discMinistradaProf->getId() );
						
            // Se o professor e a disciplina da aula em quest�o se relacionarem:
            if ( problemData->prof_Disc_Dias.find( prof_disc )
                  == problemData->prof_Disc_Dias.end() )
            {
               continue;
            }

            // Retorna lista de horarios possiveis para o professor, aula e sala
            std::list< HorarioDia * > listaHorarios;
			
            retornaHorariosPossiveis( *itProfessor, *itAula, listaHorarios );

            for ( std::list< HorarioDia * >::iterator itHor = listaHorarios.begin();
                  itHor != listaHorarios.end(); itHor++ )
            {
               HorarioDia * horarioDia = ( *itHor );

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
                    vHashOp[ v ] = lp->getNumCols();

					OPT_COL col( OPT_COL::VAR_BINARY, coeff, 0.0, 1.0,
						( char * )v.toString().c_str() );

					lp->newCol( col );
					num_vars++;				  
               }
            }
         }
	  }
   }

   // Professores virtuais para as aulas
   ITERA_GGROUP_LESSPTR( it_aula, problemData->aulas, Aula )
   {
      Aula * aula = ( *it_aula );
      Disciplina * discAula = aula->getDisciplina();

      // Retorna lista de horarios possiveis para o professor, aula e sala
      std::list< HorarioDia * > listaHorarios;

      retornaHorariosPossiveis( NULL, aula, listaHorarios );

      for ( std::list< HorarioDia * >::iterator itHor = listaHorarios.begin();
            itHor != listaHorarios.end(); itHor++ )
      {
         HorarioDia * horarioDia = ( *itHor );

         VariableOp v;
         v.reset();
         v.setType( VariableOp::V_X_PROF_AULA_HOR );

         v.setAula( aula ); 
         v.setProfessor( NULL );
         v.setHorario( horarioDia );
         v.setHorarioAula( horarioDia->getHorarioAula() );
         v.setDia( horarioDia->getDia() );
         v.setDisciplina( aula->getDisciplina() );
         v.setTurma( aula->getTurma() );
         v.setSala( aula->getSala() );
		 v.setUnidade( problemData->refUnidade[ aula->getSala()->getIdUnidade() ] );

         double coeff = 40000.0;

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

int SolverMIP::criaVariavelProfessorDisciplina()
{
   int num_vars = 0;

   double coeff = 0.0;
   double pesoNota = 100;
   double pesoPreferencia = 100;

   GGroup< Professor *, LessPtr< Professor > > professores
      = problemData->getProfessores();

   ITERA_GGROUP_LESSPTR( itProfessor, professores, Professor )
   {
      ITERA_GGROUP_LESSPTR( itMagisterio, itProfessor->magisterio, Magisterio )
      {
         Disciplina * discMinistradaProf = itMagisterio->disciplina;

         int nota = itMagisterio->getNota();
         int preferencia = itMagisterio->getPreferencia();

         ITERA_GGROUP_LESSPTR( itAula, problemData->aulas, Aula )
         {
            Disciplina * discAula = itAula->getDisciplina();

            if ( discMinistradaProf != discAula )
            {
               continue;
            }

            std::pair< int, int > prof_disc ( itProfessor->getId(),
               discMinistradaProf->getId() );

            // Se o professor e a disciplina da aula em quest�o se relacionarem:
            if ( problemData->prof_Disc_Dias.find( prof_disc )
                  == problemData->prof_Disc_Dias.end() )
            {
               continue;
            }

            VariableOp v;
            v.reset();
            v.setType( VariableOp::V_Y_PROF_DISCIPLINA );

            v.setDisciplina( discAula ); 
            v.setProfessor( ( *itProfessor ) );
            v.setTurma( itAula->getTurma() );

            //------------------------------------------------------
            // Prefer�ncia:
            // 1  --> Maior prefer�ncia
            // 10 --> Menor prefer�ncia
            //------------------------------------------------------
            // Nota de desempenho:
            // 1  --> Menor desempenho
            // 10 --> Maior desempenho
            //------------------------------------------------------
            // Assim, o 'peso' da prefer�ncia e da nota
            // de desempenho na fun��o objetivo variam
            // entre 0 e 9, sendo 0 o MELHOR CASO e 9 o PIOR CASO
            //------------------------------------------------------
            coeff = ( ( 10 - nota ) * ( pesoNota )
               + ( preferencia - 1 ) * ( pesoPreferencia ) );

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

   return num_vars;
}

int SolverMIP::criaVariavelDisciplinaHorario()
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

void SolverMIP::retornaHorariosPossiveis( Professor * prof,
   Aula * aula, std::list< HorarioDia * > & listaHor )
{
   listaHor.clear();

   // Verifica-se a disponibilidade do
   // professor, da disciplina e da sala
   if ( prof != NULL )
   {
      Disciplina * disc = aula->getDisciplina();
      Sala * sala = aula->getSala();

      int nCred = this->problemData->totalHorariosTurnoAula( aula );
      int diaS = aula->getDiaSemana();
      int nCredAula = aula->getTotalCreditos();

      for ( int i = 0; i <= problemData->maxHorariosDif; i++ )
      {
         int idx = problemData->getHorarioDiaIdx( diaS, i );

         HorarioDia * horarioDia = problemData->horariosDiaIdx[ idx ];

         if ( horarioDia == NULL )
         {
            continue;
         }

         // Verifica nCred horarios para frente
         bool horarioOK = true;

         // Verifica se disciplina, sala e professor possuem o horario
         if ( sala->horariosDia.find( horarioDia ) == sala->horariosDia.end() )
         {
            continue;
         }

         if ( disc->horariosDia.find( horarioDia ) == disc->horariosDia.end() )
         {
            continue;
         }

         if ( prof->horariosDia.find( horarioDia ) == prof->horariosDia.end() )
         {
            continue;
         }

         int nCredAux = 1;

         for ( int j = i + 1; j <= problemData->maxHorariosDif && nCredAux < nCred && nCredAux < nCredAula; j++ )
         {
            int idx2 = problemData->getHorarioDiaIdx( diaS, j );

            HorarioDia * horarioDia2 = problemData->horariosDiaIdx[ idx2 ];

            if ( horarioDia2 == NULL )
            {
               horarioOK = false;
               break;
            }

            // Verifica se disciplina e sala possuem o horario
            if ( sala->horariosDia.find( horarioDia2 ) == sala->horariosDia.end() )
            {
               horarioOK = false;
               break;
            }

            if ( disc->horariosDia.find( horarioDia2 ) == disc->horariosDia.end() )
            {
               horarioOK = false;
               break;
            }

            if ( prof->horariosDia.find( horarioDia2 ) == prof->horariosDia.end() )
            {
               horarioOK = false;
               break;
            }

            nCredAux++;
         }

         if ( nCredAux < nCredAula )
         {
            horarioOK = false;
         }

         if ( horarioOK )
         {
            bool verificaDisciplinaHorario = this->problemData->verificaDisponibilidadeDisciplinaHorario(
               aula->getDisciplina(), horarioDia->getHorarioAula() );

            if ( verificaDisciplinaHorario )
            {
               listaHor.push_back( horarioDia );
            }
         }
      }
   }
   // Como se trata de um professor virtual, iremos
   // verificar apenas a disponibilidade da disciplina e da sala
   else
   {
      Disciplina * disc = aula->getDisciplina();
      Sala * sala = aula->getSala();

      int nCred = this->problemData->totalHorariosTurnoAula( aula );
      int diaS = aula->getDiaSemana();
      int nCredAula = aula->getTotalCreditos();

      for ( int i = 0; i <= problemData->maxHorariosDif; i++ )
      {
         int idx = problemData->getHorarioDiaIdx( diaS, i );

         HorarioDia * horarioDia = problemData->horariosDiaIdx[ idx ];

         if ( horarioDia == NULL )
         {
            continue;
         }

         // Verifica nCred horarios para frente
         bool horarioOK = true;

         // Verifica se disciplina e sala possuem o horario
         if ( sala->horariosDia.find( horarioDia ) == sala->horariosDia.end() )
         {
            continue;
         }

         if ( disc->horariosDia.find( horarioDia ) == disc->horariosDia.end() )
         {
            continue;
         }

         int nCredAux = 1;

         for ( int j = i + 1; j <= problemData->maxHorariosDif && nCredAux < nCred && nCredAux < nCredAula; j++ )
         {
            int idx2 = problemData->getHorarioDiaIdx( diaS, j );

            HorarioDia * horarioDia2 = problemData->horariosDiaIdx[ idx2 ];

            if ( horarioDia2 == NULL )
            {
               horarioOK = false;
               break;
            }

            // Verifica se disciplina e sala possuem o horario
            if ( sala->horariosDia.find( horarioDia2 ) == sala->horariosDia.end() )
            {
               horarioOK = false;
               break;
            }

            if ( disc->horariosDia.find( horarioDia2 ) == disc->horariosDia.end() )
            {
               horarioOK = false;
               break;
            }

            nCredAux++;
         }

         if ( nCredAux < nCredAula )
         {
            horarioOK = false;
         }

         if ( horarioOK )
         {
            bool verificaDisciplinaHorario = this->problemData->verificaDisponibilidadeDisciplinaHorario(
               aula->getDisciplina(), horarioDia->getHorarioAula() );

            if ( verificaDisciplinaHorario )
            {
               listaHor.push_back( horarioDia );
            }
         }
      }
   }
}

int SolverMIP::criaVariavelFolgaFixProfDiscSalaDiaHor()
{
   int num_vars = 0;
   double coeff = 1000.0;

   ITERA_GGROUP_LESSPTR( itFix,
      problemData->fixacoes_Prof_Disc_Sala_Dia_Horario, Fixacao )
   {
      Fixacao * fixacao = ( *itFix );

      VariableOp v;
      v.reset();
      v.setType( VariableOp::V_F_FIX_PROF_DISC_SALA_DIA_HOR );
      v.setDisciplina( fixacao->disciplina );
      v.setProfessor( fixacao->professor );
      v.setSala( fixacao->sala );

      HorarioDia * auxHD = new HorarioDia();
      auxHD->setDia( fixacao->getDiaSemana() );
      auxHD->setHorarioAula( fixacao->horario_aula );
      auxHD->setHorarioAulaId( fixacao->getHorarioAulaId() );

      GGroup< HorarioDia *, LessPtr< HorarioDia > >::iterator
         itH = problemData->horariosDia.find( auxHD );
      delete auxHD;

      if ( itH == problemData->horariosDia.end() )
      {
         printf( "ERRO: HORARIO NAO ENCONTRADO NA FIXACAO\n" );
         continue;
      }

      v.setHorario( *itH );

      if ( vHashOp.find( v ) == vHashOp.end() )
      {
         vHashOp[v] = lp->getNumCols();

         OPT_COL col( OPT_COL::VAR_BINARY, coeff, 0.0, 1.0,
            ( char * )v.toString().c_str() );

         lp->newCol( col );
         num_vars++;
      }
   }

   return num_vars;
}

int SolverMIP::criaVariavelFolgaFixProfDiscDiaHor()
{
   int num_vars = 0;
   double coeff = 1000.0;

   ITERA_GGROUP_LESSPTR( itFix,
      problemData->fixacoes_Prof_Disc_Dia_Horario, Fixacao )
   {
      Fixacao * fixacao = ( *itFix );

      VariableOp v;
      v.reset();
      v.setType( VariableOp::V_F_FIX_PROF_DISC_DIA_HOR );
      v.setDisciplina( fixacao->disciplina );
      v.setProfessor( fixacao->professor );

      HorarioDia * auxHD = new HorarioDia();
      auxHD->setDia( fixacao->getDiaSemana() );
      auxHD->setHorarioAula( fixacao->horario_aula );
      auxHD->setHorarioAulaId( fixacao->getHorarioAulaId() );

      GGroup< HorarioDia *, LessPtr< HorarioDia > >::iterator
         itH = problemData->horariosDia.find( auxHD );
      delete auxHD;

      if ( itH == problemData->horariosDia.end() )
      {
         printf( "ERRO em SolverMIP::criaVariavelFolgaFixProfDiscDiaHor(): HORARIO NAO ENCONTRADO NA FIXACAO\n" );
         continue;
      }

      v.setHorario( *itH );

      if ( vHashOp.find( v ) == vHashOp.end() )
      {
         vHashOp[ v ] = lp->getNumCols();

         OPT_COL col( OPT_COL::VAR_BINARY, coeff, 0.0, 1.0,
            ( char * )v.toString().c_str() );

         lp->newCol( col );
         num_vars++;
      }
   }

   return num_vars;
}
   
int SolverMIP::criaVariavelFolgaFixProfDisc()
{
   int num_vars = 0;
   double coeff = 1000.0;

   ITERA_GGROUP_LESSPTR( itFix,
      problemData->fixacoes_Prof_Disc, Fixacao )
   {
      Fixacao * fixacao = ( *itFix );

      VariableOp v;
      v.reset();
      v.setType( VariableOp::V_F_FIX_PROF_DISC );
      v.setDisciplina( fixacao->disciplina );
      v.setProfessor( fixacao->professor );

      if ( vHashOp.find( v ) == vHashOp.end() )
      {
         vHashOp[v] = lp->getNumCols();

         OPT_COL col( OPT_COL::VAR_BINARY, coeff, 0.0, 1.0,
            ( char * )v.toString().c_str() );

         lp->newCol( col );
         num_vars++;
      }
   }

   return num_vars;
}

int SolverMIP::criaVariavelFolgaFixProfDiscSala()
{
   int num_vars = 0;
   double coeff = 1000.0;

   ITERA_GGROUP_LESSPTR( itFix,
      problemData->fixacoes_Prof_Disc_Sala, Fixacao )
   {
      Fixacao * fixacao = ( *itFix );

      VariableOp v;
      v.reset();
      v.setType( VariableOp::V_F_FIX_PROF_DISC_SALA );
      v.setDisciplina( fixacao->disciplina );
      v.setProfessor( fixacao->professor );

      if ( vHashOp.find( v ) == vHashOp.end() )
      {
         vHashOp[v] = lp->getNumCols();

         OPT_COL col( OPT_COL::VAR_BINARY, coeff, 0.0, 1.0,
            ( char * ) v.toString().c_str() );

         lp->newCol( col );
         num_vars++;
      }
   }

   return num_vars;
}
   
int SolverMIP::criaVariavelFolgaFixProfSala()
{
   int num_vars = 0;

   ITERA_GGROUP_LESSPTR( itFix,
      problemData->fixacoes_Prof_Sala, Fixacao )
   {
      Fixacao * fixacao = ( *itFix );

      VariableOp v;
      v.reset();
      v.setType( VariableOp::V_F_FIX_PROF_SALA );
      v.setSala(fixacao->sala);
      v.setProfessor(fixacao->professor);

      double coeff = 100000.0;

      if ( vHashOp.find(v) == vHashOp.end() )
      {
         vHashOp[v] = lp->getNumCols();

         OPT_COL col( OPT_COL::VAR_INTEGRAL, coeff, 0.0, OPT_INF,
            ( char * )v.toString().c_str() );

         lp->newCol( col );
         num_vars++;
      }
   }

   return num_vars;
}

int SolverMIP::criaVariavelFolgaDisciplinaHorario()
{
   int num_vars = 0;

   ITERA_GGROUP_LESSPTR( itAula, problemData->aulas, Aula )
   {
      Disciplina * discAula = itAula->getDisciplina();

      // Retorna lista de horarios possiveis para o professor, aula e sala
      std::list< HorarioDia * > listaHorarios;

      retornaHorariosPossiveis( NULL, *itAula, listaHorarios );

      for ( std::list< HorarioDia * >::iterator itHor = listaHorarios.begin();
            itHor != listaHorarios.end(); itHor++ )
      {
         HorarioDia * horarioDia = *itHor;

         VariableOp v;
         v.reset();
         v.setType( VariableOp::V_F_DISC_HOR );

         Aula * aula = ( *itAula );

         v.setHorarioAula( horarioDia->getHorarioAula() );
         v.setDia( horarioDia->getDia() );
         v.setDisciplina( aula->getDisciplina() );
         v.setTurma( aula->getTurma() );

         double coeff = 10.0;

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

int SolverMIP::criaVariavelProfessorCurso()
{
   int num_vars = 0;

   GGroup< Professor *, LessPtr< Professor > > professores
      = problemData->getProfessores();

   ITERA_GGROUP_LESSPTR( itProfessor, professores, Professor )
   {
      ITERA_GGROUP_LESSPTR( itMagisterio, itProfessor->magisterio, Magisterio )
      {
         Disciplina * discMinistradaProf = itMagisterio->disciplina;

         ITERA_GGROUP_LESSPTR( it_aula, problemData->aulas, Aula )
         {
            Aula * aula = ( *it_aula );

            Disciplina * discAula = aula->getDisciplina();

            if ( discMinistradaProf != discAula )
            {
               continue;
            }

            std::pair< int, int > professor_disciplina (
               itProfessor->getId(), discMinistradaProf->getId() );

            // Se o professor e a disciplina da aula em quest�o se relacionarem:
            if ( problemData->prof_Disc_Dias.find( professor_disciplina )
                  == problemData->prof_Disc_Dias.end() )
            {
               continue;
            }

            // Como o professor em quest�o est� alocado nessa aula, ent�o dizemos que
            // ele leciona disciplinas de cada curso que essa aula est� relacionada.
            // Para encontrar os cursos que a aula atende, basta verificar suas ofertas
            ITERA_GGROUP_LESSPTR( itOferta, aula->ofertas, Oferta )
            {
               Oferta * oferta = ( *itOferta );
               Curso * curso = oferta->curso;

               // Cria a variavel 'professor/curso'
               VariableOp v;
               v.reset();
               v.setType( VariableOp::V_PROF_CURSO );

               v.setProfessor( *itProfessor ); 
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
   }

   return num_vars;
}

int SolverMIP::criaVariavelCustoCorpoDocente()
{
   int num_vars = 0;
   double coeff = 0.0;
   double alfa = 500.0;

   GGroup< Professor *, LessPtr< Professor > > professores
      = problemData->getProfessores();

   ITERA_GGROUP_LESSPTR( it_prof, professores, Professor )
   {
      Professor * professor = ( *it_prof );

      VariableOp v;
      v.reset();
      v.setType( VariableOp::V_CUSTO_CORPO_DOCENTE );
      v.setProfessor( professor );

      if ( vHashOp.find( v ) == vHashOp.end() )
      {
         v.setValue( 1.0 );
         coeff = ( professor->getValorCredito() * alfa );

         vHashOp[ v ] = lp->getNumCols();

         OPT_COL col( OPT_COL::VAR_BINARY, coeff, 0.0, 1.0,
            ( char * )v.toString().c_str() );

         lp->newCol( col );
         num_vars++;
      }
   }

   return num_vars;
}

int SolverMIP::criaVariavelDiasProfessoresMinistramAulas()
{
   int num_vars = 0;
   double coeff = 1000.0;

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

   // Criando as vari�veis apenas para os dias poss�veis de cada professor
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

int SolverMIP::criaVariavelFolgaMinimoMestresCurso()
{
   int num_vars = 0;
   double coeff = 1000.0;

   GGroup< Professor *, LessPtr< Professor > > professores
      = problemData->getProfessores();

   int ub = professores.size();

   ITERA_GGROUP_LESSPTR( it_curso, problemData->cursos, Curso )
   {
      VariableOp v;
      v.reset();

      v.setType( VariableOp::V_F_MIN_MEST_CURSO );
      v.setCurso( *it_curso );

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

int SolverMIP::criaVariavelFolgaMinimoDoutoresCurso()
{
   int num_vars = 0;
   double coeff = 1000.0;

   GGroup< Professor *, LessPtr< Professor > > professores
      = problemData->getProfessores();

   int ub = professores.size();

   ITERA_GGROUP_LESSPTR( it_curso, problemData->cursos, Curso )
   {
      VariableOp v;
      v.reset();

      v.setType( VariableOp::V_F_MIN_DOUT_CURSO );
      v.setCurso( *it_curso );

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

int SolverMIP::criaVariavelMaxDiscProfCurso()
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
            v.setType( VariableOp::V_MAX_DISC_PROF_CURSO );

            v.setProfessor( professor );
            v.setDisciplina( disciplina );
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

   return num_vars;
}

int SolverMIP::criaVariavelFolgaMaxDiscProfCurso()
{
   int num_vars = 0;
   double coeff = 1000.0;

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

   return num_vars;
}

int SolverMIP::criaVariavelFolgaCargaHorariaMinimaProfessor()
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

int SolverMIP::criaVariavelFolgaCargaHorariaMinimaProfessorSemana()
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

int SolverMIP::criaVariavelFolgaCargaHorariaMaximaProfessorSemana()
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

int SolverMIP::criaVariavelAvaliacaoCorpoDocente()
{
   int num_vars = 0;
   double coeff = 0.0;
   double alfa = 500.0;

   GGroup< Professor *, LessPtr< Professor > > professores
      = problemData->getProfessores();

   ITERA_GGROUP_LESSPTR( it_prof, professores, Professor )
   {
      Professor * professor = ( *it_prof );

      VariableOp v;
      v.reset();
      v.setType( VariableOp::V_AVALIACAO_CORPO_DOCENTE );
      v.setProfessor( professor );

      if ( vHashOp.find( v ) == vHashOp.end() )
      {
         v.setValue( 1.0 );

         double avaliacao = 0.0;
         double total_notas = 0.0;

         ITERA_GGROUP_LESSPTR( it_mag, professor->magisterio, Magisterio )
         {
            avaliacao += it_mag->getNota();
            total_notas += 10.0;
         }

         coeff = ( ( total_notas - avaliacao ) * alfa );

         vHashOp[ v ] = lp->getNumCols();

         OPT_COL col( OPT_COL::VAR_BINARY, coeff, 0.0, 1.0,
            ( char * )v.toString().c_str() );

         lp->newCol( col );
         num_vars++;
      }
   }

   return num_vars;
}

int SolverMIP::criaVariavelGapsProfessores()
{
   int num_vars = 0;
   double coeff = 0.0;
   
   // Custo da vari�vel do gap, que deve 
   // multiplicado pelo n�mero de hor�rios
   // de aula o gap de hor�rios
   double alfa = 10.0;

   GGroup< Professor *, LessPtr< Professor > > professores
      = problemData->getProfessores();

   GGroup< int > dias_letivos;
   for ( int i = 2; i <= 7; i++ )
   {
      dias_letivos.add( i );
   }

   ITERA_GGROUP_LESSPTR( it_prof, professores, Professor )
   {
      Professor * professor = ( *it_prof );

      ITERA_GGROUP_N_PT( it_dia, dias_letivos, int )
      {
         int dia = ( *it_dia );

         for ( int ini = 0; ini < (int)problemData->horarios_aula_ordenados.size(); ini++ )
         {
            for ( int fim = ini + 1; fim < (int)problemData->horarios_aula_ordenados.size(); fim++ )
            {
               HorarioAula * h1 = problemData->horarios_aula_ordenados[ ini ];
               HorarioAula * h2 = problemData->horarios_aula_ordenados[ fim ];

               if ( abs( fim - ini ) <= 1 )
               {
                  continue;
               }

               if ( h1->getCalendario() != h2->getCalendario() )
               {
                  continue;
               }

               VariableOp v;
               v.reset();
               v.setType( VariableOp::V_GAPS_PROFESSORES );

               v.setProfessor( professor );
               v.setDia( dia );
               v.setH1( h1 );
               v.setH2( h2 );

               if ( vHashOp.find( v ) == vHashOp.end() )
               {
                  // Leva em considera��o o n�mero de hor�rios no gap
                  coeff = ( fim - ini ) * alfa;

                  vHashOp[ v ] = lp->getNumCols();

                  OPT_COL col( OPT_COL::VAR_BINARY, coeff, 0.0, 1.0,
                     ( char * )v.toString().c_str() );

                  lp->newCol( col );
                  num_vars++;
               }
            }
         }
      }
   }

   return num_vars;
}

int SolverMIP::criaVariavelFolgaUltimaPrimeiraAulas()
{
   int num_vars = 0;
   double coeff = 0.0;
   double alfa = 1000.0;

   GGroup< Professor *, LessPtr< Professor > > professores
      = problemData->getProfessores();

   ITERA_GGROUP_LESSPTR( it_prof, professores, Professor )
   {
      Professor * professor = ( *it_prof );

      ITERA_GGROUP_LESSPTR( it_h1,
         problemData->horariosDia, HorarioDia )
      {
         HorarioDia * h1 = ( *it_h1 );
         
         ITERA_GGROUP_LESSPTR( it_h2,
            problemData->horariosDia, HorarioDia )
         {
            HorarioDia * h2 = ( *it_h2 );
            
            // Verifica se essas aulas ocorrem no �ltimo
            // hor�rio do dia D e no primeiro hor�rio do dia ( D + 1 )
            bool verificaAulas = problemData->verificaUltimaPrimeiraAulas( h1, h2 );

            if ( !verificaAulas )
            {
               continue;
            }

            VariableOp v;
            v.reset();
            v.setType( VariableOp::V_F_ULTIMA_PRIMEIRA_AULA_PROF );

            v.setProfessor( professor );
            v.setHorarioDiaD( h1 );
            v.setHorarioDiaD1( h2 );

            if ( vHashOp.find( v ) == vHashOp.end() )
            {
               coeff = 1000.0;

               vHashOp[ v ] = lp->getNumCols();

               OPT_COL col( OPT_COL::VAR_BINARY, coeff, 0.0, 1.0,
                  ( char * ) v.toString().c_str() );

               lp->newCol( col );
               num_vars++;
            }
         }
      }
   }

   return num_vars;
}


int SolverMIP::criaVariavelFolgaDemanda( void )
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
			double coeff = 500000.0;

			vHashOp[ v ] = lp->getNumCols();

			OPT_COL col( OPT_COL::VAR_BINARY, coeff, 0.0, 1.0,
				( char * ) v.toString().c_str() );

			lp->newCol( col );
			num_vars++;
		}
   }

   return num_vars;

}

int SolverMIP::criaVariavelFolgaDisciplinaTurmaHorario( void )
{
	int num_vars = 0;
	
	ITERA_GGROUP_LESSPTR( itCampus, problemData->campi, Campus )
	{
		Campus *campus = *itCampus;
		
		GGroup<int> disciplinas = problemData->cp_discs[campus->getId()];

		ITERA_GGROUP_N_PT( itDisc, disciplinas, int )
		{
			Disciplina *disciplina = problemData->refDisciplinas[ *itDisc ];

			for ( int turma = 0; turma < disciplina->getNumTurmas(); turma++ )
			{
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
						std::cout << "\nAtencao na funcao SolverMIP::criaVariavelFolgaDisciplinaHorario:"
						<< " nao ha horarios disponiveis para a disciplina "<<disciplina->getId();
					}

					OPT_COL col( OPT_COL::VAR_INTEGRAL, coeff, 0.0, nHor,
								( char* ) v.toString().c_str() );

					lp->newCol( col );
					num_vars++;
				}			
			}
		}
	}

	return num_vars;
}


int SolverMIP::criaRestricoesOperacional()
{

	CPUTimer timer;
	double dif = 0.0;

	int restricoes = 0;

#ifdef PRINT_cria_restricoes
	int numRestAnterior = 0;
#endif

	timer.start();
	restricoes += criaRestricaoSalaHorario();
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest C_SALA_HORARIO: "
		<< ( restricoes - numRestAnterior ) <<" " <<dif <<"seg" <<std::endl;
	numRestAnterior = restricoes;
#endif

	lp->updateLP();
	timer.start();
	restricoes += criaRestricaoProfessorHorario();
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest C_PROFESSOR_HORARIO: "
		<< ( restricoes - numRestAnterior ) <<" " <<dif <<"seg" <<std::endl;
	numRestAnterior = restricoes;
#endif

	lp->updateLP();
	timer.start();
	restricoes += criaRestricaoBlocoHorario();
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest C_BLOCO_HORARIO: "
		<< ( restricoes - numRestAnterior ) <<" " <<dif <<"seg" <<std::endl;
	numRestAnterior = restricoes;
#endif

	lp->updateLP();
	timer.start();
	restricoes += criaRestricaoBlocoHorarioDisc();
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest C_BLOCO_HORARIO_DISC: "
		<< ( restricoes - numRestAnterior ) <<" " <<dif <<"seg" <<std::endl;
	numRestAnterior = restricoes;
#endif

	lp->updateLP();
	timer.start();
	restricoes += criaRestricaoAlocAula();
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest C_ALOC_AULA: "
		<< ( restricoes - numRestAnterior ) <<" " <<dif <<"seg" <<std::endl;
	numRestAnterior = restricoes;
#endif

	lp->updateLP();
	timer.start();
	restricoes += criaRestricaoProfessorDisciplina();
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest C_PROF_DISC: "
		<< ( restricoes - numRestAnterior ) <<" " <<dif <<"seg" <<std::endl;
	numRestAnterior = restricoes;
#endif

	lp->updateLP();
	timer.start();
	restricoes += criaRestricaoProfessorDisciplinaUnico();
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest C_PROF_DISC_UNI: "
		<< ( restricoes - numRestAnterior ) <<" " <<dif <<"seg" <<std::endl;
	numRestAnterior = restricoes;
#endif

	lp->updateLP();
	timer.start();
	//restricoes += criaRestricaoDisciplinaMesmoHorario();
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest C_DISC_HORARIO: "
		<< ( restricoes - numRestAnterior ) <<" " <<dif <<"seg" <<std::endl;
	numRestAnterior = restricoes;
#endif

	lp->updateLP();
	timer.start();
	//restricoes += criaRestricaoDisciplinaHorarioUnico();
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest C_DISC_HORARIO_UNICO: "
		<< ( restricoes - numRestAnterior ) <<" " <<dif <<"seg" <<std::endl;
	numRestAnterior = restricoes;
#endif

	lp->updateLP();
	timer.start();
	restricoes += criaRestricaoFixProfDiscSalaDiaHor();
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest C_FIX_PROF_DISC_SALA_DIA_HOR: "
		<< ( restricoes - numRestAnterior ) <<" " <<dif <<"seg" <<std::endl;
	numRestAnterior = restricoes;
#endif

	lp->updateLP();
	timer.start();
	restricoes += criaRestricaoFixProfDiscDiaHor();
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest C_FIX_PROF_DISC_DIA_HOR: "
		<< ( restricoes - numRestAnterior ) <<" " <<dif <<"seg" <<std::endl;
	numRestAnterior = restricoes;
#endif

	lp->updateLP();
	timer.start();
	restricoes += criaRestricaoFixProfDisc();
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest C_FIX_PROF_DISC: "
		<< ( restricoes - numRestAnterior ) <<" " <<dif <<"seg" <<std::endl;
	numRestAnterior = restricoes;
#endif

	lp->updateLP();
	timer.start();
	restricoes += criaRestricaoFixProfDiscSala();
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest C_FIX_PROF_DISC_SALA: "
		<< ( restricoes - numRestAnterior ) <<" " <<dif <<"seg" <<std::endl;
	numRestAnterior = restricoes;
#endif

	lp->updateLP();
	timer.start();
	restricoes += criaRestricaoFixProfSala();
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest C_FIX_PROF_SALA: "
		<< ( restricoes - numRestAnterior ) <<" " <<dif <<"seg" <<std::endl;
	numRestAnterior = restricoes;
#endif

	lp->updateLP();
	timer.start();
	restricoes += criaRestricaoDeslocamentoViavel();
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest C_DESLOC_VIAVEL: "
		<< ( restricoes - numRestAnterior ) <<" " <<dif <<"seg" <<std::endl;
	numRestAnterior = restricoes;
#endif

	lp->updateLP();
	timer.start();
	restricoes += criaRestricaoDeslocamentoProfessor();
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest C_DESLOC_PROF: "
		<< ( restricoes - numRestAnterior ) <<" " <<dif <<"seg" <<std::endl;
	numRestAnterior = restricoes;
#endif

	lp->updateLP();
	timer.start();
	restricoes += criaRestricaoAvaliacaoCorpoDocente();
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest C_AVALIACAO_CORPO_DOCENTE: "
		<< ( restricoes - numRestAnterior ) <<" " <<dif <<"seg" <<std::endl;
	numRestAnterior = restricoes;
#endif

	lp->updateLP();
	timer.start();
	restricoes += criaRestricaoCustoCorpoDocente();
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest C_CUSTO_CORPO_DOCENTE: "
		<< ( restricoes - numRestAnterior ) <<" " <<dif <<"seg" <<std::endl;
	numRestAnterior = restricoes;
#endif

	lp->updateLP();
	timer.start();
	restricoes += criaRestricaoRelacionaVariavelXDiaProf();
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest C_DIAS_PROF_MINISTRA_AULA: "
		<< ( restricoes - numRestAnterior ) <<" " <<dif <<"seg" <<std::endl;
	numRestAnterior = restricoes;
#endif

	lp->updateLP();
	timer.start();
	restricoes += criaRestricaoMinimoMestresCurso();
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest C_MIN_MEST_CURSO: "
		<< ( restricoes - numRestAnterior ) <<" " <<dif <<"seg" <<std::endl;
	numRestAnterior = restricoes;
#endif

	lp->updateLP();
	timer.start();
	restricoes += criaRestricaoMinimoDoutoresCurso();
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest C_MIN_DOUT_CURSO: "
		<< ( restricoes - numRestAnterior ) <<" " <<dif <<"seg" <<std::endl;
	numRestAnterior = restricoes;
#endif

	lp->updateLP();
	timer.start();
	restricoes += criaRestricaoUltimaPrimeiraAulas();
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest C_ULTIMA_PRIMEIRA_AULA_PROF: "
		<< ( restricoes - numRestAnterior ) <<" " <<dif <<"seg" <<std::endl;
	numRestAnterior = restricoes;
#endif

	lp->updateLP();
	timer.start();
	restricoes += criaRestricaoAlocacaoProfessorCurso();
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest C_ALOC_PROF_CURSO: "
		<< ( restricoes - numRestAnterior ) <<" " <<dif <<"seg" <<std::endl;
	numRestAnterior = restricoes;
#endif

	lp->updateLP();
	timer.start();
	restricoes += criaRestricaoCargaHorariaMinimaProfessor();
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest C_CARGA_HOR_MIN_PROF: "
		<< ( restricoes - numRestAnterior ) <<" " <<dif <<"seg" <<std::endl;
	numRestAnterior = restricoes;
#endif

	lp->updateLP();
	timer.start();
	restricoes += criaRestricaoMaxDiscProfCurso();
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest C_MAX_DISC_PROF_CURSO: "
		<< ( restricoes - numRestAnterior ) <<" " <<dif <<"seg" <<std::endl;
	numRestAnterior = restricoes;
#endif

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

	lp->updateLP();
	timer.start();
	restricoes += criaRestricaoCargaHorariaMaximaProfessorSemana();
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest C_CARGA_HOR_MAX_PROF_SEMANA: "
		<< ( restricoes - numRestAnterior ) <<" " <<dif <<"seg" <<std::endl;
	numRestAnterior = restricoes;
#endif

	lp->updateLP();
	timer.start();
	//restricoes += criaRestricaoGapsProfessores();
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest C_GAPS_PROFESSORES: "
		<< ( restricoes - numRestAnterior ) <<" " <<dif <<"seg" <<std::endl;
	numRestAnterior = restricoes;
#endif

	lp->updateLP();
	timer.start();
	//restricoes += criaRestricaoProfHorarioMultiUnid();
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest C_PROF_HORARIO_MULTIUNID: "
		<< ( restricoes - numRestAnterior ) <<" " <<dif <<"seg" <<std::endl;
	numRestAnterior = restricoes;
#endif

	lp->updateLP();


	return restricoes;
}

void SolverMIP::chgCoeffList(
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

   delete [] rList;
   delete [] cList;
   delete [] vList;
}

int SolverMIP::criaRestricaoSalaHorario()
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

//int SolverMIP::criaRestricaoSalaHorario()
//{
//   int restricoes = 0;
//   char name[ 200 ];
//   int nnz = ( this->problemData->aulas.size() * this->problemData->horarios_aula_ordenados.size() );
//
//   GGroup< Sala *, LessPtr< Sala > > salas = this->problemData->getSalas();
//
//   ConstraintOp c;
//   VariableOpHash::iterator vit;
//   ConstraintOpHash::iterator cit;
//
//   ITERA_GGROUP_LESSPTR( it_sala, salas, Sala )
//   {
//      Sala * sala = ( *it_sala );
//
//      ITERA_GGROUP_LESSPTRPTR( it_horario_dia,
//         sala->horariosDia, HorarioDia )
//      {
//         HorarioDia * horario_dia = ( *it_horario_dia );
//
//         int dia_semana = horario_dia->getDia();
//         HorarioAula * horario_aula = horario_dia->getHorarioAula();
//	     int idxHor1 = problemData->getHorarioDiaIdx( horario_dia );
//
//         c.reset();
//         c.setType( ConstraintOp::C_SALA_HORARIO );
//         c.setSala( sala );
//         c.setHorario( horario_dia );
//         c.setHorarioAula( horario_aula );
//         c.setDia( dia_semana );
//
//         cit = cHashOp.find( c );
//
//         if ( cit == cHashOp.end() )
//         {
//            cHashOp[ c ] = lp->getNumRows();
//
//            sprintf( name, "%s", c.toString().c_str() );
//            OPT_ROW row( nnz, OPT_ROW::LESS , 1.0, name );
//
//            bool adicionouVariavel = false;
//            vit = vHashOp.begin();
//
//            for (; vit != vHashOp.end(); vit++ )
//            {
//               VariableOp vOp = ( vit->first );
//
//               if (  vOp.getType() != VariableOp::V_X_PROF_AULA_HOR
//                  || vOp.getSala() != sala 
//                  || vOp.getDia() != dia_semana
//				  || sala->disciplinasAssociadas.find( vOp.getDisciplina() ) ==  sala->disciplinasAssociadas.end() )
//               {
//                  continue;
//               }
//
//			   int nCred2 = vOp.getAula()->getTotalCreditos();
//			   int duracao = vOp.getDisciplina()->getTempoCredSemanaLetiva();
//
//			   DateTime inicio = horario_aula->getInicio();
//			   DateTime dt2Inicio = vOp.getHorarioAula()->getInicio();
//			   DateTime fim = horario_aula->getFinal();
//			   DateTime dt2Fim = vOp.getHorarioAula()->getInicio();
//			   dt2Fim.addMinutes( duracao*nCred2 );
//
//			   if (  ( vOp.getHorarioAula() != horario_aula ) &&				    
//				    !( ( dt2Inicio <= inicio ) && ( dt2Fim > inicio ) ) &&
//					!( ( dt2Inicio >= inicio ) && ( dt2Inicio < fim ) ) )
//			   {
//				   continue;
//			   }
//
//               row.insert( vit->second, 1.0 );
//               adicionouVariavel = true;
//            }
//
//            if ( adicionouVariavel )
//            {
//               lp->addRow( row );
//               restricoes++;
//            }
//         }
//      }
//   }
//
//   return restricoes;
//}


int SolverMIP::criaRestricaoProfessorHorario()
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

	   if (vOp.getType() != VariableOp::V_X_PROF_AULA_HOR || vOp.getProfessor() == NULL)
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

		   int nCred2 = vOp.getAula()->getTotalCreditos();
		   int duracao = vOp.getDisciplina()->getTempoCredSemanaLetiva();

		   DateTime inicio = horario_aula->getInicio();
		   DateTime dt2Inicio = vOp.getHorarioAula()->getInicio();
		   DateTime fim = horario_aula->getFinal();
		   DateTime dt2Fim = vOp.getHorarioAula()->getInicio();
		   dt2Fim.addMinutes( duracao*nCred2 );

		   if (  ( vOp.getHorarioAula() != horario_aula ) &&				    
			   !( ( dt2Inicio <= inicio ) && ( dt2Fim > inicio ) ) &&
			   !( ( dt2Inicio >= inicio ) && ( dt2Inicio < fim ) ) )
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


//int SolverMIP::criaRestricaoProfessorHorario()
//{
//   int restricoes = 0;
//   char name[ 200 ];
//   int nnz = ( this->problemData->aulas.size() * this->problemData->horarios_aula_ordenados.size() );
//   
//   ConstraintOp c;
//   VariableOpHash::iterator vit;
//   ConstraintOpHash::iterator cit;
//
//   GGroup< Professor *, LessPtr< Professor > > professores
//      = problemData->getProfessores();
//
//   ITERA_GGROUP_LESSPTR( itProfessor, professores, Professor )
//   {
//	   Professor *prof = *itProfessor;
//
//      ITERA_GGROUP_LESSPTR( it_horario_dia, itProfessor->horariosDia, HorarioDia )
//      {
//		  HorarioDia * horario_dia = ( *it_horario_dia );
//		  int dia = horario_dia->getDia();
//
//		  HorarioAula * horario_aula = horario_dia->getHorarioAula();
//
//		  c.reset();
//		  c.setType( ConstraintOp::C_PROFESSOR_HORARIO );
//		  c.setHorarioAula( horario_aula );
//		  c.setDia( dia );
//		  c.setProfessor( prof );
//
//          cit = cHashOp.find( c );
//
//          if ( cit == cHashOp.end() )
//          {
//            cHashOp[ c ] = lp->getNumRows();
//
//            sprintf( name, "%s", c.toString().c_str() );
//            OPT_ROW row( nnz, OPT_ROW::LESS , 1.0, name );
//
//            bool adicionouVariavel = false;
//            vit = vHashOp.begin();
//
//            for (; vit != vHashOp.end(); vit++ )
//            {
//               VariableOp vOp = ( vit->first );
//
//               if ( vOp.getType() != VariableOp::V_X_PROF_AULA_HOR
//                  || vOp.getDia() != dia
//				  || vOp.getProfessor() != prof )
//               {
//                  continue;
//               }
//
//			   int nCred2 = vOp.getAula()->getTotalCreditos();
//			   int duracao = vOp.getDisciplina()->getTempoCredSemanaLetiva();
//
//			   DateTime inicio = horario_aula->getInicio();
//			   DateTime dt2Inicio = vOp.getHorarioAula()->getInicio();
//			   DateTime fim = horario_aula->getFinal();
//			   DateTime dt2Fim = vOp.getHorarioAula()->getInicio();
//			   dt2Fim.addMinutes( duracao*nCred2 );
//
//			   if (  ( vOp.getHorarioAula() != horario_aula ) &&				    
//				    !( ( dt2Inicio <= inicio ) && ( dt2Fim > inicio ) ) &&
//					!( ( dt2Inicio >= inicio ) && ( dt2Inicio < fim ) ) )
//			   {
//				   continue;
//			   }
//
//               row.insert( vit->second, 1.0 );
//               adicionouVariavel = true;
//            }
//
//            if ( adicionouVariavel )
//            {
//               lp->addRow( row );
//               restricoes++;
//            }
//         }
//      }
//   }
//
//   return restricoes;
//}


/*
int SolverMIP::criaRestricaoProfessorHorario()
{
   int restricoes = 0;
   int nnz;
   char name[ 200 ];

   ConstraintOp c;
   VariableOpHash::iterator vit;
   ConstraintOpHash::iterator cit;

   std::vector< std::pair< int, int > > coeffList;
   std::vector< double > coeffListVal;
   std::pair< int , int > auxCoef;

   vit = vHashOp.begin();

   for (; vit != vHashOp.end(); vit++ )
   {
      VariableOp v = vit->first;

      if ( v.getType() != VariableOp::V_X_PROF_AULA_HOR
         || v.getProfessor() == NULL )
      {
         continue;
      }

      int nCred = v.getAula()->getTotalCreditos();
      int idxHor = problemData->getHorarioDiaIdx( v.getHorario() );

      for ( int h = idxHor; h < idxHor+nCred; h++ )
      {
         c.reset();
         c.setType( ConstraintOp::C_PROFESSOR_HORARIO );
         c.setProfessor( v.getProfessor() );
         c.setHorario( problemData->horariosDiaIdx[ h ] );
         c.setDia( problemData->horariosDiaIdx[ h ]->getDia() );
         c.setHorarioAula( problemData->horariosDiaIdx[ h ]->getHorarioAula() );

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
   }

   chgCoeffList( coeffList, coeffListVal );

   return restricoes;
}
*/
int SolverMIP::criaRestricaoBlocoHorario()
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

      std::map< Aula *, GGroup< BlocoCurricular *,
         LessPtr< BlocoCurricular > >, LessPtr< Aula > >::iterator
         itAulaBlocosCurriculares = problemData->aulaBlocosCurriculares.find( v.getAula() );

      // Para cada Bloco Curricular ao qual a aula pertence
      ITERA_GGROUP_LESSPTR( itBlocoCurric,
         itAulaBlocosCurriculares->second, BlocoCurricular )
      {
         BlocoCurricular * bloco = ( *itBlocoCurric );

         int nCred = v.getAula()->getTotalCreditos();
         int idxHor = problemData->getHorarioDiaIdx( v.getHorario() );

         for ( int h = idxHor; h < ( idxHor + nCred ); h++ )
         {
            c.reset();
            c.setType( ConstraintOp::C_BLOCO_HORARIO );
            c.setBloco( bloco );
            c.setHorario( problemData->horariosDiaIdx[ h ] );
            c.setDia( problemData->horariosDiaIdx[ h ]->getDia() );
            c.setHorarioAula( problemData->horariosDiaIdx[ h ]->getHorarioAula() );
            c.setTurma( v.getTurma() );

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
      }
   }

   chgCoeffList( coeffList, coeffListVal );

   return restricoes;
}


/*
	Impede que duas disciplinas distintas de um mesmo bloco curricular sejam alocadas
	no mesmo horario do mesmo dia. 
*/

int SolverMIP::criaRestricaoBlocoHorarioDisc()
{
   int restricoes = 0;
   int nnz;
   char name[ 200 ];

   ConstraintOp c;
   VariableOpHash::iterator vit1;
   VariableOpHash::iterator vit2;
   ConstraintOpHash::iterator cit;

   std::vector< std::pair< int, int > > coeffList;
   std::vector< double > coeffListVal;
   std::pair< int, int > auxCoef;

   map<Disciplina*, map<int, vector<VariableOpHash::iterator>>, LessPtr<Disciplina>> varsDisciplina;

   for (vit1 = vHashOp.begin(); vit1 != vHashOp.end(); vit1++ )
   {
	   VariableOp v1 = vit1->first;
	   if ( v1.getType() != VariableOp::V_X_PROF_AULA_HOR )
	   {
		   continue;
	   }

	   Disciplina *d1 = v1.getDisciplina();
	   int dia1 = v1.getDia();
	   varsDisciplina[d1][dia1].push_back(vit1);
   }

   map<Disciplina*, map<int, vector<VariableOpHash::iterator>>, LessPtr<Disciplina>>::iterator vitD1;
   map<Disciplina*, map<int, vector<VariableOpHash::iterator>>, LessPtr<Disciplina>>::iterator vitD2;

   for(vitD1 = varsDisciplina.begin(); vitD1 != varsDisciplina.end(); vitD1++)
   {
	   Disciplina *d1 = vitD1->first;
	   for(map<int, vector<VariableOpHash::iterator>>::iterator it1 = vitD1->second.begin();
		   it1 != vitD1->second.end();
		   it1++)
	   {
		   int dia1 = it1->first;
		   for(vector<VariableOpHash::iterator>::iterator it2 = it1->second.begin();
			   it2 != it1->second.end();
			   it2++)
		   {
			   vit1 = *it2;
			   VariableOp v1 = vit1->first;

			   std::map< Aula *, GGroup< BlocoCurricular *, LessPtr< BlocoCurricular > >, LessPtr< Aula > >::iterator
				   itAulaBlocosCurriculares = problemData->aulaBlocosCurriculares.find( v1.getAula() );

			   // Para cada Bloco Curricular ao qual a aula pertence
			   ITERA_GGROUP_LESSPTR( itBlocoCurric,	itAulaBlocosCurriculares->second, BlocoCurricular )
			   {
				   BlocoCurricular * bloco = *itBlocoCurric;

               for (GGroup<Aula*,LessPtr<Aula> >::iterator itAB = problemData->blocoCurricularDiaAulas[bloco][v1.getDia()].begin();
               itAB != problemData->blocoCurricularDiaAulas[bloco][dia1].end(); itAB++)
               {
                  Aula *aula2 = *itAB;

                  Disciplina *d2 = aula2->getDisciplina();

                  vector<VariableOpHash::iterator> vars2 = varsDisciplina[d2][dia1];
					   for(vector<VariableOpHash::iterator>::iterator it3 = vars2.begin();
						   it3 != vars2.end();
						   it3++)
					   {
						   vit2 = *it3;
						   VariableOp v2 = vit2->first;

                     if ( v2.getAula() != aula2 )
                        continue;

						   int nCred1 = v1.getAula()->getTotalCreditos();
						   HorarioAula* h1 = v1.getHorario()->getHorarioAula();
						   //int idxHor1 = problemData->getHorarioDiaIdx( v1.getHorario() );

						   int nCred2 = v2.getAula()->getTotalCreditos();
						   HorarioAula* h2 = v2.getHorario()->getHorarioAula();
						   //int idxHor2 = problemData->getHorarioDiaIdx( v2.getHorario() );

						   //GGroup< HorarioAula* > horarios;

						   DateTime fim1 = h1->getInicio();
						   fim1.addMinutes( h1->getTempoAula() * nCred1 );

						   DateTime fim2 = h2->getInicio();
						   fim2.addMinutes( h2->getTempoAula() * nCred2 );

						   // ---------------------------------------------------

						   if ( ( ( h1->getInicio() <= h2->getInicio() ) && ( fim1 >  h2->getInicio() ) ) ||
							   ( ( h2->getInicio() <=  h1->getInicio() ) && ( fim2 >  h1->getInicio() ) ) )
						   {	// intersecao de horarios, com d1 comecando antes de (ou junto com) d2
							   // intersecao de horarios, com d2 comecando antes de (ou junto com) d1

							   c.reset();
							   c.setType( ConstraintOp::C_BLOCO_HORARIO_DISC );
							   c.setBloco( bloco );
							   c.setDia( dia1 );
							   c.setH1( h1 );
							   c.setH2( h2 );
							   c.setParDiscTurma( d1, v1.getTurma(), d2, v2.getTurma() );

							   cit = cHashOp.find( c );

							   if ( cit != cHashOp.end() )
							   {
								   lp->chgCoef(cit->second, vit1->second, 1.0);
								   lp->chgCoef(cit->second, vit2->second, 1.0);
							   }
							   else
							   {
								   sprintf( name, "%s", c.toString().c_str() );
								   nnz = 100;

								   OPT_ROW row( nnz, OPT_ROW::LESS , 1.0, name );

								   row.insert( vit1->second, 1.0 );
								   row.insert( vit2->second, 1.0 );
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

   return restricoes;
}

//int SolverMIP::criaRestricaoBlocoHorarioDisc()
//{
//   int restricoes = 0;
//   int nnz;
//   char name[ 200 ];
//
//   ConstraintOp c;
//   VariableOpHash::iterator vit1;
//   VariableOpHash::iterator vit2;
//   ConstraintOpHash::iterator cit;
//
//   std::vector< std::pair< int, int > > coeffList;
//   std::vector< double > coeffListVal;
//   std::pair< int, int > auxCoef;
//   
//   for (vit1 = vHashOp.begin(); vit1 != vHashOp.end(); vit1++ )
//   {
//	   VariableOp v1 = vit1->first;
//	   if ( v1.getType() != VariableOp::V_X_PROF_AULA_HOR )
//	   {
//		   continue;
//	   }
//	   Disciplina *d1 = v1.getDisciplina();
//	   int dia1 = v1.getDia();
//	   
//	   std::map< Aula *, GGroup< BlocoCurricular *, LessPtr< BlocoCurricular > >, LessPtr< Aula > >::iterator
//			itAulaBlocosCurriculares = problemData->aulaBlocosCurriculares.find( v1.getAula() );
//
//	   // Para cada Bloco Curricular ao qual a aula pertence
//	   ITERA_GGROUP_LESSPTR( itBlocoCurric,	itAulaBlocosCurriculares->second, BlocoCurricular )
//	   {
//		   vit2 = vit1;
//		   vit2++;
//		   for (; vit2 != vHashOp.end(); vit2++ )
//		   {
//			   VariableOp v2 = vit2->first;
//			   if ( v2.getType() != VariableOp::V_X_PROF_AULA_HOR )
//			   {
//				   continue;
//			   }
//
//			   Disciplina *d2 = v2.getAula()->getDisciplina();
//			   if( ( itBlocoCurric->disciplinas.find( d2 ) == itBlocoCurric->disciplinas.end() ) ||
//				   ( *d1 == *d2 ) ||
//				   ( dia1 != v2.getDia() ) )
//			   {	// Se a disciplina d2 nao estiver no mesmo bloco curricular que d1 OU
//				    // se as disciplinas d1 e d2 forem a mesma OU
//				    // se os dias das aulas de vit1 e vit2 foram diferentes
//				    // passa para a proxima variavel (itera vit2)
//					continue;
//			   }
//
//			   BlocoCurricular * bloco = ( *itBlocoCurric );
//
//			   int nCred1 = v1.getAula()->getTotalCreditos();
//			   HorarioAula* h1 = v1.getHorario()->getHorarioAula();
//			   //int idxHor1 = problemData->getHorarioDiaIdx( v1.getHorario() );
//
//			   int nCred2 = v2.getAula()->getTotalCreditos();
//			   HorarioAula* h2 = v2.getHorario()->getHorarioAula();
//			   //int idxHor2 = problemData->getHorarioDiaIdx( v2.getHorario() );
//			   			   
//			   GGroup< HorarioAula* > horarios;
//			   
//			   DateTime fim1 = h1->getInicio();
//			   fim1.addMinutes( h1->getTempoAula() * nCred1 );
//			   
//			   DateTime fim2 = h2->getInicio();
//			   fim2.addMinutes( h2->getTempoAula() * nCred2 );
//
//			   // ---------------------------------------------------
//
//			   if ( ( ( h1->getInicio() <= h2->getInicio() ) && ( fim1 >  h2->getInicio() ) ) ||
//					( ( h2->getInicio() <=  h1->getInicio() ) && ( fim2 >  h1->getInicio() ) ) )
//			   {	// intersecao de horarios, com d1 comecando antes de (ou junto com) d2
//				    // intersecao de horarios, com d2 comecando antes de (ou junto com) d1
//
//					c.reset();
//					c.setType( ConstraintOp::C_BLOCO_HORARIO_DISC );
//					c.setBloco( bloco );
//					c.setDia( dia1 );
//					c.setH1( h1 );
//					c.setH2( h2 );
//					c.setParDiscTurma( d1, v1.getTurma(), d2, v2.getTurma() );
//
//					cit = cHashOp.find( c );
//
//					if ( cit != cHashOp.end() )
//					{
//						auxCoef.first = cit->second; // restricao C_BLOCO_HORARIO_DISC
//						auxCoef.second = vit1->second; // variavel V_X_PROF_AULA_HOR
//						coeffList.push_back( auxCoef );
//						coeffListVal.push_back( 1.0 );
//
//						auxCoef.first = cit->second; // restricao C_BLOCO_HORARIO_DISC
//						auxCoef.second = vit2->second; // variavel V_X_PROF_AULA_HOR
//						coeffList.push_back( auxCoef );
//						coeffListVal.push_back( 1.0 );
//					}
//					else
//					{
//						sprintf( name, "%s", c.toString().c_str() );
//						nnz = 100;
//
//						OPT_ROW row( nnz, OPT_ROW::LESS , 1.0, name );
//
//						row.insert( vit1->second, 1.0 );
//						row.insert( vit2->second, 1.0 );
//						cHashOp[ c ] = lp->getNumRows();
//
//						lp->addRow( row );
//						restricoes++;
//					}
//			   }
//
//			   // ----------------------------------------------------
//
//			   /*
//
//			   if ( ( h1->getInicio() < h2->getInicio() ) &&
//				    ( fim1 >  h2->getInicio() ) )
//			   {	// intersecao de horarios, com d1 comecando antes de d2
//				    
//				    HorarioAula *hInicio = h2;
//
//					DateTime fim;
//					if ( fim1 < fim2 )
//						fim = fim1;
//					else
//						fim = fim2;
//
//					HorarioAula *h = hInicio;
//					for ( int creds = 1; creds <= nCred2; creds++ ) // adiciona horariosAula do calendario de d2
//					{
//						if ( h == NULL )
//							continue;
//						if ( h->getInicio() >= fim )
//							continue;
//						horarios.add( h );
//						h = d2->getCalendario()->getProximoHorario( h );			
//					}
//
//					h = d1->getCalendario()->getProximoHorario( h1 );
//					for ( int creds = 2; creds <= nCred1; creds++ ) // adiciona horariosAula do calendario de d1
//					{
//						if ( h == NULL )
//							continue;
//						if ( h->getInicio() >= fim )
//							continue;
//						horarios.add( h );
//						h = d1->getCalendario()->getProximoHorario( h );					
//					}
//					
//			   }
//			   else if ( ( h2->getInicio() <  h1->getInicio() ) &&
//					     ( fim2 >  h1->getInicio() ) )
//			   {	// intersecao de horarios, com d2 comecando antes de d1
//				    HorarioAula *hInicio = h1;
//
//					DateTime fim;
//					if ( fim1 < fim2 )
//						fim = fim1;
//					else
//						fim = fim2;
//
//					HorarioAula *h = hInicio;
//					for ( int creds = 1; creds <= nCred1; creds++ ) // adiciona horariosAula do calendario de d1
//					{
//						if ( h == NULL )
//							continue;
//						if ( h->getInicio() >= fim )
//							continue;
//						horarios.add( h );
//						h = d1->getCalendario()->getProximoHorario( h );		
//					}
//
//					h = d2->getCalendario()->getProximoHorario( h2 );
//					for ( int creds = 2; creds <= nCred2; creds++ ) // adiciona horariosAula do calendario de d2
//					{
//						if ( h == NULL )
//							continue;
//						if ( h->getInicio() >= fim )
//							continue;
//						horarios.add( h );
//						h = d2->getCalendario()->getProximoHorario( h );
//					}
//
//			   }
//			   else
//			   {   // sem intersecao de horarios	
//				   continue;
//			   }
//
//			   ITERA_GGROUP( itHor, horarios, HorarioAula )
//			   {
//					c.reset();
//					c.setType( ConstraintOp::C_BLOCO_HORARIO_DISC );
//					c.setBloco( bloco );
//					c.setDia( dia1 );
//					c.setHorarioAula( *itHor );
//					c.setParDiscTurma( d1, v1.getTurma(), d2, v2.getTurma() );
//
//					cit = cHashOp.find( c );
//
//					if ( cit != cHashOp.end() )
//					{
//						auxCoef.first = cit->second; // restricao C_BLOCO_HORARIO_DISC
//						auxCoef.second = vit1->second; // variavel V_X_PROF_AULA_HOR
//						coeffList.push_back( auxCoef );
//						coeffListVal.push_back( 1.0 );
//
//						auxCoef.first = cit->second; // restricao C_BLOCO_HORARIO_DISC
//						auxCoef.second = vit2->second; // variavel V_X_PROF_AULA_HOR
//						coeffList.push_back( auxCoef );
//						coeffListVal.push_back( 1.0 );
//					}
//					else
//					{
//						sprintf( name, "%s", c.toString().c_str() );
//						nnz = 100;
//
//						OPT_ROW row( nnz, OPT_ROW::LESS , 1.0, name );
//
//						row.insert( vit1->second, 1.0 );
//						row.insert( vit2->second, 1.0 );
//						cHashOp[ c ] = lp->getNumRows();
//
//						lp->addRow( row );
//						restricoes++;
//					}   
//				}
//			
//			   */
//			}
//		}
//   }
//   chgCoeffList( coeffList, coeffListVal );
//
//   return restricoes;
//}



int SolverMIP::criaRestricaoAlocAula()
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

int SolverMIP::criaRestricaoProfessorDisciplinaUnico()
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

int SolverMIP::criaRestricaoProfessorDisciplina()
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

      if ( v.getType() != VariableOp::V_Y_PROF_DISCIPLINA
            && v.getType() != VariableOp::V_X_PROF_AULA_HOR )
      {
         continue;
      }

      if ( v.getProfessor() == NULL )
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

         cit = cHashOp.find( c );

         if ( cit != cHashOp.end() )
         {
            auxCoef.first = cit->second;
            auxCoef.second = vit->second;

            coeffList.push_back( auxCoef );
            coeffListVal.push_back( -10.0 );
         }
         else
         {
            sprintf( name, "%s", c.toString().c_str() );
            nnz = 100;

            OPT_ROW row( nnz, OPT_ROW::LESS , 0.0, name );

            row.insert( vit->second, -10.0 );
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
   }

   chgCoeffList( coeffList, coeffListVal );

   return restricoes;
}

int SolverMIP::criaRestricaoFixProfDiscSalaDiaHor()
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

   ITERA_GGROUP_LESSPTR( itFix,
     problemData->fixacoes_Prof_Disc_Sala_Dia_Horario, Fixacao )
   {
      Fixacao * fixacao = ( *itFix );

      c.reset();
      c.setType( ConstraintOp::C_FIX_PROF_DISC_SALA_DIA_HOR );

      vit = vHashOp.begin();

      for (; vit != vHashOp.end(); vit++ )
      {
         VariableOp v = vit->first;

         if ( v.getType() != VariableOp::V_X_PROF_AULA_HOR
            && v.getType() != VariableOp::V_F_FIX_PROF_DISC_SALA_DIA_HOR )
         {
            continue;
         }

         // Compara para verificar se a variavel vai ser fixada
         if ( v.getProfessor()->getId() == fixacao->getProfessorId()
            && v.getDisciplina()->getId() == fixacao->getDisciplinaId()
            && v.getSala()->getId() == fixacao->getSalaId()
            && v.getHorario()->getDia() == fixacao->getDiaSemana()
            && v.getHorario()->getHorarioAulaId()  == fixacao->getHorarioAulaId() )
         {
            c.setProfessor( v.getProfessor() );
            c.setDisciplina( v.getDisciplina() );
            c.setSala( v.getSala() );
            c.setHorario( v.getHorario() );
			c.setDia( v.getHorario()->getDia() );

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

			   nnz = v.getDisciplina()->getNumTurmas() + 1;

               OPT_ROW row( nnz, OPT_ROW::EQUAL , 1.0, name );

               row.insert( vit->second, 1.0 );
               cHashOp[ c ] = lp->getNumRows();

               lp->addRow( row );
               restricoes++;
            }
         }
      }
   }

   return restricoes;
}

int SolverMIP::criaRestricaoFixProfDiscDiaHor()
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

   ITERA_GGROUP_LESSPTR( itFix,
      problemData->fixacoes_Prof_Disc_Dia_Horario, Fixacao )
   {
      Fixacao * fixacao = ( *itFix );

      c.reset();
      c.setType( ConstraintOp::C_FIX_PROF_DISC_DIA_HOR );

      vit = vHashOp.begin();

      for (; vit != vHashOp.end(); vit++ )
      {
         VariableOp v = vit->first;

         if ( v.getType() != VariableOp::V_X_PROF_AULA_HOR
            && v.getType() != VariableOp::V_F_FIX_PROF_DISC_DIA_HOR )
         {
            continue;
         }

         // Compara para verificar se a variavel vai ser fixada
         if ( v.getProfessor()->getId() == fixacao->getProfessorId()
            && v.getDisciplina()->getId() == fixacao->getDisciplinaId()
            && v.getHorario()->getDia() == fixacao->getDiaSemana()
            && v.getHorario()->getHorarioAulaId()  == fixacao->getHorarioAulaId() )
         {
            c.setProfessor( v.getProfessor() );
            c.setDisciplina( v.getDisciplina() );
            c.setHorario( v.getHorario() );
			c.setDia( v.getHorario()->getDia() );

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

               OPT_ROW row( nnz, OPT_ROW::EQUAL , 1.0, name );

               row.insert( vit->second, 1.0 );
               cHashOp[ c ] = lp->getNumRows();

               lp->addRow( row );
               restricoes++;
            }
         }
      }
   }

   return restricoes;
}

int SolverMIP::criaRestricaoFixProfDisc()
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

   ITERA_GGROUP_LESSPTR( itFix, 
      problemData->fixacoes_Prof_Disc_Dia_Horario, Fixacao )
   {
      Fixacao * fixacao = ( *itFix );

      c.reset();
      c.setType( ConstraintOp::C_FIX_PROF_DISC );
      
      vit = vHashOp.begin();

      for (; vit != vHashOp.end(); vit++ )
      {
         VariableOp v = vit->first;

         if ( v.getType() != VariableOp::V_Y_PROF_DISCIPLINA
            && v.getType() != VariableOp::V_F_FIX_PROF_DISC )
         {
            continue;
         }

         // Compara para verificar se a variavel vai ser fixada
         if ( v.getProfessor()->getId() == fixacao->getProfessorId()
            && v.getDisciplina()->getId() == fixacao->getDisciplinaId() )
         {
            c.setProfessor( v.getProfessor() );
            c.setDisciplina( v.getDisciplina() );

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
               
			   nnz = v.getDisciplina()->getNumTurmas() + 1;

               OPT_ROW row( nnz, OPT_ROW::EQUAL , 1.0, name );

               row.insert( vit->second, 1.0 );
               cHashOp[ c ] = lp->getNumRows();

               lp->addRow( row );
               restricoes++;
            }
         }
      }
   }

   return restricoes;
}

int SolverMIP::criaRestricaoFixProfDiscSala()
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

   ITERA_GGROUP_LESSPTR( itFix,
      problemData->fixacoes_Prof_Disc_Dia_Horario, Fixacao )
   {
      Fixacao * fixacao = ( *itFix );

      c.reset();
      c.setType( ConstraintOp::C_FIX_PROF_DISC_SALA );
      
      vit = vHashOp.begin();

      for (; vit != vHashOp.end(); vit++ )
      {
         VariableOp v = vit->first;

         if ( v.getType() != VariableOp::V_Y_PROF_DISCIPLINA
            && v.getType() != VariableOp::V_F_FIX_PROF_DISC_SALA )
         {
            continue;
         }

         // Compara para verificar se a variavel vai ser fixada
         if ( v.getProfessor()->getId() == fixacao->getProfessorId()
            && v.getDisciplina()->getId() == fixacao->getDisciplinaId() )
         {
            c.setProfessor( v.getProfessor() );
            c.setDisciplina( v.getDisciplina() );

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

               OPT_ROW row( nnz, OPT_ROW::EQUAL , 1.0, name );

               row.insert( vit->second, 1.0 );
               cHashOp[ c ] = lp->getNumRows();

               lp->addRow( row );
               restricoes++;
            }
         }
      }
   }

   return restricoes;
}

int SolverMIP::criaRestricaoFixProfSala()
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
 
   ITERA_GGROUP_LESSPTR( itFix,
      problemData->fixacoes_Prof_Disc_Dia_Horario, Fixacao )
   {
      Fixacao * fixacao = ( *itFix );

      c.reset();
      c.setType( ConstraintOp::C_FIX_PROF_SALA );
      
      vit = vHashOp.begin();

      for (; vit != vHashOp.end(); vit++ )
      {
         VariableOp v = vit->first;

         if ( v.getType() != VariableOp::V_X_PROF_AULA_HOR
            && v.getType() != VariableOp::V_F_FIX_PROF_SALA )
         {
            continue;
         }

         // Compara para verificar se a variavel vai ser fixada
         if ( v.getProfessor()->getId() == fixacao->getProfessorId()
            && v.getSala()->getId() != fixacao->getSalaId() )
         {
            c.setProfessor( v.getProfessor() );
            c.setSala( fixacao->sala );

            cit = cHashOp.find( c );

            if ( cit != cHashOp.end() )
            {
               auxCoef.first = cit->second;
               auxCoef.second = vit->second;

               coeffList.push_back( auxCoef );

               if ( v.getType() == VariableOp::V_F_FIX_PROF_SALA )
               {
                  coeffListVal.push_back( -1.0 );
               }
               else
               {
                  coeffListVal.push_back( 1.0 );
               }
            }
            else
            {
               sprintf( name, "%s", c.toString().c_str() );
               nnz = 100;

               OPT_ROW row( nnz, OPT_ROW::EQUAL , 0.0, name );

               if ( v.getType() == VariableOp::V_F_FIX_PROF_SALA )
               {
                  row.insert( vit->second, -1.0 );
               }
               else
               {
                  row.insert( vit->second, 1.0 );
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

int SolverMIP::criaRestricaoDisciplinaMesmoHorario()
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

      if ( v.getType() != VariableOp::V_F_DISC_HOR
         && v.getType() != VariableOp::V_Z_DISCIPLINA_HOR )
      {
         continue;
      }

      if ( v.getType() == VariableOp::V_Z_DISCIPLINA_HOR )
      {
         c.reset();
         c.setType( ConstraintOp::C_DISC_HORARIO );
         c.setDisciplina( v.getDisciplina() );
         c.setTurma( v.getTurma() );
         c.setHorarioAula( v.getHorarioAula() );

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
      else if ( v.getType() == VariableOp::V_F_DISC_HOR )
      {
         c.reset();
         c.setType( ConstraintOp::C_DISC_HORARIO );
         c.setDisciplina( v.getDisciplina() );
         c.setTurma( v.getTurma() );
         c.setHorarioAula( v.getHorarioAula() );
         c.setDia( v.getDia() );

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

   chgCoeffList( coeffList, coeffListVal );

   return restricoes;
}

int SolverMIP::criaRestricaoCustoCorpoDocente()
{
   int restricoes = 0;
   int nnz = 0;
   double rhs = 0.0;
   char name[ 200 ];
   double M = 1000000;

   ConstraintOp c;
   VariableOp v_find;
   VariableOpHash::iterator vit;
   ConstraintOpHash::iterator cit;

   GGroup< Professor *, LessPtr< Professor > > professores
      = problemData->getProfessores();

   // Informa quantas vari�veis 'y' foram criadas para cada professor.
   // Esse valor ser� o 'nnz' das restri��es desse professor
   std::map< Professor *, int > mapProfessorVariaveis;

   ITERA_GGROUP_LESSPTR( it_prof, professores, Professor )
   {
      Professor * professor = ( *it_prof );
      mapProfessorVariaveis[ professor ] += 1;

      vit = vHashOp.begin();
      for (; vit != vHashOp.end(); vit++ )
      {
         if ( vit->first.getType() == VariableOp::V_Y_PROF_DISCIPLINA
            && vit->first.getProfessor() == professor )
         {
            mapProfessorVariaveis[ professor ]++;
         }
      }
   }

   ITERA_GGROUP_LESSPTR( it_prof, professores, Professor )
   {
      Professor * professor = ( *it_prof );

      c.reset();
      c.setType( ConstraintOp::C_CUSTO_CORPO_DOCENTE );
      c.setProfessor( professor );

      if ( cHashOp.find( c ) == cHashOp.end() )
      {
         // Cria duas novas linhas no modelo
         sprintf( name, "%s", c.toString().c_str() );

         // Total de vari�veis 'y' do professor mais um ( que
         // corresponde ao lado direito da desigualdade no modelo )
         nnz = mapProfessorVariaveis[ professor ];

         OPT_ROW row( nnz, OPT_ROW::LESS, rhs, name );

         // Procura pela vari�vel 'cd' do professor
         v_find.reset();
         v_find.setType( VariableOp::V_CUSTO_CORPO_DOCENTE );
         v_find.setProfessor( professor );

         vit = vHashOp.find( v_find );

         if ( vit == vHashOp.end() )
         {
            continue;
         }
      
         VariableOp v = vit->first;

         // Insere a vari�vel 'cd' nas restri��es
         row.insert( vit->second, -1000.0 );

         // Cada var��vel 'y' do professor
         // insere um �ndice nas restri��es
         vit = vHashOp.begin();
         for (; vit != vHashOp.end(); vit++ )
         {
            if ( vit->first.getType() == VariableOp::V_Y_PROF_DISCIPLINA
               && vit->first.getProfessor() == professor )
            {
               row.insert( vit->second, 1.0 );
            }
         }

         cHashOp[ c ] = lp->getNumRows();
         lp->addRow( row );
         restricoes++;
      }
   }

   return restricoes;
}

int SolverMIP::criaRestricaoRelacionaVariavelXDiaProf()
{
	int restricoes = 0;
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

      if ( vOp.getProfessor() == NULL )
      {
         vit++;
         continue;
      }

		Professor *professor = vOp.getProfessor();
		int dia = vOp.getHorario()->getDia();

		c.reset();
		c.setType( ConstraintOp::C_DIAS_PROF_MINISTRA_AULA );
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

			Professor *professor = vOp.getProfessor();
			int dia = vOp.getDia();

			c.reset();
			c.setType( ConstraintOp::C_DIAS_PROF_MINISTRA_AULA );
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

//int SolverMIP::criaRestricaoRelacionaVariavelXDiaProf()
//{
//   int restricoes = 0;
//   int nnz = 100;
//   char name[ 200 ];
//
//   ConstraintOp c;
//   VariableOpHash::iterator vit;
//   ConstraintOpHash::iterator cit;
//
//   GGroup< Professor *, LessPtr< Professor > > professores
//      = problemData->getProfessores();
//
//   // Dias em que cada professor pode dar aula
//   std::map< Professor *, GGroup< int > > mapProfessorDias;
//
//   ITERA_GGROUP_LESSPTR( it_prof, professores, Professor )
//   {
//      Professor * professor = ( *it_prof );
//
//      ITERA_GGROUP_LESSPTR( it_horario_dia, professor->horariosDia, HorarioDia )
//      {
//         mapProfessorDias[ professor ].add( it_horario_dia->getDia() );
//      }
//   }
//
//   ITERA_GGROUP_LESSPTR( it_prof, professores, Professor )
//   {
//      Professor * professor = ( *it_prof );
//      GGroup< int > dias_professor = mapProfessorDias[ professor ];
//
//      ITERA_GGROUP_N_PT( it_dia, dias_professor, int )
//      {
//         int dia = ( *it_dia );
//         GGroup< HorarioDia *, LessPtr< HorarioDia > > horarios_dia_considerados;
//         horarios_dia_considerados.clear();
//
//         c.reset();
//         c.setType( ConstraintOp::C_DIAS_PROF_MINISTRA_AULA );
//         c.setProfessor( professor );
//         c.setDia( dia );
//
//         if ( cHashOp.find( c ) == cHashOp.end() )
//         {
//            sprintf( name, "%s", c.toString().c_str() );
//
//            OPT_ROW row( nnz, OPT_ROW::LESS , 0.0, name );
//
//            // Procura as vari�veis X_p_a_h relacionadas a esse professor e dia
//            vit = vHashOp.begin();
//
//            for (; vit != vHashOp.end(); vit++ )
//            {
//               VariableOp v = vit->first;
//
//               if ( v.getType() != VariableOp::V_X_PROF_AULA_HOR )
//               {
//                  continue;
//               }
//
//               GGroup< HorarioDia *, LessPtr< HorarioDia > >::iterator
//                  it_find = horarios_dia_considerados.find( v.getHorario() );
//
//               if ( v.getHorario()->getDia() == dia
//                  && v.getProfessor() == professor
//                  && it_find == horarios_dia_considerados.end() )
//               {
//                  horarios_dia_considerados.add( v.getHorario() );
//
//                  row.insert( vit->second, 1.0 );
//               }
//            }
//
//            // Inserindo o limite de hor�rios que podem ser considerados
//            VariableOp v;
//            v.reset();
//            v.setType( VariableOp::V_DIAS_PROF_MINISTRA_AULAS );
//            v.setProfessor( professor );
//            v.setDia( dia );
//
//            vit = vHashOp.find( v );
//
//            if ( vit != vHashOp.end() )
//            {
//               // NUM_HORARIOS_DIA * T_p_t
//               double val = (-1.0) * ( horarios_dia_considerados.size() );
//
//               row.insert( vit->second, val );
//            }
//
//            cHashOp[ c ] = lp->getNumRows();
//
//            lp->addRow( row );
//            restricoes++;
//         }
//      }
//   }
//
//   return restricoes;
//}

int SolverMIP::criaRestricaoDisciplinaHorarioUnico()
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

int SolverMIP::criaRestricaoMinimoMestresCurso()
{
   int restricoes = 0;
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

   ITERA_GGROUP_LESSPTR( it_prof, professores, Professor )
   {
      Professor * professor = ( *it_prof );

      ITERA_GGROUP_LESSPTR( it_aula, problemData->aulas, Aula )
      {
         Aula * aula = ( *it_aula );
         Disciplina * disciplina = aula->getDisciplina();

         std::pair< int, int > professor_disciplina (
            professor->getId(), disciplina->getId() );

         // Se o professor e a disciplina da aula em quest�o se relacionarem
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

   std::map< Curso *, GGroup< Professor *, LessPtr< Professor > > >::iterator
      it_map = mapCursoProfessores.begin();

   for (; it_map != mapCursoProfessores.end();
          it_map++ )
   {
      Curso * curso = it_map->first;

      GGroup< Professor *, LessPtr< Professor > > professores_curso
         = it_map->second;

      ITERA_GGROUP_LESSPTR( it_prof, professores_curso, Professor )
      {
         Professor * professor = ( *it_prof );

         c.reset();
         c.setType( ConstraintOp::C_MIN_MEST_CURSO );

         c.setCurso( curso );

         cit = cHashOp.find( c );

         if ( cit == cHashOp.end() )
         {
            sprintf( name, "%s", c.toString().c_str() );
            nnz = professores_curso.size();
            rhs = ( ( curso->regra_min_mestres.second * professores_curso.size() ) / 100.0 );

            OPT_ROW row( nnz, OPT_ROW::GREATER, rhs, name );

            // Recupera os professores que est�o associados ao curso, e que s�o mestres
            VariableOpHash::iterator vit_find = vHashOp.begin();

            for (; vit_find != vHashOp.end(); vit_find++ )
            {
               VariableOp v_find = vit_find->first;

               if ( v_find.getType() == VariableOp::V_PROF_CURSO
                  && v_find.getCurso() == curso
                  && v_find.getProfessor() == professor
                  && ( professor->getTitulacaoId() == 4 || professor->getTitulacaoId() == 5 ) )
               {
                  row.insert( vit_find->second, 1.0 );
               }
            }

            // Variavel de folga
            VariableOp v;
            v.reset();
            v.setType( VariableOp::V_F_MIN_MEST_CURSO );
            v.setCurso( curso );
            vit = vHashOp.find( v );

            if ( vit != vHashOp.end() )
            {
               row.insert( vit->second, 1.0 );
            }

            cHashOp[ c ] = lp->getNumRows();
            lp->addRow( row );

            restricoes++;
         }
      }
   }

   return restricoes;
}

int SolverMIP::criaRestricaoMinimoDoutoresCurso()
{
   int restricoes = 0;
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

   ITERA_GGROUP_LESSPTR( it_prof, professores, Professor )
   {
      Professor * professor = ( *it_prof );

      ITERA_GGROUP_LESSPTR( it_aula, problemData->aulas, Aula )
      {
         Aula * aula = ( *it_aula );
         Disciplina * disciplina = aula->getDisciplina();

         std::pair< int, int > professor_disciplina (
            professor->getId(), disciplina->getId() );

         // Se o professor e a disciplina da aula em quest�o se relacionarem
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

   std::map< Curso *, GGroup< Professor *, LessPtr< Professor > > >::iterator
      it_map = mapCursoProfessores.begin();

   for (; it_map != mapCursoProfessores.end();
          it_map++ )
   {
      Curso * curso = it_map->first;
      GGroup< Professor *, LessPtr< Professor > > professores_curso
         = it_map->second;

      ITERA_GGROUP_LESSPTR( it_prof, professores_curso, Professor )
      {
         Professor * professor = ( *it_prof );

         c.reset();
         c.setType( ConstraintOp::C_MIN_DOUT_CURSO );

         c.setCurso( curso );

         cit = cHashOp.find( c );

         if ( cit == cHashOp.end() )
         {
            sprintf( name, "%s", c.toString().c_str() );
            nnz = professores_curso.size();
            rhs = ( ( curso->regra_min_doutores.second * professores_curso.size() ) / 100.0 );

            OPT_ROW row( nnz, OPT_ROW::GREATER, rhs, name );

            // Recupera os professores que est�o associados ao curso, e que s�o mestres
            VariableOpHash::iterator vit_find = vHashOp.begin();

            for (; vit_find != vHashOp.end(); vit_find++ )
            {
               VariableOp v_find = vit_find->first;

               if ( v_find.getType() == VariableOp::V_PROF_CURSO
                  && v_find.getCurso() == curso
                  && v_find.getProfessor() == professor
                  && professor->getTitulacaoId() == 5 )
               {
                  row.insert( vit_find->second, 1.0 );
               }
            }

            // Variavel de folga
            VariableOp v;
            v.reset();
            v.setType( VariableOp::V_F_MIN_DOUT_CURSO );
            v.setCurso( curso );
            vit = vHashOp.find( v );

            if ( vit != vHashOp.end() )
            {
               row.insert( vit->second, 1.0 );
            }

            cHashOp[ c ] = lp->getNumRows();
            lp->addRow( row );

            restricoes++;
         }
      }
   }

   return restricoes;
}

int SolverMIP::criaRestricaoAlocacaoProfessorCurso()
{
	int restricoes = 0;
	int nnz = 0;
	double rhs = 0.0;
	char name[ 200 ];
	int bigM = 1000;

	ConstraintOp c;
	VariableOpHash::iterator vit;
	VariableOpHash::iterator vit_f;
	ConstraintOpHash::iterator cit;

	// Informando quais disciplinas est�o associadas a cada curso
	std::map< Disciplina *, GGroup< Curso *,
		LessPtr< Curso > > > mapCursoDisciplinas;

	ITERA_GGROUP_LESSPTR( it_aula, problemData->aulas, Aula )
	{
		Aula * aula = ( *it_aula );

		ITERA_GGROUP_LESSPTR( it_oferta, aula->ofertas, Oferta )
		{
			Oferta * oferta = ( *it_oferta );

			mapCursoDisciplinas[ aula->getDisciplina() ].add( oferta->curso );
		}
	}

	vit = vHashOp.begin();

	while(vit != vHashOp.end())
	{
		VariableOp v = ( vit->first );

		if (v.getType() != VariableOp::V_PROF_CURSO)
		{
			vit++;
			continue;
		}

		c.reset();
		c.setType( ConstraintOp::C_ALOC_PROF_CURSO );
		c.setProfessor( v.getProfessor() );
		c.setCurso( v.getCurso() );

		cit = cHashOp.find(c);
		if ( cit == cHashOp.end() )
		{
			cHashOp[ c ] = lp->getNumRows();

			sprintf( name, "%s", c.toString().c_str() );
			OPT_ROW row( 100, OPT_ROW::GREATER, rhs, name );

			row.insert( vit->second, bigM );

			lp->addRow( row );
			restricoes++;
		}
		else
		{
			lp->chgCoef(cit->second, vit->second, 1.0);
		}

		vit++;
	}

	vit = vHashOp.begin();

	while(vit != vHashOp.end())
	{
		VariableOp v = ( vit->first );

		if (v.getType() != VariableOp::V_Y_PROF_DISCIPLINA)
		{
			vit++;
			continue;
		}

		GGroup< Curso *, LessPtr< Curso > > cursos = mapCursoDisciplinas[v.getDisciplina()];
		for(GGroup< Curso *, LessPtr< Curso > >::iterator itC = cursos.begin();
			itC != cursos.end();
			itC++)
		{
			c.reset();
			c.setType( ConstraintOp::C_ALOC_PROF_CURSO );
			c.setProfessor( v.getProfessor() );
			c.setCurso( *itC );

			cit = cHashOp.find(c);
			if ( cit != cHashOp.end() )
			{
				lp->chgCoef(cit->second, vit->second, -1.0);
			}
		}

		vit++;
	}

	return restricoes;
}

//int SolverMIP::criaRestricaoAlocacaoProfessorCurso()
//{
//   int restricoes = 0;
//   int nnz = 0;
//   double rhs = 0.0;
//   char name[ 200 ];
//   int bigM = 1000;
//
//   ConstraintOp c;
//   VariableOpHash::iterator vit;
//   VariableOpHash::iterator vit_f;
//   ConstraintOpHash::iterator cit;
//
//   // Informando quais disciplinas est�o associadas a cada curso
//   std::map< Curso *, GGroup< Disciplina *,
//      LessPtr< Disciplina > > > mapCursoDisciplinas;
//
//   ITERA_GGROUP_LESSPTR( it_aula, problemData->aulas, Aula )
//   {
//      Aula * aula = ( *it_aula );
//
//      ITERA_GGROUP_LESSPTR( it_oferta, aula->ofertas, Oferta )
//      {
//         Oferta * oferta = ( *it_oferta );
//
//         mapCursoDisciplinas[ oferta->curso ].add( aula->getDisciplina() );
//      }
//   }
//
//   GGroup< Professor *, LessPtr< Professor > > professores
//      = problemData->getProfessores();
//
//   int totalCursos = problemData->cursos.size();
//   int totalProfessores = professores.size();
//
//   ITERA_GGROUP_LESSPTR( itProf, professores, Professor )
//   {
//      Professor * professor = ( *itProf );
//
//      ITERA_GGROUP_LESSPTR( itCurso, problemData->cursos, Curso )
//      {
//         Curso * curso = ( *itCurso );
//
//         VariableOp v;
//         v.reset();
//
//         v.setType( VariableOp::V_PROF_CURSO );
//         v.setProfessor( professor );
//         v.setCurso( curso );
//         vit = vHashOp.find( v );
//
//         if ( vit != vHashOp.end() )
//         {
//            // Cria a restri�ao de m ( p, c ) * M >= E ( E ( y( p, d, i ) ) )
//            c.reset();
//            c.setType( ConstraintOp::C_ALOC_PROF_CURSO );
//
//            c.setProfessor( v.getProfessor() );
//            c.setCurso( v.getCurso() );
//
//            cit = cHashOp.find( c );
//
//            if ( cit == cHashOp.end() )
//            {
//               sprintf( name, "%s", c.toString().c_str() );
//               nnz = ( totalProfessores + totalCursos );
//
//               OPT_ROW row( nnz, OPT_ROW::GREATER, rhs, name );
//               row.insert( vit->second, bigM );
//
//               VariableOpHash::iterator vit_find = vHashOp.begin();
//
//               for (; vit_find != vHashOp.end(); vit_find++ )
//               {
//                  VariableOp v_find = vit_find->first;
//
//                  if ( v_find.getType() == VariableOp::V_Y_PROF_DISCIPLINA 
//                        && v_find.getProfessor() == professor )
//                  {
//                     // Verifica se a disciplina da vari�vel est� associada ao curso
//                     GGroup< Disciplina *, LessPtr< Disciplina > >::iterator
//                        it_find = mapCursoDisciplinas[ curso ].find( v_find.getDisciplina() );
//
//                     if ( it_find != mapCursoDisciplinas[ curso ].end() )
//                     {
//                        row.insert( vit_find->second, -1.0 );
//                     }
//                  }
//               }
//
//               cHashOp[ c ] = lp->getNumRows();
//               lp->addRow( row );
//
//               restricoes++;
//            }
//         }
//      }
//   }
//
//   return restricoes;
//}


int SolverMIP::criaRestricaoCargaHorariaMinimaProfessor()
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

      // Armazena as disciplinas desse professor que j� foram consideradas
      GGroup< Disciplina *, LessPtr< Disciplina > > disciplinas_prof;
      disciplinas_prof.clear();

      c.reset();
      c.setType( ConstraintOp::C_CARGA_HOR_MIN_PROF );
      c.setProfessor( professor );

      cit = cHashOp.find( c );

      if ( cit == cHashOp.end() )
      {
         sprintf( name, "%s", c.toString().c_str() );
         rhs = professor->getChAnterior();

         OPT_ROW row( nnz, OPT_ROW::GREATER, rhs, name );
         
         vit = vHashOp.begin();

         for (; vit != vHashOp.end(); vit++ )
         {
            VariableOp v = vit->first;

            if ( v.getType() == VariableOp::V_X_PROF_AULA_HOR
               && v.getProfessor() == professor )
            {
               // Verifica se a disciplina j� foi considerada
               GGroup< Disciplina *, LessPtr< Disciplina > >::iterator
                  it_find = disciplinas_prof.find( v.getDisciplina() );

               if ( it_find != disciplinas_prof.end() )
               {
                  continue;
               }
               ///////

               // Verifica se o professor est� alocado a essa disciplina
               bool encontrou = false;

               GGroup< std::pair< Aula *, Disciplina * > >::iterator
                  it_disciplinas = problemData->mapProfessorDisciplinas[ professor ].begin();

               for (; it_disciplinas != problemData->mapProfessorDisciplinas[ professor ].end();
                      it_disciplinas++ )
               {
                  if ( ( *it_disciplinas ).second == v.getDisciplina() )
                  {
                     encontrou = true;
                     break;
                  }
               }

               if ( encontrou )
               {
                  disciplinas_prof.add( v.getDisciplina() );
                  double val = ( v.getDisciplina()->getCredTeoricos() + v.getDisciplina()->getCredPraticos() );
                  row.insert( vit->second, val );
               }
               ///////
            }
         }

         // Insere a vari�vel de folga na restri��o
         VariableOp v;
         v.reset();
         v.setType( VariableOp::V_F_CARGA_HOR_MIN_PROF );
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

int SolverMIP::criaRestricaoCargaHorariaMinimaProfessorSemana()
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
               // Verifica se o professor est� alocado a essa disciplina
               bool encontrou = false;

               GGroup< std::pair< Aula *, Disciplina * > >::iterator
                  it_disciplinas = problemData->mapProfessorDisciplinas[ professor ].begin();

               for (; it_disciplinas != problemData->mapProfessorDisciplinas[ professor ].end();
                      it_disciplinas++ )
               {
                  if ( ( *it_disciplinas ).second == v.getDisciplina() )
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

         // Insere a vari�vel de folga na restri��o
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

int SolverMIP::criaRestricaoCargaHorariaMaximaProfessorSemana()
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
      c.setType( ConstraintOp::C_CARGA_HOR_MAX_PROF_SEMANA );
      c.setProfessor( professor );

      cit = cHashOp.find( c );

      if ( cit == cHashOp.end() )
      {
         sprintf( name, "%s", c.toString().c_str() );
         rhs = ( professor->getChMax() > 0 ? professor->getChMax() : 1 );

         OPT_ROW row( nnz, OPT_ROW::LESS, rhs, name );

         vit = vHashOp.begin();

         for (; vit != vHashOp.end(); vit++ )
         {
            VariableOp v = vit->first;

            if ( v.getType() == VariableOp::V_X_PROF_AULA_HOR
               && v.getProfessor() == professor )
            {
               // Verifica se o professor est� alocado a essa disciplina
               bool encontrou = false;

               GGroup< std::pair< Aula *, Disciplina * > >::iterator
                  it_disciplinas = problemData->mapProfessorDisciplinas[ professor ].begin();

               for (; it_disciplinas != problemData->mapProfessorDisciplinas[ professor ].end();
                      it_disciplinas++ )
               {
                  if ( ( *it_disciplinas ).second == v.getDisciplina() )
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
            }
         }

         // Insere a vari�vel de folga na restri��o
         VariableOp v;
         v.reset();
         v.setType( VariableOp::V_F_CARGA_HOR_MAX_PROF_SEMANA );
         v.setProfessor( professor );

         vit = vHashOp.find( v );

         if ( vit != vHashOp.end() )
         {
            row.insert( vit->second, -1.0 );
         }

         cHashOp[ c ] = lp->getNumRows();
         lp->addRow( row );

         restricoes++;
      }
   }

   return restricoes;
}

int SolverMIP::criaRestricaoAvaliacaoCorpoDocente()
{
   int restricoes = 0;
   int nnz = 0;
   double rhs = 0.0;
   char name[ 200 ];
   double M = 1000000;

   ConstraintOp c;
   VariableOp v_find;
   VariableOpHash::iterator vit;
   ConstraintOpHash::iterator cit;

   GGroup< Professor *, LessPtr< Professor > > professores
      = problemData->getProfessores();

   // Informa quantas vari�veis 'y' foram criadas para cada professor.
   // Esse valor ser� o 'nnz' das restri��es desse professor
   std::map< Professor *, int > mapProfessorVariaveis;

   ITERA_GGROUP_LESSPTR( it_prof, professores, Professor )
   {
      Professor * professor = ( *it_prof );
      mapProfessorVariaveis[ professor ] = 1;

      vit = vHashOp.begin();
      for (; vit != vHashOp.end(); vit++ )
      {
         if ( vit->first.getType() == VariableOp::V_Y_PROF_DISCIPLINA
            && vit->first.getProfessor() == professor )
         {
            mapProfessorVariaveis[ professor ]++;
         }
      }
   }

   ITERA_GGROUP_LESSPTR( it_prof, professores, Professor )
   {
      Professor * professor = ( *it_prof );

      c.reset();
      c.setType( ConstraintOp::C_AVALIACAO_CORPO_DOCENTE );
      c.setProfessor( professor );

      if ( cHashOp.find( c ) == cHashOp.end() )
      {
         // Cria duas novas linhas no modelo
         sprintf( name, "%s", c.toString().c_str() );

         // Total de vari�veis 'y' do professor mais um ( que
         // corresponde ao lado direito da desigualdade no modelo )
         nnz = mapProfessorVariaveis[ professor ];

         if ( nnz == 0 )
         {
            continue;
         }

         OPT_ROW row( nnz, OPT_ROW::LESS, rhs, name );

         // Procura pela vari�vel 'l' do professor
         v_find.reset();
         v_find.setType( VariableOp::V_AVALIACAO_CORPO_DOCENTE );
         v_find.setProfessor( professor );

         vit = vHashOp.find( v_find );

         if ( vit == vHashOp.end() )
         {
            continue;
         }
      
         VariableOp v = vit->first;

         // Insere a vari�vel 'l' nas restri��es
         row.insert( vit->second, -1000.0 );
         ///////

         // Cada var��vel 'y' do professor
         // insere um �ndice nas restri��es
         vit = vHashOp.begin();
         for (; vit != vHashOp.end(); vit++ )
         {
            if ( vit->first.getType() == VariableOp::V_Y_PROF_DISCIPLINA
               && vit->first.getProfessor() == professor )
            {
               row.insert( vit->second, 1.0 );
            }
         }
         ///////

         cHashOp[ c ] = lp->getNumRows();
         lp->addRow( row );

         restricoes++;
      }
   }

   return restricoes;
}

int SolverMIP::criaRestricaoMaxDiscProfCurso()
{
   int restricoes = 0;
   int nnz = 0;
   double rhs = 0.0;
   char name[ 200 ];
   double M = 1000000.0;

   ConstraintOp c;
   VariableOp v_find;
   VariableOpHash::iterator vit;
   ConstraintOpHash::iterator cit;

	vit = vHashOp.begin();

	while(vit != vHashOp.end())
	{
		VariableOp v = ( vit->first );

		if (v.getType() != VariableOp::V_MAX_DISC_PROF_CURSO && v.getType() != VariableOp::V_Y_PROF_DISCIPLINA
			&& v.getType() != VariableOp::V_F_MAX_DISC_PROF_CURSO)
		{
			vit++;
			continue;
		}

		if(v.getType() == VariableOp::V_MAX_DISC_PROF_CURSO)
		{
			c.reset();
			c.setType( ConstraintOp::C_MAX_DISC_PROF_CURSO );
			c.setProfessor( v.getProfessor() );
			c.setCurso( v.getCurso() );
			c.setDisciplina( v.getDisciplina() );

			cit = cHashOp.find(c);
			if ( cit == cHashOp.end() )
			{
				cHashOp[ c ] = lp->getNumRows();

				sprintf( name, "%s", c.toString().c_str() );
				OPT_ROW row( 100, OPT_ROW::LESS, 0.0, name );

				double M = ( v.getDisciplina()->getCredTeoricos() + v.getDisciplina()->getCredPraticos() );
				row.insert( vit->second, -M );

				lp->addRow( row );
				restricoes++;
			}
			else
			{
				double M = ( v.getDisciplina()->getCredTeoricos() + v.getDisciplina()->getCredPraticos() );
				lp->chgCoef(cit->second, vit->second, -M);
			}

			c.reset();
			c.setType( ConstraintOp::C_MAX_DISC_PROF_CURSO2 );
			c.setProfessor( v.getProfessor() );
			c.setCurso( v.getCurso() );

			cit = cHashOp.find(c);
			if ( cit == cHashOp.end() )
			{
				cHashOp[ c ] = lp->getNumRows();

				sprintf( name, "%s", c.toString().c_str() );
				rhs = v.getCurso()->getQtdMaxProfDisc();
				OPT_ROW row( 100, OPT_ROW::LESS, rhs, name );

				double M = ( v.getDisciplina()->getCredTeoricos() + v.getDisciplina()->getCredPraticos() );
				row.insert( vit->second, 1.0 );

				lp->addRow( row );
				restricoes++;
			}
			else
			{
				lp->chgCoef(cit->second, vit->second, 1.0);
			}
		}
		else if(v.getType() == VariableOp::V_Y_PROF_DISCIPLINA)
		{
			ITERA_GGROUP_LESSPTR( it_curso, problemData->cursos, Curso )
			{
				Curso * curso = *it_curso;

				if(!problemData->aulaAtendeCurso( v.getAula(), curso))
					continue;

				if ( !curso->possuiDisciplina( v.getDisciplina() ) )
					continue;

				c.reset();
				c.setType( ConstraintOp::C_MAX_DISC_PROF_CURSO );
				c.setProfessor( v.getProfessor() );
				c.setCurso( curso );
				c.setDisciplina( v.getDisciplina() );

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
			}
		}
		else if(v.getType() == VariableOp::V_F_MAX_DISC_PROF_CURSO)
		{
			c.reset();
			c.setType( ConstraintOp::C_MAX_DISC_PROF_CURSO2 );
			c.setProfessor( v.getProfessor() );
			c.setCurso( v.getCurso() );

			cit = cHashOp.find(c);
			if ( cit == cHashOp.end() )
			{
				cHashOp[ c ] = lp->getNumRows();

				sprintf( name, "%s", c.toString().c_str() );
				rhs = v.getCurso()->getQtdMaxProfDisc();
				OPT_ROW row( 100, OPT_ROW::LESS, rhs, name );

				double M = ( v.getDisciplina()->getCredTeoricos() + v.getDisciplina()->getCredPraticos() );
				row.insert( vit->second, -1.0 );

				lp->addRow( row );
				restricoes++;
			}
			else
			{
				lp->chgCoef(cit->second, vit->second, -1.0);
			}
		}

		vit++;
	}


	return restricoes;
}

//int SolverMIP::criaRestricaoMaxDiscProfCurso()
//{
//   int restricoes = 0;
//   int nnz = 0;
//   double rhs = 0.0;
//   char name[ 200 ];
//   double M = 1000000.0;
//
//   ConstraintOp c;
//   VariableOp v_find;
//   VariableOpHash::iterator vit;
//   ConstraintOpHash::iterator cit;
//
//   GGroup< Professor *, LessPtr< Professor > > professores
//      = problemData->getProfessores();
//
//   GGroup< Disciplina *, LessPtr< Disciplina > > disciplinas
//      = problemData->disciplinas;
//
//   GGroup< Curso *, LessPtr< Curso > > cursos
//      = problemData->cursos;
//
//   GGroup< Aula *, LessPtr< Aula > > aulas
//      = problemData->aulas;
//
//   ITERA_GGROUP_LESSPTR( it_prof, professores, Professor )
//   {
//      Professor * professor = ( *it_prof );
//      
//      ITERA_GGROUP_LESSPTR( it_curso, cursos, Curso )
//      {
//         Curso * curso = ( *it_curso );
//
//         c.reset();
//         c.setType( ConstraintOp::C_MAX_DISC_PROF_CURSO );
//
//         c.setProfessor( professor );
//         c.setCurso( curso );
//
//         if ( cHashOp.find( c ) == cHashOp.end() )
//         {
//            sprintf( name, "%s", c.toString().c_str() );
//
//            ITERA_GGROUP_LESSPTR( it_disc, disciplinas, Disciplina )
//            {
//               Disciplina * disciplina = ( *it_disc );
//
//			   if ( !curso->possuiDisciplina( disciplina ) )
//					continue;
//
//               // Procura pela vari�vel 'Lpcd'
//               v_find.reset();
//               v_find.setType( VariableOp::V_MAX_DISC_PROF_CURSO );
//
//               v_find.setProfessor( professor );
//               v_find.setCurso( curso );
//               v_find.setDisciplina( disciplina );
//
//               vit = vHashOp.find( v_find );
//
//               if ( vit == vHashOp.end() )
//               {
//                  continue;
//               }
//
//               // Hash da vari�vel 'Lpcd'
//               int idHashVLpcd = vit->second;
//               ///////
//
//               // Sum[ Xpah ] - ( M * Lpcd ) <= 0
//               rhs = 0.0;
//               nnz = ( disciplina->getCredTeoricos() + disciplina->getCredPraticos() + 1 );
//
//               OPT_ROW row1( nnz, OPT_ROW::LESS, rhs, name );
//
//               // M�ximo de hor�rios que a disciplina pode ocupar
//               M = ( disciplina->getCredTeoricos() + disciplina->getCredPraticos() );
//
//               // ( - M * Lpcd )
//               row1.insert( idHashVLpcd, -M );
//
//               vit = vHashOp.begin();
//               for (; vit != vHashOp.end(); vit++ )
//               {
//                  if ( vit->first.getType() == VariableOp::V_Y_PROF_DISCIPLINA
//                     && vit->first.getProfessor() == professor
//                     && vit->first.getDisciplina() == disciplina )
//                  {
//                     if ( problemData->aulaAtendeCurso( vit->first.getAula(), curso ) )
//                     {
//                        // Sum[ Xpah ]
//                        row1.insert( vit->second, 1.0 );
//                     }
//                  }
//               }
//
//               lp->addRow( row1 );
//               ///////
//            }
//
//            // SUM[ Lpcd ] - SUM[ LpcdSlack ] <= MaxDiscCurso
//            rhs = curso->getQtdMaxProfDisc();
//            nnz = ( 2 * disciplinas.size() );
//
//            OPT_ROW row2( nnz, OPT_ROW::LESS, rhs, name );
//
//            vit = vHashOp.begin();
//            for (; vit != vHashOp.end(); vit++ )
//            {
//               if ( vit->first.getType() == VariableOp::V_MAX_DISC_PROF_CURSO
//                  && vit->first.getProfessor() == professor
//                  && vit->first.getCurso() == curso )
//               {
//                  // Sum[ Lpcd ]
//                  row2.insert( vit->second, 1.0 );
//               }
//
//               if ( vit->first.getType() == VariableOp::V_F_MAX_DISC_PROF_CURSO
//                  && vit->first.getProfessor() == professor
//                  && vit->first.getCurso() == curso )
//               {
//                  // - SUM[ LpcdSlack ]
//                  row2.insert( vit->second, -1.0 );
//               }
//            }
//
//            lp->addRow( row2 );
//            ///////
//
//            cHashOp[ c ] = lp->getNumRows();
//            restricoes++;
//         }
//      }
//   }
//
//   return restricoes;
//}

int SolverMIP::criaRestricaoDeslocamentoProfessor()
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

   // Hash que armazena apenas as vari�veis 'Xpah'
   VariableOpHash hashX;
   vit1 = vHashOp.begin();

   for (; vit1 != vHashOp.end(); vit1++ )
   {
      if ( vit1->first.getType() == VariableOp::V_X_PROF_AULA_HOR )
      {
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
                  // Cria a restri��o 'Xpah1 + Xpah2 <= 1'
                  // --> Aloca no m�ximo uma das aulas ao professor
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

int SolverMIP::criaRestricaoDeslocamentoViavel()
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

   // Hash que armazena apenas as vari�veis 'Xpah'
   VariableOpHash hashX;
   vit1 = vHashOp.begin();

   for (; vit1 != vHashOp.end(); vit1++ )
   {
      if ( vit1->first.getType() == VariableOp::V_X_PROF_AULA_HOR )
      {
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
                     // Cria a restri��o 'Xpah1 + Xpah2 <= 1'
                     // --> Aloca no m�ximo uma das aulas ao professor
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

int SolverMIP::criaRestricaoUltimaPrimeiraAulas()
{
   int restricoes = 0;
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
      if ( vit1->first.getType() == VariableOp::V_X_PROF_AULA_HOR && vit1->first.getProfessor() != NULL)
      {
		  VariableOp v = vit1->first;

		  if(problemData->verificaPrimeiraAulas(v.getHorario()))
			  mapPrimeirasAulas[v.getProfessor()][v.getHorario()].push_back(vit1);
		  else if(problemData->verificaUltimaAulas(v.getHorario()))
			  mapUltimassAulas[v.getProfessor()][v.getHorario()].push_back(vit1);
      }
   }

   map< Professor*, map< HorarioDia*, vector< VariableOpHash::iterator > , LessPtr < HorarioDia > >, LessPtr< Professor > >::iterator itP1 = mapPrimeirasAulas.begin();
   for(; itP1 != mapPrimeirasAulas.end(); itP1++)
   {
	   Professor *professor = itP1->first;

	   map< HorarioDia*, vector< VariableOpHash::iterator > , LessPtr < HorarioDia > >::iterator itP2 = itP1->second.begin();
	   for(; itP2 != itP1->second.end(); itP2++)
	   {
		   HorarioDia *horario1 = itP2->first;

		   map< HorarioDia*, vector< VariableOpHash::iterator > , LessPtr < HorarioDia > > mapTemp = mapUltimassAulas[professor];
		   map< HorarioDia*, vector< VariableOpHash::iterator > , LessPtr < HorarioDia > >::iterator itU1 = mapTemp.begin();
		   for(; itU1 != mapTemp.end(); itU1++)
		   {
			   HorarioDia *horario2 = itU1->first;

			   if(!problemData->verificaUltimaPrimeiraAulas(horario1, horario2))
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


			   // Adiciona a vari�vel de folga
			   VariableOp vSlack;
			   vSlack.reset();
			   vSlack.setType( VariableOp::V_F_ULTIMA_PRIMEIRA_AULA_PROF );

			   vSlack.setProfessor( professor );
			   vSlack.setHorarioDiaD( horario1 );
			   vSlack.setHorarioDiaD1( horario2 );

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

//int SolverMIP::criaRestricaoUltimaPrimeiraAulas()
//{
//   int restricoes = 0;
//   int nnz = 3;
//   double rhs = 1.0;
//   char name[ 200 ];
//
//   ConstraintOp c;
//   VariableOpHash::iterator vit1;
//   VariableOpHash::iterator vit2;
//   VariableOpHash::iterator vitSlack;
//
//   // Hash que armazena apenas as vari�veis 'Xpah'
//   VariableOpHash hashX;
//   vit1 = vHashOp.begin();
//
//   for (; vit1 != vHashOp.end(); vit1++ )
//   {
//      if ( vit1->first.getType() == VariableOp::V_X_PROF_AULA_HOR )
//      {
//         hashX[ vit1->first ] = vit1->second;
//      }
//   }
//   
//   vit1 = hashX.begin();
//
//   for (; vit1 != hashX.end(); vit1++ )
//   {
//      VariableOp v1 = vit1->first;
//
//      vit2 = vit1;
//      vit2++;
//
//      for (; vit2 != hashX.end(); vit2++ )
//      {
//         VariableOp v2 = vit2->first;
//
//         if ( v1.getProfessor() != v2.getProfessor() )
//         {
//            continue;
//         }
//
//         Professor * professor = ( v1.getProfessor() );
//
//         // Verifica se essas aulas ocorrem no �ltimo
//         // hor�rio do dia D e no primeiro hor�rio do dia ( D + 1 )
//         bool verificaAulas = problemData->verificaUltimaPrimeiraAulas(
//            v1.getHorario(), v2.getHorario() );
//
//         if ( !verificaAulas )
//         {
//            continue;
//         }
//
//         // Insere a nova restri��o professor + dias + hor�rios
//         c.reset();
//         c.setType( ConstraintOp::C_ULTIMA_PRIMEIRA_AULA_PROF );
//         c.setProfessor( professor );
//         c.setHorarioDiaD( v1.getHorario() );
//         c.setHorarioDiaD1( v2.getHorario() );
//
//         if ( cHashOp.find( c ) != cHashOp.end() )
//         {
//            continue;
//         }
//
//         sprintf( name, "%s", c.toString().c_str() );
//
//         cHashOp[ c ] = lp->getNumRows();
//         restricoes++;
//
//         // Cria a restri��o 'Xpah1 + Xpah2 <= 1'
//         // --> Aloca no m�ximo uma das aulas ao professor
//         OPT_ROW row( nnz, OPT_ROW::LESS, rhs, name );
//
//         row.insert( vit1->second, 1.0 );
//         row.insert( vit2->second, 1.0 );
//
//         // Adiciona a vari�vel de folga
//         VariableOp vSlack;
//         vSlack.reset();
//         vSlack.setType( VariableOp::V_F_ULTIMA_PRIMEIRA_AULA_PROF );
//
//         vSlack.setProfessor( professor );
//         vSlack.setHorarioDiaD( v1.getHorario() );
//         vSlack.setHorarioDiaD1( v2.getHorario() );
//
//         vitSlack = vHashOp.find( vSlack );
//
//         if ( vitSlack != vHashOp.end() )
//         {
//            row.insert( vitSlack->second, 1.0 );
//         }
//
//         // Apenar adicionamos a restri��o no modelo se existir
//         // a vari�vel de folga, para que n�o aconte�a inviabilidades
//         lp->addRow( row );
//      }
//   }
//
//   return restricoes;
//}

int SolverMIP::criaRestricaoGapsProfessores()
{
   int restricoes = 0;

   ConstraintOp c;
   VariableOpHash::iterator vit1;
   VariableOpHash::iterator vit2;

   const int totalHorariosAula = (int)(
     problemData->horarios_aula_ordenados .size() );

   // Hor�rios * Aulas * Dias Letivos
   int nnz = ( totalHorariosAula * (int)( problemData->aulas.size() ) * 5 );
   double rhs = 1.0;
   char name[ 200 ];

   if ( nnz == 0 )
   {
      return 0;
   }

   map< Professor*, map< int, vector< VariableOpHash::iterator > >, LessPtr< Professor > > variaveisHashGapsProfessores;
   map< Professor*, map< int, vector< VariableOpHash::iterator > >, LessPtr< Professor > > variaveisHashX;

   vit1 = vHashOp.begin();

   for (; vit1 != vHashOp.end(); vit1++ )
   {
      VariableOp v = ( vit1->first );

	  if ( v.getType() == VariableOp::V_GAPS_PROFESSORES && v.getProfessor() != NULL )
      {
		  variaveisHashGapsProfessores[v.getProfessor()][v.getDia()].push_back(vit1);
      }
      else if ( v.getType() == VariableOp::V_X_PROF_AULA_HOR && v.getProfessor() != NULL )
      {
         variaveisHashX[v.getProfessor()][v.getDia()].push_back(vit1);
      }
   }

   map< Professor*, map< int, vector< VariableOpHash::iterator > >, LessPtr< Professor > >::iterator it1 = variaveisHashGapsProfessores.begin();
   for(; it1 != variaveisHashGapsProfessores.end(); it1++)
   {
	   Professor *professor = it1->first;

	   map< int, vector< VariableOpHash::iterator > >::iterator it2 = it1->second.begin();
	   for(; it2 != it1->second.end(); it2++)
	   {
		   int dia = it2->first;

		   vector< VariableOpHash::iterator >::iterator it3 = it2->second.begin();
		   for(; it3 != it2->second.end(); it3++)
		   {
			   vit1 = *it3;
			   VariableOp v_temp = vit1->first;

			   // Hor�rios nos extremos do intervalo de hor�rios
			   HorarioAula * h1 = v_temp.getH1();
			   HorarioAula * h2 = v_temp.getH2();

			   c.reset();
			   c.setType( ConstraintOp::C_GAPS_PROFESSORES );
			   c.setProfessor( professor );
			   c.setDia( dia );
			   c.setH1( h1 );
			   c.setH2( h2 );

			   if ( cHashOp.find( c ) == cHashOp.end() )
			   {
				   sprintf( name, "%s", c.toString().c_str() );

				   OPT_ROW row( nnz, OPT_ROW::LESS, rhs, name );

				   // Vari�vel do gap
				   row.insert( vit1->second, -1.0 );

				   bool inseriuVariavel = false;

				   vector< VariableOpHash::iterator > vars = variaveisHashX[professor][dia];
				   vector< VariableOpHash::iterator >::iterator it4 = vars.begin();

				   for(; it4 != vars.end(); it4++)
				   {
					   vit2 = *it4;
					   VariableOp v_x = vit2->first;

					   if ( v_x.getHorario()->getHorarioAula()->getInicio() >= h1->getInicio()
						   && v_x.getHorario()->getHorarioAula()->getInicio() <= h2->getInicio() )
					   {
						   inseriuVariavel = true;

						   // Vari�veis dos hor�rios nos extremos do intervalo
						   if ( v_x.getHorario()->getHorarioAula()->getInicio() == h1->getInicio()
							   || v_x.getHorario()->getHorarioAula()->getInicio() == h2->getInicio() )
						   {
							   row.insert( vit2->second, 1.0 );
						   }
						   // Vari�veis dos hor�rios no meio do intervalo
						   else
						   {
							   row.insert( vit2->second, -1.0 );
						   }
					   }
				   }

				   if ( inseriuVariavel )
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

//int SolverMIP::criaRestricaoGapsProfessores()
//{
//   int restricoes = 0;
//
//   ConstraintOp c;
//   VariableOpHash::iterator vit1;
//   VariableOpHash::iterator vit2;
//
//   const int totalHorariosAula = (int)(
//     problemData->horarios_aula_ordenados .size() );
//
//   // Hor�rios * Aulas * Dias Letivos
//   int nnz = ( totalHorariosAula * (int)( problemData->aulas.size() ) * 5 );
//   double rhs = 1.0;
//   char name[ 200 ];
//
//   if ( nnz == 0 )
//   {
//      return 0;
//   }
//
//   VariableOpHash variaveisHashGapsProfessores;
//   VariableOpHash variaveisHashX;
//
//   vit1 = vHashOp.begin();
//
//   for (; vit1 != vHashOp.end(); vit1++ )
//   {
//      VariableOp v = ( vit1->first );
//
//      if ( v.getType() == VariableOp::V_GAPS_PROFESSORES )
//      {
//         variaveisHashGapsProfessores[ v ] = ( vit1->second );
//      }
//      else if ( v.getType() == VariableOp::V_X_PROF_AULA_HOR )
//      {
//         variaveisHashX[ v ] = ( vit1->second );
//      }
//   }
//
//   GGroup< Professor *, LessPtr< Professor > > professores
//      = problemData->getProfessores();
//
//   GGroup< int > dias_letivos;
//
//   for ( int i = 1; i <= 7; i++ )
//   {
//      dias_letivos.add( i );
//   }
//
//   ITERA_GGROUP_LESSPTR( it_prof, professores, Professor )
//   {
//      Professor * professor = ( *it_prof );
//
//      ITERA_GGROUP_N_PT( it_dia, dias_letivos, int )
//      {
//         int dia = ( *it_dia );
//
//         vit1 = variaveisHashGapsProfessores.begin();
//
//         for (; vit1 != variaveisHashGapsProfessores.end(); vit1++ )
//         {
//            VariableOp v_temp = vit1->first;
//
//            if ( v_temp.getProfessor() == professor
//               && v_temp.getDia() == dia )
//            {
//               // Hor�rios nos extremos do intervalo de hor�rios
//               HorarioAula * h1 = v_temp.getH1();
//               HorarioAula * h2 = v_temp.getH2();
//
//               c.reset();
//               c.setType( ConstraintOp::C_GAPS_PROFESSORES );
//
//               c.setProfessor( professor );
//               c.setDia( dia );
//               c.setH1( h1 );
//               c.setH2( h2 );
//
//               if ( cHashOp.find( c ) == cHashOp.end() )
//               {
//                  sprintf( name, "%s", c.toString().c_str() );
//
//                  OPT_ROW row( nnz, OPT_ROW::LESS, rhs, name );
//
//                  // Vari�vel do gap
//                  row.insert( vit1->second, -1.0 );
//
//                  bool inseriuVariavel = false;
//                  vit2 = variaveisHashX.begin();
//
//                  for (; vit2 != variaveisHashX.end(); vit2++ )
//                  {
//                     VariableOp v_x = vit2->first;
//
//                     if ( v_x.getProfessor() == professor
//                        && v_x.getHorario()->getDia() == dia 
//                        && v_x.getHorario()->getHorarioAula()->getInicio() >= h1->getInicio()
//                        && v_x.getHorario()->getHorarioAula()->getInicio() <= h2->getInicio() )
//                     {
//                        inseriuVariavel = true;
//
//                        // Vari�veis dos hor�rios nos extremos do intervalo
//                        if ( v_x.getHorario()->getHorarioAula()->getInicio() == h1->getInicio()
//                           || v_x.getHorario()->getHorarioAula()->getInicio() == h2->getInicio() )
//                        {
//                           row.insert( vit2->second, 1.0 );
//                        }
//                        // Vari�veis dos hor�rios no meio do intervalo
//                        else
//                        {
//                           row.insert( vit2->second, -1.0 );
//                        }
//                     }
//                  }
//
//                  cHashOp[ c ] = lp->getNumRows();
//
//                  if ( inseriuVariavel )
//                  {
//                     lp->addRow( row );
//                     restricoes++;
//                  }
//               }
//            }
//         }
//      }
//   }
//
//   return restricoes;
//}


void SolverMIP::buscaLocalTempoDeslocamentoSolucao()
{
   if ( this->problemData->parametros->modo_otimizacao != "OPERACIONAL" )
   {
      return;
   }

   // Dado um turno e um dia da semana, temos
   // a lista de atendimentos de cada professor
   std::map< Professor *, std::map< int, GGroup< AtendimentoBase *,
      LessPtr< AtendimentoBase > > >, LessPtr< Professor > > mapProfessorDiaAtendimentos;

   ITERA_GGROUP( it_at_campi,
      ( *this->problemSolution->atendimento_campus ), AtendimentoCampus )
   {
      Campus * campus = this->problemData->refCampus[ it_at_campi->getId() ];

      ITERA_GGROUP( it_at_unidade,
         ( *it_at_campi->atendimentos_unidades ), AtendimentoUnidade )
      {
         Unidade * unidade = this->problemData->refUnidade[ it_at_unidade->getId() ];

         ITERA_GGROUP( it_at_sala,
            ( *it_at_unidade->atendimentos_salas ), AtendimentoSala )
         {
            Sala * sala = this->problemData->refSala[ it_at_sala->getId() ];

            ITERA_GGROUP( it_at_dia,
               ( *it_at_sala->atendimentos_dias_semana ), AtendimentoDiaSemana )
            {
               int dia_semana = it_at_dia->getDiaSemana();

               ITERA_GGROUP( it_at_turno,
                  ( *it_at_dia->atendimentos_turno ), AtendimentoTurno )
               {
                  Turno * turno = this->problemData->findTurno( it_at_turno->getTurnoId() );

                  ITERA_GGROUP( it_at_horario,
                     ( *it_at_turno->atendimentos_horarios_aula ), AtendimentoHorarioAula )
                  {
                     HorarioAula * horario_aula = this->problemData->findHorarioAula(
                        it_at_horario->getHorarioAulaId() );

                     Professor * professor = this->problemData->findProfessor(
                        it_at_horario->getProfessorId() );

                     AtendimentoBase * atendimento = new AtendimentoBase();

                     atendimento->campus = campus;
                     atendimento->unidade = unidade;
                     atendimento->sala = sala;
                     atendimento->dia_semana = dia_semana;
                     atendimento->turno = turno;
                     atendimento->horario_aula = horario_aula;
                     atendimento->professor = professor;
                     atendimento->idAtHorario = it_at_horario->getId();

                     mapProfessorDiaAtendimentos[ professor ][ dia_semana ].add( atendimento );
                  } // Hor�rio da Aula
               } // Turno
            } // Dia da semana
         } // Sala
      } // Unidade
   } // Campus

   std::map< Professor *, std::map< int, GGroup< AtendimentoBase *,
      LessPtr< AtendimentoBase > > >, LessPtr< Professor > >::iterator
      it_map = mapProfessorDiaAtendimentos.begin();

   for (; it_map != mapProfessorDiaAtendimentos.end();
          it_map++ )
   {
      Professor * professor = it_map->first;

      std::map< int, GGroup< AtendimentoBase *,
         LessPtr< AtendimentoBase > > > professorDia = it_map->second;

      std::map< int, GGroup< AtendimentoBase *,
         LessPtr< AtendimentoBase > > >::iterator
         it_prof_dia = professorDia.begin();

      for (; it_prof_dia != professorDia.end();
             it_prof_dia++ )
      {
         int dia_semana = it_prof_dia->first;

         // Para haver possibilidade de alterarmos o deslocamento
         // entre unidades, deve existir pelo menos tr�s atendimentos
         // para o professor no mesmo dia, e em no m�nimo duas unidades distintas

         // Verifica se h� pelo menos 3 atendimentos
         GGroup< AtendimentoBase *,
            LessPtr< AtendimentoBase > > atendimentos = it_prof_dia->second;

         if ( atendimentos.size() <= 2 )
         {
            continue;
         }

         // Verifica se h� pelo menos 2 unidades
         GGroup< Unidade *, LessPtr< Unidade > > unidadesDistintas;

         GGroup< AtendimentoBase *, LessPtr< AtendimentoBase > >::iterator
            it_at = atendimentos.begin();

         for (; it_at != atendimentos.end();
                it_at++ )
         {
            unidadesDistintas.add( it_at->unidade );
         }

         if ( unidadesDistintas.size() <= 1 )
         {
            continue;
         }

         std::vector< AtendimentoBase > vectorAtendimentos;
         ITERA_GGROUP_LESSPTR( it_at, atendimentos, AtendimentoBase )
         {
            vectorAtendimentos.push_back( **it_at );
         }

         // Armazena todas as combina��es poss�veis dos
         // atendimentos nos hor�rios de aula do turno atual
         std::vector< std::vector< HorarioAula > > arranjosHorariosDia;

         std::vector< HorarioAula > horarios;
         for ( int i = 0; i < (int)problemData->horarios_aula_ordenados.size(); i++ )
         {
            horarios.push_back( ( *problemData->horarios_aula_ordenados.at( i ) ) );
         }

         Combinatoria< HorarioAula >::arranjos(
            horarios, (int)atendimentos.size(),  arranjosHorariosDia );

         std::vector< std::vector< HorarioAula > >::iterator
            it_arranjosHorariosDia = arranjosHorariosDia.begin();

         for (; it_arranjosHorariosDia != arranjosHorariosDia.end();
                it_arranjosHorariosDia++ )
         {
            std::vector< HorarioAula > horarios = ( *it_arranjosHorariosDia );
            std::list< int > ids_horarios_antigos;
            ids_horarios_antigos.clear();

            int deslocamentoAnterior = calculaDeslocamentoUnidades(
               professor->getId(), dia_semana );

            // Realiza a troca de hor�rios
            for ( int i = 0; i < (int)horarios.size(); i++ )
            {
               AtendimentoBase atendimento_base = vectorAtendimentos.at( i );
               HorarioAula horario_aula = horarios.at( i );

               int horario_antigo = alteraHorarioAulaAtendimento(
                  horario_aula.getId(), atendimento_base.idAtHorario );

               ids_horarios_antigos.push_back( horario_antigo );
            }

            int deslocamentoPosterior = calculaDeslocamentoUnidades(
               professor->getId(), dia_semana );

            bool solucaoValida = validateSolution->checkSolution(
               this->problemData, this->problemSolution );

            bool melhorouSolucao = ( deslocamentoPosterior < deslocamentoAnterior );

            // Desfaz a troca, caso seja invi�vel ou
            // caso n�o tenha diminuido o deslocamento
            if ( !solucaoValida || !melhorouSolucao )
            {
               for ( int i = 0; i < (int)horarios.size(); i++ )
               {
                  AtendimentoBase atendimento_base = vectorAtendimentos.at( i );
                  int horario_antigo = ids_horarios_antigos.front();
                  ids_horarios_antigos.pop_front();

                  int horario_antigo_alterado = alteraHorarioAulaAtendimento(
                     horario_antigo, atendimento_base.idAtHorario );
               }
            }
         }
      }
   }
}

/*
	Garante um intervalo de tempo m�nimo entre aulas de um mesmo professor
	em um mesmo dia e em unidades distintas.

*/
int SolverMIP::criaRestricaoProfHorarioMultiUnid( void )
{
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
	while(vit != vHashOp.end())
	{
		if(vit->first.getType() == VariableOp::V_X_PROF_AULA_HOR && vit->first.getProfessor() != NULL)
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
									if ( aula->getDiaSemana() != dia ||
										aula->getDisciplina()->horariosDia.find( horControle ) ==
										aula->getDisciplina()->horariosDia.end() )
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
									if ( aula->getDisciplina()->horariosDia.find( hor2 ) ==
										aula->getDisciplina()->horariosDia.end() )
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
										( fimControle < fim2 ) ) )
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

	return restricoes;
}

//int SolverMIP::criaRestricaoProfHorarioMultiUnid( void )
//{
//   int restricoes = 0;
//   int nnz;
//   char name[ 200 ];
//
//   ConstraintOp c;
//   VariableOpHash::iterator vit;
//   ConstraintOpHash::iterator cit;
//
//   GGroup< int > duracoesTodasAulas;
//
//   ITERA_GGROUP_LESSPTR( it_aula, problemData->aulas, Aula )
//   {
//		int tempoCred = (*it_aula)->getDisciplina()->getTempoCredSemanaLetiva();
//		int nCred = (*it_aula)->getTotalCreditos();
//		int aulaDuracao = tempoCred * nCred;
//
//		duracoesTodasAulas.add( aulaDuracao );
//   }
//
//   GGroup< Professor *, LessPtr< Professor > > professores = problemData->getProfessores();
//
//   ITERA_GGROUP_LESSPTR( it_campus, problemData->campi, Campus )
//   {
//	    Campus *cp = *it_campus;
//		
//		ITERA_GGROUP_LESSPTR( it_unid, cp->unidades, Unidade )
//		{
//			Unidade *unid = *it_unid;
//
//			ITERA_GGROUP_LESSPTR( it_horControle, problemData->horariosDia, HorarioDia )
//			{
//				HorarioDia *horControle = ( *it_horControle );
//
//				DateTime inicioControle = horControle->getHorarioAula()->getInicio();
//				
//				int dia = horControle->getDia();
//
//				ITERA_GGROUP_LESSPTR( it_hor2, problemData->horariosDia, HorarioDia )
//				{
//					HorarioDia *hor2 = ( *it_hor2 );
//					
//					if ( hor2->getDia() != dia )
//						continue;
//
//					DateTime inicio2 = hor2->getHorarioAula()->getInicio();
//
//					ITERA_GGROUP_N_PT( it_duracao, duracoesTodasAulas, int )
//					{
//						int duracaoAulaControle = *it_duracao;
//				
//						ITERA_GGROUP_LESSPTR( it_prof, professores, Professor )
//						{
//							Professor * professor = ( *it_prof );
//
//							c.reset();
//							c.setType( ConstraintOp::C_PROF_HORARIO_MULTIUNID );
//							c.setCampus( cp );
//							c.setUnidade( unid );
//							c.setProfessor( professor );
//							c.setDia( dia );
//							c.setHorarioAula( horControle->getHorarioAula() ); // Controle
//							c.setDuracaoAula( duracaoAulaControle ); // Controle
//							c.setH2( hor2->getHorarioAula() );
//
//							cit = cHashOp.find( c );
//					
//							sprintf( name, "%s", c.toString().c_str() );
//							if ( cHashOp.find( c ) != cHashOp.end() )
//							{
//								continue;
//							}
//
//							nnz = 100;
//
//							OPT_ROW row( nnz, OPT_ROW::LESS, 1.0, name );
//
//							bool inseriuUnidControle = false;
//							bool inseriuUnidDiferente = false;
//
//							GGroup< std::pair< Aula *, Disciplina * > >::iterator
//								it_aula_disc = problemData->mapProfessorDisciplinas[ professor ].begin();
//
//							for ( ; it_aula_disc != problemData->mapProfessorDisciplinas[ professor ].end(); it_aula_disc++ )
//							{
//								Aula * aula = ( *it_aula_disc ).first;
//
//								Unidade *unidAula = problemData->refUnidade[ aula->getSala()->getIdUnidade() ];
//								Campus *cpAula = problemData->retornaCampus( unidAula->getId() );
//
//								#pragma region Aulas na mesma unidade da restri��o (unidade controle)
//								if ( cpAula->getId() == cp->getId() &&
//									 unidAula->getId() == unid->getId() )
//								{
//									if ( aula->getDiaSemana() != dia ||
//										 aula->getDisciplina()->horariosDia.find( horControle ) ==
//										 aula->getDisciplina()->horariosDia.end() )
//									{
//										continue;
//									}
//
//									int nCred = aula->getTotalCreditos();
//									int tempoCred = aula->getDisciplina()->getTempoCredSemanaLetiva();							
//									int aulaDuracao = tempoCred * nCred;
//						
//									if ( aulaDuracao != duracaoAulaControle )
//									{
//										continue;
//									}
//								
//									VariableOp v;
//									v.reset();
//									v.setType( VariableOp::V_X_PROF_AULA_HOR );
//									v.setAula( aula ); 
//									v.setProfessor( professor );
//									v.setHorario( horControle );
//									v.setHorarioAula( horControle->getHorarioAula() );
//									v.setDia( dia );
//							 		v.setDisciplina( aula->getDisciplina() );
//									v.setTurma( aula->getTurma() );
//									v.setUnidade( unidAula );
//									v.setSala( aula->getSala() );				   
//							
//									vit = vHashOp.find( v );
//									if ( vit != vHashOp.end() )
//									{
//										row.insert( vit->second, 1.0 );
//										inseriuUnidControle = true;
//									}
//								}
//								#pragma endregion
//
//								#pragma region Aulas em unidades diferentes da unidade controle da restri��o
//								else
//								{
//									if ( aula->getDisciplina()->horariosDia.find( hor2 ) ==
//										 aula->getDisciplina()->horariosDia.end() )
//									{
//										 continue;
//								    }
//
//									int tempoCred = aula->getDisciplina()->getTempoCredSemanaLetiva();
//									int nCred = aula->getTotalCreditos();
//									int duracaoAula = tempoCred * nCred;
//
//									int tempoMinDesloc = problemData->calculaTempoEntreCampusUnidades( cp, cpAula, unid, unidAula );
//
//									DateTime fimControle = inicioControle;
//									fimControle.addMinutes( duracaoAulaControle + tempoMinDesloc );
//									
//									DateTime fim2 = inicio2;
//									fim2.addMinutes( duracaoAula + tempoMinDesloc );
//									
//									if ( ( ( fim2 > inicioControle ) &&
//										   ( inicio2 < fimControle ) )										
//										||
//										 ( ( fimControle > inicio2 ) &&
//										   ( fimControle < fim2 ) ) )
//									{
//										VariableOp v;
//										v.reset();
//										v.setType( VariableOp::V_X_PROF_AULA_HOR );
//										v.setAula( aula ); 
//										v.setProfessor( professor );
//										v.setHorario( hor2 );
//										v.setHorarioAula( hor2->getHorarioAula() );
//										v.setDia( dia );
//							 			v.setDisciplina( aula->getDisciplina() );
//										v.setTurma( aula->getTurma() );
//										v.setUnidade( unidAula );
//										v.setSala( aula->getSala() );				   
//							
//										vit = vHashOp.find( v );
//										if ( vit != vHashOp.end() )
//										{
//											row.insert( vit->second, 1.0 );
//											inseriuUnidDiferente = true;
//										}
//									}			
//								}
//								#pragma endregion
//							}
//
//							if ( inseriuUnidControle && inseriuUnidDiferente )
//							{
//								if ( row.getnnz() != 0 )
//								{
//									cHashOp[ c ] = lp->getNumRows();
//									lp->addRow( row );
//									restricoes++;
//								}
//							}
//						}
//					}
//				}
//			}
//		}
//   }
//
//   return restricoes;
//
//}


int SolverMIP::alteraHorarioAulaAtendimento(
   const int id_novo_horario_aula, const int id_at_horario )
{
   return 1; // ?? TODO

   ITERA_GGROUP( it_at_campi,
      ( *this->problemSolution->atendimento_campus ), AtendimentoCampus )
   {
      Campus * campus = it_at_campi->campus;

      ITERA_GGROUP( it_at_unidade,
         ( *it_at_campi->atendimentos_unidades ), AtendimentoUnidade )
      {
         Unidade * unidade = it_at_unidade->unidade;

         ITERA_GGROUP( it_at_sala,
            ( *it_at_unidade->atendimentos_salas ), AtendimentoSala )
         {
            Sala * sala = it_at_sala->sala;

            ITERA_GGROUP( it_at_dia,
               ( *it_at_sala->atendimentos_dias_semana ), AtendimentoDiaSemana )
            {
               int dia_semana = it_at_dia->getDiaSemana();

               ITERA_GGROUP( it_at_turno,
                  ( *it_at_dia->atendimentos_turno ), AtendimentoTurno )
               {
                  Turno * turno = it_at_turno->turno;

                  ITERA_GGROUP( it_at_horario,
                     ( *it_at_turno->atendimentos_horarios_aula ), AtendimentoHorarioAula )
                  {
                     AtendimentoHorarioAula * at_h = ( *it_at_horario );

                     if ( at_h->getId() == id_novo_horario_aula )
                     {
                        int id_horario_aula = at_h->getHorarioAulaId();

                        HorarioAula * novo_horario_aula
                           = problemData->refHorarioAula.find( id_novo_horario_aula )->second;

                        at_h->horario_aula = novo_horario_aula;
                        at_h->setHorarioAulaId( id_novo_horario_aula );

                        return id_horario_aula;
                     }
                  }
               }
            }
         }
      }
   }

   return -1;
}

bool ordenaAtendimentosBaseHorarioAula(
   AtendimentoBase at1 , AtendimentoBase at2 )
{
   if ( at1.horario_aula == NULL
      || at2.horario_aula == NULL )
   {
      return false;
   }

   if ( at1.horario_aula->getInicio() < at2.horario_aula->getInicio() ) return true;
   if ( at1.horario_aula->getInicio() > at2.horario_aula->getInicio() ) return false;

   return false;
}

int SolverMIP::calculaDeslocamentoUnidades(
   const int id_prof, const int dia )
{
   std::vector< AtendimentoBase > atendimentos;

   ITERA_GGROUP( it_at_campi,
      ( *this->problemSolution->atendimento_campus ), AtendimentoCampus )
   {
      Campus * campus = it_at_campi->campus;

      ITERA_GGROUP( it_at_unidade,
         ( *it_at_campi->atendimentos_unidades ), AtendimentoUnidade )
      {
         Unidade * unidade = it_at_unidade->unidade;

         ITERA_GGROUP( it_at_sala,
            ( *it_at_unidade->atendimentos_salas ), AtendimentoSala )
         {
            Sala * sala = it_at_sala->sala;

            ITERA_GGROUP( it_at_dia,
               ( *it_at_sala->atendimentos_dias_semana ), AtendimentoDiaSemana )
            {
               int dia_semana = it_at_dia->getDiaSemana();

               ITERA_GGROUP( it_at_turno,
                  ( *it_at_dia->atendimentos_turno ), AtendimentoTurno )
               {
                  Turno * turno = it_at_turno->turno;

                  ITERA_GGROUP( it_at_horario,
                     ( *it_at_turno->atendimentos_horarios_aula ), AtendimentoHorarioAula )
                  {
                     AtendimentoHorarioAula * at_h = ( *it_at_horario );

                     HorarioAula * horario_aula = it_at_horario->horario_aula;
                     Professor * professor = it_at_horario->professor;

                     if ( dia_semana == dia
                        && at_h->getProfessorId() == id_prof )
                     {
                        AtendimentoBase atendimento;

                        atendimento.unidade = unidade;
                        atendimento.horario_aula = horario_aula;
                        atendimento.horario_aula = horario_aula;

                        atendimentos.push_back( atendimento );
                     }
                  }
               }
            }
         }
      }
   }

   int contDeslocamentos = 0;

   std::sort( atendimentos.begin(), atendimentos.end(),
      ordenaAtendimentosBaseHorarioAula );

   for ( int i = 0; i < (int)atendimentos.size() - 1; i++ )
   {
      Unidade * unidade1 = atendimentos.at( i ).unidade;
      Unidade * unidade2 = atendimentos.at( i + 1 ).unidade;

      if ( unidade1->getId()  != unidade2->getId() )
      {
         contDeslocamentos++;
      }
   }

   return contDeslocamentos;
}
