#include "TaticoIntAlunoHor.h"
#include <math.h>

#include "CentroDados.h"

using namespace std;


//#ifdef TATICO_SO_HEURN
//	bool SO_HEURN=true;
//#else
//	bool SO_HEURN=false;
//#endif
//
    
int TaticoIntAlunoHor::idCounter = 0;

TaticoIntAlunoHor::TaticoIntAlunoHor( ProblemData * aProblemData, 
				GGroup< VariableTatico *, LessPtr<VariableTatico> > *aSolVarsTatico, 
				GGroup< VariableTatico *, LessPtr<VariableTatico> > *avars_xh, 
				bool *endCARREGA_SOLUCAO, bool equiv, int fase )
				: Solver( aProblemData )
{
   TaticoIntAlunoHor::idCounter++;

   solVarsTatico = aSolVarsTatico;
   vars_xh = avars_xh;

   CARREGA_SOLUCAO = endCARREGA_SOLUCAO;

   USAR_EQUIVALENCIA = equiv && problemData->parametros->considerar_equivalencia_por_aluno;
   NAO_CRIAR_RESTRICOES_CJT_ANTERIORES = true;
   FIXAR_P1 = true;
   FIXAR_TATICO_P1 = true;
   PERMITIR_NOVAS_TURMAS = ( fase == 1 || fase == 2 || fase == 3 ? true : false );
   
   CRIANDO_V_ATRAVES_DE_X = true;

   if (PERMITIR_NOVAS_TURMAS && !equiv) CRIAR_VARS_FIXADAS = false;
   else CRIAR_VARS_FIXADAS = true;

   PERMITIR_REALOCAR_ALUNO = false;

   mapPermitirAbertura.clear();
   mapDiscNroFolgasDemandas.clear();

   etapa = fase+1;

   ITERACAO = fase;
   
   BRANCH_SALA = ( fase == 4? true : false );

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

TaticoIntAlunoHor::~TaticoIntAlunoHor()
{
   if ( lp != NULL )
   {
      delete lp;
   }   
}

void TaticoIntAlunoHor::getSolution( ProblemSolution * problem_solution ){}

int TaticoIntAlunoHor::solve(){return 1;}

void TaticoIntAlunoHor::solveTaticoIntegrado( int campusId, int prioridade, int r )
{
    PERMITIR_REALOCAR_ALUNO = CRIAR_VARS_FIXADAS && !PERMITIR_NOVAS_TURMAS && prioridade==1;

	if ( USAR_EQUIVALENCIA )
	{
		std::cout<<"\n------------------------ Equiv --------------------------\n";
    } 
	if ( PERMITIR_NOVAS_TURMAS )
	{
		std::cout<<"\n------------------------ Permite Novas Turmas --- " << this->ITERACAO << " --------------------------\n";
    } 
	std::cout<<"\n----------------------------------Rodada "<< r <<" ------------------------------------\n";
	std::cout<<"\n--------------------- Campus "<< campusId << ", Prior " << prioridade << "---------------------\n";
	std::cout<<"\n------------------------------Tatico Integrado -----------------------------\n"; fflush(NULL);
	
	std::cout<<"\nPERMITIR_REALOCAR_ALUNO = " << PERMITIR_REALOCAR_ALUNO;
	std::cout<<"\nCRIAR_VARS_FIXADAS = " << CRIAR_VARS_FIXADAS;

	std::cout<<"\n\nIniciando tatico integrado...\n";
	
	std::cout<<"\nCalculando nro de folgas...\n"; fflush(NULL);
	calculaNroFolgas( prioridade, campusId );
		
	bool permiteAumentarNroTurmas = ( etapa >= Etapa2 && this->PERMITIR_NOVAS_TURMAS );

	#ifdef UNIT
	//	permiteAumentarNroTurmas = ( etapa >= Etapa3 && this->PERMITIR_NOVAS_TURMAS );
	#endif

	if ( permiteAumentarNroTurmas )
	{
		corrigeNroTurmas( prioridade, campusId );
	}
	
	std::cout<<"\nSolving...\n"; fflush(NULL);
	//solveTaticoIntAlunoHor( campusId, prioridade, r );
	solveTaticoIntAlunoHor_EtapaSimples( campusId, prioridade, r );

	std::cout<<"\nCarregando solucao tatica integrada...\n"; fflush(NULL);
	carregaVariaveisSolucaoTaticoPorAlunoHor( campusId, prioridade, r );
		
	std::cout<<"\nPreenchendo map de turnos e horarios comuns...\n"; fflush(NULL);
	problemData->preencheMapsTurmasTurnosIES();

	std::stringstream stepName;
	stepName << "TAT_INT_" << TaticoIntAlunoHor::idCounter;

	std::cout<<"\nImprimindo alocacoes...\n"; fflush(NULL);
	problemData->imprimeAlocacaoAlunos( campusId, prioridade, 0, false, r, stepName.str(), this->getRunTime() );
	std::cout<<"\nImprime utilizacao das salas...\n"; fflush(NULL);
	problemData->imprimeUtilizacaoSala( campusId, prioridade, 0, false, r, stepName.str() );

	if (this->USAR_EQUIVALENCIA)
	{
		std::cout<<"\nAtualizando demandas substitutas por equivalencia...\n"; fflush(NULL);
		atualizarDemandasEquiv( campusId, prioridade );		
	}

	std::cout<<"\nImprimeDiscNTurmas...\n"; fflush(NULL);
	problemData->imprimeDiscNTurmas( campusId, prioridade, 0, false, r, TaticoIntAlunoHor::idCounter );		
	
	std::cout<<"\nSincronizando as solucoes...\n"; fflush(NULL);
	sincronizaSolucao( campusId, prioridade, r );
	
	std::cout<<"\nImprimindo grades de alunos...\n";
	imprimeGradeHorAlunos( campusId, prioridade );
	
	std::cout<<"\nImprimindo grades de alunos por demanda...\n";
	imprimeGradeHorAlunosPorDemanda( campusId, prioridade );	
	
	std::cout<<"\nConfere corretude...\n";
	confereCorretude( campusId, prioridade );

	std::cout<<"\nCalculando nro de folgas...\n"; fflush(NULL);
	calculaNroFolgas( prioridade, campusId );
	
	bool violou = problemData->violaMinTurma( campusId );

	std::cout<<"\nFim do tatico integrado!\n"; fflush(NULL);

	return; 
}

void TaticoIntAlunoHor::chgCoeffList(
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

/*
	Verifica se seria possivel inserir o 'aluno' nas aulas em 'aulasX' (variaveis do tipo crédito, x).
	Para isso olha sobreposição de horários do aluno em todas as aulas, e capacidade das salas.
*/
bool TaticoIntAlunoHor::violaInsercao( Aluno* aluno, GGroup< VariableTatInt *, LessPtr<VariableTatInt> > aulasX )
{
	bool viola=false;
	
	// verifica todas as aulas antes de inserir
	ITERA_GGROUP_LESSPTR( itAula, aulasX, VariableTatInt )
	{
		if ( itAula->getType() != VariableTatInt::V_CREDITOS )
		{
			std::cout<<"\nErro: só deveria haver variaveis do tipo x aqui.\n";
			continue;
		}

		HorarioAula *hi = itAula->getHorarioAulaInicial();
		HorarioAula *hf = itAula->getHorarioAulaFinal();
		int dia = itAula->getDia();				
		Sala* sala = itAula->getSubCjtSala()->salas.begin()->second;

		int nroAlunos = problemData->existeTurmaDiscCampus( itAula->getTurma(), itAula->getDisciplina()->getId(), itAula->getUnidade()->getIdCampus() );

		int capacidade = sala->getCapacidade();
		if ( nroAlunos + 1 > capacidade )
		{
			viola=true;
			break;
		}
		if ( aluno->sobrepoeAulaJaAlocada( hi, hf, dia ) )
		{
			viola=true;
			break;
		}
	}

	return viola;
}

void TaticoIntAlunoHor::imprimeGradeHorAlunos( int campusId, int prioridade )
{
	stringstream fileName;
	fileName << "gradeAlunos" << TaticoIntAlunoHor::idCounter;
	fileName << "_" << problemData->getInputFileName();
	fileName << "_id" << problemData->inputIdToString() << ".txt";

	ofstream outGradeAlunos(fileName.str(), ios::out);	
	if ( !outGradeAlunos ) 
	{
		std::cout << "\nErro!! Arquivo nao pode ser aberto " << fileName.str();
		return;
	}

	outGradeAlunos << "CampusId " << campusId << "  -  Prioridade " << prioridade << endl;
	ITERA_GGROUP_LESSPTR( itAluno, problemData->alunos, Aluno )
	{
		GGroup<TurnoIES*,LessPtr<TurnoIES>> turnos;
		ITERA_GGROUP_LESSPTR( itAlDem, itAluno->demandas, AlunoDemanda )
		{
			turnos.add( itAlDem->demanda->getTurnoIES() );
		}

		outGradeAlunos << "\n\nAluno " << itAluno->getNomeAluno() << "  id " << itAluno->getAlunoId();
		outGradeAlunos << "\nTurnos associados: ";
		ITERA_GGROUP_LESSPTR( itTurnoIES, turnos, TurnoIES )
		{
			outGradeAlunos << itTurnoIES->getNome() << "; ";
		}

		for ( int dia=2; dia <= 7; dia++ )
		{
			std::string horsOcupadosAluno = itAluno->getHorariosOcupadosStr( dia );
			outGradeAlunos << "\nDia " << dia << ": " << horsOcupadosAluno;
		}
	}
	outGradeAlunos.close();
}

void TaticoIntAlunoHor::imprimeGradeHorAlunosPorDemanda( int campusId, int prioridade )
{
	stringstream fileName;
	fileName << "gradeAlunosPorDemanda" << TaticoIntAlunoHor::idCounter;
	fileName << "_" << problemData->getInputFileName();
	fileName << "_id" << problemData->inputIdToString() << ".txt";

	ofstream outGradeAlunos(fileName.str(), ios::out);	
	if ( !outGradeAlunos ) 
	{
		std::cout << "\nErro!! Arquivo nao pode ser aberto " << fileName.str();
		return;
	}

	outGradeAlunos << "CampusId " << campusId << "  -  Prioridade " << prioridade << endl;

	std::map< int /*discId*/, GGroup<AlunoDemanda*,LessPtr<AlunoDemanda>> >::iterator
		itMapDisc = this->mapDiscAlunosDemanda.begin();
	for ( ; itMapDisc != this->mapDiscAlunosDemanda.end(); itMapDisc++ )
	{
		Disciplina *disciplina = problemData->retornaDisciplina( itMapDisc->first );
		outGradeAlunos << "\n\n-----------------------------------------------------------------\n";
		outGradeAlunos << "Disciplina " << disciplina->getCodigo() << "  - id " << disciplina->getId();

		std::map<TurnoIES*, GGroup<AlunoDemanda*, LessPtr<AlunoDemanda>>> mapTurnoAlDem;
		ITERA_GGROUP_LESSPTR( itAlunoDemanda, itMapDisc->second, AlunoDemanda )
		{
			mapTurnoAlDem[ itAlunoDemanda->demanda->getTurnoIES() ].add( *itAlunoDemanda );
		}

		std::map<TurnoIES*, GGroup<AlunoDemanda*, LessPtr<AlunoDemanda>>>::iterator
			itMapTurno = mapTurnoAlDem.begin();
		for ( ; itMapTurno != mapTurnoAlDem.end(); itMapTurno++ )
		{
			ITERA_GGROUP_LESSPTR( itAlunoDemanda, itMapTurno->second, AlunoDemanda )
			{
				Aluno *aluno = itAlunoDemanda->getAluno();

				outGradeAlunos << "\n\nAluno " << aluno->getNomeAluno() 
					<< " - id " << aluno->getAlunoId()
					<< " - " << itAlunoDemanda->demanda->getTurnoIES()->getNome();

				for ( int dia=2; dia <= 7; dia++ )
				{
					std::string horsOcupadosAluno = aluno->getHorariosOcupadosStr( dia );
					outGradeAlunos << "\nDia " << dia << ": " << horsOcupadosAluno;
				}
			}
		}
	}
	outGradeAlunos.close();
}

void TaticoIntAlunoHor::confereCorretude( int campusId, int prioridade )
{
	// --------------------------------------------------------
	// Confere corretude de alocacoes de alunos em disciplinas
	problemData->confereCorretudeAlocacoes();


	// --------------------------------------------------------
	// Maps da solução
	std::map< Sala*, std::map< int /*dia*/, std::map<DateTime /*dti*/, std::pair<DateTime /*dtf*/, VariableTatico*> > >,
		LessPtr<Sala> > mapSalaDiaDtiDtfAula;
	std::map< Disciplina*, std::map< int /*turma*/, std::map<int /*dia*/, VariableTatico*> >, LessPtr<Disciplina> > mapDiscTurmaDiaAula;
	
	// --------------------------------------------------------
	// Iteradores dos maps
	std::map< Sala*, std::map< int /*dia*/, std::map<DateTime /*dti*/, std::pair<DateTime /*dtf*/, VariableTatico*> > >,
		LessPtr<Sala> >::iterator itMapSala;
	std::map< int /*dia*/, std::map<DateTime /*dti*/, std::pair<DateTime /*dtf*/, VariableTatico*> > >::iterator itMapDia;
	std::map<DateTime /*dti*/, std::pair<DateTime /*dtf*/, VariableTatico*> >::iterator itMapDti;

	std::map< Aluno*, std::map< int /*dia*/, std::map<DateTime /*dti*/, std::pair<DateTime /*dtf*/, VariableTatico*> > >,
		LessPtr<Aluno> >::iterator itMapAluno;
	std::map< Disciplina*, std::map< int /*turma*/, std::map<int /*dia*/, VariableTatico*> >, LessPtr<Disciplina> >::iterator itMapDisc;
	std::map< int /*turma*/, std::map<int /*dia*/, VariableTatico*> >::iterator itTurma;
	std::map<int /*dia*/, VariableTatico* >::iterator itDiaX;

	// --------------------------------------------------------
	// Maperando as turmas da solução

	ITERA_GGROUP_LESSPTR( itX, (*vars_xh), VariableTatico )
	{
		VariableTatico *x = *itX;
		Sala *sala = x->getSubCjtSala()->getSala();
		int dia = x->getDia();
		DateTime dti = x->getHorarioAulaInicial()->getInicio();
		DateTime dtf = x->getHorarioAulaFinal()->getFinal();

		if ( sala->getIdCampus() != campusId ) continue;

		// --------------------------------------------------------

		itMapSala = mapSalaDiaDtiDtfAula.find(sala);
		if ( itMapSala != mapSalaDiaDtiDtfAula.end() )
		{
			itMapDia = itMapSala->second.find(dia);
			if ( itMapDia != itMapSala->second.end() )
			{
				bool ok=true;

				itMapDti = itMapDia->second.begin();
				for ( ; itMapDti != itMapDia->second.end() && ok; itMapDti++ )
				{
					DateTime salaDti = itMapDti->first;

					if ( salaDti >= dtf ) break;

					DateTime salaDtf = itMapDti->second.first;
					
					if ( salaDtf > dti ) 
					{
						ok = false;
						stringstream msg;
						msg << "Conflito de horario na sala [id " << sala->getId() << ", codigo " << sala->getCodigo()
							<< "] no dia " << dia << " horario " << dti;
						CentroDados::printError( "void TaticoIntAlunoHor::confereCorretude( int campusId, int prioridade )", msg.str() );
					}
				}

				if ( ok ) 
				{
					(itMapDia->second)[dti] = std::make_pair(dtf, x);
				}
			}
			else (itMapSala->second)[dia][dti] = std::make_pair(dtf, x);
		}
		else mapSalaDiaDtiDtfAula[sala][dia][dti] = std::make_pair(dtf, x);

		// --------------------------------------------------------
		
		Disciplina *disciplina = x->getDisciplina();
		int turma = x->getTurma();
		
		itMapDisc = mapDiscTurmaDiaAula.find(disciplina);
		if ( itMapDisc != mapDiscTurmaDiaAula.end() )
		{
			itTurma = itMapDisc->second.find(turma);
			if ( itTurma != itMapDisc->second.end() )
			{
				itDiaX = itTurma->second.find(dia);
				if ( itDiaX == itTurma->second.end() )
				{
					(itTurma->second)[dia] = x;
				}
				else
				{
					stringstream msg;
					msg << "Mais de uma aula no dia " << dia << " para a turma "
						<< turma << " da disciplina " << disciplina->getId() << ". Variaveis: " << x->toString() << " e "
						<< itDiaX->second->toString();
					CentroDados::printError( "void TaticoIntAlunoHor::confereCorretude( int campusId, int prioridade )", msg.str() );
				}
			}
			else (itMapDisc->second)[turma][dia] = x;
		}
		else mapDiscTurmaDiaAula[disciplina][turma][dia] = x;

		// --------------------------------------------------------
	}	


	// --------------------------------------------------------
	// Mapeando alunos

	std::map< Aluno*, GGroup< Trio< int /*campusId*/, int /*turma*/, Disciplina* > >, LessPtr< Aluno > >::iterator
		itMapAlunoAlocacoes = problemData->mapAluno_CampusTurmaDisc.begin();
	for ( ;  itMapAlunoAlocacoes != problemData->mapAluno_CampusTurmaDisc.end(); itMapAlunoAlocacoes++ )
	{
		Aluno *aluno = itMapAlunoAlocacoes->first;
		
		std::map< int /*dia*/, std::map<DateTime /*dti*/, std::pair<DateTime /*dtf*/, VariableTatico*> > > mapAlunoDiaDtiDtfAula;

		GGroup< Trio< int /*campusId*/, int /*turma*/, Disciplina* > > *trios = &itMapAlunoAlocacoes->second;
		GGroup< Trio< int /*campusId*/, int /*turma*/, Disciplina* > >::iterator itTrio = trios->begin();
		for ( ; itTrio != trios->end(); itTrio++ )
		{
			Trio< int /*campusId*/, int /*turma*/, Disciplina* > trio = *itTrio;
			int cpId = trio.first;
			int turma = trio.second;
			Disciplina* disciplina = trio.third;

			if ( cpId != campusId ) continue;

			std::map<int /*dia*/, VariableTatico*> *mapDiaVar=nullptr;

			// Aulas da turma
			itMapDisc = mapDiscTurmaDiaAula.find(disciplina);
			if ( itMapDisc != mapDiscTurmaDiaAula.end() )
			{
				itTurma = itMapDisc->second.find(turma);
				if ( itTurma != itMapDisc->second.end() )
				{
					mapDiaVar = & itTurma->second;
				}
			}
			if ( mapDiaVar == nullptr )
			{
				stringstream msg;
				msg << "Aluno [id " << aluno->getAlunoId() << ", " << aluno->getNomeAluno()
					<< "] alocado em turma " << turma << " da disciplina " << disciplina->getId()
					<< " mas nao existe variavel de aula associada.";
				CentroDados::printError( "void TaticoIntAlunoHor::confereCorretude( int campusId, int prioridade )", msg.str() );
				continue;
			}

			// mapeia as aulas da turma no map do aluno
			itDiaX = mapDiaVar->begin();
			for ( ; itDiaX != mapDiaVar->end(); itDiaX++ )
			{
				VariableTatico *x = itDiaX->second;
				int dia = x->getDia();
				DateTime dti = x->getHorarioAulaInicial()->getInicio();
				DateTime dtf = x->getHorarioAulaFinal()->getFinal();

				itMapDia = mapAlunoDiaDtiDtfAula.find(dia);
				if ( itMapDia != mapAlunoDiaDtiDtfAula.end() )
				{
					bool ok=true;

					itMapDti = itMapDia->second.begin();
					for ( ; itMapDti != itMapDia->second.end() && ok; itMapDti++ )
					{
						DateTime alunoDti = itMapDti->first;

						if ( alunoDti >= dtf ) break;

						DateTime alunoDtf = itMapDti->second.first;
					
						if ( alunoDtf > dti ) 
						{
							ok = false;
							stringstream msg;
							msg << "Conflito de horario do aluno [id " << aluno->getAlunoId() << ", " << aluno->getNomeAluno()
								<< "] no dia " << dia << " horario " << dti;
							CentroDados::printError( "void TaticoIntAlunoHor::confereCorretude( int campusId, int prioridade )", msg.str() );
						}
					}

					if ( ok )
					{
						(itMapDia->second)[dti] = std::make_pair(dtf, x);
					}
				}
				else mapAlunoDiaDtiDtfAula[dia][dti] = std::make_pair(dtf, x);
			
			}
		}
	}

}

void TaticoIntAlunoHor::corrigeNroTurmas( int prioridade, int campusId )
{	
	std::cout<<"\nCorrige nro de turmas...\n"; fflush(NULL);

	char fileName[1024]="\0";
	strcpy( fileName, getCorrigeNrTurmasFileName( campusId, prioridade, 1 ).c_str() );	
	
	ofstream aumentafile;
	aumentafile.open( fileName, ios::out );
	if ( !aumentafile )
	{
		std::cout << "\nErro em corrigeNroTurmas(int campusAtualId, int prioridade, int r): arquivo "
					<< fileName << " nao encontrado.\n";
		return;
	}

	std::map<int /*discId*/, std::pair<int/*difTurmas*/, int/*nroNaoAtend*/> >::iterator itMap = problemData->mapDiscDif.begin();
		
	int capMediaSalas = problemData->cp_medSalas[campusId]/2;
	
	aumentafile << "Capacidade media das salas calculada: "<<capMediaSalas;

	std::map<int, std::map< Disciplina*, int /*acrescimo de turmas*/, LessPtr<Disciplina> > > mapDiscP;
	std::map<int, std::map <Disciplina*, int /*acrescimo de turmas*/, LessPtr<Disciplina> > > mapDiscT;

	for( ; itMap != problemData->mapDiscDif.end(); itMap++ )
	{
		int disciplinaId = itMap->first;
		std::pair<int/*difTurmas*/, int/*nroNaoAtend*/> par = itMap->second;
		int difTurmas = par.first;
		int nroNaoAtend = par.second;

		if ( nroNaoAtend <=0 ) continue;
				
		Disciplina *disciplina = problemData->refDisciplinas[disciplinaId];
		const int nroAtualTurmas = disciplina->getNumTurmas();
		double acrescimo=0;

		if ( difTurmas == 0 )
		{
			if ( capMediaSalas > 0 )
				acrescimo = (int)( nroNaoAtend/capMediaSalas ) + 1;

			if ( ITERACAO > 2 ) acrescimo+=1;
		}
		else if ( difTurmas > 0 )
		{
			bool remanescente = false;
			int excesso = 0;
			int excessoRemanescente = 0;
			for ( int i = nroAtualTurmas; i >= 1; i-- )
			{		
				int turmaExiste = problemData->existeTurmaDiscCampus( i, disciplina->getId(), campusId );
				if ( !turmaExiste && !remanescente )
				{
					excesso++;
				}
				if ( turmaExiste )
				{
					remanescente = true;
				}
				if ( !turmaExiste && remanescente )
				{
					excessoRemanescente++;
				}
			}
			
			acrescimo = acrescimo - excesso;

			int incremento = 0;
			if ( capMediaSalas > 0 ){
				incremento = (int)( nroNaoAtend/capMediaSalas );
				incremento = ( incremento > 0? incremento : 1 );
			}

			incremento = ( incremento - excessoRemanescente > 0 )? (incremento - excessoRemanescente) : 0;

			acrescimo = acrescimo + incremento;

		}

		GGroup<Calendario*, LessPtr<Calendario>> semanasLetivas;

		GGroup<AlunoDemanda*, LessPtr<AlunoDemanda>> alDem = 
			problemData->retornaDemandasDiscNoCampus( disciplinaId, campusId, prioridade );
		ITERA_GGROUP_LESSPTR( itAlDem, alDem, AlunoDemanda )
		{
			int id = (*itAlDem)->getAlunoId();
			Aluno *aluno = problemData->retornaAluno( id );
			if ( problemData->retornaTurmaDiscAluno( aluno, disciplina ) == -1 )
			{
				semanasLetivas.add( (*itAlDem)->demanda->getCalendario() );
			}
		}
				
		aumentafile << "\n\nDisciplina "<<disciplina->getId() << ": aumento de "<<disciplina->getNumTurmas()
			<<" turmas para "<< nroAtualTurmas + acrescimo << " turmas."
			<< "\nFolga de " << nroNaoAtend << " atendimentos com semanas letivas ids";
		ITERA_GGROUP_LESSPTR( it, semanasLetivas, Calendario )
			aumentafile << " " << (*it)->getId();

		if ( disciplina->temParPratTeor() )
		{
			if ( disciplina->getId() < 0 )
				mapDiscP[disciplina->getId()][disciplina] = acrescimo;
			else
				mapDiscT[disciplina->getId()][disciplina] = acrescimo;
		}

		disciplina->setNumTurmas( nroAtualTurmas + acrescimo );
	}

	aumentafile << "\n\n--------------\n";

	if ( problemData->parametros->discPratTeor1xN )
	{
		std::map<int, std::map< Disciplina*, int /*acrescimo de turmas*/, LessPtr<Disciplina> > >::iterator
			itMapP = mapDiscP.begin();
		for ( ; itMapP != mapDiscP.end(); itMapP++ )
		{
			int discId = itMapP->first;

			std::map< Disciplina*, int /*acrescimo de turmas*/, LessPtr<Disciplina> >::iterator
				itDiscP = itMapP->second.begin();
			for ( ; itDiscP != itMapP->second.end(); itDiscP++ )
			{
				Disciplina* discP = itDiscP->first;
				int acrescimoP = itDiscP->second;
			
				Disciplina* discT=nullptr;			
				int acrescimoT;

				auto itDiscT = mapDiscT.find( -discId );
				if ( itDiscT == mapDiscT.end() )
					continue;

				discT = itDiscT->second.begin()->first;
				acrescimoT = itDiscT->second.begin()->second;

				if ( acrescimoT > acrescimoP )
				{
					// corrige: acrescimoT <- acrescimoP
					discT->setNumTurmas( discT->getNumTurmas() - acrescimoT + acrescimoP );
					acrescimoT = acrescimoP;
				}

				int nroPratPorTeor = 0;
				if ( acrescimoT > 0 ) nroPratPorTeor = acrescimoP / acrescimoT;
			
				int turmaNovaP = discP->getNumTurmas() - acrescimoP + 1;
				int turmaNovaT = discT->getNumTurmas() - acrescimoT + 1;
			
				for ( int turmaT = turmaNovaT; turmaT <= discT->getNumTurmas(); turmaT++ )
				{
					for ( int turmaP = turmaNovaP; turmaP < turmaNovaP + nroPratPorTeor; turmaP++ )
					{
						if ( turmaP > discP->getNumTurmas() )
						{
							std::cout << "\nErro na logica de corrigir o nro de turmas PT"; break;
						}

						discT->setTurmaPratAssociada( turmaT, turmaP );
						discP->setTurmaTeorAssociada( turmaP, turmaT );

						aumentafile << "\nDisciplina "<<discP->getId() << ": turma pratica "<<turmaP
							<<" associada aa turma teorica "<< turmaT;						
					}
					turmaNovaP = turmaNovaP + nroPratPorTeor;
				}

				int dif = acrescimoP - nroPratPorTeor*acrescimoT;
				if ( dif > 0 )
				{
					// associa a "sobra" da divisao com a ultima turma teorica
					int turmaT = discT->getNumTurmas();
					for ( int turmaP = turmaNovaP; turmaP <= discP->getNumTurmas(); turmaP++ )
					{
						discT->setTurmaPratAssociada( turmaT, turmaP );
						discP->setTurmaTeorAssociada( turmaP, turmaT );

						aumentafile << "\nDisciplina "<<discP->getId() << ": turma pratica "<<turmaP
							<<" associada aa turma teorica "<< turmaT;
					}				
				}
			}
		}
	}

	aumentafile.close();
}

void TaticoIntAlunoHor::aumentaNroTurmas( int prioridade, int campusId )
{	
	std::cout<<"\nAumenta nro de turmas...\n"; fflush(NULL);

	char fileName[1024]="\0";
	strcpy( fileName, getAumentaTurmasFileName( campusId, prioridade, 1 ).c_str() );	
	
	ofstream aumentafile;
	aumentafile.open( fileName, ios::out );
	if ( !aumentafile )
	{
		std::cout << "\nErro em aumentaNroTurmas(int campusAtualId, int prioridade, int r): arquivo "
					<< fileName << " nao encontrado.\n";
		return;
	}

	std::map<int /*discId*/, std::pair<int/*difTurmas*/, int/*nroNaoAtend*/> >::iterator itMap = problemData->mapDiscDif.begin();
		
	int capMediaSalas = problemData->cp_medSalas[campusId]/3;;

//#ifndef UNIT
//	capMediaSalas = problemData->cp_medSalas[campusId]/4;
//#else
//	if ( ITERACAO > 1 )
//		capMediaSalas = problemData->cp_medSalas[campusId]/3;
//	else
//		capMediaSalas = problemData->cp_medSalas[campusId]/4;
//#endif

	aumentafile << "Capacidade media das salas calculada: "<<capMediaSalas;

	for( ; itMap != problemData->mapDiscDif.end(); itMap++ )
	{
		int disciplinaId = itMap->first;
		std::pair<int/*difTurmas*/, int/*nroNaoAtend*/> par = itMap->second;
		int difTurmas = par.first;
		int nroNaoAtend = par.second;

		if ( difTurmas !=0 || nroNaoAtend <=0 ) continue;
				
		Disciplina *disciplina = problemData->refDisciplinas[disciplinaId];
		int nroAtualTurmas = disciplina->getNumTurmas();
		double acrescimo=0;

		if ( capMediaSalas > 0 )
			acrescimo = (int)( nroNaoAtend/capMediaSalas ) + 1;

		if ( ITERACAO > 2 ) acrescimo+=3;

		GGroup<Calendario*, LessPtr<Calendario>> semanasLetivas;

		GGroup<AlunoDemanda*, LessPtr<AlunoDemanda>> alDem = 
			problemData->retornaDemandasDiscNoCampus( disciplinaId, campusId, prioridade );
		ITERA_GGROUP_LESSPTR( itAlDem, alDem, AlunoDemanda )
		{
			int id = (*itAlDem)->getAlunoId();
			Aluno *aluno = problemData->retornaAluno( id );
			if ( problemData->retornaTurmaDiscAluno( aluno, disciplina ) == -1 )
			{
				semanasLetivas.add( (*itAlDem)->demanda->getCalendario() );
				//semanasLetivas.add( aluno->getCalendario() );				
			}
		}

	//	if ( ITERACAO > 1 )
	//	{
	//		int nSL = (semanasLetivas.size() > 3)? 3:semanasLetivas.size();
	//		if (acrescimo < nSL)
	//			acrescimo = nSL;
	//	}
		
		aumentafile << "\n\nDisciplina "<<disciplina->getId() << ": aumento de "<<disciplina->getNumTurmas()
			<<" turmas para "<< nroAtualTurmas + acrescimo << " turmas."
			<< "\nFolga de " << nroNaoAtend << " atendimentos com semanas letivas ids";
		ITERA_GGROUP_LESSPTR( it, semanasLetivas, Calendario )
			aumentafile << " " << (*it)->getId();

		disciplina->setNumTurmas( nroAtualTurmas + acrescimo );
	}
	aumentafile.close();

	// CODIGO 1
	//for( ; itMap != problemData->mapDiscDif.end(); itMap++ )
	//{
	//	int disciplinaId = itMap->first;
	//	int nroNaoAtend = itMap->second;

	//	Disciplina *disciplina = problemData->refDisciplinas[disciplinaId];
	//	int nroAtualTurmas = disciplina->getNumTurmas();
	//	double k=0;

	//	if ( capMediaSalas > 0 )
	//		k = (int)( nroNaoAtend/capMediaSalas + 0.5 );
	//	if (k == 0)
	//		k = 1;

	//	disciplina->setNumTurmas( nroAtualTurmas + k );
	//}
	
	// CODIGO 2
	//ITERA_GGROUP_LESSPTR( itDisciplina, problemData->disciplinas, Disciplina )
	//{
	//	Disciplina* disciplina = *itDisciplina;

	//	int nFolgas = 0;
	//	GGroup<Calendario*, LessPtr<Calendario>> semanasLetivas;

	//	GGroup<AlunoDemanda*, LessPtr<AlunoDemanda>> alDem = 
	//		problemData->retornaDemandasDiscNoCampus( disciplina->getId(), campusId, prioridade );

	//	ITERA_GGROUP_LESSPTR( itAlDem, alDem, AlunoDemanda )
	//	{
	//		int id = (*itAlDem)->getAlunoId();
	//		Aluno *aluno = problemData->retornaAluno( id );

	//		if ( problemData->retornaTurmaDiscAluno( aluno, disciplina ) == -1 )
	//		{
	//			semanasLetivas.add( aluno->getCalendario() );
	//			nFolgas++;
	//		}
	//	}
	//	if ( nFolgas )
	//	{
	//		double acrescimo=0;

	//		if ( capMediaSalas > 0 )
	//			acrescimo = (int)( nFolgas/capMediaSalas + 0.5 );
	//		if (acrescimo == 0)
	//			acrescimo = 1;
	//		
	//		acrescimo *= semanasLetivas.size();

	//		if ( nFolgas > 40 ) acrescimo+=1;

	//		int nTurmasAbertas = problemData->getNumTurmasJaAbertas( disciplina, campusId );
	//		int nroTotalTurmas = disciplina->getNumTurmas();
	//		int delta = ( nroTotalTurmas-nTurmasAbertas );
	//		acrescimo = acrescimo - delta;
	//		if ( acrescimo > 0 )
	//		{
	//			disciplina->setNumTurmas( nroTotalTurmas + acrescimo );
	//		}
	//	}
	//}
}

void TaticoIntAlunoHor::calculaNroFolgas( int prioridade, int campusId )
{	
	this->mapDiscNroFolgasDemandas.clear();
	ITERA_GGROUP_LESSPTR( itDisc, problemData->disciplinas, Disciplina )
	{
		Disciplina * disciplina = *itDisc;

		int n = problemData->getNroFolgasDeAtendimento( prioridade, disciplina, campusId );
		this->mapDiscNroFolgasDemandas[disciplina] = n;
	}
}

bool TaticoIntAlunoHor::permitirAbertura( int turma, Disciplina *disciplina, int campusId )
{
	Trio< int, Disciplina*, int > trio;
	trio.set( turma, disciplina, campusId );

	// Procura no mapPermitirAbertura, caso já tenha sido calculado
	if ( mapPermitirAbertura.find( trio ) != mapPermitirAbertura.end() )
		return mapPermitirAbertura[trio];
	
	// Não foi calculado para esse trio ainda, calcula e insere o resultado em mapPermitirAbertura.

	if ( problemData->existeTurmaDiscCampus( turma, disciplina->getId(), campusId ) )
	{
		mapPermitirAbertura[trio] = true;
		return true;
	}
	else if ( !PERMITIR_NOVAS_TURMAS )
	{
		mapPermitirAbertura[trio] = false;
		return false; // SEM NOVAS TURMAS
	}

	int nroFolgas = haFolgaDeAtendimento( disciplina );
				
//	if ( disciplina->getId()==12705 )						
//			std::cout<<"\nnroFolgas="<<nroFolgas; fflush(NULL);

	if ( problemData->parametros->considerar_equivalencia_por_aluno && this->USAR_EQUIVALENCIA )
	{
		int nroFolgasDeEquiv = 0;
		ITERA_GGROUP_LESSPTR( itDiscEquiv, disciplina->discEquivalentes, Disciplina ) // possiveis substituidas!
			nroFolgasDeEquiv += this->haFolgaDeAtendimento( *itDiscEquiv );
		nroFolgas += nroFolgasDeEquiv;
	}
	
	if ( nroFolgas == 0 ){ mapPermitirAbertura[trio] = false; return false; }

	int minAlunos;
	int nroMaxNovasTurmas;
 
	if ( ( problemData->parametros->min_alunos_abertura_turmas && !disciplina->eLab() ) || 
		 ( problemData->parametros->min_alunos_abertura_turmas_praticas && disciplina->eLab() ) )
	{		
		if ( problemData->parametros->min_alunos_abertura_turmas_praticas && disciplina->eLab() )
				minAlunos = problemData->parametros->min_alunos_abertura_turmas_praticas_value;

		if ( problemData->parametros->min_alunos_abertura_turmas && !disciplina->eLab() )
				minAlunos = problemData->parametros->min_alunos_abertura_turmas_value;

		if (minAlunos==0) minAlunos=1;
		nroMaxNovasTurmas = (int) nroFolgas/minAlunos;
		if ( nroFolgas>0 && nroMaxNovasTurmas==0 &&
			 problemData->parametros->violar_min_alunos_turmas_formandos )
		{
			if ( problemData->parametros->considerar_equivalencia_por_aluno && this->USAR_EQUIVALENCIA )
			{
				if ( problemData->haAlunoFormandoNaoAlocadoEquiv( disciplina, campusId, 2 ) )
					nroMaxNovasTurmas=1;
			}
			else
			{
				if ( problemData->haAlunoFormandoNaoAlocado( disciplina, campusId, 2 ) )
					nroMaxNovasTurmas=1;
			}
		}
	}
	else
	{
		minAlunos = 1;
		nroMaxNovasTurmas = (int) nroFolgas/minAlunos;
		if ( nroFolgas>0 && nroMaxNovasTurmas==0 ) nroMaxNovasTurmas=1;
	}	
	
	int counter = 0;
	for ( int i = 1; i<= disciplina->getNumTurmas(); i++ )
	{
		if ( !problemData->existeTurmaDiscCampus( i, disciplina->getId(), campusId ) )
		{
			counter++;
			if ( turma == i && counter <= nroMaxNovasTurmas ){ mapPermitirAbertura[trio] = true; return true; }
			else if ( counter > nroMaxNovasTurmas ){ mapPermitirAbertura[trio] = false; return false; }
		}
	}

	mapPermitirAbertura[trio] = false;
	return false;
}

void TaticoIntAlunoHor::sincronizaSolucao( int campusAtualId, int prioridade, int r )
{
   // --------------------------------------------------------------------
   // cria variaveis do tipo VariableTatico para as novas turmas abertas
   // e insere-as na lista vars_xh que contém a solução parcial até o momento.
   addVariaveisTatico();
   
   // ----------------------------------------------------
   // deleta solVarsTatInt, vars_v e hashs
   ITERA_GGROUP_LESSPTR ( it, solVarsTatInt, VariableTatInt )
   {
		delete *it;    
   }
   vars_v.clear();
   solVarsTatInt.clear();
   vHashTatico.clear();
   cHashTatico.clear();
   // ----------------------------------------------------

   return;
}

void TaticoIntAlunoHor::addVariaveisTatico()
{
	int nroAntigoTurmas = 0;

	if ( ! this->PERMITIR_NOVAS_TURMAS && this->CRIAR_VARS_FIXADAS ) // Primeiro modelo: inserção de alunos
	{
		ITERA_GGROUP_LESSPTR( itVar, (*solVarsTatico), VariableTatico )
		{
			if ( itVar->getType() == VariableTatico::V_ABERTURA )
				nroAntigoTurmas++;

			delete *itVar;
		}
		solVarsTatico->clear();
		vars_xh->clear();
	}

	VariableTatico vSol;
	int nroAtualTurmas=0;

	ITERA_GGROUP_LESSPTR( itVar, solVarsTatInt, VariableTatInt )
	{
		VariableTatInt *v = *itVar;

		switch( v->getType() )
		{		
			 case VariableTatInt::V_ERROR:
			 {
				break;
			 }		 
			 case VariableTatInt::V_CREDITOS:  //  x_{i,d,u,s,hi,hf,t} 
			 {
				VariableTatico * x = new VariableTatico();
				x->setType( VariableTatico::V_CREDITOS );
				x->setTurma( v->getTurma() );
				x->setDisciplina( v->getDisciplina() );
				x->setDia( v->getDia() );
				x->setHorarioAulaInicial( v->getHorarioAulaInicial() );
				x->setHorarioAulaFinal( v->getHorarioAulaFinal() );
				x->setSubCjtSala( v->getSubCjtSala() );
				x->setUnidade( v->getUnidade() );
				x->setValue( v->getValue() );
				x->setDateTimeInicial( v->getDateTimeInicial() );
				x->setDateTimeFinal( v->getDateTimeFinal() );

				if ( solVarsTatico->find( x ) == solVarsTatico->end() )
				{					
					this->vars_xh->add( x );
					this->solVarsTatico->add( x );
				}
				else
					delete x;
				break;
			 }
			 case VariableTatInt::V_DIAS_CONSECUTIVOS: // c_{i,d,t,cp}
			 {
				VariableTatico * x = new VariableTatico();
				x->setType( VariableTatico::V_DIAS_CONSECUTIVOS );
				x->setTurma( v->getTurma() );
				x->setDisciplina( v->getDisciplina() );
				x->setDia( v->getDia() );
				x->setCampus( v->getCampus() );
				x->setValue( v->getValue() );

				if ( solVarsTatico->find( x ) == solVarsTatico->end() )
				{
					this->solVarsTatico->add( x );
				}
				else
					delete x;
				break;
			 }
			 case VariableTatInt::V_SLACK_DIST_CRED_DIA_SUPERIOR: // fcp_{i,d,s,t}
			 {
				VariableTatico * x = new VariableTatico();
				x->setType( VariableTatico::V_SLACK_DIST_CRED_DIA_SUPERIOR );
				x->setTurma( v->getTurma() );
				x->setDisciplina( v->getDisciplina() );
				x->setUnidade( v->getUnidade() );
				x->setSubCjtSala( v->getSubCjtSala() );
				x->setDia( v->getDia() );
				x->setValue( v->getValue() );

				if ( solVarsTatico->find( x ) == solVarsTatico->end() )
				{
					this->solVarsTatico->add( x );
				}
				else
					delete x;
				break;
			 }
			 case VariableTatInt::V_SLACK_DIST_CRED_DIA_INFERIOR: // fcm_{i,d,s,t}
			 {
				VariableTatico * x = new VariableTatico();
				x->setType( VariableTatico::V_SLACK_DIST_CRED_DIA_INFERIOR );	// fcm_{i,d,s,t}
				x->setTurma( v->getTurma() );
				x->setDisciplina( v->getDisciplina() );
				x->setUnidade( v->getUnidade() );
				x->setSubCjtSala( v->getSubCjtSala() );
				x->setDia( v->getDia() );
				x->setValue( v->getValue() );

				if ( solVarsTatico->find( x ) == solVarsTatico->end() )
				{
					this->solVarsTatico->add( x );
				}
				else
					delete x;
				break;
			 }
			 case VariableTatInt::V_SLACK_DEMANDA_ALUNO: // fd_{d,a}
			 {
				 // TODO: tem que fazer algo?
	
				 break;
			 }
			 case VariableTatInt::V_COMBINACAO_DIVISAO_CREDITO: // m{i,d,k}
			 {
				VariableTatico * x = new VariableTatico();
				x->setType( VariableTatico::V_COMBINACAO_DIVISAO_CREDITO );	// m_{i,d,k}
				x->setTurma( v->getTurma() );
				x->setDisciplina( v->getDisciplina() );
				x->setK( v->getK() );
				x->setValue( v->getValue() );

				if ( solVarsTatico->find( x ) == solVarsTatico->end() )
				{
					this->solVarsTatico->add( x );
				}
				else
					delete x;
				break;
			 }
			 case VariableTatInt::V_SLACK_COMBINACAO_DIVISAO_CREDITO_M: // fkm{i,d,t}
			 {
				VariableTatico * x = new VariableTatico();
				x->setType( VariableTatico::V_SLACK_COMBINACAO_DIVISAO_CREDITO_M );	// fkm_{i,d,t}
				x->setTurma( v->getTurma() );
				x->setDisciplina( v->getDisciplina() );
				x->setDia( v->getDia() );
				x->setValue( v->getValue() );

				if ( solVarsTatico->find( x ) == solVarsTatico->end() )
				{
					this->solVarsTatico->add( x );
				}
				else
					delete x;
				break;
			 }
			 case VariableTatInt::V_SLACK_COMBINACAO_DIVISAO_CREDITO_P: // fkp{i,d,t}
			 {
				VariableTatico * x = new VariableTatico();
				x->setType( VariableTatico::V_SLACK_COMBINACAO_DIVISAO_CREDITO_P );	// fkp_{i,d,t}
				x->setTurma( v->getTurma() );
				x->setDisciplina( v->getDisciplina() );
				x->setDia( v->getDia() );
				x->setValue( v->getValue() );

				if ( solVarsTatico->find( x ) == solVarsTatico->end() )
				{
					this->solVarsTatico->add( x );
				}
				else
					delete x;
				break;
			 }
			 case VariableTatInt::V_ABERTURA_COMPATIVEL: // zc_{d,t,cp}
			 {
				VariableTatico * x = new VariableTatico();
				x->setType( VariableTatico::V_ABERTURA_COMPATIVEL );	// zc_{d,t,cp}
				x->setCampus( v->getCampus() );
				x->setDisciplina( v->getDisciplina() );
				x->setDia( v->getDia() );
				x->setValue( v->getValue() );

				if ( solVarsTatico->find( x ) == solVarsTatico->end() )
				{
					this->solVarsTatico->add( x );
				}
				else
					delete x;
				break;								
			 }		 		 
			 case VariableTatInt::V_ABERTURA: // z_{i,d,cp}
			 {
				VariableTatico * x = new VariableTatico();
				x->setType( VariableTatico::V_ABERTURA );	// z_{i,d,cp}
				x->setCampus( v->getCampus() );
				x->setDisciplina( v->getDisciplina() );
				x->setTurma( v->getTurma() );
				x->setValue( v->getValue() );

				if ( solVarsTatico->find( x ) == solVarsTatico->end() )
				{
					nroAtualTurmas++;
					this->solVarsTatico->add( x );
				}
				else
					delete x;
				break;								
			 }		 		 
		 				 		
			 default:
			 {
				 break;
			 }
		}
	}
	cout << "\n\n" << nroAtualTurmas - nroAntigoTurmas << " novas turmas.\n\n";
}

std::string TaticoIntAlunoHor::getCorrigeNrTurmasFileName( int campusId, int prioridade, int r)
{
	std::string solName;
	solName += "corrigeNrTurmas";
	solName += problemData->inputIdToString();
	 

   if ( BRANCH_SALA )
   {
		solName += "_BranchSala"; 
   } 
  
   if ( campusId != 0 )
   {	   
		 stringstream ss;
		 ss << campusId;
		 solName += "_Cp"; 
		 solName += ss.str();
   }
   if ( prioridade != 0 )
   {
		stringstream ss;
		ss << prioridade;
		solName += "_P"; 
		solName += ss.str();   		
   }
   if ( r != 0 )
   {
		stringstream ss;
		ss << r;
		solName += "_R"; 
		solName += ss.str();   		
   }   
   if ( ITERACAO != 0 )
   {
		stringstream ss;
		ss << ITERACAO;
		solName += "_"; 
		solName += ss.str();   		
   }
   if ( USAR_EQUIVALENCIA )
   {
		solName += "_Equiv"; 
   } 

   solName += ".txt";
      
   return solName;
}

std::string TaticoIntAlunoHor::getAumentaTurmasFileName( int campusId, int prioridade, int r)
{
	std::string solName;
	solName += "aumentaTurmas";
	solName += problemData->inputIdToString();
	 

   if ( BRANCH_SALA )
   {
		solName += "_BranchSala"; 
   } 
  
   if ( campusId != 0 )
   {	   
		 stringstream ss;
		 ss << campusId;
		 solName += "_Cp"; 
		 solName += ss.str();
   }
   if ( prioridade != 0 )
   {
		stringstream ss;
		ss << prioridade;
		solName += "_P"; 
		solName += ss.str();   		
   }
   if ( r != 0 )
   {
		stringstream ss;
		ss << r;
		solName += "_R"; 
		solName += ss.str();   		
   }   
   if ( ITERACAO != 0 )
   {
		stringstream ss;
		ss << ITERACAO;
		solName += "_"; 
		solName += ss.str();   		
   }
   if ( USAR_EQUIVALENCIA )
   {
		solName += "_Equiv"; 
   } 

   solName += ".txt";
      
   return solName;
}

std::string TaticoIntAlunoHor::getTaticoLpFileName( int campusId, int prioridade, int r )
{
	std::string solName;
	solName += "SolverTriedaTatInt";
	solName += problemData->inputIdToString();

   if ( BRANCH_SALA )
   {
		solName += "_BranchSala"; 
   } 

   if ( PERMITIR_NOVAS_TURMAS )
   {
		solName += "_NovasTurmas"; 
   } 

   if ( campusId != 0 )
   {	   
		 stringstream ss;
		 ss << campusId;
		 solName += "_Cp"; 
		 solName += ss.str();
   }

   if ( prioridade != 0 )
   {
		stringstream ss;
		ss << prioridade;
		solName += "_P"; 
		solName += ss.str();   		
   }

   if ( r != 0 )
   {
		stringstream ss;
		ss << r;
		solName += "_R"; 
		solName += ss.str();   		
   }
   
   if ( ITERACAO != 0 )
   {
		stringstream ss;
		ss << ITERACAO;
		solName += "_"; 
		solName += ss.str();   		
   }

   if ( USAR_EQUIVALENCIA )
   {
		solName += "_Equiv"; 
   } 


   return solName;
}

std::string TaticoIntAlunoHor::getSolBinFileName( int campusId, int prioridade, int r)
{
	std::string solName;
	solName += "solTatIntBin";
	solName += problemData->inputIdToString();

   if ( BRANCH_SALA )
   {
		solName += "_BranchSala"; 
   } 
	
   if ( PERMITIR_NOVAS_TURMAS )
   {
		solName += "_NovasTurmas"; 
   } 
  
   if ( campusId != 0 )
   {	   
		 stringstream ss;
		 ss << campusId;
		 solName += "_Cp"; 
		 solName += ss.str();
   }

   if ( prioridade != 0 )
   {
		stringstream ss;
		ss << prioridade;
		solName += "_P"; 
		solName += ss.str();   		
   }
   if ( r != 0 )
   {
		stringstream ss;
		ss << r;
		solName += "_R"; 
		solName += ss.str();   		
   }   
   if ( ITERACAO != 0 )
   {
		stringstream ss;
		ss << ITERACAO;
		solName += "_"; 
		solName += ss.str();   		
   }
   if ( USAR_EQUIVALENCIA )
   {
		solName += "_Equiv"; 
   } 

   solName += ".bin";
      
   return solName;
}

std::string TaticoIntAlunoHor::getSolucaoTaticoFileName( int campusId, int prioridade, int r, int fase )
{
	std::string solName;
	solName += "solucaoTaticoInt";
	solName += problemData->inputIdToString();
	

   if ( BRANCH_SALA )
   {
		solName += "_BranchSala"; 
   } 

   if ( PERMITIR_NOVAS_TURMAS )
   {
		solName += "_NovasTurmas"; 
   } 
   
   if ( campusId != 0 )
   {	   
		 stringstream ss;
		 ss << campusId;
		 solName += "_Cp"; 
		 solName += ss.str();
   }
   if ( prioridade != 0 )
   {
		stringstream ss;
		ss << prioridade;
		solName += "_P"; 
		solName += ss.str();   		
   }
   if ( r != 0 )
   {
		stringstream ss;
		ss << r;
		solName += "_R"; 
		solName += ss.str();   		
   }   
   if ( ITERACAO != 0 )
   {
		stringstream ss;
		ss << ITERACAO;
		solName += "_"; 
		solName += ss.str();   		
   }
   if ( USAR_EQUIVALENCIA )
   {
		solName += "_Equiv"; 
   } 
   if ( fase != 0 )
   {
		stringstream ss;
		ss << fase;
		solName += "_Fase"; 
		solName += ss.str();   		
   }

   solName += ".txt";
      
   return solName;
}

std::string TaticoIntAlunoHor::getEquivFileName( int campusId, int prioridade )
{
	std::string solName;
	solName += "Equivalencias";
	solName += problemData->inputIdToString();

   if ( BRANCH_SALA )
   {
		solName += "_BranchSala"; 
   } 
	   
   if ( PERMITIR_NOVAS_TURMAS )
   {
		solName += "_NovasTurmas"; 
   } 
   
   if ( campusId != 0 )
   {	   
		 stringstream ss;
		 ss << campusId;
		 solName += "_Cp"; 
		 solName += ss.str();
   }
   if ( prioridade != 0 )
   {
		stringstream ss;
		ss << prioridade;
		solName += "_P"; 
		solName += ss.str();   		
   }
   solName += ".txt";
      
   return solName;
}

void TaticoIntAlunoHor::writeSolBin( int campusId, int prioridade, int r, int type, double *xSol )
{
	#ifndef PRINT_LOGS
		return;
	#endif

	char solName[1024]="\0";

	switch (type)
	{
		case (TAT_INT_BIN):
			strcpy( solName, getSolBinFileName( campusId, prioridade, r ).c_str() );			
			break;
		case (TAT_INT_BIN1):
			strcpy( solName, "1" );
			strcat( solName, getSolBinFileName( campusId, prioridade, r ).c_str() );
			break;
		case (TAT_INT_BIN2):
			strcpy( solName, "2" );
			strcat( solName, getSolBinFileName( campusId, prioridade, r ).c_str() );
			break;
		case (TAT_INT_BIN3):
			strcpy( solName, "3" );
			strcat( solName, getSolBinFileName( campusId, prioridade, r ).c_str() );
			break;
		case (TAT_INT_CALOURO_BIN):
			strcpy( solName, "calouros" );
			strcat( solName, getSolBinFileName( campusId, prioridade, r ).c_str() );
			break;
	}

	// WRITES SOLUTION
		
	FILE * fout = fopen( solName, "wb" );
	if ( fout == NULL )
	{
		std::cout << "\nErro em TaticoIntAlunoHor::writeSolBin( int campusId, int prioridade, int r, int type ):"
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

void TaticoIntAlunoHor::readSolTxtAux( char *fileName, double *xSol )
{		
	ifstream fin( fileName, ios_base::in );
	if ( fin == NULL )
	{
		std::cout << "\nErro em TaticoIntAlunoHor::readSolTxtAux:"
				<< "\nArquivo " << fileName << " nao pode ser aberto.\n";
	}
	else
	{		
      for (int i=0; i < lp->getNumCols(); i++)
      {
         xSol[i] = 0.0;
      }

      bool finishVars = false;
      std::map<std::string,double> varVals;

      while ( !finishVars )
      {
         double val = 0.0;
         char buf[2048];
         std::string varName = "";
         fin >> buf;
         varName = buf;
         fin >> buf;

         if ( strcmp(buf,"=") != 0 )
         {
            finishVars = true;
            break;
         }

         fin >> val;

         varVals[varName] = val;
      }

      VariableTatIntHash::iterator vit = vHashTatico.begin();
		while ( vit != vHashTatico.end() )
		{
         VariableTatInt v = vit->first;
         int col = vit->second;
         double value = (int)( xSol[ col ] + 0.5 );
         std::string varName = v.toString();

         if ( varVals.find(varName) != varVals.end() )
         {
            xSol[col] = varVals[varName];
         }

         vit++;
      }
      varVals.clear();
      fin.close();
   }
}

void TaticoIntAlunoHor::writeSolTxt( int campusId, int prioridade, int r, int type, double *xSol, int fase )
{
	#ifndef PRINT_LOGS
		return;
	#endif

	char solName[1024]="\0";

	switch (type)
	{
		case (TAT_INT_BIN):
			strcpy( solName, getSolucaoTaticoFileName( campusId, prioridade, r, fase ).c_str() );			
			break;
		case (TAT_INT_BIN1):
			strcpy( solName, "1" );
			strcat( solName, getSolucaoTaticoFileName( campusId, prioridade, r, fase ).c_str() );
			break;
		case (TAT_INT_BIN2):
			strcpy( solName, "2" );
			strcat( solName, getSolucaoTaticoFileName( campusId, prioridade, r, fase ).c_str() );
			break;
		case (TAT_INT_BIN3):
			strcpy( solName, "3" );
			strcat( solName, getSolucaoTaticoFileName( campusId, prioridade, r, fase ).c_str() );
			break;
		case (TAT_INT_CALOURO_BIN):
			strcpy( solName, "calouros" );
			strcat( solName, getSolucaoTaticoFileName( campusId, prioridade, r, fase ).c_str() );
			break;
	}

	// WRITES SOLUTION
		
	ofstream fout( solName, ios_base::out );
	if ( fout == NULL )
	{
		std::cout << "\nErro em TaticoIntAlunoHor::writeSolTxt( int campusId, int prioridade, int r, int type ):"
				<< "\nArquivo " << solName << " nao pode ser aberto.\n";
	}
	else
	{		
		VariableTatIntHash::iterator vit = vHashTatico.begin();
		while ( vit != vHashTatico.end() )
		{
			VariableTatInt v = vit->first;
			int col = vit->second;
			double value = xSol[ col ];
		  
			fout << v.toString() << " = " << value << endl;
				  
			vit++;
		}
		fout.close();		
	}
}

int TaticoIntAlunoHor::readSolBin( int campusId, int prioridade, int r, int type, double *xSol )
{
	char solName[1024]="\0";

	switch (type)
	{
		case (TAT_INT_BIN):
			strcpy( solName, getSolBinFileName( campusId, prioridade, r ).c_str() );			
			break;
		case (TAT_INT_BIN1):
			strcpy( solName, "1" );
			strcat( solName, getSolBinFileName( campusId, prioridade, r ).c_str() );
			break;
		case (TAT_INT_BIN2):
			strcpy( solName, "2" );
			strcat( solName, getSolBinFileName( campusId, prioridade, r ).c_str() );
			break;
		case (TAT_INT_BIN3):
			strcpy( solName, "3" );
			strcat( solName, getSolBinFileName( campusId, prioridade, r ).c_str() );
			break;
		case (TAT_INT_CALOURO_BIN):
			strcpy( solName, "calouros" );
			strcat( solName, getSolBinFileName( campusId, prioridade, r ).c_str() );
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
		std::cout << "\nErro em readSolBin(int campusAtualId, int prioridade, int r): "
					<< " \nNumero diferente de variaveis: " << nCols << " != " << nroColsLP; fflush(NULL);
		return (0);
	}
	fclose(fin);
	
	return (1);
}

int TaticoIntAlunoHor::readSolTxt( int campusId, int prioridade, int r, int type, double *xSol, int fase )
{
   char solName[1024]="\0";

	switch (type)
	{
		case (TAT_INT_BIN):
			strcpy( solName, getSolucaoTaticoFileName( campusId, prioridade, r, fase ).c_str() );			
			break;
		case (TAT_INT_BIN1):
			strcpy( solName, "1" );
			strcat( solName, getSolucaoTaticoFileName( campusId, prioridade, r, fase ).c_str() );
			break;
		case (TAT_INT_BIN2):
			strcpy( solName, "2" );
			strcat( solName, getSolucaoTaticoFileName( campusId, prioridade, r, fase ).c_str() );
			break;
		case (TAT_INT_BIN3):
			strcpy( solName, "3" );
			strcat( solName, getSolucaoTaticoFileName( campusId, prioridade, r, fase ).c_str() );
			break;
		case (TAT_INT_CALOURO_BIN):
			strcpy( solName, "calouros" );
			strcat( solName, getSolucaoTaticoFileName( campusId, prioridade, r, fase ).c_str() );
			break;
	}

	// READS SOLUTION
		
	ifstream fin( solName, ios_base::in );
	if ( fin == NULL )
	{
		std::cout << "<============ Arquivo " << solName << " nao encontrado. Fim do carregamento de solucao.\n\n"; fflush(NULL);
		return (0);
	}
	else
	{
      
      for (int i=0; i < lp->getNumCols(); i++)
      {
         xSol[i] = 0.0;
      }

      bool finishVars = false;
      std::map<std::string,double> varVals;

      while ( !finishVars )
      {
         double val = 0.0;
         char buf[2048];
         std::string varName = "";
         fin >> buf;
         varName = buf;
         fin >> buf;

         if ( strcmp(buf,"=") != 0 )
         {
            finishVars = true;
            break;
         }

         fin >> val;

         varVals[varName] = val;
      }

      VariableTatIntHash::iterator vit = vHashTatico.begin();
      while ( vit != vHashTatico.end() )
      {
         VariableTatInt v = vit->first;
         int col = vit->second;
         double value = (int)( xSol[ col ] + 0.5 );
         std::string varName = v.toString();

         if ( varVals.find(varName) != varVals.end() )
         {
            xSol[col] = varVals[varName];
         }

         vit++;
      }
      varVals.clear();
      fin.close();
   }

   return 1;
}

int TaticoIntAlunoHor::writeGapTxt( int campusId, int prioridade, int r, int type, double gap )
{
	#ifndef PRINT_LOGS
		return 1;
	#endif

	std::string step;
	switch (type)
	{
		case (TAT_INT_BIN):
			step = "Final";		
			break;
		case (TAT_INT_BIN1):
			step = "Garante Solucao";
			break;
		case (TAT_INT_BIN2):
			step = "Max Atend";
			break;
		case (TAT_INT_BIN3):
			step = "Disponibilidade dos Profs";
			break;
		case (TAT_INT_CALOURO_BIN):
			step = "Max Atend Calouros";
			break;
	}


 	// Imprime Gap
	ofstream outGaps;
	std::string gapFilename( "gap_input" );
	gapFilename += problemData->getInputFileName();
	gapFilename += ".txt";

	outGaps.open(gapFilename, ofstream::app);
	if ( !outGaps )
	{
		std::cerr<<"\nErro: Abertura do arquivo " << gapFilename << " falhou em TaticoIntAlunoHor::writeGapTxt() em " << step << endl;
		return 0;
	}
	else
	{
		outGaps << "Tatico Integrado (" << step << ") - campus "<< campusId << ", prioridade " << prioridade << ", rodada "<< r;
		outGaps << "\nGap = " << gap << "%";
		
		if ( type == TAT_INT_BIN )
			outGaps << "\n\n------------------------------------------------------------------------------------------------";
		outGaps << "\n\n";

		outGaps.close();
		return 1;
	} 
}

void TaticoIntAlunoHor::initCredsSala()
{
   problemData->mapCreditosSalas.clear();
   if ( !CRIAR_VARS_FIXADAS )
   {
	   ITERA_GGROUP_LESSPTR( it, (*this->vars_xh), VariableTatico )
	   {
		    VariableTatico *v = *it;
			HorarioAula *hi = v->getHorarioAulaInicial();
			HorarioAula *hf = v->getHorarioAulaFinal();
			int numCreds = hi->getCalendario()->retornaNroCreditosEntreHorarios( hi, hf );
			int tempo = numCreds * v->getDisciplina()->getTempoCredSemanaLetiva();
			ConjuntoSala* vCjtSala = v->getSubCjtSala();
			if ( problemData->mapCreditosSalas.find(vCjtSala) == problemData->mapCreditosSalas.end() )
				problemData->mapCreditosSalas[vCjtSala] = tempo;
			else problemData->mapCreditosSalas[vCjtSala] += tempo;			
	   }
   }
}

int TaticoIntAlunoHor::getNroMaxAlunoDemanda( int discId )
{
	std::map< int /*discId*/, GGroup<AlunoDemanda*,LessPtr<AlunoDemanda>> >::iterator 
		it = mapDiscAlunosDemanda.find(discId);
	if ( it != mapDiscAlunosDemanda.end() )
		return it->second.size();
	else
		return 0;
}


void TaticoIntAlunoHor::carregaVariaveisSolucaoTaticoPorAlunoHor( int campusAtualId, int prioridade, int r )
{

   std::cout << "\nCarregando solucao tatico integrado...\n";

   double * xSol = NULL;
   
   lp->updateLP();

   int nroColsLP = lp->getNumCols();
   xSol = new double[ nroColsLP ];
	
	if ( *(this->CARREGA_SOLUCAO) )
	{
        int status = readSolTxt(campusAtualId, prioridade, r, OutPutFileType::TAT_INT_BIN, xSol, 0 );
		if ( !status )
		{
		    std::cout << "\nErro em TaticoIntAlunoHor::carregaVariaveisSolucaoTaticoPorAlunoHor(): arquivo "
					" nao encontrado.\n";
		    delete [] xSol;
			exit(0);
		}
	}
	else
	{
		lp->getX( xSol );fflush( NULL );
	}

   std::map< std::pair<Disciplina*, Oferta*>, int > mapSlackDemanda;
   std::map< std::pair<Disciplina*, Oferta*>, int >::iterator itMapSlackDemanda;
   
   // -----------------------------------------------------
   // Deleta todas as variaveis referenciadas em solVarsTatInt e em vars_v
   ITERA_GGROUP_LESSPTR ( it, solVarsTatInt, VariableTatInt )
   {
		delete *it;    
   }
   vars_v.clear();
   solVarsTatInt.clear();
   // -----------------------------------------------------

   problemData->listSlackDemandaAluno.clear();

   char solFilename[1024];
   strcpy( solFilename, getSolucaoTaticoFileName( campusAtualId, prioridade, r, 0 ).c_str() );

   FILE * fout = fopen( solFilename, "wt" );
   if ( fout == NULL )
   {
	   std::cout << "\nErro em SolverMIP::carregaVariaveisSolucaoTaticoPorAlunoHor( int campusAtualId, int prioridade, int r )"
				 << "\nArquivo nao pode ser aberto\n";
	   fout = fopen( "solSubstituto.txt", "wt" );
	   if ( fout == NULL )
	   {
			std::cout <<"\nErro de novo. Finalizando execucao...\n";
			exit(0);
	   }
   }
      

	initCredsSala();
	
	if ( PERMITIR_REALOCAR_ALUNO && CRIAR_VARS_FIXADAS )
//	if ( !this->PERMITIR_NOVAS_TURMAS && this->etapa == TaticoIntAlunoHor::Etapa1 ) // Modo inserção de alunos
	{
		ITERA_GGROUP_LESSPTR( itAluno, problemData->alunos, Aluno )
		{
			// TODO: se o aluno estiver em mais de um campus, só posso apagar os horarios usados no campus atual
			if ( itAluno->getCampusIds().size() > 1 )
				std::cout << "\nAtencao!! Aluno " << itAluno->getAlunoId() << " esta em mais de um campus. Erro aqui.";
			else if ( itAluno->hasCampusId( campusAtualId ) )
				itAluno->clearHorariosDiaOcupados();
		}

		problemData->clearMaps( campusAtualId, prioridade );
	}

   int nroNaoAtendimentoAlunoDemanda = 0;
   int nroAtendimentoAlunoDemanda = 0;
   
   VariableTatIntHash::iterator vit = vHashTatico.begin();
   while ( vit != vHashTatico.end() )
   {
	  VariableTatInt* v = new VariableTatInt( vit->first );
      int col = vit->second;
      v->setValue( xSol[ col ] );

      if ( v->getValue() > 0.00001 )
      {
         char auxName[1024];
         lp->getColName( auxName, col, 100 );
         fprintf( fout, "%s = %f\n", auxName, v->getValue() );

		 solVarsTatInt.add( v );
		 		 
		 Trio< int /*campusId*/, int /*turma*/, Disciplina* > trio;
		 std::map< Aluno*, GGroup< Trio< int /*campusId*/, int /*turma*/, Disciplina* > >, LessPtr< Aluno > >::iterator itMap;
		 Aluno *vAluno;
		 AlunoDemanda *alunoDem;
		 Disciplina *vDisc;
		 HorarioAula *vhi;
		 HorarioAula *vhf;
		 HorarioAula *ha;
		 Sala *vSala;
		 ConjuntoSala *vCjtSala;
		 int vDia;
		 int vTurma;
		 int vCampusId;
		 int nCreds;
		 int nAlunos;
		 GGroup<HorarioDia*> horariosDias;				 

         switch( v->getType() )
         {
			 case VariableTatInt::V_ERROR:
				std::cout << "Variável inválida " << std::endl;
				break;
			 case VariableTatInt::V_ALOCA_ALUNO_TURMA_DISC:	
				 vAluno = v->getAluno();
				 vDisc = v->getDisciplina();	

				 if ( this->USAR_EQUIVALENCIA )
					alunoDem = problemData->procuraAlunoDemOrigEquiv( vDisc, vAluno, prioridade );
				 else
					 alunoDem = vAluno->getAlunoDemanda( vDisc->getId() );

				 nroAtendimentoAlunoDemanda++;
				 
				 // -------------------------------------------
				 // Remove o alunoDemanda dos não-atendimentos, caso ele esteja lá
				 if ( problemData->listSlackDemandaAluno.find( alunoDem ) !=
					  problemData->listSlackDemandaAluno.end() )
				 {
					 problemData->listSlackDemandaAluno.remove(alunoDem);
				 }
				 				 
				 itMap = problemData->mapSlackAluno_CampusTurmaDisc.find( vAluno );
				 if ( itMap != problemData->mapSlackAluno_CampusTurmaDisc.end() )
				 {
					 int campusIdRemov = -1;
					 int turmaRemov = -1;
					 GGroup< Trio< int /*campusId*/, int /*turma*/, Disciplina* > >::iterator itTrios = itMap->second.begin();
					 for ( ; itTrios!=itMap->second.end(); itTrios++ )
					 {
						 if ( (*itTrios).third->getId() == vDisc->getId() )
						 {
							 campusIdRemov = (*itTrios).first;
							 turmaRemov = (*itTrios).second;
							 itMap->second.remove( *itTrios ); break;
						 }
					 }
					 if (turmaRemov!=-1)
					 {
						Trio< int /*campusId*/, int /*turma*/, Disciplina* > trio;
						trio.set( campusIdRemov, turmaRemov, vDisc );
						problemData->mapSlackCampusTurmaDisc_AlunosDemanda[trio].remove( alunoDem );
					 }
				 }				 
				 // -------------------------------------------

				 break;
			 case VariableTatInt::V_ALUNO_CREDITOS:
				 vDia = v->getDia();
				 vhi = v->getHorarioAulaInicial();
				 vhf = v->getHorarioAulaFinal();
				 vAluno = v->getAluno();
				 vDisc = v->getDisciplina();
				 vTurma = v->getTurma();
				 vCampusId = v->getUnidade()->getIdCampus();	 
				 trio.set(vCampusId, vTurma, vDisc);
				 
				 ha = vhi;
				 nCreds = vhi->getCalendario()->retornaNroCreditosEntreHorarios( vhi, vhf );				 
				 for ( int i = 1; i <= nCreds; i++ )
				 {
					HorarioDia *hd = problemData->getHorarioDiaCorrespondente( ha, vDia );
					horariosDias.add( hd );
					ha = vhi->getCalendario()->getProximoHorario( ha );			
				 }
				 				 
				 if ( this->USAR_EQUIVALENCIA )
					alunoDem = problemData->procuraAlunoDemOrigEquiv( vDisc, vAluno, prioridade );
				 else
					 alunoDem = vAluno->getAlunoDemanda( vDisc->getId() );

				 if ( alunoDem == NULL ) std::cout<<"\nErro, alunoDemanda nao encontrado para " << v->toString();
				 else problemData->insereAlunoEmTurma( alunoDem, trio, horariosDias );

				 vSala = v->getSubCjtSala()->salas.begin()->second;
				 vSala->addHorarioDiaOcupado( horariosDias );

				 vars_v.add( v );
				 break;
			 case VariableTatInt::V_CREDITOS:				 					 
				 vhi = v->getHorarioAulaInicial();
				 vhf = v->getHorarioAulaFinal();
				 vSala = v->getSubCjtSala()->salas.begin()->second;
				 vDia = v->getDia();
				 vDisc = v->getDisciplina();
				 vTurma = v->getTurma();
				 vCampusId = v->getUnidade()->getIdCampus();
				 
				 trio.set(vCampusId, vTurma, vDisc);

				 nAlunos=0;
				 if ( problemData->mapCampusTurmaDisc_AlunosDemanda.find(trio) !=
					  problemData->mapCampusTurmaDisc_AlunosDemanda.end() )
					nAlunos = problemData->mapCampusTurmaDisc_AlunosDemanda[trio].size();
				 else
					 std::cout<<"\nERRO! Turma aberta sem alunos: "<< v->toString();

				 //std::cout << " " << nAlunos << " vagas para a aula de "
					//	  << problemData->retornaNroCreditos( vhi, vhf, vSala, vDisc, vDia )
					//	  << " creditos da disciplina " << vDisc->getCodigo() << " id" <<vDisc->getId()
					//	  << " para a turma " << vTurma
					//	  << " no dia " << vDia
					//	  << " para a sala " << vSala->getId()
					//	  << std::endl << std::endl;
				 break;
			 case VariableTatInt::V_SLACK_DEMANDA_ALUNO:
				 nroNaoAtendimentoAlunoDemanda++;
				 alunoDem = v->getAluno()->getAlunoDemandaEquiv( v->getDisciplina() );
				 if ( alunoDem!=NULL ) problemData->listSlackDemandaAluno.add( alunoDem );
				 else std::cout<<"\nErro, AlunoDemanda nao encontrado. Aluno " 
					 << v->getAluno()->getAlunoId() << " disc " << v->getDisciplina()->getId();
				 break;
			 case VariableTatInt::V_OFERECIMENTO:			
				 vDisc = v->getDisciplina();
				 vCjtSala = v->getSubCjtSala();
				 if ( problemData->mapCreditosSalas.find(vCjtSala) == problemData->mapCreditosSalas.end() )
					  problemData->mapCreditosSalas[vCjtSala] = vDisc->getTotalTempo();
				 else problemData->mapCreditosSalas[vCjtSala] += vDisc->getTotalTempo();
				 break;
         }
      }
	  else
		  delete v;
	 
      vit++;
   }

	std::cout << std::endl;

	fprintf( fout, "\nAlunosDemanda nao atendidos = %d\n", nroNaoAtendimentoAlunoDemanda );
	fprintf( fout, "AlunosDemanda atendidos = %d", nroAtendimentoAlunoDemanda );

	std::cout << std::endl;

    if ( fout )
    {
        fclose( fout );
    }	
    
    if ( xSol )
    {
       delete [] xSol;
    }
   

   std::cout << "\nSolucao tatico integrado carregada com sucesso!\n";
   std::cout << "\n-----------------------------------------------------------------\n";
   std::cout << "-----------------------------------------------------------------\n\n";

    return;
}

int TaticoIntAlunoHor::solveTaticoIntAlunoHor( int campusId, int prioridade, int r )
{	

	int status = 0;
		
	bool CARREGA_SOL_PARCIAL = * this->CARREGA_SOLUCAO;

   if ( (*this->CARREGA_SOLUCAO) )
   {
	   char solName[1024];
	   strcpy( solName, getSolucaoTaticoFileName( campusId, prioridade, r, 0 ).c_str() );
	   //strcpy( solName, getSolBinFileName( campusId, prioridade, r ).c_str() );
	   FILE* fin = fopen( solName,"rb");
	   if ( fin == NULL )
	   {
		   std::cout << "\nA partir de " << solName << " , nao foram lidas mais solucoes.\n"; fflush(NULL);
		   *CARREGA_SOLUCAO = false;
	   }
	   else
	   {
		  fclose(fin);
	   }
   }

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

   if ( vHashTatico.size() > 0 )
   {
		vHashTatico.clear();
   }
   if ( cHashTatico.size() > 0 )
   {
	   cHashTatico.clear();
   }

   char lpName[1024], id[100];
   strcpy( lpName, getTaticoLpFileName( campusId, prioridade, r ).c_str() );

	if ( problemData->parametros->funcao_objetivo == 0
		|| problemData->parametros->funcao_objetivo == 2 )
	{
		lp->createLP( lpName, OPTSENSE_MAXIMIZE, PROB_MIP );
	}
	else if( problemData->parametros->funcao_objetivo == 1 )
	{
		lp->createLP( lpName, OPTSENSE_MINIMIZE, PROB_MIP );
	}

   std::cout<<"\nCreating LP...\n"; fflush(NULL);

	// Variable creation
	varNum = criaVariaveisTatico( campusId, prioridade, r );
		
	lp->updateLP();

	#ifdef PRINT_cria_variaveis
	printf( "Total of Variables: %i\n\n", varNum );
	#endif

//	if ( TaticoIntAlunoHor::idCounter > 1 || !SO_HEURN )
	if ( ! (*this->CARREGA_SOLUCAO) )
	{
		// Constraint creation
		constNum = criaRestricoesTatico( campusId, prioridade, r );

		lp->updateLP();

		#ifdef PRINT_cria_restricoes
		printf( "Total of Constraints: %i\n\n", constNum );
		#endif
		
		#ifdef PRINT_LOGS
		lp->writeProbLP( lpName );
		#endif
	 		

	    double *xS = new double[lp->getNumCols()];

		if ( TaticoIntAlunoHor::idCounter == 1 )
			solveGaranteSolucaoInicial( campusId, prioridade, r, CARREGA_SOL_PARCIAL, xS );
		else
			solveGaranteSolucao( campusId, prioridade, r, CARREGA_SOL_PARCIAL, xS );


		if ( this->PERMITIR_NOVAS_TURMAS )
			solveMaxAtendPorFasesDoDia( campusId, prioridade, r, CARREGA_SOL_PARCIAL, xS );
		else
			solveMaxAtend( campusId, prioridade, r, CARREGA_SOL_PARCIAL, xS );
		

		// ------------------------------------------------------------------------------------
		if ( ! this->PERMITIR_NOVAS_TURMAS )
		{
			#pragma region DISPONIBILIDADE DE PROFESSORES
			if ( problemData->parametros->considerar_disponibilidade_prof_em_tatico )
			{
				std::cout << "\n=========================================";
				std::cout << "\nConsiderando a disponibilidade de professores e min turmas...\n"; fflush(NULL);
			
				int *idxs = new int[lp->getNumCols()*2];
				double *vals = new double[lp->getNumCols()*2];				
				int *idxN = new int[lp->getNumCols()];

				double *objN = new double[lp->getNumCols()];
				lp->getObj(0,lp->getNumCols()-1,objN);

				// ADICIONA À FUNÇÃO OBJETIVO A FOLGA QUE CONSIDERA PROFESSORES -----------------------

				int nBds = 0;
				VariableTatIntHash::iterator vit = vHashTatico.begin();
				for ( ; vit != vHashTatico.end(); vit++ )
				{
					VariableTatInt v = vit->first;
			
					idxN[vit->second] = vit->second;

					if ( v.getType() == VariableTatInt::V_FOLGA_HOR_PROF )
					{            
						idxs[nBds] = vit->second;
						vals[nBds] = 1.0;
						nBds++;
					}			
					else if ( v.getType() == VariableTatInt::V_ABERTURA )
					{            
						idxs[nBds] = vit->second;
						vals[nBds] = 1.0;
						nBds++;
					}
					else
					{
						idxs[nBds] = vit->second;
						vals[nBds] = 0.0;
						nBds++;
					}
				}
				lp->chgObj(nBds,idxs,vals);
				// ------------------------------------------------------------------------------------

				lp->updateLP();

		
				std::string lpName3;
				lpName3 += "3_";
				lpName3 += string(lpName);

				#ifdef PRINT_LOGS
				lp->writeProbLP( lpName3.c_str() );
				#endif

				lp->copyMIPStartSol(lp->getNumCols(),idxN,xS);

				#ifdef SOLVER_CPLEX
				lp->setNumIntSols(0);
				lp->setTimeLimit( this->getTimeLimit(Solver::TAT_INT3) );
				//lp->setMIPRelTol( 0.01 );
				lp->setPreSolve(OPT_TRUE);
				lp->setHeurFrequency(1.0);
				lp->setMIPScreenLog( 4 );
				lp->setPolishAfterTime(100000000);
				lp->setPolishAfterIntSol(100000000);
				lp->setMIPEmphasis(0);
				lp->setPolishAfterNode(1);
				lp->setSymetry(0);
			 lp->setProbe(-1);
				lp->setCuts(0);
				lp->setPreSolve(OPT_TRUE);
				lp->updateLP();
				#endif
				#ifdef SOLVER_GUROBI
				lp->setNumIntSols(0);
				lp->setTimeLimit( this->getTimeLimit(Solver::TAT_INT3));
				lp->setPreSolveIntensity(OPT_LEVEL1);
				lp->setHeurFrequency(1.0);
				lp->setMIPScreenLog( 4 );
				lp->setPolishAfterTime(100000000);
				lp->setPolishAfterIntSol(100000000);
				lp->setMIPEmphasis(1);
				lp->setPolishAfterNode(1);
				lp->setSymetry(0);
				lp->setCuts(0);
				
				lp->setRins(10);
				lp->setHeurFrequency(0.5);

				#if defined SOLVER_GUROBI && defined USAR_CALLBACK		
				cb_data.timeLimit = this->getMaxTimeNoImprov(Solver::TAT_INT3);
				cb_data.gapMax = 100;
				lp->setCallbackFunc( &timeWithoutChangeCallback, &cb_data );
				#endif

				lp->updateLP();
				#endif
				
				if ( CARREGA_SOL_PARCIAL )
				{
					// procura e carrega solucao parcial
					int statusReadBin = readSolTxt(campusId, prioridade, r, OutPutFileType::TAT_INT_BIN3, xS, 0 );
					//int statusReadBin = readSolBin( campusId, prioridade, r, OutPutFileType::TAT_INT_BIN3, xS );
					if ( !statusReadBin )
					{
						CARREGA_SOL_PARCIAL=false;
					}
					else writeSolTxt( campusId, prioridade, r, OutPutFileType::TAT_INT_BIN3, xS, 0 );
				}
				if ( !CARREGA_SOL_PARCIAL )
				{
					// GENERATES SOLUTION 		 
					status = lp->optimize( METHOD_MIP );
					std::cout<<"\nStatus TAT_INT_BIN3 = "<<status; fflush(NULL);
					lp->getX(xS);
			
					if ( prioridade==1 && !( !PERMITIR_NOVAS_TURMAS && USAR_EQUIVALENCIA ) ) // Se não for a rodada de inserção com equiv
					{
						#if !defined (DEBUG) && !defined (TESTE)
						#ifdef SOLVER_CPLEX
							polishTaticoHor(xS, 3600*2, 90, 15, 3600);
							status = lp->optimize( METHOD_MIP );
							lp->getX(xS);
						#elif defined SOLVER_GUROBI
							lp->setCallbackFunc( NULL, NULL );
							polishTaticoHor(xS, 3600*2, 90, 15, 3600);

							#if defined SOLVER_GUROBI && defined USAR_CALLBACK
							lp->setCallbackFunc( &timeWithoutChangeCallback, &cb_data );			
							#endif
						#endif
						#endif
					}

					writeSolBin( campusId, prioridade, r, OutPutFileType::TAT_INT_BIN3, xS );
					writeSolTxt( campusId, prioridade, r, OutPutFileType::TAT_INT_BIN3, xS, 0 );
				}      

				// Imprime Gap
				writeGapTxt( campusId, prioridade, r, OutPutFileType::TAT_INT_BIN3, lp->getMIPGap() * 100 );

				// FIXA A SOLUÇÃO OBTIDA ANTERIORMENTE -------------------------------------------------
				BOUNDTYPE *bds = new BOUNDTYPE[lp->getNumCols()*2];
				
				nBds = 0;
				vit = vHashTatico.begin();
				for ( ; vit != vHashTatico.end(); vit++ )
				{
					VariableTatInt v = vit->first;

					if (  v.getType() == VariableTatInt::V_FOLGA_HOR_PROF )
					{
						idxs[nBds] = vit->second;
						vals[nBds] = xS[vit->second];
						bds[nBds] = BOUNDTYPE::BOUND_UPPER;
						nBds++;
					}			
				}
				lp->chgBds(nBds,idxs,bds,vals);

			
				// ------------------------------------------------------------------------------------
				// Volta FO original

				lp->chgObj(lp->getNumCols(),idxN,objN);
			
				lp->copyMIPStartSol(lp->getNumCols(),idxN,xS);

				delete[] objN;
				delete[] idxN;
				delete[] bds;
				delete[] vals;
				delete[] idxs;
			}
			#pragma endregion
		}

		std::cout << "\n=========================================";
	    std::cout << "\nGarantindo o resto dos parametros...\n"; fflush(NULL);
		
		lp->updateLP();
		#ifdef PRINT_LOGS
		lp->writeProbLP( lpName );
		#endif
				

#ifdef SOLVER_CPLEX
		lp->setNumIntSols(0);
		if ( !PERMITIR_NOVAS_TURMAS )
			lp->setTimeLimit(300);
		else
        	lp->setTimeLimit( this->getTimeLimit(Solver::TAT_INT) );
		//lp->setMIPRelTol( 0.01 );
		lp->setPreSolve(OPT_TRUE);
		lp->setHeurFrequency(1.0);
		lp->setMIPScreenLog( 4 );
		lp->setPolishAfterTime(1800);
		lp->setPolishAfterIntSol(100000000);
		lp->setMIPEmphasis(0);
		lp->setPolishAfterNode(1);
		lp->setSymetry(0);
      lp->setProbe(-1);
		lp->setCuts(0);
		lp->updateLP();
#endif
#ifdef SOLVER_GUROBI
		lp->setNumIntSols(0);
		if ( !PERMITIR_NOVAS_TURMAS )
			lp->setTimeLimit(1800);
		else
        	lp->setTimeLimit( this->getTimeLimit(Solver::TAT_INT) );
		lp->setPreSolveIntensity(OPT_LEVEL1);
		lp->setHeurFrequency(1.0);
		lp->setMIPScreenLog( 4 );
		lp->setPolishAfterTime( this->getTimeLimit(Solver::TAT_INT) / 3 );
		lp->setPolishAfterIntSol(100000000);
		lp->setMIPEmphasis(0);
		lp->setSymetry(2);
		lp->setCuts(0);
			
		lp->setRins(10);
		lp->setHeurFrequency(0.5);

		#if defined SOLVER_GUROBI && defined USAR_CALLBACK
		cb_data.timeLimit = this->getMaxTimeNoImprov(Solver::TAT_INT);
		cb_data.gapMax = 80;
		lp->setCallbackFunc( &timeWithoutChangeCallback, &cb_data );
		#endif

		lp->updateLP();
#endif
				
		if ( CARREGA_SOL_PARCIAL )
		{
			// procura e carrega solucao parcial
			int statusReadBin = readSolTxt(campusId, prioridade, r, OutPutFileType::TAT_INT_BIN, xS, 0 );
			//int statusReadBin = readSolBin( campusId, prioridade, r, OutPutFileType::TAT_INT_BIN, xS );
			if ( !statusReadBin )
			{
				CARREGA_SOL_PARCIAL=false;
			}
			else writeSolTxt( campusId, prioridade, r, OutPutFileType::TAT_INT_BIN, xS, 0 );
		}
		if ( !CARREGA_SOL_PARCIAL )
		{
			// GENERATES SOLUTION 		 
			status = lp->optimize( METHOD_MIP );
			std::cout<<"\nStatus TAT_INT_BIN = "<<status; fflush(NULL);
			lp->getX(xS);
				
			// Comentado porque já chegava aqui com o gap quase zero
			//if ( prioridade==1 && !( !PERMITIR_NOVAS_TURMAS && USAR_EQUIVALENCIA ) ) // Se não for a rodada de inserção com equiv
			//{
			//	#if !defined (DEBUG) && !defined (TESTE)
			//	#ifdef SOLVER_CPLEX
			//		polishTaticoHor(xS, 3600*2, 90, 15, 3600);
			//	#elif defined SOLVER_GUROBI
			//		writeSolTxt( campusId, prioridade, r, OutPutFileType::TAT_INT_BIN, xS, 0 );	// intermediaria
			//		
			//		lp->setCallbackFunc( NULL, NULL );
			//		polishTaticoHor(xS, 3600, 90, 20, 1800);
			//	#endif
			//	#endif
			//}

			writeSolBin( campusId, prioridade, r, OutPutFileType::TAT_INT_BIN, xS );
			writeSolTxt( campusId, prioridade, r, OutPutFileType::TAT_INT_BIN, xS, 0 );
		}
			  		
		fflush(NULL);
		
		delete[] xS;	

	    // Imprime Gap
		writeGapTxt( campusId, prioridade, r, OutPutFileType::TAT_INT_BIN, lp->getMIPGap() * 100 );
			
		lp->updateLP();
	}

	std::cout<<"\n------------------------------Fim do Tatico Integrado -----------------------------\n"; fflush(NULL);

   return status;
}


int TaticoIntAlunoHor::solveTaticoIntAlunoHor_EtapaSimples( int campusId, int prioridade, int r )
{	

	int status = 0;
		
	bool CARREGA_SOL_PARCIAL = * this->CARREGA_SOLUCAO;

   if ( (*this->CARREGA_SOLUCAO) )
   {
	   char solName[1024];
	   strcpy( solName, getSolucaoTaticoFileName( campusId, prioridade, r, 0 ).c_str() );
	   FILE* fin = fopen( solName,"rb");
	   if ( fin == NULL )
	   {
		   std::cout << "\nA partir de " << solName << " , nao foram lidas mais solucoes.\n"; fflush(NULL);
		   *CARREGA_SOLUCAO = false;
	   }
	   else
	   {
		  fclose(fin);
	   }
   }

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

   if ( vHashTatico.size() > 0 )
   {
		vHashTatico.clear();
   }
   if ( cHashTatico.size() > 0 )
   {
	   cHashTatico.clear();
   }

   char lpName[1024], id[100];
   strcpy( lpName, getTaticoLpFileName( campusId, prioridade, r ).c_str() );

	if ( problemData->parametros->funcao_objetivo == 0
		|| problemData->parametros->funcao_objetivo == 2 )
	{
		lp->createLP( lpName, OPTSENSE_MAXIMIZE, PROB_MIP );
	}
	else if( problemData->parametros->funcao_objetivo == 1 )
	{
		lp->createLP( lpName, OPTSENSE_MINIMIZE, PROB_MIP );
	}

   std::cout<<"\nCreating LP...\n"; fflush(NULL);


	// Variable creation
	varNum = criaVariaveisTatico( campusId, prioridade, r );
		
	lp->updateLP();

	#ifdef PRINT_cria_variaveis
	printf( "Total of Variables: %i\n\n", varNum );
	#endif

	if ( ! (*this->CARREGA_SOLUCAO) )
	{
		// Constraint creation
		constNum = criaRestricoesTatico( campusId, prioridade, r );

		lp->updateLP();

		#ifdef PRINT_cria_restricoes
		printf( "Total of Constraints: %i\n\n", constNum );
		#endif
		
		#ifdef PRINT_LOGS
		lp->writeProbLP( lpName );
		#endif
	}


   if ( ! (*this->CARREGA_SOLUCAO) )
   {   	   		
	    double *xS = new double[lp->getNumCols()];

		if ( TaticoIntAlunoHor::idCounter == 1 )
			solveGaranteSolucaoInicial( campusId, prioridade, r, CARREGA_SOL_PARCIAL, xS );
		else
			solveGaranteSolucao( campusId, prioridade, r, CARREGA_SOL_PARCIAL, xS );
		

		std::cout<<"\nResolvendo o lp com FO original...\n"; fflush(NULL);
		
		lp->updateLP();
		#ifdef PRINT_LOGS
		lp->writeProbLP( lpName );
		#endif

#ifdef SOLVER_CPLEX
		lp->setNumIntSols(0);
		if ( !PERMITIR_NOVAS_TURMAS )
			lp->setTimeLimit(300);
		else
        	lp->setTimeLimit( this->getTimeLimit(Solver::TAT_INT) );
		//lp->setMIPRelTol( 0.01 );
		lp->setPreSolve(OPT_TRUE);
		lp->setHeurFrequency(1.0);
		lp->setMIPScreenLog( 4 );
		lp->setPolishAfterTime(1800);
		lp->setPolishAfterIntSol(100000000);
		lp->setMIPEmphasis(0);
		lp->setPolishAfterNode(1);
		lp->setSymetry(0);
      lp->setProbe(-1);
		lp->setCuts(0);
		lp->updateLP();
#endif
#ifdef SOLVER_GUROBI
		lp->setNumIntSols(0);
		if ( !PERMITIR_NOVAS_TURMAS )
			lp->setTimeLimit(2200);
		else
        	lp->setTimeLimit( this->getTimeLimit(Solver::TAT_INT) );
		lp->setPreSolveIntensity(OPT_LEVEL1);
		lp->setHeurFrequency(1.0);
		lp->setMIPScreenLog( 4 );
		lp->setPolishAfterTime( this->getTimeLimit(Solver::TAT_INT) / 3 );
		lp->setPolishAfterIntSol(100000000);
		lp->setMIPEmphasis(0);
		lp->setSymetry(2);
		lp->setCuts(0);
			
		lp->setRins(10);
		lp->setHeurFrequency(0.5);

		#if defined SOLVER_GUROBI && defined USAR_CALLBACK
		cb_data.timeLimit = this->getMaxTimeNoImprov(Solver::TAT_INT);
		cb_data.gapMax = 80;
		lp->setCallbackFunc( &timeWithoutChangeCallback, &cb_data );
		#endif

		lp->updateLP();
#endif
				
		if ( CARREGA_SOL_PARCIAL )
		{
			// procura e carrega solucao parcial
			int statusReadBin = readSolTxt(campusId, prioridade, r, OutPutFileType::TAT_INT_BIN, xS, 0 );
			//int statusReadBin = readSolBin( campusId, prioridade, r, OutPutFileType::TAT_INT_BIN, xS );
			if ( !statusReadBin )
			{
				CARREGA_SOL_PARCIAL=false;
			}
			else writeSolTxt( campusId, prioridade, r, OutPutFileType::TAT_INT_BIN, xS, 0 );
		}
		if ( !CARREGA_SOL_PARCIAL )
		{
			// GENERATES SOLUTION 		 
			status = lp->optimize( METHOD_MIP );
			std::cout<<"\nStatus TAT_INT_BIN = "<<status; fflush(NULL);
			lp->getX(xS);
				
			// comentado pq estava chegando aqui com gap quase zero
			//if ( prioridade==1 && !( !PERMITIR_NOVAS_TURMAS && USAR_EQUIVALENCIA ) ) // Se não for a rodada de inserção com equiv
			//{
			//	#if !defined (DEBUG) && !defined (TESTE)
			//	#ifdef SOLVER_CPLEX
			//		polishTaticoHor(xS, 3600*2, 90, 15, 3600);
			//	#elif defined SOLVER_GUROBI
			//		writeSolTxt( campusId, prioridade, r, OutPutFileType::TAT_INT_BIN, xS, 0 );	// intermediaria
			//		
			//		lp->setCallbackFunc( NULL, NULL );
			//		polishTaticoHor(xS, 3600, 90, 20, 1800);
			//	#endif
			//	#endif
			//}

			writeSolBin( campusId, prioridade, r, OutPutFileType::TAT_INT_BIN, xS );
			writeSolTxt( campusId, prioridade, r, OutPutFileType::TAT_INT_BIN, xS, 0 );
		}
			  		
		fflush(NULL);
		
		delete[] xS;	

	    // Imprime Gap
		writeGapTxt( campusId, prioridade, r, OutPutFileType::TAT_INT_BIN, lp->getMIPGap() * 100 );
			
		lp->updateLP();
	}

	std::cout<<"\n------------------------------Fim do Tatico Integrado -----------------------------\n"; fflush(NULL);

   return status;
}


int TaticoIntAlunoHor::solveGaranteSolucao( int campusId, int prioridade, int r, bool& CARREGA_SOL_PARCIAL, double *xS )
{
	//std::string lpName0;
	//lpName0 += "0_";
	//lpName0 += string(lpName);
	//#ifdef PRINT_LOGS
	//lp->writeProbLP( lpName0.c_str() );
	//#endif

	// -------------------------------------------------------------------
    char lpName[1024];
    strcpy( lpName, getTaticoLpFileName( campusId, prioridade, r ).c_str() );
	
	// -------------------------------------------------------------------
    
	//std::set< int > vHashLivresOriginais;

	int *idxs = new int[lp->getNumCols()*2];
	double *vals = new double[lp->getNumCols()*2];
	double *valsOrig = new double[lp->getNumCols()*2];
	BOUNDTYPE *bds = new BOUNDTYPE[lp->getNumCols()*2];
	int nBds = 0;

	std::cout << "\n=========================================";
	std::cout << "\nGarantindo solucao...\n"; fflush(NULL);

	VariableTatIntHash::iterator vit = vHashTatico.begin();
	for ( ; vit != vHashTatico.end(); vit++ )
	{
		VariableTatInt v = vit->first;

		int lb = (int)(lp->getLB(vit->second) + 0.5);
		int ub = (int)(lp->getUB(vit->second) + 0.5);

		//if ( lb == ub && (lb==0 || lb==1) )				// se for variavel binaria fixa
		//{
		//	if ( lb == 0 )
		//	{
		//		idxs[nBds] = vit->second;
		//		vals[nBds] = 0.0;						// manter, para achar solução inicial
		//		bds[nBds] = BOUNDTYPE::BOUND_UPPER;
 	//			valsOrig[nBds] = 1.0;					// deixar livre depois!
		//		nBds++;
		//	}
		//	else
		//	{
		//		idxs[nBds] = vit->second;
		//		vals[nBds] = 1.0;						// manter, para achar solução inicial
		//		bds[nBds] = BOUNDTYPE::BOUND_LOWER;
 	//			valsOrig[nBds] = 0.0;					// deixar livre depois!
		//		nBds++;
		//	}
		//}

		//if ( v.getType() == VariableTatInt::V_ABERTURA )
		//{
		//	int lb = (int)(lp->getLB(vit->second) + 0.5);
		//	int ub = (int)(lp->getUB(vit->second) + 0.5);

		//	if ( lb != ub )								// se for variavel livre
		//	{
		//		//vHashLivresOriginais.insert( vit->second );

		//		idxs[nBds] = vit->second;
		//		valsOrig[nBds] = lp->getUB( vit->second );
		//		vals[nBds] = 0.0;
		//		bds[nBds] = BOUNDTYPE::BOUND_UPPER;
		//		nBds++;
		//	}
		//}

		if ( v.getType() == VariableTatInt::V_ALOCA_ALUNO_TURMA_DISC )
		{
			int lb = (int)(lp->getLB(vit->second) + 0.5);
			int ub = (int)(lp->getUB(vit->second) + 0.5);

			int turma = v.getTurma();
			Aluno *aluno = v.getAluno();
			Disciplina *disciplina = v.getDisciplina();

			if ( problemData->retornaTurmaDiscAluno( aluno, disciplina ) == turma )
			{
				if ( lb != ub )							// se for variavel livre
				{						
					idxs[nBds] = vit->second;
					valsOrig[nBds] = lp->getLB( vit->second );
					vals[nBds] = 1.0;
					bds[nBds] = BOUNDTYPE::BOUND_LOWER;
					nBds++;
				}
			}
			else
			{
				if ( lb != ub )							// se for variavel livre
				{						
					idxs[nBds] = vit->second;
					valsOrig[nBds] = lp->getUB( vit->second );
					vals[nBds] = 0.0;
					bds[nBds] = BOUNDTYPE::BOUND_UPPER;
					nBds++;
				}			
			}
		}
	}
	lp->chgBds(nBds,idxs,bds,vals);
	   
#ifdef SOLVER_CPLEX
	lp->updateLP();
	lp->setTimeLimit( this->getTimeLimit(Solver::TAT_INT1) );
	lp->setPreSolve(OPT_TRUE);
	lp->setHeurFrequency(1.0);
	lp->setMIPScreenLog( 4 );
	lp->setMIPEmphasis(0);
	lp->setSymetry(0);
	lp->setCuts(3);
	lp->setNumIntSols(1);	
	lp->updateLP();
#elif SOLVER_GUROBI
	lp->updateLP();
	lp->setTimeLimit( this->getTimeLimit(Solver::TAT_INT1) );
	lp->setPreSolve(OPT_TRUE);
	lp->setHeurFrequency(1.0);
	lp->setMIPScreenLog( 4 );
	lp->setMIPEmphasis(0);
	lp->setSymetry(0);
	lp->setCuts(3);
	lp->setNumIntSols(1);	

	#if defined SOLVER_GUROBI && defined USAR_CALLBACK
	cb_data.timeLimit = this->getMaxTimeNoImprov(Solver::TAT_INT1);
	cb_data.gapMax = 80;
	lp->setCallbackFunc( &timeWithoutChangeCallback, &cb_data );
	#endif

	lp->updateLP();
#endif

	std::string lpName1;
	lpName1 += "1_";
	lpName1 += string(lpName);

	#ifdef PRINT_LOGS
	lp->writeProbLP( lpName1.c_str() );
	#endif
	
	// GENERATES SOLUTION 
	int status = lp->optimize( METHOD_MIP );
	std::cout<<"\nStatus TAT_INT_BIN1 = "<<status; fflush(NULL);
	lp->getX(xS);
	  	
	writeSolBin( campusId, prioridade, r, OutPutFileType::TAT_INT_BIN1, xS );
	writeSolTxt( campusId, prioridade, r, OutPutFileType::TAT_INT_BIN1, xS, 0 );
		
			
	// -------------------------------------------------------------------
	// Volta as variaveis que estavam livres
         
	lp->chgBds( nBds,idxs,bds,valsOrig );
	lp->updateLP();

	delete[] valsOrig;		
	delete[] vals;
	delete[] idxs;
	delete[] bds;

	return status;
}


int TaticoIntAlunoHor::solveGaranteSolucaoInicial( int campusId, int prioridade, int r, bool& CARREGA_SOL_PARCIAL, double *xS )
{
	//std::string lpName0;
	//lpName0 += "0_";
	//lpName0 += string(lpName);
	//#ifdef PRINT_LOGS
	//lp->writeProbLP( lpName0.c_str() );
	//#endif

	// -------------------------------------------------------------------
    char lpName[1024];
    strcpy( lpName, getTaticoLpFileName( campusId, prioridade, r ).c_str() );
	
	// -------------------------------------------------------------------
    
	
	int *idxs = new int[lp->getNumCols()*2];
	double *vals = new double[lp->getNumCols()*2];
	double *valsOrig = new double[lp->getNumCols()*2];
	BOUNDTYPE *bds = new BOUNDTYPE[lp->getNumCols()*2];
	int nBds = 0;

	std::cout << "\n=========================================";
	std::cout << "\nGarantindo solucao...\n"; fflush(NULL);

	SolucaoTaticoInicial *solInic = problemData->getSolTaticoInicial();

	VariableTatIntHash::iterator vit = vHashTatico.begin();
	for ( ; vit != vHashTatico.end(); vit++ )
	{
		VariableTatInt v = vit->first;

		if ( v.getType() == VariableTatInt::V_ABERTURA )
		{
			int lb = (int)(lp->getLB(vit->second) + 0.5);
			int ub = (int)(lp->getUB(vit->second) + 0.5);

			if ( lb != ub )								// se for variavel livre
			{
				idxs[nBds] = vit->second;
				valsOrig[nBds] = lp->getUB( vit->second );
				vals[nBds] = 0.0;
				bds[nBds] = BOUNDTYPE::BOUND_UPPER;
				nBds++;
			}
		}

		if ( v.getType() == VariableTatInt::V_ALOCA_ALUNO_TURMA_DISC )
		{
			int lb = (int)(lp->getLB(vit->second) + 0.5);
			int ub = (int)(lp->getUB(vit->second) + 0.5);

			int turma = v.getTurma();
			Aluno *aluno = v.getAluno();
			Disciplina *disciplina = v.getDisciplina();
			bool fixar;

			if ( problemData->retornaTurmaDiscAluno( aluno, disciplina ) == -1 )
			if ( solInic->getTurma( aluno, campusId, disciplina, fixar ) == turma )
			{
				if ( lb != ub )							// se for variavel livre
				{
					idxs[nBds] = vit->second;
					valsOrig[nBds] = lp->getLB( vit->second );
					vals[nBds] = 1.0;
					bds[nBds] = BOUNDTYPE::BOUND_LOWER;
					nBds++;
				}
			}
		}
	}
	lp->chgBds(nBds,idxs,bds,vals);
	   
#ifdef SOLVER_CPLEX
	lp->updateLP();
	lp->setTimeLimit( this->getTimeLimit(Solver::TAT_INT1) );
	lp->setPreSolve(OPT_TRUE);
	lp->setHeurFrequency(1.0);
	lp->setMIPScreenLog( 4 );
	lp->setMIPEmphasis(0);
	lp->setSymetry(0);
	lp->setCuts(3);
	lp->setNumIntSols(1);	
	lp->updateLP();
#elif SOLVER_GUROBI
	lp->updateLP();
	lp->setTimeLimit( this->getTimeLimit(Solver::TAT_INT1) );
	lp->setPreSolve(OPT_TRUE);
	lp->setHeurFrequency(1.0);
	lp->setMIPScreenLog( 4 );
	lp->setMIPEmphasis(0);
	lp->setSymetry(0);
	lp->setCuts(3);
	lp->setNumIntSols(1);	

	#if defined SOLVER_GUROBI && defined USAR_CALLBACK
	cb_data.timeLimit = this->getMaxTimeNoImprov(Solver::TAT_INT1);
	cb_data.gapMax = 80;
	lp->setCallbackFunc( &timeWithoutChangeCallback, &cb_data );
	#endif

	lp->updateLP();
#endif

	std::string lpName1;
	lpName1 += "1_";
	lpName1 += string(lpName);

	#ifdef PRINT_LOGS
	lp->writeProbLP( lpName1.c_str() );
	#endif
	
	// GENERATES SOLUTION 
	int status = lp->optimize( METHOD_MIP );
	std::cout<<"\nStatus TAT_INT_BIN1 = "<<status; fflush(NULL);
	lp->getX(xS);
	  	
	writeSolBin( campusId, prioridade, r, OutPutFileType::TAT_INT_BIN1, xS );
	writeSolTxt( campusId, prioridade, r, OutPutFileType::TAT_INT_BIN1, xS, 0 );
		
			
	// -------------------------------------------------------------------
	// Volta as variaveis que estavam livres
         
	lp->chgBds( nBds,idxs,bds,valsOrig );
	lp->updateLP();

	delete[] valsOrig;		
	delete[] vals;
	delete[] idxs;
	delete[] bds;

	return status;
}


int TaticoIntAlunoHor::solveMaxAtendPorFasesDoDia( int campusId, int prioridade, int r, bool& CARREGA_SOL_PARCIAL, double *xS )
{
	std::cout << "\n==========================>>>>";
	std::cout << "\nGarantindo maximo atendimento geral por fases do dia...\n"; fflush(NULL);
		
	int status = 0;
	
	// -------------------------------------------------------------------

    char lpName[1024];
    strcpy( lpName, getTaticoLpFileName( campusId, prioridade, r ).c_str() );
	
	// -------------------------------------------------------------------
	// Salvando função objetivo original

	int *idxN = new int[lp->getNumCols()];
	double *objOrig = new double[lp->getNumCols()];
	lp->getObj(0,lp->getNumCols()-1,objOrig);
				
	// -------------------------------------------------------------------
	// Modificando coeficientes na função objetivo

	int *idxFO_Geral = new int[lp->getNumCols()*2];
	int *idxFO_FC = new int[lp->getNumCols()*2];
	double *valFO_Geral = new double[lp->getNumCols()*2];        // FO para todos os AlunoDemanda
	double *valFO_FC = new double[lp->getNumCols()*2];			 // FO somente para ALunoDemanda de formandos/calouros
	//int nChgFO = 0;


	// -------------------------------------------------------------------
	// FO para formandos e calouros

	#pragma region FO para formandos e calouros
	int nChgFO_FC = 0;
	for ( VariableTatIntHash::iterator vit = vHashTatico.begin(); vit != vHashTatico.end(); vit++ )
	{
		VariableTatInt v = vit->first;

		idxN[vit->second] = vit->second;
		
		double coef = 0.0;
		if ( v.getType() == VariableTatInt::V_SLACK_DEMANDA_ALUNO )
		{
			if ( ( problemData->parametros->priorizarCalouros && v.getAluno()->ehCalouro() ) ||
				 ( problemData->parametros->priorizarFormandos && v.getAluno()->ehFormando() ) )
			{
				coef = v.getDisciplina()->getTotalCreditos() * v.getAluno()->getReceita( v.getDisciplina() );				
			}
		}
		else if ( v.getType() == VariableTatInt::V_ABERTURA )
		{
			coef = 1.0;
		}
		else if ( v.getType() == VariableTatInt::V_SLACK_ABERT_SEQ_TURMA )
		{  
			coef = 0.1;
		}

		idxFO_FC[nChgFO_FC] = vit->second;
		valFO_FC[nChgFO_FC] = coef;
		nChgFO_FC++;
    }
	#pragma endregion

	// -------------------------------------------------------------------
	// FO para geral
	
	#pragma region FO para todos os alunos
	int nChgFO_Geral = 0;
	for ( VariableTatIntHash::iterator vit = vHashTatico.begin(); vit != vHashTatico.end(); vit++ )
	{
		VariableTatInt v = vit->first;
											
		double coef = 0.0;
		if ( v.getType() == VariableTatInt::V_SLACK_DEMANDA_ALUNO )
		{  
			coef = 1.0;
		}
		else if ( v.getType() == VariableTatInt::V_FOLGA_ALUNO_MIN_ATEND1 )	// fmd: folga de mínimo de demanda por aluno
		{
			coef = 1.0;
		}
		else if ( v.getType() == VariableTatInt::V_FOLGA_ALUNO_MIN_ATEND2 )	// fmd: folga de mínimo de demanda por aluno
		{
			coef = 5.0;
		}
		else if ( v.getType() == VariableTatInt::V_FOLGA_ALUNO_MIN_ATEND3 )	// fmd: folga de mínimo de demanda por aluno
		{
			coef = 10.0;
		}
		else if ( v.getType() == VariableTatInt::V_ABERTURA )
		{  
			coef = 1.0;
		}
		else if ( v.getType() == VariableTatInt::V_SLACK_ABERT_SEQ_TURMA )
		{  
			coef = 0.1;
		}

		idxFO_Geral[nChgFO_Geral] = vit->second;
		valFO_Geral[nChgFO_Geral] = coef;
		nChgFO_Geral++;
    }
	#pragma endregion
	
	// =====================================================================================
	// CALCULA MÁXIMO ATENDIMENTO POR PARTES DO DIA

	int *idxUB_Fixa0 = new int[lp->getNumCols()*2];
	double *valUB_Fixa0 = new double[lp->getNumCols()*2];
	double *valOrigUB_0 = new double[lp->getNumCols()*2];
	BOUNDTYPE *btsUB_Fixa0 = new BOUNDTYPE[lp->getNumCols()*2];

	int *idxLB_Q = new int[lp->getNumCols()*2];
	double *valLB_Q = new double[lp->getNumCols()*2];
	double *valOrigLB_Q = new double[lp->getNumCols()*2];
	BOUNDTYPE *btsLB_Q = new BOUNDTYPE[lp->getNumCols()*2];
	
	int *idxUB = new int[lp->getNumCols()*2];
	double *valUB = new double[lp->getNumCols()*2];
	BOUNDTYPE *btsUB = new BOUNDTYPE[lp->getNumCols()*2];
	
	int nChgLB_Q = 0;
	for( auto itFase = problemData->fasesDosTurnos.begin(); 
		itFase != problemData->fasesDosTurnos.end(); itFase++ )
	{		 
		int fase = itFase->first;

		std::cout << "\n======>> Fase " << fase << endl;

		#pragma region Fixa não-atendimentos de fases à frente.
		int nChgUB_Fixa0 = 0;
		for ( VariableTatIntHash::iterator vit = vHashTatico.begin(); vit != vHashTatico.end(); vit++ )
		{
			VariableTatInt v = vit->first;
									
			if ( v.getType() == VariableTatInt::V_CREDITOS )
			{
				HorarioAula *ha = v.getHorarioAulaInicial();
				int turno = problemData->getFaseDoDia( ha->getInicio() );

				if ( turno > fase )
				{
					double ub = (int)( lp->getUB( vit->second ) + 0.5 );
					double lb = (int)( lp->getLB( vit->second ) + 0.5 );

					if ( lb != ub )
					{
						valOrigUB_0[nChgUB_Fixa0] = lp->getUB( vit->second );
						idxUB_Fixa0[nChgUB_Fixa0] = vit->second;
						valUB_Fixa0[nChgUB_Fixa0] = 0.0;
						btsUB_Fixa0[nChgUB_Fixa0] = BOUNDTYPE::BOUND_UPPER;
						nChgUB_Fixa0++;
					}
				}
			}
		}
		lp->chgBds( nChgUB_Fixa0, idxUB_Fixa0, btsUB_Fixa0, valUB_Fixa0 );
		#pragma endregion

		stringstream ss;
		ss << fase;
		std::string sf = ss.str();


		if ( problemData->parametros->priorizarCalouros ||
			 problemData->parametros->priorizarFormandos )
		{
			std::cout << "\n==>> Maximizacao de atendimento de calouros e formandos na fase " << fase << endl;

			// Seta FO para maximizar atendimento de calouros e formandos
			lp->chgObj(nChgFO_FC,idxFO_FC,valFO_FC);
			lp->updateLP();
			
			#ifdef PRINT_LOGS
			std::string lpNameFormandoCalouro;
			lpNameFormandoCalouro += "maxAtendFormandoCalouro" + sf + "_";
			lpNameFormandoCalouro += string(lpName);
			lp->writeProbLP( lpNameFormandoCalouro.c_str() );
			#endif
			
			#pragma region OTIMIZA OU CARREGA SOLUÇÃO PARA FORMANDOS/CALOUROS
			if ( CARREGA_SOL_PARCIAL )
			{
				// procura e carrega solucao parcial
				int statusReadBin = readSolTxt( campusId, prioridade, r, TaticoIntAlunoHor::TAT_INT_CALOURO_BIN, xS, fase );
				if ( !statusReadBin )
				{
					CARREGA_SOL_PARCIAL=false;
				}
				else{
					writeSolTxt( campusId, prioridade, r, TaticoIntAlunoHor::TAT_INT_CALOURO_BIN, xS, fase );
					lp->copyMIPStartSol( lp->getNumCols(), idxN, xS );					
				}								
			}
			if ( !CARREGA_SOL_PARCIAL )
			{
					#ifdef SOLVER_CPLEX
						lp->setNumIntSols(0);

						double runtime = 3600.0;					
						if (fase==1) runtime = (double) this->getTimeLimit(Solver::TAT_INT_CALOURO);
						if (fase==2) runtime = (double) this->getTimeLimit(Solver::TAT_INT_CALOURO);
						if (fase==3) runtime = (double) this->getTimeLimit(Solver::TAT_INT_CALOURO);
						lp->setTimeLimit( runtime );
					
						lp->setPreSolve(OPT_TRUE);
						lp->setHeurFrequency(1.0);
						lp->setMIPEmphasis(0);
						lp->setSymetry(2);
						lp->setCuts(2);
						lp->setPolishAfterNode(30);
						lp->setNodeLimit(1000000);
						lp->setVarSel(0);
						lp->setHeur(10);
						lp->setRins(10);
						lp->setMIPRelTol( 0.0 );
						lp->setMIPScreenLog( 4 );
						lp->setPreSolve(OPT_TRUE);
					#elif defined SOLVER_GUROBI											
						double runtime = 3600.0;
						if (fase==1) runtime = (double) this->getTimeLimit(Solver::TAT_INT_CALOURO);
						if (fase==2) runtime = (double) this->getTimeLimit(Solver::TAT_INT_CALOURO);
						if (fase==3) runtime = (double) this->getTimeLimit(Solver::TAT_INT_CALOURO);
						lp->setTimeLimit( runtime );
						
						lp->setNumIntSols(0);
						lp->setPreSolveIntensity( OPT_LEVEL2 );
						lp->setHeurFrequency(1.0);
						lp->setMIPEmphasis(0);
						lp->setSymetry(2);
						lp->setCuts(0);
						lp->setPolishAfterTime( runtime * 0.66 );
						lp->setNodeLimit(10000000);
						lp->setMIPScreenLog( 4 );
						lp->setMIPRelTol( 0.0 );

						#if defined SOLVER_GUROBI && defined USAR_CALLBACK
						if (fase==1) cb_data.timeLimit = (double) this->getMaxTimeNoImprov(Solver::TAT_INT_CALOURO);
						if (fase==2) cb_data.timeLimit = (double) this->getMaxTimeNoImprov(Solver::TAT_INT_CALOURO);
						if (fase==3) cb_data.timeLimit = (double) this->getMaxTimeNoImprov(Solver::TAT_INT_CALOURO);
						cb_data.gapMax = 10;
						lp->setCallbackFunc( &timeWithoutChangeCallback, &cb_data );
						#endif
					#endif

					lp->copyMIPStartSol( lp->getNumCols(), idxN, xS );
							
					// GENERATES SOLUTION
					std::cout << "\n\nOptimize...\n\n";
					status = lp->optimize( METHOD_MIP );
					lp->getX(xS);							
		
					writeSolBin( campusId, prioridade, r, TaticoIntAlunoHor::TAT_INT_CALOURO_BIN, xS );
					writeSolTxt( campusId, prioridade, r, TaticoIntAlunoHor::TAT_INT_CALOURO_BIN, xS, fase );
			}		
			#pragma endregion
						
			#pragma region FIXA ATENDIMENTO DE CALOUROS/FORMANDOS
			// Fixar o atendimento parcial ( fd_{d,a} = 0 )
			int nroTurmas = 0;
			int nAtendsFC = 0;
			int nAtends = 0;
			int nChgUB = 0;
			
			for ( VariableTatIntHash::iterator vit = vHashTatico.begin(); vit != vHashTatico.end(); vit++ )
			{
				VariableTatInt v = vit->first;
									
				if ( v.getType() == VariableTatInt::V_SLACK_DEMANDA_ALUNO )
				{
					double value = (int)( xS[ vit->second ] + 0.5 );
					double ub = (int)( lp->getUB( vit->second ) + 0.5 );
					double lb = (int)( lp->getLB( vit->second ) + 0.5 );

					if ( value == 0.0 )
					{
						if ( v.getDisciplina()->getId() > 0 )
						{ 
							nAtends++;
							if ( v.getAluno()->ehCalouro() || v.getAluno()->ehFormando() )
								nAtendsFC++;
						}

						if ( lb != ub )
						{
							idxUB[nChgUB] = vit->second;
							valUB[nChgUB] = value;
							btsUB[nChgUB] = BOUNDTYPE::BOUND_UPPER;
							nChgUB++;		
						}
					}
				}
				else if ( v.getType() == VariableTatInt::V_ABERTURA )
				{
					double value = (int)( xS[ vit->second ] + 0.5 );
					if ( value == 1.0 )
						nroTurmas++;
				}
			}
			lp->chgBds( nChgUB, idxUB, btsUB, valUB );			// Fixa fd = 0
			
			std::cout << "\n---> Total de AlunoDemanda calouros+formandos atendidos ao fim da fase " 
				<< fase << ": " << nAtendsFC;
			std::cout << "\n---> Total de AlunoDemanda atendidos ao fim da fase " 
				<< fase << ": " << nAtends;
			std::cout << "\n---> Numero de turmas abertas: "<< nroTurmas <<endl<<endl;
			#pragma endregion
		}

		std::cout << "\n==>> Maximizacao de atendimento de todos os alunos na fase " << fase << endl;
  				
		// Seta FO para maximizar atendimento geral
		lp->chgObj(nChgFO_Geral,idxFO_Geral,valFO_Geral);
		lp->updateLP();
		
		#ifdef PRINT_LOGS
		std::string lpNameGeral;
		lpNameGeral += "maxAtendGeral" + sf + "_";
		lpNameGeral += string(lpName);
		lp->writeProbLP( lpNameGeral.c_str() );
		#endif
						
		#pragma region OTIMIZA OU CARREGA SOLUÇÃO PARA TODOS ALUNOS
		if ( CARREGA_SOL_PARCIAL )
		{
			// procura e carrega solucao parcial
			int statusReadBin = readSolTxt( campusId, prioridade, r, TaticoIntAlunoHor::TAT_INT_BIN2, xS, fase );
			if ( !statusReadBin )
			{
				CARREGA_SOL_PARCIAL=false;
			}
			else{
				writeSolTxt( campusId, prioridade, r, TaticoIntAlunoHor::TAT_INT_BIN2, xS, fase );
				lp->copyMIPStartSol( lp->getNumCols(), idxN, xS );
					
			}								
		}
		if ( !CARREGA_SOL_PARCIAL )
		{
				#ifdef SOLVER_CPLEX
					lp->setNumIntSols(0);

					double runtime = 3600.0;					
					if (fase==1) runtime = (double) this->getTimeLimit(Solver::TAT_INT_M);
					if (fase==2) runtime = (double) this->getTimeLimit(Solver::TAT_INT_T);
					if (fase==3) runtime = (double) this->getTimeLimit(Solver::TAT_INT_N);
					lp->setTimeLimit( runtime );
					
					lp->setPreSolve(OPT_TRUE);
					lp->setHeurFrequency(1.0);
					lp->setMIPEmphasis(0);
					lp->setSymetry(2);
					lp->setCuts(2);
					lp->setPolishAfterNode(30);
					lp->setNodeLimit(1000000);
					lp->setVarSel(0);
					lp->setHeur(10);
					lp->setRins(10);
					lp->setMIPRelTol( 0.0 );
					lp->setMIPScreenLog( 4 );
					lp->setPreSolve(OPT_TRUE);
				#elif defined SOLVER_GUROBI
					double runtime = 3600.0;
					if (fase==1) runtime = (double) this->getTimeLimit(Solver::TAT_INT_M);
					if (fase==2) runtime = (double) this->getTimeLimit(Solver::TAT_INT_T);
					if (fase==3) runtime = (double) this->getTimeLimit(Solver::TAT_INT_N);
					lp->setTimeLimit( runtime );
					
					lp->setNumIntSols(0);
					lp->setPreSolveIntensity( OPT_LEVEL2 );
					lp->setHeurFrequency(1.0);
					lp->setMIPEmphasis(0);
					lp->setSymetry(2);
					lp->setCuts(0);
					lp->setPolishAfterTime( runtime * 0.66 );
					lp->setNodeLimit(10000000);
					lp->setMIPScreenLog( 4 );
					lp->setMIPRelTol( 0.0 );

					#if defined SOLVER_GUROBI && defined USAR_CALLBACK
					if (fase==1) cb_data.timeLimit = (double) this->getMaxTimeNoImprov(Solver::TAT_INT_M);
					if (fase==2) cb_data.timeLimit = (double) this->getMaxTimeNoImprov(Solver::TAT_INT_T);
					if (fase==3) cb_data.timeLimit = (double) this->getMaxTimeNoImprov(Solver::TAT_INT_N);
					cb_data.gapMax = 10;
					lp->setCallbackFunc( &timeWithoutChangeCallback, &cb_data );
					#endif
				#endif

				lp->copyMIPStartSol( lp->getNumCols(), idxN, xS );

		
				// GENERATES SOLUTION
				std::cout << "\n\nOptimize...\n\n";
				status = lp->optimize( METHOD_MIP );
				lp->getX(xS);							
		
				writeSolBin( campusId, prioridade, r, TaticoIntAlunoHor::TAT_INT_BIN2, xS );
				writeSolTxt( campusId, prioridade, r, TaticoIntAlunoHor::TAT_INT_BIN2, xS, fase );

				if ( 0 )
				if ( prioridade==1 )
				if ( fase == 1 || fase == 3 )
					 // TODO: não deveria ser hardcoded, a dificuldade da fase deve ser calculavel de alguma forma
				{
					double runtime = 3600;
					if ( fase == 1 ) runtime = 3600 * 1;
					if ( fase == 3 ) runtime = 3600 * 2;

				// POLISHES THE SOLUTION
				#if !defined (DEBUG) && !defined (TESTE)
				#ifdef SOLVER_CPLEX
					polishTaticoHor(xS, runtime, 80, 20, 1800);
				#elif defined SOLVER_GUROBI								
					lp->setCallbackFunc( NULL, NULL );
					polishTaticoHor(xS, runtime, 80, 20, 1800);
					#if defined SOLVER_GUROBI && defined USAR_CALLBACK
					lp->setCallbackFunc( &timeWithoutChangeCallback, &cb_data );
					lp->updateLP();
					#endif
				#endif
					writeSolBin( campusId, prioridade, r, TaticoIntAlunoHor::TAT_INT_BIN2, xS ); // solucao intermediaria
					writeSolTxt( campusId, prioridade, r, TaticoIntAlunoHor::TAT_INT_BIN2, xS, fase ); // solucao intermediaria
				#endif
				}
		}		
		#pragma endregion

		
		// Volta bounds originais de variaveis x_{i,d,s,t,hi,hf} que foram fixadas como NÃO atendidas.
		lp->chgBds( nChgUB_Fixa0, idxUB_Fixa0, btsUB_Fixa0, valOrigUB_0 );


		#pragma region FIXA ATENDIMENTO
		// Fixar o atendimento parcial ( fd_{d,a} = 0  ;  x_{i,d,s,t,hi,hf} = 1 )
		int nroTurmas = 0;
		int nAtends = 0;
		int nAtendsSemFfd = 0;
		int nAtendsTotal = 0;
		int nChgUB = 0;
		for ( VariableTatIntHash::iterator vit = vHashTatico.begin(); vit != vHashTatico.end(); vit++ )
		{
			VariableTatInt v = vit->first;
									
			if ( v.getType() == VariableTatInt::V_SLACK_DEMANDA_ALUNO )
			{
				double value = (int)( xS[ vit->second ] + 0.5 );
				double ub = (int)( lp->getUB( vit->second ) + 0.5 );
				double lb = (int)( lp->getLB( vit->second ) + 0.5 );

				if ( value == 0.0 )
				{
					nAtendsTotal++;
					if ( lb != ub )
					{
						idxUB[nChgUB] = vit->second;
						valUB[nChgUB] = value;
						btsUB[nChgUB] = BOUNDTYPE::BOUND_UPPER;
						nChgUB++;		
					}
				}
			}
			else if ( v.getType() == VariableTatInt::V_CREDITOS )
			{
				HorarioAula *ha = v.getHorarioAulaInicial();
				int turno = problemData->getFaseDoDia( ha->getInicio() );

				if ( turno <= fase )
				{
					double value = (int)( xS[ vit->second ] + 0.5 );				
					double ub = (int)( lp->getUB( vit->second ) + 0.5 );
					double lb = (int)( lp->getLB( vit->second ) + 0.5 );

					if ( value == 1 )
					if ( lb != ub )
					{
						valOrigLB_Q[nChgLB_Q] = lp->getLB( vit->second );
						idxLB_Q[nChgLB_Q] = vit->second;
						valLB_Q[nChgLB_Q] = value;
						btsLB_Q[nChgLB_Q] = BOUNDTYPE::BOUND_LOWER;
						nChgLB_Q++;
					}
					if ( value == 0 )
					if ( lb != ub )
					{
						valOrigLB_Q[nChgLB_Q] = lp->getUB( vit->second );
						idxLB_Q[nChgLB_Q] = vit->second;
						valLB_Q[nChgLB_Q] = value;
						btsLB_Q[nChgLB_Q] = BOUNDTYPE::BOUND_UPPER;
						nChgLB_Q++;
					}
				}
			}
			else if ( v.getType() == VariableTatInt::V_ABERTURA )
			{
				double value = (int)( xS[ vit->second ] + 0.5 );
				if ( value == 1.0 )
					nroTurmas++;
			}
		}
		lp->chgBds( nChgUB, idxUB, btsUB, valUB );			// Fixa fd = 0
		lp->chgBds( nChgLB_Q, idxLB_Q, btsLB_Q, valLB_Q );	// Fixa x = 1  e  x = 0
		#pragma endregion

		std::cout << "\n---> Total de AlunoDemanda p+t atendidos ao fim da fase " << fase << ": " << nAtendsTotal;
		std::cout << "\n---> Numero de turmas abertas: "<< nroTurmas <<endl<<endl;

		writeGapTxt( campusId, prioridade, r, TaticoIntAlunoHor::TAT_INT_BIN2, lp->getMIPGap() * 100 );
	}


	#ifdef PRINT_LOGS
		std::string lpNameGeralFinal;
		lpNameGeralFinal += "maxAtendGeralFinal_";
		lpNameGeralFinal += string(lpName);
		lp->writeProbLP( lpNameGeralFinal.c_str() );
	#endif
		

	// Volta lower bounds originais de variaveis x_{i,d,s,t,hi,hf}
	lp->chgBds( nChgLB_Q, idxLB_Q, btsLB_Q, valOrigLB_Q );

	// Mantem as fixações fd_{a,d} = 0				  				            
	
	// -------------------------------------------------------------------
	#pragma region Fixa abertura e não-abertura de turmas
	std::cout << "\n--> Fixa as folgas de demanda dos alunos, a abertura e a nao-abertura de turmas." << endl;
	int nChg = 0;
	for ( VariableTatIntHash::iterator vit = vHashTatico.begin(); vit != vHashTatico.end(); vit++ )
	{
		VariableTatInt v = vit->first;
									
		if ( v.getType() == VariableTatInt::V_ABERTURA )
		{
			double value = (int)( xS[ vit->second ] + 0.5 );			
			double ub = (int)( lp->getUB( vit->second ) + 0.5 );
			double lb = (int)( lp->getLB( vit->second ) + 0.5 );

			if ( value == 1 )
			if ( lb != ub )
			{
				idxUB[nChg] = vit->second;
				valUB[nChg] = value;
				btsUB[nChg] = BOUNDTYPE::BOUND_LOWER;
				nChg++;
			}
			if ( value == 0 )
			if ( lb != ub )
			{
				idxUB[nChg] = vit->second;
				valUB[nChg] = value;
				btsUB[nChg] = BOUNDTYPE::BOUND_UPPER;
				nChg++;
			}
		}
	}
	lp->chgBds( nChg, idxUB, btsUB, valUB );
	#pragma endregion

	// -------------------------------------------------------------------
	// Volta com a função objetivo original
	lp->chgObj( lp->getNumCols(),idxN,objOrig );
    
	int cpyStatus = lp->copyMIPStartSol(lp->getNumCols(),idxN,xS);
	std::cout << "\ncopyMIPStartSol = " << cpyStatus;

	lp->updateLP();
	
	std::cout << "\n================================================================================";
	
	delete [] idxUB;
	delete [] valUB;
	delete [] btsUB;
	
	delete[] idxLB_Q;
	delete[] valLB_Q;
	delete[] btsLB_Q;
	delete[] valOrigLB_Q;

	delete[] idxUB_Fixa0;
	delete[] valUB_Fixa0;
	delete[] btsUB_Fixa0;
	delete[] valOrigUB_0;
	
	delete[] idxFO_Geral;
	delete[] valFO_Geral;
	delete[] idxFO_FC;
	delete[] valFO_FC;
	delete[] idxN;
	delete[] objOrig;

	return status;
}


int TaticoIntAlunoHor::solveMaxAtendCalourosFormandos( int campusId, int prioridade, int r, bool& CARREGA_SOL_PARCIAL, double *xS )
{
	std::cout << "\n=========================================";
	std::cout << "\nGarantindo maximo atendimento para calouros e formandos...\n"; fflush(NULL);
		
	int status = 0;

	// -------------------------------------------------------------------
	// Salvando função objetivo original

	int *idxN = new int[lp->getNumCols()];
	double *objOrig = new double[lp->getNumCols()];
	lp->getObj(0,lp->getNumCols()-1,objOrig);
				
	// -------------------------------------------------------------------
	// Modificando coeficientes na função objetivo

	int *idxFO = new int[lp->getNumCols()*2];
	double *valFO = new double[lp->getNumCols()*2];        
	int nChgFO = 0;

	VariableTatIntHash::iterator vit = vHashTatico.begin();
	for ( ; vit != vHashTatico.end(); vit++ )
	{
		VariableTatInt v = vit->first;

		idxN[vit->second] = vit->second;
			
		bool nuloNaFO = true;

		if ( v.getType() == VariableTatInt::V_SLACK_DEMANDA_ALUNO )
		{
			if ( ( problemData->parametros->priorizarCalouros && v.getAluno()->ehCalouro() ) ||
				 ( problemData->parametros->priorizarFormandos && v.getAluno()->ehFormando() ) )
			{
				nuloNaFO = false;

				double coef = v.getDisciplina()->getTotalCreditos() * v.getAluno()->getReceita( v.getDisciplina() );
				idxFO[nChgFO] = vit->second;
				valFO[nChgFO] = coef;
				nChgFO++;
			}
		}
		else if ( v.getType() == VariableTatInt::V_ABERTURA )
		{
			nuloNaFO = false;

			double coef = 1.0;
			idxFO[nChgFO] = vit->second;
			valFO[nChgFO] = coef;
			nChgFO++;
		}

		if ( nuloNaFO )
		{
			idxFO[nChgFO] = vit->second;
			valFO[nChgFO] = 0.0;
			nChgFO++;
		}
    }

	lp->chgObj(nChgFO,idxFO,valFO);
    lp->updateLP();
	  
	// -------------------------------------------------------------------
	
    char lpName[1024];
    strcpy( lpName, this->getTaticoLpFileName( campusId, prioridade, r ).c_str() );

	std::string lpName2;
	lpName2 += "2_";
	lpName2 += string(lpName);

	#ifdef PRINT_LOGS
	lp->writeProbLP( lpName2.c_str() );
	#endif

	#ifdef SOLVER_CPLEX
		lp->setNumIntSols(0);
		lp->setTimeLimit( this->getTimeLimit(Solver::TAT_INT_CALOURO) );
		lp->setPreSolve(OPT_TRUE);
		lp->setHeurFrequency(1.0);
		lp->setMIPEmphasis(0);
		lp->setSymetry(0);
		lp->setCuts(2);
		lp->setPolishAfterNode(30);
		lp->setNodeLimit(1000000);
		lp->setVarSel(0);
		lp->setHeur(10);
		lp->setRins(10);
		lp->setMIPRelTol( 0.0 );
		lp->setMIPScreenLog( 4 );
		lp->setPreSolve(OPT_TRUE);
	#endif

	#ifdef SOLVER_GUROBI
		lp->setNumIntSols(0);
		lp->setTimeLimit( this->getTimeLimit(Solver::TAT_INT_CALOURO) );
		lp->setPreSolveIntensity( OPT_LEVEL1 );
		lp->setHeurFrequency(1.0);
		lp->setMIPEmphasis(0);
		lp->setSymetry(0);
		lp->setCuts(0);
		lp->setPolishAfterTime(3600);
		lp->setNodeLimit(10000000);
		lp->setMIPScreenLog( 4 );
		lp->setMIPRelTol( 0.0 );

		#if defined SOLVER_GUROBI && defined USAR_CALLBACK
		cb_data.timeLimit = this->getMaxTimeNoImprov(Solver::TAT_INT_CALOURO);
		cb_data.gapMax = 70;
		lp->setCallbackFunc( &timeWithoutChangeCallback, &cb_data );			
		#endif
	#endif
		
	if ( CARREGA_SOL_PARCIAL )
	{
		// procura e carrega solucao parcial
		int statusReadBin = readSolBin( campusId, prioridade, r, TaticoIntAlunoHor::TAT_INT_CALOURO_BIN, xS );
		if ( !statusReadBin )
		{
			CARREGA_SOL_PARCIAL=false;
		}
		else writeSolTxt( campusId, prioridade, r, TaticoIntAlunoHor::TAT_INT_CALOURO_BIN, xS, 0 );
	}
	if ( !CARREGA_SOL_PARCIAL )
	{

#if !defined (DEBUG) && !defined (TESTE)
#ifdef SOLVER_CPLEX
		polishTaticoHor(xS, 3600*2, 90, 15, 1800);
#elif defined SOLVER_GUROBI								
		lp->setCallbackFunc( NULL, NULL );
		polishTaticoHor(xS, 3600*3, 90, 10, 1800);
		#if defined SOLVER_GUROBI && defined USAR_CALLBACK
		lp->setCallbackFunc( &timeWithoutChangeCallback, &cb_data );
		#endif
		lp->setTimeLimit( this->getTimeLimit(Solver::TAT_INT_CALOURO) );
#endif
		writeSolBin( campusId, prioridade, r, TaticoIntAlunoHor::TAT_INT_CALOURO_BIN, xS ); // solucao intermediaria
		writeSolTxt( campusId, prioridade, r, TaticoIntAlunoHor::TAT_INT_CALOURO_BIN, xS, 0 ); // solucao intermediaria
#endif
		
		// GENERATES SOLUTION
		status = lp->optimize( METHOD_MIP );
		lp->getX(xS);
						
		writeSolBin( campusId, prioridade, r, TaticoIntAlunoHor::TAT_INT_CALOURO_BIN, xS );
		writeSolTxt( campusId, prioridade, r, TaticoIntAlunoHor::TAT_INT_CALOURO_BIN, xS, 0 );
	}		

	writeGapTxt( campusId, prioridade, r, TaticoIntAlunoHor::TAT_INT_CALOURO_BIN, lp->getMIPGap() * 100 );
	
	// -------------------------------------------------------------------


	std::cout << "\n=========================================";
	std::cout << "\nFixando atendimento de calouros e formandos obtido...\n"; fflush(NULL);

	int *idxUB = new int[lp->getNumCols()];
	double *valUB = new double[lp->getNumCols()];
	BOUNDTYPE *bts = new BOUNDTYPE[lp->getNumCols()];

	int nroCalourosAtendidos=0;
	int nroFormandosAtendidos=0;
	int nroAtendidos=0;
	int nChgUB = 0;	
	for ( vit = vHashTatico.begin(); vit != vHashTatico.end(); vit++ )
	{
		VariableTatInt v = vit->first;
		double upperBound = lp->getUB( vit->second );

		if ( v.getType() == VariableTatInt::V_SLACK_DEMANDA_ALUNO && xS[vit->second] < 0.1 )
		{
			idxUB[nChgUB] = vit->second;
			valUB[nChgUB] = 0.0;
			bts[nChgUB] = BOUNDTYPE::BOUND_UPPER;	
			nChgUB++;
			
			if ( v.getDisciplina()->getId() > 0 )
			{
				nroAtendidos++;
				if ( v.getAluno()->ehCalouro() ) nroCalourosAtendidos++;
				if ( v.getAluno()->ehFormando() ) nroFormandosAtendidos++;
			}
		}
	}

	lp->chgBds(nChgUB,idxUB,bts,valUB);
	  
	std::cout << "\n\nNumero de calouros atendidos: "<< nroCalourosAtendidos;
	std::cout << "\nNumero de formandos atendidos: "<< nroFormandosAtendidos;
	std::cout << "\nNumero total de AlunoDemanda atendido: "<< nroAtendidos <<endl<<endl;
	
	// -------------------------------------------------------------------
	// Volta com a função objetivo original

	lp->chgObj( lp->getNumCols(),idxN,objOrig );
    lp->updateLP();
	

	delete[] idxUB;
	delete[] valUB;
	delete[] bts;
	delete[] idxFO;
	delete[] valFO;
	delete[] idxN;
	delete[] objOrig;

	return status;
}

int TaticoIntAlunoHor::solveMaxAtend( int campusId, int prioridade, int r, bool& CARREGA_SOL_PARCIAL, double *xS )
{
	std::cout << "\n=========================================";
	std::cout << "\nGarantindo maximo atendimento...\n"; fflush(NULL);
		
	int status = 0;

	// -------------------------------------------------------------------
	// Salvando função objetivo original

	double *objN = new double[lp->getNumCols()];
	lp->getObj(0,lp->getNumCols()-1,objN);

	
	// -------------------------------------------------------------------
	// Lp name

    char lpName[1024];
    strcpy( lpName, getTaticoLpFileName( campusId, prioridade, r ).c_str() );


	// -------------------------------------------------------------------
	// FUNÇÃO OBJETIVO SOMENTE COM AS FOLGAS DE DEMANDA 

	int *idxN = new int[lp->getNumCols()];
	int *idxs = new int[lp->getNumCols()*2];
	double *vals = new double[lp->getNumCols()*2];
	BOUNDTYPE *bds = new BOUNDTYPE[lp->getNumCols()*2];
	int nBds = 0;

	#pragma region Modifica FO
    nBds = 0;
	VariableTatIntHash::iterator vit = vHashTatico.begin();
	for ( ; vit != vHashTatico.end(); vit++ )
	{
		idxN[vit->second] = vit->second;

		VariableTatInt v = vit->first;
			
		if ( v.getType() == VariableTatInt::V_SLACK_DEMANDA_ALUNO )
		{            
			idxs[nBds] = vit->second;
			vals[nBds] = 1.0;
			nBds++;
		}
		else if ( v.getType() == VariableTatInt::V_FOLGA_ALUNO_MIN_ATEND1 )	// fmd: folga de mínimo de demanda por aluno
		{
			idxs[nBds] = vit->second;
			vals[nBds] = 1.0;
			nBds++;
		}
		else if ( v.getType() == VariableTatInt::V_FOLGA_ALUNO_MIN_ATEND2 )	// fmd: folga de mínimo de demanda por aluno
		{
			idxs[nBds] = vit->second;
			vals[nBds] = 5.0;
			nBds++;
		}
		else if ( v.getType() == VariableTatInt::V_FOLGA_ALUNO_MIN_ATEND3 )	// fmd: folga de mínimo de demanda por aluno
		{
			idxs[nBds] = vit->second;
			vals[nBds] = 10.0;
			nBds++;
		}
		else
		{
			idxs[nBds] = vit->second;
			vals[nBds] = 0.0;
			nBds++;
		}
	}
	
    lp->chgObj(nBds,idxs,vals);
	#pragma endregion

	
	// ------------------------------------------------------------------------------------

    lp->updateLP();
		
	std::string lpName2;
	lpName2 += "2_";
	lpName2 += string(lpName);

	#ifdef PRINT_LOGS
	lp->writeProbLP( lpName2.c_str() );
	#endif
	
#ifdef SOLVER_CPLEX
		lp->setNumIntSols(100000000);
		lp->setTimeLimit( this->getTimeLimit(Solver::TAT_INT2) );
		lp->setMemoryEmphasis(true);
		lp->setPreSolve(OPT_TRUE);
		lp->setHeurFrequency(1.0);
		lp->setMIPScreenLog( 4 );
		lp->setMIPEmphasis(4);
		lp->setNodeLimit(100000000);
	    //lp->setPolishAfterNode(1);
		lp->setPolishAfterIntSol(100000);
		lp->setPolishAfterTime(1200);
		lp->setPolishAfterNode(1);
		lp->setSymetry(0);
		lp->setProbe(-1);
		lp->setCuts(0);
#endif
#ifdef SOLVER_GUROBI
		lp->setNumIntSols(100000000);
		lp->setTimeLimit( this->getTimeLimit(Solver::TAT_INT2) );
		lp->setPreSolveIntensity(OPT_LEVEL1);
		lp->setMIPScreenLog( 4 );
		lp->setMIPEmphasis(1);
		lp->setPolishAfterTime( this->getTimeLimit(Solver::TAT_INT2) / 2 );
		lp->setSymetry(2);
		lp->setNoCuts();			
		lp->setRins(10);
		lp->setHeurFrequency(0.5);

		#if defined SOLVER_GUROBI && defined USAR_CALLBACK
		cb_data.timeLimit = this->getMaxTimeNoImprov(Solver::TAT_INT2);
		cb_data.gapMax = 40;
		lp->setCallbackFunc( &timeWithoutChangeCallback, &cb_data );
		#endif
#endif

	lp->updateLP();				
	lp->copyMIPStartSol(lp->getNumCols(),idxN,xS);
						

	if ( CARREGA_SOL_PARCIAL )
	{
		// procura e carrega solucao parcial
		int statusReadBin = readSolTxt(campusId, prioridade, r, OutPutFileType::TAT_INT_BIN2, xS, 0 );
		if ( !statusReadBin )
		{
			CARREGA_SOL_PARCIAL=false;
		}
		else writeSolTxt( campusId, prioridade, r, OutPutFileType::TAT_INT_BIN2, xS, 0 );
	}
	if ( !CARREGA_SOL_PARCIAL )
	{
		// GENERATES SOLUTION 		 
			
		status = lp->optimize( METHOD_MIP );
		std::cout<<"\nStatus TAT_INT_BIN2 = "<<status; fflush(NULL);
		lp->getX(xS);

		if ( prioridade==1 && !( !PERMITIR_NOVAS_TURMAS && USAR_EQUIVALENCIA ) ) // Se não for a rodada de inserção com equiv
		{
			#if !defined (DEBUG) && !defined (TESTE)	  
			#ifdef SOLVER_CPLEX
				polishTaticoHor(xS, 3600*2, 90, 15, 3600);
				status = lp->optimize( METHOD_MIP );
				lp->getX(xS);
			#elif defined SOLVER_GUROBI
				lp->setCallbackFunc( NULL, NULL );
				polishTaticoHor(xS, 3600*2, 90, 15, 2200);
				#if defined SOLVER_GUROBI && defined USAR_CALLBACK
				lp->setCallbackFunc( &timeWithoutChangeCallback, &cb_data );			
				#endif
			#endif
			#endif
		}

		writeSolBin( campusId, prioridade, r, OutPutFileType::TAT_INT_BIN2, xS );
		writeSolTxt( campusId, prioridade, r, OutPutFileType::TAT_INT_BIN2, xS, 0 );
	}      
		
	writeGapTxt( campusId, prioridade, r, OutPutFileType::TAT_INT_BIN2, lp->getMIPGap() * 100 );
		
	fflush(NULL);
		

	// ------------------------------------------------------------------------------------
	// FIXA SOLUÇÃO OBTIDA ANTERIORMENTE

	#pragma region Fixa solução
    nBds = 0;
	vit = vHashTatico.begin();
	for ( ; vit != vHashTatico.end(); vit++ )
	{
		VariableTatInt v = vit->first;

		if (  v.getType() == VariableTatInt::V_SLACK_DEMANDA_ALUNO && xS[vit->second] < 0.1 ) // atendido
		{
			idxs[nBds] = vit->second;
			vals[nBds] = 0.0;
			bds[nBds] = BOUNDTYPE::BOUND_UPPER;
			nBds++;
		}
	}

    lp->chgBds(nBds,idxs,bds,vals);
	#pragma endregion

	
	// ------------------------------------------------------------------------------------
	// Volta com a função objetivo original	
	lp->chgObj(lp->getNumCols(),idxN,objN);

	// ------------------------------------------------------------------------------------
	// Copia solução
	int cpyStatus = lp->copyMIPStartSol(lp->getNumCols(),idxN,xS);
	std::cout << "\ncopyMIPStartSol = " << cpyStatus;
 
	lp->updateLP();

	delete[] idxs;
	delete[] vals;
	delete[] bds;
	delete[] idxN;
	delete[] objN;
	
	return status;
}

void TaticoIntAlunoHor::polishTaticoHor(double *xSol, double maxTime, int percIni, int percMin, double maxTempoSemMelhora)
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

   int tempoIter = 500;
   int perc = percIni;
   int *idxs = new int[lp->getNumCols()*2];
   double *vals = new double[lp->getNumCols()*2];
   BOUNDTYPE *bds = new BOUNDTYPE[lp->getNumCols()*2];
   int nBds = 0;

   int fixType = 2;


// ------------------------------------------------------------
// Procura rapidamente a solução exata, caso já se esteja perto do ótimo

//#ifdef SOLVER_CPLEX
//      lp->updateLP();
//      lp->setNodeLimit( 100000000 );
//      lp->setTimeLimit( 1800 );
//      lp->setMIPRelTol( 0.01 );
//      lp->setPolishAfterNode(1);
//      lp->setPolishAfterTime(100000000);
//      lp->setMIPEmphasis( 4 );
//      lp->setNoCuts();
//      lp->setVarSel(4);
//      lp->setNodeLimit(200);
//      lp->setHeurFrequency( 1.0 );
//#endif
//#ifdef SOLVER_GUROBI
//      lp->updateLP();
//      lp->setNodeLimit( 100000000 );
//      lp->setTimeLimit( 1800 );
//      lp->setMIPRelTol( 0.1 );
//      lp->setMIPEmphasis( 0 );
//      lp->setNoCuts();
//      lp->setVarSel(4);
//      lp->setNodeLimit(200);
//      lp->setHeurFrequency( 1.0 );
//      lp->updateLP();
//#endif
//
//      lp->copyMIPStartSol( lp->getNumCols(), idxSol, xSol );
//      lp->updateLP();
//	  
//      status = lp->optimize( METHOD_MIP );
//	  
//      lp->getX( xSol );
//	  
//	  if ( lp->getMIPGap() * 100 < 5.0 )
//	  {
//		  okIter = false;
//		  cout << "\nPolish desnecessario, gap =" << lp->getMIPGap() * 100 << std::endl;
//	  }

// ------------------------------------------------------------


   while (okIter)
   {
      VariableTatIntHash::iterator vit = vHashTatico.begin();

      // Seleciona turmas e disciplinas para fixar
      std::set<std::pair<int,Disciplina*> > paraFixarUm;
      std::set<std::pair<int,Disciplina*> > paraFixarZero;
      std::map<Trio<int,Disciplina*,int>,int > paraFixarDia;

      if ( fixType == 1 )
      {
         nBds = 0;
         while ( vit != vHashTatico.end() )
         {
            if ( vit->first.getType() == VariableTatInt::V_ABERTURA )
            {
               if ( rand() % 100 >= perc  )
               {
                  vit++;
                  continue;
               }

               if (xSol[vit->second] > 0.1 )
               {
                  idxs[nBds] = vit->second;
                  vals[nBds] = (int)(xSol[vit->second]+0.5);
                  bds[nBds] = BOUNDTYPE::BOUND_LOWER;
                  nBds++;
                  std::pair<int,Disciplina*> auxPair(vit->first.getTurma(),vit->first.getDisciplina());
                  paraFixarUm.insert(auxPair);
               }
               else
               {
                  idxs[nBds] = vit->second;
                  vals[nBds] = 0.0;
                  bds[nBds] = BOUNDTYPE::BOUND_UPPER;
                  nBds++;
                  std::pair<int,Disciplina*> auxPair(vit->first.getTurma(),vit->first.getDisciplina());
                  paraFixarZero.insert(auxPair);
               }
            }

            vit++;
         }
         lp->chgBds(nBds,idxs,bds,vals);
         lp->updateLP();
      }

      vit = vHashTatico.begin();

      nBds = 0;
      while ( vit != vHashTatico.end() )
      {
         if ( vit->first.getType() == VariableTatInt::V_CREDITOS )
         {
            if ( fixType == 1 )
            {
               std::pair<int,Disciplina*> auxPair(vit->first.getTurma(),vit->first.getDisciplina());
               if ( paraFixarUm.find(auxPair) != paraFixarUm.end() )
               {
                  if ( xSol[vit->second] > 0.1 )
                  {
                     //lp->chgLB(vit->second,(int)(xSol[vit->second]+0.5));
                     //lp->chgUB(vit->second,(int)(xSol[vit->second]+0.5));
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
                     //lp->chgUB(vit->second,0.0);
                  }
               }
               else if ( paraFixarZero.find(auxPair) != paraFixarZero.end() )
               {
                  idxs[nBds] = vit->second;
                  vals[nBds] = 0.0;
                  bds[nBds] = BOUNDTYPE::BOUND_UPPER;
                  nBds++;
                  //lp->chgUB(vit->second,0.0);
               }
            }
            else
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
                  //lp->chgLB(vit->second,(int)(xSol[vit->second]+0.5));
                  //lp->chgUB(vit->second,(int)(xSol[vit->second]+0.5));
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
      //lp->setCuts(1);
      //if ( perc < percIni )
      //   lp->setCuts(1);
      //lp->setNoCuts();
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

      lp->getX( xSol );
	  
      double objN = lp->getObjVal();

      if ( fabs(objN-objAtual) < 0.0001 && perc < percMin )
      {
         if ( fixType == 2 )
         {
            perc = (percIni + percMin ) /2;
            fixType = 2;
            tempoIter = 500;
         }
         else
         {
            perc = (percIni + percMin ) /2;
            fixType = 2;
            tempoIter = 500;
         }
      }
      else if ( fabs(objN-objAtual) < 0.0001 && perc >= percMin)
      {
         if ( fixType == 2 )
         {
            perc -= 5;
            lp->setCuts(1);
            tempoIter += 50;
         }
         else
         {
            perc -= 5;
            lp->setCuts(1);
            tempoIter += 50;
         }
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
			 cout << "\nTempo maximo atingido: " << tempoAtual << "s, maximo:" << maxTime << endl;
		  }
	  }
	  
      // Volta bounds
      vit = vHashTatico.begin();

      nBds = 0;
      while ( vit != vHashTatico.end() )
      {
         if ( vit->first.getType() == VariableTatInt::V_CREDITOS || vit->first.getType() == VariableTatInt::V_ABERTURA )
         {
            idxs[nBds] = vit->second;
            vals[nBds] = ubVars[vit->second];
            bds[nBds] = BOUNDTYPE::BOUND_UPPER;
            nBds++;
            idxs[nBds] = vit->second;
            vals[nBds] = lbVars[vit->second];
            bds[nBds] = BOUNDTYPE::BOUND_LOWER;
            nBds++;
            //lp->chgLB(vit->second,lbVars[vit->second]);
            //lp->chgUB(vit->second,ubVars[vit->second]);
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


Unidade* TaticoIntAlunoHor::retornaUnidadeDeAtendimento( int turma, Disciplina* disciplina, Campus* campus )
{
	ITERA_GGROUP_LESSPTR( itSol, (*this->solVarsTatico), VariableTatico )
	{
		VariableTatico *v = *itSol;

		if ( v->getType() == VariableTatico::V_CREDITOS )
		{
			if ( v->getTurma() == turma &&
				 v->getDisciplina()->getId() == disciplina->getId() &&
				 v->getUnidade()->getIdCampus() == campus->getId() )
			{
				return v->getUnidade();
			}
		}
	}
	
	return NULL;
}

ConjuntoSala* TaticoIntAlunoHor::retornaSalaDeAtendimento( int turma, Disciplina* disciplina, Campus* campus )
{
	ITERA_GGROUP_LESSPTR( itSol, (*this->solVarsTatico), VariableTatico )
	{
		VariableTatico *v = *itSol;

		if ( v->getType() == VariableTatico::V_CREDITOS )
		{
			if ( v->getTurma() == turma &&
				 v->getDisciplina()->getId() == disciplina->getId() &&
				 v->getUnidade()->getIdCampus() == campus->getId() )
			{
				return v->getSubCjtSala();
			}
		}
	}
	
	return NULL;
}

GGroup< VariableTatico *, LessPtr<VariableTatico> > TaticoIntAlunoHor::retornaAulasEmVarX( int turma, Disciplina* disciplina, int campusId )
{
	GGroup< VariableTatico *, LessPtr<VariableTatico> > aulasX;

	ITERA_GGROUP_LESSPTR( itSol, (*this->vars_xh), VariableTatico )
	{
		VariableTatico *v = *itSol;

		if ( v->getTurma() == turma &&
			 v->getDisciplina()->getId() == disciplina->getId() &&
			 v->getUnidade()->getIdCampus() == campusId )
		{
			aulasX.add( v );
		}		
	}
	
	return aulasX;
}

GGroup< VariableTatInt *, LessPtr<VariableTatInt> > TaticoIntAlunoHor::retornaAulasEmVarXFinal( int turma, Disciplina* disciplina, int campusId )
{
	GGroup< VariableTatInt *, LessPtr<VariableTatInt> > aulasX;

	ITERA_GGROUP_LESSPTR( itSol, this->solVarsTatInt, VariableTatInt )
	{
		VariableTatInt *x = *itSol;
		if ( x->getType() == VariableTatInt::V_CREDITOS )
		{
			if ( x->getTurma() == turma &&
				 x->getDisciplina()->getId() == disciplina->getId() &&
				 x->getUnidade()->getIdCampus() == campusId )
			{
				aulasX.add( x );
			}
		}		
	}
	
	return aulasX;
}

GGroup< VariableTatInt *, LessPtr<VariableTatInt> > 
	TaticoIntAlunoHor::retornaAulasEmVarV( Aluno* aluno, GGroup< VariableTatInt *, LessPtr<VariableTatInt> > aulasX )
{
	GGroup< VariableTatInt *, LessPtr<VariableTatInt> > aulasV;

	ITERA_GGROUP_LESSPTR( itSol, this->solVarsTatInt, VariableTatInt )
	{
		VariableTatInt *v = *itSol;
		if ( v->getType() == VariableTatInt::V_ALUNO_CREDITOS )
		{
			if ( v->getAluno() == aluno )
			{
				ITERA_GGROUP_LESSPTR( itX, aulasX, VariableTatInt )
				{
					if ( itX->getTurma() == v->getTurma() &&
						 itX->getDisciplina() == v->getDisciplina() &&
						 itX->getUnidade() == v->getUnidade() &&
						 itX->getSubCjtSala() == v->getSubCjtSala() &&
						 itX->getDia() == v->getDia() &&
						 itX->getDateTimeInicial() == v->getDateTimeInicial() &&
						 itX->getDateTimeFinal() == v->getDateTimeFinal() )
						 //itX->getHorarioAulaInicial() == v->getHorarioAulaInicial() &&
						 //itX->getHorarioAulaFinal() == v->getHorarioAulaFinal() )
					{
						aulasV.add( v ); break;
					}
				}
			}
		}		
	}
	
	return aulasV;
}

bool TaticoIntAlunoHor::SolVarsFound( VariableTatico v )
{	
	GGroup< VariableTatico *, LessPtr<VariableTatico> >::iterator itSol = this->solVarsTatico->find(&v);

	if(itSol != this->solVarsTatico->end())
		return true;
	else
		return false;
}

bool TaticoIntAlunoHor::criaVariavelTaticoInt( VariableTatInt *v, bool &fixar, int prioridade )
{	
	VariableTatico vSol;
		   
	switch( v->getType() )
	{
		
		 case VariableTatInt::V_ERROR:
		 {
			 return true;
		 }		 
		 case VariableTatInt::V_ALUNO_CREDITOS:  //  v_{a,i,d,u,s,hi,hf,t} 
		 {
			 // Testa direto
			 if ( !CRIAR_VARS_FIXADAS && CRIANDO_V_ATRAVES_DE_X )
			 {
 				 int turmaDoAluno = problemData->retornaTurmaDiscAluno( v->getAluno(), v->getDisciplina() );
				 if ( turmaDoAluno != -1 )
				 {					
					 // aluno alocado: não cria ( !CRIAR_VARS_FIXADAS)
					 std::cout<<"\nEstranho: o mapDiscAlunosDemanda deveria ter somente alunos nao atendidos."
						 <<" Aluno" << v->getAluno()->getAlunoId() <<" DISC"<< v->getDisciplina()->getId() <<" Turma"<< turmaDoAluno;
					return false;
				 }

				 Aluno *aluno = v->getAluno();
				 int dia = v->getDia();
				 HorarioAula *hi = v->getHorarioAulaInicial();
				 HorarioAula *hf = v->getHorarioAulaFinal();

				 AlunoDemanda *alDem = aluno->getAlunoDemandaEquiv( v->getDisciplina() );
				 if ( alDem == NULL ){ 
					 std::cout<<"\nERRO ao criar var v: AlunoDemanda nao encontrado para disc " 
						 << v->getDisciplina()->getId()
						 << " e aluno " << aluno->getAlunoId();
					 return false;
				 }

				 #ifdef ALUNO_UNICO_CALENDARIO
				 if ( !( aluno->getCalendario( alDem->demanda )->possuiHorarioDiaOuCorrespondente( hi, hf, dia ) ) )
					 return false;
				 #endif
				 
					//	std::cout << "\n\t\tTestando " << v->toString();

				 #ifdef ALUNO_TURNOS_DA_DEMANDA				 
				 if ( ! alDem->getOferta()->turno->possuiHorarioDiaOuCorrespondente( hi, hf, dia ) )
					 return false;
				 #endif
				 
					//	std::cout << "\tValida no turno!";


				// Verifica os horários já alocados do aluno.
				// Se houver sobreposição com os horários da variável v, não permite a criação da mesma.
				if ( aluno->sobrepoeAulaJaAlocada( hi, hf, dia ) )
					return false;

				//		std::cout << "\tValida no aluno!";

				return true;
			 }
			 
			 if ( PERMITIR_REALOCAR_ALUNO && CRIAR_VARS_FIXADAS && CRIANDO_V_ATRAVES_DE_X )
			 {
				 fixar = false;
				 return true;
			 }

			 int turmaDoAluno = problemData->retornaTurmaDiscAluno( v->getAluno(), v->getDisciplina() );

			 if ( !PERMITIR_REALOCAR_ALUNO && CRIAR_VARS_FIXADAS && CRIANDO_V_ATRAVES_DE_X )
			 {
				 if ( turmaDoAluno != v->getTurma() && turmaDoAluno != -1 )
				 {
					 // aluno alocado, mas em outra turma: NÃO CRIA A VARIAVEL
					 return false;
				 }
				 if ( turmaDoAluno == v->getTurma() )
				 {
					 // aluno alocado na turma: CRIA A VARIAVEL fixada
					 fixar=true;
					 return true;
				 }
			 }

			 if ( turmaDoAluno != v->getTurma() && turmaDoAluno != -1 )
			 {
				 // aluno alocado, mas em outra turma: NÃO CRIA A VARIAVEL
				 return false;
			 }
			 			 
			 // aluno alocado na turma corrente
			 if ( turmaDoAluno == v->getTurma() )
			 {
				 // aluno alocado nesta turma: FIXA A VARIAVEL, caso ela exista. Se não, nem cria.

				 if ( !CRIAR_VARS_FIXADAS ) return false;

				 vSol.reset();
				 vSol.setType( VariableTatico::V_CREDITOS ); // x_{i,d,u,s,hi,hf,t}
				 vSol.setTurma( v->getTurma() );
				 vSol.setDisciplina( v->getDisciplina() );
				 vSol.setUnidade( v->getUnidade() );
				 vSol.setSubCjtSala( v->getSubCjtSala() );
				 vSol.setDia( v->getDia() );								 
				 vSol.setHorarioAulaInicial( v->getHorarioAulaInicial() );	 
				 vSol.setHorarioAulaFinal( v->getHorarioAulaFinal() );
				 vSol.setDateTimeInicial( v->getDateTimeInicial() );	 
				 vSol.setDateTimeFinal( v->getDateTimeFinal() );

				 if ( SolVarsFound(vSol) )
				 {		
					fixar=true;
					return (true && CRIAR_VARS_FIXADAS);
				 }
				 else
				 {					
					return false;				 
				 }
			 }	

			 // aluno não alocado
			 if ( turmaDoAluno == -1 )
			 {
				 Aluno *aluno = v->getAluno();
				 int turma = v->getTurma();
				 Disciplina *disciplina = v->getDisciplina();
				 int campusId = v->getUnidade()->getIdCampus();
				 int dia = v->getDia();
				 HorarioAula *hi = v->getHorarioAulaInicial();
				 HorarioAula *hf = v->getHorarioAulaFinal();

				 AlunoDemanda *alDem = aluno->getAlunoDemandaEquiv( disciplina );
				 if ( alDem == NULL ){ 
					 std::cout<<"\nERRO ao criar var v: AlunoDemanda nao encontrado para disc " 
						 << v->getDisciplina()->getId()
						 << " e aluno " << aluno->getAlunoId();
					 return false;
				 }

				 #ifdef ALUNO_UNICO_CALENDARIO
				 if ( !( aluno->getCalendario( alDem->demanda )->possuiHorarioDiaOuCorrespondente( hi, hf, dia ) ) )
					 return false;
				 #endif

				 #ifdef ALUNO_TURNOS_DA_DEMANDA			 
				 if ( ! alDem->getOferta()->turno->possuiHorarioDiaOuCorrespondente( hi, hf, dia ) )
					 return false;
				 #endif

				 // aluno não alocado: cria a variavel livre se 'x' e 'z' existirem;
				 // aluno não alocado: não cria a variavel se 'x' não existir e 'z' existir;				 
				 // aluno não alocado: cria a variavel livre se nem 'x' nem 'z' existirem;
				 
				 vSol.reset();
				 vSol.setType( VariableTatico::V_CREDITOS ); // x_{i,d,u,s,hi,hf,t}
				 vSol.setTurma( turma );
				 vSol.setDisciplina( disciplina );
				 vSol.setUnidade( v->getUnidade() );
				 vSol.setSubCjtSala( v->getSubCjtSala() );
				 vSol.setDia( v->getDia() );								 
				 vSol.setHorarioAulaInicial( hi );	 
				 vSol.setHorarioAulaFinal( hf );
				 vSol.setDateTimeInicial( v->getDateTimeInicial() );	 
				 vSol.setDateTimeFinal( v->getDateTimeFinal() );
				 
				 if ( SolVarsFound(vSol) ) // existe 'x'
				 {	
					 // Verifica todas as aulas da turma (outros dias).
					 // Se houver sobreposição com horários já alocados do aluno, não criar a variável.

					 GGroup< VariableTatico *, LessPtr<VariableTatico> > aulasX = this->retornaAulasEmVarX( turma, disciplina, campusId );
					 ITERA_GGROUP_LESSPTR( itX, aulasX, VariableTatico )
					 {
						 int diaX = itX->getDia();
						 HorarioAula *hiX = itX->getHorarioAulaInicial();
						 HorarioAula *hfX = itX->getHorarioAulaFinal();
						 if ( aluno->sobrepoeAulaJaAlocada( hiX, hfX, diaX ) )
							 return false;
					 }

					 // aluno não alocado e com horários disponíveis.
					 // Cria a variavel LIVRE;

					 fixar=false;
					 return true;
				 }
				 else // não existe 'x'
				 {
					 // Verifica os horários já alocados do aluno, independente da turma já existir ou não.
					 // Se houver sobreposição com os horários da variável v, não permite a criação da mesma.
					 if ( aluno->sobrepoeAulaJaAlocada( hi, hf, dia ) )
						 return false;

					 if ( CRIANDO_V_ATRAVES_DE_X )
						 return true;

					 if ( problemData->existeTurmaDiscCampus( turma, disciplina->getId(), campusId ) )
					 {
						 // aluno não alocado: NÃO CRIA a variavel se 'x' não existir e 'z' existir;	
					 	 return false;
					 }
					 else // não existe 'z' (turma nova)
					 {
						if ( permitirAbertura( turma, disciplina, campusId ) )
						{
							// Verifica os horários já alocados da sala.
							// Se houver sobreposição com os horários da variável v, não permite a criação da mesma.
							Sala *sala = v->getSubCjtSala()->salas.begin()->second;
							if ( ! sala->ehViavelNaSala( disciplina, hi, hf, dia ) )
								return false;
							 
							// aluno não alocado: cria a variavel LIVRE se nem 'x' nem 'z' existirem;
							fixar=false;
							return true;							
						}
						else return false;
					 }
				 }
			 }
			 break;
		 }
		 case VariableTatInt::V_CREDITOS:  //  x_{i,d,u,s,hi,hf,t} 
		 {
			 vSol.reset();
			 vSol.setType( VariableTatico::V_CREDITOS ); // x_{i,d,u,s,hi,hf,t}
			 vSol.setTurma( v->getTurma() );
			 vSol.setDisciplina( v->getDisciplina() );
			 vSol.setUnidade( v->getUnidade() );
			 vSol.setSubCjtSala( v->getSubCjtSala() );
			 vSol.setDia( v->getDia() );								 
			 vSol.setHorarioAulaInicial( v->getHorarioAulaInicial() );	 
			 vSol.setHorarioAulaFinal( v->getHorarioAulaFinal() );
			 vSol.setDateTimeInicial( v->getDateTimeInicial() );	 
			 vSol.setDateTimeFinal( v->getDateTimeFinal() );

			 if ( SolVarsFound(vSol) )
			 {
				fixar=true;
				return (true && CRIAR_VARS_FIXADAS);
			 }
			 else // se não existe 'x', só cria se for pra turmas novas
			 {
				int campusId = v->getUnidade()->getIdCampus();

				if ( problemData->existeTurmaDiscCampus( v->getTurma(), v->getDisciplina()->getId(), campusId ) )
				{
					return false;
				}
				else // turma nova
				{
					if ( permitirAbertura( v->getTurma(), v->getDisciplina(), campusId ) )
					{
						// Verifica os horários já alocados da sala.
						// Se houver sobreposição com os horários da variável v, não permite a criação da mesma.
						Sala *sala = v->getSubCjtSala()->salas.begin()->second;
						int dia = v->getDia();
						HorarioAula *hi = v->getHorarioAulaInicial();
						HorarioAula *hf = v->getHorarioAulaFinal();
						
						if ( ! sala->ehViavelNaSala( v->getDisciplina(), hi, hf, dia ) )
							return false;
							 
						// turma nova: cria a variavel LIVRE se nem 'x' nem 'z' existirem;
						fixar=false;
						return true;							
					}
					else return false;
				}
			 }
			 break;
		 }
		 case VariableTatInt::V_OFERECIMENTO:  //  o_{i,d,u,s} 
		 {
			 ConjuntoSala* cjtSala = this->retornaSalaDeAtendimento( v->getTurma(), v->getDisciplina(), 
										problemData->refCampus[v->getUnidade()->getIdCampus()] );

			 if ( cjtSala == v->getSubCjtSala() ) // se achou: cria fixo
			 {
				 fixar=true;
				 return (true && CRIAR_VARS_FIXADAS);
			 }
			 else // se não existe 'x', só cria se for pra turmas novas
			 {
				if ( problemData->existeTurmaDiscCampus( v->getTurma(), v->getDisciplina()->getId(), v->getUnidade()->getIdCampus() ) )
				{
					return false;
				}
				else // turma nova
				{
					if ( permitirAbertura( v->getTurma(), v->getDisciplina(), v->getUnidade()->getIdCampus() ) )
					{
						// turma nova: cria a variavel LIVRE se nem 'x' nem 'z' existirem;
						fixar=false;
						return true;							
					}
					else return false;				 
				}
			 }
			 break;
		 }
		 case VariableTatInt::V_ALOCA_ALUNO_TURMA_DISC:  //  s_{i,d,a} 
		 {
 			 int turma = problemData->retornaTurmaDiscAluno( v->getAluno(), v->getDisciplina() );
			 bool alocadoEmOutraTurma = (turma!=v->getTurma() && turma!=-1);
			 bool alocadoNaTurma = (turma==v->getTurma() && turma!=-1);
			 bool alocadoNaoAlocado = (turma==-1);

			 if ( problemData->existeSolTaticoInicial() )
			 {
				fixar=false;
				bool fixoSolIni=false;
				int turmaInicialAluno = problemData->getSolTaticoInicial()->getTurma( 
					v->getAluno(), v->getCampus()->getId(), v->getDisciplina(), fixoSolIni );

				// Se o aluno estiver fixo na solução inicial em turma diferente, não cria.
				if ( v->getTurma() != turmaInicialAluno && fixoSolIni )
					return false;
			 }

			 if ( alocadoEmOutraTurma && !PERMITIR_REALOCAR_ALUNO )
			 {
				 return false;
			 }			 
			 if ( alocadoEmOutraTurma && PERMITIR_REALOCAR_ALUNO )
			 {
				return true;
			 }
			 if ( alocadoNaTurma )
			 {
				// aluno alocado nesta turma: cria fixada, a não ser que seja permitida realocação
				fixar = true && !PERMITIR_REALOCAR_ALUNO;
				return (true && CRIAR_VARS_FIXADAS);				
			 }			 
			 if ( alocadoNaoAlocado ) // aluno não alocado: cria livre
			 {
				 if ( permitirAbertura( v->getTurma(), v->getDisciplina(), v->getCampus()->getId() ) )
				{
					// aluno não alocado: cria a variavel LIVRE;
					fixar=false;
					return true;							
				}
				else return false;
			 }
			 break;
		 }
		 case VariableTatInt::V_DIAS_CONSECUTIVOS: // c_{i,d,t}
		 {
			 vSol.reset();
			 vSol.setType( VariableTatico::V_DIAS_CONSECUTIVOS ); // c_{i,d,t}
			 vSol.setTurma( v->getTurma() );
			 vSol.setDisciplina( v->getDisciplina() );
			 vSol.setDia( v->getDia() );
			 vSol.setCampus( v->getCampus() );

			 if ( SolVarsFound(vSol) )
			 {
				fixar=true;
				return (true && CRIAR_VARS_FIXADAS);
			 }
			 else // se não existe 'c', só cria se for pra turmas novas
			 {
				if ( problemData->existeTurmaDiscCampus( v->getTurma(), v->getDisciplina()->getId(), v->getCampus()->getId() ) )
				{
					return false;
				}
				else // turma nova
				{
					if ( permitirAbertura( v->getTurma(), v->getDisciplina(), v->getCampus()->getId() ) )
					{
						fixar=false;
						return true;							
					}
					else return false;				 
				}
			 }
			 break;
		 }
		 case VariableTatInt::V_SLACK_DIST_CRED_DIA_SUPERIOR: // fcp_{i,d,s,t}
		 {
			 vSol.reset();
			 vSol.setType( VariableTatico::V_SLACK_DIST_CRED_DIA_SUPERIOR );	// fcp_{i,d,s,t}
			 vSol.setTurma( v->getTurma() );
			 vSol.setDisciplina( v->getDisciplina() );
			 vSol.setUnidade( v->getUnidade() );
			 vSol.setSubCjtSala( v->getSubCjtSala() );
			 vSol.setDia( v->getDia() );

			 if ( SolVarsFound(vSol) )
			 {
				fixar=true;
				return (true && CRIAR_VARS_FIXADAS);
			 }
			 else // se não existe 'fcp', só cria se for pra turmas novas
			 {
				if ( problemData->existeTurmaDiscCampus( v->getTurma(), v->getDisciplina()->getId(), v->getUnidade()->getIdCampus() ) )
				{
					return false;
				}
				else // turma nova
				{
					if ( permitirAbertura( v->getTurma(), v->getDisciplina(), v->getUnidade()->getIdCampus() ) )
					{
						fixar=false;
						return true;							
					}
					else return false;				 
				}
			 }
			 break;
		 }
		 case VariableTatInt::V_SLACK_DIST_CRED_DIA_INFERIOR: // fcm_{i,d,s,t}
		 {
			 vSol.reset();
			 vSol.setType( VariableTatico::V_SLACK_DIST_CRED_DIA_INFERIOR );	// fcm_{i,d,s,t}
			 vSol.setTurma( v->getTurma() );
			 vSol.setDisciplina( v->getDisciplina() );
			 vSol.setUnidade( v->getUnidade() );
			 vSol.setSubCjtSala( v->getSubCjtSala() );
			 vSol.setDia( v->getDia() );

			 if ( SolVarsFound(vSol) )
			 {
				fixar=true;
				return (true && CRIAR_VARS_FIXADAS);
			 }
			 else // se não existe 'fcp', só cria se for pra turmas novas
			 {
				if ( problemData->existeTurmaDiscCampus( v->getTurma(), v->getDisciplina()->getId(), v->getUnidade()->getIdCampus() ) )
				{
					return false;
				}
				else // turma nova
				{
					if ( permitirAbertura( v->getTurma(), v->getDisciplina(), v->getUnidade()->getIdCampus() ) )
					{
						fixar=false;
						return true;							
					}
					else return false;				 
				}
			 }
			 break;
		 }
		 case VariableTatInt::V_SLACK_DEMANDA_ALUNO: // fd_{d,a}
		 {
			 // não permite desalocar o que já está fixo.

			 if ( problemData->retornaTurmaDiscAluno( v->getAluno(), v->getDisciplina() ) != -1 )
			 	return false;

			 fixar=false;
			 return true;	
			 break;
		 }
		 case VariableTatInt::V_COMBINACAO_DIVISAO_CREDITO: // m{i,d,k}
		 {
			 vSol.reset();
			 vSol.setType( VariableTatico::V_COMBINACAO_DIVISAO_CREDITO ); // m_{i,d,k}
			 vSol.setTurma( v->getTurma() );
			 vSol.setDisciplina( v->getDisciplina() );
			 vSol.setK( v->getK() );

			 if ( SolVarsFound(vSol) )
			 {
				fixar=true;
				return (true && CRIAR_VARS_FIXADAS);
			 }
			 else // se não existe 'm', só cria se for pra turmas novas
			 {
				if ( problemData->existeTurmaDiscCampus( v->getTurma(), v->getDisciplina()->getId(), v->getCampus()->getId() ) )
				{
					return false;
				}
				else // turma nova
				{
					if ( permitirAbertura( v->getTurma(), v->getDisciplina(), v->getCampus()->getId() ) )
					{
						fixar=false;
						return true;							
					}
					else return false;				 
				}
			 }
			 break;
		 }
		 case VariableTatInt::V_SLACK_COMBINACAO_DIVISAO_CREDITO_M: // fkm{i,d,t}
		 {
			 vSol.reset();
			 vSol.setType( VariableTatico::V_SLACK_COMBINACAO_DIVISAO_CREDITO_M ); // fkm_{i,d,t}
			 vSol.setTurma( v->getTurma() );
			 vSol.setDisciplina( v->getDisciplina() );
			 vSol.setDia( v->getDia() );

			 if ( SolVarsFound(vSol) )
			 {
				fixar=true;
				return (true && CRIAR_VARS_FIXADAS);
			 }
			 else // se não existe 'fkm', só cria se for pra turmas novas
			 {
				if ( problemData->existeTurmaDiscCampus( v->getTurma(), v->getDisciplina()->getId(), v->getCampus()->getId() ) )
				{
					return false;
				}
				else // turma nova
				{
					if ( permitirAbertura( v->getTurma(), v->getDisciplina(), v->getCampus()->getId() ) )
					{
						fixar=false;
						return true;							
					}
					else return false;					 
				}
			 }
			 break;
		 }
		 case VariableTatInt::V_SLACK_COMBINACAO_DIVISAO_CREDITO_P: // fkp{i,d,t}
		 {
			 vSol.reset();
			 vSol.setType( VariableTatico::V_SLACK_COMBINACAO_DIVISAO_CREDITO_P ); // fkp_{i,d,t}
			 vSol.setTurma( v->getTurma() );
			 vSol.setDisciplina( v->getDisciplina() );
			 vSol.setDia( v->getDia() );

			 if ( SolVarsFound(vSol) )
			 {
				fixar=true;
				return (true && CRIAR_VARS_FIXADAS);
			 }
			 else // se não existe 'fkp', só cria se for pra turmas novas
			 {
				if ( problemData->existeTurmaDiscCampus( v->getTurma(), v->getDisciplina()->getId(), v->getCampus()->getId() ) )
				{
					return false;
				}
				else // turma nova
				{
					if ( permitirAbertura( v->getTurma(), v->getDisciplina(), v->getCampus()->getId() ) )
					{
						fixar=false;
						return true;							
					}
					else return false;				 
				}
			 }
			 break;
		 }
		 case VariableTatInt::V_ABERTURA_COMPATIVEL: // zc_{d,t,cp}
		 {
			 vSol.reset();
			 vSol.setType( VariableTatico::V_ABERTURA_COMPATIVEL ); // zc_{d,t,cp}
			 vSol.setDisciplina( v->getDisciplina() );
			 vSol.setCampus( v->getCampus() );
			 vSol.setDia( v->getDia() );

			 if ( SolVarsFound(vSol) )
			 {
				fixar=true;
				return (true && CRIAR_VARS_FIXADAS);
			 }
			 else // turma nova
			 {
				 int folga = haFolgaDeAtendimento( v->getDisciplina() );
				 if ( problemData->parametros->considerar_equivalencia_por_aluno && this->USAR_EQUIVALENCIA )
				 {
					ITERA_GGROUP_LESSPTR( itDiscEquiv, v->getDisciplina()->discEquivalentes, Disciplina ) // possiveis substituidas!
						folga += this->haFolgaDeAtendimento( *itDiscEquiv );
				 }

				 if ( folga )
				 {
					fixar=false;
					return true;					 
				 }
				 return false;
			 }
			 break;
		 }		 		 
		 case VariableTatInt::V_FORMANDOS_NA_TURMA: // f_{i,d,cp}
		 {
			vSol.reset();
			vSol.setType( VariableTatico::V_ABERTURA ); // z_{i,d,cp}
			vSol.setTurma( v->getTurma() );
			vSol.setDisciplina( v->getDisciplina() );
			vSol.setCampus( v->getCampus() );

			if ( SolVarsFound(vSol) ) // existe a turma (variavel z)
			{
				return true;
			}
			else
			{
				if ( problemData->haAlunoFormandoNaoAlocado( v->getDisciplina(), v->getCampus()->getId(), prioridade ) &&
					 permitirAbertura( v->getTurma(), v->getDisciplina(), v->getCampus()->getId() ) )
				{
					fixar=false;
					return true;
				}
				else return false;				
			}
		 } 	
		 case VariableTatInt::V_ABERTURA: // z_{i,d,cp}
		 {
			 if ( problemData->existeTurmaDiscCampus( v->getTurma(), v->getDisciplina()->getId(), v->getCampus()->getId() ) )
			 {
				fixar = true;
				return (true && CRIAR_VARS_FIXADAS);
			 }
			 else
			 {
				if ( permitirAbertura( v->getTurma(), v->getDisciplina(), v->getCampus()->getId() ) )
				{
					fixar=false;
					return true;
				}
				else return false;				
			 }
		 } 	
		 case VariableTatInt::V_TURMA_ATEND_CURSO: // b_{i,d,c}
		 {	
			 if ( problemData->existeTurmaDiscCampus( v->getTurma(), v->getDisciplina()->getId(), v->getCampus()->getId() ) )
			 {
				 if ( problemData->atendeCursoTurmaDisc( v->getTurma(), v->getDisciplina(), v->getCampus()->getId(), v->getCurso()->getId() ) )
				 {
					fixar = true;
					return (true && CRIAR_VARS_FIXADAS);				 
				 }
				 else
				 {
					fixar = false;
					return true;						
				 }
			 }
			 else
			 {
				 if ( permitirAbertura( v->getTurma(), v->getDisciplina(), v->getCampus()->getId() ) )
				 {
					fixar = false;
					return true;
				 }
				 else return false;
			 }
		 }
		 case VariableTatInt::V_FOLGA_OCUPA_SALA:  //  fos_{i,d,cp} 
		 {		
			 if ( permitirAbertura( v->getTurma(), v->getDisciplina(), v->getCampus()->getId() ) )
			 {
				 fixar=false;
				 return (true && CRIAR_VARS_FIXADAS);						
			 }
			 return false;
		 }
		 case VariableTatInt::V_SLACK_ABERT_SEQ_TURMA:  //  ft_{i,d,cp} 
		 {		
			 if ( permitirAbertura( v->getTurma(), v->getDisciplina(), v->getCampus()->getId() ) )
			 {
				 fixar=false;
				 return (true);						
			 }
			 return false;
		 }
		 default:
		 {
			 fixar=false;
			 return true;
			 break;
		 }
	}

	fixar=false;
	return true;
}

/*
	Possiveis casos de equivalências:

	(Disciplina Antiga => Disciplina Nova): 
			1. (T => T)
			2. (T => PT)
			3. (PT => T)
			4. (PT => PT)

	As informações de equivalências estão presentes somente nas disciplinas teoricas. Ou seja,
	nos casos 2. e 4. a estrutura discEquivSubstitutas de T conterá P e T, e a estrutura 
	discEquivSubstitutas de P será vazia; nos casos 1. e 3. a estrutura discEquivSubstitutas
	de T conterá T, e a estrutura discEquivSubstitutas de P será vazia.

	P = disciplina de credito pratico
	T = disciplina de credito teorico
*/
void TaticoIntAlunoHor::atualizarDemandasEquiv( int campusId, int prioridade )
{	
	#pragma region Imprime Equiv
	ofstream outEquiv;
	outEquiv.open( getEquivFileName(campusId,prioridade).c_str(), ofstream::app);
	if ( !outEquiv )
	{
		std::cerr<<"\nAbertura do arquivo "<< getEquivFileName(campusId,prioridade).c_str()
			<< " falhou em TaticoIntAlunoHor::atualizarDemandasEquiv().\n";
	}
	#pragma endregion

	int idAlDemanda = problemData->retornaMaiorIdAlunoDemandas();

	ITERA_GGROUP_LESSPTR ( itVar, this->solVarsTatInt, VariableTatInt )
	{
		if ( (*itVar)->getType() == VariableTatInt::V_ALOCA_ALUNO_TURMA_DISC )
		{
			VariableTatInt *v = *itVar; // s_{a,i,d,cp}
			Aluno *aluno = v->getAluno();
			Disciplina *disciplina = v->getDisciplina();
			int turma = v->getTurma();

			if ( v->getCampus()->getId() != campusId )
				continue;
			
			// Continue, caso seja alocação de prioridade passada
			if ( prioridade==2 && problemData->procuraAlunoDemanda(disciplina->getId(), aluno->getAlunoId(), 1) != NULL )
				continue;
			
			AlunoDemanda *alDemOrig = problemData->procuraAlunoDemanda( disciplina->getId(), aluno->getAlunoId(), prioridade );


			if ( alDemOrig==NULL ) // disciplina substituta
			{
				AlunoDemanda *alDem = problemData->atualizaAlunoDemandaEquiv( turma, disciplina, campusId, aluno, prioridade );
				
				if ( alDem==NULL )
				{
					std::cout<<"\nErro em void TaticoIntAlunoHor::atualizarDemandasEquiv( int campusId, int prioridade, int r ). AlunoDemanda null.";
					continue;
				}
				if ( outEquiv )
				{
					outEquiv << "\nAluno "<<aluno->getAlunoId()<<" Disc original "<< alDem->demandaOriginal->getDisciplinaId()
						<<" Disc substituta "<<disciplina->getId()<<" Prioridade "<< alDem->getPrioridade();
				}
			}
		}
	}

	if ( outEquiv )
	{
		outEquiv.close();
	}

	problemData->preencheMapDisciplinaAlunosDemanda( this->USAR_EQUIVALENCIA );
	problemData->imprimeResumoDemandasPorAlunoPosEquiv();

}

int TaticoIntAlunoHor::criaVariaveisTatico( int campusId, int P, int r )
{
	bool DIVIDIR_DEMANDAS=false;

	int num_vars = 0;
	CPUTimer timer;
	double dif = 0.0;

#ifdef PRINT_cria_variaveis
	int numVarsAnterior = 0;
#endif
	
	// Preenche o map para auxilio na criação das variaveis
	std::cout << "\nPreenchendo mapDiscAlunosDemanda para auxilio na criacao de variaveis..." ;
	timer.start();	
	if ( problemData->parametros->considerar_equivalencia_por_aluno && USAR_EQUIVALENCIA ) 
	{		
		ITERA_GGROUP_LESSPTR( itDisc, problemData->disciplinas, Disciplina )
		{
			this->mapDiscAlunosDemanda[(*itDisc)->getId()] =
				problemData->retornaMaxDemandasDiscNoCampus_EquivTotal( *itDisc, campusId, P, this->PERMITIR_REALOCAR_ALUNO );
	
			if ( itDisc->getId() == 13039 || itDisc->getId() == 13040 )
			{
				std::cout<<"\nDisc " << itDisc->getId() << ":  ";
				GGroup<AlunoDemanda*, LessPtr<AlunoDemanda>> alunosDemanda = this->mapDiscAlunosDemanda[(*itDisc)->getId()];
				ITERA_GGROUP_LESSPTR ( itAl, alunosDemanda, AlunoDemanda )
				{
					std::cout<<" AlDem" << itAl->getId();
				}
			}
		}
	}
	else
	{
		if ( !CRIAR_VARS_FIXADAS )
		{
			// Só considera as demandas NÃO atendidas			
			ITERA_GGROUP_LESSPTR( itAlDem, problemData->listSlackDemandaAluno, AlunoDemanda )
			{		
				if ( itAlDem->getPrioridade() <= P && itAlDem->demanda->oferta->getCampusId() == campusId )
				{
					this->mapDiscAlunosDemanda[ itAlDem->demanda->getDisciplinaId() ].add( *itAlDem );					
				}				
			}
		}
		else
		{
			ITERA_GGROUP_LESSPTR( itDisc, problemData->disciplinas, Disciplina )
				this->mapDiscAlunosDemanda[(*itDisc)->getId()] = problemData->retornaDemandasDiscNoCampus( (*itDisc)->getId(), campusId, P );		
		}
	}

	if ( DIVIDIR_DEMANDAS )
	if ( ITERACAO==1 && !CRIAR_VARS_FIXADAS )
	{
	// Só considera folgas de demanda significativas
		std::cout<<"\nmapDiscAlunosDemanda:";
		int naoAtendTotal=0;
		int naoAtendRemovido=0;
		GGroup<int> discToRemove;
		std::map< int /*discId*/, GGroup<AlunoDemanda*,LessPtr<AlunoDemanda>> >::iterator
			itFolgaDiscAlDem = mapDiscAlunosDemanda.begin();
		for ( ; itFolgaDiscAlDem != mapDiscAlunosDemanda.end(); itFolgaDiscAlDem++ )
		{
			naoAtendTotal += itFolgaDiscAlDem->second.size();

			if ( itFolgaDiscAlDem->second.size() <= 25 )
				discToRemove.add( itFolgaDiscAlDem->first );
			else
			{
				naoAtendRemovido += itFolgaDiscAlDem->second.size();
				std::cout<<"\nDisc"<<itFolgaDiscAlDem->first<<", Alunos:";
				ITERA_GGROUP_LESSPTR( itAlDem, itFolgaDiscAlDem->second, AlunoDemanda )
					std::cout<<" "<<itAlDem->getAlunoId();
			}
		}
		ITERA_GGROUP_N_PT( it, discToRemove, int )
		{
			mapDiscAlunosDemanda.erase( *it );
		}
		std::cout<<"\nNro total de nao atendimentos ate entao: " << naoAtendTotal;
		std::cout<<"\nNro total de nao atendimentos considerados nessa rodada: " << naoAtendTotal-naoAtendRemovido<< endl;
	}

	timer.stop();
	dif = timer.getCronoCurrSecs();
	std::cout << " preenchido! \t" << dif <<" sec" <<std::endl;


	timer.start();
	std::cout << "Criando \"x\": ";fflush(NULL);
	//if ( P == 1 && !this->PERMITIR_NOVAS_TURMAS )
	//	num_vars += this->criaVariavelTaticoCreditosComSolInicial( campusId, P ); // x_{i,d,u,s,t,hi,hf}	
	//else
	//	num_vars += this->criaVariavelTaticoCreditos( campusId, P ); // x_{i,d,u,s,t,hi,hf}
	num_vars += this->criaVariavelTaticoCreditos__( campusId, P ); // x_{i,d,u,s,t,hi,hf}	
	timer.stop();
	dif = timer.getCronoCurrSecs();
	
#ifdef PRINT_cria_variaveis
	std::cout << "numVars \"x\": " << (num_vars - numVarsAnterior)  <<"\t "<<dif <<" sec" << std::endl; fflush(NULL);
	numVarsAnterior = num_vars;
#endif


	timer.start();	
	std::cout << "Criando \"v\": ";fflush(NULL);	
	if ( this->etapa != Etapa5 )
		num_vars += this->criaVariavelTaticoAlunoCreditosAPartirDeX_MaisFiltroAluno( campusId, P ); // v_{a,i,d,u,s,t,hi,hf}
	else
		num_vars += this->criaVariavelTaticoAlunoCreditosAPartirDeX( campusId, P ); // v_{a,i,d,u,s,t,hi,hf}
	//num_vars += criaVariavelTaticoAlunoCreditos( campusId, P ); // v_{a,i,d,u,s,t,hi,hf}
	timer.stop();
	dif = timer.getCronoCurrSecs();
	
#ifdef PRINT_cria_variaveis
	std::cout << "numVars \"v\": " << (num_vars - numVarsAnterior)  <<"\t "<<dif <<" sec" << std::endl; fflush(NULL);
	numVarsAnterior = num_vars;
#endif	

	
	timer.start();
	std::cout << "Criando \"o\": ";fflush(NULL);
	num_vars += this->criaVariavelTaticoOferecimentosAPartirDeX( campusId, P ); // o_{i,d,u,s}
	//num_vars += this->criaVariavelTaticoOferecimentos( campusId, P ); // o_{i,d,u,s}
	timer.stop();
	dif = timer.getCronoCurrSecs();
	
#ifdef PRINT_cria_variaveis
	std::cout << "numVars \"o\": " << (num_vars - numVarsAnterior)  <<"\t "<<dif <<" sec" << std::endl; fflush(NULL);
	numVarsAnterior = num_vars;
#endif
	

	timer.start();
	std::cout << "Criando \"s\": ";fflush(NULL);	
	num_vars += this->criaVariavelTaticoAlocaAlunoTurmaDiscAPartirDeV( campusId, P );		// s_{i,d,a,cp}
	//num_vars += this->criaVariavelTaticoAlocaAlunoTurmaDisc( campusId, P );		// s_{i,d,a,cp}
	//num_vars += this->criaVariavelTaticoAlocaAlunoTurmaDiscEquiv( campusId, P );  // s_{i,d,a,cp}	
	//num_vars += this->criaVariavelTaticoAlocaAlunoTurmaDiscEquivTotal( campusId, P );  // s_{i,d,a,cp}	
	timer.stop();
	dif = timer.getCronoCurrSecs();
	
#ifdef PRINT_cria_variaveis
	std::cout << "numVars \"s\": " << (num_vars - numVarsAnterior)  <<"\t "<<dif <<" sec" << std::endl; fflush(NULL);
	numVarsAnterior = num_vars;
#endif



	timer.start();
	std::cout << "Criando \"b\": ";fflush(NULL);
	num_vars += this->criaVariavelTaticoCursoAlunos( campusId, P ); // b_{i,d,c,c'}
	timer.stop();
	dif = timer.getCronoCurrSecs();
	
#ifdef PRINT_cria_variaveis
	std::cout << "numVars \"b\": " << (num_vars - numVarsAnterior)  <<"\t "<<dif <<" sec" << std::endl; fflush(NULL);
	numVarsAnterior = num_vars;
#endif


	/*
	timer.start();
	num_vars += this->criaVariavelTaticoConsecutivosAPartirDeX( campusId, P ); // c
	//num_vars += this->criaVariavelTaticoConsecutivos( campusId, P ); // c
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_variaveis
	std::cout << "numVars \"c\": " << (num_vars - numVarsAnterior)  <<"\t "<<dif <<" sec" << std::endl; fflush(NULL);
	numVarsAnterior = num_vars;
#endif
	*/

	timer.start();
	std::cout << "Criando \"m\": ";fflush(NULL);
	num_vars += this->criaVariavelTaticoCombinacaoDivisaoCreditoAPartirDeO( campusId, P ); // m_{i,d,k}
	//num_vars += this->criaVariavelTaticoCombinacaoDivisaoCredito( campusId, P ); // m_{i,d,k}
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_variaveis
	std::cout << "numVars \"m\": " << (num_vars - numVarsAnterior)  <<"\t "<<dif <<" sec" << std::endl; fflush(NULL);
	numVarsAnterior = num_vars;
#endif

				
	timer.start();
	std::cout << "Criando \"fkp\" e \"fkm\": ";fflush(NULL);
	num_vars += this->criaVariavelTaticoFolgaCombinacaoDivisaoCreditoAPartirDeO( campusId, P ); // fk_{i,d,t}
	//num_vars += this->criaVariavelTaticoFolgaCombinacaoDivisaoCredito( campusId, P ); // fk_{i,d,t}
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_variaveis
	std::cout << "numVars \"fkp e fkm\": " << (num_vars - numVarsAnterior)  <<"\t "<<dif <<" sec" << std::endl; fflush(NULL);
	numVarsAnterior = num_vars;
#endif

/*
	timer.start();
	num_vars += criaVariavelTaticoFolgaDistCredDiaSuperior( campusId, P ); // fcp_{d,t}
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_variaveis
	std::cout << "numVars \"fcp\": " << (num_vars - numVarsAnterior)  <<" "<<dif <<" sec" << std::endl; fflush(NULL);
	numVarsAnterior = num_vars;
#endif
   		

	timer.start();
	num_vars += criaVariavelTaticoFolgaDistCredDiaInferior( campusId, P ); // fcm_{d,t}
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_variaveis
	std::cout << "numVars \"fcm\": " << (num_vars - numVarsAnterior)  <<" "<<dif <<" sec" << std::endl; fflush(NULL);
	numVarsAnterior = num_vars;
#endif
*/

	timer.start();
	std::cout << "Criando \"zc\": ";fflush(NULL);
	num_vars += this->criaVariavelTaticoAberturaCompativelAPartirDeX( campusId, P ); // zc
	//num_vars += this->criaVariavelTaticoAberturaCompativel( campusId, P ); // zc
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_variaveis
	std::cout << "numVars \"zc\": " << (num_vars - numVarsAnterior)  <<"\t "<<dif <<" sec" << std::endl; fflush(NULL);
	numVarsAnterior = num_vars;
#endif


	timer.start();
	std::cout << "Criando \"fd\": ";fflush(NULL);
	num_vars += this->criaVariavelTaticoFolgaDemandaDiscAluno( campusId, P ); // fd
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_variaveis
	std::cout << "numVars \"fd\": " << (num_vars - numVarsAnterior)  <<"\t "<<dif <<" sec" << std::endl; fflush(NULL);
	numVarsAnterior = num_vars;
#endif

	/*  // Não faz sentido essa variavel para esse modelo! Tem que ser adaptada para essa modelagem de tatico integrado, usando por aluno
		// ao inves de por par de turmas
	timer.start();
	num_vars += criaVariavelTaticoFolgaAlunoUnidDifDia( campusId, P ); // fu_{i1,d1,i2,d2,t,cp}
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_variaveis
	std::cout << "numVars \"fu\": " << (num_vars - numVarsAnterior)  <<" "<<dif <<" sec" << std::endl; fflush(NULL);
	numVarsAnterior = num_vars;
#endif
	*/

	timer.start();
	std::cout << "Criando \"du\": ";fflush(NULL);
	num_vars += this->criaVariavelTaticoDiaUsadoPeloAluno( campusId, P ); // du_{a,t}
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_variaveis
	std::cout << "numVars \"du\": " << (num_vars - numVarsAnterior)  <<"\t "<<dif <<" sec" << std::endl; fflush(NULL);
	numVarsAnterior = num_vars;
#endif
		

	timer.start();
	std::cout << "Criando \"ft\": ";fflush(NULL);
	//num_vars += this->criaVariavelTaticoFolgaAbreTurmaSequencialAPartirDeX( campusId, P ); // ft_{i,d,cp}
	num_vars += this->criaVariavelTaticoFolgaAbreTurmaSequencial( campusId, P ); // ft_{i,d,cp}
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_variaveis
	std::cout << "numVars \"ft\": " << (num_vars - numVarsAnterior)  <<"\t "<<dif <<" sec" << std::endl; fflush(NULL);
	numVarsAnterior = num_vars;
#endif
		
	

	timer.start();
	std::cout << "Criando \"fpi\": ";fflush(NULL);
	num_vars += this->criaVariavelFolgaPrioridadeInf( campusId, P ); // fpi_{a,cp}
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_variaveis
	std::cout << "numVars \"fpi\": " << (num_vars - numVarsAnterior)  <<"\t "<<dif <<" sec" << std::endl; fflush(NULL);
	numVarsAnterior = num_vars;
#endif
	
	
	timer.start();
	std::cout << "Criando \"fps\": ";fflush(NULL);
	num_vars += this->criaVariavelFolgaPrioridadeSup( campusId, P ); // fps_{a,cp}
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_variaveis
	std::cout << "numVars \"fps\": " << (num_vars - numVarsAnterior)  <<"\t "<<dif <<" sec" << std::endl; fflush(NULL);
	numVarsAnterior = num_vars;
#endif


	timer.start();
	std::cout << "Criando \"f\": ";fflush(NULL);
	num_vars += this->criaVariavelTaticoFormandosNaTurma( campusId, P, r ); // f_{i,d,cp}
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_variaveis
	std::cout << "numVars \"f\": " << (num_vars - numVarsAnterior)  <<"\t "<<dif <<" sec" << std::endl; fflush(NULL);
	numVarsAnterior = num_vars;
#endif


	timer.start();
	std::cout << "Criando \"z\": ";fflush(NULL);
	num_vars += this->criaVariavelTaticoAberturaAPartirDeX( campusId, P ); // z_{i,d,cp}
	//num_vars += this->criaVariavelTaticoAbertura( campusId, P, r ); // z_{i,d,cp}
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_variaveis
	std::cout << "numVars \"z\": " << (num_vars - numVarsAnterior)  <<"\t "<<dif <<" sec" << std::endl; fflush(NULL);
	numVarsAnterior = num_vars;
#endif


	timer.start();
	std::cout << "Criando \"fp\": ";fflush(NULL);
	num_vars += this->criaVariavelTaticoFolgaProfAPartirDeX( campusId, P ); // fp_{d,t,h}
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_variaveis
	std::cout << "numVars \"fp\": " << (num_vars - numVarsAnterior)  <<"\t "<<dif <<" sec" << std::endl; fflush(NULL);
	numVarsAnterior = num_vars;
#endif


	timer.start();
	std::cout << "Criando \"ss\": ";fflush(NULL);
	num_vars += this->criaVariavelTaticoAlunosMesmaTurmaPratica( campusId, P ); // ss_{dp,a1,a2}
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_variaveis
	std::cout << "numVars \"ss\": " << (num_vars - numVarsAnterior)  <<"\t "<<dif <<" sec" << std::endl; fflush(NULL);
	numVarsAnterior = num_vars;
#endif


	timer.start();
	std::cout << "Criando \"fmd\": ";fflush(NULL);
	num_vars += this->criaVariavelTaticoFolgaMinimoDemandaPorAluno( campusId, P ); // fmd_{a}
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_variaveis
	std::cout << "numVars \"fmd\": " << (num_vars - numVarsAnterior)  <<"\t "<<dif <<" sec" << std::endl; fflush(NULL);
	numVarsAnterior = num_vars;
#endif


	timer.start();
	std::cout << "Criando \"fos\": ";fflush(NULL);
	num_vars += this->criaVariavelFolgaOcupacaoSala( campusId, P );				// fos_{i,d,cp}
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_variaveis
	std::cout << "numVars \"fos\": " << (num_vars - numVarsAnterior)  <<"\t "<<dif <<" sec" << std::endl; fflush(NULL);
	numVarsAnterior = num_vars;
#endif

	timer.start();
	std::cout << "Criando \"uu\": ";fflush(NULL);
	num_vars += this->criaVariavelUsaPeriodoUnid( campusId );				// uu_{u,oft,p}
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_variaveis
	std::cout << "numVars \"uu\": " << (num_vars - numVarsAnterior)  <<"\t "<<dif <<" sec" << std::endl; fflush(NULL);
	numVarsAnterior = num_vars;
#endif

	timer.start();
	std::cout << "Criando \"us\": ";fflush(NULL);
	num_vars += this->criaVariavelUsaPeriodoSala( campusId );				// us_{s,oft,p}
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_variaveis
	std::cout << "numVars \"us\": " << (num_vars - numVarsAnterior)  <<"\t "<<dif <<" sec" << std::endl; fflush(NULL);
	numVarsAnterior = num_vars;
#endif

	timer.start();
	std::cout << "Criando \"fuu\": ";fflush(NULL);
	num_vars += this->criaVariavelFolgaPeriodoUnid( campusId, P );				// fuu_{u,oft,p}
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_variaveis
	std::cout << "numVars \"fuu\": " << (num_vars - numVarsAnterior)  <<"\t "<<dif <<" sec" << std::endl; fflush(NULL);
	numVarsAnterior = num_vars;
#endif

	timer.start();
	std::cout << "Criando \"fus\": ";fflush(NULL);
	num_vars += this->criaVariavelFolgaPeriodoSala( campusId, P );				// fus_{s,oft,p}
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_variaveis
	std::cout << "numVars \"fus\": " << (num_vars - numVarsAnterior)  <<"\t "<<dif <<" sec" << std::endl; fflush(NULL);
	numVarsAnterior = num_vars;
#endif

	return num_vars;

}

// v_{a,i,d,u,s,hi,hf,t}
int TaticoIntAlunoHor::criaVariavelTaticoAlunoCreditosAPartirDeX_MaisFiltroAluno( int campusId, int P )
{
	int numVars = 0;

	CRIANDO_V_ATRAVES_DE_X = true;

	VariableTatIntHash varHashAux;	
	std::map< int, Trio<double, double, double> > varId_Bounds;

	std::map< int/*turma*/, std::map< Disciplina*, std::map< ConjuntoSala*, std::map< int/*dia*/, map<int/*id*/, VariableTatInt> >,
		LessPtr<ConjuntoSala> >, LessPtr<Disciplina> > > varsX;

	VariableTatIntHash::iterator vit = vHashTatico.begin();
	for ( ; vit != vHashTatico.end(); vit++)
	{		
		VariableTatInt x = vit->first;

		if( x.getType() != VariableTatInt::V_CREDITOS )
			continue;
		if ( x.getUnidade()->getIdCampus() != campusId )
			continue;

		int turma = x.getTurma();
		Disciplina* disciplina = x.getDisciplina();
		Unidade *unidade = x.getUnidade();
		ConjuntoSala *cjtSala = x.getSubCjtSala();
		int dia = x.getDia();
		
		varsX[turma][disciplina][cjtSala][dia][vit->second] = x;
	}

	std::map< int/*turma*/, std::map< Disciplina*, std::map< ConjuntoSala*, std::map< int/*dia*/, map<int/*id*/, VariableTatInt> >,
		LessPtr<ConjuntoSala> >, LessPtr<Disciplina> > >::iterator itTurma;

	std::map< Disciplina*, std::map< ConjuntoSala*, std::map< int/*dia*/, map<int/*id*/, VariableTatInt> >,
		LessPtr<ConjuntoSala> >, LessPtr<Disciplina> >::iterator itDisc;

	std::map< ConjuntoSala*, std::map< int/*dia*/, map<int/*id*/, VariableTatInt> >,
		LessPtr<ConjuntoSala> >::iterator itCjtSala;

	std::map< int/*dia*/, map<int/*id*/, VariableTatInt> >::iterator itDia;

	map< int /*id*/, VariableTatInt >::iterator itX;
		
	for ( itTurma = varsX.begin(); itTurma != varsX.end(); itTurma++ )
	{		
		int turma = itTurma->first;
		std::map< Disciplina*, std::map< ConjuntoSala*, std::map< int/*dia*/, map<int/*id*/, VariableTatInt> >,
		LessPtr<ConjuntoSala> >, LessPtr<Disciplina> > *mapDiscs = & itTurma->second;

		for ( itDisc = (*mapDiscs).begin(); itDisc != (*mapDiscs).end(); itDisc++ )
		{	
			Disciplina* disciplina = itDisc->first;
			std::map< ConjuntoSala*, std::map< int/*dia*/, map<int/*id*/, VariableTatInt> >,
			LessPtr<ConjuntoSala> > *mapCjtSala = & itDisc->second;
			
			GGroup<AlunoDemanda*,LessPtr<AlunoDemanda>> *alunosDemanda =
				& this->mapDiscAlunosDemanda[ disciplina->getId() ];

			ITERA_GGROUP_LESSPTR ( itAlDem, (*alunosDemanda), AlunoDemanda )
			{
				Aluno *aluno = problemData->retornaAluno( itAlDem->getAlunoId() );

				bool fixoInit=false;
				int t = problemData->getSolTaticoInicial()->getTurma( aluno, campusId, disciplina, fixoInit );

				if ( t != turma && fixoInit )
					continue;

				for ( itCjtSala = (*mapCjtSala).begin(); itCjtSala != (*mapCjtSala).end(); itCjtSala++ )
				{
					ConjuntoSala *cjtSala = itCjtSala->first;
					std::map< int/*dia*/, map<int/*id*/, VariableTatInt> > *mapDia = & itCjtSala->second;
				
					map< int/*dia*/, vector<VariableTatInt> > varsDiscAluno;					
					map< int/*dia*/, int/*nroCreds*/ > mapDiaCredsLivres;
					int nCredsLivres=0;
					bool tem_que_criar=false;	// só para conferir se está ok
					map< VariableTatInt, bool > fixaVarV;

					for ( itDia = (*mapDia).begin(); itDia != (*mapDia).end(); itDia++ )
					{
						int dia = itDia->first;
						map< int /*id*/, VariableTatInt > *mapX = & itDia->second;

						int nCredsLivresDia=0;

						for ( itX = (*mapX).begin(); itX != (*mapX).end(); itX++ )
						{
							int idX = itX->first;
							VariableTatInt x = itX->second;
		
							Unidade *unidade = x.getUnidade();		
							HorarioAula *hi = x.getHorarioAulaInicial();
							HorarioAula *hf = x.getHorarioAulaFinal();
							DateTime *di = x.getDateTimeInicial();
							DateTime *df = x.getDateTimeFinal();
														
							Calendario *calend = hi->getCalendario();
							int nCreds = calend->retornaNroCreditosEntreHorarios( hi, hf );		
							
							bool valid = true;
							HorarioAula *h = hi;
							for ( int j = 1; j <= nCreds; j++ )
							{
								bool possuiHor = itAlDem->demanda->getTurnoIES()->possuiHorarioDiaOuCorrespondente(h, dia);
								if ( !possuiHor )
									valid = false;
								h = calend->getProximoHorario( h );
							}
							if (!valid) 
								continue;
							
							VariableTatInt v;
							v.reset();
							v.setType( VariableTatInt::V_ALUNO_CREDITOS );
							v.setTurma( turma );            // i
							v.setDisciplina( disciplina );  // d
							v.setUnidade( unidade );		// u
							v.setSubCjtSala( cjtSala );		// tps  
							v.setDia( dia );				 // t
							v.setHorarioAulaInicial( hi );	 // hi
							v.setHorarioAulaFinal( hf );	 // hf
							v.setAluno( aluno );			 // a
							v.setDateTimeInicial( di );		// dti
							v.setDateTimeFinal( df );		// dtf
							
					//		std::cout << "\nValida: " << v.toString();

							if ( varHashAux.find( v ) == varHashAux.end() )
							{
								bool fixar=false;

								if( !PERMITIR_REALOCAR_ALUNO )
								if ( !this->criaVariavelTaticoInt( &v, fixar, P ) )
								{
									continue;
								}
								
				//				std::cout << "\n\tValida realmente!";

								varsDiscAluno[ dia ].push_back( v );
								fixaVarV[v] = fixar;

								if ( fixar ) tem_que_criar=true;

								int n = hi->getCalendario()->retornaNroCreditosEntreHorarios( hi, hf );
								if ( nCredsLivresDia < n )
									nCredsLivresDia = n;								
							}
						}
						nCredsLivres += nCredsLivresDia;
					}
					
					if ( nCredsLivres >= disciplina->getTotalCreditos() )
					{
						map< int/*dia*/, vector<VariableTatInt> >::iterator itDiaV = varsDiscAluno.begin();
						for ( ; itDiaV != varsDiscAluno.end() ; itDiaV++ )
						{
							for ( int j=0; j < itDiaV->second.size(); j++ )
							{
								VariableTatInt v = itDiaV->second[j];

								if ( varHashAux.find( v ) != varHashAux.end() )
									continue;

								int id = numVars;
								varHashAux[ v ] = id;
				
								double coef = 0.0;					
								double lowerBound = 0.0;
								double upperBound = 1.0;

								if( !PERMITIR_REALOCAR_ALUNO )
								if ( fixaVarV[v] ) {
									lowerBound = 1.0;
									//std::cout<<"\n Fixo: " << v.toString();
								}//else std::cout<<"\n Livre: " << v.toString();

								Trio<double, double, double> trio;
								trio.set( coef,lowerBound, upperBound );

								varId_Bounds[ id ] = trio;
				
								numVars++;
							}
						}					
					}else if ( tem_que_criar ){
						std::cout<<"\nErro!!! Tem variavel v fixada que nao estou criando para "
							<< "turma " << turma << " disc " << disciplina->getId() << " aluno " << aluno->getAlunoId();
					}
				}
			}
		}
    }
    
	// Insere todas as variaveis em vHashTatico e no lp	
	for ( vit = varHashAux.begin(); vit != varHashAux.end(); vit++)
	{		
		VariableTatInt v = vit->first;
		int id = vit->second;

		vHashTatico[ v ] = lp->getNumCols();
		
		double coef = varId_Bounds[id].first;
		double lowerBound = varId_Bounds[id].second;
		double upperBound = varId_Bounds[id].third;

		OPT_COL col( OPT_COL::VAR_BINARY, coef, lowerBound, upperBound,
			( char * )v.toString().c_str());

		lp->newCol( col );
	}

	return numVars;

}


// v_{a,i,d,u,s,hi,hf,t}
int TaticoIntAlunoHor::criaVariavelTaticoAlunoCreditosAPartirDeX( int campusId, int P )
{
	int numVars = 0;

	CRIANDO_V_ATRAVES_DE_X = true;

	VariableTatIntHash varHashAux;	
	std::map< int, Trio<double, double, double> > varId_Bounds;

	VariableTatIntHash::iterator vit = vHashTatico.begin();
	for ( ; vit != vHashTatico.end(); vit++)
	{		
		VariableTatInt v = vit->first;

		if( v.getType() != VariableTatInt::V_CREDITOS )
		{
			continue;
		}

		int turma = v.getTurma();
		Disciplina* disciplina = v.getDisciplina();
		Unidade *unidade = v.getUnidade();
		ConjuntoSala *cjtSala = v.getSubCjtSala();
		int dia = v.getDia();
		HorarioAula *hi = v.getHorarioAulaInicial();
		HorarioAula *hf = v.getHorarioAulaFinal();
		DateTime *di = v.getDateTimeInicial();
		DateTime *df = v.getDateTimeFinal();

		Calendario *calend = hi->getCalendario();
		int nCreds = calend->retornaNroCreditosEntreHorarios( hi, hf );
		
		GGroup<AlunoDemanda*,LessPtr<AlunoDemanda>> alunosDemanda =
			this->mapDiscAlunosDemanda[ disciplina->getId() ];

		ITERA_GGROUP_LESSPTR ( itAlDem, alunosDemanda, AlunoDemanda )
		{
			Aluno *aluno = problemData->retornaAluno( itAlDem->getAlunoId() );

			bool valid = true;
			HorarioAula *h = hi;
			for ( int j = 1; j <= nCreds; j++ )
			{
				bool possuiHor = itAlDem->demanda->getTurnoIES()->possuiHorarioDiaOuCorrespondente(h, dia);
				if ( !possuiHor )
					valid = false;
				h = calend->getProximoHorario( h );
			}

			if (!valid) continue;

			VariableTatInt v;
			v.reset();
			v.setType( VariableTatInt::V_ALUNO_CREDITOS );

			v.setTurma( turma );            // i
			v.setDisciplina( disciplina );  // d
			v.setUnidade( unidade );		// u
			v.setSubCjtSala( cjtSala );		// tps  
			v.setDia( dia );				 // t
			v.setHorarioAulaInicial( hi );	 // hi
			v.setHorarioAulaFinal( hf );	 // hf
			v.setAluno( aluno );			 // a
            v.setDateTimeInicial( di );
            v.setDateTimeFinal( df );

			if ( varHashAux.find( v ) == varHashAux.end() )
			{
				bool fixar=false;

				if ( !criaVariavelTaticoInt( &v, fixar, P ) )
				{
					continue;
				}
						
				int id = numVars;
				varHashAux[ v ] = id;
				
				double coef = 0.0;					
				double lowerBound = 0.0;
				double upperBound = 1.0;

				if ( fixar ) lowerBound = 1.0;
				
				Trio<double, double, double> trio;
				trio.set( coef,lowerBound, upperBound );

				varId_Bounds[ id ] = trio;
				
				numVars++;
			}
		}
    }
    
	// Insere todas as variaveis em vHashTatico e no lp	
	for ( vit = varHashAux.begin(); vit != varHashAux.end(); vit++)
	{		
		VariableTatInt v = vit->first;
		int id = vit->second;

		vHashTatico[ v ] = lp->getNumCols();
		
		double coef = varId_Bounds[id].first;
		double lowerBound = varId_Bounds[id].second;
		double upperBound = varId_Bounds[id].third;

		OPT_COL col( OPT_COL::VAR_BINARY, coef, lowerBound, upperBound,
			( char * )v.toString().c_str());

		lp->newCol( col );
	}

	return numVars;

}

// x_{i,d,u,s,hi,hf,t}
void TaticoIntAlunoHor::criaVariavelTaticoCreditosCopiadas( int campusId, int P, int &numVars )
{
	GGroup< VariableTatico *, LessPtr<VariableTatico> > solTaticoVarsX = *vars_xh;
	ITERA_GGROUP_LESSPTR( itVarX, solTaticoVarsX, VariableTatico )
	{
		VariableTatico x = **itVarX;
		
		if ( x.getUnidade()->getIdCampus() != campusId )
			continue;		

		VariableTatInt v;
		v.reset();
		v.setType( VariableTatInt::V_CREDITOS );

		v.setTurma( x.getTurma() );				 // i
		v.setDisciplina( x.getDisciplina() );	 // d
		v.setUnidade( x.getUnidade() );			 // u
		v.setSubCjtSala( x.getSubCjtSala() );	 // tps  
		v.setDia( x.getDia() );						// t
		v.setHorarioAulaInicial( x.getHorarioAulaInicial() );	 // hi
		v.setHorarioAulaFinal( x.getHorarioAulaFinal() );	 // hf
			
        std::pair<DateTime*,int> auxP = problemData->horarioAulaDateTime[x.getHorarioAulaInicial()->getId()];
        v.setDateTimeInicial(auxP.first);
        auxP = problemData->horarioAulaDateTime[x.getHorarioAulaFinal()->getId()];
        v.setDateTimeFinal(auxP.first);

		bool Ok = x.getDisciplina()->inicioTerminoValidos( x.getHorarioAulaInicial(), x.getHorarioAulaFinal(), x.getDia() );
		if (!Ok)
		{
			std::stringstream msg;
			msg << "Aula " << x.toString() << " criada, porem viola Disciplina::inicioTerminoValidos()";
			CentroDados::printError( "void TaticoIntAlunoHor::criaVariavelTaticoCreditosCopiadas()", msg.str() );
		}

		if ( vHashTatico.find( v ) == vHashTatico.end() )
		{
			vHashTatico[ v ] = lp->getNumCols();
									
			double lowerBound = 0.0;
			double upperBound = 1.0;
		
			if (this->PERMITIR_NOVAS_TURMAS || P>1)	// Se não forem abertas novas turmas, não fixa a fim de permitir a união de turmas
				lowerBound = 1.0;

			OPT_COL col( OPT_COL::VAR_BINARY, 0.0, lowerBound, upperBound,
				( char * )v.toString().c_str());
			
			lp->newCol( col );
			numVars++;
		}		
	}
}

// x_{i,d,u,s,hi,hf,t}
int TaticoIntAlunoHor::criaVariavelTaticoCreditos( int campusId, int P )
{
	int numVars = 0;

	// CRIA TODAS AS VARIAVEIS X QUE JÁ EXISTEM OLHANDO PARA A SOLUÇÃO ANTERIOR
	if ( this->CRIAR_VARS_FIXADAS )
	{
		criaVariavelTaticoCreditosCopiadas( campusId, P, numVars );
	}

	std::cout<<"\nCriei "<<numVars<<" variaveis x, copiadas da solucao anterior.\n"; fflush(NULL);

	// CRIA TODAS AS VARIAVEIS X DE TURMAS QUE AINDA NÃO EXISTEM
	
	

	if ( this->PERMITIR_NOVAS_TURMAS )
	{
		bool debug = false;
		//if ( disciplina->getId()==12705 )
			debug = true;

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
				 
				 if ( debug )
					std::cout<<"\nsalaId "<<salaId; fflush(NULL);

				ITERA_GGROUP_LESSPTR( it_disciplina, itCjtSala->disciplinas_associadas, Disciplina )
				{
					Disciplina *disciplina = ( *it_disciplina );				
												    
					if ( debug )
						std::cout<<"\nDisc "<<disciplina->getId(); fflush(NULL);

					if ( this->mapDiscAlunosDemanda[disciplina->getId()].size() == 0 )
						continue;
				
				   std::pair< int, int > parDiscSala = std::make_pair( disciplina->getId(), salaId );

				   std::map< std::pair< int /*idDisc*/, int /*idSala*/ >, 
							 std::map< int /*Dia*/, GGroup< HorarioAula *, LessPtr< HorarioAula > > > >::iterator

				   it_Disc_Sala_Dias_HorariosAula = problemData->disc_Salas_Dias_HorariosAula.find( parDiscSala );

				   if ( it_Disc_Sala_Dias_HorariosAula == 
						problemData->disc_Salas_Dias_HorariosAula.end() )
				   {
					   continue;
				   }

					std::map< int /*Dia*/, GGroup< HorarioAula *, LessPtr< HorarioAula > > >::iterator
						it_Dia_HorarioAula = it_Disc_Sala_Dias_HorariosAula->second.begin();
					
					for ( ; it_Dia_HorarioAula != it_Disc_Sala_Dias_HorariosAula->second.end(); it_Dia_HorarioAula++ )
					{
						int dia = it_Dia_HorarioAula->first;
							
						if ( debug )						
							std::cout<<"\n\t\tDia "<<dia; fflush(NULL);

						ITERA_GGROUP_LESSPTR( itHorarioI, it_Dia_HorarioAula->second, HorarioAula )
						{
							HorarioAula *hi = *itHorarioI;

							ITERA_GGROUP_INIC_LESSPTR( itHorarioF, itHorarioI, it_Dia_HorarioAula->second, HorarioAula )
							{
								HorarioAula *hf = *itHorarioF;
									 					
								if ( debug )
								std::cout<<"\n\t\t\tHi "<<hi->getId() << "   Hf "<<hf->getId(); fflush(NULL);

								if ( ! disciplina->inicioTerminoValidos( hi, hf, dia, it_Dia_HorarioAula->second ) )
									continue;								 
									
								if ( debug )
								std::cout<<" => validos!";

								for ( int turma = 1; turma <= disciplina->getNumTurmas(); turma++ )
								{
									if ( debug )
										std::cout<<"\n\tTurma  "<< turma; fflush(NULL);
										
									VariableTatInt v;
									v.reset();
									v.setType( VariableTatInt::V_CREDITOS );

									v.setTurma( turma );            // i
									v.setDisciplina( disciplina );  // d
									v.setUnidade( *itUnidade );     // u
									v.setSubCjtSala( *itCjtSala );  // tps  
									v.setDia( dia );				 // t
									v.setHorarioAulaInicial( hi );	 // hi
									v.setHorarioAulaFinal( hf );	 // hf

									if ( debug )
										std::cout<<"... Procurando hi..."; fflush(NULL);

									std::pair<DateTime*,int> auxP = problemData->horarioAulaDateTime[hi->getId()];
									v.setDateTimeInicial(auxP.first);

									if ( debug )
										std::cout<<"... Procurando hf..."; fflush(NULL);

									auxP = problemData->horarioAulaDateTime[hf->getId()];
									v.setDateTimeFinal(auxP.first);

									if ( vHashTatico.find( v ) == vHashTatico.end() )
									{
										bool fixar=false;
											if ( debug )
											std::cout<<". Vou criar?"; fflush(NULL);

										if ( !criaVariavelTaticoInt( &v, fixar, P ) )
										{
											if ( debug )
											std::cout<<" Nao!"; fflush(NULL);

											continue;
										}

										if ( debug )
											std::cout<<"Sim!"; fflush(NULL);

										vHashTatico[ v ] = lp->getNumCols();
									
										double lowerBound = 0.0;
										double upperBound = 1.0;

										if ( fixar ) lowerBound = 1.0;

										OPT_COL col( OPT_COL::VAR_BINARY, 0.0, lowerBound, upperBound,
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
		if ( debug )
			std::cout<<"\nFim!"; fflush(NULL);
	}

	return numVars;
}

// x_{i,d,u,s,hi,hf,t}
int TaticoIntAlunoHor::criaVariavelTaticoCreditos__( int campusId, int P )
{
	int numVars = 0;

	// CRIA TODAS AS VARIAVEIS X QUE JÁ EXISTEM OLHANDO PARA A SOLUÇÃO ANTERIOR
	if ( this->CRIAR_VARS_FIXADAS )
	{
		criaVariavelTaticoCreditosCopiadas( campusId, P, numVars );
	}

	std::cout<<"\nCriei "<<numVars<<" variaveis x, copiadas da solucao anterior.\n"; fflush(NULL);

	// CRIA TODAS AS VARIAVEIS X DE TURMAS QUE AINDA NÃO EXISTEM

	if ( this->PERMITIR_NOVAS_TURMAS )
	{
		// ============================================================================================================
		#pragma region MAPEANDO HORARIOS-DIA DAS DISCIPLINAS
		std::cout<<"\nMapeando horarios das disciplinas...";

		std::map<Disciplina*, std::map< int /*dia*/, std::map< DateTime, std::map<Calendario*, std::map<int /*turnoIES*/, 
			HorarioAula* >, LessPtr<Calendario> > > >, LessPtr<Disciplina> > mapDiscDiaDtCalendTurnoHorAula;

		ITERA_GGROUP_LESSPTR( it_disciplina, problemData->disciplinas, Disciplina )
		{
			Disciplina *disciplina = ( *it_disciplina );		
		
			ITERA_GGROUP_LESSPTR( it_hor, disciplina->horarios, Horario )
			{
				HorarioAula *ha = it_hor->horario_aula;
			
				ITERA_GGROUP_N_PT( itDia, it_hor->dias_semana, int )
				{
					mapDiscDiaDtCalendTurnoHorAula[ disciplina ][ *itDia ][ ha->getInicio() ][ ha->getCalendario() ][ ha->getTurnoIESId() ] = ha;
				}
			}
		}
		#pragma endregion

		// ============================================================================================================
		#pragma region CRIANDO PARES HI-HF POSSIVEIS PARA CADA DISCIPLINA-SALA

		std::map<Disciplina*, std::map< ConjuntoSala*, std::map< int /*dia*/, std::map< DateTime, std::map< DateTime,  
			std::pair<HorarioAula*, HorarioAula*> > > >, LessPtr<ConjuntoSala> >, LessPtr<Disciplina> > mapDiscSalaDiaDtiDtf;

		std::cout<<"\nFiltrando horarios hi-hf das disciplinas...";

		std::map<Disciplina*, std::map< int /*dia*/, std::map< DateTime, std::map<Calendario*, std::map<int /*turnoIES*/, 
			HorarioAula* >, LessPtr<Calendario> > > >, LessPtr<Disciplina> >::iterator
				itDisc = mapDiscDiaDtCalendTurnoHorAula.begin();
		for( ; itDisc != mapDiscDiaDtCalendTurnoHorAula.end(); itDisc++ )
		{
			Disciplina* disc = itDisc->first;

			std::map< int /*dia*/, std::map< DateTime, std::map<Calendario*, std::map<int /*turnoIES*/, 
				HorarioAula* >, LessPtr<Calendario> > > >::iterator
				itDia = itDisc->second.begin();
			for( ; itDia != itDisc->second.end(); itDia++ )
			{
				int dia = itDia->first;

				std::map< DateTime, std::map<Calendario*, std::map<int /*turnoIES*/, 
					HorarioAula* >, LessPtr<Calendario> > >::iterator
					itDti = itDia->second.begin();
				for( ; itDti != itDia->second.end(); itDti++ )
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
							for( ; itDtf != itDia->second.end(); itDtf++ )
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

									std::map<int /*turnoIES*/, HorarioAula* >::iterator
										itTurnoF = itCalendF->second.find( turnoIES );
									if( itTurnoF != itCalendF->second.end() )
									{
										// Mesmo turnoIES
										HorarioAula* hf = itTurnoF->second;
																				
										if ( disc->inicioTerminoValidos( hi, hf, dia ) )
										{
											for ( auto itCjtSala = disc->cjtSalasAssociados.begin();
												itCjtSala != disc->cjtSalasAssociados.end(); itCjtSala++ )
											{
												ConjuntoSala* cjtSala = itCjtSala->second;
												Sala *sala = itCjtSala->second->getSala();

												if ( sala->possuiHorariosNoDia( hi, hf, dia ) )
												{
													mapDiscSalaDiaDtiDtf[disc][cjtSala][dia][dti][dtf] = std::make_pair( hi, hf );
												}
											}
										}
									}
								}
							}	
							// -----------------------------------------------------------------						
						}
					}
				}
			}
		}

		mapDiscDiaDtCalendTurnoHorAula.clear();

		#pragma endregion
		
		#pragma region CRIANDO VARIAVEIS X

		std::cout<<"\nCriando novas variaveis x...";

		std::map<Disciplina*, std::map< ConjuntoSala*, std::map< int /*dia*/, std::map< DateTime, std::map< DateTime,  
			std::pair<HorarioAula*, HorarioAula*> > > >, LessPtr<ConjuntoSala> >, LessPtr<Disciplina> >::iterator
			itMapDiscSalaDiaDtiDtf = mapDiscSalaDiaDtiDtf.begin();

		for( ; itMapDiscSalaDiaDtiDtf != mapDiscSalaDiaDtiDtf.end(); itMapDiscSalaDiaDtiDtf++ )
		{			
			Disciplina *disciplina = itMapDiscSalaDiaDtiDtf->first;
												    
		bool debug = false;
		//if ( disciplina->getId()==13040 || disciplina->getId()==13039 )
		//	debug = true;

			if ( debug )
				std::cout<<"\nDisc "<<disciplina->getId(); fflush(NULL);

			if ( this->mapDiscAlunosDemanda[disciplina->getId()].size() == 0 )
				continue;
				
			int naoAtend = this->haFolgaDeAtendimento( disciplina );

			if ( debug )
			std::cout<<"\t"<<naoAtend<<" nao atendidos.";

			if ( naoAtend == 0 )
				continue;

			GGroup<int> turmasParaAbrir;

			for ( int turma = 1; turma <= disciplina->getNumTurmas(); turma++ )
			{									
				bool turmaExiste = problemData->existeTurmaDiscCampus( turma, disciplina->getId(), campusId );
				if ( !turmaExiste  )
				{
					turmasParaAbrir.add(turma);
				}
			}

			if ( turmasParaAbrir.size() == 0 )
				continue;

			std::map< ConjuntoSala*, std::map< int /*dia*/, std::map< DateTime, std::map< DateTime,  
				std::pair<HorarioAula*, HorarioAula*> > > >, LessPtr<ConjuntoSala> > 
				*mapSalaDiaDtiDtf = & itMapDiscSalaDiaDtiDtf->second;

			std::map< ConjuntoSala*, std::map< int /*dia*/, std::map< DateTime, std::map< DateTime,  
				std::pair<HorarioAula*, HorarioAula*> > > >, LessPtr<ConjuntoSala> >::iterator 
				itCjtSalaDiaDtiDtf = mapSalaDiaDtiDtf->begin();
			for( ; itCjtSalaDiaDtiDtf != mapSalaDiaDtiDtf->end(); itCjtSalaDiaDtiDtf++ )
			{
				ConjuntoSala* cjtSala = itCjtSalaDiaDtiDtf->first;

				if ( cjtSala->salas.size() != 1 )
				{
					std::cout<<"\nATENCAO em criaVariavelTaticoCreditos: conjunto sala deve ter somente 1 sala! \n";
				}

				int salaId = cjtSala->salas.begin()->first;
				int unidadeId = cjtSala->salas.begin()->second->getIdUnidade();
				Unidade *unidade = problemData->refUnidade[ unidadeId ];

				if ( cjtSala->getSala()->getIdCampus() != campusId )
				{
					continue;
				}

				if ( debug )
				std::cout<<"\n\tsalaId "<<salaId; fflush(NULL);

				std::map< int /*dia*/, std::map< DateTime, std::map< DateTime,  
					std::pair<HorarioAula*, HorarioAula*> > > > *mapDiaDtiDtf = & itCjtSalaDiaDtiDtf->second;

				std::map< int /*dia*/, std::map< DateTime, std::map< DateTime,  
					std::pair<HorarioAula*, HorarioAula*> > > >::iterator itDia = mapDiaDtiDtf->begin();					
				for ( ; itDia != mapDiaDtiDtf->end(); itDia++ )
				{
					int dia = itDia->first;
							
					if ( debug )						
						std::cout<<"\n\t\tDia "<<dia; fflush(NULL);

					std::map< DateTime, std::map< DateTime, std::pair<HorarioAula*, HorarioAula*> > >
						*mapDti = & itDia->second;

					std::map< DateTime, std::map< DateTime, std::pair<HorarioAula*, HorarioAula*> > >::iterator
						itMapDti = mapDti->begin();
					for ( ; itMapDti != mapDti->end(); itMapDti++ )
					{
						DateTime dti = itMapDti->first;

						std::map< DateTime, std::pair<HorarioAula*, HorarioAula*> > 
							*mapDtf = & itMapDti->second;

						std::map< DateTime, std::pair<HorarioAula*, HorarioAula*> >::iterator
							itMapDtf = mapDtf->begin();
						for ( ; itMapDtf != mapDtf->end(); itMapDtf++ )
						{
							DateTime dtf = itMapDtf->first;

							std::pair<HorarioAula*, HorarioAula*> parHiHf = itMapDtf->second;
								
							HorarioAula *hi = parHiHf.first;
							HorarioAula *hf = parHiHf.second;
									 					
							if ( debug )
							std::cout<<"\n\t\t\tHi "<<hi->getId() << "   Hf "<<hf->getId(); fflush(NULL);							
								
							ITERA_GGROUP_N_PT( itTurma, turmasParaAbrir, int )
							{									
								int turma = *itTurma;
									
								if ( debug )
									std::cout<<"\n\t\t\t\tTurma  "<< turma; fflush(NULL);
										
								VariableTatInt v;
								v.reset();
								v.setType( VariableTatInt::V_CREDITOS );

								v.setTurma( turma );            // i
								v.setDisciplina( disciplina );  // d
								v.setUnidade( unidade );     // u
								v.setSubCjtSala( cjtSala );  // tps  
								v.setDia( dia );				 // t
								v.setHorarioAulaInicial( hi );	 // hi
								v.setHorarioAulaFinal( hf );	 // hf

								if ( debug )
									std::cout<<"... Procurando hi..."; fflush(NULL);

								std::pair<DateTime*,int> auxP = problemData->horarioAulaDateTime[hi->getId()];
								v.setDateTimeInicial(auxP.first);

								if ( debug )
									std::cout<<"... Procurando hf..."; fflush(NULL);

								auxP = problemData->horarioAulaDateTime[hf->getId()];
								v.setDateTimeFinal(auxP.first);

								if ( vHashTatico.find( v ) == vHashTatico.end() )
								{
									bool fixar=false;
									if ( debug )
										std::cout<<". Vou criar?"; fflush(NULL);

									if ( !criaVariavelTaticoInt( &v, fixar, P ) )
									{
										if ( debug )
											std::cout<<" Nao!"; fflush(NULL);

										continue;
									}

									if ( debug )
										std::cout<<"Sim!"; fflush(NULL);

									vHashTatico[ v ] = lp->getNumCols();
									
									double lowerBound = 0.0;
									double upperBound = 1.0;

									if ( fixar ) lowerBound = 1.0;

									OPT_COL col( OPT_COL::VAR_BINARY, 0.0, lowerBound, upperBound,
										( char * )v.toString().c_str());

									lp->newCol( col );
									numVars++;
								}
							}
						}
					}
				}
			}
		if ( debug )
			std::cout<<"\nFim!"; fflush(NULL);
		}
		#pragma endregion
	}

	return numVars;
}


// x_{i,d,u,s,hi,hf,t}
int TaticoIntAlunoHor::criaVariavelTaticoCreditosComSolInicial( int campusId, int P )
{
	int numVars = 0;

	// CRIA TODAS AS VARIAVEIS X QUE JÁ EXISTEM OLHANDO PARA A SOLUÇÃO ANTERIOR
	if ( this->CRIAR_VARS_FIXADAS )
	{
		criaVariavelTaticoCreditosCopiadas( campusId, P, numVars );
	}

	std::cout<<"\nCriei "<<numVars<<" variaveis x, copiadas da solucao anterior.\n"; fflush(NULL);

	// CRIA TODAS AS VARIAVEIS X DE TURMAS QUE AINDA NÃO EXISTEM
	
	if ( this->PERMITIR_NOVAS_TURMAS )
	{
		ITERA_GGROUP_LESSPTR( itCampus, problemData->campi, Campus )
		{
		   if ( itCampus->getId() != campusId )
		   {
			   continue;
		   }

		   ITERA_GGROUP_LESSPTR( it_disciplina, problemData->disciplinas, Disciplina )
		   {
				Disciplina *disciplina = ( *it_disciplina );
			 
				bool debug = false;
				//if ( disciplina->getId()==12705 )
				//	debug = true;
												    
				if ( debug )
				std::cout<<"\nDisc "<<disciplina->getId(); fflush(NULL);

				if ( this->mapDiscAlunosDemanda[disciplina->getId()].size() == 0 )
					continue;
				
				for ( int turma = 1; turma <= disciplina->getNumTurmas(); turma++ )
				{			
					if ( debug )
						std::cout<<"\n\tTurma  "<< turma; fflush(NULL);
	
					bool turmaExiste = problemData->existeTurmaDiscCampus( turma, disciplina->getId(), campusId );
					if ( ( turmaExiste ) || 
						 (!turmaExiste && !this->PERMITIR_NOVAS_TURMAS) )
					{
						// variavel já foi criada acima, olhando para a solução anterior;
						// não permite novas turmas;
						continue;
					}

					GGroup< ConjuntoSala*, LessPtr<ConjuntoSala> > cjtSalasPossiveis;
					std::map< int /*Dia*/, std::pair<HorarioAula*, HorarioAula*> > mapDiaHorariosIF;
					
					bool fixaSala = false;
					bool fixaDias = false;
					bool fixaHorarios = false;

					GGroup<Aula*, LessPtr<Aula>> aulasSolInicial = 
						problemData->getSolTaticoInicial()->getAulas( campusId, disciplina, turma );
					
					if ( aulasSolInicial.size() > 0 )		// Com solucao inicial
					{
						ITERA_GGROUP_LESSPTR( itAula, aulasSolInicial, Aula )
						{
							Aula *aula = *itAula;

							if ( aula->fixaAbertura() )
							{
								if ( aula->fixaSala() )
								{
									cjtSalasPossiveis.add( aula->getCjtSala() );
									fixaSala = true;
								}
								if ( aula->fixaDia() || fixaDias ) // Ou todos os dias serão fixados ou nenhum será
								{
									mapDiaHorariosIF[ aula->getDiaSemana() ] =
										std::make_pair( aula->getHorarioAulaInicial(), aula->getHorarioAulaFinal() );
									fixaDias = true;
								}
								if ( aula->fixaHi() && aula->fixaHf() || fixaHorarios ) // Ou todos os horários serão fixados ou nenhum será
								{
									mapDiaHorariosIF[ aula->getDiaSemana() ] = 
										std::make_pair( aula->getHorarioAulaInicial(), aula->getHorarioAulaFinal());	
									fixaHorarios = true;
								}
							}
						}
					}
					
					if ( !fixaSala )
					{
						std::map< int, ConjuntoSala* >::iterator itMap = disciplina->cjtSalasAssociados.begin();
						for ( ; itMap != disciplina->cjtSalasAssociados.end(); itMap++ )
							cjtSalasPossiveis.add( (*itMap).second );
					}

					ITERA_GGROUP_LESSPTR( itCjtSala, (cjtSalasPossiveis), ConjuntoSala )
					{
						 ConjuntoSala* cjtSala = *itCjtSala;
						 Sala* sala = cjtSala->salas.begin()->second;
						 Unidade *unidade = problemData->refUnidade[ sala->getIdUnidade() ];

						 int salaId = cjtSala->salas.begin()->first;
				 
						 std::pair< int, int > parDiscSala = std::make_pair( disciplina->getId(), salaId );

						 std::map< std::pair< int /*idDisc*/, int /*idSala*/ >, 
								 std::map< int /*Dia*/, GGroup< HorarioAula *, LessPtr< HorarioAula > > > >::iterator
							it_Disc_Sala_Dias_HorariosAula = problemData->disc_Salas_Dias_HorariosAula.find( parDiscSala );
						 if ( it_Disc_Sala_Dias_HorariosAula == 
							 problemData->disc_Salas_Dias_HorariosAula.end() )
						 {
							 if ( fixaSala )
								 std::cout << "\nErro: aula com sala fixada, mas sem dias e horarios possiveis na associacao disc-sala."
								 << "\nCampus " << campusId << "\nTurma " << turma << " Disciplina " 
								 << disciplina->getId() << " sala " << salaId;
							 continue;
						 }						 					 

						 if ( !fixaHorarios )
						 {
							 std::map< int /*Dia*/, GGroup< HorarioAula *, LessPtr< HorarioAula > > >::iterator
						 		 itDiaHor = it_Disc_Sala_Dias_HorariosAula->second.begin();					
							 for ( ; itDiaHor != it_Disc_Sala_Dias_HorariosAula->second.end(); itDiaHor++ )
							 {
								int dia = itDiaHor->first;
							
								if ( fixaDias && mapDiaHorariosIF.find(dia) == mapDiaHorariosIF.end() )
									continue;

								if ( debug )						
									std::cout<<"\n\t\tDia "<<dia; fflush(NULL);
								
								// Usa todas as opções de horários achados na associação disc-sala.

								ITERA_GGROUP_LESSPTR( itHorarioI, itDiaHor->second, HorarioAula )
								{
									HorarioAula *hi = *itHorarioI;

									ITERA_GGROUP_INIC_LESSPTR( itHorarioF, itHorarioI, itDiaHor->second, HorarioAula )
									{
											HorarioAula *hf = *itHorarioF;
									 					
											if ( debug )
											std::cout<<"\n\t\t\tHi "<<hi->getId() << "   Hf "<<hf->getId(); fflush(NULL);

											if ( ! disciplina->inicioTerminoValidos( hi, hf, dia, itDiaHor->second ) )
												continue;								 

											VariableTatInt v;
											v.reset();
											v.setType( VariableTatInt::V_CREDITOS );

											v.setTurma( turma );            // i
											v.setDisciplina( disciplina );  // d
											v.setUnidade( unidade );     // u
											v.setSubCjtSala( cjtSala );  // tps  
											v.setDia( dia );				 // t
											v.setHorarioAulaInicial( hi );	 // hi
											v.setHorarioAulaFinal( hf );	 // hf

											if ( debug )
												std::cout<<"... Procurando hi..."; fflush(NULL);

											std::pair<DateTime*,int> auxP = problemData->horarioAulaDateTime[hi->getId()];
											v.setDateTimeInicial(auxP.first);

											if ( debug )
												std::cout<<"... Procurando hf..."; fflush(NULL);

											auxP = problemData->horarioAulaDateTime[hf->getId()];
											v.setDateTimeFinal(auxP.first);

											if ( vHashTatico.find( v ) == vHashTatico.end() )
											{
												bool fixar=false;
												
												if ( debug )
													std::cout<<". Vou criar?"; fflush(NULL);

												if ( !criaVariavelTaticoInt( &v, fixar, P ) )
													continue;
									 
												if ( debug )
													std::cout<<"Sim!"; fflush(NULL);

												vHashTatico[ v ] = lp->getNumCols();
									
												double lowerBound = 0.0;
												double upperBound = 1.0;

												if ( fixar ) lowerBound = 1.0;

												OPT_COL col( OPT_COL::VAR_BINARY, 0.0, lowerBound, upperBound,
													( char * )v.toString().c_str());

												lp->newCol( col );
												numVars++;
											}
									}
								}
							 }
						 }

						 if ( fixaHorarios )
						 {
							// Usa somente os horários fixados da solução inicial.
							// Fixa dias tb, obrigatoriamente.

							std::map< int /*Dia*/, std::pair<HorarioAula*, HorarioAula*> >::iterator
								itMapSolDia = mapDiaHorariosIF.begin();
							for ( ; itMapSolDia != mapDiaHorariosIF.end(); itMapSolDia++ )
							{									
								int dia = itMapSolDia->first;
								HorarioAula *hi = itMapSolDia->second.first;
								HorarioAula *hf = itMapSolDia->second.second;
																		 					
								if ( debug )
								std::cout<<"\n\t\t\tHi "<<hi->getId() << "   Hf "<<hf->getId(); fflush(NULL);
																
								VariableTatInt v;
								v.reset();
								v.setType( VariableTatInt::V_CREDITOS );
								v.setTurma( turma );            // i
								v.setDisciplina( disciplina );  // d
								v.setUnidade( unidade );		// u
								v.setSubCjtSala( cjtSala );		// tps  
								v.setDia( dia );				// t
								v.setHorarioAulaInicial( hi );	// hi
								v.setHorarioAulaFinal( hf );	// hf

								if ( debug )
									std::cout<<"... Procurando hi..."; fflush(NULL);

								std::pair<DateTime*,int> auxP = problemData->horarioAulaDateTime[hi->getId()];
								v.setDateTimeInicial(auxP.first);

								if ( debug )
									std::cout<<" Procurando hf..."; fflush(NULL);

								auxP = problemData->horarioAulaDateTime[hf->getId()];
								v.setDateTimeFinal(auxP.first);

								if ( vHashTatico.find( v ) == vHashTatico.end() )
								{
									bool fixar=false;
									
									if ( debug )
										std::cout<<"\tCriei!"; fflush(NULL);

									vHashTatico[ v ] = lp->getNumCols();
									
									double lowerBound = 0.0;
									double upperBound = 1.0;
									
									OPT_COL col( OPT_COL::VAR_BINARY, 0.0, lowerBound, upperBound,
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

	return numVars;
}


// o_{i,d,u,s}
int TaticoIntAlunoHor::criaVariavelTaticoOferecimentosAPartirDeX( int campusId, int P )
{
	int numVars = 0;
	   
	VariableTatIntHash varHashAux;
	 
	std::map< int, Trio<double, double, double> > varId_Bounds;

	VariableTatIntHash::iterator vit = vHashTatico.begin();
	for ( ; vit != vHashTatico.end(); vit++)
	{		
		VariableTatInt x = vit->first;
		if( x.getType() != VariableTatInt::V_CREDITOS )
		{
			continue;
		}

		int turma = x.getTurma();
		Disciplina* disciplina = x.getDisciplina();
		Unidade *unidade = x.getUnidade();
		ConjuntoSala *cjtSala = x.getSubCjtSala();
			
		VariableTatInt v;
		v.reset();
        v.setType( VariableTatInt::V_OFERECIMENTO );
        v.setTurma( turma );            // i
        v.setDisciplina( disciplina );  // d
        v.setUnidade( unidade );     // u
        v.setSubCjtSala( cjtSala );  // tps

		if ( varHashAux.find( v ) == varHashAux.end() )
		{
			bool fixar=false;

			if ( !criaVariavelTaticoInt( &v, fixar, P ) )
			{
				std::cout<<"\nEstranho!! Tem " << x.toString() << " e nao tem o?";
				continue;
			}
						
			int id = numVars;
			varHashAux[ v ] = id;
				
			double coef = 0.0;					
			double lowerBound = 0.0;
			double upperBound = 1.0;

			if ( fixar ) lowerBound = 1.0;
				
			Trio<double, double, double> trio;
			trio.set( coef,lowerBound, upperBound );

			varId_Bounds[ id ] = trio;
				
			numVars++;
		}
    }
    
	// Insere todas as variaveis em vHashTatico e no lp	
	for ( vit = varHashAux.begin(); vit != varHashAux.end(); vit++)
	{		
		VariableTatInt v = vit->first;
		int id = vit->second;

		vHashTatico[ v ] = lp->getNumCols();
		
		double coef = varId_Bounds[id].first;
		double lowerBound = varId_Bounds[id].second;
		double upperBound = varId_Bounds[id].third;

		OPT_COL col( OPT_COL::VAR_BINARY, coef, lowerBound, upperBound,
			( char * )v.toString().c_str());

		lp->newCol( col );
	}

	return numVars;
}

// o_{i,d,u,s}
int TaticoIntAlunoHor::criaVariavelTaticoOferecimentos( int campusId, int P )
{
	int numVars = 0;
	   
	std::map<Disciplina*, bool, LessPtr<Disciplina> > mapDiscAlunosDemandaBool;
	if ( problemData->parametros->considerar_equivalencia_por_aluno && USAR_EQUIVALENCIA ) 
	{		
		ITERA_GGROUP_LESSPTR( itDisc, problemData->disciplinas, Disciplina )
			mapDiscAlunosDemandaBool[*itDisc] = problemData->haDemandasDiscNoCampus_Equiv( (*itDisc)->getId(), campusId, P );
	}
	else
	{
		ITERA_GGROUP_LESSPTR( itDisc, problemData->disciplinas, Disciplina )
			mapDiscAlunosDemandaBool[*itDisc] = problemData->haDemandaDiscNoCampus( (*itDisc)->getId(), campusId );
	}

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
               Disciplina * disciplina = ( *it_disc );

			   #pragma region Equivalencias
				if ( ( problemData->mapDiscSubstituidaPor.find( disciplina ) !=
						problemData->mapDiscSubstituidaPor.end() ) &&
						!problemData->ehSubstituta( disciplina ) )
			   {
				   continue;
			   }
			   #pragma endregion
											    
				if ( ! mapDiscAlunosDemandaBool[disciplina] )
					continue;

				int discGrupoAlunosId = problemData->retornaCjtAlunosId( disciplina->getId() );

               for ( int turma = 1; turma <= disciplina->getNumTurmas(); turma++ )
               {
                    // Dada uma disciplina, recupera os seus dias letivos, observando as fixações
                    GGroup< int > dias_letivos = itCjtSala->dias_letivos_disciplinas[ ( disciplina ) ];

                    GGroup< int >::iterator itDiscSala_Dias = dias_letivos.begin();

                     VariableTatInt v;
                     v.reset();
                     v.setType( VariableTatInt::V_OFERECIMENTO );

                     v.setTurma( turma );            // i
                     v.setDisciplina( disciplina );  // d
                     v.setUnidade( *itUnidade );     // u
                     v.setSubCjtSala( *itCjtSala );  // tps

                     if ( vHashTatico.find( v ) == vHashTatico.end() )
                     {                        
						bool fixar=false;

						if ( !criaVariavelTaticoInt( &v, fixar, P ) )
							continue;

						vHashTatico[ v ] = lp->getNumCols();
									
						double lowerBound = 0.0;
						double upperBound = 1.0;
						double custo = 0.0;

						if ( fixar ) lowerBound = 1.0;						
						
                        OPT_COL col( OPT_COL::VAR_BINARY, custo, lowerBound, upperBound, ( char* )v.toString().c_str() );
                          
						lp->newCol( col );

						numVars++;						  
                     }
                }
            }
         }
      }
   }

	return numVars;
}

// s_{i,d,cp,a}
int TaticoIntAlunoHor::criaVariavelTaticoAlocaAlunoTurmaDiscAPartirDeV( int campusId, int P )
{
	int numVars = 0;
	   
	VariableTatIntHash varHashAux;
	 
	std::map< int, Trio<double, double, double> > varId_Bounds;

	VariableTatIntHash::iterator vit = vHashTatico.begin();
	for ( ; vit != vHashTatico.end(); vit++)
	{		
		VariableTatInt v = vit->first;
		if( v.getType() != VariableTatInt::V_ALUNO_CREDITOS )
		{
			continue;
		}

		int turma = v.getTurma();
		Disciplina* disciplina = v.getDisciplina();
		Unidade *unidade = v.getUnidade();
		Aluno *aluno = v.getAluno();

		Campus *cp = problemData->refCampus[ unidade->getIdCampus() ];
		AlunoDemanda *alDem = //aluno->getAlunoDemandaEquiv( disciplina );
			problemData->getAlunoDemandaEquiv( aluno, disciplina );

		if ( alDem == NULL )
		{
			std::cout << "\nErro: aluno-demanda nao encontrado. Disciplina " 
				<< disciplina->getId() << " Aluno " << aluno->getAlunoId();
		}


		if ( disciplina->getId() < 0 )
		{
			// Só cria associação entre aluno e turma pratica se essa turma possuir
			// turma teorica associada.
			if ( disciplina->getTurmasAssociadas( turma ).size() == 0 )
			{
				continue;
			}
		}

		VariableTatInt s;
		s.reset();
        s.setType( VariableTatInt::V_ALOCA_ALUNO_TURMA_DISC );
		s.setAluno( aluno );
		s.setDisciplina( disciplina );
		s.setTurma( turma );
		s.setCampus( cp );
		s.setAlunoDemanda( alDem );

		if ( varHashAux.find( s ) == varHashAux.end() )
		{
			bool fixar=false;

			if ( !criaVariavelTaticoInt( &s, fixar, P ) )
			{
				std::cout<<"\nEstranho!! Tem " << v.toString() << " e nao tem s? Vou criar mesmo assim.";
				//continue;
			}
						
			int id = numVars;
			varHashAux[ s ] = id;
				
			double coef = 0.0;					
			double lowerBound = 0.0;
			double upperBound = 1.0;

			if ( fixar ) lowerBound = 1.0;
				
			Trio<double, double, double> trio;
			trio.set( coef,lowerBound, upperBound );

			varId_Bounds[ id ] = trio;
				
			numVars++;
		}
    }
    
	// Insere todas as variaveis em vHashTatico e no lp	
	for ( vit = varHashAux.begin(); vit != varHashAux.end(); vit++)
	{		
		VariableTatInt v = vit->first;
		int id = vit->second;

		vHashTatico[ v ] = lp->getNumCols();
		
		double coef = varId_Bounds[id].first;
		double lowerBound = varId_Bounds[id].second;
		double upperBound = varId_Bounds[id].third;

		OPT_COL col( OPT_COL::VAR_BINARY, coef, lowerBound, upperBound,
			( char * )v.toString().c_str());

		lp->newCol( col );
	}

	return numVars;
}

// s_{i,d,a,cp}
int TaticoIntAlunoHor::criaVariavelTaticoAlocaAlunoTurmaDisc( int campusId, int P )
{
	int numVars = 0;

	if ( problemData->parametros->considerar_equivalencia_por_aluno && USAR_EQUIVALENCIA )
		return numVars;

	Campus *cp = problemData->refCampus[campusId];
	
	ITERA_GGROUP_LESSPTR( itAluno, problemData->alunos, Aluno )
	{
		Aluno *aluno = *itAluno;

		ITERA_GGROUP_LESSPTR( itAlDemanda, aluno->demandas, AlunoDemanda )
		{
			if ( itAlDemanda->demanda->oferta->getCampusId() != campusId )
			{
				continue;
			}

			Disciplina *disciplina = (*itAlDemanda)->demanda->disciplina;
	
			if ( aluno->totalmenteAtendido() )
			if ( problemData->retornaTurmaDiscAluno(aluno, disciplina) == -1 )
				continue;

			for ( int turma = 1; turma <= disciplina->getNumTurmas(); turma++ )
			{
				VariableTatInt v;
				v.reset();
				v.setType( VariableTatInt::V_ALOCA_ALUNO_TURMA_DISC );
				v.setAluno( aluno );
				v.setDisciplina( disciplina );
				v.setTurma( turma );
				v.setCampus( cp );
				v.setAlunoDemanda( *itAlDemanda );

				if ( vHashTatico.find( v ) == vHashTatico.end() )
				{
					bool fixar=false;

					if ( !criaVariavelTaticoInt( &v, fixar, P ) )
						continue;

					vHashTatico[ v ] = lp->getNumCols();
									
					double lowerBound = 0.0;
					double upperBound = 1.0;
					double coef = 0.0;		
					
					if ( fixar ) lowerBound = 1.0;		
										
					OPT_COL col( OPT_COL::VAR_BINARY, coef, lowerBound, upperBound, ( char * )v.toString().c_str() );

					lp->newCol( col );
					numVars++;
				}
			}
		}
	}

	return numVars;
}

// b_{i,d,c,cp}
int TaticoIntAlunoHor::criaVariavelTaticoCursoAlunos( int campusId, int P )
{
	int numVars = 0;
   
	ITERA_GGROUP_LESSPTR( it_Disc, problemData->disciplinas, Disciplina )
    {
		 Disciplina * disciplina = *it_Disc;

		 GGroup<AlunoDemanda*, LessPtr<AlunoDemanda>> alunosdemanda = this->mapDiscAlunosDemanda[disciplina->getId()];
		 
		 ITERA_GGROUP_LESSPTR( it_AlDem, alunosdemanda, AlunoDemanda )
		 {
			 Campus * pt_Campus = it_AlDem->demanda->oferta->campus;
			 Curso * pt_Curso = it_AlDem->demanda->oferta->curso;
		
			 if ( pt_Campus->getId() != campusId )
			 {
				 continue;
			 }

			 for ( int turma = 1; turma <= disciplina->getNumTurmas(); turma++ )
			 {
				VariableTatInt v;
				v.reset();
				v.setType( VariableTatInt::V_TURMA_ATEND_CURSO );

				v.setTurma( turma );           // i
				v.setDisciplina( disciplina ); // d
				v.setCurso( pt_Curso );        // c
				v.setCampus( pt_Campus );	    // cp

				if ( vHashTatico.find( v ) == vHashTatico.end() )
				{
					bool fixar=false;

					if ( !criaVariavelTaticoInt( &v, fixar, P ) )
						continue;

					vHashTatico[ v ] = lp->getNumCols();
									
					double lowerBound = 0.0;
					double upperBound = 1.0;

					if ( fixar ) lowerBound = 1.0;

					double coef=0.0;
					if ( problemData->parametros->funcao_objetivo == 0 )
					{
						coef = -1.0;
					}
					else if ( problemData->parametros->funcao_objetivo == 1 )
					{
						coef = 1.0;
					}

				   OPT_COL col( OPT_COL::VAR_BINARY, coef, lowerBound, upperBound,
					  ( char * )v.toString().c_str() );

				   lp->newCol( col );
				   numVars++;
				}
			 }
		 }
    }
	
	return numVars;
}

// c_{i,d,t,cp}
int TaticoIntAlunoHor::criaVariavelTaticoConsecutivosAPartirDeX( int campusId, int P )
{
	int numVars = 0;
	   
	Campus * cp = problemData->refCampus[campusId];
	
	VariableTatIntHash varHashAux;	
	std::map< int, Trio<double, double, double> > varId_Bounds;

	VariableTatIntHash::iterator vit = vHashTatico.begin();
	for ( ; vit != vHashTatico.end(); vit++)
	{		
		VariableTatInt v = vit->first;

		if( v.getType() != VariableTatInt::V_CREDITOS )
		{
			continue;
		}

		if ( v.getUnidade()->getIdCampus() != campusId )
			continue;

		int turma = v.getTurma();
		Disciplina* disciplina = v.getDisciplina();
		int dia = v.getDia();		

		v.reset();
        v.setType( VariableTatInt::V_DIAS_CONSECUTIVOS );
        v.setTurma( turma );            // i
        v.setDisciplina( disciplina );  // d
        v.setDia( dia );				// t
		v.setCampus( cp );				// cp

		if ( varHashAux.find( v ) == varHashAux.end() )
		{
			bool fixar=false;

			if ( !criaVariavelTaticoInt( &v, fixar, P ) )
			{
				continue;
			}
						
			int id = numVars;
			varHashAux[ v ] = id;
									
			double lowerBound = 0.0;
			double upperBound = 1.0;

			if ( fixar ) lowerBound = 1.0;
			
			double coef = 0.0;
			if ( problemData->parametros->funcao_objetivo == 0 )
			{
				coef = -1.0;
			}
			else if ( problemData->parametros->funcao_objetivo == 1 )
			{
				coef = 1.0;
			}

			Trio<double, double, double> trio;
			trio.set( coef,lowerBound, upperBound );

			varId_Bounds[ id ] = trio;
				
			numVars++;
		}
    }
    
	// Insere todas as variaveis em vHashTatico e no lp	
	for ( vit = varHashAux.begin(); vit != varHashAux.end(); vit++)
	{		
		VariableTatInt v = vit->first;
		int id = vit->second;

		vHashTatico[ v ] = lp->getNumCols();
		
		double coef = varId_Bounds[id].first;
		double lowerBound = varId_Bounds[id].second;
		double upperBound = varId_Bounds[id].third;

		OPT_COL col( OPT_COL::VAR_BINARY, coef, lowerBound, upperBound,
			( char * )v.toString().c_str());

		lp->newCol( col );
	}

	return numVars;
}


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
int TaticoIntAlunoHor::criaVariavelTaticoConsecutivos( int campusId, int P )
{
	int numVars = 0;

    Campus * cp = problemData->refCampus[campusId];
		
	std::map<Disciplina*, bool, LessPtr<Disciplina> > mapDiscAlunosDemandaBool;
	if ( problemData->parametros->considerar_equivalencia_por_aluno && USAR_EQUIVALENCIA ) 
	{		
		ITERA_GGROUP_LESSPTR( itDisc, problemData->disciplinas, Disciplina )
			mapDiscAlunosDemandaBool[*itDisc] = problemData->haDemandasDiscNoCampus_Equiv( (*itDisc)->getId(), campusId, P );
	}
	else
	{
		ITERA_GGROUP_LESSPTR( itDisc, problemData->disciplinas, Disciplina )
			mapDiscAlunosDemandaBool[*itDisc] = problemData->haDemandaDiscNoCampus( (*itDisc)->getId(), campusId );
	}
		
	GGroup< int > disciplinasIds = problemData->cp_discs[campusId];

	ITERA_GGROUP_N_PT( itDiscId, disciplinasIds, int )
	{
		Disciplina* disciplina = problemData->refDisciplinas[ *itDiscId ];
			   	
		#pragma region Equivalencias
		if ( ( problemData->mapDiscSubstituidaPor.find( disciplina ) !=
				problemData->mapDiscSubstituidaPor.end() ) &&
				!problemData->ehSubstituta( disciplina ) )
		{
			continue;
		}
		#pragma endregion
									    
		if ( ! mapDiscAlunosDemandaBool[disciplina] )
			continue;
		
        for ( int turma = 1; turma <= disciplina->getNumTurmas(); turma++ )
        {       
			ITERA_GGROUP_N_PT( itDiasLetDisc, disciplina->diasLetivos, int )
            {
                VariableTatInt v;
                v.reset();
                v.setType( VariableTatInt::V_DIAS_CONSECUTIVOS );

                v.setTurma( turma );            // i
                v.setDisciplina( disciplina );  // d
				v.setCampus( cp );		 // cp
                v.setDia( *itDiasLetDisc );     // t

                if ( vHashTatico.find( v ) == vHashTatico.end() )
                {
					bool fixar=false;
	
					if ( !criaVariavelTaticoInt( &v, fixar, P ) )
					{
						continue;
					}
					
					vHashTatico[ v ] = lp->getNumCols();
									
					double lowerBound = 0.0;
					double upperBound = 1.0;
					if ( fixar ) lowerBound = 1.0;
					    
					double coef = 0.0;
					if ( problemData->parametros->funcao_objetivo == 0 )
					{
						coef = -1.0;
					}
					else if ( problemData->parametros->funcao_objetivo == 1 )
					{
						coef = 1.0;
					}

					OPT_COL col( OPT_COL::VAR_BINARY, coef, lowerBound, upperBound, ( char * )v.toString().c_str() );
						                           
					lp->newCol( col );

					numVars++;
                }
            }
        }
    }
           
	return numVars;
}

// m_{d,i,k}
int TaticoIntAlunoHor::criaVariavelTaticoCombinacaoDivisaoCreditoAPartirDeO( int campusId, int P )
{
	int numVars = 0;
	   
	Campus *campus = problemData->refCampus[campusId];

	VariableTatIntHash varHashAux;
	std::map< int, Trio<double, double, double> > varId_Bounds;

	VariableTatIntHash::iterator vit = vHashTatico.begin();
	for ( ; vit != vHashTatico.end(); vit++)
	{		
		VariableTatInt v = vit->first;

		if( v.getType() != VariableTatInt::V_OFERECIMENTO )
		{
			continue;
		}

		int turma = v.getTurma();
		Disciplina* disciplina = v.getDisciplina();
				
        for ( unsigned k = 0; k < disciplina->combinacao_divisao_creditos.size(); k++ )
        {
			VariableTatInt v;
			v.reset();
			v.setType( VariableTatInt::V_COMBINACAO_DIVISAO_CREDITO );
			v.setTurma( turma );            // i
			v.setDisciplina( disciplina );  // d
			v.setK( k );	                // k
            v.setCampus( campus );			// cp

			if ( varHashAux.find( v ) == varHashAux.end() )
			{
				bool fixar=false;

				//if ( !criaVariavelTaticoInt( &v, fixar, P ) )
				//{
				//	continue;
				//}
						
				int id = numVars;
				varHashAux[ v ] = id;
				
				double coef = 0.0;					
				double lowerBound = 0.0;
				double upperBound = 1.0;

				if ( fixar ) lowerBound = 1.0;
				
				Trio<double, double, double> trio;
				trio.set( coef,lowerBound, upperBound );

				varId_Bounds[ id ] = trio;
				
				numVars++;
			}
		}
    }
    
	// Insere todas as variaveis em vHashTatico e no lp	
	for ( vit = varHashAux.begin(); vit != varHashAux.end(); vit++)
	{		
		VariableTatInt v = vit->first;
		int id = vit->second;

		vHashTatico[ v ] = lp->getNumCols();
		
		double coef = varId_Bounds[id].first;
		double lowerBound = varId_Bounds[id].second;
		double upperBound = varId_Bounds[id].third;

		OPT_COL col( OPT_COL::VAR_BINARY, coef, lowerBound, upperBound,
			( char * )v.toString().c_str());

		lp->newCol( col );
	}

	return numVars;
}

/*====================================================================/
%DocBegin TRIEDA_LOAD_MODEL

%Var m_{d,i,k} 

%Desc 
variável binária que indica se a combinação de divisão de créditos 
$k$ foi escolhida para a turma $i$ da disciplina $d$.

%DocEnd
/====================================================================*/

int TaticoIntAlunoHor::criaVariavelTaticoCombinacaoDivisaoCredito( int campusId, int P )
{
   int numVars = 0;

   Disciplina * disciplina = NULL;
  
   GGroup<int> disciplinas = problemData->cp_discs[campusId];

   ITERA_GGROUP_N_PT( it_disciplina, disciplinas, int )
   {
	   disciplina = problemData->refDisciplinas[ *it_disciplina ];

	  #pragma region Equivalencias
	  if ( ( problemData->mapDiscSubstituidaPor.find( disciplina ) !=
			problemData->mapDiscSubstituidaPor.end() ) &&
			!problemData->ehSubstituta( disciplina ) )
	  {
		  continue;
	  }
	  #pragma endregion

		if ( problemData->parametros->considerar_equivalencia_por_aluno && USAR_EQUIVALENCIA )
		{
			if ( !problemData->haDemandasDiscNoCampus_Equiv( disciplina->getId(), campusId, P ) )
				continue;
		}
		else
		{
			if ( !problemData->haDemandaDiscNoCampus( disciplina->getId(), campusId ) )
				continue;
		}
		
      for ( int turma = 1; turma <= disciplina->getNumTurmas(); turma++ )
      {
         for ( unsigned k = 0; k < disciplina->combinacao_divisao_creditos.size(); k++ )
         { 
            VariableTatInt v;
            v.reset();
            v.setType( VariableTatInt::V_COMBINACAO_DIVISAO_CREDITO );

            v.setTurma( turma );           // i
            v.setDisciplina( disciplina ); // d
            v.setK( k );	               // k
			v.setCampus( problemData->refCampus[campusId] );

            if ( vHashTatico.find( v ) == vHashTatico.end() )
            {
				bool fixar=false;

				if ( !criaVariavelTaticoInt( &v, fixar, P ) )
					continue;

				vHashTatico[ v ] = lp->getNumCols();
									
				double lowerBound = 0.0;
				double upperBound = 1.0;

				if ( fixar ) lowerBound = 1.0;
					    
                OPT_COL col( OPT_COL::VAR_BINARY, 0.0, lowerBound, upperBound, ( char * )v.toString().c_str() );

                lp->newCol( col );
                numVars++;
            }
         }
      }
   }

   return numVars;
}

// fkp_{d,i,t} e fkm_{d,i,t}
int TaticoIntAlunoHor::criaVariavelTaticoFolgaCombinacaoDivisaoCreditoAPartirDeO( int campusId, int P )
{
	int numVars = 0;
	   
	Campus *campus = problemData->refCampus[campusId];

	VariableTatIntHash varHashAux;
	std::map< int, Trio<double, double, double> > varId_Bounds;

	VariableTatIntHash::iterator vit = vHashTatico.begin();
	for ( ; vit != vHashTatico.end(); vit++)
	{		
		VariableTatInt v = vit->first;

		if( v.getType() != VariableTatInt::V_OFERECIMENTO )
		{
			continue;
		}

		int turma = v.getTurma();
		Disciplina* disciplina = v.getDisciplina();
				
		double lowerBound = 0.0;
		double upperBound = 100.0;												
		double coef = 0.0;
		if ( problemData->parametros->funcao_objetivo == 0 )
		{
			coef = -campus->getCusto()/2;
		}
		else if ( problemData->parametros->funcao_objetivo == 1 )
		{
			coef = campus->getCusto()/2;
		}
							
		Trio<double, double, double> trio;
		trio.set( coef,lowerBound, upperBound );

		ITERA_GGROUP_N_PT( itDia, disciplina->diasLetivos, int )
        {	
			int dia = *itDia;

			v.reset();
			v.setType( VariableTatInt::V_SLACK_COMBINACAO_DIVISAO_CREDITO_P );
			v.setTurma( turma );            // i
			v.setDisciplina( disciplina );  // d
			v.setDia( dia );				// t
			v.setCampus( campus );				// cp

			if ( varHashAux.find( v ) == varHashAux.end() )
			{
				bool fixar=false;
			//	if ( criaVariavelTaticoInt( &v, fixar, P ) )
			//	{						
					int id = numVars;
					varHashAux[ v ] = id;
					varId_Bounds[ id ] = trio;				
					numVars++;
			//	}
			}

			v.setType( VariableTatInt::V_SLACK_COMBINACAO_DIVISAO_CREDITO_M );
        
			if ( varHashAux.find( v ) == varHashAux.end() )
			{
				bool fixar=false;
			//	if ( criaVariavelTaticoInt( &v, fixar, P ) )
			//	{						
					int id = numVars;
					varHashAux[ v ] = id;
					varId_Bounds[ id ] = trio;				
					numVars++;
			//	}
			}
		}
    }
    
	// Insere todas as variaveis em vHashTatico e no lp	
	for ( vit = varHashAux.begin(); vit != varHashAux.end(); vit++)
	{		
		VariableTatInt v = vit->first;
		int id = vit->second;

		vHashTatico[ v ] = lp->getNumCols();
		
		double coef = varId_Bounds[id].first;
		double lowerBound = varId_Bounds[id].second;
		double upperBound = varId_Bounds[id].third;

		OPT_COL col( OPT_COL::VAR_INTEGRAL, coef, lowerBound, upperBound,
			( char * )v.toString().c_str());

		lp->newCol( col );
	}

	return numVars;
}


/*====================================================================/
%DocBegin TRIEDA_LOAD_MODEL

%Var fkp_{d,i,t} 

%Desc 
variável de folga superior para a restrição de combinação de divisão de créditos.

%ObjCoef
\psi \cdot \sum\limits_{d \in D} 
\sum\limits_{t \in T} \sum\limits_{i \in I_{d}} fkp_{d,i,t}

%Data \psi
%Desc
peso associado a função objetivo.

%Var fkm_{d,i,t} 

%Desc 
variável de folga inferior para a restrição de combinação de divisão de créditos.

%ObjCoef
\psi \cdot \sum\limits_{d \in D} 
\sum\limits_{t \in T} \sum\limits_{i \in I_{d}} fkm_{d,i,t}

%Data \psi
%Desc
peso associado a função objetivo.

%DocEnd
/====================================================================*/

int TaticoIntAlunoHor::criaVariavelTaticoFolgaCombinacaoDivisaoCredito( int campusId, int P )
{
   int numVars = 0;

   Campus * cp = problemData->refCampus[campusId];

	std::map<Disciplina*, bool, LessPtr<Disciplina> > mapDiscAlunosDemandaBool;
	if ( problemData->parametros->considerar_equivalencia_por_aluno && USAR_EQUIVALENCIA ) 
	{		
		ITERA_GGROUP_LESSPTR( itDisc, problemData->disciplinas, Disciplina )
			mapDiscAlunosDemandaBool[*itDisc] = problemData->haDemandasDiscNoCampus_Equiv( (*itDisc)->getId(), campusId, P );
	}
	else
	{
		ITERA_GGROUP_LESSPTR( itDisc, problemData->disciplinas, Disciplina )
			mapDiscAlunosDemandaBool[*itDisc] = problemData->haDemandaDiscNoCampus( (*itDisc)->getId(), campusId );
	}

	GGroup< int > disciplinasIds = problemData->cp_discs[campusId];

	ITERA_GGROUP_N_PT( itDiscId, disciplinasIds, int )
	{
		Disciplina* disciplina = problemData->refDisciplinas[ *itDiscId ];
			   
		#pragma region Equivalencias
		if ( ( problemData->mapDiscSubstituidaPor.find( disciplina ) !=
				problemData->mapDiscSubstituidaPor.end() ) &&
				!problemData->ehSubstituta( disciplina ) )
		{
			continue;
		}
		#pragma endregion
											    
		if ( ! mapDiscAlunosDemandaBool[disciplina] )
			continue;
				
		for ( int turma = 1; turma <= disciplina->getNumTurmas(); turma++ )
		{			
			ITERA_GGROUP_N_PT( itDia, disciplina->diasLetivos, int )			
			{					  
				VariableTatInt v;
				v.reset();
				v.setType( VariableTatInt::V_SLACK_COMBINACAO_DIVISAO_CREDITO_P );

				v.setTurma( turma );            // i
				v.setDisciplina( disciplina );  // d
				v.setDia( *itDia );				// t
				v.setCampus( cp );		// cp
                     
				if ( vHashTatico.find( v ) == vHashTatico.end() )
				{
					bool fixar=false;

					if ( criaVariavelTaticoInt( &v, fixar, P ) )
					{
						vHashTatico[ v ] = lp->getNumCols();
									
						double lowerBound = 0.0;
						double upperBound = 10000.0;
												
						double coef = 0.0;

						if ( problemData->parametros->funcao_objetivo == 0 )
						{
							coef = -2*cp->getCusto();
						}
						else if ( problemData->parametros->funcao_objetivo == 1 )
						{
							coef = 2*cp->getCusto();
						}
                           
						OPT_COL col( OPT_COL::VAR_INTEGRAL, coef, lowerBound, upperBound,
								( char * )v.toString().c_str() );

						lp->newCol( col );

						numVars++;
					}
				}

				v.reset();
				v.setType( VariableTatInt::V_SLACK_COMBINACAO_DIVISAO_CREDITO_M );

				v.setTurma( turma );            // i
				v.setDisciplina( disciplina );  // d
				v.setDia( *itDia );				// t
				v.setCampus( cp );		// cp
					 
				if ( vHashTatico.find( v ) == vHashTatico.end() )
				{						
					bool fixar=false;

					if ( criaVariavelTaticoInt( &v, fixar, P ) )
					{						
						vHashTatico[ v ] = lp->getNumCols();
									
						double lowerBound = 0.0;
						double upperBound = 10000.0;
												
						double coef = 0.0;

						if ( problemData->parametros->funcao_objetivo == 0 )
						{
							coef = -2*cp->getCusto();
						}
						else if ( problemData->parametros->funcao_objetivo == 1 )
						{
							coef = 2*cp->getCusto();
						}
                           
						OPT_COL col( OPT_COL::VAR_INTEGRAL, coef, lowerBound, upperBound,
								( char * )v.toString().c_str() );
						                           
						lp->newCol( col );

						numVars++;
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
variável de folga superior para a restrição de fixação da distribuição de créditos por dia.
%ObjCoef
\xi \cdot \sum\limits_{d \in D} \sum\limits_{t \in T} fcp_{d,t}

%Data \xi
%Desc
peso associado a função objetivo.

%DocEnd
/====================================================================*/

int TaticoIntAlunoHor::criaVariavelTaticoFolgaDistCredDiaSuperior( int campusId, int P )
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
			   if ( ( problemData->mapDiscSubstituidaPor.find( disciplina ) !=
				 	  problemData->mapDiscSubstituidaPor.end() ) &&
					!problemData->ehSubstituta( disciplina ) )
			   {
				   continue;
			   }
			   #pragma endregion

				if ( problemData->parametros->considerar_equivalencia_por_aluno && USAR_EQUIVALENCIA )
				{
					if ( !problemData->haDemandasDiscNoCampus_Equiv( disciplina->getId(), campusId, P ) )
						continue;
				}
				else
				{
					if ( !problemData->haDemandaDiscNoCampus( disciplina->getId(), campusId ) )
						continue;
				}

               for ( int turma = 1; turma <= disciplina->getNumTurmas(); turma++ )
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
                           VariableTatInt v;
                           v.reset();
                           v.setType( VariableTatInt::V_SLACK_DIST_CRED_DIA_SUPERIOR );

                           v.setTurma( turma );
                           v.setDisciplina( disciplina );
                           v.setDia( *itDiasLetDisc );
                           v.setSubCjtSala( *itCjtSala );

                           if ( vHashTatico.find( v ) == vHashTatico.end() )
                           {
								bool fixar=false;

								if ( !criaVariavelTaticoInt( &v, fixar, P ) )
									continue;

								vHashTatico[ v ] = lp->getNumCols();
								
                                int cred_disc_dia = it_fix->disciplina->getMaxCreds();

								double lowerBound = 0.0;
								double upperBound = cred_disc_dia;
								
								double coef = 0.0;

								if ( problemData->parametros->funcao_objetivo == 0 )
								{
									coef = -itCampus->getCusto()/2;
								}
								else if ( problemData->parametros->funcao_objetivo == 1 )
								{
									coef = itCampus->getCusto()/2;
								}
               
								OPT_COL col( OPT_COL::VAR_INTEGRAL, coef, lowerBound, upperBound,
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
variável de folga inferior para a restrição de fixação da distribuição de créditos por dia.

%ObjCoef
\xi \cdot \sum\limits_{d \in D} \sum\limits_{t \in T} fcm_{d,t}

%DocEnd
/====================================================================*/

int TaticoIntAlunoHor::criaVariavelTaticoFolgaDistCredDiaInferior( int campusId, int P )
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
				if ( ( problemData->mapDiscSubstituidaPor.find( disciplina ) !=
						problemData->mapDiscSubstituidaPor.end() ) &&
						!problemData->ehSubstituta( disciplina ) )
			   {
				  continue;
			   }
			   #pragma endregion

				if ( problemData->parametros->considerar_equivalencia_por_aluno && USAR_EQUIVALENCIA )
				{
					if ( !problemData->haDemandasDiscNoCampus_Equiv( disciplina->getId(), campusId, P ) )
						continue;
				}
				else
				{
					if ( !problemData->haDemandaDiscNoCampus( disciplina->getId(), campusId ) )
						continue;
				}

               for ( int turma = 1; turma <= disciplina->getNumTurmas(); turma++ )
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
                           VariableTatInt v;
                           v.reset();
                           v.setType( VariableTatInt::V_SLACK_DIST_CRED_DIA_INFERIOR );

                           v.setTurma( turma );
                           v.setDisciplina( disciplina );
                           v.setDia( *itDiasLetDisc );
                           v.setSubCjtSala( *itCjtSala );

                           if ( vHashTatico.find( v ) == vHashTatico.end() )
                           {
								bool fixar=false;

								if ( !criaVariavelTaticoInt( &v, fixar, P ) )
									continue;

								vHashTatico[ v ] = lp->getNumCols();
								
								int cred_disc_dia = it_fix->disciplina->getMinCreds();
								
								double lowerBound = 0.0;
								double upperBound = cred_disc_dia;								
								
							    double coef = 0.0;

								if ( problemData->parametros->funcao_objetivo == 0 )
								{
									coef = -itCampus->getCusto()/2;
								}
								else if ( problemData->parametros->funcao_objetivo == 1 )
								{
									coef = itCampus->getCusto()/2;
								}               

								OPT_COL col( OPT_COL::VAR_INTEGRAL, coef, lowerBound, upperBound,
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

// zc_{d,t,cp}
int TaticoIntAlunoHor::criaVariavelTaticoAberturaCompativelAPartirDeX( int campusId, int P )
{
	int numVars = 0;
      
	if ( ! problemData->parametros->considerar_disciplinas_incompativeis_no_dia )
	   return numVars;
	
	Campus * cp = problemData->refCampus[campusId];
	
	VariableTatIntHash varHashAux;	
	std::map< int, Trio<double, double, double> > varId_Bounds;

	VariableTatIntHash::iterator vit = vHashTatico.begin();
	for ( ; vit != vHashTatico.end(); vit++)
	{		
		VariableTatInt v = vit->first;

		if( v.getType() != VariableTatInt::V_CREDITOS )
		{
			continue;
		}

		if ( v.getUnidade()->getIdCampus() != campusId )
			continue;

		Disciplina* disciplina = v.getDisciplina();
		int dia = v.getDia();

		v.reset();
        v.setType( VariableTatInt::V_ABERTURA_COMPATIVEL );        
        v.setDisciplina( disciplina );  // d
        v.setDia( dia );				// t
		v.setCampus( cp );				// cp

		if ( varHashAux.find( v ) == varHashAux.end() )
		{
			bool fixar=false;

			if ( !criaVariavelTaticoInt( &v, fixar, P ) )
			{
				continue;
			}
						
			int id = numVars;
			varHashAux[ v ] = id;
									
			double lowerBound = 0.0;
			double upperBound = 1.0;

			if ( fixar ) lowerBound = 1.0;
			
			double coef = 0.0;
			if ( problemData->parametros->funcao_objetivo == 0 )
			{
				coef = -1.0;
			}
			else if ( problemData->parametros->funcao_objetivo == 1 )
			{
				coef = 1.0;
			}

			Trio<double, double, double> trio;
			trio.set( coef,lowerBound, upperBound );

			varId_Bounds[ id ] = trio;
				
			numVars++;
		}
    }
    
	// Insere todas as variaveis em vHashTatico e no lp	
	for ( vit = varHashAux.begin(); vit != varHashAux.end(); vit++)
	{		
		VariableTatInt v = vit->first;
		int id = vit->second;

		vHashTatico[ v ] = lp->getNumCols();
		
		double coef = varId_Bounds[id].first;
		double lowerBound = varId_Bounds[id].second;
		double upperBound = varId_Bounds[id].third;

		OPT_COL col( OPT_COL::VAR_BINARY, coef, lowerBound, upperBound,
			( char * )v.toString().c_str());

		lp->newCol( col );
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
int TaticoIntAlunoHor::criaVariavelTaticoAberturaCompativel( int campusId, int P )
{
   int numVars = 0;

   if ( ! problemData->parametros->considerar_disciplinas_incompativeis_no_dia )
	   return numVars;

   Campus *campus = problemData->refCampus[ campusId ];

	std::map<Disciplina*, bool, LessPtr<Disciplina> > mapDiscAlunosDemandaBool;
	if ( problemData->parametros->considerar_equivalencia_por_aluno && USAR_EQUIVALENCIA ) 
	{		
		ITERA_GGROUP_LESSPTR( itDisc, problemData->disciplinas, Disciplina )
			mapDiscAlunosDemandaBool[*itDisc] = problemData->haDemandasDiscNoCampus_Equiv( (*itDisc)->getId(), campusId, P );
	}
	else
	{
		ITERA_GGROUP_LESSPTR( itDisc, problemData->disciplinas, Disciplina )
			mapDiscAlunosDemandaBool[*itDisc] = problemData->haDemandaDiscNoCampus( (*itDisc)->getId(), campusId );
	}


   ITERA_GGROUP_N_PT( it_disciplina, problemData->cp_discs[campusId], int )
   {
	    Disciplina* disciplina = problemData->refDisciplinas[ *it_disciplina ];

		#pragma region Equivalencias
		if ( ( problemData->mapDiscSubstituidaPor.find( disciplina ) !=
			   problemData->mapDiscSubstituidaPor.end() ) &&
			!problemData->ehSubstituta( disciplina ) )
		{
			continue;
		}
		#pragma endregion

		if ( ! mapDiscAlunosDemandaBool[disciplina] )
			continue;
			
        ITERA_GGROUP_N_PT( itDiasLetDisc, disciplina->diasLetivos, int )
        {
            VariableTatInt v;
            v.reset();
            v.setType( VariableTatInt::V_ABERTURA_COMPATIVEL );

            v.setDisciplina( disciplina );  // d
            v.setDia( *itDiasLetDisc );     // t
			v.setCampus( campus );			// cp

            if ( vHashTatico.find( v ) == vHashTatico.end() )
            {
				bool fixar=false;

				if ( !criaVariavelTaticoInt( &v, fixar, P ) )
					continue;

				vHashTatico[ v ] = lp->getNumCols();
									
				double lowerBound = 0.0;
				double upperBound = 1.0;

				if ( fixar ) lowerBound = 1.0;					    
				
				double coef = 0.0;

				if ( problemData->parametros->funcao_objetivo == 0 )
				{
					coef = -1.0;
				}
				else if ( problemData->parametros->funcao_objetivo == 1 )
				{
					coef = 1.0;
			 	}
				
                OPT_COL col( OPT_COL::VAR_BINARY, coef, lowerBound, upperBound,
                ( char * )v.toString().c_str() );

                lp->newCol( col );
                numVars++;
            }
        }
   }

   return numVars;
}

// fd_{d,a}
int TaticoIntAlunoHor::criaVariavelTaticoFolgaDemandaDiscAluno( int campusId, int P )
{
   int numVars = 0;
   
   Campus *cp = problemData->refCampus[campusId];

   ITERA_GGROUP_LESSPTR( itAluno, problemData->alunos, Aluno )
   {
	    Aluno *aluno = *itAluno;

		ITERA_GGROUP_LESSPTR( itAlDemanda, aluno->demandas, AlunoDemanda )
		{
			if ( itAlDemanda->demanda->oferta->getCampusId() != campusId )
			{
				continue;
			}

			Disciplina *disciplina = itAlDemanda->demanda->disciplina;
			
			if ( itAlDemanda->getPrioridade() > P )
				continue;

			VariableTatInt v;
			v.reset();
			v.setType( VariableTatInt::V_SLACK_DEMANDA_ALUNO );

			v.setDisciplina( disciplina );  // d
			v.setAluno( aluno );			// a

			if ( vHashTatico.find( v ) == vHashTatico.end() )
			{
				bool fixar=false;

				if ( !criaVariavelTaticoInt( &v, fixar, P ) )
					continue;

				vHashTatico[ v ] = lp->getNumCols();
									
				double lowerBound = 0.0;
				double upperBound = 1.0;

				if ( fixar ) lowerBound = 1.0;
								
				double coef = 0.0;
				if ( itAlDemanda->getPrioridade() > 1 )
				{
					coef = 0.0; // para P2, quem controla o custo da folga de demanda são fpi e fps
				}
				else
				{
					if ( problemData->parametros->funcao_objetivo == 0 )
					{
						coef = - 50 * disciplina->getTotalCreditos() * itAlDemanda->demanda->oferta->getReceita();
					}
					else if ( problemData->parametros->funcao_objetivo == 1 )
					{
						coef = 10 * disciplina->getTotalCreditos() * itAlDemanda->demanda->oferta->getReceita();
					}
				}
								
				OPT_COL col( OPT_COL::VAR_BINARY, coef, lowerBound, upperBound,
				( char * )v.toString().c_str() );

				lp->newCol( col );
				numVars++;
			}
        }
   }

   return numVars;
}

// fu_{i1,d1,i2,d2,t,cp}
int TaticoIntAlunoHor::criaVariavelTaticoFolgaAlunoUnidDifDia( int campusId, int P )
{
	int numVars = 0;
		
    if ( ! problemData->parametros->minDeslocAlunoEntreUnidadesNoDia )
		return numVars;
   
	std::map<Disciplina*, bool, LessPtr<Disciplina> > mapDiscAlunosDemandaBool;
	if ( problemData->parametros->considerar_equivalencia_por_aluno && USAR_EQUIVALENCIA ) 
	{		
		ITERA_GGROUP_LESSPTR( itDisc, problemData->disciplinas, Disciplina )
			mapDiscAlunosDemandaBool[*itDisc] = problemData->haDemandasDiscNoCampus_Equiv( (*itDisc)->getId(), campusId, P );
	}
	else
	{
		ITERA_GGROUP_LESSPTR( itDisc, problemData->disciplinas, Disciplina )
			mapDiscAlunosDemandaBool[*itDisc] = problemData->haDemandaDiscNoCampus( (*itDisc)->getId(), campusId );
	}
	
   ITERA_GGROUP_LESSPTR( it_campus, problemData->campi, Campus )
   {
	   Campus * campus = *it_campus;

	   if ( it_campus->getId() != campusId )
	   {
		   continue;
	   }
	     
	  GGroup< int > disciplinas = problemData->cp_discs[ campusId ];

	  // Disciplina 1
      ITERA_GGROUP_N_PT( it_disc1, disciplinas, int )
      {
		  Disciplina * disciplina1 = problemData->refDisciplinas[ *it_disc1 ];
		  
		 #pragma region Equivalencias
		 if ( ( problemData->mapDiscSubstituidaPor.find( disciplina1 ) !=
				problemData->mapDiscSubstituidaPor.end() ) &&
				!problemData->ehSubstituta( disciplina1 ) )
		 {
			continue;
		 }
		 #pragma endregion
		 		 
		if ( ! mapDiscAlunosDemandaBool[disciplina1] )
			continue;
		
		 // Turma 1
         for ( int turma1 = 1; turma1 <= disciplina1->getNumTurmas(); turma1++ )
         {
			 Unidade * u1 = this->retornaUnidadeDeAtendimento( turma1, disciplina1, campus );

			 if ( u1 == NULL )
				 continue;

			  // Disciplina 2
			  ITERA_GGROUP_INIC_N_PT( it_disc2, it_disc1, disciplinas, int )
			  {
				  Disciplina * disciplina2 = problemData->refDisciplinas[ *it_disc2 ];
		  
				  #pragma region Equivalencias
				  if ( ( problemData->mapDiscSubstituidaPor.find( disciplina2 ) !=
						   problemData->mapDiscSubstituidaPor.end() ) &&
						 !problemData->ehSubstituta( disciplina2 ) )
				  {
					  continue;
				  }
				  #pragma endregion
		 		 
				  if ( ! mapDiscAlunosDemandaBool[disciplina2] )
					  continue;

				  // Turma 2
				  for ( int turma2 = 1; turma2 <= disciplina2->getNumTurmas(); turma2++ )
				  {
					   Unidade * u2 = this->retornaUnidadeDeAtendimento( turma2, disciplina2, campus );

					   if ( u2 == NULL || u1 == u2 )
							continue;

					    GGroup<Aluno*, LessPtr<Aluno>> alunosEmComum = 
							problemData->alunosEmComum( turma1, disciplina1, turma2, disciplina2, campus );

						if ( alunosEmComum.size() == 0 )
							continue;

						int nroAlunos = alunosEmComum.size();

						GGroup<int> dias = problemData->diasComunsEntreDisciplinas( disciplina1, disciplina2 );

						ITERA_GGROUP_N_PT( it_dias, dias, int )
						{
							VariableTatInt v;
							v.reset();
							v.setType( VariableTatInt::V_SLACK_ALUNO_VARIAS_UNID_DIA );

							v.setTurma1( turma1 );            // i1
							v.setDisciplina1( disciplina1 );  // d1
							v.setTurma2( turma2 );            // i2
							v.setDisciplina2( disciplina2 );  // d2
							v.setCampus( campus );		      // cp
							v.setDia( *it_dias );			  // t

							if ( vHashTatico.find(v) == vHashTatico.end() )
							{
								bool fixar=false;

								if ( !criaVariavelTaticoInt( &v, fixar, P ) )
									continue;

								vHashTatico[ v ] = lp->getNumCols();
									
								double lowerBound = 0.0;
								double upperBound = 1.0;

								if ( fixar ) lowerBound = 1.0;
					    
								double coef = 0.0;

								if ( problemData->parametros->funcao_objetivo == 0 )
								{					
									coef = - 10 * nroAlunos;
								}
								else if ( problemData->parametros->funcao_objetivo == 1 )
								{
									coef = 10 * nroAlunos;
			 					}
								
								OPT_COL col( OPT_COL::VAR_BINARY, coef, lowerBound, upperBound, ( char * )v.toString().c_str() );

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
  
// du_{a,t}
int TaticoIntAlunoHor::criaVariavelTaticoDiaUsadoPeloAluno( int campusId, int P )
{
   int numVars = 0;
   
   if ( problemData->parametros->carga_horaria_semanal_aluno == ParametrosPlanejamento::INDIFERENTE )
   {
		return numVars;
   }

   Campus *cp = problemData->refCampus[campusId];

   ITERA_GGROUP_N_PT( itDia, cp->diasLetivos, int )
   {
	   int dia = *itDia;
	   ITERA_GGROUP_LESSPTR( itAluno, problemData->alunos, Aluno )
	   {
			Aluno *aluno = *itAluno;

			VariableTatInt v;
			v.reset();
			v.setType( VariableTatInt::V_ALUNO_DIA );
			v.setAluno( aluno );	// a
			v.setDia( dia );		// t

			if ( vHashTatico.find( v ) == vHashTatico.end() )
			{
				bool fixar=false;

				if ( !criaVariavelTaticoInt( &v, fixar, P ) )
					continue;

				vHashTatico[ v ] = lp->getNumCols();
									
				double lowerBound = 0.0;
				double upperBound = 1.0;

				if ( fixar ) lowerBound = 1.0;
											
				double obj = 0.0;

				if ( problemData->parametros->funcao_objetivo == 0 ) // MAX
				{
					if ( problemData->parametros->carga_horaria_semanal_aluno == ParametrosPlanejamento::EQUILIBRAR)
					{
						obj = cp->getCusto()/4;
					}
					else if ( problemData->parametros->carga_horaria_semanal_aluno == ParametrosPlanejamento::MINIMIZAR_DIAS)
					{
						obj = -cp->getCusto()/4;
					}
				}
				else if ( problemData->parametros->funcao_objetivo == 1 ) // MIN
				{
					if ( problemData->parametros->carga_horaria_semanal_aluno == ParametrosPlanejamento::EQUILIBRAR)
					{
						obj = -cp->getCusto()/4;
					}
					else if ( problemData->parametros->carga_horaria_semanal_aluno == ParametrosPlanejamento::MINIMIZAR_DIAS)
					{
						obj = cp->getCusto()/4;
					}
				}
               
				OPT_COL col( OPT_COL::VAR_BINARY, obj, lowerBound, upperBound, ( char * )v.toString().c_str() );

				lp->newCol( col );
				numVars++;
			}
		}
   }

    return numVars;
}

// ft_{i,d,cp}
int TaticoIntAlunoHor::criaVariavelTaticoFolgaAbreTurmaSequencialAPartirDeX( int campusId, int P )
{
	int numVars = 0;

	Campus *campus = problemData->refCampus[campusId];	   
	
	VariableTatIntHash varHashAux;	
	std::map< int, Trio<double, double, double> > varId_Bounds;

	VariableTatIntHash::iterator vit = vHashTatico.begin();
	for ( ; vit != vHashTatico.end(); vit++)
	{		
		VariableTatInt v = vit->first;

		if( v.getType() != VariableTatInt::V_CREDITOS )
		{
			continue;
		}

		if ( v.getUnidade()->getIdCampus() != campusId )
			continue;
		
		int turma = v.getTurma();
		Disciplina* disciplina = v.getDisciplina();
				
		v.reset();
		v.setType( VariableTatInt::V_SLACK_ABERT_SEQ_TURMA );
        v.setTurma( turma );            // i
        v.setDisciplina( disciplina );  // d
        v.setCampus( campus );			// cp

		if ( varHashAux.find( v ) == varHashAux.end() )
		{
			bool fixar=false;

			if ( !criaVariavelTaticoInt( &v, fixar, P ) )
			{
				continue;
			}
						
			int id = numVars;
			varHashAux[ v ] = id;
				
			double coef = 0.0;
			if ( problemData->parametros->funcao_objetivo == 0 )
			{							 
				coef = -(turma+1);
			}
			else if ( problemData->parametros->funcao_objetivo == 1 )
			{					
				coef = (turma+1);
			}			
			double lowerBound = 0.0;
			double upperBound = 1.0;

			if ( fixar ) lowerBound = 1.0;
				
			Trio<double, double, double> trio;
			trio.set( coef,lowerBound, upperBound );

			varId_Bounds[ id ] = trio;
				
			numVars++;
		}
    }
    
	// Insere todas as variaveis em vHashTatico e no lp	
	for ( vit = varHashAux.begin(); vit != varHashAux.end(); vit++)
	{		
		VariableTatInt v = vit->first;
		int id = vit->second;

		vHashTatico[ v ] = lp->getNumCols();
		
		double coef = varId_Bounds[id].first;
		double lowerBound = varId_Bounds[id].second;
		double upperBound = varId_Bounds[id].third;

		OPT_COL col( OPT_COL::VAR_BINARY, coef, lowerBound, upperBound,
			( char * )v.toString().c_str());

		lp->newCol( col );
	}

	return numVars;
}

// ft_{i,d,cp}
int TaticoIntAlunoHor::criaVariavelTaticoFolgaAbreTurmaSequencial( int campusId, int P )
{
	int numVars = 0;
	
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
		 							    
		 if ( this->mapDiscAlunosDemanda[disciplina->getId()].size() == 0 )
			 continue;
		 
		 #pragma region Equivalencias
		 if ( ( problemData->mapDiscSubstituidaPor.find( disciplina ) !=
				problemData->mapDiscSubstituidaPor.end() ) &&
				!problemData->ehSubstituta( disciplina ) )
		 {
		 	continue;
		 }
		 #pragma endregion
	 		 
         for ( int turma = 1; turma <= disciplina->getNumTurmas(); turma++ )
         {
            VariableTatInt v;
            v.reset();
            v.setType( VariableTatInt::V_SLACK_ABERT_SEQ_TURMA );

            v.setTurma( turma );            // i
            v.setDisciplina( disciplina );  // d
            v.setCampus( cp );				// cp

            if ( vHashTatico.find(v) == vHashTatico.end() )
            {
				bool fixar=false;

				if ( !criaVariavelTaticoInt( &v, fixar, P ) )
					continue;

				vHashTatico[ v ] = lp->getNumCols();
									
				double lowerBound = 0.0;
				double upperBound = 1.0;

				if ( fixar ) lowerBound = 1.0;
								
			    double coef = 0.0;
				if ( problemData->parametros->funcao_objetivo == 0 )
				{							 
					coef = -10 * (turma+1);
				}
				else if ( problemData->parametros->funcao_objetivo == 1 )
				{					
					coef = 10 * (turma+1);
				}						                
						 
				OPT_COL col( OPT_COL::VAR_BINARY, coef, lowerBound, upperBound,
                     ( char * )v.toString().c_str() );

                lp->newCol( col ); 
                
				numVars++;
            }
         }
      }
   }

	return numVars;

}

// fc_{i,d,c1,c2}
int TaticoIntAlunoHor::criaVariavelFolgaProibeCompartilhamento( int campusId, int P )
{
	int numVars = 0;

   if ( problemData->parametros->permite_compartilhamento_turma_sel )
   {
		return numVars;
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
			    
				// A variavel de folga só é criada para cursos compativeis e diferentes entre si
				// Ofertas para mesmo curso sempre poderão compartilhar
				// Ofertas de cursos distintos só poderão compartilhar se forem compativeis e o compartilhamento estiver permitido
			    if ( *c1 == *c2 || !problemData->cursosCompativeis(c1, c2) )
				    continue;

				ITERA_GGROUP_LESSPTR( itDisc, problemData->disciplinas, Disciplina )
				{
					Disciplina *disciplina = *itDisc;

					#pragma region Equivalencias
					if ( ( problemData->mapDiscSubstituidaPor.find( disciplina ) !=
						   problemData->mapDiscSubstituidaPor.end() ) &&
						  !problemData->ehSubstituta( disciplina ) )
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

					// Verificando existencia de demanda
					if ( problemData->parametros->considerar_equivalencia_por_aluno && USAR_EQUIVALENCIA )
					{	if ( !problemData->haDemandaDiscNoCursoEquiv( disciplina, c1->getId() ) ||
							 !problemData->haDemandaDiscNoCursoEquiv( disciplina, c2->getId() ) )
							continue;
					}
					else 
					{	if ( problemData->haDemandaDiscNoCurso( disciplina->getId(), c1->getId() ) == 0 ||
					  	     problemData->haDemandaDiscNoCurso( disciplina->getId(), c2->getId() ) == 0 )
							continue;
					}

					for ( int turma = 1; turma <= disciplina->getNumTurmas(); turma++ )
					{
					   VariableTatInt v;
					   v.reset();
					   v.setType( VariableTatInt::V_SLACK_COMPARTILHAMENTO );
					   v.setTurma( turma );							// i
					   v.setDisciplina( disciplina );   			// d
					   v.setParCursos( std::make_pair(c1, c2) );	// c1 c2

					   if ( vHashTatico.find( v ) == vHashTatico.end() )
					   {
							bool fixar=false;

							if ( !criaVariavelTaticoInt( &v, fixar, P ) )
								continue;

							vHashTatico[ v ] = lp->getNumCols();
									
							double lowerBound = 0.0;
							double upperBound = 1.0;

							if ( fixar ) lowerBound = 1.0;
							
							double coef = 0.0;

							if ( problemData->parametros->funcao_objetivo == 0 )
							{
								coef = -200*cp->getCusto();
							}
							else if( problemData->parametros->funcao_objetivo == 1 )
							{
								coef = 200*cp->getCusto();
							}						  						 
					
							OPT_COL col( OPT_COL::VAR_BINARY, coef, lowerBound, upperBound, ( char * )v.toString().c_str() );

							lp->newCol( col );
							numVars++;
					   }
					}
				}
		    }
		}
   }

	return numVars;
}

// fpi_{a}
// Só para P2 em diante
int TaticoIntAlunoHor::criaVariavelFolgaPrioridadeInf( int campusId, int prior )
{
	int numVars = 0;

	if ( prior < 2 )
	{
	   return numVars;
	}

	Campus *cp = problemData->refCampus[campusId];

	// Para cada aluno do conjunto	
	ITERA_GGROUP_LESSPTR( itAluno, problemData->alunos, Aluno )
	{
		Aluno *aluno = *itAluno;

		VariableTatInt v;
		v.reset();
		v.setType( VariableTatInt::V_SLACK_PRIOR_INF );
		v.setAluno( aluno );
		v.setCampus( cp );

		if ( vHashTatico.find( v ) == vHashTatico.end() )
		{
			double upperBound = 0.0;
			double lowerBound = 0.0;
			int totalCreditosP2 = 0;
			double tempoMaxCred=0;

			ITERA_GGROUP_LESSPTR( itAlunoDem, aluno->demandas, AlunoDemanda )
			{
				if ( itAlunoDem->getPrioridade() != prior )
					continue;

				int nCreds = itAlunoDem->demanda->disciplina->getTotalCreditos();
				int tempo = itAlunoDem->demanda->disciplina->getTempoCredSemanaLetiva();					
				totalCreditosP2 += nCreds;

				if ( tempoMaxCred < tempo )
					tempoMaxCred=tempo;							
			}
			
			if ( totalCreditosP2 == 0 ) // se não houver demanda de P2 para o aluno
			{
				continue;
			}

			// Limita o tanto de carga horária do aluno que pode ser excedido em P2 em 2 créditos.
			upperBound = tempoMaxCred*2;

			double cargaHorariaNaoAtendida = problemData->cargaHorariaNaoAtendidaPorPrioridade( 1, aluno->getAlunoId() );
			double chAtendida = problemData->cargaHorariaJaAtendida( aluno );
			if ( upperBound < chAtendida - cargaHorariaNaoAtendida )
				upperBound = chAtendida - cargaHorariaNaoAtendida;	// pode ser que as equivalencias geraram um aumento maior que 2 unidades em creditos atendidos


			double coef = 0.0;
			if ( problemData->parametros->funcao_objetivo == 0 )
			{
				double custo = cp->getCusto();
							 
				coef = -50 * custo * totalCreditosP2;
			}
			else if ( problemData->parametros->funcao_objetivo == 1 )
			{
				double custo = cp->getCusto();

				coef = 5 * custo * totalCreditosP2;
			}	
			
			vHashTatico[v] = lp->getNumCols();

			OPT_COL col( OPT_COL::VAR_INTEGRAL, coef, lowerBound, upperBound, ( char * )v.toString().c_str() );
			
			lp->newCol( col );
			numVars++;			
		}
	}

	return numVars;
}

// fps_{a}
// Só para P2 em diante
int TaticoIntAlunoHor::criaVariavelFolgaPrioridadeSup( int campusId, int prior )
{
	int numVars = 0;
	
	if ( prior < 2 )
	{
	   return numVars;
	}

	Campus *cp = problemData->refCampus[campusId];
		
	ITERA_GGROUP_LESSPTR( itAluno, problemData->alunos, Aluno )
	{
		Aluno *aluno = *itAluno;

		VariableTatInt v;
		v.reset();
		v.setType( VariableTatInt::V_SLACK_PRIOR_SUP );
		v.setAluno( aluno );
		v.setCampus( cp );

		if ( vHashTatico.find( v ) == vHashTatico.end() )
		{
			int totalCreditos = aluno->getNroCreditosNaoAtendidos();
			double cargaHorariaNaoAtendida = problemData->cargaHorariaNaoAtendidaPorPrioridade( 1, aluno->getAlunoId() );	
			if ( cargaHorariaNaoAtendida <= 0 ) // se não houver folga de demanda de P1
			{
				continue;
			}

			double lowerBound = 0.0;
			double upperBound = cargaHorariaNaoAtendida;				

			double coef = 0.0;
			if ( problemData->parametros->funcao_objetivo == 0 )
			{
				double custo = cp->getCusto();
							 
				coef = -60 * custo * totalCreditos;
			}
			else if ( problemData->parametros->funcao_objetivo == 1 )
			{
				double custo = cp->getCusto();

				coef = 6 * custo * totalCreditos;
			}	
			
			vHashTatico[v] = lp->getNumCols();
					
			OPT_COL col( OPT_COL::VAR_INTEGRAL, coef, lowerBound, upperBound, ( char * )v.toString().c_str() );
			
			lp->newCol( col );
			numVars++;
		}
	}

	return numVars;
}

// f_{i,d,cp}
int TaticoIntAlunoHor::criaVariavelTaticoFormandosNaTurma( int campusId, int prior, int r )
{
	int numVars = 0;
	
	if ( !problemData->parametros->violar_min_alunos_turmas_formandos )
		return numVars;

	if ( prior==1 && r==1 ) // não considera formandos em r1 de p1
		return numVars;

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
		 if ( ( problemData->mapDiscSubstituidaPor.find( disciplina ) !=
				problemData->mapDiscSubstituidaPor.end() ) &&
				!problemData->ehSubstituta( disciplina ) )
		 {
		 	continue;
		 }
		 #pragma endregion
		 
		 //if ( problemData->parametros->considerar_equivalencia_por_aluno && USAR_EQUIVALENCIA )
		 //{  if ( ! problemData->haDemandaPorFormandosEquiv( disciplina, cp, prior ) )
			//   continue;
		 //}
		 //else if ( ! problemData->haDemandaPorFormandos( disciplina, cp, prior ) )
			// continue;


         for ( int turma = 1; turma <= disciplina->getNumTurmas(); turma++ )
         {
			 if ( !permitirAbertura( turma, disciplina, campusId ) )				
					continue;

			 if ( problemData->parametros->considerar_equivalencia_por_aluno && USAR_EQUIVALENCIA )
			 {
				 if ( !problemData->possuiAlunoFormando( turma, disciplina, cp ) &&
					  !problemData->haAlunoFormandoNaoAlocadoEquiv( disciplina, campusId, prior ) )
				 {
					 continue;
				 }
			 }
			 else
			 {
				 if ( !problemData->possuiAlunoFormando( turma, disciplina, cp ) &&
					  !problemData->haAlunoFormandoNaoAlocado( disciplina, campusId, prior ) )
				 {
					 continue;
				 }			 
			 }

			 VariableTatInt v;
			 v.reset();
			 v.setType( VariableTatInt::V_FORMANDOS_NA_TURMA );
			 v.setTurma( turma );            // i
			 v.setDisciplina( disciplina );  // d
			 v.setCampus( cp );				// cp

			 if ( vHashTatico.find(v) == vHashTatico.end() )
			 {
                lp->getNumCols();
                vHashTatico[v] = lp->getNumCols();

			    double coef = 0.0;
						 
				double lowerBound = 0.0;
				double upperBound = 1.0;

				OPT_COL col( OPT_COL::VAR_BINARY, coef, lowerBound, upperBound,
                     ( char * )v.toString().c_str() );

                lp->newCol( col ); 
                
				numVars++;
            }
         }
      }
   }

	return numVars;
}


// s_{i,d,a,cp}
int TaticoIntAlunoHor::criaVariavelTaticoAlocaAlunoTurmaDiscEquiv( int campusId, int P )
{
	int numVars = 0;

	if ( !problemData->parametros->considerar_equivalencia_por_aluno || !USAR_EQUIVALENCIA || !PERMITIR_NOVAS_TURMAS )
		return numVars;


	Campus *cp = problemData->refCampus[campusId];
	
	ITERA_GGROUP_LESSPTR( itAluno, problemData->alunos, Aluno )
	{
		Aluno *aluno = *itAluno;

		ITERA_GGROUP_LESSPTR( itAlDemanda, aluno->demandas, AlunoDemanda )
		{
			if ( aluno->getOferta( itAlDemanda->demanda )->getCampusId() != campusId )
			{
				continue;
			}

			Disciplina *disciplina = (*itAlDemanda)->demanda->disciplina;

			int turmaAluno = problemData->retornaTurmaDiscAluno( aluno, disciplina );
				
			if ( turmaAluno == -1 ) // aluno não alocado
			{
				if ( aluno->getCargaHorariaOrigRequeridaP1() -
					 problemData->cargaHorariaJaAtendida( aluno ) <= 0 )
					 continue;

				ITERA_GGROUP_LESSPTR( itDiscEq, disciplina->discEquivSubstitutas, Disciplina )
				{
					Disciplina *disciplinaEquiv = (*itDiscEq);

					if ( ! problemData->alocacaoEquivViavel( (*itAlDemanda)->demanda, disciplinaEquiv ) )
						continue;

					for ( int turma = 1; turma <= disciplinaEquiv->getNumTurmas(); turma++ )
					{
						VariableTatInt v;
						v.reset();
						v.setType( VariableTatInt::V_ALOCA_ALUNO_TURMA_DISC );
						v.setAluno( aluno );
						v.setDisciplina( disciplinaEquiv );
						v.setTurma( turma );
						v.setCampus( cp );
						v.setAlunoDemanda( *itAlDemanda );


						if ( vHashTatico.find( v ) == vHashTatico.end() )
						{
							bool fixar=false;
							if ( !criaVariavelTaticoInt( &v, fixar, P ) )
								continue;

							vHashTatico[ v ] = lp->getNumCols();
									
							double lowerBound = 0.0;
							double upperBound = 1.0;
							double coef = 0.0;		
					
							if ( fixar ) lowerBound = 1.0;		
										
							OPT_COL col( OPT_COL::VAR_BINARY, coef, lowerBound, upperBound, ( char * )v.toString().c_str() );

							lp->newCol( col );
							numVars++;
						}
					}
				}
			}
			else // aluno já alocado
			{
				VariableTatInt v;
				v.reset();
				v.setType( VariableTatInt::V_ALOCA_ALUNO_TURMA_DISC );
				v.setAluno( aluno );
				v.setDisciplina( disciplina );
				v.setTurma( turmaAluno );
				v.setCampus( cp );
				v.setAlunoDemanda( *itAlDemanda );

				if ( vHashTatico.find( v ) == vHashTatico.end() )
				{
					bool fixar=false;
					if ( !criaVariavelTaticoInt( &v, fixar, P ) )
					{
						if ( CRIAR_VARS_FIXADAS )
							std::cout<<"\nERRO: acho que nao devia entrar aqui, o aluno ta alocado!!\n";
						continue;
					}

					vHashTatico[ v ] = lp->getNumCols();
									
					double lowerBound = 0.0;
					double upperBound = 1.0;
					double coef = 0.0;		
											
					if ( fixar ) lowerBound = 1.0;		

					OPT_COL col( OPT_COL::VAR_BINARY, coef, lowerBound, upperBound, ( char * )v.toString().c_str() );

					lp->newCol( col );
					numVars++;
				}
			}
		}
	}

	return numVars;
}

// s_{i,d,a,cp}
int TaticoIntAlunoHor::criaVariavelTaticoAlocaAlunoTurmaDiscEquivTotal( int campusId, int P )
{
	// Cria todas as combinações possíveis usando equivalencias, inclusive para alunos já alocados.
	
	int numVars = 0;

	if ( !problemData->parametros->considerar_equivalencia_por_aluno || !USAR_EQUIVALENCIA || PERMITIR_NOVAS_TURMAS )
		return numVars;

	Campus *cp = problemData->refCampus[campusId];
	
	ITERA_GGROUP_LESSPTR( itAluno, problemData->alunos, Aluno )
	{
		Aluno *aluno = *itAluno;

		ITERA_GGROUP_LESSPTR( itAlDemanda, aluno->demandas, AlunoDemanda )
		{
			if ( aluno->getOferta( itAlDemanda->demanda )->getCampusId() != campusId )
			{
				continue;
			}

			Disciplina *disciplina = (*itAlDemanda)->demanda->disciplina;

			GGroup<Disciplina*, LessPtr<Disciplina>> disciplinasPorAlDem;				
			disciplinasPorAlDem.add( disciplina );
					
			// Se ainda não tiver usado equivalência para esse AlunoDemanda. Evita transitividade entre disciplinas equivalentes.
			if ( itAlDemanda->demandaOriginal == NULL )
			{
				ITERA_GGROUP_LESSPTR( itDisc, disciplina->discEquivSubstitutas, Disciplina )
				{
					if ( problemData->alocacaoEquivViavel( (*itAlDemanda)->demanda, *itDisc ) )
					{
						disciplinasPorAlDem.add( *itDisc );			
					}
				}
			}

			ITERA_GGROUP_LESSPTR( itDiscEq, disciplinasPorAlDem, Disciplina )
			{
				Disciplina *disciplinaEquiv = (*itDiscEq);								

				for ( int turma = 1; turma <= disciplinaEquiv->getNumTurmas(); turma++ )
				{
					VariableTatInt v;
					v.reset();
					v.setType( VariableTatInt::V_ALOCA_ALUNO_TURMA_DISC );
					v.setAluno( aluno );
					v.setDisciplina( disciplinaEquiv );
					v.setTurma( turma );
					v.setCampus( cp );
					v.setAlunoDemanda( *itAlDemanda );
					
					if ( vHashTatico.find( v ) == vHashTatico.end() )
					{						
						if ( !permitirAbertura( turma, disciplinaEquiv, campusId ) )				
							continue;
						
						double lowerBound = 0.0;
						double upperBound = 1.0;
						double coef = 0.0;		

						if ( problemData->existeSolTaticoInicial() )
						{
							if ( ! problemData->passarPorPreModeloETatico(campusId) )
							{
								// Toda solução ja existente veio da solucao inicial fixada. Continua fixada.
								int turmaAluno = problemData->retornaTurmaDiscAluno( aluno, disciplinaEquiv );
								if ( turma == turmaAluno )
									lowerBound = 1.0;
							}
							else
							{
								// Há solução inicial e já se passou pelo pre-modelo+tatico.
								// Fixa se for da solução inicial
								// C.c. não fixa, permitindo troca de turma
								bool fixar=false;
								int turmaInicialAluno = problemData->getSolTaticoInicial()->getTurma( aluno, campusId, disciplinaEquiv, fixar );
								if ( turma == turmaInicialAluno && fixar )
									lowerBound = 1.0;
							}
						}

						vHashTatico[ v ] = lp->getNumCols();
										
						OPT_COL col( OPT_COL::VAR_BINARY, coef, lowerBound, upperBound, ( char * )v.toString().c_str() );

						lp->newCol( col );
						numVars++;
					}
				}				
			}
		}
	}

	return numVars;
}


// z_{i,d,cp}
int TaticoIntAlunoHor::criaVariavelTaticoAberturaAPartirDeX( int campusId, int prior )
{
	int numVars = 0;

	Campus *campus = problemData->refCampus[campusId];	   
	
	VariableTatIntHash varHashAux;	
	std::map< int, Trio<double, double, double> > varId_Bounds;

	VariableTatIntHash::iterator vit = vHashTatico.begin();
	for ( ; vit != vHashTatico.end(); vit++)
	{		
		VariableTatInt x = vit->first;

		if( x.getType() != VariableTatInt::V_CREDITOS )
		{
			continue;
		}

		if ( x.getUnidade()->getIdCampus() != campusId )
			continue;
		
		int turma = x.getTurma();
		Disciplina* disciplina = x.getDisciplina();
				
		VariableTatInt v;
		v.reset();
		v.setType( VariableTatInt::V_ABERTURA );
        v.setTurma( turma );            // i
        v.setDisciplina( disciplina );  // d
        v.setCampus( campus );			// cp

		if ( varHashAux.find( v ) == varHashAux.end() )
		{
			bool fixar=false;
			if ( !criaVariavelTaticoInt( &v, fixar, prior ) )
					continue;

			bool fixarInit;
			int existe = problemData->getSolTaticoInicial()->existeTurma( campusId, disciplina, turma, fixarInit );
			if ( existe && fixarInit )
				fixar=true;
					
			double lowerBound = 0.0;
			double upperBound = 1.0;
			if ( fixar ) lowerBound = 1.0;
			
			double coef = disciplina->getTotalCreditos() * campus->getCusto();		
			Trio<double, double, double> trio;
			trio.set( coef,lowerBound, upperBound );

			int id = lp->getNumCols();
			varHashAux[ v ] = id;
					
			varId_Bounds[ id ] = trio;
				
			numVars++;
		}
    }
    
	// Insere todas as variaveis em vHashTatico e no lp	
	for ( vit = varHashAux.begin(); vit != varHashAux.end(); vit++)
	{		
		VariableTatInt v = vit->first;
		int id = vit->second;

		vHashTatico[ v ] = lp->getNumCols();
		
		double coef = varId_Bounds[id].first;
		double lowerBound = varId_Bounds[id].second;
		double upperBound = varId_Bounds[id].third;

		OPT_COL col( OPT_COL::VAR_BINARY, coef, lowerBound, upperBound,
			( char * )v.toString().c_str());

		lp->newCol( col );
	}

	return numVars;
}

// z_{i,d,cp}
int TaticoIntAlunoHor::criaVariavelTaticoAbertura( int campusId, int prior, int r )
{
	int numVars = 0;
	
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
		 if ( ( problemData->mapDiscSubstituidaPor.find( disciplina ) !=
				problemData->mapDiscSubstituidaPor.end() ) &&
				!problemData->ehSubstituta( disciplina ) )
		 {
		 	continue;
		 }
		 #pragma endregion
	 
		 if ( problemData->parametros->considerar_equivalencia_por_aluno && USAR_EQUIVALENCIA )
		 {
			if ( !problemData->haDemandasDiscNoCampus_Equiv( disciplina->getId(), campusId, prior ) )
				continue;
		 }
		 else
		 {
			if ( !problemData->haDemandaDiscNoCampus( disciplina->getId(), campusId ) )
				continue;
		 }
		 
         for ( int turma = 1; turma <= disciplina->getNumTurmas(); turma++ )
         {
			 VariableTatInt v;
			 v.reset();
			 v.setType( VariableTatInt::V_ABERTURA );
			 v.setTurma( turma );            // i
			 v.setDisciplina( disciplina );  // d
			 v.setCampus( cp );				// cp

			 if ( vHashTatico.find(v) == vHashTatico.end() )
			 {
				bool fixar=false;
				if ( !criaVariavelTaticoInt( &v, fixar, prior ) )
					continue;

                vHashTatico[v] = lp->getNumCols();

			    //double coef = 1.0;
				double coef = disciplina->getTotalCreditos() * cp->getCusto();
				
				double lowerBound = 0.0;
				if ( fixar ) lowerBound = 1.0;

				double upperBound = 1.0;

				OPT_COL col( OPT_COL::VAR_BINARY, coef, lowerBound, upperBound,
                     ( char * )v.toString().c_str() );

                lp->newCol( col ); 
                
				numVars++;
            }
         }
      }
   }

	return numVars;

}


// fp_{d,t,h}
int TaticoIntAlunoHor::criaVariavelTaticoFolgaProfAPartirDeX( int campusId, int P )
{
	int numVars = 0;
	
	if ( ! problemData->parametros->considerar_disponibilidade_prof_em_tatico )
		return numVars;

	VariableTatIntHash varHashAux;	
	std::map< int, Trio<double, double, double> > varId_Bounds;

	VariableTatIntHash::iterator vit = vHashTatico.begin();
	for ( ; vit != vHashTatico.end(); vit++)
	{		
		VariableTatInt x = vit->first;

		if( x.getType() != VariableTatInt::V_CREDITOS )
		{
			continue;
		}

		Disciplina* disciplina = x.getDisciplina();
		int dia = x.getDia();
		HorarioAula *hi = x.getHorarioAulaInicial();
		HorarioAula *hf = x.getHorarioAulaFinal();
		
		HorarioAula *ha = hi;

		bool end=false;
		while ( !end )
		{
			VariableTatInt v;
			v.reset();
			v.setType( VariableTatInt::V_FOLGA_HOR_PROF );
			v.setDisciplina( disciplina );				// d  
			v.setDia( dia );							// t
			v.setHorarioAulaInicial( ha );		// h
			std::pair<DateTime*,int> auxP = problemData->horarioAulaDateTime[ha->getId()];
			v.setDateTimeInicial( auxP.first );

			if ( varHashAux.find( v ) == varHashAux.end() )
			{
				bool fixar=false;
				if ( !criaVariavelTaticoInt( &v, fixar, P ) )
				{
					continue;
				}
						
				int id = numVars;
				varHashAux[ v ] = id;
				
				double coef = 0.0;
				if ( problemData->parametros->funcao_objetivo == 0 )
				{
					coef = -50.0;
				}
				else if ( problemData->parametros->funcao_objetivo == 1 )
				{
					coef = 50.0;
			 	}

				double lowerBound = 0.0;
				double upperBound = 50.0;
								
				Trio<double, double, double> trio;
				trio.set( coef,lowerBound, upperBound );

				varId_Bounds[ id ] = trio;
				
				numVars++;
			}

			if ( ha->getInicio() == hf->getInicio() ) end=true;
			else ha = ha->getCalendario()->getProximoHorario( ha );

			if ( ha==NULL )
			{
				std::cout<<"\n\nErro em criaVariavelTaticoFolgaProfAPartirDeX! horario eh null antes de chegar em hf";
				std::cout<<"\nVar " << x.toString();
				std::cout<<"\nHf " << hf->getId() << " = " << hf->getInicio();
				end=true;

				HorarioAula *hteste = hi;
				std::cout<<"\nCalend " << hteste->getCalendario()->getId();
				while ( hteste!=NULL )
				{
					std::cout<<"\n\th (" << hteste->getId() << ") = " << hteste->getInicio();
					hteste = hteste->getCalendario()->getProximoHorario( hteste );
				}
			}			
		}
    }
    
	// Insere todas as variaveis em vHashTatico e no lp	
	for ( vit = varHashAux.begin(); vit != varHashAux.end(); vit++)
	{		
		VariableTatInt v = vit->first;
		int id = vit->second;

		vHashTatico[ v ] = lp->getNumCols();
		
		double coef = varId_Bounds[id].first;
		double lowerBound = varId_Bounds[id].second;
		double upperBound = varId_Bounds[id].third;

		OPT_COL col( OPT_COL::VAR_INTEGRAL, coef, lowerBound, upperBound,
			( char * )v.toString().c_str());

		lp->newCol( col );
	}

	return numVars;

}


// ss_{a1,a2,dp}
int TaticoIntAlunoHor::criaVariavelTaticoAlunosMesmaTurmaPratica( int campusId, int P )
{
	int numVars = 0;
	
	// relação 1xN
	if ( ! problemData->parametros->discPratTeor1xN_antigo )
	{
		return numVars;
	}
	
	Campus *cp = problemData->refCampus[campusId];
	
	std::map< Disciplina*, GGroup<Aluno*, LessPtr<Aluno>>, LessPtr<Disciplina> > mapDiscPratAlunos;

	ITERA_GGROUP_LESSPTR( itAluno, problemData->alunos, Aluno )
	{
		Aluno *aluno = *itAluno;

		ITERA_GGROUP_LESSPTR( itAlDemanda, aluno->demandas, AlunoDemanda )
		{
			Disciplina *disciplina = (*itAlDemanda)->demanda->disciplina;

			if ( itAlDemanda->getCampus()->getId() != campusId )
			{
				continue;
			}

			GGroup<Disciplina*, LessPtr<Disciplina>> disciplinasPorAlDem;

			if ( disciplina->getId() < 0 )
				mapDiscPratAlunos[ disciplina ].add( aluno );
			
			#ifdef EQUIVALENCIA_DESDE_O_INICIO
			int turmaAluno = problemData->retornaTurmaDiscAluno( aluno, disciplina );		
			if ( turmaAluno == -1 && problemData->parametros->considerar_equivalencia_por_aluno ) // aluno não alocado
			{
				int chRequerida = aluno->getCargaHorariaOrigRequeridaP1();
				int chAtendida = problemData->cargaHorariaJaAtendida( aluno );
				if ( chRequerida - chAtendida <= 0 )
						continue;

				ITERA_GGROUP_LESSPTR( itDisc, disciplina->discEquivSubstitutas, Disciplina )
				{
					if ( itDisc->getId() < 0 )
					if ( problemData->alocacaoEquivViavel( (*itAlDemanda)->demanda, *itDisc ) )
					{
						mapDiscPratAlunos[ *itDisc ].add( aluno );
					}
				}
			}
			#endif
		}
	}

	std::map< Disciplina*, GGroup<Aluno*, LessPtr<Aluno>>, LessPtr<Disciplina> >::iterator
		itMap = mapDiscPratAlunos.begin();
	for ( ; itMap != mapDiscPratAlunos.end(); itMap++ )
	{
		Disciplina *discPratica = itMap->first;
		GGroup<Aluno*, LessPtr<Aluno>> alunos = itMap->second;

		ITERA_GGROUP_LESSPTR( itAluno1, alunos, Aluno )
		{
			ITERA_GGROUP_LESSPTR( itAluno2, alunos, Aluno )
			{
				if ( itAluno1->getAlunoId() >= itAluno2->getAlunoId() )
					continue;

				VariableTatInt v;
				v.reset();
				v.setType( VariableTatInt::V_ALUNOS_MESMA_TURMA_PRAT );
				v.setParAlunos( *itAluno1, *itAluno2 );
				v.setDisciplina( discPratica );
					
				if ( vHashTatico.find( v ) == vHashTatico.end() )
				{
					vHashTatico[ v ] = lp->getNumCols();
									
					double lowerBound = 0.0;
					double upperBound = 1.0;
					double coef = 0.0;		
					
					OPT_COL col( OPT_COL::VAR_BINARY, coef, lowerBound, upperBound, ( char * )v.toString().c_str() );

					lp->newCol( col );
					numVars++;
				}
			}
		}
	}

	return numVars;
}

// fmd_{a}
int TaticoIntAlunoHor::criaVariavelTaticoFolgaMinimoDemandaPorAluno( int campusId, int P_ATUAL )
{
    int numVars = 0;
 
   if ( ! problemData->parametros->considerarMinPercAtendAluno )
   {
		return numVars;
   }

	Campus *cp = problemData->refCampus[campusId];

	double coef1 = 100;
	double coef2 = 500;
	double coef3 = 1000;

	ITERA_GGROUP_LESSPTR( itAluno, problemData->alunos, Aluno )
	{
		Aluno *aluno = *itAluno;

		double ub = (double) aluno->getNroCredsOrigRequeridosP1();
		
		double ub1 = ub/3;
		double ub2 = ub/3;
		double ub3 = ub/3;

		VariableTatInt v;
		v.reset();
		v.setType( VariableTatInt::V_FOLGA_ALUNO_MIN_ATEND1 );
		v.setAluno( aluno );
		
		if ( vHashTatico.find( v ) == vHashTatico.end() )
		{
			vHashTatico[v] = lp->getNumCols();
			
			OPT_COL col( OPT_COL::VAR_CONTINUOUS, coef1, 0.0, ub1, ( char * )v.toString().c_str() );
			lp->newCol(col);
				
			numVars++;
		}		

		v.reset();
		v.setType( VariableTatInt::V_FOLGA_ALUNO_MIN_ATEND2 );
		v.setAluno( aluno );
		
		if ( vHashTatico.find( v ) == vHashTatico.end() )
		{
			vHashTatico[v] = lp->getNumCols();
			
			OPT_COL col( OPT_COL::VAR_CONTINUOUS, coef2, 0.0, ub2, ( char * )v.toString().c_str() );
			lp->newCol(col);
				
			numVars++;
		}		

		v.reset();
		v.setType( VariableTatInt::V_FOLGA_ALUNO_MIN_ATEND3 );
		v.setAluno( aluno );
		
		if ( vHashTatico.find( v ) == vHashTatico.end() )
		{
			vHashTatico[v] = lp->getNumCols();
			
			OPT_COL col( OPT_COL::VAR_CONTINUOUS, coef3, 0.0, ub3, ( char * )v.toString().c_str() );
			lp->newCol(col);
				
			numVars++;
		}	
	}

	return numVars;
}

// fos_{i,d,cp}
int TaticoIntAlunoHor::criaVariavelFolgaOcupacaoSala( int campusId, int P_ATUAL )
{
	int num_vars = 0;
	
	if ( ! problemData->parametros->min_folga_ocupacao_sala )
	{
		return num_vars;
	}

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
		 		 
         for ( int turma = 1; turma <= disciplina->getNumTurmas(); turma++ )
         {
            VariableTatInt v;
            v.reset();
            v.setType( VariableTatInt::V_FOLGA_OCUPA_SALA );

            v.setTurma( turma );            // i
            v.setDisciplina( disciplina );  // d
            v.setCampus( cp );				// cp

            if ( vHashTatico.find(v) == vHashTatico.end() )
            {
				bool fixar=false;
				if ( !criaVariavelTaticoInt( &v, fixar, P_ATUAL ) )
				{
					continue;
				}

                lp->getNumCols();
                vHashTatico[v] = lp->getNumCols();

			    int maxP = disciplina->getMaxAlunosP();
			    int maxT = disciplina->getMaxAlunosT();
			    int maxAlunosTurma = ( (maxP > maxT) ? maxP : maxT );
			   
			    double coef = 2.0;
				double lowerBound = 0.0;
				double upperBound = maxAlunosTurma;

				OPT_COL col( OPT_COL::VAR_INTEGRAL, coef, lowerBound, upperBound,
                     ( char * )v.toString().c_str() );

                lp->newCol( col ); 
                
				num_vars++;
            }
         }
      }
   }

	return num_vars;
}

// uu_{u,oft,p}
int TaticoIntAlunoHor::criaVariavelUsaPeriodoUnid( int campusId )
{
	int num_vars = 0;
	
	if ( !problemData->parametros->limitar_unidades_por_periodo )
	{
		return num_vars;
	}

	Campus* cp = problemData->refCampus[campusId];
	  
	ITERA_GGROUP_LESSPTR( itUnidade, cp->unidades, Unidade )
	{
		ITERA_GGROUP_LESSPTR( itOferta, cp->ofertas, Oferta )
		{
			Oferta* oft = *itOferta;
					
			ITERA_GGROUP_N_PT( itPeriodo, oft->curriculo->periodos, int )
			{
				int periodo = *itPeriodo;

				VariableTatInt v;
				v.reset();
				v.setType( VariableTatInt::V_UNID_PERIODO );
				v.setPeriodo( periodo );				// p
				v.setOferta( oft );						// oft
				v.setUnidade( *itUnidade );				// u
								
				if ( vHashTatico.find(v) == vHashTatico.end() )
				{
					vHashTatico[v] = lp->getNumCols();
									
					double coef = 0.0;
					int lowerBound = 0;
					int upperBound = 1;
						
					OPT_COL col( OPT_COL::VAR_BINARY, coef, lowerBound, upperBound,
						( char * )v.toString().c_str() );

					lp->newCol( col );
					num_vars++;
				}
			}
		}
   }

	return num_vars;
}

// us_{s,oft,p}
int TaticoIntAlunoHor::criaVariavelUsaPeriodoSala( int campusId )
{
	int num_vars = 0;
	
	if ( !problemData->parametros->limitar_salas_por_periodo )
	{
		return num_vars;
	}

	Campus* cp = problemData->refCampus[campusId];
	  
	ITERA_GGROUP_LESSPTR( itUnidade, cp->unidades, Unidade )
	{
		ITERA_GGROUP_LESSPTR( itCjtSala, itUnidade->conjutoSalas, ConjuntoSala )
		{
			ITERA_GGROUP_LESSPTR( itOferta, cp->ofertas, Oferta )
			{
				Oferta* oft = *itOferta;
					
				ITERA_GGROUP_N_PT( itPeriodo, oft->curriculo->periodos, int )
				{
					int periodo = *itPeriodo;

					VariableTatInt v;
					v.reset();
					v.setType( VariableTatInt::V_SALA_PERIODO );
					v.setPeriodo( periodo );				// p
					v.setOferta( oft );						// oft
					v.setSubCjtSala( *itCjtSala );			// s
								
					if ( vHashTatico.find(v) == vHashTatico.end() )
					{
						vHashTatico[v] = lp->getNumCols();
									
						double coef = 0.0;
						int lowerBound = 0;
						int upperBound = 1;
						
						OPT_COL col( OPT_COL::VAR_BINARY, coef, lowerBound, upperBound,
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

// fuu_{u,oft,p}
int TaticoIntAlunoHor::criaVariavelFolgaPeriodoUnid( int campusId, int prioridade )
{
	int num_vars = 0;
	
	if ( !problemData->parametros->limitar_unidades_por_periodo )
	{
		return num_vars;
	}

	Campus* cp = problemData->refCampus[campusId];
	  
	ITERA_GGROUP_LESSPTR( itOferta, cp->ofertas, Oferta )
	{
		Oferta* oft = *itOferta;
					
		ITERA_GGROUP_N_PT( itPeriodo, oft->curriculo->periodos, int )
		{
			int periodo = *itPeriodo;

			if ( ! problemData->possibilidCompartNoPeriodo( oft, periodo, campusId, prioridade ) )
				continue;

			VariableTatInt v;
			v.reset();
			v.setType( VariableTatInt::V_FOLGA_UNID_PERIODO );
			v.setPeriodo( periodo );				// p
			v.setOferta( oft );						// oft
								
			if ( vHashTatico.find(v) == vHashTatico.end() )
			{
				vHashTatico[v] = lp->getNumCols();
									
				double coef = 200.0;
				int lowerBound = 0;
				int upperBound = max( lowerBound, cp->unidades.size() - 1 );
						
				OPT_COL col( OPT_COL::VAR_INTEGRAL, coef, lowerBound, upperBound,
					( char * )v.toString().c_str() );

				lp->newCol( col );
				num_vars++;
			}
		}
   }

	return num_vars;
}

// fus_{s,oft,p}
int TaticoIntAlunoHor::criaVariavelFolgaPeriodoSala( int campusId, int prioridade )
{
	int num_vars = 0;
	
	if ( !problemData->parametros->limitar_salas_por_periodo )
	{
		return num_vars;
	}

	Campus* cp = problemData->refCampus[campusId];
	  
	ITERA_GGROUP_LESSPTR( itOferta, cp->ofertas, Oferta )
	{
		Oferta* oft = *itOferta;
					
		ITERA_GGROUP_N_PT( itPeriodo, oft->curriculo->periodos, int )
		{
			int periodo = *itPeriodo;
			
			if ( ! problemData->possibilidCompartNoPeriodo( oft, periodo, campusId, prioridade ) )
				continue;

			VariableTatInt v;
			v.reset();
			v.setType( VariableTatInt::V_FOLGA_SALA_PERIODO );
			v.setPeriodo( periodo );				// p
			v.setOferta( oft );						// oft
								
			if ( vHashTatico.find(v) == vHashTatico.end() )
			{
				vHashTatico[v] = lp->getNumCols();
									
				double coef = 100.0;
				int lowerBound = 0;
				int upperBound = max( lowerBound, cp->getTotalSalas() - 1 );
						
				OPT_COL col( OPT_COL::VAR_INTEGRAL, coef, lowerBound, upperBound,
					( char * )v.toString().c_str() );

				lp->newCol( col );
				num_vars++;
			}
		}
	}

	return num_vars;
}





/* ----------------------------------------------------------------------------------
	
							RESTRICOES TATICO POR ALUNO COM HORARIOS
 ---------------------------------------------------------------------------------- */


int TaticoIntAlunoHor::criaRestricoesTatico( int campusId, int prioridade, int r )
{
	int restricoes = 0;
	CPUTimer timer;
	double dif = 0.0;

#ifdef PRINT_cria_restricoes
	int numRestAnterior = 0;
#endif
	
	timer.start();
	restricoes += criaRestricaoProfDescansoMinimo( campusId );
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest criaRestricaoProfDescansoMinimo: " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
#endif	
	
	timer.start();
	restricoes += criaRestricaoTaticoAssociaVeX( campusId );				// Restricao 1.2.2
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest criaRestricaoTaticoAssociaVeX: " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
#endif
	

	timer.start();
	restricoes += criaRestricaoTaticoUsoDeSalaParaCadaHorario( campusId );				// Restricao 1.2.3
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest criaRestricaoTaticoUsoDeSalaParaCadaHorario: " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
#endif


	timer.start();
	restricoes += criaRestricaoTaticoAtendimentoUnicoTurmaDiscDia( campusId );				// Restricao 1.2.4
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest criaRestricaoTaticoAtendimentoUnicoTurmaDiscDia: " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
#endif


	timer.start();
	restricoes += criaRestricaoTaticoAtendeAluno( campusId, prioridade );						// Restricao 1.2.5
	restricoes += criaRestricaoTaticoAtendeAlunoEquiv( campusId, prioridade );					// Restricao 1.2.5
	restricoes += criaRestricaoTaticoAtendeAlunoEquivTotal( campusId, prioridade );				// Restricao 1.2.5	
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest criaRestricaoTaticoAtendeAluno: " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
#endif

	/*
	timer.start();
	restricoes += criaRestricaoTaticoTurmaDiscDiasConsec( campusId );				// Restricao 1.2.6
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest \"1.2.6\": " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
#endif*/


	
	timer.start();
	restricoes += criaRestricaoTaticoLimitaAberturaTurmas( campusId, prioridade );			// Restricao 1.2.7
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest criaRestricaoTaticoLimitaAberturaTurmas: " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
#endif

	
	
	timer.start();
	restricoes += criaRestricaoTaticoLimitaMaximoAlunosNasTurmas( campusId, prioridade );
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest criaRestricaoTaticoLimitaMaximoAlunosNasTurmas: " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
#endif


	timer.start();
	restricoes += criaRestricaoTaticoDivisaoCredito_hash( campusId );			// Restricao 1.2.8	
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
	restricoes += criaRestricaoTaticoAlunoHorario( campusId );					// Restricao 1.2.12	
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest \"1.2.12\": " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
#endif

  	timer.start();
//	restricoes += criaRestricaoTaticoAlunoUnidDifDia( campusId );			// Restricao 1.2.14
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest \"1.2.14\": " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
#endif

  	timer.start();
	restricoes += criaRestricaoTaticoAlunoDiscPraticaTeorica_MxN( campusId );				// Restricao 1.2.17
	restricoes += criaRestricaoTaticoAlunoDiscPraticaTeoricaEquiv_MxN( campusId, prioridade );	// Restricao 1.2.17
	restricoes += criaRestricaoTaticoAlunoDiscPraticaTeorica_1x1( campusId );	
	restricoes += criaRestricaoTaticoAlunoDiscPraticaTeorica_1xN( campusId );	
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest \"1.2.17\": " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
#endif

	/*
  	timer.start();
	restricoes += criaRestricaoTaticoAlunoDiscPraticaTeorica_1xN_antiga( campusId ); // Restricao 1.2.30	
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest \"1.2.17b\": " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
#endif

  	timer.start();
	restricoes += criaRestricaoTaticoAlunosMesmaTurmaPratica( campusId );	// Restricao 1.2.30	
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest \"1.2.17c\": " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
#endif
	*/


  	timer.start();
	restricoes += criaRestricaoTaticoMinDiasAluno( campusId );	// Restricao 1.2.18
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest \"1.2.18\": " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
#endif


  	timer.start();
	restricoes += criaRestricaoTaticoMaxDiasAluno( campusId );	// Restricao 1.2.19
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest \"1.2.19\": " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
#endif
			   

  	timer.start();
	restricoes += criaRestricaoTaticoSalaUnica( campusId );	// Restricao 1.2.19
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest \"1.2.20\": " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
#endif


  	timer.start();
	restricoes += criaRestricaoTaticoSalaPorTurma( campusId );	// Restricao 1.2.19
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest \"1.2.21\": " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
#endif


  	timer.start();
	restricoes += criaRestricaoTaticoAssociaAlunoEGaranteNroCreds( campusId );	// Restricao 1.2.19
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest \"1.2.22\": " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
#endif
		

  	timer.start();
	restricoes += criaRestricaoTaticoAbreTurmasEmSequencia( campusId );	// Restricao 1.2.19
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest \"1.2.23\": " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
#endif
	

  	timer.start();
	restricoes += criaRestricaoTaticoAlunoCurso( campusId );	// Restricao 1.2.19
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest \"1.2.24\": " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
#endif

  	timer.start();
	restricoes += criaRestricaoTaticoCursosIncompat( campusId );	// Restricao 1.2.20
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest \"1.2.25\": " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
#endif


  	timer.start();
	restricoes += criaRestricaoTaticoProibeCompartilhamento( campusId );	// Restricao 1.2.21
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest \"1.2.26\": " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
#endif
	
  	timer.start();
	restricoes += criaRestricaoPrioridadesDemanda_v2( campusId, prioridade );	// Restricao 1.2.21
	//restricoes += criaRestricaoPrioridadesDemanda( campusId, prioridade );	// Restricao 1.2.21
	//restricoes += criaRestricaoPrioridadesDemandaEquiv( campusId, prioridade );	// Restricao 1.2.21
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest \"1.2.27\": " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
#endif
	

  	timer.start();
	restricoes += criaRestricaoTaticoFormandos( campusId, prioridade, r );	// Restricao 1.2.21
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest \"1.2.28\": " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
#endif
	
// Não precisa, a restrição criaRestricaoTaticoSalaUnica() já engloba essa
//  	timer.start();
//	restricoes += criaRestricaoTaticoAtivaZ( campusId );	// Restricao 1.2.29
//	timer.stop();
//	dif = timer.getCronoCurrSecs();
//
//#ifdef PRINT_cria_restricoes
//	std::cout << "numRest \"1.2.29\": " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
//	numRestAnterior = restricoes;
//#endif
	

  	timer.start();
	restricoes += criaRestricaoTaticoTurmaComOsMesmosAlunosPorAula( campusId );	// Restricao 1.2.29
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest \"1.2.29\": " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
#endif
	

  	timer.start();
	restricoes += criaRestricaoTaticoConsideraHorariosProfs( campusId );	// Restricao 1.2.30
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest \"1.2.30\": " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
#endif


  	timer.start();
	restricoes += criaRestricaoTaticoAlocMinAluno( campusId );	// Restricao 1.2.31
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest \"1.2.31\": " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
#endif
	

  	timer.start();
	restricoes += criaRestricaoFolgaOcupacaoSala( campusId );	// Restricao 1.2.32
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest \"1.2.32\": " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
#endif
	

  	timer.start();
	restricoes += criaRestricaoLimitaUnidPeriodo( campusId );	// Restricao 1.2.33
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest \"1.2.33\": " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
#endif
	

  	timer.start();
	restricoes += criaRestricaoUsaUnidPeriodo( campusId );	// Restricao 1.2.34
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest \"1.2.34\": " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
#endif


  	timer.start();
	restricoes += criaRestricaoLimitaSalaPeriodo( campusId );	// Restricao 1.2.35
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest \"1.2.35\": " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
#endif
	

  	timer.start();
	restricoes += criaRestricaoUsaSalaPeriodo( campusId );	// Restricao 1.2.36
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest \"1.2.36\": " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
#endif


	return restricoes;
}


int TaticoIntAlunoHor::criaRestricaoTaticoAssociaVeX( int campusId )
{
	int restricoes = 0;
	int nnz;
	char name[ 1024 ];

	VariableTatInt v;
	ConstraintTatInt c;

	VariableTatIntHash::iterator vit;
	ConstraintTatIntHash::iterator cit;
		
    std::vector<int> idxC;
    std::vector<int> idxR;
    std::vector<double> valC;

	vit = vHashTatico.begin();
	for ( ; vit != vHashTatico.end(); vit++ )
	{
		if( vit->first.getType() != VariableTatInt::V_CREDITOS &&
			vit->first.getType() != VariableTatInt::V_ALUNO_CREDITOS )
		{
			continue;
		}

		VariableTatInt v = vit->first;
		
		if ( v.getUnidade()->getIdCampus() != campusId )
			continue;

		c.reset();
		c.setType( ConstraintTatInt::C_ASSOCIA_V_X );		
		c.setTurma( v.getTurma() );
		c.setDisciplina( v.getDisciplina() );
		c.setUnidade( v.getUnidade() );
		c.setSubCjtSala( v.getSubCjtSala() );
		c.setDia( v.getDia() );
		c.setHorarioAulaInicial( v.getHorarioAulaInicial() );
		c.setHorarioAulaFinal( v.getHorarioAulaFinal() );
      c.setDateTimeInicial( v.getDateTimeInicial() );
      c.setDateTimeFinal( v.getDateTimeFinal() );
		
		double coef=0.0;
		if ( vit->first.getType() == VariableTatInt::V_CREDITOS )
			coef = - v.getSubCjtSala()->getCapacidadeRepr() ;
		else if ( vit->first.getType() == VariableTatInt::V_ALUNO_CREDITOS )
			coef = 1.0;

		cit = cHashTatico.find(c);
		if(cit == cHashTatico.end())
		{
			sprintf( name, "%s", c.toString( etapa ).c_str() );
			OPT_ROW row( 100, OPT_ROW::LESS , 0.0 , name );

			row.insert( vit->second, coef);

			cHashTatico[ c ] = lp->getNumRows();
			lp->addRow( row );
			restricoes++;
		}
		else
		{
			idxC.push_back(vit->second);
			idxR.push_back(cit->second);
			valC.push_back(coef);
		}
	}

	lp->updateLP();
	lp->chgCoefList(idxC.size(),idxR.data(),idxC.data(),valC.data());
	lp->updateLP();

	idxC.clear();
	idxR.clear();
	valC.clear();

	return restricoes;

}

int TaticoIntAlunoHor::criaRestricaoTaticoUsoDeSalaParaCadaHorario( int campusId )
{
	int restricoes = 0;

	int nnz;
	char name[ 1024 ];

	VariableTatInt v;
	ConstraintTatInt c;

	VariableTatIntHash::iterator vit;
	ConstraintTatIntHash::iterator cit;

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

   std::vector<int> idxC;
   std::vector<int> idxR;
   std::vector<double> valC;

	vit = vHashTatico.begin();
	while(vit != vHashTatico.end())
	{
		if(vit->first.getType() != VariableTatInt::V_CREDITOS)
		{
			vit++;
			continue;
		}

		VariableTatInt v = vit->first;

		if ( campus->unidades.find(v.getUnidade()) == campus->unidades.end())
		{
			vit++;
			continue;
		}

      //Elimina repetidos
      GGroup<DateTime*,LessPtr<DateTime> > horariosDifSala;

		Sala *sala = v.getSubCjtSala()->salas.begin()->second;
		ITERA_GGROUP( itHor, sala->horariosDia, HorarioDia )
		{
			HorarioAula* h = itHor->getHorarioAula();

			if ( itHor->getDia() != v.getDia() )					
				continue;

			 std::pair<DateTime*,int> auxP = problemData->horarioAulaDateTime[h->getId()];

			 if (horariosDifSala.find(auxP.first) != horariosDifSala.end())
			 {
				continue;
			 }
			 else
			 {
				horariosDifSala.add(auxP.first);
			 }

			DateTime fimF = v.getHorarioAulaFinal()->getInicio();
			fimF.addMinutes( v.getHorarioAulaFinal()->getTempoAula() );

			DateTime fimH = h->getInicio(); // controle
			fimH.addMinutes( h->getTempoAula() );
			
			if ( ( v.getHorarioAulaInicial()->getInicio() <= h->getInicio() ) && ( fimF >  h->getInicio() ) )
			{
				c.reset();
				c.setType( ConstraintTatInt::C_SALA_HORARIO );
				c.setCampus( campus );
				c.setUnidade( v.getUnidade() );
				c.setSubCjtSala( v.getSubCjtSala() );
				c.setDia( v.getDia() );
				c.setHorarioAulaInicial( h );
            std::pair<DateTime*,int> auxP = problemData->horarioAulaDateTime[h->getId()];
				c.setDateTimeInicial(auxP.first);

				cit = cHashTatico.find(c);
				if(cit == cHashTatico.end())
				{
					sprintf( name, "%s", c.toString( etapa ).c_str() );
					OPT_ROW row( 100, OPT_ROW::LESS , 1 , name );

					row.insert( vit->second, 1.0);

					cHashTatico[ c ] = lp->getNumRows();
					lp->addRow( row );
					restricoes++;
				}
				else
				{
					idxC.push_back(vit->second);
					idxR.push_back(cit->second);
					valC.push_back(1.0);
				}
			}
		}

		vit++;
	}

   lp->updateLP();
   lp->chgCoefList(idxC.size(),idxR.data(),idxC.data(),valC.data());
   lp->updateLP();

   idxC.clear();
   idxR.clear();
   valC.clear();

   if ( ! this->CRIAR_VARS_FIXADAS )
   {
	    GGroup< VariableTatico *, LessPtr<VariableTatico> > variaveis_xh = *vars_xh;
		ITERA_GGROUP_LESSPTR( itX, variaveis_xh, VariableTatico )
		{
			if( itX->getType() != VariableTatico::V_CREDITOS )
			{
				itX++;
				continue;
			}

			VariableTatico v = **itX;

			if ( campus->unidades.find(v.getUnidade()) == campus->unidades.end())
			{
				continue;
			}

			int dia = v.getDia();
			Sala *sala = v.getSubCjtSala()->salas.begin()->second;
			ITERA_GGROUP( itHor, sala->horariosDia, HorarioDia )
			{
				HorarioAula* h = itHor->getHorarioAula();

				if ( itHor->getDia() != dia )					
					continue;

				DateTime fimF = v.getHorarioAulaFinal()->getInicio();
				fimF.addMinutes( v.getHorarioAulaFinal()->getTempoAula() );

				DateTime fimH = h->getInicio(); // controle
				fimH.addMinutes( h->getTempoAula() );
			
				if ( ( v.getHorarioAulaInicial()->getInicio() <= h->getInicio() ) && ( fimF >  h->getInicio() ) )
				{
					c.reset();
					c.setType( ConstraintTatInt::C_SALA_HORARIO );
					c.setCampus( campus );
					c.setUnidade( v.getUnidade() );
					c.setSubCjtSala( v.getSubCjtSala() );
					c.setDia( dia );
					c.setHorarioAulaInicial( h );
               std::pair<DateTime*,int> auxP = problemData->horarioAulaDateTime[h->getId()];
				   c.setDateTimeInicial(auxP.first);
               
					
					cit = cHashTatico.find(c);
					if ( cit != cHashTatico.end() )
					{		
						double newRhs = lp->getRHS( cit->second ) - 1;
						lp->chgRHS( cit->second, newRhs );
					}
				}
			}
		}
		
		lp->updateLP();
   }

	return restricoes;

}

int TaticoIntAlunoHor::criaRestricaoTaticoAtendimentoUnicoTurmaDiscDia( int campusId )
{
	int restricoes = 0;

	int nnz;
	char name[ 1024 ];

	VariableTatInt v;
	ConstraintTatInt c;

	VariableTatIntHash::iterator vit;
	ConstraintTatIntHash::iterator cit;

	Campus *campus = NULL;
   std::vector<int> idxC;
   std::vector<int> idxR;
   std::vector<double> valC;

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
	for ( ; vit != vHashTatico.end(); vit++)
	{		
		VariableTatInt v = vit->first;

		if( v.getType() != VariableTatInt::V_CREDITOS )
		{
			continue;
		}
		if ( problemData->cp_discs[campusId].find( v.getDisciplina()->getId() ) ==
			 problemData->cp_discs[campusId].end() )
		{
			continue;
		}

		c.reset();
		c.setType( ConstraintTatInt::C_UNICO_ATEND_TURMA_DISC_DIA );
		c.setCampus( campus );
		c.setTurma( v.getTurma() );
		c.setDisciplina( v.getDisciplina() );
		c.setDia( v.getDia() );

		cit = cHashTatico.find(c);

		if(cit == cHashTatico.end())
		{
			sprintf( name, "%s", c.toString( etapa ).c_str() );
			OPT_ROW row( 100, OPT_ROW::LESS , 1.0 , name );

			row.insert( vit->second, 1.0);

			cHashTatico[ c ] = lp->getNumRows();
			lp->addRow( row );
			restricoes++;
		}
		else
		{
			idxC.push_back(vit->second);
			idxR.push_back(cit->second);
			valC.push_back(1.0);
		}
	}

   lp->updateLP();
   lp->chgCoefList(idxC.size(),idxR.data(),idxC.data(),valC.data());
   lp->updateLP();

   idxC.clear();
   idxR.clear();
   valC.clear();

	return restricoes;
}

// Restricao 1.2.8
/*====================================================================/
%DocBegin TRIEDA_LOAD_MODEL

%Constraint 
Contabiliza se há turmas da mesma disciplina em dias consecutivos (*)
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
int TaticoIntAlunoHor::criaRestricaoTaticoTurmaDiscDiasConsec( int campusId )
{
   int restricoes = 0;

   char name[ 1024 ];
   int nnz;

   ConstraintTatInt c;
   VariableTatInt v;
   VariableTatIntHash::iterator it_v;

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
		  if ( ( problemData->mapDiscSubstituidaPor.find( disciplina ) !=
				 problemData->mapDiscSubstituidaPor.end() ) &&
				!problemData->ehSubstituta( disciplina ) )
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

		  for ( int turma = 1; turma <= disciplina->getNumTurmas(); turma++ )
		  {
			 GGroup< int, std::less<int> >::iterator itDiasLetDisc = disciplina->diasLetivos.begin();
			 
			 // Só cria as restrições a partir do segundo dia
			 // Já que a estrutura é ordenada, pula o primeiro.
			 if ( itDiasLetDisc != disciplina->diasLetivos.end() )
				itDiasLetDisc++;

			 for (; itDiasLetDisc != disciplina->diasLetivos.end(); itDiasLetDisc++ )
			 {
				 int dia = *itDiasLetDisc;

				c.reset();
				c.setType( ConstraintTatInt::C_TURMA_DISC_DIAS_CONSEC );

				c.setCampus( *itCampus );
				c.setDisciplina( disciplina );
				c.setTurma( turma );
				c.setDia( dia );

				sprintf( name, "%s", c.toString( etapa ).c_str() ); 

				if ( cHashTatico.find( c ) != cHashTatico.end() )
				{
				   continue;
				}

				nnz = ( problemData->totalSalas * 2 + 1 );
				OPT_ROW row( nnz, OPT_ROW::GREATER , -1.0 , name );

				v.reset();
				v.setType( VariableTatInt::V_DIAS_CONSECUTIVOS );
				v.setTurma( turma );
				v.setDisciplina( disciplina );
				v.setDia( dia );
				v.setCampus( *itCampus );

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

								if ( *hf < *hi )
								{
							 		continue;
								}

								v.reset();
								v.setType( VariableTatInt::V_CREDITOS );

								v.setTurma( turma );
								v.setDisciplina( disciplina );
								v.setUnidade( *itUnidade );
								v.setSubCjtSala( *itCjtSala );
								v.setHorarioAulaInicial( hi );
								v.setHorarioAulaFinal( hf );
								v.setDia( dia );
                        std::pair<DateTime*,int> auxP = problemData->horarioAulaDateTime[hi->getId()];
							   v.setDateTimeInicial(auxP.first);
                        auxP = problemData->horarioAulaDateTime[hf->getId()];
							   v.setDateTimeFinal(auxP.first);

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

				if ( row.getnnz() > 1 )
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
Não permitir que alunos de cursos diferentes incompatíveis compartilhem turmas (*)
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

int TaticoIntAlunoHor::criaRestricaoTaticoCursosIncompat( int campusId )
{
   int restricoes = 0;
   char name[ 1024 ];
   int nnz;

   ConstraintTatInt c;
   VariableTatInt v;
   VariableTatIntHash::iterator it_v;

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
					if ( ( problemData->mapDiscSubstituidaPor.find( disciplina ) !=
						   problemData->mapDiscSubstituidaPor.end() ) &&
						 !problemData->ehSubstituta( disciplina ) )
					{
						continue;
					}
					#pragma endregion	
										
					if ( !c1->possuiDisciplina(disciplina) || !c2->possuiDisciplina(disciplina) )
						continue;
					

					for ( int turma = 1; turma <= disciplina->getNumTurmas(); turma++ )
					{
						c.reset();
						c.setType( ConstraintTatInt::C_ALUNOS_CURSOS_INCOMP );
						c.setCampus( *itCampus );
						c.setParCursos( std::make_pair( c1, c2 ) );
						c.setDisciplina( disciplina );
						c.setTurma( turma );

						sprintf( name, "%s", c.toString( etapa ).c_str() ); 

						if ( cHashTatico.find( c ) != cHashTatico.end() )
						{
							continue;
						}

						nnz = 3;

						OPT_ROW row( nnz, OPT_ROW::LESS , 1.0 , name );

						v.reset();
						v.setType( VariableTatInt::V_TURMA_ATEND_CURSO );
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
						v.setType( VariableTatInt::V_TURMA_ATEND_CURSO );
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


// Restricao 1.2.11
/*====================================================================/
%DocBegin TRIEDA_LOAD_MODEL

%Constraint 
Não permitir que alunos de cursos diferentes (mesmo que compativeis) compartilhem turmas.
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

int TaticoIntAlunoHor::criaRestricaoTaticoProibeCompartilhamento( int campusId )
{
   int restricoes = 0;

   if ( problemData->parametros->permite_compartilhamento_turma_sel )
   {
	   return restricoes;
   }

   char name[ 1024 ];
   int nnz;

   ConstraintTatInt c;
   VariableTatInt v;
   VariableTatIntHash::iterator it_v;

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
				if ( ( problemData->mapDiscSubstituidaPor.find( disciplina ) !=
						problemData->mapDiscSubstituidaPor.end() ) &&
						!problemData->ehSubstituta( disciplina ) )
				{
					continue;
				}
				#pragma endregion

				for ( int turma = 1; turma <= disciplina->getNumTurmas(); turma++ )
				{
					c.reset();
					c.setType( ConstraintTatInt::C_PROIBE_COMPARTILHAMENTO );
					c.setCampus( *itCampus );
					c.setParCursos( std::make_pair( c1, c2 ) );
					c.setDisciplina( disciplina );
					c.setTurma( turma );

					sprintf( name, "%s", c.toString( etapa ).c_str() ); 

					if ( cHashTatico.find( c ) != cHashTatico.end() )
					{
						continue;
					}

					nnz = 3;

					OPT_ROW row( nnz, OPT_ROW::LESS , 1.0 , name );

					v.reset();
					v.setType( VariableTatInt::V_TURMA_ATEND_CURSO );
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
					v.setType( VariableTatInt::V_TURMA_ATEND_CURSO );
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
					v.setType( VariableTatInt::V_SLACK_COMPARTILHAMENTO );
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

/*
	Para cada turma i, disciplina d:

	sum[a] s_{i,d,a} >= MinAlunos * (z_{i,d,cp} - f_{i,d,cp})
*/
int TaticoIntAlunoHor::criaRestricaoTaticoLimitaAberturaTurmas( int campusId, int prioridade )
{
   int restricoes = 0;
   
   if ( !problemData->parametros->min_alunos_abertura_turmas &&
	    !this->PERMITIR_NOVAS_TURMAS )
   {
	   return restricoes;
   }

   int MinAlunosPrat=1;
   int MinAlunosTeor=1;
   
   if ( problemData->parametros->min_alunos_abertura_turmas )
   {
	   MinAlunosTeor = problemData->parametros->min_alunos_abertura_turmas_value;
	   MinAlunosPrat = problemData->parametros->min_alunos_abertura_turmas_praticas_value;
   }

   if ( MinAlunosPrat <= 0 ) MinAlunosPrat=1;
   if ( MinAlunosTeor <= 0 ) MinAlunosTeor=1;

   int nnz;
   char name[ 1024 ];

   ConstraintTatInt c;
   VariableTatIntHash::iterator vit;
   ConstraintTatIntHash::iterator cit;

   std::vector< std::pair< int, int > > coeffList;
   std::vector< double > coeffListVal;
   std::pair< int, int > auxCoef;

   vit = vHashTatico.begin();

   for (; vit != vHashTatico.end(); vit++ )
   {
		VariableTatInt v = vit->first;

		if ( v.getType() != VariableTatInt::V_ALOCA_ALUNO_TURMA_DISC &&
			 v.getType() != VariableTatInt::V_FORMANDOS_NA_TURMA &&
			 v.getType() != VariableTatInt::V_ABERTURA )
		{
			continue;
		}
		
		Campus *campus;
		
		if ( v.getType() == VariableTatInt::V_ALOCA_ALUNO_TURMA_DISC )
			campus = v.getAlunoDemanda()->demanda->oferta->campus;
		else if ( v.getType() == VariableTatInt::V_FORMANDOS_NA_TURMA || v.getType() == VariableTatInt::V_ABERTURA )
			campus = v.getCampus();

		int turma = v.getTurma();
		Disciplina* disc = v.getDisciplina();
		
		int MinAlunos;
		if (disc->eLab()) 
			MinAlunos = MinAlunosPrat;
		else
			MinAlunos = MinAlunosTeor;

		double coef=0.0;
		if ( v.getType() == VariableTatInt::V_ALOCA_ALUNO_TURMA_DISC )
			coef = 1.0;
		else if ( v.getType() == VariableTatInt::V_FORMANDOS_NA_TURMA )
			coef = MinAlunos;
		else if ( v.getType() == VariableTatInt::V_ABERTURA )
			coef = - MinAlunos;

		c.reset();
		c.setType( ConstraintTatInt::C_MIN_ALUNOS_TURMA );
		c.setTurma( turma );
		c.setDisciplina( disc );
		c.setCampus( campus );

		cit = cHashTatico.find( c );

		if ( cit != cHashTatico.end() )
		{
			auxCoef.first = cit->second;
			auxCoef.second = vit->second;

			coeffList.push_back( auxCoef );
			coeffListVal.push_back( coef );
		}
		else
		{			
			sprintf( name, "%s", c.toString( etapa ).c_str() );
			nnz = 60; // TODO

			OPT_ROW row( nnz, OPT_ROW::GREATER, 0, name );
						
			row.insert( vit->second, coef );

			// Insere restrição
			cHashTatico[ c ] = lp->getNumRows();
			lp->addRow( row );
			restricoes++;
		}
   }

   chgCoeffList( coeffList, coeffListVal );

   return restricoes;
}

/*
	Para cada turma i, disciplina d:

	sum[a] s_{i,d,a} <= MaxAlunos_{d} * z_{i,d,cp}
*/
int TaticoIntAlunoHor::criaRestricaoTaticoLimitaMaximoAlunosNasTurmas( int campusId, int prioridade )
{
   int restricoes = 0;
   int nnz;
   char name[ 1024 ];

   ConstraintTatInt c;
   VariableTatIntHash::iterator vit;
   ConstraintTatIntHash::iterator cit;

   std::vector< std::pair< int, int > > coeffList;
   std::vector< double > coeffListVal;
   std::pair< int, int > auxCoef;

   vit = vHashTatico.begin();

   for (; vit != vHashTatico.end(); vit++ )
   {
		VariableTatInt v = vit->first;

		if ( v.getType() != VariableTatInt::V_ALOCA_ALUNO_TURMA_DISC &&
			 v.getType() != VariableTatInt::V_ABERTURA )
		{
			continue;
		}
		
		Campus *campus;
		
		if ( v.getType() == VariableTatInt::V_ALOCA_ALUNO_TURMA_DISC )
			campus = v.getAlunoDemanda()->demanda->oferta->campus;
		else if ( v.getType() == VariableTatInt::V_ABERTURA )
			campus = v.getCampus();

		if ( campus->getId() != campusId ) continue;

		int turma = v.getTurma();
		Disciplina* disc = v.getDisciplina();
		
		int maxP = v.getDisciplina()->getMaxAlunosP();
		int maxT = v.getDisciplina()->getMaxAlunosT();
		int maximoAlunosTurma = ( (maxP > maxT) ? maxP : maxT );

		if ( maximoAlunosTurma <= 0 )
		{
			std::cout << "\nEstranho. Maximo de alunos na disciplina " << v.getDisciplina()->getId() << " igual a zero";
			continue;
		}

		double coef=0.0;
		if ( v.getType() == VariableTatInt::V_ALOCA_ALUNO_TURMA_DISC )
			coef = 1.0;
		else if ( v.getType() == VariableTatInt::V_ABERTURA )
		{		
			int nroAlunosFixosNaTurma = 0;
			if ( ! this->CRIAR_VARS_FIXADAS )
				nroAlunosFixosNaTurma = problemData->existeTurmaDiscCampus( turma, disc->getId(), campusId );

			coef = - ( maximoAlunosTurma - nroAlunosFixosNaTurma );
			
			if ( maximoAlunosTurma < nroAlunosFixosNaTurma )
			{
				std::cout << "\nErro. Maximo de alunos na turma " << v.toString() << " violado."
					<< " Maximo " << maximoAlunosTurma << ", Nro de alunos " << nroAlunosFixosNaTurma;
				continue;
			}
		}
				
		c.reset();
		c.setType( ConstraintTatInt::C_MAX_ALUNOS_TURMA );
		c.setTurma( turma );
		c.setDisciplina( disc );
		c.setCampus( campus );

		cit = cHashTatico.find( c );

		if ( cit != cHashTatico.end() )
		{
			auxCoef.first = cit->second;
			auxCoef.second = vit->second;

			coeffList.push_back( auxCoef );
			coeffListVal.push_back( coef );
		}
		else
		{
			sprintf( name, "%s", c.toString( etapa ).c_str() );
			nnz = 60; // TODO

			OPT_ROW row( nnz, OPT_ROW::LESS, 0, name );
						
			row.insert( vit->second, coef );

			// Insere restrição
			cHashTatico[ c ] = lp->getNumRows();
			lp->addRow( row );
			restricoes++;
		}
   }

   chgCoeffList( coeffList, coeffListVal );

   return restricoes;
}



int TaticoIntAlunoHor::criaRestricaoTaticoAlunoDiscPraticaTeorica_MxN( int campusId )
{
   int restricoes = 0;
	
	if ( !problemData->parametros->discPratTeorMxN )
	{
		return restricoes;
	}
	
   if ( problemData->parametros->considerar_equivalencia_por_aluno && USAR_EQUIVALENCIA )
		return restricoes;

   char name[ 1024 ];
   int nnz;

   ConstraintTatInt c;
   VariableTatInt v;
   VariableTatIntHash::iterator it_v;   

   Campus *campus = problemData->refCampus[campusId];

	ITERA_GGROUP_LESSPTR( itAluno, problemData->alunos, Aluno )
	{
		Aluno *aluno = *itAluno;

		ITERA_GGROUP_LESSPTR( itAlDemanda, aluno->demandas, AlunoDemanda )
		{
			if ( itAlDemanda->demanda->oferta->getCampusId() != campusId )
			{
				continue;
			}						  

			AlunoDemanda *alDemPratica = *itAlDemanda;
			Disciplina *discPratica = itAlDemanda->demanda->disciplina;
			int prioridade = itAlDemanda->getPrioridade();

			// Pula disciplina teorica
			if ( discPratica->getId() > 0 )
				continue;

			Disciplina *discTeorica = problemData->refDisciplinas[ - discPratica->getId() ];

			if ( discTeorica == NULL )
			{
				std::cout<<"\nErro em criaRestricaoTaticoAlunoDiscPraticaTeorica_MxN: disciplina teorica nao encontrada.\n";
				continue;
			}
			

			c.reset();
			c.setType( ConstraintTatInt::C_ALUNO_DISC_PRATICA_TEORICA );
			c.setAluno( aluno );
			c.setDisciplina( discPratica );
			c.setCampus( campus );

			sprintf( name, "%s", c.toString( etapa ).c_str() ); 

			if ( cHashTatico.find( c ) != cHashTatico.end() )
			{
				continue;
			}

			nnz = discPratica->getNumTurmas() + discTeorica->getNumTurmas() + 1;

			OPT_ROW row( nnz, OPT_ROW::EQUAL , 0.0 , name );

			for ( int turma = 1; turma <= discPratica->getNumTurmas(); turma++ )
			{
				VariableTatInt v;
				v.reset();
				v.setType( VariableTatInt::V_ALOCA_ALUNO_TURMA_DISC );
				v.setAluno( aluno );
				v.setDisciplina( discPratica );
				v.setTurma( turma );
				v.setCampus( campus );
				v.setAlunoDemanda( alDemPratica );
				
				it_v = vHashTatico.find( v );
				if( it_v != vHashTatico.end() )
				{
					row.insert( it_v->second, 1.0 );
				}
			}

			for ( int turma = 1; turma <= discTeorica->getNumTurmas(); turma++ )
			{			
				AlunoDemanda *alDemTeorica = problemData->procuraAlunoDemanda(discTeorica->getId(), aluno->getAlunoId(), prioridade);
				if ( alDemTeorica==NULL ){
					std::cout<<"\nErro: AlunoDemanda teorico nao encontrado. Disc" << discTeorica->getId() << " Aluno" << aluno->getAlunoId();
					continue;
				}

				VariableTatInt v;
				v.reset();
				v.setType( VariableTatInt::V_ALOCA_ALUNO_TURMA_DISC );
				v.setAluno( aluno );
				v.setDisciplina( discTeorica );
				v.setTurma( turma );
				v.setCampus( campus );
				v.setAlunoDemanda( alDemTeorica );

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

	return restricoes;

}


int TaticoIntAlunoHor::criaRestricaoTaticoAlunoDiscPraticaTeorica_1x1( int campusId )
{
	int restricoes=0;

	// relação 1x1
	if ( ! problemData->parametros->discPratTeor1x1 )
	{
		return restricoes;
	}

    char name[ 100 ];

	VariableTatInt v;
	ConstraintTatInt c;
	VariableTatIntHash::iterator vit;
	ConstraintTatIntHash::iterator cit;

	vit = vHashTatico.begin();
	for ( ; vit != vHashTatico.end(); vit++ )
	{
		v = vit->first;
	
		if( v.getType() == VariableTatInt::V_ALOCA_ALUNO_TURMA_DISC )
		{						
			Disciplina *disciplinaPratica = NULL;
						
			int discId = v.getDisciplina()->getId();
			if ( discId > 0 )
			{			
				// Só para disciplinas praticas/teoricas
				std::map< int, Disciplina * >::iterator itMapDisc = problemData->refDisciplinas.find( - discId );
				if ( itMapDisc == problemData->refDisciplinas.end() )
					continue;
				else
					disciplinaPratica = itMapDisc->second;
			}
			else
			{
				disciplinaPratica = v.getDisciplina();
			}
		
			c.reset();
			c.setType( ConstraintTatInt::C_ALUNO_DISC_PRATICA_TEORICA_1x1 );
			c.setDisciplina( disciplinaPratica );
			c.setTurma( v.getTurma() );
			c.setAluno( v.getAluno() );

			double coef = 0.0;
			if ( v.getDisciplina()->getId() < 0 )
				coef = 1.0;
			else
				coef = -1.0;

			cit = cHashTatico.find(c);
			if(cit != cHashTatico.end())
			{
				lp->chgCoef(cit->second, vit->second, coef);
			}
			else
			{
				int nnz = 2;

				sprintf( name, "%s", c.toString( etapa ).c_str() ); 
				OPT_ROW row( nnz, OPT_ROW::EQUAL, 0.0, name );

				row.insert(vit->second, coef);

				cHashTatico[ c ] = lp->getNumRows();

				lp->addRow( row );
				restricoes++;
			}
		}
	}

	return restricoes;
}


int TaticoIntAlunoHor::criaRestricaoTaticoAlunoDiscPraticaTeorica_1xN( int campusId )
{
	int restricoes=0;

	// relação 1xN
	if ( ! problemData->parametros->discPratTeor1xN )
	{
		return restricoes;
	}

    char name[ 100 ];
	
	VariableTatIntHash::iterator vit;
	ConstraintTatIntHash::iterator cit;

	vit = vHashTatico.begin();
	for ( ; vit != vHashTatico.end(); vit++ )
	{
		VariableTatInt v = vit->first;
	
		if( v.getType() == VariableTatInt::V_ALOCA_ALUNO_TURMA_DISC )
		{							
			Disciplina *disciplinaPratica = NULL;
			Disciplina *disciplinaTeorica = NULL;
			int turmaTeorica = -1;

			int discId = v.getDisciplina()->getId();
			if ( discId > 0 )
			{
				// Só para disciplinas praticas/teoricas
				std::map< int, Disciplina * >::iterator itMapDisc = problemData->refDisciplinas.find( - discId );
				if ( itMapDisc == problemData->refDisciplinas.end() )
					continue;

				disciplinaPratica = itMapDisc->second;				
				disciplinaTeorica = v.getDisciplina();
				turmaTeorica = v.getTurma();
			}
			else
			{
				disciplinaPratica = v.getDisciplina();
				disciplinaTeorica = problemData->retornaDisciplina( -discId );

				GGroup<int> tt = disciplinaPratica->getTurmasAssociadas( v.getTurma() );
				if ( tt.size() != 1 )
					std::cout<<"\nErro: relacao 1xN e disc pratica com " << tt.size() << " turmas teorica associadas.";

				turmaTeorica = *(tt.begin());					
			}
		
			ConstraintTatInt c;
			c.reset();
			c.setType( ConstraintTatInt::C_ALUNO_DISC_PRATICA_TEORICA_1xN );
			c.setDisciplina( disciplinaTeorica );	// disciplina TEORICA que tenha pratica associada
			c.setTurma( turmaTeorica );				// turma TEORICA
			c.setAluno( v.getAluno() );				// aluno

			double coef = 0.0;
			if ( v.getDisciplina()->getId() > 0 )
				coef = 1.0;
			else
				coef = -1.0;

			cit = cHashTatico.find(c);
			if(cit != cHashTatico.end())
			{
				lp->chgCoef(cit->second, vit->second, coef);
			}
			else
			{
				int totalTurmas = ( 1 + disciplinaTeorica->getTurmasAssociadas(turmaTeorica).size() );				
				int nnz = totalTurmas * problemData->getNroTotalDeFasesDoDia();
				
				sprintf( name, "%s", c.toString( etapa ).c_str() ); 
				OPT_ROW row( nnz, OPT_ROW::EQUAL, 0.0, name );

				row.insert(vit->second, coef);

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
Regra de divisão de créditos
%Desc 

%MatExp

\begin{eqnarray}
\sum\limits_{u \in U} \sum\limits_{s \in S_{u}} \sum\limits_{hi \in H} \sum\limits_{hf \in H} nCreds_{hi,hf} \cdot x_{i,d,u,s,hi,hf,t} = 
 \sum\limits_{k \in K_{d}}N_{d,k,t} \cdot m_{d,i,k} + fkp_{d,i,t} - fkm_{d,i,t} \nonumber \qquad 
\forall d \in D \quad
\forall i \in I_{d} \quad
\forall t \in T
\end{eqnarray}

%Data N_{d,k,t}
%Desc
número de créditos determinados para a disciplina $d$ no dia $t$ na combinação de divisão de crédito $k$.

%DocEnd
/====================================================================*/

int TaticoIntAlunoHor::criaRestricaoTaticoDivisaoCredito_hash( int campusId )
{
   int restricoes = 0;
   char name[ 1024 ];
   int nnz;

   std::vector< std::pair< int, int > > coeffList;
   std::vector< double > coeffListVal;
   std::pair< int, int > auxCoef;
   
   VariableTatIntHash::iterator vit;

   vit = vHashTatico.begin();
   while(vit != vHashTatico.end())
   {
	   VariableTatInt v = vit->first;

	   Disciplina *disciplina=NULL;
	   int turma;
	   GGroup<int> dias;
	   std::vector<double> coefs;

       if(vit->first.getType() == VariableTatInt::V_CREDITOS)
       {		 
		   Calendario *calendario = v.getHorarioAulaInicial()->getCalendario();
		   int nCreds = calendario->retornaNroCreditosEntreHorarios(v.getHorarioAulaInicial(), v.getHorarioAulaFinal());
		   coefs.push_back( nCreds );
		   disciplina = v.getDisciplina();
		   dias.add( v.getDia() );
		   turma = v.getTurma();
       }
       else if(vit->first.getType() == VariableTatInt::V_SLACK_COMBINACAO_DIVISAO_CREDITO_P)
       {         
		   coefs.push_back( -1.0 );		   
		   disciplina = v.getDisciplina();
		   dias.add( v.getDia() );
		   turma = v.getTurma();
       }
       else if(vit->first.getType() == VariableTatInt::V_SLACK_COMBINACAO_DIVISAO_CREDITO_M)
       {         
		   coefs.push_back( 1.0 );
		   disciplina = v.getDisciplina();
		   dias.add( v.getDia() );
		   turma = v.getTurma();
       }
       else if(vit->first.getType() == VariableTatInt::V_COMBINACAO_DIVISAO_CREDITO)
       {		   
		   int k = v.getK();
		   disciplina = v.getDisciplina();
		   turma = v.getTurma();
		   ITERA_GGROUP_N_PT( itDia, disciplina->diasLetivos, int )
		   {  			   
			   int numCreditos = disciplina->getNroCredsRegraDiv( k, *itDia );  // N{d,k,t}
			   coefs.push_back( -numCreditos );
			   dias.add( *itDia );
		   }
       }
	   else
	   {
		   vit++;
		   continue;
	   }

	   int at=0;	      
	   ITERA_GGROUP_N_PT( itDia, dias, int )
	   {	
		    if ( coefs[at] < 0.01 && coefs[at] > -0.01 )
			{
				at++;
				continue;
			}

		    int dia = *itDia;

		    ConstraintTatInt c;
		    c.reset();
		    c.setType( ConstraintTatInt::C_DIVISAO_CREDITO );
		    c.setDisciplina( disciplina );
		    c.setTurma( turma );
		    c.setDia( dia );
		    sprintf( name, "%s", c.toString( etapa ).c_str() ); 

			ConstraintTatIntHash::iterator cit;
			cit = cHashTatico.find( c );

			if ( cit != cHashTatico.end() )
			{
				auxCoef.first = cit->second;
				auxCoef.second = vit->second;

				coeffList.push_back( auxCoef );
				coeffListVal.push_back( coefs[at] );
			}
			else
			{
				sprintf( name, "%s", c.toString( etapa ).c_str() );
				nnz = 100;

				OPT_ROW row( nnz, OPT_ROW::EQUAL , 0.0, name );

				row.insert( vit->second, coefs[at] );
				cHashTatico[ c ] = lp->getNumRows();

				lp->addRow( row );
				restricoes++;
			}

			at++;
	   }

       vit++;
   }

   chgCoeffList( coeffList, coeffListVal );

   return restricoes;
}


// Restricao 1.2.15
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
int TaticoIntAlunoHor::criaRestricaoTaticoCombinacaoDivisaoCredito( int campusId )
{
   int restricoes = 0;
   char name[ 1024 ];
   int nnz;

   VariableTatInt v;
   ConstraintTatInt c;
   ConstraintTatIntHash::iterator cit;
   VariableTatIntHash::iterator vit;

   std::vector<int> idxC;
   std::vector<int> idxR;
   std::vector<double> valC;

	vit = vHashTatico.begin();
	for ( ; vit != vHashTatico.end(); vit++)
	{		
		VariableTatInt v = vit->first;

		if( v.getType() != VariableTatInt::V_COMBINACAO_DIVISAO_CREDITO )
		{
			continue;
		}
         
		c.reset();
        c.setType( ConstraintTatInt::C_COMBINACAO_DIVISAO_CREDITO );
        c.setDisciplina( v.getDisciplina() );
		c.setTurma( v.getTurma() );

		cit = cHashTatico.find(c);

		if(cit == cHashTatico.end())
		{
			sprintf( name, "%s", c.toString( etapa ).c_str() );
			OPT_ROW row( 10, OPT_ROW::LESS , 1.0 , name );

			row.insert( vit->second, 1.0);

			cHashTatico[ c ] = lp->getNumRows();
			lp->addRow( row );
			restricoes++;
		}
		else
		{
			idxC.push_back(vit->second);
			idxR.push_back(cit->second);
			valC.push_back(1.0);
		}
	}

   lp->updateLP();
   lp->chgCoefList(idxC.size(),idxR.data(),idxC.data(),valC.data());
   lp->updateLP();

   idxC.clear();
   idxR.clear();
   valC.clear();
   		
   		/*
   
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
	  if ( ( problemData->mapDiscSubstituidaPor.find( disciplina ) !=
			problemData->mapDiscSubstituidaPor.end() ) &&
			!problemData->ehSubstituta( disciplina ) )
	  {
		  continue;
	  }
	  #pragma endregion	

      for ( int i = 0; i < disciplina->getNumTurmas(); i++ )
      {
         c.reset();
         c.setType( ConstraintTatInt::C_COMBINACAO_DIVISAO_CREDITO );
         c.setDisciplina( disciplina );
         c.setTurma( i );

         sprintf( name, "%s", c.toString( etapa ).c_str() ); 

         if ( cHashTatico.find( c ) != cHashTatico.end() )
         {
            continue;
         }

         nnz = (int)( disciplina->combinacao_divisao_creditos.size() );
         OPT_ROW row( nnz, OPT_ROW::LESS , 1.0, name );

         for ( int k = 0; k < (int)disciplina->combinacao_divisao_creditos.size(); k++ )
         {
            v.reset();
            v.setType( VariableTatInt::V_COMBINACAO_DIVISAO_CREDITO );
            v.setTurma( i );
            v.setDisciplina( disciplina );
            v.setK( k );
			v.setCampus( problemData->refCampus[campusId] );

            it_v = vHashTatico.find( v );

            if ( it_v != vHashTatico.end() )
            {
               row.insert( it_v->second, 1.0 );
            }
         }

         if ( row.getnnz() > 1 )
         {
            cHashTatico[ c ] = lp->getNumRows();

            lp->addRow( row );
            restricoes++;
         }
      }
   } */

   return restricoes;
}


// Restricao 1.2.16
/*====================================================================/
%DocBegin TRIEDA_LOAD_MODEL

%Constraint 
Ativação da variável zc
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
int TaticoIntAlunoHor::criaRestricaoTaticoAtivacaoVarZC( int campusId )
{
   int restricoes = 0;
      
	if ( ! problemData->parametros->considerar_disciplinas_incompativeis_no_dia )
	   return restricoes;
	
   char name[ 1024 ];
   int nnz;

   ConstraintTatInt c;
   VariableTatInt v;
   ConstraintTatIntHash::iterator cit;
   VariableTatIntHash::iterator vit;

   std::vector<int> idxC;
   std::vector<int> idxR;
   std::vector<double> valC;

	vit = vHashTatico.begin();
	for ( ; vit != vHashTatico.end(); vit++)
	{		
		VariableTatInt v = vit->first;
		
		double coef = 0.0;
		if( v.getType() == VariableTatInt::V_ABERTURA_COMPATIVEL )
		{
			coef = - v.getDisciplina()->getNumTurmas();			
		}
		else if( v.getType() == VariableTatInt::V_CREDITOS )
		{
			if ( v.getUnidade()->getIdCampus() != campusId )
				continue;

			coef = 1.0;
		}
		else
		{
			continue;
		}

		c.reset();
        c.setType( ConstraintTatInt::C_VAR_ZC );
        c.setDisciplina( v.getDisciplina() );
		c.setDia( v.getDia() );
		c.setCampus( problemData->refCampus[campusId] );

		cit = cHashTatico.find(c);

		if ( cit == cHashTatico.end() )
		{
			sprintf( name, "%s", c.toString( etapa ).c_str() );
			OPT_ROW row( 100, OPT_ROW::LESS, 0.0, name );

			row.insert( vit->second, coef);

			cHashTatico[ c ] = lp->getNumRows();
			lp->addRow( row );
			restricoes++;
		}
		else
		{
			idxC.push_back(vit->second);
			idxR.push_back(cit->second);
			valC.push_back(coef);
		}
	}

   lp->updateLP();
   lp->chgCoefList(idxC.size(),idxR.data(),idxC.data(),valC.data());
   lp->updateLP();

   idxC.clear();
   idxR.clear();
   valC.clear();

   /*
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
		  if ( ( problemData->mapDiscSubstituidaPor.find( disciplina ) !=
				 problemData->mapDiscSubstituidaPor.end() ) &&
				!problemData->ehSubstituta( disciplina ) )
		  {
			   continue;
		  }
		  #pragma endregion
		  		 
		  GGroup< int >::iterator itDiasLetDisc = disciplina->diasLetivos.begin();

		  for (; itDiasLetDisc != disciplina->diasLetivos.end(); itDiasLetDisc++ )
		  {
             int dia = *itDiasLetDisc;

			 c.reset();
			 c.setType( ConstraintTatInt::C_VAR_ZC );

			 c.setCampus( *itCampus );
			 c.setDisciplina( disciplina );
			 c.setDia( dia );

			 sprintf( name, "%s", c.toString( etapa ).c_str() ); 
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

							if ( *hf < *hi )
							{
							 	continue;
							}

							for ( int turma = 1; turma <= disciplina->getNumTurmas(); turma++ )
							{
								v.reset();
								v.setType( VariableTatInt::V_CREDITOS );

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
			 v.setType( VariableTatInt::V_ABERTURA_COMPATIVEL );

			 v.setDisciplina( disciplina );
			 v.setDia( *itDiasLetDisc );
			 v.setCampus( *itCampus );

			 it_v = vHashTatico.find( v );
			 if ( it_v != vHashTatico.end() )
			 {
				 row.insert( it_v->second, - disciplina->getNumTurmas() );
			 }

			 if ( row.getnnz() > 1 )
			 {
				cHashTatico[ c ] = lp->getNumRows();

				lp->addRow( row );
				restricoes++;
			 }
		  }
	   }
   }
   */
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

int TaticoIntAlunoHor::criaRestricaoTaticoDisciplinasIncompativeis( int campusId )
{
   int restricoes = 0;
      
	if ( ! problemData->parametros->considerar_disciplinas_incompativeis_no_dia )
	   return restricoes;

   char name[ 1024 ];
   int nnz;

   ConstraintTatInt c;
   VariableTatInt v;
   VariableTatIntHash::iterator it_v;

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
	  if ( ( problemData->mapDiscSubstituidaPor.find( disciplina ) !=
			 problemData->mapDiscSubstituidaPor.end() ) &&
			!problemData->ehSubstituta( disciplina ) )
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
			if ( ( problemData->mapDiscSubstituidaPor.find( nova_disc ) !=
				   problemData->mapDiscSubstituidaPor.end() ) &&
				  !problemData->ehSubstituta( nova_disc ) )
			{
				continue;
			}
			#pragma endregion

            c.reset();
            c.setType( ConstraintTatInt::C_DISC_INCOMPATIVEIS );
			c.setParDisciplinas( std::make_pair( nova_disc, disciplina ) );
			c.setCampus( campus );
			c.setDia( *itDiasLetDisc );

            sprintf( name, "%s", c.toString( etapa ).c_str() ); 

            if ( cHashTatico.find( c ) != cHashTatico.end() )
            {
               continue;
            }

            nnz = 2;
            OPT_ROW row( nnz, OPT_ROW::LESS, 1.0, name );

            v.reset();
            v.setType( VariableTatInt::V_ABERTURA_COMPATIVEL );

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
int TaticoIntAlunoHor::criaRestricaoTaticoUnicaSalaParaTurmaDisc( int campusId )
{
   int restricoes = 0;

   int nnz;
   char name[ 100 ];

   VariableTatInt v;
   ConstraintTatInt c;
   VariableTatIntHash::iterator it_v;

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
		 if ( ( problemData->mapDiscSubstituidaPor.find( disciplina ) !=
				problemData->mapDiscSubstituidaPor.end() ) &&
				!problemData->ehSubstituta( disciplina ) )
		 {
			continue;
		 }
		 #pragma endregion

         for ( int turma = 1; turma <= disciplina->getNumTurmas(); turma++ )
         {
            c.reset();
            c.setType( ConstraintTatInt::C_TURMA_SALA );

            c.setCampus( *itCampus );
            c.setTurma( turma );
            c.setDisciplina( disciplina );

            sprintf( name, "%s", c.toString( etapa ).c_str() );

            if ( cHashTatico.find( c ) != cHashTatico.end() )
            {
               continue;
            }

            nnz = ( itCampus->getTotalSalas() * 7 );

            OPT_ROW row( nnz, OPT_ROW::LESS, 1, name );

            v.reset();
            v.setType( VariableTatInt::V_CREDITOS );

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

            // Insere restrição no Hash ---

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


/*
	Impede a alocação de aulas de um aluno em horários com sobreposição.
	Para cada dia t, horário h, e aluno al:

	sum[u] sum[s] sum[hi] sum[hf] v_{a,i,d,u,s,hi,hf,t} <= 1

	sendo al alocado em (i,d) e	(hi,hf) sobrepõe o inicio de h
*/
int TaticoIntAlunoHor::criaRestricaoTaticoAlunoHorario( int campusId )
{
   int restricoes = 0;
   int nnz;
   char name[ 1024 ];

   ConstraintTatInt c;
   VariableTatIntHash::iterator vit;
   ConstraintTatIntHash::iterator cit;

   std::vector< std::pair< int, int > > coeffList;
   std::vector< double > coeffListVal;
   std::pair< int, int > auxCoef;

   vit = vHashTatico.begin();

   for (; vit != vHashTatico.end(); vit++ )
   {
		VariableTatInt v = vit->first;

		if ( v.getType() != VariableTatInt::V_ALUNO_CREDITOS )
		{
			continue;
		}
		
		//Elimina repetidos
		GGroup<DateTime*,LessPtr<DateTime> > horariosDifSala;

		Aluno *aluno = v.getAluno();
		Sala *sala = v.getSubCjtSala()->salas.begin()->second;
		ITERA_GGROUP_LESSPTRPTR( it_horario_dia, sala->horariosDia, HorarioDia )
		{
			HorarioDia * horario_dia = ( *it_horario_dia );

			int dia = horario_dia->getDia();

			if ( v.getDia() != dia )
				continue;

			HorarioAula * horario_aula = horario_dia->getHorarioAula();
			std::pair<DateTime*,int> auxP = problemData->horarioAulaDateTime[horario_aula->getId()];

			 if (horariosDifSala.find(auxP.first) != horariosDifSala.end())
			 {
				continue;
			 }
			 else
			 {
				horariosDifSala.add(auxP.first);
			 }

			DateTime inicio = horario_aula->getInicio();
		
			DateTime vInicio = v.getHorarioAulaInicial()->getInicio();
			HorarioAula *horarioAulaFim = v.getHorarioAulaFinal();
			DateTime vFim = horarioAulaFim->getFinal();

			if ( !( ( vInicio <= inicio ) && ( vFim > inicio ) ) )
			{
				continue;
			}					
					
			c.reset();
			c.setType( ConstraintTatInt::C_ALUNO_HORARIO );
			c.setAluno( aluno );
			c.setDia( dia );
			c.setHorarioAulaInicial( horario_aula );
			c.setDateTimeInicial(auxP.first);

			cit = cHashTatico.find( c );

			if ( cit != cHashTatico.end() )
			{
				auxCoef.first = cit->second;
				auxCoef.second = vit->second;

				coeffList.push_back( auxCoef );
				coeffListVal.push_back( 1.0 );
			}
			else
			{
				double add_rhs = 0;

				if ( !CRIAR_VARS_FIXADAS )
				{
					std::map< Aluno*, GGroup< Trio< int /*campusId*/, int /*turma*/, Disciplina* > >, LessPtr< Aluno > >::iterator
						itMap = problemData->mapAluno_CampusTurmaDisc.find( aluno );
					if ( itMap != problemData->mapAluno_CampusTurmaDisc.end() )
					{
						GGroup< Trio< int /*campusId*/, int /*turma*/, Disciplina* > > ggroupAlocacoes = itMap->second;
						GGroup< Trio< int /*campusId*/, int /*turma*/, Disciplina* > >::iterator it = ggroupAlocacoes.begin();
						for( ; it!=ggroupAlocacoes.end(); it++ )
						{
							Trio< int /*campusId*/, int /*turma*/, Disciplina* > trio = *it;
							int campusId = trio.first;
							int turma = trio.second;
							Disciplina *disciplina = trio.third;
							
							GGroup< VariableTatico *, LessPtr<VariableTatico> > ggroupX = this->retornaAulasEmVarX( turma, disciplina, campusId );
							ITERA_GGROUP_LESSPTR( itX, ggroupX, VariableTatico )
							{
								if ( itX->getDia() == dia )
								{		
									DateTime vInicio = itX->getHorarioAulaInicial()->getInicio();
									HorarioAula *horarioAulaFim = itX->getHorarioAulaFinal();
									DateTime vFim = horarioAulaFim->getFinal();

									if ( ( vInicio <= inicio ) && ( vFim > inicio ) )
									{
										// insere -1 no lado direito da restrição
										add_rhs += -1.0;
									}
									break;
								}
							}
						}
					}
				}

				if ( add_rhs < -1.01 ) 
					std::cout << "\nErro em TaticoIntAlunoHor::criaRestricaoTaticoAlunoHorario()."
					<< "\nAluno "<< aluno->getAlunoId() << ", Dia "<<dia<<", Horario "<< horario_aula->getId() <<", add_rhs = "<<add_rhs<<endl;

				sprintf( name, "%s", c.toString( etapa ).c_str() );
				nnz = 100;

				double rhs = 1.0;
				
				rhs += add_rhs;

				OPT_ROW row( nnz, OPT_ROW::LESS , rhs, name );

				row.insert( vit->second, 1.0 );
				
				cHashTatico[ c ] = lp->getNumRows();

				lp->addRow( row );
				restricoes++;
			}   
       }
   }

   chgCoeffList( coeffList, coeffListVal );

   return restricoes;
}

// fu_{i1,d1,i2,d2,t,cp}
// Talvez tenha que reformular
/*
int TaticoIntAlunoHor::criaRestricaoTaticoAlunoUnidDifDia( int campusId )
{
   int restricoes = 0;

   if ( ! problemData->parametros->minDeslocAlunoEntreUnidadesNoDia )
		return restricoes;

   int nnz;
   char name[ 100 ];

   VariableTatInt v;
   ConstraintTatInt c;
   VariableTatIntHash::iterator it_v;

   Disciplina * disciplina = NULL;

   ITERA_GGROUP_LESSPTR( it_campus, problemData->campi, Campus )
   {
	   if ( it_campus->getId() != campusId )
	   {
		   continue;
	   }

	   Campus* campus = *it_campus;
	     
	  GGroup< int > disciplinas = problemData->cp_discs[ campusId ];

	  // Disciplina 1
      ITERA_GGROUP_N_PT( it_disc1, disciplinas, int )
      {
		  Disciplina * disciplina1 = problemData->refDisciplinas[ *it_disc1 ];
		  
		 #pragma region Equivalencias
		 if ( ( problemData->mapDiscSubstituidaPor.find( disciplina1 ) !=
				problemData->mapDiscSubstituidaPor.end() ) &&
				!problemData->ehSubstituta( disciplina1 ) )
		 {
			continue;
		 }
		 #pragma endregion
		 		 
		 if ( ! problemData->haDemandaDiscNoCampus( disciplina1->getId(), campusId ) )
			  continue;
		
		 // Turma 1
         for ( int turma1 = 0; turma1< disciplina1->getNumTurmas(); turma1++ )
         {
			  Unidade *u1 = this->retornaUnidadeDeAtendimento( turma1, disciplina1, campus );
					   
			  if ( u1 == NULL )
			  {
				  continue;
			  }
			  
			  // Disciplina 2
			  ITERA_GGROUP_INIC_N_PT( it_disc2, it_disc1, disciplinas, int )
			  {
				  Disciplina * disciplina2 = problemData->refDisciplinas[ *it_disc2 ];
		  
				  #pragma region Equivalencias
				  if ( ( problemData->mapDiscSubstituidaPor.find( disciplina2 ) !=
					     problemData->mapDiscSubstituidaPor.end() ) &&
						!problemData->ehSubstituta( disciplina2 ) )
				  {
					  continue;
				  }
				  #pragma endregion
		 		 
				  if ( ! problemData->haDemandaDiscNoCampus( disciplina2->getId(), campusId ) )
					  continue;

				  // Turma 2
				  for ( int turma2 = 0; turma2 < disciplina2->getNumTurmas(); turma2++ )
				  {
					   Unidade *u2 = this->retornaUnidadeDeAtendimento( turma2, disciplina2, campus );

					   if ( u2 == NULL || u2 == u1 )
					   {
						   continue;
					   }

					    GGroup<Aluno*, LessPtr<Aluno>> alunosEmComum = 
							problemData->alunosEmComum( turma1, disciplina1, turma2, disciplina2, campus );

						if ( alunosEmComum.size() == 0 )
						{
							continue;
						}

						int nroAlunos = alunosEmComum.size();

						GGroup<int> dias = problemData->diasComunsEntreDisciplinas( disciplina1, disciplina2 );
						ITERA_GGROUP_N_PT( it_dias, dias, int )
						{
							int dia = *it_dias;

							// CONSTRAINT --------------------------------------

							c.reset();
							c.setType( ConstraintTatInt::C_ALUNO_VARIAS_UNIDADES_DIA );

							c.setCampus( campus );
							c.setTurma1( turma1 );
							c.setTurma2( turma2 );
							c.setDisciplina1( disciplina1 );
							c.setDisciplina2( disciplina2 );
							c.setDia( dia );
							c.setCampus( campus );

							sprintf( name, "%s", c.toString( etapa ).c_str() );

							if ( cHashTatico.find( c ) != cHashTatico.end() )
							{
								continue;
							}

							nnz = 100;

							OPT_ROW row( nnz + 1, OPT_ROW::LESS , 1 , name );

							// Insere variavel fu_{i1,d1,i2,d2,t,cp} -------------------------------------------------------
							VariableTatInt v;
							v.reset();
							v.setType( VariableTatInt::V_SLACK_ALUNO_VARIAS_UNID_DIA );
							v.setTurma1( turma1 );            // i1
							v.setDisciplina1( disciplina1 );  // d1
							v.setTurma2( turma2 );            // i2
							v.setDisciplina2( disciplina2 );  // d2
							v.setCampus( campus );			  // cp
							v.setDia( dia );				  // t

							it_v = vHashTatico.find( v );
							if( it_v != vHashTatico.end() )
							{
								row.insert( it_v->second, -1.0 );
							}

							// Insere variaveis x_{i,d,u,s,hi,hf,t} -------------------------------------------------------
							it_v = vHashTatico.begin();

							for (; it_v != vHashTatico.end(); it_v++ )
							{
								VariableTatInt v = it_v->first;

								if ( v.getType() != VariableTatInt::V_CREDITOS )
								{
									continue;
								}

								// Insere variavel x_{i1,d1,u1,s1,hi,hf,t} ou x_{i2,d2,u2,s2,hi,hf,t}
								
								if ( ( v.getTurma() == turma1 &&
									   v.getDisciplina() == disciplina1 && 
									   v.getDia() == dia && 
									   v.getUnidade() == u1 ) 
									   ||
									 ( v.getTurma() == turma2 &&
									   v.getDisciplina() == disciplina2 && 
									   v.getDia() == dia && 
									   v.getUnidade() == u2 ) )
								{
									row.insert( it_v->second, 1.0 );
								}
							}

							// Insere restrição no Hash --------------------------------------------------------
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
	
	return restricoes;

}
*/

int TaticoIntAlunoHor::criaRestricaoTaticoMinDiasAluno( int campusId )
{
   int restricoes = 0;

   if ( problemData->parametros->carga_horaria_semanal_aluno != ParametrosPlanejamento::MINIMIZAR_DIAS )
   {
		return restricoes;
   }

   int nnz;
   char name[ 1024 ];

   ConstraintTatInt c;
   VariableTatIntHash::iterator vit;
   ConstraintTatIntHash::iterator cit;

   std::vector< std::pair< int, int > > coeffList;
   std::vector< double > coeffListVal;
   std::pair< int, int > auxCoef;

   vit = vHashTatico.begin();

   for (; vit != vHashTatico.end(); vit++ )
   {
		VariableTatInt v = vit->first;

		if ( v.getType() != VariableTatInt::V_ALUNO_CREDITOS && 
			 v.getType() != VariableTatInt::V_ALUNO_DIA )
		{
			continue;
		}
		
		Aluno *aluno = v.getAluno();					
		int dia = v.getDia();

		double coef=0.0;
		if ( v.getType() == VariableTatInt::V_ALUNO_CREDITOS )
			coef = -1.0;
		else if ( v.getType() == VariableTatInt::V_ALUNO_DIA )
			coef = 10.0;

		c.reset();
		c.setType( ConstraintTatInt::C_MIN_DIAS_ALUNO );
		c.setAluno( aluno );
		c.setDia( dia );

		cit = cHashTatico.find( c );
		if ( cit != cHashTatico.end() )
		{
			auxCoef.first = cit->second;
			auxCoef.second = vit->second;

			coeffList.push_back( auxCoef );
			coeffListVal.push_back( coef );
		}
		else
		{
			sprintf( name, "%s", c.toString( etapa ).c_str() );
			nnz = 100;

			OPT_ROW row( nnz, OPT_ROW::GREATER, 0.0, name );

			row.insert( vit->second, coef );
			cHashTatico[ c ] = lp->getNumRows();

			lp->addRow( row );
			restricoes++;
		}
   }

   chgCoeffList( coeffList, coeffListVal );

   return restricoes;
}

int TaticoIntAlunoHor::criaRestricaoTaticoMaxDiasAluno( int campusId )
{
   int restricoes = 0;

   if ( problemData->parametros->carga_horaria_semanal_aluno != ParametrosPlanejamento::EQUILIBRAR )
   {
		return restricoes;
   }

   int nnz;
   char name[ 1024 ];

   ConstraintTatInt c;
   VariableTatIntHash::iterator vit;
   ConstraintTatIntHash::iterator cit;

   std::vector< std::pair< int, int > > coeffList;
   std::vector< double > coeffListVal;
   std::pair< int, int > auxCoef;

   vit = vHashTatico.begin();

   for (; vit != vHashTatico.end(); vit++ )
   {
		VariableTatInt v = vit->first;

		if ( v.getType() != VariableTatInt::V_ALUNO_CREDITOS && 
			 v.getType() != VariableTatInt::V_ALUNO_DIA )
		{
			continue;
		}
		
		Aluno *aluno = v.getAluno();					
		int dia = v.getDia();

		double coef=0.0;
		if ( v.getType() == VariableTatInt::V_ALUNO_CREDITOS )
			coef = -1.0;
		else if ( v.getType() == VariableTatInt::V_ALUNO_DIA )
			coef = 1.0;

		c.reset();
		c.setType( ConstraintTatInt::C_MAX_DIAS_ALUNO );
		c.setAluno( aluno );
		c.setDia( dia );

		cit = cHashTatico.find( c );
		if ( cit != cHashTatico.end() )
		{
			auxCoef.first = cit->second;
			auxCoef.second = vit->second;

			coeffList.push_back( auxCoef );
			coeffListVal.push_back( coef );
		}
		else
		{
			sprintf( name, "%s", c.toString( etapa ).c_str() );
			nnz = 100;

			OPT_ROW row( nnz, OPT_ROW::LESS, 0.0, name );

			row.insert( vit->second, coef );
			cHashTatico[ c ] = lp->getNumRows();

			lp->addRow( row );
			restricoes++;
		}
   }

   chgCoeffList( coeffList, coeffListVal );

   return restricoes;
}

/*
	Para cada aluno a, disciplina d:

	sum[i] s_{i,d,a} + fd_{d,a} = 1
*/
int TaticoIntAlunoHor::criaRestricaoTaticoAtendeAluno( int campusId, int prioridade )
{
   int restricoes = 0;
   
   if ( problemData->parametros->considerar_equivalencia_por_aluno && USAR_EQUIVALENCIA )
		return restricoes;

   int nnz;
   char name[ 1024 ];

   ConstraintTatInt c;
   VariableTatIntHash::iterator vit;
   ConstraintTatIntHash::iterator cit;

   std::vector< std::pair< int, int > > coeffList;
   std::vector< double > coeffListVal;
   std::pair< int, int > auxCoef;

   vit = vHashTatico.begin();

   for (; vit != vHashTatico.end(); vit++ )
   {
		VariableTatInt v = vit->first;

		if ( v.getType() != VariableTatInt::V_ALOCA_ALUNO_TURMA_DISC &&
			 v.getType() != VariableTatInt::V_SLACK_DEMANDA_ALUNO )
		{
			continue;
		}
		
		Aluno *aluno = v.getAluno();
		Disciplina* disc = v.getDisciplina();
			
		c.reset();
		c.setType( ConstraintTatInt::C_DEMANDA_DISC_ALUNO );
		c.setAluno( aluno );
		c.setDisciplina( disc );

		cit = cHashTatico.find( c );

		if ( cit != cHashTatico.end() )
		{
			auxCoef.first = cit->second;
			auxCoef.second = vit->second;

			coeffList.push_back( auxCoef );
			coeffListVal.push_back( 1.0 );
		}
		else
		{			
			sprintf( name, "%s", c.toString( etapa ).c_str() );
			nnz = 100; // TODO

			OPT_ROW row( nnz, OPT_ROW::EQUAL , 1.0, name );
						
			row.insert( vit->second, 1.0 );

			// Insere restrição
			cHashTatico[ c ] = lp->getNumRows();
			lp->addRow( row );
			restricoes++;
		}
   }

   chgCoeffList( coeffList, coeffListVal );

   return restricoes;
}

/*
	Para cada turma i, disciplina d:

	sum[s] o_{i,d,s} <= z_{i,d,cp}
*/
int TaticoIntAlunoHor::criaRestricaoTaticoSalaUnica( int campusId )
{
   int restricoes = 0;
   int nnz;
   char name[ 1024 ];

   ConstraintTatInt c;
   VariableTatIntHash::iterator vit;
   ConstraintTatIntHash::iterator cit;

   std::vector< std::pair< int, int > > coeffList;
   std::vector< double > coeffListVal;
   std::pair< int, int > auxCoef;

   vit = vHashTatico.begin();

   for (; vit != vHashTatico.end(); vit++ )
   {
		VariableTatInt v = vit->first;
		
		double coef=0.0;

		if ( v.getType() == VariableTatInt::V_OFERECIMENTO )
		{
			coef = 1.0;
		}
		else if ( v.getType() == VariableTatInt::V_ABERTURA )
		{
			coef = -1.0;
		}
		else continue;

		int turma = v.getTurma();
		Disciplina* disc = v.getDisciplina();
			
		c.reset();
		c.setType( ConstraintTatInt::C_SALA_UNICA );
		c.setTurma( turma );
		c.setDisciplina( disc );

		cit = cHashTatico.find( c );

		if ( cit != cHashTatico.end() )
		{
			auxCoef.first = cit->second;
			auxCoef.second = vit->second;

			coeffList.push_back( auxCoef );
			coeffListVal.push_back( coef );
		}
		else
		{			
			sprintf( name, "%s", c.toString( etapa ).c_str() );
			nnz = 100;

			OPT_ROW row( nnz, OPT_ROW::LESS, 0.0, name );
						
			row.insert( vit->second, coef );

			// Insere restrição
			cHashTatico[ c ] = lp->getNumRows();
			lp->addRow( row );
			restricoes++;
		}
   }

   chgCoeffList( coeffList, coeffListVal );

   return restricoes;
}


/*
	Para cada turma i, disciplina d, sala s:

	M * o_{i,d,s} >= sum[t]sum[hi]sum[hf] x_{i,d,u,s,t,hi,hf}

*/
int TaticoIntAlunoHor::criaRestricaoTaticoSalaPorTurma( int campusId )
{
   int restricoes = 0;
   int nnz;
   char name[ 1024 ];

   ConstraintTatInt c;
   VariableTatIntHash::iterator vit;
   ConstraintTatIntHash::iterator cit;

   std::vector< std::pair< int, int > > coeffList;
   std::vector< double > coeffListVal;
   std::pair< int, int > auxCoef;

   vit = vHashTatico.begin();

   for (; vit != vHashTatico.end(); vit++ )
   {
		VariableTatInt v = vit->first;

		if ( v.getType() != VariableTatInt::V_OFERECIMENTO && 
			 v.getType() != VariableTatInt::V_CREDITOS )
		{
			continue;
		}
		
		int turma = v.getTurma();
		Disciplina* disc = v.getDisciplina();
		ConjuntoSala *cjtSala = v.getSubCjtSala();
			
		double coef = 0.0;
		if ( v.getType() == VariableTatInt::V_OFERECIMENTO )
			coef = disc->getTotalCreditos();
		else if ( v.getType() == VariableTatInt::V_CREDITOS )
			coef = -1.0;

		c.reset();
		c.setType( ConstraintTatInt::C_SALA_TURMA );
		c.setTurma( turma );
		c.setDisciplina( disc );
		c.setSubCjtSala( cjtSala );

		cit = cHashTatico.find( c );
		if ( cit != cHashTatico.end() )
		{
			auxCoef.first = cit->second;
			auxCoef.second = vit->second;

			coeffList.push_back( auxCoef );
			coeffListVal.push_back( coef );
		}
		else
		{			
			sprintf( name, "%s", c.toString( etapa ).c_str() );
			nnz = 100;

			OPT_ROW row( nnz, OPT_ROW::GREATER, 0.0, name );
						
			row.insert( vit->second, coef );

			// Insere restrição
			cHashTatico[ c ] = lp->getNumRows();
			lp->addRow( row );
			restricoes++;
		}
   }

   chgCoeffList( coeffList, coeffListVal );

   return restricoes;
}


/*
	Para cada aluno a, disciplina d, turma i:

	C_{d} * s_{i,d,a} = sum[u]sum[s]sum[t]sum[hi]sum[hf] NHC_{d,hi,hf} * v_{a,i,d,u,s,t,hi,hf}
*/
int TaticoIntAlunoHor::criaRestricaoTaticoAssociaAlunoEGaranteNroCreds( int campusId )
{
   int restricoes = 0;
   int nnz;
   char name[ 1024 ];

   ConstraintTatInt c;
   VariableTatIntHash::iterator vit;
   ConstraintTatIntHash::iterator cit;

   std::vector< std::pair< int, int > > coeffList;
   std::vector< double > coeffListVal;
   std::pair< int, int > auxCoef;

   vit = vHashTatico.begin();

   for (; vit != vHashTatico.end(); vit++ )
   {
		VariableTatInt v = vit->first;

		if ( v.getType() != VariableTatInt::V_ALOCA_ALUNO_TURMA_DISC &&
			 v.getType() != VariableTatInt::V_ALUNO_CREDITOS )
		{
			continue;
		}
		
		Aluno *aluno = v.getAluno();
		Disciplina* disc = v.getDisciplina();
		int turma = v.getTurma();	
					
		double coef = 0.0;
		if ( v.getType() == VariableTatInt::V_ALOCA_ALUNO_TURMA_DISC ) // s
			coef = disc->getTotalCreditos();
		else if ( v.getType() == VariableTatInt::V_ALUNO_CREDITOS ) // v
		{
			HorarioAula *hi = v.getHorarioAulaInicial();
			HorarioAula *hf = v.getHorarioAulaFinal();
			Calendario *calendario = hi->getCalendario();
			coef = - calendario->retornaNroCreditosEntreHorarios(hi, hf);
		}
		
		c.reset();
		c.setType( ConstraintTatInt::C_ASSOCIA_S_V );
		c.setAluno( aluno );
		c.setDisciplina( disc );
		c.setTurma( turma );

		cit = cHashTatico.find( c );

		if ( cit != cHashTatico.end() )
		{
			auxCoef.first = cit->second;
			auxCoef.second = vit->second;

			coeffList.push_back( auxCoef );
			coeffListVal.push_back( coef );
		}
		else
		{			
			sprintf( name, "%s", c.toString( etapa ).c_str() );
			nnz = 100; // TODO

			OPT_ROW row( nnz, OPT_ROW::EQUAL, 0.0, name );
						
			row.insert( vit->second, coef );

			// Insere restrição
			cHashTatico[ c ] = lp->getNumRows();
			lp->addRow( row );
			restricoes++;
		}
   }

   chgCoeffList( coeffList, coeffListVal );

   return restricoes;
}


int TaticoIntAlunoHor::criaRestricaoTaticoAbreTurmasEmSequencia( int campusId )
{
   int restricoes = 0;
   char name[ 1024 ];
   int nnz;

   VariableTatInt v;
   ConstraintTatInt c;
   VariableTatIntHash::iterator it_v;

   Disciplina * disciplina = NULL;

   Campus *campus = problemData->refCampus[ campusId ];
   
   ITERA_GGROUP_LESSPTR( it_disciplina, problemData->disciplinas, Disciplina )
   {
      disciplina = ( *it_disciplina );

	  if ( problemData->cp_discs[ campusId ].find( disciplina->getId() ) ==
		   problemData->cp_discs[ campusId ].end() )
	  {
		  continue;
 	  }

	  #pragma region Equivalencias
	  if ( ( problemData->mapDiscSubstituidaPor.find( disciplina ) !=
			 problemData->mapDiscSubstituidaPor.end() ) &&
			!problemData->ehSubstituta( disciplina ) )
	  {
		  continue;
	  }
	  #pragma endregion
	  
	  if ( !this->USAR_EQUIVALENCIA && !problemData->haDemandaDiscNoCampus( disciplina->getId(), campusId ) )
	  {
		  continue;
	  }

      if ( disciplina->getNumTurmas() > 1 )
      {
         for ( int turma = 1; turma <= ( disciplina->getNumTurmas() - 1 ); turma++ )
         {
			 if ( !PERMITIR_NOVAS_TURMAS )
			 if ( !problemData->existeTurmaDiscCampus( turma, disciplina->getId(), campusId ) ||
			 	  !problemData->existeTurmaDiscCampus( turma+1, disciplina->getId(), campusId ) )
			 {
				continue;
			 }

			 //if ( ! this->CRIAR_VARS_FIXADAS )
			 //{
				// if ( !problemData->existeTurmaDiscCampus( turma, disciplina->getId(), campusId ) ||
				//	  !problemData->existeTurmaDiscCampus( turma+1, disciplina->getId(), campusId ) )
				// {
				//	continue;
				// }
			 //}

            c.reset();
            c.setType( ConstraintTatInt::C_ABRE_TURMAS_EM_SEQUENCIA );
            c.setDisciplina( disciplina );
            c.setTurma( turma );

            sprintf( name, "%s", c.toString( etapa ).c_str() ); 
            if ( cHashTatico.find( c ) != cHashTatico.end() )
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
                        v.setType( VariableTatInt::V_OFERECIMENTO );
                        v.setTurma( turma );
                        v.setDisciplina( disciplina );
                        v.setUnidade( *itUnidade );
                        v.setSubCjtSala( *itCjtSala );

                        it_v = vHashTatico.find( v );
                        if ( it_v != vHashTatico.end() )
                        {
                           row.insert( it_v->second, 1.0 );
                        }

                        v.reset();
                        v.setType( VariableTatInt::V_OFERECIMENTO );                        
                        v.setTurma( turma + 1 );
                        v.setDisciplina( disciplina );
                        v.setUnidade( *itUnidade );
                        v.setSubCjtSala( *itCjtSala );

                        it_v = vHashTatico.find( v );
                        if ( it_v != vHashTatico.end() )
                        {
                           row.insert( it_v->second, -1.0 );
                        }
                  }
               }
            }

            v.reset();
            v.setType( VariableTatInt::V_SLACK_ABERT_SEQ_TURMA );
            v.setTurma( turma );
            v.setDisciplina( disciplina );
			v.setCampus( campus );

            it_v = vHashTatico.find( v );
            if ( it_v != vHashTatico.end() )
            {
                row.insert( it_v->second, 1.0 );
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

/*
	Para cada turma i, disciplina d, curso c:

	sum[a] s_{i,d,a} <= M * b_{i,d,c}
*/
int TaticoIntAlunoHor::criaRestricaoTaticoAlunoCurso( int campusId )
{
   int restricoes = 0;
   int nnz;
   char name[ 1024 ];

   ConstraintTatInt c;
   VariableTatIntHash::iterator vit;
   ConstraintTatIntHash::iterator cit;

   std::vector< std::pair< int, int > > coeffList;
   std::vector< double > coeffListVal;
   std::pair< int, int > auxCoef;

   vit = vHashTatico.begin();

   for (; vit != vHashTatico.end(); vit++ )
   {
		VariableTatInt v = vit->first;

		if ( v.getType() != VariableTatInt::V_TURMA_ATEND_CURSO &&
			 v.getType() != VariableTatInt::V_ALOCA_ALUNO_TURMA_DISC )
		{
			continue;
		}
		
		//std::cout << "\nVar = " << v.toString();

		int turma = v.getTurma();
		Disciplina* disc = v.getDisciplina();
		Curso *curso = NULL;

		double coef = 0.0;
		if ( v.getType() == VariableTatInt::V_TURMA_ATEND_CURSO ) // b
		{
			curso = v.getCurso();
			if ( USAR_EQUIVALENCIA ) 
				coef = - 500; //this->getNroMaxAlunoDemanda( disc->getId() );
				//coef = - problemData->maxDemandaDiscNoCursoEquiv( disc, curso->getId() );
			else 
				coef = - problemData->haDemandaDiscNoCurso( disc->getId(), curso->getId() );
		}
		else if ( v.getType() == VariableTatInt::V_ALOCA_ALUNO_TURMA_DISC ) // s
		{
			curso = v.getAlunoDemanda()->getCurso();
			coef = 1.0;
		}

		c.reset();
		c.setType( ConstraintTatInt::C_ALUNO_CURSO );
		c.setTurma( turma );
		c.setDisciplina( disc );
		c.setCurso( curso );

		cit = cHashTatico.find( c );

		if ( cit != cHashTatico.end() )
		{
			auxCoef.first = cit->second;
			auxCoef.second = vit->second;

			coeffList.push_back( auxCoef );
			coeffListVal.push_back( coef );
		}
		else
		{			
			sprintf( name, "%s", c.toString( etapa ).c_str() );
			nnz = 100;

			OPT_ROW row( nnz, OPT_ROW::LESS, 0.0, name );
						
			row.insert( vit->second, coef );

			// Insere restrição
			cHashTatico[ c ] = lp->getNumRows();
			lp->addRow( row );
			restricoes++;
		}
   }

   chgCoeffList( coeffList, coeffListVal );

   return restricoes;
}

// Restricao 1.21
int TaticoIntAlunoHor::criaRestricaoPrioridadesDemanda( int campusId, int prior )
{
    int restricoes = 0;
	
	if ( problemData->parametros->considerar_equivalencia_por_aluno && this->USAR_EQUIVALENCIA )
		return restricoes;

    if ( prior < 2 )
	   return restricoes;
	
	std::cout<<"\ncriaRestricaoPrioridadesDemanda...";

    char name[ 1024 ];
    int nnz;

    ConstraintTatInt c;
    VariableTatInt v;
    VariableTatIntHash::iterator it_v;   

    Campus *campus = problemData->refCampus[campusId];

	ITERA_GGROUP_LESSPTR( itAluno, problemData->alunos, Aluno )
	{
		Aluno *aluno = *itAluno;

		c.reset();
		c.setType( ConstraintTatInt::C_ALUNO_PRIORIDADES_DEMANDA );
		c.setAluno( aluno );
		c.setCampus( campus );

		sprintf( name, "%s", c.toString( etapa ).c_str() ); 

		if ( cHashTatico.find( c ) != cHashTatico.end() )
		{
			continue;
		}

		nnz = aluno->demandas.size()*3 + 2;
		
		double cargaHorariaNaoAtendida = problemData->cargaHorariaNaoAtendidaPorPrioridade( 1, aluno->getAlunoId() );		
		double cargaHorariaP2 = problemData->cargaHorariaAtualRequeridaPorPrioridade( prior, aluno );

		double rhs = cargaHorariaNaoAtendida;
		if (rhs>cargaHorariaP2)
			rhs=cargaHorariaP2;

		OPT_ROW row( nnz, OPT_ROW::EQUAL , rhs, name );
		
		ITERA_GGROUP_LESSPTR( itAlDemanda, aluno->demandas, AlunoDemanda )
		{
			Disciplina *disciplina = itAlDemanda->demanda->disciplina;

			if ( itAlDemanda->getPrioridade() != prior )
				continue;

			for ( int turma = 1; turma <= disciplina->getNumTurmas(); turma++ )
			{
				VariableTatInt v;
				v.reset();
				v.setType( VariableTatInt::V_ALOCA_ALUNO_TURMA_DISC );
				v.setAluno( aluno );
				v.setDisciplina( disciplina );
				v.setTurma( turma );
				v.setCampus( campus );
				v.setAlunoDemanda( *itAlDemanda );

				it_v = vHashTatico.find( v );
				if( it_v != vHashTatico.end() )
				{
					double tempo = disciplina->getTotalCreditos() * disciplina->getTempoCredSemanaLetiva();

					row.insert( it_v->second, tempo );
				}
			}			
		}

		VariableTatInt v;
		v.reset();
		v.setType( VariableTatInt::V_SLACK_PRIOR_INF );
		v.setAluno( aluno );
		v.setCampus( campus );
		it_v = vHashTatico.find( v );
		if( it_v != vHashTatico.end() )
		{
			row.insert( it_v->second, -1.0 );
		}

		v.reset();
		v.setType( VariableTatInt::V_SLACK_PRIOR_SUP );
		v.setAluno( aluno );
		v.setCampus( campus );
		it_v = vHashTatico.find( v );
		if( it_v != vHashTatico.end() )
		{
			row.insert( it_v->second, 1.0 );
		}


		if ( row.getnnz() >= 2 )
		{
			cHashTatico[ c ] = lp->getNumRows();
			lp->addRow( row );
			restricoes++;
		}
	}

	return restricoes;

}

// Restricao 1.21
int TaticoIntAlunoHor::criaRestricaoPrioridadesDemanda_v2( int campusId, int prior )
{
    int restricoes = 0;
	
//	if ( problemData->parametros->considerar_equivalencia_por_aluno && this->USAR_EQUIVALENCIA )
//		return restricoes;
    if ( prior < 2 )
	   return restricoes;	
	
	std::cout<<"\ncriaRestricaoPrioridadesDemanda_v2...";

    Campus *campus = problemData->refCampus[campusId];

    char name[ 1024 ];
    int nnz;
    
    VariableTatIntHash::iterator vit;
    
    std::vector< std::pair< int, int > > coeffList;
    std::vector< double > coeffListVal;
    std::pair< int, int > auxCoef;

    vit = vHashTatico.begin();
    for (; vit != vHashTatico.end(); vit++ )
    {
		VariableTatInt v = vit->first;
		
		double coef=0.0;

		if ( v.getType() == VariableTatInt::V_ALOCA_ALUNO_TURMA_DISC )
		{
			if ( v.getAlunoDemanda()->getPrioridade() != prior )
				continue;

			double tempo = v.getDisciplina()->getTotalCreditos() * v.getDisciplina()->getTempoCredSemanaLetiva();
			coef = tempo;
		}
		else if ( v.getType() == VariableTatInt::V_SLACK_PRIOR_INF )
		{
			coef = -1.0;
		}
		else if ( v.getType() == VariableTatInt::V_SLACK_PRIOR_SUP )
		{
			coef = 1.0;
		}
		else continue;

		Aluno *aluno = v.getAluno();

		//if ( v.getType() == VariableTatInt::V_SLACK_PRIOR_SUP )
		//	std::cout<<"\nV_SLACK_PRIOR_SUP  v="<<v.toString();
		//if ( v.getType() == VariableTatInt::V_SLACK_PRIOR_INF )
		//	std::cout<<"\nV_SLACK_PRIOR_INF  v="<<v.toString();
		//if ( v.getType() == VariableTatInt::V_ALOCA_ALUNO_TURMA_DISC )
		//	std::cout<<"\nV_ALOCA_ALUNO_TURMA_DISC  v="<<v.toString();

		ConstraintTatInt c;
		c.reset();
		c.setType( ConstraintTatInt::C_ALUNO_PRIORIDADES_DEMANDA );
		c.setAluno( aluno );
		c.setCampus( campus );

		sprintf( name, "%s", c.toString( etapa ).c_str() ); 

		nnz = aluno->demandas.size()*3 + 2;
						
		ConstraintTatIntHash::iterator cit = cHashTatico.find( c );
		if ( cit != cHashTatico.end() )
		{
			auxCoef.first = cit->second;
			auxCoef.second = vit->second;

			coeffList.push_back( auxCoef );
			coeffListVal.push_back( coef );
		}
		else
		{			
			double cargaHorariaNaoAtendida = problemData->cargaHorariaNaoAtendidaPorPrioridade( 1, aluno->getAlunoId() );		
			double cargaHorariaP2 = problemData->cargaHorariaAtualRequeridaPorPrioridade( prior, aluno );
			double rhs = cargaHorariaNaoAtendida;
			if (rhs>cargaHorariaP2)
				rhs=cargaHorariaP2;

			sprintf( name, "%s", c.toString( etapa ).c_str() );
			nnz = 100;

			OPT_ROW row( nnz, OPT_ROW::EQUAL, rhs, name );
						
			row.insert( vit->second, coef );

			// Insere restrição
			cHashTatico[ c ] = lp->getNumRows();
			lp->addRow( row );
			restricoes++;
		}
   }

   chgCoeffList( coeffList, coeffListVal );

   return restricoes;
}


// Restricao 1.21
int TaticoIntAlunoHor::criaRestricaoPrioridadesDemandaEquiv( int campusId, int prior )
{
    int restricoes = 0;
	
	if ( !problemData->parametros->considerar_equivalencia_por_aluno || !this->USAR_EQUIVALENCIA )
		return restricoes;

    if ( prior < 2 )
	   return restricoes;

	std::cout<<"\ncriaRestricaoPrioridadesDemandaEquiv...";

    char name[ 1024 ];
    int nnz;

    ConstraintTatInt c;
    VariableTatInt v;
    VariableTatIntHash::iterator it_v;   

    Campus *campus = problemData->refCampus[campusId];

	ITERA_GGROUP_LESSPTR( itAluno, problemData->alunos, Aluno )
	{
		Aluno *aluno = *itAluno;

		c.reset();
		c.setType( ConstraintTatInt::C_ALUNO_PRIORIDADES_DEMANDA );
		c.setAluno( aluno );
		c.setCampus( campus );

		sprintf( name, "%s", c.toString( etapa ).c_str() ); 

		if ( cHashTatico.find( c ) != cHashTatico.end() )
		{
			continue;
		}

		nnz = aluno->demandas.size()*3 + 2;
		
		double cargaHorariaNaoAtendida = problemData->cargaHorariaNaoAtendidaPorPrioridade( prior-1, aluno->getAlunoId() );		
		double cargaHorariaP2 = problemData->cargaHorariaAtualRequeridaPorPrioridade( prior, aluno );

		double rhs = cargaHorariaNaoAtendida;
		if (rhs>cargaHorariaP2)
			rhs=cargaHorariaP2;

		OPT_ROW row( nnz, OPT_ROW::EQUAL , rhs, name );
		
		ITERA_GGROUP_LESSPTR( itAlDemanda, aluno->demandas, AlunoDemanda )
		{
			Disciplina *disciplina = itAlDemanda->demanda->disciplina;

			if ( itAlDemanda->getPrioridade() != prior )
				continue;

			int turmaAluno = problemData->retornaTurmaDiscAluno( aluno, disciplina );				
			if ( turmaAluno == -1 ) // aluno não alocado
			{
				ITERA_GGROUP_LESSPTR( itDiscEq, disciplina->discEquivSubstitutas, Disciplina )
				{
					Disciplina *disciplinaEquiv = (*itDiscEq);
					
					if ( ! problemData->alocacaoEquivViavel( (*itAlDemanda)->demanda, disciplinaEquiv ) )
						continue;	

					int turmaAluno = problemData->retornaTurmaDiscAluno( aluno, disciplinaEquiv );				
					if ( turmaAluno != -1 ) // dentre as equivalentes, evita aqui considerar as duplicatas de p1
						continue;

					for ( int turma = 1; turma <= disciplinaEquiv->getNumTurmas(); turma++ )
					{
						VariableTatInt v;
						v.reset();
						v.setType( VariableTatInt::V_ALOCA_ALUNO_TURMA_DISC );
						v.setAluno( aluno );
						v.setDisciplina( disciplinaEquiv );
						v.setTurma( turma );
						v.setCampus( campus );
						v.setAlunoDemanda( *itAlDemanda );

						it_v = vHashTatico.find( v );
						if( it_v != vHashTatico.end() )
						{
							double tempo = disciplinaEquiv->getTotalCreditos() * disciplinaEquiv->getTempoCredSemanaLetiva();

							row.insert( it_v->second, tempo );
						}
					}
				}				
			}
			else
			{
				for ( int turma = 1; turma <= disciplina->getNumTurmas(); turma++ )
				{
					VariableTatInt v;
					v.reset();
					v.setType( VariableTatInt::V_ALOCA_ALUNO_TURMA_DISC );
					v.setAluno( aluno );
					v.setDisciplina( disciplina );
					v.setTurma( turma );
					v.setCampus( campus );
					v.setAlunoDemanda( *itAlDemanda );

					it_v = vHashTatico.find( v );
					if( it_v != vHashTatico.end() )
					{
						double tempo = disciplina->getTotalCreditos() * disciplina->getTempoCredSemanaLetiva();

						row.insert( it_v->second, tempo );
					}
				}
			}
		}

		VariableTatInt v;
		v.reset();
		v.setType( VariableTatInt::V_SLACK_PRIOR_INF );
		v.setAluno( aluno );
		v.setCampus( campus );
		it_v = vHashTatico.find( v );
		if( it_v != vHashTatico.end() )
		{
			row.insert( it_v->second, -1.0 );
		}

		v.reset();
		v.setType( VariableTatInt::V_SLACK_PRIOR_SUP );
		v.setAluno( aluno );
		v.setCampus( campus );
		it_v = vHashTatico.find( v );
		if( it_v != vHashTatico.end() )
		{
			row.insert( it_v->second, 1.0 );
		}


		if ( row.getnnz() >= 2 )
		{
			cHashTatico[ c ] = lp->getNumRows();
			lp->addRow( row );
			restricoes++;
		}
	}

	return restricoes;

}


/*
	Seta a variavel f_{i,d,cp}, que indica se uma turma possui aluno formando. Só está sendo necessária em rodada 2
	(só a partir de r=2 pode ser permitida a violação do min de alunos por turma caso haja formando).

	Restrição 1: 
	
		M * f_{i,d,cp} >= sum[a] s_{i,d,a}	sendo 'a' formando		Para cada turma i, disc d, campus cp

	Restrição 2: 
	
		f_{i,d,cp} <= sum[a] s_{i,d,a}  sendo 'a' formando		Para cada turma i, disc d, campus cp
*/
int TaticoIntAlunoHor::criaRestricaoTaticoFormandos( int campusId, int prioridade, int r )
{
    int restricoes = 0;

	if ( !problemData->parametros->violar_min_alunos_turmas_formandos )
		return restricoes;		

	if ( prioridade==1 && r==1 ) // só considera formandos na segunda rodada
		return restricoes;
		
    char name[ 1024 ];

    ConstraintTatInt c;
	ConstraintTatIntHash::iterator cit;
    VariableTatInt v;
    VariableTatIntHash::iterator vit;

	vit = vHashTatico.begin();

	for ( ; vit != vHashTatico.end(); vit++ )
	{
		// s_{i,d,a}
		if( vit->first.getType() == VariableTatInt::V_ALOCA_ALUNO_TURMA_DISC )
		{			
			VariableTatInt v = vit->first;

			Aluno *aluno = v.getAluno();
			Disciplina *disciplina = v.getDisciplina();
			int turma = v.getTurma();
			Campus *campus = v.getCampus();

			if ( !aluno->ehFormando() )
				continue;

			// -----------------------------------------
			// Constraint 1
		
			c.reset();
			c.setType( ConstraintTatInt::C_FORMANDOS1 );
			c.setDisciplina(disciplina);
			c.setTurma(turma);
			c.setCampus(campus);

			cit = cHashTatico.find(c);

			if(cit != cHashTatico.end())
			{
				lp->chgCoef(cit->second, vit->second, -1.0);
			}
			else
			{
				int nnz=50;
				sprintf( name, "%s", c.toString( etapa ).c_str() ); 
				OPT_ROW row( nnz, OPT_ROW::GREATER, 0.0, name );

				row.insert(vit->second, -1.0);

				cHashTatico[ c ] = lp->getNumRows();

				lp->addRow( row );
				restricoes++;
			}						
		
			// -----------------------------------------
			// Constraint 2
			
			c.reset();
			c.setType( ConstraintTatInt::C_FORMANDOS2 );
			c.setDisciplina(disciplina);
			c.setTurma(turma);
			c.setCampus(campus);

			cit = cHashTatico.find(c);

			if(cit != cHashTatico.end())
			{
				lp->chgCoef(cit->second, vit->second, -1.0);
			}
			else
			{
				int nnz=50;
				sprintf( name, "%s", c.toString( etapa ).c_str() ); 
				OPT_ROW row( nnz, OPT_ROW::LESS, 0.0, name );

				row.insert(vit->second, -1.0);

				cHashTatico[ c ] = lp->getNumRows();

				lp->addRow( row );
				restricoes++;
			}
		}

		// f_{i,d,cp}
		else if( vit->first.getType() == VariableTatInt::V_FORMANDOS_NA_TURMA )
		{			
			VariableTatInt v = vit->first;

			Campus *campus = v.getCampus();
			Disciplina *disciplina = v.getDisciplina();
			int turma = v.getTurma();

			// -----------------------------------------
			// Constraint 1

			double M = problemData->getNroDemandaPorFormandos( disciplina, campus, prioridade );

			c.reset();
			c.setType( ConstraintTatInt::C_FORMANDOS1 );
			c.setDisciplina(disciplina);
			c.setTurma(turma);
			c.setCampus(campus);

			cit = cHashTatico.find(c);
			if(cit != cHashTatico.end())
			{
				lp->chgCoef(cit->second, vit->second, M);
			}
			else
			{
				int nnz=50;
				sprintf( name, "%s", c.toString( etapa ).c_str() ); 
				OPT_ROW row( nnz, OPT_ROW::GREATER, 0.0, name );

				row.insert(vit->second, M);

				cHashTatico[ c ] = lp->getNumRows();

				lp->addRow( row );
				restricoes++;
			}						
		
			// -----------------------------------------
			// Constraint 2
						
			c.reset();
			c.setType( ConstraintTatInt::C_FORMANDOS2 );
			c.setDisciplina(disciplina);
			c.setTurma(turma);
			c.setCampus(campus);

			cit = cHashTatico.find(c);

			if(cit != cHashTatico.end())
			{
				lp->chgCoef(cit->second, vit->second, 1.0);
			}
			else
			{
				int nnz=50;
				sprintf( name, "%s", c.toString( etapa ).c_str() ); 
				OPT_ROW row( nnz, OPT_ROW::LESS, 0.0, name );

				row.insert(vit->second, 1.0);

				cHashTatico[ c ] = lp->getNumRows();

				lp->addRow( row );
				restricoes++;
			}		
		}
	}
	
	return restricoes;
}

/*
	Para cada aluno a, disciplina d não atendida em rodada anterior:

	sum[deq]sum[i] s_{i,deq,a} + fd_{d,a} = 1

	aonde deq é uma disciplina equivalente a d. Caso deq tenha creditos praticos e teoricos, 
	só entram na restricao os teoricos.
*/
int TaticoIntAlunoHor::criaRestricaoTaticoAtendeAlunoEquiv( int campusId, int prioridade )
{
    int restricoes = 0;

	if ( !USAR_EQUIVALENCIA || !PERMITIR_NOVAS_TURMAS )
		return restricoes;
			
    char name[ 1024 ];
    int nnz;

    ConstraintTatInt c;
    VariableTatInt v;
    VariableTatIntHash::iterator it_v;   

    Campus *campus = problemData->refCampus[campusId];

	ITERA_GGROUP_LESSPTR( itAluno, problemData->alunos, Aluno )
	{
		Aluno *aluno = *itAluno;
		
		ITERA_GGROUP_LESSPTR( itAlDemanda, aluno->demandas, AlunoDemanda )
		{
			Disciplina *disciplina = itAlDemanda->demanda->disciplina;

			if ( itAlDemanda->getPrioridade() != prioridade )
				continue;
			
			if ( problemData->retornaTurmaDiscAluno( aluno, disciplina ) != -1 )
				continue;

			c.reset();
			c.setType( ConstraintTatInt::C_DEMANDA_EQUIV_ALUNO );
			c.setAluno( aluno );
			c.setDisciplina( disciplina );
			c.setCampus( campus );

			sprintf( name, "%s", c.toString( etapa ).c_str() );

			if ( cHashTatico.find( c ) != cHashTatico.end() )
			{
				continue;
			}

			nnz = 10;
			OPT_ROW row( nnz, OPT_ROW::EQUAL, 1.0, name );

			ITERA_GGROUP_LESSPTR( itDisc, disciplina->discEquivSubstitutas, Disciplina )
			{
				Disciplina *deq = *itDisc;
				
				// Pula disciplina pratica
				if ( deq->getId() < 0 && deq!=disciplina )
					continue;

				if ( ! problemData->alocacaoEquivViavel( (*itAlDemanda)->demanda, *itDisc ) )
					continue;	

				if ( problemData->retornaTurmaDiscAluno( aluno, deq ) != -1 )
				{
					std::cout<<"\nEstranho, isso nao faz muito sentido. Significa que o aluno possui duas demandas"
						" diferentes para disciplinas consideradas equivalentes. Aluno id = " << aluno->getAlunoId()
						<< ", disciplina original id = "<< disciplina->getId() << " e disciplina substituta id=" 
						<< deq->getId() << endl;
					continue;
				}

				for ( int turma = 1; turma <= deq->getNumTurmas(); turma++ )
				{
					VariableTatInt v;
					v.reset();
					v.setType( VariableTatInt::V_ALOCA_ALUNO_TURMA_DISC );
					v.setAluno( aluno );
					v.setDisciplina( deq );
					v.setTurma( turma );
					v.setCampus( campus );
					v.setAlunoDemanda( *itAlDemanda );

					it_v = vHashTatico.find( v );
					if( it_v != vHashTatico.end() )
					{
						row.insert( it_v->second, 1.0 );
					}
				}
			}

			VariableTatInt v;
			v.reset();
			v.setType( VariableTatInt::V_SLACK_DEMANDA_ALUNO );
			v.setAluno( aluno );
			v.setDisciplina( disciplina );
			it_v = vHashTatico.find( v );
			if( it_v != vHashTatico.end() )
			{
				row.insert( it_v->second, 1.0 );
			}

			if ( row.getnnz() >= 1 )
			{
				cHashTatico[ c ] = lp->getNumRows();
				lp->addRow( row );
				restricoes++;
			}
		}
	}

	return restricoes;

}


/*
	Para cada AlunoDemanda a-d:

	sum[deq]sum[i] s_{i,deq,a} + fd_{d,a} = 1

	aonde deq é uma disciplina equivalente a d. Caso deq tenha creditos praticos e teoricos, 
	só entram na restricao os teoricos.
*/
int TaticoIntAlunoHor::criaRestricaoTaticoAtendeAlunoEquivTotal( int campusId, int prioridade )
{
    int restricoes = 0;

	if ( !USAR_EQUIVALENCIA || PERMITIR_NOVAS_TURMAS )
		return restricoes;
		
    char name[ 1024 ];
    int nnz;

    ConstraintTatInt c;
    VariableTatInt v;
    VariableTatIntHash::iterator it_v;   

    Campus *campus = problemData->refCampus[campusId];

	ITERA_GGROUP_LESSPTR( itAluno, problemData->alunos, Aluno )
	{
		Aluno *aluno = *itAluno;
		
		ITERA_GGROUP_LESSPTR( itAlDemanda, aluno->demandas, AlunoDemanda )
		{
			Disciplina *disciplina = itAlDemanda->demanda->disciplina;
						
			if ( itAlDemanda->getPrioridade() != prioridade )
				continue;
						
			c.reset();
			c.setType( ConstraintTatInt::C_DEMANDA_EQUIV_ALUNO );
			c.setAluno( aluno );
			c.setDisciplina( disciplina );
			c.setCampus( campus );

			sprintf( name, "%s", c.toString( etapa ).c_str() );

			if ( cHashTatico.find( c ) != cHashTatico.end() )
			{
				continue;
			}

			nnz = 10;
			OPT_ROW row( nnz, OPT_ROW::EQUAL, 1.0, name );

			GGroup< Disciplina*, LessPtr<Disciplina>> discs;
			discs.add( disciplina );
			discs.add( disciplina->discEquivSubstitutas );

			ITERA_GGROUP_LESSPTR( itDisc, discs, Disciplina )
			{
				Disciplina *deq = *itDisc;
			//	std::cout<<"\n\tdisc " << deq->getId();

				// Pula disciplina pratica
				if ( deq->getId() < 0 && deq!=disciplina )
					continue;

				if ( ! problemData->alocacaoEquivViavel( (*itAlDemanda)->demanda, *itDisc ) )
					continue;	
	//			std::cout<<"  viavel ";

				for ( int turma = 1; turma <= deq->getNumTurmas(); turma++ )
				{
					VariableTatInt v;
					v.reset();
					v.setType( VariableTatInt::V_ALOCA_ALUNO_TURMA_DISC );
					v.setAluno( aluno );
					v.setDisciplina( deq );
					v.setTurma( turma );
					v.setCampus( campus );
					v.setAlunoDemanda( *itAlDemanda );

					it_v = vHashTatico.find( v );
					if( it_v != vHashTatico.end() )
					{
						row.insert( it_v->second, 1.0 );
					}
				}
			}

			VariableTatInt v;
			v.reset();
			v.setType( VariableTatInt::V_SLACK_DEMANDA_ALUNO );
			v.setAluno( aluno );
			v.setDisciplina( disciplina );
			it_v = vHashTatico.find( v );
			if( it_v != vHashTatico.end() )
			{
				row.insert( it_v->second, 1.0 );
			}

			if ( row.getnnz() >= 1 )
			{
				cHashTatico[ c ] = lp->getNumRows();
				lp->addRow( row );
				restricoes++;
			}
		}
	}

	return restricoes;

}



/*
	Para cada disciplina substituta (por equivalencia) a uma demanda não atendida de um aluno,
	se ela for pratica+teorica, cria restrição que garante atendimento mútuo ou nenhum.
*/
int TaticoIntAlunoHor::criaRestricaoTaticoAlunoDiscPraticaTeoricaEquiv_MxN( int campusId, int prioridade )
{
	// TODO: TEM ALGUM ERRO. ESTÁ OCORRENDO CASOS DE, QUANDO EXISTE SOMENTE A VARIAVEL S PARA A PRATICA,
	// NÃO É CRIADA A RESTRIÇÃO, E CONSEQUENTEMENTE É PERMITIDA A ALOCAÇÃO DO ALUNO SOMENTE EM CREDITO
	// PRATICO. REESCREVER ESSE MÉTODO VARRENDO O HASH!

   int restricoes = 0;   

	if ( !problemData->parametros->discPratTeorMxN )
	{
		return restricoes;
	}

	if ( ! problemData->parametros->considerar_equivalencia_por_aluno || !USAR_EQUIVALENCIA )
		return restricoes;

   char name[ 1024 ];
   int nnz;

   ConstraintTatInt c;
   VariableTatInt v;
   VariableTatIntHash::iterator it_v;   

   Campus *campus = problemData->refCampus[campusId];

	ITERA_GGROUP_LESSPTR( itAluno, problemData->alunos, Aluno )
	{
		Aluno *aluno = *itAluno;
		  

		ITERA_GGROUP_LESSPTR( itAlDemanda, aluno->demandas, AlunoDemanda )
		{
			if ( itAlDemanda->getOferta()->getCampusId() != campusId )
			{
				continue;
			}				

			if ( itAlDemanda->getPrioridade() != prioridade )
				continue;
			
			if ( problemData->retornaTurmaDiscAluno( aluno, itAlDemanda->demanda->disciplina ) != -1 )
				continue;

			// Demanda ainda não atendida

			// Pula disciplina pratica, pq a referencia para as equivalentes é informada pelas teoricas
			if ( itAlDemanda->demanda->disciplina->getId() < 0 )
				continue;

			Disciplina *discOrigTeorica = itAlDemanda->demanda->disciplina;

			ITERA_GGROUP_LESSPTR( itDisc, discOrigTeorica->discEquivSubstitutas, Disciplina )
			{								
				// Pula disciplina pratica
				if ( itDisc->getId() < 0 )
					continue;
				
				Disciplina *discTeorica = *itDisc;
				
				if ( ! problemData->alocacaoEquivViavel( (*itAlDemanda)->demanda, *itDisc ) )
					continue;	

				if ( problemData->refDisciplinas.find( - discTeorica->getId() ) ==
					 problemData->refDisciplinas.end() ) // Restrição somente para disciplinas de creditos praticos+teoricos
					continue;

				Disciplina *discPratica = problemData->refDisciplinas[ - discTeorica->getId() ];
							
				c.reset();
				c.setType( ConstraintTatInt::C_ALUNO_DISC_PRATICA_TEORICA_EQUIV );
				c.setAluno( aluno );
				c.setDisciplina( discPratica );
				c.setCampus( campus );

				sprintf( name, "%s", c.toString( etapa ).c_str() ); 

				if ( cHashTatico.find( c ) != cHashTatico.end() )
				{
					continue;
				}

				nnz = discPratica->getNumTurmas() + discTeorica->getNumTurmas();

				OPT_ROW row( nnz, OPT_ROW::EQUAL , 0.0 , name );

				for ( int turma = 1; turma <= discPratica->getNumTurmas(); turma++ )
				{
					VariableTatInt v;
					v.reset();
					v.setType( VariableTatInt::V_ALOCA_ALUNO_TURMA_DISC );
					v.setAluno( aluno );
					v.setDisciplina( discPratica );
					v.setTurma( turma );
					v.setCampus( campus );

					// Em equivalentes, a referencia do AlunoDemanda para disc pratica está a mesma que para disc teorica,
					// porque pode existir caso de equivalencia entre disciplinas: T -> TP
					// Aí o AlunoDemanda pratico só é criado após a substituição por equivalência ser feita pelo modelo.
					v.setAlunoDemanda( *itAlDemanda ); 

					it_v = vHashTatico.find( v );
					if( it_v != vHashTatico.end() )
					{
						row.insert( it_v->second, 1.0 );
					}
				}

				for ( int turma = 1; turma <= discTeorica->getNumTurmas(); turma++ )
				{
					VariableTatInt v;
					v.reset();
					v.setType( VariableTatInt::V_ALOCA_ALUNO_TURMA_DISC );
					v.setAluno( aluno );
					v.setDisciplina( discTeorica );
					v.setTurma( turma );
					v.setCampus( campus );
					v.setAlunoDemanda( *itAlDemanda );

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

	return restricoes;

}

int TaticoIntAlunoHor::criaRestricaoTaticoAtivaZ( int campusId )
{
   int restricoes = 0;
   int nnz;
   char name[ 1024 ];

   ConstraintTatInt c;
   VariableTatIntHash::iterator vit;
   ConstraintTatIntHash::iterator cit;

   std::vector< std::pair< int, int > > coeffList;
   std::vector< double > coeffListVal;
   std::pair< int, int > auxCoef;

   vit = vHashTatico.begin();

   for (; vit != vHashTatico.end(); vit++ )
   {
		VariableTatInt v = vit->first;

		if ( v.getType() != VariableTatInt::V_OFERECIMENTO &&
			 v.getType() != VariableTatInt::V_ABERTURA )
		{
			continue;
		}
		
		int turma = v.getTurma();
		Disciplina* disc = v.getDisciplina();
			
		double coef=0;
		if ( v.getType() == VariableTatInt::V_OFERECIMENTO )
		{
			coef = -1.0;
		}
		else if ( v.getType() == VariableTatInt::V_ABERTURA )
		{
			coef = 1.0;			
		}

		c.reset();
		c.setType( ConstraintTatInt::C_ATIVA_Z );
		c.setTurma( turma );
		c.setDisciplina( disc );

		cit = cHashTatico.find( c );

		if ( cit != cHashTatico.end() )
		{
			auxCoef.first = cit->second;
			auxCoef.second = vit->second;

			coeffList.push_back( auxCoef );
			coeffListVal.push_back( coef );
		}
		else
		{			
			sprintf( name, "%s", c.toString( etapa ).c_str() );
			nnz = 100;

			OPT_ROW row( nnz, OPT_ROW::GREATER, 0.0, name );
						
			row.insert( vit->second, coef );

			// Insere restrição
			cHashTatico[ c ] = lp->getNumRows();
			lp->addRow( row );
			restricoes++;
		}
   }

   chgCoeffList( coeffList, coeffListVal );

   return restricoes;
}


/*
	Para cada turma i, disciplina d, campus cp:

	sum[s]sum[t]sum[hi]sum[hf] NHC_{d,hi,hf} * x_{i,d,s,t,hi,hf} = NC_{d} * z_{i,d,cp}

	NHC_{d,hi,hf} = numero de creditos da disciplina d entre os horarios hi e hf
	NHC_{d} = numero total de creditos da disciplina d

	Impede que haja um x_{i,d,s,t1,hi,hf} com um grupo de G1 de alunos e um outro x_{i,d,s,t2,hi,hf}
	com outro grupo G2 de alunos, G1 != G2. Ou seja, se dois alunos pertencem a (i,d,cp), eles
	pertencem às mesmas aulas de (i,d,cp). De quebrada, garante o total de créditos da disciplina,
	mas isso já é garantido em outra restrição, logo não é o essencial dessa restrição.
*/
int TaticoIntAlunoHor::criaRestricaoTaticoTurmaComOsMesmosAlunosPorAula( int campusId )
{
   int restricoes = 0;

   if ( ! this->PERMITIR_NOVAS_TURMAS )
	   return restricoes;

   int nnz;
   char name[ 1024 ];

   ConstraintTatInt c;
   VariableTatIntHash::iterator vit;
   ConstraintTatIntHash::iterator cit;

   std::vector< std::pair< int, int > > coeffList;
   std::vector< double > coeffListVal;
   std::pair< int, int > auxCoef;

   vit = vHashTatico.begin();

   for (; vit != vHashTatico.end(); vit++ )
   {
		VariableTatInt v = vit->first;

		if ( v.getType() != VariableTatInt::V_CREDITOS &&
			 v.getType() != VariableTatInt::V_ABERTURA )
		{
			continue;
		}
		
		int turma = v.getTurma();
		Disciplina* disc = v.getDisciplina();
		Campus *cp=NULL;

		double coef=0;
		if ( v.getType() == VariableTatInt::V_CREDITOS )
		{
			cp = problemData->refCampus[ v.getUnidade()->getIdCampus() ];
			if ( cp->getId() != campusId ) continue;

			HorarioAula *hi = v.getHorarioAulaInicial();
			HorarioAula *hf = v.getHorarioAulaFinal();
			Calendario *calendario = hi->getCalendario();
			coef = calendario->retornaNroCreditosEntreHorarios(hi, hf);
		}
		else if ( v.getType() == VariableTatInt::V_ABERTURA )
		{
			cp = v.getCampus();
			if ( cp->getId() != campusId ) continue;

			coef = - disc->getTotalCreditos();			
		}

		c.reset();
		c.setType( ConstraintTatInt::C_TURMA_COM_MESMOS_ALUNOS_POR_AULA );
		c.setTurma( turma );
		c.setDisciplina( disc );
		c.setCampus( cp );

		cit = cHashTatico.find( c );

		if ( cit != cHashTatico.end() )
		{
			auxCoef.first = cit->second;
			auxCoef.second = vit->second;

			coeffList.push_back( auxCoef );
			coeffListVal.push_back( coef );
		}
		else
		{			
			sprintf( name, "%s", c.toString( etapa ).c_str() );
			nnz = 100;

			OPT_ROW row( nnz, OPT_ROW::EQUAL, 0.0, name );
						
			row.insert( vit->second, coef );

			// Insere restrição
			cHashTatico[ c ] = lp->getNumRows();
			lp->addRow( row );
			restricoes++;
		}
   }

   chgCoeffList( coeffList, coeffListVal );

   return restricoes;
}


/*
	Para cada disciplina d, dia t, horarioAula h:

	sum[s]sum[i]sum[hi]sum[hf] x_{i,d,s,t,hi,hf} - fp_{d,t,h} <= nProfs_{d,t,h}

	sendo que (hi,hf) contém h

*/
int TaticoIntAlunoHor::criaRestricaoTaticoConsideraHorariosProfs( int campusId )
{
	int restricoes = 0;
	if ( ! problemData->parametros->considerar_disponibilidade_prof_em_tatico )
		return restricoes;

	int nnz;
	char name[ 1024 ];

	VariableTatInt v;	
    ConstraintTatInt c;
    VariableTatIntHash::iterator vit;
    ConstraintTatIntHash::iterator cit;
			
	std::map< Disciplina*, std::map<int /*dia*/, std::map< HorarioAula*, GGroup<Professor*, LessPtr<Professor>>, LessPtr<HorarioAula> >>, LessPtr<Disciplina> >
		mapDiscDiaHorProfs;
    GGroup< Professor *, LessPtr< Professor > > professores = problemData->getProfessores();
    ITERA_GGROUP_LESSPTR( itProf, professores, Professor )
    {
		Professor *prof = *itProf;

		ITERA_GGROUP_LESSPTR( itHorDia, prof->horariosDia, HorarioDia )
		{
			int dia = itHorDia->getDia();
			HorarioAula *horAula = itHorDia->getHorarioAula();
					
			ITERA_GGROUP_LESSPTR( itMagist, itProf->magisterio, Magisterio )
			{
				Disciplina *disciplina = itMagist->disciplina;
				mapDiscDiaHorProfs[disciplina][dia][horAula].add(prof);;
			}
		}
	}

   std::vector<int> idxC;
   std::vector<int> idxR;
   std::vector<double> valC;

	vit = vHashTatico.begin();
	for ( ; vit != vHashTatico.end(); vit++ )
	{		
		VariableTatInt v = vit->first;
		
		if ( v.getType() == VariableTatInt::V_CREDITOS )
		{		
			Disciplina *disciplina = v.getDisciplina();
			int dia = v.getDia();
			HorarioAula *hf = v.getHorarioAulaFinal();
			HorarioAula *ha = v.getHorarioAulaInicial();
			
			bool end=false;
			while ( !end )
			{
				c.reset();
				c.setType( ConstraintTatInt::C_DISC_DIA_HOR_PROF );
				c.setDisciplina( disciplina );
				c.setDia( dia );
				c.setHorarioAulaInicial( ha );
				std::pair<DateTime*,int> auxP = problemData->horarioAulaDateTime[ha->getId()];
				c.setDateTimeInicial(auxP.first);
				
				cit = cHashTatico.find(c);
				if(cit == cHashTatico.end())
				{
					int nProfs = mapDiscDiaHorProfs[disciplina][dia][ha].size();

					sprintf( name, "%s", c.toString( etapa ).c_str() );
					OPT_ROW row( 100, OPT_ROW::LESS, nProfs, name );

					row.insert( vit->second, 1.0);

					cHashTatico[ c ] = lp->getNumRows();
					lp->addRow( row );
					restricoes++;
				}
				else
				{
					bool jaExiste=false;
					for ( int i=0; i<(int)idxC.size(); i++ )
					{
						if ( idxC[i]==vit->second && idxR[i]==cit->second )
						{
							jaExiste=true;break;
						}
					}
					if (!jaExiste)
					{
						idxC.push_back(vit->second);
						idxR.push_back(cit->second);
						valC.push_back(1.0);
					}
				}			

				if ( ha->getInicio() == hf->getInicio() ) end=true;
				else ha = ha->getCalendario()->getProximoHorario( ha );

				if ( ha==NULL )
				{
					std::cout<<"\n\nErro em TatInt! horario eh null antes de chegar em hf";
					std::cout<<"\nVar "<< v.toString();
					end=true;
				}
			}
		}
	}


	vit = vHashTatico.begin();
	for ( ; vit != vHashTatico.end(); vit++ )
	{		
		VariableTatInt v = vit->first;
		
		if ( v.getType() == VariableTatInt::V_FOLGA_HOR_PROF )
		{
			Disciplina *disciplina = v.getDisciplina();
			int dia = v.getDia();

			HorarioAula *ha = v.getHorarioAulaInicial();
			double coef = -1.0;

			c.reset();
			c.setType( ConstraintTatInt::C_DISC_DIA_HOR_PROF );			
			c.setDisciplina( disciplina );
			c.setDia( dia );
			c.setHorarioAulaInicial( ha );
			std::pair<DateTime*,int> auxP = problemData->horarioAulaDateTime[ha->getId()];
			c.setDateTimeInicial(auxP.first);

			cit = cHashTatico.find(c);
			if(cit != cHashTatico.end())
			{
				idxC.push_back(vit->second);
				idxR.push_back(cit->second);
				valC.push_back(coef);
			}		
		}
	}	

   lp->updateLP();
   lp->chgCoefList(idxC.size(),idxR.data(),idxC.data(),valC.data());
   lp->updateLP();

   idxC.clear();
   idxR.clear();
   valC.clear();

	return restricoes;
}



/*
Aulas de disciplinas pratica/teorica continuas

sum[s] x_{it,dt,s,t,hi,hf} <= sum[h]sum[s] x_{ip,dp,s,t,h,hi-1} + sum[h]sum[s] x_{ip,dp,s,t,hf+1,h}

Para toda disciplina d=(dt,dp) sendo d com obrigação de créditos contínuos
Para toda turma it \in I_{dt}, ip \in I_{dp} sendo que (it,dt) tem aluno comum com (ip,dp)
Para todo dia t
Para todo par de horários hi,hf
	
*/
int TaticoIntAlunoHor::criaRestricaoTaticoDiscPTAulasContinuas( int campusId, int prioridade )
{
	int restricoes = 0;
	int nnz;
	char name[ 100 ];

	VariableTatInt v;
	ConstraintTatInt c;
	VariableTatIntHash::iterator vit;
	ConstraintTatIntHash::iterator cit;

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

   std::map<Disciplina*, std::map< int /*turma*/, std::map< int /*dia*/, std::map< DateTime, GGroup<VariableTatInt> > > >, LessPtr<Disciplina>> mapDiscDiaHiVar;

	vit = vHashTatico.begin();
	while(vit != vHashTatico.end())
	{
		if( vit->first.getType() == VariableTatInt::V_CREDITOS )
		{
			VariableTatInt v = vit->first;
			if (v.getUnidade()->getIdCampus() == campusId)
			{
				if ( v.getDisciplina()->getId() > 0 )
				if ( problemData->getDisciplinaTeorPrat( v.getDisciplina() ) == NULL )
				{
					vit++; continue;
				}
				
				if ( v.getDisciplina()->aulasContinuas() )
				{	
					mapDiscDiaHiVar[ v.getDisciplina() ][ v.getTurma() ][ v.getDia() ][ v.getHorarioAulaInicial()->getInicio() ].add( v );
				}
			}
		}
		vit++;
	}

	std::map<Disciplina*, std::map< int /*turma*/, std::map< int /*dia*/, std::map<DateTime, GGroup<VariableTatInt>> > >, LessPtr<Disciplina>>::iterator
		itMapDisc = mapDiscDiaHiVar.begin();
	for ( ; itMapDisc != mapDiscDiaHiVar.end(); itMapDisc++ )
	{	
		if ( itMapDisc->first->getId() < 0 ) continue;

		Disciplina *disciplinaTeor = itMapDisc->first;

		std::map< int /*turma*/, std::map< int /*dia*/, std::map< DateTime, GGroup<VariableTatInt> > > >::iterator
			itTurma = itMapDisc->second.begin();
		for ( ; itTurma != itMapDisc->second.end(); itTurma++ )
		{
			int turmaTeor = itTurma->first;

			std::map< int /*dia*/, std::map< DateTime, GGroup<VariableTatInt> > >::iterator
				itMapDia = itTurma->second.begin();

			for ( ; itMapDia != itTurma->second.end(); itMapDia++ )
			{	
				int dia = itMapDia->first;

				std::map< DateTime, GGroup<VariableTatInt> >::iterator
					itMapDateTime = itMapDia->second.begin();
				for ( ; itMapDateTime != itMapDia->second.end(); itMapDateTime++ )
				{	
					DateTime dt = itMapDateTime->first;
					GGroup<VariableTatInt> ggroupVars = itMapDateTime->second;
				
					ITERA_GGROUP_N_PT( itVars, ggroupVars, VariableTatInt )
					{
						VariableTatInt v_t = *itVars;

						HorarioAula *hi = v_t.getHorarioAulaInicial();
						HorarioAula *hf = v_t.getHorarioAulaFinal();				
					
						DateTime *dti = problemData->horarioAulaDateTime[hi->getId()].first;
						DateTime *dtf = problemData->horarioAulaDateTime[hf->getId()].first;

						c.reset();
						c.setType( ConstraintTatInt::C_AULA_PT_SEQUENCIAL );
						c.setCampus( campus );
						c.setTurma( turmaTeor );
						c.setDisciplina( disciplinaTeor );
						c.setDia( dia );
						c.setDateTimeInicial( dti );
						c.setDateTimeFinal( dtf );
					
						sprintf( name, "%s", c.toString( etapa ).c_str() );
						OPT_ROW row( 100, OPT_ROW::LESS , 0.0 , name );
				
						// --------------------
						// teórica
						row.insert( vHashTatico[v_t], 1.0 );
				
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
								std::map< int /*dia*/, std::map< DateTime, GGroup<VariableTatInt> > > >, LessPtr<Disciplina>>::iterator
							itDisc = mapDiscDiaHiVar.find( disciplinaPrat );
							if ( itDisc != mapDiscDiaHiVar.end() )
							{
								std::map< int /*turma*/, std::map< int /*dia*/, std::map< DateTime, GGroup<VariableTatInt> > > >::iterator
								itTurma = itDisc->second.find( turmaTeor );
								if ( itTurma != itDisc->second.end() )
								{
									std::map< int /*dia*/, std::map< DateTime, GGroup<VariableTatInt> > >::iterator
									itDia = itTurma->second.find( dia );
									if ( itDia != itTurma->second.end() )
									{
										std::map< DateTime, GGroup<VariableTatInt> >::iterator
										itDt = itDia->second.find( dti_p1 );
										if ( itDt != itDia->second.end() )
										{
											GGroup<VariableTatInt> vars = itDt->second;
											ITERA_GGROUP_N_PT( itVars, ggroupVars, VariableTatInt )
											{
												VariableTatInt v_p1 = *itVars;
												row.insert( vHashTatico[v_p1], -1.0 );
											}
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
								std::map< int /*dia*/, std::map< DateTime, GGroup<VariableTatInt> > > >, LessPtr<Disciplina>>::iterator
							itDisc = mapDiscDiaHiVar.find( disciplinaPrat );
							if ( itDisc != mapDiscDiaHiVar.end() )
							{
								std::map< int /*turma*/, std::map< int /*dia*/, std::map< DateTime, GGroup<VariableTatInt> > > >::iterator
								itTurma = itDisc->second.find( turmaTeor );
								if ( itTurma != itDisc->second.end() )
								{
									std::map< int /*dia*/, std::map< DateTime, GGroup<VariableTatInt> > >::iterator
									itDia = itTurma->second.find( dia );
									if ( itDia != itTurma->second.end() )
									{
										std::map< DateTime, GGroup<VariableTatInt> >::iterator
										itDt = itDia->second.find( dti_p2 );
										if ( itDt != itDia->second.end() )
										{
											GGroup<VariableTatInt> vars = itDt->second;
											ITERA_GGROUP_N_PT( itVars, ggroupVars, VariableTatInt )
											{
												VariableTatInt v_p2 = *itVars;
												row.insert( vHashTatico[v_p2], -1.0 );
											}
										}
									}
								}
							}
						}

						// --------------------
						if( row.getnnz() > 0 )
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

	if ( restricoes > 0 && !problemData->parametros->discPratTeor1x1 )
	{
		std::cout << "\nERRO: ha obrigatoriedade de aulas continuas para disciplinas praticas e teoricas"
			<< " mesmo permitindo-se a mistura de alunos entre turmas praticas e teoricas. Essa funcao nao "
			<< " esta preparada para isso.\n";
	}

	return restricoes;
}


int TaticoIntAlunoHor::criaRestricaoTaticoAlunoDiscPraticaTeorica_1xN_antiga( int campusId )
{
	int numRest=0;

	// Por enquanto só a Unit usa essa relação 1xN
	if ( ! problemData->parametros->discPratTeor1xN_antigo )
	{
		return numRest;
	}

    char name[ 100 ];

	VariableTatInt v;
	ConstraintTatInt c;
	VariableTatIntHash::iterator vit;
	ConstraintTatIntHash::iterator cit;

	map< int /*turma*/, map< Disciplina*, map<int, VariableTatInt>, LessPtr<Disciplina> > >
		mapTurmaDiscTeorAlunos;

	map< Disciplina*, map< Aluno*, map<Aluno*, int, LessPtr<Aluno>>, LessPtr<Aluno> >, LessPtr<Disciplina> >
		mapDiscTeorAluno1Aluno2VarSS;

	vit = vHashTatico.begin();
	for ( ; vit != vHashTatico.end(); vit++ )
	{
		v = vit->first;

		if( v.getType() == VariableTatInt::V_ALOCA_ALUNO_TURMA_DISC ) // s_{i,d,a}
		{				
			int discId = v.getDisciplina()->getId();
			if ( discId < 0 ) continue;

			// Só para disciplinas praticas/teoricas
			std::map< int, Disciplina * >::iterator itMapDisc = problemData->refDisciplinas.find( - discId );
			if ( itMapDisc == problemData->refDisciplinas.end() )
				continue;
		
			int turmaTeor = v.getTurma();
			Disciplina *discTeor = v.getDisciplina();

			mapTurmaDiscTeorAlunos[turmaTeor][discTeor][vit->second] = v;
		}
		else if( v.getType() == VariableTatInt::V_ALUNOS_MESMA_TURMA_PRAT ) // ss_{a1,a2,dp}
		{				
			int discPratId = v.getDisciplina()->getId();
			
			Disciplina *discTeor = problemData->refDisciplinas[ -discPratId ];

			Aluno* aluno1 = v.getParAlunos().first;
			Aluno* aluno2 = v.getParAlunos().second;

			if ( aluno1->getAlunoId() < aluno2->getAlunoId() )
				mapDiscTeorAluno1Aluno2VarSS[discTeor][aluno1][aluno2] = vit->second;
			else
				mapDiscTeorAluno1Aluno2VarSS[discTeor][aluno2][aluno1] = vit->second;
		}
	}

	map< int /*turma*/, map< Disciplina*, map<int, VariableTatInt>, LessPtr<Disciplina> > >::iterator
		itMapTurma = mapTurmaDiscTeorAlunos.begin();
	for ( ; itMapTurma != mapTurmaDiscTeorAlunos.end(); itMapTurma++ )
	{
		int turmaTeor = itMapTurma->first;

		map< Disciplina*, map<int, VariableTatInt>, LessPtr<Disciplina> >::iterator
			itMapDisc = itMapTurma->second.begin();
		for ( ; itMapDisc != itMapTurma->second.end(); itMapDisc++ )
		{
			Disciplina *discTeor = itMapDisc->first;

			map<int, VariableTatInt> variables = itMapDisc->second;

			map<int, VariableTatInt>::iterator itMapVar1 = variables.begin();
			for( ; itMapVar1 != variables.end(); itMapVar1++ )
			{
				VariableTatInt v1 = itMapVar1->second; // s_{i,d,a1}
				int indx1 = itMapVar1->first;

				map<int, VariableTatInt>::iterator itMapVar2 = variables.begin();
				for( ; itMapVar2 != variables.end(); itMapVar2++ )
				{
					VariableTatInt v2 = itMapVar2->second; // s_{i,d,a2}
					int indx2 = itMapVar2->first;

					if ( v1.getAluno()->getAlunoId() >= v2.getAluno()->getAlunoId() ) continue;

					std::pair<Aluno*, Aluno*> parAlunos( v1.getAluno(), v2.getAluno() );

					int indx3 = mapDiscTeorAluno1Aluno2VarSS[discTeor][v1.getAluno()][v2.getAluno()]; // ss_{dp,a1,a2}

					c.reset();
					c.setType( ConstraintTatInt::C_ALUNO_DISC_PRATICA_TEORICA_1xN );
					c.setDisciplina( discTeor );
					c.setTurma( turmaTeor );
					c.setParAlunos( parAlunos );

					cit = cHashTatico.find(c);
					if(cit == cHashTatico.end())
					{
						int nnz = 3;

						sprintf( name, "%s", c.toString(etapa).c_str() ); 
						OPT_ROW row( nnz, OPT_ROW::LESS, 0.0, name );

						row.insert(indx1, -1.0);
						row.insert(indx2, -1.0);
						row.insert(indx3, 2.0);

						cHashTatico[ c ] = lp->getNumRows();

						lp->addRow( row );
						numRest++;
					}
				}
			}
		}
	}

	return numRest;
}


int TaticoIntAlunoHor::criaRestricaoTaticoAlunosMesmaTurmaPratica( int campusId )
{
	int numRest=0;

	// Por enquanto só a Unit usa a relação 1xN
	if ( ! problemData->parametros->discPratTeor1xN_antigo )
	{
		return numRest;
	}

    char name[ 100 ];

	VariableTatInt v;
	ConstraintTatInt c;
	VariableTatIntHash::iterator vit;
	ConstraintTatIntHash::iterator cit;

	map< int /*turma*/, map< Disciplina*, map<int, VariableTatInt>, LessPtr<Disciplina> > >
		mapTurmaDiscPratAlunos;

	map< Disciplina*, map< Aluno*, map<Aluno*, int, LessPtr<Aluno>>, LessPtr<Aluno> >, LessPtr<Disciplina> >
		mapDiscPratAluno1Aluno2VarSS;

	vit = vHashTatico.begin();
	for ( ; vit != vHashTatico.end(); vit++ )
	{
		v = vit->first;

		if( v.getType() == VariableTatInt::V_ALOCA_ALUNO_TURMA_DISC ) // s_{i,d,a}
		{				
			int discId = v.getDisciplina()->getId();
			if ( discId > 0 ) continue;
					
			int turmaPrat = v.getTurma();
			Disciplina *discPrat = v.getDisciplina();

			mapTurmaDiscPratAlunos[turmaPrat][discPrat][vit->second] = v;
		}
		else if( v.getType() == VariableTatInt::V_ALUNOS_MESMA_TURMA_PRAT ) // ss_{a1,a2,dp}
		{				
			Disciplina *discPrat = v.getDisciplina();			
			Aluno* aluno1 = v.getParAlunos().first;
			Aluno* aluno2 = v.getParAlunos().second;

			if ( aluno1->getAlunoId() < aluno2->getAlunoId() )
				mapDiscPratAluno1Aluno2VarSS[discPrat][aluno1][aluno2] = vit->second;
			else
				mapDiscPratAluno1Aluno2VarSS[discPrat][aluno2][aluno1] = vit->second;
		}
	}

	map< int /*turma*/, map< Disciplina*, map<int, VariableTatInt>, LessPtr<Disciplina> > >::iterator
		itMapTurma = mapTurmaDiscPratAlunos.begin();
	for ( ; itMapTurma != mapTurmaDiscPratAlunos.end(); itMapTurma++ )
	{
		int turmaPrat = itMapTurma->first;

		map< Disciplina*, map<int, VariableTatInt>, LessPtr<Disciplina> >::iterator
			itMapDisc = itMapTurma->second.begin();
		for ( ; itMapDisc != itMapTurma->second.end(); itMapDisc++ )
		{
			Disciplina *discPrat = itMapDisc->first;

			map<int, VariableTatInt> variables = itMapDisc->second;

			map<int, VariableTatInt>::iterator itMapVar1 = variables.begin();
			for( ; itMapVar1 != variables.end(); itMapVar1++ )
			{
				VariableTatInt v1 = itMapVar1->second; // s_{i,d,a1}
				int indx1 = itMapVar1->first;

				map<int, VariableTatInt>::iterator itMapVar2 = variables.begin();
				for( ; itMapVar2 != variables.end(); itMapVar2++ )
				{
					VariableTatInt v2 = itMapVar2->second; // s_{i,d,a2}
					int indx2 = itMapVar2->first;

					if ( v1.getAluno()->getAlunoId() >= v2.getAluno()->getAlunoId() ) continue;

					std::pair<Aluno*, Aluno*> parAlunos( v1.getAluno(), v2.getAluno() );

					int indx3 = mapDiscPratAluno1Aluno2VarSS[discPrat][v1.getAluno()][v2.getAluno()]; // ss_{dp,a1,a2}

					c.reset();
					c.setType( ConstraintTatInt::C_ALUNOS_MESMA_TURMA_PRAT );
					c.setDisciplina( discPrat );
					c.setTurma( turmaPrat );
					c.setParAlunos( parAlunos );

					cit = cHashTatico.find(c);
					if(cit == cHashTatico.end())
					{
						int nnz = 3;

						sprintf( name, "%s", c.toString(etapa).c_str() ); 
						OPT_ROW row( nnz, OPT_ROW::LESS, 1.0, name );

						row.insert(indx1, 1.0);
						row.insert(indx2, 1.0);
						row.insert(indx3, -1.0);

						cHashTatico[ c ] = lp->getNumRows();

						lp->addRow( row );
						numRest++;
					}
				}
			}
		}
	}

	return numRest;
}



/*
	Alocação minima de demanda por aluno

	sum[d] nCreds_{d} * (1 - fd_{d,a}) >= MinAtendPerc * TotalDemanda_{a} - fmd1_{a} - fmd2_{a} - fmd3_{a}, para cada aluno a
	
	min sum[a] fmd{a}
*/
int TaticoIntAlunoHor::criaRestricaoTaticoAlocMinAluno( int campusId )
{
   int restricoes = 0;
 
   if ( ! problemData->parametros->considerarMinPercAtendAluno )
   {
		return restricoes;
   }

   char name[ 200 ];
   int nnz=0;

   ConstraintTatInt c;
   ConstraintTatIntHash::iterator cit;
   VariableTatInt v;
   VariableTatIntHash::iterator vit;   

   std::map<int, int> mapConstraintSomaCredFD;

	vit = vHashTatico.begin();
	for ( ; vit != vHashTatico.end(); vit++ )
	{
		VariableTatInt v = vit->first;
			
		double coef;
				
		if( v.getType() == VariableTatInt::V_SLACK_DEMANDA_ALUNO )			// fd_{d,a}
		{
			coef = - v.getDisciplina()->getTotalCreditos();
		}
		else if( v.getType() == VariableTatInt::V_FOLGA_ALUNO_MIN_ATEND1 || // fmd1_{a}
				 v.getType() == VariableTatInt::V_FOLGA_ALUNO_MIN_ATEND2 ||	// fmd2_{a}
				 v.getType() == VariableTatInt::V_FOLGA_ALUNO_MIN_ATEND3 )	// fmd3_{a}
		{ 
			coef = 1.0;
		}
		else continue;

		Aluno *aluno = v.getAluno();
		
		if ( !aluno->hasCampusId(campusId) )
		{
			continue;
		}
				
		c.reset();
		c.setType( ConstraintTatInt::C_ALOC_MIN_ALUNO );
		c.setAluno( aluno );
		
		sprintf( name, "%s", c.toString(etapa).c_str() ); 

		cit = cHashTatico.find( c ); 
		if ( cit != cHashTatico.end() )
		{
			lp->chgCoef( cit->second, vit->second, coef );
			
			if( v.getType() == VariableTatInt::V_SLACK_DEMANDA_ALUNO ) // fd_{d,a}
			{
				int nCreds = v.getDisciplina()->getTotalCreditos();				
				std::map<int, int>::iterator itMap = mapConstraintSomaCredFD.find( cit->second );
				if ( itMap == mapConstraintSomaCredFD.end() )
					mapConstraintSomaCredFD[ cit->second ] = nCreds;
				else
					itMap->second += nCreds;
			}
		}
		else
		{
			double init_rhs = problemData->parametros->minAtendPercPorAluno * aluno->getNroCredsOrigRequeridosP1();
		
			double jaAtendido = aluno->getNroCreditosJaAlocados();	

			init_rhs -= jaAtendido; // subtraio o já atendido pq não há folga de demanda para atendimentos já fixados pelas etapas anteriores

			nnz += aluno->demandas.size() + 1;
			OPT_ROW row( nnz, OPT_ROW::GREATER, init_rhs , name );
		
			row.insert( vit->second, coef );
			cHashTatico[ c ] = lp->getNumRows();
			
			if( v.getType() == VariableTatInt::V_SLACK_DEMANDA_ALUNO ) // fd_{d,a}
			{
				int nCreds = v.getDisciplina()->getTotalCreditos();				
				std::map<int, int>::iterator itMap = mapConstraintSomaCredFD.find( lp->getNumRows() );
				if ( itMap == mapConstraintSomaCredFD.end() )
					mapConstraintSomaCredFD[ lp->getNumRows() ] = nCreds;
				else
					itMap->second += nCreds;
			}
			
			lp->addRow( row );
			restricoes++;
		}
	}

	std::map<int, int>::iterator itMap = mapConstraintSomaCredFD.begin();			
	for ( ; itMap != mapConstraintSomaCredFD.end(); itMap++ )
	{		
		int rowId = itMap->first;
		int somaCredsFd = itMap->second;
		double init_rhs = lp->getRHS( rowId );
		double rhs = init_rhs - somaCredsFd;
		lp->chgRHS( rowId, rhs );
	}

	return restricoes;
}

/*
	Minimiza o gap entre capacidade da sala e sua ocupação para cada turma

	sum[s] Cap_{s} * o_{i,d,s,cp} - sum[a] s_{i,d,cp,a} = fos_{i,d,cp}
	
	para cada turma i, disciplina d, campus cp
	
	min sum[i]sum[d]sum[cp] fos{i,d,cp}
*/
int TaticoIntAlunoHor::criaRestricaoFolgaOcupacaoSala( int campusId )
{
    int restricoes = 0;		

	if ( ! problemData->parametros->min_folga_ocupacao_sala )
	{
		return restricoes;
	}

    char name[ 100 ];
    ConstraintTatInt c;
	ConstraintTatIntHash::iterator cit;
    VariableTatInt v;
    VariableTatIntHash::iterator vit;

	vit = vHashTatico.begin();
	for ( ; vit != vHashTatico.end(); vit++ )
	{
		// o_{i,d,s}
		if( vit->first.getType() == VariableTatInt::V_OFERECIMENTO )
		{			
			VariableTatInt v = vit->first;

			Disciplina *disciplina = v.getDisciplina();
			int turma = v.getTurma();
			Campus* campus = problemData->refCampus[ v.getUnidade()->getIdCampus() ];

			c.reset();
			c.setType( ConstraintTatInt::C_FOLGA_OCUPACAO_SALA );
			c.setDisciplina(disciplina);
			c.setTurma(turma);
			c.setCampus( campus );

			double coef = v.getSubCjtSala()->salas.begin()->second->getCapacidade();

			cit = cHashTatico.find(c);
			if(cit != cHashTatico.end())
			{
				lp->chgCoef(cit->second, vit->second, coef);
			}
			else
			{
				int nnz=50;
				sprintf( name, "%s", c.toString(etapa).c_str() ); 
				OPT_ROW row( nnz, OPT_ROW::EQUAL, 0.0, name );

				row.insert(vit->second, coef);

				cHashTatico[ c ] = lp->getNumRows();

				lp->addRow( row );
				restricoes++;
			}
		}

		// s_{i,d,cp,a}
		else if( vit->first.getType() == VariableTatInt::V_ALOCA_ALUNO_TURMA_DISC )
		{			
			VariableTatInt v = vit->first;

			Disciplina *disciplina = v.getDisciplina();
			int turma = v.getTurma();
			Campus* campus = v.getCampus();

			c.reset();
			c.setType( ConstraintTatInt::C_FOLGA_OCUPACAO_SALA );
			c.setDisciplina( disciplina );
			c.setTurma( turma );
			c.setCampus( campus );

			double coef = -1.0;

			cit = cHashTatico.find(c);
			if(cit != cHashTatico.end())
			{
				lp->chgCoef(cit->second, vit->second, coef);
			}
			else
			{
				int nnz=50;
				sprintf( name, "%s", c.toString(etapa).c_str() ); 
				OPT_ROW row( nnz, OPT_ROW::EQUAL, 0.0, name );

				row.insert(vit->second, coef);

				cHashTatico[ c ] = lp->getNumRows();

				lp->addRow( row );
				restricoes++;
			}	
		}

		// fos_{i,d,cp}
		else if( vit->first.getType() == VariableTatInt::V_FOLGA_OCUPA_SALA )
		{			
			VariableTatInt v = vit->first;

			Disciplina *disciplina = v.getDisciplina();
			int turma = v.getTurma();
			Campus* campus = v.getCampus();

			c.reset();
			c.setType( ConstraintTatInt::C_FOLGA_OCUPACAO_SALA );
			c.setDisciplina(disciplina);
			c.setTurma(turma);
			c.setCampus( campus );

			double coef = -1.0;

			cit = cHashTatico.find(c);
			if(cit != cHashTatico.end())
			{
				lp->chgCoef(cit->second, vit->second, coef);
			}
			else
			{
				int nnz=50;
				sprintf( name, "%s", c.toString(etapa).c_str() ); 
				OPT_ROW row( nnz, OPT_ROW::EQUAL, 0.0, name );

				row.insert(vit->second, coef);

				cHashTatico[ c ] = lp->getNumRows();

				lp->addRow( row );
				restricoes++;
			}	
		}
	}
	
	return restricoes;
}

/*
	Para cada periodo p de oferta oft

	sum[u] uu_{u,oft,p}  + fuu_{p} <= 1

*/
int TaticoIntAlunoHor::criaRestricaoLimitaUnidPeriodo( int campusId )
{
   int numRest=0; 
   
   if ( !problemData->parametros->limitar_unidades_por_periodo )
   {
		return numRest;
   }

   int nnz;
   char name[ 100 ];

   VariableTatInt v;
   ConstraintTatInt c;
   VariableTatIntHash::iterator vit;
   ConstraintTatIntHash::iterator cit;
   std::vector<int> idxC;
   std::vector<int> idxR;
   std::vector<double> valC;
   
   nnz = problemData->refCampus[campusId]->unidades.size();

   vit = vHashTatico.begin();
   for ( ; vit != vHashTatico.end(); vit++)
	{		
		VariableTatInt v = vit->first;

		if( v.getType() == VariableTatInt::V_UNID_PERIODO )
		{
			 c.reset();
			 c.setType( ConstraintTatInt::C_UNID_PERIODO );
			 c.setOferta( v.getOferta() );
			 c.setPeriodo( v.getPeriodo() );
			 			 
			 double coef = 1.0;

			 cit = cHashTatico.find(c);
			 if ( cit == cHashTatico.end() )
			 {
				sprintf( name, "%s", c.toString(etapa).c_str() );
				OPT_ROW row( nnz, OPT_ROW::LESS, 1.0, name );

				row.insert( vit->second, coef );

				cHashTatico[ c ] = lp->getNumRows();
				lp->addRow( row );
				numRest++;
			 }
			 else
			 {
				idxC.push_back(vit->second);
				idxR.push_back(cit->second);
				valC.push_back(coef);
			 }
		}
		else if( v.getType() == VariableTatInt::V_FOLGA_UNID_PERIODO )
		{
			 c.reset();
			 c.setType( ConstraintTatInt::C_UNID_PERIODO );
			 c.setOferta( v.getOferta() );
			 c.setPeriodo( v.getPeriodo() );
			 			 
			 double coef = 1.0;

			 cit = cHashTatico.find(c);
			 if ( cit == cHashTatico.end() )
			 {
				sprintf( name, "%s", c.toString(etapa).c_str() );
				OPT_ROW row( nnz, OPT_ROW::LESS, 1.0, name );

				row.insert( vit->second, coef );

				cHashTatico[ c ] = lp->getNumRows();
				lp->addRow( row );
				numRest++;
			 }
			 else
			 {
				idxC.push_back(vit->second);
				idxR.push_back(cit->second);
				valC.push_back(coef);
			 }
		}
   }
   
   lp->updateLP();
   lp->chgCoefList((int)(idxC.size()),idxR.data(),idxC.data(),valC.data());
   lp->updateLP();

	return numRest;
}

/*
	Para cada periodo p de oferta oft, e para cada unidade u

	sum[a]sum[i]sum[d]sum[s]sum[t]sum[hi]sum[hf] v_{a,i,d,s,u,t,hi,hf} <= bigM * uu_{u,oft,p}

	a = aluno cujo AlunoDemanda (a,d) esteja associado ao período p da oferta oft

*/
int TaticoIntAlunoHor::criaRestricaoUsaUnidPeriodo( int campusId )
{
   int numRest=0; 
   
   if ( !problemData->parametros->limitar_unidades_por_periodo )
   {
		return numRest;
   }

   int nnz;
   char name[ 100 ];

   VariableTatInt v;
   ConstraintTatInt c;
   VariableTatIntHash::iterator vit;
   ConstraintTatIntHash::iterator cit;
   std::vector<int> idxC;
   std::vector<int> idxR;
   std::vector<double> valC;
   
   nnz = problemData->refCampus[campusId]->unidades.size();

   vit = vHashTatico.begin();
   for ( ; vit != vHashTatico.end(); vit++)
	{		
		VariableTatInt v = vit->first;

		if( v.getType() == VariableTatInt::V_UNID_PERIODO )
		{
			 c.reset();
			 c.setType( ConstraintTatInt::C_USA_UNID_PERIODO );
			 c.setOferta( v.getOferta() );
			 c.setPeriodo( v.getPeriodo() );
			 c.setUnidade( v.getUnidade() );
			 			 
			 double coef = -100000;

			 cit = cHashTatico.find(c);
			 if ( cit == cHashTatico.end() )
			 {
				sprintf( name, "%s", c.toString(etapa).c_str() );
				OPT_ROW row( nnz, OPT_ROW::LESS, 0.0, name );

				row.insert( vit->second, coef );

				cHashTatico[ c ] = lp->getNumRows();
				lp->addRow( row );
				numRest++;
			 }
			 else
			 {
				idxC.push_back(vit->second);
				idxR.push_back(cit->second);
				valC.push_back(coef);
			 }
		}
		else if ( v.getType() == VariableTatInt::V_ALUNO_CREDITOS )
		{	
			Disciplina *disciplina = v.getDisciplina();

			// Não restringe unidade única em caso de disciplina prática
			if ( disciplina->getId() < 0 )
				continue;

			AlunoDemanda *ad = v.getAluno()->getAlunoDemandaEquiv( disciplina );
			Oferta *oferta = ad->getOferta();

			int periodo;
			if ( problemData->parametros->considerar_equivalencia_por_aluno )
				periodo = oferta->curriculo->getPeriodoEquiv( disciplina );
			else
				periodo = oferta->curriculo->getPeriodo( disciplina );

			if ( periodo < 0 )
			{
				std::cout<<"\nErro em TaticoIntAlunoHor::criaRestricaoUsaUnidPeriodo(), periodo de disciplina " 
					<< disciplina->getId() << " nao encontrado na oferta " << oferta->getId();		
			}
			
			c.reset();
			c.setType( ConstraintTatInt::C_USA_UNID_PERIODO );
			c.setOferta( oferta );
			c.setPeriodo( periodo );
			c.setUnidade( v.getUnidade() );
			 
			double coef = 1.0;

			cit = cHashTatico.find(c);
			if ( cit == cHashTatico.end() )
			{
				sprintf( name, "%s", c.toString(etapa).c_str() );
				OPT_ROW row( nnz, OPT_ROW::LESS, 0.0, name );

				row.insert( vit->second, coef );

				cHashTatico[ c ] = lp->getNumRows();
				lp->addRow( row );
				numRest++;
			}
			else
			{
				idxC.push_back(vit->second);
				idxR.push_back(cit->second);
				valC.push_back(coef);
			}
		}
   }
   
   lp->updateLP();
   lp->chgCoefList((int)(idxC.size()),idxR.data(),idxC.data(),valC.data());
   lp->updateLP();

	return numRest;
}



/*
	Para cada periodo p de oferta oft

	sum[u] us_{u,oft,p}  + fus_{p} <= 1

*/
int TaticoIntAlunoHor::criaRestricaoLimitaSalaPeriodo( int campusId )
{
   int numRest=0; 
   
   if ( !problemData->parametros->limitar_salas_por_periodo )
   {
		return numRest;
   }

   int nnz;
   char name[ 100 ];

   VariableTatInt v;
   ConstraintTatInt c;
   VariableTatIntHash::iterator vit;
   ConstraintTatIntHash::iterator cit;
   std::vector<int> idxC;
   std::vector<int> idxR;
   std::vector<double> valC;
   
   nnz = problemData->refCampus[campusId]->getTotalSalas();

   vit = vHashTatico.begin();
   for ( ; vit != vHashTatico.end(); vit++)
	{		
		VariableTatInt v = vit->first;

		if( v.getType() == VariableTatInt::V_SALA_PERIODO )
		{
			 c.reset();
			 c.setType( ConstraintTatInt::C_SALA_PERIODO );
			 c.setOferta( v.getOferta() );
			 c.setPeriodo( v.getPeriodo() );
			 			 
			 double coef = 1.0;

			 cit = cHashTatico.find(c);
			 if ( cit == cHashTatico.end() )
			 {
				sprintf( name, "%s", c.toString(etapa).c_str() );
				OPT_ROW row( nnz, OPT_ROW::LESS, 1.0, name );

				row.insert( vit->second, coef );

				cHashTatico[ c ] = lp->getNumRows();
				lp->addRow( row );
				numRest++;
			 }
			 else
			 {
				idxC.push_back(vit->second);
				idxR.push_back(cit->second);
				valC.push_back(coef);
			 }
		}
		else if( v.getType() == VariableTatInt::V_FOLGA_SALA_PERIODO )
		{
			 c.reset();
			 c.setType( ConstraintTatInt::C_SALA_PERIODO );
			 c.setOferta( v.getOferta() );
			 c.setPeriodo( v.getPeriodo() );
			 			 
			 double coef = 1.0;

			 cit = cHashTatico.find(c);
			 if ( cit == cHashTatico.end() )
			 {
				sprintf( name, "%s", c.toString(etapa).c_str() );
				OPT_ROW row( nnz, OPT_ROW::LESS, 1.0, name );

				row.insert( vit->second, coef );

				cHashTatico[ c ] = lp->getNumRows();
				lp->addRow( row );
				numRest++;
			 }
			 else
			 {
				idxC.push_back(vit->second);
				idxR.push_back(cit->second);
				valC.push_back(coef);
			 }
		}
   }
   
   lp->updateLP();
   lp->chgCoefList((int)(idxC.size()),idxR.data(),idxC.data(),valC.data());
   lp->updateLP();

	return numRest;
}

/*
	Para cada periodo p de oferta oft, e para cada sala s

	sum[a]sum[i]sum[d]sum[u]sum[t]sum[hi]sum[hf] v_{a,i,d,s,u,t,hi,hf} <= bigM * us_{s,oft,p}

	a = aluno cujo AlunoDemanda (a,d) esteja associado ao período p da oferta oft

*/
int TaticoIntAlunoHor::criaRestricaoUsaSalaPeriodo( int campusId )
{
   int numRest=0; 
   
   if ( !problemData->parametros->limitar_salas_por_periodo )
   {
		return numRest;
   }

   int nnz;
   char name[ 100 ];

   VariableTatInt v;
   ConstraintTatInt c;
   VariableTatIntHash::iterator vit;
   ConstraintTatIntHash::iterator cit;
   std::vector<int> idxC;
   std::vector<int> idxR;
   std::vector<double> valC;
   
   nnz = problemData->refCampus[campusId]->getTotalSalas();

   vit = vHashTatico.begin();
   for ( ; vit != vHashTatico.end(); vit++)
	{		
		VariableTatInt v = vit->first;

		if( v.getType() == VariableTatInt::V_SALA_PERIODO )
		{
			 c.reset();
			 c.setType( ConstraintTatInt::C_USA_SALA_PERIODO );
			 c.setOferta( v.getOferta() );
			 c.setPeriodo( v.getPeriodo() );
			 c.setSubCjtSala( v.getSubCjtSala() );
			 			 
			 double coef = -100000;

			 cit = cHashTatico.find(c);
			 if ( cit == cHashTatico.end() )
			 {
				sprintf( name, "%s", c.toString(etapa).c_str() );
				OPT_ROW row( nnz, OPT_ROW::LESS, 0.0, name );

				row.insert( vit->second, coef );

				cHashTatico[ c ] = lp->getNumRows();
				lp->addRow( row );
				numRest++;
			 }
			 else
			 {
				idxC.push_back(vit->second);
				idxR.push_back(cit->second);
				valC.push_back(coef);
			 }
		}
		else if ( v.getType() == VariableTatInt::V_ALUNO_CREDITOS )
		{	
			Disciplina *disciplina = v.getDisciplina();

			// Não restringe unidade única em caso de disciplina prática
			if ( disciplina->getId() < 0 )
				continue;

			AlunoDemanda *ad = v.getAluno()->getAlunoDemandaEquiv( disciplina );
			Oferta *oferta = ad->getOferta();

			int periodo;
			if ( problemData->parametros->considerar_equivalencia_por_aluno )
				periodo = oferta->curriculo->getPeriodoEquiv( disciplina );
			else
				periodo = oferta->curriculo->getPeriodo( disciplina );

			if ( periodo < 0 )
			{
				std::cout<<"\nErro em TaticoIntAlunoHor::criaRestricaoUsaSalaPeriodo(), periodo de disciplina " 
					<< disciplina->getId() << " nao encontrado na oferta " << oferta->getId();		
			}
			
			c.reset();
			c.setType( ConstraintTatInt::C_USA_SALA_PERIODO );
			c.setOferta( oferta );
			c.setPeriodo( periodo );
			c.setSubCjtSala( v.getSubCjtSala() );
			 
			double coef = 1.0;

			cit = cHashTatico.find(c);
			if ( cit == cHashTatico.end() )
			{
				sprintf( name, "%s", c.toString(etapa).c_str() );
				OPT_ROW row( nnz, OPT_ROW::LESS, 0.0, name );

				row.insert( vit->second, coef );

				cHashTatico[ c ] = lp->getNumRows();
				lp->addRow( row );
				numRest++;
			}
			else
			{
				idxC.push_back(vit->second);
				idxR.push_back(cit->second);
				valC.push_back(coef);
			}
		}
   }
   
   lp->updateLP();
   lp->chgCoefList((int)(idxC.size()),idxR.data(),idxC.data(),valC.data());
   lp->updateLP();

	return numRest;
}


/*
	Garante o tempo de descanso mínimo entre o ultimo horario de aula que o professor
	ministra no dia t e o primeiro horario de aula que este ministra no dia t+1

	Necessário no tático caso o professor das aulas de mesma turma tenha que ser o mesmo.

	Para cada par de aulas de uma mesma turma em dias consecutivos com [ (hf,t) e (hi,t+1) ] 
	tal que o tempo entre (hf,t) e (hi,t+1) é menor do que o tempo mínimo de descanso:

	x_{a1,hf} +  x_{a2,hi} <= 1

	com a1 E At,  a2 E At+1

*/
int TaticoIntAlunoHor::criaRestricaoProfDescansoMinimo( int campusId )
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

   VariableTatInt v;
   ConstraintTatInt c;
   VariableTatIntHash::iterator vit;
   ConstraintTatIntHash::iterator cit;
   
   // Aulas organizadas em relação aos DateTimes de INÍCIO da aula
   std::map<Disciplina*, std::map< int /*turma*/,  std::map< int /*dia*/, 
	   std::map<DateTime*, GGroup<int> > > >, LessPtr<Disciplina> > mapDiscTurmaDiaHiVarId;

   // Aulas organizadas em relação aos DateTimes de TÉRMIDO da aula
   std::map<Disciplina*, std::map< int /*turma*/,  std::map< int /*dia*/, 
	   std::map<DateTime*, GGroup<int> > > >, LessPtr<Disciplina> > mapDiscTurmaDiaHfVarId;

   std::cout<<"\nCriando map de vars..."; fflush(NULL);

	vit = vHashTatico.begin();
	for ( ; vit != vHashTatico.end(); vit++ )
	{
		if ( vit->first.getType() == VariableTatInt::V_CREDITOS )
		{	
			VariableTatInt v = vit->first;
			
			if ( v.getUnidade()->getIdCampus() != campusId ) continue;

			HorarioAula *hi = v.getHorarioAulaInicial();
			HorarioAula *hf = v.getHorarioAulaFinal();
			DateTime *dti = problemData->horarioAulaDateTime[ hi->getId() ].first;
			DateTime *dtf = problemData->getDateTimeFinal( hf->getFinal() );
			
			mapDiscTurmaDiaHiVarId[ v.getDisciplina() ][ v.getTurma() ][ v.getDia() ][ dti ].add( vit->second );			
			mapDiscTurmaDiaHfVarId[ v.getDisciplina() ][ v.getTurma() ][ v.getDia() ][ dtf ].add( vit->second );	
		}
	}

	std::cout<<"\nCriando restricoes..."; fflush(NULL);

   std::map<Disciplina*, std::map< int /*turma*/,  std::map< int /*dia*/, 
	   std::map<DateTime*, GGroup<int> > > >, LessPtr<Disciplina> >::iterator
	   itDiscTurmaDiaDtf = mapDiscTurmaDiaHfVarId.begin();
	for ( ; itDiscTurmaDiaDtf != mapDiscTurmaDiaHfVarId.end(); itDiscTurmaDiaDtf++ )
	{		
		Disciplina *disciplina = itDiscTurmaDiaDtf->first;

		std::map< int /*turma*/,  std::map< int /*dia*/, 
			std::map<DateTime*, GGroup<int> > > > *mapTurmaDiaDtf = & itDiscTurmaDiaDtf->second;

	    std::map< int /*turma*/,  std::map< int /*dia*/, std::map<DateTime*, GGroup<int> > > >::iterator
		   itTurmaDiaDtf = mapTurmaDiaDtf->begin();
		for ( ; itTurmaDiaDtf != mapTurmaDiaDtf->end(); itTurmaDiaDtf++ )
		{		
			int turma = itTurmaDiaDtf->first;
			
			std::map< int /*dia*/, std::map<DateTime*, GGroup<int> > >::iterator
				itDiaDtf = itTurmaDiaDtf->second.begin();	
			for ( ; itDiaDtf != itTurmaDiaDtf->second.end(); itDiaDtf++ )
			{		
				int dia = itDiaDtf->first;

				//std::cout<<"\nDiscTurmaDia " << disciplina->getId() <<" " << turma <<" " << dia; fflush(NULL);

				// Verifica se há aulas iniciando no dia seguinte
				std::map< int /*dia*/, std::map<DateTime*, GGroup<int> > >::iterator
					itDiaSeguinte = mapDiscTurmaDiaHiVarId[disciplina][turma].find( dia+1 );	
				if ( itDiaSeguinte == mapDiscTurmaDiaHiVarId[disciplina][turma].end() )
				{
					continue;
				}
				std::map<DateTime*, GGroup<int> > mapDiaSeguinteDtiVar = itDiaSeguinte->second;


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

					GGroup<DateTime*, LessPtr<DateTime> > dtIncompat = 
						problemData->getDtiAnteriores( dtf_maisDescanso );

					ITERA_GGROUP_LESSPTR( itDt, dtIncompat, DateTime )
					{
						DateTime *dti = *itDt;
					
						// Procura aulas no dia seguinte da turma que comecem em dti					
						std::map<DateTime*, GGroup<int>>::iterator itDtiDiaSeg = mapDiaSeguinteDtiVar.find( dti );
						if ( itDtiDiaSeg == mapDiaSeguinteDtiVar.end() )
						{
							continue;
						}
					
						GGroup<int> varsX_diaSeguinte = itDtiDiaSeg->second;

						c.reset();
						c.setType( ConstraintTatInt::C_PROF_MIN_DESCANSO );
						c.setDisciplina( disciplina );
						c.setTurma( turma );
						c.setDia( dia );
						c.setDateTimeFinal( dtf );
						c.setDateTimeInicial( dti );
					
						double coef = 1.0;
						
				
						cit = cHashTatico.find( c ); 
						if ( cit == cHashTatico.end() )
						{
							nnz = varsX_diaT.size() + varsX_diaSeguinte.size();
						
							sprintf( name, "%s", c.toString(etapa).c_str() );
							OPT_ROW row( 100, OPT_ROW::LESS, 1.0, name );
						
							ITERA_GGROUP_N_PT( itVar, varsX_diaT, int )
							{
								row.insert( *itVar, coef );
							}
							ITERA_GGROUP_N_PT( itVar, varsX_diaSeguinte, int )
							{
								row.insert( *itVar, coef );
							}
						
							cHashTatico[ c ] = lp->getNumRows();
			
							lp->addRow( row );
							restricoes++;
						}
						else
						{
							ITERA_GGROUP_N_PT( itVar, varsX_diaT, int )
							{
								lp->chgCoef( cit->second, *itVar, coef );
							}
							ITERA_GGROUP_N_PT( itVar, varsX_diaSeguinte, int )
							{
								lp->chgCoef( cit->second, *itVar, coef );
							}
						
						}
				
					}

				}
			}		
		}
	}

   return restricoes;
}









/***************************************************************************************************************************
 **************************************************************************************************************************

											MODELO PARA TROCA DE SALAS

 **************************************************************************************************************************
****************************************************************************************************************************/


void TaticoIntAlunoHor::solveTaticoIntegradoBranchSalas( int campusId, int prioridade, int r )
{
	std::cout<<"\n---------------------------  Branch Salas  -----------------------\n";
    std::cout<<"\n------------------------ Rodada "<< r <<" ------------------------\n";
	std::cout<<"\n------- Campus "<< campusId << ", Prior " << prioridade << "----------\n";
	std::cout<<"\n------------------------------Tatico Integrado -----------------------------\n"; fflush(NULL);

	std::cout<<"\nIniciando tatico integrado...\n"; fflush(NULL);
	nroSalasUsadas();

	std::cout<<"\nSolving...\n"; fflush(NULL);
	solveBranchSala( campusId, prioridade, r );

	std::cout<<"\nCarregando solucao tatica integrada...\n"; fflush(NULL);
	carregaSolucaoBranchSala( campusId, prioridade, r );
	
	std::cout<<"\nImprimindo alocacoes...\n"; fflush(NULL);

	std::cout<<"\nImprime utilizacao das salas...\n"; fflush(NULL);
	problemData->imprimeUtilizacaoSala( campusId, prioridade, 0, false, r, toString(TAT_INT_BRANCH_SALA) );
			
	std::cout<<"\nSincronizando as solucoes...\n"; fflush(NULL);
	sincronizaSolucaoBranchSala( campusId, prioridade, r );
	
	nroSalasUsadas();

	std::cout<<"\nFim do tatico integrado!\n"; fflush(NULL);

	return; 
}

void TaticoIntAlunoHor::nroSalasUsadas()
{	
	GGroup< ConjuntoSala*, LessPtr<ConjuntoSala> > cjsSalas;
	GGroup< VariableTatico *, LessPtr<VariableTatico> > solTaticoVarsX = *vars_xh;
	ITERA_GGROUP_LESSPTR( itVarX, solTaticoVarsX, VariableTatico )
	{
		VariableTatico x = **itVarX;
		
		cjsSalas.add( x.getSubCjtSala() );
	}
	std::cout<<"\nTotal de salas usadas atualmente = " << cjsSalas.size() << std::endl; fflush(NULL);
}

int TaticoIntAlunoHor::solveBranchSala( int campusId, int prioridade, int r )
{	

	int status = 0;
		
	bool CARREGA_SOL_PARCIAL = * this->CARREGA_SOLUCAO;

   if ( (*this->CARREGA_SOLUCAO) )
   {
	   char solName[1024];
	   strcpy( solName, getSolucaoTaticoFileName( campusId, prioridade, r, 0 ).c_str() );
	   //strcpy( solName, getSolBinFileName( campusId, prioridade, r ).c_str() );
	   FILE* fin = fopen( solName,"rb");
	   if ( fin == NULL )
	   {
		   std::cout << "\nA partir de " << solName << " , nao foram lidas mais solucoes.\n"; fflush(NULL);
		   *CARREGA_SOLUCAO = false;
	   }
	   else
	   {
		  fclose(fin);
	   }
   }

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

   if ( vHashTatico.size() > 0 )
   {
		vHashTatico.clear();
   }
   if ( cHashTatico.size() > 0 )
   {
	   cHashTatico.clear();
   }

   char lpName[1024], id[100];
   strcpy( lpName, getTaticoLpFileName( campusId, prioridade, r ).c_str() );

	if ( problemData->parametros->funcao_objetivo == 0
		|| problemData->parametros->funcao_objetivo == 2 )
	{
		lp->createLP( lpName, OPTSENSE_MAXIMIZE, PROB_MIP );
	}
	else if( problemData->parametros->funcao_objetivo == 1 )
	{
		lp->createLP( lpName, OPTSENSE_MINIMIZE, PROB_MIP );
	}

   std::cout<<"\nCreating LP...\n"; fflush(NULL);

// ---------------------------------------------------------------
// Tatico por aluno com horarios:
 
   if ( problemData->parametros->otimizarPor == "ALUNO" )
   {
	    // Variable creation
	    varNum = criaVariaveisBranchSala( campusId, prioridade, r );
		
	    lp->updateLP();

		#ifdef PRINT_cria_variaveis
	    printf( "Total of Variables: %i\n\n", varNum );
		#endif

		if ( ! (*this->CARREGA_SOLUCAO) )
		{
		   // Constraint creation
		   constNum = criaRestricoesBranchSala( campusId );

		   lp->updateLP();

			#ifdef PRINT_cria_restricoes
		   printf( "Total of Constraints: %i\n\n", constNum );
			#endif

			#ifdef PRINT_LOGS
			lp->writeProbLP( lpName );
			#endif
		}
   }
// ---------------------------------------------------------------

   else
   {
		std::cerr<<"\nErro: Parametro otimizarPor deve ser ALUNO!\n"; fflush(NULL);
		exit(0);
   }  

   if ( ! (*this->CARREGA_SOLUCAO) )
   {   

		std::cout << "\n=========================================";
	    std::cout << "\nGarantindo solucao inicial...\n"; fflush(NULL);
	   
		lp->updateLP();
		lp->setTimeLimit( this->getTimeLimit(Solver::TAT_INT1) );
		lp->setPreSolve(OPT_TRUE);
		lp->setHeurFrequency(1.0);
		lp->setMIPScreenLog( 4 );
		lp->setMIPEmphasis(0);
		lp->setSymetry(0);
		lp->setCuts(3);
		lp->setNumIntSols(1);	
		lp->updateLP();

		std::string lpName1;
		lpName1 += "1_";
		lpName1 += string(lpName);
		
		#ifdef PRINT_LOGS
		lp->writeProbLP( lpName1.c_str() );
		#endif

	    double *xS = new double[lp->getNumCols()];

		// GENERATES INITIAL SOLUTION 

		status = lp->optimize( METHOD_MIP );
		std::cout<<"\nStatus TAT_INT_BIN1 = "<<status; fflush(NULL);
		lp->getX(xS);
	  	
		writeSolBin( campusId, prioridade, r, OutPutFileType::TAT_INT_BIN1, xS );
		writeSolTxt( campusId, prioridade, r, OutPutFileType::TAT_INT_BIN1, xS, 0 );
		

	    // -------------------------------------------------------------------
	    
		std::cout << "\n=========================================";
	    std::cout << "\nTentando troca de salas...\n"; fflush(NULL);
			
		int *idxN = new int[lp->getNumCols()];
		int *idxs = new int[lp->getNumCols()*2];
		double *vals = new double[lp->getNumCols()*2];
		BOUNDTYPE *bds = new BOUNDTYPE[lp->getNumCols()*2];
		int nBds = 0;

		VariableTatIntHash::iterator vit = vHashTatico.begin();
		for ( ; vit != vHashTatico.end(); vit++ )
		{
			VariableTatInt v = vit->first;
	   
			idxN[vit->second] = vit->second;		

			int lb = (int)(lp->getLB(vit->second) + 0.5);
			int ub = (int)(lp->getUB(vit->second) + 0.5);

			if ( lb == ub ) // desfixa
			{
				idxs[nBds] = vit->second;
				vals[nBds] = 0.0;
				bds[nBds] = BOUNDTYPE::BOUND_LOWER;
				nBds++;
			}
		}
		lp->chgBds(nBds,idxs,bds,vals);

		lp->updateLP();
				
#if defined SOLVER_CPLEX
		lp->setNumIntSols(100000000);
		lp->setTimeLimit( this->getTimeLimit(Solver::TAT_INT) );
		lp->setMemoryEmphasis(true);
		lp->setPreSolve(OPT_TRUE);
		lp->setHeurFrequency(1.0);
		lp->setMIPScreenLog( 4 );
		lp->setMIPEmphasis(4);
		lp->setNodeLimit(100000000);
		lp->setPolishAfterTime(1200);
		lp->setPolishAfterNode(1);
		lp->setSymetry(0);
		lp->setProbe(-1);
		lp->setCuts(0);
		lp->updateLP();
#elif defined SOLVER_GUROBI
		lp->setNumIntSols(100000000);
		lp->setTimeLimit( this->getTimeLimit(Solver::TAT_INT) );
		lp->setPreSolve(OPT_TRUE);
		lp->setHeurFrequency(1.0);
		lp->setMIPScreenLog( 4 );
		lp->setMIPEmphasis(4);
		lp->setNodeLimit(100000000);
		lp->setPolishAfterTime(1200);
		lp->setPolishAfterNode(1);
		lp->setSymetry(0);
		lp->setCuts(0);
		lp->updateLP();
#endif

		#ifdef PRINT_LOGS
		lp->writeProbLP( lpName );
		#endif
		
		lp->copyMIPStartSol(lp->getNumCols(),idxN,xS);
		
		lp->updateLP();

		delete[] idxN;
		delete[] idxs;
		delete[] vals;
		delete[] bds;

		// GENERATES SOLUTION

		status = lp->optimize( METHOD_MIP );
		std::cout<<"\nStatus TAT_INT_BIN = "<<status; fflush(NULL);
		lp->getX(xS);
	  
		writeSolBin( campusId, prioridade, r, OutPutFileType::TAT_INT_BIN, xS );
		writeSolTxt( campusId, prioridade, r, OutPutFileType::TAT_INT_BIN, xS, 0 );
			  		
		fflush(NULL);

		delete[] xS;	
						

	    // Imprime Gap
		ofstream outGaps;
		std::string gapFilename( "gap_input" );
		gapFilename += problemData->getInputFileName();
		gapFilename += ".txt";

		outGaps.open(gapFilename, ofstream::app);
		if ( !outGaps )
		{
			std::cerr<<"\nErro: Abertura do arquivo gaps.txt falhou em TaticoIntAlunoHor::solveTaticoIntAlunoHor().\n";
		}
		else
		{
			outGaps << "TaticoIntAlunoHor - campus "<< campusId << ", prioridade " << prioridade;
			outGaps << ", r "<< r;
			outGaps << "\nGap = " << lp->getMIPGap() * 100 << "%";
			outGaps << "\n\n\n";
			outGaps.close();
		}
			
		lp->updateLP();
				
	}

	std::cout<<"\n------------------------------Fim do Tatico Integrado -----------------------------\n"; fflush(NULL);

   return status;
}

void TaticoIntAlunoHor::sincronizaSolucaoBranchSala( int campusAtualId, int prioridade, int r )
{
   // --------------------------------------------------------------------
   // deleta variaveis antigas do tipo VariableTatico::Creditos
   // e insere as novas nas listas vars_xh e solVarsTatico.
   atualizaVariaveisTatico();
   
   // ----------------------------------------------------
   // deleta solVarsTatInt, vars_v e hashs
   solVarsTatInt.deleteElements();
   solVarsTatInt.clear();
   vars_v.clear();
   vHashTatico.clear();
   cHashTatico.clear();
   // ----------------------------------------------------

   return;
}

void TaticoIntAlunoHor::atualizaVariaveisTatico()
{
	VariableTatico vSol;
	
	// --------------------------------------------------------
	// Deleta variaveis x anteriores

	GGroup< VariableTatico *, LessPtr<VariableTatico> > todelete;
	ITERA_GGROUP_LESSPTR( itVar, (*solVarsTatico), VariableTatico )
	{
		if ( itVar->getType() == VariableTatico::V_CREDITOS )
			todelete.add( *itVar );
	}
	ITERA_GGROUP_LESSPTR( itVar, todelete, VariableTatico )
	{
		(*this->solVarsTatico).remove( *itVar );
	}
	todelete.deleteElements();
	vars_xh->clear();

	// --------------------------------------------------------

	ITERA_GGROUP_LESSPTR( itVar, solVarsTatInt, VariableTatInt )
	{
		VariableTatInt *v = *itVar;

		switch( v->getType() )
		{		
			 case VariableTatInt::V_ERROR:
			 {
				break;
			 }		 
			 case VariableTatInt::V_CREDITOS:  //  x_{i,d,u,s,hi,hf,t} 
			 {
				VariableTatico * x = new VariableTatico();
				x->setType( VariableTatico::V_CREDITOS );
				x->setTurma( v->getTurma() );
				x->setDisciplina( v->getDisciplina() );
				x->setDia( v->getDia() );
				x->setHorarioAulaInicial( v->getHorarioAulaInicial() );
				x->setHorarioAulaFinal( v->getHorarioAulaFinal() );
				x->setSubCjtSala( v->getSubCjtSala() );
				x->setUnidade( v->getUnidade() );
				x->setValue( v->getValue() );
				x->setDateTimeInicial( v->getDateTimeInicial() );
				x->setDateTimeFinal( v->getDateTimeFinal() );

				if ( solVarsTatico->find( x ) == solVarsTatico->end() )
				{					
					this->vars_xh->add( x );
					this->solVarsTatico->add( x );
				}
				else
					delete x;
				break;
			 }	 		 
		 				 		
			 default:
			 {
				 break;
			 }
		}
	}
}

void TaticoIntAlunoHor::carregaSolucaoBranchSala( int campusAtualId, int prioridade, int r )
{
   std::cout << "\nCarregando solucao tatico integrado para branch em salas...\n";

   double * xSol = NULL;
      
   int nroColsLP = lp->getNumCols();
   xSol = new double[ nroColsLP ];
	
	if ( *(this->CARREGA_SOLUCAO) )
	{
        int status = readSolTxt(campusAtualId, prioridade, r, OutPutFileType::TAT_INT_BIN, xSol, 0 );
		if ( !status )
		{
		    std::cout << "\nErro em TaticoIntAlunoHor::carregaSolucaoBranchSala(): arquivo "
					" nao encontrado.\n";
		    delete [] xSol;
			exit(0);
		}
	}
	else
	{
		lp->getX( xSol );fflush( NULL );
	}

	char solFilename[1024];
	strcpy( solFilename, getSolucaoTaticoFileName( campusAtualId, prioridade, r, 0 ).c_str() );
	ofstream fout;
	fout.open( solFilename, ios::out );
	if ( !fout )
	{
		std::cout << "\nErro em SolverMIP::carregaSolucaoBranchSala( int campusAtualId, int prioridade, int r )"
					<< "\nArquivo nao pode ser aberto\n";
		fout.open( "solTatIntBranchSala_2.txt", ios::out );
		if ( !fout )
		{
			std::cout <<"\nErro de novo. Finalizando execucao...\n";
			exit(0);
		}
	}
   	   
   // -----------------------------------------------------

   // Deleta todas as variaveis referenciadas em solVarsTatInt e em vars_v
   solVarsTatInt.deleteElements();
   solVarsTatInt.clear();
   vars_v.clear();
   
   // Limpa horarios-dia usados por sala
   ITERA_GGROUP_LESSPTR( itCampus, problemData->campi, Campus )
   {
	  Campus *cp = *itCampus;
	  if ( cp->getId() != campusAtualId ) continue;

      ITERA_GGROUP_LESSPTR( itUnidade, cp->unidades, Unidade )
      {
         ITERA_GGROUP_LESSPTR( itCjtSala, itUnidade->conjutoSalas, ConjuntoSala )
         {
			 Sala * sala = itCjtSala->salas.begin()->second;
			 sala->deleteHorarioDiaOcupados();
		 }
	  }
   }

   // Limpa nro de creditos usados por sala
   problemData->mapCreditosSalas.clear();
   
   // -----------------------------------------------------

   int nSala = 0;
   
   VariableTatIntHash::iterator vit = vHashTatico.begin();
   while ( vit != vHashTatico.end() )
   {
	  VariableTatInt* v = new VariableTatInt( vit->first );
      int col = vit->second;
      v->setValue( xSol[ col ] );

      if ( v->getValue() > 0.00001 )
      {
         char auxName[1024];
         lp->getColName( auxName, col, 1000 );
         fout << auxName << " = " << v->getValue() << std::endl;

		 solVarsTatInt.add( v );
		 		 
		 Disciplina *vDisc;
		 HorarioAula *vhi;
		 HorarioAula *vhf;
		 HorarioAula *ha;
		 Sala *vSala;
		 ConjuntoSala *vCjtSala;
		 int vDia;
		 int vTurma;
		 int vCampusId;
		 int nCreds;
		 GGroup<HorarioDia*> horariosDias;				 

         switch( v->getType() )
         {
			 case VariableTatInt::V_CREDITOS:
				 vDia = v->getDia();
				 vhi = v->getHorarioAulaInicial();
				 vhf = v->getHorarioAulaFinal();
				 vDisc = v->getDisciplina();
				 vTurma = v->getTurma();
				 vCampusId = v->getUnidade()->getIdCampus();	 
				 
				 ha = vhi;
				 nCreds = vhi->getCalendario()->retornaNroCreditosEntreHorarios( vhi, vhf );				 
				 for ( int i = 1; i <= nCreds; i++ )
				 {
					HorarioDia *hd = problemData->getHorarioDiaCorrespondente( ha, vDia );
					horariosDias.add( hd );
					ha = vhi->getCalendario()->getProximoHorario( ha );			
				 }

				 vSala = v->getSubCjtSala()->salas.begin()->second;
				 vSala->addHorarioDiaOcupado( horariosDias );
				 break;
			 case VariableTatInt::V_OFERECIMENTO:			
				 vDisc = v->getDisciplina();
				 vCjtSala = v->getSubCjtSala();
				 if ( problemData->mapCreditosSalas.find(vCjtSala) == problemData->mapCreditosSalas.end() )
					  problemData->mapCreditosSalas[vCjtSala] = vDisc->getTotalTempo();
				 else problemData->mapCreditosSalas[vCjtSala] += vDisc->getTotalTempo();
				 break;
			 case VariableTatInt::V_SALA:
				 nSala++;
				 break;
         }
      }
	  else
		  delete v;
	 
      vit++;
   }
   	
	fout << "\nNumero de salas usadas = " << nSala;
	
    fout.close();
    
    if ( xSol )
    {
       delete [] xSol;
    }   

   std::cout << "\n\nSolucao tatico integrado branch salas carregada com sucesso!\n";
   std::cout << "\n-----------------------------------------------------------------\n";
   std::cout << "-----------------------------------------------------------------\n\n";

    return;
}



// Variaveis

int TaticoIntAlunoHor::criaVariaveisBranchSala( int campusId, int P, int r )
{
	int num_vars = 0;
	CPUTimer timer;
	double dif = 0.0;

#ifdef PRINT_cria_variaveis
	int numVarsAnterior = 0;
#endif

	timer.start();
	std::cout << "Criando \"x\": ";fflush(NULL);
	num_vars += this->criaVariavelCreditosCopiadasSalaLivre( campusId, P );				// x
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_variaveis
	std::cout << "numVars \"x\": " << (num_vars - numVarsAnterior)  <<"\t "<<dif <<" sec" << std::endl; fflush(NULL);
	numVarsAnterior = num_vars;
#endif
	

	timer.start();
	std::cout << "Criando \"o\": ";fflush(NULL);
	num_vars += this->criaVariavelOferecimento( campusId, P );							// o
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_variaveis
	std::cout << "numVars \"o\": " << (num_vars - numVarsAnterior)  <<"\t "<<dif <<" sec" << std::endl; fflush(NULL);
	numVarsAnterior = num_vars;
#endif


	timer.start();
	std::cout << "Criando \"u\": ";fflush(NULL);
	num_vars += this->criaVariavelSalaUsada( campusId, P );								// u
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_variaveis
	std::cout << "numVars \"u\": " << (num_vars - numVarsAnterior)  <<"\t "<<dif <<" sec" << std::endl; fflush(NULL);
	numVarsAnterior = num_vars;
#endif

	return num_vars;

}


/*	
	x_{i,d,u,s,hi,hf,t}
*/
int TaticoIntAlunoHor::criaVariavelCreditosCopiadasSalaLivre( int campusId, int P )
{
	int numVars = 0;

	GGroup< VariableTatico *, LessPtr<VariableTatico> > solTaticoVarsX = *vars_xh;
	ITERA_GGROUP_LESSPTR( itVarX, solTaticoVarsX, VariableTatico )
	{
		VariableTatico x = **itVarX;

		VariableTatInt v;
		v.reset();
		v.setType( VariableTatInt::V_CREDITOS );

		v.setTurma( x.getTurma() );								// i
		v.setDisciplina( x.getDisciplina() );					// d
		v.setDia( x.getDia() );									// t
		v.setHorarioAulaInicial( x.getHorarioAulaInicial() );	// hi
		v.setHorarioAulaFinal( x.getHorarioAulaFinal() );		// hf
			
        std::pair<DateTime*,int> auxP = problemData->horarioAulaDateTime[x.getHorarioAulaInicial()->getId()];
        v.setDateTimeInicial(auxP.first);
        auxP = problemData->horarioAulaDateTime[x.getHorarioAulaFinal()->getId()];
        v.setDateTimeFinal(auxP.first);

		std::map< int, ConjuntoSala* >::iterator 
			itMap = x.getDisciplina()->cjtSalasAssociados.begin();
		for ( ; itMap != x.getDisciplina()->cjtSalasAssociados.end(); itMap++ )
		{
			ConjuntoSala* cjtSala = itMap->second;
			Sala * sala = cjtSala->salas.begin()->second;
			int unidId = sala->getIdUnidade();
			Unidade *unidade = problemData->refUnidade[unidId];
				
			// -------------------------------------------
			// Verifica se a sala possui o horario da aula
			bool valid = true;	
			if ( sala->horariosDiaMap.find( x.getDia() ) != sala->horariosDiaMap.end() )
			{
				HorarioAula *h = x.getHorarioAulaInicial();
				int nCreds = h->getCalendario()->retornaNroCreditosEntreHorarios( x.getHorarioAulaInicial(), x.getHorarioAulaFinal() );
				GGroup< HorarioDia * > horsDia = sala->horariosDiaMap[ x.getDia() ];

				for ( int i = 1; i <= nCreds && valid; i++ )
				{
					HorarioDia * hd = problemData->getHorarioDiaCorrespondente( h, x.getDia() );					
					if ( horsDia.find( hd ) == horsDia.end() )
						valid=false;
					else
						h = h->getCalendario()->getProximoHorario( h );

					if ( h == NULL && i!=nCreds ){ std::cout<<"\nErro! HorarioAula nao encontrado."; valid = false; }
				}
			}
			else valid=false;

			if ( !valid ) continue;
			// -------------------------------------------


			v.setUnidade( unidade );							// u
			v.setSubCjtSala( cjtSala );							// tps

			if ( vHashTatico.find( v ) == vHashTatico.end() )
			{
				vHashTatico[ v ] = lp->getNumCols();
									
				double lowerBound = 0.0;
				double upperBound = 1.0;
		
				if ( cjtSala == x.getSubCjtSala() ) // Fixa somente para setar solução inicial
					lowerBound = 1.0;

				OPT_COL col( OPT_COL::VAR_BINARY, 0.0, lowerBound, upperBound,
					( char * )v.toString().c_str());

				lp->newCol( col );
				numVars++;
			}	
		}	
	}

	return numVars;
}

/*	
	o_{i,d,u,s}
*/
int TaticoIntAlunoHor::criaVariavelOferecimento( int campusId, int P )
{
	int numVars = 0;

	GGroup< VariableTatico *, LessPtr<VariableTatico> > solTaticoVarsX = *vars_xh;
	ITERA_GGROUP_LESSPTR( itVarX, solTaticoVarsX, VariableTatico )
	{
		VariableTatico x = **itVarX;

		VariableTatInt v;
		v.reset();
		v.setType( VariableTatInt::V_OFERECIMENTO );
		v.setTurma( x.getTurma() );								// i
		v.setDisciplina( x.getDisciplina() );					// d
			
		std::map< int, ConjuntoSala* >::iterator 
			itMap = x.getDisciplina()->cjtSalasAssociados.begin();
		for ( ; itMap != x.getDisciplina()->cjtSalasAssociados.end(); itMap++ )
		{
			ConjuntoSala* cjtSala = itMap->second;
			Sala * sala = cjtSala->salas.begin()->second;
			int unidId = sala->getIdUnidade();
			Unidade *unidade = problemData->refUnidade[unidId];
				
			// -------------------------------------------
			// Verifica se a sala possui o horario da aula
			bool valid = true;	
			if ( sala->horariosDiaMap.find( x.getDia() ) != sala->horariosDiaMap.end() )
			{
				HorarioAula *h = x.getHorarioAulaInicial();
				int nCreds = h->getCalendario()->retornaNroCreditosEntreHorarios( x.getHorarioAulaInicial(), x.getHorarioAulaFinal() );
				GGroup< HorarioDia * > horsDia = sala->horariosDiaMap[ x.getDia() ];

				for ( int i = 1; i <= nCreds && valid; i++ )
				{
					HorarioDia * hd = problemData->getHorarioDiaCorrespondente( h, x.getDia() );					
					if ( horsDia.find( hd ) == horsDia.end() )
						valid=false;
					else
						h = h->getCalendario()->getProximoHorario( h );

					if ( h == NULL && i!=nCreds ){ std::cout<<"\nErro! HorarioAula nao encontrado."; valid = false; }
				}
			}
			else valid=false;

			if ( !valid ) continue;
			// -------------------------------------------


			v.setUnidade( unidade );							// u
			v.setSubCjtSala( cjtSala );							// tps

			if ( vHashTatico.find( v ) == vHashTatico.end() )
			{
				vHashTatico[ v ] = lp->getNumCols();
									
				double lowerBound = 0.0;
				double upperBound = 1.0;
		
				if ( cjtSala == x.getSubCjtSala() ) // Fixa somente para setar solução inicial
					lowerBound = 1.0;

				OPT_COL col( OPT_COL::VAR_BINARY, 0.0, lowerBound, upperBound,
					( char * )v.toString().c_str());

				lp->newCol( col );
				numVars++;
			}	
		}	
	}

	return numVars;
}

/*	
	u_{s}
*/
int TaticoIntAlunoHor::criaVariavelSalaUsada( int campusId, int P )
{
	int numVars = 0;

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
				VariableTatInt v;
				v.reset();
				v.setType( VariableTatInt::V_SALA );
				v.setSubCjtSala( *itCjtSala );						// tps
		
				if ( vHashTatico.find( v ) == vHashTatico.end() )
				{
					vHashTatico[ v ] = lp->getNumCols();
									
					double lowerBound = 0.0;
					double upperBound = 1.0;
		
					OPT_COL col( OPT_COL::VAR_BINARY, 1.0, lowerBound, upperBound,
						( char * )v.toString().c_str());

					lp->newCol( col );
					numVars++;
				}
			}
		}	
	}

	return numVars;
}



   
   
// Restrições

int TaticoIntAlunoHor::criaRestricoesBranchSala( int campusId )
{
	int restricoes = 0;

	CPUTimer timer;
	double dif = 0.0;

#ifdef PRINT_cria_restricoes
	int numRestAnterior = 0;
#endif
		
	timer.start();
	restricoes += criaRestricaoTotalAtend( campusId );				
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest \"C_ATEND_TOTAL\": " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
#endif

	timer.start();
	restricoes += criaRestricaoSalaUnica( campusId );		
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest \"C_SALA_UNICA\": " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
#endif

	timer.start();
	restricoes += criaRestricaoSalaHorario( campusId );		
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest \"C_SALA_HORARIO\": " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
#endif
	
	timer.start();
	restricoes += criaRestricaoCapacidadeSala( campusId );		
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest \"C_CAPACIDADE_SALA\": " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
#endif

	timer.start();
	restricoes += criaRestricaoSalaUsada( campusId );			
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest \"C_SALA_USADA\": " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
#endif
	

	return restricoes;
}


int TaticoIntAlunoHor::criaRestricaoCapacidadeSala( int campusId )
{
	int restricoes = 0;
	int nnz;
	char name[ 1024 ];

	VariableTatInt v;
	ConstraintTatInt c;

	VariableTatIntHash::iterator vit;
	ConstraintTatIntHash::iterator cit;
		
    std::vector<int> idxC;
    std::vector<int> idxR;
    std::vector<double> valC;

	std::map< int, std::map< int, std::map< int, int > > > mapO;

	vit = vHashTatico.begin();
	while(vit != vHashTatico.end())
	{
		VariableTatInt v = vit->first;
		if( v.getType() == VariableTatInt::V_OFERECIMENTO )
		{			
			mapO[ v.getTurma() ][ v.getDisciplina()->getId() ][ v.getSubCjtSala()->getId() ] = vit->second;
		}			
		vit++;
	}
	
	vit = vHashTatico.begin();
	while(vit != vHashTatico.end())
	{
		double coef;

		VariableTatInt v = vit->first;
		if( v.getType() == VariableTatInt::V_CREDITOS )
		{
			double nroAlunos = problemData->existeTurmaDiscCampus( v.getTurma(), v.getDisciplina()->getId(), v.getUnidade()->getIdCampus() );		
			coef = nroAlunos;
		}
		else
		{
			vit++;
			continue;
		}
		
		if ( v.getUnidade()->getIdCampus() != campusId )
			continue;

		c.reset();
		c.setType( ConstraintTatInt::C_CAPACIDADE_SALA );		
		c.setTurma( v.getTurma() );
		c.setDisciplina( v.getDisciplina() );
		c.setUnidade( v.getUnidade() );
		c.setSubCjtSala( v.getSubCjtSala() );
		c.setDia( v.getDia() );
		c.setHorarioAulaInicial( v.getHorarioAulaInicial() );
		c.setHorarioAulaFinal( v.getHorarioAulaFinal() );
        c.setDateTimeInicial( v.getDateTimeInicial() );
        c.setDateTimeFinal( v.getDateTimeFinal() );
		
		cit = cHashTatico.find(c);
		if(cit == cHashTatico.end())
		{
			sprintf( name, "%s", c.toString( etapa ).c_str() );
			OPT_ROW row( 100, OPT_ROW::LESS, 0.0, name );

			row.insert( vit->second, coef);

			int idxO = mapO[ v.getTurma() ][ v.getDisciplina()->getId() ][ v.getSubCjtSala()->getId() ];
			double cap = v.getSubCjtSala()->salas.begin()->second->getCapacidade();			
			row.insert( idxO, -cap);

			cHashTatico[ c ] = lp->getNumRows();
			lp->addRow( row );
			restricoes++;
		}
		else
		{
			idxC.push_back(vit->second);
			idxR.push_back(cit->second);
			valC.push_back(coef);
		}

		vit++;
	}
	
	lp->updateLP();
	lp->chgCoefList(idxC.size(),idxR.data(),idxC.data(),valC.data());
	lp->updateLP();

	idxC.clear();
	idxR.clear();
	valC.clear();

	return restricoes;

}

/*
	Para cada turma i, disciplina d, campus cp, dia t, horarios hi,hf:

	sum[s] x_{i,d,s,t,hi,hf} = 1
*/
int TaticoIntAlunoHor::criaRestricaoTotalAtend( int campusId )
{
   int restricoes = 0;
   int nnz;
   char name[ 1024 ];

   ConstraintTatInt c;
   VariableTatIntHash::iterator vit;
   ConstraintTatIntHash::iterator cit;

   std::vector< std::pair< int, int > > coeffList;
   std::vector< double > coeffListVal;
   std::pair< int, int > auxCoef;

   vit = vHashTatico.begin();

   for (; vit != vHashTatico.end(); vit++ )
   {
		VariableTatInt v = vit->first;
		
		double coef=0.0;

		if ( v.getType() == VariableTatInt::V_CREDITOS )
		{
			coef = 1.0;
		}
		else continue;

		int turma = v.getTurma();
		Disciplina* disc = v.getDisciplina();
		Campus * campus = problemData->refCampus[ v.getUnidade()->getIdCampus() ];

		c.reset();
		c.setType( ConstraintTatInt::C_ATEND_TOTAL );
		c.setTurma( turma );
		c.setDisciplina( disc );
		c.setCampus( campus );
		c.setDia( v.getDia() );									// t
		c.setHorarioAulaInicial( v.getHorarioAulaInicial() );	// hi
		c.setHorarioAulaFinal( v.getHorarioAulaFinal() );		// hf
        
		c.setDateTimeInicial( v.getDateTimeInicial() );
		c.setDateTimeFinal( v.getDateTimeFinal() );

		cit = cHashTatico.find( c );

		if ( cit != cHashTatico.end() )
		{
			auxCoef.first = cit->second;
			auxCoef.second = vit->second;

			coeffList.push_back( auxCoef );
			coeffListVal.push_back( coef );
		}
		else
		{			
			sprintf( name, "%s", c.toString( etapa ).c_str() );
			nnz = disc->cjtSalasAssociados.size();

			OPT_ROW row( nnz, OPT_ROW::EQUAL, 1.0, name );
						
			row.insert( vit->second, coef );

			cHashTatico[ c ] = lp->getNumRows();
			lp->addRow( row );
			restricoes++;
		}
   }

   chgCoeffList( coeffList, coeffListVal );

   return restricoes;
}



/*
	Para cada turma i, disciplina d, campus cp:

	sum[s] o_{i,d,cp,s} = 1
*/
int TaticoIntAlunoHor::criaRestricaoSalaUnica( int campusId )
{
   int restricoes = 0;
   int nnz;
   char name[ 1024 ];

   ConstraintTatInt c;
   VariableTatIntHash::iterator vit;
   ConstraintTatIntHash::iterator cit;

   std::vector< std::pair< int, int > > coeffList;
   std::vector< double > coeffListVal;
   std::pair< int, int > auxCoef;

   vit = vHashTatico.begin();

   for (; vit != vHashTatico.end(); vit++ )
   {
		VariableTatInt v = vit->first;
		
		double coef=0.0;

		if ( v.getType() == VariableTatInt::V_OFERECIMENTO )
		{
			coef = 1.0;
		}
		else continue;

		int turma = v.getTurma();
		Disciplina* disc = v.getDisciplina();
		Campus * campus = problemData->refCampus[ v.getUnidade()->getIdCampus() ];

		c.reset();
		c.setType( ConstraintTatInt::C_SALA_UNICA );
		c.setTurma( turma );
		c.setDisciplina( disc );
		c.setCampus( campus );

		cit = cHashTatico.find( c );

		if ( cit != cHashTatico.end() )
		{
			auxCoef.first = cit->second;
			auxCoef.second = vit->second;

			coeffList.push_back( auxCoef );
			coeffListVal.push_back( coef );
		}
		else
		{			
			sprintf( name, "%s", c.toString( etapa ).c_str() );
			nnz = disc->cjtSalasAssociados.size();

			OPT_ROW row( nnz, OPT_ROW::EQUAL, 1.0, name );
						
			row.insert( vit->second, coef );

			cHashTatico[ c ] = lp->getNumRows();
			lp->addRow( row );
			restricoes++;
		}
   }

   chgCoeffList( coeffList, coeffListVal );

   return restricoes;
}


int TaticoIntAlunoHor::criaRestricaoSalaHorario( int campusId )
{
	int restricoes = 0;
	int nnz;
	char name[ 1024 ];

	VariableTatInt v;
	ConstraintTatInt c;

	VariableTatIntHash::iterator vit;
	ConstraintTatIntHash::iterator cit;

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

   std::vector<int> idxC;
   std::vector<int> idxR;
   std::vector<double> valC;

	vit = vHashTatico.begin();
	while(vit != vHashTatico.end())
	{
		if(vit->first.getType() != VariableTatInt::V_CREDITOS)
		{
			vit++;
			continue;
		}

		VariableTatInt v = vit->first;

		if ( campus->unidades.find(v.getUnidade()) == campus->unidades.end())
		{
			vit++;
			continue;
		}

      //Elimina repetidos
      GGroup<DateTime*,LessPtr<DateTime> > horariosDifSala;

		Sala *sala = v.getSubCjtSala()->salas.begin()->second;
		ITERA_GGROUP( itHor, sala->horariosDia, HorarioDia )
		{
			HorarioAula* h = itHor->getHorarioAula();

			if ( itHor->getDia() != v.getDia() )					
				continue;

			 std::pair<DateTime*,int> auxP = problemData->horarioAulaDateTime[h->getId()];

			 if (horariosDifSala.find(auxP.first) != horariosDifSala.end())
			 {
				continue;
			 }
			 else
			 {
				horariosDifSala.add(auxP.first);
			 }

			DateTime fimF = v.getHorarioAulaFinal()->getInicio();
			fimF.addMinutes( v.getHorarioAulaFinal()->getTempoAula() );

			DateTime fimH = h->getInicio(); // controle
			fimH.addMinutes( h->getTempoAula() );
			
			if ( ( v.getHorarioAulaInicial()->getInicio() <= h->getInicio() ) && ( fimF >  h->getInicio() ) )
			{
				c.reset();
				c.setType( ConstraintTatInt::C_SALA_HORARIO );
				c.setCampus( campus );
				c.setUnidade( v.getUnidade() );
				c.setSubCjtSala( v.getSubCjtSala() );
				c.setDia( v.getDia() );
				c.setHorarioAulaInicial( h );
            std::pair<DateTime*,int> auxP = problemData->horarioAulaDateTime[h->getId()];
				c.setDateTimeInicial(auxP.first);

				cit = cHashTatico.find(c);
				if(cit == cHashTatico.end())
				{
					sprintf( name, "%s", c.toString( etapa ).c_str() );
					OPT_ROW row( 100, OPT_ROW::LESS , 1 , name );

					row.insert( vit->second, 1.0);

					cHashTatico[ c ] = lp->getNumRows();
					lp->addRow( row );
					restricoes++;
				}
				else
				{
					idxC.push_back(vit->second);
					idxR.push_back(cit->second);
					valC.push_back(1.0);
				}
			}
		}

		vit++;
	}

   lp->updateLP();
   lp->chgCoefList(idxC.size(),idxR.data(),idxC.data(),valC.data());
   lp->updateLP();

   idxC.clear();
   idxR.clear();
   valC.clear(); 

	return restricoes;

}


/*
	Para cada sala s:

	sum[i]sum[d]sum[t]sum[hi]sum[hf] x_{i,d,s,t,hi,hf} <= M * u_{s}
*/
int TaticoIntAlunoHor::criaRestricaoSalaUsada( int campusId )
{
   int restricoes = 0;
   int nnz;
   char name[ 1024 ];

   ConstraintTatInt c;
   VariableTatIntHash::iterator vit;
   ConstraintTatIntHash::iterator cit;

   std::vector< std::pair< int, int > > coeffList;
   std::vector< double > coeffListVal;
   std::pair< int, int > auxCoef;

   vit = vHashTatico.begin();

   for (; vit != vHashTatico.end(); vit++ )
   {
		VariableTatInt v = vit->first;
		
		double coef=0.0;

		if ( v.getType() == VariableTatInt::V_CREDITOS )
		{
			coef = 1.0;
		}
		else if ( v.getType() == VariableTatInt::V_SALA )
		{
			coef = -100.0;
		}
		else continue;
				
		c.reset();
		c.setType( ConstraintTatInt::C_SALA_USADA );
		c.setSubCjtSala( v.getSubCjtSala() );

		cit = cHashTatico.find( c );

		if ( cit != cHashTatico.end() )
		{
			auxCoef.first = cit->second;
			auxCoef.second = vit->second;

			coeffList.push_back( auxCoef );
			coeffListVal.push_back( coef );
		}
		else
		{			
			sprintf( name, "%s", c.toString( etapa ).c_str() );

			if ( v.getType() == VariableTatInt::V_CREDITOS )
				nnz = v.getDisciplina()->cjtSalasAssociados.size();
			else
				nnz = problemData->refSala.size();

			OPT_ROW row( nnz, OPT_ROW::LESS, 0.0, name );
						
			row.insert( vit->second, coef );

			cHashTatico[ c ] = lp->getNumRows();
			lp->addRow( row );
			restricoes++;
		}
   }

   chgCoeffList( coeffList, coeffListVal );

   return restricoes;
}

