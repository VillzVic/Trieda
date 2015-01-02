#include "ProblemSolution.h"
#include "ErrorHandler.h"
#include "NaoAtendimento.h"
#include "GGroup.h"
#include "InputMethods.h"

#include "Indicadores.h"
#include "CentroDados.h"
#include "ProblemData.h"
#include "AtendimentoHorarioAula.h"
#include "AlocacaoProfVirtual.h"
#include "AlunoSolution.h"



ProblemSolution::ProblemSolution( bool _modoOtmTatico )
   : modoOtmTatico( _modoOtmTatico )
{
   folgas = new RestricaoVioladaGroup();
   atendimento_campus = new GGroup< AtendimentoCampus * >();
   professores_virtuais = new GGroup< ProfessorVirtualOutput * >();
   alunosDemanda = new GGroup< AlunoDemanda *, LessPtr< AlunoDemanda > >();
   nao_atendimentos = new GGroup< NaoAtendimento *, LessPtr< NaoAtendimento > >();
   idsAtendimentos = 1;
   cenarioId = 0;
	
   nroTotalAlunoDemandaP1=0;
   nroAlunoDemAtendP1=0;
   nroAlunoDemNaoAtendP1=0;
   nroAlunoDemAtendP2=0;
   nroAlunoDemAtendP1P2=0;
   nrMaxDiscSimult_=0;
}

ProblemSolution::~ProblemSolution()
{
	clearMapsDaSolucao();

   if ( folgas )
   {
      folgas->deleteElements();
      delete folgas;
   }

   if( atendimento_campus )
   {
      atendimento_campus->deleteElements();
      delete atendimento_campus;
   }
   
   if ( professores_virtuais )
   {
      professores_virtuais->deleteElements();
      delete professores_virtuais;
   }

   // alunosDemanda: dados de ProblemData, não deletar.

   if ( nao_atendimentos )
   {
      nao_atendimentos->deleteElements();
      delete nao_atendimentos;
   }
}

void ProblemSolution::resetProblemSolution()
{
   if ( folgas != NULL )
   {
      folgas->deleteElements();
      delete folgas;
   }
   if( atendimento_campus != NULL )
   {
      atendimento_campus->deleteElements();
      delete atendimento_campus;
   }
   if ( professores_virtuais != NULL )
   {
      professores_virtuais->deleteElements();
      delete professores_virtuais;
   }
   if ( alunosDemanda != NULL )
   {
	  // alunosDemanda: dados de ProblemData, não deletar.
      delete alunosDemanda;
   }
   if ( nao_atendimentos != NULL )
   {
      nao_atendimentos->deleteElements();
      delete nao_atendimentos;
   }

   folgas = new RestricaoVioladaGroup();
   atendimento_campus = new GGroup< AtendimentoCampus * >();
   professores_virtuais = new GGroup< ProfessorVirtualOutput * >();
   alunosDemanda = new GGroup< AlunoDemanda *, LessPtr< AlunoDemanda > >();
   nao_atendimentos = new GGroup< NaoAtendimento *, LessPtr< NaoAtendimento > >();
   idsAtendimentos = 1;
}

ProfessorVirtualOutput* ProblemSolution::getProfVirtualOutput( int id )
{
	ProfessorVirtualOutput *pvo=nullptr;

	auto itFinder = this->professores_virtuais->begin();
	for ( ; itFinder != this->professores_virtuais->end(); itFinder++ )
	{
		if ( itFinder->getId() == id )
		{
			pvo = *itFinder;
			break;
		}
	}

	return pvo;
}

map< Sala*, vector< map<int /*dia*/, GGroup<HorarioAula*, LessPtr<HorarioAula>> > >, LessPtr<Sala> >
	ProblemSolution::procuraCombinacaoLivreEmSalas( Disciplina *disciplina, TurnoIES* turno, int campusId )
{
	map< Sala*, vector< map<int /*dia*/, GGroup<HorarioAula*, LessPtr<HorarioAula>> > >, LessPtr<Sala> >
		mapSalaCombinacoesLivres;

	std::map< int, ConjuntoSala* >::iterator it_conjunto_sala = disciplina->cjtSalasAssociados.begin();
	for ( ; it_conjunto_sala != disciplina->cjtSalasAssociados.end(); it_conjunto_sala++ )
	{
		Sala *sala = (*it_conjunto_sala).second->getSala();

		if ( sala->getIdCampus() != campusId )
			continue;

		vector< map<int /*dia*/, GGroup<HorarioAula*, LessPtr<HorarioAula>> > > 
			opcoesLivresNaSala = this->procuraCombinacaoLivreNaSala( disciplina, turno, sala );
		if ( opcoesLivresNaSala.size() != 0 )
		{
			mapSalaCombinacoesLivres[sala] = opcoesLivresNaSala;
		}
	}

	return mapSalaCombinacoesLivres;
}

vector< map<int /*dia*/, GGroup<HorarioAula*, LessPtr<HorarioAula>> > > 
	ProblemSolution::procuraCombinacaoLivreNaSala( Disciplina *disciplina, TurnoIES* turno, Sala *sala )
{
	bool haSalaDisponivel = false;

	bool debug=false;
	if ( disciplina->getId() == 14290 && sala->getId() == 2016 && turno->getId() == 38 )
	{
		//std::cout<<"\n\nDEBUG\n";
		//debug = true;
	}

	vector< map<int /*dia*/, GGroup<HorarioAula*, LessPtr<HorarioAula>> > > mapCombinacoesLivres;

	std::map<int /*dia*/, std::map<DateTime /*dti*/, std::map<DateTime /*dtf*/, GGroup<HorarioAula*, LessPtr<HorarioAula> > > > > 
		*map_dia_horarios_vagos = &mapSalaDiaHorariosVagos[sala];

	if ( debug )
	{
		if ( map_dia_horarios_vagos->size() == 0 ) 
			std::cout<<"\nSala cheia.";
		else
			std::cout<<"\nHors livres da sala:";
			std::map<int, std::map<DateTime /*dti*/, std::map<DateTime /*dtf*/, GGroup<HorarioAula*, LessPtr<HorarioAula> > > > >::iterator 
				itMap1 = map_dia_horarios_vagos->begin();
			for ( ; itMap1 != map_dia_horarios_vagos->end(); itMap1++ )
			{
				cout<< "\nDia " << itMap1->first;
				std::map<DateTime /*dti*/, std::map<DateTime /*dtf*/, GGroup<HorarioAula*, LessPtr<HorarioAula> > > >::iterator
					itMapDti = itMap1->second.begin();
				for ( ; itMapDti != itMap1->second.end(); itMapDti++ )
				{
					DateTime dti = itMapDti->first;
					std::map<DateTime /*dtf*/, GGroup<HorarioAula*, LessPtr<HorarioAula>> >::iterator
						itMapDtf = itMapDti->second.begin();
					for ( ; itMapDtf != itMapDti->second.end(); itMapDtf++ )
					{
						DateTime dtf = itMapDtf->first;
						cout<< "   " << dti.getHour() << ":" << dti.getMinute();
						cout<< " - " << dtf.getHour() << ":" << dtf.getMinute();
					}
				}
			}
	}

	std::vector< std::vector< std::pair< int /*dia*/, int /*numCreditos*/ > > >::iterator
		it_combinacao_creditos = disciplina->combinacao_divisao_creditos.begin();
	for ( ; it_combinacao_creditos != disciplina->combinacao_divisao_creditos.end() ; it_combinacao_creditos++ )
	{
		bool combinacaoOK = true;
		map<int /*dia*/, GGroup<HorarioAula*, LessPtr<HorarioAula>> > opcaoValida;
		
		if ( debug )
			std::cout<<"\n\nCombinacao: ";

		std::vector< std::pair< int /*dia*/, int /*numCreditos*/ > >::iterator it_creditos = (*it_combinacao_creditos).begin();
		for ( ; it_creditos != (*it_combinacao_creditos).end() ; it_creditos++ )
		{
			int dia_credito = (*it_creditos).first;
			int n_credito = (*it_creditos).second;
			bool blocoCred = false;
			
			if ( debug )
				std::cout<<"\ndia " << dia_credito << "- n = " << n_credito;

			if ( n_credito > 0 )
			{
				// PERCORRE HORARIOS LIVRES DA SALA NO DIA

				std::map<int, std::map<DateTime /*dti*/, std::map<DateTime /*dtf*/, GGroup<HorarioAula*, LessPtr<HorarioAula> > > > >::iterator 
					itMap1 = map_dia_horarios_vagos->find( dia_credito );
				if ( itMap1 != map_dia_horarios_vagos->end() )
				{
					std::map<DateTime /*dti*/, std::map<DateTime /*dtf*/, GGroup<HorarioAula*, LessPtr<HorarioAula> > > >::iterator
						itMapDti = itMap1->second.begin();
					for ( ; itMapDti != itMap1->second.end(); itMapDti++ )
					{
						DateTime dti = itMapDti->first;
						std::map<DateTime /*dtf*/, GGroup<HorarioAula*, LessPtr<HorarioAula>> >::iterator
							itMapDtf = itMapDti->second.begin();
						for ( ; itMapDtf != itMapDti->second.end(); itMapDtf++ )
						{
							DateTime dtf = itMapDtf->first;
							HorarioAula *horarioAula = *(itMapDtf->second.begin());
								
							GGroup<HorarioAula*, LessPtr<HorarioAula>> horsValidosAPartirDeHi;
							
							if ( debug ) std::cout<<"\n-->Hi = (" << horarioAula->getInicio() << " - " << horarioAula->getFinal() << ")";

							if ( turno->possuiHorarioDiaOuCorrespondente(horarioAula,dia_credito) )
							{
								horsValidosAPartirDeHi.add( horarioAula );
							}
							else
							{
								if ( debug ) std::cout<<"\n\tTurno NAO possui " << horarioAula->getInicio() << " no dia " << dia_credito;
								continue;
							}

							bool hInicialComNCreditosOK = true;

							#pragma region CONFERE SE OS N HORÁRIOS DO BLOCO SÃO VÁLIDOS NO DIA NOS HORÁRIOS LIVRES DA SALA E NO TURNO
							HorarioAula* h = horarioAula;
							for ( int i=2; i<=n_credito && hInicialComNCreditosOK; i++ )
							{
								h = h->getCalendario()->getProximoHorario( h );

								if ( h == NULL )
								{
									hInicialComNCreditosOK = false;
									break;
								}

								DateTime dti_next = h->getInicio();
								DateTime dtf_next = h->getFinal();

								// CONFERE SE O HORARIO-DIA ESTÁ DISPONIVEL NA SALA

								bool achouNaSala=false;
								std::map<DateTime /*dti*/, std::map<DateTime /*dtf*/, GGroup<HorarioAula*, LessPtr<HorarioAula> > > >::iterator
									itMapDtiProx = itMapDti;
								for ( ; itMapDtiProx != itMap1->second.end(); itMapDtiProx++ )
								{
									if ( itMapDtiProx->first > dti_next )
										break;
									if ( itMapDtiProx->first == dti_next )
									{
										std::map<DateTime /*dtf*/, GGroup<HorarioAula*, LessPtr<HorarioAula>> >::iterator
											itMapDtfProx = itMapDtiProx->second.begin();
										for ( ; itMapDtfProx != itMapDtiProx->second.end(); itMapDtfProx++ )
										{
											if ( itMapDtfProx->first > dtf_next )
												break;
											if ( itMapDtfProx->first == dtf_next )
											{
												achouNaSala = true;
												break;
											}
										}
									}
								}

								if ( !achouNaSala ){ hInicialComNCreditosOK = false; if ( debug ) std::cout<<"\n\tNao achou na sala";}
									
								// CONFERE SE O HORARIO-DIA ESTÁ DISPONIVEL NO TURNO

								if ( hInicialComNCreditosOK )
								{
									if ( turno->possuiHorarioDiaOuCorrespondente(h,dia_credito) )
									{
										horsValidosAPartirDeHi.add( horarioAula );
									}
									else
									{
										hInicialComNCreditosOK = false;
										if ( debug ) 
											std::cout<<"\n\tTurno NAO possui " << horarioAula->getInicio() << " no dia " << dia_credito;
									}
								}
							}
							#pragma endregion

							// SE OS HORARIOS-DIAS ESTÃO DISPONIVEIS NA SALA E NO TURNO, CONFERE SE É UMA AULA VÁLIDA NA DISCIPLINA
							if ( hInicialComNCreditosOK )
							{
								if ( disciplina->inicioTerminoValidos( horarioAula, h, dia_credito ) )
								{
									if ( debug ) std::cout<<"\n\tInicio fim validos!";
									blocoCred = true;
									opcaoValida[dia_credito].add( horsValidosAPartirDeHi );
								}
								else if ( debug ) std::cout<<"\n\tInicio fim nao validos";
							}
						}
					}
				}
			}
			else
			{
				blocoCred = true;
			}

			if ( !blocoCred )
			{
				combinacaoOK = false;
				break;
			}
		}
		if(combinacaoOK)
		{
			mapCombinacoesLivres.push_back( opcaoValida );
			haSalaDisponivel = true;
		}
	}

	return mapCombinacoesLivres;
}

void ProblemSolution::procuraOpcoesSemChoque( 
	const map< int /*opcao*/, map<int /*dia*/, GGroup<HorarioAula*, LessPtr<HorarioAula>> > > &semChoquesNaSala, 
	const map< TurnoIES*, GGroup<AlunoDemanda*, LessPtr<AlunoDemanda>>, LessPtr<TurnoIES> > &mapAlsDemNaoAtend, 
	map< int /*opção*/, GGroup<AlunoDemanda*, LessPtr<AlunoDemanda>> > &mapOpcaoAlunosSemChoque )
{
	// Percorre as opções de conjuntos de horários
	auto itOpcao = semChoquesNaSala.begin();
	for ( ; itOpcao != semChoquesNaSala.end(); itOpcao++ )
	{
		int opcaoIdx = itOpcao->first;
		
		// Percorre Alunos não atendidos na disciplina
		auto itTurnoNaoAtend = mapAlsDemNaoAtend.begin();
		for ( ; itTurnoNaoAtend != mapAlsDemNaoAtend.end(); itTurnoNaoAtend++ )
		{
			ITERA_GGROUP_LESSPTR( itAlDemNaoAtend, itTurnoNaoAtend->second, AlunoDemanda )
			{
				AlunoDemanda *alDem = *itAlDemNaoAtend;
				Aluno *aluno = alDem->getAluno();

				bool semChoque=true;

				AlunoSolution *alunoSol = this->getAlunoSolution( aluno );
				if ( alunoSol!=nullptr )
				{
					// Percorre os horarios da opção corrente para verificar se o aluno tem choque em suas alocações 
					
					for ( auto itDiaOpcao = itOpcao->second.begin(); 
							itDiaOpcao != itOpcao->second.end() && semChoque; itDiaOpcao++ )
					{
						int dia = itDiaOpcao->first;

						map< DateTime /*dti*/, map< DateTime /*dtf*/, map< Disciplina*, Aula*, 
							LessPtr<Disciplina> > > > *mapOcupacaoNoDia = alunoSol->getMapAulasNoDia( dia );

						if ( mapOcupacaoNoDia == nullptr )
							continue;

						ITERA_GGROUP_LESSPTR( itHorOpcao, itDiaOpcao->second, HorarioAula )
						{
							HorarioAula *h_op = *itHorOpcao;
							DateTime dti_op = h_op->getInicio();
							DateTime dtf_op = h_op->getFinal();

							auto itDtiOcup = mapOcupacaoNoDia->begin();
							for ( ; itDtiOcup != mapOcupacaoNoDia->end() && semChoque; itDtiOcup++ )
							{
								DateTime dti_ocup = itDtiOcup->first;
								
								if ( dtf_op <= dti_ocup )
									break;

								if ( dti_ocup >= dti_op && dti_ocup < dtf_op )
									semChoque=false;

								auto itDtfOcup = itDtiOcup->second.begin();
								for ( ; itDtfOcup != itDtiOcup->second.end() && semChoque; itDtfOcup++ )
								{
									DateTime dtf_ocup = itDtfOcup->first;

									if ( (dti_ocup >= dti_op && dti_ocup < dtf_op) ||
										 (dti_op >= dti_ocup && dti_op < dtf_ocup) )
									{
										semChoque=false;	// Achou choque
									}
								}
							}

							if ( !semChoque ) break;
						}
					}

				}

				if ( semChoque )
				{
					(mapOpcaoAlunosSemChoque[opcaoIdx]).add( alDem );
				}
			}
		}
	}

}

