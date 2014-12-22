#include "MIPUnico.h"
#include <math.h>

#include "CentroDados.h"

#include "ProblemData.h"
#include "ProblemSolution.h"
#include "VariableTatico.h"
#include "Util.h"

#include "Polish.h"

using namespace std;

#define MIP_ESCOLA
    

const bool CONSTR_GAP_PROF_SEPARADO = true;

// -----------------------------------------------------------------------------------------------

int MIPUnico::idCounter = 0;

// -----------------------------------------------------------------------------------------------
// Par�metros

// Gurobi
const int MIPUnico::timeLimitMaxAtend = 3600*3;
const int MIPUnico::timeLimitMaxAtendSemMelhora = 3600;
const int MIPUnico::timeLimitMinProfVirt = 3600*3;
const int MIPUnico::timeLimitMinProfVirtSemMelhora = 3600*2;
const int MIPUnico::timeLimitMinGapProf = 3600*2.5;
const int MIPUnico::timeLimitMinGapProfSemMelhora = 3600;
const int MIPUnico::timeLimitGeneral= 3600*2;
const int MIPUnico::timeLimitGeneralSemMelhora = 3600;

// Disciplinas
const int MIPUnico::consideraDivCredDisc = ParametrosPlanejamento::Weak;

// Professores
const bool MIPUnico::permiteCriarPV = true;
const bool MIPUnico::minimizarCustoProf = false;
const int MIPUnico::pesoGapProf = 1;

// Alunos
const int MIPUnico::pesoGapAluno = 3;
const int MIPUnico::pesoMinCredDiaAluno = 500;
const int MIPUnico::desvioMinCredDiaAluno = 2;
const int MIPUnico::considerarMinCredDiaAluno = ParametrosPlanejamento::Off;

// -----------------------------------------------------------------------------------------------

MIPUnico::MIPUnico( ProblemData * aProblemData, 
				GGroup< VariableTatico *, LessPtr<VariableTatico> > *aSolVarsTatico, 
				GGroup< VariableTatico *, LessPtr<VariableTatico> > *avars_xh, 
				bool *endCARREGA_SOLUCAO, bool equiv, int fase )				
				: Solver( aProblemData ), solVarsTatico(aSolVarsTatico), vars_xh(avars_xh),
				CARREGA_SOLUCAO(endCARREGA_SOLUCAO), xSol_(nullptr), optLogFileName("mipUnico"), optimized_(false)
{
   MIPUnico::idCounter++;

   USAR_EQUIVALENCIA = equiv && problemData->parametros->considerar_equivalencia_por_aluno;
   PERMITIR_NOVAS_TURMAS = ( fase == 1 || fase == 2 || fase == 3 ? true : false );
   
   if (PERMITIR_NOVAS_TURMAS && !equiv) 
	   CRIAR_VARS_FIXADAS = false;
   else 
	   CRIAR_VARS_FIXADAS = true;

   PERMITIR_REALOCAR_ALUNO = false;

   etapa = fase+1;

   ITERACAO = fase;
   
   lp = nullptr;
}

MIPUnico::~MIPUnico()
{
   if (lp)
      delete lp;
   if (xSol_)
	   delete [] xSol_;
}

void MIPUnico::getSolution( ProblemSolution * problem_solution ){}

int MIPUnico::solve(){return 1;}

void MIPUnico::solveMainEscola( int campusId, int prioridade, int r )
{
	updateOptLogFileName(campusId, prioridade, r);

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
	
	calculaNroFolgas( prioridade, campusId );

	corrigeNroTurmas( prioridade, campusId );
	
	solveMIPUnico( campusId, prioridade, r );
	
	carregaVariaveisSolucao( campusId, prioridade, r );
		
	problemData->preencheMapsTurmasTurnosIES();

	std::stringstream stepName;
	stepName << "ESCOLA_" << MIPUnico::idCounter;

	problemData->imprimeAlocacaoAlunos( campusId, prioridade, 0, false, r, stepName.str(), this->getRunTime() );
	
	problemData->imprimeUtilizacaoSala( campusId, prioridade, 0, false, r, stepName.str() );

	if (this->USAR_EQUIVALENCIA)
	{
		atualizarDemandasEquiv( campusId, prioridade );		
	}

	problemData->imprimeDiscNTurmas( campusId, prioridade, 0, false, r, MIPUnico::idCounter );		
	
	sincronizaSolucao( campusId, prioridade, r );
	
	imprimeTurmaProf( campusId, prioridade );
	
	imprimeProfTurmas( campusId, prioridade );

	imprimeGradeHorAlunos( campusId, prioridade );
	
	imprimeGradeHorAlunosPorDemanda( campusId, prioridade );	
	
	confereCorretude( campusId, prioridade );

	calculaNroFolgas( prioridade, campusId );
	
	bool violou = problemData->violaMinTurma( campusId );

	std::cout<<"\nFim do tatico integrado!\n"; fflush(NULL);

	return; 
}

void MIPUnico::updateOptLogFileName(int campusId, int prioridade, int r)
{
   if ( campusId != 0 )
   {	   
		 stringstream ss;
		 ss << campusId;
		 optLogFileName += "_Cp"; 
		 optLogFileName += ss.str();
   }
   if ( prioridade != 0 )
   {
		stringstream ss;
		ss << prioridade;
		optLogFileName += "_P"; 
		optLogFileName += ss.str();   		
   }
   if ( r != 0 )
   {
		stringstream ss;
		ss << r;
		optLogFileName += "_R"; 
		optLogFileName += ss.str();   		
   }
}

