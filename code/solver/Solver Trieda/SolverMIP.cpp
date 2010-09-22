#include "opt_cplex.h"
#include "ProblemData.h"
#include "ProblemSolution.h"
#include "SolverMIP.h"

SolverMIP::SolverMIP(ProblemData *aProblemData)
:Solver(aProblemData)
{
	alpha = beta = gamma = delta = lambda = M = 1.0;
	try
	{
		lp = new OPT_CPLEX;
	}
	catch(...)
	{
	}

	solVars.clear();
}

SolverMIP::~SolverMIP()
{
	int i;

	if (lp != NULL) delete lp;

	for (i=0; i < (int)solVars.size(); i++)
	{
		if ( solVars[i] != NULL )
			delete solVars[i];
	}
	solVars.clear();
}

int SolverMIP::solve()
{
	int varNum = 0;
	int constNum = 0;

	lp->createLP("Solver Trieda", OPTSENSE_MINIMIZE, PROB_MIP);

#ifdef DEBUG
	printf("Creating LP...\n");
#endif

	/* Variable creation */
	varNum = cria_variaveis();

#ifdef DEBUG
	printf("Total of Variables: %i\n\n",varNum);
#endif

	/* Constraint creation */
	constNum = cria_restricoes();

#ifdef DEBUG
	printf("Total of Constraints: %i\n",constNum);
#endif

#ifdef DEBUG
	lp->writeProbLP("Solver Trieda");
#endif

	int status = lp->optimize(METHOD_MIP);

	return status;
}


void SolverMIP::getSolution(ProblemSolution *problemSolution)
{
	double *xSol = NULL;
	VariableHash::iterator vit;

	xSol = new double[lp->getNumCols()];
	lp->getX(xSol);

	vit = vHash.begin();

#ifdef DEBUG
	FILE *fout = fopen("solucao.txt","wt");
#endif

	while (vit != vHash.end())
	{
		Variable *v = new Variable(vit->first);
		int col = vit->second;

		v->setValue( xSol[col] );

		if ( v->getValue() > 0.00001 )
		{
#ifdef DEBUG
			char auxName[100];
			lp->getColName(auxName,col,100);
			fprintf(fout,"%s = %f\n",auxName,v->getValue());
#endif
			/**
			ToDo:
			*/
		}

		vit++;
	}

	// Fill the solution

#ifdef DEBUG
	if ( fout )
		fclose(fout);
#endif

	if ( xSol )
		delete[] xSol;
}


int SolverMIP::cria_variaveis(void)
{
	int num_vars = 0;
	num_vars += cria_variavel_oferecimentos();
	num_vars += cria_variavel_creditos();
	num_vars += cria_variavel_abertura();
	num_vars += cria_variavel_alunos();
	num_vars += cria_variavel_consecutivos();
	num_vars += cria_variavel_max_creds();
	num_vars += cria_variavel_min_creds();
	num_vars += cria_variavel_aloc_disciplina();
	num_vars += cria_variavel_num_subblocos();
	num_vars += cria_variavel_num_abertura_turma_bloco();

	return num_vars;
}