int ProblemSolution::retornaTurmaDiscAluno( AlunoDemanda* alunoDemanda, bool teorica )
{
	int turma = -1;
	auto itAluno = this->mapAlunoSolution.find( alunoDemanda->getAluno() );
	if ( itAluno != this->mapAlunoSolution.end() )
	{
		AlunoSolution *alSol = itAluno->second;
		turma = alSol->getTurma( alunoDemanda, teorica );
	}
	return turma;
}

bool ProblemSolution::getAlunoSolution( Aluno* aluno, AlunoSolution *& alunoSolution )
{
	bool ehNovo = false;

	auto itFinderAluno = mapAlunoSolution.find(aluno);
	if ( itFinderAluno != mapAlunoSolution.end() )
	{
		alunoSolution = itFinderAluno->second;
	}
	else
	{
		alunoSolution = new AlunoSolution( aluno );
		ehNovo = true;
	}

	return ehNovo;
}

AlunoSolution* ProblemSolution::getAlunoSolution( Aluno* aluno )
{
	AlunoSolution* alunoSolution = nullptr;

	auto itFinderAluno = mapAlunoSolution.find(aluno);
	if ( itFinderAluno != mapAlunoSolution.end() )
	{
		alunoSolution = itFinderAluno->second;
	}

	return alunoSolution;
}


void ProblemSolution::constroiMapsDaSolucao()
{
	std::cout << "\nconstroiMapsDaSolucao..."; fflush(0);

	ProblemData *problemData = CentroDados::getProblemData();
		
	// ------------------------------------------------------------------------------------------
	// Indicadores

	int nroCredsProfsVirtuais=0;
	int nroCredsProfsReais=0;
	double chProfsReal=0;
	double chProfsVirt=0;
	
	// ------------------------------------------------------------------------------------------
	// Inicializa zerados os creditos já alocados aos alunos

	ITERA_GGROUP_LESSPTR( itAluno, problemData->alunos, Aluno )
	{
		itAluno->setNroCreditosJaAlocados(0);
	}


	// ------------------------------------------------------------------------------------------
	// Conjunto de aulas para cada turma criada

	mapSolDiscTurmaDiaAula.clear();

	map< int, map< Disciplina*, map<int, int> > > tempCpDiscTurmaPVId; // < campusId, < disc, <turma,pvId> > >

	std::cout << " lendo atendimentos..."; fflush(0);

	if ( this->atendimento_campus != nullptr )
	{
		ITERA_GGROUP( it_At_Campus, ( *this->atendimento_campus ), AtendimentoCampus )
	    {
		    // Campus do atendimento	   
		    Campus *campus = it_At_Campus->campus;
		    int campusId = campus->getId();
	   
		    if ( it_At_Campus->atendimentos_unidades == nullptr )
		    {
			   std::cout << "\nProvavel erro! atendimentos_unidades null em ProblemSolution.\n";
			   continue;
		    }

			ITERA_GGROUP_LESSPTR( it_At_Unidade,
				( *it_At_Campus->atendimentos_unidades ), AtendimentoUnidade )
			{
				// Unidade do atendimento
				Unidade *unidade = problemData->refUnidade[ it_At_Unidade->getId() ];
	   				
			    if ( it_At_Unidade->atendimentos_salas == nullptr )
			    {
				   std::cout << "\nProvavel erro! atendimentos_salas null em ProblemSolution.\n";
				   continue;
			    }

				ITERA_GGROUP_LESSPTR( it_At_Sala,
					( *it_At_Unidade->atendimentos_salas ), AtendimentoSala )
				{
					// Sala do atendimento
					Sala *sala = problemData->refSala[ it_At_Sala->getId() ];
			   				
					if ( it_At_Sala->atendimentos_dias_semana == nullptr )
					{
					   std::cout << "\nProvavel erro! atendimentos_dias_semana null em ProblemSolution.\n";
					   continue;
					}

					ITERA_GGROUP_LESSPTR( it_At_DiaSemana, 
						( *it_At_Sala->atendimentos_dias_semana ), AtendimentoDiaSemana )
					{
						// Dia da semana do atendimento
						int dia = it_At_DiaSemana->getDiaSemana();
			    			   				
						if ( it_At_DiaSemana->atendimentos_turno == nullptr )
						{
						   std::cout << "\nProvavel erro! atendimentos_turno null em ProblemSolution.\n";
						   continue;
						}

						ITERA_GGROUP_LESSPTR( it_At_Turno,
							( *it_At_DiaSemana->atendimentos_turno ), AtendimentoTurno )
						{	
							// Turno do atendimento
							TurnoIES* turno = it_At_Turno->turno;

							if ( it_At_Turno->atendimentos_horarios_aula == nullptr )
							{
							   std::cout << "\nProvavel erro! atendimentos_horarios_aula null em ProblemSolution.\n";
							   continue;
							}
							
							ITERA_GGROUP_LESSPTR( it_At_Hor_Aula,
								( *it_At_Turno->atendimentos_horarios_aula ), AtendimentoHorarioAula )
							{
								// Horario do atendimento
								int horAulaId = it_At_Hor_Aula->getHorarioAulaId();
								HorarioAula* horAula = it_At_Hor_Aula->horario_aula;

								// Valida horario-dia alocado na sala						   		    
								if ( ! sala->possuiHorariosNoDia( horAula, horAula, dia ) )
								{
									stringstream msg;
									msg << "Solucao usa horario invalido " << horAula->getInicio().hourMinToStr()
										<< " no dia " << dia << " na sala " << sala->getCodigo();
									CentroDados::printError( "void ProblemSolution::constroiMapsDaSolucao()", msg.str() );
								}

								// Tipo de Credito
								bool ehTeorica = it_At_Hor_Aula->getCreditoTeorico();
						   
								// Atualiza horario ocupado na sala
								HorarioDia *hd = problemData->getHorarioDiaCorrespondente( horAula, dia );
								sala->addHorarioDiaOcupado(hd);

								// Professor usado
								int profId = it_At_Hor_Aula->getProfessorId();
								bool profVirtual = it_At_Hor_Aula->profVirtual();

								#pragma region Procura o objeto Professor
								Professor *profReal = nullptr;
								ProfessorVirtualOutput *profVirtualOut = nullptr;
								if ( profVirtual )
								{
									profVirtualOut = this->getProfVirtualOutput( profId );
									if ( profVirtualOut == nullptr )
									{
										stringstream msg;
										msg << "Professor virtual id " << profId << " nao encontrado.";
										CentroDados::printError( "void ProblemSolution::constroiMapsDaSolucao()", msg.str() );
									}
								}
								else
								{
									profReal = problemData->findProfessor(profId);
									if ( profReal == nullptr )
									{
										stringstream msg;
										msg << "Professor real id " << profId << " nao encontrado.";
										CentroDados::printError( "void ProblemSolution::constroiMapsDaSolucao()", msg.str() );
									}
									else
									{							  
										// Valida horario-dia alocado no professor						   		    
										if ( ! profReal->possuiHorariosNoDia( horAula, horAula, dia ) )
										{
											stringstream msg;
											msg << "Solucao usa horario invalido " << horAula->getInicio().hourMinToStr()
												<< " no dia " << dia << " no professor " << profReal->getNome();
											CentroDados::printError( "void ProblemSolution::constroiMapsDaSolucao()", msg.str() );
										} 
									}
								}

								#pragma endregion

								int tempoDoCredito = 0;
						   						   
								if ( it_At_Hor_Aula->atendimentos_ofertas != nullptr )
								{
									// Atendimentos por Oferta
									ITERA_GGROUP_LESSPTR( it_At_Oft,
										( *it_At_Hor_Aula->atendimentos_ofertas ), AtendimentoOferta )
									{
										int turma = it_At_Oft->getTurma();
										int qtd = it_At_Oft->getQuantidade();														
										GGroup<int /*alDem*/> *alDemAtend = & it_At_Oft->alunosDemandasAtendidas;
										string strOfertaId = it_At_Oft->getOfertaCursoCampiId();
										int ofertaId = atoi(strOfertaId.c_str());
										Oferta *oferta = problemData->refOfertas[ofertaId];

										#pragma region Procura disciplina real (caso de equivalência e/ou prática-teórica)

										int discOrigId = it_At_Oft->getDisciplinaId();
										int discRealId = it_At_Oft->getDisciplinaSubstitutaId();

										// Se não houve substituição por equivalência, a original é igual à real
										if ( discRealId == NULL )
											discRealId = discOrigId;

										Disciplina* discOrig = problemData->refDisciplinas[discOrigId];
										Disciplina* discReal = problemData->refDisciplinas[discRealId];
								
										if (!ehTeorica)
										{
											Disciplina* discTemp;
											int discTempId = -discRealId;
											discTemp = problemData->getDisciplinaTeorPrat(discReal);
											if ( discTemp != NULL )
											{
												discReal = discTemp;
												discRealId = discTempId;
											}
										}

										// Valida horario-dia alocado na disciplina						   		    
										if ( ! discReal->possuiHorariosNoDia( horAula, horAula, dia ) )
										{
											stringstream msg;
											msg << "Solucao usa horario invalido " << horAula->getInicio().hourMinToStr()
												<< " no dia " << dia << " para a turma " << turma 
												<< " da disciplina " << discReal->getCodigo();
											CentroDados::printError( "void ProblemSolution::constroiMapsDaSolucao()", msg.str() );
										} 

										// Valida equivalência
 										bool usouEquiv = discReal != discOrig;
										if ( usouEquiv )
										{																
											if ( ! problemData->ehSubstituivel( discOrig->getId(), discReal->getId(), oferta->curso ) )
											{
												stringstream msg;
												msg << "Solucao usa equivalencia invalida " << discReal->getCodigo()
													<< " -> " << discOrig->getCodigo()
													<< " para a turma " << turma << " com alunos do curso " << oferta->curso->getCodigo();
												CentroDados::printError( "void ProblemSolution::constroiMapsDaSolucao()", msg.str() );
											}
										} 
										#pragma endregion
								
										mapSolTurmaCursos[campusId][discReal][turma].add( oferta->curso );

										tempoDoCredito = discReal->getTempoCredSemanaLetiva();
								
										#pragma region Procura ou cria nova aula, e insere

										bool novaAula = true;
										Aula *aula = nullptr;

										auto itFinderCp = mapSolDiscTurmaDiaAula.find( campusId );
										if ( itFinderCp != mapSolDiscTurmaDiaAula.end() )
										{
											auto itFinderDisc = itFinderCp->second.find( discReal );
											if ( itFinderDisc != itFinderCp->second.end() )
											{
												auto itFinderTurma = itFinderDisc->second.find( turma );
												if ( itFinderTurma != itFinderDisc->second.end() )
												{
													auto itFinderDia = itFinderTurma->second.first.find( dia );
													if ( itFinderDia != itFinderTurma->second.first.end() )
													{
														aula = itFinderDia->second;
														novaAula = false;
													}
												}
											}
										}
								 
										if( novaAula )
										{
											// Monta o objeto 'aula'
											aula = new Aula();
									
											int nCredTeor = ( ehTeorica? 1 : 0 );
											int nCredPrat = ( ehTeorica? 0 : 1 );

											aula->setTurma( turma );
											aula->setDisciplina( discReal );
											aula->setCampus( campus );
											aula->setSala( sala );
											aula->setUnidade( unidade );
											aula->setDiaSemana( dia );
											aula->setCreditosTeoricos( nCredTeor );
											aula->setCreditosPraticos( nCredPrat );

											HorarioAula *hi = horAula;
											HorarioAula *hf = horAula;
											aula->setHorarioAulaInicial( hi );	
											aula->setHorarioAulaFinal( hf );	

											DateTime *dti = problemData->horarioAulaDateTime[hi->getId()].first;
											DateTime *dtf = problemData->horarioAulaDateTime[hf->getId()].first;
											aula->setDateTimeInicial( dti );
											aula->setDateTimeFinal( dtf );		
												
											mapSolDiscTurmaDiaAula[campusId][discReal][turma].first[dia] = aula;
											mapSolDiscTurmaDiaAula[campusId][discReal][turma].second.add( *alDemAtend );
										}
										else
										{		
											HorarioAula *hi = aula->getHorarioAulaInicial();
											HorarioAula *hf = aula->getHorarioAulaFinal();

											bool temHorario = true;									
											bool horAulaMenorQueHi = horAula->comparaMenor( *hi ); 
											bool horAulaMaiorQueHf = hf->comparaMenor( *horAula ); 
									
											// Se horAula não está entre hi e hf, então horAula ainda não está sendo englobado na aula
											if ( horAulaMenorQueHi || horAulaMaiorQueHf )
												temHorario = false;
									
											if ( !temHorario )
											{
												if ( horAulaMenorQueHi )
												{
													hi = horAula;
													aula->setHorarioAulaInicial( hi );
													DateTime *dti = problemData->horarioAulaDateTime[hi->getId()].first;
													aula->setDateTimeInicial( dti );
												}
												if ( horAulaMaiorQueHf )
												{
													hf = horAula;
													aula->setHorarioAulaFinal( hf );
													DateTime *dtf = problemData->horarioAulaDateTime[hf->getId()].first;
													aula->setDateTimeFinal( dtf );
												}

												Calendario *calendario = hi->getCalendario();
												int nCreds = calendario->retornaNroCreditosEntreHorarios(hi,hf);

												if ( ehTeorica )
													aula->setCreditosTeoricos( nCreds );
												else
													aula->setCreditosPraticos( nCreds );
											}

											mapSolDiscTurmaDiaAula[campusId][discReal][turma].second.add( *alDemAtend );
										}
										#pragma endregion
	
										#pragma region Atualiza situação dos alunos da turma
										// Adiciona a aula aos maps dos alunos da turma
										ITERA_GGROUP_N_PT( itAlDem, *alDemAtend, int )
										{
											AlunoDemanda *alunoDemandaTeor = problemData->retornaAlunoDemanda( *itAlDem );
											if ( alunoDemandaTeor != nullptr )
											{
												Aluno *aluno = alunoDemandaTeor->getAluno();

												aluno->addNroCreditosJaAlocados(1);
										
												AlunoSolution *alunoSolution=nullptr;
												bool novo = getAlunoSolution(aluno, alunoSolution);
												alunoSolution->addNroCreditosJaAlocados(1);
												alunoSolution->addChAtendida( tempoDoCredito );
												alunoSolution->addDiaDiscAula( dia, discReal, aula );
												alunoSolution->addTurma( alunoDemandaTeor, discReal, turma, campus );
												if ( novo ) mapAlunoSolution[aluno] = alunoSolution;
										
												#pragma region Valida equivalência forçada
												if ( alunoDemandaTeor->getExigeEquivalenciaForcada() && !usouEquiv )
												{
													// Equivalencia NÃO usada							
													if ( alunoDemandaTeor->demanda->disciplina->discEquivSubstitutas.size() > 0 )
													{
														stringstream msg;
														msg << "Equivalencia forcada nao respeitada. Aluno " 
															<< alunoDemandaTeor->getAlunoId() << " e Disciplina " << discReal->getId();
														CentroDados::printError( "void ProblemData::confereCorretudeAlocacoes()", msg.str() );
													}
													else
													{
														stringstream msg;
														msg << "Disciplina sem equivalentes encontradas, mas exige equivalencia forcada. Aluno "
															<< alunoDemandaTeor->getAlunoId() << " e Disciplina " << discReal->getId();
														CentroDados::printError( "void ProblemData::confereCorretudeAlocacoes()", msg.str() );							   
													}
												}
												#pragma endregion
											}
											else
											{
												stringstream msg;
												msg << "AlunoDemanda "<< *itAlDem << " nao encontrado";
												CentroDados::printError( "void ProblemSolution::constroiMapsDaSolucao()", msg.str() );
											}
										}
										#pragma endregion

										// Adiciona a aula ao map do professor
										if ( profVirtual && profVirtualOut != nullptr )
										{
											mapSolTurmaProfVirtualDiaAula[campusId][discReal][turma][profVirtualOut][dia] = aula;
										}

										// temporario
										if ( profVirtual )
											tempCpDiscTurmaPVId[campusId][discReal][turma] = profId;
									}
								}
								else
								{									
								   std::cout << "\nProvavel erro! atendimentos_ofertas null em ProblemSolution.\n";
								}


								#pragma region Atualiza situação do professor
								// Adiciona horario-dia ocupado do professor
								if ( !profVirtual && profReal != nullptr )
									mapSolProfRealDiaHorarios[profReal][dia].push_back(hd);
							
								// Atualiza a carga horária semanal usada do professor
								auto itProf = this->quantChProfs.find( profId );
								if ( itProf != this->quantChProfs.end() )
								{
									itProf->second += tempoDoCredito;
								}
								else
								{
									this->quantChProfs[ profId ] = tempoDoCredito;
								}
							
								if ( profVirtual )
								{
									nroCredsProfsVirtuais++;
									chProfsVirt += tempoDoCredito;
								}
								else
								{
									nroCredsProfsReais++;
									chProfsReal += tempoDoCredito;
								}
								#pragma endregion
							}
						}
					}
				}
			}
		}
	}
	else
	{
		std::cout << "\nProvavel erro! atendimento_campus null em ProblemSolution.\n";
	}

	// ------------------------------------------------------------------------------------------
	// Preenche map com horários livres das salas
	
	std::cout << " preenchendo map com horarios livres das salas..."; fflush(0);

	ITERA_GGROUP_LESSPTR( itCp, problemData->campi, Campus )
	{
		ITERA_GGROUP_LESSPTR( itUnidade, itCp->unidades, Unidade )
		{
			ITERA_GGROUP_LESSPTR( itSala, itUnidade->salas, Sala )
			{
				mapSalaDiaHorariosVagos[ *itSala ] = (*itSala)->retornaHorariosAulaVagos();
			}
		}
	}

	
	// ------------------------------------------------------------------------------------------
	#pragma region INDICADORES DE QUALIDADE DA SOLUÇÃO OPERACIONAL
	
	std::cout << " indicadores op..."; fflush(0);

	// Calcula nro de turmas
	int nTurmasT=0;
	int nTurmasP=0;

	// Calcula nro total de turmas
	for ( auto itMapCp = mapSolDiscTurmaDiaAula.begin(); itMapCp != mapSolDiscTurmaDiaAula.end(); itMapCp++ )
	{
		for ( auto itMapDisc = itMapCp->second.begin(); itMapDisc != itMapCp->second.end(); itMapDisc++ )
		{
			if ( itMapDisc->first->getId() < 0 )
			{
				nTurmasP += itMapDisc->second.size();
			}
			else
			{
				nTurmasT += itMapDisc->second.size();
			}
		}
	}
	
	// Calcula nro de turmas ministradas por profs virtuais
	int nroTotalProfsVirtuaisUsados = 0;
	int nTurmasVirtT=0;
	int nTurmasVirtP=0;
	int nTurmasPV_semHabReal=0;
	int nCredsPV_semHabReal=0;
	int nPV_semHabReal = 0;
	GGroup<ProfessorVirtualOutput*> pvo_discSemProfHab;
	GGroup<ProfessorVirtualOutput*> pvo_totais;

	for ( auto itMapCp = mapSolTurmaProfVirtualDiaAula.begin(); itMapCp != mapSolTurmaProfVirtualDiaAula.end(); itMapCp++ )
	{
		for ( auto itMapDisc = itMapCp->second.begin(); itMapDisc != itMapCp->second.end(); itMapDisc++ )
		{
			Disciplina *disc = itMapDisc->first;
			if ( disc->getId() < 0 )
			{
				nTurmasVirtP += itMapDisc->second.size();
			}
			else
			{
				nTurmasVirtT += itMapDisc->second.size();
			}		

			bool semProfRealHab = (disc->getNroProfRealHabilit() == 0);

			for ( auto itMapTurma = itMapDisc->second.begin(); itMapTurma != itMapDisc->second.end(); itMapTurma++ )
			{
				// Calcula nro de profs virtuais usados para turmas de disciplinas sem prof real habilitado
				if ( semProfRealHab )
				{
					nCredsPV_semHabReal += disc->getTotalCreditos();

					if ( disc->getId() > 0 ) nTurmasPV_semHabReal++;
				}

				for ( auto itMapPVO = itMapTurma->second.begin(); itMapPVO != itMapTurma->second.end(); itMapPVO++ )
				{
					if ( semProfRealHab )
						pvo_discSemProfHab.add( itMapPVO->first );

					pvo_totais.add( itMapPVO->first );
				}
			}
		}
	}
		
	nroTotalProfsVirtuaisUsados = (int) pvo_totais.size();
	nPV_semHabReal = (int) pvo_discSemProfHab.size();

	GGroup<int> pvoId_totais;
	GGroup<int> pvoId_discSemProfHab;

	// --------------------------------------------------------
	// Temporario: só para leitura de solução, quando mapSolTurmaProfVirtualDiaAula 
	// não será preenchido por falta do prof virtual individualizado
	if ( mapSolTurmaProfVirtualDiaAula.size() == 0 )
	{		
		auto itCp = tempCpDiscTurmaPVId.begin();
		for ( ; itCp != tempCpDiscTurmaPVId.end(); itCp++ )
		{
			auto itDisc = itCp->second.begin();
			for ( ; itDisc != itCp->second.end(); itDisc++ )
			{
				Disciplina *disc = itDisc->first;
				if ( disc->getNroProfRealHabilit() == 0 )
				{
					auto itTurma = itDisc->second.begin();
					for ( ; itTurma != itDisc->second.end(); itTurma++ )
					{
						if ( disc->getId() < 0 )
							nTurmasVirtP++;
						else
							nTurmasVirtT++;
						
						pvoId_totais.add( itTurma->second );

						// Calcula nro de profs virtuais usados para turmas de disciplinas sem prof real habilitado
						if ( disc->getNroProfRealHabilit() == 0 )
						{
							if ( disc->getId() > 0 ) nTurmasPV_semHabReal++;
						
							nCredsPV_semHabReal += disc->getTotalCreditos();

							pvoId_discSemProfHab.add( itTurma->second );
						}
					}
				}
			}		
		}

		nPV_semHabReal = (int) pvoId_discSemProfHab.size();
		nroTotalProfsVirtuaisUsados = pvoId_totais.size();
	}
	

	// --------------------------------------------------------
	
	int nTurmasRealT = nTurmasT - nTurmasVirtT;
	int nTurmasRealP = nTurmasP - nTurmasVirtP;

	int nroTotalProfsReaisUsados = mapSolProfRealDiaHorarios.size();
	int nroTotalProfsUsados = nroTotalProfsReaisUsados + nroTotalProfsVirtuaisUsados;

	int nroCredsProfsUsados = nroCredsProfsReais + nroCredsProfsVirtuais;
		
	Indicadores::printSeparator(3);
	Indicadores::printIndicador( "\n\t\t\tFINAL: Professores virtuais individualizados\n");

	Indicadores::printIndicador( "\nNumero total de professores usados: ", nroTotalProfsUsados );
	Indicadores::printIndicador( "\nNumero de professores virtuais usados: ", nroTotalProfsVirtuaisUsados );	
	Indicadores::printIndicador( "\nNumero de professores reais usados: ", nroTotalProfsReaisUsados );	
	Indicadores::printSeparator(1);
	Indicadores::printIndicador( "\nNumero total de creditos alocados a professores: ", nroCredsProfsUsados );
	Indicadores::printIndicador( "\nNumero de creditos alocados a professores reais: ", nroCredsProfsReais );
	Indicadores::printIndicador( "\nNumero de creditos alocados a professores virtuais: ", nroCredsProfsVirtuais );
	Indicadores::printSeparator(1);
	
	Indicadores::printIndicador( "\nCarga horaria alocada a professores (h): ", (chProfsReal+chProfsVirt) / 60, 2 );
	Indicadores::printIndicador( "\nCarga horaria alocada a professores reais (h): ", chProfsReal/60, 2 );
	Indicadores::printIndicador( "\nCarga horaria alocada a professores virtuais (h): ", chProfsVirt/60, 2 );
	Indicadores::printSeparator(1);
	Indicadores::printIndicador( "\nNumero de turmas atendidas: ", nTurmasT );
	Indicadores::printIndicador( "\nNumero de turmas atendidas por professores reais: ", nTurmasRealT );
	Indicadores::printIndicador( "\nNumero de turmas atendidas por professores virtuais: ", nTurmasVirtT );
	Indicadores::printIndicador( "\nNumero de turmas praticas (com divisao PT) atendidas: ", nTurmasP );
	Indicadores::printIndicador( "\nNumero de turmas praticas (com divisao PT) atendidas por professores reais: ", nTurmasRealP );
	Indicadores::printIndicador( "\nNumero de turmas praticas (com divisao PT) atendidas por professores virtuais: ", nTurmasVirtP );
	Indicadores::printSeparator(1);
	Indicadores::printIndicador( "\nPara disciplinas sem professor real habilitado:" );
	Indicadores::printIndicador( "\n\tNumero de professores virtuais usados: ", nPV_semHabReal );
	Indicadores::printIndicador( "\n\tNumero de turmas teóricas atendidas por professores virtuais: ", nTurmasPV_semHabReal );
	Indicadores::printIndicador( "\n\tNumero de créditos atendidos por professores virtuais: ", nCredsPV_semHabReal );
	#pragma endregion

	std::cout << " fim!"; fflush(0);
}

