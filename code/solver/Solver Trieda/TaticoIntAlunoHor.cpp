#include "TaticoIntAlunoHor.h"
#include <math.h>


using namespace std;

TaticoIntAlunoHor::TaticoIntAlunoHor( ProblemData * &aProblemData, 
				GGroup< VariableTatico *, LessPtr<VariableTatico> > *aSolVarsTatico, 
				GGroup< VariableTatico *, LessPtr<VariableTatico> > *avars_xh, 
				bool *endCARREGA_SOLUCAO, bool equiv, bool permitirNovasTurmas )
				: Solver( aProblemData )
{
   solVarsTatico = aSolVarsTatico;
   vars_xh = avars_xh;

   CARREGA_SOLUCAO = endCARREGA_SOLUCAO;

   USAR_EQUIVALENCIA = equiv;
   NAO_CRIAR_RESTRICOES_CJT_ANTERIORES = true;
   FIXAR_P1 = true;
   FIXAR_TATICO_P1 = true;
   //PERMITIR_INSERCAO_ALUNODEMANDAP2_EM_TURMAP1 = false;
   PERMITIR_NOVAS_TURMAS = permitirNovasTurmas;

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
   int i;
   if ( lp != NULL )
   {
      delete lp;
   }
   
}

void TaticoIntAlunoHor::getSolution( ProblemSolution * problem_solution ){}

int TaticoIntAlunoHor::solve(){return 1;}

