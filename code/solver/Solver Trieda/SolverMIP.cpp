#include "opt_cplex.h"
#include "ProblemData.h"
#include "ProblemSolution.h"
#include "SolverMIP.h"

SolverMIP::SolverMIP(ProblemData *aProblemData)
:Solver(aProblemData)
{
	//rho = 1 faz o codigo ficar mais lento, pq?
	alpha = beta = gamma = delta = lambda = epsilon = M = 1.0;

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
	lp->writeSolution("Solver Trieda.sol");

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

	/**
	ToDo:
	Será preciso preencher as classes "Atendimento" da seguinte forma:
	Ao encontrar um oferecimento, há de se verificar qual campus, 
	unidade, sala, e dia da semana ele corresponde. 

	As variáveis mais importantes são:
	x_{i,d,u,s,t}: indica quantos créditos são oferecidos naquele 
	dia naquela sala daquela disciplina
	a_{i,d,c,cp}: para quantos alunos a disciplina será oferecida

	Essas duas variáveis contêm toda a informação necessária para
	construção da solução;

	Sugiro guardar esses valores em alguns maps:
	De <disciplina,turma> para um vetor de <dia,sala> (unidade)
	De <disciplina,turma> para alunos.
	*/

	typedef
		std::map<std::pair<int,int>,std::vector<std::pair<int,int> > >
		ParVetor;
	typedef 
		std::map<std::pair<int,int>,int>
		ParInteiro;

	// >>>
	// A chave deverá sempre ser um par de inteiros com os respectivos atributos <disciplina,turma>.
	typedef
		std::map<std::pair<int,int>,
			std::vector/*vetor de dias*/<std::pair<int/*valor da variavel*/,
				std::pair<int/*id_unidade*/,int/*id_sala*/> > > > X___i_d_u_s_t;
	// <<<

	// >>>
	// A chave deverá sempre ser um vetor de tamanho 3 com os respectivos atributos <disciplina,turma,curso,campus>.
	typedef
		std::map<std::vector<int>,int>
		A___i_d_c_cp;
	// <<<

	ParVetor creditos_por_dia;
	ParInteiro alunos;

	// >>>
	X___i_d_u_s_t x;
	// <<<

	// >>>
	A___i_d_c_cp a;
	// <<<

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

			std::pair<int,int> key;

			// >>>
			std::vector<int> key_alunos;
			// <<<

			switch(v->getType()) {
			case Variable::V_ERROR:
				std::cout << "Variável inválida " << std::endl;
				break;
			case Variable::V_CREDITOS: 
				key = std::make_pair(v->getDisciplina()->getId(),v->getTurma());

					cout << "Oferta de " << v->getValue() << 
					" creditos da disciplina " << v->getDisciplina()->codigo
					<< " para a turma " << v->getTurma()
					<< " no dia " << v->getDia() << " e na sala " <<
					v->getSala()->getId() << std::endl;

				if(creditos_por_dia[key].size() == 0) 
					creditos_por_dia[key] = 
					std::vector<std::pair<int,int> >(7);
				creditos_por_dia[key][v->getDia()] = 
					std::make_pair((int)v->getValue(),v->getSala()->getId());

				// >>>
				if(x[key].size() == 0) {
					//x[key] = std::vector<std::pair<int,std::pair<int,int>> >(7);
					x[key] = std::vector<std::pair<int,std::pair<int,int>> >
						(7,std::make_pair(-1,std::make_pair(-1,-1)));
				}
				
				x[key][v->getDia()] = std::make_pair((int)v->getValue(),
					std::make_pair(v->getUnidade()->getId(),v->getSala()->getId()));
				// <<<


				break;
			case Variable::V_OFERECIMENTO: break;
			case Variable::V_ABERTURA: break;
			case Variable::V_ALUNOS:
				key = std::make_pair(v->getDisciplina()->getId(),v->getTurma());

					cout << "Oferecimento de " << v->getValue() << 
					" vagas da disciplina " << v->getDisciplina()->codigo
					<< " para a turma " << v->getTurma() << std::endl;

				alunos[key] = (int) v->getValue();

				// >>>
				key_alunos.push_back(v->getDisciplina()->getId());
				key_alunos.push_back(v->getTurma());
				key_alunos.push_back(v->getCurso()->getId());
				key_alunos.push_back(v->getCampus()->getId());

				a[key_alunos] = (int) v->getValue();
				// <<<

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
	/* Depois do map montado, é possível gerar os dados de saída */
	/* Como sugestão, vou escrever uma rotina de impressão */

	std::cout << 
		std::endl << "       RESUMO DA SOLUCAO       " << std::endl;
	for(ParVetor::iterator it = creditos_por_dia.begin();
		it != creditos_por_dia.end(); ++it) {
			std::cout << "..............................." << std::endl;
			std::cout << "Disciplina " << it->first.first << 
				", turma " << it->first.second << ": " << std::endl;
			std::cout << "  Oferta: " << alunos[it->first] << " vagas" 
				<< std::endl;
			for(int i=0;i<7;i++) {
				if(it->second[i].first > 0)
					std::cout << "   Dia " << i << ": " << it->second[i].first
					<< " creditos, sala " << it->second[i].second
					<< std::endl;
			}
	}
	
	// Fill the solution


	problemSolution->addCampus(1,"CP_1");
	problemSolution->addCampus(2,"CP_2");
	problemSolution->addCampus(2,"CP_3");

	GGroup<AtendimentoCampus*>::iterator at_campus = problemSolution->atendimento_campus.begin();

	at_campus->addUnidade(1,"Unidade 1",1);
	at_campus->addUnidade(2,"Unidade 2",1);
	at_campus->addUnidade(2,"Unidade 3",2);
	at_campus->addUnidade(4,"Unidade 4",3);



	std::cout << "Saindo .. ." << std::endl;
	exit(1);

	/*
	// >>>

	std::cout << "\nIniciando .. ." << std::endl;

	for(A___i_d_c_cp::iterator it_a = a.begin(); it_a != a.end(); it_a++) {
		AtendimentoCampus *at_campus;
		bool addCampus = true;

		// Vai servir para ir adicionando os dados.
		GGroup<AtendimentoCampus*>::iterator it_at_campus = 
			problemSolution->atendimento_campus.end(); // end ou begin ?

		if(problemSolution->atendimento_campus.size() > 0 ) {
			ITERA_GGROUP(it_cp, problemSolution->atendimento_campus,AtendimentoCampus) {
				if(it_cp->getId() == it_a->first.at(3) ) {
					std::cout << "CAMPUS encontrado nos registros." << std::endl;
					addCampus = false;
					it_at_campus = it_cp;
					break;
				}
			}
		}
		else {
			std::cout << "Nao existe nenhum CAMPUS registrado ainda .. ." << std::endl;
			at_campus = new AtendimentoCampus;
			at_campus->setId(it_a->first.at(3));
			problemSolution->atendimento_campus.add(at_campus);
			it_at_campus = problemSolution->atendimento_campus.begin();
			addCampus = false;
			std::cout << ".. . CAMPUS adicionado (" << problemSolution->atendimento_campus.size() << ")." << std::endl;
		}

		if(addCampus) {
			std::cout << "Adicionando um novo CAMPUS." << std::endl;
			at_campus = new AtendimentoCampus;
			at_campus->setId(it_a->first.at(3));
			problemSolution->atendimento_campus.add(at_campus);

			// ToDo : FALTA ATUALIZAR O ITERADOR DE CAMPUS ...
			// PARA INSTANCIAS COM APENAS 1 CAMPUS NAO DEVERAO EXISTIR ERROS.

			std::cout << "Saindo -> ToDo : FALTA ATUALIZAR O ITERADOR DE CAMPUS ..." << std::endl;
			exit(1);
		}

		// LEMBRAR DE ADD DEPOIS
		//GGroup<AtendimentoUnidade*> *at_unidade = new GGroup<AtendimentoUnidade*>;

		X___i_d_u_s_t::iterator it_x = x.find(std::make_pair(it_a->first.at(0),it_a->first.at(1)));

		while(it_x != x.end() && !x.empty()) {
			std::cout << "..............................." << std::endl;
			std::cout << "CAMPUS: " << it_a->first.at(3) << std::endl;
			std::cout << "Disciplina " << it_x->first.first << 
				", turma " << it_x->first.second << ": " << std::endl;

			//GGroup<AtendimentoUnidade*> at_unidade = it_at_campus->atendimentos_unidades;
			//AtendimentoUnidade *at_unidade = new AtendimentoUnidade;
			AtendimentoUnidade *at_unidade;

			// Vai servir para ir adicionando os dados.
			GGroup<AtendimentoUnidade*>::iterator it_at_unidade = 
				it_at_campus->atendimentos_unidades.end(); // end ou begin ?

			//std::vector<std::pair<int,std::pair<int,int> > >::iterator it_dia = it_x->second.begin();
			std::vector<std::pair<int,std::pair<int,int> > >::iterator it_dia = it_x->second.begin();

			// Precisa do iterador e da variavel declarada abaixo.
			int dia_semana = 0;

			// DUVIDA: Precisa de criar uma unidade para cada dia?
			// Se sim, basta descomentar o laço abaixo.
			// LEMBRAR DE FECHA-LO ANTES DE DELETAR A VARIAVEL
			for(;it_dia != it_x->second.end(); it_dia++) {

				bool add_unidade = true;
				bool nova_sala = false;

				//if(problemSolution->atendimento_campus.size() == 1) {
				//if(addCampus) {
				if( !(at_campus->atendimentos_unidades.size() > 0) ) {
					// Caiu na condicao do <addCampus> = true e, portanto, ainda nao existem unidades.

					if( it_dia->second.first != -1 ) {

						std::cout << "Nao existe nenhuma UNIDADE registrada para o CAMPUS em questao.. ." << std::endl;

						at_unidade = new AtendimentoUnidade;
						at_unidade->setId(it_dia->second.first);
						it_at_campus->atendimentos_unidades.add(at_unidade);

						it_at_unidade = it_at_campus->atendimentos_unidades.begin();

						std::cout << ".. . UNIDADE adicionada (" << at_campus->atendimentos_unidades.size() << ")." << std::endl;

						nova_sala = true;

					}

					add_unidade = false;
					//nova_sala = true;

				}
				else {
					// Pode ser que ja existam unidades .. .
					// Iterar sobre elas, procurando a unidade desejada.

					ITERA_GGROUP(it_und, it_at_campus->atendimentos_unidades,AtendimentoUnidade) {
						if( it_dia->second.first != -1 ) {
							if(it_und->getId() == it_dia->second.first) {
								std::cout << "UNIDADE encontrada nos registros." << std::endl;
								add_unidade = false;
								nova_sala = true;
								it_at_unidade = it_und;
								break;
							}
						}
						else{
							add_unidade = false;
							break;
						}
					}
				}

				if(add_unidade) {
					std::cout << "Adicionando uma nova UNIDADE." << std::endl;
					at_unidade = new AtendimentoUnidade;
					at_unidade->setId(it_dia->second.first);
					at_campus->atendimentos_unidades.add(at_unidade);

					nova_sala = true;

					// ToDo : FALTA ATUALIZAR O ITERADOR DE UNIDADES ...
					// PARA INSTANCIAS COM APENAS 1 UNIDADE NAO DEVERAO EXISTIR ERROS.

					std::cout << "Saindo -> ToDo : FALTA ATUALIZAR O ITERADOR DE UNIDADES ..." << std::endl;
					exit(1);
				}

				if(nova_sala) {
					AtendimentoSala *at_sala;// = new AtendimentoSala;
					AtendimentoDiaSemana *at_dia_semana;

					// Vai servir para ir adicionando os dados.
					GGroup<AtendimentoSala*>::iterator it_at_sala = 
						it_at_unidade->atendimentos_salas.end(); // end ou begin ?

					bool add_sala = true;
					bool add_dia_semana = true;

					if( !(at_unidade->atendimentos_salas.size() > 0) ) {

						std::cout << "Nao existe nenhuma SALA registrada para a UNIDADE em questao.. ." << std::endl;

						at_sala = new AtendimentoSala;
						at_sala->setId(it_dia->second.second);
						it_at_unidade->atendimentos_salas.add(at_sala);

						it_at_sala = it_at_unidade->atendimentos_salas.begin();

						add_sala = false;

						std::cout << ".. . SALA adicionada (" << at_unidade->atendimentos_salas.size() << ")." << std::endl;

// >>>
						std::cout << "Nao existe nenhum DIA_SEMANA registrado para a SALA em questao.. ." << std::endl;

						at_dia_semana = new AtendimentoDiaSemana;
						at_dia_semana->setId(dia_semana);
						it_at_sala->atendimentos_dias_semana.add(at_dia_semana);

						add_dia_semana = false;

						std::cout << ".. . DIA_SEMANA adicionado (" << at_sala->atendimentos_dias_semana.size() << ")." << std::endl;
// <<<

					}
					else {
						// Pode ser que ja existam salas .. .
						// Iterar sobre elas, procurando a sala desejada.

						ITERA_GGROUP(it_sala, it_at_unidade->atendimentos_salas,AtendimentoSala) {
							if(it_sala->getId() == it_dia->second.second) {
								std::cout << "SALA encontrada nos registros." << std::endl;
								add_sala = false;
								it_at_sala = it_sala;
								break;
							}
						}

					}

					if(add_sala) {
						std::cout << "Adicionando uma nova SALA." << std::endl;
						at_sala = new AtendimentoSala;
						at_sala->setId(it_dia->second.second);
						it_at_unidade->atendimentos_salas.add(at_sala);

						// ToDo : FALTA ATUALIZAR O ITERADOR DE SALAS ...
						// PARA INSTANCIAS COM APENAS 1 SALA NAO DEVERAO EXISTIR ERROS.

						// Pode deletar isso .. .
						// So pra garantir o garantido
						bool aindaFalta = true;

						ITERA_GGROUP(it_sala, it_at_unidade->atendimentos_salas,AtendimentoSala) {
							if(it_sala->getId() == it_dia->second.second) {
								it_at_sala = it_sala;
								aindaFalta = false;
								break;
							}
						}

						if(aindaFalta) {
							std::cout << "Saindo -> ToDo : FALTA ATUALIZAR O ITERADOR DE SALAS ..." << std::endl;
							exit(1);
						}
					}

					if(add_dia_semana) {
						std::cout << "Adicionando um novo DIA_SEMANA." << std::endl;
						at_dia_semana = new AtendimentoDiaSemana;
						at_dia_semana->setId(dia_semana);
						it_at_sala->atendimentos_dias_semana.add(at_dia_semana);

						//it_at_dia_semana = it_at_sala->atendimentos_dias_semana.begin();

						// ToDo : FALTA ATUALIZAR O ITERADOR DE DIA_SEMANA ...
						// PARA INSTANCIAS COM APENAS 1 DIA_SEMANA NAO DEVERAO EXISTIR ERROS.

						// Pode deletar isso .. .
						// So pra garantir o garantido
						bool aindaFalta = true;
					}

//					AtendimentoTatico *at_tatico = ;

				}

				dia_semana++;
			} // FIM DO LACO DE DIAS !!!!

			x.erase(it_x);
			std::cout << "i-esima variavel X_{i} do map de variaveis X removida.\n";
			it_x = x.find(std::make_pair(it_a->first.at(0),it_a->first.at(1)));
		}


		// REMOVER O CAMPUS QUE TRABALHEI EM CIMA .. . (DELETAR VARIAVEL A)
		// APENAS PARA NAO FICAR NA MEMORIA. PARA A EXECUCAO DO PROGRAMA NAO FAZ DIFERENCA SE NAO REMOVER.
	}

	std::cout << "\nSaindo .. ." << std::endl;
	//exit(1);

	// <<<

	*/

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
	num_vars += cria_variavel_oferecimentos(); // o
	num_vars += cria_variavel_creditos(); // x
	num_vars += cria_variavel_abertura(); // z
	num_vars += cria_variavel_alunos(); // a 
	num_vars += cria_variavel_consecutivos(); // c
	num_vars += cria_variavel_max_creds(); // H
	num_vars += cria_variavel_min_creds(); // h
	num_vars += cria_variavel_aloc_disciplina(); // y
	num_vars += cria_variavel_num_subblocos(); // w
	num_vars += cria_variavel_num_abertura_turma_bloco(); // v
	num_vars += cria_variavel_aloc_alunos(); // b
	num_vars += cria_variavel_de_folga_dist_cred_dia_superior(); // fcp
	num_vars += cria_variavel_de_folga_dist_cred_dia_inferior(); // fcm

	return num_vars;
}

int SolverMIP::cria_variavel_creditos(void)
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
							v.setType(Variable::V_CREDITOS);

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
	Desse modo, variaveis desnecessárias (relacionadas à disciplinas que não existem em outros campus)
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
					v.setCampus(*it_campus);	  // cp
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

				OPT_COL col(OPT_COL::VAR_INTEGRAL,rho,0.0,4.0,
					(char*)v.toString().c_str());

				lp->newCol(col);

				num_vars += 1;
			}
		}
	}

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

				OPT_COL col(OPT_COL::VAR_BINARY,beta,0.0,1.0,
					(char*)v.toString().c_str());

				lp->newCol(col);

				num_vars += 1;
			}

		}
	}

	return num_vars;
}