void ProblemSolution::clearMapsDaSolucao()
{
	std::cout << "\nclearMapsDaSolucao..."; fflush(0);

	// ----------------------------------------------------------
	// DELETA AULAS
	for ( auto itMapCp = mapSolDiscTurmaDiaAula.begin(); itMapCp != mapSolDiscTurmaDiaAula.end(); itMapCp++ )
	{
		for ( auto itMapDisc = itMapCp->second.begin(); itMapDisc != itMapCp->second.end(); itMapDisc++ )
		{
			for ( auto itMapTurma = itMapDisc->second.begin(); itMapTurma != itMapDisc->second.end(); itMapTurma++ )
			{
				for ( auto itMapDia = itMapTurma->second.first.begin(); itMapDia != itMapTurma->second.first.end(); itMapDia++ )
				{
					if ( itMapDia->second != nullptr )
						delete itMapDia->second;
				}
				itMapTurma->second.first.clear();
				itMapTurma->second.second.clear();
			}
			itMapDisc->second.clear();
		}
		itMapCp->second.clear();
	}
	mapSolDiscTurmaDiaAula.clear();
	mapSalaDiaHorariosVagos.clear();
	mapSolProfRealDiaHorarios.clear();
	mapSolTurmaProfVirtualDiaAula.clear();
	quantChProfs.clear();
	mapSolTurmaCursos.clear();
		
	std::cout << " limpando alunoSolution..."; fflush(0);

	// ----------------------------------------------------------	
	// DELETA ALUNO_SOLUTION
	for ( auto itMap = mapAlunoSolution.begin(); itMap != mapAlunoSolution.end(); itMap++ )
	{
		if ( itMap->second != nullptr )
			delete itMap->second;
	}
	mapAlunoSolution.clear();
	// ----------------------------------------------------------

	std::cout << " limpo!"; fflush(0);
}

void ProblemSolution::verificaNrDiscSimultVirtual()
{
	cout << "\nverificaNrDiscSimultVirtual..."; fflush(0);

   // PERCORRE TODAS AS TURMAS COM PROFESSORES VIRTUAIS
   nrMaxDiscSimult_=0;
   auto itFinder1 = mapSolTurmaProfVirtualDiaAula.begin();
   for ( ; itFinder1 != mapSolTurmaProfVirtualDiaAula.end(); itFinder1++ )
   {
	    int cpId = itFinder1->first;
		
		auto itFinder2 = itFinder1->second.begin();
		for ( ; itFinder2 != itFinder1->second.end(); itFinder2++ )
		{
			Disciplina* disciplina = itFinder2->first;
			
			unordered_map<int, map<DateTime, unordered_set<Aula*>>> mapDiaDtiAulas;
			
			// agrupa as aulas da disciplina de profs virtuais por dia/dti
			auto itFinder3 = itFinder2->second.begin();
			for ( ; itFinder3 != itFinder2->second.end(); itFinder3++ )
			{	   
			   auto itFinder4 = itFinder3->second.begin();			   
			   for ( ; itFinder4 != itFinder3->second.end(); itFinder4++ )
			   {
				   auto itFinder5 = itFinder4->second.begin();
				   for ( ; itFinder5 != itFinder4->second.end(); itFinder5++ )
				   {
					   int dia = itFinder5->first;
					   Aula *aula = itFinder5->second;

					   mapDiaDtiAulas[dia][*aula->getDateTimeInicial()].insert(aula);
				   }
			   }
			}

			// histograma
			for (auto itDia=mapDiaDtiAulas.cbegin(); itDia!=mapDiaDtiAulas.cend(); itDia++)
			{
				for (auto itDti=itDia->second.cbegin(); itDti!=itDia->second.cend(); itDti++)
				{
					int qtd = itDti->second.size();
					if (qtd>1)
						mapNrDiscSimult[qtd].insert(disciplina);
					if (qtd>nrMaxDiscSimult_)
						nrMaxDiscSimult_ = qtd;
				}
			}
		}
   }

   stringstream msg;
   msg << "\nNr maximo de aulas de mesma disciplina simultaneas " << nrMaxDiscSimult_;
   Indicadores::printIndicador( msg.str() );
}