void TaticoIntAlunoHor::solveTaticoIntegrado( int campusId, int prioridade, int r )
{	
	std::cout<<"\nIniciando tatico integrado...\n";
	
	std::cout<<"\nCalculando nro de folgas...\n"; fflush(NULL);

	calculaNroFolgas( prioridade, campusId );
	
	std::cout<<"\nSolving...\n"; fflush(NULL);

	solveTaticoIntAlunoHor( campusId, prioridade, r );

	std::cout<<"\nCarregando solucao tatica integrada...\n"; fflush(NULL);

	carregaVariaveisSolucaoTaticoPorAlunoHor( campusId, prioridade, r );
	
	std::cout<<"\nImprimindo alocacoes...\n"; fflush(NULL);

	if (this->USAR_EQUIVALENCIA)
		problemData->imprimeAlocacaoAlunos( campusId, prioridade, 0, false, r, -3 ); // -3 = rodada de equivalencia
	else if (this->PERMITIR_NOVAS_TURMAS)
		problemData->imprimeAlocacaoAlunos( campusId, prioridade, 0, false, r, -2 ); // -2 = abertura e inserção de alunos
	else
		problemData->imprimeAlocacaoAlunos( campusId, prioridade, 0, false, r, -1 ); // -1 = só inserção de alunos

	if (this->USAR_EQUIVALENCIA)
	{
		std::cout<<"\nAtualizando demandas substitutas por equivalencia...\n"; fflush(NULL);
		atualizarDemandasEquiv( campusId, prioridade );		
	}
	
	std::cout<<"\nInvestigando nao atendimentos...\n";

	investigandoNaoAtendimentos( campusId, prioridade, r );

	std::cout<<"\nSincronizando as solucoes...\n"; fflush(NULL);

	sincronizaSolucao( campusId, prioridade, r );
	
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

void TaticoIntAlunoHor::investigandoNaoAtendimentos( int campusId, int prioridade, int r )
{
	ofstream investigaNaoAtendFile;
	std::string investigaNaoAtendFilename( "investigaNaoAtend_" );
	investigaNaoAtendFilename += problemData->getInputFileName();
	investigaNaoAtendFilename += ".txt";
	investigaNaoAtendFile.open(investigaNaoAtendFilename, ios::app);
	if (!investigaNaoAtendFile)
	{
		cerr << "Error: Can't open output file " << investigaNaoAtendFilename << endl;
		return;
	}

	// Agrupa os não-atendimentos por disciplina
	std::map< Disciplina*, GGroup<Aluno*,LessPtr<Aluno>>, LessPtr<Disciplina> > folgasDeDemandas;
	ITERA_GGROUP_LESSPTR( itVar, solVarsTatInt, VariableTatInt )
	{
		VariableTatInt *v_fd = *itVar;
		if ( v_fd->getType() == VariableTatInt::V_SLACK_DEMANDA_ALUNO )
		{
			if ( v_fd->getAluno()->getCampusId() == campusId )
			{
				folgasDeDemandas[ v_fd->getDisciplina() ].add( v_fd->getAluno() );
			}
		}
	}

	investigaNaoAtendFile << "\n====================================================================";
	if ( this->USAR_EQUIVALENCIA ) investigaNaoAtendFile << "\nEquivalencia";
	if ( this->PERMITIR_NOVAS_TURMAS ) investigaNaoAtendFile << "\nPermite novas turmas";
	else investigaNaoAtendFile << "\nSomente insercao de alunos";
	if ( problemData->parametros->min_alunos_abertura_turmas )
	{
		investigaNaoAtendFile << "\nMinimo de alunos por turma = " << problemData->parametros->min_alunos_abertura_turmas_value;
		if ( problemData->parametros->violar_min_alunos_turmas_formandos && r==2 ) investigaNaoAtendFile << ", exceto turmas com formandos";
	}
	investigaNaoAtendFile << "\nCampus " << campusId << "\tPrioridade " << prioridade << "\tRodada " << r << "\n";


	if ( folgasDeDemandas.size() == 0 ) return;

	// Procura motivos para os não-atendimentos:
	// disponibilidade em sala & disponibilidade do aluno

	investigaNaoAtendFile << "\n\nInvestigando novas turmas...\n";

	std::map< Disciplina*, std::map< Sala*, std::map< Aluno*, std::map<int, GGroup<VariableTatInt>>, LessPtr<Aluno> >, LessPtr<Sala> >, LessPtr<Disciplina> > v_possiveis;

	// Na criação das variaveis já houve filtros de acordo com a divisão de creditos, horários, salas ocupadas, etc.
	// Logo, não é necessário fazer tudo isso novamente, somente filtrar novamente essas variáveis de acordo com
	// horários ocupados para sala/aluno, já que houve alocações depois de suas criações.
	VariableTatIntHash::iterator vit = vHashTatico.begin();
	for ( ; vit != vHashTatico.end(); vit++ )
	{
		VariableTatInt v = vit->first;

		if ( v.getType() == VariableTatInt::V_ALUNO_CREDITOS )
		{		
			Disciplina *disciplina = v.getDisciplina();
			Aluno *aluno = v.getAluno();

			if ( folgasDeDemandas.find( disciplina ) == folgasDeDemandas.end() )
				continue;

			if ( disciplina->getId()==9609 )
				investigaNaoAtendFile << "\nFolga de Disc9609";

			GGroup<Aluno*,LessPtr<Aluno>> alunos = folgasDeDemandas[disciplina];
			if ( alunos.find( aluno ) == alunos.end() )
				continue;

			// Aluno não-alocado na disciplina

			int turma = v.getTurma();
			Sala *sala = v.getSubCjtSala()->salas.begin()->second;				
			HorarioAula *hi = v.getHorarioAulaInicial();
			HorarioAula *hf = v.getHorarioAulaFinal();
			int dia = v.getDia();
			
			if ( disciplina->getId()==9609 )
				investigaNaoAtendFile << "\nFolga de Disc9609 Aluno"<<aluno->getAlunoId() << " turma "<<turma;

			// desse jeito (filtrando as salas ocupadas) estou considerando só as possiveis novas turmas (não-abertas)
			if ( problemData->existeTurmaDiscCampus( turma, disciplina->getId(), campusId ) )
				continue;

			if ( disciplina->getId()==9609 )
				investigaNaoAtendFile << "\n"<<v.toString();

			if ( sala->sobrepoeAulaJaAlocada( hi, hf, dia ) )
				continue;
			
			if ( disciplina->getId()==9609 )
				investigaNaoAtendFile << "\tNao sobrepoe sala!";

			if ( aluno->sobrepoeAulaJaAlocada( hi, hf, dia ) )
				continue;					

			if ( disciplina->getId()==9609 )
				investigaNaoAtendFile << "\tNao sobrepoe aluno!";

			v_possiveis[disciplina][sala][aluno][dia].add( v );
		}
	}


	// TESTE 1-----------------------------------
	Disciplina *discTeste = ( problemData->refDisciplinas.find(9609) != problemData->refDisciplinas.end() ) ? problemData->refDisciplinas[9609] : NULL;
	investigaNaoAtendFile << "\n\nTESTE 1\n";
	std::map< Disciplina*, std::map< Sala*, std::map< Aluno*, std::map<int, GGroup<VariableTatInt>>, LessPtr<Aluno> >, LessPtr<Sala> >, LessPtr<Disciplina> >::iterator
		itMapDisciplina;
	if ( discTeste!=NULL ) itMapDisciplina = v_possiveis.find( discTeste );
	else itMapDisciplina = v_possiveis.end();

	if( itMapDisciplina != v_possiveis.end() )
	{
		std::map< Sala*, std::map< Aluno*, std::map<int, GGroup<VariableTatInt>>, LessPtr<Aluno> >, LessPtr<Sala> >::iterator
			itMapSala = itMapDisciplina->second.begin();	
		for( ; itMapSala != itMapDisciplina->second.end(); itMapSala++ )
		{

			std::map< Aluno*, std::map<int, GGroup<VariableTatInt>>, LessPtr<Aluno> >::iterator itAluno = itMapSala->second.begin();
			for( ; itAluno != itMapSala->second.end(); itAluno++ )
			{
				int somaCreds = 0;
				std::map<int, GGroup<VariableTatInt>>::iterator itDia = itAluno->second.begin();
				for( ; itDia != itAluno->second.end(); itDia++ )
				{
					int dia = itDia->first;
					int nCredsLivresDia=0;
					GGroup<VariableTatInt>::iterator itVarV = itDia->second.begin();
					for( ; itVarV != itDia->second.end(); itVarV++ )
					{
						investigaNaoAtendFile << "\n"<< (*itVarV).toString();
					}
				}
			}
		}
	}
	// TESTE 1-----------------------------------

	

	GGroup<Disciplina*, LessPtr<Disciplina>> removerDisc;

	//std::map< Disciplina*, std::map< Sala*, std::map< Aluno*, std::map<int, GGroup<VariableTatInt>>, LessPtr<Aluno> >, LessPtr<Sala> >, LessPtr<Disciplina> >::iterator
		itMapDisciplina = v_possiveis.begin();
	for( ; itMapDisciplina != v_possiveis.end(); itMapDisciplina++ )
	{
		Disciplina *disciplina = itMapDisciplina->first;
		int nroCredsDisc = disciplina->getTotalCreditos();

		GGroup<Sala*, LessPtr<Sala>> removerSala;

		std::map< Sala*, std::map< Aluno*, std::map<int, GGroup<VariableTatInt>>, LessPtr<Aluno> >, LessPtr<Sala> >::iterator
			itMapSala = itMapDisciplina->second.begin();	
		for( ; itMapSala != itMapDisciplina->second.end(); itMapSala++ )
		{
			Sala *sala = itMapSala->first;
			
			GGroup<Aluno*, LessPtr<Aluno>> removerAluno;

			std::map< Aluno*, std::map<int, GGroup<VariableTatInt>>, LessPtr<Aluno> >::iterator itAluno = itMapSala->second.begin();
			for( ; itAluno != itMapSala->second.end(); itAluno++ )
			{
				int somaCreds = 0;
				std::map<int, GGroup<VariableTatInt>>::iterator itDia = itAluno->second.begin();
				for( ; itDia != itAluno->second.end(); itDia++ )
				{
					int dia = itDia->first;
					int nCredsLivresDia=0;
					GGroup<VariableTatInt>::iterator itVarV = itDia->second.begin();
					for( ; itVarV != itDia->second.end(); itVarV++ )
					{
						HorarioAula *hi = (*itVarV).getHorarioAulaInicial();
						HorarioAula *hf = (*itVarV).getHorarioAulaFinal();
						int ncreds = disciplina->getCalendario()->retornaNroCreditosEntreHorarios( hi, hf );
						if ( nCredsLivresDia < ncreds ) nCredsLivresDia = ncreds;
					}
					somaCreds += nCredsLivresDia;
				}
				if ( somaCreds < nroCredsDisc )
				{					
					if ( disciplina->getId()==9609 )
						investigaNaoAtendFile << "\nRemove Aluno "<< (*itAluno).first->getAlunoId();

					removerAluno.add( (*itAluno).first ); // remover aluno do map
				}
			}

			ITERA_GGROUP_LESSPTR( itAluno, removerAluno, Aluno )
				itMapSala->second.erase( *itAluno );

			if ( itMapSala->second.size() == 0 )
				removerSala.add( sala );
		}

		ITERA_GGROUP_LESSPTR( itSala, removerSala, Sala )
			itMapDisciplina->second.erase( *itSala );
			
		if ( itMapDisciplina->second.size() == 0 )
			removerDisc.add( disciplina );

	}
	ITERA_GGROUP_LESSPTR( itDisc, removerDisc, Disciplina )
		v_possiveis.erase( *itDisc );


	investigaNaoAtendFile << "\nHorarios livres possiveis:\n";

	//std::map< Disciplina*, std::map< Sala*, std::map< Aluno*, GGroup<VariableTatInt>, LessPtr<Aluno> >, LessPtr<Sala> >, LessPtr<Disciplina> >::iterator
		itMapDisciplina = v_possiveis.begin();
	for( ; itMapDisciplina != v_possiveis.end(); itMapDisciplina++ )
	{
		Disciplina *disciplina = itMapDisciplina->first;

		investigaNaoAtendFile << "\nDisciplinaId "<<disciplina->getId() << ", " << disciplina->getTotalCreditos() << " creditos.";

		std::map< Sala*, std::map< Aluno*, std::map<int, GGroup<VariableTatInt>>, LessPtr<Aluno> >, LessPtr<Sala> >::iterator
			itMapSala = itMapDisciplina->second.begin();	
		for( ; itMapSala != itMapDisciplina->second.end(); itMapSala++ )
		{
			Sala *sala = itMapSala->first;
			investigaNaoAtendFile << "\n\tSalaId "<<sala->getId();
						
			std::map< Aluno*, std::map<int, GGroup<VariableTatInt>>, LessPtr<Aluno> >::iterator itAluno = itMapSala->second.begin();
			for( ; itAluno != itMapSala->second.end(); itAluno++ )
			{
				investigaNaoAtendFile << "\n\t\tAlunoId "<<itAluno->first->getAlunoId() << ", Formando="<<itAluno->first->ehFormando();

				std::map<int, GGroup<VariableTatInt>>::iterator itDia = itAluno->second.begin();
				for( ; itDia != itAluno->second.end(); itDia++ )
				{
					investigaNaoAtendFile << "\n\t\t\tDia " << itDia->first;

					GGroup<VariableTatInt>::iterator itVarV = itDia->second.begin();
					for( ; itVarV != itDia->second.end(); itVarV++ )
					{
						investigaNaoAtendFile << "\n\t\t\t\t" << (*itVarV).toString();

						HorarioAula *hi = (*itVarV).getHorarioAulaInicial();
						HorarioAula *hf = (*itVarV).getHorarioAulaFinal();
						investigaNaoAtendFile << " => " << disciplina->getCalendario()->retornaNroCreditosEntreHorarios( hi, hf );
						investigaNaoAtendFile << " creditos";
					}
				}
			}
		}
	}

	investigaNaoAtendFile << "\n\n\nInvestigando insercao de alunos...\n";

	// Turmas existentes (possível inserção do aluno na turma)
	std::map< Disciplina*, GGroup<Aluno*,LessPtr<Aluno>>, LessPtr<Disciplina> >::iterator
		itMap = folgasDeDemandas.begin();

	for( ; itMap != folgasDeDemandas.end(); itMap++ )
	{
		Disciplina *disciplina = itMap->first;

		if ( disciplina->getId() < 0 )
			continue;

		bool haPratica=false;
		if ( problemData->refDisciplinas.find( - disciplina->getId() ) !=
			 problemData->refDisciplinas.end() )
			 haPratica = true;

		// AULAS PRATICAS
		Disciplina *dp=NULL;
		std::map< int, GGroup< VariableTatInt *, LessPtr<VariableTatInt> > > aulasXPraticas;			
		if ( haPratica )
		{
			dp = problemData->refDisciplinas[ - disciplina->getId() ];
			for ( int turma=0; turma<dp->getNumTurmas(); turma++ )
			{
				GGroup< VariableTatInt *, LessPtr<VariableTatInt> > 
					aulas = retornaAulasEmVarXFinal( turma, dp, campusId );
			
				if ( aulas.size() != 0 ) 
					aulasXPraticas[turma] = aulas;
			}
		}

		// AULAS TEORICAS
		std::map< int, GGroup< VariableTatInt *, LessPtr<VariableTatInt> > > aulasXTeoricas;
		for ( int turma=0; turma<disciplina->getNumTurmas(); turma++ )
		{		
			GGroup< VariableTatInt *, LessPtr<VariableTatInt> > 
					aulas = retornaAulasEmVarXFinal( turma, disciplina, campusId );
			
			if ( aulas.size() != 0 )
				aulasXTeoricas[turma] = aulas;
		}

		// TENTATIVAS DE INSERÇÕES NAS PRATICAS E TEORICAS, OU SÓ TEORICAS CASO NAO EXISTA PRATICA
		ITERA_GGROUP_LESSPTR( itAluno, itMap->second, Aluno )
		{
			Aluno *aluno = (*itAluno);
				
			// TENTATIVAS DE INSERÇÕES NAS TURMAS TEORICAS
			bool violaTodasAsTeoricas=true;
			bool *violaTeorica = new bool[disciplina->getNumTurmas()];
			for (int i=0; i<disciplina->getNumTurmas(); i++) violaTeorica[i]=true;
			
			std::map< int, GGroup< VariableTatInt *, LessPtr<VariableTatInt> > >::iterator
				itTurmasTeoricas = aulasXTeoricas.begin();
			for( ; itTurmasTeoricas != aulasXTeoricas.end(); itTurmasTeoricas++ )
			{
				int turma = itTurmasTeoricas->first;
				if ( violaTeorica[turma] ) violaTeorica[turma] = violaInsercao( aluno, itTurmasTeoricas->second );
				violaTodasAsTeoricas = violaTodasAsTeoricas && violaTeorica[turma];
			}
			if (violaTodasAsTeoricas) continue; // Se viola todas as turmas teoricas, nem tenta as praticas.
						
			// TENTATIVAS DE INSERÇÕES NAS TURMAS PRATICAS
			bool *violaPratica;
			if ( haPratica ) 
			{
				// Verifica a inserção em turmas da disciplina pratica
				bool violaTodasAsPraticas=true;
				violaPratica = new bool[dp->getNumTurmas()];
				for (int i=0; i<dp->getNumTurmas(); i++) violaPratica[i]=true;

				int i=problemData->retornaTurmaDiscAluno( aluno, dp );
				if ( i != -1 ){
					violaPratica[ i ]=false;
					std::cout<<"\nAtencao: Aluno com folga na teorica, mas ja alocado na pratica! Estranho!\n";
				}

				std::map< int, GGroup< VariableTatInt *, LessPtr<VariableTatInt> > >::iterator
				itTurmasPraticas = aulasXPraticas.begin();
				for( ; itTurmasPraticas != aulasXPraticas.end(); itTurmasPraticas++ )
				{
					int turma = itTurmasPraticas->first;
					if ( violaPratica[turma] ) violaPratica[turma] = violaInsercao( aluno, itTurmasPraticas->second );
					violaTodasAsPraticas = violaTodasAsPraticas && violaPratica[turma];
				}

				if (violaTodasAsPraticas) continue;
			}
			
			// ACHOU POSSIBILIDADE DE INSERÇÃO EM TEORICA E PRATICA (CASO HAJA PRATICA).

			// TURMAS TEORICAS
			for (int i=0; i<disciplina->getNumTurmas(); i++) 
			{
				if ( !violaTeorica[i] )
				{					
					investigaNaoAtendFile << "\n\nPoderia ter inserido o aluno!";
					investigaNaoAtendFile<<"\nAlunoId "<<aluno->getAlunoId()<<", turma "<<i<<", disciplinaId "<<disciplina->getId();

					GGroup< VariableTatInt *, LessPtr<VariableTatInt> > aulasV = retornaAulasEmVarV( aluno, aulasXTeoricas[i] );

					if ( aulasXTeoricas[i].size() != aulasV.size() )
					{
						investigaNaoAtendFile<<"\nErro: "<<aulasXTeoricas[i].size()<<" variaveis x e "<<aulasV.size()<<" variaveis v.\n";
					}
					//else // comentado pq nao esta havendo necessidade em usar v_possiveis nessa parte
					//{
					//	// insere as variaveis v correspondentes
					//	ITERA_GGROUP_LESSPTR( itAulaV, aulasV, VariableTatInt )
					//		v_possiveis[disciplina][sala][aluno][itAulaV->getDia()].add( **itAulaV );
					//}

					investigaNaoAtendFile<<"\nVariaveis v: ";
					ITERA_GGROUP_LESSPTR( itAulaV, aulasV, VariableTatInt )
						investigaNaoAtendFile<<" "<< (*itAulaV)->toString();
					investigaNaoAtendFile<<"\nVariaveis x: ";
					ITERA_GGROUP_LESSPTR( itAulaX, aulasXTeoricas[i], VariableTatInt )
						investigaNaoAtendFile<<" "<< (*itAulaX)->toString();
					investigaNaoAtendFile<<"\n";
				}
			}

			// TURMAS PRATICAS
			if ( haPratica ) 
			{	
				for (int i=0; i<dp->getNumTurmas(); i++) 
				{
					if ( !violaPratica[i] )
					{
						investigaNaoAtendFile << "\n\nPoderia ter inserido o aluno!";
						investigaNaoAtendFile<<"\nAlunoId "<<aluno->getAlunoId()<<", turma "<<i<<", disciplinaId "<<disciplina->getId();

						GGroup< VariableTatInt *, LessPtr<VariableTatInt> > aulasV = retornaAulasEmVarV( aluno, aulasXPraticas[i] );

						if ( aulasXTeoricas[i].size() != aulasV.size() )
						{
							investigaNaoAtendFile<<"\nErro: "<<aulasXPraticas[i].size()<<" variaveis x e "<<aulasV.size()<<" variaveis v.\n";
						}
						//else // comentado pq nao esta havendo necessidade em usar v_possiveis nessa parte
						//{
						//	// insere as variaveis v correspondentes
						//	ITERA_GGROUP_LESSPTR( itAulaV, aulasV, VariableTatInt )
						//		v_possiveis[disciplina][sala][aluno][itAulaV->getDia()].add( **itAulaV );
						//}

						investigaNaoAtendFile<<"\nVariaveis v: ";
						ITERA_GGROUP_LESSPTR( itAulaV, aulasV, VariableTatInt )
							investigaNaoAtendFile<<" "<< (*itAulaV)->toString();
						investigaNaoAtendFile<<"\nVariaveis x: ";
						ITERA_GGROUP_LESSPTR( itAulaX, aulasXPraticas[i], VariableTatInt )
							investigaNaoAtendFile<<" "<< (*itAulaX)->toString();
						investigaNaoAtendFile<<"\n";
					}
				}					
			}				
		}
	}
	
	investigaNaoAtendFile.close();

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

void TaticoIntAlunoHor::calculaNroFolgas( int prioridade, int campusId )
{	
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
	
	if ( problemData->parametros->considerar_equivalencia_por_aluno && this->USAR_EQUIVALENCIA )
	{
		int nroFolgasDeEquiv = 0;
		ITERA_GGROUP_LESSPTR( itDiscEquiv, disciplina->discEquivalentes, Disciplina ) // possiveis substituidas!
			nroFolgasDeEquiv += this->haFolgaDeAtendimento( *itDiscEquiv );
		nroFolgas += nroFolgasDeEquiv;
	}
	
	int minAlunos;
	int nroMaxNovasTurmas;

	if ( problemData->parametros->min_alunos_abertura_turmas )
	{
		minAlunos = problemData->parametros->min_alunos_abertura_turmas_value;
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
		minAlunos = 5; // todo
		nroMaxNovasTurmas = (int) nroFolgas/minAlunos;
		if ( nroFolgas>0 && nroMaxNovasTurmas==0 ) nroMaxNovasTurmas=1;
	}	
	
	int counter = 0;
	for ( int i = 0; disciplina->getNumTurmas(); i++ )
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
	VariableTatico vSol;
	int nroNovasTurmas=0;

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
					nroNovasTurmas++;
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
	cout<<"\n\n"<<nroNovasTurmas<<" novas turmas.\n\n";
}

std::string TaticoIntAlunoHor::getTaticoLpFileName( int campusId, int prioridade, int r )
{
   std::string solName( "SolverTriedaTatInt" );
   
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

   if ( USAR_EQUIVALENCIA )
   {
		solName += "_Equiv"; 
   } 

   return solName;
}

std::string TaticoIntAlunoHor::getSolBinFileName( int campusId, int prioridade, int r)
{
   std::string solName( "solTatIntBin" );
    
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
   if ( USAR_EQUIVALENCIA )
   {
		solName += "_Equiv"; 
   } 

   solName += ".bin";
      
   return solName;
}

std::string TaticoIntAlunoHor::getSolucaoTaticoFileName( int campusId, int prioridade, int r )
{
   std::string solName( "solucaoTaticoInt" );
   
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
   if ( USAR_EQUIVALENCIA )
   {
		solName += "_Equiv"; 
   } 

   solName += ".txt";
      
   return solName;
}

std::string TaticoIntAlunoHor::getEquivFileName( int campusId, int prioridade )
{
   std::string solName( "equivalencias" );
   
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

void TaticoIntAlunoHor::carregaVariaveisSolucaoTaticoPorAlunoHor( int campusAtualId, int prioridade, int r )
{
   double * xSol = NULL;
   
   lp->updateLP();

   int nroColsLP = lp->getNumCols();
   xSol = new double[ nroColsLP ];
	
	#pragma region Carrega solucao
   if ( (*this->CARREGA_SOLUCAO) )
   {
	   char solName[1024];

	   strcpy( solName, getSolBinFileName( campusAtualId, prioridade, r ).c_str() );
	   cout<<"====================> carregando " <<solName <<endl;
	   FILE* fin = fopen( solName,"rb");
      
	   if ( fin == NULL )
	   {
		   std::cout << "\nErro em carregaVariaveisSolucaoTaticoPorAlunoHor(int campusAtualId, int prioridade, int r): arquivo "
					 << solName << " nao encontrado.\n";
		   exit(0);
	   }

	   int nCols = 0;

	   fread(&nCols,sizeof(int),1,fin);

	   if ( nCols == nroColsLP )
	   {
		  for (int i =0; i < nCols; i++)
		  {
			 double auxDbl;
			 fread(&auxDbl,sizeof(double),1,fin);
			 xSol[i] = auxDbl;
		  }
	   }
	   else
	   {
		   std::cout << "\nErro em carregaVariaveisSolucaoTaticoPorAlunoHor(int campusAtualId, int prioridade, int r): "
					 << " \nNumero diferente de variaveis: " << nCols << " != " << nroColsLP;
	   }

	   fclose(fin);
   }
   else
   {
	   lp->getX( xSol ); fflush( NULL );
   }
	#pragma endregion

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

   char solFilename[1024];
   strcpy( solFilename, getSolucaoTaticoFileName( campusAtualId, prioridade, r ).c_str() );

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
         char auxName[100];
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
		 int vDia;
		 int vTurma;
		 int vCampusId;
		 int nCreds;
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
					alunoDem = problemData->procuraAlunoDemandaEquiv( vDisc, vAluno, prioridade );
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
				 nCreds = vDisc->getCalendario()->retornaNroCreditosEntreHorarios( vhi, vhf );				 
				 for ( int i = 1; i <= nCreds; i++ )
				 {
					HorarioDia *hd = problemData->getHorarioDiaCorrespondente( ha, vDia );
					horariosDias.add( hd );
					ha = vDisc->getCalendario()->getProximoHorario( ha );			
				 }
				 				 
				 if ( this->USAR_EQUIVALENCIA )
					alunoDem = problemData->procuraAlunoDemandaEquiv( vDisc, vAluno, prioridade );
				 else
					 alunoDem = vAluno->getAlunoDemanda( vDisc->getId() );

				 problemData->insereAlunoEmTurma( alunoDem, trio, horariosDias );

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
				 std::cout << problemData->mapCampusTurmaDisc_AlunosDemanda[trio].size() << " vagas para a aula de "
						  << problemData->retornaNroCreditos( vhi, vhf, vSala, vDisc, vDia )
						  << " creditos da disciplina " << vDisc->getCodigo() << " id" <<vDisc->getId()
						  << " para a turma " << vTurma
						  << " no dia " << vDia
						  << " para a sala " << vSala->getId()
						  << std::endl << std::endl;
				 break;
			 case VariableTatInt::V_SLACK_DEMANDA_ALUNO:
				 nroNaoAtendimentoAlunoDemanda++;
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
   
    return;
}

int TaticoIntAlunoHor::solveTaticoIntAlunoHor( int campusId, int prioridade, int r )
{	
	if ( USAR_EQUIVALENCIA )
	{
		std::cout<<"\n------------------------ Equiv --------------------------\n";
    } 
	if ( PERMITIR_NOVAS_TURMAS )
	{
		std::cout<<"\n------------------------ Permite Novas Turmas --------------------------\n";
    } 
	std::cout<<"\n---------------------Rodada "<< r <<" -----------------------\n";
	std::cout<<"\n------- Campus "<< campusId << ", Prior " << prioridade << "----------\n";
	std::cout<<"\n------------------------------Tatico Integrado -----------------------------\n"; fflush(NULL);
		
	int status = 0;
		
   if ( (*this->CARREGA_SOLUCAO) )
   {
	   char solName[1024];
	   strcpy( solName, getSolBinFileName( campusId, prioridade, r ).c_str() );
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

		   lp->writeProbLP( lpName );
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
	   /* 

	   std::set< int > vHashLivresOriginais;

		int *idxs = new int[lp->getNumCols()*2];
		double *vals = new double[lp->getNumCols()*2];
		BOUNDTYPE *bds = new BOUNDTYPE[lp->getNumCols()*2];
		int nBds = 0;

		std::cout << "\n=========================================";
	    std::cout << "\nGarantindo solucao...\n"; fflush(NULL);

		VariableTatIntHash::iterator vit = vHashTatico.begin();
		for ( ; vit != vHashTatico.end(); vit++ )
		{
			VariableTatInt v = vit->first;
	   
			if ( v.getType() == VariableTatInt::V_OFERECIMENTO )
			{
				int lb = (int)(lp->getLB(vit->second) + 0.5);
				int ub = (int)(lp->getUB(vit->second) + 0.5);

				if ( lb != ub ) // se for variavel livre
				{
					vHashLivresOriginais.insert( vit->second );

				   idxs[nBds] = vit->second;
				   vals[nBds] = 0.0;
				   bds[nBds] = BOUNDTYPE::BOUND_UPPER;
				   nBds++;
				}
			}
		}
		lp->chgBds(nBds,idxs,bds,vals);
	    
		lp->updateLP();
		lp->setTimeLimit( 1e10 );
		lp->setPreSolve(OPT_TRUE);
		lp->setHeurFrequency(1.0);
		lp->setMIPScreenLog( 4 );
		lp->setMIPEmphasis(0);
		lp->setSymetry(0);
		lp->setCuts(3);
		lp->setNumIntSols(1);	
		lp->updateLP();
		lp->writeProbLP( string("1"+ string(lpName) ).c_str() );

		if( ! (*this->CARREGA_SOLUCAO) )	
		{ 
			status = lp->optimize( METHOD_MIP );	
		}

#pragma region Imprime Gap
	 	  // Imprime Gap
		ofstream outGaps;
		outGaps.open("gaps.txt", ofstream::app);
		if ( !outGaps )
		{
			std::cerr<<"\nAbertura do arquivo gaps.txt falhou em TaticoIntAlunoHor::solveTaticoIntAlunoHor().\n";
		}
		else
		{
			outGaps << "Tatico (Garante solucao) - campus "<< campusId <<", prioridade " << prioridade;
			outGaps << "\nGap = " << lp->getMIPGap() * 100 << "%";
			outGaps << "\n\n";
			outGaps.close();
		} 
#pragma endregion

		fflush(NULL);
		
	
		// -------------------------------------------------------------------
		// Volta as variaveis z_{i,d,cp} que estavam livres
         
		nBds = 0;

		for ( std::set< int >::iterator it = vHashLivresOriginais.begin();
			  it != vHashLivresOriginais.end(); it++)
		{
			idxs[nBds] = *it;
			vals[nBds] = 1.0;
			bds[nBds] = BOUNDTYPE::BOUND_UPPER;
			nBds++;
		}

		lp->chgBds(nBds,idxs,bds,vals);
		lp->updateLP();

		
#ifdef SOLVER_CPLEX
		lp->setNumIntSols(0);
		lp->setTimeLimit( 7200 );
		lp->setPreSolve(OPT_TRUE);
		lp->setHeurFrequency(1.0);
		lp->setMIPScreenLog( 4 );
		lp->setMIPEmphasis(4);
	    lp->setPolishAfterNode(1);
		lp->setSymetry(0);
		lp->setCuts(2);
		lp->updateLP();
#endif
#ifdef SOLVER_GUROBI
		lp->setNumIntSols(0);
		lp->setTimeLimit( 10800 );
		lp->setPreSolve(OPT_TRUE);
		lp->setHeurFrequency(1.0);
		lp->setMIPScreenLog( 4 );
		lp->setMIPEmphasis(0);
		lp->setSymetry(0);
		lp->setCuts(-1);
		lp->updateLP();
#endif

		lp->updateLP();
		
   
	    // -------------------------------------------------------------------
	    
		std::cout << "\n=========================================";
	    std::cout << "\nGarantindo maximo atendimento...\n"; fflush(NULL);

		double *objN = new double[lp->getNumCols()];
		lp->getObj(0,lp->getNumCols()-1,objN);

        nBds = 0;

		vit = vHashTatico.begin();
		for ( ; vit != vHashTatico.end(); vit++ )
		{
			VariableTatInt v = vit->first;

			if ( v.getType() == VariableTatInt::V_SLACK_DEMANDA_ALUNO )
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
		lp->writeProbLP( string( "2"+ string(lpName) ).c_str() );

		int *idxN = new int[lp->getNumCols()];

		for ( int i = 0; i < lp->getNumCols(); i++ )
		{
			idxN[i] = i;
		}
		

		double *xS = new double[lp->getNumCols()];

	 	status = lp->optimize( METHOD_MIP );	
		lp->getX(xS); 
	  		 
#pragma region Imprime Gap
	 	  // Imprime Gap
		outGaps.open("gaps.txt", ofstream::app);
		if ( !outGaps )
		{
			std::cerr<<"\nAbertura do arquivo gaps.txt falhou em TaticoIntAlunoHor::solveTaticoBasicoCjtAlunos().\n";
		}
		else
		{
			outGaps << "Tatico (Max Atend) - campus "<< campusId << ", prioridade " << prioridade;
			outGaps << "\nGap = " << lp->getMIPGap() * 100 << "%";
			outGaps << "\n\n";
			outGaps.close();
		} 
#pragma endregion

	    fflush(NULL);
		std::cout << "\n=========================================";
	    std::cout << "\nGarantindo o resto dos parametros...\n"; fflush(NULL);

        nBds = 0;

		vit = vHashTatico.begin();
		for ( ; vit != vHashTatico.end(); vit++ )
		{
			VariableTatInt v = vit->first;

			if (  v.getType() == VariableTatInt::V_SLACK_DEMANDA_ALUNO && xS[vit->second] < 0.1 ) // atendido
			{
				int discId = v.getDisciplina()->getId();
				
				idxs[nBds] = vit->second;
				vals[nBds] = 0.0;
				bds[nBds] = BOUNDTYPE::BOUND_UPPER;
				nBds++;
			}
		}


        lp->chgBds(nBds,idxs,bds,vals);
		lp->chgObj(lp->getNumCols(),idxN,objN);

		lp->updateLP();
		lp->writeProbLP( lpName );

		*/

#ifdef SOLVER_CPLEX
		lp->setNumIntSols(0);
		lp->setTimeLimit(7200 + 7200);
		//lp->setMIPRelTol( 0.01 );
		lp->setPreSolve(OPT_TRUE);
		lp->setHeurFrequency(1.0);
		lp->setMIPScreenLog( 4 );
		lp->setPolishAfterTime(7200);
		lp->setPolishAfterIntSol(1);
		lp->setMIPEmphasis(0);
		lp->setPolishAfterNode(1);
		lp->setSymetry(0);
		lp->setCuts(1);
	//	lp->setScreenLog(OPT_TRUE); // deu msg de erro com esse parametro
		lp->updateLP();
#endif
#ifdef SOLVER_GUROBI
		lp->setNumIntSols(0);
		lp->setTimeLimit(7200);
		lp->setPreSolve(OPT_TRUE);
		lp->setHeurFrequency(1.0);
		lp->setMIPScreenLog( 4 );
		lp->setPolishAfterTime(100000000);
		lp->setPolishAfterIntSol(100000000);
		lp->setMIPEmphasis(0);
		lp->setSymetry(0);
		lp->setCuts(1);
		lp->updateLP();
#endif

		// GENERATES SOLUTION
        status = lp->optimize(METHOD_MIP);
		double * xSol = NULL;
		xSol = new double[ lp->getNumCols() ];
        lp->getX(xSol);
		
	    // Imprime Gap
		ofstream outGaps;
		std::string gapFilename( "gap_input" );
		gapFilename += problemData->getInputFileName();
		gapFilename += ".txt";
		outGaps.open(gapFilename, ofstream::app);
		if ( !outGaps )
		{
			std::cerr<<"\nAbertura do arquivo gaps.txt falhou em TaticoIntAlunoHor::solveTaticoIntAlunoHor().\n"; fflush(NULL);
		}
		else
		{
			outGaps << "Tatico Integrado - campus "<< campusId << ", Prioridade " << prioridade << ", R "<< r;
			if ( USAR_EQUIVALENCIA ) outGaps << ", Com equivalencias";			
			outGaps << "\nGap = " << lp->getMIPGap() * 100 << "%";
			outGaps << "\n\n\n";
			outGaps.close();
		}
			
		lp->updateLP();
		
		// WRITES SOLUTION
		char solName[1024];
		strcpy( solName, getSolBinFileName( campusId, prioridade, r ).c_str() );
		FILE * fout = fopen( solName, "wb" );
		if ( fout == NULL )
		{
			std::cout << "\nErro em TaticoIntAlunoHor::solveTaticoIntAlunoHor( int campusId, int prioridade, int cjtAlunosId ):"
					<< "\nArquivo " << solName << " nao pode ser aberto.\n"; fflush(NULL);
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

		delete [] xSol;
   }

	std::cout<<"\n------------------------------Fim do Tatico Integrado -----------------------------\n"; fflush(NULL);

   return status;
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
						 itX->getHorarioAulaInicial() == v->getHorarioAulaInicial() &&
						 itX->getHorarioAulaFinal() == v->getHorarioAulaFinal() )
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

bool TaticoIntAlunoHor::criaVariavelTatico( VariableTatInt *v, bool &fixar, int prioridade )
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
 			 int turmaDoAluno = problemData->retornaTurmaDiscAluno( v->getAluno(), v->getDisciplina() );
			 if ( turmaDoAluno != v->getTurma() && turmaDoAluno != -1 )
			 {
				 // aluno alocado, mas em outra turma: NÃO CRIA A VARIAVEL
				 return false;
			 }
			 			 
			 // aluno alocado na turma corrente
			 if ( turmaDoAluno == v->getTurma() )
			 {
				 // aluno alocado nesta turma: FIXA A VARIAVEL, caso ela exista. Se não, nem cria.

				 vSol.reset();
				 vSol.setType( VariableTatico::V_CREDITOS ); // x_{i,d,u,s,hi,hf,t}
				 vSol.setTurma( v->getTurma() );
				 vSol.setDisciplina( v->getDisciplina() );
				 vSol.setUnidade( v->getUnidade() );
				 vSol.setSubCjtSala( v->getSubCjtSala() );
				 vSol.setDia( v->getDia() );								 
				 vSol.setHorarioAulaInicial( v->getHorarioAulaInicial() );	 
				 vSol.setHorarioAulaFinal( v->getHorarioAulaFinal() );

				 if ( SolVarsFound(vSol) )
				 {		
					fixar=true;
					return true;
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

				 // Verifica os horários já alocados do aluno, independente da turma já existir ou não.
				 // Se houver sobreposição com os horários da variável v, não permite a criação da mesma.
				 int dia = v->getDia();
				 HorarioAula *hi = v->getHorarioAulaInicial();
				 HorarioAula *hf = v->getHorarioAulaFinal();
				 if ( aluno->sobrepoeAulaJaAlocada( hi, hf, dia ) )
					 return false;

				 // aluno não alocado: cria a variavel livre se 'x' e 'z' existirem;
				 // aluno não alocado: não cria a variavel se 'x' não existir e 'z' existir;				 
				 // aluno não alocado: cria a variavel livre se nem 'x' nem 'z' existirem;
				 
				 vSol.reset();
				 vSol.setType( VariableTatico::V_CREDITOS ); // x_{i,d,u,s,hi,hf,t}
				 vSol.setTurma( turma );
				 vSol.setDisciplina( disciplina );
				 vSol.setUnidade( v->getUnidade() );
				 vSol.setSubCjtSala( v->getSubCjtSala() );
				 vSol.setDia( dia );								 
				 vSol.setHorarioAulaInicial( v->getHorarioAulaInicial() );	 
				 vSol.setHorarioAulaFinal( v->getHorarioAulaFinal() );
				 				 
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
							if ( sala->sobrepoeAulaJaAlocada( hi, hf, dia ) )
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

			 if ( SolVarsFound(vSol) )
			 {
				fixar=true;
				return true;
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
						if ( sala->sobrepoeAulaJaAlocada( hi, hf, dia ) )
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
			 vSol.reset();
			 vSol.setType( VariableTatico::V_CREDITOS ); // x_{i,d,u,s,hi,hf,t}
			 vSol.setTurma( v->getTurma() );
			 vSol.setDisciplina( v->getDisciplina() );
			 vSol.setUnidade( v->getUnidade() );
			 vSol.setSubCjtSala( v->getSubCjtSala() );

			 ConjuntoSala* cjtSala = this->retornaSalaDeAtendimento( v->getTurma(), v->getDisciplina(), 
										problemData->refCampus[v->getUnidade()->getIdCampus()] );

			 if ( cjtSala == v->getSubCjtSala() ) // se achou: cria fixo
			 {
				 fixar=true;
				 return true;
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
			 if ( turma != v->getTurma() && turma != -1 )
			 {
				 // aluno alocado, mas em outra turma: NÃO CRIA A VARIAVEL
				 return false;
			 }
			 			 
			 // aluno alocado na turma corrente
			 if ( turma == v->getTurma() )
			 {
				// aluno alocado nesta turma: cria fixada
				fixar=true;
				return true;				
			 }
			 else if ( turma == -1 ) // aluno não alocado: cria livre
			 {
				if ( permitirAbertura( v->getTurma(), v->getDisciplina(), v->getAluno()->getOferta()->getCampusId() ) )
				{
					// aluno não alocado: cria a variavel LIVRE se nem 'x' nem 'z' existirem;
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
				return true;
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
				return true;
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
				return true;
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
				return true;
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
				return true;
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
				return true;
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
				return true;
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
			vSol.reset();
			vSol.setType( VariableTatico::V_ABERTURA ); // z_{i,d,cp}
			vSol.setTurma( v->getTurma() );
			vSol.setDisciplina( v->getDisciplina() );
			vSol.setCampus( v->getCampus() );

			if ( SolVarsFound(vSol) ) // existe a turma (variavel z)
			{
				fixar = true;
				return true;
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

			if ( v->getCampus()->getId() != campusId )
				continue;
			
			// Continue, caso seja alocação de prioridade passada
			if ( prioridade==2 && problemData->procuraAlunoDemanda(disciplina->getId(), aluno->getAlunoId(), 1) != NULL )
				continue;
			
			AlunoDemanda *alDemOrig = problemData->procuraAlunoDemanda( disciplina->getId(), aluno->getAlunoId(), prioridade );
			
			if ( alDemOrig==NULL ) // disciplina substituta
			{
				bool achou=false;
				
				ITERA_GGROUP_LESSPTR ( itAlDem, aluno->demandas, AlunoDemanda ) // procura AlunoDemanda original
				{
					AlunoDemanda *alDem = (*itAlDem);

					Disciplina *discOrig = alDem->demanda->disciplina;
					int turmaAluno = problemData->retornaTurmaDiscAluno( aluno, discOrig );	

					if ( turmaAluno == -1 ) // aluno não alocado
					if ( discOrig->discEquivSubstitutas.find( disciplina ) !=
						 discOrig->discEquivSubstitutas.end() ) // 'disciplina' é substituta de 'discOrig'
					{
						achou=true;
										
						int cjtAlunoId = problemData->retornaCjtAlunosId( aluno );
						if ( cjtAlunoId==0 ) std::cout<<"\nERRO: nao achei cjtAlunoId, muito estranho!\n";

						bool existePraticaNova = problemData->refDisciplinas.find(-disciplina->getId()) != problemData->refDisciplinas.end();
						bool existePraticaAntiga = problemData->refDisciplinas.find(-discOrig->getId()) != problemData->refDisciplinas.end();
						int caso = ( !existePraticaAntiga && !existePraticaNova ? 1 : 
								 ( ( !existePraticaAntiga && existePraticaNova ? 2 :
								   ( existePraticaAntiga && !existePraticaNova ? 3 : 
								   ( existePraticaAntiga && existePraticaNova ? 4 : 0 ) ) ) ) );
						
						switch ( caso )
						{
							case 1: // (T => T)
								break;
							case 2: // (T => PT)								
								// CRIA ALUNODEMANDA PARA DISCIPLINA PRATICA
								break;
							case 3: // (PT => T)
								// REMOVE ALUNODEMANDA DE DISCIPLINA PRATICA
								if (1)
								{
									AlunoDemanda *alunoDemandaPraticaOrig = aluno->getAlunoDemanda( - abs( discOrig->getId() ) );
									aluno->demandas.remove( alunoDemandaPraticaOrig );
									problemData->listSlackDemandaAluno.remove( alunoDemandaPraticaOrig );
									problemData->cjtAlunoDemanda[cjtAlunoId].remove( alunoDemandaPraticaOrig );

									// Reduz qtd demanda pratica original
									Demanda *demPraticaOriginal = alunoDemandaPraticaOrig->demanda;
									demPraticaOriginal->setQuantidade( demPraticaOriginal->getQuantidade() - 1 );
									this->problemData->mapDemandaAlunos[ demPraticaOriginal ].remove( alunoDemandaPraticaOrig );
									if ( this->problemData->mapDemandaAlunos[demPraticaOriginal ].size() == 0 )
										this->problemData->mapDemandaAlunos.erase( demPraticaOriginal );
								}
								break;
							case 4: // (PT => PT)
								// ASSOCIA ALUNODEMANDA DE DISCIPLINA PRATICA ANTIGA COM PRATICA NOVA
								if ( disciplina->getId() < 0 ) // pratica
								{
									alDem = aluno->getAlunoDemanda( - abs( discOrig->getId() ) );
									discOrig = alDem->demanda->disciplina;

									int turmaAluno = problemData->retornaTurmaDiscAluno( aluno, discOrig );	
									if ( turmaAluno != -1 )
										std::cout<<"\nErro! como o aluno esta alocado na pratica, mas nao na teorica??\n";

									// Reduz qtd demanda pratica original
									Demanda *demPraticaOriginal = alDem->demanda;
									demPraticaOriginal->setQuantidade( demPraticaOriginal->getQuantidade() - 1 );
								}
								break;
							default: break;
						}

						// Debita 1 unidade na quantidade de alunos para a disc de credito teorico original							
						if ( disciplina->getId() > 0 ) 
						{
							alDem->demanda->setQuantidade( alDem->demanda->getQuantidade() - 1 );							
						}

						// Modificar referência de demanda
						// -----------------------------------------------------------------
						//							DEMANDAS
												
						Oferta *oft = alDem->demanda->oferta;
						Demanda *dem = problemData->buscaDemanda( oft->getId(), disciplina->getId() );

						if ( dem == NULL )
						{
							// Cria nova Demanda
							int id = problemData->retornaMaiorIdDemandas() + 1;
							dem = new Demanda();
							dem->setId( id );
							dem->setOfertaId( oft->getId() );
							dem->setDisciplinaId( disciplina->getId() );
							dem->setQuantidade( 0 );
							dem->oferta = oft;
							dem->disciplina = disciplina;

							problemData->demandasTotal.add( dem );
					 		problemData->demandas.add( dem );

							problemData->ofertasDisc[ disciplina->getId() ].add( oft );														
							problemData->cjtDemandas[cjtAlunoId].add( dem );
						}

						// Cria AlunoDemanda para disciplina pratica
						if ( caso==2 && disciplina->getId() < 0 )
						{
							idAlDemanda++;
							AlunoDemanda *novo_aluno_demanda_p = new AlunoDemanda( idAlDemanda, aluno->getAlunoId(), alDem->getPrioridade(), dem );
							problemData->alunosDemandaTotal.add( novo_aluno_demanda_p );
							problemData->alunosDemanda.add( novo_aluno_demanda_p );
							aluno->demandas.add( novo_aluno_demanda_p );
							this->problemData->mapDemandaAlunos[ dem ].add( novo_aluno_demanda_p );
							this->problemData->cjtAlunoDemanda[cjtAlunoId].add( novo_aluno_demanda_p );

							alDem = novo_aluno_demanda_p;
						}

						// Liga AlunoDemanda à Demanda
						alDem->demandaOriginal = alDem->demanda;														
						alDem->demanda = dem;
						alDem->setDemandaId( dem->getId() );
						dem->setQuantidade( dem->getQuantidade() + 1 );
						problemData->listSlackDemandaAluno.remove( alDem );
						this->problemData->mapDemandaAlunos[ alDem->demandaOriginal ].remove( alDem );
						if ( this->problemData->mapDemandaAlunos[ alDem->demandaOriginal ].size() == 0 )
							this->problemData->mapDemandaAlunos.erase( alDem->demandaOriginal );
						this->problemData->mapDemandaAlunos[ dem ].add( alDem );

						// -----------------------------------------------------------------

						if ( outEquiv )
						{
							outEquiv<<"\nAluno "<<aluno->getAlunoId()<<" Disc original "<<discOrig->getId()
								<<" Disc substituta "<<disciplina->getId()<<" Prioridade "<<alDem->getPrioridade();
						}

						break;
					}
				}
				if (!achou)
				{
					if (  prioridade==1 || 
						( prioridade==2 && problemData->procuraAlunoDemanda(disciplina->getId(), aluno->getAlunoId(), 1)==NULL ) )
					{
						std::cout<<"\nErro! Nao achei AlunoDemanda original nem substituto. Aluno"
						<<aluno->getAlunoId()<<" Disc"<<disciplina->getId()<<". Rodada P"<<prioridade<<"\n";
					}
				}
			}
		}
	}

	if ( outEquiv )
	{
		outEquiv.close();
	}
}


/* ----------------------------------------------------------------------------------
							VARIAVEIS TATICO POR ALUNO COM HORARIOS
 ---------------------------------------------------------------------------------- */

int TaticoIntAlunoHor::criaVariaveisTatico( int campusId, int P, int r )
{
	int num_vars = 0;
	CPUTimer timer;
	double dif = 0.0;

#ifdef PRINT_cria_variaveis
	int numVarsAnterior = 0;
#endif
	

	timer.start();
	num_vars += this->criaVariavelTaticoCreditos( campusId, P ); // x_{i,d,u,s,t,h,i,hf}
	timer.stop();
	dif = timer.getCronoCurrSecs();
	
#ifdef PRINT_cria_variaveis
	std::cout << "numVars \"x\": " << (num_vars - numVarsAnterior)  <<"\t "<<dif <<" sec" << std::endl; fflush(NULL);
	numVarsAnterior = num_vars;
#endif


	timer.start();	
	num_vars += this->criaVariavelTaticoAlunoCreditosAPartirDeX( campusId, P ); // v_{a,i,d,u,s,t,h,i,hf}
	//num_vars += criaVariavelTaticoAlunoCreditos( campusId, P ); // v_{a,i,d,u,s,t,h,i,hf}
	timer.stop();
	dif = timer.getCronoCurrSecs();
	
#ifdef PRINT_cria_variaveis
	std::cout << "numVars \"v\": " << (num_vars - numVarsAnterior)  <<"\t "<<dif <<" sec" << std::endl; fflush(NULL);
	numVarsAnterior = num_vars;
#endif	

	
	timer.start();
	num_vars += this->criaVariavelTaticoOferecimentosAPartirDeX( campusId, P ); // o_{i,d,u,s}
	//num_vars += this->criaVariavelTaticoOferecimentos( campusId, P ); // o_{i,d,u,s}
	timer.stop();
	dif = timer.getCronoCurrSecs();
	
#ifdef PRINT_cria_variaveis
	std::cout << "numVars \"o\": " << (num_vars - numVarsAnterior)  <<"\t "<<dif <<" sec" << std::endl; fflush(NULL);
	numVarsAnterior = num_vars;
#endif
	

	timer.start();
	num_vars += this->criaVariavelTaticoAlocaAlunoTurmaDisc( campusId, P );		// s_{i,d,a,cp}
	num_vars += this->criaVariavelTaticoAlocaAlunoTurmaDiscEquiv( campusId, P );  // s_{i,d,a,cp}	
	timer.stop();
	dif = timer.getCronoCurrSecs();
	
#ifdef PRINT_cria_variaveis
	std::cout << "numVars \"s\": " << (num_vars - numVarsAnterior)  <<"\t "<<dif <<" sec" << std::endl; fflush(NULL);
	numVarsAnterior = num_vars;
#endif



	timer.start();
	num_vars += this->criaVariavelTaticoCursoAlunos( campusId, P ); // b_{i,d,c,c'}
	timer.stop();
	dif = timer.getCronoCurrSecs();
	
#ifdef PRINT_cria_variaveis
	std::cout << "numVars \"b\": " << (num_vars - numVarsAnterior)  <<"\t "<<dif <<" sec" << std::endl; fflush(NULL);
	numVarsAnterior = num_vars;
#endif



	timer.start();
	num_vars += this->criaVariavelTaticoConsecutivosAPartirDeX( campusId, P ); // c
	//num_vars += this->criaVariavelTaticoConsecutivos( campusId, P ); // c
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_variaveis
	std::cout << "numVars \"c\": " << (num_vars - numVarsAnterior)  <<"\t "<<dif <<" sec" << std::endl; fflush(NULL);
	numVarsAnterior = num_vars;
#endif


	timer.start();
	num_vars += this->criaVariavelTaticoCombinacaoDivisaoCreditoAPartirDeO( campusId, P ); // m_{i,d,k}
	//num_vars += this->criaVariavelTaticoCombinacaoDivisaoCredito( campusId, P ); // m_{i,d,k}
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_variaveis
	std::cout << "numVars \"m\": " << (num_vars - numVarsAnterior)  <<"\t "<<dif <<" sec" << std::endl; fflush(NULL);
	numVarsAnterior = num_vars;
#endif

				
	timer.start();
	num_vars += this->criaVariavelTaticoFolgaCombinacaoDivisaoCreditoAPartirDeX( campusId, P ); // fk_{i,d,t}
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
	num_vars += this->criaVariavelTaticoAberturaCompativelAPartirDeX( campusId, P ); // zc
	//num_vars += this->criaVariavelTaticoAberturaCompativel( campusId, P ); // zc
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_variaveis
	std::cout << "numVars \"zc\": " << (num_vars - numVarsAnterior)  <<"\t "<<dif <<" sec" << std::endl; fflush(NULL);
	numVarsAnterior = num_vars;
#endif


	timer.start();
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
	num_vars += this->criaVariavelTaticoDiaUsadoPeloAluno( campusId, P ); // du_{a,t}
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_variaveis
	std::cout << "numVars \"du\": " << (num_vars - numVarsAnterior)  <<"\t "<<dif <<" sec" << std::endl; fflush(NULL);
	numVarsAnterior = num_vars;
#endif
		

	timer.start();
	num_vars += this->criaVariavelTaticoFolgaAbreTurmaSequencialAPartirDeX( campusId, P ); // ft_{i,d,cp}
	//num_vars += this->criaVariavelTaticoFolgaAbreTurmaSequencial( campusId, P ); // ft_{i,d,cp}
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_variaveis
	std::cout << "numVars \"ft\": " << (num_vars - numVarsAnterior)  <<"\t "<<dif <<" sec" << std::endl; fflush(NULL);
	numVarsAnterior = num_vars;
#endif
		

	timer.start();
	num_vars += this->criaVariavelFolgaPrioridadeInf( campusId, P ); // fpi_{a,cp}
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_variaveis
	std::cout << "numVars \"fpi\": " << (num_vars - numVarsAnterior)  <<"\t "<<dif <<" sec" << std::endl; fflush(NULL);
	numVarsAnterior = num_vars;
#endif
		

	timer.start();
	num_vars += this->criaVariavelFolgaPrioridadeSup( campusId, P ); // fps_{a,cp}
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_variaveis
	std::cout << "numVars \"fps\": " << (num_vars - numVarsAnterior)  <<"\t "<<dif <<" sec" << std::endl; fflush(NULL);
	numVarsAnterior = num_vars;
#endif
	

	timer.start();
	num_vars += this->criaVariavelTaticoFormandosNaTurma( campusId, P, r ); // f_{i,d,cp}
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_variaveis
	std::cout << "numVars \"f\": " << (num_vars - numVarsAnterior)  <<"\t "<<dif <<" sec" << std::endl; fflush(NULL);
	numVarsAnterior = num_vars;
#endif


	timer.start();
	num_vars += this->criaVariavelTaticoAberturaAPartirDeX( campusId, P ); // z_{i,d,cp}
	//num_vars += this->criaVariavelTaticoAbertura( campusId, P, r ); // z_{i,d,cp}
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_variaveis
	std::cout << "numVars \"z\": " << (num_vars - numVarsAnterior)  <<"\t "<<dif <<" sec" << std::endl; fflush(NULL);
	numVarsAnterior = num_vars;
#endif

	return num_vars;

}

// v_{a,i,d,u,s,hi,hf,t}
int TaticoIntAlunoHor::criaVariavelTaticoAlunoCreditosAPartirDeX( int campusId, int P )
{
	int numVars = 0;

	std::map<Disciplina*, GGroup<AlunoDemanda*,LessPtr<AlunoDemanda>>, LessPtr<Disciplina> > mapDiscAlunosDemanda;
	if ( problemData->parametros->considerar_equivalencia_por_aluno && USAR_EQUIVALENCIA ) 
	{		
		ITERA_GGROUP_LESSPTR( itDisc, problemData->disciplinas, Disciplina )
			mapDiscAlunosDemanda[*itDisc] = problemData->retornaDemandasDiscNoCampus_Equiv( *itDisc, campusId, P );
	}
	else
	{
		ITERA_GGROUP_LESSPTR( itDisc, problemData->disciplinas, Disciplina )
			mapDiscAlunosDemanda[*itDisc] = problemData->retornaDemandasDiscNoCampus( (*itDisc)->getId(), campusId, P );
	}
	
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

		GGroup<AlunoDemanda*,LessPtr<AlunoDemanda>> alunosDemanda =
			mapDiscAlunosDemanda[ disciplina ];

		ITERA_GGROUP_LESSPTR ( itAlDem, alunosDemanda, AlunoDemanda )
		{
			Aluno *aluno = problemData->retornaAluno( itAlDem->getAlunoId() );

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

			if ( varHashAux.find( v ) == varHashAux.end() )
			{
				bool fixar=false;

				if ( !criaVariavelTatico( &v, fixar, P ) )
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

// pode deletar essa criaVariavelTaticoAlunoCreditos caso criaVariavelTaticoAlunoCreditosAPartirDeX funcione bem, sem problemas
// v_{a,i,d,u,s,hi,hf,t}
int TaticoIntAlunoHor::criaVariavelTaticoAlunoCreditos( int campusId, int P )
{
	int numVars = 0;
    	
	std::map<Disciplina*, GGroup<AlunoDemanda*,LessPtr<AlunoDemanda>>, LessPtr<Disciplina> > mapDiscAlunosDemanda;
	if ( problemData->parametros->considerar_equivalencia_por_aluno && USAR_EQUIVALENCIA ) 
	{		
		ITERA_GGROUP_LESSPTR( itDisc, problemData->disciplinas, Disciplina )
			mapDiscAlunosDemanda[*itDisc] = problemData->retornaDemandasDiscNoCampus_Equiv( *itDisc, campusId, P );
	}
	else
	{
		ITERA_GGROUP_LESSPTR( itDisc, problemData->disciplinas, Disciplina )
			mapDiscAlunosDemanda[*itDisc] = problemData->retornaDemandasDiscNoCampus( (*itDisc)->getId(), campusId, P );
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
			 if ( itCjtSala->salas.size() > 1 )
			 {
				std::cout<<"\nATENCAO em criaVariavelTaticoCreditos: conjunto sala deve ter somente 1 sala! \n";
			 }

			 int salaId = itCjtSala->salas.begin()->first;

            ITERA_GGROUP_LESSPTR( it_disciplina, itCjtSala->disciplinas_associadas, Disciplina )
            {
				Disciplina * disciplina = ( *it_disciplina );
			   
				std::cout<<"\n\nDisc"<<disciplina->getId();

				#pragma region Equivalencias
				if ( ( problemData->mapDiscSubstituidaPor.find( disciplina ) !=
					   problemData->mapDiscSubstituidaPor.end() ) &&
					!problemData->ehSubstituta( disciplina ) )
				{
					continue;
				}
				#pragma endregion
				
				GGroup<AlunoDemanda*, LessPtr<AlunoDemanda>> alunosDemanda = mapDiscAlunosDemanda[disciplina];		
				if ( alunosDemanda.size() == 0 )
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

								 if ( ! disciplina->inicioTerminoValidos( hi, hf ) )
									 continue;
								 
								 std::cout<<"\nTurma"<<turma<<" Dia"<<dia<<" Hi"<<hi->getId()<<" Hf"<<hf->getId();
								 
								 ITERA_GGROUP_LESSPTR ( itAlDem, alunosDemanda, AlunoDemanda )
								 {
									 Aluno *aluno = problemData->retornaAluno( itAlDem->getAlunoId() );

									 std::cout<<"\nAluno"<<aluno->getAlunoId();

									 VariableTatInt v;
									 v.reset();
									 v.setType( VariableTatInt::V_ALUNO_CREDITOS );

									 v.setTurma( turma );            // i
									 v.setDisciplina( disciplina );  // d
									 v.setUnidade( *itUnidade );     // u
									 v.setSubCjtSala( *itCjtSala );  // tps  
									 v.setDia( dia );				 // t
									 v.setHorarioAulaInicial( hi );	 // hi
									 v.setHorarioAulaFinal( hf );	 // hf
									 v.setAluno( aluno );			 // a

									 if ( vHashTatico.find( v ) == vHashTatico.end() )
									 {
										bool fixar=false;

										std::cout<<" Verifica criacao...";
										if ( !criaVariavelTatico( &v, fixar, P ) )
										{	std::cout<<" nao criar!";
											continue;
										}
										std::cout<<" ok!";
										
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
    }

	return numVars;

}

// x_{i,d,u,s,hi,hf,t}
int TaticoIntAlunoHor::criaVariavelTaticoCreditos( int campusId, int P )
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
                Disciplina *disciplina = ( *it_disciplina );
			   
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

								 if ( ! disciplina->inicioTerminoValidos( hi, hf ) )
									 continue;								 

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

								 if ( vHashTatico.find( v ) == vHashTatico.end() )
								 {
									bool fixar=false;

									if ( !criaVariavelTatico( &v, fixar, P ) )
										continue;

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
		VariableTatInt v = vit->first;

		if( v.getType() != VariableTatInt::V_CREDITOS )
		{
			continue;
		}

		int turma = v.getTurma();
		Disciplina* disciplina = v.getDisciplina();
		Unidade *unidade = v.getUnidade();
		ConjuntoSala *cjtSala = v.getSubCjtSala();
				
		v.reset();
        v.setType( VariableTatInt::V_OFERECIMENTO );
        v.setTurma( turma );            // i
        v.setDisciplina( disciplina );  // d
        v.setUnidade( unidade );     // u
        v.setSubCjtSala( cjtSala );  // tps

		if ( varHashAux.find( v ) == varHashAux.end() )
		{
			bool fixar=false;

			if ( !criaVariavelTatico( &v, fixar, P ) )
			{
				std::cout<<"\nEstranho!! Tem x e nao tem o?";
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

               for ( int turma = 0; turma < disciplina->getNumTurmas(); turma++ )
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

						if ( !criaVariavelTatico( &v, fixar, P ) )
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

		if ( aluno->getOferta()->getCampusId() != campusId )
		{
			continue;
		}

		ITERA_GGROUP_LESSPTR( itAlDemanda, aluno->demandas, AlunoDemanda )
		{
			Disciplina *disciplina = (*itAlDemanda)->demanda->disciplina;
	
			for ( int turma = 0; turma < disciplina->getNumTurmas(); turma++ )
			{
				VariableTatInt v;
				v.reset();
				v.setType( VariableTatInt::V_ALOCA_ALUNO_TURMA_DISC );
				v.setAluno( aluno );
				v.setDisciplina( disciplina );
				v.setTurma( turma );
				v.setCampus( cp );

				if ( vHashTatico.find( v ) == vHashTatico.end() )
				{
					bool fixar=false;

					if ( !criaVariavelTatico( &v, fixar, P ) )
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
		  if ( ( problemData->mapDiscSubstituidaPor.find( disciplina ) !=
				problemData->mapDiscSubstituidaPor.end() ) &&
				!problemData->ehSubstituta( disciplina ) )
		  {
			  continue;
		  }
		  #pragma endregion

		  int n=0;		  
		  if ( problemData->parametros->considerar_equivalencia_por_aluno && USAR_EQUIVALENCIA )
			n = problemData->haDemandaDiscNoCursoEquiv( disciplina, pt_Curso->getId() );
		  else
			n = problemData->haDemandaDiscNoCurso( disciplina->getId(), pt_Curso->getId() );
		  
		  if ( n <= 0 ) continue;

         for ( int turma = 0; turma < disciplina->getNumTurmas(); turma++ )
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

				if ( !criaVariavelTatico( &v, fixar, P ) )
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

			if ( !criaVariavelTatico( &v, fixar, P ) )
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
		
        for ( int turma = 0; turma < disciplina->getNumTurmas(); turma++ )
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
	
					if ( !criaVariavelTatico( &v, fixar, P ) )
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

				if ( !criaVariavelTatico( &v, fixar, P ) )
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
		
      for ( int turma = 0; turma < disciplina->getNumTurmas(); turma++ )
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

				if ( !criaVariavelTatico( &v, fixar, P ) )
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
int TaticoIntAlunoHor::criaVariavelTaticoFolgaCombinacaoDivisaoCreditoAPartirDeX( int campusId, int P )
{
	int numVars = 0;
	
	Campus *cp = problemData->refCampus[campusId];

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
		if ( v.getUnidade()->getIdCampus() != campusId ) continue;

		int turma = v.getTurma();
		Disciplina* disciplina = v.getDisciplina();
		int dia = v.getDia();

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
							
		Trio<double, double, double> trio;
		trio.set( coef,lowerBound, upperBound );

		v.reset();
        v.setType( VariableTatInt::V_SLACK_COMBINACAO_DIVISAO_CREDITO_P );
        v.setTurma( turma );            // i
        v.setDisciplina( disciplina );  // d
		v.setDia( dia );				// t
		v.setCampus( cp );				// cp

		if ( varHashAux.find( v ) == varHashAux.end() )
		{
			bool fixar=false;
			if ( criaVariavelTatico( &v, fixar, P ) )
			{						
				int id = numVars;
				varHashAux[ v ] = id;
				varId_Bounds[ id ] = trio;				
				numVars++;
			}
		}

        v.setType( VariableTatInt::V_SLACK_COMBINACAO_DIVISAO_CREDITO_M );
        
		if ( varHashAux.find( v ) == varHashAux.end() )
		{
			bool fixar=false;
			if ( criaVariavelTatico( &v, fixar, P ) )
			{						
				int id = numVars;
				varHashAux[ v ] = id;
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
				
		for ( int turma = 0; turma < disciplina->getNumTurmas(); turma++ )
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

					if ( criaVariavelTatico( &v, fixar, P ) )
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

					if ( criaVariavelTatico( &v, fixar, P ) )
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

								if ( !criaVariavelTatico( &v, fixar, P ) )
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

								if ( !criaVariavelTatico( &v, fixar, P ) )
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

			if ( !criaVariavelTatico( &v, fixar, P ) )
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

				if ( !criaVariavelTatico( &v, fixar, P ) )
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

	    if ( aluno->getOferta()->getCampusId() != campusId )
	    {
		    continue;
	    }

		ITERA_GGROUP_LESSPTR( itAlDemanda, aluno->demandas, AlunoDemanda )
		{
			Disciplina *disciplina = itAlDemanda->demanda->disciplina;

			#pragma region Equivalencias
			if ( ( problemData->mapDiscSubstituidaPor.find( disciplina ) !=
					problemData->mapDiscSubstituidaPor.end() ) &&
					!problemData->ehSubstituta( disciplina ) )
			{
				continue;
			}
			#pragma endregion

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

				if ( !criaVariavelTatico( &v, fixar, P ) )
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
						coef = - 50 * disciplina->getTotalCreditos() * aluno->getOferta()->getReceita();
					}
					else if ( problemData->parametros->funcao_objetivo == 1 )
					{
						coef = 10 * disciplina->getTotalCreditos() * cp->getCusto();
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
         for ( int turma1 = 0; turma1< disciplina1->getNumTurmas(); turma1++ )
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
				  for ( int turma2 = 0; turma2 < disciplina2->getNumTurmas(); turma2++ )
				  {
					   Unidade * u2 = this->retornaUnidadeDeAtendimento( turma2, disciplina2, campus );

					   if ( u2 == NULL || u1 == u2 )
							continue;

					    GGroup<Aluno*> alunosEmComum = 
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

								if ( !criaVariavelTatico( &v, fixar, P ) )
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

			if ( aluno->getOferta()->getCampusId() != campusId )
			{
				continue;
			}

			VariableTatInt v;
			v.reset();
			v.setType( VariableTatInt::V_ALUNO_DIA );
			v.setAluno( aluno );	// a
			v.setDia( dia );		// t

			if ( vHashTatico.find( v ) == vHashTatico.end() )
			{
				bool fixar=false;

				if ( !criaVariavelTatico( &v, fixar, P ) )
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

			if ( !criaVariavelTatico( &v, fixar, P ) )
			{
				continue;
			}
						
			int id = numVars;
			varHashAux[ v ] = id;
				
			double coef = 0.0;
			if ( problemData->parametros->funcao_objetivo == 0 )
			{							 
				coef = -10 * (turma+1);
			}
			else if ( problemData->parametros->funcao_objetivo == 1 )
			{					
				coef = 10 * (turma+1);
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

		 #pragma region Equivalencias
		 if ( ( problemData->mapDiscSubstituidaPor.find( disciplina ) !=
				problemData->mapDiscSubstituidaPor.end() ) &&
				!problemData->ehSubstituta( disciplina ) )
		 {
		 	continue;
		 }
		 #pragma endregion
	 		 
         for ( int turma = 0; turma < disciplina->getNumTurmas(); turma++ )
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

				if ( !criaVariavelTatico( &v, fixar, P ) )
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
					{	if ( problemData->haDemandaDiscNoCursoEquiv( disciplina, c1->getId() ) == 0 ||
							 problemData->haDemandaDiscNoCursoEquiv( disciplina, c2->getId() ) == 0 )
							continue;
					}
					else 
					{	if ( problemData->haDemandaDiscNoCurso( disciplina->getId(), c1->getId() ) == 0 ||
					  	     problemData->haDemandaDiscNoCurso( disciplina->getId(), c2->getId() ) == 0 )
							continue;
					}

					for ( int turma = 0; turma < disciplina->getNumTurmas(); turma++ )
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

							if ( !criaVariavelTatico( &v, fixar, P ) )
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

		if ( aluno->getOferta()->getCampusId() != campusId )
		{
			continue;
		}

		VariableTatInt v;
		v.reset();
		v.setType( VariableTatInt::V_SLACK_PRIOR_INF );
		v.setAluno( aluno );
		v.setCampus( cp );

		if ( vHashTatico.find( v ) == vHashTatico.end() )
		{
			vHashTatico[v] = lp->getNumCols();					

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

		if ( aluno->getOferta()->getCampusId() != campusId )
		{
			continue;
		}

		VariableTatInt v;
		v.reset();
		v.setType( VariableTatInt::V_SLACK_PRIOR_SUP );
		v.setAluno( aluno );
		v.setCampus( cp );

		if ( vHashTatico.find( v ) == vHashTatico.end() )
		{
			vHashTatico[v] = lp->getNumCols();
					
			int totalCreditos = problemData->creditosNaoAtendidosPorPrioridade( 1, aluno->getAlunoId() );
			double cargaHorariaNaoAtendida = problemData->cargaHorariaNaoAtendidaPorPrioridade( 1, aluno->getAlunoId() );	
			if ( cargaHorariaNaoAtendida == 0 ) // se não houver folga de demanda de P1
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

	if ( prior==1 && r==1 ) // só considera formandos a partir da segunda rodada
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
		 
		 if ( problemData->parametros->considerar_equivalencia_por_aluno && USAR_EQUIVALENCIA )
		 {  if ( ! problemData->haDemandaPorFormandosEquiv( disciplina, cp, prior ) )
			   continue;
		 }
		 else if ( ! problemData->haDemandaPorFormandos( disciplina, cp, prior ) )
			 continue;


         for ( int turma = 0; turma < disciplina->getNumTurmas(); turma++ )
         {
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
				bool fixar=false;
				if ( !criaVariavelTatico( &v, fixar, prior ) )
					continue;

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

	if ( !problemData->parametros->considerar_equivalencia_por_aluno || !USAR_EQUIVALENCIA )
		return numVars;

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

			int turmaAluno = problemData->retornaTurmaDiscAluno( aluno, disciplina );
				
			if ( turmaAluno == -1 ) // aluno não alocado
			{
				if ( problemData->cargaHorariaOriginalRequeridaPorPrioridade( 1, aluno ) -
					 problemData->cargaHorariaJaAtendida( aluno ) <= 0 )
					 continue;

				ITERA_GGROUP_LESSPTR( itDiscEq, disciplina->discEquivSubstitutas, Disciplina )
				{
					Disciplina *disciplinaEquiv = (*itDiscEq);

					for ( int turma = 0; turma < disciplinaEquiv->getNumTurmas(); turma++ )
					{
						VariableTatInt v;
						v.reset();
						v.setType( VariableTatInt::V_ALOCA_ALUNO_TURMA_DISC );
						v.setAluno( aluno );
						v.setDisciplina( disciplinaEquiv );
						v.setTurma( turma );
						v.setCampus( cp );

						if ( vHashTatico.find( v ) == vHashTatico.end() )
						{
							bool fixar=false;
							if ( !criaVariavelTatico( &v, fixar, P ) )
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

				if ( vHashTatico.find( v ) == vHashTatico.end() )
				{
					bool fixar=false;
					if ( !criaVariavelTatico( &v, fixar, P ) )
					{	std::cout<<"\nERRO: acho que nao devia entrar aqui, o aluno ta alocado!!\n";
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
		v.setType( VariableTatInt::V_ABERTURA );
        v.setTurma( turma );            // i
        v.setDisciplina( disciplina );  // d
        v.setCampus( campus );			// cp

		if ( varHashAux.find( v ) == varHashAux.end() )
		{
			bool fixar=false;

			if ( !criaVariavelTatico( &v, fixar, prior ) )
			{
				std::cout<<"\nEstranho!! Tem x e nao tem z?";
				continue;
			}
						
			int id = numVars;
			varHashAux[ v ] = id;
				
			double coef = 1.0;		
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
		 
         for ( int turma = 0; turma < disciplina->getNumTurmas(); turma++ )
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
				if ( !criaVariavelTatico( &v, fixar, prior ) )
					continue;

                lp->getNumCols();
                vHashTatico[v] = lp->getNumCols();

			    double coef = 1.0;
						 
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
	restricoes += criaRestricaoTaticoAssociaVeX( campusId );				// Restricao 1.2.2
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
	restricoes += criaRestricaoTaticoAtendeAluno( campusId, prioridade );						// Restricao 1.2.5
	restricoes += criaRestricaoTaticoAtendeAlunoEquiv( campusId, prioridade );					// Restricao 1.2.5
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
	restricoes += criaRestricaoTaticoLimitaAberturaTurmas( campusId, prioridade );			// Restricao 1.2.7
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
	restricoes += criaRestricaoTaticoAlunoDiscPraticaTeorica( campusId );					// Restricao 1.2.17
	restricoes += criaRestricaoTaticoAlunoDiscPraticaTeoricaEquiv( campusId, prioridade );	// Restricao 1.2.17
	timer.stop();
	dif = timer.getCronoCurrSecs();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest \"1.2.17\": " << (restricoes - numRestAnterior)  <<" "<<dif <<" sec" << std::endl;
	numRestAnterior = restricoes;
#endif


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
	restricoes += criaRestricaoPrioridadesDemanda( campusId, prioridade );	// Restricao 1.2.21
	restricoes += criaRestricaoPrioridadesDemandaEquiv( campusId, prioridade );	// Restricao 1.2.21
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

	return restricoes;
}


int TaticoIntAlunoHor::criaRestricaoTaticoAssociaVeX( int campusId )
{
	int restricoes = 0;
	int nnz;
	char name[ 100 ];

	VariableTatInt v;
	ConstraintTatInt c;

	VariableTatIntHash::iterator vit;
	ConstraintTatIntHash::iterator cit;
		
    std::vector<int> idxC;
    std::vector<int> idxR;
    std::vector<double> valC;

	vit = vHashTatico.begin();
	while(vit != vHashTatico.end())
	{
		if( vit->first.getType() != VariableTatInt::V_CREDITOS &&
			vit->first.getType() != VariableTatInt::V_ALUNO_CREDITOS )
		{
			vit++;
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
		
		double coef=0.0;
		if ( vit->first.getType() == VariableTatInt::V_CREDITOS )
			coef = - v.getSubCjtSala()->getCapacidadeRepr() ;
		else if ( vit->first.getType() == VariableTatInt::V_ALUNO_CREDITOS )
			coef = 1.0;

		cit = cHashTatico.find(c);
		if(cit == cHashTatico.end())
		{
			sprintf( name, "%s", c.toString().c_str() );
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

int TaticoIntAlunoHor::criaRestricaoTaticoUsoDeSalaParaCadaHorario( int campusId )
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
				c.setType( ConstraintTatInt::C_SALA_HORARIO );
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
               idxC.push_back(vit->second);
               idxR.push_back(cit->second);
               valC.push_back(1.0);
					//lp->chgCoef(cit->second, vit->second,  1.0);
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

int TaticoIntAlunoHor::criaRestricaoTaticoAtendimentoUnicoTurmaDiscDia( int campusId )
{
	int restricoes = 0;

	int nnz;
	char name[ 100 ];

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
			sprintf( name, "%s", c.toString().c_str() );
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

   char name[ 100 ];
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

		  for ( int turma = 0; turma < disciplina->getNumTurmas(); turma++ )
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

				sprintf( name, "%s", c.toString().c_str() ); 

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
   char name[ 100 ];
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
					

					for ( int turma = 0; turma < disciplina->getNumTurmas(); turma++ )
					{
						c.reset();
						c.setType( ConstraintTatInt::C_ALUNOS_CURSOS_INCOMP );
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

   char name[ 100 ];
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

				for ( int turma = 0; turma < disciplina->getNumTurmas(); turma++ )
				{
					c.reset();
					c.setType( ConstraintTatInt::C_PROIBE_COMPARTILHAMENTO );
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
   
   if ( !problemData->parametros->min_alunos_abertura_turmas )
   {
	   return restricoes;
   }

   int MinAlunos = problemData->parametros->min_alunos_abertura_turmas_value;
   if (MinAlunos<=0) MinAlunos=1;

   int nnz;
   char name[ 200 ];

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
			campus = v.getAluno()->getOferta()->campus;
		else if ( v.getType() == VariableTatInt::V_FORMANDOS_NA_TURMA || v.getType() == VariableTatInt::V_ABERTURA )
			campus = v.getCampus();

		int turma = v.getTurma();
		Disciplina* disc = v.getDisciplina();

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
			sprintf( name, "%s", c.toString().c_str() );
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


int TaticoIntAlunoHor::criaRestricaoTaticoAlunoDiscPraticaTeorica( int campusId )
{
   int restricoes = 0;

   if ( problemData->parametros->considerar_equivalencia_por_aluno && USAR_EQUIVALENCIA )
		return restricoes;

   char name[ 200 ];
   int nnz;

   ConstraintTatInt c;
   VariableTatInt v;
   VariableTatIntHash::iterator it_v;   

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
				std::cout<<"\nErro em criaRestricaoTaticoAlunoDiscPraticaTeorica: disciplina teorica nao encontrada.\n";
				continue;
			}
			
			c.reset();
			c.setType( ConstraintTatInt::C_ALUNO_DISC_PRATICA_TEORICA );
			c.setAluno( aluno );
			c.setDisciplina( discPratica );
			c.setCampus( campus );

			sprintf( name, "%s", c.toString().c_str() ); 

			if ( cHashTatico.find( c ) != cHashTatico.end() )
			{
				continue;
			}

			nnz = discPratica->getNumTurmas() + discTeorica->getNumTurmas() + 1;

			OPT_ROW row( nnz, OPT_ROW::EQUAL , 0.0 , name );

			for ( int turma = 0; turma < discPratica->getNumTurmas(); turma++ )
			{
				VariableTatInt v;
				v.reset();
				v.setType( VariableTatInt::V_ALOCA_ALUNO_TURMA_DISC );
				v.setAluno( aluno );
				v.setDisciplina( discPratica );
				v.setTurma( turma );
				v.setCampus( campus );

				it_v = vHashTatico.find( v );
				if( it_v != vHashTatico.end() )
				{
					row.insert( it_v->second, 1.0 );
				}
			}

			for ( int turma = 0; turma < discTeorica->getNumTurmas(); turma++ )
			{
				VariableTatInt v;
				v.reset();
				v.setType( VariableTatInt::V_ALOCA_ALUNO_TURMA_DISC );
				v.setAluno( aluno );
				v.setDisciplina( discTeorica );
				v.setTurma( turma );
				v.setCampus( campus );

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

int TaticoIntAlunoHor::criaRestricaoTaticoDivisaoCredito( int campusId )
{
   int restricoes = 0;
   char name[ 100 ];
   int nnz;

   ConstraintTatInt c;
   VariableTatInt v;
   VariableTatIntHash::iterator it_v;

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
	  		 
	  if ( disciplina->divisao_creditos.size() != 0 )
      {		 
         for ( int turma = 0; turma < disciplina->getNumTurmas(); turma++ )
         {		
            ITERA_GGROUP_N_PT( itDiasLetDisc, disciplina->diasLetivos, int )
            {
				int dia = *itDiasLetDisc;

                c.reset();
                c.setType( ConstraintTatInt::C_DIVISAO_CREDITO );

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

				it_v = vHashTatico.begin();
				for ( ; it_v != vHashTatico.end(); it_v++)
				{		
					VariableTatInt v = it_v->first;

					if( v.getType() != VariableTatInt::V_CREDITOS )
					{
						continue;
					}
					if( v.getDisciplina()->getId() == disciplina->getId() && v.getDia() == dia && v.getTurma() == turma )
					{
						int nCreds = disciplina->getCalendario()->retornaNroCreditosEntreHorarios(v.getHorarioAulaInicial(), v.getHorarioAulaFinal());
						row.insert( it_v->second, nCreds );
					}										
				}				

				for ( int k = 0; k < (int)disciplina->combinacao_divisao_creditos.size(); k++ )
				{				
					v.reset();
					v.setType( VariableTatInt::V_COMBINACAO_DIVISAO_CREDITO );
					v.setDisciplina( disciplina );
					v.setTurma( turma );
					v.setK( k );
					v.setCampus( problemData->refCampus[campusId] );

					// N{d,k,t}
					int numCreditos = ( disciplina->combinacao_divisao_creditos[ k ] )[ dia - 2 ].second;
					
					it_v = vHashTatico.find( v );
					if ( it_v != vHashTatico.end() )
					{
						row.insert( it_v->second, -numCreditos );
					}
				}

				// fkm
				v.reset();
				v.setType( VariableTatInt::V_SLACK_COMBINACAO_DIVISAO_CREDITO_M );
				v.setDisciplina( disciplina );
				v.setTurma( turma );
				v.setDia( dia );
				v.setCampus( problemData->refCampus[campusId] );
				it_v = vHashTatico.find( v );
				if( it_v != vHashTatico.end() )
				{
					row.insert( it_v->second, 1.0 );
				}

				// fkp
				v.reset();
				v.setType( VariableTatInt::V_SLACK_COMBINACAO_DIVISAO_CREDITO_P );
				v.setDisciplina( disciplina );
				v.setTurma( turma );
				v.setDia( dia );
				v.setCampus( problemData->refCampus[campusId] );
				it_v = vHashTatico.find( v );
				if( it_v != vHashTatico.end() )
				{
					row.insert( it_v->second, -1.0 );
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
   char name[ 100 ];
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
			sprintf( name, "%s", c.toString().c_str() );
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
   char name[ 100 ];
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
			sprintf( name, "%s", c.toString().c_str() );
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

							if ( *hf < *hi )
							{
							 	continue;
							}

							for ( int turma = 0; turma < disciplina->getNumTurmas(); turma++ )
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
   char name[ 100 ];
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

            sprintf( name, "%s", c.toString().c_str() ); 

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

         for ( int turma = 0; turma < disciplina->getNumTurmas(); turma++ )
         {
            c.reset();
            c.setType( ConstraintTatInt::C_TURMA_SALA );

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
   char name[ 200 ];

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
		
		Aluno *aluno = v.getAluno();

		Sala *sala = v.getSubCjtSala()->salas.begin()->second;
		ITERA_GGROUP_LESSPTRPTR( it_horario_dia, sala->horariosDia, HorarioDia )
		{
			HorarioDia * horario_dia = ( *it_horario_dia );

			int dia = horario_dia->getDia();

			if ( v.getDia() != dia )
				continue;

			HorarioAula * horario_aula = horario_dia->getHorarioAula();
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
			c.setHorarioAula( horario_aula );

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
				sprintf( name, "%s", c.toString().c_str() );
				nnz = 100;

				OPT_ROW row( nnz, OPT_ROW::LESS , 1.0, name );

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

					    GGroup<Aluno*> alunosEmComum = 
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

							sprintf( name, "%s", c.toString().c_str() );

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
   char name[ 200 ];

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
			sprintf( name, "%s", c.toString().c_str() );
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
   char name[ 200 ];

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
			sprintf( name, "%s", c.toString().c_str() );
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
   char name[ 200 ];

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
			sprintf( name, "%s", c.toString().c_str() );
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
   char name[ 200 ];

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
			sprintf( name, "%s", c.toString().c_str() );
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
   char name[ 200 ];

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
			sprintf( name, "%s", c.toString().c_str() );
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
   char name[ 200 ];

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
			coef = - disc->getCalendario()->retornaNroCreditosEntreHorarios( hi, hf );
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
			sprintf( name, "%s", c.toString().c_str() );
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
   char name[ 100 ];
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
         for ( int turma = 0; turma < ( disciplina->getNumTurmas() - 1 ); turma++ )
         {
            c.reset();
            c.setType( ConstraintTatInt::C_ABRE_TURMAS_EM_SEQUENCIA );
            c.setDisciplina( disciplina );
            c.setTurma( turma );

            sprintf( name, "%s", c.toString().c_str() ); 
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
   char name[ 200 ];

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
		
		int turma = v.getTurma();
		Disciplina* disc = v.getDisciplina();
		Curso *curso = NULL;

		double coef = 0.0;
		if ( v.getType() == VariableTatInt::V_TURMA_ATEND_CURSO ) // b
		{
			curso = v.getCurso();
			if ( USAR_EQUIVALENCIA ) coef = - problemData->haDemandaDiscNoCursoEquiv( disc, curso->getId() );
			else coef = - problemData->haDemandaDiscNoCurso( disc->getId(), curso->getId() );
		}
		else if ( v.getType() == VariableTatInt::V_ALOCA_ALUNO_TURMA_DISC ) // s
		{
			curso = v.getAluno()->getOferta()->curso;
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
			sprintf( name, "%s", c.toString().c_str() );
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

    char name[ 200 ];
    int nnz;

    ConstraintTatInt c;
    VariableTatInt v;
    VariableTatIntHash::iterator it_v;   

    Campus *campus = problemData->refCampus[campusId];

	ITERA_GGROUP_LESSPTR( itAluno, problemData->alunos, Aluno )
	{
		Aluno *aluno = *itAluno;

		if ( aluno->getOferta()->getCampusId() != campusId )
		{
			continue;
		}

		c.reset();
		c.setType( ConstraintTatInt::C_ALUNO_PRIORIDADES_DEMANDA );
		c.setAluno( aluno );
		c.setCampus( campus );

		sprintf( name, "%s", c.toString().c_str() ); 

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

			for ( int turma = 0; turma < disciplina->getNumTurmas(); turma++ )
			{
				VariableTatInt v;
				v.reset();
				v.setType( VariableTatInt::V_ALOCA_ALUNO_TURMA_DISC );
				v.setAluno( aluno );
				v.setDisciplina( disciplina );
				v.setTurma( turma );
				v.setCampus( campus );

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


		if ( row.getnnz() >= 3 )
		{
			cHashTatico[ c ] = lp->getNumRows();
			lp->addRow( row );
			restricoes++;
		}
	}

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

    char name[ 200 ];
    int nnz;

    ConstraintTatInt c;
    VariableTatInt v;
    VariableTatIntHash::iterator it_v;   

    Campus *campus = problemData->refCampus[campusId];

	ITERA_GGROUP_LESSPTR( itAluno, problemData->alunos, Aluno )
	{
		Aluno *aluno = *itAluno;

		if ( aluno->getOferta()->getCampusId() != campusId )
		{
			continue;
		}

		c.reset();
		c.setType( ConstraintTatInt::C_ALUNO_PRIORIDADES_DEMANDA );
		c.setAluno( aluno );
		c.setCampus( campus );

		sprintf( name, "%s", c.toString().c_str() ); 

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

					int turmaAluno = problemData->retornaTurmaDiscAluno( aluno, disciplinaEquiv );				
					if ( turmaAluno != -1 ) // dentre as equivalentes, evita aqui considerar as duplicatas de p1
						continue;

					for ( int turma = 0; turma < disciplinaEquiv->getNumTurmas(); turma++ )
					{
						VariableTatInt v;
						v.reset();
						v.setType( VariableTatInt::V_ALOCA_ALUNO_TURMA_DISC );
						v.setAluno( aluno );
						v.setDisciplina( disciplinaEquiv );
						v.setTurma( turma );
						v.setCampus( campus );

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
				for ( int turma = 0; turma < disciplina->getNumTurmas(); turma++ )
				{
					VariableTatInt v;
					v.reset();
					v.setType( VariableTatInt::V_ALOCA_ALUNO_TURMA_DISC );
					v.setAluno( aluno );
					v.setDisciplina( disciplina );
					v.setTurma( turma );
					v.setCampus( campus );

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


		if ( row.getnnz() >= 3 )
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
		
    char name[ 100 ];

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
				sprintf( name, "%s", c.toString().c_str() ); 
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
				sprintf( name, "%s", c.toString().c_str() ); 
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
				sprintf( name, "%s", c.toString().c_str() ); 
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
				sprintf( name, "%s", c.toString().c_str() ); 
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

	if ( ! problemData->parametros->considerar_equivalencia_por_aluno || !USAR_EQUIVALENCIA )
		return restricoes;
		
    char name[ 200 ];
    int nnz;

    ConstraintTatInt c;
    VariableTatInt v;
    VariableTatIntHash::iterator it_v;   

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

			if ( itAlDemanda->getPrioridade() != prioridade )
				continue;
			
			if ( problemData->retornaTurmaDiscAluno( aluno, disciplina ) != -1 )
				continue;

			c.reset();
			c.setType( ConstraintTatInt::C_DEMANDA_EQUIV_ALUNO );
			c.setAluno( aluno );
			c.setDisciplina( disciplina );
			c.setCampus( campus );

			sprintf( name, "%s", c.toString().c_str() );

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
				if ( deq->getId() < 0 )
					continue;

				if ( problemData->retornaTurmaDiscAluno( aluno, deq ) != -1 )
				{
					std::cout<<"\nEstranho, isso nao faz muito sentido. Significa que o aluno possui duas demandas"
						" diferentes para disciplinas consideradas equivalentes. Aluno id = " << aluno->getAlunoId()
						<< ", disciplina original id = "<< disciplina->getId() << " e disciplina substituta id=" 
						<< deq->getId() << endl;
					continue;
				}

				for ( int turma = 0; turma < disciplina->getNumTurmas(); turma++ )
				{
					VariableTatInt v;
					v.reset();
					v.setType( VariableTatInt::V_ALOCA_ALUNO_TURMA_DISC );
					v.setAluno( aluno );
					v.setDisciplina( deq );
					v.setTurma( turma );
					v.setCampus( campus );

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
int TaticoIntAlunoHor::criaRestricaoTaticoAlunoDiscPraticaTeoricaEquiv( int campusId, int prioridade )
{
   int restricoes = 0;   

	if ( ! problemData->parametros->considerar_equivalencia_por_aluno || !USAR_EQUIVALENCIA )
		return restricoes;

   char name[ 200 ];
   int nnz;

   ConstraintTatInt c;
   VariableTatInt v;
   VariableTatIntHash::iterator it_v;   

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
			if ( itAlDemanda->getPrioridade() != prioridade )
				continue;
			
			if ( problemData->retornaTurmaDiscAluno( aluno, itAlDemanda->demanda->disciplina ) != -1 )
				continue;

			// Demanda ainda não atendida

			// Pula disciplina pratica, pq a referencia para as equivalentes é informada pelas teoricas
			if ( itAlDemanda->demanda->disciplina->getId() < 0 )
				continue;

			Disciplina *discTeorica = itAlDemanda->demanda->disciplina;

			ITERA_GGROUP_LESSPTR( itDisc, discTeorica->discEquivSubstitutas, Disciplina )
			{								
				// Pula disciplina pratica
				if ( itDisc->getId() < 0 )
					continue;
				
				Disciplina *discTeorica = *itDisc;

				if ( problemData->refDisciplinas.find( - discTeorica->getId() ) ==
					 problemData->refDisciplinas.end() ) // Restrição somente para disciplinas de creditos praticos+teoricos
					continue;

				Disciplina *discPratica = problemData->refDisciplinas[ - discTeorica->getId() ];

				c.reset();
				c.setType( ConstraintTatInt::C_ALUNO_DISC_PRATICA_TEORICA_EQUIV );
				c.setAluno( aluno );
				c.setDisciplina( discPratica );
				c.setCampus( campus );

				sprintf( name, "%s", c.toString().c_str() ); 

				if ( cHashTatico.find( c ) != cHashTatico.end() )
				{
					continue;
				}

				nnz = discPratica->getNumTurmas() + discTeorica->getNumTurmas();

				OPT_ROW row( nnz, OPT_ROW::EQUAL , 0.0 , name );

				for ( int turma = 0; turma < discPratica->getNumTurmas(); turma++ )
				{
					VariableTatInt v;
					v.reset();
					v.setType( VariableTatInt::V_ALOCA_ALUNO_TURMA_DISC );
					v.setAluno( aluno );
					v.setDisciplina( discPratica );
					v.setTurma( turma );
					v.setCampus( campus );

					it_v = vHashTatico.find( v );
					if( it_v != vHashTatico.end() )
					{
						row.insert( it_v->second, 1.0 );
					}
				}

				for ( int turma = 0; turma < discTeorica->getNumTurmas(); turma++ )
				{
					VariableTatInt v;
					v.reset();
					v.setType( VariableTatInt::V_ALOCA_ALUNO_TURMA_DISC );
					v.setAluno( aluno );
					v.setDisciplina( discTeorica );
					v.setTurma( turma );
					v.setCampus( campus );

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
   char name[ 200 ];

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
			sprintf( name, "%s", c.toString().c_str() );
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
   char name[ 200 ];

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

			coef = disc->getCalendario()->retornaNroCreditosEntreHorarios(hi, hf);
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
			sprintf( name, "%s", c.toString().c_str() );
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