int SolverMIP::cria_variavel_creditos(void)
{
	int num_vars = 0;

	//ITERA_GGROUP(it_unidades,problemData->unidades,Unidade)
	ITERA_GGROUP(it_campus,problemData->campi,Campus)
	{
		ITERA_GGROUP(it_unidades,it_campus->unidades,Unidade)
		{
			ITERA_GGROUP(it_salas,it_unidades->salas,Sala) 
			{
				ITERA_GGROUP(it_disc,problemData->disciplinas,Disciplina)
				{
					//ITERA_GGROUP(it_turma,it_disc->turmas,Turma)
					for(int turma=0;turma<it_disc->num_turmas;turma++)
					{
						for(int dia=0;dia<7;dia++)
						{
							Variable v;
							v.reset();
							v.setType(Variable::V_CREDITOS);

							//v.setTurma(*it_turma);
							v.setTurma(turma);            // i
							v.setDisciplina(*it_disc);    // d
							v.setUnidade(*it_unidades);   // u
							v.setSala(*it_salas);         // s  
							v.setDia(dia);                // t

							if (vHash.find(v) == vHash.end())
							{
								vHash[v] = lp->getNumCols();

								OPT_COL col(OPT_COL::VAR_INTEGRAL,0.0,0.0,24.0,
									(char*)v.toString().c_str());

								lp->newCol(col);

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

int SolverMIP::cria_variavel_oferecimentos(void)
{
	int num_vars = 0;

	ITERA_GGROUP(it_campus,problemData->campi,Campus)
	{
		ITERA_GGROUP(it_unidades,it_campus->unidades,Unidade)
		{
			ITERA_GGROUP(it_salas,it_unidades->salas,Sala) 
			{
				ITERA_GGROUP(it_disc,problemData->disciplinas,Disciplina)
				{
					for(int turma=0;turma<it_disc->num_turmas;turma++)
					{
						for(int dia=0;dia<7;dia++)
						{
							Variable v;
							v.reset();
							v.setType(Variable::V_OFERECIMENTO);

							v.setTurma(turma);            // i
							v.setDisciplina(*it_disc);    // d
							v.setUnidade(*it_unidades);   // u
							v.setSala(*it_salas);         // s  
							v.setDia(dia);                // t

							if (vHash.find(v) == vHash.end())
							{
								vHash[v] = lp->getNumCols();

								OPT_COL col(OPT_COL::VAR_BINARY,0.0,0.0,1.0,
									(char*)v.toString().c_str());

								lp->newCol(col);

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

int SolverMIP::cria_variavel_abertura(void)
{
	int num_vars = 0;

	/*
	Pode ser implementado de uma maneira melhor, listando apenas as disciplinas que podem
	ser abertas em um campus (atraves do OFERTACURSO) e criando as suas respectivas variaveis.
	Desse modo, variaveis desnecess�rias (relacionadas � disciplinas que n�o existem em outros campus)
	seriam evitadas.

	VER <demandas_campus> em <ProblemData>
	*/

	ITERA_GGROUP(it_campus,problemData->campi,Campus)
	{
		ITERA_GGROUP(it_disc,problemData->disciplinas,Disciplina)
		{
			for(int turma=0;turma<it_disc->num_turmas;turma++)
			{
				Variable v;
				v.reset();
				v.setType(Variable::V_ABERTURA);

				v.setTurma(turma);            // i
				v.setDisciplina(*it_disc);    // d
				v.setCampus(*it_campus);	  // cp

				std::pair<int,int> dc = std::make_pair
					(it_disc->getId(),it_campus->getId());


				if(problemData->demandas_campus.find(dc) ==	problemData->demandas_campus.end())
				{
					problemData->demandas_campus[dc] = 0;
				}

				double ratioDem = ( it_disc->demanda_total - 
					problemData->demandas_campus[dc] ) 
					/ (1.0 * it_disc->demanda_total);

				double coeff = alpha + gamma * ratioDem;

				if (vHash.find(v) == vHash.end())
				{
					lp->getNumCols();
					vHash[v] = lp->getNumCols();

					OPT_COL col(OPT_COL::VAR_BINARY,coeff,0.0,1.0,
						(char*)v.toString().c_str());

					lp->newCol(col);

					num_vars += 1;
				}
			}
		}
	}

	/***
	ITERA_GGROUP(it_campus,problemData->campi,Campus)
	{
	ITERA_GGROUP(it_unidades,it_campus->unidades,Unidade)
	{
	ITERA_GGROUP(it_disc,problemData->disciplinas,Disciplina)
	{
	for(int turma=0;turma<it_disc->num_turmas;turma++)
	{
	Variable v;
	v.reset();
	v.setType(Variable::V_ABERTURA);

	v.setTurma(turma);            // i
	v.setDisciplina(*it_disc);    // d
	v.setUnidade(*it_unidades);   // u

	std::pair<int,int> dc = std::make_pair
	(it_disc->getId(),it_campus->getId());

	if(problemData->demandas_campus.find(dc) !=
	problemData->demandas_campus.end())
	problemData->demandas_campus[dc] = 0;



	double ratioDem = ( it_disc->demanda_total - 
	problemData->demandas_campus[dc] ) 
	/ (1.0 * it_disc->demanda_total);
	// = ( max_demanda - demanda_unidade ) / max_demanda;

	double coeff = alpha + gamma * ratioDem;

	if (vHash.find(v) == vHash.end())
	{
	lp->getNumCols();
	vHash[v] = lp->getNumCols();

	OPT_COL col(OPT_COL::VAR_BINARY,coeff,0.0,1.0,
	(char*)v.toString().c_str());

	lp->newCol(col);

	num_vars += 1;
	}
	}
	}
	}
	}
	***/

	return num_vars;
}

int SolverMIP::cria_variavel_alunos(void)
{
	int num_vars = 0;

	ITERA_GGROUP(it_campus,problemData->campi,Campus)
	{
		ITERA_GGROUP(it_disc,problemData->disciplinas,Disciplina)
		{
			for(int turma=0;turma<it_disc->num_turmas;turma++)
			{
				ITERA_GGROUP(it_cursos,problemData->cursos,Curso)
				{
					Variable v;
					v.reset();
					v.setType(Variable::V_ALUNOS);

					v.setTurma(turma);            // i
					v.setDisciplina(*it_disc);    // d
					v.setCampus(*it_campus);		 // cp
					v.setCurso(*it_cursos);       // c

					if (vHash.find(v) == vHash.end())
					{
						vHash[v] = lp->getNumCols();

						OPT_COL col(OPT_COL::VAR_INTEGRAL,0.0,0.0,1000.0,
							(char*)v.toString().c_str());

						lp->newCol(col);

						num_vars += 1;
					}

				}
			}
		}
	}

	/***
	ITERA_GGROUP(it_campus,problemData->campi,Campus)
	{
	ITERA_GGROUP(it_unidades,it_campus->unidades,Unidade)
	{
	ITERA_GGROUP(it_disc,problemData->disciplinas,Disciplina)
	{
	for(int turma=0;turma<it_disc->num_turmas;turma++)
	{
	ITERA_GGROUP(it_cursos,problemData->cursos,Curso)
	{
	Variable v;
	v.reset();
	v.setType(Variable::V_ALUNOS);

	v.setTurma(turma);            // i
	v.setDisciplina(*it_disc);    // d
	v.setUnidade(*it_unidades);   // u
	v.setCurso(*it_cursos);       // c

	if (vHash.find(v) == vHash.end())
	{
	vHash[v] = lp->getNumCols();

	OPT_COL col(OPT_COL::VAR_INTEGRAL,0.0,0.0,1000.0,
	(char*)v.toString().c_str());

	lp->newCol(col);

	num_vars += 1;
	}
	}
	}
	}
	}
	}
	***/
	return num_vars;
}

int SolverMIP::cria_variavel_aloc_alunos(void)
{
	int num_vars = 0;

	ITERA_GGROUP(it_campus,problemData->campi,Campus)
	{
		ITERA_GGROUP(it_disc,problemData->disciplinas,Disciplina)
		{
			for(int turma=0;turma<it_disc->num_turmas;turma++)
			{
				ITERA_GGROUP(it_cursos,problemData->cursos,Curso)
				{
					Variable v;
					v.reset();
					v.setType(Variable::V_ALOC_ALUNO);

					v.setTurma(turma);            // i
					v.setDisciplina(*it_disc);    // d
					v.setCampus(*it_campus);		 // cp
					v.setCurso(*it_cursos);       // c

					if (vHash.find(v) == vHash.end())
					{
						vHash[v] = lp->getNumCols();

						OPT_COL col(OPT_COL::VAR_BINARY,0,0.0,1.0,
							(char*)v.toString().c_str());

						lp->newCol(col);

						num_vars += 1;
					}
				}

			}
		}
	}

	/***
	ITERA_GGROUP(it_campus,problemData->campi,Campus)
	{
	ITERA_GGROUP(it_unidades,it_campus->unidades,Unidade)
	{
	ITERA_GGROUP(it_disc,problemData->disciplinas,Disciplina)
	{
	for(int turma=0;turma<it_disc->num_turmas;turma++)
	{
	ITERA_GGROUP(it_cursos,problemData->cursos,Curso)
	{
	Variable v;
	v.reset();
	v.setType(Variable::V_ALOC_ALUNO);

	v.setTurma(turma);            // i
	v.setDisciplina(*it_disc);    // d
	v.setUnidade(*it_unidades);   // u
	v.setCurso(*it_cursos);       // c

	if (vHash.find(v) == vHash.end())
	{
	vHash[v] = lp->getNumCols();

	OPT_COL col(OPT_COL::VAR_BINARY,0,0.0,1.0,
	(char*)v.toString().c_str());

	lp->newCol(col);

	num_vars += 1;
	}
	}
	}
	}
	}
	}
	***/
	return num_vars;

}

int SolverMIP::cria_variavel_consecutivos(void)
{
	int num_vars = 0;

	ITERA_GGROUP(it_disc,problemData->disciplinas,Disciplina)
	{
		for(int turma=0;turma<it_disc->num_turmas;turma++)
		{
			for(int dia=1;dia<7;dia++) 
			{
				Variable v;
				v.reset();
				v.setType(Variable::V_DIAS_CONSECUTIVOS);

				v.setTurma(turma);            // i
				v.setDisciplina(*it_disc);    // d
				v.setDia(dia);                // t

				if (vHash.find(v) == vHash.end())
				{
					vHash[v] = lp->getNumCols();

					OPT_COL col(OPT_COL::VAR_BINARY,delta,0.0,1.0,
						(char*)v.toString().c_str());

					lp->newCol(col);

					num_vars += 1;
				}
			}
		}
	}
	return num_vars;
}
int SolverMIP::cria_variavel_min_creds(void)
{
	int num_vars = 0;
	ITERA_GGROUP(it_bloco,problemData->blocos,BlocoCurricular)
	{
		ITERA_GGROUP(it_disc,it_bloco->disciplinas,Disciplina)
		{
			for(int i=0;i<it_disc->num_turmas;i++)
			{
				Variable v;
				v.reset();
				v.setType(Variable::V_MIN_CRED_SEMANA);
				v.setTurma(i);
				v.setBloco(*it_bloco);

				if (vHash.find(v) == vHash.end())
				{
					vHash[v] = lp->getNumCols();

					OPT_COL col(OPT_COL::VAR_INTEGRAL,-lambda,0.0,1000.0,
						(char*)v.toString().c_str());

					lp->newCol(col);

					num_vars += 1;
				}
			}
		}
	}
	return num_vars;
}

int SolverMIP::cria_variavel_max_creds(void)
{
	int num_vars = 0;
	ITERA_GGROUP(it_bloco,problemData->blocos,BlocoCurricular)
	{
		ITERA_GGROUP(it_disc,it_bloco->disciplinas,Disciplina)
		{
			for(int i=0;i<it_disc->num_turmas;i++)
			{
				Variable v;
				v.reset();
				v.setType(Variable::V_MAX_CRED_SEMANA);
				v.setTurma(i);
				v.setBloco(*it_bloco);

				if (vHash.find(v) == vHash.end())
				{
					vHash[v] = lp->getNumCols();

					OPT_COL col(OPT_COL::VAR_INTEGRAL,lambda,0.0,1000.0,
						(char*)v.toString().c_str());

					lp->newCol(col);

					num_vars += 1;
				}
			}
		}
	}
	return num_vars;
}

int SolverMIP::cria_variavel_aloc_disciplina(void)
{
	int num_vars = 0;

	ITERA_GGROUP(it_campus,problemData->campi,Campus)
	{
		ITERA_GGROUP(it_unidades,it_campus->unidades,Unidade)
		{
			ITERA_GGROUP(it_salas,it_unidades->salas,Sala) 
			{
				ITERA_GGROUP(it_disc,problemData->disciplinas,Disciplina)
				{
					for(int turma=0;turma<it_disc->num_turmas;turma++)
					{
						Variable v;
						v.reset();
						v.setType(Variable::V_ALOC_DISCIPLINA);

						v.setUnidade(*it_unidades);   // u
						v.setSala(*it_salas);         // s  
						v.setTurma(turma);            // i
						v.setDisciplina(*it_disc);    // d

						if (vHash.find(v) == vHash.end())
						{
							vHash[v] = lp->getNumCols();

							OPT_COL col(OPT_COL::VAR_BINARY,0.0,0.0,1.0,
								(char*)v.toString().c_str());

							lp->newCol(col);

							num_vars += 1;
						}

					}
				}
			}
		}
	}

	return num_vars;
}

int SolverMIP::cria_variavel_num_subblocos(void)
{
	int num_vars = 0;

	ITERA_GGROUP(it_bloco,problemData->blocos,BlocoCurricular)
	{
		for(int dia=0;dia<7;dia++)
		{
			Variable v;
			v.reset();

			v.setType(Variable::V_N_SUBBLOCOS);
			v.setBloco(*it_bloco);
			v.setDia(dia);
			v.setCampus(it_bloco->campus);

			if (vHash.find(v) == vHash.end())
			{
				vHash[v] = lp->getNumCols();

				OPT_COL col(OPT_COL::VAR_INTEGRAL,rho,0.0,0.0,
					(char*)v.toString().c_str());

				lp->newCol(col);

				num_vars += 1;
			}
		}
	}

	/***
	ITERA_GGROUP(it_unidades,problemData->unidades,Unidade)
	{
	ITERA_GGROUP(it_bloco,problemData->blocos,BlocoCurricular)
	{
	for(int dia=0;dia<7;dia++)
	{
	Variable v;
	v.reset();
	v.setType(Variable::V_TURMA_BLOCO);
	v.setBloco(*it_bloco);
	v.setDia(dia);
	v.setUnidade(*it_unidades);

	if (vHash.find(v) == vHash.end())
	{
	vHash[v] = lp->getNumCols();

	// PERGUNTAR PRO MARCELO OU PRO ANDRE
	OPT_COL col(OPT_COL::VAR_BINARY,beta,0.0,1.0,
	(char*)v.toString().c_str());

	lp->newCol(col);

	num_vars += 1;
	}
	}
	}
	}
	***/

	return num_vars;
}


int SolverMIP::cria_variavel_num_abertura_turma_bloco(void)
{
	int num_vars = 0;

	ITERA_GGROUP(it_bloco,problemData->blocos,BlocoCurricular)
	{
		for(int dia=0;dia<7;dia++)
		{
			Variable v;
			v.reset();

			v.setType(Variable::V_N_ABERT_TURMA_BLOCO);
			v.setBloco(*it_bloco);
			v.setDia(dia);

			if (vHash.find(v) == vHash.end())
			{
				vHash[v] = lp->getNumCols();

				OPT_COL col(OPT_COL::VAR_INTEGRAL,beta,0.0,0.0,
					(char*)v.toString().c_str());

				lp->newCol(col);

				num_vars += 1;
			}

		}
	}

	return num_vars;
}




// ==============================================================
//							CONSTRAINTS
// ==============================================================

/*
int SolverMIP::cria_restricoes(void) {
int restricoes = 0;

restricoes += cria_restricao_carga();
restricoes += cria_restricao_max_creditos_sd();
restricoes += cria_restricao_min_creditos();
restricoes += cria_restricao_ativacao();
restricoes += cria_restricao_sobreposicao();
restricoes += cria_restricao_mesma_unidade();
restricoes += cria_restricao_max_creditos();
restricoes += cria_restricao_turmas_bloco();
restricoes += cria_restricao_cap_demanda();
restricoes += cria_restricao_cap_sala();
restricoes += cria_restricao_cap_sala_unidade();
restricoes += cria_restricao_dias_consecutivos();

return restricoes;
}
*/


int SolverMIP::cria_restricoes(void)
{
	int restricoes = 0;

	restricoes += cria_restricao_carga_horaria();				// Restricao 1.2.3
	restricoes += cria_restricao_max_cred_sd();					// Restricao 1.2.4
	restricoes += cria_restricao_min_cred_dd();					// Restricao 1.2.5
	restricoes += cria_restricao_ativacao();					// Restricao 1.2.6
	restricoes += cria_restricao_evita_sobreposicao();			// Restricao 1.2.7
	restricoes += cria_restricao_disciplina_sala();				// Restricao 1.2.8
	restricoes += cria_restricao_turma_sala();					// Restricao 1.2.9
	restricoes += cria_restricao_evita_turma_disc_camp_d();		// Restricao 1.2.10
	restricoes += cria_restricao_turmas_bloco();				// Restricao 1.2.11
	restricoes += cria_restricao_max_cred_disc_bloco();			// Restricao 1.2.12
	restricoes += cria_restricao_num_tur_bloc_dia_difunid();	// Restricao 1.2.13
	restricoes += cria_restricao_lim_cred_diar_disc();			// Restricao 1.2.14
	restricoes += cria_restricao_cap_aloc_dem_disc();			// Restricao 1.2.15
	restricoes += cria_restricao_cap_sala_compativel_turma();	// Restricao 1.2.16
	restricoes += cria_restricao_cap_sala_unidade();			// Restricao 1.2.17
	restricoes += cria_restricao_turma_disc_dias_consec();		// Restricao 1.2.18
	restricoes += cria_restricao_min_creds_turm_bloco();		// Restricao 1.2.19
	restricoes += cria_restricao_max_creds_turm_bloco();		// Restricao 1.2.20
	restricoes += cria_restricao_aluno_curso_disc();			// Restricao 1.2.21
	restricoes += cria_restricao_alunos_cursos_dif();			// Restricao 1.2.22

	return restricoes;
}

int SolverMIP::cria_restricao_carga_horaria(void)
{
	int restricoes = 0;

	char name[100];
	int nnz;

	Constraint c;

	Variable v;
	VariableHash::iterator it_v;

	ITERA_GGROUP(it_campus,problemData->campi,Campus) {
		ITERA_GGROUP(it_disciplina,problemData->disciplinas,Disciplina) {
			for(int i=0;i<it_disciplina->num_turmas;i++) {

				c.reset();
				c.setType(Constraint::C_CARGA_HORARIA);

				c.setCampus(*it_campus);
				c.setTurma(i);
				c.setDisciplina(*it_disciplina);

				sprintf( name, "%s", c.toString().c_str() );

				if (cHash.find(c) != cHash.end()) continue;

				cHash[ c ] = lp->getNumRows();

				int numSalasCampus = 0;

				ITERA_GGROUP(it_unidade,it_campus->unidades,Unidade) {
					numSalasCampus += it_unidade->salas.size();
				}

				nnz = numSalasCampus * 7;

				OPT_ROW row( nnz + 1, OPT_ROW::EQUAL , 0, name );

				v.reset();
				v.setType(Variable::V_CREDITOS);

				// ---

				ITERA_GGROUP(it_unidade,it_campus->unidades,Unidade) {
					ITERA_GGROUP(it_sala,it_unidade->salas,Sala) {
						for(int dia=0;dia<7;dia++) {
							v.setUnidade(*it_unidade);
							v.setSala(*it_sala);

							v.setTurma(i);
							v.setDisciplina(*it_disciplina);

							v.setDia(dia);

							it_v = vHash.find(v);
							if( it_v != vHash.end() ){
								row.insert(it_v->second, 1.0);

							}
						}
					}
				}

				// ---

				v.reset();
				v.setType(Variable::V_ABERTURA);

				v.setCampus(*it_campus);
				v.setTurma(i);
				v.setDisciplina(*it_disciplina);

				it_v = vHash.find(v);
				if( it_v != vHash.end() ) {
					row.insert(it_v->second, 
						-(it_disciplina->cred_praticos + 
						it_disciplina->cred_teoricos));
				}

				// ---

				lp->addRow(row);
				restricoes++;
			}
		}
	}

	return restricoes;
}

int SolverMIP::cria_restricao_max_cred_sd(void)
{
	int restricoes = 0;

	char name[100];
	int nnz;
	Constraint c;
	Variable v;
	VariableHash::iterator it_v;

	ITERA_GGROUP(it_campus,problemData->campi,Campus) {

		ITERA_GGROUP(it_unidade,it_campus->unidades,Unidade) {
			ITERA_GGROUP(it_sala,it_unidade->salas,Sala) {
				for(int dia=0;dia<7;dia++) {
					c.reset();
					c.setType(Constraint::C_MAX_CREDITOS_SD);

					c.setUnidade(*it_unidade);
					c.setSala(*it_sala);
					c.setDia(dia);

					sprintf( name, "%s", c.toString().c_str() );

					if (cHash.find(c) != cHash.end()) continue;

					cHash[ c ] = lp->getNumRows();

					nnz = 0;
					ITERA_GGROUP(it_disc,problemData->disciplinas,Disciplina) {
						nnz += it_disc->num_turmas;
					}

					int max_creds = it_sala->max_creds(dia);

					OPT_ROW row( nnz, OPT_ROW::LESS , max_creds, name );

					ITERA_GGROUP(it_disc,problemData->disciplinas,Disciplina) {
						for(int i=0;i<it_disc->num_turmas;i++) {

							v.reset();
							v.setType(Variable::V_CREDITOS);

							v.setUnidade(*it_unidade);
							v.setSala(*it_sala);

							v.setDisciplina(*it_disc);
							v.setTurma(i);

							v.setDia(dia);

							it_v = vHash.find(v);
							if( it_v != vHash.end() )
							{
								row.insert(it_v->second, 1.0);
							}
						}
					}

					lp->addRow(row);
					restricoes++;
				}
			}
		}
	}

	return restricoes;
}

int SolverMIP::cria_restricao_min_cred_dd(void)
{
	int restricoes = 0;
	return restricoes;
}

int SolverMIP::cria_restricao_ativacao(void)
{
	int restricoes = 0;
	char name[100];
	int nnz;

	Constraint c;

	Variable v;
	VariableHash::iterator it_v;

	ITERA_GGROUP(it_campus,problemData->campi,Campus) {

		ITERA_GGROUP(it_unidade,it_campus->unidades,Unidade) {
			ITERA_GGROUP(it_sala,it_unidade->salas,Sala) {
				for(int dia=0;dia<7;dia++) {
					ITERA_GGROUP(it_disc,problemData->disciplinas,Disciplina) {
						for(int i=0;i<it_disc->num_turmas;i++) {

							c.reset();
							c.setType(Constraint::C_VAR_O);

							c.setUnidade(*it_unidade);
							c.setSala(*it_sala);

							c.setDisciplina(*it_disc);
							c.setTurma(i);

							c.setDia(dia);

							sprintf( name, "%s", c.toString().c_str() ); 

							if (cHash.find(c) != cHash.end()) continue;

							cHash[ c ] = lp->getNumRows();

							nnz = 2;

							OPT_ROW row( nnz, OPT_ROW::GREATER , 0.0, name );

							v.reset();
							v.setType(Variable::V_OFERECIMENTO);

							v.setUnidade(*it_unidade);
							v.setSala(*it_sala);

							v.setDisciplina(*it_disc);
							v.setTurma(i);

							v.setDia(dia);

							it_v = vHash.find(v);
							if( it_v != vHash.end() )
							{
								row.insert(it_v->second, (it_disc->cred_praticos + 
									it_disc->cred_teoricos));
							}

							v.reset();
							v.setType(Variable::V_CREDITOS);

							v.setUnidade(*it_unidade);
							v.setSala(*it_sala);

							v.setDisciplina(*it_disc);
							v.setTurma(i);

							v.setDia(dia);

							it_v = vHash.find(v);
							if( it_v != vHash.end() )
							{
								row.insert(it_v->second, -1.0);
							}

							lp->addRow(row);
							restricoes++;
						}
					}
				}
			}
		}
	}

	return restricoes;
}

int SolverMIP::cria_restricao_evita_sobreposicao(void)
{
	int restricoes = 0;

	char name[100];
	int nnz;
	Constraint c;
	Variable v;
	VariableHash::iterator it_v;

	ITERA_GGROUP(it_campus,problemData->campi,Campus) {

		ITERA_GGROUP(it_unidade,it_campus->unidades,Unidade) {
			for(int dia=0;dia<7;dia++) {
				ITERA_GGROUP(it_disc,problemData->disciplinas,Disciplina) {
					for(int i=0;i<it_disc->num_turmas;i++) {

						c.reset();
						c.setType(Constraint::C_EVITA_SOBREPOSICAO_TD);

						c.setUnidade(*it_unidade);
						//c.setSala(*it_sala);
						c.setDia(dia);
						c.setDisciplina(*it_disc);
						c.setTurma(i);

						sprintf( name, "%s", c.toString().c_str() ); 

						if (cHash.find(c) != cHash.end()) continue;

						cHash[ c ] = lp->getNumRows();

						nnz = it_unidade->salas.size();

						/*
						int creds = it_disc->cred_praticos + 
						it_disc->cred_teoricos;
						*/

						OPT_ROW row( nnz, OPT_ROW::LESS , 1.0, name );

						ITERA_GGROUP(it_sala,it_unidade->salas,Sala) 
						{

							v.reset();
							v.setType(Variable::V_OFERECIMENTO);

							v.setUnidade(*it_unidade);
							v.setSala(*it_sala);

							v.setDisciplina(*it_disc);
							v.setTurma(i);

							v.setDia(dia);

							it_v = vHash.find(v);
							if( it_v != vHash.end() )
							{
								//row.insert(it_v->second, creds);
								row.insert(it_v->second, 1.0);
							}
						}

						lp->addRow(row);
						restricoes++;
					}
				}
			}
		}
	}

	return restricoes;
}

int SolverMIP::cria_restricao_disciplina_sala(void)
{
	int restricoes = 0;
	return restricoes;
}

int SolverMIP::cria_restricao_turma_sala(void)
{
	int restricoes = 0;
	return restricoes;
}

int SolverMIP::cria_restricao_evita_turma_disc_camp_d(void)
{
	int restricoes = 0;
	return restricoes;
}

int SolverMIP::cria_restricao_turmas_bloco(void)
{	
	int restricoes = 0;
	return restricoes;
}

int SolverMIP::cria_restricao_max_cred_disc_bloco(void)
{
	int restricoes = 0;
	return restricoes;
}

int SolverMIP::cria_restricao_num_tur_bloc_dia_difunid(void)
{
	int restricoes = 0;
	return restricoes;
}

int SolverMIP::cria_restricao_lim_cred_diar_disc(void)
{
	int restricoes = 0;
	return restricoes;
}

int SolverMIP::cria_restricao_cap_aloc_dem_disc(void)
{
	int restricoes = 0;
	return restricoes;
}

int SolverMIP::cria_restricao_cap_sala_compativel_turma(void)
{
	int restricoes = 0;
	return restricoes;
}

int SolverMIP::cria_restricao_cap_sala_unidade(void)
{
	int restricoes = 0;
	return restricoes;
}

int SolverMIP::cria_restricao_turma_disc_dias_consec(void)
{
	int restricoes = 0;
	return restricoes;
}

int SolverMIP::cria_restricao_min_creds_turm_bloco(void)
{
	int restricoes = 0;
	return restricoes;
}

int SolverMIP::cria_restricao_max_creds_turm_bloco(void)
{
	int restricoes = 0;
	return restricoes;
}

int SolverMIP::cria_restricao_aluno_curso_disc(void)
{
	int restricoes = 0;
	return restricoes;
}

int SolverMIP::cria_restricao_alunos_cursos_dif(void)
{
	int restricoes = 0;
	return restricoes;
}



/*
int SolverMIP::cria_restricao_carga(void)
{
int restricoes = 0;
char name[100];
int nnz;
Constraint c;
Variable v;
VariableHash::iterator it_v;

ITERA_GGROUP(it_unidade,problemData->unidades,Unidade) {
ITERA_GGROUP(it_disc,problemData->disciplinas,Disciplina) {
ITERA_GGROUP(it_turma,it_disc->turmas,Turma) {

c.reset();
c.setType(Constraint::C_CARGA_HORARIA);
c.setUnidade(*it_unidade);
c.setDisciplina(*it_disc);
c.setTurma(*it_turma);

sprintf( name, "%s", c.toString().c_str() ); 

if (cHash.find(c) != cHash.end()) continue;

cHash[ c ] = lp->getNumRows();

nnz = it_unidade->salas.size() * 7;
int creditos = it_disc->cred_praticos + 
it_disc->cred_teoricos;

OPT_ROW row( nnz + 1, OPT_ROW::EQUAL , 0, name );

v.reset();
v.setType(Variable::V_ABERTURA);

v.setTurma(*it_turma);
v.setDisciplina(*it_disc);
v.setUnidade(*it_unidade);

it_v = vHash.find(v);
if( it_v != vHash.end() )
{
row.insert(it_v->second, -creditos);
}

ITERA_GGROUP(it_sala,it_unidade->salas,Sala) {
for(int dia=0;dia<7;dia++) {

v.reset();
v.setType(Variable::V_CREDITOS);

v.setTurma(*it_turma);
v.setDisciplina(*it_disc);
v.setUnidade(*it_unidade);
v.setSala(*it_sala);
v.setDia(dia);

it_v = vHash.find(v);
if( it_v != vHash.end() )
{
row.insert(it_v->second, 1.0);
}
}
}
lp->addRow(row);
restricoes++;
}
}
}

return restricoes;
}

int SolverMIP::cria_restricao_max_creditos_sd(void)
{
int restricoes = 0;
char name[100];
int nnz;
Constraint c;
Variable v;
VariableHash::iterator it_v;

ITERA_GGROUP(it_unidade,problemData->unidades,Unidade) 
{
ITERA_GGROUP(it_sala,it_unidade->salas,Sala) 
{
for(int dia=0;dia<7;dia++) 
{
c.reset();
c.setType(Constraint::C_MAX_CREDITOS_SD);

c.setUnidade(*it_unidade);
c.setSala(*it_sala);
c.setDia(dia);

sprintf( name, "%s", c.toString().c_str() ); 

if (cHash.find(c) != cHash.end()) continue;

cHash[ c ] = lp->getNumRows();

nnz = 0;
ITERA_GGROUP(it_disc,problemData->disciplinas,Disciplina)
nnz += it_disc->turmas.size();

int max_creds = 99999999; //TODO, achar max_creds
OPT_ROW row( nnz, OPT_ROW::LESS , max_creds, name );

ITERA_GGROUP(it_disc,problemData->disciplinas,Disciplina) {
ITERA_GGROUP(it_turma,it_disc->turmas,Turma) {

v.reset();
v.setType(Variable::V_CREDITOS);

v.setTurma(*it_turma);
v.setDisciplina(*it_disc);
v.setUnidade(*it_unidade);
v.setSala(*it_sala);
v.setDia(dia);

it_v = vHash.find(v);
if( it_v != vHash.end() )
{
row.insert(it_v->second, 1.0);
}
}
}
lp->addRow(row);
restricoes++;

}
}
}
return restricoes;
}

int SolverMIP::cria_restricao_min_creditos(void)
{
int restricoes = 0;
char name[100];
int nnz;
Constraint c;
Variable v;
VariableHash::iterator it_v;

ITERA_GGROUP(it_unidade,problemData->unidades,Unidade) 
{
ITERA_GGROUP(it_sala,it_unidade->salas,Sala) 
{
for(int dia=0;dia<7;dia++) 
{
ITERA_GGROUP(it_disc,problemData->disciplinas,Disciplina) 
{
ITERA_GGROUP(it_turma,it_disc->turmas,Turma) 
{

c.reset();
c.setType(Constraint::C_MIN_CREDITOS);

c.setUnidade(*it_unidade);
c.setSala(*it_sala);
c.setDia(dia);
c.setDisciplina(*it_disc);
c.setTurma(*it_turma);

sprintf( name, "%s", c.toString().c_str() ); 

if (cHash.find(c) != cHash.end()) continue;

cHash[ c ] = lp->getNumRows();

nnz = 2;

int min_creds = 0; //TODO, achar min_creds

OPT_ROW row( nnz, OPT_ROW::LESS , 0.0, name );

v.reset();
v.setType(Variable::V_OFERECIMENTO);

v.setTurma(*it_turma);
v.setDisciplina(*it_disc);
v.setUnidade(*it_unidade);
v.setSala(*it_sala);
v.setDia(dia);

it_v = vHash.find(v);
if( it_v != vHash.end() )
{
row.insert(it_v->second, min_creds);
}

v.reset();
v.setType(Variable::V_CREDITOS);

v.setTurma(*it_turma);
v.setDisciplina(*it_disc);
v.setUnidade(*it_unidade);
v.setSala(*it_sala);
v.setDia(dia);

it_v = vHash.find(v);
if( it_v != vHash.end() )
{
row.insert(it_v->second, -1.0);
}

lp->addRow(row);
restricoes++;
}
}
}
}
}
return restricoes;
}

int SolverMIP::cria_restricao_ativacao(void)
{
int restricoes = 0;
char name[100];
int nnz;
Constraint c;
Variable v;
VariableHash::iterator it_v;

ITERA_GGROUP(it_unidade,problemData->unidades,Unidade) 
{
ITERA_GGROUP(it_sala,it_unidade->salas,Sala) 
{
for(int dia=0;dia<7;dia++) 
{
ITERA_GGROUP(it_disc,problemData->disciplinas,Disciplina) 
{
ITERA_GGROUP(it_turma,it_disc->turmas,Turma) 
{

c.reset();
c.setType(Constraint::C_VAR_O);

c.setUnidade(*it_unidade);
c.setSala(*it_sala);
c.setDia(dia);
c.setDisciplina(*it_disc);
c.setTurma(*it_turma);

sprintf( name, "%s", c.toString().c_str() ); 

if (cHash.find(c) != cHash.end()) continue;

cHash[ c ] = lp->getNumRows();

nnz = 2;

int creds = it_disc->cred_praticos + 
it_disc->cred_teoricos;

OPT_ROW row( nnz, OPT_ROW::GREATER , 0.0, name );

v.reset();
v.setType(Variable::V_OFERECIMENTO);

v.setTurma(*it_turma);
v.setDisciplina(*it_disc);
v.setUnidade(*it_unidade);
v.setSala(*it_sala);
v.setDia(dia);

it_v = vHash.find(v);
if( it_v != vHash.end() )
{
row.insert(it_v->second, creds);
}

v.reset();
v.setType(Variable::V_CREDITOS);

v.setTurma(*it_turma);
v.setDisciplina(*it_disc);
v.setUnidade(*it_unidade);
v.setSala(*it_sala);
v.setDia(dia);

it_v = vHash.find(v);
if( it_v != vHash.end() )
{
row.insert(it_v->second, -1.0);
}

lp->addRow(row);
restricoes++;
}
}
}
}
}
return restricoes;
}

int SolverMIP::cria_restricao_sobreposicao(void)
{
int restricoes = 0;
char name[100];
int nnz;
Constraint c;
Variable v;
VariableHash::iterator it_v;

ITERA_GGROUP(it_unidade,problemData->unidades,Unidade) 
{
for(int dia=0;dia<7;dia++) 
{
ITERA_GGROUP(it_disc,problemData->disciplinas,Disciplina) 
{
ITERA_GGROUP(it_turma,it_disc->turmas,Turma) 
{

c.reset();
c.setType(Constraint::C_EVITA_SOBREPOSICAO);

c.setUnidade(*it_unidade);
//c.setSala(*it_sala);
c.setDia(dia);
c.setDisciplina(*it_disc);
c.setTurma(*it_turma);

sprintf( name, "%s", c.toString().c_str() ); 

if (cHash.find(c) != cHash.end()) continue;

cHash[ c ] = lp->getNumRows();

nnz = it_unidade->salas.size();

int creds = it_disc->cred_praticos + 
it_disc->cred_teoricos;

OPT_ROW row( nnz, OPT_ROW::LESS , 1.0, name );

ITERA_GGROUP(it_sala,it_unidade->salas,Sala) 
{

v.reset();
v.setType(Variable::V_OFERECIMENTO);

v.setTurma(*it_turma);
v.setDisciplina(*it_disc);
v.setUnidade(*it_unidade);
v.setSala(*it_sala);
v.setDia(dia);

it_v = vHash.find(v);
if( it_v != vHash.end() )
{
row.insert(it_v->second, creds);
}
}
lp->addRow(row);
restricoes++;
}
}
}
}
return restricoes;
}

int SolverMIP::cria_restricao_mesma_unidade(void)
{
int restricoes = 0;
char name[100];
int nnz;
Constraint c;
Variable v;
VariableHash::iterator it_v;
ITERA_GGROUP(it_disc,problemData->disciplinas,Disciplina) {
ITERA_GGROUP(it_turma,it_disc->turmas,Turma) {

c.reset();
c.setType(Constraint::C_MESMA_UNIDADE);

//         c.setUnidade(*it_unidade);
//         c.setSala(*it_sala);
//         c.setDia(dia);
c.setDisciplina(*it_disc);
c.setTurma(*it_turma);

sprintf( name, "%s", c.toString().c_str() ); 

if (cHash.find(c) != cHash.end()) continue;

cHash[ c ] = lp->getNumRows();

nnz = problemData->unidades.size();

OPT_ROW row( nnz, OPT_ROW::LESS , 1.0, name );

ITERA_GGROUP(it_unidade,problemData->unidades,Unidade) {

v.reset();
v.setType(Variable::V_ABERTURA);

v.setTurma(*it_turma);
v.setDisciplina(*it_disc);
v.setUnidade(*it_unidade);

it_v = vHash.find(v);
if( it_v != vHash.end() )
{
row.insert(it_v->second, 1.0);
}
}
lp->addRow(row);
restricoes++;
}
}
return restricoes;
}

int SolverMIP::cria_restricao_max_creditos(void)
{
int restricoes = 0;
char name[100];
int nnz;
Constraint c;
Variable v;
VariableHash::iterator it_v;

ITERA_GGROUP(it_unidade,problemData->unidades,Unidade) 
{
ITERA_GGROUP(it_sala,it_unidade->salas,Sala) 
{
for(int dia=0;dia<7;dia++) 
{
ITERA_GGROUP(it_disc,problemData->disciplinas,Disciplina) 
{
ITERA_GGROUP(it_turma,it_disc->turmas,Turma) 
{

c.reset();
c.setType(Constraint::C_MAX_CREDITOS);

c.setUnidade(*it_unidade);
c.setSala(*it_sala);
c.setDia(dia);
c.setDisciplina(*it_disc);
c.setTurma(*it_turma);

sprintf( name, "%s", c.toString().c_str() ); 

if (cHash.find(c) != cHash.end()) continue;

cHash[ c ] = lp->getNumRows();

nnz = 1;

int max_creds = 99999999; //TODO, achar min_creds

OPT_ROW row( nnz, OPT_ROW::LESS , max_creds, name );

v.reset();
v.setType(Variable::V_CREDITOS);

v.setTurma(*it_turma);
v.setDisciplina(*it_disc);
v.setUnidade(*it_unidade);
v.setSala(*it_sala);
v.setDia(dia);

it_v = vHash.find(v);
if( it_v != vHash.end() )
{
row.insert(it_v->second, -1.0);
}

lp->addRow(row);
restricoes++;
}
}
}
}
}
return restricoes;
}
*/

/*
int SolverMIP::cria_restricao_turmas_bloco(void)
{
int restricoes = 0;
char name[100];
int nnz;
Constraint c;
Variable v;
VariableHash::iterator it_v;

ITERA_GGROUP(it_unidade,problemData->unidades,Unidade) 
{
for(int dia=0;dia<7;dia++) 
{
ITERA_GGROUP(it_bloco,problemData->blocos,BlocoCurricular) 
{
c.reset();
c.setType(Constraint::C_TURMAS_BLOCO);

c.setUnidade(*it_unidade);
c.setBloco(*it_bloco);
c.setDia(dia);

sprintf( name, "%s", c.toString().c_str() ); 

if (cHash.find(c) != cHash.end()) continue;

cHash[ c ] = lp->getNumRows();
nnz = 0;

ITERA_GGROUP(it_disc,it_bloco->disciplinas,Disciplina) 
nnz += it_disc->turmas.size();

nnz *= it_unidade->salas.size();
nnz += 1;

OPT_ROW row( nnz, OPT_ROW::LESS , 0.0, name );

v.reset();
v.setType(Variable::V_TURMA_BLOCO);

v.setBloco(*it_bloco);
v.setUnidade(*it_unidade);
v.setDia(dia);

it_v = vHash.find(v);
if( it_v != vHash.end() )
{
row.insert(it_v->second, -M);
}

ITERA_GGROUP(it_sala,it_unidade->salas,Sala) 
{
ITERA_GGROUP(it_disc,it_bloco->disciplinas,Disciplina) 
{
ITERA_GGROUP(it_turma,it_disc->turmas,Turma) 
{
v.reset();
v.setType(Variable::V_OFERECIMENTO);

v.setTurma(*it_turma);
v.setDisciplina(*it_disc);
v.setUnidade(*it_unidade);
v.setSala(*it_sala);
v.setDia(dia);

it_v = vHash.find(v);
if( it_v != vHash.end() )
{
row.insert(it_v->second, 1.0);
}
}
}
}
lp->addRow(row);
restricoes++;
}
}
}
return restricoes;
}

int SolverMIP::cria_restricao_cap_demanda(void)
{
int restricoes = 0;
char name[100];
int nnz;
Constraint c;
Variable v;
VariableHash::iterator it_v;

ITERA_GGROUP(it_disc,problemData->disciplinas,Disciplina) 
{
c.reset();
c.setType(Constraint::C_CAP_DEMANDA);
c.setDisciplina(*it_disc);

sprintf( name, "%s", c.toString().c_str() ); 

if (cHash.find(c) != cHash.end()) continue;

cHash[ c ] = lp->getNumRows();

nnz = problemData->unidades.size() * it_disc->turmas.size();

int total_demanda = 0;
ITERA_GGROUP(it_unidade,problemData->unidades,Unidade)
ITERA_GGROUP(it_dem,it_disc->demandas,Demanda) 
total_demanda += it_dem->quantidade;

OPT_ROW row( nnz, OPT_ROW::GREATER , total_demanda, name );

ITERA_GGROUP(it_unidade,problemData->unidades,Unidade)
{
ITERA_GGROUP(it_turma,it_disc->turmas,Turma)
{
v.reset();
v.setType(Variable::V_ALUNOS);

v.setUnidade(*it_unidade);
v.setDisciplina(*it_disc);
v.setTurma(*it_turma);

it_v = vHash.find(v);
if( it_v != vHash.end() )
{
row.insert(it_v->second, 1.0);
}
}
}
lp->addRow(row);
restricoes++;
}
return restricoes;
}

int SolverMIP::cria_restricao_cap_sala(void)
{
int restricoes = 0;
char name[100];
int nnz;
Constraint c;
Variable v;
VariableHash::iterator it_v;
ITERA_GGROUP(it_unidade,problemData->unidades,Unidade) {
ITERA_GGROUP(it_disc,problemData->disciplinas,Disciplina) {
ITERA_GGROUP(it_turma,it_disc->turmas,Turma) {

c.reset();
c.setType(Constraint::C_CAP_SALA);
c.setUnidade(*it_unidade);
c.setDisciplina(*it_disc);
c.setTurma(*it_turma);

sprintf( name, "%s", c.toString().c_str() ); 

if (cHash.find(c) != cHash.end()) continue;

cHash[ c ] = lp->getNumRows();

nnz = it_unidade->salas.size() * 7;

OPT_ROW row( nnz + 1, OPT_ROW::GREATER , 0.0, name );

v.reset();
v.setType(Variable::V_ALUNOS);

v.setTurma(*it_turma);
v.setDisciplina(*it_disc);
v.setUnidade(*it_unidade);

it_v = vHash.find(v);
if( it_v != vHash.end() )
{
row.insert(it_v->second, -1.0);
}

ITERA_GGROUP(it_sala,it_unidade->salas,Sala) {
for(int dia=0;dia<7;dia++) {

v.reset();
v.setType(Variable::V_OFERECIMENTO);

v.setTurma(*it_turma);
v.setDisciplina(*it_disc);
v.setUnidade(*it_unidade);
v.setSala(*it_sala);
v.setDia(dia);

it_v = vHash.find(v);
if( it_v != vHash.end() )
{
row.insert(it_v->second, it_sala->capacidade);
}
}
}
lp->addRow(row);
restricoes++;
}
}
}

return restricoes;
}

int SolverMIP::cria_restricao_cap_sala_unidade(void)
{
int restricoes = 0;
char name[100];
int nnz;
Constraint c;
Variable v;
VariableHash::iterator it_v;

ITERA_GGROUP(it_unidade,problemData->unidades,Unidade) 
{
ITERA_GGROUP(it_sala,it_unidade->salas,Sala) 
{
for(int dia=0;dia<7;dia++) 
{
ITERA_GGROUP(it_disc,problemData->disciplinas,Disciplina) 
{
ITERA_GGROUP(it_turma,it_disc->turmas,Turma) 
{

c.reset();
c.setType(Constraint::C_CAP_SALA_U);
c.setUnidade(*it_unidade);
c.setSala(*it_sala);
c.setDia(dia);
c.setDisciplina(*it_disc);
c.setTurma(*it_turma);

sprintf( name, "%s", c.toString().c_str() ); 

if (cHash.find(c) != cHash.end()) continue;

cHash[ c ] = lp->getNumRows();

nnz = 2;

double rhs = it_sala->capacidade + M;
OPT_ROW row( nnz, OPT_ROW::LESS , rhs, name );

v.reset();
v.setType(Variable::V_OFERECIMENTO);

v.setTurma(*it_turma);
v.setDisciplina(*it_disc);
v.setUnidade(*it_unidade);
v.setSala(*it_sala);
v.setDia(dia);

it_v = vHash.find(v);
if( it_v != vHash.end() )
{
row.insert(it_v->second, M);
}

v.reset();
v.setType(Variable::V_ALUNOS);

v.setTurma(*it_turma);
v.setDisciplina(*it_disc);
v.setUnidade(*it_unidade);

it_v = vHash.find(v);
if( it_v != vHash.end() )
{
row.insert(it_v->second, 1.0);
}

lp->addRow(row);
restricoes++;
}
}
}
}
}
return restricoes;
}

int SolverMIP::cria_restricao_dias_consecutivos(void)
{
int restricoes = 0;
char name[100];
int nnz;
Constraint c;
Variable v;
VariableHash::iterator it_v;
for(int dia=1;dia<7;dia++) {
ITERA_GGROUP(it_disc,problemData->disciplinas,Disciplina) {
ITERA_GGROUP(it_turma,it_disc->turmas,Turma) {

c.reset();
c.setType(Constraint::C_CAP_SALA);
c.setTurma(*it_turma);
c.setDisciplina(*it_disc);
c.setDia(dia);

sprintf( name, "%s", c.toString().c_str() ); 

if (cHash.find(c) != cHash.end()) continue;

cHash[ c ] = lp->getNumRows();

nnz = 0;
ITERA_GGROUP(it_unidade,problemData->unidades,Unidade)
nnz += it_unidade->salas.size();

OPT_ROW row( 2*nnz + 1, OPT_ROW::LESS , 1.0, name );

v.reset();
v.setType(Variable::V_DIAS_CONSECUTIVOS);

v.setTurma(*it_turma);
v.setDisciplina(*it_disc);
v.setDia(dia);

it_v = vHash.find(v);
if( it_v != vHash.end() )
{
row.insert(it_v->second, -1.0);
}

ITERA_GGROUP(it_unidade,problemData->unidades,Unidade) 
{
ITERA_GGROUP(it_sala,it_unidade->salas,Sala) {

v.reset();
v.setType(Variable::V_OFERECIMENTO);

v.setTurma(*it_turma);
v.setDisciplina(*it_disc);
v.setUnidade(*it_unidade);
v.setSala(*it_sala);
v.setDia(dia-1);

it_v = vHash.find(v);
if( it_v != vHash.end() )
{
row.insert(it_v->second, -1.0);
}
v.setDia(dia);

it_v = vHash.find(v);
if( it_v != vHash.end() )
{
row.insert(it_v->second, 1.0);
}

}
}
lp->addRow(row);
restricoes++;
}
}
}
return restricoes;
}
*/