void ProblemSolution::verificaNaoAtendimentosTaticos()
{
	/*
		Algoritmo para, a partir de uma solução do modo Tático do Trieda, determinar
		o motivo de não atendimento de um par [aluno, disciplina].
	*/
	
	CentroDados::startTimer();

	std::cout<<"\nVerificando nao atendimentos tatico..."; fflush(NULL);	

	/* 		
	Possiveis restricoes:
	* Salas lotadas
	* Choque de horarios
	* Disciplina criada um outro turno
	* Demanda minima nao atingida
	* Nao ha sala disponivel para criar a disciplina
	*/
	
	ProblemData* const problemData = CentroDados::getProblemData();
		
	// ------------------------------------------------------------------------------------------
	// Maps para Alunos-Demanda não atendidos
	map< Disciplina*, map< TurnoIES*, GGroup<AlunoDemanda*, 
		LessPtr<AlunoDemanda>>, LessPtr<TurnoIES> >, LessPtr<Disciplina> > mapDiscTurnoAlDemNaoAtend;

	map< Disciplina*, map< TurnoIES*, GGroup<AlunoDemanda*, 
		LessPtr<AlunoDemanda>>, LessPtr<TurnoIES> >, LessPtr<Disciplina> > mapDiscTotalNaoAtend;	

	vector< AlunoDemanda* > alunoChCompletaMasP1NaoAtend;

	int nroAlunoDemAtendP1=0;
	int nroAlunoDemAtendP2=0;
	int nroAlunoDemNaoAtendP1=0;
	int nroTotalAlunoDemFormandoP1=0;
	int nroTotalAlunoDemCalouroP1=0;
	int nroAlunoDemFormandoAtendP1=0;
	int nroAlunoDemCalouroAtendP1=0;
	
	// ------------------------------------------------------------------------------------------
	// Percore todos os Alunos-Demanda do problema, agrupando os não atendidos

	ITERA_GGROUP_LESSPTR(it_alunos_demanda, problemData->alunosDemandaTotal, AlunoDemanda)
	{
		AlunoDemanda* ad = (*it_alunos_demanda);		
		Demanda *demanda = ( ad->demandaOriginal==nullptr ? ad->demanda : ad->demandaOriginal );
		
		// Número de disciplinas requeridas x Número de disciplinas atendidas
		int nroDiscsReqP1 = ad->getAluno()->getNroDiscsOrigRequeridosP1();
		int nroDiscsAtend = 0;
		AlunoSolution *alunoSol = this->getAlunoSolution( ad->getAluno() );
		if ( alunoSol!=nullptr )
			nroDiscsAtend = alunoSol->getNroDiscAtendidas();
		
		// Completude do atendimento do aluno
		bool nroDiscsCompleto = (nroDiscsAtend >= nroDiscsReqP1);
		bool chCompleta = ad->getAluno()->totalmenteAtendido();

		bool P1 = (ad->getPrioridade()==1);
		bool teorica = (demanda->disciplina->getId() > 0);
		
		// ------------------------------------------------- procura AlunoDemanda original.
		AlunoDemanda* adOrig = problemData->procuraAlunoDemanda( abs(demanda->getDisciplinaId()), ad->getAlunoId(), ad->getPrioridade() );
		if ( adOrig==nullptr )
		{
			CentroDados::printError("","Erro, adOrig null");
			continue;
		}
		// Procura sempre por alocação teórica, pq a equivalencia pode ter eliminado a existência de prática
		int nTurma = this->retornaTurmaDiscAluno(adOrig, true);
		bool alocado = ( nTurma == -1 ? false:true );
		// -------------------------------------------------------------------------------
		
		if ( alocado && P1 && teorica ) nroAlunoDemAtendP1++;
		else if ( !alocado && P1 && teorica ) nroAlunoDemNaoAtendP1++;
		else if ( alocado && !P1 && teorica ) nroAlunoDemAtendP2++;		
		
		if ( P1 && teorica )
		{
			if ( ad->getAluno()->ehFormando() )
			{
				nroTotalAlunoDemFormandoP1++;
				if ( alocado ) nroAlunoDemFormandoAtendP1++;
			}
			if ( ad->getAluno()->ehCalouro() )
			{
				nroTotalAlunoDemCalouroP1++;
				if ( alocado ) nroAlunoDemCalouroAtendP1++;
			}
		}
		
		if ( !alocado )
		{
			//if ( ad->getAluno()->ehFormando() )
			//	formandosNaoAtendidos[ad->getAluno()][ad->getPrioridade()].add( ad );
			//if ( ad->getAluno()->ehCalouro() )
			//	calourosNaoAtendidos[ad->getAluno()][ad->getPrioridade()].add( ad );

			if ( ! (chCompleta || nroDiscsCompleto) )
			{
				mapDiscTurnoAlDemNaoAtend[demanda->disciplina][ad->getOferta()->turno].add( ad );

				// ------------------------------------------------
				// Agrupamento de todas as folgas, incluindo as disciplinas equivalentes
				mapDiscTotalNaoAtend[demanda->disciplina][ad->getOferta()->turno].add( ad );
				if ( problemData->parametros->considerar_equivalencia_por_aluno && ad->demandaOriginal==NULL )
				{
					ITERA_GGROUP_LESSPTR(itDiscEquiv, ad->demanda->disciplina->discEquivSubstitutas, Disciplina)
					{	
						if ( problemData->alocacaoEquivViavel( ad->demanda, *itDiscEquiv ) )
							mapDiscTotalNaoAtend[*itDiscEquiv][ad->getOferta()->turno].add( ad );
					}					
				}
				// ------------------------------------------------
			}
			else if ( (chCompleta || nroDiscsCompleto) && P1 && teorica )
				alunoChCompletaMasP1NaoAtend.push_back( ad );
		}
	}
	
	// ------------------------------------------------------------------------------------------
	setNroTotalAlunoDemandaP1( nroAlunoDemAtendP1+nroAlunoDemNaoAtendP1 );
	setNroAlunoDemAtendP1( nroAlunoDemAtendP1 );
	setNroAlunoDemNaoAtendP1( nroAlunoDemNaoAtendP1 );
	setNroAlunoDemAtendP2( nroAlunoDemAtendP2 );
	setNroAlunoDemAtendP1P2( nroAlunoDemAtendP1+nroAlunoDemAtendP2 );
	setNroTotalAlunoDemFormandosP1( nroTotalAlunoDemFormandoP1 );
	setNroTotalAlunoDemCalourosP1( nroTotalAlunoDemCalouroP1 );
	setNroTotalAlunoDemFormandosAtendP1( nroAlunoDemFormandoAtendP1 );
	setNroTotalAlunoDemCalourosAtendP1( nroAlunoDemCalouroAtendP1 );

	imprimeIndicadores();

	// ------------------------------------------------------------------------------------------
	// Alunos com carga horária completa ou número de disciplinas pedido completo,
	// mas com folga de disciplina original de P1

	for ( int at=0; at < alunoChCompletaMasP1NaoAtend.size(); at++ )
	{	
		AlunoDemanda* ad = alunoChCompletaMasP1NaoAtend[at];

		NaoAtendimento* naoAtendimento = new NaoAtendimento( ad->getId() );
		auto itNaoAtend = this->nao_atendimentos->find( naoAtendimento );
		if ( itNaoAtend == this->nao_atendimentos->end() )
		{
			stringstream ss;
			ss << "O aluno teve o total de carga horária e/ou o total de disciplinas requisitado de prioridade 1 atendido, "
				<< "porém com uso de prioridade 2 e/ou equivalências.";
			naoAtendimento->addMotivo( ss.str() );
				
			this->nao_atendimentos->add( naoAtendimento );
		}
		else delete naoAtendimento;
	}
	
	// ------------------------------------------------------------------------------------------
	// Investiga AlunosDemanda não atendidos

	std::map<int, std::vector<string> > aluno_saida_em_txt;
	
	map< Disciplina*, map< TurnoIES*, GGroup<AlunoDemanda*, 
		LessPtr<AlunoDemanda>>, LessPtr<TurnoIES> >, LessPtr<Disciplina> >::iterator itDisc = mapDiscTurnoAlDemNaoAtend.begin();
	for ( ; itDisc != mapDiscTurnoAlDemNaoAtend.end(); itDisc++ )
	{	
		map< TurnoIES*, GGroup<AlunoDemanda*, 
			LessPtr<AlunoDemanda>>, LessPtr<TurnoIES> > *mapTurnoAlDemNaoAtend = & itDisc->second;

		map< TurnoIES*, GGroup<AlunoDemanda*, 
			LessPtr<AlunoDemanda>>, LessPtr<TurnoIES> >::iterator itTurno = (*mapTurnoAlDemNaoAtend).begin();
		for ( ; itTurno != (*mapTurnoAlDemNaoAtend).end(); itTurno++ )
		{
			GGroup<AlunoDemanda*, LessPtr<AlunoDemanda>> *alDemNaoAtend = & itTurno->second;
			ITERA_GGROUP_LESSPTR(itAlDem, *alDemNaoAtend, AlunoDemanda)
			{
				AlunoDemanda* ad = (*itAlDem);
				Demanda *demanda = ( ad->demandaOriginal==nullptr ? ad->demanda : ad->demandaOriginal );

			//	std::cout << "\nVerificando nao atendimento do AlunoDemanda id " 
			//		<< ad->getId() << ", aluno " << ad->getAlunoId() << ", disc " << ad->demanda->disciplina->getId();

				bool bd = false;				
				if ( ad->getAlunoId()==111180 && demanda->getDisciplinaId() == 24690 )
				{
					//bd = true;
				}


				if (bd) cout<< "\nALuno " << ad->getAlunoId() << " Disc " << demanda->disciplina->getId()
					<< " Prior " << ad->getPrioridade();
				
				//if ( ad->getAlunoId() == 53254 && ad->demanda->disciplina->getId() == -16004 ){
					//bd = true;
				//}

				int campusAlunoId = ad->getCampus()->getId();
				Aluno *aluno = ad->getAluno();
				int alunoId = ad->getAluno()->getAlunoId();
				int alDemId = ad->getId();
				int turnoIESid = demanda->getTurnoIES()->getId();
				TurnoIES* turnoIES = demanda->getTurnoIES();
				string turnoIESname = demanda->getTurnoIES()->getNome();

				if (bd) std::cout<<"\n1"; fflush(NULL);
							
				int alDemIdOrig = ad->getId();
				if ( demanda->getDisciplinaId() < 0 )
				{
					AlunoDemanda *adOrig = problemData->procuraAlunoDemanda( -demanda->getDisciplinaId(), ad->getAlunoId(), ad->getPrioridade() );
					if ( adOrig == nullptr )
					{
						stringstream msg;
						msg << "2. AlunoDemanda teorico associado ao pratico nao encontrado para a disc " 
							<< -demanda->getDisciplinaId() << " e aluno " << ad->getAlunoId();
						CentroDados::printError("void ProblemSolution::verificaNaoAtendimentosTaticos()", msg.str() );
						continue;
					}
					else alDemIdOrig = adOrig->getId();
				}
				
				NaoAtendimento* naoAtendimento = new NaoAtendimento( alDemIdOrig );
				auto itNaoAtend = this->nao_atendimentos->find( naoAtendimento );
				if ( itNaoAtend != this->nao_atendimentos->end() )
				{
					delete naoAtendimento;
					naoAtendimento = *itNaoAtend;
					this->nao_atendimentos->remove( itNaoAtend );
				}

				if (bd) std::cout<<"\n2"; fflush(NULL);

				// Verifica se alguma turma para o campus foi criada
				auto itMapSol = mapSolDiscTurmaDiaAula.find( campusAlunoId );
				if ( itMapSol == mapSolDiscTurmaDiaAula.end() )
				{				
					stringstream ss2;
					ss2 << "Nenhuma turma de nenhuma disciplina criada no campus " << ad->getCampus()->getCodigo() << ".";
					naoAtendimento->addMotivo( ss2.str() );
				
					this->nao_atendimentos->add( naoAtendimento );

					continue;
				}

				if (bd) std::cout<<"\n3"; fflush(NULL);

				// Acha as aulas do aluno
				AlunoSolution* alSol=nullptr;
				auto itAulasAluno = this->mapAlunoSolution.find(aluno);				
				if ( itAulasAluno != this->mapAlunoSolution.end() )
					alSol = itAulasAluno->second;

				// Disciplinas que serviriam para atender esse aluno-demanda
				GGroup< Disciplina *, LessPtr< Disciplina > > disciplinas;
				disciplinas.add(demanda->disciplina);

				if ( problemData->parametros->considerar_equivalencia_por_aluno )
				{
					if ( ad->demandaOriginal==NULL )
					{
						ITERA_GGROUP_LESSPTR(itDiscEquiv, ad->demanda->disciplina->discEquivSubstitutas, Disciplina)
						{	
							if ( problemData->alocacaoEquivViavel( ad->demanda, *itDiscEquiv ) )
								disciplinas.add( *itDiscEquiv );
						}
					}
					else
						disciplinas.add(ad->demandaOriginal->disciplina);
				}

				if (bd) std::cout<<"\n4"; fflush(NULL);

				ITERA_GGROUP_LESSPTR(it_disciplina, disciplinas, Disciplina)
				{				
					Disciplina *discEquiv = *it_disciplina;

					bool discEhEquiv;
					if ( ad->demandaOriginal==nullptr )
						discEhEquiv = discEquiv->getId() != ad->demanda->getDisciplinaId();
					else
						discEhEquiv = discEquiv->getId() != ad->demandaOriginal->getDisciplinaId();

					if (bd) std::cout<<"\n\t4.1"; fflush(NULL);
				
					// Equivalência forçada
					if ( ad->getExigeEquivalenciaForcada() && !discEhEquiv )
					{
						stringstream ss2;
						ss2 << "Equivalência forçada para a disciplina " << discEquiv->getCodigo() << ".";
						naoAtendimento->addMotivo( ss2.str() );
						continue;
					}

					// Disciplina pratica resultante de divisão de créditos práticos+teóricos
					bool discPratica=false;
					if ( discEquiv->getId() < 0 )
						discPratica=true;
					
					string strTurmaTipoCred("");
					string strTurmasTipoCred("");
					if ( discPratica )
					{
						strTurmaTipoCred = "prática ";
						strTurmasTipoCred = "práticas ";
					}
					else if ( discEquiv->temParPratTeor() )
					{
						strTurmaTipoCred = "teórica ";
						strTurmasTipoCred = "teóricas ";
					}


					// Calcula minimo de alunos na turma
					int minAlunosNaTurma = 0;
					if ( problemData->parametros->min_alunos_abertura_turmas && !discEquiv->eLab() )
					{
						minAlunosNaTurma = problemData->parametros->min_alunos_abertura_turmas_value;
					}
					else if ( problemData->parametros->min_alunos_abertura_turmas_praticas && discEquiv->eLab() )
					{
						 minAlunosNaTurma = problemData->parametros->min_alunos_abertura_turmas_praticas_value;
					}


					// Procura todos os não atendimentos na disciplina discEquiv
					map< TurnoIES*, GGroup<AlunoDemanda*, LessPtr<AlunoDemanda>>, LessPtr<TurnoIES> > 
						*mapAlsDemNaoAtend = nullptr;
					auto itDiscNaoAtend = mapDiscTotalNaoAtend.find( discEquiv );
					if ( itDiscNaoAtend != mapDiscTotalNaoAtend.end() )
						mapAlsDemNaoAtend = & itDiscNaoAtend->second;


					// Verifica se alguma turma no campus para a disciplina foi criada
					map< Disciplina*, map< int /*turma*/, std::pair< map<int /*dia*/, Aula*>,
						GGroup<int /*alDem*/> > >, LessPtr<Disciplina> > *mapDiscsSol = & itMapSol->second;
					auto itDiscSol = (*mapDiscsSol).find( discEquiv );
					if ( itDiscSol != (*mapDiscsSol).end() )
					{
						if (bd) std::cout<<"\n\t4.2"; fflush(NULL);

						/* 
						Disciplina Foi Criada
						- Capacidade da sala
						- Se houve choque de horario
						- Verificar se foi em outro turno
						*/
						#pragma region Disciplina_Criada

						// Indica se alguma turma foi aberta no turno da oferta
						bool haTurmaMesmoTurno = false;

						// Conjunto de turmas que estao com salas lotadas
						GGroup< std::pair<int /*turma*/, Sala*> > salas_lotadas;

						// Conjunto de turmas em que houve choque de horario
						map< int /*turma existente*/, map< Trio< int , int , Disciplina* >, GGroup<Aula*,LessPtr<Aula>> > > turmas_choque_horarios;
					
						GGroup< int /*turmas existentes*/ > turmasForaDoTurno;
						GGroup< int /*turmas existentes*/ > turmasMaxPedagAlunosAtingido;

						int maxAlunosPedagogico = discEquiv->getMaxAlunosP() + discEquiv->getMaxAlunosT();

						int totalTurmasCriadas = itDiscSol->second.size();
						
						GGroup<int> turmasSemMotivo;

						#pragma region Pesquisa os motivos de não inserção do aluno em cada turma aberta da disciplina

						map< int /*turma*/, std::pair< map<int /*dia*/, Aula*>,	GGroup<int /*alDem*/> > > 
							*turmasCriadas = & itDiscSol->second;
						auto itTurmasSol = (*turmasCriadas).begin();
						for ( ; itTurmasSol != (*turmasCriadas).end(); itTurmasSol++ )
						{
							int turma = itTurmasSol->first;						
						
							int turmaSize = itTurmasSol->second.second.size();
							
							if ( turmaSize >= maxAlunosPedagogico )
							{
								turmasMaxPedagAlunosAtingido.add( turma );
								break;
							}

							map< int /*dia*/, Aula* >::iterator itDiaSol = itTurmasSol->second.first.begin();
							for ( ; itDiaSol != itTurmasSol->second.first.end(); itDiaSol++ )
							{
								Aula *aula = itDiaSol->second;
								HorarioAula *hi = aula->getHorarioAulaInicial();
								HorarioAula *hf = aula->getHorarioAulaFinal();
								DateTime inicio = hi->getInicio();
								DateTime fim = hf->getFinal();
								int dia = aula->getDiaSemana();
								Sala* sala = aula->getSala();

								if ( ad->getOferta()->turno->possuiHorarioDiaOuCorrespondente(hi,hf,dia) )
								{								
									if((unsigned) sala->getCapacidade() > turmaSize )
									{
										/*  Ha vaga na sala;
											Checar se houve choque de horario 
										*/
										
										turmasSemMotivo.add(turma);

										// Turmas em que o aluno foi alocado
										if ( alSol != nullptr )
										{											
											GGroup< Aula* > aulasComChoque;
											alSol->procuraChoqueDeHorariosAluno( dia, inicio, fim, aulasComChoque );
											ITERA_GGROUP( itAula, aulasComChoque, Aula )
											{
												Aula *aula = *itAula;
												Trio< int , int , Disciplina* > trio_aluno
													( aula->getCampus()->getId(), aula->getTurma(), aula->getDisciplina() );
																			
												turmas_choque_horarios[turma][trio_aluno].add( aula );
											}								
										}										
									}
									else
									{
										/* Sala lotada */
										std::pair<int /*turma*/, Sala*> lotacao(turma,sala);
										salas_lotadas.add(lotacao);
									}
								}
								else
								{
									turmasForaDoTurno.add( turma );
								}
							}
						}
						#pragma endregion
											
						if( turmasForaDoTurno.size() == totalTurmasCriadas )
						{
							// TODAS AS TURMAS CRIADAS ESTÃO EM TURNOS DIFERENTES
							stringstream ss2;
							ss2 << "Todas as turmas " << strTurmasTipoCred
									<< "da disciplina " << discEquiv->getCodigo()
									<< ( discEhEquiv ?"(equivalente) ":" ")
									<< "criadas estão em turnos diferentes do turno do aluno.";
							naoAtendimento->addMotivo( ss2.str() );
						}
						else
						{
							#pragma region Turmas com limite pedagógico de alunos atingido
							int nroTurmasMaxPedag = turmasMaxPedagAlunosAtingido.size();
							if ( nroTurmasMaxPedag == 1 )
							{																
								stringstream ss;
								ss << "A turma " << strTurmaTipoCred
									<< *(turmasMaxPedagAlunosAtingido.begin())
									<< " da disciplina " << discEquiv->getCodigo()
									<< ( discEhEquiv ?"(equivalente)":" ")
									<< " atingiu o máximo pedagógico de " << maxAlunosPedagogico
									<< " alunos na turma.";
								naoAtendimento->addMotivo( ss.str() );
							}
							else if ( nroTurmasMaxPedag > 1 )
							{
								stringstream ss;
								ss << "As turmas " << strTurmasTipoCred;
								int counter=0;
								ITERA_GGROUP_N_PT( itTurma, turmasMaxPedagAlunosAtingido, int )
								{
									counter++;
									ss << *itTurma;
									if ( counter + 1 < nroTurmasMaxPedag ) ss << ", ";			// antes do penultimo
									else if ( counter + 1 == nroTurmasMaxPedag ) ss << " e ";	// penultimo
								}

								ss << "da disciplina " << discEquiv->getCodigo()
									<< ( discEhEquiv ?"(equivalente)":" ")
									<< " atingiram o máximo pedagógico de " 
									<< maxAlunosPedagogico << " alunos.";								
							
								naoAtendimento->addMotivo( ss.str() );
							}
							#pragma endregion

							#pragma region Salas lotadas
							int nroSalasLotadas = salas_lotadas.size();
							if ( nroSalasLotadas == 1 )
							{																
								stringstream ss;
								ss << "A turma " << strTurmaTipoCred
									<< (*salas_lotadas.begin()).first 
									<< " da disciplina " << discEquiv->getCodigo()
									<< ( discEhEquiv ?"(equivalente)":" ") 
									<< " está lotada na sala " << (*salas_lotadas.begin()).second->getCodigo() << ".";
								naoAtendimento->addMotivo( ss.str() );
							}
							else if ( nroSalasLotadas > 1 )
							{
								stringstream ss;
								ss << "As turmas " << strTurmasTipoCred;
								int counter=0;
								GGroup< std::pair<int /*turma*/, Sala*> >::iterator it_salas_lotadas = salas_lotadas.begin();
								for(;it_salas_lotadas != salas_lotadas.end(); it_salas_lotadas++)
								{
									counter++;
									ss << (*it_salas_lotadas).first;
									if ( counter + 1 < nroSalasLotadas ) ss << ", ";			// antes do penultimo
									else if ( counter + 1 == nroSalasLotadas ) ss << " e ";		// penultimo
								}

								ss << " da disciplina " << discEquiv->getCodigo()
									<< ( discEhEquiv ?"(equivalente)":" ") 
									<< " estão lotadas.";					
							
								naoAtendimento->addMotivo( ss.str() );
							}
							#pragma endregion
							
							#pragma region Turmas fora do turno do aluno
							int nroTurmasForaDoTurno = turmasForaDoTurno.size();
							if ( nroTurmasForaDoTurno == 1 )
							{																
								stringstream ss;
								ss << "A turma " << strTurmaTipoCred
									<< *(turmasForaDoTurno.begin())
									<< " da disciplina " << discEquiv->getCodigo()
									<< ( discEhEquiv ?"(equivalente)":" ")
									<< " está em turno diferente do turno do aluno.";
								naoAtendimento->addMotivo( ss.str() );
							}
							else if ( nroTurmasForaDoTurno > 1 )
							{
								stringstream ss;
								ss << "As turmas " << strTurmasTipoCred;
								int counter=0;
								ITERA_GGROUP_N_PT( itTurma, turmasForaDoTurno, int )
								{
									counter++;
									ss << *itTurma;
									if ( counter + 1 < nroTurmasForaDoTurno ) ss << ", ";			// antes do penultimo
									else if ( counter + 1 == nroTurmasForaDoTurno ) ss << " e ";	// penultimo
								}

								ss << "da disciplina " << discEquiv->getCodigo()
									<< ( discEhEquiv ?"(equivalente)":" ")
									<< " estão em turno diferente do turno do aluno.";								
							
								naoAtendimento->addMotivo( ss.str() );
							}
							#pragma endregion

							#pragma region Turmas com choques de horários com o aluno
							map< int, map< Trio< int , int , Disciplina* >, GGroup<Aula*,LessPtr<Aula>> > >::iterator 
								it_turma_criada = turmas_choque_horarios.begin();
							for(;it_turma_criada != turmas_choque_horarios.end(); it_turma_criada++)
							{
								int turmaCriada = it_turma_criada->first;
									
								turmasSemMotivo.remove( turmaCriada );

								map< Trio< int , int , Disciplina* >, GGroup<Aula*,LessPtr<Aula>> > ::iterator 
									it_turmas_choque_horarios = it_turma_criada->second.begin();
								for(;it_turmas_choque_horarios != it_turma_criada->second.end(); it_turmas_choque_horarios++)
								{
									Disciplina *discChoque = (*it_turmas_choque_horarios).first.third;
									string strChoqueTipoCred("");
									if ( discChoque->temParPratTeor() && discChoque->getId() < 0 )
										strChoqueTipoCred = "pratica ";
									else if ( discChoque->temParPratTeor() )
										strChoqueTipoCred = "teorica ";

									// HOUVE CHOQUE DE HORARIO COM OUTRAS AULAS DO ALUNO
									stringstream ss2;
									ss2 << "A turma " << strTurmaTipoCred
										<< turmaCriada << " da disciplina " << discEquiv->getCodigo()
										<< ( discEhEquiv ? " (equivalente)":"" ) 
										<< " tem choque nos horários do aluno com a turma "	<< strChoqueTipoCred
										<< (*it_turmas_choque_horarios).first.second
										<< " da disciplina " << discChoque->getCodigo()
										<< " no campus " << (*it_turmas_choque_horarios).first.first << ".";
									naoAtendimento->addMotivo( ss2.str() );				
								}
							}
							#pragma endregion

							#pragma region Turmas sem motivo encontrado para não inserção do aluno
							for ( auto itTurmaSemMot = turmasSemMotivo.begin(); 
								itTurmaSemMot != turmasSemMotivo.end(); itTurmaSemMot++ )
							{
								stringstream ss;
								ss << "Não foi encontrado motivo para não inserção do aluno na turma "
									<< strTurmaTipoCred << *itTurmaSemMot 
									<< " da disciplina " << discEquiv->getCodigo()
									<< ( discEhEquiv ? " (equivalente)":"" ) << ".";
								if ( discPratica )
									ss << " Verifique se o aluno pode ser atendido em alguma turma teórica associada.";
								naoAtendimento->addMotivo( ss.str() );									
							}
							#pragma endregion
						}

						if ( totalTurmasCriadas == discEquiv->getNumTurmas() )
						{
							// FOI CRIADO O MAXIMO DE TURMAS		
						
							stringstream ss2;
							ss2 << "Não é possível abrir mais turmas " << strTurmasTipoCred
								<< "para a disciplina " << discEquiv->getCodigo()
								<< ( discEhEquiv ? " (equivalente)":"" ) << ".";
							naoAtendimento->addMotivo( ss2.str() );
						}
						else
						{
							this->verificaPossivelNovaTurma( naoAtendimento, discEquiv, ad, campusAlunoId, *mapAlsDemNaoAtend );							
						}
						#pragma endregion
					}
					else
					{
						if (bd) std::cout<<"\n\t4.3"; fflush(NULL);

						/* 
						Disciplina Nao Foi Criada
						- verificar se a disciplina poderia ser criada no turno ofertado
						- Verificar se houve demanda suficiente
						- Verificar se havia sala disponivel para criar a disciplina
						*/
						#pragma region Disciplina_Nao_Criada
					
						if ( !discEquiv->possuiTurnoIES( turnoIES ) )
						{
							// DISCIPLINA COM HORARIO DISPONIVEL EM TURNO DIFERENTE DA OFERTA						
							stringstream ss2;
							ss2 << "A disciplina " 
								<< discEquiv->getCodigo() 
								<< ( discEhEquiv ? " (EQUIVALENTE)":" " )
								<< ", não tem horários suficientes disponíveis no turno "
								<< demanda->getTurnoIES()->getNome() << " associado ao aluno.";
							naoAtendimento->addMotivo( ss2.str() );
				
							continue;		
						}
					
						if (bd) std::cout<<"\n\t\t4.31"; fflush(NULL);
						

						#pragma region Verifica demanda mínima necessária
						int demandaSizeNoTurnoIES = 0;
						if ( minAlunosNaTurma > 0 )
						{
							bool formando = false;
								
							if (bd) std::cout<<"\n\t\t4.32"; fflush(NULL);

							if ( mapAlsDemNaoAtend!=nullptr )
							{
								auto itTurnoNaoAtend = mapAlsDemNaoAtend->begin();
								for ( ; itTurnoNaoAtend != mapAlsDemNaoAtend->end(); itTurnoNaoAtend++ )
								{
									if ( itTurnoNaoAtend->first->getId() != turnoIESid )
										continue;

									ITERA_GGROUP_LESSPTR(itAlDem, itTurnoNaoAtend->second, AlunoDemanda)
									{	
										demandaSizeNoTurnoIES++;

										if ( (*itAlDem)->getAluno()->ehFormando() )
										{
											formando = true;
										}
									}
								}
							}
		
							if (bd) std::cout<<"\n\t\t4.33"; fflush(NULL);

							if ( demandaSizeNoTurnoIES < minAlunosNaTurma )
							if ( ( problemData->parametros->violar_min_alunos_turmas_formandos && !formando ) ||
								 ( !problemData->parametros->violar_min_alunos_turmas_formandos ) )
							{
								// DEMANDA MINIMA NAO ATINGIDA
								stringstream ss2;
								ss2 << "Demanda mínima não atingida pela disciplina " 
									<< discEquiv->getCodigo()
									<< ( discEhEquiv ? " (equivalente)":" " )
									<< ", no turno " << turnoIESname << ".";
								naoAtendimento->addMotivo( ss2.str() );
				
								this->nao_atendimentos->add( naoAtendimento );

								continue;
							}
						}
						#pragma endregion

						if (bd) std::cout<<"\n\t\t4.34"; fflush(NULL);

						this->verificaPossivelNovaTurma( naoAtendimento, discEquiv, ad, campusAlunoId, *mapAlsDemNaoAtend );
						
						if (bd) std::cout<<"\n\t\t4.35"; fflush(NULL);

						#pragma endregion
					}
				}
			
				if (bd) std::cout<<"\n5"; fflush(NULL);

				this->nao_atendimentos->add( naoAtendimento );

				if (bd) std::cout<<"\n6"; fflush(NULL);
			}
		}
	}

	
	int dif = CentroDados::getLastRunTime();
    CentroDados::stopTimer();
	std::cout << " " << dif << "sec"; fflush(NULL);

}