int SolverMIP::cria_variavel_de_folga_dist_cred_dia_superior(void)
{
	int num_vars = 0;

	ITERA_GGROUP(it_disc,problemData->disciplinas,Disciplina) {
		for(int dia=0;dia<7;dia++) {
			Variable v;
			v.reset();

			v.setType(Variable::V_SLACK_DIST_CRED_DIA_SUPERIOR);
			v.setDisciplina(*it_disc);
			v.setDia(dia);

			if (vHash.find(v) == vHash.end())
			{
				vHash[v] = lp->getNumCols();

				int cred_disc_dia = it_disc->min_creds;

				if( it_disc->divisao_creditos != NULL )
				{
					cred_disc_dia = it_disc->divisao_creditos->dia[dia];
				}

				//OPT_COL col(OPT_COL::VAR_INTEGRAL,epsilon,0.0, cred_disc_dia, (char*)v.toString().c_str());
				OPT_COL col(OPT_COL::VAR_INTEGRAL,epsilon,0.0, OPT_INF, (char*)v.toString().c_str());

				lp->newCol(col);

				num_vars += 1;
			}

		}
	}

	return num_vars;
}

int SolverMIP::cria_variavel_de_folga_dist_cred_dia_inferior(void)
{
	int num_vars = 0;

	ITERA_GGROUP(it_disc,problemData->disciplinas,Disciplina) {
		for(int dia=0;dia<7;dia++) {
			Variable v;
			v.reset();

			v.setType(Variable::V_SLACK_DIST_CRED_DIA_INFERIOR);
			v.setDisciplina(*it_disc);
			v.setDia(dia);

			if (vHash.find(v) == vHash.end())
			{
				vHash[v] = lp->getNumCols();

				int cred_disc_dia = it_disc->max_creds;

				if( it_disc->divisao_creditos != NULL )
				{
					cred_disc_dia = it_disc->divisao_creditos->dia[dia];
				}

				//OPT_COL col(OPT_COL::VAR_INTEGRAL,epsilon,0.0, cred_disc_dia, (char*)v.toString().c_str());
				OPT_COL col(OPT_COL::VAR_INTEGRAL,epsilon,0.0, OPT_INF, (char*)v.toString().c_str());

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
	restricoes += cria_restricao_de_folga_dist_cred_dia();		// Restricao 1.2.23

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

				nnz = it_campus->total_salas * 7;

				OPT_ROW row( nnz + 1, OPT_ROW::EQUAL , 0 , name );

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

							OPT_ROW row( nnz, OPT_ROW::LESS , 0.0, name );

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
								row.insert(it_v->second, 1.0); 
								// FIXME
								/* Minimo de um crédito, se é oferecida, só 
								para forçar o oferecimento */
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
	char name[100];
	int nnz;
	Constraint c;
	Variable v;
	VariableHash::iterator it_v;

	ITERA_GGROUP(it_campus,problemData->campi,Campus) {
		ITERA_GGROUP(it_u,it_campus->unidades,Unidade) {
			ITERA_GGROUP(it_sala,it_u->salas,Sala) {
				ITERA_GGROUP(it_disc,problemData->disciplinas,Disciplina) {
					for(int i=0;i<it_disc->num_turmas;++i) {
						c.reset();
						c.setType(Constraint::C_TURMA_DISCIPLINA_SALA);
						c.setUnidade(*it_u);
						c.setSala(*it_sala);
						c.setDisciplina(*it_disc);
						c.setTurma(i);

						sprintf( name, "%s", c.toString().c_str() ); 

						if (cHash.find(c) != cHash.end()) continue;

						cHash[ c ] = lp->getNumRows();

						nnz = 8;
						OPT_ROW row( nnz, OPT_ROW::LESS , 0.0, name );

						for(int t=0;t<7;t++) {
							v.reset();
							v.setDia(t);
							v.setDisciplina(*it_disc);
							v.setUnidade(*it_u);
							v.setSala(*it_sala);
							v.setTurma(i);
							v.setType(Variable::V_OFERECIMENTO);

							it_v = vHash.find(v);
							if( it_v != vHash.end() )
							{
								row.insert(it_v->second, 1.0);
							}
						}
						v.reset();
						v.setTurma(i);
						v.setDisciplina(*it_disc);
						v.setUnidade(*it_u);
						v.setSala(*it_sala);
						v.setType(Variable::V_ALOC_DISCIPLINA);

						it_v = vHash.find(v);
						if( it_v != vHash.end() )
						{
							row.insert(it_v->second, -7.0);
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

int SolverMIP::cria_restricao_turma_sala(void)
{
	int restricoes = 0;
	char name[100];
	int nnz;
	Constraint c;
	Variable v;
	VariableHash::iterator it_v;

	ITERA_GGROUP(it_disc,problemData->disciplinas,Disciplina) {
		for(int i=0;i<it_disc->num_turmas;++i) {
			c.reset();
			c.setType(Constraint::C_TURMA_SALA);
			c.setDisciplina(*it_disc);
			c.setTurma(i);

			sprintf( name, "%s", c.toString().c_str() ); 

			if (cHash.find(c) != cHash.end()) continue;

			cHash[ c ] = lp->getNumRows();

			nnz = problemData->total_salas;
			OPT_ROW row( nnz, OPT_ROW::LESS , 1.0, name );

			ITERA_GGROUP(it_campus,problemData->campi,Campus) {
				ITERA_GGROUP(it_u,it_campus->unidades,Unidade) {
					ITERA_GGROUP(it_sala,it_u->salas,Sala) {
						v.reset();
						v.setType(Variable::V_ALOC_DISCIPLINA);
						v.setTurma(i);
						v.setDisciplina(*it_disc);
						v.setUnidade(*it_u);
						v.setSala(*it_sala);

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
	return restricoes;
}

int SolverMIP::cria_restricao_evita_turma_disc_camp_d(void)
{
	int restricoes = 0;
	char name[100];
	int nnz;
	Constraint c;
	Variable v;
	VariableHash::iterator it_v;

	ITERA_GGROUP(it_disc,problemData->disciplinas,Disciplina) {
		for(int i=0;i<it_disc->num_turmas;++i) {
			c.reset();
			c.setType(Constraint::C_EVITA_TURMA_DISC_CAMP_D);
			c.setDisciplina(*it_disc);
			c.setTurma(i);

			sprintf( name, "%s", c.toString().c_str() ); 

			if (cHash.find(c) != cHash.end()) continue;

			cHash[ c ] = lp->getNumRows();

			nnz = problemData->total_salas;
			OPT_ROW row( nnz, OPT_ROW::LESS , 1.0, name );

			ITERA_GGROUP(it_campus,problemData->campi,Campus) {
				v.reset();
				v.setType(Variable::V_ABERTURA);
				v.setTurma(i);
				v.setDisciplina(*it_disc);
				v.setCampus(*it_campus);

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

int SolverMIP::cria_restricao_turmas_bloco(void)
{	
	int restricoes = 0;
	char name[100];
	int nnz;
	Constraint c;
	Variable v;
	VariableHash::iterator it_v;

	ITERA_GGROUP(it_campus,problemData->campi,Campus) {
		for(int t=0;t<7;t++) {
			ITERA_GGROUP(it_bloco,problemData->blocos,BlocoCurricular) {
				c.reset();
				c.setType(Constraint::C_TURMAS_BLOCO);
				c.setBloco(*it_bloco);
				c.setDia(t);
				c.setCampus(*it_campus);

				sprintf( name, "%s", c.toString().c_str() ); 

				if (cHash.find(c) != cHash.end()) continue;

				cHash[ c ] = lp->getNumRows();

				nnz = it_bloco->total_turmas * 
					problemData->total_salas;

				OPT_ROW row( nnz + 1, OPT_ROW::LESS , 0.0, name );

				ITERA_GGROUP(it_disc,it_bloco->disciplinas,Disciplina) {
					ITERA_GGROUP(it_u,it_campus->unidades,Unidade) {
						ITERA_GGROUP(it_sala,it_u->salas,Sala) {
							for(int i=0;i<it_disc->num_turmas;++i) {
								v.reset();
								v.setType(Variable::V_OFERECIMENTO);
								v.setTurma(i);
								v.setDisciplina(*it_disc);
								v.setUnidade(*it_u);
								v.setSala(*it_sala);
								v.setDia(t);

								it_v = vHash.find(v);
								if( it_v != vHash.end() )
								{
									row.insert(it_v->second, 1.0);
								}
							}
						}
					}
				}
				v.reset();
				v.setType(Variable::V_N_SUBBLOCOS);
				v.setBloco(*it_bloco);
				v.setDia(t);
				v.setCampus(*it_campus);

				it_v = vHash.find(v);
				if( it_v != vHash.end() )
				{
					row.insert(it_v->second, -9999.0);
				}
				/* Provavelmente esta restrição é inútil */

				lp->addRow(row);
				++restricoes;
			}
		}
	}
	return restricoes;
}

int SolverMIP::cria_restricao_max_cred_disc_bloco(void)
{
	int restricoes = 0;
	char name[100];
	int nnz;
	Constraint c;
	Variable v;
	VariableHash::iterator it_v;

	ITERA_GGROUP(it_campus,problemData->campi,Campus) {
		for(int t=0;t<7;t++) {
			ITERA_GGROUP(it_bloco,problemData->blocos,BlocoCurricular) {
				c.reset();
				c.setType(Constraint::C_MAX_CRED_DISC_BLOCO);
				c.setBloco(*it_bloco);
				c.setDia(t);
				c.setCampus(*it_campus);

				sprintf( name, "%s", c.toString().c_str() ); 

				if (cHash.find(c) != cHash.end()) continue;

				cHash[ c ] = lp->getNumRows();

				nnz = it_bloco->total_turmas * 
					problemData->total_salas;

				OPT_ROW row( nnz + 1, OPT_ROW::LESS , 0.0, name );

				ITERA_GGROUP(it_disc,it_bloco->disciplinas,Disciplina) {
					ITERA_GGROUP(it_u,it_campus->unidades,Unidade) {
						ITERA_GGROUP(it_sala,it_u->salas,Sala) {
							for(int i=0;i<it_disc->num_turmas;++i) {
								v.reset();
								v.setType(Variable::V_CREDITOS);
								v.setTurma(i);
								v.setDisciplina(*it_disc);
								v.setUnidade(*it_u);
								v.setSala(*it_sala);
								v.setDia(t);

								it_v = vHash.find(v);
								if( it_v != vHash.end() )
								{
									row.insert(it_v->second, 1.0);
								}
							}
						}
					}
				}
				v.reset();
				v.setType(Variable::V_N_SUBBLOCOS);
				v.setBloco(*it_bloco);
				v.setDia(t);
				v.setCampus(*it_campus);

				it_v = vHash.find(v);
				if( it_v != vHash.end() )
				{
					row.insert(it_v->second, -24.0); // #Warning: FIXME
				}
				/* Descobrir valor de H_t */

				lp->addRow(row);
				++restricoes;
			}
		}
	}
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

	char name[100];
	int nnz;
	Constraint c;
	Variable v;
	VariableHash::iterator it_v;

	ITERA_GGROUP(it_campus,problemData->campi,Campus) {
		ITERA_GGROUP(it_u,it_campus->unidades,Unidade) {
			ITERA_GGROUP(it_sala,it_u->salas,Sala) {
				for(int t=0;t<7;t++) {
					ITERA_GGROUP(it_disc,problemData->disciplinas,Disciplina) {
						for(int i=0;i<it_disc->num_turmas;i++) {
							c.reset();
							c.setType(Constraint::C_LIM_CRED_DIAR_DISC);
							c.setTurma(i);
							c.setDisciplina(*it_disc);
							c.setUnidade(*it_u);
							c.setSala(*it_sala);
							c.setDia(t);

							sprintf( name, "%s", c.toString().c_str() ); 

							if (cHash.find(c) != cHash.end()) continue;

							cHash[ c ] = lp->getNumRows();

							nnz = 1;

							OPT_ROW row( 1, OPT_ROW::LESS , it_disc->max_creds , 
								name );

							v.reset();
							v.setType(Variable::V_CREDITOS);
							v.setTurma(i);
							v.setDisciplina(*it_disc);
							v.setUnidade(*it_u);
							v.setSala(*it_sala);
							v.setDia(t);

							it_v = vHash.find(v);
							if( it_v != vHash.end() )
							{
								row.insert(it_v->second, 1.0);
							}

							lp->addRow(row);
							++restricoes;
						}
					}
				}
			}
		}
	}
	return restricoes;
}

int SolverMIP::cria_restricao_cap_aloc_dem_disc(void)
{
	int restricoes = 0;
	char name[100];
	int nnz;
	Constraint c;
	Variable v;
	VariableHash::iterator it_v;

	ITERA_GGROUP(it_campus,problemData->campi,Campus) {
		ITERA_GGROUP(it_disc,problemData->disciplinas,Disciplina) {
			ITERA_GGROUP(it_curso,problemData->cursos,Curso) {
				c.reset();
				c.setType(Constraint::C_CAP_ALOC_DEM_DISC);
				c.setCampus(*it_campus);
				c.setDisciplina(*it_disc);
				c.setCurso(*it_curso);

				sprintf( name, "%s", c.toString().c_str() ); 

				if (cHash.find(c) != cHash.end()) continue;

				cHash[ c ] = lp->getNumRows();

				nnz = it_disc->num_turmas;

				int rhs = 0;
				ITERA_GGROUP(it_dem,problemData->demandas,Demanda) {
					if (it_dem->disciplina->getId() == it_disc->getId() &&
						it_dem->oferta->curso->getId() == it_curso->getId() &&
						it_dem->oferta->campus->getId() == it_campus->getId())
					{
						rhs += it_dem->quantidade;
					}
				}

				OPT_ROW row( nnz , OPT_ROW::EQUAL , rhs , name );

				for(int i=0;i<it_disc->num_turmas;++i) {
					v.reset();
					v.setType(Variable::V_ALUNOS);
					v.setTurma(i);
					v.setDisciplina(*it_disc);
					v.setCampus(*it_campus);
					v.setCurso(*it_curso);

					it_v = vHash.find(v);
					if( it_v != vHash.end() )
					{
						row.insert(it_v->second, 1.0);
					}
				}

				lp->addRow(row);
				++restricoes;
			}
		}
	}
	return restricoes;
}

int SolverMIP::cria_restricao_cap_sala_compativel_turma(void)
{
	int restricoes = 0;
	char name[100];
	int nnz;
	Constraint c;
	Variable v;
	VariableHash::iterator it_v;

	ITERA_GGROUP(it_campus,problemData->campi,Campus) {
		ITERA_GGROUP(it_disc,problemData->disciplinas,Disciplina) {
			for(int i=0;i<it_disc->num_turmas;++i) {

				c.reset();
				c.setType(Constraint::C_CAP_SALA_COMPATIVEL_TURMA);
				c.setCampus(*it_campus);
				c.setDisciplina(*it_disc);
				c.setTurma(i);

				sprintf( name, "%s", c.toString().c_str() ); 

				if (cHash.find(c) != cHash.end()) continue;

				cHash[ c ] = lp->getNumRows();

				nnz = problemData->cursos.size() +
					it_campus->total_salas * 7;

				OPT_ROW row( nnz , OPT_ROW::LESS , 0.0 , name );

				ITERA_GGROUP(it_curso,problemData->cursos,Curso) {
					v.reset();
					v.setType(Variable::V_ALUNOS);
					v.setTurma(i);
					v.setDisciplina(*it_disc);
					v.setCampus(*it_campus);
					v.setCurso(*it_curso);

					it_v = vHash.find(v);
					if( it_v != vHash.end() )
					{
						row.insert(it_v->second, 1.0);
					}
				}

				ITERA_GGROUP(it_u,it_campus->unidades,Unidade) {
					ITERA_GGROUP(it_sala,it_u->salas,Sala) {
						for(int t=0;t<7;t++) {
							v.reset();
							v.setType(Variable::V_OFERECIMENTO);
							v.setTurma(i);
							v.setDisciplina(*it_disc);
							v.setUnidade(*it_u);
							v.setSala(*it_sala);
							v.setDia(t);

							it_v = vHash.find(v);
							if( it_v != vHash.end() )
							{
								row.insert(it_v->second, -it_sala->capacidade);
							}
						}
					}
				}
				lp->addRow(row);
				++restricoes;
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

	ITERA_GGROUP(it_campus,problemData->campi,Campus) {
		ITERA_GGROUP(it_u,it_campus->unidades,Unidade) {
			ITERA_GGROUP(it_sala,it_u->salas,Sala) {
				ITERA_GGROUP(it_disc,problemData->disciplinas,Disciplina) {
					for(int i=0;i<it_disc->num_turmas;++i) {
						for(int t=0;t<7;t++) {
							c.reset();
							c.setType(Constraint::C_CAP_SALA_UNIDADE);
							c.setUnidade(*it_u);
							c.setSala(*it_sala);
							c.setDisciplina(*it_disc);
							c.setTurma(i);
							c.setCampus(*it_campus);
							c.setDia(t);

							sprintf( name, "%s", c.toString().c_str() ); 

							if (cHash.find(c) != cHash.end()) continue;

							cHash[ c ] = lp->getNumRows();

							nnz = problemData->cursos.size() + 1;
							int rhs = it_sala->capacidade + it_u->maior_sala;

							OPT_ROW row( nnz, OPT_ROW::LESS , rhs , name );

							v.reset();
							v.setDia(t);
							v.setDisciplina(*it_disc);
							v.setUnidade(*it_u);
							v.setSala(*it_sala);
							v.setTurma(i);
							v.setType(Variable::V_OFERECIMENTO);

							it_v = vHash.find(v);
							if( it_v != vHash.end() )
							{
								row.insert(it_v->second, it_u->maior_sala);
							}

							ITERA_GGROUP(it_curso,problemData->cursos,Curso) {

								v.reset();
								v.setTurma(i);
								v.setDisciplina(*it_disc);
								v.setCampus(*it_campus);
								v.setCurso(*it_curso);

								v.setType(Variable::V_ALUNOS);

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
				}
			}
		}
	}
	return restricoes;
}

int SolverMIP::cria_restricao_turma_disc_dias_consec(void)
{
	int restricoes = 0;

	char name[100];
	int nnz;
	Constraint c;
	Variable v;
	VariableHash::iterator it_v;

	ITERA_GGROUP(it_disc,problemData->disciplinas,Disciplina) {
		for(int i=0;i<it_disc->num_turmas;++i) {
			for(int t=1;t<7;t++) {
				c.reset();
				c.setDisciplina(*it_disc);
				c.setType(Constraint::C_TURMA_DISC_DIAS_CONSEC);
				c.setTurma(i);
				c.setDia(t);

				sprintf( name, "%s", c.toString().c_str() ); 

				if (cHash.find(c) != cHash.end()) continue;

				cHash[ c ] = lp->getNumRows();

				nnz = problemData->total_salas*2 + 1;

				OPT_ROW row( nnz, OPT_ROW::GREATER , -1 , name );

				v.reset();
				v.setType(Variable::V_DIAS_CONSECUTIVOS);
				v.setTurma(i);
				v.setDisciplina(*it_disc);
				v.setDia(t);

				it_v = vHash.find(v);
				if( it_v != vHash.end() )
				{
					row.insert(it_v->second, 1.0);
				}

				ITERA_GGROUP(it_campus,problemData->campi,Campus) {
					ITERA_GGROUP(it_u,it_campus->unidades,Unidade) {
						ITERA_GGROUP(it_sala,it_u->salas,Sala) {
							v.reset();
							v.setType(Variable::V_OFERECIMENTO);
							v.setTurma(i);
							v.setDisciplina(*it_disc);
							v.setUnidade(*it_u);
							v.setSala(*it_sala);
							v.setDia(t);

							it_v = vHash.find(v);
							if( it_v != vHash.end() )
							{
								row.insert(it_v->second, -1.0);
							}

							v.setDia(t-1);

							it_v = vHash.find(v);
							if( it_v != vHash.end() )
							{
								row.insert(it_v->second, 1.0);
							}
						}
					}
				}
				lp->addRow(row);
				++restricoes;
			}
		}
	}

	return restricoes;
}

int SolverMIP::cria_restricao_min_creds_turm_bloco(void)
{
	int restricoes = 0;

	char name[100];
	int nnz;
	Constraint c;
	Variable v;
	VariableHash::iterator it_v;

	ITERA_GGROUP(it_bloco,problemData->blocos,BlocoCurricular) {
		ITERA_GGROUP(it_disc,it_bloco->disciplinas,Disciplina) {
			for(int i=0;i<it_disc->num_turmas;i++){
				for(int dia=0;dia<7;dia++) {
					c.reset();
					c.setType(Constraint::C_MIN_CREDS_TURM_BLOCO);

					c.setBloco(*it_bloco);
					c.setDisciplina(*it_disc);
					c.setDia(dia);

					sprintf( name, "%s", c.toString().c_str() ); 

					if (cHash.find(c) != cHash.end()) continue;

					cHash[ c ] = lp->getNumRows();

					// Conferir nnz depois.
					nnz = (it_bloco->disciplinas.size() * problemData->total_salas) + 1;

					OPT_ROW row( nnz, OPT_ROW::GREATER , 0.0 , name );

					ITERA_GGROUP(it_b,problemData->blocos,BlocoCurricular) {
						ITERA_GGROUP(it_disc,it_b->disciplinas,Disciplina) {
							ITERA_GGROUP(it_u,it_b->campus->unidades,Unidade) {
								ITERA_GGROUP(it_sala,it_u->salas,Sala) {
									v.reset();
									v.setType(Variable::V_CREDITOS);

									v.setTurma(i);
									v.setDisciplina(*it_disc);
									v.setUnidade(*it_u);
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
					}

					v.reset();
					v.setType(Variable::V_MIN_CRED_SEMANA);

					v.setTurma(i);
					v.setBloco(*it_bloco);

					it_v = vHash.find(v);
					if( it_v != vHash.end() )
					{
						row.insert(it_v->second, -1.0);
					}

					lp->addRow(row);
					++restricoes;
				}
			}
		}
	}

	return restricoes;
}

int SolverMIP::cria_restricao_max_creds_turm_bloco(void)
{
	int restricoes = 0;

	char name[100];
	int nnz;
	Constraint c;
	Variable v;
	VariableHash::iterator it_v;

	ITERA_GGROUP(it_bloco,problemData->blocos,BlocoCurricular) {
		ITERA_GGROUP(it_disc,it_bloco->disciplinas,Disciplina) {
			for(int i=0;i<it_disc->num_turmas;i++){
				for(int dia=0;dia<7;dia++) {
					c.reset();
					c.setType(Constraint::C_MAX_CREDS_TURM_BLOCO);

					c.setBloco(*it_bloco);
					c.setDisciplina(*it_disc);
					c.setDia(dia);

					sprintf( name, "%s", c.toString().c_str() ); 

					if (cHash.find(c) != cHash.end()) continue;

					cHash[ c ] = lp->getNumRows();

					// Conferir nnz depois.
					nnz = (it_bloco->disciplinas.size() * problemData->total_salas) + 1;

					OPT_ROW row( nnz, OPT_ROW::LESS , 0.0 , name );

					ITERA_GGROUP(it_b,problemData->blocos,BlocoCurricular) {
						ITERA_GGROUP(it_disc,it_b->disciplinas,Disciplina) {
							ITERA_GGROUP(it_u,it_b->campus->unidades,Unidade) {
								ITERA_GGROUP(it_sala,it_u->salas,Sala) {
									v.reset();
									v.setType(Variable::V_CREDITOS);

									v.setTurma(i);
									v.setDisciplina(*it_disc);
									v.setUnidade(*it_u);
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
					}

					v.reset();
					v.setType(Variable::V_MAX_CRED_SEMANA);

					v.setTurma(i);
					v.setBloco(*it_bloco);

					it_v = vHash.find(v);
					if( it_v != vHash.end() )
					{
						row.insert(it_v->second, -1.0);
					}

					lp->addRow(row);
					++restricoes;
				}
			}
		}
	}

	return restricoes;
}

int SolverMIP::cria_restricao_aluno_curso_disc(void)
{
	int restricoes = 0;

	char name[100];
	int nnz;
	Constraint c;
	Variable v;
	VariableHash::iterator it_v;

	ITERA_GGROUP(it_campus,problemData->campi,Campus) {
		ITERA_GGROUP(it_disc,problemData->disciplinas,Disciplina) {
			for(int i=0;i<it_disc->num_turmas;++i) {
				ITERA_GGROUP(it_curso,problemData->cursos,Curso) {

					c.reset();
					c.setType(Constraint::C_ALUNO_CURSO_DISC);
					c.setDisciplina(*it_disc);
					c.setTurma(i);
					c.setCurso(*it_curso);
					c.setCampus(*it_campus);

					sprintf( name, "%s", c.toString().c_str() ); 

					if (cHash.find(c) != cHash.end()) continue;

					cHash[ c ] = lp->getNumRows();

					nnz = 2;

					OPT_ROW row( nnz, OPT_ROW::LESS , 0.0 , name );

					v.reset();
					v.setType(Variable::V_ALUNOS);
					v.setTurma(i);
					v.setDisciplina(*it_disc);
					v.setCurso(*it_curso);               
					v.setCampus(*it_campus);

					it_v = vHash.find(v);
					if( it_v != vHash.end() )
					{
						row.insert(it_v->second, 1.0);
					}

					v.setType(Variable::V_ALOC_ALUNO);
					it_v = vHash.find(v);
					if( it_v != vHash.end() )
					{
						row.insert(it_v->second, -it_campus->maior_sala);
					}

					lp->addRow(row);
					++restricoes;
				}
			}
		}
	}
	return restricoes;
}

int SolverMIP::cria_restricao_alunos_cursos_dif(void)
{
	int restricoes = 0;
	char name[100];
	int nnz;
	Constraint c;
	Variable v;
	VariableHash::iterator it_v;

	ITERA_GGROUP(it_campus,problemData->campi,Campus) {
		ITERA_GGROUP(it_disc,problemData->disciplinas,Disciplina) {
			for(int i=0;i<it_disc->num_turmas;++i) {

				std::map<std::pair<Curso*,Curso*>,bool>::iterator it_compat_cursos =
					problemData->compat_cursos.begin();

				for(; it_compat_cursos != problemData->compat_cursos.end(); ++it_compat_cursos) {

					if(!it_compat_cursos->second) {

						c.reset();
						c.setType(Constraint::C_ALUNOS_CURSOS_DIF);

						c.setCampus(*it_campus);

						c.setCurso(it_compat_cursos->first.first);
						c.setCursoIncompat(it_compat_cursos->first.second);

						c.setDisciplina(*it_disc);
						c.setTurma(i);

						sprintf( name, "%s", c.toString().c_str() ); 

						if (cHash.find(c) != cHash.end()) continue;

						// Testando com os cursos invertidos

						c.setCurso(it_compat_cursos->first.second);
						c.setCursoIncompat(it_compat_cursos->first.first);

						if (cHash.find(c) != cHash.end()) continue;

						cHash[ c ] = lp->getNumRows();

						nnz = 2;

						OPT_ROW row( nnz, OPT_ROW::LESS , 1.0 , name );

						// ---

						v.reset();
						v.setType(Variable::V_ALOC_ALUNO);

						v.setTurma(i);
						v.setDisciplina(*it_disc);
						v.setCampus(*it_campus);
						v.setCurso(it_compat_cursos->first.first);

						it_v = vHash.find(v);
						if( it_v != vHash.end() )
						{
							row.insert(it_v->second, 1);
						}

						// ---

						v.reset();
						v.setType(Variable::V_ALOC_ALUNO);

						v.setTurma(i);
						v.setDisciplina(*it_disc);
						v.setCampus(*it_campus);
						v.setCurso(it_compat_cursos->first.second);

						it_v = vHash.find(v);
						if( it_v != vHash.end() )
						{
							row.insert(it_v->second, 1);
						}

						lp->addRow(row);
						++restricoes;

					}
				}

			}
		}
	}

	return restricoes;
}

int SolverMIP::cria_restricao_de_folga_dist_cred_dia(void)
{
	int restricoes = 0;

	char name[100];
	int nnz;
	Constraint c;
	Variable v;
	VariableHash::iterator it_v;

	ITERA_GGROUP(it_disc,problemData->disciplinas,Disciplina) {
		for(int dia=0;dia<7;dia++) {
			c.reset();
			c.setType(Constraint::C_SLACK_DIST_CRED_DIA);

			c.setDisciplina(*it_disc);
			c.setDia(dia);

			sprintf( name, "%s", c.toString().c_str() ); 

			if (cHash.find(c) != cHash.end()) continue;

			cHash[ c ] = lp->getNumRows();

			nnz = problemData->total_salas * it_disc->num_turmas  + 1;

			int rhs = it_disc->max_creds;
			if( it_disc->divisao_creditos != NULL )
			{
				rhs = it_disc->divisao_creditos->dia[dia];
			}

			OPT_ROW row( nnz, OPT_ROW::EQUAL , rhs , name );

			ITERA_GGROUP(it_campus,problemData->campi,Campus) {

				ITERA_GGROUP(it_u,it_campus->unidades,Unidade) {
					ITERA_GGROUP(it_sala,it_u->salas,Sala) {

						ITERA_GGROUP(it_disc,problemData->disciplinas,Disciplina) {
							for(int i=0;i<it_disc->num_turmas;i++) {

								v.reset();
								v.setType(Variable::V_CREDITOS);

								v.setTurma(i);
								v.setDisciplina(*it_disc);
								v.setUnidade(*it_u);
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
				}
			}

			v.reset();
			v.setType(Variable::V_SLACK_DIST_CRED_DIA_SUPERIOR);

			v.setDisciplina(*it_disc);
			v.setDia(dia);

			it_v = vHash.find(v);
			if( it_v != vHash.end() )
			{
				row.insert(it_v->second, 1.0);
			}

			v.reset();
			v.setType(Variable::V_SLACK_DIST_CRED_DIA_INFERIOR);

			v.setDisciplina(*it_disc);
			v.setDia(dia);

			it_v = vHash.find(v);
			if( it_v != vHash.end() )
			{
				row.insert(it_v->second, -1.0);
			}

			lp->addRow(row);
			++restricoes;
		}
	}

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