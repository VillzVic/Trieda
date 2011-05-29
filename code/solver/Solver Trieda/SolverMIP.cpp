#include "SolverMIP.h"

/*==================================================================/
%DocBegin TRIEDA_LOAD_MODEL
%Title Módulo Tático

%ProbSense MIN

%Set CP 
%Desc 
Conjunto de campus. Os elementos desse conjunto são denotados por $cp$.

%Set U
%Desc 
Conjunto de unidades. Os elementos desse conjunto são denotados por $u$.

%Set S_{u}
%Desc 
Conjunto de salas da unidade $u$. Os elementos desse conjunto são 
denotados por $s$.

%Set SCAP_{u}
%Desc 
Conjunto de salas da unidade $u$ classificadas de acordo com as suas 
capacidades. Os elementos desse conjunto são denotados por $tps$.

%Set T 
%Desc 
Conjunto de dias letivos da semana. Os elementos desse conjunto 
são denotados por $t$.

%Set C
%Desc 
Conjunto de cursos. Os elementos desse conjunto são denotados por $c$.

%Set CC
%Desc 
Conjunto de cursos compatíveis.

%Set D
%Desc 
Conjunto de disciplinas. Os elementos desse conjunto são denotados por $d$.

%Set B
%Desc 
Conjunto de blocos curriculares. Os elementos desse conjunto são 
denotados por $bc$.

%Set D_{bc}
%Desc 
Conjunto de disciplinas que pertencem ao bloco curricular $bc$. 
Os elementos desse conjunto são denotados por $d_{bc}$.

%Set I_{bc}
%Desc 
Conjunto de turmas de um bloco curricular $bc$. 
Os elementos desse conjunto são denotados por $i_{bc}$.

%Set O
%Desc 
Conjunto de ofertas de cursos. Os elementos desse conjunto são 
denotados por $oft$.

%Set D_{oft}
%Desc 
Conjunto de disciplinas de uma oferta $oft$. 
Os elementos desse conjunto são denotados por $d_{oft}$.

%Set O_{d}
%Desc 
Conjunto de ofertas de uma uma disciplina $d$. 
Os elementos desse conjunto são denotados por $oft_{d}$.

%Set K_{d}
%Desc 
Conjunto de combinações possíveis de divisão de créditos de uma uma disciplina $d$. 
Os elementos desse conjunto são denotados por $k$.

%DocEnd
/===================================================================*/

/*==================================================================/
%DocBegin TRIEDA_LOAD_MODEL

%Data A_{u,s}
%Desc
capacidade da sala $s$ da unidade $u$.


%Data C_{d}
%Desc
Total de créditos da disciplina $d$.

%Data \overline{H_{d}}
%Desc 
máximo de créditos diários da disciplina $d$.

%Data \underline{H_{d}}
%Desc 
mínimo de créditos diários da disciplina $d$.

%Data I_{d}
%Desc 
máximo de turmas que podem ser abertas da disciplina $d$.

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
máximo de créditos permitidos por dia $t$.

%Data A_{u,s}
%Desc 
capacidade da sala $s$ da unidade $u$.

%Data HTPS_{t,tps}
%Desc 
máximo de créditos permitidos por dia $t$ para o conjunto de salas do tipo (capacidade) $tps$.

%Data A_{u,tps}
%Desc 
capacidade total das salas de um conjunto de salas do tipo (capacidade) $tps$ da unidade $u$.

%Data O_{cp}
%Desc
conjunto de ofertas de um campus $cp$.

%Data FC_{d,t}
%Desc 
número de créditos fixados para a disciplina $d$ no dia $t$.

%Data N_{d,k,t}
%Desc 
número de créditos determinados para a disciplina $d$ no dia $t$ na combinação de divisão de crédito $k$.

%Data M
%Desc 
big $M$.

%Data \alpha
%Desc 
peso associado a função objetivo.
%Data \beta
%Desc 
peso associado a função objetivo.
%Data \gamma
%Desc 
peso associado a função objetivo.
%Data \delta
%Desc 
peso associado a função objetivo.
%Data \lambda
%Desc 
peso associado a função objetivo.
%Data \rho
%Desc 
peso associado a função objetivo.
%Data \xi
%Desc 
pesos associados a cada item da função objetivo.
%Data \psi
%Desc 
peso associado a função objetivo.
%Data \theta
%Desc 
peso associado a função objetivo.
%Data \omega
%Desc 
peso associado a função objetivo.
%Data \tau
%Desc 
peso associado a função objetivo.
%Data \eta
%Desc 
peso associado a função objetivo.

%DocEnd
/===================================================================*/
bool ordenaDiscSalas( std::pair< int /*Disc id*/, int /*Qtd salas associadas*/ > & left,
                     std::pair< int /*Disc id*/, int /*Qtd salas associadas*/ > & right )
{
   return ( left.second < right.second );
}

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
                     ProblemSolution * _ProblemSolution,
                     ProblemDataLoader * _problemDataLoader )
                     :Solver( aProblemData )
{
   problemSolution = _ProblemSolution;
   problemDataLoader = _problemDataLoader;

   alpha = 5.0;
   beta = 10.0;
   gamma = 0;
   delta = 0;
   lambda = 0.1;
   epsilon = 1.0;
   M = 1.0;
   rho = 5;

   // Verificar o valor
   psi = 1.0;
   tau = 1.0;

   try
   {
      lp = new OPT_GUROBI;
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
      if ( solVars[i] != NULL )
      {
         delete solVars[i];
      }
   }

   solVars.clear();
}

void SolverMIP::carregaVariaveisSolucaoTatico()
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

   //#ifdef DEBUG
   FILE * fout = fopen( "solucao.txt", "wt" );
   //#endif

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
            std::cout << "Variável inválida " << std::endl;
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
            std::cout << "Oferecimento de " << v->getValue()
                      << " vagas da disciplina " << v->getDisciplina()->getCodigo()
                      << " para a turma " << v->getTurma()
                      << " do curso " << v->getOferta()->curso->getCodigo()
                      << std::endl << std::endl;

            vars_a[ std::make_pair( v->getTurma(), v->getDisciplina() ) ].push_back( v );
            break;
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
         sLoader.setFolgas(v);
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
   // Imprimindo as variáveis x_{i,d,u,tps,t} coletadas.

   std::cout << "x\t\ti\td\tu\ttps\tt\n";

   ITERA_VECTOR( it_Vars_x, vars_x, Variable )
   {
      std::cout << ( *it_Vars_x )->getValue() << "\t\t"
         << ( *it_Vars_x )->getTurma() << "\t"
         << ( *it_Vars_x )->getDisciplina()->getCodigo() << "\t"
         << ( *it_Vars_x )->getUnidade()->getCodigo() << "\t"
         << ( *it_Vars_x )->getSubCjtSala()->getId() << "\t"
         << ( *it_Vars_x )->getDia() << "\n";
   }

   std::cout << "\n\n\n";

   // Imprimindo as variáveis a_{i,d,o} coletadas.
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

int SolverMIP::solveTatico()
{
   int varNum = 0;
   int constNum = 0;

   if ( problemData->parametros->funcao_objetivo == 0
      || problemData->parametros->funcao_objetivo == 2 )
   {
      lp->createLP( "Solver Trieda", OPTSENSE_MINIMIZE, PROB_MIP );
   }
   else if( problemData->parametros->funcao_objetivo == 1 )
   {
      lp->createLP( "Solver Trieda", OPTSENSE_MAXIMIZE, PROB_MIP );
   }

#ifdef DEBUG
   printf( "Creating LP...\n" );
#endif

   // Variable creation
   varNum = cria_variaveis();

   lp->updateLP();

#ifdef PRINT_cria_variaveis
   printf( "Total of Variables: %i\n\n", varNum );
#endif

   // Constraint creation
   constNum = cria_restricoes();

   lp->updateLP();

#ifdef PRINT_cria_restricoes
   printf( "Total of Constraints: %i\n\n", constNum );
#endif

#ifdef DEBUG
   lp->writeProbLP( "Solver Trieda" );
#endif

   int status = 0;

#ifndef READ_SOLUTION_TATICO_BIN
   // Muda FO para considerar somente atendimento
   double * objOrig = new double[ lp->getNumCols() ];
   lp->getObj( 0, lp->getNumCols()-1, objOrig );
   double * objNova = new double[ lp->getNumCols() ];
   int * idxNova = new int[ lp->getNumCols() ];

   for (int i=0; i < lp->getNumCols(); i++)
   {
      objNova[i] = 0;
      idxNova[i] = i;
   }

   VariableHash::iterator vit = vHash.begin();
   while ( vit != vHash.end() )
   {
      if ( vit->first.getType() == Variable::V_SLACK_DEMANDA )
      {
         objNova[ vit->second ] = 1.0;
      }
      vit++;
   }

   lp->chgObj( lp->getNumCols(), idxNova, objNova );

   //lp->setHeurFrequency(1.0);
   lp->setTimeLimit(2400);
   //lp->setMIPStartAlg( METHOD_PRIMAL );
   lp->setMIPEmphasis(0);
   lp->setMIPScreenLog(4);
   // lp->setMIPRelTol(0.02);
   // lp->setNoCuts();
   // lp->setNodeLimit(1);
   lp->setPreSolve( OPT_TRUE );

   // Resolve problema olhando somente atendimento
   status = lp->optimize( METHOD_MIP );

   // Passa solucao inicial obtida e fixa atendimento
   double * xSolInic = new double[ lp->getNumCols() ];
   lp->getX(xSolInic);

   double lbAtend = lp->getBestObj();
   double ubAtend = lp->getObjVal();

   OPT_ROW rowLB( 100, OPT_ROW::GREATER , lbAtend , "LBATEND" );
   OPT_ROW rowUB( 100, OPT_ROW::LESS , ubAtend , "UBATEND" );

   vit = vHash.begin();
   while ( vit != vHash.end() )
   {
      if ( vit->first.getType() == Variable::V_SLACK_DEMANDA )
      {
         rowLB.insert(vit->second,1.0);
         rowUB.insert(vit->second,1.0);
      }
      if ( vit->first.getType() == Variable::V_N_SUBBLOCOS )
      {
         xSolInic[ vit->second ] = GRB_UNDEFINED;
      }

      vit++;
   }

   lp->addRow( rowLB );
   lp->addRow( rowUB );
   lp->updateLP();

   lp->setHeurFrequency(1.0);
   lp->setTimeLimit(1200);
   // lp->setMIPStartAlg( METHOD_PRIMAL );
   lp->setMIPEmphasis(1);
   lp->setMIPScreenLog(4);
   // lp->setMIPRelTol(0.02);
   // lp->setNoCuts();
   // lp->setNodeLimit(1);
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

   delete[] xSol;
#endif

   return status;
}

int SolverMIP::solveTaticoBasico()
{
   int varNum = 0;
   int constNum = 0;

   if ( problemData->parametros->funcao_objetivo == 0
      || problemData->parametros->funcao_objetivo == 2 )
   {
      lp->createLP( "Solver Trieda", OPTSENSE_MINIMIZE, PROB_MIP );
   }
   else if( problemData->parametros->funcao_objetivo == 1 )
   {
      lp->createLP( "Solver Trieda", OPTSENSE_MAXIMIZE, PROB_MIP );
   }

#ifdef DEBUG
   printf( "Creating LP...\n" );
#endif

   // Variable creation
   varNum = cria_variaveis();

   lp->updateLP();

#ifdef PRINT_cria_variaveis
   printf( "Total of Variables: %i\n\n", varNum );
#endif

   // Constraint creation
   constNum = cria_restricoes();

   lp->updateLP();

#ifdef PRINT_cria_restricoes
   printf( "Total of Constraints: %i\n\n", constNum );
#endif

#ifdef DEBUG
   lp->writeProbLP( "Solver Trieda" );
#endif

   int status = 0;

   lp->setTimeLimit(15);
   lp->setMIPScreenLog(4);

#ifndef READ_SOLUTION_TATICO_BIN
   status = lp->optimize( METHOD_PRIMAL );
#endif

#ifdef WRITE_SOLUTION_TATICO_BIN
   double * xSol = NULL;
   xSol = new double[ lp->getNumCols() ];
   lp->getX( xSol );
   FILE * fout = fopen( "solBin.bin", "wb" );
   int nCols = lp->getNumCols();

   fwrite( &nCols, sizeof(int), 1, fout );
   for ( int i = 0; i < lp->getNumCols(); i++ )
   {
      fwrite( &( xSol[i] ), sizeof( double ), 1, fout );
   }

   fclose( fout );

   delete [] xSol;
#endif

   return status;
}