void ProblemSolution::verificaPossivelNovaTurma( 
					NaoAtendimento *naoAtendimento, 
					Disciplina *discEquiv, 
					AlunoDemanda *ad, 
					int campusAlunoId, 
					const map< TurnoIES*, GGroup<AlunoDemanda*, LessPtr<AlunoDemanda>>, LessPtr<TurnoIES> > &mapAlsDemNaoAtend )
{	
	// ---------------------------------
	// Saída com detalhes para análise interna
	bool printMotivosInternos = CentroDados::getPrintLogs() && false;
	ofstream outMotivosInternos;
	if (printMotivosInternos)
	{
		outMotivosInternos.open( "infoExtraPossivelNovaTurm.txt", ios::app );
	}
	
	// ---------------------------------
	// Referencia para ProblemData
	ProblemData* const problemData = CentroDados::getProblemData();

	// ---------------------------------
	// Debug
	bool bd=false;
	
	// ---------------------------------
	// Equiv
	bool discEhEquiv = ( ad->demanda->disciplina != discEquiv );
	
	// ---------------------------------
	// Disciplina pratica resultante de divisão de créditos práticos+teóricos
	bool discPratica=false;
	if ( discEquiv->getId() < 0 )
		discPratica=true;
					
	string strTurmaTipoCred("");
	string strTurmasTipoCred("");
	if ( discPratica )
	{
		strTurmaTipoCred = "prática ";
		strTurmasTipoCred = "práticas ";
	}
	else if ( discEquiv->temParPratTeor() )
	{
		strTurmaTipoCred = "teórica ";
		strTurmasTipoCred = "teóricas ";
	}
	
	// ---------------------------------
	// Calcula minimo de alunos na turma
	int minAlunosNaTurma = 0;
	if ( problemData->parametros->min_alunos_abertura_turmas && !discEquiv->eLab() )
	{
		minAlunosNaTurma = problemData->parametros->min_alunos_abertura_turmas_value;
	}
	else if ( problemData->parametros->min_alunos_abertura_turmas_praticas && discEquiv->eLab() )
	{
		minAlunosNaTurma = problemData->parametros->min_alunos_abertura_turmas_praticas_value;
	}
	
	// ---------------------------------
	// Acha as aulas do aluno
	Aluno *aluno = ad->getAluno();
	AlunoSolution* alSol=nullptr;
	auto itAulasAluno = this->mapAlunoSolution.find(aluno);				
	if ( itAulasAluno != this->mapAlunoSolution.end() )
		alSol = itAulasAluno->second;
	

	// ---------------------------------------------------------------------------------------------------
	// VERIFICA SALAS

	map< Sala*, vector< map<int /*dia*/, GGroup<HorarioAula*, LessPtr<HorarioAula>> > >, LessPtr<Sala> >							
		mapSalaDiaHorsLivres = this->procuraCombinacaoLivreEmSalas( discEquiv, ad->demanda->getTurnoIES(), campusAlunoId );
									
	if ( mapSalaDiaHorsLivres.size() == 0 )		// Não há salas com horários livres suficientes
	{
		stringstream ss;						
		stringstream ss2;
		ss2 << "Não existe sala com horários livres suficientes para abrir turmas "
			<< strTurmasTipoCred << "da disciplina " 
		<< discEquiv->getCodigo() << ( discEhEquiv ? " (equivalente)":"" ) << ".";
		naoAtendimento->addMotivo( ss2.str() );													
	}						
	else										// Há salas com horários livres suficientes
	{
		// Verifica para cada sala com horário livre se há choque de horários com o aluno
		stringstream ss1ComChoqueComAlunos;
		stringstream ss2ComChoqueComAlunos;
		ss1ComChoqueComAlunos << "Não há alunos com horários livres em comum "
						<< "no turno " << ad->demanda->getTurnoIES()->getNome()
						<< " suficientes "
						<< "para abrir nova turma " << strTurmaTipoCred 
						<< "da disciplina " << discEquiv->getCodigo()
						<< ( discEhEquiv ? " (equivalente)":"" );	
		int opcaoComChoqueComAlunos=0;

		stringstream ss1SemChoque;
		stringstream ss2SemChoque;
		ss1SemChoque << "Possibilidade para abrir nova turma " << strTurmaTipoCred 
			<< "da disciplina " << discEquiv->getCodigo()
			<< ( discEhEquiv ? " (equivalente)":"" )
			<< " encontrada";	
		int opcaoSemChoque=0;

		stringstream ss1ComChoqueEmSala;
		stringstream ss2ComChoqueEmSala;
		ss1ComChoqueEmSala << "Não há horários livres "
					<< "no turno " << ad->demanda->getTurnoIES()->getNome()
					<< " em comum com o aluno suficientes "
					<< "para abrir mais turmas " << strTurmasTipoCred 
					<< "da disciplina " << discEquiv->getCodigo()
					<< ( discEhEquiv ? " (equivalente)":"" );
		int opcaoComChoqueEmSala=0;

		map< Sala*, vector< map<int /*dia*/, GGroup<HorarioAula*, LessPtr<HorarioAula>> > >, LessPtr<Sala> >::iterator
			itSala = mapSalaDiaHorsLivres.begin();
		for ( ; itSala != mapSalaDiaHorsLivres.end(); itSala++ )
		{
			Sala *sala = itSala->first;
			vector< map<int /*dia*/, GGroup<HorarioAula*, LessPtr<HorarioAula>> > > 
				*mapOpcoesSalaLivre = & itSala->second;

			if (bd) std::cout<<"\n\t\t\t4.34.1  sala "<< sala->getId(); fflush(NULL);

			map< int, GGroup< Trio<int,int,Disciplina*> > > choques;
								
			if ( alSol!=nullptr )
				alSol->procuraChoqueDeHorariosAluno( *mapOpcoesSalaLivre, choques );

			if (bd) std::cout<<"\n\t\t\t4.34.2"; fflush(NULL);

			if ( choques.size() == itSala->second.size() )	// Todas as opcoes da sala têm choques
			{
				opcaoComChoqueEmSala++;
				ss2ComChoqueEmSala << sala->getCodigo() << "; ";
			}
			else											// Há opção sem choque
			{
				map< int /*opcao*/, map<int /*dia*/, GGroup<HorarioAula*, LessPtr<HorarioAula>> > > semChoquesNaSala;

				for ( int at = 0; at < mapOpcoesSalaLivre->size(); at++ )
				{
					if ( choques.find(at) == choques.end() )
					{
						semChoquesNaSala[at] = (*mapOpcoesSalaLivre)[at];
					}
				}
									
				map< int /*opção*/, GGroup<AlunoDemanda*, LessPtr<AlunoDemanda>> > mapOpcaoAlunosSemChoque;

				// Verifica cjt mínimo de alunos com disponibilidade em uma das opções sem choque
				this->procuraOpcoesSemChoque( semChoquesNaSala, mapAlsDemNaoAtend, mapOpcaoAlunosSemChoque );

				if (bd) std::cout<<"\n\t\t\t4.34.3"; fflush(NULL);

				bool opcaoEncontrada=false;
				auto itAlunosSemChoque = mapOpcaoAlunosSemChoque.begin();
				for ( ; itAlunosSemChoque != mapOpcaoAlunosSemChoque.end(); itAlunosSemChoque++ )
				{
					int opcaoIdx = itAlunosSemChoque->first;
					int nroAlunos = itAlunosSemChoque->second.size();
					bool haFormando=false;

					if ( problemData->parametros->violar_min_alunos_turmas_formandos )
					{
						ITERA_GGROUP_LESSPTR( itAl, itAlunosSemChoque->second, AlunoDemanda )
						{
							if ( itAl->getAluno()->ehFormando() )
								haFormando=true;
						}
					}

					if ( nroAlunos > minAlunosNaTurma || haFormando)
					{
						// opção encontrada para nova turma!
						opcaoEncontrada=true;

						map<int /*dia*/, GGroup<HorarioAula*, LessPtr<HorarioAula>> >
							*opcao = & semChoquesNaSala[opcaoIdx];
										
						if (printMotivosInternos)
						{
							outMotivosInternos << "\n\n----------------------------------------------";
							outMotivosInternos << "\nAnalise do nao atendimento do AlunoDemanda Id " << ad->getId();
							outMotivosInternos << "\nPossivel nova turma " << strTurmaTipoCred 
								<< "no turno " << ad->demanda->getTurnoIES()->getNome() << "com seguinte alocacao:";
							outMotivosInternos << "\nDisc " << discEquiv->getId() << "  " << discEquiv->getCodigo();
							if ( discEquiv->temParPratTeor() )
								outMotivosInternos << " (possui par teorico/prat)";
							outMotivosInternos << " - " << discEquiv->getTotalCreditos() << " creditos.";
							outMotivosInternos << "\nSala " << sala->getId() << "  " << sala->getCodigo();
							auto itDiaOpcao = opcao->begin();
							for ( ; itDiaOpcao != opcao->end(); itDiaOpcao++ )
							{												
								outMotivosInternos << "\nDia " << itDiaOpcao->first	<< "\nHorarios: ";
								ITERA_GGROUP_LESSPTR( itHor, itDiaOpcao->second, HorarioAula )
								{
									outMotivosInternos << " " << itHor->getInicio().hourMinToStr();
								}
							}

							outMotivosInternos << "\nAlsDem (" << itAlunosSemChoque->second.size() << " alunos): ";
							ITERA_GGROUP_LESSPTR( itAl, itAlunosSemChoque->second, AlunoDemanda )
							{
								outMotivosInternos << " " << itAl->getId();
								if ( itAl->getAluno()->ehFormando() )
									outMotivosInternos << " (formando)";
							}
						}
					}
				}

				if ( !opcaoEncontrada )
				{
					opcaoComChoqueComAlunos++;
					ss2ComChoqueComAlunos << sala->getCodigo() << "; ";
					// nenhuma das opcoes na sala atinge o mínimo de alunos com horarios em comum
				}
				else
				{
					opcaoSemChoque++;
					ss2SemChoque << sala->getCodigo() << "; ";
				}
			}
		}

		if (bd) std::cout<<"\n\t\t\t4.35"; fflush(NULL);

		if ( opcaoSemChoque )
		{
			stringstream ss;
			ss << ss1SemChoque.str();

			if ( opcaoSemChoque == 1 ) ss << " na sala ";
			else ss << " nas salas ";
									
			ss << ss2SemChoque.str();

			if ( discEquiv->temParPratTeor() )
				ss << " Verificar se a parte "
				<< ( discEquiv->getId() < 0 ? "teórica ": "prática " )
					<< "também poderia ser alocada.";

			naoAtendimento->addMotivo( ss.str() );
		}
		if ( opcaoComChoqueEmSala )
		{
			stringstream ss;
			ss << ss1ComChoqueEmSala.str();

			if ( opcaoComChoqueEmSala == 1 ) ss << " na sala ";
			else ss << " nas salas ";
									
			ss << ss2ComChoqueEmSala.str();

			naoAtendimento->addMotivo( ss.str() );
		}
		if ( opcaoComChoqueComAlunos )
		{
			stringstream ss;
			ss << ss1ComChoqueComAlunos.str();

			if ( opcaoComChoqueComAlunos == 1 ) ss << " na sala ";
			else ss << " nas salas ";
									
			ss << ss2ComChoqueComAlunos.str();

			naoAtendimento->addMotivo( ss.str() );
		}
	}

	if (printMotivosInternos)
	{
		outMotivosInternos.close();
	}
}