void MIPUnico::chgCoeffList(
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
	Verifica se seria possivel inserir o 'aluno' nas aulas em 'aulasX' (variaveis do tipo cr�dito, x).
	Para isso olha sobreposi��o de hor�rios do aluno em todas as aulas, e capacidade das salas.
*/
bool MIPUnico::violaInsercao( Aluno* aluno, GGroup< VariableMIPUnico *, LessPtr<VariableMIPUnico> > aulasX )
{
	bool viola=false;
	
	// verifica todas as aulas antes de inserir
	ITERA_GGROUP_LESSPTR( itAula, aulasX, VariableMIPUnico )
	{
		if ( itAula->getType() != VariableMIPUnico::V_CREDITOS )
		{
			std::cout<<"\nErro: s� deveria haver variaveis do tipo x aqui.\n";
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

void MIPUnico::imprimeGradeHorAlunos( int campusId, int prioridade )
{
	std::cout<<"\nImprimindo grades de alunos...\n";

	stringstream fileName;
	fileName << "gradeAlunos" << MIPUnico::idCounter;
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

void MIPUnico::imprimeGradeHorAlunosPorDemanda( int campusId, int prioridade )
{
	std::cout<<"\nImprimindo grades de alunos por demanda...\n";

	stringstream fileName;
	fileName << "gradeAlunosPorDemanda" << MIPUnico::idCounter;
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

void MIPUnico::imprimeTurmaProf( int campusId, int prioridade )
{
	std::cout<<"\nImprimindo assignment de turma a prof...\n";

	if (!CentroDados::getPrintLogs())
		return;
	
	stringstream fileName;
	fileName << "solTurmaProf" << MIPUnico::idCounter;
	fileName << "_" << problemData->getInputFileName();
	fileName << "_id" << problemData->inputIdToString() << ".txt";

	ofstream outTurmaProf(fileName.str(), ios::out);	
	if ( !outTurmaProf ) 
	{
		std::cout << "\nErro!! Arquivo nao pode ser aberto " << fileName.str();
		return;
	}
	
	outTurmaProf << "CampusId " << campusId << "  -  Prioridade " << prioridade << endl;
	for ( auto itCp = solAlocTurmaProf.begin(); itCp != solAlocTurmaProf.end(); itCp++ )
	{
		for ( auto itDisc = itCp->second.begin(); itDisc != itCp->second.end(); itDisc++ )
		{
			for ( auto itTurma = itDisc->second.begin(); itTurma != itDisc->second.end(); itTurma++ )
			{				
				outTurmaProf << "\n\nCp " << itCp->first->getId()
					<< " Disc " << itDisc->first->getId()
					<< " Turma " << itTurma->first << ":";

				if ( itTurma->second.size() == 1 )
				{
					outTurmaProf << " Prof " << (*itTurma->second.begin())->getId();
				}
				else
				{
					CentroDados::printError("void MIPUnico::imprimeTurmaProf()", "Mais de um professor associado.");
					for ( auto itProf = itTurma->second.begin(); itProf != itTurma->second.end(); itProf++ )
						outTurmaProf << " Prof " << (*itProf)->getId();
				}
			}
		}
	}
	outTurmaProf.close();
}

void MIPUnico::imprimeProfTurmas( int campusId, int prioridade )
{
	std::cout<<"\nImprimindo assignment de prof e turmas...\n";

	if (!CentroDados::getPrintLogs())
		return;
	
	stringstream fileName;
	fileName << "solProfTurmas" << MIPUnico::idCounter;
	fileName << "_" << problemData->getInputFileName();
	fileName << "_id" << problemData->inputIdToString() << ".txt";

	ofstream outProfTurmas(fileName.str(), ios::out);	
	if ( !outProfTurmas ) 
	{
		std::cout << "\nErro!! Arquivo nao pode ser aberto " << fileName.str();
		return;
	}
	
	outProfTurmas << "CampusId " << campusId << "  -  Prioridade " << prioridade << endl;
	for ( auto itProf = solAlocProfTurma.begin(); itProf != solAlocProfTurma.end(); itProf++ )
	{
		outProfTurmas << "\n\nProf " << itProf->first->getId() << ": ";

		for ( auto itCp = itProf->second.begin(); itCp != itProf->second.end(); itCp++ )
		{
			for ( auto itDisc = itCp->second.begin(); itDisc != itCp->second.end(); itDisc++ )
			{
				for ( auto itTurma = itDisc->second.begin(); itTurma != itDisc->second.end(); itTurma++ )
				{				
					outProfTurmas << " (Cp" << itCp->first->getId()
						<< "Disc" << itDisc->first->getId()
						<< "Turma" << *itTurma << ")";
				}
			}
		}
	}
	outProfTurmas.close();
}

void MIPUnico::imprimeTodasVars(int p)
{
	// Para confer�ncia de cria��o das vari�veis. Imprime todas as variaveis criadas com seus respectivos numeros de coluna.

	if (!CentroDados::getPrintLogs())
		return;
	
	std::map<int, set<VariableMIPUnico> > mapColVar;
	for ( auto vit = vHashTatico.begin(); vit != vHashTatico.end(); vit++ )
	{
		mapColVar[vit->second].insert(vit->first);
	}

	stringstream sstr;
	sstr << "allVars_P" << p << ".txt";
	ofstream outFile;
	outFile.open(sstr.str(),ios::app);
	for ( auto itCol = mapColVar.begin(); itCol != mapColVar.end(); itCol++ )
	{
		outFile << endl << itCol->first;

		for ( auto itVar = itCol->second.begin(); itVar != itCol->second.end(); itVar++ )
		{
			outFile << "\t" << itVar->toString();
		}

		if ( itCol->second.size() != 1 )
			CentroDados::printError("void MIPUnico::imprimeTodasVars(int p)", "Mais de uma variavel por id de coluna");
	}
	outFile << endl << endl << endl;
		
	outFile << "vars_abertura_turma:";
	for ( auto itCp = vars_abertura_turma.begin(); itCp != vars_abertura_turma.end(); itCp++)
	{		
		for ( auto itDisc = itCp->second.begin(); itDisc != itCp->second.end(); itDisc++)
		{		
			for ( auto itTurma = itDisc->second.begin(); itTurma != itDisc->second.end(); itTurma++)
			{
				outFile << "\n" << itTurma->second.first
						<< "\t" << itTurma->second.second.toString();
				
				char name[200];
				int col = itTurma->second.first;
				
				if ( lp->getColName( name, col, 200 ) )
				{
					outFile << " \tLp: " << name;
				}
				else outFile << " \tLp: ERROR";

				fflush(0);
			}				
		}
	}

	outFile.close();
}

void MIPUnico::confereCorretude( int campusId, int prioridade )
{
	std::cout<<"\nConfere corretude...\n";

	// --------------------------------------------------------
	// Confere corretude de alocacoes de alunos em disciplinas
	problemData->confereCorretudeAlocacoes();


	// --------------------------------------------------------
	// Maps da solu��o
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
	// Maperando as turmas da solu��o

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
						CentroDados::printError( "void MIPUnico::confereCorretude( int campusId, int prioridade )", msg.str() );
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
					CentroDados::printError( "void MIPUnico::confereCorretude( int campusId, int prioridade )", msg.str() );
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
				CentroDados::printError( "void MIPUnico::confereCorretude( int campusId, int prioridade )", msg.str() );
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
							CentroDados::printError( "void MIPUnico::confereCorretude( int campusId, int prioridade )", msg.str() );
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

void MIPUnico::corrigeNroTurmas( int prioridade, int campusId )
{	
#ifdef MIP_ESCOLA
	std::cout<<"\nCorrigindo nro de turmas: cada aluno-demanda corresponde uma turma da disciplina.\n"; fflush(NULL);

	char fileName[1024]="\0";
	strcpy( fileName, getCorrigeNrTurmasFileName( campusId, prioridade, 1 ).c_str() );	
	
	ofstream nrTurmasFile;
	nrTurmasFile.open( fileName, ios::out );
	if ( !nrTurmasFile )
	{
		std::cout << "\nErro em corrigeNroTurmas(int campusAtualId, int prioridade, int r): arquivo "
					<< fileName << " nao encontrado.\n";
		return;
	}
	
	std::unordered_map< Disciplina*, unordered_set<AlunoDemanda*> > mapDiscDemanda;
	ITERA_GGROUP_LESSPTR( itAlDem, problemData->alunosDemanda, AlunoDemanda )
	{
		if (itAlDem->demanda->oferta->getCampusId() == campusId && 
			itAlDem->getPrioridade() <= prioridade)
			mapDiscDemanda[itAlDem->demanda->disciplina].insert(*itAlDem);
	}

	ITERA_GGROUP_LESSPTR( itDisc, problemData->disciplinas, Disciplina )
	{
		Disciplina* disciplina = *itDisc;
		int nTurmas = mapDiscDemanda[disciplina].size();
		disciplina->setNumTurmas(nTurmas);

		nrTurmasFile << "\nDisciplina "<<disciplina->getId() << ": " << nTurmas << " turmas.";
	}

	nrTurmasFile.close();
#endif
}

void MIPUnico::calculaNroFolgas( int prioridade, int campusId )
{	
	std::cout<<"\nCalculando nro de folgas...\n"; fflush(NULL);

	this->mapDiscNroFolgasDemandas.clear();
	ITERA_GGROUP_LESSPTR( itDisc, problemData->disciplinas, Disciplina )
	{
		Disciplina * disciplina = *itDisc;

		int n = problemData->getNroFolgasDeAtendimento( prioridade, disciplina, campusId );
		this->mapDiscNroFolgasDemandas[disciplina] = n;
	}
}

bool MIPUnico::permitirAbertura( int turma, Disciplina *disciplina, int campusId )
{
	Trio< int, Disciplina*, int > trio;
	trio.set( turma, disciplina, campusId );

	// Procura no mapPermitirAbertura, caso j� tenha sido calculado
	if ( mapPermitirAbertura.find( trio ) != mapPermitirAbertura.end() )
		return mapPermitirAbertura[trio];
	
	// N�o foi calculado para esse trio ainda, calcula e insere o resultado em mapPermitirAbertura.

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

void MIPUnico::sincronizaSolucao( int campusAtualId, int prioridade, int r )
{
	std::cout<<"\nSincronizando as solucoes...\n"; fflush(NULL);

   // --------------------------------------------------------------------
   // cria variaveis do tipo VariableTatico para as novas turmas abertas
   // e insere-as na lista vars_xh que cont�m a solu��o parcial at� o momento.
   addVariaveisTatico();
   
   // ----------------------------------------------------
   // deleta solVarsTatInt, vars_v e hashs
   ITERA_GGROUP_LESSPTR ( it, solVarsTatInt, VariableMIPUnico )
   {
		delete *it;    
   }
   vars_v.clear();
   solVarsTatInt.clear();
   clearVariablesMaps();
   cHashTatico.clear();
   // ----------------------------------------------------

   return;
}

void MIPUnico::addVariaveisTatico()
{
	int nroAntigoTurmas = 0;

	if ( ! this->PERMITIR_NOVAS_TURMAS && this->CRIAR_VARS_FIXADAS ) // Primeiro modelo: inser��o de alunos
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

	ITERA_GGROUP_LESSPTR( itVar, solVarsTatInt, VariableMIPUnico )
	{
		VariableMIPUnico *v = *itVar;

		switch( v->getType() )
		{
			 case VariableMIPUnico::V_CREDITOS:  //  x_{i,d,u,s,hi,hf,t} 
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
				
				std::pair<DateTime*,int> auxPi = problemData->horarioAulaDateTime[v->getHorarioAulaInicial()->getId()];
				x->setDateTimeInicial( auxPi.first );
				std::pair<DateTime*,int> auxPf = problemData->horarioAulaDateTime[v->getHorarioAulaFinal()->getId()];
				x->setDateTimeFinal( auxPf.first );
			
				if ( solVarsTatico->find( x ) == solVarsTatico->end() )
				{					
					this->vars_xh->add( x );
					this->solVarsTatico->add( x );
				}
				else delete x;
				break;
			 }
			 case VariableMIPUnico::V_ABERTURA: // z_{i,d,cp}
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
				else delete x;
				break;								
			 }
		}
	}
	cout << "\n\n" << nroAtualTurmas - nroAntigoTurmas << " novas turmas.\n\n";
}

std::string MIPUnico::getCorrigeNrTurmasFileName( int campusId, int prioridade, int r)
{
	std::string solName;
	solName += "corrigeNrTurmas";
	solName += problemData->inputIdToString();
	   
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

std::string MIPUnico::getAumentaTurmasFileName( int campusId, int prioridade, int r)
{
	std::string solName;
	solName += "aumentaTurmas";
	solName += problemData->inputIdToString();
	 
	  
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

std::string MIPUnico::getTaticoLpFileName( int campusId, int prioridade, int r )
{
	std::string solName;
	solName += "MIPUnico";
	solName += problemData->inputIdToString();

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

std::string MIPUnico::getSolBinFileName( int campusId, int prioridade, int r)
{
	std::string solName;
	solName += "solMIPUnico";
	solName += problemData->inputIdToString();
	
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

std::string MIPUnico::getSolucaoTaticoFileName( int campusId, int prioridade, int r, int fase )
{
	std::string solName;
	solName += "solucaoMIPUnico";
	solName += problemData->inputIdToString();
	

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

std::string MIPUnico::getEquivFileName( int campusId, int prioridade )
{
	std::string solName;
	solName += "Equivalencias";
	solName += problemData->inputIdToString();
		   
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

void MIPUnico::writeSolBin( int campusId, int prioridade, int r, int type, double *xSol )
{
	if (!CentroDados::getPrintLogs())
		return;
	
	char solName[1024]="\0";

	switch (type)
	{
		case (MIP_GENERAL):
			strcpy( solName, getSolBinFileName( campusId, prioridade, r ).c_str() );			
			break;
		case (MIP_GARANTE_SOL):
			strcpy( solName, "garanteSol" );
			strcat( solName, getSolBinFileName( campusId, prioridade, r ).c_str() );
			break;
		case (MIP_MAX_ATEND):
			strcpy( solName, "maxAtend" );
			strcat( solName, getSolBinFileName( campusId, prioridade, r ).c_str() );
			break;
		case (MIP_MIN_VIRT):
			strcpy( solName, "minVirt" );
			strcat( solName, getSolBinFileName( campusId, prioridade, r ).c_str() );
			break;
		case (MIP_MIN_GAP_PROF):
			strcpy( solName, "minGapProf" );
			strcat( solName, getSolBinFileName( campusId, prioridade, r ).c_str() );
			break;
	}

	// WRITES SOLUTION
		
	FILE * fout = fopen( solName, "wb" );
	if ( fout == NULL )
	{
		std::cout << "\nErro em MIPUnico::writeSolBin( int campusId, int prioridade, int r, int type ):"
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

void MIPUnico::readSolTxtAux( char *fileName, double *xSol )
{		
	ifstream fin( fileName, ios_base::in );
	if ( fin == NULL )
	{
		std::cout << "\nErro em MIPUnico::readSolTxtAux:"
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

      VariableMIPUnicoHash::iterator vit = vHashTatico.begin();
		while ( vit != vHashTatico.end() )
		{
         VariableMIPUnico v = vit->first;
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

void MIPUnico::writeSolTxt( int campusId, int prioridade, int r, int type, double *xSol, int fase )
{

	char solName[1024]="\0";

	switch (type)
	{
		case (MIP_GENERAL):
			strcpy( solName, getSolucaoTaticoFileName( campusId, prioridade, r, fase ).c_str() );			
			break;
		case (MIP_GARANTE_SOL):
			strcpy( solName, "garanteSol" );
			strcat( solName, getSolucaoTaticoFileName( campusId, prioridade, r, fase ).c_str() );
			break;
		case (MIP_MAX_ATEND):
			strcpy( solName, "maxAtend" );
			strcat( solName, getSolucaoTaticoFileName( campusId, prioridade, r, fase ).c_str() );
			break;
		case (MIP_MIN_VIRT):
			strcpy( solName, "minVirt" );
			strcat( solName, getSolucaoTaticoFileName( campusId, prioridade, r, fase ).c_str() );
			break;
		case (MIP_MIN_GAP_PROF):
			strcpy( solName, "minGapProf" );
			strcat( solName, getSolucaoTaticoFileName( campusId, prioridade, r, fase ).c_str() );
			break;
	}

	// WRITES SOLUTION
		
	ofstream fout( solName, ios_base::out );
	if ( fout == NULL )
	{
		std::cout << "\nErro em MIPUnico::writeSolTxt( int campusId, int prioridade, int r, int type ):"
				<< "\nArquivo " << solName << " nao pode ser aberto.\n";
	}
	else
	{		
		VariableMIPUnicoHash::iterator vit = vHashTatico.begin();
		while ( vit != vHashTatico.end() )
		{
			VariableMIPUnico v = vit->first;
			int col = vit->second;
			double value = xSol[ col ];
		  
			fout << v.toString() << " = " << value << endl;
				  
			vit++;
		}
		fout.close();		
	}
}

int MIPUnico::readSolBin( int campusId, int prioridade, int r, int type, double *xSol )
{
	char solName[1024]="\0";

	switch (type)
	{
		case (MIP_GENERAL):
			strcpy( solName, getSolBinFileName( campusId, prioridade, r ).c_str() );			
			break;
		case (MIP_GARANTE_SOL):
			strcpy( solName, "garanteSol" );
			strcat( solName, getSolBinFileName( campusId, prioridade, r ).c_str() );
			break;
		case (MIP_MAX_ATEND):
			strcpy( solName, "maxAtend" );
			strcat( solName, getSolBinFileName( campusId, prioridade, r ).c_str() );
			break;
		case (MIP_MIN_VIRT):
			strcpy( solName, "minVirt" );
			strcat( solName, getSolBinFileName( campusId, prioridade, r ).c_str() );
			break;
		case (MIP_MIN_GAP_PROF):
			strcpy( solName, "minGapProf" );
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

int MIPUnico::readSolTxt( int campusId, int prioridade, int r, int type, double *xSol, int fase )
{
   char solName[1024]="\0";

	switch (type)
	{
		case (MIP_GENERAL):
			strcpy( solName, getSolucaoTaticoFileName( campusId, prioridade, r, fase ).c_str() );			
			break;
		case (MIP_GARANTE_SOL):
			strcpy( solName, "garanteSol" );
			strcat( solName, getSolucaoTaticoFileName( campusId, prioridade, r, fase ).c_str() );
			break;
		case (MIP_MAX_ATEND):
			strcpy( solName, "maxAtend" );
			strcat( solName, getSolucaoTaticoFileName( campusId, prioridade, r, fase ).c_str() );
			break;
		case (MIP_MIN_VIRT):
			strcpy( solName, "minVirt" );
			strcat( solName, getSolucaoTaticoFileName( campusId, prioridade, r, fase ).c_str() );
			break;
		case (MIP_MIN_GAP_PROF):
			strcpy( solName, "minGapProf" );
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

      VariableMIPUnicoHash::iterator vit = vHashTatico.begin();
      while ( vit != vHashTatico.end() )
      {
         VariableMIPUnico v = vit->first;
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

int MIPUnico::writeGapTxt( int campusId, int prioridade, int r, int type, double gap )
{
	if (!CentroDados::getPrintLogs())
		return 1;
	
	std::string step;
	switch (type)
	{
		case (MIP_GENERAL):
			step = "Final";		
			break;
		case (MIP_GARANTE_SOL):
			step = "Garante Solucao";
			break;
		case (MIP_MAX_ATEND):
			step = "Max Atend";
			break;
		case (MIP_MIN_VIRT):
			step = "Min Profs Virtuais";
			break;
		case (MIP_MIN_GAP_PROF):
			step = "Min Gaps Profs";
			break;
		default:
			step = "No type";
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
		std::cerr<<"\nErro: Abertura do arquivo " << gapFilename << " falhou em MIPUnico::writeGapTxt() em " << step << endl;
		return 0;
	}
	else
	{
		outGaps << "Tatico Integrado (" << step << ") - campus "<< campusId << ", prioridade " << prioridade << ", rodada "<< r;
		outGaps << "\nGap = " << gap << "%";
		
		if ( type == MIP_GENERAL )
			outGaps << "\n\n------------------------------------------------------------------------------------------------";
		outGaps << "\n\n";

		outGaps.close();
		return 1;
	} 
}

void MIPUnico::initCredsSala()
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

int MIPUnico::getNroMaxAlunoDemanda( int discId )
{
	std::map< int /*discId*/, GGroup<AlunoDemanda*,LessPtr<AlunoDemanda>> >::iterator 
		it = mapDiscAlunosDemanda.find(discId);
	if ( it != mapDiscAlunosDemanda.end() )
		return it->second.size();
	else
		return 0;
}

void MIPUnico::setOptLogFile(std::ofstream &logMip, string name, bool clear)
{
   stringstream ss;
   ss << name << ".log";
   
   if (!clear)
	   logMip.open(ss.str(),ios::app);
   else
	   logMip.open(ss.str(),ios::out);

   if(lp)
	   lp->setLogFile((char*)ss.str().c_str());
}

void MIPUnico::clearVariablesMaps()
{
	vHashTatico.clear();
	vars_prof_aula1.clear();
	vars_prof_aula2.clear();
	vars_prof_aula3.clear();
	vars_aluno_aula.clear();
	vars_prof_dia_fase_dt.clear();
	vars_aluno_dia_dt.clear();
	varsProfDiaFaseHiHf.clear();
	varsAlunoDiaHiHf.clear();
	varsProfFolgaGap.clear();
	varsAlunoFolgaGap.clear();
	vars_prof_turma.clear();	
	vars_turma_aula.clear();
	vars_turma_aula2.clear();
	vars_abertura_turma.clear();
}

void MIPUnico::printLog( string msg )
{
//	if(mipFile)
//	{
		mipFile.flush();
		mipFile.seekp(0,ios::end);
		mipFile << msg << endl;
		mipFile.flush();
//	}
}

void MIPUnico::carregaVariaveisSolucao( int campusAtualId, int prioridade, int r )
{
    std::cout << "\nCarregando solucao tatico integrado...\n";

    lp->updateLP();  
	
	if ( *(this->CARREGA_SOLUCAO) )
	{
	    int nroColsLP = lp->getNumCols();
		
		if (xSol_) delete [] xSol_;

		xSol_ = new double[ nroColsLP ];
        
		int status = readSolTxt(campusAtualId, prioridade, r, OutPutFileType::MIP_GENERAL, xSol_, 0 );
		if ( !status )
		{
			CentroDados::printError("MIPUnico::carregaVariaveisSolucao()", "Arquivo nao encontrado!");
		    delete [] xSol_;
			xSol_ = nullptr;
			exit(EXIT_FAILURE);
		}
	}
	else if (!optimized_)
	{
		CentroDados::printError("MIPUnico::carregaVariaveisSolucao()", "Problema nao-otimizado e sem solucao carregada!");
		return;
	}

   std::map< std::pair<Disciplina*, Oferta*>, int > mapSlackDemanda;
   std::map< std::pair<Disciplina*, Oferta*>, int >::iterator itMapSlackDemanda;
   
   // -----------------------------------------------------
   // Deleta todas as variaveis referenciadas em solVarsTatInt e em vars_v
   ITERA_GGROUP_LESSPTR ( it, solVarsTatInt, VariableMIPUnico )
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
	   std::cout << "\nErro em SolverMIP::carregaVariaveisSolucao( int campusAtualId, int prioridade, int r )"
				 << "\nArquivo nao pode ser aberto\n";
	   fout = fopen( "solSubstituto.txt", "wt" );
	   if ( fout == NULL )
	   {
			std::cout <<"\nErro de novo. Finalizando execucao...\n";
			exit(0);
	   }
   }
      

	initCredsSala();
	
	if ( PERMITIR_REALOCAR_ALUNO && CRIAR_VARS_FIXADAS )	// Modo inser��o de alunos
	{
		ITERA_GGROUP_LESSPTR( itAluno, problemData->alunos, Aluno )
		{
			// TODO: se o aluno estiver em mais de um campus, s� posso apagar os horarios usados no campus atual
			if ( itAluno->getCampusIds().size() > 1 )
				std::cout << "\nAtencao!! Aluno " << itAluno->getAlunoId() << " esta em mais de um campus. Erro aqui.";
			else if ( itAluno->hasCampusId( campusAtualId ) )
				itAluno->clearHorariosDiaOcupados();
		}

		problemData->clearMaps( campusAtualId, prioridade );
	}

   int nroNaoAtendimentoAlunoDemanda = 0;
   int nroAtendimentoAlunoDemanda = 0;
   
   VariableMIPUnicoHash::iterator vit = vHashTatico.begin();
   while ( vit != vHashTatico.end() )
   {
	  VariableMIPUnico* v = new VariableMIPUnico( vit->first );
      int col = vit->second;
      v->setValue( xSol_[ col ] );

      if ( v->getValue() > 0.001 )
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
		 Professor *vProf;
		 Campus *vCampus;

         switch( v->getType() )
         {
			 case VariableMIPUnico::V_ERROR:
				std::cout << "Vari�vel inv�lida " << std::endl;
				break;
			 case VariableMIPUnico::V_ALOCA_ALUNO_TURMA_DISC:	
				 vAluno = v->getAluno();
				 vDisc = v->getDisciplina();	

				 if ( this->USAR_EQUIVALENCIA )
					alunoDem = problemData->procuraAlunoDemOrigEquiv( vDisc, vAluno, prioridade );
				 else
					 alunoDem = vAluno->getAlunoDemanda( vDisc->getId() );

				 nroAtendimentoAlunoDemanda++;
				 
				 // -------------------------------------------
				 // Remove o alunoDemanda dos n�o-atendimentos, caso ele esteja l�
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
			 case VariableMIPUnico::V_ALUNO_CREDITOS:
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
			 case VariableMIPUnico::V_CREDITOS:				 					 
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
				 break;
			 case VariableMIPUnico::V_SLACK_DEMANDA_ALUNO:
				 nroNaoAtendimentoAlunoDemanda++;
				 alunoDem = v->getAluno()->getAlunoDemandaEquiv( v->getDisciplina() );
				 if ( alunoDem!=NULL ) problemData->listSlackDemandaAluno.add( alunoDem );
				 else std::cout<<"\nErro, AlunoDemanda nao encontrado. Aluno " 
					 << v->getAluno()->getAlunoId() << " disc " << v->getDisciplina()->getId();
				 break;
			 case VariableMIPUnico::V_OFERECIMENTO:			
				 vDisc = v->getDisciplina();
				 vCjtSala = v->getSubCjtSala();
				 if ( problemData->mapCreditosSalas.find(vCjtSala) == problemData->mapCreditosSalas.end() )
					  problemData->mapCreditosSalas[vCjtSala] = vDisc->getTotalTempo();
				 else problemData->mapCreditosSalas[vCjtSala] += vDisc->getTotalTempo();
				 break;
			 case VariableMIPUnico::V_PROF_TURMA:
				 vProf = v->getProfessor();
				 vDisc = v->getDisciplina();
				 vTurma = v->getTurma();
				 vCampus = v->getCampus();
				 solAlocProfTurma[vProf][vCampus][vDisc].insert(vTurma);
				 solAlocTurmaProf[vCampus][vDisc][vTurma].insert(vProf);
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
    
    if ( xSol_ )
    {
       delete [] xSol_;
	   xSol_ = nullptr;
    }
   

   std::cout << "\nSolucao tatico integrado carregada com sucesso!\n";
   std::cout << "\n-----------------------------------------------------------------\n";
   std::cout << "-----------------------------------------------------------------\n\n";

    return;
}

void MIPUnico::verificaCarregaSolucao( int campusId, int prioridade, int r )
{
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
}

void MIPUnico::criaNewLp( int campusId, int prioridade, int r )
{
	std::cout<<"\nCreating LP...\n"; fflush(NULL);

	if ( lp != nullptr )
	{
		lp->freeProb();
		delete lp;
		lp = nullptr;
	}

	#ifdef SOLVER_CPLEX
		lp = new OPT_CPLEX; 
	#elif SOLVER_GUROBI
		lp = new OPT_GUROBI; 
	#endif
	
    char lpName[1024];
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
}

void MIPUnico::clearStrutures()
{
    if ( vHashTatico.size() > 0 )
    {
		clearVariablesMaps();
    }
    if ( cHashTatico.size() > 0 )
    {
	   cHashTatico.clear();
    }
}

int MIPUnico::solveMIPUnico( int campusId, int prioridade, int r )
{	
	std::cout<<"\nSolving...\n"; fflush(NULL);
	bool status=true;
	bool CARREGA_SOL_PARCIAL = * this->CARREGA_SOLUCAO;
	
	verificaCarregaSolucao(campusId, prioridade, r);

    int varNum = 0;
    int constNum = 0;
   
    criaNewLp(campusId, prioridade, r);
   
    setOptLogFile(mipFile,optLogFileName);
	
	clearStrutures();
	
	criaNewLp(campusId, prioridade, r);

	// Variable creation
	varNum = criaVariaveisTatico( campusId, prioridade, r );
		
	if ( ! (*this->CARREGA_SOLUCAO) )
	{
		// Constraint creation
		constNum = criaRestricoesTatico( campusId, prioridade, r );
					 			
		if (xSol_) delete [] xSol_;

	    xSol_ = new double[lp->getNumCols()];
				
		solveGaranteSolucao( campusId, prioridade, r, CARREGA_SOL_PARCIAL, xSol_ );
		
		solveMaxAtend( campusId, prioridade, r, CARREGA_SOL_PARCIAL, xSol_ );
		
		solveMinProfVirt( campusId, prioridade, r, CARREGA_SOL_PARCIAL, xSol_ );
			
		solveMinGapProf( campusId, prioridade, r, CARREGA_SOL_PARCIAL, xSol_ );
			
		status=solveGeneral( campusId, prioridade, r, CARREGA_SOL_PARCIAL, xSol_ );
	}

	std::cout<<"\n------------------------------Fim do Tatico Integrado -----------------------------\n"; fflush(NULL);

   return status;
}

int MIPUnico::solveGaranteSolucao( int campusId, int prioridade, int r, bool& CARREGA_SOL_PARCIAL, double *xS )
{	
	stringstream ss;
	ss << "\n-----------------------------------------------------------------"
		<< "\nGarantindo solucao...\n";
	printLog( ss.str() );

	// -------------------------------------------------------------------
    char lpName[1024];
    strcpy( lpName, getTaticoLpFileName( campusId, prioridade, r ).c_str() );
	
	int *idxs = new int[lp->getNumCols()*2];
	double *vals = new double[lp->getNumCols()*2];
	double *valsOrig = new double[lp->getNumCols()*2];
	BOUNDTYPE *bds = new BOUNDTYPE[lp->getNumCols()*2];
	int nBds = 0;

	std::cout << "\n=========================================";
	std::cout << "\nGarantindo solucao...\n"; fflush(NULL);

	VariableMIPUnicoHash::iterator vit = vHashTatico.begin();
	for ( ; vit != vHashTatico.end(); vit++ )
	{
		VariableMIPUnico v = vit->first;

		int lb = (int)(lp->getLB(vit->second) + 0.5);
		int ub = (int)(lp->getUB(vit->second) + 0.5);
		
		if ( v.getType() == VariableMIPUnico::V_ALOCA_ALUNO_TURMA_DISC )
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
	lp->setMIPEmphasis(0);
	lp->setSymetry(0);
	lp->setCuts(3);
	lp->setNumIntSols(1);
	lp->updateLP();
#endif

	std::string lpName1;
	lpName1 += "garanteSol_";
	lpName1 += string(lpName);
		
	optimize();
	
	getXSol(xS);
	
	writeSolTxt( campusId, prioridade, r, OutPutFileType::MIP_GARANTE_SOL, xS, 0 );
					
	// -------------------------------------------------------------------
	// Volta as variaveis que estavam livres
         
	lp->chgBds( nBds,idxs,bds,valsOrig );
	lp->updateLP();

	delete[] valsOrig;		
	delete[] vals;
	delete[] idxs;
	delete[] bds;

	return optimized_;
}

int MIPUnico::solveMaxAtend( int campusId, int prioridade, int r, bool& CARREGA_SOL_PARCIAL, double *xS )
{
	stringstream ss;
	ss << "\n-----------------------------------------------------------------"
		<< "\nMaximizando atendimento...\n";
	printLog( ss.str() );

	std::cout << "\n=========================================";
	std::cout << "\nGarantindo maximo atendimento...\n"; fflush(NULL);
		
	// -------------------------------------------------------------------
	// Salvando fun��o objetivo original

	double *objN = new double[lp->getNumCols()];
	lp->getObj(0,lp->getNumCols()-1,objN);

	
	// -------------------------------------------------------------------
	// Lp name

    char lpName[1024];
    strcpy( lpName, getTaticoLpFileName( campusId, prioridade, r ).c_str() );


	// -------------------------------------------------------------------
	// FUN��O OBJETIVO SOMENTE COM AS FOLGAS DE DEMANDA 

	int *idxN = new int[lp->getNumCols()];
	int *idxs = new int[lp->getNumCols()*2];
	double *vals = new double[lp->getNumCols()*2];
	BOUNDTYPE *bds = new BOUNDTYPE[lp->getNumCols()*2];
	int nBds = 0;

	#pragma region Modifica FO
    nBds = 0;
	VariableMIPUnicoHash::iterator vit = vHashTatico.begin();
	for ( ; vit != vHashTatico.end(); vit++ )
	{
		idxN[vit->second] = vit->second;

		VariableMIPUnico v = vit->first;
			
		if ( v.getType() == VariableMIPUnico::V_SLACK_DEMANDA_ALUNO )
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
    lp->updateLP();
	#pragma endregion

	
	// ------------------------------------------------------------------------------------

		
	std::string lpName2;
	lpName2 += "maxAtend_";
	lpName2 += string(lpName);

	if (CentroDados::getPrintLogs())
		lp->writeProbLP( lpName2.c_str() );
	
	lp->copyMIPStartSol(lp->getNumCols(),idxN,xS);
					
	lp->updateLP();			

	if ( CARREGA_SOL_PARCIAL )
	{
		// procura e carrega solucao parcial
		int statusReadBin = readSolTxt(campusId, prioridade, r, OutPutFileType::MIP_MAX_ATEND, xS, 0 );
		if ( !statusReadBin )
		{
			CARREGA_SOL_PARCIAL=false;
		}
		else writeSolTxt( campusId, prioridade, r, OutPutFileType::MIP_MAX_ATEND, xS, 0 );
	}
	if ( !CARREGA_SOL_PARCIAL )
	{
		// GENERATES SOLUTION 		 
		
		bool polishing = true;
		if ( polishing )
		{  
			#ifdef SOLVER_CPLEX
				Polish *pol = new Polish(lp, vHashTatico, optLogFileName);
				polishing = pol->polish(xS, 3600, 90, 1000);
				delete pol;
			#elif defined SOLVER_GUROBI				
				Polish *pol = new Polish(lp, vHashTatico, optLogFileName);
				polishing = pol->polish(xS, timeLimitMaxAtend, 90, timeLimitMaxAtendSemMelhora);
				delete pol;
			#endif
		}
		if (!polishing)
		{
			#ifdef SOLVER_CPLEX
				lp->setNumIntSols(100000000);
				lp->setTimeLimit( this->getTimeLimit(Solver::TAT_INT2) );
				lp->setMemoryEmphasis(true);
				lp->setPreSolve(OPT_TRUE);
				lp->setHeurFrequency(1.0);
				lp->setMIPScreenLog( 4 );
				lp->setMIPEmphasis(4);
				lp->setNodeLimit(100000000);
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
				lp->setPreSolveIntensity(OPT_LEVEL2);
				lp->setMIPEmphasis(1);
				lp->setSymetry(-1);
				lp->setCuts(2);
				lp->setHeurFrequency(0.8);
				lp->setPolishAfterTime( this->getTimeLimit(Solver::TAT_INT2) / 3 );

				#if defined SOLVER_GUROBI && defined USAR_CALLBACK
				cb_data.timeLimit = this->getMaxTimeNoImprov(Solver::TAT_INT2);
				cb_data.gapMax = 40;
				lp->setCallbackFunc( &timeWithoutChangeCallback, &cb_data );
				#endif
			#endif
			
			optimize();	
			getXSol(xS);
		}

		writeSolTxt( campusId, prioridade, r, OutPutFileType::MIP_MAX_ATEND, xS, 0 );
	}      
	
	fflush(NULL);
		

	// ------------------------------------------------------------------------------------
	// FIXA SOLU��O OBTIDA ANTERIORMENTE

	#pragma region Fixa solu��o
    nBds = 0;
	vit = vHashTatico.begin();
	for ( ; vit != vHashTatico.end(); vit++ )
	{
		VariableMIPUnico v = vit->first;

		if (  v.getType() == VariableMIPUnico::V_SLACK_DEMANDA_ALUNO && xS[vit->second] < 0.1 ) // atendido
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
	// Volta com a fun��o objetivo original	
	lp->chgObj(lp->getNumCols(),idxN,objN);

	lp->updateLP();
	
	// ------------------------------------------------------------------------------------
	// Copia solu��o
	int cpyStatus = lp->copyMIPStartSol(lp->getNumCols(),idxN,xS);
	std::cout << "\ncopyMIPStartSol = " << cpyStatus;
 
	lp->updateLP();

	delete[] idxs;
	delete[] vals;
	delete[] bds;
	delete[] idxN;
	delete[] objN;
	
	return cpyStatus;
}

int MIPUnico::solveMinProfVirt( int campusId, int prioridade, int r, bool& CARREGA_SOL_PARCIAL, double *xS )
{
	stringstream ss;
	ss << "\n-----------------------------------------------------------------"
		<< "\nMinimizando nro de profs virtuais...\n";
	printLog( ss.str() );

	std::cout << "\n=========================================";
	std::cout << "\nGarantindo min profs virtuais...\n"; fflush(NULL);
		
	int status = 0;

	// -------------------------------------------------------------------
	// Salvando fun��o objetivo original
	double *objN = new double[lp->getNumCols()];
	lp->getObj(0,lp->getNumCols()-1,objN);
		
	// -------------------------------------------------------------------
	// Lp name
    char lpName[1024];
    strcpy( lpName, getTaticoLpFileName( campusId, prioridade, r ).c_str() );
	
	// -------------------------------------------------------------------
	// FUN��O OBJETIVO SOMENTE COM AS VARIAVEIS DE PROFS

	int *idxN = new int[lp->getNumCols()];
	int *idxs = new int[lp->getNumCols()*2];
	double *vals = new double[lp->getNumCols()*2];
	BOUNDTYPE *bds = new BOUNDTYPE[lp->getNumCols()*2];
	int nBds = 0;

	#pragma region Modifica FO
    nBds = 0;
	auto vit = vHashTatico.begin();
	for ( ; vit != vHashTatico.end(); vit++ )
	{
		idxN[vit->second] = vit->second;

		VariableMIPUnico v = vit->first;
			
		double coef = 0.0;		
		if ( v.getType() == VariableMIPUnico::V_PROF_TURMA )
		{     
			if ( v.getProfessor()->eVirtual() )
				coef = 1.0;
		}

		idxs[nBds] = vit->second;
		vals[nBds] = coef;
		nBds++;
	}
	
    lp->chgObj(nBds,idxs,vals);
    lp->updateLP();
	#pragma endregion
		
	// ------------------------------------------------------------------------------------
			
	std::string lpName2;
	lpName2 += "minVirt_";
	lpName2 += string(lpName);

	if (CentroDados::getPrintLogs())
		lp->writeProbLP( lpName2.c_str() );
	
	lp->copyMIPStartSol(lp->getNumCols(),idxN,xS);				
	lp->updateLP();

	if ( CARREGA_SOL_PARCIAL )
	{
		// procura e carrega solucao parcial
		int statusReadBin = readSolTxt(campusId, prioridade, r, OutPutFileType::MIP_MIN_VIRT, xS, 0 );
		if ( !statusReadBin )
		{
			CARREGA_SOL_PARCIAL=false;
		}
		else{
			writeSolTxt( campusId, prioridade, r, OutPutFileType::MIP_MIN_VIRT, xS, 0 );
			//CARREGA_SOL_PARCIAL=false;
		}
	}
	if ( !CARREGA_SOL_PARCIAL )
	{
		// GENERATES SOLUTION 		 
		
		bool polishing = true;
		if ( polishing )
		{  
			#ifdef SOLVER_CPLEX
				Polish *pol = new Polish(lp, vHashTatico, optLogFileName, Polish::PH_MIN_PV);
				polishing = pol->polish(xS, 3600, 90, 1000);
				delete pol;
			#elif defined SOLVER_GUROBI				
				Polish *pol = new Polish(lp, vHashTatico, optLogFileName, Polish::PH_MIN_PV);
				polishing = pol->polish(xS, timeLimitMinProfVirt, 90, timeLimitMinProfVirtSemMelhora);
				delete pol;
			#endif
		}
		if (!polishing)
		{
			#ifdef SOLVER_CPLEX
				lp->setNumIntSols(100000000);
				lp->setTimeLimit( this->getTimeLimit(Solver::TAT_INT2) );
				lp->setMemoryEmphasis(true);
				lp->setPreSolve(OPT_TRUE);
				lp->setHeurFrequency(1.0);
				lp->setMIPScreenLog( 4 );
				lp->setMIPEmphasis(4);
				lp->setNodeLimit(100000000);
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
				lp->setPreSolveIntensity(OPT_LEVEL2);
				lp->setMIPEmphasis(1);
				lp->setSymetry(-1);
				lp->setCuts(2);
				lp->setHeurFrequency(0.8);
				lp->setPolishAfterTime( this->getTimeLimit(Solver::TAT_INT2) / 3 );

				#if defined SOLVER_GUROBI && defined USAR_CALLBACK
				cb_data.timeLimit = 1800;
				cb_data.gapMax = 40;
				lp->setCallbackFunc( &timeWithoutChangeCallback, &cb_data );
				#endif
			#endif
			
			optimize();	
			getXSol(xS);
		}

		writeSolTxt( campusId, prioridade, r, OutPutFileType::MIP_MIN_VIRT, xS, 0 );
	}      
	
	fflush(NULL);
		

	// ------------------------------------------------------------------------------------
	// FIXA SOLU��O OBTIDA ANTERIORMENTE

	#pragma region Fixa solu��o
    nBds = 0;
	vit = vHashTatico.begin();
	for ( ; vit != vHashTatico.end(); vit++ )
	{
		VariableMIPUnico v = vit->first;

		if ( v.getType() != VariableMIPUnico::V_PROF_TURMA ) continue;	// ToDo: O melhor � fixar o m�ximo de turmas com virtuais, embora fixar 'quais' deixa o prob mais facil
		
		if ( v.getProfessor()->eVirtual() && xS[vit->second] < 0.1 )	// turma sem prof virtual
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
	// Volta com a fun��o objetivo original	
	lp->chgObj(lp->getNumCols(),idxN,objN);

	lp->updateLP();
	
	// ------------------------------------------------------------------------------------
	// Copia solu��o
	bool cpyStatus = lp->copyMIPStartSol(lp->getNumCols(),idxN,xS);
	std::cout << "\ncopyMIPStartSol = " << cpyStatus;
 
	lp->updateLP();

	delete[] idxs;
	delete[] vals;
	delete[] bds;
	delete[] idxN;
	delete[] objN;
	
	return cpyStatus;
}

int MIPUnico::solveMinGapProf( int campusId, int prioridade, int r, bool& CARREGA_SOL_PARCIAL, double *xS )
{
	if (problemData->parametros->proibirProfGapMTN == ParametrosPlanejamento::Off)
		return 1;

	stringstream ss;
	ss << "\n-----------------------------------------------------------------"
		<< "\nMinimizando gaps de profs...\n";
	printLog( ss.str() );

	std::cout << "\n=========================================";
	std::cout << "\nGarantindo min gaps de profs...\n"; fflush(NULL);
		
	if ( CONSTR_GAP_PROF_SEPARADO )
		addConstrGapProf();
	
	// -------------------------------------------------------------------
	// Salvando fun��o objetivo original
	double *objN = new double[lp->getNumCols()];
	lp->getObj(0,lp->getNumCols()-1,objN);
		
	// -------------------------------------------------------------------
	// Lp name
    char lpName[1024];
    strcpy( lpName, getTaticoLpFileName( campusId, prioridade, r ).c_str() );
	
	// -------------------------------------------------------------------
	// FUN��O OBJETIVO SOMENTE COM AS VARIAVEIS DE FOLGA DE GAP DE PROFS

	int *idxN = new int[lp->getNumCols()];
	int *idxs = new int[lp->getNumCols()*2];
	double *vals = new double[lp->getNumCols()*2];
	BOUNDTYPE *bds = new BOUNDTYPE[lp->getNumCols()*2];
	int nBds = 0;

	#pragma region Modifica FO
    nBds = 0;
	auto vit = vHashTatico.begin();
	for ( ; vit != vHashTatico.end(); vit++ )
	{
		idxN[vit->second] = vit->second;

		VariableMIPUnico v = vit->first;
			
		double coef = 0.0;		
		if ( v.getType() == VariableMIPUnico::V_FOLGA_GAP_PROF )
		{     
			coef = 1.0;
		}

		idxs[nBds] = vit->second;
		vals[nBds] = coef;
		nBds++;
	}
	
    lp->chgObj(nBds,idxs,vals);
    lp->updateLP();
	#pragma endregion
		
	// ------------------------------------------------------------------------------------
			
	std::string lpName2;
	lpName2 += "minGapProf_";
	lpName2 += string(lpName);

	if (CentroDados::getPrintLogs())
		lp->writeProbLP( lpName2.c_str() );
	
	lp->copyMIPStartSol(lp->getNumCols(),idxN,xS);				
	lp->updateLP();

	if ( CARREGA_SOL_PARCIAL )
	{
		// procura e carrega solucao parcial
		int statusReadBin = readSolTxt(campusId, prioridade, r, OutPutFileType::MIP_MIN_GAP_PROF, xS, 0 );
		if ( !statusReadBin )
		{
			CARREGA_SOL_PARCIAL=false;
		}
		else writeSolTxt( campusId, prioridade, r, OutPutFileType::MIP_MIN_GAP_PROF, xS, 0 );
	}
	if ( !CARREGA_SOL_PARCIAL )
	{		
		// ----------------------------
		// Polish
		Polish *pol = new Polish(lp, vHashTatico, optLogFileName);
		pol->polish(xS, timeLimitMinGapProf, 90, timeLimitMinGapProfSemMelhora);
		delete pol;

		writeSolTxt( campusId, prioridade, r, OutPutFileType::MIP_MIN_GAP_PROF, xS, 0 );
	}      
	
	fflush(NULL);
		

	// ------------------------------------------------------------------------------------
	// FIXA SOLU��O OBTIDA ANTERIORMENTE

	#pragma region Fixa solu��o
    nBds = 0;
	vit = vHashTatico.begin();
	for ( ; vit != vHashTatico.end(); vit++ )
	{
		VariableMIPUnico v = vit->first;

		if ( v.getType() != VariableMIPUnico::V_FOLGA_GAP_PROF ) continue;
		
		if ( !v.getProfessor()->eVirtual() )	// prof com gap m�nimo
		{
			idxs[nBds] = vit->second;
			vals[nBds] = xS[vit->second];
			bds[nBds] = BOUNDTYPE::BOUND_UPPER;
			nBds++;
		}
	}

    lp->chgBds(nBds,idxs,bds,vals);
	#pragma endregion
	
	// ------------------------------------------------------------------------------------
	// Volta com a fun��o objetivo original	
	lp->chgObj(lp->getNumCols(),idxN,objN);

	lp->updateLP();
	
	// ------------------------------------------------------------------------------------
	// Copia solu��o
	int cpyStatus = lp->copyMIPStartSol(lp->getNumCols(),idxN,xS);
	std::cout << "\ncopyMIPStartSol = " << cpyStatus;
 
	lp->updateLP();

	delete[] idxs;
	delete[] vals;
	delete[] bds;
	delete[] idxN;
	delete[] objN;
	
	return cpyStatus;
}

int MIPUnico::solveGeneral( int campusId, int prioridade, int r, bool& CARREGA_SOL_PARCIAL, double *xS )
{		
	bool optStatus=false;

	stringstream ss;
	ss << "\n-----------------------------------------------------------------"
		<< "\nGarantindo o resto dos parametros...\n";
	printLog( ss.str() );

	std::cout << "\n=========================================";
	std::cout << "\nGarantindo o resto dos parametros...\n"; fflush(NULL);
		
	char lpName[1024];
    strcpy( lpName, getTaticoLpFileName( campusId, prioridade, r ).c_str() );
	
	if (CentroDados::getPrintLogs())
		lp->writeProbLP( lpName );
				
	if ( CARREGA_SOL_PARCIAL )
	{
		// procura e carrega solucao parcial
		int statusReadBin = readSolTxt(campusId, prioridade, r, OutPutFileType::MIP_GENERAL, xS, 0 );
		if ( !statusReadBin )
		{
			CARREGA_SOL_PARCIAL=false;
		}
		else writeSolTxt( campusId, prioridade, r, OutPutFileType::MIP_GENERAL, xS, 0 );
	}
	if ( !CARREGA_SOL_PARCIAL )
	{
		// GENERATES SOLUTION
		bool polishing=true;
		if ( polishing )
		{
			#ifdef SOLVER_CPLEX
				Polish *pol = new Polish(lp, vHashTatico, optLogFileName);
				polishing = pol->polish(xS, 1800, 90, 1000);
				delete pol;
			#elif defined SOLVER_GUROBI				
				Polish *pol = new Polish(lp, vHashTatico, optLogFileName);
				polishing = pol->polish(xS, timeLimitGeneral, 90, timeLimitGeneralSemMelhora);
				optStatus = polishing;
				delete pol;
			#endif
		}

		if (!polishing)
		{
			#ifdef SOLVER_CPLEX
			lp->setNumIntSols(0);
       		lp->setTimeLimit( this->getTimeLimit(Solver::TAT_INT) );
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
			#elif SOLVER_GUROBI
			lp->setNumIntSols(0);
			lp->setTimeLimit( this->getTimeLimit(Solver::TAT_INT) );
			lp->setPreSolveIntensity(OPT_LEVEL2);
			lp->setHeurFrequency(0.8);
			lp->setPolishAfterTime( this->getTimeLimit(Solver::TAT_INT) / 2 );
			lp->setMIPEmphasis(1);
			lp->setSymetry(2);
			lp->setCuts(2);
			#if defined SOLVER_GUROBI && defined USAR_CALLBACK
			cb_data.timeLimit = this->getMaxTimeNoImprov(Solver::TAT_INT);
			cb_data.gapMax = 30;
			lp->setCallbackFunc( &timeWithoutChangeCallback, &cb_data );
			#endif
			lp->updateLP();
			#endif

			optStatus = optimize();	
			getXSol(xS);
		}

		writeSolTxt( campusId, prioridade, r, OutPutFileType::MIP_GENERAL, xS, 0 );
	}
	
	fflush(0);
			
	return optStatus;
}

void MIPUnico::getXSol(double *xS)
{
	if (optimized_)
	{
		lp->getX( xS );
	}
}

bool MIPUnico::optimize()
{
    OPTSTAT status = lp->optimize( METHOD_MIP );
	
	if(isOptimized(status)) optimized_ = true;
	else optimized_ = false;

	return optimized_;
}

bool MIPUnico::isOptimized(OPTSTAT status)
{
	if (status == OPTSTAT_MIPOPTIMAL || status == OPTSTAT_LPOPTIMAL || status == OPTSTAT_FEASIBLE)
		return true;
	
	checkFeasibility(status);
	return false;
}

bool MIPUnico::infeasible(OPTSTAT status)
{
	if (status == OPTSTAT_INFEASIBLE)
		return true;
	return false;
}

void MIPUnico::checkFeasibility(OPTSTAT status)
{
	if (infeasible(status))
	{
		stringstream ss;
		ss <<"Error! Model is infeasible." << std::endl;
		CentroDados::printError("bool MIPUnico::checkFeasibility()",ss.str());
	}
}

int MIPUnico::addConstrGapProf()
{
	int restricoes = 0;
	CPUTimer timer;
	double dif = 0.0;
	timer.start();
	restricoes += criarRestricaoProfHiHf_();	// Restricao 1.2.38
	timer.stop();
	dif = timer.getCronoCurrSecs();
	lp->updateLP();

#ifdef PRINT_cria_restricoes
	std::cout << "\nnumRest criarRestricaoProfHiHf_: " << restricoes  <<" "<<dif <<" sec" << std::endl;
#endif

	copyInitialSolutionGapProf();

	return restricoes;
}

int MIPUnico::copyInitialSolutionGapProf()
{	
	if (!optimized_) return false;

	int nCols = 0;
	int *idxN = new int[lp->getNumCols()];
	double *x = new double[lp->getNumCols()];
	for ( auto vit = vHashTatico.begin(); vit != vHashTatico.end(); vit++ )
	{
		if (vit->first.getType() != VariableMIPUnico::V_HI_PROFESSORES &&
			vit->first.getType() != VariableMIPUnico::V_HF_PROFESSORES &&
			vit->first.getType() != VariableMIPUnico::V_FOLGA_GAP_PROF)
		{
			idxN[nCols] = vit->second;
			x[nCols] = xSol_[vit->second];
			nCols++;
		}
	}

	bool stat = lp->copyMIPStartSol(nCols,idxN,x);
	if (!stat)
		CentroDados::printError("int MIPUnico::copyInitialSolutionGapProf()",
					"Copying start solution has failed.");

	delete [] idxN;
	delete [] x;

	return stat;
}

Unidade* MIPUnico::retornaUnidadeDeAtendimento( int turma, Disciplina* disciplina, Campus* campus )
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

ConjuntoSala* MIPUnico::retornaSalaDeAtendimento( int turma, Disciplina* disciplina, Campus* campus )
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

GGroup< VariableTatico *, LessPtr<VariableTatico> > MIPUnico::retornaAulasEmVarX( int turma, Disciplina* disciplina, int campusId )
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

bool MIPUnico::SolVarsFound( VariableTatico v )
{	
	GGroup< VariableTatico *, LessPtr<VariableTatico> >::iterator itSol = this->solVarsTatico->find(&v);

	if(itSol != this->solVarsTatico->end())
		return true;
	else
		return false;
}

bool MIPUnico::criaVariavelTaticoInt( VariableMIPUnico *v, bool &fixar, int prioridade )
{	
	VariableTatico vSol;
		   
	switch( v->getType() )
	{
		
		 case VariableMIPUnico::V_ERROR:
		 {
			 return true;
		 }		 
		 case VariableMIPUnico::V_ALUNO_CREDITOS:  //  v_{a,i,d,u,s,hi,hf,t} 
		 {
			 if ( !CRIAR_VARS_FIXADAS )
			 {
 				 int turmaDoAluno = problemData->retornaTurmaDiscAluno( v->getAluno(), v->getDisciplina() );
				 if ( turmaDoAluno != -1 )
				 {					
					 // aluno alocado: n�o cria ( !CRIAR_VARS_FIXADAS)
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
						 << v->getDisciplina()->getId() << " e aluno " << aluno->getAlunoId();
					 return false;
				 }
				 				 
				 #ifdef ALUNO_TURNOS_DA_DEMANDA				 
				 if ( ! alDem->getOferta()->turno->possuiHorarioDiaOuCorrespondente( hi, hf, dia ) )
					 return false;
				 #endif
				 
				// Verifica os hor�rios j� alocados do aluno.
				// Se houver sobreposi��o com os hor�rios da vari�vel v, n�o permite a cria��o da mesma.
				if ( aluno->sobrepoeAulaJaAlocada( hi, hf, dia ) )
					return false;
				
				return true;
			 }
			 
			 // CRIAR_VARS_FIXADAS

			 if ( PERMITIR_REALOCAR_ALUNO ){
				 fixar = false;
				 return true;
			 }

			 int turmaDoAluno = problemData->retornaTurmaDiscAluno( v->getAluno(), v->getDisciplina() );
			 bool alunoAlocado = turmaDoAluno != -1;
			 bool alunoAlocadoOutraTurma = alunoAlocado && (turmaDoAluno != v->getTurma());
			 bool alunoAlocadoNessaTurma = alunoAlocado && (turmaDoAluno == v->getTurma());

			 if ( alunoAlocadoOutraTurma )
				 return false;
			 if ( alunoAlocadoNessaTurma ){
				 fixar=true;
				 return true;
			 }		 		 			 
			 
			 // Aluno n�o alocado!

			 Aluno *aluno = v->getAluno();
			 int turma = v->getTurma();
			 Disciplina *disciplina = v->getDisciplina();
			 int campusId = v->getUnidade()->getIdCampus();
			 int dia = v->getDia();
			 HorarioAula *hi = v->getHorarioAulaInicial();
			 HorarioAula *hf = v->getHorarioAulaFinal();

			 AlunoDemanda *alDem = aluno->getAlunoDemandaEquiv( disciplina );
			 if ( alDem == nullptr ){ 
				stringstream msg;
				msg <<"\nERRO ao criar var v: AlunoDemanda nao encontrado para disc " 
					<< v->getDisciplina()->getId() << " e aluno " << aluno->getAlunoId();
				CentroDados::printError("MIPUnico::criaVariavelTaticoInt()",msg.str());
				exit(1);
			 }
				 
			 #ifdef ALUNO_TURNOS_DA_DEMANDA			 
			 if ( ! alDem->getOferta()->turno->possuiHorarioDiaOuCorrespondente( hi, hf, dia ) )
				 return false;
			 #endif

			 // aluno n�o alocado: cria a variavel livre se 'x' e 'z' existirem;
			 // aluno n�o alocado: n�o cria a variavel se 'x' n�o existir e 'z' existir;				 
			 // aluno n�o alocado: cria a variavel livre se nem 'x' nem 'z' existirem;
				 
			 // Se a turma j� existe
			 if ( problemData->existeTurmaDiscCampus(turma, disciplina->getId(), campusId) )
			 {					
				// Verifica todas as aulas da turma (outros dias).
				// Se houver sobreposi��o com hor�rios j� alocados do aluno, n�o criar a vari�vel.

				GGroup< VariableTatico *, LessPtr<VariableTatico> > aulasX = this->retornaAulasEmVarX( turma, disciplina, campusId );
				ITERA_GGROUP_LESSPTR( itX, aulasX, VariableTatico )
				{
					int diaX = itX->getDia();
					HorarioAula *hiX = itX->getHorarioAulaInicial();
					HorarioAula *hfX = itX->getHorarioAulaFinal();
					if ( aluno->sobrepoeAulaJaAlocada( hiX, hfX, diaX ) )
						return false;
				}

				// aluno n�o alocado e com hor�rios dispon�veis.
				// Cria a variavel LIVRE;

				fixar=false;
				return true;
			 }

			 // Turma n�o existe

			 // Verifica os hor�rios j� alocados do aluno.
			 // Se houver sobreposi��o com os hor�rios da vari�vel v, n�o permite a cria��o da mesma.
			 if ( aluno->sobrepoeAulaJaAlocada( hi, hf, dia ) )
				 return false;
			
			 fixar=false;
			 return true;

			 break;
		 }
		 case VariableMIPUnico::V_CREDITOS:  //  x_{i,d,u,s,hi,hf,t} 
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
			  
			 std::pair<DateTime*,int> auxP = problemData->horarioAulaDateTime[v->getHorarioAulaInicial()->getId()];
			 vSol.setDateTimeInicial( auxP.first );
			 std::pair<DateTime*,int> auxPf = problemData->horarioAulaDateTime[v->getHorarioAulaFinal()->getId()];
			 vSol.setDateTimeFinal( auxPf.first );
			

			 if ( SolVarsFound(vSol) )
			 {
				fixar=true;
				return (true && CRIAR_VARS_FIXADAS);
			 }
			 else // se n�o existe 'x', s� cria se for pra turmas novas
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
						// Verifica os hor�rios j� alocados da sala.
						// Se houver sobreposi��o com os hor�rios da vari�vel v, n�o permite a cria��o da mesma.
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
		 case VariableMIPUnico::V_SLACK_DEMANDA_ALUNO: // fd_{d,a}
		 {
			 // n�o permite desalocar o que j� est� fixo.

			 if ( problemData->retornaTurmaDiscAluno( v->getAluno(), v->getDisciplina() ) != -1 )
			 	return false;

			 fixar=false;
			 return true;	
			 break;
		 }
		 case VariableMIPUnico::V_TURMA_ATEND_CURSO: // b_{i,d,c}
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
		 case VariableMIPUnico::V_FOLGA_OCUPA_SALA:  //  fos_{i,d,cp} 
		 {		
			 if ( permitirAbertura( v->getTurma(), v->getDisciplina(), v->getCampus()->getId() ) )
			 {
				 fixar=false;
				 return (true && CRIAR_VARS_FIXADAS);						
			 }
			 return false;
		 }
		 case VariableMIPUnico::V_SLACK_ABERT_SEQ_TURMA:  //  ft_{i,d,cp} 
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
	Possiveis casos de equival�ncias:

	(Disciplina Antiga => Disciplina Nova): 
			1. (T => T)
			2. (T => PT)
			3. (PT => T)
			4. (PT => PT)

	As informa��es de equival�ncias est�o presentes somente nas disciplinas teoricas. Ou seja,
	nos casos 2. e 4. a estrutura discEquivSubstitutas de T conter� P e T, e a estrutura 
	discEquivSubstitutas de P ser� vazia; nos casos 1. e 3. a estrutura discEquivSubstitutas
	de T conter� T, e a estrutura discEquivSubstitutas de P ser� vazia.

	P = disciplina de credito pratico
	T = disciplina de credito teorico
*/
void MIPUnico::atualizarDemandasEquiv( int campusId, int prioridade )
{	
	std::cout<<"\nAtualizando demandas substitutas por equivalencia...\n"; fflush(NULL);

	#pragma region Imprime Equiv
	ofstream outEquiv;
	outEquiv.open( getEquivFileName(campusId,prioridade).c_str(), ofstream::app);
	if ( !outEquiv )
	{
		std::cerr<<"\nAbertura do arquivo "<< getEquivFileName(campusId,prioridade).c_str()
			<< " falhou em MIPUnico::atualizarDemandasEquiv().\n";
	}
	#pragma endregion

	int idAlDemanda = problemData->retornaMaiorIdAlunoDemandas();

	ITERA_GGROUP_LESSPTR ( itVar, this->solVarsTatInt, VariableMIPUnico )
	{
		if ( (*itVar)->getType() == VariableMIPUnico::V_ALOCA_ALUNO_TURMA_DISC )
		{
			VariableMIPUnico *v = *itVar; // s_{a,i,d,cp}
			Aluno *aluno = v->getAluno();
			Disciplina *disciplina = v->getDisciplina();
			int turma = v->getTurma();

			if ( v->getCampus()->getId() != campusId )
				continue;
			
			// Continue, caso seja aloca��o de prioridade passada
			if ( prioridade==2 && problemData->procuraAlunoDemanda(disciplina->getId(), aluno->getAlunoId(), 1) != NULL )
				continue;
			
			AlunoDemanda *alDemOrig = problemData->procuraAlunoDemanda( disciplina->getId(), aluno->getAlunoId(), prioridade );


			if ( alDemOrig==NULL ) // disciplina substituta
			{
				AlunoDemanda *alDem = problemData->atualizaAlunoDemandaEquiv( turma, disciplina, campusId, aluno, prioridade );
				
				if ( alDem==NULL )
				{
					std::cout<<"\nErro em void MIPUnico::atualizarDemandasEquiv( int campusId, int prioridade, int r ). AlunoDemanda null.";
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


void MIPUnico::preencheMapDiscAlunosDemanda( int campusId, int P, int r )
{
	// Preenche o map para auxilio na cria��o das variaveis
	std::cout << "\nPreenchendo mapDiscAlunosDemanda para auxilio na criacao de variaveis..." ;

	CPUTimer timer;
	double dif = 0.0;	
	timer.start();

	if ( problemData->parametros->considerar_equivalencia_por_aluno && USAR_EQUIVALENCIA ) 
	{		
		ITERA_GGROUP_LESSPTR( itDisc, problemData->disciplinas, Disciplina )
		{
			this->mapDiscAlunosDemanda[(*itDisc)->getId()] =
				problemData->retornaMaxDemandasDiscNoCampus_EquivTotal( *itDisc, campusId, P, this->PERMITIR_REALOCAR_ALUNO );
		}
	}
	else
	{
		if ( !CRIAR_VARS_FIXADAS )
		{
			// S� considera as demandas N�O atendidas			
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

	timer.stop();
	dif = timer.getCronoCurrSecs();
	std::cout << " preenchido! \t" << dif <<" sec" <<std::endl;
	
	std::cout << this->mapDiscAlunosDemanda.size() << " disciplinas com demanda." << std::endl;
}

int MIPUnico::criaVariaveisTatico( int campusId, int P, int r )
{
	int num_vars = 0;
	CPUTimer timer;
	double dif = 0.0;

	int numVarsAnterior = 0;
	
	preencheMapDiscAlunosDemanda( campusId, P, r );
	
	timer.start();
	std::cout << "Criando \"x\": ";fflush(NULL);
	num_vars += this->criaVariavelTaticoCreditos( campusId, P ); // x_{i,d,u,s,t,hi,hf}	
	timer.stop();
	dif = timer.getCronoCurrSecs();
	
#ifdef PRINT_cria_variaveis
	std::cout << "numVars \"x\": " << (num_vars - numVarsAnterior)  <<"\t "<<dif <<" sec" << std::endl; fflush(NULL);
	numVarsAnterior = num_vars;
#endif


	timer.start();	
	std::cout << "Criando \"v\": ";fflush(NULL);	
	//if ( this->etapa != Etapa5 )
		num_vars += this->criaVariavelTaticoAlunoCreditosAPartirDeX_MaisFiltroAluno( campusId, P ); // v_{a,i,d,u,s,t,hi,hf}
	//else
	//	num_vars += this->criaVariavelTaticoAlunoCreditosAPartirDeX( campusId, P ); // v_{a,i,d,u,s,t,hi,hf}
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
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_variaveis
	std::cout << "numVars \"fkp e fkm\": " << (num_vars - numVarsAnterior)  <<"\t "<<dif <<" sec" << std::endl; fflush(NULL);
	numVarsAnterior = num_vars;
#endif
	
	timer.start();
	std::cout << "Criando \"zc\": ";fflush(NULL);
	num_vars += this->criaVariavelTaticoAberturaCompativelAPartirDeX( campusId, P ); // zc
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
	std::cout << "Criando \"z\": ";fflush(NULL);
	num_vars += this->criaVariavelTaticoAberturaAPartirDeX( campusId, P ); // z_{i,d,cp}
	timer.stop();
	dif = timer.getCronoCurrSecs();
	std::cout << "numVars \"z\": " << (num_vars - numVarsAnterior)  <<"\t "<<dif <<" sec" << std::endl; fflush(NULL);
	numVarsAnterior = num_vars;
	

	timer.start();
	std::cout << "Criando \"ss\": ";fflush(NULL);
	num_vars += this->criaVariavelTaticoAlunosMesmaTurmaPratica( campusId, P ); // ss_{dp,a1,a2}
	timer.stop();
	dif = timer.getCronoCurrSecs();
	std::cout << "numVars \"ss\": " << (num_vars - numVarsAnterior)  <<"\t "<<dif <<" sec" << std::endl; fflush(NULL);
	numVarsAnterior = num_vars;


	timer.start();
	std::cout << "Criando \"fmd\": ";fflush(NULL);
	num_vars += this->criaVariavelTaticoFolgaMinimoDemandaPorAluno( campusId, P ); // fmd_{a}
	timer.stop();
	dif = timer.getCronoCurrSecs();
	std::cout << "numVars \"fmd\": " << (num_vars - numVarsAnterior)  <<"\t "<<dif <<" sec" << std::endl; fflush(NULL);
	numVarsAnterior = num_vars;

	
	timer.start();
	std::cout << "Criando \"fos\": ";fflush(NULL);
	num_vars += this->criaVariavelFolgaOcupacaoSala( campusId, P );				// fos_{i,d,cp}
	timer.stop();
	dif = timer.getCronoCurrSecs();
	std::cout << "numVars \"fos\": " << (num_vars - numVarsAnterior)  <<"\t "<<dif <<" sec" << std::endl; fflush(NULL);
	numVarsAnterior = num_vars;
	

	timer.start();
	std::cout << "Criando \"y\": ";fflush(NULL);
	num_vars += this->criaVariavelProfTurmaAPartirDeZ();				// y_{p,i,d,cp}
	timer.stop();
	dif = timer.getCronoCurrSecs();
	std::cout << "numVars \"y\": " << (num_vars - numVarsAnterior)  <<"\t "<<dif <<" sec" << std::endl; fflush(NULL);
	numVarsAnterior = num_vars;
	
	
	timer.start();
	std::cout << "Criando \"k\": ";fflush(NULL);
	num_vars += this->criaVariavelProfAulaAPartirDeX();				// k_{p,i,d,cp,t,hi}
	timer.stop();
	dif = timer.getCronoCurrSecs();
	std::cout << "numVars \"k\": " << (num_vars - numVarsAnterior)  <<"\t "<<dif <<" sec" << std::endl; fflush(NULL);
	numVarsAnterior = num_vars;


   timer.start();
	std::cout << "Criando \"hip_hfp\": ";fflush(NULL);				// hip_{p,t,f} e hfp_{p,t,f}
   num_vars += this->criaVariaveisHiHfProfFaseDoDiaAPartirDeK();			
	timer.stop();
	dif = timer.getCronoCurrSecs();
	std::cout << "numVars \"hip hfp\": " << (num_vars - numVarsAnterior)  <<"\t "<<dif <<" sec" << std::endl; fflush(NULL);
	numVarsAnterior = num_vars;
	
   timer.start();
	std::cout << "Criando \"hia_hfa\": ";fflush(NULL);				// hia_{p,t} e hfa_{p,t}
   num_vars += this->criaVariaveisHiHfAlunoDiaAPartirDeV();
	timer.stop();
	dif = timer.getCronoCurrSecs();
	std::cout << "numVars \"hia hfa\": " << (num_vars - numVarsAnterior)  <<"\t "<<dif <<" sec" << std::endl; fflush(NULL);
	numVarsAnterior = num_vars;
	
	
   timer.start();
	std::cout << "Criando \"fpgap\": ";fflush(NULL);				// fpgap{p,t,f}
   num_vars += this->criaVariavelFolgaGapProfAPartirDeK();
	timer.stop();
	dif = timer.getCronoCurrSecs();
	std::cout << "numVars \"fpgap\": " << (num_vars - numVarsAnterior)  <<"\t "<<dif <<" sec" << std::endl; fflush(NULL);
	numVarsAnterior = num_vars;
	
   timer.start();
	std::cout << "Criando \"fagap\": ";fflush(NULL);				// fagap_{a,t}
   num_vars += this->criaVariavelFolgaGapAlunoAPartirDeV();
	timer.stop();
	dif = timer.getCronoCurrSecs();
	std::cout << "numVars \"fagap\": " << (num_vars - numVarsAnterior)  <<"\t "<<dif <<" sec" << std::endl; fflush(NULL);
	numVarsAnterior = num_vars;
		
	
   timer.start();
	std::cout << "Criando \"fcad\": ";fflush(NULL);				// fcad_{a,t}
   num_vars += this->criarVariavelFolgaMinCredsDiaAluno();
	timer.stop();
	dif = timer.getCronoCurrSecs();
	std::cout << "numVars \"fcad\": " << (num_vars - numVarsAnterior)  <<"\t "<<dif <<" sec" << std::endl; fflush(NULL);
	numVarsAnterior = num_vars;
			
	
   timer.start();
	std::cout << "Criando \"fch\": ";fflush(NULL);				// fch_{p}
   num_vars += this->criaVariavelFolgaCargaHorariaAnteriorProfessor();
	timer.stop();
	dif = timer.getCronoCurrSecs();
	std::cout << "numVars \"fch\": " << (num_vars - numVarsAnterior)  <<"\t "<<dif <<" sec" << std::endl; fflush(NULL);
	numVarsAnterior = num_vars;
			
	

	lp->updateLP();


	#ifdef PRINT_cria_variaveis
	printf( "Total of Variables: %i\n\n", num_vars );
	#endif
	
	return num_vars;
}

// v_{a,i,d,u,s,hi,hf,t}
int MIPUnico::criaVariavelTaticoAlunoCreditosAPartirDeX_MaisFiltroAluno( int campusId, int P )
{
	int numVars = 0;
	
	std::map< VariableMIPUnico, Trio<double, double, double> > var_Bounds;

	std::map< int/*turma*/, std::map< Disciplina*, std::map< ConjuntoSala*, std::map< int/*dia*/, map<int/*id*/, VariableMIPUnico> >,
		LessPtr<ConjuntoSala> >, LessPtr<Disciplina> > > varsX;

	VariableMIPUnicoHash::iterator vit = vHashTatico.begin();
	for ( ; vit != vHashTatico.end(); vit++)
	{		
		VariableMIPUnico x = vit->first;

		if( x.getType() != VariableMIPUnico::V_CREDITOS )
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
		
	for ( auto itTurma = varsX.begin(); itTurma != varsX.end(); itTurma++ )
	{		
		int turma = itTurma->first;
		auto *mapDiscs = & itTurma->second;

		for ( auto itDisc = (*mapDiscs).begin(); itDisc != (*mapDiscs).end(); itDisc++ )
		{	
			Disciplina* disciplina = itDisc->first;
			auto *mapCjtSala = & itDisc->second;
			
			GGroup<AlunoDemanda*,LessPtr<AlunoDemanda>> *alunosDemanda =
				& this->mapDiscAlunosDemanda[ disciplina->getId() ];

			ITERA_GGROUP_LESSPTR ( itAlDem, (*alunosDemanda), AlunoDemanda )
			{
				Aluno *aluno = problemData->retornaAluno( itAlDem->getAlunoId() );

				#ifdef BUILD_COM_SOL_INICIAL

				bool fixoInit=false;
				int t = problemData->getSolTaticoInicial()->getTurma( aluno, campusId, disciplina, fixoInit );

				if ( t != turma && fixoInit )
					continue;

				#endif

				for ( auto itCjtSala = (*mapCjtSala).begin(); itCjtSala != (*mapCjtSala).end(); itCjtSala++ )
				{
					ConjuntoSala *cjtSala = itCjtSala->first;
					auto *mapDia = & itCjtSala->second;
				
					map< int/*dia*/, vector<VariableMIPUnico> > varsDiscAluno;					
					map< int/*dia*/, int/*nroCreds*/ > mapDiaCredsLivres;
					int nCredsLivres=0;
					bool tem_que_criar=false;	// s� para conferir se est� ok
					map< VariableMIPUnico, bool > fixaVarV;

					for ( auto itDia = (*mapDia).begin(); itDia != (*mapDia).end(); itDia++ )
					{
						int dia = itDia->first;
						auto *mapX = & itDia->second;

						int nCredsLivresDia=0;

						for ( auto itX = (*mapX).begin(); itX != (*mapX).end(); itX++ )
						{
							int idX = itX->first;
							VariableMIPUnico x = itX->second;
		
							Unidade *unidade = x.getUnidade();		
							HorarioAula *hi = x.getHorarioAulaInicial();
							HorarioAula *hf = x.getHorarioAulaFinal();
							DateTime di = x.getDateTimeInicial();
							DateTime df = x.getDateTimeFinal();
														
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
							
							VariableMIPUnico v;
							v.reset();
							v.setType( VariableMIPUnico::V_ALUNO_CREDITOS );
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
							
							if ( var_Bounds.find( v ) == var_Bounds.end() )
							{
								bool fixar=false;

								if( !PERMITIR_REALOCAR_ALUNO )
								if ( !this->criaVariavelTaticoInt( &v, fixar, P ) )
								{
									continue;
								}
								
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
						auto itDiaV = varsDiscAluno.begin();
						for ( ; itDiaV != varsDiscAluno.end() ; itDiaV++ )
						{
							for ( int j=0; j < itDiaV->second.size(); j++ )
							{
								VariableMIPUnico v = itDiaV->second[j];

								if ( var_Bounds.find( v ) != var_Bounds.end() )
									continue;
												
								double coef = 0.0;					
								double lowerBound = 0.0;
								double upperBound = 1.0;

								if( !PERMITIR_REALOCAR_ALUNO )
								if ( fixaVarV[v] )
									lowerBound = 1.0;

								Trio<double, double, double> trio;
								trio.set( coef,lowerBound, upperBound );

								var_Bounds[ v ] = trio;
				
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
    
	
	int colNr = lp->getNumCols();

	// Insere todas as variaveis em vHashTatico e no lp
	// Faz isso separado, dado que a inser��o � no mesmo hash que estava sendo percorrido.
	for ( auto itAux = var_Bounds.begin(); itAux != var_Bounds.end(); itAux++)
	{		
		const VariableMIPUnico v = itAux->first;
				
		const double coef = itAux->second.first;
		const double lowerBound = itAux->second.second;
		const double upperBound = itAux->second.third;
		
		vHashTatico[ v ] = colNr;

		// ---------------------------------------
		#pragma region Preenche vars_aluno_aula
		Campus *cp = problemData->refCampus[v.getUnidade()->getIdCampus()];
		vars_aluno_aula[v.getAluno()][v.getDia()][v.getDateTimeInicial()][cp][v.getDisciplina()][v.getTurma()].insert(
			make_pair(colNr,v) );
		#pragma endregion

		// ---------------------------------------
		#pragma region Preenche vars_aluno_dia_dt
		pair<DateTime /*dti*/, DateTime /*dtf*/> *pairDtiDtf = nullptr;

		DateTime vInicio( v.getHorarioAulaInicial()->getInicio() );
		DateTime vFim( v.getHorarioAulaFinal()->getFinal() );

		auto finderAluno = vars_aluno_dia_dt.find(v.getAluno());
		if ( finderAluno != vars_aluno_dia_dt.end() )
		{
			auto finderDia = finderAluno->second.find(v.getDia());
			if ( finderDia != finderAluno->second.end() )
			{
				pairDtiDtf = & finderDia->second;
			}
		}
		if (pairDtiDtf==nullptr)
		{
			pairDtiDtf = & vars_aluno_dia_dt[v.getAluno()][v.getDia()];
			(*pairDtiDtf) = make_pair(vInicio, vFim);
		}
		else
		{
			(*pairDtiDtf) = make_pair( min( (*pairDtiDtf).first, vInicio ),
										max( (*pairDtiDtf).second, vFim ) );
		}
		#pragma endregion


		OPT_COL col( OPT_COL::VAR_BINARY, coef, lowerBound, upperBound,
			( char * )v.toString().c_str());

		lp->newCol( col );
		
		colNr++;
	}

	return numVars;

}

// v_{a,i,d,u,s,hi,hf,t}
int MIPUnico::criaVariavelTaticoAlunoCreditosAPartirDeX( int campusId, int P )
{
	int numVars = 0;

	std::map< VariableMIPUnico, Trio<double, double, double> > var_Bounds;

	VariableMIPUnicoHash::iterator vit = vHashTatico.begin();
	for ( ; vit != vHashTatico.end(); vit++)
	{		
		VariableMIPUnico v = vit->first;

		if( v.getType() != VariableMIPUnico::V_CREDITOS )
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
		DateTime di = v.getDateTimeInicial();
		DateTime df = v.getDateTimeFinal();

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

			VariableMIPUnico v;
			v.reset();
			v.setType( VariableMIPUnico::V_ALUNO_CREDITOS );

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

			if ( var_Bounds.find( v ) == var_Bounds.end() )
			{
				bool fixar=false;

				if ( !criaVariavelTaticoInt( &v, fixar, P ) )
				{
					continue;
				}
										
				double coef = 0.0;					
				double lowerBound = 0.0;
				double upperBound = 1.0;

				if ( fixar ) lowerBound = 1.0;
				
				Trio<double, double, double> trio;
				trio.set( coef,lowerBound, upperBound );

				var_Bounds[ v ] = trio;
				
				numVars++;
			}
		}
    }
    

	int colNr = lp->getNumCols();

	// Insere todas as variaveis em vHashTatico e no lp
	// Faz isso separado, dado que a inser��o � no mesmo hash que estava sendo percorrido.
	for ( auto itAux = var_Bounds.begin(); itAux != var_Bounds.end(); itAux++)
	{		
		VariableMIPUnico v = itAux->first;
				
		double coef = itAux->second.first;
		double lowerBound = itAux->second.second;
		double upperBound = itAux->second.third;
		
		vHashTatico[ v ] = colNr;
		
		// ---------------------------------------
		#pragma region Preenche vars_aluno_aula
		Campus *cp = problemData->refCampus[v.getUnidade()->getIdCampus()];
		vars_aluno_aula[v.getAluno()][v.getDia()][v.getDateTimeInicial()][cp][v.getDisciplina()][v.getTurma()].insert(
			make_pair(colNr,v) );
		#pragma endregion

		// ---------------------------------------
		#pragma region Preenche vars_aluno_dia_dt
		pair<DateTime /*dti*/, DateTime /*dtf*/> *pairDtiDtf = nullptr;

		DateTime vInicio( v.getHorarioAulaInicial()->getInicio() );
		DateTime vFim( v.getHorarioAulaFinal()->getFinal() );

		auto finderAluno = vars_aluno_dia_dt.find(v.getAluno());
		if ( finderAluno != vars_aluno_dia_dt.end() )
		{
			auto finderDia = finderAluno->second.find(v.getDia());
			if ( finderDia != finderAluno->second.end() )
			{
				pairDtiDtf = & finderDia->second;
			}
		}
		if (pairDtiDtf==nullptr)
		{
			pairDtiDtf = & vars_aluno_dia_dt[v.getAluno()][v.getDia()];
			(*pairDtiDtf) = make_pair(vInicio, vFim);
		}
		else
		{
			(*pairDtiDtf) = make_pair( min( (*pairDtiDtf).first, vInicio ),
										max( (*pairDtiDtf).second, vFim ) );
		}
		#pragma endregion

		OPT_COL col( OPT_COL::VAR_BINARY, coef, lowerBound, upperBound,
			( char * )v.toString().c_str());

		lp->newCol( col );
		
		colNr++;
	}

	return numVars;

}

// x_{i,d,u,s,hi,hf,t}
void MIPUnico::criaVariavelTaticoCreditosCopiadas( int campusId, int P, int &numVars )
{
	GGroup< VariableTatico *, LessPtr<VariableTatico> > solTaticoVarsX = *vars_xh;
	ITERA_GGROUP_LESSPTR( itVarX, solTaticoVarsX, VariableTatico )
	{
		VariableTatico x = **itVarX;
		
		if ( x.getUnidade()->getIdCampus() != campusId )
			continue;		

		VariableMIPUnico v;
		v.reset();
		v.setType( VariableMIPUnico::V_CREDITOS );

		v.setTurma( x.getTurma() );				 // i
		v.setDisciplina( x.getDisciplina() );	 // d
		v.setUnidade( x.getUnidade() );			 // u
		v.setSubCjtSala( x.getSubCjtSala() );	 // tps  
		v.setDia( x.getDia() );						// t
		v.setHorarioAulaInicial( x.getHorarioAulaInicial() );	 // hi
		v.setHorarioAulaFinal( x.getHorarioAulaFinal() );	 // hf
			
        std::pair<DateTime*,int> auxP = problemData->horarioAulaDateTime[x.getHorarioAulaInicial()->getId()];
        v.setDateTimeInicial(*auxP.first);
        auxP = problemData->horarioAulaDateTime[x.getHorarioAulaFinal()->getId()];
        v.setDateTimeFinal(*auxP.first);

		bool Ok = x.getDisciplina()->inicioTerminoValidos( x.getHorarioAulaInicial(), x.getHorarioAulaFinal(), x.getDia() );
		if (!Ok)
		{
			std::stringstream msg;
			msg << "Aula " << x.toString() << " criada, porem viola Disciplina::inicioTerminoValidos()";
			CentroDados::printError( "void MIPUnico::criaVariavelTaticoCreditosCopiadas()", msg.str() );
		}

		if ( vHashTatico.find( v ) == vHashTatico.end() )
		{
			vHashTatico[ v ] = lp->getNumCols();
									
			double lowerBound = 0.0;
			double upperBound = 1.0;
		
			if (this->PERMITIR_NOVAS_TURMAS || P>1)	// Se n�o forem abertas novas turmas, n�o fixa a fim de permitir a uni�o de turmas
				lowerBound = 1.0;

			OPT_COL col( OPT_COL::VAR_BINARY, 0.0, lowerBound, upperBound,
				( char * )v.toString().c_str());
			
			lp->newCol( col );
			numVars++;
		}		
	}
}

// x_{i,d,u,s,hi,hf,t}
int MIPUnico::criaVariavelTaticoCreditos( int campusId, int P )
{
	int numVars = 0;
	
	Campus *campus = nullptr;
	auto finderCp = problemData->refCampus.find(campusId);
	if (finderCp == problemData->refCampus.end())
		return numVars;
	campus = finderCp->second;

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

											if ( sala->getIdCampus() != campusId )
												continue;

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

	auto itMapDiscSalaDiaDtiDtf = mapDiscSalaDiaDtiDtf.begin();
	for( ; itMapDiscSalaDiaDtiDtf != mapDiscSalaDiaDtiDtf.end(); itMapDiscSalaDiaDtiDtf++ )
	{			
		Disciplina *disciplina = itMapDiscSalaDiaDtiDtf->first;
					
		if ( this->mapDiscAlunosDemanda[disciplina->getId()].size() == 0 )
			continue;
				
		int naoAtend = this->haFolgaDeAtendimento( disciplina );
		if ( naoAtend == 0 )
			continue;
		
		GGroup<int> turmasParaAbrir;
		for ( int turma = 1; turma <= disciplina->getNumTurmas(); turma++ )
			turmasParaAbrir.add(turma);

		auto *mapSalaDiaDtiDtf = & itMapDiscSalaDiaDtiDtf->second;

		auto itCjtSalaDiaDtiDtf = mapSalaDiaDtiDtf->begin();
		for( ; itCjtSalaDiaDtiDtf != mapSalaDiaDtiDtf->end(); itCjtSalaDiaDtiDtf++ )
		{
			ConjuntoSala* cjtSala = itCjtSalaDiaDtiDtf->first;
			
			int salaId = cjtSala->salas.begin()->first;
			int unidadeId = cjtSala->salas.begin()->second->getIdUnidade();
			Unidade *unidade = problemData->refUnidade[ unidadeId ];

			auto *mapDiaDtiDtf = & itCjtSalaDiaDtiDtf->second;

			for (auto itDia = mapDiaDtiDtf->begin(); itDia != mapDiaDtiDtf->end(); itDia++ )
			{
				int dia = itDia->first;							
				auto *mapDti = & itDia->second;

				for (auto itMapDti = mapDti->begin(); itMapDti != mapDti->end(); itMapDti++ )
				{
					DateTime dti = itMapDti->first;

					auto *mapDtf = & itMapDti->second;
										
					for ( auto itMapDtf = mapDtf->begin(); itMapDtf != mapDtf->end(); itMapDtf++ )
					{
						DateTime dtf = itMapDtf->first;

						std::pair<HorarioAula*, HorarioAula*> parHiHf = itMapDtf->second;
								
						HorarioAula *hi = parHiHf.first;
						HorarioAula *hf = parHiHf.second;
									 									
						ITERA_GGROUP_N_PT( itTurma, turmasParaAbrir, int )
						{									
							int turma = *itTurma;
																			
							VariableMIPUnico v;
							v.reset();
							v.setType( VariableMIPUnico::V_CREDITOS );

							v.setTurma( turma );            // i
							v.setDisciplina( disciplina );  // d
							v.setUnidade( unidade );     // u
							v.setSubCjtSala( cjtSala );  // tps  
							v.setDia( dia );				 // t
							v.setHorarioAulaInicial( hi );	 // hi
							v.setHorarioAulaFinal( hf );	 // hf

							std::pair<DateTime*,int> auxP;
							if (!problemData->getPairDateTime(hi->getId(), auxP))
							{
								stringstream ss;
								ss <<  "Erro em ProblemData::getPairDateTime(). HorarioAula "
									<< hi->getId() << " nao encontrado.";								
								CentroDados::printError("MIPUnico::criaVariavelTaticoCreditos()",ss.str());
								exit(1);
							}
							v.setDateTimeInicial(*auxP.first);
						
							if (!problemData->getPairDateTime(hf->getId(), auxP))
							{
								stringstream ss;
								ss <<  "Erro em ProblemData::getPairDateTime(). HorarioAula "
									<< hi->getId() << " nao encontrado.";								
								CentroDados::printError("MIPUnico::criaVariavelTaticoCreditos()",ss.str());
								exit(1);
							}
							v.setDateTimeFinal(*auxP.first);

							if ( vHashTatico.find( v ) == vHashTatico.end() )
							{						
								bool fixar=false;
								//if ( !criaVariavelTaticoInt( &v, fixar, P ) )
								//	continue;

								vHashTatico[ v ] = lp->getNumCols();
									
								double lowerBound = 0.0;
								double upperBound = 1.0;
								if (fixar) lowerBound = upperBound;
								
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
	#pragma endregion

	// Insere variaveis x criadas em map organizado s� com variaveis x
	VariableMIPUnicoHash::iterator vit = vHashTatico.begin();
	for ( ; vit != vHashTatico.end(); vit++)
	{		
		VariableMIPUnico x = vit->first;
		if( x.getType() != VariableMIPUnico::V_CREDITOS )
			continue;

		int turma = x.getTurma();
		Disciplina* disciplina = x.getDisciplina();
		ConjuntoSala *cjtSala = x.getSubCjtSala();
		Unidade* unidade = x.getUnidade();
		int dia = x.getDia();
		DateTime dti = x.getDateTimeInicial();
		
		int col = vit->second;
		
		vars_turma_aula[campus][disciplina][turma][dia][dti][col] = x;
		vars_turma_aula2[unidade][disciplina][turma][dia][dti][col] = x;
	}

	return numVars;
}

// x_{i,d,u,s,hi,hf,t}
int MIPUnico::criaVariavelTaticoCreditosComSolInicial( int campusId, int P )
{
	int numVars = 0;

	// CRIA TODAS AS VARIAVEIS X QUE J� EXISTEM OLHANDO PARA A SOLU��O ANTERIOR
	if ( this->CRIAR_VARS_FIXADAS )
	{
		criaVariavelTaticoCreditosCopiadas( campusId, P, numVars );
	}

	std::cout<<"\nCriei "<<numVars<<" variaveis x, copiadas da solucao anterior.\n"; fflush(NULL);

	// CRIA TODAS AS VARIAVEIS X DE TURMAS QUE AINDA N�O EXISTEM
	
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
						// variavel j� foi criada acima, olhando para a solu��o anterior;
						// n�o permite novas turmas;
						continue;
					}

					GGroup< ConjuntoSala*, LessPtr<ConjuntoSala> > cjtSalasPossiveis;
					std::map< int /*Dia*/, std::pair<HorarioAula*, HorarioAula*> > mapDiaHorariosIF;
					
					bool fixaSala = false;
					bool fixaDias = false;
					bool fixaHorarios = false;

					#ifdef BUILD_COM_SOL_INICIAL
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
								if ( aula->fixaDia() || fixaDias ) // Ou todos os dias ser�o fixados ou nenhum ser�
								{
									mapDiaHorariosIF[ aula->getDiaSemana() ] =
										std::make_pair( aula->getHorarioAulaInicial(), aula->getHorarioAulaFinal() );
									fixaDias = true;
								}
								if ( aula->fixaHi() && aula->fixaHf() || fixaHorarios ) // Ou todos os hor�rios ser�o fixados ou nenhum ser�
								{
									mapDiaHorariosIF[ aula->getDiaSemana() ] = 
										std::make_pair( aula->getHorarioAulaInicial(), aula->getHorarioAulaFinal());	
									fixaHorarios = true;
								}
							}
						}
					}
					#endif
					
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
								
								// Usa todas as op��es de hor�rios achados na associa��o disc-sala.

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

											VariableMIPUnico v;
											v.reset();
											v.setType( VariableMIPUnico::V_CREDITOS );

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
											v.setDateTimeInicial(*auxP.first);

											if ( debug )
												std::cout<<"... Procurando hf..."; fflush(NULL);

											auxP = problemData->horarioAulaDateTime[hf->getId()];
											v.setDateTimeFinal(*auxP.first);

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
							// Usa somente os hor�rios fixados da solu��o inicial.
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
																
								VariableMIPUnico v;
								v.reset();
								v.setType( VariableMIPUnico::V_CREDITOS );
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
								v.setDateTimeInicial(*auxP.first);

								if ( debug )
									std::cout<<" Procurando hf..."; fflush(NULL);

								auxP = problemData->horarioAulaDateTime[hf->getId()];
								v.setDateTimeFinal(*auxP.first);

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
int MIPUnico::criaVariavelTaticoOferecimentosAPartirDeX( int campusId, int P )
{
	int numVars = 0;
 
	std::map< VariableMIPUnico, Trio<double, double, double> > var_Bounds;

	VariableMIPUnicoHash::iterator vit = vHashTatico.begin();
	for ( ; vit != vHashTatico.end(); vit++)
	{		
		VariableMIPUnico x = vit->first;
		if( x.getType() != VariableMIPUnico::V_CREDITOS )
		{
			continue;
		}

		int turma = x.getTurma();
		Disciplina* disciplina = x.getDisciplina();
		Unidade *unidade = x.getUnidade();
		ConjuntoSala *cjtSala = x.getSubCjtSala();
			
		VariableMIPUnico v;
		v.reset();
        v.setType( VariableMIPUnico::V_OFERECIMENTO );
        v.setTurma( turma );            // i
        v.setDisciplina( disciplina );  // d
        v.setUnidade( unidade );     // u
        v.setSubCjtSala( cjtSala );  // tps

		if ( var_Bounds.find( v ) == var_Bounds.end() )
		{	
			double coef = 0.0;					
			double lowerBound = 0.0;
			double upperBound = 1.0;
							
			Trio<double, double, double> trio;
			trio.set( coef,lowerBound, upperBound );

			var_Bounds[ v ] = trio;
				
			numVars++;
		}
    }
    

	int colNr = lp->getNumCols();

	// Insere todas as variaveis em vHashTatico e no lp
	// Faz isso separado, dado que a inser��o � no mesmo hash que estava sendo percorrido.
	for ( auto itAux = var_Bounds.begin(); itAux != var_Bounds.end(); itAux++)
	{		
		VariableMIPUnico v = itAux->first;
				
		double coef = itAux->second.first;
		double lowerBound = itAux->second.second;
		double upperBound = itAux->second.third;
		
		vHashTatico[ v ] = colNr;

		OPT_COL col( OPT_COL::VAR_BINARY, coef, lowerBound, upperBound,
			( char * )v.toString().c_str());

		lp->newCol( col );
		
		colNr++;
	}
	
	return numVars;
}

// s_{i,d,cp,a}
int MIPUnico::criaVariavelTaticoAlocaAlunoTurmaDiscAPartirDeV( int campusId, int P )
{
	int numVars = 0;
	
	std::map< VariableMIPUnico, Trio<double, double, double> > var_Bounds;

	VariableMIPUnicoHash::iterator vit = vHashTatico.begin();
	for ( ; vit != vHashTatico.end(); vit++)
	{		
		VariableMIPUnico v = vit->first;
		if( v.getType() != VariableMIPUnico::V_ALUNO_CREDITOS )
			continue;

		int turma = v.getTurma();
		Disciplina* disciplina = v.getDisciplina();
		Unidade *unidade = v.getUnidade();
		Aluno *aluno = v.getAluno();

		Campus *cp = problemData->refCampus[ unidade->getIdCampus() ];
		AlunoDemanda *alDem = problemData->getAlunoDemandaEquiv( aluno, disciplina );

		if ( alDem == nullptr )
		{
			stringstream msg;
			msg << "\nErro: aluno-demanda nao encontrado. Disciplina " 
				<< disciplina->getId() << " Aluno " << aluno->getAlunoId();
			CentroDados::printError("MIPUnico::criaVariavelTaticoAlocaAlunoTurmaDiscAPartirDeV()", msg.str());
			exit(1);
		}
		
		if ( disciplina->getId() < 0 )
		{
			// S� cria associa��o entre aluno e turma pratica se essa turma possuir
			// turma teorica associada.
			if ( disciplina->getTurmasAssociadas( turma ).size() == 0 )
				continue;
		}

		VariableMIPUnico s;
		s.reset();
        s.setType( VariableMIPUnico::V_ALOCA_ALUNO_TURMA_DISC );
		s.setAluno( aluno );
		s.setDisciplina( disciplina );
		s.setTurma( turma );
		s.setCampus( cp );
		s.setAlunoDemanda( alDem );

		if ( var_Bounds.find( s ) == var_Bounds.end() )
		{										
			double coef = 0.0;					
			double lowerBound = 0.0;
			double upperBound = 1.0;
							
			Trio<double, double, double> trio;
			trio.set( coef,lowerBound, upperBound );

			var_Bounds[ s ] = trio;
				
			numVars++;
		}
    }

	int colNr = lp->getNumCols();

	// Insere todas as variaveis em vHashTatico e no lp
	// Faz isso separado, dado que a inser��o � no mesmo hash que estava sendo percorrido.
	for ( auto itAux = var_Bounds.begin(); itAux != var_Bounds.end(); itAux++)
	{		
		VariableMIPUnico v = itAux->first;
				
		double coef = itAux->second.first;
		double lowerBound = itAux->second.second;
		double upperBound = itAux->second.third;
		
		vHashTatico[ v ] = colNr;

		OPT_COL col( OPT_COL::VAR_BINARY, coef, lowerBound, upperBound,
			( char * )v.toString().c_str());

		lp->newCol( col );
		
		colNr++;
	}
    
	return numVars;
}

// b_{i,d,c,cp}
int MIPUnico::criaVariavelTaticoCursoAlunos( int campusId, int P )
{
	int numVars = 0;

    if ( problemData->parametros->permite_compartilhamento_turma_sel && 
	     problemData->parametros->nao_permite_compart_turma.size() == 0 )
    {
		return numVars;
    }
   
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
				VariableMIPUnico v;
				v.reset();
				v.setType( VariableMIPUnico::V_TURMA_ATEND_CURSO );

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
               coef = 0.0;

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

// m_{d,i,k}
int MIPUnico::criaVariavelTaticoCombinacaoDivisaoCreditoAPartirDeO( int campusId, int P )
{
	int numVars = 0;
	if ( MIPUnico::consideraDivCredDisc == 0 )
		return numVars;

	Campus *campus = problemData->refCampus[campusId];

	std::map< VariableMIPUnico, Trio<double, double, double> > var_Bounds;

	VariableMIPUnicoHash::iterator vit = vHashTatico.begin();
	for ( ; vit != vHashTatico.end(); vit++)
	{		
		VariableMIPUnico v = vit->first;

		if( v.getType() != VariableMIPUnico::V_OFERECIMENTO )
		{
			continue;
		}

		int turma = v.getTurma();
		Disciplina* disciplina = v.getDisciplina();
							  
	    if ( !v.getDisciplina()->possuiRegraCred() )
		    continue;

        for ( unsigned k = 0; k < disciplina->combinacao_divisao_creditos.size(); k++ )
        {
			VariableMIPUnico v;
			v.reset();
			v.setType( VariableMIPUnico::V_COMBINACAO_DIVISAO_CREDITO );
			v.setTurma( turma );            // i
			v.setDisciplina( disciplina );  // d
			v.setK( k );	                // k
            v.setCampus( campus );			// cp

			if ( var_Bounds.find( v ) == var_Bounds.end() )
			{				
				double coef = 0.0;					
				double lowerBound = 0.0;
				double upperBound = 1.0;
								
				Trio<double, double, double> trio;
				trio.set( coef,lowerBound, upperBound );

				var_Bounds[ v ] = trio;
				
				numVars++;
			}
		}
    }
    
	int colNr = lp->getNumCols();

	// Insere todas as variaveis em vHashTatico e no lp
	// Faz isso separado, dado que a inser��o � no mesmo hash que estava sendo percorrido.
	for ( auto itAux = var_Bounds.begin(); itAux != var_Bounds.end(); itAux++)
	{		
		VariableMIPUnico v = itAux->first;
				
		double coef = itAux->second.first;
		double lowerBound = itAux->second.second;
		double upperBound = itAux->second.third;
		
		vHashTatico[ v ] = colNr;

		OPT_COL col( OPT_COL::VAR_BINARY, coef, lowerBound, upperBound,
			( char * )v.toString().c_str());

		lp->newCol( col );
		
		colNr++;
	}

	return numVars;
}

// fkp_{d,i,t} e fkm_{d,i,t}
int MIPUnico::criaVariavelTaticoFolgaCombinacaoDivisaoCreditoAPartirDeO( int campusId, int P )
{
	int numVars = 0;
		
	if ( MIPUnico::consideraDivCredDisc != 1 )
	   return numVars;

	Campus *campus = problemData->refCampus[campusId];

	std::map< VariableMIPUnico, Trio<double, double, double> > var_Bounds;

	VariableMIPUnicoHash::iterator vit = vHashTatico.begin();
	for ( ; vit != vHashTatico.end(); vit++)
	{		
		VariableMIPUnico v = vit->first;

		if( v.getType() != VariableMIPUnico::V_OFERECIMENTO )
			continue;

		int turma = v.getTurma();
		Disciplina* disciplina = v.getDisciplina();
											  
	    if ( !v.getDisciplina()->possuiRegraCred() )
		    continue;

		double lowerBound = 0.0;
		double upperBound = 100.0;												
		double coef = 0.0;
		if ( problemData->parametros->funcao_objetivo == 0 )
			coef = -1;
		else if ( problemData->parametros->funcao_objetivo == 1 )
			coef = 1;
							
		Trio<double, double, double> trio;
		trio.set( coef,lowerBound, upperBound );

		ITERA_GGROUP_N_PT( itDia, disciplina->diasLetivos, int )
        {	
			int dia = *itDia;

			v.reset();
			v.setType( VariableMIPUnico::V_SLACK_COMBINACAO_DIVISAO_CREDITO_P );
			v.setTurma( turma );            // i
			v.setDisciplina( disciplina );  // d
			v.setDia( dia );				// t
			v.setCampus( campus );				// cp

			if ( var_Bounds.find( v ) == var_Bounds.end() )
			{
				var_Bounds[ v ] = trio;				
				numVars++;
			}

			v.setType( VariableMIPUnico::V_SLACK_COMBINACAO_DIVISAO_CREDITO_M );
        
			if ( var_Bounds.find( v ) == var_Bounds.end() )
			{
				var_Bounds[ v ] = trio;				
				numVars++;
			}
		}
    }
    
	int colNr = lp->getNumCols();

	// Insere todas as variaveis em vHashTatico e no lp
	// Faz isso separado, dado que a inser��o � no mesmo hash que estava sendo percorrido.
	for ( auto itAux = var_Bounds.begin(); itAux != var_Bounds.end(); itAux++)
	{		
		VariableMIPUnico v = itAux->first;
				
		double coef = itAux->second.first;
		double lowerBound = itAux->second.second;
		double upperBound = itAux->second.third;
		
		vHashTatico[ v ] = colNr;

		OPT_COL col( OPT_COL::VAR_INTEGRAL, coef, lowerBound, upperBound,
			( char * )v.toString().c_str());

		lp->newCol( col );
		
		colNr++;
	}


	return numVars;
}

// zc_{d,t,cp}
int MIPUnico::criaVariavelTaticoAberturaCompativelAPartirDeX( int campusId, int P )
{
	int numVars = 0;

	if ( ! problemData->parametros->considerar_disciplinas_incompativeis_no_dia )
	   return numVars;
	
	Campus * cp = problemData->refCampus[campusId];
	
	std::map< VariableMIPUnico, Trio<double, double, double> > var_Bounds;

	VariableMIPUnicoHash::iterator vit = vHashTatico.begin();
	for ( ; vit != vHashTatico.end(); vit++)
	{		
		VariableMIPUnico v = vit->first;

		if( v.getType() != VariableMIPUnico::V_CREDITOS )
		{
			continue;
		}

		if ( v.getUnidade()->getIdCampus() != campusId )
			continue;

		Disciplina* disciplina = v.getDisciplina();
		int dia = v.getDia();

		v.reset();
        v.setType( VariableMIPUnico::V_ABERTURA_COMPATIVEL );        
        v.setDisciplina( disciplina );  // d
        v.setDia( dia );				// t
		v.setCampus( cp );				// cp

		if ( var_Bounds.find( v ) == var_Bounds.end() )
		{													
			double lowerBound = 0.0;
			double upperBound = 1.0;

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
			
			var_Bounds[ v ] = trio;
				
			numVars++;
		}
    }

	int colNr = lp->getNumCols();

	// Insere todas as variaveis em vHashTatico e no lp
	// Faz isso separado, dado que a inser��o � no mesmo hash que estava sendo percorrido.
	for ( auto itAux = var_Bounds.begin(); itAux != var_Bounds.end(); itAux++)
	{		
		VariableMIPUnico v = itAux->first;
				
		double coef = itAux->second.first;
		double lowerBound = itAux->second.second;
		double upperBound = itAux->second.third;
		
		vHashTatico[ v ] = colNr;

		OPT_COL col( OPT_COL::VAR_BINARY, coef, lowerBound, upperBound,
			( char * )v.toString().c_str());

		lp->newCol( col );
		
		colNr++;
	}

	return numVars;
}

// fd_{d,a}
int MIPUnico::criaVariavelTaticoFolgaDemandaDiscAluno( int campusId, int P )
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

			VariableMIPUnico v;
			v.reset();
			v.setType( VariableMIPUnico::V_SLACK_DEMANDA_ALUNO );

			v.setDisciplina( disciplina );  // d
			v.setAluno( aluno );			// a

			if ( vHashTatico.find( v ) == vHashTatico.end() )
			{
				bool fixar=false;

				if ( !criaVariavelTaticoInt( &v, fixar, P ) )
					continue;

									
				double lowerBound = 0.0;
				double upperBound = 1.0;

				if ( fixar ) lowerBound = 1.0;
								
				double coef = 0.0;
				if ( itAlDemanda->getPrioridade() > 1 )
				{
					coef = 0.0; // para P2, quem controla o custo da folga de demanda s�o fpi e fps
				}
				else
				{
					if ( problemData->parametros->funcao_objetivo == 0 )
					{
						coef = - 5000 * disciplina->getTotalCreditos() * itAlDemanda->demanda->oferta->getReceita();
					}
					else if ( problemData->parametros->funcao_objetivo == 1 )
					{
						coef = 1000 * disciplina->getTotalCreditos() * itAlDemanda->demanda->oferta->getReceita();
					}
				}

				vHashTatico[ v ] = lp->getNumCols();
								
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
int MIPUnico::criaVariavelTaticoFolgaAlunoUnidDifDia( int campusId, int P )
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
							VariableMIPUnico v;
							v.reset();
							v.setType( VariableMIPUnico::V_SLACK_ALUNO_VARIAS_UNID_DIA );

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

int MIPUnico::criaVariavelProfessorDiaHorarioIF()
{
   int num_vars = 0;

   if (problemData->parametros->proibirProfGapMTN == ParametrosPlanejamento::Off)
	   return num_vars;

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
			
         VariableMIPUnico v;
            v.reset();
            v.setProfessor( *itProfessor );
            v.setDia( dia );
			
			v.setType( VariableMIPUnico::V_HI_PROFESSORES );			
						
			if ( problemData->parametros->funcao_objetivo == 0 ) // max
			{
				coeff = 3.0;
			}
			else if ( problemData->parametros->funcao_objetivo == 1 ) // min
			{
				coeff = -3.0;
			}              

            if ( vHashTatico.find( v ) == vHashTatico.end() )
            {
               vHashTatico[ v ] = lp->getNumCols();
			   
               OPT_COL col( OPT_COL::VAR_INTEGRAL, peso*coeff, lb, ub,
                  ( char * ) v.toString().c_str() );

               lp->newCol( col );
               num_vars++;
            }

            v.setType( VariableMIPUnico::V_HF_PROFESSORES );
			
			if ( problemData->parametros->funcao_objetivo == 0 ) // max
			{
				coeff = -3.0;
			}
			else if ( problemData->parametros->funcao_objetivo == 1 ) // min
			{
				coeff = 3.0;
			}     

            if ( vHashTatico.find( v ) == vHashTatico.end() )
            {
               vHashTatico[ v ] = lp->getNumCols();

               OPT_COL col( OPT_COL::VAR_INTEGRAL, peso*coeff, lb, ub,
                  ( char * ) v.toString().c_str() );

               lp->newCol( col );
               num_vars++;
            }
	   }
   }

   return num_vars;
}

// du_{a,t}
int MIPUnico::criaVariavelTaticoDiaUsadoPeloAluno( int campusId, int P )
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

			VariableMIPUnico v;
			v.reset();
			v.setType( VariableMIPUnico::V_ALUNO_DIA );
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
int MIPUnico::criaVariavelTaticoFolgaAbreTurmaSequencial( int campusId, int P )
{
	int numVars = 0;
	
	// N�o cria nenhuma var, porque nesse modelo atualmente n�o h� casos
	// de fechamento de turmas em pos processamento.
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
            VariableMIPUnico v;
            v.reset();
            v.setType( VariableMIPUnico::V_SLACK_ABERT_SEQ_TURMA );

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
int MIPUnico::criaVariavelFolgaProibeCompartilhamento( int campusId, int P )
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
			    
				// A variavel de folga s� � criada para cursos compativeis e diferentes entre si
				// Ofertas para mesmo curso sempre poder�o compartilhar
				// Ofertas de cursos distintos s� poder�o compartilhar se forem compativeis e o compartilhamento estiver permitido
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
					   VariableMIPUnico v;
					   v.reset();
					   v.setType( VariableMIPUnico::V_SLACK_COMPARTILHAMENTO );
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
// S� para P2 em diante
int MIPUnico::criaVariavelFolgaPrioridadeInf( int campusId, int prior )
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

		VariableMIPUnico v;
		v.reset();
		v.setType( VariableMIPUnico::V_SLACK_PRIOR_INF );
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
			
			if ( totalCreditosP2 == 0 ) // se n�o houver demanda de P2 para o aluno
			{
				continue;
			}

			// Limita o tanto de carga hor�ria do aluno que pode ser excedido em P2 em 2 cr�ditos.
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
// S� para P2 em diante
int MIPUnico::criaVariavelFolgaPrioridadeSup( int campusId, int prior )
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

		VariableMIPUnico v;
		v.reset();
		v.setType( VariableMIPUnico::V_SLACK_PRIOR_SUP );
		v.setAluno( aluno );
		v.setCampus( cp );

		if ( vHashTatico.find( v ) == vHashTatico.end() )
		{
			int totalCreditos = aluno->getNroCreditosNaoAtendidos();
			double cargaHorariaNaoAtendida = problemData->cargaHorariaNaoAtendidaPorPrioridade( 1, aluno->getAlunoId() );	
			if ( cargaHorariaNaoAtendida <= 0 ) // se n�o houver folga de demanda de P1
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

// z_{i,d,cp}
int MIPUnico::criaVariavelTaticoAberturaAPartirDeX( int campusId, int prior )
{
	int numVars = 0;

	Campus *campus = problemData->refCampus[campusId];	   
	
	std::map< VariableMIPUnico, Trio<double, double, double> > var_Bounds;

	VariableMIPUnicoHash::iterator vit = vHashTatico.begin();
	for ( ; vit != vHashTatico.end(); vit++)
	{		
		VariableMIPUnico x = vit->first;

		if( x.getType() != VariableMIPUnico::V_CREDITOS )
		{
			continue;
		}

		if ( x.getUnidade()->getIdCampus() != campusId )
			continue;
		
		int turma = x.getTurma();
		Disciplina* disciplina = x.getDisciplina();
				
		VariableMIPUnico v;
		v.reset();
		v.setType( VariableMIPUnico::V_ABERTURA );
        v.setTurma( turma );            // i
        v.setDisciplina( disciplina );  // d
        v.setCampus( campus );			// cp

		if ( var_Bounds.find( v ) == var_Bounds.end() )
		{
			double lowerBound = 0.0;
			double upperBound = 1.0;
			
			double coef = disciplina->getTotalCreditos();
			Trio<double, double, double> trio;
			trio.set( coef,lowerBound, upperBound );

			var_Bounds[ v ] = trio;

			numVars++;
		}
    }
    
	int colNr = lp->getNumCols();

	// Insere todas as variaveis em vHashTatico e no lp
	// Faz isso separado, dado que a inser��o � no mesmo hash que estava sendo percorrido.
	for ( auto itAux = var_Bounds.begin(); itAux != var_Bounds.end(); itAux++)
	{		
		VariableMIPUnico v = itAux->first;
				
		double coef = itAux->second.first;
		double lowerBound = itAux->second.second;
		double upperBound = itAux->second.third;
		
		vHashTatico[ v ] = colNr;

		OPT_COL col( OPT_COL::VAR_BINARY, coef, lowerBound, upperBound,
			( char * )v.toString().c_str());

		lp->newCol( col );

		// Insere a variavel em map organizado s� com variaveis z
		vars_abertura_turma[v.getCampus()][v.getDisciplina()][v.getTurma()] = make_pair(colNr, v);

		colNr++;
	}

	return numVars;
}

// ss_{a1,a2,dp}
int MIPUnico::criaVariavelTaticoAlunosMesmaTurmaPratica( int campusId, int P )
{
	int numVars = 0;
	
	// rela��o 1xN
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
			if ( turmaAluno == -1 && problemData->parametros->considerar_equivalencia_por_aluno ) // aluno n�o alocado
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

				VariableMIPUnico v;
				v.reset();
				v.setType( VariableMIPUnico::V_ALUNOS_MESMA_TURMA_PRAT );
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
int MIPUnico::criaVariavelTaticoFolgaMinimoDemandaPorAluno( int campusId, int P_ATUAL )
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

		VariableMIPUnico v;
		v.reset();
		v.setType( VariableMIPUnico::V_FOLGA_ALUNO_MIN_ATEND1 );
		v.setAluno( aluno );
		
		if ( vHashTatico.find( v ) == vHashTatico.end() )
		{
			vHashTatico[v] = lp->getNumCols();
			
			OPT_COL col( OPT_COL::VAR_CONTINUOUS, coef1, 0.0, ub1, ( char * )v.toString().c_str() );
			lp->newCol(col);
				
			numVars++;
		}		

		v.reset();
		v.setType( VariableMIPUnico::V_FOLGA_ALUNO_MIN_ATEND2 );
		v.setAluno( aluno );
		
		if ( vHashTatico.find( v ) == vHashTatico.end() )
		{
			vHashTatico[v] = lp->getNumCols();
			
			OPT_COL col( OPT_COL::VAR_CONTINUOUS, coef2, 0.0, ub2, ( char * )v.toString().c_str() );
			lp->newCol(col);
				
			numVars++;
		}		

		v.reset();
		v.setType( VariableMIPUnico::V_FOLGA_ALUNO_MIN_ATEND3 );
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
int MIPUnico::criaVariavelFolgaOcupacaoSala( int campusId, int P_ATUAL )
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
            VariableMIPUnico v;
            v.reset();
            v.setType( VariableMIPUnico::V_FOLGA_OCUPA_SALA );

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


// y_{p,i,d,cp}
int MIPUnico::criaVariavelProfTurmaAPartirDeZ()
{
	int numVars = 0;
		
	std::map<Campus*, std::map<Disciplina*, std::set<Professor*>>> mapCpDiscProfReaisPVUnico;

	for ( auto itCp = vars_turma_aula.begin(); itCp != vars_turma_aula.end(); itCp++ )
	{
		for ( auto itDisc = itCp->second.begin(); itDisc != itCp->second.end(); itDisc++ )
		{
			for ( auto itProf = itCp->first->professores.begin();
					itProf != itCp->first->professores.end(); itProf++ )
			{
				if ( itProf->possuiMagisterioEm( itDisc->first ) )
					mapCpDiscProfReaisPVUnico[itCp->first][itDisc->first].insert( *itProf );
			}

			if ( MIPUnico::permiteCriarPV )
			{
				ITERA_GGROUP_LESSPTR( itPV, problemData->profsVirtuais, Professor )
				{
					mapCpDiscProfReaisPVUnico[itCp->first][itDisc->first].insert( *itPV );
				}
			}
		}
	}

	auto itCp = vars_abertura_turma.begin();
	for ( ; itCp != vars_abertura_turma.end(); itCp++)
	{		
		Campus * const campus = itCp->first;

		auto itDisc = itCp->second.begin();
		for ( ; itDisc != itCp->second.end(); itDisc++)
		{		
			Disciplina * const disciplina = itDisc->first;

			// ----------------------------------
			// Procura professores v�lidos
			std::set<Professor*> *allProfsValidos = nullptr;
			auto cpFinder = mapCpDiscProfReaisPVUnico.find(campus);
			if ( cpFinder != mapCpDiscProfReaisPVUnico.end() )
			{
				auto discFinder = cpFinder->second.find(disciplina);
				if ( discFinder != cpFinder->second.end() )
					allProfsValidos = & discFinder->second;
			}

			if ( allProfsValidos==nullptr ) continue;
			// ----------------------------------

			for ( auto itProf = allProfsValidos->begin(); itProf != allProfsValidos->end(); itProf++ )
			{
				Professor * const professor = *itProf;

				auto itTurma = itDisc->second.begin();
				for ( ; itTurma != itDisc->second.end(); itTurma++)
				{
					int const turma = itTurma->first;
									
					VariableMIPUnico v;
					v.reset();
					v.setType( VariableMIPUnico::V_PROF_TURMA );
					v.setTurma( turma );            // i
					v.setDisciplina( disciplina );  // d
					v.setCampus( campus );			// cp
					v.setProfessor( professor );	// p
												
					if ( vHashTatico.find( v ) == vHashTatico.end() )
					{
						int const colNr = lp->getNumCols();
						vHashTatico[v] = colNr;
						vars_prof_turma[campus][disciplina][turma][professor] = make_pair(colNr,v);

						double lowerBound = 0.0;
						double upperBound = 1.0;
						double coef = 0;
																			
						if ( professor->eVirtual() )
							coef = disciplina->getTotalCreditos();
						if ( MIPUnico::minimizarCustoProf )
							coef = disciplina->getTotalCreditos() * professor->getValorCredito();

						OPT_COL col( OPT_COL::VAR_BINARY, coef, lowerBound, upperBound, ( char * )v.toString().c_str());

						lp->newCol( col );
						numVars++;
					}
				}
			}
		}
    }

	return numVars;
}


// k_{p,i,d,u,t,h}
int MIPUnico::criaVariavelProfAulaAPartirDeX()
{
	int numVars = 0;

	std::map<Campus*, std::map<Disciplina*, std::set<Professor*>>> mapCpDiscProfReaisPVUnico;

	for ( auto itCp = vars_turma_aula.begin(); itCp != vars_turma_aula.end(); itCp++ )
	{
		for ( auto itDisc = itCp->second.begin(); itDisc != itCp->second.end(); itDisc++ )
		{
			for ( auto itProf = itCp->first->professores.begin();
					itProf != itCp->first->professores.end(); itProf++ )
			{
				if ( itProf->possuiMagisterioEm( itDisc->first ) )
					mapCpDiscProfReaisPVUnico[itCp->first][itDisc->first].insert( *itProf );
			}

			if ( MIPUnico::permiteCriarPV )
			{
				ITERA_GGROUP_LESSPTR( itPV, problemData->profsVirtuais, Professor )
				{
					mapCpDiscProfReaisPVUnico[itCp->first][itDisc->first].insert( *itPV );
				}
			}
		}
	}

	for ( auto itCp = vars_turma_aula.begin(); itCp != vars_turma_aula.end(); itCp++ )
	{
		for ( auto itDisc = itCp->second.begin(); itDisc != itCp->second.end(); itDisc++ )
		{
			// ----------------------------------
			// Procura professores v�lidos
			std::set<Professor*> *allProfsValidos = nullptr;
			auto cpFinder = mapCpDiscProfReaisPVUnico.find(itCp->first);
			if ( cpFinder != mapCpDiscProfReaisPVUnico.end() )
			{
				auto discFinder = cpFinder->second.find(itDisc->first);
				if ( discFinder != cpFinder->second.end() )
					allProfsValidos = & discFinder->second;
			}

			if ( allProfsValidos==nullptr ) continue;
			// ----------------------------------

			for ( auto itProf = allProfsValidos->begin(); itProf != allProfsValidos->end(); itProf++ )
			{
				Professor *professor = *itProf;
			
				for ( auto itTurma = itDisc->second.begin(); itTurma != itDisc->second.end(); itTurma++ )
				{
					for ( auto itDia = itTurma->second.begin(); itDia != itTurma->second.end(); itDia++ )
					{
						for ( auto itDti = itDia->second.begin(); itDti != itDia->second.end(); itDti++ )
						{
							for ( auto itVar = itDti->second.begin(); itVar != itDti->second.end(); itVar++ )
							{
								int colNr = itVar->first;
								VariableMIPUnico x = itVar->second;

								const int turma = x.getTurma();
								Disciplina * const disciplina = x.getDisciplina();
								const int dia = x.getDia();
								Unidade * const unidade = x.getUnidade();
								HorarioAula * const hf = x.getHorarioAulaFinal();
								int fase = CentroDados::getFaseDoDia( x.getHorarioAulaInicial()->getInicio() );

								if ( !professor->eVirtual() &&
									 !professor->possuiHorariosNoDia( x.getHorarioAulaInicial(), x.getHorarioAulaFinal(), dia ) )
									continue;

								auto *map1 = & vars_prof_aula1[unidade][disciplina][turma][professor][dia];
								auto *map2 = & vars_prof_aula2[professor][dia];
								auto *map3 = & vars_prof_aula3[professor][dia][unidade];
								
																
								// ---------------------------------------
								#pragma region Preenche vars_prof_dia_dt
								pair<DateTime /*dti*/, DateTime /*dtf*/> *pairDtiDtf = nullptr;

								DateTime menorInicio( x.getHorarioAulaInicial()->getInicio() );
								DateTime maiorFim( x.getHorarioAulaFinal()->getFinal() );

								auto finderProf = vars_prof_dia_fase_dt.find(professor);
								if ( finderProf != vars_prof_dia_fase_dt.end() )
								{
									auto finderDia = finderProf->second.find(dia);
									if ( finderDia != finderProf->second.end() )
									{
										auto finderFase = finderDia->second.find(fase);
										if ( finderFase != finderDia->second.end() )
											pairDtiDtf = & finderFase->second;
									}
								}
								if (pairDtiDtf==nullptr)
								{
									pairDtiDtf = & vars_prof_dia_fase_dt[professor][dia][fase];
									(*pairDtiDtf) = make_pair(menorInicio, maiorFim);
								}
								else
								{
									(*pairDtiDtf) = make_pair( min( (*pairDtiDtf).first, menorInicio ),
															   max( (*pairDtiDtf).second, maiorFim ) );
								}
								#pragma endregion

								// ---------------------------------------
								HorarioAula *h = x.getHorarioAulaInicial();
								bool END = (h==nullptr);

								while ( !END )
								{
									DateTime xDti = h->getInicio();
									DateTime xDtf = h->getFinal();
									
									VariableMIPUnico v;
									v.reset();
									v.setType( VariableMIPUnico::V_PROF_AULA );
									v.setTurma( turma );				// i
									v.setDisciplina( disciplina );		// d
									v.setUnidade( unidade );			// u
									v.setDia( dia );					// t
									v.setProfessor( professor );		// p
									v.setDateTimeInicial(xDti);			// dti
									v.setDateTimeFinal(xDtf);			// dtf
									v.setHorarioAulaInicial(h);
														 
									if ( vHashTatico.find( v ) == vHashTatico.end() )
									{					
										const int colNr = lp->getNumCols();
										vHashTatico[ v ] = colNr;
			
										(*map1)[xDti] = make_pair(colNr,v);
										(*map2)[xDti][unidade][disciplina][turma] = make_pair(colNr,v);
										(*map3)[h].insert( make_pair(colNr,v) );

										double lowerBound = 0.0;
										double upperBound = 1.0;
										double coef = 0.0;
		
										OPT_COL col( OPT_COL::VAR_BINARY, coef, lowerBound, upperBound,
											( char * )v.toString().c_str());

										lp->newCol( col );
										numVars++;
									}
			
									h = h->getCalendario()->getProximoHorario(h);

									if ( h==nullptr )
										END=true;
									else if ( h->getInicio() > hf->getInicio() )
										END=true;
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

// hip_{p,t,f} e hfp_{p,t,f}
int MIPUnico::criaVariaveisHiHfProfFaseDoDiaAPartirDeK(void)
{
	// Cria vars inteiras que indicam o primeiro e o �ltimo hor�rio
	// da fase do dia (M/T/N) usados pelo professor
	
	int numVars = 0;
		
	if ( CentroDados::getProblemData()->parametros->proibirProfGapMTN ==
		 ParametrosPlanejamento::ConstraintLevel::Off )
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
				int dtiMinutes = dti.getDateMinutes();
				int dtfMinutes = dtf.getDateMinutes();

				double coef = 0.0;
				int lbi = dtiMinutes;
				int ubf = dtfMinutes;
				int ubi = ubf;
				int lbf = lbi;
				
				VariableMIPUnico var;
				var.reset();
				var.setProfessor(itProf->first);
				var.setDia(itDia->first);
				var.setFaseDoDia(itFase->first);
				
				// -----------------------------------------------
				// varProfHi
				var.setType( VariableMIPUnico::V_HI_PROFESSORES );				
				
				int colNrHi = lp->getNumCols();

				vHashTatico[ var ] = colNrHi;
				varsProfDiaFaseHiHf[itProf->first][itDia->first][itFase->first].first = colNrHi;

				OPT_COL colHi( OPT_COL::VAR_INTEGRAL, coef, lbi, ubi,
					( char * ) var.toString().c_str() );

				lp->newCol( colHi );
				numVars++;
				
				// -----------------------------------------------
				// varProfHf
				var.setType( VariableMIPUnico::V_HF_PROFESSORES );
				
				int colNrHf = lp->getNumCols();

				vHashTatico[ var ] = colNrHf;
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
int MIPUnico::criaVariavelFolgaGapProfAPartirDeK(void)
{
	// Cria var inteira de folga que indica o n�mero de minutos que
	// violaram a restri��o de gap na fase do dia de um dia do professor
	
	int numVars = 0;
		
	if ( CentroDados::getProblemData()->parametros->proibirProfGapMTN !=
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

				const double coef = MIPUnico::pesoGapProf;
				const int lb = 0;
				const int ub = dtfMinutes - dtiMinutes;
			
				VariableMIPUnico var;
				var.reset();
				var.setType( VariableMIPUnico::V_FOLGA_GAP_PROF );	// fpgap_{p,t,f}
				var.setProfessor(itProf->first);
				var.setDia(itDia->first);
				var.setFaseDoDia(itFase->first);

				const int colNr = lp->getNumCols();

				vHashTatico[ var ] = colNr;
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

// hia_{a,t} e hfa_{a,t}
int MIPUnico::criaVariaveisHiHfAlunoDiaAPartirDeV(void)
{
	// Cria vars inteiras que indicam o primeiro e o �ltimo hor�rio
	// do dia (em minutos) usados pelo aluno
	
	int numVars = 0;
		
	if ( CentroDados::getProblemData()->parametros->proibirAlunoGap ==
		 ParametrosPlanejamento::ConstraintLevel::Off )
		return numVars;

	// min dti e max dtf para cada aluno/dia => obtido a partir de variaveis v_{a,i,d,s,t,hi,hf}
	for(auto itAluno = vars_aluno_dia_dt.begin(); itAluno != vars_aluno_dia_dt.end(); ++itAluno)
	{
		Aluno * const aluno = itAluno->first;

		for(auto itDia = itAluno->second.begin(); itDia != itAluno->second.end(); ++itDia)
		{
			const int dia = itDia->first;

			DateTime dti = itDia->second.first;
			DateTime dtf = itDia->second.second;
			const int dtiMinutes = dti.getDateMinutes();
			const int dtfMinutes = dtf.getDateMinutes();

			const double coef = 0.0;
			const int lbi = dtiMinutes;
			const int ubf = dtfMinutes;
			const int ubi = ubf;
			const int lbf = lbi;
				
			VariableMIPUnico var;
			var.reset();
			var.setAluno(aluno);
			var.setDia(dia);
				
			// -----------------------------------------------
			// varAlunoHi
			var.setType( VariableMIPUnico::V_HI_ALUNOS );				
				
			const int colNrHi = lp->getNumCols();

			vHashTatico[ var ] = colNrHi;
			varsAlunoDiaHiHf[aluno][dia].first = colNrHi;

			OPT_COL colHi( OPT_COL::VAR_INTEGRAL, coef, lbi, ubi,
				( char * ) var.toString().c_str() );

			lp->newCol( colHi );
			numVars++;
			
			// -----------------------------------------------
			// varAlunoHf
			var.setType( VariableMIPUnico::V_HF_ALUNOS );
				
			const int colNrHf = lp->getNumCols();

			vHashTatico[ var ] = colNrHf;
			varsAlunoDiaHiHf[aluno][dia].second = colNrHf;

			OPT_COL colHf( OPT_COL::VAR_INTEGRAL, coef, lbf, ubf,
				( char * ) var.toString().c_str() );

			lp->newCol( colHf );
			numVars++;
		}
	}
	return numVars;
}

// fagap_{a,t}
int MIPUnico::criaVariavelFolgaGapAlunoAPartirDeV(void)
{
	// Cria var inteira de folga que indica o n�mero de minutos que
	// violaram a restri��o de gap no dia do aluno
	
	int numVars = 0;
		
	if ( CentroDados::getProblemData()->parametros->proibirAlunoGap !=
		 ParametrosPlanejamento::ConstraintLevel::Weak )
		return numVars;

	// min dti e max dtf para cada aluno/dia => obtido a partir de variaveis v_{a,i,d,s,t,hi,hf}
	for(auto itAluno = vars_aluno_dia_dt.begin(); itAluno != vars_aluno_dia_dt.end(); ++itAluno)
	{
		Aluno * const aluno = itAluno->first;

		for(auto itDia = itAluno->second.begin(); itDia != itAluno->second.end(); ++itDia)
		{
			const int dia = itDia->first;

			DateTime dti = itDia->second.first;
			DateTime dtf = itDia->second.second;
			const int dtiMinutes = dti.getDateMinutes();
			const int dtfMinutes = dtf.getDateMinutes();

			const double coef = MIPUnico::pesoGapAluno;
			const int lb = 0;
			const int ub = dtfMinutes - dtiMinutes;
			
			VariableMIPUnico var;
			var.reset();
			var.setType( VariableMIPUnico::V_FOLGA_GAP_ALUNOS );	// fagap_{a,t}
			var.setAluno(aluno);
			var.setDia(dia);
				
			const int colNr = lp->getNumCols();

			vHashTatico[ var ] = colNr;
			varsAlunoFolgaGap[aluno][dia] = colNr;

			OPT_COL col( OPT_COL::VAR_INTEGRAL, coef, lb, ub,
				( char * ) var.toString().c_str() );

			lp->newCol( col );
			numVars++;			
		}
	}
	return numVars;
}

// fcad_{a,t}
int MIPUnico::criarVariavelFolgaMinCredsDiaAluno()
{
	// Folga do m�nimo de cr�ditos do aluno no dia
	int numVars = 0;
	
	if ( MIPUnico::considerarMinCredDiaAluno != 1 )
		return numVars;

	for(auto itAluno = vars_aluno_aula.begin(); itAluno != vars_aluno_aula.end(); ++itAluno)
	{
		Aluno * const aluno = itAluno->first;

		int maxFolga = aluno->getNrMedioCredsDia() - MIPUnico::desvioMinCredDiaAluno;

		const double coef = MIPUnico::pesoMinCredDiaAluno;
		const int lb = 0;
		const int ub = max(maxFolga, 0);
			
		for(auto itDia = itAluno->second.begin(); itDia != itAluno->second.end(); ++itDia)
		{
			const int dia = itDia->first;

			VariableMIPUnico var;
			var.reset();
			var.setType( VariableMIPUnico::V_FOLGA_MIN_CRED_DIA_ALUNO );	// fcad_{a,t}
			var.setAluno(aluno);
			var.setDia(dia);
				
			const int colNr = lp->getNumCols();

			vHashTatico[ var ] = colNr;
			
			OPT_COL col( OPT_COL::VAR_INTEGRAL, coef, lb, ub,
				( char * ) var.toString().c_str() );

			lp->newCol( col );
			numVars++;			
		}
	}

	return numVars;
}

// fch_{p}
int MIPUnico::criaVariavelFolgaCargaHorariaAnteriorProfessor()
{
   int num_vars = 0;
   double coeff = 10.0;

   if ( !problemData->parametros->evitar_reducao_carga_horaria_prof )
      return num_vars;

   GGroup< Professor *, LessPtr< Professor > > professores
      = problemData->getProfessores();
   ITERA_GGROUP_LESSPTR( it_prof, professores, Professor )
   {
      VariableMIPUnico v;
      v.reset();
      v.setType( VariableMIPUnico::V_F_CARGA_HOR_ANT_PROF );
      v.setProfessor( *it_prof );

      if ( vHashTatico.find( v ) == vHashTatico.end() )
      {
         vHashTatico[ v ] = lp->getNumCols();
		 
		 int ub = (*it_prof)->getChAnterior();

		 OPT_COL col( OPT_COL::VAR_CONTINUOUS, coeff, 0.0, ub,
            ( char * )v.toString().c_str() );

         lp->newCol( col );
         num_vars++;
      }
   }

   return num_vars;
}


/* ----------------------------------------------------------------------------------
	
							RESTRICOES TATICO POR ALUNO COM HORARIOS
 ---------------------------------------------------------------------------------- */


int MIPUnico::criaRestricoesTatico( int campusId, int prioridade, int r )
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
	std::cout << "numRest criaRestricaoTaticoDivisaoCredito_hash: " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
#endif


	timer.start();
	restricoes += criaRestricaoTaticoCombinacaoDivisaoCredito( campusId );			// Restricao 1.2.9
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest criaRestricaoTaticoCombinacaoDivisaoCredito: " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
#endif
      

	timer.start();
	restricoes += criaRestricaoTaticoAtivacaoVarZC( campusId );			// Restricao 1.2.10
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest criaRestricaoTaticoAtivacaoVarZC: " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
#endif


  	timer.start();
	restricoes += criaRestricaoTaticoDisciplinasIncompativeis( campusId );			// Restricao 1.2.11
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest criaRestricaoTaticoDisciplinasIncompativeis: " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
#endif

  	timer.start();
	restricoes += criaRestricaoTaticoAlunoHorario( campusId );					// Restricao 1.2.12	
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest criaRestricaoTaticoAlunoHorario: " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
#endif

  	timer.start();
//	restricoes += criaRestricaoTaticoAlunoUnidDifDia( campusId );			// Restricao 1.2.14
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest criaRestricaoTaticoAlunoUnidDifDia: " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
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
	std::cout << "numRest criaRestricaoTaticoAlunoDiscPraticaTeorica: " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
#endif

	/*
  	timer.start();
	restricoes += criaRestricaoTaticoAlunoDiscPraticaTeorica_1xN_antiga( campusId ); // Restricao 1.2.30	
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest criaRestricaoTaticoAlunoDiscPraticaTeorica_1xN_antiga: " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
#endif

  	timer.start();
	restricoes += criaRestricaoTaticoAlunosMesmaTurmaPratica( campusId );	// Restricao 1.2.30	
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest criaRestricaoTaticoAlunosMesmaTurmaPratica: " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
#endif
	*/


  	timer.start();
	restricoes += criaRestricaoTaticoMinDiasAluno( campusId );	// Restricao 1.2.18
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest criaRestricaoTaticoMinDiasAluno: " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
#endif


  	timer.start();
	restricoes += criaRestricaoTaticoMaxDiasAluno( campusId );	// Restricao 1.2.19
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest criaRestricaoTaticoMaxDiasAluno: " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
#endif
			   

  	timer.start();
	restricoes += criaRestricaoTaticoSalaUnica( campusId );	// Restricao 1.2.19
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest criaRestricaoTaticoSalaUnica: " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
#endif


  	timer.start();
	restricoes += criaRestricaoTaticoSalaPorTurma( campusId );	// Restricao 1.2.19
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest criaRestricaoTaticoSalaPorTurma: " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
#endif


  	timer.start();
	restricoes += criaRestricaoTaticoAssociaAlunoEGaranteNroCreds( campusId );	// Restricao 1.2.19
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest criaRestricaoTaticoAssociaAlunoEGaranteNroCreds: " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
#endif
		

  	timer.start();
	restricoes += criaRestricaoTaticoAbreTurmasEmSequencia( campusId );	// Restricao 1.2.19
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest criaRestricaoTaticoAbreTurmasEmSequencia: " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
#endif
	

  	timer.start();
	restricoes += criaRestricaoTaticoAlunoCurso( campusId );	// Restricao 1.2.19
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest criaRestricaoTaticoAlunoCurso: " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
#endif

  	timer.start();
	restricoes += criaRestricaoTaticoCursosIncompat( campusId );	// Restricao 1.2.20
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest criaRestricaoTaticoCursosIncompat: " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
#endif


  	timer.start();
	restricoes += criaRestricaoTaticoProibeCompartilhamento( campusId );	// Restricao 1.2.21
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest criaRestricaoTaticoProibeCompartilhamento: " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
#endif
	
  	timer.start();
	restricoes += criaRestricaoPrioridadesDemanda_v2( campusId, prioridade );	// Restricao 1.2.21
	//restricoes += criaRestricaoPrioridadesDemanda( campusId, prioridade );	// Restricao 1.2.21
	//restricoes += criaRestricaoPrioridadesDemandaEquiv( campusId, prioridade );	// Restricao 1.2.21
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest criaRestricaoPrioridadesDemanda_v2: " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
#endif
	
	
// N�o precisa, a restri��o criaRestricaoTaticoSalaUnica() j� engloba essa
//  	timer.start();
//	restricoes += criaRestricaoTaticoAtivaZ( campusId );	// Restricao 1.2.29
//	timer.stop();
//	dif = timer.getCronoCurrSecs();
//
//#ifdef PRINT_cria_restricoes
//	std::cout << "numRest criaRestricaoTaticoAtivaZ: " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
//	numRestAnterior = restricoes;
//#endif
	

  	timer.start();
	restricoes += criaRestricaoTaticoTurmaComOsMesmosAlunosPorAula( campusId );	// Restricao 1.2.29
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest criaRestricaoTaticoTurmaComOsMesmosAlunosPorAula: " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
#endif
	

  	timer.start();
	restricoes += criaRestricaoTaticoAlocMinAluno( campusId );	// Restricao 1.2.31
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest criaRestricaoTaticoAlocMinAluno: " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
#endif
	

  	timer.start();
	restricoes += criaRestricaoFolgaOcupacaoSala( campusId );	// Restricao 1.2.32
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest criaRestricaoFolgaOcupacaoSala: " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
#endif
	

  	timer.start();
	restricoes += criaRestricaoSobreposHorariosProfs();	// Restricao 1.2.33
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest criaRestricaoSobreposHorariosProfs: " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
#endif
	

  	timer.start();
	restricoes += criaRestricaoProfAula();	// Restricao 1.2.34
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest criaRestricaoProfAula: " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
#endif
	
	
  	timer.start();
	restricoes += criaRestricaoProfAulaSum();	// Restricao 1.2.34
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest criaRestricaoProfAulaSum: " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
#endif
	

  	timer.start();
	restricoes += criaRestricaoProfTurma();	// Restricao 1.2.35
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest criaRestricaoProfTurma: " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
#endif


	timer.start();
	restricoes += criaRestricaoProfUnico();	// Restricao 1.2.36
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest criaRestricaoProfUnico: " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
#endif

	if ( !CONSTR_GAP_PROF_SEPARADO )
	{
	timer.start();
	restricoes += criarRestricaoProfHiHf_();	// Restricao 1.2.38
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest criarRestricaoProfHiHf_: " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
#endif
	}

	timer.start();
	restricoes += criarRestricaoAlunoHiHf_();	// Restricao 1.2.38
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest criarRestricaoAlunoHiHf_: " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
#endif
		
	timer.start();
	restricoes += criarRestricaoMinCredsDiaAluno();
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest criarRestricaoMinCredsDiaAluno: " << (restricoes - numRestAnterior)  
		<<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
#endif
	
	timer.start();
	restricoes += criaRestricaoTempoDeslocProfessor();
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest criaRestricaoTempoDeslocProfessor: " << (restricoes - numRestAnterior)  
		<<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
#endif
	
	timer.start();
	restricoes += criaRestricaoNrMaxDeslocProfessor();
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest criaRestricaoNrMaxDeslocProfessor: " << (restricoes - numRestAnterior)  
		<<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
#endif
	

	lp->updateLP();
	
	#ifdef PRINT_cria_restricoes
	std::cout << "Total of Constraints: " << restricoes << "\n\n"; fflush(0);
	#endif
	
	return restricoes;
}


int MIPUnico::criaRestricaoTaticoAssociaVeX( int campusId )
{
	int restricoes = 0;
	int nnz;
	char name[ 1024 ];

	VariableMIPUnico v;
	ConstraintMIPUnico c;

	VariableMIPUnicoHash::iterator vit;
	ConstraintMIPUnicoHash::iterator cit;
		
    std::vector<int> idxC;
    std::vector<int> idxR;
    std::vector<double> valC;

	vit = vHashTatico.begin();
	for ( ; vit != vHashTatico.end(); vit++ )
	{
		if( vit->first.getType() != VariableMIPUnico::V_CREDITOS &&
			vit->first.getType() != VariableMIPUnico::V_ALUNO_CREDITOS )
		{
			continue;
		}

		VariableMIPUnico v = vit->first;
		
		if ( v.getUnidade()->getIdCampus() != campusId )
			continue;

		c.reset();
		c.setType( ConstraintMIPUnico::C_ASSOCIA_V_X );		
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
		if ( vit->first.getType() == VariableMIPUnico::V_CREDITOS )
			coef = - v.getSubCjtSala()->getCapacidadeRepr() ;
		else if ( vit->first.getType() == VariableMIPUnico::V_ALUNO_CREDITOS )
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

int MIPUnico::criaRestricaoTaticoUsoDeSalaParaCadaHorario( int campusId )
{
	int restricoes = 0;

	int nnz;
	char name[ 1024 ];

	VariableMIPUnico v;
	ConstraintMIPUnico c;

	VariableMIPUnicoHash::iterator vit;
	ConstraintMIPUnicoHash::iterator cit;

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
		if(vit->first.getType() != VariableMIPUnico::V_CREDITOS)
		{
			vit++;
			continue;
		}

		VariableMIPUnico v = vit->first;

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
				c.setType( ConstraintMIPUnico::C_SALA_HORARIO );
				c.setCampus( campus );
				c.setUnidade( v.getUnidade() );
				c.setSubCjtSala( v.getSubCjtSala() );
				c.setDia( v.getDia() );
				c.setHorarioAulaInicial( h );
            std::pair<DateTime*,int> auxP = problemData->horarioAulaDateTime[h->getId()];
				c.setDateTimeInicial( *auxP.first);

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
					c.setType( ConstraintMIPUnico::C_SALA_HORARIO );
					c.setCampus( campus );
					c.setUnidade( v.getUnidade() );
					c.setSubCjtSala( v.getSubCjtSala() );
					c.setDia( dia );
					c.setHorarioAulaInicial( h );
               std::pair<DateTime*,int> auxP = problemData->horarioAulaDateTime[h->getId()];
				   c.setDateTimeInicial( *auxP.first);
               
					
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

int MIPUnico::criaRestricaoTaticoAtendimentoUnicoTurmaDiscDia( int campusId )
{
	int restricoes = 0;

	int nnz;
	char name[ 1024 ];

	VariableMIPUnico v;
	ConstraintMIPUnico c;

	VariableMIPUnicoHash::iterator vit;
	ConstraintMIPUnicoHash::iterator cit;

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
		VariableMIPUnico v = vit->first;

		if( v.getType() != VariableMIPUnico::V_CREDITOS )
		{
			continue;
		}
		if ( problemData->cp_discs[campusId].find( v.getDisciplina()->getId() ) ==
			 problemData->cp_discs[campusId].end() )
		{
			continue;
		}

		c.reset();
		c.setType( ConstraintMIPUnico::C_UNICO_ATEND_TURMA_DISC_DIA );
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
int MIPUnico::criaRestricaoTaticoTurmaDiscDiasConsec( int campusId )
{
   int restricoes = 0;

   char name[ 1024 ];
   int nnz;

   ConstraintMIPUnico c;
   VariableMIPUnico v;
   VariableMIPUnicoHash::iterator it_v;

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
			 
			 // S� cria as restri��es a partir do segundo dia
			 // J� que a estrutura � ordenada, pula o primeiro.
			 if ( itDiasLetDisc != disciplina->diasLetivos.end() )
				itDiasLetDisc++;

			 for (; itDiasLetDisc != disciplina->diasLetivos.end(); itDiasLetDisc++ )
			 {
				 int dia = *itDiasLetDisc;

				c.reset();
				c.setType( ConstraintMIPUnico::C_TURMA_DISC_DIAS_CONSEC );

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
				v.setType( VariableMIPUnico::V_DIAS_CONSECUTIVOS );
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
								v.setType( VariableMIPUnico::V_CREDITOS );

								v.setTurma( turma );
								v.setDisciplina( disciplina );
								v.setUnidade( *itUnidade );
								v.setSubCjtSala( *itCjtSala );
								v.setHorarioAulaInicial( hi );
								v.setHorarioAulaFinal( hf );
								v.setDia( dia );
                        std::pair<DateTime*,int> auxP = problemData->horarioAulaDateTime[hi->getId()];
							   v.setDateTimeInicial(*auxP.first);
                        auxP = problemData->horarioAulaDateTime[hf->getId()];
							   v.setDateTimeFinal(*auxP.first);

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

int MIPUnico::criaRestricaoTaticoCursosIncompat( int campusId )
{
   int restricoes = 0;

   if ( problemData->parametros->nao_permite_compart_turma.size() == 0 )
	   return restricoes;

   char name[ 1024 ];
   int nnz;

   ConstraintMIPUnico c;
   VariableMIPUnico v;
   VariableMIPUnicoHash::iterator it_v;

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
						c.setType( ConstraintMIPUnico::C_ALUNOS_CURSOS_INCOMP );
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
						v.setType( VariableMIPUnico::V_TURMA_ATEND_CURSO );
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
						v.setType( VariableMIPUnico::V_TURMA_ATEND_CURSO );
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

int MIPUnico::criaRestricaoTaticoProibeCompartilhamento( int campusId )
{
   int restricoes = 0;

   if ( problemData->parametros->permite_compartilhamento_turma_sel )
   {
	   return restricoes;
   }

   char name[ 1024 ];
   int nnz;

   ConstraintMIPUnico c;
   VariableMIPUnico v;
   VariableMIPUnicoHash::iterator it_v;

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
					c.setType( ConstraintMIPUnico::C_PROIBE_COMPARTILHAMENTO );
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
					v.setType( VariableMIPUnico::V_TURMA_ATEND_CURSO );
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
					v.setType( VariableMIPUnico::V_TURMA_ATEND_CURSO );
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
					v.setType( VariableMIPUnico::V_SLACK_COMPARTILHAMENTO );
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
int MIPUnico::criaRestricaoTaticoLimitaAberturaTurmas( int campusId, int prioridade )
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

   ConstraintMIPUnico c;
   VariableMIPUnicoHash::iterator vit;
   ConstraintMIPUnicoHash::iterator cit;

   std::vector< std::pair< int, int > > coeffList;
   std::vector< double > coeffListVal;
   std::pair< int, int > auxCoef;

   vit = vHashTatico.begin();

   for (; vit != vHashTatico.end(); vit++ )
   {
		VariableMIPUnico v = vit->first;

		if ( v.getType() != VariableMIPUnico::V_ALOCA_ALUNO_TURMA_DISC &&
			 v.getType() != VariableMIPUnico::V_ABERTURA )
		{
			continue;
		}
		
		Campus *campus;
		
		if ( v.getType() == VariableMIPUnico::V_ALOCA_ALUNO_TURMA_DISC )
			campus = v.getAlunoDemanda()->demanda->oferta->campus;
		else if ( v.getType() == VariableMIPUnico::V_ABERTURA )
			campus = v.getCampus();

		int turma = v.getTurma();
		Disciplina* disc = v.getDisciplina();
		
		int MinAlunos;
		if (disc->eLab()) 
			MinAlunos = MinAlunosPrat;
		else
			MinAlunos = MinAlunosTeor;

		double coef=0.0;
		if ( v.getType() == VariableMIPUnico::V_ALOCA_ALUNO_TURMA_DISC )
			coef = 1.0;
		else if ( v.getType() == VariableMIPUnico::V_ABERTURA )
			coef = - MinAlunos;

		c.reset();
		c.setType( ConstraintMIPUnico::C_MIN_ALUNOS_TURMA );
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

			// Insere restri��o
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
int MIPUnico::criaRestricaoTaticoLimitaMaximoAlunosNasTurmas( int campusId, int prioridade )
{
   int restricoes = 0;
   int nnz;
   char name[ 1024 ];

   ConstraintMIPUnico c;
   VariableMIPUnicoHash::iterator vit;
   ConstraintMIPUnicoHash::iterator cit;

   std::vector< std::pair< int, int > > coeffList;
   std::vector< double > coeffListVal;
   std::pair< int, int > auxCoef;

   vit = vHashTatico.begin();

   for (; vit != vHashTatico.end(); vit++ )
   {
		VariableMIPUnico v = vit->first;

		if ( v.getType() != VariableMIPUnico::V_ALOCA_ALUNO_TURMA_DISC &&
			 v.getType() != VariableMIPUnico::V_ABERTURA )
		{
			continue;
		}
		
		Campus *campus;
		
		if ( v.getType() == VariableMIPUnico::V_ALOCA_ALUNO_TURMA_DISC )
			campus = v.getAlunoDemanda()->demanda->oferta->campus;
		else if ( v.getType() == VariableMIPUnico::V_ABERTURA )
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
		if ( v.getType() == VariableMIPUnico::V_ALOCA_ALUNO_TURMA_DISC )
			coef = 1.0;
		else if ( v.getType() == VariableMIPUnico::V_ABERTURA )
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
		c.setType( ConstraintMIPUnico::C_MAX_ALUNOS_TURMA );
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

			// Insere restri��o
			cHashTatico[ c ] = lp->getNumRows();
			lp->addRow( row );
			restricoes++;
		}
   }

   chgCoeffList( coeffList, coeffListVal );

   return restricoes;
}



int MIPUnico::criaRestricaoTaticoAlunoDiscPraticaTeorica_MxN( int campusId )
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

   ConstraintMIPUnico c;
   VariableMIPUnico v;
   VariableMIPUnicoHash::iterator it_v;   

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
			c.setType( ConstraintMIPUnico::C_ALUNO_DISC_PRATICA_TEORICA );
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
				VariableMIPUnico v;
				v.reset();
				v.setType( VariableMIPUnico::V_ALOCA_ALUNO_TURMA_DISC );
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

				VariableMIPUnico v;
				v.reset();
				v.setType( VariableMIPUnico::V_ALOCA_ALUNO_TURMA_DISC );
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


int MIPUnico::criaRestricaoTaticoAlunoDiscPraticaTeorica_1x1( int campusId )
{
	int restricoes=0;

	// rela��o 1x1
	if ( ! problemData->parametros->discPratTeor1x1 )
	{
		return restricoes;
	}

    char name[ 100 ];

	VariableMIPUnico v;
	ConstraintMIPUnico c;
	VariableMIPUnicoHash::iterator vit;
	ConstraintMIPUnicoHash::iterator cit;

	vit = vHashTatico.begin();
	for ( ; vit != vHashTatico.end(); vit++ )
	{
		v = vit->first;
	
		if( v.getType() == VariableMIPUnico::V_ALOCA_ALUNO_TURMA_DISC )
		{						
			Disciplina *disciplinaPratica = NULL;
						
			int discId = v.getDisciplina()->getId();
			if ( discId > 0 )
			{			
				// S� para disciplinas praticas/teoricas
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
			c.setType( ConstraintMIPUnico::C_ALUNO_DISC_PRATICA_TEORICA_1x1 );
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


int MIPUnico::criaRestricaoTaticoAlunoDiscPraticaTeorica_1xN( int campusId )
{
	int restricoes=0;

	// rela��o 1xN
	if ( ! problemData->parametros->discPratTeor1xN )
	{
		return restricoes;
	}

    char name[ 100 ];
	
	VariableMIPUnicoHash::iterator vit;
	ConstraintMIPUnicoHash::iterator cit;

	vit = vHashTatico.begin();
	for ( ; vit != vHashTatico.end(); vit++ )
	{
		VariableMIPUnico v = vit->first;
	
		if( v.getType() == VariableMIPUnico::V_ALOCA_ALUNO_TURMA_DISC )
		{							
			Disciplina *disciplinaPratica = NULL;
			Disciplina *disciplinaTeorica = NULL;
			int turmaTeorica = -1;

			int discId = v.getDisciplina()->getId();
			if ( discId > 0 )
			{
				// S� para disciplinas praticas/teoricas
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
		
			ConstraintMIPUnico c;
			c.reset();
			c.setType( ConstraintMIPUnico::C_ALUNO_DISC_PRATICA_TEORICA_1xN );
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
Regra de divis�o de cr�ditos
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
n�mero de cr�ditos determinados para a disciplina $d$ no dia $t$ na combina��o de divis�o de cr�dito $k$.

%DocEnd
/====================================================================*/

int MIPUnico::criaRestricaoTaticoDivisaoCredito_hash( int campusId )
{
   int restricoes = 0;

   if ( MIPUnico::consideraDivCredDisc == 0 )
	   return restricoes;

   char name[ 1024 ];
   int nnz;

   std::vector< std::pair< int, int > > coeffList;
   std::vector< double > coeffListVal;
   std::pair< int, int > auxCoef;
   
   VariableMIPUnicoHash::iterator vit;

   for ( vit = vHashTatico.begin(); vit != vHashTatico.end(); vit++ )
   {
	   VariableMIPUnico v = vit->first;

	   Disciplina *disciplina=nullptr;
	   int turma;
	   GGroup<int> dias;
	   std::vector<double> coefs;
	   	   
	   if ( v.getDisciplina() != nullptr )
	   if ( !v.getDisciplina()->possuiRegraCred() )
		   continue;

       if(vit->first.getType() == VariableMIPUnico::V_CREDITOS)
       {		 
		   Calendario *calendario = v.getHorarioAulaInicial()->getCalendario();
		   int nCreds = calendario->retornaNroCreditosEntreHorarios(v.getHorarioAulaInicial(), v.getHorarioAulaFinal());
		   coefs.push_back( nCreds );
		   disciplina = v.getDisciplina();
		   dias.add( v.getDia() );
		   turma = v.getTurma();
       }
       else if(vit->first.getType() == VariableMIPUnico::V_SLACK_COMBINACAO_DIVISAO_CREDITO_P)
       {         
		   coefs.push_back( -1.0 );		   
		   disciplina = v.getDisciplina();
		   dias.add( v.getDia() );
		   turma = v.getTurma();
       }
       else if(vit->first.getType() == VariableMIPUnico::V_SLACK_COMBINACAO_DIVISAO_CREDITO_M)
       {         
		   coefs.push_back( 1.0 );
		   disciplina = v.getDisciplina();
		   dias.add( v.getDia() );
		   turma = v.getTurma();
       }
       else if(vit->first.getType() == VariableMIPUnico::V_COMBINACAO_DIVISAO_CREDITO)
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

		    ConstraintMIPUnico c;
		    c.reset();
		    c.setType( ConstraintMIPUnico::C_DIVISAO_CREDITO );
		    c.setDisciplina( disciplina );
		    c.setTurma( turma );
		    c.setDia( dia );
		    sprintf( name, "%s", c.toString( etapa ).c_str() ); 

			ConstraintMIPUnicoHash::iterator cit;
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
   }

   chgCoeffList( coeffList, coeffListVal );

   return restricoes;
}


// Restricao 1.2.15
/*====================================================================/
%DocBegin TRIEDA_LOAD_MODEL

%Constraint 
Exatamente uma combina��o de regra de divis�o de cr�ditos deve ser escolhida
para cada turma aberta.
%Desc 

%MatExp

\begin{eqnarray}
\sum\limits_{k \in K_{d}} m_{d,i,cp,k} = z_{i,d,cp} \nonumber \qquad 
\forall d \in D \quad
\forall i \in I_{d} \quad
\forall cp \in Cp
\end{eqnarray}

%DocEnd
/====================================================================*/
int MIPUnico::criaRestricaoTaticoCombinacaoDivisaoCredito( int campusId )
{
   int restricoes = 0;

   if ( MIPUnico::consideraDivCredDisc == 0 )
	   return restricoes;

   char name[ 1024 ];
   int nnz;

   VariableMIPUnico v;
   ConstraintMIPUnico c;
   ConstraintMIPUnicoHash::iterator cit;
   VariableMIPUnicoHash::iterator vit;

   std::vector<int> idxC;
   std::vector<int> idxR;
   std::vector<double> valC;

	vit = vHashTatico.begin();
	for ( ; vit != vHashTatico.end(); vit++)
	{		
		VariableMIPUnico v = vit->first;

		double coef;

		if( v.getType() == VariableMIPUnico::V_COMBINACAO_DIVISAO_CREDITO )
		{
			coef = 1.0;			
		}         
		else if( v.getType() == VariableMIPUnico::V_ABERTURA )
		{
			coef = -1.0;
		}
		else continue;

		if ( !v.getDisciplina()->possuiRegraCred() )
			continue;

		c.reset();
        c.setType( ConstraintMIPUnico::C_COMBINACAO_DIVISAO_CREDITO );
        c.setDisciplina( v.getDisciplina() );
		c.setTurma( v.getTurma() );
		c.setCampus( v.getCampus() );

		cit = cHashTatico.find(c);

		if(cit == cHashTatico.end())
		{
			sprintf( name, "%s", c.toString( etapa ).c_str() );
			OPT_ROW row( 10, OPT_ROW::EQUAL , 0.0 , name );

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
int MIPUnico::criaRestricaoTaticoAtivacaoVarZC( int campusId )
{
   int restricoes = 0;
      
	if ( ! problemData->parametros->considerar_disciplinas_incompativeis_no_dia )
	   return restricoes;
	
   char name[ 1024 ];
   int nnz;

   ConstraintMIPUnico c;
   VariableMIPUnico v;
   ConstraintMIPUnicoHash::iterator cit;
   VariableMIPUnicoHash::iterator vit;

   std::vector<int> idxC;
   std::vector<int> idxR;
   std::vector<double> valC;

	vit = vHashTatico.begin();
	for ( ; vit != vHashTatico.end(); vit++)
	{		
		VariableMIPUnico v = vit->first;
		
		double coef = 0.0;
		if( v.getType() == VariableMIPUnico::V_ABERTURA_COMPATIVEL )
		{
			coef = - v.getDisciplina()->getNumTurmas();			
		}
		else if( v.getType() == VariableMIPUnico::V_CREDITOS )
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
        c.setType( ConstraintMIPUnico::C_VAR_ZC );
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
			 c.setType( ConstraintMIPUnico::C_VAR_ZC );

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
								v.setType( VariableMIPUnico::V_CREDITOS );

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
			 v.setType( VariableMIPUnico::V_ABERTURA_COMPATIVEL );

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

int MIPUnico::criaRestricaoTaticoDisciplinasIncompativeis( int campusId )
{
   int restricoes = 0;
      
	if ( ! problemData->parametros->considerar_disciplinas_incompativeis_no_dia )
	   return restricoes;

   char name[ 1024 ];
   int nnz;

   ConstraintMIPUnico c;
   VariableMIPUnico v;
   VariableMIPUnicoHash::iterator it_v;

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
            c.setType( ConstraintMIPUnico::C_DISC_INCOMPATIVEIS );
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
            v.setType( VariableMIPUnico::V_ABERTURA_COMPATIVEL );

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


/*
	Impede a aloca��o de aulas de um aluno em hor�rios com sobreposi��o.
	Para cada dia t, hor�rio h, e aluno al:

	sum[u] sum[s] sum[hi] sum[hf] v_{a,i,d,u,s,hi,hf,t} <= 1

	sendo al alocado em (i,d) e	(hi,hf) sobrep�e o inicio de h
*/
int MIPUnico::criaRestricaoTaticoAlunoHorario( int campusId )
{
   int restricoes = 0;
   int nnz;
   char name[ 1024 ];

   ConstraintMIPUnico c;
   VariableMIPUnicoHash::iterator vit;
   ConstraintMIPUnicoHash::iterator cit;

   std::vector< std::pair< int, int > > coeffList;
   std::vector< double > coeffListVal;
   std::pair< int, int > auxCoef;

   vit = vHashTatico.begin();

   for (; vit != vHashTatico.end(); vit++ )
   {
		VariableMIPUnico v = vit->first;

		if ( v.getType() != VariableMIPUnico::V_ALUNO_CREDITOS )
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
			c.setType( ConstraintMIPUnico::C_ALUNO_HORARIO );
			c.setAluno( aluno );
			c.setDia( dia );
			c.setHorarioAulaInicial( horario_aula );
			c.setDateTimeInicial( *auxP.first);

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
										// insere -1 no lado direito da restri��o
										add_rhs += -1.0;
									}
									break;
								}
							}
						}
					}
				}

				if ( add_rhs < -1.01 ) 
					std::cout << "\nErro em MIPUnico::criaRestricaoTaticoAlunoHorario()."
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

int MIPUnico::criaRestricaoTaticoMinDiasAluno( int campusId )
{
   int restricoes = 0;

   if ( problemData->parametros->carga_horaria_semanal_aluno != ParametrosPlanejamento::MINIMIZAR_DIAS )
   {
		return restricoes;
   }

   int nnz;
   char name[ 1024 ];

   ConstraintMIPUnico c;
   VariableMIPUnicoHash::iterator vit;
   ConstraintMIPUnicoHash::iterator cit;

   std::vector< std::pair< int, int > > coeffList;
   std::vector< double > coeffListVal;
   std::pair< int, int > auxCoef;

   vit = vHashTatico.begin();

   for (; vit != vHashTatico.end(); vit++ )
   {
		VariableMIPUnico v = vit->first;

		if ( v.getType() != VariableMIPUnico::V_ALUNO_CREDITOS && 
			 v.getType() != VariableMIPUnico::V_ALUNO_DIA )
		{
			continue;
		}
		
		Aluno *aluno = v.getAluno();					
		int dia = v.getDia();

		double coef=0.0;
		if ( v.getType() == VariableMIPUnico::V_ALUNO_CREDITOS )
			coef = -1.0;
		else if ( v.getType() == VariableMIPUnico::V_ALUNO_DIA )
			coef = 10.0;

		c.reset();
		c.setType( ConstraintMIPUnico::C_MIN_DIAS_ALUNO );
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

int MIPUnico::criaRestricaoTaticoMaxDiasAluno( int campusId )
{
   int restricoes = 0;

   if ( problemData->parametros->carga_horaria_semanal_aluno != ParametrosPlanejamento::EQUILIBRAR )
   {
		return restricoes;
   }

   int nnz;
   char name[ 1024 ];

   ConstraintMIPUnico c;
   VariableMIPUnicoHash::iterator vit;
   ConstraintMIPUnicoHash::iterator cit;

   std::vector< std::pair< int, int > > coeffList;
   std::vector< double > coeffListVal;
   std::pair< int, int > auxCoef;

   vit = vHashTatico.begin();

   for (; vit != vHashTatico.end(); vit++ )
   {
		VariableMIPUnico v = vit->first;

		if ( v.getType() != VariableMIPUnico::V_ALUNO_CREDITOS && 
			 v.getType() != VariableMIPUnico::V_ALUNO_DIA )
		{
			continue;
		}
		
		Aluno *aluno = v.getAluno();					
		int dia = v.getDia();

		double coef=0.0;
		if ( v.getType() == VariableMIPUnico::V_ALUNO_CREDITOS )
			coef = -1.0;
		else if ( v.getType() == VariableMIPUnico::V_ALUNO_DIA )
			coef = 1.0;

		c.reset();
		c.setType( ConstraintMIPUnico::C_MAX_DIAS_ALUNO );
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
int MIPUnico::criaRestricaoTaticoAtendeAluno( int campusId, int prioridade )
{
   int restricoes = 0;
   
   if ( problemData->parametros->considerar_equivalencia_por_aluno && USAR_EQUIVALENCIA )
		return restricoes;

   int nnz;
   char name[ 1024 ];

   ConstraintMIPUnico c;
   VariableMIPUnicoHash::iterator vit;
   ConstraintMIPUnicoHash::iterator cit;

   std::vector< std::pair< int, int > > coeffList;
   std::vector< double > coeffListVal;
   std::pair< int, int > auxCoef;

   vit = vHashTatico.begin();

   for (; vit != vHashTatico.end(); vit++ )
   {
		VariableMIPUnico v = vit->first;

		if ( v.getType() != VariableMIPUnico::V_ALOCA_ALUNO_TURMA_DISC &&
			 v.getType() != VariableMIPUnico::V_SLACK_DEMANDA_ALUNO )
		{
			continue;
		}
		
		Aluno *aluno = v.getAluno();
		Disciplina* disc = v.getDisciplina();
			
		c.reset();
		c.setType( ConstraintMIPUnico::C_DEMANDA_DISC_ALUNO );
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

			// Insere restri��o
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
int MIPUnico::criaRestricaoTaticoSalaUnica( int campusId )

{
   int restricoes = 0;
   int nnz;
   char name[ 1024 ];

   ConstraintMIPUnico c;
   VariableMIPUnicoHash::iterator vit;
   ConstraintMIPUnicoHash::iterator cit;

   std::vector< std::pair< int, int > > coeffList;
   std::vector< double > coeffListVal;
   std::pair< int, int > auxCoef;

   vit = vHashTatico.begin();

   for (; vit != vHashTatico.end(); vit++ )
   {
		VariableMIPUnico v = vit->first;
		
		double coef=0.0;

		if ( v.getType() == VariableMIPUnico::V_OFERECIMENTO )
		{
			coef = 1.0;
		}
		else if ( v.getType() == VariableMIPUnico::V_ABERTURA )
		{
			coef = -1.0;
		}
		else continue;

		int turma = v.getTurma();
		Disciplina* disc = v.getDisciplina();
			
		c.reset();
		c.setType( ConstraintMIPUnico::C_SALA_UNICA );
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

			// Insere restri��o
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
int MIPUnico::criaRestricaoTaticoSalaPorTurma( int campusId )
{
   int restricoes = 0;
   int nnz;
   char name[ 1024 ];

   ConstraintMIPUnico c;
   VariableMIPUnicoHash::iterator vit;
   ConstraintMIPUnicoHash::iterator cit;

   std::vector< std::pair< int, int > > coeffList;
   std::vector< double > coeffListVal;
   std::pair< int, int > auxCoef;

   vit = vHashTatico.begin();

   for (; vit != vHashTatico.end(); vit++ )
   {
		VariableMIPUnico v = vit->first;

		if ( v.getType() != VariableMIPUnico::V_OFERECIMENTO && 
			 v.getType() != VariableMIPUnico::V_CREDITOS )
		{
			continue;
		}
		
		int turma = v.getTurma();
		Disciplina* disc = v.getDisciplina();
		ConjuntoSala *cjtSala = v.getSubCjtSala();
			
		double coef = 0.0;
		if ( v.getType() == VariableMIPUnico::V_OFERECIMENTO )
			coef = disc->getTotalCreditos();
		else if ( v.getType() == VariableMIPUnico::V_CREDITOS )
			coef = -1.0;

		c.reset();
		c.setType( ConstraintMIPUnico::C_SALA_TURMA );
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

			// Insere restri��o
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
int MIPUnico::criaRestricaoTaticoAssociaAlunoEGaranteNroCreds( int campusId )
{
   int restricoes = 0;
   int nnz;
   char name[ 1024 ];

   ConstraintMIPUnico c;
   VariableMIPUnicoHash::iterator vit;
   ConstraintMIPUnicoHash::iterator cit;

   std::vector< std::pair< int, int > > coeffList;
   std::vector< double > coeffListVal;
   std::pair< int, int > auxCoef;

   vit = vHashTatico.begin();

   for (; vit != vHashTatico.end(); vit++ )
   {
		VariableMIPUnico v = vit->first;

		if ( v.getType() != VariableMIPUnico::V_ALOCA_ALUNO_TURMA_DISC &&
			 v.getType() != VariableMIPUnico::V_ALUNO_CREDITOS )
		{
			continue;
		}
		
		Aluno *aluno = v.getAluno();
		Disciplina* disc = v.getDisciplina();
		int turma = v.getTurma();	
					
		double coef = 0.0;
		if ( v.getType() == VariableMIPUnico::V_ALOCA_ALUNO_TURMA_DISC ) // s
			coef = disc->getTotalCreditos();
		else if ( v.getType() == VariableMIPUnico::V_ALUNO_CREDITOS ) // v
		{
			HorarioAula *hi = v.getHorarioAulaInicial();
			HorarioAula *hf = v.getHorarioAulaFinal();
			Calendario *calendario = hi->getCalendario();
			coef = - calendario->retornaNroCreditosEntreHorarios(hi, hf);
		}
		
		c.reset();
		c.setType( ConstraintMIPUnico::C_ASSOCIA_S_V );
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

			// Insere restri��o
			cHashTatico[ c ] = lp->getNumRows();
			lp->addRow( row );
			restricoes++;
		}
   }

   chgCoeffList( coeffList, coeffListVal );

   return restricoes;
}


int MIPUnico::criaRestricaoTaticoAbreTurmasEmSequencia( int campusId )
{
   int restricoes = 0;
   char name[ 1024 ];
   int nnz;

   VariableMIPUnico v;
   ConstraintMIPUnico c;
   VariableMIPUnicoHash::iterator it_v;

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
            c.setType( ConstraintMIPUnico::C_ABRE_TURMAS_EM_SEQUENCIA );
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
                        v.setType( VariableMIPUnico::V_OFERECIMENTO );
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
                        v.setType( VariableMIPUnico::V_OFERECIMENTO );                        
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
            v.setType( VariableMIPUnico::V_SLACK_ABERT_SEQ_TURMA );
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
int MIPUnico::criaRestricaoTaticoAlunoCurso( int campusId )
{
   int restricoes = 0;

   if ( problemData->parametros->permite_compartilhamento_turma_sel && 
	    problemData->parametros->nao_permite_compart_turma.size() == 0 )
   {
	   return restricoes;
   }

   int nnz;
   char name[ 1024 ];

   ConstraintMIPUnico c;
   VariableMIPUnicoHash::iterator vit;
   ConstraintMIPUnicoHash::iterator cit;

   std::vector< std::pair< int, int > > coeffList;
   std::vector< double > coeffListVal;
   std::pair< int, int > auxCoef;

   vit = vHashTatico.begin();

   for (; vit != vHashTatico.end(); vit++ )
   {
		VariableMIPUnico v = vit->first;

		if ( v.getType() != VariableMIPUnico::V_TURMA_ATEND_CURSO &&
			 v.getType() != VariableMIPUnico::V_ALOCA_ALUNO_TURMA_DISC )
		{
			continue;
		}
		
		//std::cout << "\nVar = " << v.toString();

		int turma = v.getTurma();
		Disciplina* disc = v.getDisciplina();
		Curso *curso = NULL;

		double coef = 0.0;
		if ( v.getType() == VariableMIPUnico::V_TURMA_ATEND_CURSO ) // b
		{
			curso = v.getCurso();
			if ( USAR_EQUIVALENCIA ) 
				coef = - 500; //this->getNroMaxAlunoDemanda( disc->getId() );
				//coef = - problemData->maxDemandaDiscNoCursoEquiv( disc, curso->getId() );
			else 
				coef = - problemData->haDemandaDiscNoCurso( disc->getId(), curso->getId() );
		}
		else if ( v.getType() == VariableMIPUnico::V_ALOCA_ALUNO_TURMA_DISC ) // s
		{
			curso = v.getAlunoDemanda()->getCurso();
			coef = 1.0;
		}

		c.reset();
		c.setType( ConstraintMIPUnico::C_ALUNO_CURSO );
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

			// Insere restri��o
			cHashTatico[ c ] = lp->getNumRows();
			lp->addRow( row );
			restricoes++;
		}
   }

   chgCoeffList( coeffList, coeffListVal );

   return restricoes;
}

// Restricao 1.21
int MIPUnico::criaRestricaoPrioridadesDemanda( int campusId, int prior )
{
    int restricoes = 0;
	
	if ( problemData->parametros->considerar_equivalencia_por_aluno && this->USAR_EQUIVALENCIA )
		return restricoes;

    if ( prior < 2 )
	   return restricoes;
	
	std::cout<<"\ncriaRestricaoPrioridadesDemanda...";

    char name[ 1024 ];
    int nnz;

    ConstraintMIPUnico c;
    VariableMIPUnico v;
    VariableMIPUnicoHash::iterator it_v;   

    Campus *campus = problemData->refCampus[campusId];

	ITERA_GGROUP_LESSPTR( itAluno, problemData->alunos, Aluno )
	{
		Aluno *aluno = *itAluno;

		c.reset();
		c.setType( ConstraintMIPUnico::C_ALUNO_PRIORIDADES_DEMANDA );
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
				VariableMIPUnico v;
				v.reset();
				v.setType( VariableMIPUnico::V_ALOCA_ALUNO_TURMA_DISC );
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

		VariableMIPUnico v;
		v.reset();
		v.setType( VariableMIPUnico::V_SLACK_PRIOR_INF );
		v.setAluno( aluno );
		v.setCampus( campus );
		it_v = vHashTatico.find( v );
		if( it_v != vHashTatico.end() )
		{
			row.insert( it_v->second, -1.0 );
		}

		v.reset();
		v.setType( VariableMIPUnico::V_SLACK_PRIOR_SUP );
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
int MIPUnico::criaRestricaoPrioridadesDemanda_v2( int campusId, int prior )
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
    
    VariableMIPUnicoHash::iterator vit;
    
    std::vector< std::pair< int, int > > coeffList;
    std::vector< double > coeffListVal;
    std::pair< int, int > auxCoef;

    vit = vHashTatico.begin();
    for (; vit != vHashTatico.end(); vit++ )
    {
		VariableMIPUnico v = vit->first;
		
		double coef=0.0;

		if ( v.getType() == VariableMIPUnico::V_ALOCA_ALUNO_TURMA_DISC )
		{
			if ( v.getAlunoDemanda()->getPrioridade() != prior )
				continue;

			double tempo = v.getDisciplina()->getTotalCreditos() * v.getDisciplina()->getTempoCredSemanaLetiva();
			coef = tempo;
		}
		else if ( v.getType() == VariableMIPUnico::V_SLACK_PRIOR_INF )
		{
			coef = -1.0;
		}
		else if ( v.getType() == VariableMIPUnico::V_SLACK_PRIOR_SUP )
		{
			coef = 1.0;
		}
		else continue;

		Aluno *aluno = v.getAluno();

		//if ( v.getType() == VariableMIPUnico::V_SLACK_PRIOR_SUP )
		//	std::cout<<"\nV_SLACK_PRIOR_SUP  v="<<v.toString();
		//if ( v.getType() == VariableMIPUnico::V_SLACK_PRIOR_INF )
		//	std::cout<<"\nV_SLACK_PRIOR_INF  v="<<v.toString();
		//if ( v.getType() == VariableMIPUnico::V_ALOCA_ALUNO_TURMA_DISC )
		//	std::cout<<"\nV_ALOCA_ALUNO_TURMA_DISC  v="<<v.toString();

		ConstraintMIPUnico c;
		c.reset();
		c.setType( ConstraintMIPUnico::C_ALUNO_PRIORIDADES_DEMANDA );
		c.setAluno( aluno );
		c.setCampus( campus );

		sprintf( name, "%s", c.toString( etapa ).c_str() ); 

		nnz = aluno->demandas.size()*3 + 2;
						
		ConstraintMIPUnicoHash::iterator cit = cHashTatico.find( c );
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

			// Insere restri��o
			cHashTatico[ c ] = lp->getNumRows();
			lp->addRow( row );
			restricoes++;
		}
   }

   chgCoeffList( coeffList, coeffListVal );

   return restricoes;
}


// Restricao 1.21
int MIPUnico::criaRestricaoPrioridadesDemandaEquiv( int campusId, int prior )
{
    int restricoes = 0;
	
	if ( !problemData->parametros->considerar_equivalencia_por_aluno || !this->USAR_EQUIVALENCIA )
		return restricoes;

    if ( prior < 2 )
	   return restricoes;

	std::cout<<"\ncriaRestricaoPrioridadesDemandaEquiv...";

    char name[ 1024 ];
    int nnz;

    ConstraintMIPUnico c;
    VariableMIPUnico v;
    VariableMIPUnicoHash::iterator it_v;   

    Campus *campus = problemData->refCampus[campusId];

	ITERA_GGROUP_LESSPTR( itAluno, problemData->alunos, Aluno )
	{
		Aluno *aluno = *itAluno;

		c.reset();
		c.setType( ConstraintMIPUnico::C_ALUNO_PRIORIDADES_DEMANDA );
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
			if ( turmaAluno == -1 ) // aluno n�o alocado
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
						VariableMIPUnico v;
						v.reset();
						v.setType( VariableMIPUnico::V_ALOCA_ALUNO_TURMA_DISC );
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
					VariableMIPUnico v;
					v.reset();
					v.setType( VariableMIPUnico::V_ALOCA_ALUNO_TURMA_DISC );
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

		VariableMIPUnico v;
		v.reset();
		v.setType( VariableMIPUnico::V_SLACK_PRIOR_INF );
		v.setAluno( aluno );
		v.setCampus( campus );
		it_v = vHashTatico.find( v );
		if( it_v != vHashTatico.end() )
		{
			row.insert( it_v->second, -1.0 );
		}

		v.reset();
		v.setType( VariableMIPUnico::V_SLACK_PRIOR_SUP );
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
	Para cada AlunoDemanda a-d:

	sum[deq]sum[i] s_{i,deq,a} + fd_{d,a} = 1

	aonde deq � uma disciplina equivalente a d. Caso deq tenha creditos praticos e teoricos, 
	s� entram na restricao os teoricos.
*/
int MIPUnico::criaRestricaoTaticoAtendeAlunoEquivTotal( int campusId, int prioridade )
{
    int restricoes = 0;

	if ( !USAR_EQUIVALENCIA )
		return restricoes;
		
    char name[ 1024 ];
    int nnz;

    ConstraintMIPUnico c;
    VariableMIPUnico v;
    VariableMIPUnicoHash::iterator it_v;   

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
			c.setType( ConstraintMIPUnico::C_DEMANDA_EQUIV_ALUNO );
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
					VariableMIPUnico v;
					v.reset();
					v.setType( VariableMIPUnico::V_ALOCA_ALUNO_TURMA_DISC );
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

			VariableMIPUnico v;
			v.reset();
			v.setType( VariableMIPUnico::V_SLACK_DEMANDA_ALUNO );
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
	Para cada disciplina substituta (por equivalencia) a uma demanda n�o atendida de um aluno,
	se ela for pratica+teorica, cria restri��o que garante atendimento m�tuo ou nenhum.
*/
int MIPUnico::criaRestricaoTaticoAlunoDiscPraticaTeoricaEquiv_MxN( int campusId, int prioridade )
{
	// TODO: TEM ALGUM ERRO. EST� OCORRENDO CASOS DE, QUANDO EXISTE SOMENTE A VARIAVEL S PARA A PRATICA,
	// N�O � CRIADA A RESTRI��O, E CONSEQUENTEMENTE � PERMITIDA A ALOCA��O DO ALUNO SOMENTE EM CREDITO
	// PRATICO. REESCREVER ESSE M�TODO VARRENDO O HASH!

   int restricoes = 0;   

	if ( !problemData->parametros->discPratTeorMxN )
	{
		return restricoes;
	}

	if ( ! problemData->parametros->considerar_equivalencia_por_aluno || !USAR_EQUIVALENCIA )
		return restricoes;

   char name[ 1024 ];
   int nnz;

   ConstraintMIPUnico c;
   VariableMIPUnico v;
   VariableMIPUnicoHash::iterator it_v;   

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

			// Demanda ainda n�o atendida

			// Pula disciplina pratica, pq a referencia para as equivalentes � informada pelas teoricas
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
					 problemData->refDisciplinas.end() ) // Restri��o somente para disciplinas de creditos praticos+teoricos
					continue;

				Disciplina *discPratica = problemData->refDisciplinas[ - discTeorica->getId() ];
							
				c.reset();
				c.setType( ConstraintMIPUnico::C_ALUNO_DISC_PRATICA_TEORICA_EQUIV );
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
					VariableMIPUnico v;
					v.reset();
					v.setType( VariableMIPUnico::V_ALOCA_ALUNO_TURMA_DISC );
					v.setAluno( aluno );
					v.setDisciplina( discPratica );
					v.setTurma( turma );
					v.setCampus( campus );

					// Em equivalentes, a referencia do AlunoDemanda para disc pratica est� a mesma que para disc teorica,
					// porque pode existir caso de equivalencia entre disciplinas: T -> TP
					// A� o AlunoDemanda pratico s� � criado ap�s a substitui��o por equival�ncia ser feita pelo modelo.
					v.setAlunoDemanda( *itAlDemanda ); 

					it_v = vHashTatico.find( v );
					if( it_v != vHashTatico.end() )
					{
						row.insert( it_v->second, 1.0 );
					}
				}

				for ( int turma = 1; turma <= discTeorica->getNumTurmas(); turma++ )
				{
					VariableMIPUnico v;
					v.reset();
					v.setType( VariableMIPUnico::V_ALOCA_ALUNO_TURMA_DISC );
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

int MIPUnico::criaRestricaoTaticoAtivaZ( int campusId )
{
   int restricoes = 0;
   int nnz;
   char name[ 1024 ];

   ConstraintMIPUnico c;
   VariableMIPUnicoHash::iterator vit;
   ConstraintMIPUnicoHash::iterator cit;

   std::vector< std::pair< int, int > > coeffList;
   std::vector< double > coeffListVal;
   std::pair< int, int > auxCoef;

   vit = vHashTatico.begin();

   for (; vit != vHashTatico.end(); vit++ )
   {
		VariableMIPUnico v = vit->first;

		if ( v.getType() != VariableMIPUnico::V_OFERECIMENTO &&
			 v.getType() != VariableMIPUnico::V_ABERTURA )
		{
			continue;
		}
		
		int turma = v.getTurma();
		Disciplina* disc = v.getDisciplina();
			
		double coef=0;
		if ( v.getType() == VariableMIPUnico::V_OFERECIMENTO )
		{
			coef = -1.0;
		}
		else if ( v.getType() == VariableMIPUnico::V_ABERTURA )
		{
			coef = 1.0;			
		}

		c.reset();
		c.setType( ConstraintMIPUnico::C_ATIVA_Z );
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

			// Insere restri��o
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
	pertencem �s mesmas aulas de (i,d,cp). De quebrada, garante o total de cr�ditos da disciplina,
	mas isso j� � garantido em outra restri��o, logo n�o � o essencial dessa restri��o.
*/
int MIPUnico::criaRestricaoTaticoTurmaComOsMesmosAlunosPorAula( int campusId )
{
   int restricoes = 0;

   if ( ! this->PERMITIR_NOVAS_TURMAS )
	   return restricoes;

   int nnz;
   char name[ 1024 ];

   ConstraintMIPUnico c;
   VariableMIPUnicoHash::iterator vit;
   ConstraintMIPUnicoHash::iterator cit;

   std::vector< std::pair< int, int > > coeffList;
   std::vector< double > coeffListVal;
   std::pair< int, int > auxCoef;

   vit = vHashTatico.begin();

   for (; vit != vHashTatico.end(); vit++ )
   {
		VariableMIPUnico v = vit->first;

		if ( v.getType() != VariableMIPUnico::V_CREDITOS &&
			 v.getType() != VariableMIPUnico::V_ABERTURA )
		{
			continue;
		}
		
		int turma = v.getTurma();
		Disciplina* disc = v.getDisciplina();
		Campus *cp=NULL;

		double coef=0;
		if ( v.getType() == VariableMIPUnico::V_CREDITOS )
		{
			cp = problemData->refCampus[ v.getUnidade()->getIdCampus() ];
			if ( cp->getId() != campusId ) continue;

			HorarioAula *hi = v.getHorarioAulaInicial();
			HorarioAula *hf = v.getHorarioAulaFinal();
			Calendario *calendario = hi->getCalendario();
			coef = calendario->retornaNroCreditosEntreHorarios(hi, hf);
		}
		else if ( v.getType() == VariableMIPUnico::V_ABERTURA )
		{
			cp = v.getCampus();
			if ( cp->getId() != campusId ) continue;

			coef = - disc->getTotalCreditos();			
		}

		c.reset();
		c.setType( ConstraintMIPUnico::C_TURMA_COM_MESMOS_ALUNOS_POR_AULA );
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

			// Insere restri��o
			cHashTatico[ c ] = lp->getNumRows();
			lp->addRow( row );
			restricoes++;
		}
   }

   chgCoeffList( coeffList, coeffListVal );

   return restricoes;
}


/*
Aulas de disciplinas pratica/teorica continuas

sum[s] x_{it,dt,s,t,hi,hf} <= sum[h]sum[s] x_{ip,dp,s,t,h,hi-1} + sum[h]sum[s] x_{ip,dp,s,t,hf+1,h}

Para toda disciplina d=(dt,dp) sendo d com obriga��o de cr�ditos cont�nuos
Para toda turma it \in I_{dt}, ip \in I_{dp} sendo que (it,dt) tem aluno comum com (ip,dp)
Para todo dia t
Para todo par de hor�rios hi,hf
	
*/
int MIPUnico::criaRestricaoTaticoDiscPTAulasContinuas( int campusId, int prioridade )
{
	int restricoes = 0;
	int nnz;
	char name[ 100 ];

	VariableMIPUnico v;
	ConstraintMIPUnico c;
	VariableMIPUnicoHash::iterator vit;
	ConstraintMIPUnicoHash::iterator cit;

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

   std::map<Disciplina*, std::map< int /*turma*/, std::map< int /*dia*/, std::map< DateTime, GGroup<VariableMIPUnico> > > >, LessPtr<Disciplina>> mapDiscDiaHiVar;

	vit = vHashTatico.begin();
	while(vit != vHashTatico.end())
	{
		if( vit->first.getType() == VariableMIPUnico::V_CREDITOS )
		{
			VariableMIPUnico v = vit->first;
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

	std::map<Disciplina*, std::map< int /*turma*/, std::map< int /*dia*/, std::map<DateTime, GGroup<VariableMIPUnico>> > >, LessPtr<Disciplina>>::iterator
		itMapDisc = mapDiscDiaHiVar.begin();
	for ( ; itMapDisc != mapDiscDiaHiVar.end(); itMapDisc++ )
	{	
		if ( itMapDisc->first->getId() < 0 ) continue;

		Disciplina *disciplinaTeor = itMapDisc->first;

		std::map< int /*turma*/, std::map< int /*dia*/, std::map< DateTime, GGroup<VariableMIPUnico> > > >::iterator
			itTurma = itMapDisc->second.begin();
		for ( ; itTurma != itMapDisc->second.end(); itTurma++ )
		{
			int turmaTeor = itTurma->first;

			std::map< int /*dia*/, std::map< DateTime, GGroup<VariableMIPUnico> > >::iterator
				itMapDia = itTurma->second.begin();

			for ( ; itMapDia != itTurma->second.end(); itMapDia++ )
			{	
				int dia = itMapDia->first;

				std::map< DateTime, GGroup<VariableMIPUnico> >::iterator
					itMapDateTime = itMapDia->second.begin();
				for ( ; itMapDateTime != itMapDia->second.end(); itMapDateTime++ )
				{	
					DateTime dt = itMapDateTime->first;
					GGroup<VariableMIPUnico> ggroupVars = itMapDateTime->second;
				
					ITERA_GGROUP_N_PT( itVars, ggroupVars, VariableMIPUnico )
					{
						VariableMIPUnico v_t = *itVars;

						HorarioAula *hi = v_t.getHorarioAulaInicial();
						HorarioAula *hf = v_t.getHorarioAulaFinal();				
					
						DateTime *dti = problemData->horarioAulaDateTime[hi->getId()].first;
						DateTime *dtf = problemData->horarioAulaDateTime[hf->getId()].first;

						c.reset();
						c.setType( ConstraintMIPUnico::C_AULA_PT_SEQUENCIAL );
						c.setCampus( campus );
						c.setTurma( turmaTeor );
						c.setDisciplina( disciplinaTeor );
						c.setDia( dia );
						c.setDateTimeInicial( *dti );
						c.setDateTimeFinal( *dtf );
					
						sprintf( name, "%s", c.toString( etapa ).c_str() );
						OPT_ROW row( 100, OPT_ROW::LESS , 0.0 , name );
				
						// --------------------
						// te�rica
						row.insert( vHashTatico[v_t], 1.0 );
				
						Disciplina * disciplinaPrat = problemData->getDisciplinaTeorPrat( disciplinaTeor );
						Calendario * calendario = hf->getCalendario();
						int nCredsPrat = disciplinaPrat->getTotalCreditos();

						// --------------------
						// antes da te�rica
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
								std::map< int /*dia*/, std::map< DateTime, GGroup<VariableMIPUnico> > > >, LessPtr<Disciplina>>::iterator
							itDisc = mapDiscDiaHiVar.find( disciplinaPrat );
							if ( itDisc != mapDiscDiaHiVar.end() )
							{
								std::map< int /*turma*/, std::map< int /*dia*/, std::map< DateTime, GGroup<VariableMIPUnico> > > >::iterator
								itTurma = itDisc->second.find( turmaTeor );
								if ( itTurma != itDisc->second.end() )
								{
									std::map< int /*dia*/, std::map< DateTime, GGroup<VariableMIPUnico> > >::iterator
									itDia = itTurma->second.find( dia );
									if ( itDia != itTurma->second.end() )
									{
										std::map< DateTime, GGroup<VariableMIPUnico> >::iterator
										itDt = itDia->second.find( dti_p1 );
										if ( itDt != itDia->second.end() )
										{
											GGroup<VariableMIPUnico> vars = itDt->second;
											ITERA_GGROUP_N_PT( itVars, ggroupVars, VariableMIPUnico )
											{
												VariableMIPUnico v_p1 = *itVars;
												row.insert( vHashTatico[v_p1], -1.0 );
											}
										}
									}
								}
							}										
						}

						// --------------------
						// ap�s a te�rica
						HorarioAula *hi_p2 = calendario->getProximoHorario( hf );
						if ( hi_p2 != NULL )
						{
							DateTime dti_p2 = hi_p2->getInicio();
									
							std::map<Disciplina*, std::map< int /*turma*/, 
								std::map< int /*dia*/, std::map< DateTime, GGroup<VariableMIPUnico> > > >, LessPtr<Disciplina>>::iterator
							itDisc = mapDiscDiaHiVar.find( disciplinaPrat );
							if ( itDisc != mapDiscDiaHiVar.end() )
							{
								std::map< int /*turma*/, std::map< int /*dia*/, std::map< DateTime, GGroup<VariableMIPUnico> > > >::iterator
								itTurma = itDisc->second.find( turmaTeor );
								if ( itTurma != itDisc->second.end() )
								{
									std::map< int /*dia*/, std::map< DateTime, GGroup<VariableMIPUnico> > >::iterator
									itDia = itTurma->second.find( dia );
									if ( itDia != itTurma->second.end() )
									{
										std::map< DateTime, GGroup<VariableMIPUnico> >::iterator
										itDt = itDia->second.find( dti_p2 );
										if ( itDt != itDia->second.end() )
										{
											GGroup<VariableMIPUnico> vars = itDt->second;
											ITERA_GGROUP_N_PT( itVars, ggroupVars, VariableMIPUnico )
											{
												VariableMIPUnico v_p2 = *itVars;
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


int MIPUnico::criaRestricaoTaticoAlunoDiscPraticaTeorica_1xN_antiga( int campusId )
{
	int numRest=0;

	// Por enquanto s� a Unit usa essa rela��o 1xN
	if ( ! problemData->parametros->discPratTeor1xN_antigo )
	{
		return numRest;
	}

    char name[ 100 ];

	VariableMIPUnico v;
	ConstraintMIPUnico c;
	VariableMIPUnicoHash::iterator vit;
	ConstraintMIPUnicoHash::iterator cit;

	map< int /*turma*/, map< Disciplina*, map<int, VariableMIPUnico>, LessPtr<Disciplina> > >
		mapTurmaDiscTeorAlunos;

	map< Disciplina*, map< Aluno*, map<Aluno*, int, LessPtr<Aluno>>, LessPtr<Aluno> >, LessPtr<Disciplina> >
		mapDiscTeorAluno1Aluno2VarSS;

	vit = vHashTatico.begin();
	for ( ; vit != vHashTatico.end(); vit++ )
	{
		v = vit->first;

		if( v.getType() == VariableMIPUnico::V_ALOCA_ALUNO_TURMA_DISC ) // s_{i,d,a}
		{				
			int discId = v.getDisciplina()->getId();
			if ( discId < 0 ) continue;

			// S� para disciplinas praticas/teoricas
			std::map< int, Disciplina * >::iterator itMapDisc = problemData->refDisciplinas.find( - discId );
			if ( itMapDisc == problemData->refDisciplinas.end() )
				continue;
		
			int turmaTeor = v.getTurma();
			Disciplina *discTeor = v.getDisciplina();

			mapTurmaDiscTeorAlunos[turmaTeor][discTeor][vit->second] = v;
		}
		else if( v.getType() == VariableMIPUnico::V_ALUNOS_MESMA_TURMA_PRAT ) // ss_{a1,a2,dp}
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

	map< int /*turma*/, map< Disciplina*, map<int, VariableMIPUnico>, LessPtr<Disciplina> > >::iterator
		itMapTurma = mapTurmaDiscTeorAlunos.begin();
	for ( ; itMapTurma != mapTurmaDiscTeorAlunos.end(); itMapTurma++ )
	{
		int turmaTeor = itMapTurma->first;

		map< Disciplina*, map<int, VariableMIPUnico>, LessPtr<Disciplina> >::iterator
			itMapDisc = itMapTurma->second.begin();
		for ( ; itMapDisc != itMapTurma->second.end(); itMapDisc++ )
		{
			Disciplina *discTeor = itMapDisc->first;

			map<int, VariableMIPUnico> variables = itMapDisc->second;

			map<int, VariableMIPUnico>::iterator itMapVar1 = variables.begin();
			for( ; itMapVar1 != variables.end(); itMapVar1++ )
			{
				VariableMIPUnico v1 = itMapVar1->second; // s_{i,d,a1}
				int indx1 = itMapVar1->first;

				map<int, VariableMIPUnico>::iterator itMapVar2 = variables.begin();
				for( ; itMapVar2 != variables.end(); itMapVar2++ )
				{
					VariableMIPUnico v2 = itMapVar2->second; // s_{i,d,a2}
					int indx2 = itMapVar2->first;

					if ( v1.getAluno()->getAlunoId() >= v2.getAluno()->getAlunoId() ) continue;

					std::pair<Aluno*, Aluno*> parAlunos( v1.getAluno(), v2.getAluno() );

					int indx3 = mapDiscTeorAluno1Aluno2VarSS[discTeor][v1.getAluno()][v2.getAluno()]; // ss_{dp,a1,a2}

					c.reset();
					c.setType( ConstraintMIPUnico::C_ALUNO_DISC_PRATICA_TEORICA_1xN );
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


int MIPUnico::criaRestricaoTaticoAlunosMesmaTurmaPratica( int campusId )
{
	int numRest=0;

	// Por enquanto s� a Unit usa a rela��o 1xN
	if ( ! problemData->parametros->discPratTeor1xN_antigo )
	{
		return numRest;
	}

    char name[ 100 ];

	VariableMIPUnico v;
	ConstraintMIPUnico c;
	VariableMIPUnicoHash::iterator vit;
	ConstraintMIPUnicoHash::iterator cit;

	map< int /*turma*/, map< Disciplina*, map<int, VariableMIPUnico>, LessPtr<Disciplina> > >
		mapTurmaDiscPratAlunos;

	map< Disciplina*, map< Aluno*, map<Aluno*, int, LessPtr<Aluno>>, LessPtr<Aluno> >, LessPtr<Disciplina> >
		mapDiscPratAluno1Aluno2VarSS;

	vit = vHashTatico.begin();
	for ( ; vit != vHashTatico.end(); vit++ )
	{
		v = vit->first;

		if( v.getType() == VariableMIPUnico::V_ALOCA_ALUNO_TURMA_DISC ) // s_{i,d,a}
		{				
			int discId = v.getDisciplina()->getId();
			if ( discId > 0 ) continue;
					
			int turmaPrat = v.getTurma();
			Disciplina *discPrat = v.getDisciplina();

			mapTurmaDiscPratAlunos[turmaPrat][discPrat][vit->second] = v;
		}
		else if( v.getType() == VariableMIPUnico::V_ALUNOS_MESMA_TURMA_PRAT ) // ss_{a1,a2,dp}
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

	map< int /*turma*/, map< Disciplina*, map<int, VariableMIPUnico>, LessPtr<Disciplina> > >::iterator
		itMapTurma = mapTurmaDiscPratAlunos.begin();
	for ( ; itMapTurma != mapTurmaDiscPratAlunos.end(); itMapTurma++ )
	{
		int turmaPrat = itMapTurma->first;

		map< Disciplina*, map<int, VariableMIPUnico>, LessPtr<Disciplina> >::iterator
			itMapDisc = itMapTurma->second.begin();
		for ( ; itMapDisc != itMapTurma->second.end(); itMapDisc++ )
		{
			Disciplina *discPrat = itMapDisc->first;

			map<int, VariableMIPUnico> variables = itMapDisc->second;

			map<int, VariableMIPUnico>::iterator itMapVar1 = variables.begin();
			for( ; itMapVar1 != variables.end(); itMapVar1++ )
			{
				VariableMIPUnico v1 = itMapVar1->second; // s_{i,d,a1}
				int indx1 = itMapVar1->first;

				map<int, VariableMIPUnico>::iterator itMapVar2 = variables.begin();
				for( ; itMapVar2 != variables.end(); itMapVar2++ )
				{
					VariableMIPUnico v2 = itMapVar2->second; // s_{i,d,a2}
					int indx2 = itMapVar2->first;

					if ( v1.getAluno()->getAlunoId() >= v2.getAluno()->getAlunoId() ) continue;

					std::pair<Aluno*, Aluno*> parAlunos( v1.getAluno(), v2.getAluno() );

					int indx3 = mapDiscPratAluno1Aluno2VarSS[discPrat][v1.getAluno()][v2.getAluno()]; // ss_{dp,a1,a2}

					c.reset();
					c.setType( ConstraintMIPUnico::C_ALUNOS_MESMA_TURMA_PRAT );
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
	Aloca��o minima de demanda por aluno

	sum[d] nCreds_{d} * (1 - fd_{d,a}) >= MinAtendPerc * TotalDemanda_{a} - fmd1_{a} - fmd2_{a} - fmd3_{a}, para cada aluno a
	
	min sum[a] fmd{a}
*/
int MIPUnico::criaRestricaoTaticoAlocMinAluno( int campusId )
{
   int restricoes = 0;
 
   if ( ! problemData->parametros->considerarMinPercAtendAluno )
   {
		return restricoes;
   }

   char name[ 200 ];
   int nnz=0;

   ConstraintMIPUnico c;
   ConstraintMIPUnicoHash::iterator cit;
   VariableMIPUnico v;
   VariableMIPUnicoHash::iterator vit;   

   std::map<int, int> mapConstraintSomaCredFD;

	vit = vHashTatico.begin();
	for ( ; vit != vHashTatico.end(); vit++ )
	{
		VariableMIPUnico v = vit->first;
			
		double coef;
				
		if( v.getType() == VariableMIPUnico::V_SLACK_DEMANDA_ALUNO )			// fd_{d,a}
		{
			coef = - v.getDisciplina()->getTotalCreditos();
		}
		else if( v.getType() == VariableMIPUnico::V_FOLGA_ALUNO_MIN_ATEND1 || // fmd1_{a}
				 v.getType() == VariableMIPUnico::V_FOLGA_ALUNO_MIN_ATEND2 ||	// fmd2_{a}
				 v.getType() == VariableMIPUnico::V_FOLGA_ALUNO_MIN_ATEND3 )	// fmd3_{a}
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
		c.setType( ConstraintMIPUnico::C_ALOC_MIN_ALUNO );
		c.setAluno( aluno );
		
		sprintf( name, "%s", c.toString(etapa).c_str() ); 

		cit = cHashTatico.find( c ); 
		if ( cit != cHashTatico.end() )
		{
			lp->chgCoef( cit->second, vit->second, coef );
			
			if( v.getType() == VariableMIPUnico::V_SLACK_DEMANDA_ALUNO ) // fd_{d,a}
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

			init_rhs -= jaAtendido; // subtraio o j� atendido pq n�o h� folga de demanda para atendimentos j� fixados pelas etapas anteriores

			nnz += aluno->demandas.size() + 1;
			OPT_ROW row( nnz, OPT_ROW::GREATER, init_rhs , name );
		
			row.insert( vit->second, coef );
			cHashTatico[ c ] = lp->getNumRows();
			
			if( v.getType() == VariableMIPUnico::V_SLACK_DEMANDA_ALUNO ) // fd_{d,a}
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
	Minimiza o gap entre capacidade da sala e sua ocupa��o para cada turma

	sum[s] Cap_{s} * o_{i,d,s,cp} - sum[a] s_{i,d,cp,a} = fos_{i,d,cp}
	
	para cada turma i, disciplina d, campus cp
	
	min sum[i]sum[d]sum[cp] fos{i,d,cp}
*/
int MIPUnico::criaRestricaoFolgaOcupacaoSala( int campusId )
{
    int restricoes = 0;		

	if ( ! problemData->parametros->min_folga_ocupacao_sala )
	{
		return restricoes;
	}

    char name[ 100 ];
    ConstraintMIPUnico c;
	ConstraintMIPUnicoHash::iterator cit;
    VariableMIPUnico v;
    VariableMIPUnicoHash::iterator vit;

	vit = vHashTatico.begin();
	for ( ; vit != vHashTatico.end(); vit++ )
	{
		// o_{i,d,s}
		if( vit->first.getType() == VariableMIPUnico::V_OFERECIMENTO )
		{			
			VariableMIPUnico v = vit->first;

			Disciplina *disciplina = v.getDisciplina();
			int turma = v.getTurma();
			Campus* campus = problemData->refCampus[ v.getUnidade()->getIdCampus() ];

			c.reset();
			c.setType( ConstraintMIPUnico::C_FOLGA_OCUPACAO_SALA );
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
		else if( vit->first.getType() == VariableMIPUnico::V_ALOCA_ALUNO_TURMA_DISC )
		{			
			VariableMIPUnico v = vit->first;

			Disciplina *disciplina = v.getDisciplina();
			int turma = v.getTurma();
			Campus* campus = v.getCampus();

			c.reset();
			c.setType( ConstraintMIPUnico::C_FOLGA_OCUPACAO_SALA );
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
		else if( vit->first.getType() == VariableMIPUnico::V_FOLGA_OCUPA_SALA )
		{			
			VariableMIPUnico v = vit->first;

			Disciplina *disciplina = v.getDisciplina();
			int turma = v.getTurma();
			Campus* campus = v.getCampus();

			c.reset();
			c.setType( ConstraintMIPUnico::C_FOLGA_OCUPACAO_SALA );
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
	Garante o tempo de descanso m�nimo entre o ultimo horario de aula que o professor
	ministra no dia t e o primeiro horario de aula que este ministra no dia t+1

	Necess�rio no t�tico caso o professor das aulas de mesma turma tenha que ser o mesmo.

	Para cada par de aulas de uma mesma turma em dias consecutivos com [ (hf,t) e (hi,t+1) ] 
	tal que o tempo entre (hf,t) e (hi,t+1) � menor do que o tempo m�nimo de descanso:

	x_{a1,hf} +  x_{a2,hi} <= 1

	com a1 E At,  a2 E At+1

*/
int MIPUnico::criaRestricaoProfDescansoMinimo( int campusId )
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

   VariableMIPUnico v;
   ConstraintMIPUnico c;
   VariableMIPUnicoHash::iterator vit;
   ConstraintMIPUnicoHash::iterator cit;
   
   // Aulas organizadas em rela��o aos DateTimes de IN�CIO da aula
   std::map<Disciplina*, std::map< int /*turma*/,  std::map< int /*dia*/, 
	   std::map<DateTime*, GGroup<int> > > >, LessPtr<Disciplina> > mapDiscTurmaDiaHiVarId;

   // Aulas organizadas em rela��o aos DateTimes de T�RMIDO da aula
   std::map<Disciplina*, std::map< int /*turma*/,  std::map< int /*dia*/, 
	   std::map<DateTime*, GGroup<int> > > >, LessPtr<Disciplina> > mapDiscTurmaDiaHfVarId;

   std::cout<<"\nCriando map de vars..."; fflush(NULL);

	vit = vHashTatico.begin();
	for ( ; vit != vHashTatico.end(); vit++ )
	{
		if ( vit->first.getType() == VariableMIPUnico::V_CREDITOS )
		{	
			VariableMIPUnico v = vit->first;
			
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

				// Verifica se h� aulas iniciando no dia seguinte
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

					// A restri��o � somente para a ultima aula de um dia e a primeira aula do dia SEGUINTE
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
						c.setType( ConstraintMIPUnico::C_PROF_MIN_DESCANSO );
						c.setDisciplina( disciplina );
						c.setTurma( turma );
						c.setDia( dia );
						c.setDateTimeFinal( *dtf );
						c.setDateTimeInicial( *dti );
					
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


/*
	Evita sobreposi��o de hor�rios de professores em um mesmo dia.
	
	Para cada professor p, dia t, datetime dt:

	sum[i]sum[d]sum[u]sum[dti]sum[dtf] k_{p,i,d,u,t,dti,dtf} <= 1

	sendo que (dti,dtf) sobrep�e dt

*/
int MIPUnico::criaRestricaoSobreposHorariosProfs()
{
	int restricoes = 0;
	int nnz;
	char name[ 1024 ];

	VariableMIPUnico v;	
    ConstraintMIPUnico c;
    ConstraintMIPUnicoHash::iterator cit;

    std::vector<int> idxC;
    std::vector<int> idxR;
    std::vector<double> valC;
				
	// Prof
	auto itVarsProf = vars_prof_aula2.begin();
	for ( ; itVarsProf != vars_prof_aula2.end(); itVarsProf++ )
	{
		Professor *professor = itVarsProf->first;

		if ( professor->eVirtual() ) continue;
		
		// Dia
		auto itVarsDia = itVarsProf->second.begin();
		for ( ; itVarsDia != itVarsProf->second.end(); itVarsDia++ )
		{
			int dia = itVarsDia->first;
			
			// Horarios de disponib do prof
			auto *mapDia = & professor->mapDiaDtiDtf[dia];
			for( auto itDti=mapDia->begin(); itDti!=mapDia->end(); itDti++ )
			{				
				DateTime pDti = itDti->first;
					
				// Sets constraint
				c.reset();
				c.setType( ConstraintMIPUnico::C_PROF_DIA_HOR );
				c.setProfessor( professor );
				c.setDia( dia );
				c.setDateTimeInicial( pDti );
				
				cit = cHashTatico.find(c);
				if(cit == cHashTatico.end())
				{
					int nnz = itVarsDia->second.size();

					sprintf( name, "%s", c.toString( etapa ).c_str() );
					OPT_ROW row( nnz, OPT_ROW::LESS, 1, name );
					
					int nVars = 0;

					// Dti das vari�veis
					auto itVarsDti = itVarsDia->second.begin();
					for ( ; itVarsDti != itVarsDia->second.end(); itVarsDti++ )
					{
						DateTime vDti = itVarsDti->first;
						
						if ( vDti > pDti )
							break;

						// Vars
						auto itVarsUnid = itVarsDti->second.begin();
						for ( ; itVarsUnid != itVarsDti->second.end(); itVarsUnid++ )
						{
							auto itVarsDisc = itVarsUnid->second.begin();
							for ( ; itVarsDisc != itVarsUnid->second.end(); itVarsDisc++ )
							{
								auto itVarsTurma = itVarsDisc->second.begin();
								for ( ; itVarsTurma != itVarsDisc->second.end(); itVarsTurma++ )
								{
									int varCol = itVarsTurma->second.first;
									VariableMIPUnico v = itVarsTurma->second.second;

									if ( pDti >= v.getDateTimeInicial() && pDti < v.getDateTimeFinal() )
									{
										row.insert( varCol, 1.0);
										nVars++;
									}
								}
							}
						}
					}

					// Inserts constraint
					if ( nVars )
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
	Associa professor � aula (seta vari�vel k).
	
	Para cada turma i, disciplina d, unidade u, professor p, dia t, datetime dti:

	sum[hi] sum[hf] sum[s] x_{i,d,s,u,t,hi,hf} <= k_{p,i,d,u,t,dti} + ( 1 - y_{p,i,d,cp} )

	sendo que (hi,hf) cont�m dti

*/
int MIPUnico::criaRestricaoProfAula()
{
	int restricoes = 0;
	int nnz;
	char name[ 1024 ];

	VariableMIPUnico v;	
    ConstraintMIPUnico c;
    ConstraintMIPUnicoHash::iterator cit;
	
	// Percorrendo variaveis k
	// Unidade
	auto itVarsUnid = vars_prof_aula1.begin();
	for ( ; itVarsUnid != vars_prof_aula1.end(); itVarsUnid++ )
	{
		Unidade *unidade = itVarsUnid->first;
		
		auto itVarUnidX = vars_turma_aula2.find(unidade);
		if ( itVarUnidX == vars_turma_aula2.end() ) continue;

		Campus* campus = problemData->refCampus[unidade->getIdCampus()];
		auto itVarCpY = vars_prof_turma.find(campus);
		if ( itVarCpY == vars_prof_turma.end() ) continue;				

		// Disciplina
		auto itVarsDisc = itVarsUnid->second.begin();
		for ( ; itVarsDisc != itVarsUnid->second.end(); itVarsDisc++ )
		{
			Disciplina *disciplina = itVarsDisc->first;
					
			auto itVarDiscX = itVarUnidX->second.find(disciplina);
			if ( itVarDiscX == itVarUnidX->second.end() ) continue;
			
			auto itVarDiscY = itVarCpY->second.find(disciplina);
			if ( itVarDiscY == itVarCpY->second.end() ) continue;

			// Turma
			auto itVarsTurma = itVarsDisc->second.begin();
			for ( ; itVarsTurma != itVarsDisc->second.end(); itVarsTurma++ )
			{
				int turma = itVarsTurma->first;

				auto itVarTurmaX = itVarDiscX->second.find(turma);
				if ( itVarTurmaX == itVarDiscX->second.end() ) continue;
				
				auto itVarTurmaY = itVarDiscY->second.find(turma);
				if ( itVarTurmaY == itVarDiscY->second.end() ) continue;

				// Professor
				auto itVarsProf = itVarsTurma->second.begin();
				for ( ; itVarsProf != itVarsTurma->second.end(); itVarsProf++ )
				{
					Professor *professor = itVarsProf->first;
	
					// Var y_{p,i,d,cp}
					auto itVarProfY = itVarTurmaY->second.find(professor);
					if ( itVarProfY == itVarTurmaY->second.end() ) continue;
					int yCol = itVarProfY->second.first;

					// Dia
					auto itVarsDia = itVarsProf->second.begin();
					for ( ; itVarsDia != itVarsProf->second.end(); itVarsDia++ )
					{
						int dia = itVarsDia->first;
				
						auto itVarDiaX = itVarTurmaX->second.find(dia);
						if ( itVarDiaX == itVarTurmaX->second.end() ) continue;

						auto *mapDtiX = & itVarDiaX->second;

						// Dti
						auto itVarsDti = itVarsDia->second.begin();
						for ( ; itVarsDti != itVarsDia->second.end(); itVarsDti++ )
						{
							DateTime dti = itVarsDti->first;
				
							// Vars
							int kCol = itVarsDti->second.first;
							VariableMIPUnico kVar = itVarsDti->second.second;

							DateTime kDti = kVar.getDateTimeInicial();
							DateTime kDtf = kVar.getDateTimeFinal();

							// Sets constraint
							c.reset();
							c.setType( ConstraintMIPUnico::C_PROF_AULA );
							c.setProfessor( kVar.getProfessor() );
							c.setDia( kVar.getDia() );
							c.setDateTimeInicial( kVar.getDateTimeInicial() );
							c.setDateTimeFinal( kVar.getDateTimeFinal() );
							c.setTurma( kVar.getTurma() );
							c.setDisciplina( kVar.getDisciplina() );
							c.setUnidade( kVar.getUnidade() );

							cit = cHashTatico.find(c);
							if(cit == cHashTatico.end())
							{
								int nnz = itVarsDia->second.size();

								sprintf( name, "%s", c.toString( etapa ).c_str() );
								OPT_ROW row( nnz, OPT_ROW::LESS, 1, name );
								
								// -------------------------------------
								// k
								row.insert( kCol, -1.0 );
								
								// -------------------------------------
								// y
								row.insert( yCol, 1.0 );

								// -------------------------------------
								// x
								int nVars=0;								
								for ( auto itDtiX = mapDtiX->begin(); itDtiX != mapDtiX->end(); itDtiX++ )
								{
									if ( itDtiX->first > kDtf )					// x_{dti} > k_{dtf}
										break;

									for ( auto itVarsX = itDtiX->second.begin(); itVarsX != itDtiX->second.end(); itVarsX++ )
									{
										int xCol = itVarsX->first;
										VariableMIPUnico x = itVarsX->second;

										if ( x.getDateTimeFinal() < kDti )		// x_{dtf} < k_{dti}
											continue;

										bool found=false;

										HorarioAula *h = x.getHorarioAulaInicial();								
										while ( h!=nullptr && !found )
										{
											DateTime xDti = h->getInicio();
											DateTime xDtf = h->getFinal();											

											if ( xDti.sameTime(kDti) && xDtf.sameTime(kDtf) )
												found = true;					// aula x possui o horario indicado na variavel k
											else
												h = h->getCalendario()->getProximoHorario(h);
										}

										// inserts x
										if (found)
										{
											row.insert( xCol, 1.0);
											nVars++;
										}
									}
								}								
								// -------------------------------------

								// Inserts constraint
								if ( nVars )
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
	
	return restricoes;
}


/*
	Associa professor � aula (seta vari�vel k).
	
	Para cada turma i, disciplina d, unidade u, dia t, datetime dti:

	sum[hi] sum[hf] sum[s] x_{i,d,s,u,t,hi,hf} = sum[p] k_{p,i,d,u,t,dti}

	sendo que (hi,hf) cont�m dti

*/
int MIPUnico::criaRestricaoProfAulaSum()
{
	int restricoes = 0;
	int nnz;
	char name[ 1024 ];

	VariableMIPUnico v;	
    ConstraintMIPUnico c;
    ConstraintMIPUnicoHash::iterator cit;
	
	// Percorrendo variaveis k
	// Unidade
	auto itVarsUnid = vars_prof_aula1.begin();
	for ( ; itVarsUnid != vars_prof_aula1.end(); itVarsUnid++ )
	{
		Unidade *unidade = itVarsUnid->first;
		
		auto itVarUnidX = vars_turma_aula2.find(unidade);
		if ( itVarUnidX == vars_turma_aula2.end() ) continue;

		// Disciplina
		auto itVarsDisc = itVarsUnid->second.begin();
		for ( ; itVarsDisc != itVarsUnid->second.end(); itVarsDisc++ )
		{
			Disciplina *disciplina = itVarsDisc->first;
					
			auto itVarDiscX = itVarUnidX->second.find(disciplina);
			if ( itVarDiscX == itVarUnidX->second.end() ) continue;

			// Turma
			auto itVarsTurma = itVarsDisc->second.begin();
			for ( ; itVarsTurma != itVarsDisc->second.end(); itVarsTurma++ )
			{
				int turma = itVarsTurma->first;

				auto itVarTurmaX = itVarDiscX->second.find(turma);
				if ( itVarTurmaX == itVarDiscX->second.end() ) continue;

				// Professor
				auto itVarsProf = itVarsTurma->second.begin();
				for ( ; itVarsProf != itVarsTurma->second.end(); itVarsProf++ )
				{
					Professor *professor = itVarsProf->first;
	
					// Dia
					auto itVarsDia = itVarsProf->second.begin();
					for ( ; itVarsDia != itVarsProf->second.end(); itVarsDia++ )
					{
						int dia = itVarsDia->first;
				
						auto itVarDiaX = itVarTurmaX->second.find(dia);
						if ( itVarDiaX == itVarTurmaX->second.end() ) continue;

						auto *mapDtiX = & itVarDiaX->second;

						// Dti
						auto itVarsDti = itVarsDia->second.begin();
						for ( ; itVarsDti != itVarsDia->second.end(); itVarsDti++ )
						{
							DateTime dti = itVarsDti->first;
				
							// Vars
							int kCol = itVarsDti->second.first;
							VariableMIPUnico kVar = itVarsDti->second.second;

							DateTime kDti = kVar.getDateTimeInicial();
							DateTime kDtf = kVar.getDateTimeFinal();

							// Sets constraint
							c.reset();
							c.setType( ConstraintMIPUnico::C_PROF_AULA_SUM );
							c.setDia( kVar.getDia() );
							c.setDateTimeInicial( kVar.getDateTimeInicial() );
							c.setDateTimeFinal( kVar.getDateTimeFinal() );
							c.setTurma( kVar.getTurma() );
							c.setDisciplina( kVar.getDisciplina() );
							c.setUnidade( kVar.getUnidade() );

							cit = cHashTatico.find(c);
							if(cit == cHashTatico.end())
							{
								int nnz = itVarsDia->second.size();

								sprintf( name, "%s", c.toString( etapa ).c_str() );
								OPT_ROW row( nnz, OPT_ROW::EQUAL, 0, name );
								
								// -------------------------------------
								// k
								row.insert( kCol, -1.0);

								// -------------------------------------
								// x
								int nVars=0;								
								for ( auto itDtiX = mapDtiX->begin(); itDtiX != mapDtiX->end(); itDtiX++ )
								{
									if ( itDtiX->first > kDtf )					// x_{dti} > k_{dtf}
										break;

									for ( auto itVarsX = itDtiX->second.begin(); itVarsX != itDtiX->second.end(); itVarsX++ )
									{
										int xCol = itVarsX->first;
										VariableMIPUnico x = itVarsX->second;

										if ( x.getDateTimeFinal() < kDti )		// x_{dtf} < k_{dti}
											continue;

										bool found=false;

										HorarioAula *h = x.getHorarioAulaInicial();								
										while ( h!=nullptr && !found )
										{
											DateTime xDti = h->getInicio();
											DateTime xDtf = h->getFinal();											

											if ( xDti.sameTime(kDti) && xDtf.sameTime(kDtf) )
												found = true;					// aula x possui o horario indicado na variavel k
											else
												h = h->getCalendario()->getProximoHorario(h);
										}

										// inserts x
										if (found)
										{
											row.insert( xCol, 1.0);
											nVars++;
										}
									}
								}								
								// -------------------------------------

								// Inserts constraint
								if ( nVars )
								{
									cHashTatico[ c ] = lp->getNumRows();
									lp->addRow( row );
									restricoes++;
								}
							}
							else
							{
								// -------------------------------------
								// k
								lp->chgCoef( cit->second, kCol, -1.0 );

								// -------------------------------------
								// x
								int nVars=0;								
								for ( auto itDtiX = mapDtiX->begin(); itDtiX != mapDtiX->end(); itDtiX++ )
								{
									if ( itDtiX->first > kDtf )					// x_{dti} > k_{dtf}
										break;

									for ( auto itVarsX = itDtiX->second.begin(); itVarsX != itDtiX->second.end(); itVarsX++ )
									{
										int xCol = itVarsX->first;
										VariableMIPUnico x = itVarsX->second;

										if ( x.getDateTimeFinal() < kDti )		// x_{dtf} < k_{dti}
											continue;

										bool found=false;

										HorarioAula *h = x.getHorarioAulaInicial();								
										while ( h!=nullptr && !found )
										{
											DateTime xDti = h->getInicio();
											DateTime xDtf = h->getFinal();											

											if ( xDti.sameTime(kDti) && xDtf.sameTime(kDtf) )
												found = true;					// aula x possui o horario indicado na variavel k
											else
												h = h->getCalendario()->getProximoHorario(h);
										}

										// inserts x
										if (found)
										{
											lp->chgCoef( cit->second, xCol, 1.0 );
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


/*
	Associa professor � turma (seta vari�vel y).
	
	Para cada turma i, disciplina d, campus cp, professor p:

	sum[u] sum[t] sum[h] k_{p,i,d,u,t,h} = nCreds_{d} * y_{p,i,d,cp}
	
*/
int MIPUnico::criaRestricaoProfTurma()
{
	int restricoes = 0;
	int nnz;
	char name[ 1024 ];

	VariableMIPUnico v;	
    ConstraintMIPUnico c;
    ConstraintMIPUnicoHash::iterator cit;


	// Percorrendo variaveis k
	// Unidade
	auto itVarsUnid = vars_prof_aula1.begin();
	for ( ; itVarsUnid != vars_prof_aula1.end(); itVarsUnid++ )
	{
		Unidade *unidade = itVarsUnid->first;
		
		Campus *campus = problemData->refCampus[unidade->getIdCampus()];
		auto itVarCpY = vars_prof_turma.find(campus);
		if ( itVarCpY == vars_prof_turma.end() ) continue;

		// Disciplina
		auto itVarsDisc = itVarsUnid->second.begin();
		for ( ; itVarsDisc != itVarsUnid->second.end(); itVarsDisc++ )
		{
			Disciplina *disciplina = itVarsDisc->first;
					
			auto itVarDiscY = itVarCpY->second.find(disciplina);
			if ( itVarDiscY == itVarCpY->second.end() ) continue;

			// Turma
			auto itVarsTurma = itVarsDisc->second.begin();
			for ( ; itVarsTurma != itVarsDisc->second.end(); itVarsTurma++ )
			{
				int turma = itVarsTurma->first;

				auto itVarTurmaY = itVarDiscY->second.find(turma);
				if ( itVarTurmaY == itVarDiscY->second.end() ) continue;

				// Professor
				auto itVarsProf = itVarsTurma->second.begin();
				for ( ; itVarsProf != itVarsTurma->second.end(); itVarsProf++ )
				{
					Professor *professor = itVarsProf->first;
	
					auto itVarProfY = itVarTurmaY->second.find(professor);
					if ( itVarProfY == itVarTurmaY->second.end() ) continue;

					// -------------------------------------
					// Variavel y_{p,i,d,cp}
					int yCol = itVarProfY->second.first;
										
					// -------------------------------------
					// Sets constraint
					c.reset();
					c.setType( ConstraintMIPUnico::C_PROF_TURMA );
					c.setProfessor( professor );
					c.setTurma( turma );
					c.setDisciplina( disciplina );
					c.setCampus( campus );
					
					cit = cHashTatico.find(c);
					if(cit == cHashTatico.end())
					{
						int nnz = itVarsProf->second.size() + 1;

						sprintf( name, "%s", c.toString( etapa ).c_str() );
						OPT_ROW row( nnz, OPT_ROW::EQUAL, 0, name );
						
						// -------------------------------------
						// Insere variaveis k_{p,i,d,u,t,dti}

						// Dia
						auto itVarsDia = itVarsProf->second.begin();
						for ( ; itVarsDia != itVarsProf->second.end(); itVarsDia++ )
						{
							// Dti
							auto itVarsDti = itVarsDia->second.begin();
							for ( ; itVarsDti != itVarsDia->second.end(); itVarsDti++ )
							{				
								// Vars
								int kCol = itVarsDti->second.first;
								row.insert( kCol, 1.0 );
							}
						}
						
						// -------------------------------------
						// Insere variaveis y_{p,i,d,cp}
						row.insert( yCol, -disciplina->getTotalCreditos() );
						
						// -------------------------------------
						// Insere restri��o
						cHashTatico[ c ] = lp->getNumRows();
						lp->addRow( row );
						restricoes++;
					}
					else
					{
						// -------------------------------------
						// Insere variaveis k_{p,i,d,u,t,dti}

						// Dia
						auto itVarsDia = itVarsProf->second.begin();
						for ( ; itVarsDia != itVarsProf->second.end(); itVarsDia++ )
						{
							// Dti
							auto itVarsDti = itVarsDia->second.begin();
							for ( ; itVarsDti != itVarsDia->second.end(); itVarsDti++ )
							{				
								// Vars
								int kCol = itVarsDti->second.first;
								lp->chgCoef(cit->second, kCol, 1.0 );
							}
						}						
					}
				}
			}
		}
	}
	
	return restricoes;
}


/*
	Associa �nico professor � turma.
	
	Para cada turma i, disciplina d, campus cp:

	sum[p] y_{p,i,d,cp} = z_{i,d,cp}
	
*/
int MIPUnico::criaRestricaoProfUnico()
{
	int restricoes = 0;
	int nnz;
	char name[ 1024 ];

	VariableMIPUnico v;	
    ConstraintMIPUnico c;
    ConstraintMIPUnicoHash::iterator cit;


	// Percorrendo variaveis y
	// Campus
	auto itVarsCp = vars_prof_turma.begin();
	for ( ; itVarsCp != vars_prof_turma.end(); itVarsCp++ )
	{
		Campus * const campus = itVarsCp->first;
		
		// Var z
		auto itCpZ = vars_abertura_turma.find(campus);
		if ( itCpZ == vars_abertura_turma.end() ) continue;

		// Disciplina
		auto itVarsDisc = itVarsCp->second.begin();
		for ( ; itVarsDisc != itVarsCp->second.end(); itVarsDisc++ )
		{
			Disciplina * const disciplina = itVarsDisc->first;
		
			// Var z
			auto itDiscZ = itCpZ->second.find(disciplina);
			if ( itDiscZ == itCpZ->second.end() ) continue;

			// Turma
			auto itVarsTurma = itVarsDisc->second.begin();
			for ( ; itVarsTurma != itVarsDisc->second.end(); itVarsTurma++ )
			{
				const int turma = itVarsTurma->first;
												
				// Var z
				auto itTurmaZ = itDiscZ->second.find(turma);
				if ( itTurmaZ == itDiscZ->second.end() ) continue;
				const int zCol = itTurmaZ->second.first;
				
				// -------------------------------------
				// Sets constraint
				c.reset();
				c.setType( ConstraintMIPUnico::C_PROF_UNICO );
				c.setTurma( turma );
				c.setDisciplina( disciplina );
				c.setCampus( campus );
					
				cit = cHashTatico.find(c);
				if(cit == cHashTatico.end())
				{
					int nnz = itVarsTurma->second.size() + 1;

					sprintf( name, "%s", c.toString( etapa ).c_str() );
					OPT_ROW row( nnz, OPT_ROW::EQUAL, 0, name );
					
					// -------------------------------------
					// Insere variaveis z_{i,d,cp}
					row.insert( zCol, -1.0 );
					
					// -------------------------------------
					// Insere variaveis y_{p,i,d,cp}
					auto itVarsProf = itVarsTurma->second.begin();
					for ( ; itVarsProf != itVarsTurma->second.end(); itVarsProf++ )
					{
						const int yCol = itVarsProf->second.first;					
						row.insert( yCol, 1.0 );						
					}

					// -------------------------------------
					// Insere restri��o
					cHashTatico[ c ] = lp->getNumRows();
					lp->addRow( row );
					restricoes++;
				}
			}
		}
	}
	
	return restricoes;
}


// --------------------------------------------------------------

// criar restri��es que impedem gaps nos hor�rios do professor em uma mesma fase de um dia

int MIPUnico::criarRestricaoProfHiHf_()
{	
	int numRestr = 0;
	int numRestAnterior = numRestr;

	if ( CentroDados::getProblemData()->parametros->proibirProfGapMTN ==
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
	#pragma region Agrupa vari�veis em mapProfDiaFaseHiHf e mapProfDiaFase
	
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

				for(auto itUnid = itDti->second.begin(); itUnid != itDti->second.end(); ++itUnid)
				{
					for(auto itDisc = itUnid->second.begin(); itDisc != itUnid->second.end(); ++itDisc)
					{
						for(auto itTurma = itDisc->second.begin(); itTurma != itDisc->second.end(); ++itTurma)
						{
							const int col = itTurma->second.first;
							const VariableMIPUnico var = itTurma->second.second;
							
							DateTime dtf = var.getDateTimeFinal();			
				
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
	// - Restri��es para setar ub de hip e lb de hfp de cada dia-faseDoDia do professor
	// - Constru��o de mapProfDiaFaseHilbHfub
	#pragma region Restri��es para setar ub de hip e lb de hfp de cada dia-faseDoDia do professor & Constru��o de mapProfDiaFaseHilbHfub

	for(auto itProf = mapProfDiaFaseHiHf.begin(); itProf != mapProfDiaFaseHiHf.end(); ++itProf)
	{
		Professor *prof = itProf->first;

		auto itProfHiHf = varsProfDiaFaseHiHf.find(prof);
		if ( itProfHiHf == varsProfDiaFaseHiHf.end() )
		{
			stringstream ss;
			ss << "var hip-hfp nao encontrada para prof " << prof->getId();
			CentroDados::printError("MIPUnico::criarRestProfHiHf_", ss.str());
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

					// Adiciona todas as vars x cujo tempo de inicio � MENOR que dti_2
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

					// Adiciona todas as vars x cujo tempo de fim � MAIOR que dtf_2
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
	#pragma region Restri��es para setar lb de hip e ub de hfp de cada dia-faseDoDia do professor
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
	#pragma region Calcula os valores que ser�o usados como rhs das restri��es de gap (a seguir)
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
	#pragma region Restri��es para impedir gap nos hor�rios de mesma fase do dia do professor

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

				// Right hand side � (-) o m�ximo de tempo ocioso permitido dentro de uma mesma fase f do dia t
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

int MIPUnico::criarRestricaoProfHiUB_( Professor* const prof, const int dia, const int fase,
	const int colHi, map<DateTime, set< pair<int,double> >> const &mapDtiVars )
{
	/*
		Restri��o que limita superiormente a vari�vel hip.

		Para todo professor p, todo dia t, todo fase do dia f, todo DateTime dt:

		hip_{p,t,f} <= m(dt) + M*( 1 - sum[x t.q. dti==dt] x_{p,t,h} )

		aonde:
		m(dt)		= datetime dt em n�mero de minutos. Por ex: m(7:30) = 7*60 + 30
		x_{p,t,h}	= vari�vel bin�ria indicando se o prof p d� aula no dia t iniciando no hor�rio h
		hip_{p,t,f}	= vari�vel inteira indicando o primeiro hor�rio (valor em n�mero de minutos) 
					  da fase f do dia t em que prof p d� aula.
		
		"x t.q. dti==dt" significa as vari�veis x tais que o DateTime de fim da aula dti � igual ao dt da restri��o
	*/
	int numRestr = 0;

	//CentroDados::printTest("criarRestricaoProfHiUB_", "criando rest ub hi");

	for(auto itDateTime = mapDtiVars.begin(); itDateTime != mapDtiVars.end(); ++itDateTime)
	{					
		DateTime dti = itDateTime->first;
		int dtMin = dti.getDateMinutes();

		double bigM = 9999999;

		ConstraintMIPUnico cons;
		cons.reset();
		cons.setType(ConstraintMIPUnico::C_PROF_HOR_INIC_UB);
		cons.setProfessor(prof);
		cons.setDia(dia);
		cons.setFaseDoDia(fase);
		cons.setDateTimeInicial(dti);

		OPT_ROW rowHi (OPT_ROW::ROWSENSE::LESS, 0.0, (char*) cons.toString(etapa).c_str() );

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

int MIPUnico::criarRestricaoProfHiLB_( Professor* const prof, const int dia, const int fase,
	const int colHi, map<DateTime, set< pair<int,double> >> const &mapDtiVars )
{
	/*
		Restri��o que limita inferiormente a vari�vel hip.

		Para todo professor p, todo dia t, todo fase do dia f, todo DateTime dt:

		hip_{p,t,f} >= m(dt) * ( 1 - sum[x t.q. dti<dt] x_{p,t,dti} )

		aonde:
		m(dt) = datetime dt em n�mero de minutos. Por ex: m(7:30) = 7*60 + 30
		x_{p,t,dti}	= vari�vel bin�ria indicando se o prof p d� aula no dia t iniciando no DateTime dti
		hip_{p,t,f}	= vari�vel inteira indicando o primeiro hor�rio (valor em n�mero de minutos) 
					  da fase f do dia t em que prof p d� aula.
		
		"x t.q. dti<dt" significa as vari�veis x tais que o DateTime dti de inicio da aula � menor ao dt da restri��o
	*/
	
	int numRestr = 0;
//	CentroDados::printTest("criarRestProfHiLB_", "criando rest lb hf");

	for(auto itDateTime = mapDtiVars.begin(); itDateTime != mapDtiVars.end(); ++itDateTime)
	{					
		DateTime dti= itDateTime->first;
		int dtMin = dti.getDateMinutes();

		ConstraintMIPUnico cons;
		cons.reset();
		cons.setType(ConstraintMIPUnico::C_PROF_HOR_INIC_LB);
		cons.setProfessor(prof);
		cons.setDia(dia);
		cons.setFaseDoDia(fase);
		cons.setDateTimeInicial(dti);

		int rhs = dtMin;
		OPT_ROW rowHi (OPT_ROW::ROWSENSE::GREATER, rhs, (char*) cons.toString(etapa).c_str());

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

int MIPUnico::criarRestricaoProfHfLB_( Professor* const prof, const int dia, const int fase,
	const int colHf, map<DateTime, set< pair<int,double> >> const &mapDtfVars )
{
	/*
		Para todo professor p, todo dia t, todo fase do dia f, todo DateTime dt:

		hfp_{p,t,f} >= sum[x t.q. dtf==dt] m(dt) * x_{p,t,h}

		aonde:
		m(dt) = datetime dt em n�mero de minutos. Por ex: m(7:30) = 7*60 + 30
		x_{p,t,h}	= vari�vel bin�ria indicando se o prof p d� aula no dia t iniciando no hor�rio h
		hfp_{p,t,f}	= vari�vel inteira indicando o �ltimo hor�rio (valor em n�mero de minutos) 
					  da fase f do dia t em que prof p d� aula.
		
		"x t.q. dtf==dt" significa as vari�veis x tais que o DateTime de fim da aula dtf � igual ao dt da restri��o
	*/
	
	int numRestr = 0;
//	CentroDados::printTest("criarRestProfHfLB_", "criando rest lb hf");

	for(auto itDateTime = mapDtfVars.begin(); itDateTime != mapDtfVars.end(); ++itDateTime)
	{					
		DateTime dtf= itDateTime->first;
		int dtMin = dtf.getDateMinutes();

		ConstraintMIPUnico cons;
		cons.reset();
		cons.setType(ConstraintMIPUnico::C_PROF_HOR_FIN_LB);
		cons.setProfessor(prof);
		cons.setDia(dia);
		cons.setFaseDoDia(fase);
		cons.setDateTimeInicial(dtf);

		int rhs = 0.0;
		OPT_ROW rowHf (OPT_ROW::ROWSENSE::GREATER, rhs, (char*) cons.toString(etapa).c_str());

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

int MIPUnico::criarRestricaoProfHfUB_( Professor* const prof, const int dia, const int fase,
	const int colHf, map<DateTime, set< pair<int,double> >> const &mapDtfVars )
{
	/*
		Para todo professor p, todo dia t, todo fase do dia f, todo DateTime dt:

		hfp_{p,t,f} <= m(dt) + M*( sum[x t.q. dtf>dt] x_{p,t,h} )

		aonde:
		m(dt)		= datetime dt em n�mero de minutos. Por ex: m(7:30) = 7*60 + 30
		x_{p,t,h}	= vari�vel bin�ria indicando se o prof p d� aula no dia t que inicia no horario h
		hfp_{p,t,f}	= vari�vel inteira indicando o �ltimo hor�rio (valor em n�mero de minutos) 
					  da fase f do dia t em que prof p d� aula.
		
		"x t.q. dtf>dt" significa as vari�veis x tais que o DateTime dtf de fim da aula � maior que o dt da restri��o
	*/
	
	int numRestr = 0;

	for(auto itDateTime = mapDtfVars.begin(); itDateTime != mapDtfVars.end(); ++itDateTime)
	{					
		DateTime dtf = itDateTime->first;
		int dtMax = dtf.getDateMinutes();
		double rhs = dtMax;

		ConstraintMIPUnico cons;
		cons.reset();
		cons.setType(ConstraintMIPUnico::C_PROF_HOR_FIN_UB);
		cons.setProfessor(prof);
		cons.setDia(dia);
		cons.setFaseDoDia(fase);
		cons.setDateTimeInicial(dtf);

		OPT_ROW rowHf (OPT_ROW::ROWSENSE::LESS, rhs, (char*) cons.toString(etapa).c_str());

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

int MIPUnico::criarRestricaoProfGapMTN_( Professor* const prof, const int dia, const int fase,
	const int rhs, const int colHi, const int colHf, set< pair<int,double> > const &varsColCoef )
{
	/*
		Para todo professor p, todo dia t, todo fase do dia f:

		sum[x t.q. h e Hf] tempo_{x} * x_{p,t,h} + delta_{f,t} + fpgap_{p,t,f} >= hfp_{p,t,f} - hip_{p,t,f}

		aonde:
		duracao_{x}		= tempo de dura��o (min) da aula representada por x
		delta_{f,t}		= m�ximo de tempo ocioso permitido dentro de uma mesma fase f do dia t,
						entre a primeira e a ultima aula do professor na fase. Conta, basicamente,
						o tempo do intervalo entre aulas.
		x_{p,t,h}		= vari�vel bin�ria indicando se o prof p d� aula no dia t que inicia no hor�rio h
		hfp_{p,t,f}		= vari�vel inteira indicando o �ltimo hor�rio (valor em n�mero de minutos) 
						da fase f do dia t em que prof p d� aula.
		hip_{p,t,f}		= vari�vel inteira indicando o primeiro hor�rio (valor em n�mero de minutos) 
						da fase f do dia t em que prof p d� aula.
		fpgap_{p,t,f}	= vari�vel inteira de folga.

		"x t.q. h e Hf" significa as vari�veis x tais que o hor�rio h de inicio da aula 
			pertence aos hor�rios do turno f
	*/
	
	int numRestr = 0;

	ConstraintMIPUnico cons;
	cons.reset();
	cons.setType(ConstraintMIPUnico::C_PROF_GAP);
	cons.setProfessor(prof);
	cons.setDia(dia);
	cons.setFaseDoDia(fase);

	OPT_ROW row (OPT_ROW::ROWSENSE::GREATER, rhs, (char*) cons.toString(etapa).c_str());

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
	if ( CentroDados::getProblemData()->parametros->proibirProfGapMTN ==
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



// --------------------------------------------------------------

// criar restri��es que impedem gaps nos hor�rios do aluno em um mesmo dia

int MIPUnico::criarRestricaoAlunoHiHf_()
{	
	int numRestr = 0;
	int numRestAnterior = numRestr;

	if ( CentroDados::getProblemData()->parametros->proibirAlunoGap ==
		 ParametrosPlanejamento::ConstraintLevel::Off )
		return numRestr;

	// Par: ( DateTimeInicial -> set<(col,coef)> ), ( DateTimeFinal -> set<(col,coef)> ) 
	typedef pair< map<DateTime, set< pair<int,double> >>, 
				  map<DateTime, set< pair<int,double> >> > ParMapDtiDtf;

	// Aluno -> Dia -> Par-DateTime
	unordered_map<Aluno*, unordered_map<int, ParMapDtiDtf>> mapAlunoDiaHiHf;
	
	// Aluno -> Dia -> <(col,coef)>
	map<Aluno*, map<int, set<pair<int,double>> > > mapAlunoDia;
	
	// -------------------------------------------------------------------------------------------------
	#pragma region Agrupa vari�veis em mapAlunoDiaHiHf e mapAlunoDia
	
	// Percorre variaveis v_{a,i,d,s,t,hi,hf}	
	for(auto itAluno = vars_aluno_aula.begin(); itAluno != vars_aluno_aula.end(); ++itAluno)
	{
		Aluno* const aluno = itAluno->first;
		
		auto *mapAluno1 = &mapAlunoDiaHiHf[aluno];
		auto *mapAluno2 = &mapAlunoDia[aluno];

		for(auto itDia = itAluno->second.begin(); itDia != itAluno->second.end(); ++itDia)
		{
			const int dia = itDia->first;
			ParMapDtiDtf *mapAlunoDia1 = &(*mapAluno1)[dia];
			auto *mapAlunoDia2 = &(*mapAluno2)[dia];

			int colHia = varsAlunoDiaHiHf[aluno][dia].first;

			for(auto itDti = itDia->second.begin(); itDti != itDia->second.end(); ++itDti)
			{		
				const DateTime dti = itDti->first;
				
				for(auto itCp = itDti->second.begin(); itCp != itDti->second.end(); ++itCp)
				{
					for(auto itDisc = itCp->second.begin(); itDisc != itCp->second.end(); ++itDisc)
					{
						for(auto itTurma = itDisc->second.begin(); itTurma != itDisc->second.end(); ++itTurma)
						{
							for(auto itVars = itTurma->second.begin(); itVars != itTurma->second.end(); ++itVars)
							{
								const int colV = itVars->first;
								const VariableMIPUnico var = itVars->second;
							
								HorarioAula *hi = var.getHorarioAulaInicial();
								HorarioAula *hf = var.getHorarioAulaFinal();
								Calendario *calend = hi->getCalendario();

								DateTime dtf = hf->getFinal();			
				
								// ----------- mapAlunoDiaHiHf

								double bigM = lp->getUB( colHia ); // ub de hia!
								double coefXiUB = bigM;
								((*mapAlunoDia1).first)[dti].insert( make_pair(colV,coefXiUB) );

								double coefXfLB = - dtf.getDateMinutes();
								((*mapAlunoDia1).second)[dtf].insert( make_pair(colV,coefXfLB) );

								// ----------- mapAlunoDia
					
								int nrCreds = calend->retornaNroCreditosEntreHorarios(hi,hf);
								double duracaoAula = hi->getTempoAula() * nrCreds;
								(*mapAlunoDia2).insert( make_pair(colV,duracaoAula) );
							}
						}
					}
				}
			}
		}
	}
	#pragma endregion
	
	// -------------------------------------------------------------------------------------------------
	// Aluno -> Dia -> Par-DateTime

	unordered_map<Aluno*, unordered_map<int, ParMapDtiDtf>> mapAlunoDiaHilbHfub;

	// -------------------------------------------------------------------------------------------------
	// - Restri��es para setar ub de hip e lb de hfp de cada dia-faseDoDia do Aluno
	// - Constru��o de mapAlunoDiaHilbHfub
	#pragma region Restri��es para setar ub de hia e lb de hfa de cada dia-faseDoDia do aluno & Constru��o de mapAlunoDiaHilbHfub

	for(auto itAluno = mapAlunoDiaHiHf.begin(); itAluno != mapAlunoDiaHiHf.end(); ++itAluno)
	{
		Aluno *aluno = itAluno->first;

		auto itAlunoHiHf = varsAlunoDiaHiHf.find(aluno);
		if ( itAlunoHiHf == varsAlunoDiaHiHf.end() )
		{
			stringstream ss;
			ss << "var hia-hfa nao encontrada para aluno " << aluno->getAlunoId();
			CentroDados::printError("MIPUnico::criarRestAlunoHiHf_", ss.str());
		}
		
		unordered_map<int, ParMapDtiDtf> *mapAluno3 = &mapAlunoDiaHilbHfub[aluno];

		for(auto itDia = itAluno->second.begin(); itDia != itAluno->second.end(); ++itDia)
		{
			int dia = itDia->first;

			auto itDiaHiHf = itAlunoHiHf->second.find(dia);
			if ( itDiaHiHf == itAlunoHiHf->second.end() )
			{
				stringstream ss;
				ss << "var hia-hfa nao encontrada para dia " << dia;
				CentroDados::printError("MIPUnico::criarRestAlunoHiHf_", ss.str());
			}
			
			ParMapDtiDtf *mapDia3 = &(*mapAluno3)[dia];
							
			// Procura vars hia e hfa
			int colHi = -1, colHf = -1;
			colHi = itDiaHiHf->second.first;
			colHf = itDiaHiHf->second.second;

			if ((colHi < 0) || (colHi >= lp->getNumCols()))
			{
				CentroDados::printError("MIPUnico::criarRestAlunoHiHf_", "index de coluna hia invalido!");
			}
			if ((colHf < 0) || (colHf >= lp->getNumCols()))
			{
				CentroDados::printError("MIPUnico::criarRestAlunoHiHf_", "index de coluna hfa invalido!");
			}
				
			map<DateTime, set< pair<int,double> >> *mapEqualDti = &itDia->second.first;
			map<DateTime, set< pair<int,double> >> *mapEqualDtf = &itDia->second.second;

			numRestr += criarRestricaoAlunoHiUB_( aluno, dia, colHi, *mapEqualDti );
			numRestr += criarRestricaoAlunoHfLB_( aluno, dia, colHf, *mapEqualDtf );

				
			// ----------------------------- preenche mapAluno3 (pointer para parte de mapAlunoDiaHilbHfub)
				
			map<DateTime, set< pair<int,double> >> *mapLessDti3 = &mapDia3->first;
			map<DateTime, set< pair<int,double> >> *mapGreaterDtf3 = &mapDia3->second;

			// preenche map com vars para setar lower bound de hi:  percorre do menor para o maior dt
			for ( auto itDti_2 = mapEqualDti->begin(); itDti_2 != mapEqualDti->end(); itDti_2++ )
			{
				DateTime dti_2 = itDti_2->first;
				double coefXiLB = dti_2.getDateMinutes();

				// Adiciona todas as vars v cujo tempo de inicio � MENOR que dti_2
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
			
			const double bigM = lp->getUB( colHf );

			// preenche map com vars para setar upper bound de hf:  percorre do maior para o menor dt
			for ( auto itDtf_1 = mapEqualDtf->rbegin(); itDtf_1 != mapEqualDtf->rend(); itDtf_1++ )
			{
				DateTime dtf_1 = itDtf_1->first;
				const double coefXfUB = - bigM;

				// Adiciona todas as vars v cujo tempo de fim � MAIOR que dtf_2
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

	#ifdef PRINT_cria_restricoes
		std::cout << std::endl << "numRest criarRestricaoAlunoHiUB_ e criarRestricaoAlunoHfLB_: " 
			<< numRestr-numRestAnterior << std::endl;
		numRestAnterior = numRestr;
	#endif
	#pragma endregion

	// -------------------------------------------------------------------------------------------------
	#pragma region Restri��es para setar lb de hia e ub de hfa de cada dia do aluno
	for(auto itAluno = mapAlunoDiaHilbHfub.begin(); itAluno != mapAlunoDiaHilbHfub.end(); ++itAluno)
	{
		Aluno *aluno = itAluno->first;

		auto itAlunoHiHf = varsAlunoDiaHiHf.find(aluno);
		if ( itAlunoHiHf == varsAlunoDiaHiHf.end() )
		{
			stringstream ss;
			ss << "var hia-hfa nao encontrada para aluno " << aluno->getAlunoId();
			CentroDados::printError("MIPUnico::criarRestAlunoHiHf_", ss.str());
		}

		for(auto itDia = itAluno->second.begin(); itDia != itAluno->second.end(); ++itDia)
		{
			int dia = itDia->first;

			auto itDiaHiHf = itAlunoHiHf->second.find(dia);
			if ( itDiaHiHf == itAlunoHiHf->second.end() )
			{
				stringstream ss;
				ss << "var hia-hfa nao encontrada para dia " << dia;
				CentroDados::printError("MIPUnico::criarRestAlunosHiHf_", ss.str());
			}
											
			// Procura vars hia e hfa
			int colHi = -1, colHf = -1;
			colHi = itDiaHiHf->second.first;
			colHf = itDiaHiHf->second.second;

			if ((colHi < 0) || (colHi >= lp->getNumCols()))
			{
				CentroDados::printError("MIPUnico::criarRestAlunofHiHf_", "index de coluna hia invalido!");
			}
			if ((colHf < 0) || (colHf >= lp->getNumCols()))
			{
				CentroDados::printError("MIPUnico::criarRestAlunoHiHf_", "index de coluna hfa invalido!");
			}

			numRestr += criarRestricaoAlunoHiLB_( aluno, dia, colHi, itDia->second.first );
			numRestr += criarRestricaoAlunoHfUB_( aluno, dia, colHf, itDia->second.second );
		}
	}

	#ifdef PRINT_cria_restricoes
		std::cout << "numRest criarRestricaoAlunoHiLB_ e criarRestricaoAlunoHfUB_: " 
			<< numRestr-numRestAnterior << std::endl;
		numRestAnterior = numRestr;
	#endif
	#pragma endregion

	// -------------------------------------------------------------------------------------------------
	#pragma region Restri��es para impedir gap nos hor�rios do dia do aluno

	for(auto itAluno = mapAlunoDia.begin(); itAluno != mapAlunoDia.end(); ++itAluno)
	{
		Aluno *aluno = itAluno->first;
		auto itAlunoHiHf = varsAlunoDiaHiHf.find(aluno);

		Calendario *calendAluno = aluno->getCalendario();
		if ( calendAluno==nullptr )
		{
			stringstream ss;
			ss << "Calendario do aluno " << aluno->getAlunoId() << " nao encontrado";
			CentroDados::printError("MIPUnico::criarRestAlunoHiHf_", ss.str());
		}
					
		for(auto itDia = itAluno->second.begin(); itDia != itAluno->second.end(); ++itDia)
		{
			int dia = itDia->first;

			auto itDiaHiHf = itAlunoHiHf->second.find(dia);
			if ( itDiaHiHf == itAlunoHiHf->second.end() )
			{
				stringstream ss;
				ss << "var hia-hfa nao encontrada para aluno " << aluno->getAlunoId();
				CentroDados::printError("MIPUnico::criarRestAlunoHiHf_", ss.str());
			}

			// Procura vars hia e hfa
			int colHi = -1, colHf = -1;
			colHi = itDiaHiHf->second.first;
			colHf = itDiaHiHf->second.second;

			// Right hand side � (-) o m�ximo de tempo ocioso permitido dentro do dia t
			int delta = calendAluno->getSomaInterv( dia );
			int rhs = - delta;

			numRestr += criarRestricaoAlunoGap_( aluno, dia, rhs, colHi, colHf, itDia->second );
		}
	}
	#ifdef PRINT_cria_restricoes
		std::cout << "numRest criarRestricaoAlunoGap_: "
			<< numRestr-numRestAnterior << std::endl;
		numRestAnterior = numRestr;
	#endif
	#pragma endregion

	return numRestr;
}

int MIPUnico::criarRestricaoAlunoHiUB_( Aluno* const aluno, const int dia,
	const int colHi, map<DateTime, set< pair<int,double> >> const &mapDtiVars )
{
	/*
		Restri��o que limita superiormente a vari�vel hia.

		Para todo aluno a, todo dia t, todo DateTime dt:

		hia_{a,t} <= m(dt) + M*( 1 - sum[v t.q. dti==dt] v_{a,t,h} )

		aonde:
		m(dt)		= datetime dt em n�mero de minutos. Por ex: m(7:30) = 7*60 + 30
		v_{a,t,h}	= vari�vel bin�ria indicando se o aluno a tem aula no dia t iniciando no hor�rio h
		hia_{a,t}	= vari�vel inteira indicando o primeiro hor�rio (valor em n�mero de minutos) 
					  do dia t em que aluno a tem aula.
		
		"v t.q. dti==dt" significa as vari�veis v tais que o DateTime de fim da aula dti � igual ao dt da restri��o
	*/
	int numRestr = 0;

//	CentroDados::printTest("criarRestAlunoHi_", "criando rest ub hi");

	for(auto itDateTime = mapDtiVars.begin(); itDateTime != mapDtiVars.end(); ++itDateTime)
	{					
		DateTime dti = itDateTime->first;
		int dtMin = dti.getDateMinutes();

		double bigM = 9999999;

		ConstraintMIPUnico cons;
		cons.reset();
		cons.setType(ConstraintMIPUnico::C_ALUNO_HOR_INIC_UB);
		cons.setAluno(aluno);
		cons.setDia(dia);
		cons.setDateTimeInicial(dti);

		OPT_ROW rowHi (OPT_ROW::ROWSENSE::LESS, 0.0, (char*) cons.toString(etapa).c_str() );

		// Insere vars AlunoTurma
		for ( auto itVar = itDateTime->second.begin(); itVar != itDateTime->second.end(); itVar++ )
		{
			rowHi.insert(itVar->first,itVar->second);

			bigM = itVar->second;
		}

		// Corrige rhs
		double rhs = dtMin + bigM;
		rowHi.setRhs(rhs);	

		// Insere var hia
		rowHi.insert(colHi,1.0);
					
		lp->addRow(rowHi);
		numRestr++;
	}

	return numRestr;
}

int MIPUnico::criarRestricaoAlunoHiLB_( Aluno* const aluno, const int dia,
	const int colHi, map<DateTime, set< pair<int,double> >> const &mapDtiVars )
{
	/*
		Restri��o que limita inferiormente a vari�vel hia.

		Para todo aluno a, todo dia t, todo DateTime dt:

		hia_{a,t} >= m(dt) * ( 1 - sum[v t.q. dti<dt] v_{a,t,dti} )

		aonde:
		m(dt)		= datetime dt em n�mero de minutos. Por ex: m(7:30) = 7*60 + 30
		v_{a,t,dti}	= vari�vel bin�ria indicando se o aluno a tem aula no dia t iniciando no DateTime dti
		hia_{a,t}	= vari�vel inteira indicando o primeiro hor�rio (valor em n�mero de minutos) 
					  do dia t em que aluno a tem aula.
		
		"v t.q. dti<dt" significa as vari�veis v tais que o DateTime dti de inicio da aula � menor ao dt da restri��o
	*/
	
	int numRestr = 0;
//	CentroDados::printTest("criarRestAlunoHiLB_", "criando rest lb hf");

	for(auto itDateTime = mapDtiVars.begin(); itDateTime != mapDtiVars.end(); ++itDateTime)
	{					
		DateTime dti= itDateTime->first;
		int dtMin = dti.getDateMinutes();

		ConstraintMIPUnico cons;
		cons.reset();
		cons.setType(ConstraintMIPUnico::C_ALUNO_HOR_INIC_LB);
		cons.setAluno(aluno);
		cons.setDia(dia);
		cons.setDateTimeInicial(dti);

		int rhs = dtMin;
		OPT_ROW rowHi (OPT_ROW::ROWSENSE::GREATER, rhs, (char*) cons.toString(etapa).c_str());

		// Insere vars AlunoTurma
		for ( auto itVar = itDateTime->second.begin(); itVar != itDateTime->second.end(); itVar++ )
		{
			rowHi.insert(itVar->first,itVar->second);

			if ( itVar->second != rhs )
				CentroDados::printError("MIPAlocarAluno::criarRestAlunoHiLB_()", "Coef de v diferente do rhs.");
		}

		// Insere var hia
		rowHi.insert(colHi,1.0);
					
		lp->addRow(rowHi);
		numRestr++;
	}
	return numRestr;
}

int MIPUnico::criarRestricaoAlunoHfLB_( Aluno* const aluno, const int dia,
	const int colHf, map<DateTime, set< pair<int,double> >> const &mapDtfVars )
{
	/*
		Para todo aluno a, todo dia t, todo DateTime dt:

		hfa_{a,t} >= sum[v t.q. dtf==dt] m(dt) * v_{a,t,h}

		aonde:
		m(dt) = datetime dt em n�mero de minutos. Por ex: m(7:30) = 7*60 + 30
		v_{a,t,h}	= vari�vel bin�ria indicando se o aluno a tem aula no dia t iniciando no hor�rio h
		hfa_{a,t}	= vari�vel inteira indicando o �ltimo hor�rio (valor em n�mero de minutos) 
					  do dia t em que aluno a tem aula.
		
		"v t.q. dtf==dt" significa as vari�veis v tais que o DateTime de fim da aula dtf � igual ao dt da restri��o
	*/
	
	int numRestr = 0;
//	CentroDados::printTest("criarRestAlunoHfLB_", "criando rest lb hf");

	for(auto itDateTime = mapDtfVars.begin(); itDateTime != mapDtfVars.end(); ++itDateTime)
	{					
		DateTime dtf= itDateTime->first;
		int dtMin = dtf.getDateMinutes();

		ConstraintMIPUnico cons;
		cons.reset();
		cons.setType(ConstraintMIPUnico::C_ALUNO_HOR_FIN_LB);
		cons.setAluno(aluno);
		cons.setDia(dia);
		cons.setDateTimeInicial(dtf);

		int rhs = 0.0;
		OPT_ROW rowHf (OPT_ROW::ROWSENSE::GREATER, rhs, (char*) cons.toString(etapa).c_str());

		// Insere vars AlunoTurma
		for ( auto itVar = itDateTime->second.begin(); itVar != itDateTime->second.end(); itVar++ )
		{
			rowHf.insert(itVar->first,itVar->second);										
		}
										
		// Insere var hfa
		rowHf.insert(colHf,1.0);
					
		lp->addRow(rowHf);
		numRestr++;
	}
	return numRestr;
}

int MIPUnico::criarRestricaoAlunoHfUB_( Aluno* const aluno, const int dia,
	const int colHf, map<DateTime, set< pair<int,double> >> const &mapDtfVars )
{
	/*
		Para todo aluno a, todo dia t, todo DateTime dt:

		hfa_{a,t} <= m(dt) + M*( sum[v t.q. dtf>dt] v_{a,t,h} )

		aonde:
		m(dt)		= datetime dt em n�mero de minutos. Por ex: m(7:30) = 7*60 + 30
		v_{a,t,h}	= vari�vel bin�ria indicando se o aluno a tem aula no dia t que inicia no horario h
		hfa_{a,t}	= vari�vel inteira indicando o �ltimo hor�rio (valor em n�mero de minutos) 
					  do dia t em que aluno a tem aula.
		
		"v t.q. dtf>dt" significa as vari�veis v tais que o DateTime dtf de fim da aula � maior que o dt da restri��o
	*/
	
	int numRestr = 0;

	for(auto itDateTime = mapDtfVars.begin(); itDateTime != mapDtfVars.end(); ++itDateTime)
	{					
		DateTime dtf = itDateTime->first;
		int dtMax = dtf.getDateMinutes();
		double rhs = dtMax;

		ConstraintMIPUnico cons;
		cons.reset();
		cons.setType(ConstraintMIPUnico::C_ALUNO_HOR_FIN_UB);
		cons.setAluno(aluno);
		cons.setDia(dia);
		cons.setDateTimeInicial(dtf);

		OPT_ROW rowHf (OPT_ROW::ROWSENSE::LESS, rhs, (char*) cons.toString(etapa).c_str());

		// Insere vars AlunoTurma
		for ( auto itVar = itDateTime->second.begin(); itVar != itDateTime->second.end(); itVar++ )
		{
			rowHf.insert(itVar->first,itVar->second);
		}
		
		// Insere var hfa
		rowHf.insert(colHf,1.0);
					
		lp->addRow(rowHf);
		numRestr++;
	}

	return numRestr;
}

int MIPUnico::criarRestricaoAlunoGap_( Aluno* const aluno, const int dia,
	const int rhs, const int colHi, const int colHf, set< pair<int,double> > const &varsColCoef )
{
	/*
		Para todo aluno a, todo dia t:

		sum[v] duracao_{v} * v_{a,i,d,s,t,hi,hf} + delta_{t} + fagap_{a,t} >= hfa_{a,t} - hia_{a,t}

		aonde:
		duracao_{v}	= tempo de dura��o (min) da aula representada por v, SEM contar tempo de intervalos!
		delta_{t} = m�ximo de tempo ocioso permitido dentro do dia t,
					  entre a primeira e a ultima aula do aluno. Conta, basicamente,
					  o tempo do intervalo entre aulas.
		v_{a,i,d,s,t,hi,hf}	= vari�vel bin�ria indicando se o aluno a tem aula no dia t que inicia no hor�rio hi...
		hfa_{a,t}	= vari�vel inteira indicando o �ltimo hor�rio (valor em n�mero de minutos) 
					  do dia t em que aluno a tem aula.
		hia_{a,t}	= vari�vel inteira indicando o primeiro hor�rio (valor em n�mero de minutos) 
					  do dia t em que aluno a tem aula.
		fagap_{a,t}	= vari�vel inteira de folga.
	*/
	
	int numRestr = 0;

	ConstraintMIPUnico cons;
	cons.reset();
	cons.setType(ConstraintMIPUnico::C_ALUNO_GAP);
	cons.setAluno(aluno);
	cons.setDia(dia);

	OPT_ROW row (OPT_ROW::ROWSENSE::GREATER, rhs, (char*) cons.toString(etapa).c_str());

	for(auto itVars = varsColCoef.begin(); itVars != varsColCoef.end(); ++itVars)
	{					
		int col = itVars->first;
		double coef = itVars->second;
		
		// Insere vars v (AlunoTurma)
		row.insert(col,coef);				
	}		

	// Insere var hip
	row.insert(colHi,1.0);
	// Insere var hfp
	row.insert(colHf,-1.0);
	
	// Insere var fagap
	if ( CentroDados::getProblemData()->parametros->proibirAlunoGap ==
		ParametrosPlanejamento::ConstraintLevel::Weak )
	{
		auto itFolgaAluno = varsAlunoFolgaGap.find(aluno);
		if ( itFolgaAluno != varsAlunoFolgaGap.end() )
		{
			auto itFolgaDia = itFolgaAluno->second.find(dia);
			if ( itFolgaDia != itFolgaAluno->second.end() )
			{
				int colFagap = itFolgaDia->second;
				row.insert(colFagap,1.0);
			}
		}
	}

	lp->addRow(row);
	numRestr++;

	return numRestr;
}


int MIPUnico::criarRestricaoMinCredsDiaAluno()
{
	int restricoes = 0;
	
	if ( MIPUnico::considerarMinCredDiaAluno == 0 )
		return restricoes;

	char name[ 1024 ];
	
	for(auto itAluno = vars_aluno_aula.begin(); itAluno != vars_aluno_aula.end(); ++itAluno)
	{
		Aluno* const aluno = itAluno->first;
		
		for(auto itDia = itAluno->second.begin(); itDia != itAluno->second.end(); ++itDia)
		{
			const int dia = itDia->first;
			
			// -------------------------------------
			// Sets the constraint

			ConstraintMIPUnico c;
			c.reset();
			c.setType( ConstraintMIPUnico::C_ALUNO_MIN_CREDS_DIA );
			c.setAluno( aluno );
			c.setDia( dia );
					
			auto cit = cHashTatico.find(c);
			if(cit != cHashTatico.end())
			{
				continue;
			}

			int nnz = 50;

			int maxFolga = aluno->getNrMedioCredsDia() - MIPUnico::desvioMinCredDiaAluno;

			double rhs = max(maxFolga, 0);
			sprintf( name, "%s", c.toString( etapa ).c_str() );
			OPT_ROW row( nnz, OPT_ROW::GREATER, rhs, name );

			// -------------------------------------
			// Insere variaveis v_{a,i,d,s,t,hi,hf}

			for(auto itDti = itDia->second.begin(); itDti != itDia->second.end(); ++itDti)
			{				
				for(auto itCp = itDti->second.begin(); itCp != itDti->second.end(); ++itCp)
				{
					for(auto itDisc = itCp->second.begin(); itDisc != itCp->second.end(); ++itDisc)
					{
						for(auto itTurma = itDisc->second.begin(); itTurma != itDisc->second.end(); ++itTurma)
						{
							for(auto itVars = itTurma->second.begin(); itVars != itTurma->second.end(); ++itVars)
							{
								const int colV = itVars->first;
								const VariableMIPUnico var = itVars->second;			
									
								HorarioAula *hf = var.getHorarioAulaFinal();
								HorarioAula *hi = var.getHorarioAulaInicial();
								Calendario *calendario = hi->getCalendario();

								int nCreds = calendario->retornaNroCreditosEntreHorarios(hi,hf);

								row.insert( colV, nCreds );
							}
						}
					}
				}
			}

			// ------------------------------------
			// Folga

			VariableMIPUnico var;
			var.reset();
			var.setType( VariableMIPUnico::V_FOLGA_MIN_CRED_DIA_ALUNO );	// fcad_{a,t}
			var.setAluno(aluno);
			var.setDia(dia);
			auto vit = vHashTatico.find(var);
			if(vit != vHashTatico.end())
			{
				row.insert( vit->second, 1.0 );
			}

			// -------------------------------------
			// Insere restri��o

			if ( row.getnnz() )
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
	Para todo prof p, unidades u1 e u2, dia t, horarios h1 e h2, 
	tal que o intervalo de tempo entre as aulas h1 e h2 seja menor
	do que o tempo de deslocamento entre as unidades u1 e u2.

	sum[i]sum[d] k_{p,i,d,t,u1,h1} + sum[i]sum[d] k_{p,i,d,t,u2,h2} <= 1

*/
int MIPUnico::criaRestricaoTempoDeslocProfessor()
{
   int restricoes = 0;
   char name[ 200 ];

   if ( problemData->tempo_campi.size() == 0
      && problemData->tempo_unidades.size() == 0 )
   {
      return restricoes;
   }     

   for( auto itProf = vars_prof_aula3.cbegin(); itProf != vars_prof_aula3.cend(); itProf++ )
   {
	   Professor * professor = itProf->first;

	   if (professor->eVirtual()) continue;

	   for( auto itDia = itProf->second.cbegin(); itDia != itProf->second.cend(); itDia++ )
	   {
		    int dia = itDia->first;
		   	   
			for( auto itUnid1 = itDia->second.cbegin(); itUnid1 != itDia->second.cend(); itUnid1++ )
			{
				Unidade *unidade1 = itUnid1->first;

				auto itUnid2 = std::next(itUnid1);
				for( ; itUnid2 != itDia->second.cend(); itUnid2++ )
				{
					Unidade *unidade2 = itUnid2->first;
									
					for( auto itHor1 = itUnid1->second.cbegin(); itHor1 != itUnid1->second.cend(); itHor1++ )
					{			
						HorarioAula* h1 = itHor1->first;

						for( auto itHor2 = itUnid2->second.cbegin(); itHor2 != itUnid2->second.cend(); itHor2++ )
						{
							HorarioAula* h2 = itHor2->first;

							if ( sobrepoem(1, h1, 1, h2) )
								continue;
							
							Unidade* unidadeOrig=nullptr;
							Unidade* unidadeDest=nullptr;
							if ( h1->getInicio() < h2->getInicio() ) // v1 � origem
							{								
								unidadeOrig = unidade1;
								unidadeDest = unidade2;
							}
							else									// v1 � destino
							{								
								unidadeOrig = unidade2;
								unidadeDest = unidade1;
							}
							
							Campus* campusOrig = problemData->refCampus[ unidadeOrig->getIdCampus() ];
							Campus* campusDest = problemData->refCampus[ unidadeDest->getIdCampus() ];

							int tempo_minimo = problemData->calculaTempoEntreCampusUnidades(
								campusDest, campusOrig, unidadeDest, unidadeOrig );

							int nCreds1 = 1;
							int nCreds2 = 1;		
							DateTime fimOrig;
							DateTime inicioDest;					
							getFim1Inicio2(h1,nCreds1,h2,nCreds2,fimOrig,inicioDest);

							int tempo_interv = minutosIntervalo(inicioDest,fimOrig);

							if ( tempo_minimo <= tempo_interv )
								continue;
							
							ConstraintMIPUnico c;
							c.reset();
							c.setType( ConstraintMIPUnico::C_TEMPO_DESLOC_PROF );
							c.setProfessor(professor);
							c.setDia(dia);
							c.setUnidadeOrig(unidadeOrig);
							c.setUnidadeDest(unidadeDest);
							c.setDateTimeFinal(fimOrig);			// fim da aula na origem						
							c.setDateTimeInicial(inicioDest);		// inicio da aula no destino

							auto cit = cHashTatico.find(c);
							if ( cit == cHashTatico.end() )
							{				
								sprintf( name, "%s", c.toString(etapa).c_str() );

								int nnz = itHor1->second.size() + itHor2->second.size();
								OPT_ROW row( nnz, OPT_ROW::LESS, 1.0, name );

								for( auto itVar1 = itHor1->second.cbegin(); itVar1 != itHor1->second.cend(); itVar1++ )
								{
									row.insert( itVar1->first, 1.0 );
								}
								for( auto itVar2 = itHor2->second.cbegin(); itVar2 != itHor2->second.cend(); itVar2++ )
								{	
									row.insert( itVar2->first, 1.0 );
								}
								lp->addRow( row );
				  
								cHashTatico[ c ] = lp->getNumRows();
								restricoes++;								
							}
							else
							{
								for( auto itVar1 = itHor1->second.cbegin(); itVar1 != itHor1->second.cend(); itVar1++ )
								{
									lp->chgCoef( cit->second, itVar1->first, 1.0 );
								}
								for( auto itVar2 = itHor2->second.cbegin(); itVar2 != itHor2->second.cend(); itVar2++ )
								{	
									lp->chgCoef( cit->second, itVar2->first, 1.0 );
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


/*
	Para todo prof p, unidade u2, dia t, horario h2, 
	
	sum[i]sum[d] k_{p,i,d,t,u1,h1} + sum[i]sum[d] k_{p,i,d,t,u2,h2} + sum[i]sum[d] k_{p,i,d,t,u3,h3} <= 2

	tal que u1!=u2 e u3!=u2; h1<h2 e h3>h2.

*/
int MIPUnico::criaRestricaoNrMaxDeslocProfessor()
{
   int restricoes = 0;
   char name[ 200 ];

   if (!problemData->parametros->minimizar_desloc_prof)
	   return restricoes;
   
   for( auto itProf = vars_prof_aula3.cbegin(); itProf != vars_prof_aula3.cend(); itProf++ )
   {
	   Professor * professor = itProf->first;

	   if (professor->eVirtual()) continue;

	   for( auto itDia = itProf->second.cbegin(); itDia != itProf->second.cend(); itDia++ )
	   {
		    int dia = itDia->first;
		   	   
			// para cada unidade central
			for( auto itUnidAtual = itDia->second.cbegin(); itUnidAtual != itDia->second.cend(); itUnidAtual++ )
			{
				Unidade *unidadeAtual = itUnidAtual->first;

				// unidade origem != unidade atual
				auto itUnidOrig = itDia->second.cbegin();
				for( ; itUnidOrig != itDia->second.cend(); itUnidOrig++ )
				{
					if (itUnidOrig==itUnidAtual) continue;
					Unidade *unidadeOrig = itUnidOrig->first;
					Campus* campusOrig = problemData->refCampus[ unidadeOrig->getIdCampus() ];

					// unidade destino != unidade atual
					auto itUnidDest = itDia->second.cbegin();
					for( ; itUnidDest != itDia->second.cend(); itUnidDest++ )
					{
						if (itUnidDest==itUnidAtual) continue;
						Unidade *unidadeDest = itUnidDest->first;
						Campus* campusDest = problemData->refCampus[ unidadeDest->getIdCampus() ];

						// Horario na unidade atual
						for( auto itHorAtual = itUnidAtual->second.cbegin(); itHorAtual != itUnidAtual->second.cend(); itHorAtual++ )
						{			
							HorarioAula* hAtual = itHorAtual->first;

							map< HorarioAula*, set< pair<int /*col*/,VariableMIPUnico> >, LessPtr<HorarioAula> >
								mapHorsOrig;
							map< HorarioAula*, set< pair<int /*col*/,VariableMIPUnico> >, LessPtr<HorarioAula> >
								mapHorsDest;

							for( auto itHorOrig = itUnidOrig->second.cbegin(); itHorOrig != itUnidOrig->second.cend(); itHorOrig++ )
							{			
								HorarioAula* hOrig = itHorOrig->first;

								// hOrig >= hAtual
								if ( hAtual->comparaMenor(*hOrig) || hAtual->inicioFimIguais(*hOrig) )
									continue;

								mapHorsOrig[itHorOrig->first] = itHorOrig->second;
							}

							for( auto itHorDest = itUnidDest->second.cbegin(); itHorDest != itUnidDest->second.cend(); itHorDest++ )
							{
								HorarioAula* hDest = itHorDest->first;
																		
								// hDest <= hAtual
								if ( hDest->comparaMenor(*hAtual) || hDest->inicioFimIguais(*hAtual) )
									continue;

								mapHorsDest[itHorDest->first] = itHorDest->second;
							}
							
							// Combina as duas unidades (origem e destino), que podem ser inclusive iguais,
							// por�m os horarios diferentes. A unidade atual � fixa.
							for( auto itHorOrig = mapHorsOrig.cbegin(); itHorOrig != mapHorsOrig.cend(); itHorOrig++ )
							{
								HorarioAula* hOrig = itHorOrig->first;

								for( auto itHorDest = mapHorsDest.cbegin(); itHorDest != mapHorsDest.cend(); itHorDest++ )
								{							
									HorarioAula* hDest = itHorDest->first;

									ConstraintMIPUnico c;
									c.reset();
									c.setType( ConstraintMIPUnico::C_NR_DESLOC_PROF );
									c.setProfessor(professor);
									c.setDia(dia);
									c.setUnidadeOrig(unidadeOrig);
									c.setUnidadeAtual(unidadeAtual);
									c.setUnidadeDest(unidadeDest);
									c.setHorarioAulaOrig(hOrig);
									c.setHorarioAulaAtual(hAtual);		
									c.setHorarioAulaDest(hDest);

									auto cit = cHashTatico.find(c);
									if ( cit == cHashTatico.end() )
									{				
										sprintf( name, "%s", c.toString(etapa).c_str() );

										int nnz = itHorOrig->second.size() + itHorDest->second.size();
										OPT_ROW row( nnz, OPT_ROW::LESS, 2.0, name );

										for( auto itVarAtual = itHorAtual->second.cbegin(); itVarAtual != itHorAtual->second.cend(); itVarAtual++ )
										{
											row.insert( itVarAtual->first, 1.0 );
										}
										for( auto itVarOrig = itHorOrig->second.cbegin(); itVarOrig != itHorOrig->second.cend(); itVarOrig++ )
										{
											row.insert( itVarOrig->first, 1.0 );
										}
										for( auto itVarDest = itHorDest->second.cbegin(); itVarDest != itHorDest->second.cend(); itVarDest++ )
										{	
											row.insert( itVarDest->first, 1.0 );
										}
										lp->addRow( row );
				  
										cHashTatico[ c ] = lp->getNumRows();
										restricoes++;								
									}
									else
									{
										for( auto itVarAtual = itHorAtual->second.cbegin(); itVarAtual != itHorAtual->second.cend(); itVarAtual++ )
										{
											lp->chgCoef( cit->second, itVarAtual->first, 1.0 );
										}
										for( auto itVarOrig = itHorOrig->second.cbegin(); itVarOrig != itHorOrig->second.cend(); itVarOrig++ )
										{
											lp->chgCoef( cit->second, itVarOrig->first, 1.0 );
										}
										for( auto itVarDest = itHorDest->second.cbegin(); itVarDest != itHorDest->second.cend(); itVarDest++ )
										{	
											lp->chgCoef( cit->second, itVarDest->first, 1.0 );
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


int MIPUnico::criaRestricaoRedCargaHorAnteriorProfessor()
{
   int restricoes = 0;
   char name[ 200 ];

   if ( !problemData->parametros->evitar_reducao_carga_horaria_prof )
      return restricoes;

   double percMaxReducaoCHP = problemData->parametros->perc_max_reducao_CHP;
     
   for (auto vit = vHashTatico.begin(); vit != vHashTatico.end(); vit++ )
   {	   
	   VariableMIPUnico v = vit->first;

	   if (v.getType() != VariableMIPUnico::V_PROF_TURMA &&
		   v.getType() != VariableMIPUnico::V_F_CARGA_HOR_ANT_PROF)
		   continue;
	          
	   Professor * professor = v.getProfessor();
	        
	   if (professor->eVirtual()) continue;

	   double coef = 0;	   
	   if (v.getType() == VariableMIPUnico::V_PROF_TURMA)
		   coef = v.getDisciplina()->getTotalCreditos();
	   if (v.getType() == VariableMIPUnico::V_F_CARGA_HOR_ANT_PROF)
		   coef = 1.0;

	   ConstraintMIPUnico c;
       c.reset();
       c.setType( ConstraintMIPUnico::C_CARGA_HOR_ANTERIOR );
       c.setProfessor( professor );

       auto cit = cHashTatico.find( c );
       if ( cit == cHashTatico.end() )
       {
			sprintf( name, "%s", c.toString(etapa).c_str() );
			double rhs = professor->getChAnterior() * ( 1 - percMaxReducaoCHP/100 );
			
			int nnz = professor->magisterio.size();

			OPT_ROW row( nnz, OPT_ROW::GREATER, rhs, name );

			row.insert( vit->second, coef );
		     
			cHashTatico[ c ] = lp->getNumRows();
			lp->addRow( row );
			restricoes++;
       }
	   else
	   {
		   lp->chgCoef( cit->second, vit->second, coef);
	   }
   }

   return restricoes;
}

