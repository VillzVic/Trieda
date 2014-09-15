#include "SolverMIPUnico.h"

#include <math.h>
#include <hash_set>

#include "NaoAtendimento.h"
#include "AtendimentoHorarioAula.h"
#include "AtendimentoTurno.h"
#include "MIPUnico.h"
#include "opt_lp.h"



using namespace std;

/*  ----------------------------------------------------------------------------------------------------------
	----------------------------------------------------------------------------------------------------------
		
											SolverMIPUnico

	----------------------------------------------------------------------------------------------------------
	----------------------------------------------------------------------------------------------------------
*/

SolverMIPUnico::SolverMIPUnico( ProblemData * aProblemData,
  ProblemSolution * _ProblemSolution, ProblemDataLoader * _problemDataLoader )
   : Solver( aProblemData )
{
   problemSolution = _ProblemSolution;
   problemDataLoader = _problemDataLoader;
   
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

SolverMIPUnico::~SolverMIPUnico()
{
   if ( lp != NULL )
   {
      delete lp;
   }
   
}

int SolverMIPUnico::solve()
{
   int status = 0;

#ifdef READ_SOLUTION
	this->CARREGA_SOLUCAO = true;
#endif
#ifndef READ_SOLUTION
	this->CARREGA_SOLUCAO = false;
#endif


   if ( problemData->parametros->modo_otimizacao == "OPERACIONAL" )
   {
	  std::cout<<"\n------------------------------Operacional------------------------------\n";
	  
	  // OPERACIONAL COM TÁTICO
      if ( problemData->atendimentosTatico != NULL
            && problemData->atendimentosTatico->size() > 0 )
      {
		  ITERA_GGROUP_LESSPTR( itAtTat, ( *problemData->atendimentosTatico ), AtendimentoCampusSolucao )
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
		  
		  // -----------------------
		  // Resolvendo o modelo operacional
		  Operacional * solverOp = new Operacional( this->problemData, &this->CARREGA_SOLUCAO, &(this->solVarsOp), this->problemSolution );
		  status = solverOp->solveOperacionalEtapas();
		  delete solverOp;
		  // -----------------------

      }

	  // OPERACIONAL SEM TÁTICO
      else
      {
			// Neste caso, primeiro deve-se gerar uma saída para
			// o modelo tático. Em seguida, deve-se resolver o
			// modelo operacional com base na saída do modelo tático gerada.
		  
			// -------------------------------------------------
			status = solveEscola();
			
			// -------------------------------------------------
			relacionaAlunosDemandas();
			
			// -------------------------------------------------
			// Write output tático
			std::cout<<"\nImprimindo output tatico... ";
			stringstream ssOutputFile;
			ssOutputFile << "output_tat_";
			ssOutputFile << problemData->getInputFileName().c_str();
			ssOutputFile << "F";
			if ( problemData->getInputId() ) ssOutputFile << "_id" << problemData->getInputId();

			std::ofstream file;
			file.open( ssOutputFile.str(), ios::out );
			if ( file )
			{
				file << ( *problemSolution );
				file.close();
			}
			else std::cout<<"Erro ao abrir arquivo "<< ssOutputFile.str();
			std::cout<<" fim!\n";

			// -------------------------------------------------
			// Preenchendo a estrutura "atendimentosTatico".
			problemData->atendimentosTatico
				= new GGroup< AtendimentoCampusSolucao *, LessPtr< AtendimentoCampusSolucao > >();

			ITERA_GGROUP( it_At_Campus,
				( *problemSolution->atendimento_campus ), AtendimentoCampus )
			{
				std::cout<<"\nit_At_Campus..."; fflush(0);
				problemData->atendimentosTatico->add(
					new AtendimentoCampusSolucao( **it_At_Campus ) );
			}
			
			// -------------------------------------------------
			// Remove a referência para os atendimentos tático (que pertencem ao output tático)
			std::cout<<"\nRemovendo referencias para atend tatico..."; fflush(0);
			ITERA_GGROUP( it_At_Campus,
				( *problemSolution->atendimento_campus ), AtendimentoCampus )
			{
				ITERA_GGROUP_LESSPTR( it_At_Unidade,
					( *it_At_Campus->atendimentos_unidades ), AtendimentoUnidade )
				{
					ITERA_GGROUP_LESSPTR( it_At_Sala,
						( *it_At_Unidade->atendimentos_salas ), AtendimentoSala )
					{
						ITERA_GGROUP_LESSPTR( it_At_DiaSemana,
							( *it_At_Sala->atendimentos_dias_semana ), AtendimentoDiaSemana )
						{
							GGroup< AtendimentoTatico*, LessPtr<AtendimentoTatico> > * atendimentos_tatico
								= it_At_DiaSemana->atendimentos_tatico;

							atendimentos_tatico->clear();

							GGroup< AtendimentoTurno*, LessPtr<AtendimentoTurno> > * atendimentos_turno
								= it_At_DiaSemana->atendimentos_turno;

							atendimentos_turno->clear();
						}
					}
				}
			}
	    
			std::cout<<"\n\n\n------------------------------Operacional------------------------------\n\n";
			
			// -------------------------------------------------
			// Criando as aulas que serão utilizadas
			// para resolver o modelo operacional
			problemDataLoader->criaAulas();

			// -----------------------
			// Resolvendo o modelo operacional
			Operacional * solverOp = new Operacional( this->problemData, &this->CARREGA_SOLUCAO, &(this->solVarsOp), this->problemSolution );
			status = solverOp->solveOperacionalEtapas();
			delete solverOp;
						
			this->problemSolution->computaMotivos(true,true);
			// -----------------------
      }	  

   }
      
   relacionaAlunosDemandas();
   
   int runtime = this->getRunTime();
   int hours = runtime / 3600;						// h
   int min = (int) ( (int) runtime % 3600 ) / 60;	// min
   std::stringstream runtimess;
   runtimess << hours << "h" << min;

   std::cout << "\n\nTotal Run Time = " << runtimess.str() << endl << endl;


   return status;
}

int SolverMIPUnico::solveEscola()
{
	int status = 1;

	std::cout<<"\n\n--------------------------------------------------------------------------------";
	std::cout<<"\n\n--------------------------------------------------------------------------------";
	std::cout << "\nIniciando MIP puro..." << endl;
	
	if ( this->CARREGA_SOLUCAO )
	{
		std::cout << "\nCARREGA_SOLUCAO = true\n" << endl;
	}
	
    ITERA_GGROUP_LESSPTR( itCampus, problemData->campi, Campus )
    {		
		int campusId = ( *itCampus )->getId();
		this->campusAtualId = campusId;
		
		int n_prioridades = problemData->nPrioridadesDemanda[campusId];
		int P = 1;
		
		// ============================================ P1 =======================================================


		std::cout<<"\n-------------------------- Campus " << campusId << "----------------------------\n";		
		std::cout<<"\n-------------------------- Prioridade " << P << "---------------------------\n";
		std::cout << "\nNumero total de niveis de prioridade no campus: " << n_prioridades << endl;
					
		// Cria e ordena os conjuntos de alunos
		problemData->criaCjtAlunos( campusId, P, true );		
		problemData->imprimeCjtAlunos( campusId );
		problemData->constroiHistogramaDiscDemanda();
		
		if ( problemData->cjtAlunos.size() != 1 )
		{
			std::cout << "\nSomente 1 conjunto de alunos eh esperado aqui. Saindo...\n";
			exit(1);
		}

		int grupoId = problemData->cjtAlunos.begin()->first;
		
		int r = 1;
		if ( problemData->parametros->min_alunos_abertura_turmas )
			r = 2;


		// ==================================
		// MODELO INTEGRADO	COM NOVAS TURMAS	

		int NOVAS_TURMAS = 1;
		bool EQUIV=true;
		MIPUnico * solverEscola = new MIPUnico( this->problemData, &(this->solVarsTatico), &(this->vars_xh),
																	&this->CARREGA_SOLUCAO, EQUIV, NOVAS_TURMAS );
		solverEscola->solveMainEscola( campusId, P, r );
		delete solverEscola;

				
//		verificaNaoAtendimentos( campusId, P );
		
		// ============================================ P2 =======================================================

		P++;

		if ( problemData->parametros->utilizarDemandasP2 )
		if ( P <= n_prioridades )
		{
			std::cout << "\nAtualizacao de demandas de prioridade " << P <<"..."; fflush(NULL);
			problemData->atualizaDemandas( P, campusId );
			std::cout << "  atualizadas!\n"; fflush(NULL);

			// ==================================
			// MODELO INTEGRADO	PARA P2
			if ( problemData->listSlackDemandaAluno.size() != 0 )
			{
				MIPUnico * solverEscola = new MIPUnico( 
					this->problemData, &(this->solVarsTatico), &(this->vars_xh), &this->CARREGA_SOLUCAO, true, 0 );
				solverEscola->solveMainEscola( campusId, P, 1 );
				delete solverEscola;
			}
						
			if ( problemData->listSlackDemandaAluno.size() != 0 )
			{
				MIPUnico * solverEscola = new MIPUnico( 
					this->problemData, &(this->solVarsTatico), &(this->vars_xh), &this->CARREGA_SOLUCAO, true, 3 );
				solverEscola->solveMainEscola( campusId, P, 1 );
				delete solverEscola;
			}
			// ==================================
		}
		
//		verificaNaoAtendimentos( campusId, P );
		
		// ========================================= FIM DO CAMPUS =================================================

		problemData->confereExcessoP2( campusId );		
		problemData->listSlackDemandaAluno.clear();
		mudaCjtSalaParaSala();
		getSolutionTaticoPorAlunoComHorario();

	}
	
	return (status);
}

void SolverMIPUnico::relacionaAlunosDemandas()
{
	// Método que relaciona cada demanda atendida aos
	// correspondentes alunos que assistirão as aulas 

	std::cout<<"\nRelacionando AlunosDemanda...";

   Campus * campus = NULL;
   Unidade * unidade = NULL;
   Sala * sala = NULL;
   int dia_semana = 0;

   // Lendo os atendimentos oferta da solução
   GGroup< AtendimentoOferta * > atendimentosOferta;

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

               // Modelo Tático
               if ( it_At_DiaSemana->atendimentos_tatico != NULL
                  && it_At_DiaSemana->atendimentos_tatico->size() > 0 )
               {
                  ITERA_GGROUP_LESSPTR( it_at_tatico,
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
                  ITERA_GGROUP_LESSPTR( it_at_turno,
                     ( *it_At_DiaSemana->atendimentos_turno ), AtendimentoTurno )
                  {
                     AtendimentoTurno * at_turno = ( *it_at_turno );

                     ITERA_GGROUP_LESSPTR( it_horario_aula,
                        ( *at_turno->atendimentos_horarios_aula ), AtendimentoHorarioAula )
                     {
                        AtendimentoHorarioAula * horario_aula = ( *it_horario_aula );

                        ITERA_GGROUP_LESSPTR( it_oferta,
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

   // Armazenando a informação de quantos alunos
   // de cada demanda foram atendidos pela solução
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
			   if ( (*it_at_aluno).third->getId() == disc->getId() )
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
	   std::cout<<"\nNot done. Exiting...\n";
	   exit(1);
   }
   std::cout<<" done!\n";
}

void SolverMIPUnico::mudaCjtSalaParaSala()
{
	std::cout<<"\nMudando de CjtSala para Sala...\n";
	
	if ( problemData->parametros->otimizarPor == "ALUNO" )
	{
		ITERA_GGROUP_LESSPTR( it_Vars_x, vars_xh, VariableTatico )
		{
			if ( ( *it_Vars_x )->getSubCjtSala()->salas.size() > 0 )
			{
				Sala *auxSala = (( *it_Vars_x )->getSubCjtSala()->salas.begin())->second;
				( *it_Vars_x )->setSala(auxSala);
			}
		}

		//std::cout<<"\nSolucao:";
		//
		// Imprimindo as variáveis x_{i,d,u,s,hi,hf,t} convertidas.
		//std::cout << "\n\n\n";
		//std::cout << "x\t\ti\td\tu\ts\t\thi\thf\tt\n\n";
		//
		//ITERA_GGROUP_LESSPTR( it_Vars_x, vars_xh, VariableTatico )
		//{
		//	if ( ( *it_Vars_x )->getSala() == NULL )
		//	{
		//		printf( "\nOPA. Variavel x (x_i(%d)_d(%d)_u(%d)_tps(%d)_t(%d))nao convertida.\n\n",
		//				( *it_Vars_x )->getTurma(),
		//				( *it_Vars_x )->getDisciplina()->getId(),
		//				( *it_Vars_x )->getUnidade()->getId(),
		//				( *it_Vars_x )->getSubCjtSala()->getId(),
		//				( *it_Vars_x )->getHorarioAulaInicial()->getId(),
		//				( *it_Vars_x )->getHorarioAulaFinal()->getId(),
		//				( *it_Vars_x )->getDia() );

		//		exit( 1 );
		//	}

		//	std::cout << (*it_Vars_x)->getValue() << "\t\t"
		//			<< ( *it_Vars_x )->getTurma() << "\t"
		//			<< ( *it_Vars_x )->getDisciplina()->getCodigo() << "\t"
		//			<< ( *it_Vars_x )->getUnidade()->getCodigo() << "\t"
		//			<< ( *it_Vars_x )->getSala()->getCodigo() << "\t"
		//			<< ( *it_Vars_x )->getHorarioAulaInicial()->getId() << "\t"
		//			<< ( *it_Vars_x )->getHorarioAulaFinal()->getId() << "\t"
		//			<< ( *it_Vars_x )->getDia() << "\n\n";
		//	fflush(NULL);
		//}
	}
}

void SolverMIPUnico::getSolutionTaticoPorAlunoComHorario()
{
	std::cout<<"\nPreenchendo a estrutura atendimento_campus com a saida.\n"; fflush(NULL);		

   // POVOANDO AS CLASSES DE SAIDA

   int at_Tatico_Counter = 0;

   // Iterando sobre as variáveis do tipo x.
   ITERA_GGROUP_LESSPTR( it_Vars_x, vars_xh, VariableTatico )
   {
	  int dia = ( *it_Vars_x )->getDia();
	  Disciplina *d = ( *it_Vars_x )->getDisciplina();
	  int turma = ( *it_Vars_x )->getTurma();
	  HorarioAula *hi = ( *it_Vars_x )->getHorarioAulaInicial();
	  HorarioAula *hf = ( *it_Vars_x )->getHorarioAulaFinal();
  	  int nCreds = hi->getCalendario()->retornaNroCreditosEntreHorarios( hi, hf );
	  Sala *sala = ( *it_Vars_x )->getSala();
      Unidade * unidade = ( *it_Vars_x )->getUnidade();
	  
      // Descobrindo qual Campus a variável x em questão pertence.
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
               ITERA_GGROUP_LESSPTR( it_At_Unidade, ( *it_At_Campus->atendimentos_unidades ), AtendimentoUnidade )
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
                        ITERA_GGROUP_LESSPTR( it_At_Sala, ( *it_At_Unidade->atendimentos_salas ), AtendimentoSala )
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
									// Verifica se o AtendimentoDiaSemana já existe
									ITERA_GGROUP_LESSPTR( it_At_Dia, ( *it_At_Sala->atendimentos_dias_semana ), AtendimentoDiaSemana )
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

									std::map< Trio<int,int,Disciplina*>, GGroup< AlunoDemanda*, LessPtr< AlunoDemanda > > >::iterator
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
									map< Oferta*, map< Disciplina*, GGroup< AlunoDemanda*, LessPtr<AlunoDemanda> >, LessPtr<Disciplina> >, LessPtr<Oferta> > mapOftDiscOriginais;
									ITERA_GGROUP_LESSPTR( itAlunoDemanda, itMap->second, AlunoDemanda )
									{									
										Demanda *demOrig = itAlunoDemanda->demandaOriginal;
										if ( demOrig != NULL )
											mapOftDiscOriginais[ itAlunoDemanda->demanda->oferta ][ demOrig->disciplina ].add( *itAlunoDemanda );
										else
											mapOftDiscOriginais[ itAlunoDemanda->demanda->oferta ][ itAlunoDemanda->demanda->disciplina ].add( *itAlunoDemanda );
									}
																							
									map< Oferta*, map< Disciplina*, GGroup< AlunoDemanda*, LessPtr<AlunoDemanda> >, LessPtr<Disciplina> >, LessPtr<Oferta> >::iterator
											itMapOft_DiscOrig = mapOftDiscOriginais.begin();
									for ( ; itMapOft_DiscOrig != mapOftDiscOriginais.end() ; itMapOft_DiscOrig++ )
									{
										Oferta *oferta = itMapOft_DiscOrig->first;

										map< Disciplina*, GGroup< AlunoDemanda*, LessPtr<AlunoDemanda> >, LessPtr<Disciplina> > 
											mapDisc_Alunos = itMapOft_DiscOrig->second;

										map< Disciplina*, GGroup< AlunoDemanda*, LessPtr<AlunoDemanda> >, LessPtr<Disciplina> >::iterator
											itMap_DiscOrig = mapDisc_Alunos.begin();
										for ( ; itMap_DiscOrig != mapDisc_Alunos.end(); itMap_DiscOrig++ )
										{	
											Disciplina* discOrig = itMap_DiscOrig->first;																																		
										
											GGroup< AlunoDemanda*, LessPtr<AlunoDemanda> > alunosDemanda = itMap_DiscOrig->second;

											AtendimentoTatico * at_Tatico = new AtendimentoTatico(
											this->problemSolution->getIdAtendimentos(),
											this->problemSolution->getIdAtendimentos() );

											// Verificando se a disciplina é de carater prático ou teórico.
											if ( d->getId() > 0 && d->getCredTeoricos() > 0 )
											{
												at_Tatico->setQtdCreditosTeoricos( nCreds );
											}
											else
											{
												at_Tatico->setQtdCreditosPraticos( nCreds );
											}

											AtendimentoOferta * at_Oferta = at_Tatico->atendimento_oferta;
										
											ITERA_GGROUP_LESSPTR( itAlunoDemanda, alunosDemanda, AlunoDemanda )
											{
												if ( itAlunoDemanda->demanda->getDisciplinaId() < 0 )
												{
													int alunoId = itAlunoDemanda->getAlunoId();
													int discId = - itAlunoDemanda->demanda->getDisciplinaId();
													Aluno* aluno = problemData->retornaAluno( alunoId );

													// Disciplina pratica que teve seu AlunoDemanda criado internamente, pelo solver.
													// Deve-se passar o AlunoDemanda original, que corresponde ao da disciplina teorica.
													AlunoDemanda* alunoDemanda = aluno->getAlunoDemanda( discId );
													if ( alunoDemanda != NULL )
													{
														at_Oferta->alunosDemandasAtendidas.add( alunoDemanda->getId() );
													}
													else std::cout<<"\nERROR: alunodemanda teorico nao encontrado\n";
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
											
											if ( problemData->parametros->considerar_equivalencia &&
												! problemData->parametros->considerar_equivalencia_por_aluno )
											{
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
											}
											else
											{
												at_Oferta->setDisciplinaId( discOrig->getId() );
												at_Oferta->disciplina = d;

												if ( id_disc != discOrig->getId() )
												{
													at_Oferta->setDisciplinaSubstitutaId( id_disc );
												}
											}

											at_Oferta->setQuantidade( at_Oferta->alunosDemandasAtendidas.size() );
											at_Oferta->setTurma( turma );
											at_Oferta->oferta = oferta;

											HorarioAula *h = hi;
											while (1)
											{
												at_Tatico->addHorarioAula( h->getId() );
												if ( h->getInicio() == hf->getInicio() ) break;
												h = h->getCalendario()->getProximoHorario( h );
												if ( h == NULL )
												{
													std::cout<<"\nErro 1!!! horario NULL antes de encontrar hf\n";
													break;
												}
											}

											at_Dia_Semana->atendimentos_tatico->add( at_Tatico );

											++at_Tatico_Counter;
										}

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

							std::map< Trio<int,int,Disciplina*>, GGroup< AlunoDemanda*, LessPtr< AlunoDemanda > > >::iterator
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
							map< Oferta*, map< Disciplina*, GGroup< AlunoDemanda*, LessPtr<AlunoDemanda> >, LessPtr<Disciplina> >, LessPtr<Oferta> > mapOftDiscOriginais;
							ITERA_GGROUP_LESSPTR( itAlunoDemanda, itMap->second, AlunoDemanda )
							{									
								Demanda *demOrig = itAlunoDemanda->demandaOriginal;
								if ( demOrig != NULL )
									mapOftDiscOriginais[ itAlunoDemanda->demanda->oferta ][ demOrig->disciplina ].add( *itAlunoDemanda );
								else
									mapOftDiscOriginais[ itAlunoDemanda->demanda->oferta ][ itAlunoDemanda->demanda->disciplina ].add( *itAlunoDemanda );
							}
																							
							map< Oferta*, map< Disciplina*, GGroup< AlunoDemanda*, LessPtr<AlunoDemanda> >, LessPtr<Disciplina> >, LessPtr<Oferta> >::iterator
									itMapOft_DiscOrig = mapOftDiscOriginais.begin();
							for ( ; itMapOft_DiscOrig != mapOftDiscOriginais.end() ; itMapOft_DiscOrig++ )
							{
								Oferta *oferta = itMapOft_DiscOrig->first;

								map< Disciplina*, GGroup< AlunoDemanda*, LessPtr<AlunoDemanda> >, LessPtr<Disciplina> > 
									mapDisc_Alunos = itMapOft_DiscOrig->second;

								map< Disciplina*, GGroup< AlunoDemanda*, LessPtr<AlunoDemanda> >, LessPtr<Disciplina> >::iterator
									itMap_DiscOrig = mapDisc_Alunos.begin();
								for ( ; itMap_DiscOrig != mapDisc_Alunos.end(); itMap_DiscOrig++ )
								{	
									Disciplina* discOrig = itMap_DiscOrig->first;																																		
										
									GGroup< AlunoDemanda*, LessPtr<AlunoDemanda> > alunosDemanda = itMap_DiscOrig->second;

									AtendimentoTatico * at_Tatico = new AtendimentoTatico(
									this->problemSolution->getIdAtendimentos(),
									this->problemSolution->getIdAtendimentos() );

									// Verificando se a disciplina é de carater prático ou teórico.
									if ( d->getId() > 0 && d->getCredTeoricos() > 0 )
									{
										at_Tatico->setQtdCreditosTeoricos( nCreds );
									}
									else
									{
										at_Tatico->setQtdCreditosPraticos( nCreds );
									}

									AtendimentoOferta * at_Oferta = at_Tatico->atendimento_oferta;
										
									ITERA_GGROUP_LESSPTR( itAlunoDemanda, alunosDemanda, AlunoDemanda )
									{
										if ( itAlunoDemanda->demanda->getDisciplinaId() < 0 )
										{
											int alunoId = itAlunoDemanda->getAlunoId();
											int discId = - itAlunoDemanda->demanda->getDisciplinaId();
											Aluno* aluno = problemData->retornaAluno( alunoId );

											// Disciplina pratica que teve seu AlunoDemanda criado internamente, pelo solver.
											// Deve-se passar o AlunoDemanda original, que corresponde ao da disciplina teorica.
											AlunoDemanda* alunoDemanda = aluno->getAlunoDemanda( discId );
											if ( alunoDemanda != NULL )
											{
												at_Oferta->alunosDemandasAtendidas.add( alunoDemanda->getId() );
											}
											else std::cout<<"\nERROR: alunodemanda teorico nao encontrado\n";
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
											
									if ( problemData->parametros->considerar_equivalencia &&
										! problemData->parametros->considerar_equivalencia_por_aluno )
									{
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
									}
									else
									{		
										at_Oferta->setDisciplinaId( discOrig->getId() );
										at_Oferta->disciplina = d;

										if ( id_disc != discOrig->getId() )
										{
											at_Oferta->setDisciplinaSubstitutaId( id_disc );
										}																					
									}

									at_Oferta->setQuantidade( at_Oferta->alunosDemandasAtendidas.size() );
									at_Oferta->setTurma( turma );
									at_Oferta->oferta = oferta;
									
									HorarioAula *h = hi;
									while (1)
									{
										at_Tatico->addHorarioAula( h->getId() );
										if ( h->getInicio() == hf->getInicio() ) break;
										h = h->getCalendario()->getProximoHorario( h );
										if ( h == NULL )
										{
											std::cout<<"\nErro 2!!! horario NULL antes de encontrar hf\n";
											break;
										}
									}

									at_Dia_Semana->atendimentos_tatico->add( at_Tatico );

									++at_Tatico_Counter;
								}

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

					std::map< Trio<int,int,Disciplina*>, GGroup< AlunoDemanda*, LessPtr< AlunoDemanda > > >::iterator
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
					map< Oferta*, map< Disciplina*, GGroup< AlunoDemanda*, LessPtr<AlunoDemanda> >, LessPtr<Disciplina> >, LessPtr<Oferta> > mapOftDiscOriginais;
					ITERA_GGROUP_LESSPTR( itAlunoDemanda, itMap->second, AlunoDemanda )
					{									
						Demanda *demOrig = itAlunoDemanda->demandaOriginal;
						if ( demOrig != NULL )
							mapOftDiscOriginais[ itAlunoDemanda->demanda->oferta ][ demOrig->disciplina ].add( *itAlunoDemanda );
						else
							mapOftDiscOriginais[ itAlunoDemanda->demanda->oferta ][ itAlunoDemanda->demanda->disciplina ].add( *itAlunoDemanda );
					}
																							
					map< Oferta*, map< Disciplina*, GGroup< AlunoDemanda*, LessPtr<AlunoDemanda> >, LessPtr<Disciplina> >, LessPtr<Oferta> >::iterator
							itMapOft_DiscOrig = mapOftDiscOriginais.begin();
					for ( ; itMapOft_DiscOrig != mapOftDiscOriginais.end() ; itMapOft_DiscOrig++ )
					{
						Oferta *oferta = itMapOft_DiscOrig->first;

						map< Disciplina*, GGroup< AlunoDemanda*, LessPtr<AlunoDemanda> >, LessPtr<Disciplina> > 
							mapDisc_Alunos = itMapOft_DiscOrig->second;

						map< Disciplina*, GGroup< AlunoDemanda*, LessPtr<AlunoDemanda> >, LessPtr<Disciplina> >::iterator
							itMap_DiscOrig = mapDisc_Alunos.begin();
						for ( ; itMap_DiscOrig != mapDisc_Alunos.end(); itMap_DiscOrig++ )
						{	
							Disciplina* discOrig = itMap_DiscOrig->first;																																		
										
							GGroup< AlunoDemanda*, LessPtr<AlunoDemanda> > alunosDemanda = itMap_DiscOrig->second;

							AtendimentoTatico * at_Tatico = new AtendimentoTatico(
							this->problemSolution->getIdAtendimentos(),
							this->problemSolution->getIdAtendimentos() );

							// Verificando se a disciplina é de carater prático ou teórico.
							if ( d->getId() > 0 && d->getCredTeoricos() > 0 )
							{
								at_Tatico->setQtdCreditosTeoricos( nCreds );
							}
							else
							{
								at_Tatico->setQtdCreditosPraticos( nCreds );
							}

							AtendimentoOferta * at_Oferta = at_Tatico->atendimento_oferta;
										
							ITERA_GGROUP_LESSPTR( itAlunoDemanda, alunosDemanda, AlunoDemanda )
							{
								if ( itAlunoDemanda->demanda->getDisciplinaId() < 0 )
								{
									int alunoId = itAlunoDemanda->getAlunoId();
									int discId = - itAlunoDemanda->demanda->getDisciplinaId();
									Aluno* aluno = problemData->retornaAluno( alunoId );

									// Disciplina pratica que teve seu AlunoDemanda criado internamente, pelo solver.
									// Deve-se passar o AlunoDemanda original, que corresponde ao da disciplina teorica.
									AlunoDemanda* alunoDemanda = aluno->getAlunoDemanda( discId );
									if ( alunoDemanda != NULL )
									{
										at_Oferta->alunosDemandasAtendidas.add( alunoDemanda->getId() );
									}
									else std::cout<<"\nERROR: alunodemanda teorico nao encontrado\n";
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
											
							if ( problemData->parametros->considerar_equivalencia &&
								! problemData->parametros->considerar_equivalencia_por_aluno )
							{
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
							}
							else
							{		
								at_Oferta->setDisciplinaId( discOrig->getId() );
								at_Oferta->disciplina = d;

								if ( id_disc != discOrig->getId() )
								{
									at_Oferta->setDisciplinaSubstitutaId( id_disc );
								}																				
							}

							at_Oferta->setQuantidade( at_Oferta->alunosDemandasAtendidas.size() );
							at_Oferta->setTurma( turma );
							at_Oferta->oferta = oferta;
														
							HorarioAula *h = hi;
							while (1)
							{
								at_Tatico->addHorarioAula( h->getId() );
								if ( h->getInicio() == hf->getInicio() ) break;
								h = h->getCalendario()->getProximoHorario( h );
								if ( h == NULL )
								{
									std::cout<<"\nErro 3!!! horario NULL antes de encontrar hf\n";
									break;
								}
							}

							at_Dia_Semana->atendimentos_tatico->add( at_Tatico );

							++at_Tatico_Counter;
						}

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

			std::map< Trio<int,int,Disciplina*>, GGroup< AlunoDemanda*, LessPtr< AlunoDemanda > > >::iterator
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
			map< Oferta*, map< Disciplina*, GGroup< AlunoDemanda*, LessPtr<AlunoDemanda> >, LessPtr<Disciplina> >, LessPtr<Oferta> > mapOftDiscOriginais;
			ITERA_GGROUP_LESSPTR( itAlunoDemanda, itMap->second, AlunoDemanda )
			{									
				Demanda *demOrig = itAlunoDemanda->demandaOriginal;
				if ( demOrig != NULL )
					mapOftDiscOriginais[ itAlunoDemanda->demanda->oferta ][ demOrig->disciplina ].add( *itAlunoDemanda );
				else
					mapOftDiscOriginais[ itAlunoDemanda->demanda->oferta ][ itAlunoDemanda->demanda->disciplina ].add( *itAlunoDemanda );
			}
																							
			map< Oferta*, map< Disciplina*, GGroup< AlunoDemanda*, LessPtr<AlunoDemanda> >, LessPtr<Disciplina> >, LessPtr<Oferta> >::iterator
					itMapOft_DiscOrig = mapOftDiscOriginais.begin();
			for ( ; itMapOft_DiscOrig != mapOftDiscOriginais.end() ; itMapOft_DiscOrig++ )
			{
				Oferta *oferta = itMapOft_DiscOrig->first;

				map< Disciplina*, GGroup< AlunoDemanda*, LessPtr<AlunoDemanda> >, LessPtr<Disciplina> > 
					mapDisc_Alunos = itMapOft_DiscOrig->second;

				map< Disciplina*, GGroup< AlunoDemanda*, LessPtr<AlunoDemanda> >, LessPtr<Disciplina> >::iterator
					itMap_DiscOrig = mapDisc_Alunos.begin();
				for ( ; itMap_DiscOrig != mapDisc_Alunos.end(); itMap_DiscOrig++ )
				{	
					Disciplina* discOrig = itMap_DiscOrig->first;																																		
										
					GGroup< AlunoDemanda*, LessPtr<AlunoDemanda> > alunosDemanda = itMap_DiscOrig->second;

					AtendimentoTatico * at_Tatico = new AtendimentoTatico(
					this->problemSolution->getIdAtendimentos(),
					this->problemSolution->getIdAtendimentos() );

					// Verificando se a disciplina é de carater prático ou teórico.
					if ( d->getId() > 0 && d->getCredTeoricos() > 0 )
					{
						at_Tatico->setQtdCreditosTeoricos( nCreds );
					}
					else
					{
						at_Tatico->setQtdCreditosPraticos( nCreds );
					}

					AtendimentoOferta * at_Oferta = at_Tatico->atendimento_oferta;
										
					ITERA_GGROUP_LESSPTR( itAlunoDemanda, alunosDemanda, AlunoDemanda )
					{
						if ( itAlunoDemanda->demanda->getDisciplinaId() < 0 )
						{
							int alunoId = itAlunoDemanda->getAlunoId();
							int discId = - itAlunoDemanda->demanda->getDisciplinaId();
							Aluno* aluno = problemData->retornaAluno( alunoId );

							// Disciplina pratica que teve seu AlunoDemanda criado internamente, pelo solver.
							// Deve-se passar o AlunoDemanda original, que corresponde ao da disciplina teorica.
							AlunoDemanda* alunoDemanda = aluno->getAlunoDemanda( discId );
							if ( alunoDemanda != NULL )
							{
								at_Oferta->alunosDemandasAtendidas.add( alunoDemanda->getId() );
							}
							else std::cout<<"\nERROR: alunodemanda teorico nao encontrado\n";
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
											
					if ( problemData->parametros->considerar_equivalencia &&
						! problemData->parametros->considerar_equivalencia_por_aluno )
					{
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
					}
					else
					{		
						at_Oferta->setDisciplinaId( discOrig->getId() );
						at_Oferta->disciplina = d;

						if ( id_disc != discOrig->getId() )
						{
							at_Oferta->setDisciplinaSubstitutaId( id_disc );
						}
					}

					at_Oferta->setQuantidade( at_Oferta->alunosDemandasAtendidas.size() );
					at_Oferta->setTurma( turma );
					at_Oferta->oferta = oferta;
					
					HorarioAula *h = hi;
					while (1)
					{
						at_Tatico->addHorarioAula( h->getId() );
						if ( h->getInicio() == hf->getInicio() ) break;
						h = h->getCalendario()->getProximoHorario( h );
						if ( h == NULL )
						{
							std::cout<<"\nErro 4!!! horario NULL antes de encontrar hf\n";
							break;
						}
					}

					at_Dia_Semana->atendimentos_tatico->add( at_Tatico );

					++at_Tatico_Counter;
				}
			}

			#pragma endregion							

			at_Sala->atendimentos_dias_semana->add( at_Dia_Semana );
			at_Unidade->atendimentos_salas->add( at_Sala );
			at_Campus->atendimentos_unidades->add( at_Unidade );

			problemSolution->atendimento_campus->add( at_Campus );
      }

	  // todo: posso deletar isso aqui msm? Acrescentei qdo comecei a testar solucao inicial fixada, indo direto pro TatInt
	  delete (*it_Vars_x);
   }

   // todo: posso deletar isso aqui msm? Acrescentei qdo comecei a testar solucao inicial fixada, indo direto pro TatInt
   vars_xh.clear();
}