void ProblemSolution::verificaUsoDeProfsVirtuais()
{
	CentroDados::startTimer();

	std::cout<<"\nVerifica Uso De Profs Virtuais..."; fflush(NULL);	

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
	
   ProblemData *problemData = CentroDados::getProblemData();
	
   // Flags para a escrita dos arquivos de saida
   bool escreveAlocacoesAlteradas = false;
   
   ofstream saidaAlteracoesFile;
   string saidaAlteracoesFilename("saidaTestaTrocarProfVirtualPorReal_Alteracoes.txt");
   saidaAlteracoesFile.open(saidaAlteracoesFilename, ios::out);
   
   bool DETALHAR_TODOS_OS_MOTIVOS = true;	// Não pára no primeiro motivo encontrado. Percorre todos.
   bool DETALHAR_TODOS_PROFS = false;		// Detalha até mesmo os profs sem magisterio na disciplina ou nao cadastrados no campus
   bool IMPRIMIR_SOMENTE_RESUMO = true;
   
   // PERCORRE TODAS AS TURMAS COM PROFESSORES VIRTUAIS

   auto itFinder1 = mapSolTurmaProfVirtualDiaAula.begin();
   for ( ; itFinder1 != mapSolTurmaProfVirtualDiaAula.end(); itFinder1++ )
   {
	    int cpId = itFinder1->first;
		Campus* cp = problemData->refCampus[cpId];
		
		auto itFinder2 = itFinder1->second.begin();
		for ( ; itFinder2 != itFinder1->second.end(); itFinder2++ )
		{
			Disciplina* disciplina = itFinder2->first;
			int discId = disciplina->getId();

			auto itFinder3 = itFinder2->second.begin();
			for ( ; itFinder3 != itFinder2->second.end(); itFinder3++ )
			{
			   int turma = itFinder3->first;
			   
			   std::vector<Aula*> listaAulasDoPar;
			   ProfessorVirtualOutput* profVirtualOut = nullptr;

			   #pragma region AGRUPA AULAS DA TURMA E SETA PROFESSOR VIRTUAL USADO NA TURMA
			   auto itFinder4 = itFinder3->second.begin();			   
			   for ( ; itFinder4 != itFinder3->second.end(); itFinder4++ )
			   {
				   profVirtualOut = itFinder4->first;

				   auto itFinder5 = itFinder4->second.begin();
				   for ( ; itFinder5 != itFinder4->second.end(); itFinder5++ )
				   {
					   Aula *aula = itFinder5->second;

					   listaAulasDoPar.push_back(aula);
				   }
			   }
			   #pragma endregion

			   if ( itFinder3->second.size() > 1 )
			   {
				   std::stringstream msg;
				   msg << "Mais de um prof ministrando aulas para a turma " << turma << " da disc " << discId;
				   CentroDados::printError("void ProblemSolution::verificaUsoDeProfsVirtuais()", msg.str());
			   }
			   
			   std::vector<std::pair<Aula*, Professor*> >possiveisTrocasParcial;
			   std::vector<std::pair<Aula*, Professor*> > alocacoesAlteradasParcial;

			   std::map<std::pair<int, int>, std::pair<Aula, int> > variableOpsNaoDisponiveisHorarioIdsHorAulas;
			   
			   #pragma region FLAGS
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
			   #pragma endregion

			   Aula* aula0 = listaAulasDoPar[0];
			   
			   bool ehPrat = (bool) aula0->getCreditosPraticos();
			   
			   #pragma region PROCURA OBJETOS AlocacaoProfVirtual E TipoTitulacao
			   AlocacaoProfVirtual *alocacao = profVirtualOut->getAlocacao( abs( discId ), turma, cpId, ehPrat );
			   if ( alocacao == nullptr )
			   {
				   std::stringstream msg;
				   msg << "Alocacao nao encontrada. Disc" << abs( discId )
					   << ", turma "<< turma <<", cp "<< cpId <<", pratica "<< ehPrat;
				   CentroDados::printError("void ProblemSolution::verificaUsoDeProfsVirtuais()", msg.str());
				   continue;
			   }
			   
			   TipoTitulacao *titulacaoProfVirt = problemData->retornaTitulacaoPorId( profVirtualOut->getTitulacaoId() );
			   if ( titulacaoProfVirt==nullptr )
			   {
				   std::stringstream msg;
				   msg << "Titulacao id " << profVirtualOut->getTitulacaoId() << " nao encontrada";
				   CentroDados::printError("void ProblemSolution::verificaUsoDeProfsVirtuais()", msg.str());
				   continue;
			   }
			   #pragma endregion

			   #pragma region CONFERE TODOS OS PROFESSORES REAIS POSSÍVEIS PARA A TURMA

			   //Para cada aula confere todos os professores
			   auto itProf = problemData->refProfessores.begin();	
			   for (; itProf != problemData->refProfessores.end(); itProf++)
			   {
				    Professor* p = itProf->second;
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
						flagProfSemMagisterio = ! ( p->possuiMagisterioEm(aula0->getDisciplina()) );

						flagFaltaMagistrado = flagFaltaMagistrado && flagProfSemMagisterio;

						flagNroFaltaMagistrado += (flagProfSemMagisterio? 1 : 0);

						if( !flagProfSemMagisterio || DETALHAR_TODOS_PROFS ) // Se possui magisterio
						{
							flagProfSemTituloMinimo = ! ( p->getTitulacao() >= titulacaoProfVirt->getTitulacao() );
							
							flagFaltaMestreDoutor = flagFaltaMestreDoutor && flagProfSemTituloMinimo;

							flagNroFaltaMestreDoutor += (flagProfSemTituloMinimo? 1 : 0);

							if( !flagProfSemTituloMinimo || DETALHAR_TODOS_OS_MOTIVOS ) // Se a titulação for maior ou igual
							{			
								int acrescimo = aula0->getDisciplina()->getTotalCreditos() *
												aula0->getDisciplina()->getTempoCredSemanaLetiva();

								flagProfCHMaxima = ! ( quantChProfs[p->getId()] + acrescimo <= p->getChMax()*60 );
								
								flagFaltaCargaHoraria = flagFaltaCargaHoraria && flagProfCHMaxima;

								flagNroFaltaCargaHoraria += (flagProfCHMaxima? 1 : 0);

								if( !flagProfCHMaxima || DETALHAR_TODOS_OS_MOTIVOS ) // Se a carga horaria maxima desse professor nao é violada
								{
									auto itDiaHorsAlocados = mapSolProfRealDiaHorarios.find(p);
							
									// -------------------------------------------------
									// Percorre todas as aulas do trio e confere
									for(int i=0; i<listaAulasDoPar.size(); i++)
									{
										#pragma region CONFERE A I-ÉSIMA AULA DA TURMA

										if( !flagPossivelTroca && !DETALHAR_TODOS_OS_MOTIVOS ){
											break;
										}

										Aula* aula = listaAulasDoPar[i];
						
										HorarioAula* horarioAulaV = aula->getHorarioAulaInicial();
										int nCredDia = aula->getTotalCreditos();
						
										// Horarios ocupados no dia do professor
										std::vector<HorarioDia*> horariosAlocadosNoDia;
										if( itDiaHorsAlocados != mapSolProfRealDiaHorarios.end() )
										{
											int nroDiasUsados = itDiaHorsAlocados->second.size();

											auto itHorsAlocados = itDiaHorsAlocados->second.find( aula->getDiaSemana() );
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
													HorarioDia *h = horariosAlocadosNoDia[l];
													HorarioAula* hAula = h->getHorarioAula();
																								
													if( (hAula->getInicio() >= horarioAulaV->getInicio()) && (hAula->getInicio() < horarioAulaV->getFinal()) ||
														(hAula->getFinal() > horarioAulaV->getInicio()) && (hAula->getFinal() <= horarioAulaV->getFinal()) )
													{
														// Há conflito
														// Seta as flags para zero
														flagAltera = 0;
														flagPossivelTroca = 0;
														flagProfHorarioConflito = 1;
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

												if(aula->getDiaSemana()==itrHorDispProf->getDia()){		// Se for no mesmo dia
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

												std::pair<int, int> chaveMap(aula->getDiaSemana(), horarioAulaV->getId());

												if(variableOpsNaoDisponiveisHorarioIdsHorAulas.find(chaveMap) == variableOpsNaoDisponiveisHorarioIdsHorAulas.end()){										
													std::pair<Aula, int> pairVHorario(*aula, horarioAulaV->getId());
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
												if( itDiaHorsAlocados != mapSolProfRealDiaHorarios.end() )
												{
													auto itHorsDiaSeg = itDiaHorsAlocados->second.find( aula->getDiaSemana() + 1 );
													if ( itHorsDiaSeg != itDiaHorsAlocados->second.end() )
													{
														for ( int at = 0; at < itHorsDiaSeg->second.size(); at++ )
														{
															HorarioDia* hd = itHorsDiaSeg->second[at];
															if ( less_than( hd->getHorarioAula()->getInicio(), dtf_maisDescanso ) )
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
												if( itDiaHorsAlocados != mapSolProfRealDiaHorarios.end() )
												{
													auto itHorsDiaAnt = itDiaHorsAlocados->second.find( aula->getDiaSemana() - 1 );
													if ( itHorsDiaAnt != itDiaHorsAlocados->second.end() )
													{
														for ( int at = 0; at < itHorsDiaAnt->second.size(); at++ )
														{
															HorarioDia *hd = itHorsDiaAnt->second[at];
															if ( less_than( dti_menosDescanso, hd->getHorarioAula()->getFinal() ) )
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
											possiveisTrocasParcial.push_back(std::pair<Aula*, Professor*>(aula, p));
										}
										if( flagAltera ){							
											alocacoesAlteradasParcial.push_back(std::pair<Aula*, Professor*>(aula, p));							
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
								<< titulacaoProfVirt->getNome() << ".";
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
						if ( possiveisTrocasParcial.size() > 0 )
						{
							stringstream ss;
							ss << "Incluir na disponibilidade do professor os horários:";
							for(int i=0; i<possiveisTrocasParcial.size();i++)
							{
								Aula* aula = possiveisTrocasParcial[i].first;
								int hour1 = aula->getHorarioAulaInicial()->getInicio().getHour();
								int min1 = aula->getHorarioAulaInicial()->getInicio().getMinute();
								int ncreds = aula->getTotalCreditos();
								int tempoAula = aula->getDisciplina()->getTempoCredSemanaLetiva();

								DateTime final = aula->getHorarioAulaInicial()->getInicio();
								final.addMinutes( ncreds*tempoAula );
								int hour2 = final.getHour();
								int min2 = final.getMinute();
								ss << " de " << hour1 << ":" << (min1<10 ? "0":"") << min1;
								ss << " a " << hour2 << ":" << (min2<10 ? "0":"") << min2;
								ss << " n" << problemData->getDiaEmString( aula->getDiaSemana(), true ) << ";";
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
					}													  
					possiveisTrocasParcial.clear();					  
			
					if( flagAltera )
					{
						flagFaltaHorarioDisponivel = 0;
						variableOpsNaoDisponiveisHorarioIdsHorAulas.clear();

						if(!escreveAlocacoesAlteradas)
						{
							escreveAlocacoesAlteradas = true;
							if (!saidaAlteracoesFile){
								cout << "Can't open output file " << saidaAlteracoesFilename << endl;
							}
							else{
								saidaAlteracoesFile << "-------------------------------------------------------------------------------------------------"<<endl;
								saidaAlteracoesFile << "------------------------ALTERACOES ENTRE DE PROFESSORES VIRTUAIS POR REAIS-----------------------"<<endl;
								saidaAlteracoesFile << "---------Considerando todas as restrições fortes, ou seja, indica possivel erro no modelo--------"<<endl;
								saidaAlteracoesFile << "-------------------------------------------------------------------------------------------------\n"<<endl;
								saidaAlteracoesFile << "=================================================================================================\n"<<endl;
							}
						}

						if (saidaAlteracoesFile) 
						{
							for(int i=0; i<alocacoesAlteradasParcial.size();i++)
							{
								std::pair<Aula*, Professor*> alteracao = alocacoesAlteradasParcial[i];

								saidaAlteracoesFile << "Aula IDENTIFICADA:" <<endl;
								saidaAlteracoesFile << alteracao.first->toString() <<endl <<endl;
								saidaAlteracoesFile << "PROFESSOR REAL POSSIVEL PARA ALTERACAO:" <<endl;
								saidaAlteracoesFile << "ID: " << alteracao.second->getId() <<endl;
								saidaAlteracoesFile << "NOME: " << alteracao.second->getNome() <<endl;
								saidaAlteracoesFile << "CPF: " << alteracao.second->getCpf() <<endl;
								saidaAlteracoesFile << "TITULO: " << alteracao.second->titulacao->getNome() <<endl;
								saidaAlteracoesFile << "VALOR CREDITO: " << alteracao.second->getValorCredito() <<endl <<endl;
								saidaAlteracoesFile << "=================================================================================================\n"<<endl;
							}	
						}
					}
					
					alocacoesAlteradasParcial.clear();									
			   }
			   
			   #pragma endregion
			   			   
			   #pragma region IMPRIME CAUSAS E O NRO DE PROFESSORES ASSOCIADOS À CAUSA
			   if ( IMPRIMIR_SOMENTE_RESUMO )
			   {
				   //Gera o relatorio de motivos da criação de professores virtuais
				   
				   if( flagNroFaltaCadastroNoCampus ){
					    stringstream ss;
						if ( flagNroFaltaCadastroNoCampus == 1 )
							ss << flagNroFaltaCadastroNoCampus << " professor não cadastrado no campus dessa turma.";
						else
							ss << flagNroFaltaCadastroNoCampus << " professores não cadastrados no campus dessa turma.";
						alocacao->addDescricaoDoMotivo( -1, ss.str() );
				   }			   
				   if( flagNroFaltaMagistrado ){
					    stringstream ss;
						if ( flagNroFaltaMagistrado == 1 )
							ss << flagNroFaltaMagistrado << " professor não possui habilitação nesta disciplina.";
						else
							ss << flagNroFaltaMagistrado << " professores não possuem habilitação nesta disciplina.";
						alocacao->addDescricaoDoMotivo( -1, ss.str() );
				   }			   
				   if( flagNroFaltaMestreDoutor ){
						stringstream ss;
						if ( flagNroFaltaMestreDoutor == 1 )
							ss << flagNroFaltaMestreDoutor << " professor não possui titulação necessária para manter o mínimo de mestres e doutores no curso atendido pela turma.";
						else
							ss << flagNroFaltaMestreDoutor << " professores não possuem titulação necessária para manter o mínimo de mestres e doutores no curso atendido pela turma.";
						alocacao->addDescricaoDoMotivo( -1, ss.str() );
				   }
				   if( flagNroFaltaCargaHoraria ){
						stringstream ss;
						if ( flagNroFaltaCargaHoraria == 1 )
							ss << flagNroFaltaCargaHoraria << " professor excederia a carga horária máxima semanal.";
						else
							ss << flagNroFaltaCargaHoraria << " professores excederiam a carga horária máxima semanal.";
						alocacao->addDescricaoDoMotivo( -1, ss.str() );
				   }
				   if( flagNroFaltaHorarioConflito ){				    
						stringstream ss;
						ss << "Para ";
						if ( flagNroFaltaHorarioConflito == 1 )
							ss << flagNroFaltaHorarioConflito << " professor há conflito entre os horários da turma com os horários de outras turmas alocadas.";
						else
							ss << flagNroFaltaHorarioConflito << " professores há conflito entre os horários da turma com os horários de outras turmas alocadas.";
						alocacao->addDescricaoDoMotivo( -1, ss.str() );						
					}
					if( flagNroFaltaHorarioDisponivel && (!variableOpsNaoDisponiveisHorarioIdsHorAulas.empty()) ){
						stringstream ss;
						if ( flagNroFaltaHorarioDisponivel == 1 )
							ss << flagNroFaltaHorarioDisponivel << " professor não possui horários disponíveis para esta turma.";
						else
							ss << flagNroFaltaHorarioDisponivel << " professores não possuem horários disponíveis para esta turma.";
						alocacao->addDescricaoDoMotivo( -1, ss.str() );
					}
					if ( flagNroFaltaMaxDiasSemana ){
						stringstream ss;
						if ( flagNroFaltaMaxDiasSemana == 1 )
							ss << flagNroFaltaMaxDiasSemana << " professor já atingiu o máximo de dias da semana.";
						else
							ss << flagNroFaltaMaxDiasSemana << " professores já atingiram o máximo de dias da semana.";
						alocacao->addDescricaoDoMotivo( -1, ss.str() );
					}
					if ( flagNroFaltaMinCredsDiarios ){
						stringstream ss;
						ss << "O mínimo de créditos nos dias das aulas da turma não foi atingido para ";
						if ( flagNroFaltaMinCredsDiarios == 1 )
							ss << flagNroFaltaMinCredsDiarios << " professor.";
						else
							ss << flagNroFaltaMinCredsDiarios << " professores.";
						alocacao->addDescricaoDoMotivo( -1, ss.str() );
					}
					if ( flagNroFaltaInterjornada ){
						stringstream ss;
						if ( flagNroFaltaInterjornada == 1 )
							ss << flagNroFaltaInterjornada << " professor já atingiu a interjornada mínima.";
						else
							ss << flagNroFaltaInterjornada << " professores já atingiram a interjornada mínima.";
						alocacao->addDescricaoDoMotivo( -1, ss.str() );
					}
				}
				#pragma endregion
			   
			   #pragma region IMPRIME CAUSAS COMUNS A TODOS OS PROFESSORES
			   if ( ! IMPRIMIR_SOMENTE_RESUMO )
			   {
				   //Gera o relatorio de motivos da criação de professores virtuais
				   
				   if(flagFaltaCadastroNoCampus == 1){				   
						alocacao->addDescricaoDoMotivo( -1, "Não existe professor cadastrado no campus dessa turma." );
				   }			   
				   if(flagFaltaMagistrado == 1){					 
						alocacao->addDescricaoDoMotivo( -1, "Não existe professor com habilitação nesta disciplina." );
				   }			   
				   if(flagFaltaMestreDoutor == 1){
						alocacao->addDescricaoDoMotivo( -1, "Não existe professor com titulação necessária para manter o mínimo de mestres e doutores no curso atendido pela turma." );
				   }
				   if(flagFaltaCargaHoraria == 1){
						alocacao->addDescricaoDoMotivo( -1, "Não existe professor capaz de ministrar esta turma sem exceder a sua carga horaria maxima semanal." );
				   }
				   if(flagFaltaHorarioConflito == 1){
						alocacao->addDescricaoDoMotivo( -1, "Há conflito entre os horários da turma com os horários de outras turmas alocadas aos professores." );
					}
					if( (flagFaltaHorarioDisponivel == 1) && (!variableOpsNaoDisponiveisHorarioIdsHorAulas.empty()) ){
						alocacao->addDescricaoDoMotivo( -1, "Não há horarios disponiveis para esta turma." );
					}
					if ( flagFaltaMaxDiasSemana > 0 ){
						alocacao->addDescricaoDoMotivo( -1, "Todos os professores já atingiram o máximo de dias da semana." );
					}
					if ( flagFaltaMinCredsDiarios > 0 ){
						alocacao->addDescricaoDoMotivo( -1, "O mínimo de créditos nos dias das aulas da turma não foi atingido para nenhum professor." );
					}
					if ( flagFaltaInterjornada == 1 ){
						alocacao->addDescricaoDoMotivo( -1, "Todos os professores já atingiram a interjornada mínima." );
					}
				}
				#pragma endregion
			}
		}
	}
		
	if ( saidaAlteracoesFile )
		saidaAlteracoesFile.close();

	int dif = CentroDados::getLastRunTime();
    CentroDados::stopTimer();
	std::cout << " " << dif << "sec"; fflush(NULL);
	
	verificaNrDiscSimultVirtual();	
}

void ProblemSolution::computaMotivos( bool motivoNaoAtend, bool motivoUsoPV )
{
	cout << "\nComputando motivos..."; fflush(0);

	clearMapsDaSolucao();
	constroiMapsDaSolucao();
	imprimeMapsDaSolucao();
	if (motivoNaoAtend) verificaNaoAtendimentosTaticos();
	if (motivoUsoPV) verificaUsoDeProfsVirtuais();
	clearMapsDaSolucao();
}

void ProblemSolution::imprimeIndicadores()
{
	cout << "\timprimindo Indicadores..."; fflush(0);

	// -----------------------------------------------------------------------------------------------
	#pragma region TURMAS COM COMPARTILHAMENTO DE CURSOS
	Indicadores::printSeparator(1);

	auto itCpId = mapSolTurmaCursos.begin();
	for ( ; itCpId != mapSolTurmaCursos.end(); itCpId++ )
	{
		int nroTurmasMaisDeUmCurso=0;
		auto itDisc = itCpId->second.begin();
		for ( ; itDisc != itCpId->second.end(); itDisc++ )
		{
			// considera turmas teóricas apenas
			if ( itDisc->first->getId() < 0 )
				continue;

			auto itTurma = itDisc->second.begin();
			for ( ; itTurma != itDisc->second.end(); itTurma++ )
			{
				int nCursos = itTurma->second.size();
				if ( nCursos>1 ) nroTurmasMaisDeUmCurso++;
			}			
		}
		stringstream msg;
		msg << "\nNumero de turmas com compartilhamento de curso no campus " << itCpId->first << ": " << nroTurmasMaisDeUmCurso;
		Indicadores::printIndicador( msg.str() );
	}
	#pragma endregion

	#pragma region GERAL
	Indicadores::printSeparator(2);
	Indicadores::printIndicador( "\nNumero total de alunos-demanda de P1: ", getNroTotalAlunoDemandaP1() );
	Indicadores::printIndicador( "\nNumero total de alunos-demanda de P1 atendidos: ", getNroAlunoDemAtendP1() );
	Indicadores::printIndicador( "\nNumero total de alunos-demanda de P1 não atendidos: ", getNroAlunoDemNaoAtendP1() );
	Indicadores::printIndicador( "\nNumero total de alunos-demanda de P2 atendidos: ", getNroAlunoDemAtendP2() );
	Indicadores::printIndicador( "\nNumero total de alunos-demanda de P1+P2 atendidos: ", getNroAlunoDemAtendP1P2() );
	#pragma endregion

	#pragma region FORMANDOS
	Indicadores::printSeparator(1);
	double nroAlDemFormP1 = getNroTotalAlunoDemFormandosP1();
	double nroAlDemFormAtendP1 = getNroTotalAlunoDemFormandosAtendP1();
	double percForm = ( nroAlDemFormP1>0 ? (100 * nroAlDemFormAtendP1 / nroAlDemFormP1) : 100 );
	Indicadores::printIndicador( "\nNumero total de alunos-demanda de formandos P1: ", (int) nroAlDemFormP1 );
	stringstream msgFormAtend;
	msgFormAtend << "\nNumero total de alunos-demanda de formandos P1 atendidos: " 
		<< (int) nroAlDemFormAtendP1 << " (" << percForm << "%)";
	Indicadores::printIndicador( msgFormAtend.str() );
	#pragma endregion

	#pragma region CALOUROS
	Indicadores::printSeparator(1);
	double nroAlDemCalouroP1 = getNroTotalAlunoDemCalourosP1();
	double nroAlDemCalouroAtendP1 = getNroTotalAlunoDemCalourosAtendP1();
	double percCalouro = ( nroAlDemFormP1>0 ? (100 * nroAlDemCalouroAtendP1 / nroAlDemCalouroP1) : 100 );
	Indicadores::printIndicador( "\nNumero total de alunos-demanda de calouros P1: ", (int) nroAlDemCalouroP1 );
	stringstream msgCalouAtend;
	msgCalouAtend << "\nNumero total de alunos-demanda de calouros P1 atendidos: " 
		<< (int) nroAlDemCalouroAtendP1 << " (" << percCalouro << "%)";
	Indicadores::printIndicador( msgCalouAtend.str() );
	#pragma endregion
}

void ProblemSolution::imprimeMapsDaSolucao()
{
	std::cout<<"\nimprime Maps Da Solucao..."; fflush(NULL);	

	// Map de aulas da solução por turma
	imprimeMapSolDiscTurmaDiaAula();
	
	// Maps de aulas da solução por aluno
	imprimeMapSolAlunoDiaDiscAulas();
	imprimeMapAlunoDiscTurmaCp();
	    
	// Map de aulas da solução por sala
	imprimeMapSalaDiaHorariosVagos();

	// Maps de aulas da solução com professor
	imprimeMapSolTurmaProfVirtualDiaAula();			
	imprimeMapSolProfRealDiaHorarios();
	imprimeQuantChProfs();

	// Histograma para disciplinas com profs virtuais e aulas simultaneas
	imprimeNrDiscSimultVirtual();
}

void ProblemSolution::imprimeMapSolDiscTurmaDiaAula()
{
	if (!CentroDados::getPrintLogs())
	{
		return;
	}

	ofstream outFile;
	string fileName("solDiscTurmaDiaAula.txt");
	outFile.open( fileName, ofstream::out );

	if ( !outFile )
	{
		std::cout << "\nErro em void ProblemSolution::imprimeMapSolDiscTurmaDiaAula():"
			<< " o arquivo " << fileName << " nao pode ser aberto.";
		return;
	}

	auto itMap1 = mapSolDiscTurmaDiaAula.begin();
	for ( ; itMap1 != mapSolDiscTurmaDiaAula.end(); itMap1++ )
	{
		int campusId = itMap1->first;

		map< Disciplina*, map< int /*turma*/, std::pair< std::map<int /*dia*/, Aula*>,
			GGroup<int /*alDem*/> > >, LessPtr<Disciplina> > *map2 = & itMap1->second;

		auto itMap2 = (*map2).begin();
		for ( ; itMap2 != (*map2).end(); itMap2++ )
		{
			Disciplina *disc = itMap2->first;

			map< int /*turma*/, std::pair< std::map<int /*dia*/, Aula*>,
				GGroup<int /*alDem*/> > > *map3 = & itMap2->second;

			auto itMap3 = (*map3).begin();
			for ( ; itMap3 != (*map3).end(); itMap3++ )
			{
				int turma = itMap3->first;
				
				outFile << "\n\n=========================================================================" 
					<< "\nCampus " << campusId << ", Turma " << turma << ", Disciplina " << disc->getId()
					<< "  -  " << disc->getTotalCreditos() << " creditos";
				
				std::pair< std::map<int /*dia*/, Aula*>, GGroup<int /*alDem*/> > 
					*pairMapDiaAulasAlsDem = & itMap3->second;
				
				GGroup<int /*alDem*/> *alunosDemandaId = & pairMapDiaAulasAlsDem->second;

				outFile << "\n--------";
				outFile << "\nAlunosDemanda (" << alunosDemandaId->size() << " alunos): ";
				ITERA_GGROUP_N_PT( itAlDemId, (*alunosDemandaId), int )
				{
					outFile << *itAlDemId << "; ";
				}
				outFile << "\n--------";
				
				std::map<int /*dia*/, Aula*> *mapDiaAula = & pairMapDiaAulasAlsDem->first;
				
				for( auto itDiaAula = mapDiaAula->begin(); itDiaAula != mapDiaAula->end(); itDiaAula++ )
				{
					Aula* aula = itDiaAula->second;

					outFile << "\nDia " << aula->getDiaSemana();					
					outFile << ", Sala " << aula->getSala()->getId();
					
					if ( aula->getHorarioAulaInicial() != NULL )
						outFile << ", Hi " << aula->getHorarioAulaInicial()->getInicio().getHour() << ":"
								<< aula->getHorarioAulaInicial()->getInicio().getMinute()
								<< " (id" << aula->getHorarioAulaInicial()->getId() << ")";
					if ( aula->getHorarioAulaFinal() != NULL )
						outFile << ", Hf " << aula->getHorarioAulaFinal()->getInicio().getHour() << ":"
								<< aula->getHorarioAulaFinal()->getInicio().getMinute() << " (id" 
								<< aula->getHorarioAulaFinal()->getId() << ")";
					
					outFile << "; " << aula->getTotalCreditos() << " creditos";
				}
			}
		}
	}
	outFile.close();
}

void ProblemSolution::imprimeMapSolAlunoDiaDiscAulas()
{
	if (!CentroDados::getPrintLogs())
	{
		return;
	}

	ofstream outFile;
	string fileName("solAlunoDiaDiscAulas.txt");
	outFile.open( fileName, ofstream::out );

	if ( !outFile )
	{
		std::cout << "\nErro em void ProblemSolution::imprimeMapSolAlunoDiaDiscAulas():"
			<< " o arquivo " << fileName << " nao pode ser aberto.";
		return;
	}

	for( auto itAluno = mapAlunoSolution.begin(); itAluno != mapAlunoSolution.end(); itAluno++ )
	{
		Aluno* aluno = itAluno->first;
		AlunoSolution *alSol = itAluno->second;

		alSol->mapDiaDiscAulasToStream( outFile );
	}
		
	outFile.close();
}
 
void ProblemSolution::imprimeMapAlunoDiscTurmaCp()
{
	if (!CentroDados::getPrintLogs())
	{
		return;
	}

	ofstream outFile;
	string fileName("solAlunoDiscTurmaCp.txt");
	outFile.open( fileName, ofstream::out );

	if ( !outFile )
	{
		std::cout << "\nErro em void ProblemSolution::imprimeMapAlunoDiscTurmaCp():"
			<< " o arquivo " << fileName << " nao pode ser aberto.";
		return;
	}

	for( auto itAluno = mapAlunoSolution.begin(); itAluno != mapAlunoSolution.end(); itAluno++ )
	{
		Aluno* aluno = itAluno->first;
		AlunoSolution *alSol = itAluno->second;

		alSol->mapAlDemDiscTurmaCpToStream( outFile );
	}
	
	outFile.close();
}

void ProblemSolution::imprimeMapSalaDiaHorariosVagos()
{
	if (!CentroDados::getPrintLogs())
	{
		return;
	}

	ofstream outFile;
	string fileName("solSalaDiaHorariosVagos.txt");
	outFile.open( fileName, ofstream::out );

	if ( !outFile )
	{
		std::cout << "\nErro em void ProblemSolution::imprimeMapSalaDiaHorariosVagos():"
			<< " o arquivo " << fileName << " nao pode ser aberto.";
		return;
	}
		
	for( auto itMapSala = mapSalaDiaHorariosVagos.begin(); itMapSala != mapSalaDiaHorariosVagos.end(); itMapSala++ )
	{
		Sala* sala = itMapSala->first;

		outFile << "\n\n-----------------------------------------------------------------------";
		outFile << "\nSala " << sala->getId() << "\t" << sala->getCodigo();
		for( auto itMapDia = itMapSala->second.begin(); itMapDia != itMapSala->second.end(); itMapDia++ )
		{
			int dia = itMapDia->first;
			
			outFile << "\n\tDia " << dia << ": ";
			for( auto itDti = itMapDia->second.begin(); itDti != itMapDia->second.end(); itDti++ )
			{
				DateTime dti = itDti->first;

				for( auto itDtf = itDti->second.begin(); itDtf != itDti->second.end(); itDtf++ )
				{
					DateTime dtf = itDtf->first;
				
					outFile << "  " << dti.getHour() << ":" << dti.getMinute()
							<< " - " << dtf.getHour() << ":" << dtf.getMinute() << " || ";
				}
			}
		}
	}
	
	outFile.close();
}
 
void ProblemSolution::imprimeMapSolTurmaProfVirtualDiaAula()
{
	if (!CentroDados::getPrintLogs())
	{
		return;
	}

	ofstream outFile;
	string fileName("solTurmaProfVirtualDiaAula.txt");
	outFile.open( fileName, ofstream::out );

	if ( !outFile )
	{
		std::cout << "\nErro em void ProblemSolution::imprimeMapSolTurmaProfVirtualDiaAula():"
			<< " o arquivo " << fileName << " nao pode ser aberto.";
		return;
	}
	
	for( auto itMapCpId = mapSolTurmaProfVirtualDiaAula.begin(); itMapCpId != mapSolTurmaProfVirtualDiaAula.end(); itMapCpId++ )
	{
		int cpId = itMapCpId->first;

		outFile << "\n\n-----------------------------------------------------------------------";
		outFile << "\nCampus " << cpId;
		for( auto itDisc = itMapCpId->second.begin(); itDisc != itMapCpId->second.end(); itDisc++ )
		{
			Disciplina *disciplina = itDisc->first;

			outFile << "\nDisc " << disciplina->getId();

			for( auto itTurma = itDisc->second.begin(); itTurma != itDisc->second.end(); itTurma++ )
			{
				int turma = itTurma->first;
				
				outFile << "\n\tTurma " << turma;

				for( auto itPV = itTurma->second.begin(); itPV != itTurma->second.end(); itPV++ )
				{
					outFile << "\n\t\tProf " << itPV->first->getId();

					for( auto itDiaAula = itPV->second.begin(); itDiaAula != itPV->second.end(); itDiaAula++ )
					{
						outFile << "\n\t\t\tDia " << itDiaAula->first;
						outFile << ", Aula " << itDiaAula->second->toString();
					}
				}
			}
		}
	}
		
	outFile.close();
}
 
void ProblemSolution::imprimeMapSolProfRealDiaHorarios()
{
	if (!CentroDados::getPrintLogs())
	{
		return;
	}

	ofstream outFile;
	string fileName("solProfRealDiaHorariosAlocados.txt");
	outFile.open( fileName, ofstream::out );

	if ( !outFile )
	{
		std::cout << "\nErro em void ProblemSolution::imprimeMapSolProfRealDiaHorarios():"
			<< " o arquivo " << fileName << " nao pode ser aberto.";
		return;
	}

	for( auto itMapProfReal = mapSolProfRealDiaHorarios.begin(); itMapProfReal != mapSolProfRealDiaHorarios.end(); itMapProfReal++ )
	{
		Professor* professor = itMapProfReal->first;

		outFile << "\n\n-----------------------------------------------------------------------";
		outFile << "\nProfessor " << professor->getId() << "\t" << professor->getNome();
		for( auto itMapDia = itMapProfReal->second.begin(); itMapDia != itMapProfReal->second.end(); itMapDia++ )
		{
			int dia = itMapDia->first;
			vector<HorarioDia*> *horsDia = & itMapDia->second;
			int size = horsDia->size();

			outFile << "\n\tDia " << dia << ": ";
			for( int idx = 0; idx != size; idx++ )
			{
				HorarioDia* hd = (*horsDia)[idx];
				DateTime dti = hd->getHorarioAula()->getInicio();
				DateTime dtf = hd->getHorarioAula()->getFinal();
				
				outFile << "  " << dti.getHour() << ":" << dti.getMinute()
						<< " - " << dtf.getHour() << ":" << dtf.getMinute() << " || ";
			}
		}
	}
	
	outFile.close();
}

void ProblemSolution::imprimeQuantChProfs()
{
	if (!CentroDados::getPrintLogs())
	{
		return;
	}

	ofstream outFile;
	string fileName("solQuantChProfs.txt");
	outFile.open( fileName, ofstream::out );

	if ( !outFile )
	{
		std::cout << "\nErro em void ProblemSolution::imprimeQuantChProfs():"
			<< " o arquivo " << fileName << " nao pode ser aberto.";
		return;
	}

	outFile << "\n-----------------------------------------------------------------\n";
	outFile << "Prof \t Qtd total de creditos alocados na semana\n";
	for ( auto itProf = quantChProfs.begin(); itProf != quantChProfs.end(); itProf++ )
	{
		outFile << "\n" << itProf->first << "\t\t\t" << itProf->second;
	}
		
	outFile.close();
}

void ProblemSolution::imprimeNrDiscSimultVirtual()
{	
	if (!CentroDados::getPrintLogs())
		return;

	ofstream outFile;
	string fileName("nrDiscSimultVirtual.txt");
	outFile.open( fileName, ofstream::out );
	if ( !outFile )
	{
		std::cout << "\nErro em void ProblemSolution::imprimeNrDiscSimultVirtual():"
			<< " o arquivo " << fileName << " nao pode ser aberto.";
		return;
	}
	
	for (auto itNrSimult=mapNrDiscSimult.cbegin(); itNrSimult!=mapNrDiscSimult.cend(); itNrSimult++)
	{
		int qtd = itNrSimult->first;
		outFile << "\nQtd de aulas simultaneas: " << qtd << "\n\tDisciplinas: ";
		for (auto itDisc = itNrSimult->second.cbegin(); itDisc != itNrSimult->second.cend(); itDisc++)
		{			
			outFile << " " << (*itDisc)->getId();
		}
	}
	outFile.close();
}

// ler output de um arquivo
ProblemSolution* ProblemSolution::lerSolucao(char* const filePath, bool modoTatico)
{
	std::cout << ">>>> Ler solucao de arquivo:" << endl;

	// abrir arquivo
	std::ifstream file;
	file.open(filePath);
	if(!file.is_open())
	{
		cout << filePath << endl;
		cout << "[WARN] Arquivo nao foi aberto!" << endl;
		return nullptr;
	}

	// extrair
	istream& fileStream = file;
	ProblemSolution* const probSolution = new ProblemSolution(modoTatico);
	fileStream >> probSolution;
	file.close();

	ProblemSolution* ps = nullptr;

	// libera memoria se nao foi encontrada solucao inicial
	if (probSolution->atendimento_campus->size()==0)
	{
		delete probSolution;
	}
	else
	{
		ps = probSolution;
	}

	std::cout << "solucao carregada!" << endl;

	return ps;
}

std::ostream & operator << (
   std::ostream & out, ProblemSolution & solution )
{
   // TATICO
   if ( solution.modoOtmTatico )
   {
      out << "<?xml version=\"1.0\" encoding=\"ISO-8859-1\" standalone=\"yes\"?>"
          << std::endl;
	  // out << "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>"
       //   << std::endl;

      out << "<TriedaOutput>" << std::endl;
	  
     // out << "<cenarioId>" << solution.getCenarioId() << "</cenarioId>";

      //-----------------------------------------------------------------------
      out << "<atendimentos>" << std::endl;
      GGroup< AtendimentoCampus * >::GGroupIterator it_campus
         = solution.atendimento_campus->begin();

      for (; it_campus != solution.atendimento_campus->end();
             it_campus++ )
      {
         out << ( **it_campus );
      }

      out << "</atendimentos>" << std::endl;
      //-----------------------------------------------------------------------

	  if ( solution.nao_atendimentos != NULL )
      {
		 out << "<NaoAtendimentosTatico>" << std::endl;
         GGroup< NaoAtendimento *, LessPtr< NaoAtendimento > >::GGroupIterator it_aluno_demanda
            = solution.nao_atendimentos->begin();

         for ( ; it_aluno_demanda != solution.nao_atendimentos->end(); it_aluno_demanda++ )
         {
            out << ( **it_aluno_demanda );
         }
		 out << "</NaoAtendimentosTatico>" << std::endl;
	  }
	  else out << "<NaoAtendimentosTatico/>" << std::endl;

	  

      //-----------------------------------------------------------------------
      // Erros e warnings:
      out << ( *ErrorHandler::getInstance() );
      //-----------------------------------------------------------------------

      //-----------------------------------------------------------------------
      // Folgas:
      out << "<restricoesVioladas>\n";
      RestricaoVioladaGroup::iterator it
         = solution.getFolgas()->begin();

      for (; it != solution.getFolgas()->end(); ++it )
      {
         out << ( **it );
      }

      out << "</restricoesVioladas>" << std::endl;
      //-----------------------------------------------------------------------

      out << "<professoresVirtuais/>" << std::endl;

      //-----------------------------------------------------------------------
	  /*
      if ( solution.alunosDemanda != NULL )
      {
         out << "<alunosDemanda>" << std::endl;

         GGroup< AlunoDemanda *, LessPtr< AlunoDemanda > >::GGroupIterator it_aluno_demanda
            = solution.alunosDemanda->begin();

         for (; it_aluno_demanda != solution.alunosDemanda->end();
                it_aluno_demanda++ )
         {
            out << ( **it_aluno_demanda );
         }

         out << "</alunosDemanda>" << std::endl;
      }
      else
      {
         out << "<alunosDemanda/>" << std::endl;
      }
	  */
      //-----------------------------------------------------------------------

      out << "</TriedaOutput>" << std::endl;
   }
   // OPERACIONAL
   else
   {
      out << "<?xml version=\"1.0\" encoding=\"ISO-8859-1\" standalone=\"yes\"?>"
          << std::endl;
	  
	  //out << "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>"
		//    << std::endl;

      out << "<TriedaOutput>" << std::endl;
	  
   //   out << "<cenarioId>" << solution.getCenarioId() << "</cenarioId>";

      //-----------------------------------------------------------------------
      out << "<atendimentos>" << std::endl;
      if ( solution.atendimento_campus != NULL )
      {
         GGroup< AtendimentoCampus * >::GGroupIterator it_campus
            = solution.atendimento_campus->begin();

         for (; it_campus != solution.atendimento_campus->end();
                it_campus++ )
         {
            out << ( **it_campus );
         }

         out << "</atendimentos>" << std::endl;
      }
      //-----------------------------------------------------------------------
	  	
	  if ( solution.nao_atendimentos != NULL )
	  {
		  out << "<NaoAtendimentosTatico>" << std::endl;

		  GGroup< NaoAtendimento *, LessPtr< NaoAtendimento > >::GGroupIterator it_aluno_demanda
			= solution.nao_atendimentos->begin();

		  for ( ; it_aluno_demanda != solution.nao_atendimentos->end(); it_aluno_demanda++ )
		  {
			  out << ( **it_aluno_demanda );
		  }

		  out << "</NaoAtendimentosTatico>" << std::endl;
	  }
	  else out << "<NaoAtendimentosTatico/>" << std::endl;


      //-----------------------------------------------------------------------
      // Erros e warnings:
      out << ( *ErrorHandler::getInstance() );
      //-----------------------------------------------------------------------

      //-----------------------------------------------------------------------
      // Folgas:
      out << "<restricoesVioladas>\n";
      RestricaoVioladaGroup::iterator it
         = solution.getFolgas()->begin();

      for (; it != solution.getFolgas()->end(); ++it )
      {
         out << ( **it );
      }

      out << "</restricoesVioladas>" << std::endl;
      //-----------------------------------------------------------------------

      //-----------------------------------------------------------------------
      // Código relacionado à issue TRIEDA-833 e TRIEDA-883
      out << "<professoresVirtuais>" << std::endl;
      if ( solution.professores_virtuais != NULL )
      {
         GGroup< ProfessorVirtualOutput * >::GGroupIterator it_professor_virtual
            = solution.professores_virtuais->begin();

         for (; it_professor_virtual != solution.professores_virtuais->end();
                it_professor_virtual++ )
         {
            out << ( **it_professor_virtual );
         }

         out << "</professoresVirtuais>" << std::endl;
      }
      //-----------------------------------------------------------------------

      //-----------------------------------------------------------------------
      if ( solution.alunosDemanda != NULL )
      {
         out << "<alunosDemanda>" << std::endl;

         GGroup< AlunoDemanda *, LessPtr< AlunoDemanda > >::GGroupIterator it_aluno_demanda
            = solution.alunosDemanda->begin();

         for (; it_aluno_demanda != solution.alunosDemanda->end();
                it_aluno_demanda++ )
         {
            out << ( **it_aluno_demanda );
         }

         out << "</alunosDemanda>" << std::endl;
      }
      else
      {
         out << "<alunosDemanda/>" << std::endl;
      }
      //-----------------------------------------------------------------------

      out << "</TriedaOutput>" << std::endl;
   }

   return out;
}

std::istream& operator >> ( std::istream &file, ProblemSolution* const &ptrProbSolution)
{
	std::string line;
	while(!getline( file, line ).eof())
	{
		// ATENDIMENTOS CAMPUS
		// ---------------------------------------------------------- 
		if(line.find("<AtendimentoCampus>") != string::npos)
		{
			AtendimentoCampus* const atendCampus = new AtendimentoCampus();
			file >> atendCampus;
			if(atendCampus->getId() == InputMethods::fakeId)
				throw "[EXC: ProblemSolution::operator>>] AtendimentoCampus* e nullptr!";
			ptrProbSolution->atendimento_campus->add(atendCampus);
		}

		// PROFESSORES VIRTUAIS
		// ---------------------------------------------------------- 
		
		// NÃO IMPLEMENTADO ENQUANTO NÃO CONSEGUIR LIGAR PROF A HABILITAÇÕES (por exemplo por curso ID)
	}
	return file;
}