#include "SolverMIPUnico.h"

#include <math.h>
#include <hash_set>
#include <vector>
#include <iostream>
#include <algorithm>
#include <stdio.h>
#include <stdlib.h>

#include "opt_lp.h"
#include "NaoAtendimento.h"
#include "AtendimentoHorarioAula.h"
#include "AtendimentoTurno.h"
#include "MIPUnico.h"
#include "CentroDados.h"
#include "Operacional.h"

#include "ErrorHandler.h"
#include "ProblemDataLoader.h"
#include "ProblemData.h"
#include "ProblemSolution.h"
#include "ConstraintOp.h"
#include "AlocacaoProfVirtual.h"
#include "Indicadores.h"

using namespace std;


const bool SolverMIPUnico::RODAR_OPERACIONAL_ = false;


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
   
   clear();
}

void SolverMIPUnico::clear()
{
	if (!SolverMIPUnico::RODAR_OPERACIONAL_)
	{
		for (auto vit=solMipUnico_.begin(); vit!=solMipUnico_.end(); vit++)
		{
			if (*vit) delete *vit;
		}
	}
	solMipUnico_.clear();
}

int SolverMIPUnico::solve()
{
   int status = 0;

   if (CentroDados::getLoadPartialSol())
		this->CARREGA_SOLUCAO = true;
   else
		this->CARREGA_SOLUCAO = false;
	
   if ( problemData->parametros->modo_otimizacao == "OPERACIONAL" )
   {
	  std::cout<<"\n------------------------------Operacional------------------------------\n";
	  
	  // OPERACIONAL COM TÁTICO
      if ( problemData->atendimentosTatico != nullptr
            && problemData->atendimentosTatico->size() > 0 )
      {
		  status = solveOpComTatico();
      }
	  // OPERACIONAL SEM TÁTICO
      else
      {
		  status = solveOpSemTatico();
      }
   }
   else
   {
	   CentroDados::printError("SolverMIPUnico::solve()", "Modo tatico esta obsoleto! Somente modo operacional eh aceito.");
	   exit(1);
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

int SolverMIPUnico::solveOpComTatico()
{ 
	int status=true;

	preencheAtendTaticoProbSol();

	status = status && solveEscolaOp();
	
	return status;
}

void SolverMIPUnico::preencheAtendTaticoProbSol()
{
	if (!problemData->atendimentosTatico)
		return;

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
}

int SolverMIPUnico::solveOpSemTatico()
{
	// Primeiro deve-se gerar uma saída para
	// o modelo tático. Em seguida, deve-se resolver o
	// modelo operacional com base na saída do modelo tático gerada.

	int status=true;

	// -------------------------------------------------
	status = status && solveEscolaTat();
			
	// -------------------------------------------------
	relacionaAlunosDemandas();
			
	// -------------------------------------------------
	//writeOutputTatico();
	
	// -------------------------------------------------
	preencheAtendTaticoProbData();
			
	// -------------------------------------------------
	clearAtendTaticoProbSol();
	
	// -------------------------------------------------
	if (!RODAR_OPERACIONAL_)
	{
		extractSolution_();
		contabilizaGapProfReal_();
		criarOutputFinal_(problemSolution);
		problemSolution->computaMotivos(true,true);
		writeOutputOp_();

		return status;
	}

	// -------------------------------------------------
	// Criando as aulas que serão utilizadas para resolver o modelo operacional
	problemDataLoader->criaAulas();

	// -----------------------
	status = status && solveEscolaOp();
	
	return status;
}

bool SolverMIPUnico::preencheAtendTaticoProbData()
{
	if ( !problemSolution ){
		CentroDados::printError("SolverMIPUnico::solve()", "problemSolution null!");
		return false;
	}
	if ( !problemSolution->atendimento_campus ){
		CentroDados::printError("SolverMIPUnico::solve()", "problemSolution->atendimento_campus null!");
		return false;
	}

	// Preenchendo a estrutura "atendimentosTatico" de problemData.
				
	if (problemData->atendimentosTatico)
		delete problemData->atendimentosTatico;
	problemData->atendimentosTatico
		= new GGroup< AtendimentoCampusSolucao *, LessPtr< AtendimentoCampusSolucao > >();
	
	ITERA_GGROUP( it_At_Campus,
		( *problemSolution->atendimento_campus ), AtendimentoCampus )
	{
		problemData->atendimentosTatico->add(
			new AtendimentoCampusSolucao( **it_At_Campus ) );
	}

	return true;
}

bool SolverMIPUnico::clearAtendTaticoProbSol()
{	
	if ( !problemSolution ){
		CentroDados::printError("SolverMIPUnico::clearAtendTaticoProbSol()", "problemSolution null!");
		return false;
	}
	if ( !problemSolution->atendimento_campus ){
		CentroDados::printError("SolverMIPUnico::clearAtendTaticoProbSol()", "problemSolution->atendimento_campus null!");
		return false;
	}

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

	return true;
}

int SolverMIPUnico::solveEscolaTat()
{
	int status = 1;

	std::cout<<"\n\n--------------------------------------------------------------------------------";
	std::cout<<"\n\n--------------------------------------------------------------------------------";
	std::cout << "\nIniciando MIP puro..." << endl;
	
	if ( this->CARREGA_SOLUCAO )
		std::cout << "\nCARREGA_SOLUCAO = true\n" << endl;
	
    ITERA_GGROUP_LESSPTR( itCampus, problemData->campi, Campus )
    {		
		int campusId = ( *itCampus )->getId();
		this->campusAtualId = campusId;
		
		solveCampusP1Escola();
		solveCampusP2Escola();
		
		problemData->confereExcessoP2( campusId );		
		problemData->listSlackDemandaAluno.clear();
		mudaCjtSalaParaSala();
		getSolutionTaticoPorAlunoComHorario();

	}
	
	return (status);
}

void SolverMIPUnico::solveCampusP1Escola()
{
	int P = 1;
	int n_prioridades = problemData->nPrioridadesDemanda[this->campusAtualId];

	std::cout<<"\n-------------------------- Campus " << this->campusAtualId << "----------------------------\n";		
	std::cout<<"\n-------------------------- Prioridade " << P << "---------------------------\n";
	std::cout << "\nNumero total de niveis de prioridade no campus: " << n_prioridades << endl;
					
	// Cria e ordena os conjuntos de alunos
	problemData->criaCjtAlunos( this->campusAtualId, P, true );		
	problemData->imprimeCjtAlunos( this->campusAtualId );
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
	solverEscola->solveMainEscola( this->campusAtualId, P, r, solMipUnico_ );
	delete solverEscola;	
}

void SolverMIPUnico::solveCampusP2Escola()
{
	int P = 2;
	int n_prioridades = problemData->nPrioridadesDemanda[this->campusAtualId];

	if ( problemData->parametros->utilizarDemandasP2 )
	if ( P <= n_prioridades )
	{		
		std::cout<<"\n-------------------------- Campus " << this->campusAtualId << "----------------------------\n";		
		std::cout<<"\n-------------------------- Prioridade " << P << "---------------------------\n";

		problemData->atualizaDemandas( P, this->campusAtualId );

		if ( problemData->listSlackDemandaAluno.size() != 0 )
		{
			// Só inserção de alunos
			MIPUnico * solverEscola = new MIPUnico( 
				this->problemData, &(this->solVarsTatico), &(this->vars_xh), &this->CARREGA_SOLUCAO, true, 0 );
			solverEscola->solveMainEscola( this->campusAtualId, P, 1, solMipUnico_ );
			delete solverEscola;
		}
						
		if ( problemData->listSlackDemandaAluno.size() != 0 )
		{
			// Permite novas turmas
			MIPUnico * solverEscola = new MIPUnico( 
				this->problemData, &(this->solVarsTatico), &(this->vars_xh), &this->CARREGA_SOLUCAO, true, 3 );
			solverEscola->solveMainEscola( this->campusAtualId, P, 1, solMipUnico_ );
			delete solverEscola;
		}
	}
}

void SolverMIPUnico::relacionaAlunosDemandas()
{
	// Método que relaciona cada demanda atendida aos
	// correspondentes alunos que assistirão as aulas 

	std::cout<<"\nRelacionando AlunosDemanda..."; fflush(0);

   Campus * campus = NULL;
   Unidade * unidade = NULL;
   Sala * sala = NULL;
   int dia_semana = 0;

   // Lendo os atendimentos oferta da solução
   GGroup< AtendimentoOferta * > atendimentosOferta;

	if (!problemSolution)
	{
		CentroDados::printError("void SolverMIPUnico::relacionaAlunosDemandas()",
								"problemSolution null");
		return;
	}

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

	if (!problemSolution)
	{
		CentroDados::printError("void SolverMIPUnico::getSolutionTaticoPorAlunoComHorario()",
								"problemSolution null");
		return;
	}

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
   }

   clearSolutionTat();
}

int SolverMIPUnico::solveEscolaOp()
{
	int status=true;

	Operacional * solverOp = new Operacional( this->problemData, &this->CARREGA_SOLUCAO, &(this->solVarsOp), this->problemSolution );
	status = status && solverOp->solveOperacionalEtapas();
	delete solverOp;
	
	clearSolutionOp();

	this->problemSolution->computaMotivos(true,true);

	return status;
}

void SolverMIPUnico::clearSolutionTat()
{
	solVarsTatico.deleteElements();
	vars_xh.clear();
}

void SolverMIPUnico::clearSolutionOp()
{
	solVarsOp.deleteElements();
}

void SolverMIPUnico::writeOutputTatico()
{
	if(!problemSolution) return;

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
}

void SolverMIPUnico::writeOutputOp_()
{
	if(!problemSolution) return;

	// Write final output
	std::cout<<"\nImprimindo output final... ";
	stringstream ssOutputFile;
	ssOutputFile << "output";
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
}

void SolverMIPUnico::extractSolution_()
{
	extractSolutionToMaps_();
	extractSolutionFromMapY_();
	extractSolutionFromMapS_();
	individualizaProfsVirtuais_();
	extractSolutionFromMapX_();
}

void SolverMIPUnico::extractSolutionToMaps_()
{
	std::cout<<"\nextractSolutionToMaps..."; fflush(0);

	for (auto itVar=solMipUnico_.begin(); itVar!=solMipUnico_.end(); itVar++)
	{
		if ((*itVar)->getType() == VariableMIPUnico::V_CREDITOS)
		{
			solMipUnicoX_.insert(*itVar);
		}
		if ((*itVar)->getType() == VariableMIPUnico::V_PROF_TURMA)
		{
			solMipUnicoY_.insert(*itVar);
		}
		if ((*itVar)->getType() == VariableMIPUnico::V_ALOCA_ALUNO_TURMA_DISC)
		{
			solMipUnicoS_.insert(*itVar);
		}
	}	
}

void SolverMIPUnico::extractSolutionFromMapY_()
{
	std::cout<<"\nextractSolutionFromMapY..."; fflush(0);

	for (auto itVar=solMipUnicoY_.cbegin(); itVar!=solMipUnicoY_.cend(); itVar++)
	{
		VariableMIPUnico* const y = (*itVar);

		Professor* const p = y->getProfessor();
		Campus* const cp = y->getCampus();
		Disciplina* const d = y->getDisciplina();
		int const turma = y->getTurma();
		
		if (p->eVirtual())
		{
			cadastraTurmaMapPV(cp, d, turma);	// A ser individualizado
		}
		else
		{
			associaProfATurmaMapAulas(cp, d, turma, p);
		}
	}
}

void SolverMIPUnico::extractSolutionFromMapX_()
{
	std::cout<<"\nextractSolutionFromMapX..."; fflush(0);

	for (auto itVar=solMipUnicoX_.cbegin(); itVar!=solMipUnicoX_.cend(); itVar++)
	{
		VariableMIPUnico* const x = (*itVar);

		int cpId = x->getUnidade()->getIdCampus();
		Campus* const cp = problemData->refCampus[cpId];
		Disciplina* const d = x->getDisciplina();
		int const turma = x->getTurma();
		
		unordered_map<Sala*, unordered_map<int, unordered_set<HorarioAula*> >> *ptMapSala = nullptr;
		getAulas(cp, d, turma, ptMapSala);
		if (!ptMapSala) continue;
		
		ConjuntoSala* const cjtSala = x->getSubCjtSala();
		Sala* const sala = cjtSala->getSala();
		int const dia = x->getDia();
		HorarioAula* const hi = x->getHorarioAulaInicial();
		HorarioAula* const hf = x->getHorarioAulaFinal();
		
		// Insere sala
		auto finderSala = ptMapSala->find(sala);
		if (finderSala == ptMapSala->end())
		{
			unordered_map<int, unordered_set<HorarioAula*>> empty;
			finderSala = ptMapSala->insert(
				pair<Sala*, unordered_map<int, unordered_set<HorarioAula*>>> (sala, empty)).first;		
		}
		
		// Insere dia
		auto finderDia = finderSala->second.find(dia);
		if (finderDia == finderSala->second.end())
		{
			unordered_set<HorarioAula*> empty;
			finderDia = finderSala->second.insert(
				pair<int, unordered_set<HorarioAula*>> (dia, empty)).first;
		}
		
		unordered_set<HorarioAula*> *mapHorAls = &finderDia->second;
		
		// Insere cada horario
		Calendario* const c = hi->getCalendario();
		int nrTotalCreds = c->retornaNroCreditosEntreHorarios(hi,hf);
		HorarioAula* h = hi;
		int n=0;
		while (n < nrTotalCreds && h!=nullptr)
		{
			mapHorAls->insert(h);

			h = c->getProximoHorario(h);
			n++;
		}
	}	
}

void SolverMIPUnico::extractSolutionFromMapS_()
{
	std::cout<<"\nextractSolutionFromMapS..."; fflush(0);

	for (auto itVar=solMipUnicoS_.cbegin(); itVar!=solMipUnicoS_.cend(); itVar++)
	{
		VariableMIPUnico* const s = (*itVar);
		
		Aluno* const aluno = s->getAluno();
		AlunoDemanda* const alDem = s->getAlunoDemanda();
		Campus* const cp = s->getCampus();
		Disciplina* const d = s->getDisciplina();
		int const turma = s->getTurma();

		solTurmaAlunosAloc_[cp][d][turma].insert(alDem);
	}	
}

void SolverMIPUnico::individualizaProfsVirtuais_()
{
	unordered_map<Aluno*, unordered_map<Campus*, unordered_map<Disciplina*, int>>> alunoTurmasParaIndiv;

	agrupaTurmaProfVirtPorAluno_(alunoTurmasParaIndiv);
	individualProfVirtPorAluno_(alunoTurmasParaIndiv);
}

void SolverMIPUnico::agrupaTurmaProfVirtPorAluno_(
	unordered_map<Aluno*, unordered_map<Campus*, unordered_map<Disciplina*, int>>> &alunoTurmasParaIndiv)
{	
	std::cout<<"\nagrupaTurmaProfVirtPorAluno_..."; fflush(0);

	// Para cada turma com pv
	for (auto itCp=solTurmasComPV_.cbegin(); itCp!=solTurmasComPV_.cend(); itCp++)
	{
		auto finderCp = solTurmaAlunosAloc_.find(itCp->first);
		if (finderCp == solTurmaAlunosAloc_.end())
			CentroDados::printError("SolverMIPUnico::individualizaProfsVirtuais_()","Campus nao encontrado no map de alunos!");

		for (auto itDisc=itCp->second.cbegin(); itDisc!=itCp->second.cend(); itDisc++)
		{
			auto finderDisc = finderCp->second.find(itDisc->first);
			if (finderDisc == finderCp->second.end())
				CentroDados::printError("SolverMIPUnico::individualizaProfsVirtuais_()","Dísc nao encontrada no map de alunos!");

			for (auto itTurma=itDisc->second.cbegin(); itTurma!=itDisc->second.cend(); itTurma++)
			{
				auto finderTurma = finderDisc->second.find(itTurma->first);
				if (finderTurma == finderDisc->second.end())
					CentroDados::printError("SolverMIPUnico::individualizaProfsVirtuais_()","Turma nao encontrada no map de alunos!");
				if (finderTurma->second.size()==0)
					CentroDados::printError("SolverMIPUnico::individualizaProfsVirtuais_()","Turma vazia!");

				for (auto itAl=finderTurma->second.cbegin(); itAl!=finderTurma->second.cend(); itAl++)
				{
					Aluno* const aluno = (*itAl)->getAluno();
					
					alunoTurmasParaIndiv[aluno][itCp->first][itDisc->first] = itTurma->first;
				}
			}
		}
	}
}

void SolverMIPUnico::individualProfVirtPorAluno_(
	unordered_map<Aluno*, unordered_map<Campus*, unordered_map<Disciplina*, int>>> const &alunoTurmasParaIndiv)
{
	std::cout<<"\nindividualProfVirtPorAluno_..."; fflush(0);

	for (auto itAl=alunoTurmasParaIndiv.cbegin(); itAl!=alunoTurmasParaIndiv.cend(); itAl++)
	{
		unordered_map<Campus*, unordered_map<Disciplina*, int>> turmas;
		
		// Seleciona as turmas que não possuem prof associado,
		// pois pode já ter havido alocação de prof virtual individual em caso de compartilhamento de turmas
		for (auto itCp=itAl->second.cbegin(); itCp!=itAl->second.cend(); itCp++)
		{
			for (auto itDisc=itCp->second.cbegin(); itDisc!=itCp->second.cend(); itDisc++)
			{
				int turma = itDisc->second;
				
				if (!existeProfVirt(itCp->first, itDisc->first, turma))
				{
					turmas[itCp->first][itDisc->first] = turma;
				}
			}
		}
		
		// Cria prof virtual para o aluno e o conecta às turmas correspondentes
		if (turmas.size() > 0)
		{
			Professor* pvPorAluno = criaProfessorVirtual();

			for (auto itCp=turmas.cbegin(); itCp!=turmas.cend(); itCp++)
			{
				for (auto itDisc=itCp->second.cbegin(); itDisc!=itCp->second.cend(); itDisc++)
				{
					pvPorAluno->addMagisterio(itDisc->first->getId(), 0, 10);
					associaProfATurma(itCp->first, itDisc->first, itDisc->second, pvPorAluno);
				}	
				itCp->first->professores.add(pvPorAluno);
			}
		}
	}
}

Professor* SolverMIPUnico::criaProfessorVirtual()
{	
	int idProf = - 1 * (int) problemData->professores_virtuais.size();
	
	Professor* professor = new Professor( true );   
	idProf--;
	professor->setId(idProf);
	professor->titulacao = problemData->retornaTipoTitulacaoMinimo();
	professor->setTitulacaoId( professor->titulacao->getId() );

	std::string nome = professor->getNome();
	stringstream ss; ss << idProf;
	nome += ss.str();
	professor->setNome(nome);

	//professor->setCursoAssociado(curso);

	professor->tipo_contrato = problemData->retornaTipoContratoMinimo();
	professor->setTipoContratoId(professor->tipo_contrato->getId());
		
	problemData->professores_virtuais.push_back(professor);

	return professor;
}

bool SolverMIPUnico::cadastraTurmaMapPV(Campus* const cp, Disciplina* const d, int const turma)
{	
	auto finderCp = solTurmasComPV_.find(cp);
	if (finderCp == solTurmasComPV_.end())
	{
		unordered_map<Disciplina*, unordered_map<int, Professor*>> empty;
		finderCp = solTurmasComPV_.insert(
			pair<Campus*, unordered_map<Disciplina*, unordered_map<int, Professor*>>> (cp, empty)).first;
	}

	auto finderDisc = finderCp->second.find(d);
	if (finderDisc == finderCp->second.end())
	{
		unordered_map<int, Professor*> empty;
		finderDisc = finderCp->second.insert(
			pair<Disciplina*, unordered_map<int, Professor*>> (d, empty)).first;
	}

	auto finderTurma = finderDisc->second.find(turma);
	if (finderTurma == finderDisc->second.end())
	{
		finderTurma = finderDisc->second.insert(
			pair<int, Professor*> (turma, nullptr)).first;
	}
	
	return false;
}

bool SolverMIPUnico::associaProfATurma(Campus* const cp, Disciplina* const d, int const turma, Professor* const p)
{
	bool assoc = false;
	assoc = associaProfATurmaMapPV(cp, d, turma, p);
	assoc &= associaProfATurmaMapAulas(cp, d, turma, p);
	return assoc;
}

bool SolverMIPUnico::associaProfATurmaMapPV(Campus* const cp, Disciplina* const d, int const turma, Professor* const p)
{
	if (!p->eVirtual()) return false;

	auto finderCp = solTurmasComPV_.find(cp);
	if (finderCp == solTurmasComPV_.end())
	{
		unordered_map<Disciplina*, unordered_map<int, Professor*>> empty;
		finderCp = solTurmasComPV_.insert(
			pair<Campus*, unordered_map<Disciplina*, unordered_map<int, Professor*>>> (cp, empty)).first;
	}

	auto finderDisc = finderCp->second.find(d);
	if (finderDisc == finderCp->second.end())
	{
		unordered_map<int, Professor*> empty;
		finderDisc = finderCp->second.insert(
			pair<Disciplina*, unordered_map<int, Professor*>> (d, empty)).first;
	}

	auto finderTurma = finderDisc->second.find(turma);
	if (finderTurma == finderDisc->second.end())
	{
		finderTurma = finderDisc->second.insert(
			pair<int, Professor*> (turma, nullptr)).first;
	}

	if (finderTurma->second == nullptr)
	{
		finderTurma->second = p;
		return true;
	}

	return false;
}

bool SolverMIPUnico::associaProfATurmaMapAulas(Campus* const cp, Disciplina* const d, int const turma, Professor* const p)
{
	auto finderCp = solAulas_.find(cp);
	if (finderCp == solAulas_.end())
	{
		MapDiscTurmProfSalaDiaHors empty;
		finderCp = solAulas_.insert(pair<Campus*, MapDiscTurmProfSalaDiaHors> (cp, empty)).first;
	}

	auto finderDisc = finderCp->second.find(d);
	if (finderDisc == finderCp->second.end())
	{
		MapTurmProfSalaDiaHors empty;
		finderDisc = finderCp->second.insert(pair<Disciplina*, MapTurmProfSalaDiaHors> (d, empty)).first;
	}

	auto finderTurma = finderDisc->second.find(turma);
	if (finderTurma == finderDisc->second.end())
	{
		MapProfSalaDiaHors empty;
		finderTurma = finderDisc->second.insert(pair<int, MapProfSalaDiaHors> (turma, empty)).first;
	}

	auto finderProf = finderTurma->second.find(p);
	if (finderProf == finderTurma->second.end())
	{
		if (finderTurma->second.size() > 0)
			CentroDados::printError("SolverMIPUnico::associaProfATurmaMapAulas", "Mais de um professor associado aa turma!!");

		MapSalaDiaHors empty;
		finderProf = finderTurma->second.insert(pair<Professor*, MapSalaDiaHors> (p, empty)).first;				
		return true;
	}

	return false;
}

bool SolverMIPUnico::existeProfVirt(Campus* const cp, Disciplina* const d, int const turma) const
{
	auto finderCp = solTurmasComPV_.find(cp);
	if (finderCp != solTurmasComPV_.end())
	{
		auto finderDisc = finderCp->second.find(d);
		if (finderDisc != finderCp->second.end())
		{
			auto finderTurma = finderDisc->second.find(turma);
			if (finderTurma != finderDisc->second.end())
			{
				if (finderTurma->second != nullptr)
					return true;
			}
		}
	}
	return false;
}

void SolverMIPUnico::contabilizaGapProfReal_()
{
	mapSolutionProfReal_();
	countGapProfReal_();
}

void SolverMIPUnico::mapSolutionProfReal_()
{
	for(auto itCampus = solAulas_.cbegin(); itCampus != solAulas_.cend(); ++itCampus)
	{
		Campus* const campus = itCampus->first;
		
		for(auto itDisc = itCampus->second.cbegin(); itDisc != itCampus->second.cend(); ++itDisc)
		{
			Disciplina* const disciplina = itDisc->first;
		
			for(auto itTurmas = itDisc->second.cbegin(); itTurmas != itDisc->second.cend(); ++itTurmas)
			{		
				const int turmaId = itTurmas->first;
				
				for(auto itProf = itTurmas->second.cbegin(); itProf != itTurmas->second.cend(); ++itProf)
				{
					Professor* const professor = itProf->first;					
					if (professor->eVirtual()) continue;

					auto ptSolProf = & solProfRealAloc_[professor];

					for(auto itSala = itProf->second.cbegin(); itSala != itProf->second.cend(); ++itSala)
					{
						Sala* const sala = itSala->first;
						int unidadeId = sala->getIdUnidade();
						Unidade* const unid = problemData->refUnidade[unidadeId];

						for(auto itAulas = itSala->second.cbegin(); itAulas != itSala->second.cend(); ++itAulas)
						{
							int dia = itAulas->first;

							auto ptSolProfDia = &(*ptSolProf)[dia];

							for(auto itHor = itAulas->second.cbegin(); itHor != itAulas->second.cend(); ++itHor)
							{
								HorarioAula* const h = *itHor;
								
								std::pair<DateTime,Unidade*> parDtfUnid (h->getFinal(), unid);
								bool added = ptSolProfDia->insert(
									pair<DateTime, pair<DateTime,Unidade*>> (h->getInicio(), parDtfUnid)).second;

								if (!added) CentroDados::printError("SolverMIPUnico::mapSolutionProfReal_()",
									"Aula de prof nao adicionada!");
							}
						}
					}
				}
			}
		}
	}	
}

void SolverMIPUnico::countGapProfReal_()
{
	int nrTotalGap = 0;
	int nrGapIgnoraDesloc = 0;

	for(auto itProf = solProfRealAloc_.cbegin(); itProf != solProfRealAloc_.cend(); ++itProf)
	{
		Professor* const professor = itProf->first;					
		if (professor->eVirtual()) continue;		

	//	cout << "\nProf" << professor->getId();

		for(auto itDia = itProf->second.cbegin(); itDia != itProf->second.cend(); ++itDia)
		{
		//	cout << "\n\tDia" << itDia->first;

			for(auto itDti = itDia->second.cbegin(); *itDti != *itDia->second.crbegin(); ++itDti)
			{
				DateTime dti = itDti->first;
				DateTime dtf = itDti->second.first;
				Unidade* const unid = itDti->second.second;
				int faseDoDia = CentroDados::getFaseDoDia(dti);

				auto itNextDti = std::next(itDti);
				DateTime nextDti = itNextDti->first;
				Unidade* const nextUnid = itNextDti->second.second;
				int nextFaseDoDia = CentroDados::getFaseDoDia(nextDti);				
				
			//	cout << "\n\t\tDtf " << dtf.hourMinToStr() << "   NextDti " << nextDti.hourMinToStr();

				if (faseDoDia != nextFaseDoDia) continue;

				int difMin = (nextDti - dtf).getDateMinutes();
				int nr = difMin / 50;

			//	cout << "\n\t\tDif = " << difMin << "   nr = " << nr;

				nrTotalGap += nr;
				if (unid == nextUnid)
					nrGapIgnoraDesloc += nr;
			}
		}
	}

	Indicadores::printSeparator(1);
	Indicadores::printIndicador("\nNumero total de gaps de professores: ", nrTotalGap);
	Indicadores::printIndicador("\nNumero total de gaps de professores ignorando deslocamentos: ", nrGapIgnoraDesloc);
}

void SolverMIPUnico::getAulas(Campus* const cp, Disciplina* const d, int const turma,
	unordered_map<Sala*, unordered_map<int, unordered_set<HorarioAula*> >> * &ptMapSala)
{
	auto finderCp = solAulas_.find(cp);
	if (finderCp != solAulas_.end())
	{
		auto finderDisc = finderCp->second.find(d);
		if (finderDisc != finderCp->second.end())
		{
			auto finderTurma = finderDisc->second.find(turma);
			if (finderTurma != finderDisc->second.end())
			{
				auto *itProf = &finderTurma->second;
				if (itProf->size() != 1)
				{
					if (itProf->size() > 1)
						CentroDados::printError("SolverMIPUnico::getAulas",
						"Turma com mais de um professor no mapeamento de aulas!");
					if (itProf->size() == 0)
						CentroDados::printError("SolverMIPUnico::getAulas",
							"Turma sem professores no mapeamento de aulas!");
					return;
				}

				ptMapSala = & itProf->begin()->second;
				return;
			}
		}
	}
	CentroDados::printWarning("SolverMIPUnico::getAulas", "Turma nao encontrada no mapeamento de aulas!");
}

void SolverMIPUnico::getAlDemsAlocados(Campus* const cp, Disciplina* const d, 
						int const turma, unordered_set<AlunoDemanda*> &alDems) const
{
	auto finderCp = solTurmaAlunosAloc_.find(cp);
	if (finderCp != solTurmaAlunosAloc_.cend())
	{
		auto finderDisc = finderCp->second.find(d);
		if (finderDisc != finderCp->second.cend())
		{
			auto finderTurma = finderDisc->second.find(turma);
			if (finderTurma != finderDisc->second.cend())
			{
				for (auto itAlDem=finderTurma->second.cbegin(); itAlDem!=finderTurma->second.cend(); itAlDem++)
				{
					alDems.insert(*itAlDem);
				}
				return;
			}
		}
	}
	CentroDados::printWarning("SolverMIPUnico::getAlDemsAlocados", "Turma nao encontrada no mapeamento de alunos!");
}

// cria o output dos atendimentos
void SolverMIPUnico::criarOutputFinal_(ProblemSolution* const solution) const
{
	cout << "Criando output op de atendimento da solução... "; fflush(0);
	if (!solution)
	{
		CentroDados::printWarning("SolverMIPUnico::criarOutputNovo_", "Solucao null!");
		return;
	}
	if (solAulas_.size()==0)
	{
		CentroDados::printWarning("SolverMIPUnico::criarOutputNovo_", "Map de solucao vazio!");
		return;
	}
	if (solTurmaAlunosAloc_.size()==0)
	{
		CentroDados::printWarning("SolverMIPUnico::criarOutputNovo_", "Map de alocacao de alunos vazio!");
		return;
	}
	
	// Cenário id
	solution->setCenarioId( problemData->getCenarioId() );
	
	// CAMPUS
	for(auto itCampus = solAulas_.cbegin(); itCampus != solAulas_.cend(); ++itCampus)
	{
		Campus* const campus = itCampus->first;
		AtendimentoCampus* const atendCampus = solution->getAddAtendCampus(campus->getId());
		atendCampus->campus = campus;
		atendCampus->setCampusId(campus->getCodigo());
		
		// DISCIPLINAS / OFERTAS
		for(auto itDisc = itCampus->second.cbegin(); itDisc != itCampus->second.cend(); ++itDisc)
		{
			Disciplina* const disciplina = itDisc->first;
		
			// TURMAS
			for(auto itTurmas = itDisc->second.cbegin(); itTurmas != itDisc->second.cend(); ++itTurmas)
			{		
				const int turmaId = itTurmas->first;

				unordered_set<AlunoDemanda*> alsDemAlocados;
				getAlDemsAlocados(campus, disciplina, turmaId, alsDemAlocados);

				unordered_map<Demanda*, unordered_set<AlunoDemanda*>> mapDemAlDems;
				for(auto itAlDem = alsDemAlocados.cbegin(); itAlDem != alsDemAlocados.cend(); ++itAlDem)
				{
					mapDemAlDems[(*itAlDem)->demanda].insert(*itAlDem);
				}

				criarTurmaOutput_(*atendCampus, disciplina, turmaId, itTurmas->second, mapDemAlDems);
			}
		}
	}

	criarOutProfsVirtuais_(solution);

	cout << "Output atendimento criado!";
}

void SolverMIPUnico::criarTurmaOutput_(AtendimentoCampus &atendCampus, Disciplina* const disciplina, int turmaId, 
	   unordered_map<Professor*, unordered_map<Sala*, unordered_map<int,
	   unordered_set<HorarioAula*>>> > const & mapTurma,
	   unordered_map<Demanda*, unordered_set<AlunoDemanda*>> const &mapDemAlDems) const
{		
	if (mapTurma.size()==0)
		CentroDados::printWarning("SolverMIPUnico::criarTurmaOutput_", "Map de prof da turma vazio!");

	for(auto itProf = mapTurma.cbegin(); itProf != mapTurma.cend(); ++itProf)
	{
		Professor* const professor = itProf->first;
	
		if (itProf->second.size()==0)
			CentroDados::printWarning("SolverMIPUnico::criarTurmaOutput_", "Map de sala da turma vazio!");

		for(auto itSala = itProf->second.cbegin(); itSala != itProf->second.cend(); ++itSala)
		{
			Sala* const sala = itSala->first;

			// get atendimento unidade
			int unidadeId = sala->getIdUnidade();
			AtendimentoUnidade* const atendUnid = atendCampus.getAddAtendUnidade(unidadeId);
			auto itUnid = problemData->refUnidade.find(unidadeId);
			if(itUnid != problemData->refUnidade.end())
			{
				atendUnid->unidade = itUnid->second;
				atendUnid->setCodigoUnidade(itUnid->second->getCodigo());
			}
			else
			{
				CentroDados::printError("SolverMIPUnico::criarTurmaOutput_", "Unidade nao encontrada");
				continue;
			}

			// get atendimento sala
			AtendimentoSala* const atendSala = atendUnid->getAddAtendSala(sala->getId());
			atendSala->sala = sala;
			atendSala->setSalaId(sala->getCodigo());
			
			// aulas
			if (itSala->second.size()==0)
				CentroDados::printWarning("SolverMIPUnico::criarTurmaOutput_", "Map de aulas da turma vazio!");

			for(auto itAulas = itSala->second.cbegin(); itAulas != itSala->second.cend(); ++itAulas)
			{
				int dia = itAulas->first;

				criarAulasOutput_(*atendSala, disciplina, turmaId, professor, dia, itAulas->second, mapDemAlDems);
			}
		}
	}
}

void SolverMIPUnico::criarAulasOutput_(AtendimentoSala &atendSala, Disciplina* const disciplina, int turmaId,
	Professor* const professor, int dia, unordered_set<HorarioAula*> const &mapHorAlDems,
	unordered_map<Demanda*, unordered_set<AlunoDemanda*>> const &mapDemAlDems) const
{	
	// get atendimento diaSemana
	AtendimentoDiaSemana* const atendDia = atendSala.getAddAtendDiaSemana(dia);
	
	// HORARIOS
	for(auto itHor = mapHorAlDems.cbegin(); itHor != mapHorAlDems.cend(); ++itHor)
	{			
		HorarioAula* const h = *itHor;
		
		criarAulaPorOfertaOutput_(*atendDia, disciplina, turmaId, professor, h, dia, mapDemAlDems);
	}
}

void SolverMIPUnico::criarAulaPorOfertaOutput_(AtendimentoDiaSemana &atendDia, Disciplina* const disciplina, int turmaId,
	Professor* const professor, HorarioAula* const hBase, int dia,
	unordered_map<Demanda*, unordered_set<AlunoDemanda*>> const &mapDemAlDems) const
{	
	for(auto itDem = mapDemAlDems.cbegin(); itDem != mapDemAlDems.cend(); ++itDem)
	{
		Demanda* const demanda = itDem->first;

		Oferta* const oferta = demanda->oferta;
		TurnoIES* const turno = oferta->turno;
		Calendario* const calendario = demanda->getCalendario();

		HorarioAula* const horario = turno->getHorarioDiaOuCorrespondente(calendario, hBase, dia);
		if(!horario)
		{
			stringstream msg;
			msg << "Horario " << hBase->getInicio() << " nao encontrado no dia " << dia 
				<< " em turno " << turno->getId() << " e calendario " << calendario->getId();
			CentroDados::printError("SolverMIPUnico::criarAulaPorOfertaOutput_",msg.str());
			return;
		}

		const bool credTeor = (disciplina->getId() > 0);

		// get atendimento turno
		AtendimentoTurno* const atendTurno = atendDia.getAddAtendTurno(turno->getId());
		atendTurno->turno = turno;
		atendTurno->setTurnoId(turno->getId());

		AtendimentoHorarioAula* const atendHor = atendTurno->getAddAtendHorarioAula(horario->getId());
		// set informação
		atendHor->horario_aula = horario;
		atendHor->setProfessorId(professor->getId());
		atendHor->professor = professor;
		atendHor->setProfVirtual(professor->eVirtual());
		atendHor->setCreditoTeorico(credTeor);

		AtendimentoOferta* const atendOferta = atendHor->getAddAtendOferta(oferta->getId());
		// set informação
		stringstream ss;
		ss << oferta->getId();
		atendOferta->setOfertaCursoCampiId(ss.str());
		// set disciplina da alocação
		atendOferta->disciplina = disciplina;
		// set disciplina original
		atendOferta->setDisciplinaId( abs(demanda->getDisciplinaId()) );
		// get disciplina substituta (caso equivalencia)
		if(demanda->getDisciplinaId() != disciplina->getId())
			atendOferta->setDisciplinaSubstitutaId( abs(disciplina->getId()) );
		// set nr alunos
		atendOferta->addQuantidade(itDem->second.size());
		// set turma
		atendOferta->setTurma(turmaId);

		for(auto itAlDem = itDem->second.cbegin(); itAlDem != itDem->second.cend(); ++itAlDem)
		{
			AlunoDemanda* const alDem = *itAlDem;
			atendOferta->alunosDemandasAtendidas.add(alDem->getId());
		}
	}
}

// cria o output de professores virtuais
void SolverMIPUnico::criarOutProfsVirtuais_(ProblemSolution* const solution) const
{
	cout << "\ncriarOutProfsVirtuais_...";

	GGroup<ProfessorVirtualOutput*>* profsVirtuaisOutput = solution->professores_virtuais;

	// iterar profs virtuais
	for(auto itCp = solTurmasComPV_.cbegin(); itCp != solTurmasComPV_.cend(); ++itCp)
	{
		for(auto itDisc = itCp->second.cbegin(); itDisc != itCp->second.cend(); ++itDisc)
		{
			for(auto itTurma = itDisc->second.cbegin(); itTurma != itDisc->second.cend(); ++itTurma)
			{
				// criar output prof virtual único (se ele tiver turmas)
				Professor* const pv = itTurma->second;
				int virtualId = pv->getId();
				
				ProfessorVirtualOutput* profVirtualOut = solution->getProfVirtualOutput(virtualId);
				
				if (!profVirtualOut)
				{
					profVirtualOut = new ProfessorVirtualOutput(virtualId);
					if(pv->titulacao != nullptr)
						profVirtualOut->setTitulacaoId(pv->getTitulacaoId());
					if(pv->tipo_contrato != nullptr)
						profVirtualOut->setContratoId(pv->getTipoContratoId());
					profsVirtuaisOutput->add(profVirtualOut);
				}

				int disciplina = itDisc->first->getId();
				int turmaNr = itTurma->first;
				int campus = itCp->first->getId();
				bool ehPrat = (disciplina < 0);
		
				AlocacaoProfVirtual* alocacao = new AlocacaoProfVirtual(disciplina, turmaNr, campus, ehPrat);
				
				bool jaExiste=false;
				if (profVirtualOut->alocacoes.size() > 0)
					jaExiste=true;
				
				profVirtualOut->alocacoes.add(alocacao);
			}
		}
	}
}