void SolverMIP::converteCjtSalaEmSala()
{
   // ---------------------------------------

   // POS PROCESSAMENTO
   // Convertendo as variáveis x_{i,d,u,tps,t} para x_{i,d,u,s,t}.

   // PASSO 1: Criando uma estrutura que irá gerenciar os créditos livres para cada sala.
   std::map< Sala *, std::vector< std::pair< int /*dia*/, int/*creds. Livres*/ > > > creditos_Livres_Sala;

   // Inicializando a estrutura criada acima
   ITERA_GGROUP_LESSPTR( it_Campus, problemData->campi, Campus )
   {
      ITERA_GGROUP_LESSPTR( it_Unidade, it_Campus->unidades, Unidade )
      {
         ITERA_GGROUP_LESSPTR( it_Sala, it_Unidade->salas, Sala )
         {
            ITERA_GGROUP( it_Creds_Disp, it_Sala->creditos_disponiveis, CreditoDisponivel )
            { 
               creditos_Livres_Sala[ ( *it_Sala ) ].push_back(
                  std::make_pair( it_Creds_Disp->getDiaSemana(),
                  it_Creds_Disp->getMaxCreditos() ) );
            }
         }
      }
   }

   // PASSO 2: Ordenando os vetores de Salas para cada disciplina da estrutura <disc_Salas_Pref>.

   // As disciplinas que possuem o menor número de
   // preferências, acima de 0, serão consideradas primeiro.
   // Em seguida, a ordem das demais disciplinas será mantida.

   // Vetor de disciplinas responsável por informar a ordem em que a 
   // heurística irá alocar as disicplinas.
   std::vector< std::pair< int/*Disc id*/, int/*Qtd salas associadas*/ > > disc_Salas_Cont;

   // Adicionando as disicplinas que possuem alguma associação.
   std::map< Disciplina *, GGroup< Sala * > >::iterator 
      it_Disc_Salas_Pref = problemData->disc_Salas_Pref.begin();

   for(; it_Disc_Salas_Pref != problemData->disc_Salas_Pref.end();
      it_Disc_Salas_Pref++ )
   {
      disc_Salas_Cont.push_back(
         std::make_pair( it_Disc_Salas_Pref->first->getId(),
         it_Disc_Salas_Pref->second.size() ) );
   }

   // Ordenando.
   std::sort( disc_Salas_Cont.begin(), disc_Salas_Cont.end(), ordenaDiscSalas );

   // Adicionando as demais disciplinas (as que não possuem nenhuma associação).
   std::vector< std::pair< int /*Disc id*/, int /*Qtd salas associadas*/ > > disc_Salas_TEMP;
   ITERA_GGROUP_LESSPTR( it_Disciplina, problemData->disciplinas, Disciplina )
   {
      // Estrutura: Dado o 'id' de uma disciplina,
      // retorna-se quantas salas estão associadas a essa disciplina
      std::pair< int, int > chave( it_Disciplina->getId(),
         problemData->disc_Salas_Pref[ *it_Disciplina ].size() );

      // Verificando para disciplina não adicionar a mesma disciplina várias vezes.
      // Estrutura dos itens do vector: Dado o 'id' de uma disciplina,
      // retorna-se quantas salas estão associadas a essa disciplina
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

   // Para cada disciplina da estrutura <disc_Salas> realiza-se alguma alocação
   std::vector< std::pair< int /*Disc id*/, int /*Qtd salas associadas*/ > >::iterator
      it_Disc_Salas = disc_Salas_Cont.begin();
   for (; it_Disc_Salas != disc_Salas_Cont.end(); ++it_Disc_Salas )
   {
      // Obtendo uma referencia para a disciplina em questão.
      std::map< int /*Id Disc*/, Disciplina * >::iterator 
         it_Ref_Disciplinas = problemData->refDisciplinas.find( it_Disc_Salas->first );

      if ( it_Ref_Disciplinas == problemData->refDisciplinas.end() )
      {
         std::cout << "\nOpa. Disciplina inexistente."
            << "\n(SolverMIP::converteCjtSalaEmSala()) !!!"
            << "\n\nSaindo." << std::endl;

         exit(1);
      }

      Disciplina * disciplina = it_Ref_Disciplinas->second;

      // Estrutura responsável por armazenar as
      // variaveis "x" para a disciplina em questão.
      std::vector< Variable * > vars_x_Disc;

      // Listando todas as variaveis "x" para a disciplina em questão.
      ITERA_VECTOR( it_Vars_x, vars_x, Variable )
      {
         if ( ( *it_Vars_x )->getDisciplina() == disciplina )
         {
            vars_x_Disc.push_back( *it_Vars_x );
         }
      }

      // Ordenando as variaveis coletadas segundo
      // a ordem dos critérios estabelecidos:
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

         chave.push_back( ( *it_Vars_x_Disc)->getUnidade()->getId() );
         chave.push_back( ( *it_Vars_x_Disc)->getSubCjtSala()->getId() );
         chave.push_back( ( *it_Vars_x_Disc)->getTurma() );

         vars_x_Disc_Und_TPS_Turma[ chave ].push_back( *it_Vars_x_Disc );
      }

      // Estrutura que armazena as salas na ordem em
      // que se deve tentar alocar a disciplina em questão.

      std::vector< Sala * > salas_Ordenadas;

      // Ordenando a estrutura <discSalas> do problemData
      // de acordo com os seguintes critérios:
      // 1 - Salas preferenciais sugeridas pelo usuário.
      // 2 - Outras salas da mesma unidade em que todas,
      // ou a maioria, das salas estavam associadas.
      // 3 - Qualquer outra sala.
      // OBS.: Por eqto tento fazer o passo 1 e depois o 3.
      // O 2 talvez nem precise pq agt ja esta convertendo
      // a lista de cursos predios para associacoes de disciplinas a salas.

      // Adicionando as salas que foram associadas pelo usuario.
      std::map< Disciplina *, GGroup< Sala * > >::iterator
         it_Disc_Salas_Pref = problemData->disc_Salas_Pref.find( disciplina );
      if ( it_Disc_Salas_Pref != problemData->disc_Salas_Pref.end() )
      {
         // Iterando sobre as salas preferenciais para a disciplina em questão.
         ITERA_GGROUP( it_Sala, it_Disc_Salas_Pref->second, Sala )
         {
            salas_Ordenadas.push_back( *it_Sala );
         }
      }

      // Adicionando as demais salas associadas à disciplina em questão.
      std::map< Disciplina *, std::vector< Sala * > >::iterator
         it_Disc_Demais_Salas = problemData->discSalas.find( disciplina );
      if ( it_Disc_Demais_Salas != problemData->discSalas.end() )
      {
         ITERA_VECTOR( it_Sala, it_Disc_Demais_Salas->second, Sala )
         {
            // Para não adicionar repetidas
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

         // Iterando sobre as salas ordenadas para a disciplina em questão.
         ITERA_VECTOR( it_Salas_Ordenadas, salas_Ordenadas, Sala )
         {
            // Checando se a sala em questão pertence ao TPS especificado pelo solver
            if ( it_Vars_x_Disc_Und_TPS_Turma->second.front()->getSubCjtSala()->salas.find(
               ( *it_Salas_Ordenadas )->getId() ) != 
               it_Vars_x_Disc_Und_TPS_Turma->second.front()->getSubCjtSala()->salas.end() )
            {
               std::map< Sala *, std::vector< std::pair< int /*dia*/, int /*creds. Livres*/ > > >::iterator
                  it_Creditos_Livres_Sala = creditos_Livres_Sala.find( *it_Salas_Ordenadas );

               if(it_Creditos_Livres_Sala == creditos_Livres_Sala.end())
               {
                  std::cout << "Opa. Sala nao encontrada na estrutura\n"
                     << "<creditos_Livres_Sala> (SolverMIP::converteCjtSalaEmSala()) !!!"
                     << "\n\nSaindo." <<std::endl;

                  exit(1);
               }

               // Indica se os dias demandados pelas vars x são
               // compatíveis com os dias disponíveis da sala.
               bool dias_Sala_Compativeis = true;

               { // METODO

                  // Iterando em cada variavel X armazenada para o conjunto em questão
                  ITERA_VECTOR(it_Dias_Demandados_Vars_x, it_Vars_x_Disc_Und_TPS_Turma->second, Variable)
                  {
                     // Iterando nos dias disponiveis da sala
                     std::vector< std::pair< int /*dia*/, int /*creds. Livres*/ > >::iterator
                        it_Dia = it_Creditos_Livres_Sala->second.begin();

                     for(; it_Dia != it_Creditos_Livres_Sala->second.end(); ++it_Dia)
                     {
                        // Se encontrei o dia, testo se tem a qtd de creds livres necessaria. Caso
                        // nao possua a qtd de creditos livres necessaria, posso parar de tentar
                        // alocar nessa sala.
                        if(it_Dia->first == (*it_Dias_Demandados_Vars_x)->getDia())
                        {
                           if(it_Dia->second >= (*it_Dias_Demandados_Vars_x)->getValue())
                           {
                              // Nao faço nada aqui. A busca pelos outros dias continua.
                              // Apenas dou um break por eficiência
                              break;
                           }
                           else
                           {
                              dias_Sala_Compativeis = false;

                              // Já que o dia é inviável, não
                              // faz sentido buscar os outros dias.
                              break;
                           }
                        }
                     }

                     if(!dias_Sala_Compativeis)
                     {
                        // Parando o iterador <it_Dias_Demandados_Vars_x>.
                        // Já se sabe que a sala não é compatível para o dia em questão. Portanto
                        // paro a busca pelos demais dias livres que a sala pode ter.
                        break;
                     }
                  }

                  // Teste para saber se posso alocar na sala em questão.
                  if(dias_Sala_Compativeis)
                  {
                     // Iterando em cada variavel X armazenada
                     ITERA_VECTOR(it_Dias_Demandados_Vars_x,
                        it_Vars_x_Disc_Und_TPS_Turma->second, Variable)
                     {
                        if((*it_Dias_Demandados_Vars_x)->getSala() != NULL)
                        {
                           std::cout << "Opa. Fui setar a sala para uma\n"
                              << "var x__i_u_tps_t e ja estava setada.\n"
                              << "(// Já que o dia e inviável, não faz sentido\n"
                              << "buscar os outros dias.) !!!"
                              << "\n\nSaindo." << std::endl;

                           exit(1);
                        }

                        // Setando a sala na(s) variavel(eis)
                        (*it_Dias_Demandados_Vars_x)->setSala( *it_Salas_Ordenadas );

                        // Iterando nos dias disponiveis da sala
                        std::vector< std::pair< int /*dia*/, int /*creds. Livres*/ > >::iterator
                           it_Dia = it_Creditos_Livres_Sala->second.begin();

                        for(; it_Dia != it_Creditos_Livres_Sala->second.end(); ++it_Dia)
                        {
                           // Quando encontro o dia correto, atualizo
                           // a estrutura que armazena  os créditos livres.
                           if(it_Dia->first == (*it_Dias_Demandados_Vars_x)->getDia())
                           {
                              it_Dia->second -= (int) (*it_Dias_Demandados_Vars_x)->getValue();

                              // Apenas por eficiência
                              break;
                           }
                        }
                     }

                     // Setando a FLAG que indica se alocou ou não
                     alocou = true;

                     // Parando a busca de salas.
                     break;
                  }
                  else
                  {
                     // Nao faço nada. Deixo continuar
                     // tentando alocar nas outras salas associadas.
                  }
               }
            }
            else
            {
               // Nao faço nada. Deixo continuar
               // tentando alocar nas outras salas associadas.
            }
         }

         // Checando se a alocação foi realizada.

         // Abaixo, devemos tratar da possível causa da alocação da turma da disciplina em questão
         // não ter sido realizada.

         // 1 - Quando a disciplina foi dividida em mais de 1 dia letivo para realizar um atendimento,
         // pode ser que não se tenha mais nenhuma sala com os dias letivos demandados com créditos livres
         // o suficiente.

         // Solução: Por eqto, atendo em quantas salas diferentes forem necessárias, sem um padrão de
         // escolha das salas.

         // Tinha outras causas, mas como mudei a heurística pra poder rodar pra todas as salas
         // associadas (pelo usuário e pelo solver), acredito que este seja o único erro.

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

               chave.push_back( ( *it_Dias_Demandados_Vars_x)->getUnidade()->getId() );
               chave.push_back( ( *it_Dias_Demandados_Vars_x)->getSubCjtSala()->getId() );
               chave.push_back( ( *it_Dias_Demandados_Vars_x)->getTurma() );
               chave.push_back( ( *it_Dias_Demandados_Vars_x)->getDia() );

               if ( vars_x_Disc_Und_TPS_Turma_DIA.find( chave )
                  != vars_x_Disc_Und_TPS_Turma_DIA.end() )
               {
                  std::cout << "Opa. Fui add um dado na estrutura\n"
                     << "<vars_x_Disc_Und_TPS_Turma_DIA> e iria\n"
                     << "sobrescrever um existente. VOLTAR PARA VECTOR.\n"
                     << "(SolverMIP::converteCjtSalaEmSala()) !!!"
                     << "\n\nSaindo." << std::endl;

                  exit(1);
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

               // Iterando sobre as salas ordenadas para a disciplina em questão.
               ITERA_VECTOR( it_Salas_Ordenadas, salas_Ordenadas, Sala )
               {
                  // Checando se a sala em questão pertence ao TPS especificado pelo solver
                  if ( it_Vars_x_Disc_Und_TPS_Turma_DIA->second->getSubCjtSala()->salas.find(
                     ( *it_Salas_Ordenadas )->getId()) != 
                     it_Vars_x_Disc_Und_TPS_Turma_DIA->second->getSubCjtSala()->salas.end() )
                  {
                     std::map< Sala *, std::vector< std::pair< int /*dia*/, int /*creds. Livres*/ > > >::iterator
                        it_Creditos_Livres_Sala = creditos_Livres_Sala.find( *it_Salas_Ordenadas );

                     if(it_Creditos_Livres_Sala == creditos_Livres_Sala.end())
                     {
                        std::cout << "Opa. Sala nao encontrada na estrutura <creditos_Livres_Sala>\n"
                           << "(SolverMIP::converteCjtSalaEmSala()) !!!\n"
                           << "\n\nSaindo." << std::endl;

                        exit(1);
                     }

                     // Indica se o dia demandado pela var x em
                     // questão é compatível com o dia disponível da sala.

                     // Iterando nos dias disponiveis da sala
                     std::vector< std::pair< int /*dia*/, int /*creds. Livres*/ > >::iterator
                        it_Dia = it_Creditos_Livres_Sala->second.begin();

                     for (; it_Dia != it_Creditos_Livres_Sala->second.end(); it_Dia++ )
                     {
                        // Se encontrei o dia, testo se tem a qtd de creds livres necessaria. Caso
                        // nao possua a qtd de creditos livres necessaria, posso parar de tentar
                        // alocar nessa sala.

                        if ( it_Dia->first == it_Vars_x_Disc_Und_TPS_Turma_DIA->second->getDia() )
                        {
                           if ( it_Dia->second >= ( (int) it_Vars_x_Disc_Und_TPS_Turma_DIA->second->getValue() ) )
                           {
                              // Já posso alocar. Pois trata-se de apenas um dia.

                              // Setando a sala na variavel
                              it_Vars_x_Disc_Und_TPS_Turma_DIA->second->setSala( *it_Salas_Ordenadas );

                              // Atualizo a estrutura que armazena os créditos livres.
                              it_Dia->second -= (int) it_Vars_x_Disc_Und_TPS_Turma_DIA->second->getValue();
                              it_Salas_Ordenadas = salas_Ordenadas.begin();

                              alocou = true;

                              // Apenas dou um break por eficiência
                              break;
                           }
                           else
                           {
                              // Continuo a busca em outras salas.
                              continuaBusca = true;

                              // Dia é inviável.
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
                     // Nao faço nada. Deixo continuar
                     // tentando alocar nas outras salas associadas.
                  }
               }

               // TRIEDA-715
               // Ainda assim, a variável x em questão pode não ter sido convertida.
               // Pode acontecer o caso em que a variável x tem valor maior que 1 e todas as salas
               // do TPS especificado pela variável em questão possuem apenas 1 cred livre. Desse modo,
               // deve-se dividir os créditos da disciplina, alocando-os separadamente por sala.

               if ( continuaBusca && !alocou )
               {
                  // Referenciando a variável em questão.
                  Variable * pt_Var_x = ( it_Vars_x_Disc_Und_TPS_Turma_DIA->second );

                  // A ideia aqui é pegar a variável em questão e sair tentando alocar os créditos
                  // separadamente. Eles serão separados de acordo com a disponibilidade de cada
                  // sala.
                  // Quando a variável for totalmente alocada, aponta-se para NULL e finaliza a alocação da mesma.
                  // IMPORTANTE: Como estou dividindo os créditos, tenho que criar novas variáveis. Lembrar de adiciona-las
                  // à estrutura <vars_x> que armazena todas as variáveis do tipo x.

                  while ( pt_Var_x )
                  {
                     // Iterando sobre as salas ordenadas para a disciplina em questão.
                     ITERA_VECTOR( it_Salas_Ordenadas, salas_Ordenadas, Sala )
                     {
                        // Checando se a sala em questão pertence ao TPS especificado pelo solver
                        if ( pt_Var_x->getSubCjtSala()->salas.find( ( *it_Salas_Ordenadas )->getId() ) != 
                           pt_Var_x->getSubCjtSala()->salas.end() )
                        {
                           std::map< Sala *, std::vector< std::pair< int /*dia*/, int /*creds. Livres*/ > > >::iterator
                              it_Creditos_Livres_Sala = creditos_Livres_Sala.find( *it_Salas_Ordenadas );

                           if ( it_Creditos_Livres_Sala == creditos_Livres_Sala.end() )
                           {
                              std::cout << "Opa. Sala nao encontrada na estrutura\n"
                                 << "<creditos_Livres_Sala>\n"
                                 << "(SolverMIP::converteCjtSalaEmSala()) !!!"
                                 << "\n\nSaindo." << std::endl;

                              exit(1);
                           }

                           // Iterando nos dias disponiveis da sala
                           std::vector< std::pair< int /*dia*/, int /*creds. Livres*/ > >::iterator
                              it_Dia = ( it_Creditos_Livres_Sala->second.begin() );

                           for (; it_Dia != it_Creditos_Livres_Sala->second.end();
                              it_Dia++ )
                           {
                              // Se encontrei o dia, testo se tem a qtd de creds
                              // livres necessaria. Caso nao possua a qtd de creditos
                              // livres necessaria, posso parar de tentar alocar nessa sala.
                              if ( it_Dia->first == pt_Var_x->getDia() )
                              {
                                 if ( it_Dia->second > 0 )
                                 {
                                    // Já posso alocar. Pois trata-se de apenas um dia.

                                    // Cálculo do total de créditos que serão alocados.
                                    int creds_Alocar = (int)( pt_Var_x->getValue() - it_Dia->second );

                                    // Criando uma nova variável para uma divisão da variável em questão.
                                    Variable * var_x_NAO_ALOCADA = NULL;

                                    // Se o total de créditos a serem alocados for maior do que a 
                                    // o total de crédtios livres da sala (creds_Alocar > 0), devo
                                    // criar uma cópia da variável 'x' em questão e atualizar o seu
                                    // valor com a diferença entre o total de créditos da var subtraido
                                    // do total de cred. livre da sala. Já no caso em que o total de
                                    // créditos alocados é menor ou igual ao total de créditos livres
                                    // da sala em questão (creds_Alocar <= 0), devo apenas alocar.

                                    if ( creds_Alocar > 0 )
                                    {
                                       // Criando uma nova variável para a divisão da variável em questão.
                                       var_x_NAO_ALOCADA = new Variable( *pt_Var_x );

                                       // Atualizando a quantidade de créditos não alocada.
                                       var_x_NAO_ALOCADA->setValue( pt_Var_x->getValue() - creds_Alocar );

                                       // Adicionando a nova variável à estrutura <vars_x>
                                       vars_x.push_back( var_x_NAO_ALOCADA );

                                       // Atualizando o valor da variável a ser atendida.
                                       pt_Var_x->setValue( creds_Alocar );
                                    }

                                    // Setando a sala na variavel
                                    pt_Var_x->setSala( *it_Salas_Ordenadas );

                                    // Atualizo a estrutura que armazena os créditos livres.
                                    it_Dia->second -= (int) pt_Var_x->getValue();

                                    // Checando se ainda existe algum crédito para alocar.
                                    if ( var_x_NAO_ALOCADA )
                                    {
                                       pt_Var_x = var_x_NAO_ALOCADA;
                                    }
                                    else
                                    {
                                       // Condição para sair do loop.
                                       pt_Var_x = NULL;
                                    }

                                    alocou = true;

                                    // Apenas dou um break por eficiência
                                    break;
                                 }
                                 else
                                 {
                                    // Dia é inviável.
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
                           // Nao faço nada. Deixo continuar tentando alocar nas outras salas associadas.
                        }
                     }
                  }
               }
            }
         }
      }
   }

   // ---------------------------------------
   // Imprimindo as variáveis x_{i,d,u,s,t} convertidas.

   std::cout << "\n\n\n";
   std::cout << "x\t\ti\td\tu\ts\tt\n";

   ITERA_VECTOR( it_Vars_x, vars_x, Variable )
   {
      if ( (*it_Vars_x)->getSala() == NULL )
      {
         printf( "OPA. Variavel x (x_i(%d)_d(%d)_u(%d)_tps(%d)_t(%d))nao convertida.\n",
            (*it_Vars_x)->getTurma(),
            (*it_Vars_x)->getDisciplina()->getId(),
            (*it_Vars_x)->getUnidade()->getId(),
            (*it_Vars_x)->getSubCjtSala()->getId(),
            (*it_Vars_x)->getDia());

         exit(1);
      }

      std::cout << (*it_Vars_x)->getValue() << "\t\t"
         << (*it_Vars_x)->getTurma() << "\t"
         << (*it_Vars_x)->getDisciplina()->getCodigo() << "\t"
         << (*it_Vars_x)->getUnidade()->getCodigo() << "\t"
         << (*it_Vars_x)->getSala()->getCodigo() << "\t"
         << (*it_Vars_x)->getDia() << "\n";
   }
}

void SolverMIP::getSolutionTatico()
{
   // POVOANDO AS CLASSES DE SAIDA

   int at_Tatico_Counter = 0;

   // Iterando sobre as variáveis do tipo x.
   ITERA_VECTOR( it_Vars_x, vars_x, Variable )
   {
      // Descobrindo qual Campus a variável x em questão pertence.
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
               exit(1);
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
                        exit(1);
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
                                 exit(1);
                              }
                              else
                              {
                                 int dia = ( *it_Vars_x )->getDia();

                                 bool novo_Dia = true;
                                 ITERA_GGROUP( it_At_Dia, ( *it_At_Sala->atendimentos_dias_semana ),
                                               AtendimentoDiaSemana )
                                 {
                                    if ( it_At_Dia->getDiaSemana() == dia )
                                    {
                                       if ( it_At_Dia->atendimentos_tatico->size() == 0 )
                                       {
                                          std::cout << "Achei que nao era pra cair aqui <dbg4>" << std::endl;

                                          // NOVO ATENDIMENTO
                                          exit(1);                                             
                                       }
                                       else
                                       {
                                          // CADASTRO DE ATENDIMENTO TATICO

                                          // Para cada variavel a__i_d_o existem para a variavel x__i_d_u_s_t em questão.
                                          ITERA_VECTOR( it_Vars_a,
                                                        vars_a.find( std::make_pair( ( *it_Vars_x )->getTurma(),
                                                        ( *it_Vars_x )->getDisciplina() ) )->second, Variable )
                                          {
                                             AtendimentoTatico * at_Tatico = new AtendimentoTatico();

                                             // Verificando se a disicplina é de carater prático ou teórico.
                                             if ( ( *it_Vars_x )->getDisciplina()->getId() > 0
                                                && ( *it_Vars_x )->getDisciplina()->getCredTeoricos() > 0 )
                                             {
                                                at_Tatico->setQtdCreditosTeoricos( (int)( ( *it_Vars_x )->getValue() ) );
                                             }
                                             else
                                             {
                                                at_Tatico->setQtdCreditosPraticos( (int)( ( *it_Vars_x )->getValue() ) );
                                             }

                                             AtendimentoOferta * at_Oferta = new AtendimentoOferta();

                                             stringstream str;
                                             str << ( *it_Vars_a )->getOferta()->getId();
                                             at_Oferta->setOfertaCursoCampiId( str.str() );

                                             at_Oferta->setDisciplinaId(
                                                ( ( *it_Vars_a )->getDisciplina()->getId() > 0 ? 
                                                ( *it_Vars_a )->getDisciplina()->getId() :
                                                ( -( *it_Vars_a )->getDisciplina()->getId() ) ) );

                                             at_Oferta->setQuantidade( (int)( ( *it_Vars_a )->getValue() ) );
                                             at_Oferta->setTurma( ( *it_Vars_a )->getTurma() );
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
                                    // Cadastrando o dia da semana
                                    AtendimentoDiaSemana * at_Dia_Semana = new AtendimentoDiaSemana();

                                    at_Dia_Semana->setDiaSemana( ( *it_Vars_x )->getDia() );

                                    // Para cada variavel a__i_d_o existem para a variavel x__i_d_u_s_t em questão.
                                    ITERA_VECTOR( it_Vars_a,
                                                  vars_a.find( std::make_pair( ( *it_Vars_x )->getTurma(),
                                                  ( *it_Vars_x )->getDisciplina() ) )->second, Variable )
                                    {
                                       AtendimentoTatico * at_Tatico = new AtendimentoTatico();

                                       // Verificando se a disicplina é de carater prático ou teórico.
                                       if (  ( *it_Vars_x )->getDisciplina()->getId() > 0
                                          && ( *it_Vars_x )->getDisciplina()->getCredTeoricos() > 0 )
                                       {
                                          at_Tatico->setQtdCreditosTeoricos( (int)( ( *it_Vars_x )->getValue() ) );
                                       }
                                       else
                                       {
                                          at_Tatico->setQtdCreditosPraticos( (int)( ( *it_Vars_x )->getValue() ) );
                                       }

                                       AtendimentoOferta * at_Oferta = new AtendimentoOferta();

                                       stringstream str;
                                       str << ( *it_Vars_a )->getOferta()->getId();
                                       at_Oferta->setOfertaCursoCampiId( str.str() );

                                       at_Oferta->setDisciplinaId( 
                                          ( ( *it_Vars_a )->getDisciplina()->getId() > 0 ? 
                                          ( *it_Vars_a )->getDisciplina()->getId() :
                                          ( -( *it_Vars_a )->getDisciplina()->getId() ) ) );

                                       at_Oferta->setQuantidade( (int)( ( *it_Vars_a )->getValue() ) );
                                       at_Oferta->setTurma( ( *it_Vars_a )->getTurma() );
                                       at_Oferta->oferta = ( *it_Vars_a )->getOferta();
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
                           // Cadastrando a Sala
                           AtendimentoSala * at_Sala = new AtendimentoSala();

                           at_Sala->setId( ( *it_Vars_x )->getSala()->getId() );
                           at_Sala->setSalaId( ( *it_Vars_x )->getSala()->getCodigo() );
                           at_Sala->sala = ( *it_Vars_x )->getSala();

                           // Cadastrando o dia da semana
                           AtendimentoDiaSemana * at_Dia_Semana = new AtendimentoDiaSemana();
                           at_Dia_Semana->setDiaSemana( ( *it_Vars_x )->getDia() );

                           // Para cada variavel a__i_d_o existem para a variavel x__i_d_u_s_t em questão.
                           ITERA_VECTOR( it_Vars_a,
                                         vars_a.find( std::make_pair( ( *it_Vars_x )->getTurma(),
                                         ( *it_Vars_x )->getDisciplina() ) )->second, Variable )
                           {
                              AtendimentoTatico * at_Tatico = new AtendimentoTatico();

                              // Verificando se a disicplina é de carater prático ou teórico.
                              if (  ( *it_Vars_x )->getDisciplina()->getId() > 0
                                 && ( *it_Vars_x )->getDisciplina()->getCredTeoricos() > 0 )
                              {
                                 at_Tatico->setQtdCreditosTeoricos( (int)( ( *it_Vars_x )->getValue() ) );
                              }
                              else
                              {
                                 at_Tatico->setQtdCreditosPraticos( (int)( ( *it_Vars_x )->getValue() ) );
                              }

                              AtendimentoOferta * at_Oferta = new AtendimentoOferta();

                              stringstream str;
                              str << ( *it_Vars_a )->getOferta()->getId();
                              at_Oferta->setOfertaCursoCampiId( str.str() );

                              at_Oferta->setDisciplinaId( 
                                 ( ( *it_Vars_a )->getDisciplina()->getId() > 0 ? 
                                 ( *it_Vars_a )->getDisciplina()->getId() :
                                 ( -( *it_Vars_a )->getDisciplina()->getId() ) ) );

                              at_Oferta->setQuantidade( (int)( ( *it_Vars_a )->getValue() ) );
                              at_Oferta->setTurma( (*it_Vars_a)->getTurma() );
                              at_Oferta->oferta = (*it_Vars_a)->getOferta();
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
                  // Cadastrando a Unidade
                  AtendimentoUnidade * at_Unidade = new AtendimentoUnidade();

                  at_Unidade->setId( ( *it_Vars_x )->getUnidade()->getId() );
                  at_Unidade->setCodigoUnidade( ( *it_Vars_x )->getUnidade()->getCodigo() );
                  at_Unidade->unidade = ( *it_Vars_x )->getUnidade();

                  // Cadastrando a Sala
                  AtendimentoSala * at_Sala = new AtendimentoSala();

                  at_Sala->setId( ( *it_Vars_x )->getSala()->getId() );
                  at_Sala->setSalaId( ( *it_Vars_x )->getSala()->getCodigo() );
                  at_Sala->sala = ( *it_Vars_x )->getSala();

                  // Cadastrando o dia da semana
                  AtendimentoDiaSemana * at_Dia_Semana = new AtendimentoDiaSemana();

                  at_Dia_Semana->setDiaSemana( ( *it_Vars_x )->getDia() );

                  // Para cada variavel a__i_d_o existem para a variavel x__i_d_u_s_t em questão.
                  ITERA_VECTOR( it_Vars_a,
                                vars_a.find( std::make_pair( (*it_Vars_x)->getTurma(),
                                ( *it_Vars_x )->getDisciplina() ) )->second, Variable )
                  {
                     AtendimentoTatico * at_Tatico = new AtendimentoTatico();

                     // Verificando se a disicplina é de carater prático ou teórico.
                     if (  ( *it_Vars_x )->getDisciplina()->getId() > 0
                        && ( *it_Vars_x )->getDisciplina()->getCredTeoricos() > 0 )
                     {
                        at_Tatico->setQtdCreditosTeoricos( (int)( ( *it_Vars_x )->getValue() ) );
                     }
                     else
                     {
                        at_Tatico->setQtdCreditosPraticos( (int)( ( *it_Vars_x )->getValue() ) );
                     }

                     AtendimentoOferta * at_Oferta = new AtendimentoOferta();

                     stringstream str;
                     str << ( *it_Vars_a )->getOferta()->getId();
                     at_Oferta->setOfertaCursoCampiId( str.str() );

                     at_Oferta->setDisciplinaId( 
                        ( ( *it_Vars_a )->getDisciplina()->getId() > 0 ? 
                        ( *it_Vars_a )->getDisciplina()->getId() :
                        ( -( *it_Vars_a )->getDisciplina()->getId() ) ) );

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
         AtendimentoCampus * at_Campus = new AtendimentoCampus();

         at_Campus->setId( campus->getId() );
         at_Campus->campus_id = campus->getCodigo();
         at_Campus->campus = campus;

         // Cadastrando a Unidade
         AtendimentoUnidade * at_Unidade = new AtendimentoUnidade();
         at_Unidade->setId( ( *it_Vars_x )->getUnidade()->getId() );
         at_Unidade->setCodigoUnidade( ( *it_Vars_x )->getUnidade()->getCodigo() );
         at_Unidade->unidade = ( *it_Vars_x )->getUnidade();

         // Cadastrando a Sala
         AtendimentoSala * at_Sala = new AtendimentoSala();

         at_Sala->setId( ( *it_Vars_x )->getSala()->getId() );
         at_Sala->setSalaId( ( *it_Vars_x )->getSala()->getCodigo() );
         at_Sala->sala = ( *it_Vars_x )->getSala();

         // Cadastrando o dia da semana
         AtendimentoDiaSemana * at_Dia_Semana = new AtendimentoDiaSemana();

         at_Dia_Semana->setDiaSemana( ( *it_Vars_x )->getDia() );

         // Para cada variavel a__i_d_o existem para a variavel x__i_d_u_s_t em questão.
         ITERA_VECTOR( it_Vars_a,
                       vars_a.find( std::make_pair( ( *it_Vars_x )->getTurma(),
                       ( *it_Vars_x )->getDisciplina() ) )->second, Variable )
         {
            AtendimentoTatico * at_Tatico = new AtendimentoTatico();

            // Verificando se a disicplina é de carater prático ou teórico.
            if ( ( *it_Vars_x )->getDisciplina()->getId() > 0
               && ( *it_Vars_x )->getDisciplina()->getCredTeoricos() > 0 )
            {
               at_Tatico->setQtdCreditosTeoricos( (int)( ( *it_Vars_x )->getValue() ) );
            }
            else
            {
               at_Tatico->setQtdCreditosPraticos( (int)( ( *it_Vars_x )->getValue() ) );
            }

            AtendimentoOferta * at_Oferta = new AtendimentoOferta();

            stringstream str;
            str << ( *it_Vars_a )->getOferta()->getId();
            at_Oferta->setOfertaCursoCampiId( str.str() );

            at_Oferta->setDisciplinaId( 
               ( ( *it_Vars_a )->getDisciplina()->getId() > 0 ? 
               ( *it_Vars_a )->getDisciplina()->getId() :
               ( -( *it_Vars_a )->getDisciplina()->getId() ) ) );

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
   // Criando uma solução inicial
   SolucaoInicialOperacional solIni( *problemData );

   std::cout << "Gerando uma solucao inicial para o modelo operacional" << std::endl;
   SolucaoOperacional & solucaoOperacional = solIni.geraSolucaoInicial();
   std::cout << "Solucao Inidial gerada." << std::endl;

   solucaoOperacional.toString2();

   // Avaliador
   Avaliador avaliador;
   avaliador.avaliaSolucao( solucaoOperacional, true );

   // Estruturas de Vizinhança
   // NSSeqSwapEqBlocks nsSeqSwapEqBlocks ( *problemData );
   // NSSwapEqSchedulesBlocks nsSwapEqSchedulesBlocks ( *problemData );
   // NSSwapEqTeachersBlocks nsSwapEqTeachersBlocks ( *problemData );

//   NSShift nsShift( *problemData );
//   nsShift.move(solucaoOperacional);
   
   // Heurísticas de Busca Local - Descida Randômica
   // RandomDescentMethod rdmSeqSwapEqBlocks ( avaliador, nsSeqSwapEqBlocks, 10 );
   // RandomDescentMethod rdmSwapEqSchedulesBlocks ( avaliador, nsSwapEqSchedulesBlocks, 10 );
   // RandomDescentMethod rdmSwapEqTeachersBlocks ( avaliador, nsSwapEqTeachersBlocks, 10 );
   // RandomDescentMethod rdmShift ( avaliador, nsShift, 10 );

   // Mecanismo de perturbação
   // ILSLPerturbationLPlus2 ilslPerturbationPlus2 ( avaliador, -1, nsSeqSwapEqBlocks );
   // ilslPerturbationPlus2.add_ns( nsSwapEqSchedulesBlocks );
   // ilslPerturbationPlus2.add_ns( nsSwapEqTeachersBlocks );
   // ilslPerturbationPlus2.add_ns( nsShift );

   // RVND
   // std::vector< Heuristic * > heuristicasBuscaLocal;
   // heuristicasBuscaLocal.push_back( &rdmSeqSwapEqBlocks );
   // heuristicasBuscaLocal.push_back( &rdmSwapEqSchedulesBlocks );
   // heuristicasBuscaLocal.push_back( &rdmSwapEqTeachersBlocks );
   // heuristicasBuscaLocal.push_back( &rdmShift );

   // RVND rvnd( avaliador, heuristicasBuscaLocal );

   // Busca Local Iterada por Níveis
   // IteratedLocalSearchLevels ilsl ( avaliador, rvnd, ilslPerturbationPlus2, 20 , 4 );
   // ilsl.exec( solucaoOperacional, 30, 0 ); // Parâmetro 2 : tempo ( em segundos )

   // Parâmetro 2 : tempo ( em segundos )
   // rdmSeqSwapEqBlocks.exec( solucaoOperacional, 30, 0 );

   // Avaliação final
   // avaliador.avaliaSolucao( solucaoOperacional, true );

   // Armazena a solução operacional no problem solution
   problemSolution->solucao_operacional = &( solucaoOperacional );

   return 1;
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

      // Procura pelas disciplinas que o professor deverá ministrar
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

int SolverMIP::solve()
{
   int status = 0;

   if ( problemData->parametros->modo_otimizacao == "TATICO"
      && problemData->atendimentosTatico == NULL )
   {
      // status = solveTatico();
      status = solveTaticoBasico();

      carregaVariaveisSolucaoTatico();
      converteCjtSalaEmSala();
	  separaDisciplinasEquivalentes();
   }
   else if ( problemData->parametros->modo_otimizacao == "OPERACIONAL" )
   {
      problemSolution->atendimento_campus;

      if ( problemData->atendimentosTatico != NULL
            && problemData->atendimentosTatico->size() > 0 )
      {
         ITERA_GGROUP(itAtTat,*problemData->atendimentosTatico,AtendimentoCampusSolucao)
         { 
            Campus * campus = problemData->refCampus[itAtTat->getCampusId()];

            AtendimentoCampus * atCampus = new AtendimentoCampus();

            atCampus->setId( campus->getId() );
            atCampus->campus_id = campus->getCodigo();
            atCampus->campus = campus;

            ITERA_GGROUP( itAtUnd, itAtTat->atendimentosUnidades, AtendimentoUnidadeSolucao )
            {
               Unidade * unidade = problemData->refUnidade[itAtUnd->getUnidadeId()];

               AtendimentoUnidade * atUnidade = new AtendimentoUnidade();

               atUnidade->setId( unidade->getId() );
               atUnidade->setCodigoUnidade( unidade->getCodigo() );
               atUnidade->unidade = unidade;

               ITERA_GGROUP( itAtSala, itAtUnd->atendimentosSalas, AtendimentoSalaSolucao )
               {
                  Sala * sala = problemData->refSala[itAtSala->getSalaId()];

                  AtendimentoSala * atSala = new AtendimentoSala();

                  atSala->setId( sala->getId() );
                  atSala->setSalaId( sala->getCodigo() );
                  atSala->sala = sala;

                  ITERA_GGROUP(itAtDiaSemana,itAtSala->atendimentosDiasSemana,AtendimentoDiaSemanaSolucao)
                  {
                     AtendimentoDiaSemana * atDiaSemana = new AtendimentoDiaSemana();

                     atDiaSemana->setDiaSemana( itAtDiaSemana->getDiaSemana() );

                     atSala->atendimentos_dias_semana->add( atDiaSemana );
                  }

                  atUnidade->atendimentos_salas->add( atSala );
               }

               atCampus->atendimentos_unidades->add( atUnidade );
            }
            problemSolution->atendimento_campus->add(atCampus);
         }

         // Resolvendo o modelo operacional
         status = solveOperacional();

         // Preenche as classes do output operacional
         preencheOutputOperacional( problemSolution );
      }
      else
      {
         // Neste caso, primeiro deve-se gerar uma saída para
         // o modelo tático. Em seguida, deve-se resolver o
         // modelo operacional com base na saída do modelo tático gerada.

         // Gerando uma saída para o modelo tático.

         // status = solveTatico();
         status = solveTaticoBasico();

         carregaVariaveisSolucaoTatico();
         converteCjtSalaEmSala();
         separaDisciplinasEquivalentes();

         // Preenchendo a estrutura "atendimento_campus" com a saída.
         getSolutionTatico();

         // Preenchendo a estrutura "atendimentosTatico".
         problemData->atendimentosTatico = new GGroup< AtendimentoCampusSolucao * >();
         ITERA_GGROUP( it_At_Campus, ( *problemSolution->atendimento_campus ), AtendimentoCampus )
         {
            problemData->atendimentosTatico->add( new AtendimentoCampusSolucao( **it_At_Campus ) );
         }

         // Remove a referência para os atendimentos tático (que pertencem ao output tático)
         ITERA_GGROUP( it_At_Campus, ( *problemSolution->atendimento_campus ), AtendimentoCampus )
         {
            ITERA_GGROUP( it_At_Unidade, ( *it_At_Campus->atendimentos_unidades ), AtendimentoUnidade )
            {
               ITERA_GGROUP( it_At_Sala, ( *it_At_Unidade->atendimentos_salas ), AtendimentoSala )
               {
                  ITERA_GGROUP( it_At_DiaSemana, ( *it_At_Sala->atendimentos_dias_semana ), AtendimentoDiaSemana )
                  {
                     GGroup< AtendimentoTatico * > * atendimentos_tatico = it_At_DiaSemana->atendimentos_tatico;

                     atendimentos_tatico->clear();
                  }
               }
            }
         }

         // Criando as aulas que serão utilizadas para resolver o modelo operacional
         problemDataLoader->criaAulas();

         // Resolvendo o modelo operacional
         solveOperacional();

         // Preenche as classes do output ooperacional
         preencheOutputOperacional( problemSolution );
      }
   }

   return status;
}

void SolverMIP::preencheOutputOperacional( ProblemSolution * solution )
{
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

               vector< vector< Aula * > * >::iterator
                  it_matriz_aulas = matriz_aulas->begin();
               for (; it_matriz_aulas != matriz_aulas->end(); it_matriz_aulas++ )
               {
                  int linha_professor = std::distance( matriz_aulas->begin(), it_matriz_aulas );

                  vector< Aula * > * aulas = ( *it_matriz_aulas );
                  vector< Aula * >::iterator it_aula = aulas->begin();
                  for (; it_aula != aulas->end(); it_aula++ )
                  {
                     int horario_aula_id = ( std::distance( aulas->begin(), it_aula ) % total_horarios );

                     Aula * aula = ( *it_aula );
                     if ( aula == NULL || aula->eVirtual() == true )
                     {
                        continue;
                     }

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

                        it_At_DiaSemana->atendimentos_turno->add( atendimento_turno );
                     }
                     //-------------------------------------------------------------------------------------

                     //-------------------------------------------------------------------------------------
                     if ( aulaAlocada( aula, campus, unidade, sala, dia_semana ) )
                     {
                        AtendimentoHorarioAula * atendimento_horario_aula = new AtendimentoHorarioAula();

                        HorarioAula * horario_aula = problemData->horarios_aula_ordenados[ horario_aula_id ];
                        Professor * professor = solution->solucao_operacional->getProfessorMatriz( linha_professor );

                        atendimento_horario_aula->setId( horario_aula->getId() );
                        atendimento_horario_aula->setHorarioAulaId( horario_aula->getId() );
                        atendimento_horario_aula->setProfessorId( professor->getId() );
                        atendimento_horario_aula->setProfVirtual( professor->eVirtual() );
                        atendimento_horario_aula->setCreditoTeorico( aula->getCreditosTeoricos() > 0 );

                        ITERA_GGROUP_LESSPTR( it_oferta, aula->ofertas, Oferta )
                        {
                           Oferta * oferta = ( *it_oferta );
                           AtendimentoOferta * atendimento_oferta = new AtendimentoOferta();

                           atendimento_oferta->setId( oferta->getId() );
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
                     }
                     //-------------------------------------------------------------------------------------
                  }
               }
            }
         }
      }
   }
}

bool SolverMIP::aulaAlocada( Aula * aula, Campus * campus,
   Unidade * unidade, Sala * sala, int dia_semana ) 
{
   if ( aula == NULL || aula->eVirtual() )
   {
      return false;
   }

   bool aula_alocada = true;

   GGroup< Oferta *, LessPtr< Oferta > >::iterator it_oferta
      = aula->ofertas.begin();
   int id_campus_aula = it_oferta->campus->getId();
   int id_unidade_aula = aula->getSala()->getIdUnidade();
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
   // Primeiramente, devo criar as variáveis de alocação de alunos ( variáveis 'a' )
   // para as disciplinas que foram substituídas. A alocação é feita utilizando o total
   // de alunos que foram alocados para a disciplina que as substituiu
   criaVariaveisAlunosDisciplinasSubstituidas();

   // Após criadas as variáveis de alocação de alunos, então devo criar
   // as variáveis de créditos ( variáveis 'x' )para as disciplinas substituídas
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

   // Procura criar variáveis 'a' para as disciplinas que foram substituídas
   for (; it_disc_substituidas != problemData->mapGroupDisciplinasSubstituidas.end();
          it_disc_substituidas++ )
   {
      curso = it_disc_substituidas->first.first;
      curriculo = it_disc_substituidas->first.second;

		// Cria variáveis para cada uma das disciplinas substituídas
      std::map< Disciplina *, GGroup< Disciplina *, LessPtr< Disciplina > > >::iterator
         it_conjunto_disc = it_disc_substituidas->second.begin();

      for (; it_conjunto_disc != it_disc_substituidas->second.end();
             it_conjunto_disc++ )
      {
         // Disciplina para a qual as variáveis foram criadas
         disciplina_substituta = ( it_conjunto_disc->first );

         // Variáveis criadas para 'disciplina_substituta' em cursos compatíveis
         // com os cursos das disciplinas que foram substituídas por ela
         std::vector< Variable * > variaveis_alunos
            = variaveisAlunosAtendidos( curso, disciplina_substituta );

         // Total de alunos que foram atendidos
         int alunos_atendidos = 0;

         // Armazena a variável que diz respeito EXATAMENTE à 'disciplina_substituta'
         Variable * v_disc_substituta = NULL;

         for ( int i = 0; i < (int)variaveis_alunos.size(); i++ )
         {
            Variable * v = variaveis_alunos[i];
            alunos_atendidos += (int)( v->getValue() );

            if ( v_disc_substituta == NULL
					&& v->getOferta()->curso->getId() == curso->getId()
               && v->getOferta()->curriculo->getId() == curriculo->getId() )
            {
               v_disc_substituta = v;
            }
         }

         // Primeiramente, devo atender todos a demanda da disciplina
         // 'disciplina_substituta', e em seguida alocar a demanda de
         // alunos das suas disciplinas equivalentes, enquanto for possível
         Demanda * demanda_substituta
            = problemData->buscaDemanda( curso, disciplina_substituta );

         // Demanda da disciplina que substituiu as demais
         int alunos_disciplina_substituta = demanda_substituta->getQuantidade();

         if ( alunos_disciplina_substituta <= alunos_atendidos )
         {
            // Atende a disciplina 'disciplina_substituta'
            v_disc_substituta->setValue( alunos_disciplina_substituta );
            alunos_atendidos -= alunos_disciplina_substituta;

            if ( alunos_atendidos == 0 )
            {
               continue;
            }
         }
         else
         {
            // Assim, temos que só a disciplina 'disciplina_substituta' será
            // atendida, não sendo criada nenhuma nova variável para as suas
            // disciplinas equivalentes, pois as mesmas não foram atendidas
            continue;
         }

         //------------------------------------------------------------------------------------
         // Enquanto for possível, criamos variáveis referentes ao atendimento da demanda
         // das disciplinas substituídas. OBS.: Partimos do princípio que a escolha de qual
         // disciplina deve ser atendida prioritariamente é INDIFERENTE para a solução
         ITERA_GGROUP_LESSPTR( it_disc_equi, it_conjunto_disc->second, Disciplina )
         {
            disciplina_equivalente = ( *it_disc_equi );

            // Procura pela demanda original da disciplina que foi substituída
            // Demanda * demanda_equivalente = problemData->buscaDemanda( curso, disciplina_equivalente );
            Demanda * demanda_equivalente
               = problemData->demandasDisciplinasSubstituidas[ disciplina_equivalente ];

            // Demanda da disciplina equivalente
            int alunos_disciplina_equivalente = demanda_equivalente->getQuantidade();

            //------------------------------------------------------------
            // Criando uma nova  variável 'a' (alunos)
            Campus * campus_alunos = demanda_equivalente->oferta->campus;
            Unidade * unidade_alunos = v_disc_substituta->getUnidade();
            ConjuntoSala * cjtSala_alunos = v_disc_substituta->getSubCjtSala();
            Sala * sala_alunos = v_disc_substituta->getSala();
            int dia_alunos = v_disc_substituta->getDia();
            Oferta * oferta_alunos = demanda_equivalente->oferta;
            Curso * curso_alunos = oferta_alunos->curso;
            Disciplina * disc_alunos = disciplina_equivalente;

				// int turma_alunos = v_disc_substituta->getTurma();
				int turma_alunos = -1;

            Variable * v = criaVariavelAlunos( campus_alunos, unidade_alunos, cjtSala_alunos, sala_alunos,
                                               dia_alunos, oferta_alunos, curso_alunos, disc_alunos, turma_alunos );

            if ( alunos_disciplina_equivalente <= alunos_atendidos )
            {
               // Atende toda a demanda da disciplina 'disciplina_equivalente'
               v->setValue( alunos_disciplina_equivalente );
               alunos_atendidos -= alunos_disciplina_equivalente;
            }
            else
            {
               // Atende-se a parcela de alunos da demanda que for possível alocar
               v->setValue( alunos_atendidos );
               alunos_atendidos = 0;
            }

            // Diz que a disciplina equivalente foi atendida, ou seja,
            // posso posteriormente criar variável de créditos para essa disciplina
            problemData->disciplinasSubstituidasAtendidas[ disciplina_equivalente ] = true;
            //------------------------------------------------------------

            // Não pode atender mais disciplinas
            if ( alunos_atendidos == 0 )
            {
               break;
            }
         }
         //------------------------------------------------------------------------------------
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

	// Procura criar variáveis 'x' para as disciplinas que foram substituídas
   for (; it_disc_substituidas != problemData->mapGroupDisciplinasSubstituidas.end();
          it_disc_substituidas++ )
   {
      curso = it_disc_substituidas->first.first;
      curriculo = it_disc_substituidas->first.second;

		// Cria variáveis para cada uma das disciplinas substituídas
      std::map< Disciplina *, GGroup< Disciplina *, LessPtr< Disciplina > > >::iterator
         it_conjunto_disc = it_disc_substituidas->second.begin();

      for (; it_conjunto_disc != it_disc_substituidas->second.end();
             it_conjunto_disc++ )
      {
         disciplina_substituta = ( it_conjunto_disc->first );

			// Busca pelas ofertas da disciplina 'disciplina_substituta'
			// GGroup< Oferta *, LessPtr< Oferta > > ofertas_disc
			// 	= problemData->ofertasDisc[ disciplina_substituta->getId() ];

			// Procura pelas variáveis créditos que foram
			// criadas para atender a 'disciplina_substituta'
         std::vector< Variable * > variaveis_creditos
            = variaveisCreditosAtendidos( disciplina_substituta );

			// Procura pela variável 'x' que corresponde EXATAMENTE à disciplina
			// 'disciplina_substituta', e não à alguma das disciplinas que substituiu
         Variable * v_disc_substituta = variaveis_creditos[ 0 ];

			int cont_variaveis = 0;

         //------------------------------------------------------------------------------------
         // Enquanto for possível, criamos variáveis referentes
         // ao atendimento da demanda das disciplinas substituídas.
         // OBS.: Partimos do princípio que a escolha de qual disciplina
         // deve ser atendida prioritariamente é INDIFERENTE para a solução
         ITERA_GGROUP_LESSPTR( it_disc_equi, it_conjunto_disc->second, Disciplina )
         {
            disciplina_equivalente = ( *it_disc_equi );

				// Seleciona dentre as variáveis 'x' que correspondem à disciplina
				// 'disciplina_substituta' qual será utilizada para 'disciplina_equivalente'
				Variable * v = NULL;
				if ( cont_variaveis < (int)variaveis_creditos.size() )
				{
					v = variaveis_creditos[ cont_variaveis ];
					cont_variaveis++;
				}
				else
				{
					v = variaveis_creditos.back();
				}

				// Procura pela oferta da disciplina 'disciplina_equivalente'
				Oferta * oferta_disc_equi = problemData->retornaOfertaDiscilpina(
					curso, curriculo, disciplina_equivalente );

				// Bloco curricular da disciplina 'disciplina_equivalente'
				std::pair< Curso *, Disciplina * > curso_disciplina
					= std::make_pair( curso, disciplina_equivalente );

				BlocoCurricular * bloco_curricular_equi
					= problemData->mapCursoDisciplina_BlocoCurricular[ curso_disciplina ];

            // Verifica se a disciplina equivalente
				// teve pelo menos um aluno atendido na solução
            bool disciplina_atendida = false;

				std::map< Disciplina *, bool >::iterator it_find_disciplina
               = problemData->disciplinasSubstituidasAtendidas.find( disciplina_equivalente );

				if ( it_find_disciplina != problemData->disciplinasSubstituidasAtendidas.end() )
				{
					disciplina_atendida = it_find_disciplina->second;
				}

            if ( disciplina_atendida )
            {
               //------------------------------------------------------------
               // Criando uma nova  variável 'x' (créditos) para a disciplina equivalente
					Campus * campus_creditos = problemData->refCampus[ v->getUnidade()->getIdCampus() ];
               Unidade * unidade_creditos = v->getUnidade();
               ConjuntoSala * cjtSala_creditos = v->getSubCjtSala();
               Sala * sala_creditos = v->getSala();
               int dia_creditos = v->getDia();
               Oferta * oferta_creditos = oferta_disc_equi;
               Curso * curso_creditos = curso;
					Disciplina * disc_creditos = disciplina_equivalente;
               int turma_creditos = v->getTurma();
               BlocoCurricular * bloco_curricular = bloco_curricular_equi;

               Variable * v = criaVariavelCreditos( campus_creditos, unidade_creditos, cjtSala_creditos,
                                                    sala_creditos, dia_creditos, oferta_creditos, curso_creditos,
                                                    disc_creditos, turma_creditos, bloco_curricular );

               v->setValue( v_disc_substituta->getValue() );
               vars_x.push_back( v );

					// Adiciono uma variável 'a' a mais para o getSolutionTatico()
					std::pair< int, Disciplina * > turma_disciplina
                  = std::make_pair( v->getTurma(), v->getDisciplina() );
               vars_a[ turma_disciplina ].push_back( v );
               //------------------------------------------------------------
            }
         }
      }
   }
}

// Retorna as variáveis de alunos referentes à essa disciplina,
// em qualquer dos cursos que sejam compatíveis com o curso dessa da disciplina
std::vector< Variable * > SolverMIP::variaveisAlunosAtendidos( Curso * curso, Disciplina * disciplina )
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

// Retorna as variáveis de alunos referentes à essa disciplina
std::vector< Variable * > SolverMIP::variaveisCreditosAtendidos( Disciplina * disciplina )
{
   std::vector< Variable * > variaveis;

   for ( int i = 0; i < (int)vars_x.size(); i++ )
   {
      Variable * v = vars_x[i];

      if ( abs( v->getDisciplina()->getId() ) == abs( disciplina->getId() ) )
      {
         variaveis.push_back( v );
      }
   }

   return variaveis;
}

Variable * SolverMIP::criaVariavelAlunos(
   Campus * campus, Unidade * unidade, ConjuntoSala * cjtSala, Sala * sala,
   int dia_semana, Oferta * oferta, Curso * curso, Disciplina * disciplina, int turma )
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
   Campus * campus, Unidade * unidade, ConjuntoSala * cjtSala, Sala * sala,
   int dia_semana, Oferta * oferta, Curso * curso, Disciplina * disciplina, int turma, BlocoCurricular * bloco_curricular )
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

int SolverMIP::localBranching( double * xSol, double maxTime )
{
   // Adiciona restrição de local branching
   int status = 0;
   int nIter = 0;
   int * idxSol = new int[ lp->getNumCols() ];

   for ( int i = 0; i < lp->getNumCols(); i++ )
   {
      idxSol[i] = i;
   }

   while ( nIter < 3 )
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
               nR.insert( vit->second,-1.0 );
            }
         }

         vit++;
      }

      nR.setRhs( rhsLB );
      lp->addRow( nR );

      lp->updateLP();

      lp->setNodeLimit(100000000);
      lp->setTimeLimit(1200);
      lp->setNodeLimit(1);
      lp->setMIPEmphasis(1);
      lp->setHeurFrequency(1.0);

      lp->copyMIPStartSol( lp->getNumCols(), idxSol, xSol );

      status = lp->optimize( METHOD_MIP );

      if ( nIter == 2 )
      {
         break;
      }

      lp->getX( xSol );

      int *idxs = new int[1];
      idxs[0] = lp->getNumRows() - 1;
      lp->delSetRows(1,idxs);
      lp->updateLP();
      delete[] idxs;
      nIter++;
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
      getSolutionTatico();
   }
   // Input OPERACIONAL
   else if ( problemData->parametros->modo_otimizacao == "OPERACIONAL" )
   {
      if ( problemData->atendimentosTatico == NULL )
      {
         getSolutionTatico();
      }

      getSolutionOperacional();
   }
}

// ==============================================================
//							VARIABLES
// ==============================================================

int SolverMIP::cria_variaveis()
{
   int num_vars = 0;

#ifdef PRINT_cria_variaveis
   int numVarsAnterior = 0;
#endif

   //if(!problemData->parametros->permitir_alunos_em_varios_campi)
   num_vars += cria_variavel_oferecimentos(); // variável 'o'
   //else
   //   num_vars += cria_variavel_oferecimentos_permitir_alunos_varios_campi(); // variavel o

#ifdef PRINT_cria_variaveis
   std::cout << "numVars \"o\": " << (num_vars - numVarsAnterior) << std::endl;
   numVarsAnterior = num_vars;
#endif

   //if (!problemData->parametros->permitir_alunos_em_varios_campi)
   //{
   num_vars += cria_variavel_creditos(); // x
   //}
   //else
   //{
   //   num_vars += cria_variavel_creditos_permitir_alunos_varios_campi(); // x
   //}

#ifdef PRINT_cria_variaveis
   std::cout << "numVars \"x\": " << (num_vars - numVarsAnterior) << std::endl;
   numVarsAnterior = num_vars;
#endif

   //if(!problemData->parametros->permitir_alunos_em_varios_campi)
   num_vars += cria_variavel_abertura(); // z
   //else
   //   num_vars += cria_variavel_abertura_permitir_alunos_varios_campi(); // z

#ifdef PRINT_cria_variaveis
   std::cout << "numVars \"z\": " << (num_vars - numVarsAnterior) << std::endl;
   numVarsAnterior = num_vars;
#endif

   num_vars += cria_variavel_alunos(); // a

#ifdef PRINT_cria_variaveis
   std::cout << "numVars \"a\": " << (num_vars - numVarsAnterior) << std::endl;
   numVarsAnterior = num_vars;
#endif

   num_vars += cria_variavel_consecutivos(); // c

#ifdef PRINT_cria_variaveis
   std::cout << "numVars \"c\": " << (num_vars - numVarsAnterior) << std::endl;
   numVarsAnterior = num_vars;
#endif

   num_vars += cria_variavel_max_creds(); // H

#ifdef PRINT_cria_variaveis
   std::cout << "numVars \"H\": " << (num_vars - numVarsAnterior) << std::endl;
   numVarsAnterior = num_vars;
#endif

   num_vars += cria_variavel_min_creds(); // h

#ifdef PRINT_cria_variaveis
   std::cout << "numVars \"h\": " << (num_vars - numVarsAnterior) << std::endl;
   numVarsAnterior = num_vars;
#endif

   //if(!problemData->parametros->permitir_alunos_em_varios_campi)
   num_vars += cria_variavel_aloc_disciplina(); // y
   //else
   //   num_vars += cria_variavel_aloc_disciplina_permitir_alunos_varios_campi(); // y

#ifdef PRINT_cria_variaveis
   std::cout << "numVars \"y\": " << (num_vars - numVarsAnterior) << std::endl;
   numVarsAnterior = num_vars;
#endif

   num_vars += cria_variavel_num_subblocos(); // w

#ifdef PRINT_cria_variaveis
   std::cout << "numVars \"w\": " << (num_vars - numVarsAnterior) << std::endl;
   numVarsAnterior = num_vars;
#endif

   num_vars += cria_variavel_num_abertura_turma_bloco(); // v

#ifdef PRINT_cria_variaveis
   std::cout << "numVars \"v\": " << (num_vars - numVarsAnterior) << std::endl;
   numVarsAnterior = num_vars;
#endif

   //if (!problemData->parametros->permitir_alunos_em_varios_campi)
   num_vars += cria_variavel_aloc_alunos(); // b
   //else
   //   num_vars += cria_variavel_aloc_alunos_permitir_alunos_varios_campi(); // b

#ifdef PRINT_cria_variaveis
   std::cout << "numVars \"b\": " << (num_vars - numVarsAnterior) << std::endl;
   numVarsAnterior = num_vars;
#endif

   //if(!problemData->parametros->permitir_alunos_em_varios_campi)
   num_vars += cria_variavel_de_folga_dist_cred_dia_superior(); // fcp
   //else
   //   num_vars += cria_variavel_de_folga_dist_cred_dia_superior_permitir_alunos_varios_campi(); // fcp

#ifdef PRINT_cria_variaveis
   std::cout << "numVars \"fcp\": " << (num_vars - numVarsAnterior) << std::endl;
   numVarsAnterior = num_vars;
#endif

   //if(!problemData->parametros->permitir_alunos_em_varios_campi)
   num_vars += cria_variavel_de_folga_dist_cred_dia_inferior(); // fcm
   //else
   //   num_vars += cria_variavel_de_folga_dist_cred_dia_inferior_permitir_alunos_varios_campi(); // fcm

#ifdef PRINT_cria_variaveis
   std::cout << "numVars \"fcm\": " << (num_vars - numVarsAnterior) << std::endl;
   numVarsAnterior = num_vars;
#endif

   num_vars += cria_variavel_abertura_subbloco_de_blc_dia_campus(); // r

#ifdef PRINT_cria_variaveis
   std::cout << "numVars \"r\": " << (num_vars - numVarsAnterior) << std::endl;
   numVarsAnterior = num_vars;
#endif

   //if(!problemData->parametros->permitir_alunos_em_varios_campi)
   num_vars += cria_variavel_de_folga_aloc_alunos_curso_incompat(); // bs
   //else
   //   num_vars += cria_variavel_de_folga_aloc_alunos_curso_incompat_permitir_alunos_varios_campi(); // bs

#ifdef PRINT_cria_variaveis
   std::cout << "numVars \"bs\": " << (num_vars - numVarsAnterior) << std::endl;
   numVarsAnterior = num_vars;
#endif

   num_vars += cria_variavel_de_folga_demanda_disciplina(); // fd

#ifdef PRINT_cria_variaveis
   std::cout << "numVars \"fd\": " << (num_vars - numVarsAnterior) << std::endl;
   numVarsAnterior = num_vars;
#endif

   num_vars += cria_variavel_combinacao_divisao_credito(); // m

#ifdef PRINT_cria_variaveis
   std::cout << "numVars \"m\": " << (num_vars - numVarsAnterior) << std::endl;
   numVarsAnterior = num_vars;
#endif

   num_vars += cria_variavel_de_folga_combinacao_divisao_credito(); // fk

#ifdef PRINT_cria_variaveis
   std::cout << "numVars \"fk\": " << (num_vars - numVarsAnterior) << std::endl;
   numVarsAnterior = num_vars;
#endif

   //if(problemData->parametros->permitir_alunos_em_varios_campi)
   num_vars += cria_variavel_creditos_modificada(); // xm
   //else
   //   num_vars += cria_variavel_creditos_modificada_permitir_alunos_varios_campi(); // xm

#ifdef PRINT_cria_variaveis
   std::cout << "numVars \"xm\": " << (num_vars - numVarsAnterior) << std::endl;
   numVarsAnterior = num_vars;
#endif

   num_vars += cria_variavel_abertura_compativel(); // zc

#ifdef PRINT_cria_variaveis
   std::cout << "numVars \"zc\": " << (num_vars - numVarsAnterior) << std::endl;
   numVarsAnterior = num_vars;
#endif

   num_vars += cria_variavel_abertura_bloco_mesmoTPS(); // n

#ifdef PRINT_cria_variaveis
   std::cout << "numVars \"n\": " << (num_vars - numVarsAnterior) << std::endl;
   numVarsAnterior = num_vars;
#endif

   num_vars += cria_variavel_de_folga_abertura_bloco_mesmoTPS(); // fn

#ifdef PRINT_cria_variaveis
   std::cout << "numVars \"fn\": " << (num_vars - numVarsAnterior) << std::endl;
   numVarsAnterior = num_vars;
#endif

   return num_vars;
}

/*====================================================================/
%DocBegin TRIEDA_LOAD_MODEL

%Var x_{i,d,u,tps,t}

%Desc 
número de créditos da turma $i$ da disciplina $d$ na unidade $u$ 
em salas do tipo (capacidade) $tps$ no dia $t$. 

%ObjCoef
\theta \cdot \sum\limits_{u \in U}\sum\limits_{tps \in SCAP_{u}} 
\sum\limits_{d \in D}\sum\limits_{t \in T}
\sum\limits_{i \in I_{d}} x_{i,d,u,tps,t}

%DocEnd
/====================================================================*/

int SolverMIP::cria_variavel_creditos( void )
{
   int num_vars = 0;

   Disciplina * disciplina = NULL;
   Disciplina * disciplina_equivalente = NULL;

   Curso * curso = NULL;
   Curriculo * curriculo = NULL;

   ITERA_GGROUP_LESSPTR( itCampus, problemData->campi, Campus )
   {
      ITERA_GGROUP_LESSPTR( itUnidade, itCampus->unidades, Unidade )
      {
         ITERA_GGROUP_LESSPTR( itCjtSala, itUnidade->conjutoSalas, ConjuntoSala )
         {
            ITERA_GGROUP_LESSPTR( it_disc, itCjtSala->disciplinas_associadas, Disciplina )
            {
               disciplina = ( *it_disc );

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
                  // Dada uma disciplina, recupera os seus dias letivos, observando as fixações
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
                        ITERA_GGROUP_LESSPTR( it_prof, itCampus->professores, Professor )
                        {
                           // id_professor + id_disciplina
                           std::pair< int, int > prof_Disc 
                              ( it_prof->getId(), disciplina->getId() );

                           if ( problemData->prof_Disc_Dias.find( prof_Disc )
                              != problemData->prof_Disc_Dias.end() )
                           {
                              eta = 0;
                           }
                           else
                           {
                              eta = 10;
                           }

                           if ( vHash.find( v ) == vHash.end() )
                           {
                              vHash[v] = lp->getNumCols();

                              if ( problemData->parametros->funcao_objetivo == 0 )
                              {
                                 OPT_COL col( OPT_COL::VAR_INTEGRAL, eta, 0.0,
                                    itCjtSala->maxCredsDia( *itDiscSala_Dias ),
                                    ( char * )v.toString().c_str() );

                                 lp->newCol( col );
                              }
                              else if( problemData->parametros->funcao_objetivo == 1
                                 || problemData->parametros->funcao_objetivo == 2 )
                              {
                                 OPT_COL col( OPT_COL::VAR_INTEGRAL, 0.0, 0.0,
                                    itCjtSala->maxCredsDia( *itDiscSala_Dias ),
                                    ( char * )v.toString().c_str() );

                                 lp->newCol( col );
                              }

                              num_vars++;
                           }
                        }
                     }
                     else
                     {
                        if ( vHash.find( v ) == vHash.end() )
                        {
                           vHash[v] = lp->getNumCols();

                           OPT_COL col( OPT_COL::VAR_INTEGRAL, 0.0, 0.0,
                              itCjtSala->maxCredsDia( *itDiscSala_Dias ),
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
//                  // Dada uma disciplina, recupera os seus dias letivos, observando as fixações
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

int SolverMIP::cria_variavel_oferecimentos(void)
{
   int num_vars = 0;

   Disciplina * disciplina = NULL;
   Disciplina * disciplina_equivalente = NULL;

   Curso * curso = NULL;
   Curriculo * curriculo = NULL;

   ITERA_GGROUP_LESSPTR( itCampus, problemData->campi, Campus )
   {
      ITERA_GGROUP_LESSPTR( itUnidade, itCampus->unidades, Unidade )
      {
         ITERA_GGROUP_LESSPTR( itCjtSala, itUnidade->conjutoSalas, ConjuntoSala )
         {
            ITERA_GGROUP_LESSPTR( it_disciplina, itCjtSala->disciplinas_associadas, Disciplina )
            {
               disciplina = ( *it_disciplina );

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
                  // Dada uma disciplina, recupera os seus dias letivos, observando as fixações
                  GGroup< int > dias_letivos =
                     itCjtSala->dias_letivos_disciplinas[ ( disciplina ) ];

                  GGroup< int >::iterator itDiscSala_Dias = dias_letivos.begin();

                  for (; itDiscSala_Dias != dias_letivos.end(); itDiscSala_Dias++ )
                  {
                     Variable v;
                     v.reset();
                     v.setType( Variable::V_OFERECIMENTO );

                     v.setTurma( turma );            // i
                     v.setDisciplina( disciplina );     // d
                     v.setUnidade( *itUnidade );     // u
                     v.setSubCjtSala( *itCjtSala );  // tps  
                     v.setDia( *itDiscSala_Dias );   // t

                     if ( vHash.find( v ) == vHash.end() )
                     {
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
//                  // Dada uma disciplina, recupera os seus dias letivos, observando as fixações
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
indica se houve abertura da $i$-ésima turma da disciplina $d$ no campus $cp$.

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
peso associado a função objetivo.

%Data \gamma
%Desc
peso associado a função objetivo.

%DocEnd
/====================================================================*/

int SolverMIP::cria_variavel_abertura(void)
{
   int num_vars = 0;

   // Pode ser implementado de uma maneira melhor, listando
   // apenas as disciplinas que podem ser abertas em um campus
   // (atraves do OFERTACURSO) e criando as suas respectivas
   // variaveis. Desse modo, variaveis desnecessárias (relacionadas
   // à disciplinas que não existem em outros campus) seriam evitadas.
   // VER <demandas_campus> em <ProblemData>

   Disciplina * disciplina = NULL;
   Disciplina * disciplina_equivalente = NULL;

   Curso * curso = NULL;
   Curriculo * curriculo = NULL;

   ITERA_GGROUP_LESSPTR( it_campus, problemData->campi, Campus )
   {
      ITERA_GGROUP_LESSPTR( it_disciplina, problemData->disciplinas, Disciplina )
      {
         disciplina = ( *it_disciplina );

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

            double ratioDem = ( disciplina->getDemandaTotal() - 
               problemData->demandas_campus[ dc ] ) 
               / (1.0 * disciplina->getDemandaTotal() );

            double coeff = alpha + gamma * ratioDem;

            int numCreditos = ( disciplina->getCredTeoricos() + disciplina->getCredPraticos() );
            double valorCredito = 1600.0;
            double coef_FO_1_2 = ( numCreditos * valorCredito );

            if ( vHash.find(v) == vHash.end() )
            {
               lp->getNumCols();
               vHash[v] = lp->getNumCols();

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
               }

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
número de alunos de uma oferta $oft$ alocados para a $i$-ésima turma da disciplina $d$.

%DocEnd
/====================================================================*/

int SolverMIP::cria_variavel_alunos(void)
{
   int total_Vars = 0;
   int num_vars = 0;

   Disciplina * disciplina_equivalente = NULL;

   Curso * curso = NULL;
   Curriculo * curriculo = NULL;

   ITERA_GGROUP_LESSPTR( itOferta, problemData->ofertas, Oferta )
   {
      GGroup< std::pair< int, Disciplina * > >::iterator itPrdDisc = 
         itOferta->curriculo->disciplinas_periodo.begin();
      for(; itPrdDisc != itOferta->curriculo->disciplinas_periodo.end();
         itPrdDisc++ )
      {
         Disciplina * ptDisc = ( *itPrdDisc ).second;

         std::pair< Curso *, Curriculo * > curso_curriculo
            = problemData->map_Disc_CursoCurriculo[ ptDisc ];
         curso = curso_curriculo.first;
         curriculo = curso_curriculo.second;

         disciplina_equivalente = problemData->retornaDisciplinaSubstituta( curso, curriculo, ptDisc );
         if ( disciplina_equivalente != NULL )
         {
            ptDisc = disciplina_equivalente;
         }

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

            int numCreditos = ( ptDisc->getCredTeoricos() + ptDisc->getCredPraticos() );
            double valorCredito = 300.0;
            double coeff = ( numCreditos * valorCredito );

            if ( vHash.find(v) == vHash.end() )
            {
               vHash[v] = lp->getNumCols();

               if ( problemData->parametros->funcao_objetivo == 0 ||
                  problemData->parametros->funcao_objetivo == 2 )
               {
                  OPT_COL col( OPT_COL::VAR_INTEGRAL, 0.0, 0.0, qtdDem,
                     ( char * )v.toString().c_str() );

                  lp->newCol( col );
               }
               else if ( problemData->parametros->funcao_objetivo == 1 )
               {
                  OPT_COL col( OPT_COL::VAR_INTEGRAL, coeff, 0.0, qtdDem,
                     ( char * )v.toString().c_str() );

                  lp->newCol( col );
               }

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
indica se algum aluno do curso $c$ foi alocado para a $i$-ésima turma da disciplina $d$ no campus $cp$.

%DocEnd
/====================================================================*/

int SolverMIP::cria_variavel_aloc_alunos(void)
{
   int num_vars = 0;

   Disciplina * disciplina = NULL;
   Disciplina * disciplina_equivalente = NULL;

   Curso * curso = NULL;
   Curriculo * curriculo = NULL;

   ITERA_GGROUP_LESSPTR( it_Oferta, problemData->ofertas, Oferta )
   {
      Campus * pt_Campus = it_Oferta->campus;
      Curso * pt_Curso = it_Oferta->curso;

      GGroup< std::pair< int, Disciplina * > >::iterator it_Prd_Disc = 
         it_Oferta->curriculo->disciplinas_periodo.begin();
      for(; it_Prd_Disc != it_Oferta->curriculo->disciplinas_periodo.end();
         it_Prd_Disc++ )
      {
         disciplina = ( *it_Prd_Disc ).second;

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
            Variable v;
            v.reset();
            v.setType( Variable::V_ALOC_ALUNO );

            v.setTurma( turma );           // i
            v.setDisciplina( disciplina );    // d
            v.setCurso( pt_Curso );        // c
            v.setCampus( pt_Campus );	   // cp

            if ( vHash.find( v ) == vHash.end() )
            {
               vHash[v] = lp->getNumCols();

               OPT_COL col( OPT_COL::VAR_BINARY, 0, 0.0, 1.0,
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
peso associado a função objetivo.

%DocEnd
/====================================================================*/

int SolverMIP::cria_variavel_consecutivos( void )
{
   int num_vars = 0;

   Disciplina * disciplina = NULL;
   Disciplina * disciplina_equivalente = NULL;

   Curso * curso = NULL;
   Curriculo * curriculo = NULL;

   ITERA_GGROUP_LESSPTR( it_campus, problemData->campi, Campus )
   {
      ITERA_GGROUP_LESSPTR( it_unidade, it_campus->unidades, Unidade )
      {
         ITERA_GGROUP_LESSPTR( it_conjunto_sala, it_unidade->conjutoSalas, ConjuntoSala )
         {
            ITERA_GGROUP_LESSPTR( it_disciplina, it_conjunto_sala->disciplinas_associadas, Disciplina )
            {
               disciplina = ( *it_disciplina );

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
                  // Dada uma disciplina, recupera os seus dias letivos, observando as fixações
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
                        vHash[v] = lp->getNumCols();

                        if ( problemData->parametros->funcao_objetivo == 0 )
                        {
                           OPT_COL col( OPT_COL::VAR_BINARY, delta, 0.0, 1.0,
                              ( char * )v.toString().c_str() );

                           lp->newCol( col );
                        }
                        else if ( problemData->parametros->funcao_objetivo == 1
                           || problemData->parametros->funcao_objetivo == 2 )
                        {
                           OPT_COL col( OPT_COL::VAR_BINARY, 0.0, 0.0, 1.0,
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

/*====================================================================/
%DocBegin TRIEDA_LOAD_MODEL

%Var \underline{h}_{bc,i} 

%Desc 
mínimo de créditos alocados na semana na $i$-ésima turma do bloco $bc$.
%ObjCoef
\lambda \cdot \sum\limits_{bc \in B} 
\sum\limits_{i \in I_{d}, d \in D_{bc}} \left( \overline{h}_{bc,i} - \underline{h}_{bc,i} \right)

%Data \underline{H_{d}} 
%Desc
mínimo de créditos diários da disciplina $d$.

%Data \lambda
%Desc
peso associado a função objetivo.

%DocEnd
/====================================================================*/

int SolverMIP::cria_variavel_min_creds(void)
{
   int num_vars = 0;

   Disciplina * disciplina = NULL;
   Disciplina * disciplina_equivalente = NULL;

   Curso * curso = NULL;
   Curriculo * curriculo = NULL;

   ITERA_GGROUP_LESSPTR( it_bloco, problemData->blocos, BlocoCurricular )
   {
      ITERA_GGROUP_LESSPTR( it_disciplina, it_bloco->disciplinas, Disciplina )
      {
         disciplina = ( *it_disciplina );

         std::pair< Curso *, Curriculo * > curso_curriculo
            = problemData->map_Disc_CursoCurriculo[ disciplina ];
         curso = curso_curriculo.first;
         curriculo = curso_curriculo.second;

         disciplina_equivalente = problemData->retornaDisciplinaSubstituta( curso, curriculo, disciplina );
         if ( disciplina_equivalente != NULL )
         {
            disciplina = disciplina_equivalente;
         }

         for ( int i = 0; i < disciplina->getNumTurmas(); i++ )
         {
            Variable v;
            v.reset();
            v.setType( Variable::V_MIN_CRED_SEMANA );

            v.setBloco( *it_bloco );
            v.setTurma( i );

            if ( vHash.find( v ) == vHash.end() )
            {
               vHash[ v ] = lp->getNumCols();

               if ( problemData->parametros->funcao_objetivo == 0 )
               {
                  OPT_COL col( OPT_COL::VAR_INTEGRAL, -lambda, 0.0, 1000.0,
                     ( char * )v.toString().c_str() );

                  lp->newCol( col );
               }
               else if ( problemData->parametros->funcao_objetivo == 1
                  || problemData->parametros->funcao_objetivo == 2 )
               {
                  OPT_COL col( OPT_COL::VAR_INTEGRAL, 0.0, 0.0, 1000.0,
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

%Var \overline{h}_{bc,i} 

%Desc 
máximo de créditos alocados na semana na $i$-ésima turma do bloco $bc$.

%Data \overline{H_{d}} 
%Desc
máximo de créditos diários da disciplina $d$.

%DocEnd
/====================================================================*/

int SolverMIP::cria_variavel_max_creds(void)
{
   int num_vars = 0;

   Disciplina * disciplina = NULL;
   Disciplina * disciplina_equivalente = NULL;

   Curso * curso = NULL;
   Curriculo * curriculo = NULL;

   ITERA_GGROUP_LESSPTR( it_bloco, problemData->blocos, BlocoCurricular )
   {
      ITERA_GGROUP_LESSPTR( it_disciplina, it_bloco->disciplinas, Disciplina )
      {
         disciplina = ( *it_disciplina );

         std::pair< Curso *, Curriculo * > curso_curriculo
            = problemData->map_Disc_CursoCurriculo[ disciplina ];
         curso = curso_curriculo.first;
         curriculo = curso_curriculo.second;

         disciplina_equivalente = problemData->retornaDisciplinaSubstituta( curso, curriculo, disciplina );
         if ( disciplina_equivalente != NULL )
         {
            disciplina = disciplina_equivalente;
         }

         for ( int i = 0; i < disciplina->getNumTurmas(); i++ )
         {
            Variable v;
            v.reset();
            v.setType( Variable::V_MAX_CRED_SEMANA );

            v.setBloco( *it_bloco );
            v.setTurma( i );

            if ( vHash.find( v ) == vHash.end() )
            {
               vHash[v] = lp->getNumCols();

               if ( problemData->parametros->funcao_objetivo == 0 )
               {
                  OPT_COL col( OPT_COL::VAR_INTEGRAL, lambda, 0.0, 1000.0,
                     ( char * )v.toString().c_str() );

                  lp->newCol( col );
               }
               else if ( problemData->parametros->funcao_objetivo == 1 ||
                  problemData->parametros->funcao_objetivo == 2 )
               {
                  OPT_COL col( OPT_COL::VAR_INTEGRAL, 0.0, 0.0, 1000.0,
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

%Var y_{i,d,tps,u} 

%Desc 
indica que a turma $i$ da disciplina $d$ foi alocada em alguma sala do tipo $tps$ da unidade $u$.

%DocEnd
/====================================================================*/

int SolverMIP::cria_variavel_aloc_disciplina(void)
{
   // Poderia criar as variaveis apenas qdo a
   // disciplina for compativel com a sala. Por
   // exemplo, não criar uma variável qdo a disciplina
   // demanda horarios que a sala não dispõe.

   int num_vars = 0;

   Disciplina * disciplina = NULL;
   Disciplina * disciplina_equivalente = NULL;

   Curso * curso = NULL;
   Curriculo * curriculo = NULL;

   ITERA_GGROUP_LESSPTR( itCampus, problemData->campi, Campus )
   {
      ITERA_GGROUP_LESSPTR( itUnidade, itCampus->unidades, Unidade )
      {
         ITERA_GGROUP_LESSPTR( itCjtSala, itUnidade->conjutoSalas, ConjuntoSala )
         {
            ITERA_GGROUP_LESSPTR( it_disciplina, itCjtSala->disciplinas_associadas, Disciplina )
            {
               disciplina = ( *it_disciplina );

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
                  Variable v;
                  v.reset();
                  v.setType( Variable::V_ALOC_DISCIPLINA );

                  v.setTurma( turma );            // i
                  v.setDisciplina( disciplina );  // d
                  v.setSubCjtSala( *itCjtSala );  // tps
                  v.setUnidade( *itUnidade );     // u

                  if ( vHash.find( v ) == vHash.end() )
                  {
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
//   // for compativel com a sala. Por exemplo, não criar
//   // uma variável qdo a disciplia demanda horarios que a sala não dispõe.
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
indica o número sub-blocos abertos do bloco curricular $bc$ no dia $t$ no campus $cp$.
%ObjCoef
\rho \cdot \sum\limits_{bc \in B}\sum\limits_{t \in T} (\sum\limits_{cp \in CP} w_{bc,t,cp})

%Data \rho
%Desc
peso associado a função objetivo.

%DocEnd
/====================================================================*/

int SolverMIP::cria_variavel_num_subblocos(void)
{
   int num_vars = 0;

   ITERA_GGROUP_LESSPTR( it_bloco, problemData->blocos, BlocoCurricular )
   {
      Campus * campus = it_bloco->campus;

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
            vHash[ v ] = lp->getNumCols();

            if ( problemData->parametros->funcao_objetivo == 0 )
            {
               OPT_COL col( OPT_COL::VAR_INTEGRAL, rho, 0.0, 10,
                  ( char * )v.toString().c_str() );

               lp->newCol( col );
            }
            else if ( problemData->parametros->funcao_objetivo == 1 ||
               problemData->parametros->funcao_objetivo == 2 )
            {
               OPT_COL col( OPT_COL::VAR_INTEGRAL, 0.0, 0.0, 10,
                  ( char * )v.toString().c_str() );

               lp->newCol( col );
            }

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
peso associado a função objetivo.

%DocEnd
/====================================================================*/

int SolverMIP::cria_variavel_num_abertura_turma_bloco(void)
{
   int num_vars = 0;

   ITERA_GGROUP_LESSPTR( it_bloco, problemData->blocos, BlocoCurricular )
   {
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
            vHash[v] = lp->getNumCols();

            if ( problemData->parametros->funcao_objetivo == 0 )
            {
               OPT_COL col( OPT_COL::VAR_BINARY, beta, 0.0, 1.0,
                  ( char * )v.toString().c_str() );

               lp->newCol( col );
            }
            else if ( problemData->parametros->funcao_objetivo == 1 ||
               problemData->parametros->funcao_objetivo == 2 )
            {
               OPT_COL col( OPT_COL::VAR_BINARY, 0.0, 0.0, 1.0,
                  (char*)v.toString().c_str() );

               lp->newCol( col );
            }

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
variável de folga superior para a restrição de fixação da distribuição de créditos por dia.
%ObjCoef
\xi \cdot \sum\limits_{d \in D} \sum\limits_{t \in T} fcp_{d,t}

%Data \xi
%Desc
peso associado a função objetivo.

%DocEnd
/====================================================================*/

int SolverMIP::cria_variavel_de_folga_dist_cred_dia_superior(void)
{
   int num_vars = 0;

   Disciplina * disciplina = NULL;
   Disciplina * disciplina_equivalente = NULL;

   Curso * curso = NULL;
   Curriculo * curriculo = NULL;

   ITERA_GGROUP_LESSPTR( itCampus, problemData->campi, Campus )
   {
      ITERA_GGROUP_LESSPTR( itUnidade, itCampus->unidades, Unidade )
      {
         ITERA_GGROUP_LESSPTR( itCjtSala, itUnidade->conjutoSalas, ConjuntoSala )
         {
            ITERA_GGROUP_LESSPTR( it_disciplina, problemData->disciplinas, Disciplina )
            {
               disciplina = ( *it_disciplina );

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
                  // Dada uma disciplina, recupera os seus dias letivos, observando as fixações
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
                              vHash[v] = lp->getNumCols();
                              int cred_disc_dia = it_fix->disciplina->getMaxCreds();

                              OPT_COL col( OPT_COL::VAR_INTEGRAL,
                                 epsilon, 0.0, cred_disc_dia,
                                 ( char * )v.toString().c_str() );

                              if ( problemData->parametros->funcao_objetivo == 0 )
                              {
                                 OPT_COL col( OPT_COL::VAR_INTEGRAL, epsilon, 0.0, cred_disc_dia,
                                    ( char * )v.toString().c_str() );

                                 lp->newCol( col );
                              }
                              else if ( problemData->parametros->funcao_objetivo == 1 ||
                                 problemData->parametros->funcao_objetivo == 2 )
                              {
                                 OPT_COL col( OPT_COL::VAR_INTEGRAL, 100.0, 0.0, cred_disc_dia,
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
//                  // Dada uma disciplina, recupera os seus dias letivos, observando as fixações
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
variável de folga inferior para a restrição de fixação da distribuição de créditos por dia.

%ObjCoef
\xi \cdot \sum\limits_{d \in D} \sum\limits_{t \in T} fcm_{d,t}

%DocEnd
/====================================================================*/

int SolverMIP::cria_variavel_de_folga_dist_cred_dia_inferior(void)
{
   int num_vars = 0;

   Disciplina * disciplina = NULL;
   Disciplina * disciplina_equivalente = NULL;

   Curso * curso = NULL;
   Curriculo * curriculo = NULL;

   ITERA_GGROUP_LESSPTR( itCampus, problemData->campi, Campus )
   {
      ITERA_GGROUP_LESSPTR( itUnidade, itCampus->unidades, Unidade )
      {
         ITERA_GGROUP_LESSPTR( itCjtSala, itUnidade->conjutoSalas, ConjuntoSala )
         {
            ITERA_GGROUP_LESSPTR( it_disciplina, problemData->disciplinas, Disciplina )
            {
               disciplina = ( *it_disciplina );

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
                  // Dada uma disciplina, recupera os seus dias letivos, observando as fixações
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
                              vHash[v] = lp->getNumCols();
                              int cred_disc_dia = it_fix->disciplina->getMinCreds();

                              OPT_COL col( OPT_COL::VAR_INTEGRAL,
                                 epsilon, 0.0, cred_disc_dia,
                                 ( char* )v.toString().c_str() );

                              if ( problemData->parametros->funcao_objetivo == 0 )
                              {
                                 OPT_COL col( OPT_COL::VAR_INTEGRAL,
                                    epsilon, 0.0, cred_disc_dia,
                                    ( char* )v.toString().c_str() );

                                 lp->newCol( col );
                              }
                              else if( problemData->parametros->funcao_objetivo == 1
                                 || problemData->parametros->funcao_objetivo == 2 )
                              {
                                 OPT_COL col( OPT_COL::VAR_INTEGRAL,
                                    100.0, 0.0, cred_disc_dia,
                                    ( char* )v.toString().c_str() );

                                 lp->newCol( col );
                              }

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
//                  // Dada uma disciplina, recupera os seus dias letivos, observando as fixações
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

int SolverMIP::cria_variavel_abertura_subbloco_de_blc_dia_campus()
{
   int num_vars = 0;

   ITERA_GGROUP_LESSPTR( it_bloco, problemData->blocos, BlocoCurricular )
   {
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
            vHash[ v ] = lp->getNumCols();

            OPT_COL col( OPT_COL::VAR_BINARY,
               0.0, 0.0, 1.0,
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
variável de folga para a restrição em que o compartilhamento de turmas 
de alunos de cursos diferentes é proibido.

%DocEnd
/====================================================================*/

int SolverMIP::cria_variavel_de_folga_aloc_alunos_curso_incompat()
{
   int num_vars = 0;

   Disciplina * disciplina = NULL;
   Disciplina * disciplina_equivalente = NULL;

   Curso * curso = NULL;
   Curriculo * curriculo = NULL;

   ITERA_GGROUP_LESSPTR( it_Oferta_Incompat, problemData->ofertas, Oferta )
   {
      Curso * pt_Curso_Incompat = it_Oferta_Incompat->curso;

      ITERA_GGROUP_LESSPTR( it_Oferta, problemData->ofertas, Oferta )
      {
         Campus * pt_Campus = it_Oferta->campus;
         Curso * pt_Curso = it_Oferta->curso;

         GGroup< std::pair< int, Disciplina * > >::iterator it_Prd_Disc = 
            it_Oferta->curriculo->disciplinas_periodo.begin();

         for (; it_Prd_Disc != it_Oferta->curriculo->disciplinas_periodo.end(); it_Prd_Disc++ )
         {
            disciplina = ( *it_Prd_Disc ).second;

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
               Variable v;
               v.reset();
               v.setType( Variable::V_SLACK_ALOC_ALUNOS_CURSO_INCOMPAT );

               v.setTurma( turma );					 // i
               v.setDisciplina( disciplina );   		 // d
               v.setCurso( pt_Curso );					 // c
               v.setCursoIncompat( pt_Curso_Incompat );
               v.setCampus( pt_Campus );		         // cp

               if ( vHash.find( v ) == vHash.end() )
               {
                  vHash[v] = lp->getNumCols();

                  OPT_COL col( OPT_COL::VAR_BINARY,
                     0, 0.0, 1.0, ( char * )v.toString().c_str() );

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
variável de folga para a restrição "Capacidade alocada tem que 
permitir atender demanda da disciplina".

%ObjCoef
\omega \cdot \sum\limits_{oft \in O} \sum\limits_{d \in D_{oft}} fd_{d,oft}

%DocEnd
/====================================================================*/

int SolverMIP::cria_variavel_de_folga_demanda_disciplina()
{
   int num_vars = 0;

   Disciplina * disciplina = NULL;
   Disciplina * disciplina_equivalente = NULL;

   Curso * curso = NULL;
   Curriculo * curriculo = NULL;

   ITERA_GGROUP_LESSPTR( itOferta, problemData->ofertas, Oferta )
   {
      GGroup< std::pair< int, Disciplina * > >::iterator itPrdDisc = 
         itOferta->curriculo->disciplinas_periodo.begin();

      for (; itPrdDisc != itOferta->curriculo->disciplinas_periodo.end(); itPrdDisc++ )
      {
         // Calculando P_{d,o}
         int qtdDem = 0;

         disciplina = ( *itPrdDisc ).second;

         std::pair< Curso *, Curriculo * > curso_curriculo
            = problemData->map_Disc_CursoCurriculo[ disciplina ];

         curso = curso_curriculo.first;
         curriculo = curso_curriculo.second;

         disciplina_equivalente = problemData->retornaDisciplinaSubstituta( curso, curriculo, disciplina );
         if ( disciplina_equivalente != NULL )
         {
            disciplina = disciplina_equivalente;
         }

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
         v.setOferta( *itOferta );  // o

         if ( vHash.find( v ) == vHash.end() )
         {
            vHash[ v ] = lp->getNumCols();

            double ub = qtdDem;
            if ( problemData->parametros->funcao_objetivo == 0 )
            {
               OPT_COL col( OPT_COL::VAR_INTEGRAL,
                  1000.0, 0.0, ub,
                  ( char * )v.toString().c_str() );

               lp->newCol( col );
            }
            else if( problemData->parametros->funcao_objetivo == 1 ||
               problemData->parametros->funcao_objetivo == 2 )
            {
               OPT_COL col( OPT_COL::VAR_INTEGRAL, 1000.0, 0.0, ub,
                  ( char * )v.toString().c_str() );

               lp->newCol(col);
            }

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
variável binária que indica se a combinação de divisão de créditos 
$k$ foi escolhida para a turma $i$ da disciplina $d$.

%DocEnd
/====================================================================*/

int SolverMIP::cria_variavel_combinacao_divisao_credito()
{
   int num_vars = 0;

   Disciplina * disciplina = NULL;
   Disciplina * disciplina_equivalente = NULL;

   Curso * curso = NULL;
   Curriculo * curriculo = NULL;

   ITERA_GGROUP_LESSPTR( it_disciplina, problemData->disciplinas, Disciplina )
   {
      disciplina = ( *it_disciplina );

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
         for ( unsigned k = 0; k < disciplina->combinacao_divisao_creditos.size(); k++ )
         { 
            Variable v;
            v.reset();
            v.setType( Variable::V_COMBINACAO_DIVISAO_CREDITO );

            v.setTurma( turma );        // i
            v.setDisciplina( disciplina ); // d
            v.setK( k );	            // k

            if ( vHash.find( v ) == vHash.end() )
            {
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

%Var fk_{d,i,t} 

%Desc 
variável de folga para a restrição de combinação de divisão de créditos.

%ObjCoef
\psi \cdot \sum\limits_{d \in D} 
\sum\limits_{t \in T} \sum\limits_{i \in I_{d}} fk_{d,i,k}

%Data \psi
%Desc
peso associado a função objetivo.

%DocEnd
/====================================================================*/

int SolverMIP::cria_variavel_de_folga_combinacao_divisao_credito()
{
   int num_vars = 0;

   Disciplina * disciplina = NULL;
   Disciplina * disciplina_equivalente = NULL;

   Curso * curso = NULL;
   Curriculo * curriculo = NULL;

   ITERA_GGROUP_LESSPTR( it_campus, problemData->campi, Campus )
   {
      ITERA_GGROUP_LESSPTR( it_unidade, it_campus->unidades, Unidade )
      {
         ITERA_GGROUP_LESSPTR( it_conjunto_sala, it_unidade->conjutoSalas, ConjuntoSala )
         {
            ITERA_GGROUP_LESSPTR( it_disciplina, it_conjunto_sala->disciplinas_associadas, Disciplina )
            {
               disciplina = ( *it_disciplina );

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
                  // Dada uma disciplina, recupera os seus dias letivos, observando as fixações
                  GGroup< int > dias_letivos =
                     it_conjunto_sala->dias_letivos_disciplinas[ ( disciplina ) ];

                  GGroup< int >::iterator itDiasLetDisc = dias_letivos.begin();

                  for (; itDiasLetDisc != dias_letivos.end(); itDiasLetDisc++ )
                  {
                     Variable v;
                     v.reset();
                     v.setType( Variable::V_SLACK_COMBINACAO_DIVISAO_CREDITO );

                     v.setTurma( turma );            // i
                     v.setDisciplina( disciplina );  // d
                     v.setDia( *itDiasLetDisc );	    // t

                     if ( vHash.find( v ) == vHash.end() )
                     {
                        vHash[v] = lp->getNumCols();

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

/*====================================================================/
%DocBegin TRIEDA_LOAD_MODEL

%Var xm_{bc,d,t}

%Desc
máximo de créditos alocados para qualquer turma da disciplina $d$ no bloco $bc$ no dia $t$.

%DocEnd
/====================================================================*/

int SolverMIP::cria_variavel_creditos_modificada(void)
{
   int num_vars = 0;

   Disciplina * disciplina = NULL;
   Disciplina * disciplina_equivalente = NULL;

   Curso * curso = NULL;
   Curriculo * curriculo = NULL;

   ITERA_GGROUP_LESSPTR( it_bloco, problemData->blocos, BlocoCurricular )
   {
      ITERA_GGROUP_LESSPTR(it_disciplina,it_bloco->disciplinas,Disciplina)
      {
         disciplina = ( *it_disciplina );

         std::pair< Curso *, Curriculo * > curso_curriculo
            = problemData->map_Disc_CursoCurriculo[ disciplina ];
         curso = curso_curriculo.first;
         curriculo = curso_curriculo.second;
         disciplina_equivalente = problemData->retornaDisciplinaSubstituta( curso, curriculo, disciplina );
         if ( disciplina_equivalente != NULL )
         {
            disciplina = disciplina_equivalente;
         }
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
               vHash[v] = lp->getNumCols();

               OPT_COL col( OPT_COL::VAR_INTEGRAL,
                  0.0, 0.0, 50,
                  ( char* )v.toString().c_str() );

               lp->newCol( col );
               num_vars++;
            }
         }
      }
   }

   /*ITERA_GGROUP_LESSPTR( itCampus, problemData->campi, Campus )
   {
      ITERA_GGROUP_LESSPTR( itUnidade, itCampus->unidades, Unidade )
      {
         ITERA_GGROUP_LESSPTR( itCjtSala, itUnidade->conjutoSalas, ConjuntoSala )
         {
            ITERA_GGROUP_LESSPTR( it_disciplina, itCjtSala->disciplinas_associadas, Disciplina )
            {
               disciplina = ( *it_disciplina );

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
                  // Dada uma disciplina, recupera os seus dias letivos, observando as fixações
                  GGroup< int > dias_letivos =
                     itCjtSala->dias_letivos_disciplinas[ ( disciplina ) ];

                  ITERA_GGROUP_N_PT( itDiscSala_Dias, dias_letivos, int )
                  {
                     Variable v;
                     v.reset();
                     v.setType( Variable::V_CREDITOS_MODF );

                     v.setDisciplina( disciplina );  // d
                     v.setDia( *itDiscSala_Dias );   // t

                     if ( vHash.find( v ) == vHash.end() )
                     {
                        vHash[v] = lp->getNumCols();

                        OPT_COL col( OPT_COL::VAR_INTEGRAL,
                           0.0, 0.0, 50 ,
                           ( char* )v.toString().c_str() );

                        lp->newCol( col );
                        num_vars++;
                     }
                  }
               }
            }
         }
      }
   }*/

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
//                  // Dada uma disciplina, recupera os seus dias letivos, observando as fixações
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

int SolverMIP::cria_variavel_abertura_compativel( void )
{
   int num_vars = 0;

   Disciplina * disciplina = NULL;
   Disciplina * disciplina_equivalente = NULL;

   Curso * curso = NULL;
   Curriculo * curriculo = NULL;

   ITERA_GGROUP_LESSPTR( it_campus, problemData->campi, Campus )
   {
      ITERA_GGROUP_LESSPTR( it_unidade, it_campus->unidades, Unidade )
      {
         ITERA_GGROUP_LESSPTR( it_conjunto_sala, it_unidade->conjutoSalas, ConjuntoSala )
         {
            ITERA_GGROUP_LESSPTR( it_disciplina, it_conjunto_sala->disciplinas_associadas, Disciplina )
            {
               disciplina = ( *it_disciplina );

               std::pair< Curso *, Curriculo * > curso_curriculo
                  = problemData->map_Disc_CursoCurriculo[ disciplina ];
               curso = curso_curriculo.first;
               curriculo = curso_curriculo.second;

               disciplina_equivalente = problemData->retornaDisciplinaSubstituta( curso, curriculo, disciplina );
               if ( disciplina_equivalente != NULL )
               {
                  disciplina = disciplina_equivalente;
               }

               // Dada uma disciplina, recupera os seus dias letivos, observando as fixações
               GGroup< int > dias_letivos =
                  it_conjunto_sala->dias_letivos_disciplinas[ ( disciplina ) ];

               ITERA_GGROUP_N_PT( itDiasLetDisc, dias_letivos, int )
               {
                  Variable v;
                  v.reset();
                  v.setType( Variable::V_ABERTURA_COMPATIVEL );

                  v.setDisciplina( disciplina );  // d
                  v.setDia( *itDiasLetDisc );     // t

                  if ( vHash.find( v ) == vHash.end() )
                  {
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
      }
   }

   return num_vars;
}

/*====================================================================/
%DocBegin TRIEDA_LOAD_MODEL

%Var n_{bc,tps} 

%Desc 
variável binária que indica se o bloco $bc$ foi alocado na sala $tps$. 

%DocEnd
/====================================================================*/

int SolverMIP::cria_variavel_abertura_bloco_mesmoTPS(void)
{
   int num_vars = 0;

   ITERA_GGROUP_LESSPTR( itBloco, problemData->blocos, BlocoCurricular )
   {
      ITERA_GGROUP_LESSPTR( itCampus, problemData->campi, Campus )
      {
         ITERA_GGROUP_LESSPTR( itUnidade, itCampus->unidades, Unidade )
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
                  vHash[v] = lp->getNumCols();

                  OPT_COL col( OPT_COL::VAR_BINARY,
                     0.0, 0.0, 1.0, ( char* )v.toString().c_str() );

                  lp->newCol( col );

                  num_vars += 1;
               }
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
variável de folga para a restrição "Evitar alocação do mesmo 
bloco curricular em tipos de salas diferentes".

%ObjCoef
\tau \cdot \sum\limits_{bc \in B} \sum\limits_{tps \in SCAP_{u}} fn_{bc,tps}

%Data \tau
%Desc
peso associado a função objetivo.

%DocEnd
/====================================================================*/

int SolverMIP::cria_variavel_de_folga_abertura_bloco_mesmoTPS()
{
   int num_vars = 0;

   ITERA_GGROUP_LESSPTR( itBloco, problemData->blocos, BlocoCurricular )
   {
      ITERA_GGROUP_LESSPTR( itCampus, problemData->campi, Campus )
      {
         ITERA_GGROUP_LESSPTR( itUnidade, itCampus->unidades, Unidade )
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
   }

   return num_vars;
}

// ==============================================================
//							CONSTRAINTS
// ==============================================================

int SolverMIP::cria_restricoes( void )
{
   int restricoes = 0;

#ifdef PRINT_cria_restricoes
   int numRestAnterior = 0;
#endif

   restricoes += cria_restricao_carga_horaria();				// Restricao 1.2.2

#ifdef PRINT_cria_restricoes
   std::cout << "numRest \"1.2.2\": " << (restricoes - numRestAnterior) << std::endl;
   numRestAnterior = restricoes;
#endif

   restricoes += cria_restricao_max_cred_sd();					// Restricao 1.2.3

#ifdef PRINT_cria_restricoes
   std::cout << "numRest \"1.2.3\": " << (restricoes - numRestAnterior) << std::endl;
   numRestAnterior = restricoes;
#endif

   restricoes += cria_restricao_min_cred_dd();					// Restricao 1.2.4

#ifdef PRINT_cria_restricoes
   std::cout << "numRest \"1.2.4\": " << (restricoes - numRestAnterior) << std::endl;
   numRestAnterior = restricoes;
#endif

   restricoes += cria_restricao_ativacao_var_o();					// Restricao 1.2.5

#ifdef PRINT_cria_restricoes
   std::cout << "numRest \"1.2.5\": " << (restricoes - numRestAnterior) << std::endl;
   numRestAnterior = restricoes;
#endif

   restricoes += cria_restricao_evita_sobreposicao();			// Restricao 1.2.6

#ifdef PRINT_cria_restricoes
   std::cout << "numRest \"1.2.6\": " << (restricoes - numRestAnterior) << std::endl;
   numRestAnterior = restricoes;
#endif

   restricoes += cria_restricao_disciplina_sala();				// Restricao 1.2.7

#ifdef PRINT_cria_restricoes
   std::cout << "numRest \"1.2.7\": " << (restricoes - numRestAnterior) << std::endl;
   numRestAnterior = restricoes;
#endif

   restricoes += cria_restricao_turma_sala();					// Restricao 1.2.8

#ifdef PRINT_cria_restricoes
   std::cout << "numRest \"1.2.8\": " << (restricoes - numRestAnterior) << std::endl;
   numRestAnterior = restricoes;
#endif

   restricoes += cria_restricao_evita_turma_disc_camp_d();		// Restricao 1.2.9

#ifdef PRINT_cria_restricoes
   std::cout << "numRest \"1.2.9\": " << (restricoes - numRestAnterior) << std::endl;
   numRestAnterior = restricoes;
#endif

   restricoes += cria_restricao_turmas_bloco();				// Restricao 1.2.10

#ifdef PRINT_cria_restricoes
   std::cout << "numRest \"1.2.10\": " << (restricoes - numRestAnterior) << std::endl;
   numRestAnterior = restricoes;
#endif

   restricoes += cria_restricao_max_cred_disc_bloco();			// Restricao 1.2.11

#ifdef PRINT_cria_restricoes
   std::cout << "numRest \"1.2.11\": " << (restricoes - numRestAnterior) << std::endl;
   numRestAnterior = restricoes;
#endif

   restricoes += cria_restricao_num_tur_bloc_dia_difunid();	// Restricao 1.2.12

#ifdef PRINT_cria_restricoes
   std::cout << "numRest \"1.2.12\": " << (restricoes - numRestAnterior) << std::endl;
   numRestAnterior = restricoes;
#endif

   restricoes += cria_restricao_lim_cred_diar_disc();			// Restricao 1.2.13

#ifdef PRINT_cria_restricoes
   std::cout << "numRest \"1.2.13\": " << (restricoes - numRestAnterior) << std::endl;
   numRestAnterior = restricoes;
#endif

   restricoes += cria_restricao_cap_aloc_dem_disc();			// Restricao 1.2.14

#ifdef PRINT_cria_restricoes
   std::cout << "numRest \"1.2.14\": " << (restricoes - numRestAnterior) << std::endl;
   numRestAnterior = restricoes;
#endif

   restricoes += cria_restricao_cap_sala_compativel_turma();	// Restricao 1.2.15

#ifdef PRINT_cria_restricoes
   std::cout << "numRest \"1.2.15\": " << (restricoes - numRestAnterior) << std::endl;
   numRestAnterior = restricoes;
#endif

   restricoes += cria_restricao_cap_sala_unidade();			// Restricao 1.2.16

#ifdef PRINT_cria_restricoes
   std::cout << "numRest \"1.2.16\": " << (restricoes - numRestAnterior) << std::endl;
   numRestAnterior = restricoes;
#endif

   //restricoes += cria_restricao_turma_disc_dias_consec();		// Restricao 1.2.17

#ifdef PRINT_cria_restricoes
   //std::cout << "numRest \"1.2.17\": " << (restricoes - numRestAnterior) << std::endl;
   std::cout << "numRest \"1.2.17\": NAO ESTA SENDO CRIADA DEVIDO A ERROS DE IMPLEMENTACAO - VER ToDo 12 (MARIO)" << std::endl;
   numRestAnterior = restricoes;
#endif

   restricoes += cria_restricao_min_creds_turm_bloco();		// Restricao 1.2.18

#ifdef PRINT_cria_restricoes
   std::cout << "numRest \"1.2.18\": " << (restricoes - numRestAnterior) << std::endl;
   numRestAnterior = restricoes;
#endif

   restricoes += cria_restricao_max_creds_turm_bloco();		// Restricao 1.2.19

#ifdef PRINT_cria_restricoes
   std::cout << "numRest \"1.2.19\": " << (restricoes - numRestAnterior) << std::endl;
   numRestAnterior = restricoes;
#endif

   restricoes += cria_restricao_aluno_curso_disc();			// Restricao 1.2.20

#ifdef PRINT_cria_restricoes
   std::cout << "numRest \"1.2.20\": " << (restricoes - numRestAnterior) << std::endl;
   numRestAnterior = restricoes;
#endif

   restricoes += cria_restricao_alunos_cursos_dif();			// Restricao 1.2.21

#ifdef PRINT_cria_restricoes
   std::cout << "numRest \"1.2.21\": " << (restricoes - numRestAnterior) << std::endl;
   //std::cout << "numVars \"1.2.21\": NAO ESTA SENDO CRIADA DEVIDO A ERROS DE IMPLEMENTACAO (A Inst. UNI-BH nao precisa dessa restricao implementada)." << std::endl;
   numRestAnterior = restricoes;
#endif

   restricoes += cria_restricao_de_folga_dist_cred_dia();		// Restricao 1.2.22

#ifdef PRINT_cria_restricoes
   std::cout << "numRest \"1.2.22\": " << (restricoes - numRestAnterior) << std::endl;
   //std::cout << "numRest \"1.2.22\": NAO ESTA SENDO CRIADA DEVIDO A NOVA MODELAGEM QUE O MARCELO FEZ." << std::endl;
   numRestAnterior = restricoes;
#endif

   restricoes += cria_restricao_ativacao_var_r();						// Restricao 1.2.23

#ifdef PRINT_cria_restricoes
   std::cout << "numRest \"1.2.23\": " << (restricoes - numRestAnterior) << std::endl;
   numRestAnterior = restricoes;
#endif

   restricoes += cria_restricao_limita_abertura_turmas();						// Restricao 1.2.24

#ifdef PRINT_cria_restricoes
   std::cout << "numRest \"1.2.24\": " << (restricoes - numRestAnterior) << std::endl;
   numRestAnterior = restricoes;
#endif

   restricoes += cria_restricao_abre_turmas_em_sequencia();						// Restricao 1.2.25

#ifdef PRINT_cria_restricoes
   std::cout << "numRest \"1.2.25\": " << (restricoes - numRestAnterior) << std::endl;
   numRestAnterior = restricoes;
#endif

   restricoes += cria_restricao_divisao_credito();

#ifdef PRINT_cria_restricoes
   std::cout << "numRest \"1.2.26\": " << (restricoes - numRestAnterior) << std::endl;
   numRestAnterior = restricoes;
#endif

   restricoes += cria_restricao_combinacao_divisao_credito();

#ifdef PRINT_cria_restricoes
   std::cout << "numRest \"1.2.27\": " << (restricoes - numRestAnterior) << std::endl;
   numRestAnterior = restricoes;
#endif

   restricoes += cria_restricao_ativacao_var_y();

#ifdef PRINT_cria_restricoes
   std::cout << "numRest \"1.2.28\": " << (restricoes - numRestAnterior) << std::endl;
   numRestAnterior = restricoes;
#endif

   restricoes += cria_restricao_max_creds_disc_dia();

#ifdef PRINT_cria_restricoes
   std::cout << "numRest \"1.2.29\": " << (restricoes - numRestAnterior) << std::endl;
   numRestAnterior = restricoes;
#endif

   restricoes += cria_restricao_max_creds_bloco_dia();

#ifdef PRINT_cria_restricoes
   std::cout << "numRest \"1.2.30\": " << (restricoes - numRestAnterior) << std::endl;
   numRestAnterior = restricoes;
#endif

   restricoes += cria_restricao_ativacao_var_zc();

#ifdef PRINT_cria_restricoes
   std::cout << "numRest \"1.2.31\": " << (restricoes - numRestAnterior) << std::endl;
   numRestAnterior = restricoes;
#endif

   restricoes +=  cria_restricao_disciplinas_incompativeis();

#ifdef PRINT_cria_restricoes
   std::cout << "numRest \"1.2.32\": " << (restricoes - numRestAnterior) << std::endl;
   numRestAnterior = restricoes;
#endif

   //restricoes +=  cria_restricao_abertura_bloco_mesmoTPS();

#ifdef PRINT_cria_restricoes
   //std::cout << "numRest \"1.2.33\": " << (restricoes - numRestAnterior) << std::endl;
   std::cout << "numRest \"1.2.33\": NAO ESTA SENDO CRIADA DEVIDO A ERRO DE MODELAGEM - (MARIO)" << std::endl;
   numRestAnterior = restricoes;
#endif

   //restricoes +=  cria_restricao_folga_abertura_bloco_mesmoTPS();

#ifdef PRINT_cria_restricoes
   //std::cout << "numRest \"1.2.34\": " << (restricoes - numRestAnterior) << std::endl;
   std::cout << "numRest \"1.2.34\": NAO ESTA SENDO CRIADA DEVIDO A ERRO DE MODELAGEM DA RESTRICAO 1.2.33 - (MARIO)" << std::endl;
   numRestAnterior = restricoes;
#endif

   return restricoes;
}

/*====================================================================/
%DocBegin TRIEDA_LOAD_MODEL

%Constraint 
Carga horária de todas as turmas de cada disciplina

%MatExp 

\begin{eqnarray}
\sum\limits_{u \in U}\sum\limits_{tps \in SCAP_{u}}\sum\limits_{t \in T} x_{i,d,u,tps,t}  =  C_{d} \cdot z_{i,d,cp} \nonumber \qquad 
\forall d \in D \quad
\forall i \in I_{d} \quad 
\forall cp \in CP 
\end{eqnarray}

%Data C_{d}
%Desc
Total de créditos da disciplina $d$.

%Data I_{d}
%Desc
Máximo de turmas que podem ser abertas da disciplina $d$.

%DocEnd
/====================================================================*/

int SolverMIP::cria_restricao_carga_horaria(void)
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
      ITERA_GGROUP_LESSPTR( it_disciplina, problemData->disciplinas, Disciplina )
      {
         disciplina = ( *it_disciplina );

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
Máximo de créditos por sala e dia
%Desc 

%MatExp

\begin{eqnarray}
\sum\limits_{d \in D}\sum\limits_{i \in I_{d}} x_{i,d,u,tps,t} \leq  HTPS_{t,tps} \nonumber \qquad 
\forall u \in U \quad
\forall tps \in SCAP_{u} \quad
\forall t \in T
\end{eqnarray}

%Data HTPS_{t,tps}
%Desc
máximo de créditos permitidos por dia $t$ para o conjunto de salas do tipo (capacidade) $tps$.

%DocEnd
/====================================================================*/

int SolverMIP::cria_restricao_max_cred_sd( void )
{
   int restricoes = 0;

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
      ITERA_GGROUP_LESSPTR( itUnidade, itCampus->unidades, Unidade )
      {
         ITERA_GGROUP_LESSPTR( itCjtSala, itUnidade->conjutoSalas, ConjuntoSala )
         {
            //////////////////////////////////////////////////////////////////////
            // Percorre cada disciplina do conjunto de salas
            std::map< Disciplina *, GGroup< int > >::iterator
               it_disc_dias = itCjtSala->dias_letivos_disciplinas.begin();

            for (; it_disc_dias != itCjtSala->dias_letivos_disciplinas.end();
               it_disc_dias++ )
            {
               // Verifica se a disciplina foi substituída
               disciplina = it_disc_dias->first;

               std::pair< Curso *, Curriculo * > curso_curriculo
                  = problemData->map_Disc_CursoCurriculo[ disciplina ];
               curso = curso_curriculo.first;
               curriculo = curso_curriculo.second;

               if ( problemData->retornaDisciplinaSubstituta( curso, curriculo, disciplina ) != NULL )
               {
                  // Como a disciplina foi substituída,
                  // não percorremos os dias letivos da mesma
                  continue;
               }

               GGroup< int >::iterator itDiasLetCjtSala =
                  it_disc_dias->second.begin();

               for (; itDiasLetCjtSala != it_disc_dias->second.end();
                  itDiasLetCjtSala++ )
               {
                  c.reset();
                  c.setType( Constraint::C_MAX_CREDITOS_SD );

                  c.setUnidade( *itUnidade );
                  c.setSubCjtSala( *itCjtSala );
                  c.setDia( *itDiasLetCjtSala );

                  sprintf( name, "%s", c.toString().c_str() );
                  if ( cHash.find(c) != cHash.end() )
                  {
                     continue;
                  }

                  nnz = 0;
                  ITERA_GGROUP_LESSPTR( itDisc, problemData->disciplinas, Disciplina )
                  {
                     nnz += ( itDisc->getNumTurmas() );
                  }

                  int maxCreds = itCjtSala->maxCredsPermitidos( *itDiasLetCjtSala );
                  OPT_ROW row( nnz, OPT_ROW::LESS , maxCreds, name );

                  ITERA_GGROUP_LESSPTR( itDisc, problemData->disciplinas, Disciplina )
                  {
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
Mínimo de créditos diários da disciplina (*)
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
mínimo de créditos diários da disciplina $d$.

%DocEnd
/====================================================================*/

// TRIEDA-405 Cont-Mínimo de créditos diários da disciplina(*)
int SolverMIP::cria_restricao_min_cred_dd(void)
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
      ITERA_GGROUP_LESSPTR( itUnidade, itCampus->unidades, Unidade )
      {
         ITERA_GGROUP_LESSPTR( itCjtSala, itUnidade->conjutoSalas, ConjuntoSala )
         {
            ITERA_GGROUP_LESSPTR( it_disciplina, problemData->disciplinas, Disciplina )
            {
               disciplina = ( *it_disciplina );

               std::pair< Curso *, Curriculo * > curso_curriculo
                  = problemData->map_Disc_CursoCurriculo[ disciplina ];
               curso = curso_curriculo.first;
               curriculo = curso_curriculo.second;

               disciplina_equivalente = problemData->retornaDisciplinaSubstituta( curso, curriculo, disciplina );
               if ( disciplina_equivalente != NULL )
               {
                  disciplina = disciplina_equivalente;
               }

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
                     // Mínimo de créditos diários da disciplina (*)
                     int min_creditos = disciplina->getMinCreds();
                     int min_creditos_fixacao
                        = problemData->creditosFixadosDisciplinaDia( disciplina, *itDiscSala_Dias, *itCjtSala );

                     // Quando existir uma fixação, o mínimo de créditos
                     // deverá ser exatamente a fixação. Caso contrário,
                     // continua sendo o mínimo de créditos da disciplina
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
Ativação da variável $o$
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
Total de créditos da disciplina $d$.

%DocEnd
/====================================================================*/

int SolverMIP::cria_restricao_ativacao_var_o(void)
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
      ITERA_GGROUP_LESSPTR( itUnidade, itCampus->unidades, Unidade )
      {
         ITERA_GGROUP_LESSPTR( itCjtSala, itUnidade->conjutoSalas, ConjuntoSala )
         {
            ITERA_GGROUP_LESSPTR( it_disciplina, problemData->disciplinas, Disciplina )
            {
               disciplina = ( *it_disciplina );

               std::pair< Curso *, Curriculo * > curso_curriculo
                  = problemData->map_Disc_CursoCurriculo[ disciplina ];
               curso = curso_curriculo.first;
               curriculo = curso_curriculo.second;

               disciplina_equivalente = problemData->retornaDisciplinaSubstituta( curso, curriculo, disciplina );
               if ( disciplina_equivalente != NULL )
               {
                  disciplina = disciplina_equivalente;
               }

               GGroup< int /*Dias*/ >::iterator itDiscSala_Dias =
                  problemData->disc_Conjutno_Salas__Dias
                  [ std::make_pair< int, int > ( disciplina->getId(), itCjtSala->getId() ) ].begin();
               for(; itDiscSala_Dias != problemData->disc_Conjutno_Salas__Dias[
                  std::make_pair< int, int > ( disciplina->getId(), itCjtSala->getId() ) ].end(); itDiscSala_Dias++ )
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

                        OPT_ROW row( nnz, OPT_ROW::GREATER , 0.0, name );

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
                              ( disciplina->getCredPraticos() + 
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
Evitar sobreposição de turmas da mesma disciplina
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

int SolverMIP::cria_restricao_evita_sobreposicao(void)
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
      ITERA_GGROUP_LESSPTR( itUnidade, itCampus->unidades, Unidade )
      {
         GGroup< int >::iterator itDiasLetUnid =
            itUnidade->dias_letivos.begin();

         for (; itDiasLetUnid != itUnidade->dias_letivos.end(); itDiasLetUnid++ )
         {
            ITERA_GGROUP_LESSPTR( it_disciplina, problemData->disciplinas, Disciplina )
            {
               disciplina = ( *it_disciplina );

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
Indicação de que uma turma de uma disciplina foi alocada 
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

int SolverMIP::cria_restricao_disciplina_sala(void)
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
      ITERA_GGROUP_LESSPTR( itUnidade, itCampus->unidades, Unidade )
      {
         ITERA_GGROUP_LESSPTR( itCjtSala, itUnidade->conjutoSalas, ConjuntoSala )
         {
            ITERA_GGROUP_LESSPTR( it_disciplina, problemData->disciplinas, Disciplina )
            {
               disciplina = ( *it_disciplina );

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
Uma turma só pode ser alocada a um tipo de sala
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
int SolverMIP::cria_restricao_turma_sala(void)
{
   int restricoes = 0;
   char name[100];
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

      std::pair< Curso *, Curriculo * > curso_curriculo
         = problemData->map_Disc_CursoCurriculo[ disciplina ];
      curso = curso_curriculo.first;
      curriculo = curso_curriculo.second;

      disciplina_equivalente = problemData->retornaDisciplinaSubstituta( curso, curriculo, disciplina );
      if ( disciplina_equivalente != NULL )
      {
         disciplina = disciplina_equivalente;
      }

      for (int turma = 0; turma < disciplina->getNumTurmas(); turma++ )
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
Evitar alocação de turmas da mesma disciplina em campus diferentes
%Desc 

%MatExp

\begin{eqnarray}
\sum\limits_{cp \in CP} z_{i,d,cp}  \leq  1  \nonumber \qquad 
\forall d \in D \quad
\forall i \in I_{d}
\end{eqnarray}

%DocEnd
/====================================================================*/

int SolverMIP::cria_restricao_evita_turma_disc_camp_d(void)
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

      std::pair< Curso *, Curriculo * > curso_curriculo
         = problemData->map_Disc_CursoCurriculo[ disciplina ];
      curso = curso_curriculo.first;
      curriculo = curso_curriculo.second;

      disciplina_equivalente = problemData->retornaDisciplinaSubstituta( curso, curriculo, disciplina );
      if ( disciplina_equivalente != NULL )
      {
         disciplina = disciplina_equivalente;
      }

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

int SolverMIP::cria_restricao_turmas_bloco(void)
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
      GGroup< int >::iterator itDiasLetCampus =
         itCampus->diasLetivos.begin();

      for(; itDiasLetCampus != itCampus->diasLetivos.end();
         itDiasLetCampus++ )
      {
         ITERA_GGROUP_LESSPTR( itBloco, problemData->blocos, BlocoCurricular )
         {
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

               std::pair< Curso *, Curriculo * > curso_curriculo
                  = problemData->map_Disc_CursoCurriculo[ disciplina ];
               curso = curso_curriculo.first;
               curriculo = curso_curriculo.second;

               disciplina_equivalente = problemData->retornaDisciplinaSubstituta( curso, curriculo, disciplina );
               if ( disciplina_equivalente != NULL )
               {
                  disciplina = disciplina_equivalente;
               }

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
            // Provavelmente esta restrição é inútil

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
Disciplinas de mesmo bloco não devem exceder máximo de créditos por dia
%Desc 

%MatExp

\begin{eqnarray}
\sum\limits_{d \in D_{bc}} \sum\limits_{u \in U} \sum\limits_{tps \in SCAP_{u}} \sum\limits_{i \in I_{d}} x_{i,d,u,tps,t}  \leq H_{t} \cdot w_{bc,t,cp}  \nonumber \qquad 
\forall bc \in B \quad
\forall cp \in CP
\forall t \in T \quad
\end{eqnarray}

%Data H_{t}
%Desc
máximo de créditos permitidos por dia $t$.

%DocEnd
/====================================================================*/

int SolverMIP::cria_restricao_max_cred_disc_bloco(void)
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
      GGroup< int >::iterator itDiasLetCampus =
         itCampus->diasLetivos.begin();

      for (; itDiasLetCampus != itCampus->diasLetivos.end(); itDiasLetCampus++ )
      {
         ITERA_GGROUP_LESSPTR( itBloco, problemData->blocos, BlocoCurricular )
         {
            c.reset();
            c.setType( Constraint::C_MAX_CRED_DISC_BLOCO );

            c.setBloco( *itBloco );
            c.setDia( *itDiasLetCampus );
            c.setCampus( *itCampus );

            sprintf( name, "%s", c.toString().c_str() ); 
            if ( cHash.find( c ) != cHash.end() )
            {
               continue;
            }

            nnz =  100; /*FIX-ME*/

            OPT_ROW row( nnz + 1, OPT_ROW::LESS , 0.0, name );

            // Variavel responsavel por armazenar o maximo de
            // creditos disponiveis  dentre todas as salas para um dado dia.
            int maxCredsSalaDia = 0;
            int maxCredsProfDia = 0;
            ITERA_GGROUP_LESSPTR( it_disciplina, itBloco->disciplinas, Disciplina )
            {
               disciplina = ( *it_disciplina );

               std::pair< Curso *, Curriculo * > curso_curriculo
                  = problemData->map_Disc_CursoCurriculo[ disciplina ];
               curso = curso_curriculo.first;
               curriculo = curso_curriculo.second;

               disciplina_equivalente = problemData->retornaDisciplinaSubstituta( curso, curriculo, disciplina );
               if ( disciplina_equivalente != NULL )
               {
                  disciplina = disciplina_equivalente;
               }

               ITERA_GGROUP_LESSPTR( itUnidade, itCampus->unidades, Unidade )
               {
                  ITERA_GGROUP_LESSPTR( itCjtSala, itUnidade->conjutoSalas, ConjuntoSala )
                  {
                     for ( int turma = 0; turma < disciplina->getNumTurmas(); turma++ )
                     {
                        v.reset();
                        v.setType( Variable::V_CREDITOS );

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

                        /////////////////////////////////////////////////////////////////////////////////////////////////
                        // Percorre os dias letivos das
                        // disciplinas pertencentes ao conjunto sala
                        int cred_dia = 0;
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
                              if ( ( *itDiasLetCampus ) == ( *itDiasLetCjtSala ) )
                              {
                                 cred_dia = itCjtSala->maxCredsDia( *itDiasLetCjtSala );
                                 maxCredsSalaDia = ( maxCredsSalaDia < cred_dia ?
cred_dia : maxCredsSalaDia );
                              }
                           }
                        }
                        /////////////////////////////////////////////////////////////////////////////////////////////////
                     }
                  }
               }
            }

            v.reset();
            v.setType( Variable::V_N_SUBBLOCOS );

            v.setBloco( *itBloco );
            v.setDia( *itDiasLetCampus );
            v.setCampus( *itCampus );

            int H_t = maxCredsSalaDia;
            it_v = vHash.find( v );
            if( it_v != vHash.end() )
            {
               row.insert( it_v->second, -H_t );
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
Contabiliza se há turmas do mesmo bloco curricular abertas no mesmo 
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

int SolverMIP::cria_restricao_num_tur_bloc_dia_difunid(void)
{
   int restricoes = 0;
   char name[ 100 ];
   int nnz;

   Variable v;
   Constraint c;
   VariableHash::iterator it_v;

   ITERA_GGROUP_LESSPTR( it_bloco, problemData->blocos, BlocoCurricular )
   {
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
Limite de créditos diários de disciplina (*)
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
máximo de créditos diários da disciplina $d$.

%DocEnd
/====================================================================*/

// TRIEDA-406 Cont-Limite de créditos diários de disciplina
int SolverMIP::cria_restricao_lim_cred_diar_disc(void)
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
      ITERA_GGROUP_LESSPTR( itUnidade, itCampus->unidades, Unidade )
      {
         ITERA_GGROUP_LESSPTR( itCjtSala, itUnidade->conjutoSalas, ConjuntoSala )
         {
            ITERA_GGROUP_LESSPTR( it_disciplina, problemData->disciplinas, Disciplina )
            {
               disciplina = ( *it_disciplina );

               std::pair< Curso *, Curriculo * > curso_curriculo
                  = problemData->map_Disc_CursoCurriculo[ disciplina ];
               curso = curso_curriculo.first;
               curriculo = curso_curriculo.second;

               disciplina_equivalente = problemData->retornaDisciplinaSubstituta( curso, curriculo, disciplina );
               if ( disciplina_equivalente != NULL )
               {
                  disciplina = disciplina_equivalente;
               }

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
                     // Limite de créditos diários de disciplina (*)
                     int maximo_creditos = itCjtSala->maxCredsDia( *itDiscSala_Dias );
                     int maximo_creditos_fixacao
                        = problemData->creditosFixadosDisciplinaDia( disciplina, *itDiscSala_Dias, *itCjtSala );

                     // Quando existir uma fixação, o máximo de créditos deverá
                     // ser exatamente a fixação. Caso contrário, continua sendo
                     // o máximo de créditos por dia do conjunto de salas
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

int SolverMIP::cria_restricao_cap_aloc_dem_disc(void)
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

   ITERA_GGROUP_LESSPTR( itOferta, problemData->ofertas, Oferta )
   {
      GGroup< std::pair< int, Disciplina * > >::iterator itPrdDisc = 
         itOferta->curriculo->disciplinas_periodo.begin();

      for (; itPrdDisc != itOferta->curriculo->disciplinas_periodo.end(); itPrdDisc++ )
      {
         disciplina = ( *itPrdDisc ).second;

         std::pair< Curso *, Curriculo * > curso_curriculo
            = problemData->map_Disc_CursoCurriculo[ disciplina ];
         curso = curso_curriculo.first;
         curriculo = curso_curriculo.second;

         disciplina_equivalente = problemData->retornaDisciplinaSubstituta( curso, curriculo, disciplina );
         if ( disciplina_equivalente != NULL )
         {
            disciplina = disciplina_equivalente;
         }

         c.reset();
         c.setType( Constraint::C_CAP_ALOC_DEM_DISC );

         c.setOferta( *itOferta );
         c.setDisciplina( disciplina );

         sprintf( name, "%s", c.toString().c_str() ); 
         if ( cHash.find( c ) != cHash.end() )
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
int SolverMIP::cria_restricao_cap_sala_compativel_turma(void)
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
      ITERA_GGROUP_LESSPTR( it_disciplina, problemData->disciplinas, Disciplina )
      {
         disciplina = ( *it_disciplina );

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
int SolverMIP::cria_restricao_cap_sala_unidade(void)
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
      ITERA_GGROUP_LESSPTR( itUnidade, itCampus->unidades, Unidade)
      {
         ITERA_GGROUP_LESSPTR( itCjtSala, itUnidade->conjutoSalas, ConjuntoSala )
         {
            ITERA_GGROUP_LESSPTR( it_disciplina, problemData->disciplinas, Disciplina )
            {
               disciplina = ( *it_disciplina );

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

                     GGroup< Oferta *, LessPtr< Oferta > > group_ofertas
                        = problemData->ofertasDisc[ disciplina->getId() ];
                     ITERA_GGROUP_LESSPTR( itOferta, group_ofertas, Oferta )
                     {
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
Contabiliza se há turmas da mesma disciplina em dias consecutivos (*)
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
int SolverMIP::cria_restricao_turma_disc_dias_consec(void)
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
Mínimo de créditos alocados para turmas de um bloco (*)
%Desc 

%MatExp

\begin{eqnarray}
\underline{h}_{bc,i} \leq \sum\limits_{d \in D_{bc}} \sum\limits_{u \in U} \sum\limits_{tps \in SCAP_{u}} x_{i,d,u,tps,t} \nonumber \qquad 
\forall bc \in B \quad
\forall i \in I_{oft} \quad
\forall t \in T
\end{eqnarray}

%DocEnd
/====================================================================*/

// TRIEDA-393
int SolverMIP::cria_restricao_min_creds_turm_bloco(void)
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

               std::pair< Curso *, Curriculo * > curso_curriculo
                  = problemData->map_Disc_CursoCurriculo[ disciplina ];
               curso = curso_curriculo.first;
               curriculo = curso_curriculo.second;

               disciplina_equivalente = problemData->retornaDisciplinaSubstituta( curso, curriculo, disciplina );
               if ( disciplina_equivalente != NULL )
               {
                  disciplina = disciplina_equivalente;
               }

               ITERA_GGROUP_LESSPTR( itUnidade, itBloco->campus->unidades, Unidade )
               {
                  ITERA_GGROUP_LESSPTR( itCjtSala, itUnidade->conjutoSalas, ConjuntoSala )
                  {
                     v.reset();
                     v.setType( Variable::V_CREDITOS );

                     v.setTurma( turma );
                     v.setDisciplina( disciplina );
                     v.setUnidade( *itUnidade );
                     v.setSubCjtSala( *itCjtSala );
                     v.setDia( *itDiasLetivosBlocoCurric );

                     it_v = vHash.find( v );
                     if( it_v != vHash.end() )
                     {
                        row.insert( it_v->second, 1.0 );
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

            // Para evitar a criação da restrição no caso em que só a variável h seja encontrada. Isso é só uma
            // garantia. Como os dias letivos estão sendo respeitados, não devemos notar erros.
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
Máximo de créditos alocados para turmas de um bloco (*)
%Desc 

%MatExp

\begin{eqnarray}
\overline{h}_{bc,i} \geq \sum\limits_{d \in D_{bc}} \sum\limits_{u \in U} \sum\limits_{tps \in SCAP_{u}} x_{i,d,u,tps,t} \nonumber \qquad 
\forall bc \in B \quad
\forall i \in I_{oft} \quad
\forall t \in T
\end{eqnarray}

%DocEnd
/====================================================================*/

// TRIEDA-394
int SolverMIP::cria_restricao_max_creds_turm_bloco(void)
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

               std::pair< Curso *, Curriculo * > curso_curriculo
                  = problemData->map_Disc_CursoCurriculo[ disciplina ];
               curso = curso_curriculo.first;
               curriculo = curso_curriculo.second;

               disciplina_equivalente = problemData->retornaDisciplinaSubstituta( curso, curriculo, disciplina );
               if ( disciplina_equivalente != NULL )
               {
                  disciplina = disciplina_equivalente;
               }

               ITERA_GGROUP_LESSPTR( itUnidade, itBloco->campus->unidades, Unidade )
               {
                  ITERA_GGROUP_LESSPTR( itCjtSala, itUnidade->conjutoSalas, ConjuntoSala )
                  {
                     v.reset();
                     v.setType( Variable::V_CREDITOS );

                     v.setTurma( turma );
                     v.setDisciplina( disciplina );
                     v.setUnidade( *itUnidade );
                     v.setSubCjtSala( *itCjtSala );
                     v.setDia( *itDiasLetivosBlocoCurric );

                     it_v = vHash.find( v );
                     if ( it_v != vHash.end() )
                     {
                        row.insert( it_v->second, 1.0 );
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

            // Para evitar a criação da restrição no caso em que só a
            // variável h seja encontrada. Isso é só uma garantia.
            // Como os dias letivos estão sendo respeitados, não devemos notar erros.
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

int SolverMIP::cria_restricao_aluno_curso_disc(void)
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
      ITERA_GGROUP_LESSPTR( itCurso, problemData->cursos, Curso )
      {
         ITERA_GGROUP_LESSPTR( it_disciplina, problemData->disciplinas, Disciplina )
         {
            disciplina = ( *it_disciplina );

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
Não permitir que alunos de cursos diferentes compartilhem turmas (*)
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

int SolverMIP::cria_restricao_alunos_cursos_dif(void)
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
      ITERA_GGROUP_LESSPTR( it_disciplina, problemData->disciplinas, Disciplina )
      {
         disciplina = ( *it_disciplina );

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
            std::map< std::pair< Curso *, Curso * >, std::vector< int > >::iterator
               it_cursoComp_disc = problemData->cursosComp_disc.begin();

            for (; it_cursoComp_disc != problemData->cursosComp_disc.end(); it_cursoComp_disc++ )
            {
               std::vector< int >::iterator it_disc
                  = it_cursoComp_disc->second.begin();

               for (; it_disc != it_cursoComp_disc->second.end(); ++it_disc )
               {
                  Disciplina * nova_disc = new Disciplina();
                  nova_disc->setId( *it_disc );

                  c.reset();
                  c.setType( Constraint::C_ALUNOS_CURSOS_DIF );

                  c.setCampus( *itCampus );
                  c.setCurso( it_cursoComp_disc->first.first );
                  c.setCursoIncompat( it_cursoComp_disc->first.second );
                  c.setDisciplina( nova_disc );
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
                  v.setDisciplina( nova_disc );
                  v.setCurso( it_cursoComp_disc->first.first );
                  v.setCampus( *itCampus );

                  it_v = vHash.find( v );
                  if( it_v != vHash.end() )
                  {
                     row.insert( it_v->second, 1 );
                  }

                  v.reset();
                  v.setType( Variable::V_ALOC_ALUNO );

                  v.setTurma( turma );
                  v.setDisciplina( nova_disc );
                  v.setCurso( it_cursoComp_disc->first.second );
                  v.setCampus( *itCampus );

                  it_v = vHash.find( v );
                  if( it_v != vHash.end() )
                  {
                     row.insert(it_v->second, 1);
                  }

                  v.reset();
                  v.setType( Variable::V_SLACK_ALOC_ALUNOS_CURSO_INCOMPAT );

                  v.setTurma( turma );
                  v.setDisciplina( nova_disc );
                  v.setCurso( it_cursoComp_disc->first.first );
                  v.setCursoIncompat( it_cursoComp_disc->first.second );
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
Fixação da distribuição de créditos por dia (*)
%Desc 

%MatExp

\begin{eqnarray}
\sum\limits_{u \in U} x_{i,d,u,tps,t} + fcp_{i,d,tps,t} - fcm_{i,d,tps,t} = FC_{d,tps,t} \cdot z_{i,d,cp}  \nonumber \qquad 
\forall cp \in CP
\forall tps \in SCAP_{u} \quad
\forall d \in D \quad
\forall i \in I_{d} \quad
\forall t \in T
\end{eqnarray}

%DocEnd
/====================================================================*/

// TRIEDA-395
int SolverMIP::cria_restricao_de_folga_dist_cred_dia(void)
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
      ITERA_GGROUP_LESSPTR( itUnidade, itCampus->unidades, Unidade )
      {
         ITERA_GGROUP_LESSPTR( itCjtSala, itUnidade->conjutoSalas, ConjuntoSala )
         {
            ITERA_GGROUP_LESSPTR( it_disciplina, problemData->disciplinas, Disciplina )
            {
               disciplina = ( *it_disciplina );

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
                  GGroup< int >::iterator itDiasLetDisc =
                     disciplina->diasLetivos.begin();

                  for (; itDiasLetDisc != disciplina->diasLetivos.end(); itDiasLetDisc++ )
                  {
                     ITERA_GGROUP_LESSPTR( it_fix, problemData->fixacoes, Fixacao )
                     {
                        if ( it_fix->getDisciplinaId() == disciplina->getId()
                           && it_fix->getDiaSemana() == ( *itDiasLetDisc ) )
                        {					
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

                           nnz = ( problemData->totalConjuntosSalas * disciplina->getNumTurmas() + 1 );
                           OPT_ROW row( nnz, OPT_ROW::EQUAL , 0.0 , name );

                           if ( it_fix->sala != NULL )
                           {
                              if ( itCjtSala->capTotalSalas() == it_fix->sala->getCapacidade() )
                              {
                                 ITERA_GGROUP_LESSPTR( itCampus1, problemData->campi, Campus )
                                 {
                                    if ( itCampus->getId() == itCampus1->getId() )
                                    {
                                       ITERA_GGROUP_LESSPTR( itUnidade1, itCampus->unidades, Unidade )
                                       {
                                          v.reset();
                                          v.setType( Variable::V_CREDITOS );

                                          v.setTurma( turma );
                                          v.setDisciplina( disciplina );
                                          v.setUnidade( *itUnidade1 );
                                          v.setSubCjtSala( *itCjtSala );
                                          v.setDia( *itDiasLetDisc );

                                          it_v = vHash.find(v);
                                          if( it_v != vHash.end() )
                                          {
                                             row.insert( it_v->second, 1.0 );
                                          }
                                       }
                                    }
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
                                       -( it_fix->disciplina->getMaxCreds() ) );
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
      }
   }

   return restricoes;
}

/*====================================================================/
%DocBegin TRIEDA_LOAD_MODEL

%Constraint 
Ativação da variável r
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

int SolverMIP::cria_restricao_ativacao_var_r()
{
   int restricoes = 0;
   char name[ 100 ];
   int nnz;

   Variable v;
   Constraint c;
   VariableHash::iterator it_v;

   ITERA_GGROUP_LESSPTR( it_bloco, problemData->blocos, BlocoCurricular )
   {
      GGroup< int >::iterator itDiasLetBloco =
         it_bloco->diasLetivos.begin();

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

int SolverMIP::cria_restricao_limita_abertura_turmas()
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
      ITERA_GGROUP_LESSPTR( it_disciplina, problemData->disciplinas, Disciplina )
      {
         disciplina = ( *it_disciplina );

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
               row.insert( it_v->second,
                  problemData->parametros->min_alunos_abertura_turmas );
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

int SolverMIP::cria_restricao_abre_turmas_em_sequencia(void)
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

   ITERA_GGROUP_LESSPTR( it_disciplina, problemData->disciplinas, Disciplina )
   {
      disciplina = ( *it_disciplina );

      std::pair< Curso *, Curriculo * > curso_curriculo
         = problemData->map_Disc_CursoCurriculo[ disciplina ];
      curso = curso_curriculo.first;
      curriculo = curso_curriculo.second;

      disciplina_equivalente = problemData->retornaDisciplinaSubstituta( curso, curriculo, disciplina );
      if ( disciplina_equivalente != NULL )
      {
         disciplina = disciplina_equivalente;
      }

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
Regra de divisão de créditos
%Desc 

%MatExp

\begin{eqnarray}
\sum\limits_{u \in U} \sum\limits_{tps \in SCAP_{u}} x_{i,d,u,tps,t} = \sum\limits_{k \in K_{d}}N_{d,k,t} \cdot m_{d,i,k} + fk_{d,i,t} \nonumber \qquad 
\forall d \in D \quad
\forall i \in I_{d} \quad
\forall t \in T
\end{eqnarray}

%Data N_{d,k,t}
%Desc
número de créditos determinados para a disciplina $d$ no dia $t$ na combinação de divisão de crédito $k$.

%DocEnd
/====================================================================*/

int SolverMIP::cria_restricao_divisao_credito()
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

      std::pair< Curso *, Curriculo * > curso_curriculo
         = problemData->map_Disc_CursoCurriculo[ disciplina ];
      curso = curso_curriculo.first;
      curriculo = curso_curriculo.second;

      disciplina_equivalente = problemData->retornaDisciplinaSubstituta( curso, curriculo, disciplina );
      if ( disciplina_equivalente != NULL )
      {
         disciplina = disciplina_equivalente;
      }

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

               nnz = ( problemData->totalSalas + ( disciplina->combinacao_divisao_creditos.size() * 2 ) );
               OPT_ROW row( nnz, OPT_ROW::EQUAL , 0.0 , name );

               ITERA_GGROUP_LESSPTR( itCampus, problemData->campi, Campus )
               {
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
                           v.setType( Variable::V_SLACK_COMBINACAO_DIVISAO_CREDITO );

                           v.setDisciplina( disciplina );
                           v.setTurma( turma );
                           v.setDia( *itDiasLetDisc );

                           it_v = vHash.find( v );
                           if( it_v != vHash.end() )
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
   }

   return restricoes;
}

/*====================================================================/
%DocBegin TRIEDA_LOAD_MODEL

%Constraint 
Somente uma combinação de regra de divisão de créditos pode ser escolhida
%Desc 

%MatExp

\begin{eqnarray}
\sum\limits_{k \in K_{d}} m_{d,i,k} \leq 1 \nonumber \qquad 
\forall d \in D \quad
\forall i \in I_{d}
\end{eqnarray}

%DocEnd
/====================================================================*/

int SolverMIP::cria_restricao_combinacao_divisao_credito(){

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

   ITERA_GGROUP_LESSPTR( it_disciplina, problemData->disciplinas, Disciplina )
   {
      disciplina = ( *it_disciplina );

      std::pair< Curso *, Curriculo * > curso_curriculo
         = problemData->map_Disc_CursoCurriculo[ disciplina ];
      curso = curso_curriculo.first;
      curriculo = curso_curriculo.second;

      disciplina_equivalente = problemData->retornaDisciplinaSubstituta( curso, curriculo, disciplina );
      if ( disciplina_equivalente != NULL )
      {
         disciplina = disciplina_equivalente;
      }

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

         nnz = disciplina->combinacao_divisao_creditos.size();
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
Ativação da variável y
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

int SolverMIP::cria_restricao_ativacao_var_y()
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
      ITERA_GGROUP_LESSPTR( itUnidade, itCampus->unidades, Unidade )
      {
         ITERA_GGROUP_LESSPTR( itCjtSala, itUnidade->conjutoSalas, ConjuntoSala )
         {
            ITERA_GGROUP_LESSPTR( it_disciplina, problemData->disciplinas, Disciplina )
            {
               disciplina = ( *it_disciplina );

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
Máximo de créditos diários da disciplina
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

int SolverMIP::cria_restricao_max_creds_disc_dia()
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

   ITERA_GGROUP_LESSPTR( it_bloco, problemData->blocos, BlocoCurricular )
   {
      ITERA_GGROUP_LESSPTR(it_disciplina,it_bloco->disciplinas,Disciplina)
      {
         disciplina = ( *it_disciplina );

         std::pair< Curso *, Curriculo * > curso_curriculo
            = problemData->map_Disc_CursoCurriculo[ disciplina ];
         curso = curso_curriculo.first;
         curriculo = curso_curriculo.second;
         disciplina_equivalente = problemData->retornaDisciplinaSubstituta( curso, curriculo, disciplina );
         if ( disciplina_equivalente != NULL )
         {
            disciplina = disciplina_equivalente;
         }
         GGroup< int >::iterator itDiasLetBloco =
            it_bloco->diasLetivos.begin();
         for (; itDiasLetBloco != it_bloco->diasLetivos.end();
            itDiasLetBloco++ )
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
                           // Testando se a dia (referenciado por <*it_Dias_Letivos>) é um dia 
                           // letivo comum à disciplina e o conjunto de salas em questão.
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
        

   /*ITERA_GGROUP_LESSPTR( it_disciplina, problemData->disciplinas, Disciplina )
   {
      disciplina = ( *it_disciplina );

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
         ITERA_GGROUP_N_PT( it_Dias_Letivos, disciplina->diasLetivos, int )
         {
            c.reset();
            c.setType( Constraint::C_MAX_CREDS_DISC_DIA );

            c.setDisciplina( disciplina );
            c.setTurma( turma );
            c.setDia( *it_Dias_Letivos );

            sprintf( name, "%s", c.toString().c_str() ); 
            if (cHash.find(c) != cHash.end())
            {
               continue;
            }

            nnz = 100;

            OPT_ROW row( nnz, OPT_ROW::LESS, 0.0, name );

            ITERA_GGROUP_LESSPTR( it_Campus, problemData->campi, Campus )
            {
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
                        // Testando se a dia (referenciado por <*it_Dias_Letivos>) é um dia 
                        // letivo comum à disciplina e o conjunto de salas em questão.
                        if ( it_Disc_Cjt_Salas__Dias->second.find( *it_Dias_Letivos ) != 
                           it_Disc_Cjt_Salas__Dias->second.end() )
                        {
                           v.reset();
                           v.setType( Variable::V_CREDITOS );

                           v.setTurma( turma );
                           v.setDisciplina( disciplina );
                           v.setUnidade( *it_Unidade );
                           v.setSubCjtSala( *it_Cjt_Sala );
                           v.setDia( *it_Dias_Letivos );

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
            v.setDia( *it_Dias_Letivos );

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
   }*/

   return restricoes;
}

/*====================================================================/
%DocBegin TRIEDA_LOAD_MODEL

%Constraint 
Máximo de créditos diários do bloco
%Desc

%MatExp
\begin{eqnarray}
\sum\limits_{d \in D_{bc}} xm_{bc,d, t} \leq 4 \nonumber \qquad 
\forall bc \in B \quad
\forall t \in T
\end{eqnarray}

%DocEnd
/====================================================================*/

int SolverMIP::cria_restricao_max_creds_bloco_dia()
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

   ITERA_GGROUP_LESSPTR( it_bloco, problemData->blocos, BlocoCurricular )
   {
      GGroup< int >::iterator itDiasLetBloco =
         it_bloco->diasLetivos.begin();
      for (; itDiasLetBloco != it_bloco->diasLetivos.end();
         itDiasLetBloco++ )
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
         double rhs = 4.0;

         OPT_ROW row( nnz, OPT_ROW::LESS, rhs, name );

         ITERA_GGROUP_LESSPTR(it_disciplina,it_bloco->disciplinas,Disciplina)
         {
            disciplina = ( *it_disciplina );

            std::pair< Curso *, Curriculo * > curso_curriculo
               = problemData->map_Disc_CursoCurriculo[ disciplina ];
            curso = curso_curriculo.first;
            curriculo = curso_curriculo.second;
            disciplina_equivalente = problemData->retornaDisciplinaSubstituta( curso, curriculo, disciplina );
            if ( disciplina_equivalente != NULL )
            {
               disciplina = disciplina_equivalente;
            }

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

   /*ITERA_GGROUP_LESSPTR( it_Bloco, problemData->blocos, BlocoCurricular )
   {
      ITERA_GGROUP_N_PT( it_Dias_Letivos, it_Bloco->diasLetivos, int )
      {
         c.reset();
         c.setType( Constraint::C_MAX_CREDS_BLOCO_DIA );

         c.setBloco( *it_Bloco );
         c.setDia( *it_Dias_Letivos );

         sprintf( name, "%s", c.toString().c_str() ); 

         if ( cHash.find(c) != cHash.end() )
         {
            continue;
         }

         nnz = 100;
         double rhs = 4.0;

         //if ( *it_Dias_Letivos == 7 )
         //{
         //   rhs = 12.0;
         //}

         OPT_ROW row( nnz, OPT_ROW::LESS, rhs, name );

         ITERA_GGROUP_LESSPTR( it_disciplina, it_Bloco->disciplinas, Disciplina )
         {
            disciplina = ( *it_disciplina );

            std::pair< Curso *, Curriculo * > curso_curriculo
               = problemData->map_Disc_CursoCurriculo[ disciplina ];
            curso = curso_curriculo.first;
            curriculo = curso_curriculo.second;

            disciplina_equivalente = problemData->retornaDisciplinaSubstituta( curso, curriculo, disciplina );
            if ( disciplina_equivalente != NULL )
            {
               disciplina = disciplina_equivalente;
            }

            v.reset();
            v.setType( Variable::V_CREDITOS_MODF );

            v.setDisciplina( disciplina );
            v.setDia( *it_Dias_Letivos );

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
   }*/

   return restricoes;
}

/*====================================================================/
%DocBegin TRIEDA_LOAD_MODEL

%Constraint 
Ativação da variável zc
%Desc

%MatExp
\begin{eqnarray}
\sum\limits_{i \in I} \sum\limits_{u \in U} \sum\limits_{tps \in SCAP_{u}} o_{i,d,u,tps,t} \leq zc_{d,t} \cdot N \nonumber \qquad 
\forall d \in D \quad
\forall t \in T
\end{eqnarray}


%DocEnd
/====================================================================*/

int SolverMIP::cria_restricao_ativacao_var_zc()
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

      std::pair< Curso *, Curriculo * > curso_curriculo
         = problemData->map_Disc_CursoCurriculo[ disciplina ];
      curso = curso_curriculo.first;
      curriculo = curso_curriculo.second;

      disciplina_equivalente = problemData->retornaDisciplinaSubstituta( curso, curriculo, disciplina );
      if ( disciplina_equivalente != NULL )
      {
         disciplina = disciplina_equivalente;
      }

      GGroup< int >::iterator itDiasLetDisc = disciplina->diasLetivos.begin();

      for (; itDiasLetDisc != disciplina->diasLetivos.end(); itDiasLetDisc++ )
      {
         c.reset();
         c.setType( Constraint::C_VAR_ZC );

         c.setDisciplina( disciplina );
         c.setDia( *itDiasLetDisc );

         sprintf( name, "%s", c.toString().c_str() ); 
         if ( cHash.find( c ) != cHash.end() )
         {
            continue;
         }

         nnz = 100;

         OPT_ROW row( nnz, OPT_ROW::LESS, 0.0, name );

         ITERA_GGROUP_LESSPTR( itCampus, problemData->campi, Campus )
         {
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
                     v.setDia( *itDiasLetDisc );

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
         v.setType( Variable::V_ABERTURA_COMPATIVEL );

         v.setDisciplina( disciplina );
         v.setDia( *itDiasLetDisc );

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
Disciplinas incompatíveis
%Desc

%MatExp
\begin{eqnarray}
zc_{d_1,t} + zc_{d_2,t} \leq 1 \nonumber \qquad 
(d_1, d_2),
\forall t \in T
\end{eqnarray}

%DocEnd
/====================================================================*/

int SolverMIP::cria_restricao_disciplinas_incompativeis()
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

      std::pair< Curso *, Curriculo * > curso_curriculo
         = problemData->map_Disc_CursoCurriculo[ disciplina ];
      curso = curso_curriculo.first;
      curriculo = curso_curriculo.second;

      disciplina_equivalente = problemData->retornaDisciplinaSubstituta( curso, curriculo, disciplina );
      if ( disciplina_equivalente != NULL )
      {
         disciplina = disciplina_equivalente;
      }

      ITERA_GGROUP_N_PT( itDiasLetDisc, disciplina->diasLetivos, int )
      {
         ITERA_GGROUP_N_PT( it_inc, disciplina->ids_disciplinas_incompativeis, int )
         {
            Disciplina * nova_disc = new Disciplina();
            nova_disc->setId( *it_inc );

            c.reset();
            c.setType( Constraint::C_DISC_INCOMPATIVEIS );

            c.setDisciplina( nova_disc );
            c.setDia( *itDiasLetDisc );

            sprintf( name, "%s", c.toString().c_str() ); 

            if ( cHash.find( c ) != cHash.end() )
            {
               continue;
            }

            nnz = 100;
            OPT_ROW row( nnz, OPT_ROW::LESS, 1.0, name );

            v.reset();
            v.setType( Variable::V_ABERTURA_COMPATIVEL );

            v.setDisciplina( disciplina );
            v.setDia( *itDiasLetDisc );

            it_v = vHash.find( v );
            if ( it_v != vHash.end() )
            {
               row.insert( it_v->second, 1.0 );
            }

            v.setDisciplina( nova_disc );

            delete nova_disc;

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
Ativação da variável n
%Desc

%MatExp
\begin{eqnarray}
\sum\limits_{u \in U} \sum\limits_{i \in I} \sum\limits_{d \in D_{bc}} 
o_{i,d,u,tps,t} \leq n_{bc,tps} \nonumber \qquad 
\forall tps \in SCAP_{u}
\forall bc \in B
\forall t \in T
\end{eqnarray}

%DocEnd
/====================================================================*/
int SolverMIP::cria_restricao_abertura_bloco_mesmoTPS()
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
      ITERA_GGROUP_LESSPTR( itCampus, problemData->campi, Campus )
      {
         ITERA_GGROUP_LESSPTR( itUnidade, itCampus->unidades, Unidade )
         {
            ITERA_GGROUP_LESSPTR( itCjtSala, itUnidade->conjutoSalas, ConjuntoSala )
            {
               //////////////////////////////////////////////////////////////////////////////////
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

                     ITERA_GGROUP_LESSPTR( it_campus, problemData->campi, Campus )
                     {
                        ITERA_GGROUP_LESSPTR( it_unidade, it_campus->unidades, Unidade )
                        {
                           ITERA_GGROUP_LESSPTR( it_disciplina, itBloco->disciplinas, Disciplina )
                           {
                              disciplina = ( *it_disciplina );

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
                                 v.reset();
                                 v.setType( Variable::V_OFERECIMENTO );

                                 v.setTurma( turma );
                                 v.setDisciplina( disciplina );
                                 v.setUnidade( *it_unidade );
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
               //////////////////////////////////////////////////////////////////////////////////
            }
         }
      }
   }

   return restricoes;
}

/*====================================================================/
%DocBegin TRIEDA_LOAD_MODEL

%Constraint 
Evitar alocação do mesmo bloco curricular em tipos de salas diferentes
%Desc

%MatExp
\begin{eqnarray}
\sum\limits_{tps \in SCAP_{u}} n_{bc,tps} + fn_{bc,tps} 
\leq 1 \nonumber \qquad 
\forall bc \in B
\end{eqnarray}

%DocEnd
/====================================================================*/
int SolverMIP::cria_restricao_folga_abertura_bloco_mesmoTPS()
{
   int restricoes = 0;
   char name[ 100 ];
   int nnz;

   Constraint c;
   Variable v;
   VariableHash::iterator it_v;

   ITERA_GGROUP_LESSPTR( itBloco, problemData->blocos, BlocoCurricular )
   {
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

      ITERA_GGROUP_LESSPTR( itCampus, problemData->campi, Campus )
      {
         ITERA_GGROUP_LESSPTR( itUnidade, itCampus->unidades, Unidade )
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
