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
	
%DocEnd
/===================================================================*/

//#include "opt_cplex.h"
#include "opt_gurobi.h"
#include "ProblemData.h"
#include "ProblemSolution.h"
#include "SolverMIP.h"
#include "SolutionLoader.h"
#include "ErrorHandler.h"

bool ordenaSolucoes(Solucao const * left, Solucao const * right)
{
   return (left->demanda_Nao_Atendida > right->demanda_Nao_Atendida);
}

// >>>
//#define PRINT_CSV
// <<<

SolverMIP::SolverMIP(ProblemData *aProblemData)
:Solver(aProblemData)
{
	alpha = 5.0;
	beta = 10.0;
	gamma = 0;
	delta = 0;
	lambda = 0.1;
	epsilon = 1.0;
	M = 1.0;
	rho = 5;
	//verificar o valor
	psi = 1.0;

	try
	{
		//lp = new OPT_CPLEX;
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

   lp->updateLP();

#ifdef DEBUG
	printf("Total of Variables: %i\n\n",varNum);
#endif

	/* Constraint creation */
	constNum = cria_restricoes();

   lp->updateLP();

#ifdef DEBUG
	printf("Total of Constraints: %i\n\n",constNum);
#endif

#ifdef DEBUG
	lp->writeProbLP("Solver Trieda");
#endif

   // TRIEDA-560

   //// Gerando Soluções
   //vector<Solucao*> conjunto_de_Solucoes;

   //for(int num_Sol = 0; num_Sol < 5; num_Sol++)
   {
      //Solucao * sol = new Solucao(*problemData);
      //Solucao sol (*problemData);
   //   //sol->geraSolucao();
      //sol.geraSolucaoSubBlocos();
      //sol.imprimeInfo();
   //   cout << "FO: " << sol->demanda_Nao_Atendida << endl;

   //   conjunto_de_Solucoes.push_back(sol);
   }

   //sort(conjunto_de_Solucoes.begin(),conjunto_de_Solucoes.end(),ordenaSolucoes);


   //getchar();

   // Gerando Soluções - FIM

	int status = 0;

	//lp->setTimeLimit(20);
	//lp->setMIPStartAlg(METHOD_PRIMAL);
	//lp->setMIPEmphasis(4);
	lp->setMIPScreenLog(4);

   lp->setHeurFrequency(1.0);

	status = lp->optimize(METHOD_MIP);

	lp->writeSolution("Solver Trieda.sol");

	return status;
}

void SolverMIP::getSolution(ProblemSolution *problemSolution)
{
	double *xSol = NULL;
	VariableHash::iterator vit;

	SolutionLoader sLoader(problemData, problemSolution);

	xSol = new double[lp->getNumCols()];
	lp->getX(xSol);

	vit = vHash.begin();

#ifdef DEBUG
	FILE *fout = fopen("solucao.txt","wt");
#endif

#ifdef PRINT_CSV
	FILE *f_V_CREDITOS = fopen("./CSV/V_CREDITOS.csv","wt");
	bool printLegend_V_CREDITOS = true;

	bool printLegend_V_ALUNOS = true;
	FILE *f_V_ALUNOS = fopen("./CSV/V_ALUNOS.csv","wt");
#endif

	// A chave deverá sempre ser um par de inteiros com os respectivos atributos <disciplina,turma>.
	typedef
		std::map<std::pair<int/*turma*/,int/*disciplina*/>,
		std::vector/*vetor de dias*/<std::pair<int/*valor da variavel*/,
		std::pair<int/*id_unidade*/,int/*id do tipo de sala*/> > > > X___i_d_u_tps_t;

	// A chave deverá sempre ser um vetor de tamanho 3 com os respectivos atributos <turma,disciplina,oferta>.
	typedef
		std::map<std::vector<int>,int>
		A___i_d_o;

	X___i_d_u_tps_t x;

	A___i_d_o a;

	while (vit != vHash.end())
	{
		Variable *v = new Variable(vit->first);
		int col = vit->second;

		v->setValue( xSol[col] );

		if ( v->getValue() > 0 )
		{
#ifdef DEBUG
			char auxName[100];
			lp->getColName(auxName,col,100);
			fprintf(fout,"%s = %f\n",auxName,v->getValue());
#endif

			std::pair<int,int> key;
			std::vector<int> key_alunos;
			int id_disc;

			switch(v->getType()) {
		 case Variable::V_ERROR:
			 std::cout << "Variável inválida " << std::endl;
			 break;
		 case Variable::V_CREDITOS:

			 id_disc = v->getDisciplina()->getId();

			 key = std::make_pair(v->getTurma(),id_disc);

			 cout << "Oferta de " << v->getValue() << 
				 " creditos da disciplina " << v->getDisciplina()->codigo
				 << " para a turma " << v->getTurma()
				 << " no dia " << v->getDia() << " para alguma de sala com capacidade " <<
				 v->getSubCjtSala()->getId() << std::endl;
			 if(x[key].size() == 0) 
			 {
				 x[key] = std::vector<std::pair<int,std::pair<int,int> > >
					 (7,std::make_pair(-1,std::make_pair(-1,-1)));
			 }

			 if(x[key][v->getDia()].first > 0)
			 {
				 x[key][v->getDia()] = std::make_pair(
					 (x[key][v->getDia()].first + (int)(v->getValue()+.5)),
					 std::make_pair(v->getUnidade()->getId(),v->getSubCjtSala()->getId()));
			 }
			 else if(x[key][v->getDia()].first == -1) 
			 {
				 x[key][v->getDia()] = std::make_pair(
					 (int)(v->getValue()+.5),
					 std::make_pair(v->getUnidade()->getId(),v->getSubCjtSala()->getId()));
			 }

#ifdef PRINT_CSV
			 if(printLegend_V_CREDITOS){
				 fprintf(f_V_CREDITOS,"Var. x,\t\ti,\td,\tu,\ts,\t\tt,\n");
				 printLegend_V_CREDITOS = false;
			 }
			 // >>> 07/10/2010
			 /*
			 fprintf(f_V_CREDITOS,"%f,\t%d,\t%d,\t%d,\t%d,\t%d,\n",v->getValue(),v->getTurma(),v->getDisciplina()->getId(),
			 v->getUnidade()->getId(),v->getSala()->getId(),v->getDia());
			 */
			 fprintf(f_V_CREDITOS,"%f,\t%d,\t%d,\t%d,\t%d,\t%d,\n",v->getValue(),v->getTurma(),id_disc,
				 v->getUnidade()->getId(),v->getSala()->getId(),v->getDia());
			 // <<< 07/10/2010
#endif

			 break;
		 case Variable::V_OFERECIMENTO: break;
		 case Variable::V_ABERTURA: break;
		 case Variable::V_ALUNOS:
			 id_disc = v->getDisciplina()->getId();

			 cout << "Oferecimento de " << v->getValue() << 
				 " vagas da disciplina " << v->getDisciplina()->codigo
				 << " para a turma " << v->getTurma()
				 << " do curso " << v->getOferta()->curso->codigo << std::endl;

			 key_alunos.push_back(v->getTurma());
			 key_alunos.push_back(id_disc);
			 key_alunos.push_back(v->getOferta()->getId());

			 if(a.find(key_alunos) == a.end())
			 { a[key_alunos] = (int) (v->getValue()+.5); }

#ifdef PRINT_CSV
			 if(printLegend_V_ALUNOS){
				 fprintf(f_V_ALUNOS,"Var. a,\t\ti,\td,\tc,\tcp,\n");
				 printLegend_V_ALUNOS = false;
			 }

			 // >>> 07/10/2010
			 /*
			 fprintf(f_V_ALUNOS,"%f,\t%d,\t%d,\t%d,\t%d,\n",v->getValue(),v->getTurma(),
			 v->getDisciplina()->getId(),v->getCurso()->getId(),v->getCampus()->getId());
			 */
			 fprintf(f_V_ALUNOS,"%f,\t%d,\t%d,\t%d,\t%d,\n",v->getValue(),v->getTurma(),
				 id_disc,v->getCurso()->getId(),v->getCampus()->getId());
			 // <<< 07/10/2010
#endif
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

	std::cout << "\nx\t -\t i\t d\t u\t tps\t t" << std::endl;
	for(X___i_d_u_tps_t::iterator it_x = x.begin(); it_x != x.end(); it_x++)
	{
		for(int dia = 0; dia < 7; dia++) 
		{
			if(it_x->second.at(dia).first > 0)
			{
				std::cout << it_x->second[dia].first << "\t\t" <<
					it_x->first.first << "\t" <<
					it_x->first.second << "\t" <<
					it_x->second[dia].second.first << "\t" <<
					it_x->second[dia].second.second << "\t" <<
					dia << std::endl;
			}
		}
	}

	std::cout << "\na\t -\t i\t d\t o" << std::endl;
	for(A___i_d_o::iterator it_a = a.begin(); it_a != a.end(); it_a++)
	{
		if(it_a->second > 0)
		{
			std::cout << it_a->second << "\t\t" <<
				it_a->first.at(0) << "\t" <<
				it_a->first.at(1) << "\t" <<
				it_a->first.at(2) << "\t" << std::endl;
		}
	}

	// ==============================================================
	// ==============================================================
	// PÓS-PROCESSAMENTO 

	// Cada vetor interno representa uma variável x mais o seu valor.
	typedef std::vector<std::vector<int/*i,d,u,s,t*/> > X___i_d_u_s_t;

	// Variável x após o pós-processamento.
	X___i_d_u_s_t vXpp;

	for(X___i_d_u_tps_t::iterator it_x = x.begin(); it_x != x.end(); it_x++)
	{
		int turma = it_x->first.first;
		int idDisc = it_x->first.second;

		// Inicializando vXppp
		std::pair<int,int> chaveTurmaDisc (turma,idDisc);

		for(unsigned dia = 0; dia < it_x->second.size(); dia++)
		{
			/* Se a var. x possui valor maior do que 0 */
			if(it_x->second.at(dia).first > 0)
			{ 
				int valorX = it_x->second.at(dia).first;
				int und = it_x->second.at(dia).second.first;
				int tipoSala = it_x->second.at(dia).second.second;

				std::vector<int> vtAux;
				vtAux.push_back(valorX);
				vtAux.push_back(turma);
				vtAux.push_back(idDisc);
				vtAux.push_back(und);
				vtAux.push_back(tipoSala);
				vtAux.push_back(dia);

				vXpp.push_back(vtAux);
			}
		}
	}

	sort(vXpp.begin(),vXpp.end(),ordenaPorCreditos);

	// Alocando as turmas às salas.
	for(X___i_d_u_s_t::iterator it_x = vXpp.begin(); it_x != vXpp.end(); it_x++)
	{
		int idDisc = it_x->at(2);
		int dia = it_x->at(5);
		
		ITERA_GGROUP(itSala,problemData->discSalas[idDisc],Sala)
		{
			bool alocou = false;

			if(it_x->at(4) < 0)
				it_x->at(4) *= -1;
			
			/* Se a capacidade da sala é igual à capacidade especificada pelo solver */
			if(itSala->capacidade == it_x->at(4))
			{
				/* Se a sala comporta (possui créditos livres) o valor demandado */
				if( (itSala->credsLivres.at(dia) - 
					it_x->at(0)) >= 0)
				{
					// Subtraindo os créditos
					itSala->credsLivres.at(dia) -= it_x->at(0);

					// Setando a sala
					it_x->at(4) = itSala->getId();
					alocou = true;
					break; // Já aloquei, então posso parar
				}
			}
			if(!alocou)
			{
				ITERA_GGROUP(itCampus,problemData->campi,Campus)
				{
					ITERA_GGROUP(itUnidade,itCampus->unidades,Unidade)
					{
						ITERA_GGROUP(it_Sala,itUnidade->salas,Sala)
						{
							if(it_Sala->capacidade == it_x->at(4))
							{
								/* Se a sala comporta (possui créditos livres) o valor demandado */
								if( (it_Sala->credsLivres.at(dia) - 
									it_x->at(0)) >= 0)
								{
									// Subtraindo os créditos
									it_Sala->credsLivres.at(dia) -= it_x->at(0);

									// Setando a sala
									it_x->at(4) = it_Sala->getId();
									break; // Já aloquei, então posso parar
								}
							}
						}
					}
				}
			}
		}
	}
	// ------------------

	std::cout << "\nx\t -\t i\t d\t u\t s\t t" << std::endl;
	for(X___i_d_u_s_t::iterator it_x = vXpp.begin(); it_x != vXpp.end(); it_x++)
	{
		std::cout << it_x->at(0) << "\t\t" <<
			it_x->at(1) << "\t" <<
			it_x->at(2) << "\t" <<
			it_x->at(3) << "\t" <<
			it_x->at(4) << "\t" <<
			it_x->at(5) << std::endl;
	}

	// ==============================================================
	// ==============================================================

	// Fill the solution

	// Coletando os ids dos campus, das unidades e das salas existentes  na entrada e suas respectivas descricoes.
	std::map<int/*Id Campus*/,std::string/*Desc*/> input_cp_desc;
	std::map<int/*Id Unidade*/,std::pair<std::string/*Desc*/,int/*Id Campus*/> > input_und_desc;
	std::map<int/*Id Sala*/,std::pair<std::string/*Desc*/,int/*Id Unidade*/> > input_sala_desc;

	ITERA_GGROUP(it_cp,problemData->campi,Campus)
	{
		input_cp_desc[it_cp->getId()] = it_cp->codigo;

		ITERA_GGROUP(it_und,it_cp->unidades,Unidade)
		{
			input_und_desc[it_und->getId()] = std::make_pair(it_und->codigo,it_und->id_campus);

			ITERA_GGROUP(it_sala,it_und->salas,Sala)
			{
				input_sala_desc[it_sala->getId()] = std::make_pair(it_sala->codigo,it_sala->id_unidade);
			}
		}
	}

	// Coletando todos os campus considerados para a saida.
	GGroup<int> ids_cp;

	for(A___i_d_o::iterator it_a = a.begin(); it_a != a.end(); it_a++)
	{ ids_cp.add(problemData->refOfertas[it_a->first.at(2)]->campus_id); }

	// Coletando todas as unidades e salas consideradas para a saida.
	GGroup<int> ids_und;
	GGroup<int> ids_sala;

	for(X___i_d_u_s_t::iterator it_x = vXpp.begin(); it_x != vXpp.end(); it_x++)
	{
		int idU = it_x->at(3);
		int idS = it_x->at(4);

		if( idU != -1 )
		{ ids_und.add(idU); }

		if( idS != -1 )
		{ ids_sala.add(idS); }
	}

	// Adicionando os campus as classes de output
	for(GGroup<int>::iterator it_ids_cp = ids_cp.begin(); it_ids_cp != ids_cp.end(); it_ids_cp++)
	{
		int id = *it_ids_cp;
		problemSolution->addCampus(id,input_cp_desc[id]);
	}

	GGroup<AtendimentoCampus*>::iterator at_campus = problemSolution->atendimento_campus.begin();

	// Adicionando as unidades as classes de output
	for(GGroup<int>::iterator it_ids_und = ids_und.begin(); it_ids_und != ids_und.end(); it_ids_und++) {
		int id = *it_ids_und;
		at_campus->addUnidade(id,input_und_desc[id].first,input_und_desc[id].second);
	}

	GGroup<AtendimentoUnidade*>::iterator at_unidade = at_campus->atendimentos_unidades.begin();

	// Adicionando as salas as classes de output
	for(GGroup<int>::iterator it_ids_sala = ids_sala.begin(); it_ids_sala != ids_sala.end(); it_ids_sala++) {
		int id = *it_ids_sala;
		//problemSolution->atendimento_campus.begin()->atendimentos_unidades.begin()->addSala(id,input_sala_desc[id].first,input_sala_desc[id].second);
		at_unidade->addSala(id,input_sala_desc[id].first,input_sala_desc[id].second);
	}

	// >>>

	// ToDo: Fazer o msm que fiz aqui em baixo pro cp e und acima. nao sei se cp vai precisar

	// Adicionando os dias da semana as classes de output
	for(X___i_d_u_s_t::iterator it_x = vXpp.begin(); it_x != vXpp.end(); it_x++) {

		// Procurando pela sala certa para adicionar o dia da semana
		ITERA_GGROUP(it_at_cp,problemSolution->atendimento_campus,AtendimentoCampus){
			ITERA_GGROUP(it_at_und,it_at_cp->atendimentos_unidades,AtendimentoUnidade){
				ITERA_GGROUP(it_at_sala,it_at_und->atendimentos_salas,AtendimentoSala){
					if(it_at_sala->getId() == it_x->at(4))
					{
						int var_value = it_x->at(0);

						if( var_value != -1)
						{ 
							it_at_sala->addDiaSemana(it_x->at(5),"",it_x->at(4)); 
							break;
						}
					}
				}
			}
		}
	}

	// <<<

	// Configurando as solucoes

	std::map<std::vector<int/*<i,d,u,s,t>*/>,
		std::pair<std::pair<int/*valor var x - teo*/,int/*valor var x - prat*/>,
		GGroup<std::pair<int/*valor var a*/,int/*id Oferta*/> > > > solucao;

	for(X___i_d_u_s_t::iterator it_x = vXpp.begin(); it_x != vXpp.end(); it_x++)
	{
		int valor_var_x = it_x->at(0);

		std::vector<int/*<i,d,u,s,t>*/> chave(5);

		// Setando turma
		chave.at(0) = it_x->at(1);
		// Setando disciplina - sempre positiva
		chave.at(1) = (it_x->at(2) > 0 ? it_x->at(2): -it_x->at(2));
		// Setando unidade
		chave.at(2) = it_x->at(3);
		// Setando sala
		chave.at(3) = it_x->at(4);
		// Setando dia
		chave.at(4) = it_x->at(5);

		GGroup<std::pair<int/*valor var a*/,int/*id oferta*/> > atendimento_por_oferta;

		// Já considera o caso em que a disc tem o id negativo
		for(A___i_d_o::iterator it_a = a.begin(); it_a != a.end(); it_a++)
		{
			if(it_a->first.at(0) == chave.at(0) &&
				it_a->first.at(1) == it_x->at(2))
			{ atendimento_por_oferta.add(std::make_pair<int,int>(it_a->second,it_a->first.at(2))); }
		}

		Disciplina * disc = problemData->refDisciplinas[it_x->at(2)];
			
		if((disc->cred_teoricos > 0) && (disc->cred_praticos == 0)){
			if(solucao.find(chave) == solucao.end())
			{
				solucao[chave] =             
					std::make_pair(std::make_pair<int/*valor var x - teo*/,int/*valor var x - prat*/>
					(valor_var_x,0),atendimento_por_oferta);
			}
			else
			{
				solucao[chave].first.first += valor_var_x;

				GGroup<std::pair<int/*valor var a*/,int/*id oferta*/> >::iterator 
					it_atendimento_por_oferta = atendimento_por_oferta.begin();

				for(; it_atendimento_por_oferta != atendimento_por_oferta.end();
					it_atendimento_por_oferta++)
				{
					solucao[chave].second.add(*it_atendimento_por_oferta);
				}
			}
		}
		else  if((disc->cred_teoricos == 0) && (disc->cred_praticos > 0)){
			if(solucao.find(chave) == solucao.end())
			{
				solucao[chave] =
					std::make_pair(std::make_pair<int/*valor var x - teo*/,int/*valor var x - prat*/>
					(0,valor_var_x),atendimento_por_oferta);
			}
			else
			{
				solucao[chave].first.second += valor_var_x;

				GGroup<std::pair<int/*valor var a*/,int/*id oferta*/> >::iterator 
					it_atendimento_por_oferta = atendimento_por_oferta.begin();

				for(; it_atendimento_por_oferta != atendimento_por_oferta.end();
					it_atendimento_por_oferta++)
				{
					solucao[chave].second.add(*it_atendimento_por_oferta);
				}
			}
		}
	}

	// TRIEDA-482

	// Procurando a posição correta para inserir
	ITERA_GGROUP(it_at_cp,problemSolution->atendimento_campus,AtendimentoCampus)
	{
		int id_at_cp = it_at_cp->getId();

		ITERA_GGROUP(it_at_und,it_at_cp->atendimentos_unidades,AtendimentoUnidade)
		{
			int id_at_und = it_at_und->getId();

			ITERA_GGROUP(it_at_sala,it_at_und->atendimentos_salas,AtendimentoSala)
			{
				int id_at_sala = it_at_sala->getId();

				ITERA_GGROUP(it_at_dia_sem,it_at_sala->atendimentos_dias_semana,AtendimentoDiaSemana)
				{
					int id_at_dia_sem = it_at_dia_sem->key.second;

					std::map<std::vector<int/*<i,d,u,s,t>*/>,
						std::pair<std::pair<int/*valor var x - teo*/,int/*valor var x - prat*/>,
						GGroup<std::pair<int/*valor var a*/,int/*id Oferta*/> > > >::iterator

						it_solucao = solucao.begin();

					// Selecionando a(s) solução(ões) com a configuração em questão
					for(; it_solucao != solucao.end(); it_solucao++) 
					{
						GGroup<std::pair<int/*valor var a*/,int/*id oferta*/> >::iterator
							it_atendimento_por_oferta = it_solucao->second.second.begin();

						/* Para cada solução selecionada:
						registra-se um atendimento tatico para cada atendimento realizado */
						for(; it_atendimento_por_oferta != it_solucao->second.second.end();
							it_atendimento_por_oferta++ )
						{
							Campus * pt_campus_atendido = problemData->refOfertas[
								(*it_atendimento_por_oferta).second]->campus;

								if( pt_campus_atendido->getId() == id_at_cp &&
									it_solucao->first.at(2) == id_at_und &&
									it_solucao->first.at(3) == id_at_sala &&
									it_solucao->first.at(4) == id_at_dia_sem )
								{
									AtendimentoOferta * at_oferta = new AtendimentoOferta();

									std::stringstream oferta_str;
									oferta_str << (*it_atendimento_por_oferta).second;

									at_oferta->oferta_curso_campi_id = oferta_str.str();

									std::stringstream disc_str;
									disc_str << it_solucao->first.at(1);

									at_oferta->disciplina_id = disc_str.str();

									at_oferta->quantidade = (*it_atendimento_por_oferta).first;

									at_oferta->turma = it_solucao->first.at(0);

									AtendimentoTatico * at_tatico = new AtendimentoTatico();

									at_tatico->atendimento_oferta = at_oferta;

									at_tatico->qtde_creditos_teoricos = 
										it_solucao->second.first.first;

									at_tatico->qtde_creditos_praticos = 
										it_solucao->second.first.second;

									it_at_dia_sem->atendimentos_tatico.add(at_tatico);
								}
						}
					}
				}
			}
		}
	}

	for (vit = vHash.begin(); vit != vHash.end(); ++vit)
	{
		Variable* v = new Variable(vit->first);
		int col = vit->second;
		v->setValue(xSol[col]);
		if ( v->getValue() > 0.00001 )
		{
			sLoader.setFolgas(v);
		}
		delete v;
	}

#ifdef DEBUG
	if ( fout )
		fclose(fout);
#endif

#ifdef PRINT_CSV
	if (f_V_CREDITOS)
		fclose(f_V_CREDITOS);

	if(f_V_ALUNOS)
		fclose(f_V_ALUNOS);
#endif

	if ( xSol )
		delete[] xSol;
}

int SolverMIP::cria_variaveis()
{
	int num_vars = 0;

#ifdef PRINT_cria_variaveis
	int numVarsAnterior = 0;
#endif

	num_vars += cria_variavel_oferecimentos(); // o

#ifdef PRINT_cria_variaveis
	std::cout << "numVars \"o\": " << (num_vars - numVarsAnterior) << std::endl;
	numVarsAnterior = num_vars;
#endif

	num_vars += cria_variavel_creditos(); // x

#ifdef PRINT_cria_variaveis
	std::cout << "numVars \"x\": " << (num_vars - numVarsAnterior) << std::endl;
	numVarsAnterior = num_vars;
#endif

	num_vars += cria_variavel_abertura(); // z

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

	num_vars += cria_variavel_aloc_disciplina(); // y

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

	num_vars += cria_variavel_aloc_alunos(); // b

#ifdef PRINT_cria_variaveis
	std::cout << "numVars \"b\": " << (num_vars - numVarsAnterior) << std::endl;
	numVarsAnterior = num_vars;
#endif

	num_vars += cria_variavel_de_folga_dist_cred_dia_superior(); // fcp

#ifdef PRINT_cria_variaveis
	std::cout << "numVars \"fcp\": " << (num_vars - numVarsAnterior) << std::endl;
	numVarsAnterior = num_vars;
#endif

	num_vars += cria_variavel_de_folga_dist_cred_dia_inferior(); // fcm

#ifdef PRINT_cria_variaveis
	std::cout << "numVars \"fcm\": " << (num_vars - numVarsAnterior) << std::endl;
   //std::cout << "numVars \"fcm\": NAO ESTA SENDO CRIADA DEVIDO A NOVA MODELAGEM QUE O MARCELO FEZ." << std::endl;
	numVarsAnterior = num_vars;
#endif

	num_vars += cria_variavel_abertura_subbloco_de_blc_dia_campus(); // r

#ifdef PRINT_cria_variaveis
	std::cout << "numVars \"r\": " << (num_vars - numVarsAnterior) << std::endl;
	numVarsAnterior = num_vars;
#endif

	num_vars += cria_variavel_de_folga_aloc_alunos_curso_incompat(); // bs

#ifdef PRINT_cria_variaveis
	std::cout << "numVars \"bs\": " << (num_vars - numVarsAnterior) << std::endl;
	numVarsAnterior = num_vars;
#endif

	num_vars += cria_variavel_de_folga_demanda_disciplina(); // fd

#ifdef PRINT_cria_variaveis
	std::cout << "numVars \"fd\": " << (num_vars - numVarsAnterior) << std::endl;
	numVarsAnterior = num_vars;
#endif

	num_vars += cria_variavel_combinacao_divisao_credito(); //m

#ifdef PRINT_cria_variaveis
	std::cout << "numVars \"m\": " << (num_vars - numVarsAnterior) << std::endl;
	numVarsAnterior = num_vars;
#endif

	num_vars += cria_variavel_de_folga_combinacao_divisao_credito(); //fk

#ifdef PRINT_cria_variaveis
	std::cout << "numVars \"fk\": " << (num_vars - numVarsAnterior) << std::endl;
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
%DocEnd
/====================================================================*/

int SolverMIP::cria_variavel_creditos(void)
{
	int num_vars = 0;

	ITERA_GGROUP(itCampus,problemData->campi,Campus)
	{
		ITERA_GGROUP(itUnidade,itCampus->unidades,Unidade)
		{
			ITERA_GGROUP(itCjtSala,itUnidade->conjutoSalas,ConjuntoSala)
			{
				ITERA_GGROUP(itDisc,itCjtSala->getDiscsAssociadas(),Disciplina)
				{
					for(int turma=0;turma<itDisc->num_turmas;turma++)
					{
						GGroup<int/*Dias*/>::iterator itDiscSala_Dias =
							//problemData->discSala_Dias[std::make_pair<int,int>
                     problemData->disc_Conjutno_Salas__Dias[std::make_pair<int,int>
                     (itDisc->getId(),itCjtSala->getId())].begin();

						for(; itDiscSala_Dias != 
                     problemData->disc_Conjutno_Salas__Dias[std::make_pair<int,int>
							//problemData->discSala_Dias[std::make_pair<int,int>
							(itDisc->getId(),itCjtSala->getId())].end(); itDiscSala_Dias++)
							//for(int dia = 0; dia < 7; dia++)
						{
							Variable v;
							v.reset();
							v.setType(Variable::V_CREDITOS);

							v.setTurma(turma);            // i
							v.setDisciplina(*itDisc);     // d
							v.setUnidade(*itUnidade);     // u
							v.setSubCjtSala(*itCjtSala);  // tps  
							v.setDia(*itDiscSala_Dias);   // t
							//v.setDia(dia);

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

	// >>>

	// OLD !!

	//ITERA_GGROUP(it_campus,problemData->campi,Campus)
	//{
	//   ITERA_GGROUP(it_unidades,it_campus->unidades,Unidade)
	//   {
	//      ITERA_GGROUP(it_salas,it_unidades->salas,Sala)
	//      {
	//         ITERA_GGROUP(it_disc,it_salas->disciplinasAssociadas,Disciplina)
	//         {
	//            for(int turma=0;turma<it_disc->num_turmas;turma++)
	//            {
	//               for(int dia=0;dia<7;dia++)
	//               {
	//                  Variable v;
	//                  v.reset();
	//                  v.setType(Variable::V_CREDITOS);

	//                  v.setTurma(turma);            // i
	//                  v.setDisciplina(*it_disc);    // d
	//                  v.setUnidade(*it_unidades);   // u
	//                  v.setSala(*it_salas);         // s  
	//                  v.setDia(dia);                // t

	//                  if (vHash.find(v) == vHash.end())
	//                  {
	//                     vHash[v] = lp->getNumCols();

	//                     OPT_COL col(OPT_COL::VAR_INTEGRAL,0.0,0.0,24.0,
	//                        (char*)v.toString().c_str());

	//                     lp->newCol(col);

	//                     num_vars += 1;
	//                  }
	//               }
	//            }
	//         }
	//      }
	//   }
	//}

	// <<<

	return num_vars;
}

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

	ITERA_GGROUP(itCampus,problemData->campi,Campus)
	{
		ITERA_GGROUP(itUnidade,itCampus->unidades,Unidade)
		{
			ITERA_GGROUP(itCjtSala,itUnidade->conjutoSalas,ConjuntoSala)
			{
				ITERA_GGROUP(itDisc,itCjtSala->getDiscsAssociadas(),Disciplina)
				{
					for(int turma=0;turma<itDisc->num_turmas;turma++)
					{
						GGroup<int/*Dias*/>::iterator itDiscSala_Dias =
                     problemData->disc_Conjutno_Salas__Dias[std::make_pair<int,int>
							(itDisc->getId(),itCjtSala->getId())].begin();

						for(; itDiscSala_Dias !=
							problemData->disc_Conjutno_Salas__Dias[std::make_pair<int,int>
							(itDisc->getId(),itCjtSala->getId())].end(); itDiscSala_Dias++)
						{
							Variable v;
							v.reset();
							v.setType(Variable::V_OFERECIMENTO);

							v.setTurma(turma);            // i
							v.setDisciplina(*itDisc);     // d
							v.setUnidade(*itUnidade);     // u
							v.setSubCjtSala(*itCjtSala);  // tps  
							v.setDia(*itDiscSala_Dias);   // t

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

   //std::cout << "end" << std::endl;
   //getchar();

   return num_vars;
}

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
				{ problemData->demandas_campus[dc] = 0; }

				double ratioDem = ( it_disc->getDemandaTotal() - 
					problemData->demandas_campus[dc] ) 
					/ (1.0 * it_disc->getDemandaTotal());

				double coeff = alpha + gamma * ratioDem;

            //if(coeff <= 0)
            //{
            //   std::cout << "alpha: " << alpha << std::endl;
            //   std::cout << "gamma: " << gamma << std::endl;
            //   std::cout << std::endl;
            //   std::cout << "ratioDem: " << ratioDem << std::endl;
            //   std::cout << "it_disc->getDemandaTotal(): " << it_disc->getDemandaTotal() << std::endl;
            //   std::cout << "problemData->demandas_campus[dc]: " << problemData->demandas_campus[dc] << std::endl;
            //   getchar();
            //}

            //double coeff = 0;

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

/*====================================================================/
%DocBegin TRIEDA_LOAD_MODEL

%Var a_{i,d,oft}
%Desc 
    número de alunos de uma oferta $oft$ alocados para a $i$-ésima turma da disciplina $d$.

%DocEnd
/====================================================================*/

int SolverMIP::cria_variavel_alunos(void)
{
	int num_vars = 0;

	ITERA_GGROUP(itOferta,problemData->ofertas,Oferta)
	{
		GGroup<DisciplinaPeriodo>::iterator itPrdDisc = 
			itOferta->curriculo->disciplinas_periodo.begin();

		for(; itPrdDisc != itOferta->curriculo->disciplinas_periodo.end(); itPrdDisc++)
		{
			Disciplina * ptDisc = problemData->refDisciplinas[(*itPrdDisc).second];

			for(int turma = 0; turma < ptDisc->num_turmas; turma++)
			{
				Variable v;
				v.reset();
				v.setType(Variable::V_ALUNOS);

				v.setTurma(turma);               // i
				v.setDisciplina(ptDisc);         // d
				v.setOferta(*itOferta);          // oft

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

   ITERA_GGROUP(it_Oferta,problemData->ofertas,Oferta)
   {
      Campus * pt_Campus = it_Oferta->campus;

      Curso * pt_Curso = it_Oferta->curso;

      GGroup<DisciplinaPeriodo>::iterator it_Prd_Disc = 
         it_Oferta->curriculo->disciplinas_periodo.begin();

      for(; it_Prd_Disc != it_Oferta->curriculo->disciplinas_periodo.end(); it_Prd_Disc++)
      {
         Disciplina * pt_Disc = problemData->refDisciplinas[(*it_Prd_Disc).second];

         for(int turma=0;turma<pt_Disc->num_turmas;turma++)
         {
            Variable v;
            v.reset();
            v.setType(Variable::V_ALOC_ALUNO);

            v.setTurma(turma);            // i
            v.setDisciplina(pt_Disc);    // d
            v.setCurso(pt_Curso);       // c
            v.setCampus(pt_Campus);		 // cp

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

   //ITERA_GGROUP(it_campus,problemData->campi,Campus)
   //{
   //   ITERA_GGROUP(it_disc,problemData->disciplinas,Disciplina)
   //   {
   //      for(int turma=0;turma<it_disc->num_turmas;turma++)
   //      {
   //         ITERA_GGROUP(it_cursos,problemData->cursos,Curso)
   //         {
   //            Variable v;
   //            v.reset();
   //            v.setType(Variable::V_ALOC_ALUNO);

   //            v.setTurma(turma);            // i
   //            v.setDisciplina(*it_disc);    // d
   //            v.setCurso(*it_cursos);       // c
   //            v.setCampus(*it_campus);		 // cp

   //            if (vHash.find(v) == vHash.end())
   //            {
   //               vHash[v] = lp->getNumCols();

   //               OPT_COL col(OPT_COL::VAR_BINARY,0,0.0,1.0,
   //                  (char*)v.toString().c_str());

   //               lp->newCol(col);

   //               num_vars += 1;
   //            }
   //         }

   //      }
   //   }
   //}

	return num_vars;
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

int SolverMIP::cria_variavel_consecutivos(void)
{
   int num_vars = 0;

	ITERA_GGROUP(it_disc,problemData->disciplinas,Disciplina)
	{
		for(int turma=0;turma<it_disc->num_turmas;turma++)
		{
			GGroup<int>::iterator itDiasLetDisc =
				it_disc->diasLetivos.begin();

			for(; itDiasLetDisc != it_disc->diasLetivos.end(); itDiasLetDisc++ )
			{
				Variable v;
				v.reset();
				v.setType(Variable::V_DIAS_CONSECUTIVOS);

				v.setTurma(turma);            // i
				v.setDisciplina(*it_disc);    // d
				v.setDia(*itDiasLetDisc);     // t

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
	ITERA_GGROUP(it_bloco,problemData->blocos,BlocoCurricular)
	{
		ITERA_GGROUP(it_disc,it_bloco->disciplinas,Disciplina)
		{
			for(int i=0;i<it_disc->num_turmas;i++)
			{
				Variable v;
				v.reset();
				v.setType(Variable::V_MIN_CRED_SEMANA);
            
            v.setBloco(*it_bloco);
				v.setTurma(i);
				
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
	ITERA_GGROUP(it_bloco,problemData->blocos,BlocoCurricular)
	{
		ITERA_GGROUP(it_disc,it_bloco->disciplinas,Disciplina)
		{
			for(int i=0;i<it_disc->num_turmas;i++)
			{
				Variable v;
				v.reset();
				v.setType(Variable::V_MAX_CRED_SEMANA);
				
				v.setBloco(*it_bloco);
            v.setTurma(i);

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

/*====================================================================/
%DocBegin TRIEDA_LOAD_MODEL

%Var y_{i,d,tps,u} 
%Desc 
    indica que a turma $i$ da disciplina $d$ foi alocada em alguma sala do tipo $tps$ da unidade $u$.

%DocEnd
/====================================================================*/

int SolverMIP::cria_variavel_aloc_disciplina(void)
{
	/* Poderia criar as variaveis apenas qdo a disciplina for compativel com a 
	sala. Por exemplo, não criar uma variável qdo a disciplia demanda horarios 
	que a sala não dispõe.*/

	int num_vars = 0;

	ITERA_GGROUP(itCampus,problemData->campi,Campus)
	{
		ITERA_GGROUP(itUnidade,itCampus->unidades,Unidade)
		{
			ITERA_GGROUP(itCjtSala,itUnidade->conjutoSalas,ConjuntoSala) 
			{
				ITERA_GGROUP(itDisc,itCjtSala->getDiscsAssociadas(),Disciplina)
				{
					for(int turma=0;turma<itDisc->num_turmas;turma++)
					{
						Variable v;
						v.reset();
						v.setType(Variable::V_ALOC_DISCIPLINA);

                  v.setTurma(turma);            // i
                  v.setDisciplina(*itDisc);     // d
						v.setSubCjtSala(*itCjtSala);  // tps
                  v.setUnidade(*itUnidade);     // u

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

	// >>>

	// OLD

	//ITERA_GGROUP(it_campus,problemData->campi,Campus)
	//{
	//   ITERA_GGROUP(it_unidades,it_campus->unidades,Unidade)
	//   {
	//      ITERA_GGROUP(it_salas,it_unidades->salas,Sala) 
	//      {
	//         ITERA_GGROUP(it_disc,problemData->disciplinas,Disciplina)
	//         {
	//            for(int turma=0;turma<it_disc->num_turmas;turma++)
	//            {
	//               Variable v;
	//               v.reset();
	//               v.setType(Variable::V_ALOC_DISCIPLINA);

	//               v.setUnidade(*it_unidades);   // u
	//               v.setSala(*it_salas);         // s  
	//               v.setTurma(turma);            // i
	//               v.setDisciplina(*it_disc);    // d

	//               if (vHash.find(v) == vHash.end())
	//               {
	//                  vHash[v] = lp->getNumCols();

	//                  OPT_COL col(OPT_COL::VAR_BINARY,0.0,0.0,1.0,
	//                     (char*)v.toString().c_str());

	//                  lp->newCol(col);

	//                  num_vars += 1;
	//               }

	//            }
	//         }
	//      }
	//   }
	//}

	// <<<

	return num_vars;
}

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

   ITERA_GGROUP(it_bloco,problemData->blocos,BlocoCurricular)
   {
      Campus * campus = it_bloco->campus;

      ITERA_GGROUP_N_PT(it_Dias_Letivos,problemData->bloco_Campus_Dias
         [std::make_pair(it_bloco->getId(),campus->getId())],int)
      {
         Variable v;
         v.reset();
         v.setType(Variable::V_N_SUBBLOCOS);

         v.setBloco(*it_bloco);
         v.setDia(*it_Dias_Letivos);
         v.setCampus(campus);

         if (vHash.find(v) == vHash.end())
         {
            vHash[v] = lp->getNumCols();

            //OPT_COL col(OPT_COL::VAR_INTEGRAL,rho,0.0,4.0,(char*)v.toString().c_str());
            OPT_COL col(OPT_COL::VAR_INTEGRAL,rho,0.0,OPT_INF,(char*)v.toString().c_str());

            lp->newCol(col);

            num_vars += 1;
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

	ITERA_GGROUP(it_bloco,problemData->blocos,BlocoCurricular)
	{
		GGroup<int>::iterator itDiasLetBloco =
			it_bloco->diasLetivos.begin();

		for(; itDiasLetBloco != it_bloco->diasLetivos.end(); itDiasLetBloco++)
		{
			Variable v;
			v.reset();
			v.setType(Variable::V_N_ABERT_TURMA_BLOCO);

			v.setBloco(*it_bloco);
			v.setDia(*itDiasLetBloco);

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

	ITERA_GGROUP(itCampus,problemData->campi,Campus) 
	{
		ITERA_GGROUP(itUnidade,itCampus->unidades,Unidade)
		{
			ITERA_GGROUP(itCjtSala,itUnidade->conjutoSalas,ConjuntoSala)
			{
				ITERA_GGROUP(it_disc,problemData->disciplinas,Disciplina) 
				{
					for(int turma = 0; turma < it_disc->num_turmas; turma++)
					{
						GGroup<int>::iterator itDiasLetDisc =
							it_disc->diasLetivos.begin();

						for(; itDiasLetDisc != it_disc->diasLetivos.end(); itDiasLetDisc++ )
						{
							ITERA_GGROUP(it_fix,problemData->fixacoes,Fixacao)
							{
								if(it_fix->disciplina_id == it_disc->getId() 
									&& it_fix->dia_semana == *itDiasLetDisc
									/*&& itCjtSala->getId() == it_fix->sala->capacidade*/) 
								{
									Variable v;
									v.reset();
									v.setType(Variable::V_SLACK_DIST_CRED_DIA_SUPERIOR);

									v.setTurma(turma);
									v.setDisciplina(*it_disc);
									v.setDia(*itDiasLetDisc);
									v.setSubCjtSala(*itCjtSala);

									int idDisc = it_disc->getId();

									if (vHash.find(v) == vHash.end())
									{
										vHash[v] = lp->getNumCols();

										int cred_disc_dia = it_fix->disciplina->max_creds;

										OPT_COL col(OPT_COL::VAR_INTEGRAL,epsilon,0.0, cred_disc_dia, (char*)v.toString().c_str());

										lp->newCol(col);

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

	ITERA_GGROUP(itCampus,problemData->campi,Campus) 
	{
		ITERA_GGROUP(itUnidade,itCampus->unidades,Unidade)
		{
			ITERA_GGROUP(itCjtSala,itUnidade->conjutoSalas,ConjuntoSala)
			{
				ITERA_GGROUP(it_disc,problemData->disciplinas,Disciplina) 
				{
					for(int turma = 0; turma < it_disc->num_turmas; turma++)
					{
						GGroup<int>::iterator itDiasLetDisc =
							it_disc->diasLetivos.begin();

						for(; itDiasLetDisc != it_disc->diasLetivos.end(); itDiasLetDisc++ )
						{
							ITERA_GGROUP(it_fix,problemData->fixacoes,Fixacao)
							{
								if(it_fix->disciplina_id == it_disc->getId() 
									&& it_fix->dia_semana == *itDiasLetDisc
									/*&& itCjtSala->getId() == it_fix->sala->capacidade*/) 
								{
									Variable v;
									v.reset();
									v.setType(Variable::V_SLACK_DIST_CRED_DIA_INFERIOR);

									v.setTurma(turma);
									v.setDisciplina(*it_disc);
									v.setDia(*itDiasLetDisc);
									v.setSubCjtSala(*itCjtSala);

									if (vHash.find(v) == vHash.end())
									{
										vHash[v] = lp->getNumCols();

										int cred_disc_dia = it_fix->disciplina->min_creds;

										OPT_COL col(OPT_COL::VAR_INTEGRAL,epsilon,0.0, cred_disc_dia, (char*)v.toString().c_str());

										lp->newCol(col);

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

	ITERA_GGROUP(it_bloco,problemData->blocos,BlocoCurricular)
	{
		GGroup<int>::iterator itDia =
			it_bloco->campus->diasLetivos.begin();

		for(; itDia != it_bloco->campus->diasLetivos.end(); itDia++)
		{
			Variable v;
			v.reset();
			v.setType(Variable::V_ABERTURA_SUBBLOCO_DE_BLC_DIA_CAMPUS);

			v.setBloco(*it_bloco);
			v.setDia(*itDia);
			v.setCampus(it_bloco->campus);

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
					v.setType(Variable::V_SLACK_ALOC_ALUNOS_CURSO_INCOMPAT);

					v.setTurma(turma);            // i
					v.setDisciplina(*it_disc);    // d
					v.setCurso(*it_cursos);       // c
					v.setCampus(*it_campus);		 // cp

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

/*====================================================================/
%DocBegin TRIEDA_LOAD_MODEL

%Var fd_{d,oft} 
%Desc 
    variável de folga para a restrição "Capacidade alocada tem que 
	   permitir atender demanda da disciplina".

%DocEnd
/====================================================================*/

int SolverMIP::cria_variavel_de_folga_demanda_disciplina()
{
	int num_vars = 0;


	ITERA_GGROUP(itOferta,problemData->ofertas,Oferta)
	{
		GGroup<DisciplinaPeriodo>::iterator itPrdDisc = 
			itOferta->curriculo->disciplinas_periodo.begin();

		for(; itPrdDisc != itOferta->curriculo->disciplinas_periodo.end(); itPrdDisc++)
		{
			Variable v;
			v.reset();
			v.setType(Variable::V_SLACK_DEMANDA);

			v.setDisciplina(problemData->refDisciplinas[(*itPrdDisc).second]);    // d
			v.setOferta(*itOferta); // o

			if (vHash.find(v) == vHash.end())
			{
				vHash[v] = lp->getNumCols();

				OPT_COL col(OPT_COL::VAR_INTEGRAL,1000,0.0,10000.0,
					(char*)v.toString().c_str());

				lp->newCol(col);

				num_vars += 1;
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

int SolverMIP::cria_variavel_combinacao_divisao_credito(){
	
	int num_vars = 0;
	

	ITERA_GGROUP(itDisc,problemData->disciplinas,Disciplina)
	{
		for(int turma=0;turma<itDisc->num_turmas;turma++)
		{
			for(unsigned k = 0; k < itDisc->combinacao_divisao_creditos.size();k++)
			{ 
				
				Variable v;
				v.reset();
				v.setType(Variable::V_COMBINACAO_DIVISAO_CREDITO);
				v.setTurma(turma);            // i
				v.setDisciplina(*itDisc);     // d
				v.setK(k);	  // k
	
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

int SolverMIP::cria_variavel_de_folga_combinacao_divisao_credito(){

	int num_vars = 0;

	ITERA_GGROUP(itDisc,problemData->disciplinas,Disciplina)
	{
		for(int turma=0;turma<itDisc->num_turmas;turma++)
		{
			GGroup<int>::iterator itDiasLetDisc =
					itDisc->diasLetivos.begin();

			for(; itDiasLetDisc != itDisc->diasLetivos.end(); itDiasLetDisc++ )
			{
					Variable v;
					v.reset();
					v.setType(Variable::V_SLACK_COMBINACAO_DIVISAO_CREDITO);
					v.setTurma(turma);            // i
					v.setDisciplina(*itDisc);     // d
					v.setDia(*itDiasLetDisc);	  // t

					if (vHash.find(v) == vHash.end())
					{
						vHash[v] = lp->getNumCols();

						OPT_COL col(OPT_COL::VAR_INTEGRAL,psi,0.0,10000.0,
							(char*)v.toString().c_str());

						lp->newCol(col);

						num_vars += 1;
					}
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

#ifdef PRINT_cria_restricoes
	int numRestAnterior = 0;
#endif

	restricoes += cria_restricao_carga_horaria();				// Restricao 1.2.3

#ifdef PRINT_cria_restricoes
	std::cout << "numRest \"1.2.3\": " << (restricoes - numRestAnterior) << std::endl;
	numRestAnterior = restricoes;
#endif

	restricoes += cria_restricao_max_cred_sd();					// Restricao 1.2.4

#ifdef PRINT_cria_restricoes
	std::cout << "numRest \"1.2.4\": " << (restricoes - numRestAnterior) << std::endl;
	numRestAnterior = restricoes;
#endif

	restricoes += cria_restricao_min_cred_dd();					// Restricao 1.2.5

#ifdef PRINT_cria_restricoes
	std::cout << "numRest \"1.2.5\": " << (restricoes - numRestAnterior) << std::endl;
	numRestAnterior = restricoes;
#endif

	restricoes += cria_restricao_ativacao_var_o();					// Restricao 1.2.6

#ifdef PRINT_cria_restricoes
	std::cout << "numRest \"1.2.6\": " << (restricoes - numRestAnterior) << std::endl;
	numRestAnterior = restricoes;
#endif

	restricoes += cria_restricao_evita_sobreposicao();			// Restricao 1.2.7

#ifdef PRINT_cria_restricoes
	std::cout << "numRest \"1.2.7\": " << (restricoes - numRestAnterior) << std::endl;
	numRestAnterior = restricoes;
#endif

	restricoes += cria_restricao_disciplina_sala();				// Restricao 1.2.8

#ifdef PRINT_cria_restricoes
	std::cout << "numRest \"1.2.8\": " << (restricoes - numRestAnterior) << std::endl;
	numRestAnterior = restricoes;
#endif

	restricoes += cria_restricao_turma_sala();					// Restricao 1.2.9

#ifdef PRINT_cria_restricoes
	std::cout << "numRest \"1.2.9\": " << (restricoes - numRestAnterior) << std::endl;
	numRestAnterior = restricoes;
#endif

	restricoes += cria_restricao_evita_turma_disc_camp_d();		// Restricao 1.2.10

#ifdef PRINT_cria_restricoes
	std::cout << "numRest \"1.2.10\": " << (restricoes - numRestAnterior) << std::endl;
	numRestAnterior = restricoes;
#endif

	restricoes += cria_restricao_turmas_bloco();				// Restricao 1.2.11

#ifdef PRINT_cria_restricoes
	std::cout << "numRest \"1.2.11\": " << (restricoes - numRestAnterior) << std::endl;
	numRestAnterior = restricoes;
#endif

	restricoes += cria_restricao_max_cred_disc_bloco();			// Restricao 1.2.12

#ifdef PRINT_cria_restricoes
	std::cout << "numRest \"1.2.12\": " << (restricoes - numRestAnterior) << std::endl;
	numRestAnterior = restricoes;
#endif

	restricoes += cria_restricao_num_tur_bloc_dia_difunid();	// Restricao 1.2.13

#ifdef PRINT_cria_restricoes
	std::cout << "numRest \"1.2.13\": " << (restricoes - numRestAnterior) << std::endl;
	numRestAnterior = restricoes;
#endif

	restricoes += cria_restricao_lim_cred_diar_disc();			// Restricao 1.2.14

#ifdef PRINT_cria_restricoes
	std::cout << "numRest \"1.2.14\": " << (restricoes - numRestAnterior) << std::endl;
	numRestAnterior = restricoes;
#endif

	restricoes += cria_restricao_cap_aloc_dem_disc();			// Restricao 1.2.15

#ifdef PRINT_cria_restricoes
	std::cout << "numRest \"1.2.15\": " << (restricoes - numRestAnterior) << std::endl;
	numRestAnterior = restricoes;
#endif

	restricoes += cria_restricao_cap_sala_compativel_turma();	// Restricao 1.2.16

#ifdef PRINT_cria_restricoes
	std::cout << "numRest \"1.2.16\": " << (restricoes - numRestAnterior) << std::endl;
	numRestAnterior = restricoes;
#endif

	restricoes += cria_restricao_cap_sala_unidade();			// Restricao 1.2.17

#ifdef PRINT_cria_restricoes
	std::cout << "numRest \"1.2.17\": " << (restricoes - numRestAnterior) << std::endl;
	numRestAnterior = restricoes;
#endif

   //restricoes += cria_restricao_turma_disc_dias_consec();		// Restricao 1.2.18

#ifdef PRINT_cria_restricoes
   //std::cout << "numRest \"1.2.18\": " << (restricoes - numRestAnterior) << std::endl;
   std::cout << "numRest \"1.2.18\": NAO ESTA SENDO CRIADA DEVIDO A ERROS DE IMPLEMENTACAO - VER ToDo 12 (MARIO)" << std::endl;
   numRestAnterior = restricoes;
#endif

	restricoes += cria_restricao_min_creds_turm_bloco();		// Restricao 1.2.19

#ifdef PRINT_cria_restricoes
	std::cout << "numRest \"1.2.19\": " << (restricoes - numRestAnterior) << std::endl;
	numRestAnterior = restricoes;
#endif

	restricoes += cria_restricao_max_creds_turm_bloco();		// Restricao 1.2.20

#ifdef PRINT_cria_restricoes
	std::cout << "numRest \"1.2.20\": " << (restricoes - numRestAnterior) << std::endl;
	numRestAnterior = restricoes;
#endif

	restricoes += cria_restricao_aluno_curso_disc();			// Restricao 1.2.21

#ifdef PRINT_cria_restricoes
	std::cout << "numRest \"1.2.21\": " << (restricoes - numRestAnterior) << std::endl;
	numRestAnterior = restricoes;
#endif

	restricoes += cria_restricao_alunos_cursos_dif();			// Restricao 1.2.22

#ifdef PRINT_cria_restricoes
	std::cout << "numRest \"1.2.22\": " << (restricoes - numRestAnterior) << std::endl;
	numRestAnterior = restricoes;
#endif

	restricoes += cria_restricao_de_folga_dist_cred_dia();		// Restricao 1.2.23

#ifdef PRINT_cria_restricoes
	std::cout << "numRest \"1.2.23\": " << (restricoes - numRestAnterior) << std::endl;
   //std::cout << "numRest \"1.2.23\": NAO ESTA SENDO CRIADA DEVIDO A NOVA MODELAGEM QUE O MARCELO FEZ." << std::endl;
	numRestAnterior = restricoes;
#endif

	restricoes += cria_restricao_ativacao_var_r();						// Restricao 1.2.24

#ifdef PRINT_cria_restricoes
	std::cout << "numRest \"1.2.24\": " << (restricoes - numRestAnterior) << std::endl;
	numRestAnterior = restricoes;
#endif

	/**/
	restricoes += cria_restricao_limita_abertura_turmas();						// Restricao NOVA

#ifdef PRINT_cria_restricoes
	std::cout << "numRest \"1.2.25\": " << (restricoes - numRestAnterior) << std::endl;
	numRestAnterior = restricoes;
#endif
	/**/

	restricoes += cria_restricao_abre_turmas_em_sequencia();						// Restricao NOVA

#ifdef PRINT_cria_restricoes
	std::cout << "numRest \"1.2.26\": " << (restricoes - numRestAnterior) << std::endl;
	numRestAnterior = restricoes;
#endif

	restricoes += cria_restricao_divisao_credito();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest \"1.2.27\": " << (restricoes - numRestAnterior) << std::endl;
	numRestAnterior = restricoes;
#endif

	restricoes += cria_restricao_combinacao_divisao_credito();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest \"1.2.28\": " << (restricoes - numRestAnterior) << std::endl;
	numRestAnterior = restricoes;
#endif

   restricoes += cria_restricao_ativacao_var_y();

#ifdef PRINT_cria_restricoes
	std::cout << "numRest \"1.2.29\": " << (restricoes - numRestAnterior) << std::endl;
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

	char name[100];
	int nnz;

	Constraint c;

	Variable v;
	VariableHash::iterator it_v;

	ITERA_GGROUP(itCampus,problemData->campi,Campus)
	{
		ITERA_GGROUP(itDisciplina,problemData->disciplinas,Disciplina)
		{
			for(int turma = 0; turma < itDisciplina->num_turmas; turma++)
			{
				c.reset();
				c.setType(Constraint::C_CARGA_HORARIA);

				c.setCampus(*itCampus);
				c.setTurma(turma);
				c.setDisciplina(*itDisciplina);

				sprintf( name, "%s", c.toString().c_str() );

				if (cHash.find(c) != cHash.end()) continue;

				nnz = itCampus->totalSalas * 7;

				OPT_ROW row( nnz + 1, OPT_ROW::EQUAL , 0 , name );

				v.reset();
				v.setType(Variable::V_CREDITOS);

				// ---

				ITERA_GGROUP(itUnidade,itCampus->unidades,Unidade)
				{
					ITERA_GGROUP(itCjtSala,itUnidade->conjutoSalas,ConjuntoSala)
					{
						GGroup<int/*Dias*/>::iterator itDiscSala_Dias =
                     problemData->disc_Conjutno_Salas__Dias[std::make_pair<int,int>
							//problemData->discSala_Dias[std::make_pair<int,int>
							(itDisciplina->getId(),itCjtSala->getId())].begin();

						for(; itDiscSala_Dias != 
                     problemData->disc_Conjutno_Salas__Dias[std::make_pair<int,int>
                     //problemData->discSala_Dias[std::make_pair<int,int>
							(itDisciplina->getId(),itCjtSala->getId())].end(); itDiscSala_Dias++)
						{

							v.setTurma(turma);
							v.setDisciplina(*itDisciplina);
							v.setUnidade(*itUnidade);
							v.setSubCjtSala(*itCjtSala);
							v.setDia(*itDiscSala_Dias);                   

							it_v = vHash.find(v);
							if( it_v != vHash.end() )
							{ row.insert(it_v->second, 1.0); }
						}
					}
				}

				// ---

				v.reset();
				v.setType(Variable::V_ABERTURA);

				v.setCampus(*itCampus);
				v.setDisciplina(*itDisciplina);
				v.setTurma(turma);

				it_v = vHash.find(v);
				if( it_v != vHash.end() )
				{
					row.insert(it_v->second,
						-(itDisciplina->cred_praticos + 
						itDisciplina->cred_teoricos));
				}

				// ---

				if(row.getnnz() != 0)
				{
					cHash[ c ] = lp->getNumRows();

					lp->addRow(row);
					restricoes++;
				}

				//lp->addRow(row);
				//restricoes++;
			}
		}
	}


	// >>>

	// OLD

	//ITERA_GGROUP(it_campus,problemData->campi,Campus) {
	//   ITERA_GGROUP(it_disciplina,problemData->disciplinas,Disciplina) {
	//      for(int i=0;i<it_disciplina->num_turmas;i++) {

	//         c.reset();
	//         c.setType(Constraint::C_CARGA_HORARIA);

	//         c.setCampus(*it_campus);
	//         c.setTurma(i);
	//         c.setDisciplina(*it_disciplina);

	//         sprintf( name, "%s", c.toString().c_str() );

	//         if (cHash.find(c) != cHash.end()) continue;

	//         cHash[ c ] = lp->getNumRows();

	//         nnz = it_campus->totalSalas * 7;

	//         OPT_ROW row( nnz + 1, OPT_ROW::EQUAL , 0 , name );

	//         v.reset();
	//         v.setType(Variable::V_CREDITOS);

	//         // ---

	//         ITERA_GGROUP(it_unidade,it_campus->unidades,Unidade) {
	//            ITERA_GGROUP(it_sala,it_unidade->salas,Sala) {
	//               for(int dia=0;dia<7;dia++) {
	//                  v.setUnidade(*it_unidade);
	//                  v.setSala(*it_sala);

	//                  v.setTurma(i);
	//                  v.setDisciplina(*it_disciplina);

	//                  v.setDia(dia);

	//                  it_v = vHash.find(v);
	//                  if( it_v != vHash.end() ){
	//                     row.insert(it_v->second, 1.0);

	//                  }
	//               }
	//            }
	//         }

	//         // ---

	//         v.reset();
	//         v.setType(Variable::V_ABERTURA);

	//         v.setCampus(*it_campus);
	//         v.setTurma(i);
	//         v.setDisciplina(*it_disciplina);

	//         it_v = vHash.find(v);
	//         if( it_v != vHash.end() ) {
	//            row.insert(it_v->second, 
	//               -(it_disciplina->cred_praticos + 
	//               it_disciplina->cred_teoricos));
	//         }

	//         // ---

	//         lp->addRow(row);
	//         restricoes++;
	//      }
	//   }
	//}

	// <<<

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

int SolverMIP::cria_restricao_max_cred_sd(void)
{
	int restricoes = 0;

	char name[100];
	int nnz;
	Constraint c;
	Variable v;
	VariableHash::iterator it_v;

	ITERA_GGROUP(itCampus,problemData->campi,Campus)
	{
		ITERA_GGROUP(itUnidade,itCampus->unidades,Unidade)
		{
			ITERA_GGROUP(itCjtSala,itUnidade->conjutoSalas,ConjuntoSala)
			{
				GGroup<int>::iterator itDiasLetCjtSala =
					itCjtSala->diasLetivos.begin();

				for(; itDiasLetCjtSala != itCjtSala->diasLetivos.end(); itDiasLetCjtSala++ )
				{
					c.reset();
					c.setType(Constraint::C_MAX_CREDITOS_SD);

					c.setUnidade(*itUnidade);
					c.setSubCjtSala(*itCjtSala);
					c.setDia(*itDiasLetCjtSala);

					sprintf( name, "%s", c.toString().c_str() );

					if (cHash.find(c) != cHash.end()) continue;

					nnz = 0;
					ITERA_GGROUP(itDisc,problemData->disciplinas,Disciplina)
					{ nnz += itDisc->num_turmas; }

					int maxCreds = itCjtSala->maxCredsPermitidos(*itDiasLetCjtSala);

					OPT_ROW row( nnz, OPT_ROW::LESS , maxCreds, name );

					ITERA_GGROUP(itDisc,problemData->disciplinas,Disciplina)
					{
						for(int turma = 0; turma < itDisc->num_turmas; turma++)
						{
							v.reset();
							v.setType(Variable::V_CREDITOS);

							v.setTurma(turma);
							v.setUnidade(*itUnidade);
							v.setDisciplina(*itDisc);
							v.setSubCjtSala(*itCjtSala);
							v.setDia(*itDiasLetCjtSala);

							it_v = vHash.find(v);
							if( it_v != vHash.end() )
							{ row.insert(it_v->second, 1.0); }
						}
					}

					if(row.getnnz() != 0)
					{
						cHash[ c ] = lp->getNumRows();

						lp->addRow(row);
						restricoes++;
					}
				}
			}
		}
	}

	// >>>

	// OLD

	//ITERA_GGROUP(it_campus,problemData->campi,Campus) {

	//   ITERA_GGROUP(it_unidade,it_campus->unidades,Unidade) {
	//      ITERA_GGROUP(it_sala,it_unidade->salas,Sala) {
	//         for(int dia=0;dia<7;dia++) {
	//            c.reset();
	//            c.setType(Constraint::C_MAX_CREDITOS_SD);

	//            c.setUnidade(*it_unidade);
	//            c.setSala(*it_sala);
	//            c.setDia(dia);

	//            sprintf( name, "%s", c.toString().c_str() );

	//            if (cHash.find(c) != cHash.end()) continue;

	//            cHash[ c ] = lp->getNumRows();

	//            nnz = 0;
	//            ITERA_GGROUP(it_disc,problemData->disciplinas,Disciplina) {
	//               nnz += it_disc->num_turmas;
	//            }

	//            int max_creds = it_sala->max_creds(dia);

	//            OPT_ROW row( nnz, OPT_ROW::LESS , max_creds, name );

	//            ITERA_GGROUP(it_disc,problemData->disciplinas,Disciplina) {
	//               for(int i=0;i<it_disc->num_turmas;i++) {

	//                  v.reset();
	//                  v.setType(Variable::V_CREDITOS);

	//                  v.setUnidade(*it_unidade);
	//                  v.setSala(*it_sala);

	//                  v.setDisciplina(*it_disc);
	//                  v.setTurma(i);

	//                  v.setDia(dia);

	//                  it_v = vHash.find(v);
	//                  if( it_v != vHash.end() )
	//                  {
	//                     row.insert(it_v->second, 1.0);
	//                  }
	//               }
	//            }

	//            lp->addRow(row);
	//            restricoes++;
	//         }
	//      }
	//   }
	//}

	// <<<

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
	char name[100];
	int nnz;

	Constraint c;

	Variable v;
	VariableHash::iterator it_v;

	ITERA_GGROUP(itCampus,problemData->campi,Campus)
	{
		ITERA_GGROUP(itUnidade,itCampus->unidades,Unidade)
		{
			ITERA_GGROUP(itCjtSala,itUnidade->conjutoSalas,ConjuntoSala)
			{
				ITERA_GGROUP(itDisc,problemData->disciplinas,Disciplina)
			 {
				 GGroup<int/*Dias*/>::iterator itDiscSala_Dias =
                problemData->disc_Conjutno_Salas__Dias[std::make_pair<int,int>
                //problemData->discSala_Dias[std::make_pair<int,int>
					 (itDisc->getId(),itCjtSala->getId())].begin();

				 for(; itDiscSala_Dias !=
					 problemData->disc_Conjutno_Salas__Dias[std::make_pair<int,int>
                //problemData->discSala_Dias[std::make_pair<int,int>
					 (itDisc->getId(),itCjtSala->getId())].end(); itDiscSala_Dias++)
				 {

					 for(int turma = 0; turma < itDisc->num_turmas; turma++)
					 {
						 c.reset();

						 c.setType(Constraint::C_MIN_CREDITOS_DD);

						 c.setUnidade(*itUnidade);
						 c.setSubCjtSala(*itCjtSala);
						 c.setDia(*itDiscSala_Dias);
						 c.setDisciplina(*itDisc);
						 c.setTurma(turma);

						 sprintf( name, "%s", c.toString().c_str() ); 

						 if (cHash.find(c) != cHash.end()) continue;

						 nnz = 2;

						 OPT_ROW row( nnz, OPT_ROW::LESS , 0.0, name );

						 v.reset();
						 v.setType(Variable::V_OFERECIMENTO);

						 v.setTurma(turma);
						 v.setDisciplina(*itDisc);
						 v.setUnidade(*itUnidade);
						 v.setSubCjtSala(*itCjtSala);
						 v.setDia(*itDiscSala_Dias);

						 it_v = vHash.find(v);
						 if( it_v != vHash.end() )
						 {
							 row.insert(it_v->second,itDisc->min_creds);
							 //row.insert(it_v->second, 1.0); 
							 // FIXME
							 /* Minimo de um crédito, se é oferecida, só 
							 para forçar o oferecimento */
						 }

						 v.reset();
						 v.setType(Variable::V_CREDITOS);

						 v.setTurma(turma);
						 v.setDisciplina(*itDisc);
						 v.setUnidade(*itUnidade);
						 v.setSubCjtSala(*itCjtSala);
						 v.setDia(*itDiscSala_Dias);

						 it_v = vHash.find(v);
						 if( it_v != vHash.end() )
						 { row.insert(it_v->second, -1.0); }

						 if(row.getnnz() != 0)
						 {
							 cHash[ c ] = lp->getNumRows();

							 lp->addRow(row);
							 restricoes++;
						 }
					 }
				 }
				}
			}
		}
	}

	//ITERA_GGROUP(it_campus,problemData->campi,Campus) {

	//   ITERA_GGROUP(it_unidade,it_campus->unidades,Unidade) {
	//      ITERA_GGROUP(it_sala,it_unidade->salas,Sala) {
	//         for(int dia=0;dia<7;dia++) {
	//            ITERA_GGROUP(it_disc,problemData->disciplinas,Disciplina) {
	//               for(int i=0;i<it_disc->num_turmas;i++) {

	//                  c.reset();
	//                  //c.setType(Constraint::C_VAR_O);
	//                  c.setType(Constraint::C_MIN_CREDITOS_DD);
	//                  c.setUnidade(*it_unidade);
	//                  c.setSala(*it_sala);

	//                  c.setDisciplina(*it_disc);
	//                  c.setTurma(i);

	//                  c.setDia(dia);

	//                  sprintf( name, "%s", c.toString().c_str() ); 

	//                  if (cHash.find(c) != cHash.end()) continue;

	//                  cHash[ c ] = lp->getNumRows();

	//                  nnz = 2;

	//                  OPT_ROW row( nnz, OPT_ROW::LESS , 0.0, name );

	//                  v.reset();
	//                  v.setType(Variable::V_OFERECIMENTO);

	//                  v.setUnidade(*it_unidade);
	//                  v.setSala(*it_sala);

	//                  v.setDisciplina(*it_disc);
	//                  v.setTurma(i);

	//                  v.setDia(dia);

	//                  it_v = vHash.find(v);
	//                  if( it_v != vHash.end() )
	//                  {
	//                     row.insert(it_v->second, 1.0); 
	//                     // FIXME
	//                     /* Minimo de um crédito, se é oferecida, só 
	//                     para forçar o oferecimento */
	//                  }

	//                  v.reset();
	//                  v.setType(Variable::V_CREDITOS);

	//                  v.setUnidade(*it_unidade);
	//                  v.setSala(*it_sala);

	//                  v.setDisciplina(*it_disc);
	//                  v.setTurma(i);

	//                  v.setDia(dia);

	//                  it_v = vHash.find(v);
	//                  if( it_v != vHash.end() )
	//                  {
	//                     row.insert(it_v->second, -1.0);
	//                  }

	//                  lp->addRow(row);
	//                  restricoes++;
	//               }
	//            }
	//         }
	//      }
	//   }
	//}


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
	char name[100];
	int nnz;

	Constraint c;

	Variable v;
	VariableHash::iterator it_v;

	ITERA_GGROUP(itCampus,problemData->campi,Campus)
	{
		ITERA_GGROUP(itUnidade,itCampus->unidades,Unidade)
		{
			ITERA_GGROUP(itCjtSala,itUnidade->conjutoSalas,ConjuntoSala)
			{
				ITERA_GGROUP(itDisc,problemData->disciplinas,Disciplina)
				{
					GGroup<int/*Dias*/>::iterator itDiscSala_Dias =
						problemData->disc_Conjutno_Salas__Dias[std::make_pair<int,int>
                  //problemData->discSala_Dias[std::make_pair<int,int>
						(itDisc->getId(),itCjtSala->getId())].begin();

					for(; itDiscSala_Dias !=
						problemData->disc_Conjutno_Salas__Dias[std::make_pair<int,int>
                  //problemData->discSala_Dias[std::make_pair<int,int>
						(itDisc->getId(),itCjtSala->getId())].end(); itDiscSala_Dias++)
				 {
					 for(int turma = 0; turma < itDisc->num_turmas; turma++)
					 {
						 c.reset();
						 c.setType(Constraint::C_VAR_O);

						 c.setUnidade(*itUnidade);
						 c.setSubCjtSala(*itCjtSala);
						 c.setDia(*itDiscSala_Dias);
						 c.setDisciplina(*itDisc);
						 c.setTurma(turma);

						 sprintf( name, "%s", c.toString().c_str() ); 

						 if (cHash.find(c) != cHash.end()) continue;

						 nnz = 2;

						 OPT_ROW row( nnz, OPT_ROW::GREATER , 0.0, name );

						 v.reset();
						 v.setType(Variable::V_OFERECIMENTO);

						 v.setTurma(turma);
						 v.setDisciplina(*itDisc);
						 v.setUnidade(*itUnidade);
						 v.setSubCjtSala(*itCjtSala);
						 v.setDia(*itDiscSala_Dias);

						 it_v = vHash.find(v);
						 if( it_v != vHash.end() )
						 {
							 row.insert(it_v->second, (itDisc->cred_praticos + 
								 itDisc->cred_teoricos));
						 }

						 v.reset();
						 v.setType(Variable::V_CREDITOS);

						 v.setTurma(turma);
						 v.setDisciplina(*itDisc);
						 v.setUnidade(*itUnidade);
						 v.setSubCjtSala(*itCjtSala);
						 v.setDia(*itDiscSala_Dias);

						 it_v = vHash.find(v);
						 if( it_v != vHash.end() )
						 {
							 row.insert(it_v->second, -1.0);
						 }

						 if(row.getnnz() != 0)
						 {
							 cHash[ c ] = lp->getNumRows();

							 lp->addRow(row);
							 restricoes++;
						 }

						 //lp->addRow(row);
						 //restricoes++;
					 }
					}
				}
			}
		}
	}

	//ITERA_GGROUP(it_campus,problemData->campi,Campus) {

	//   ITERA_GGROUP(it_unidade,it_campus->unidades,Unidade) {
	//      ITERA_GGROUP(it_sala,it_unidade->salas,Sala) {
	//         for(int dia=0;dia<7;dia++) {
	//            ITERA_GGROUP(it_disc,problemData->disciplinas,Disciplina) {
	//               for(int i=0;i<it_disc->num_turmas;i++) {

	//                  c.reset();
	//                  c.setType(Constraint::C_VAR_O);

	//                  c.setUnidade(*it_unidade);
	//                  c.setSala(*it_sala);

	//                  c.setDisciplina(*it_disc);
	//                  c.setTurma(i);

	//                  c.setDia(dia);

	//                  sprintf( name, "%s", c.toString().c_str() ); 

	//                  if (cHash.find(c) != cHash.end()) continue;

	//                  cHash[ c ] = lp->getNumRows();

	//                  nnz = 2;

	//                  OPT_ROW row( nnz, OPT_ROW::GREATER , 0.0, name );

	//                  v.reset();
	//                  v.setType(Variable::V_OFERECIMENTO);

	//                  v.setUnidade(*it_unidade);
	//                  v.setSala(*it_sala);

	//                  v.setDisciplina(*it_disc);
	//                  v.setTurma(i);

	//                  v.setDia(dia);

	//                  it_v = vHash.find(v);
	//                  if( it_v != vHash.end() )
	//                  {
	//                     row.insert(it_v->second, (it_disc->cred_praticos + 
	//                        it_disc->cred_teoricos));
	//                  }

	//                  v.reset();
	//                  v.setType(Variable::V_CREDITOS);

	//                  v.setUnidade(*it_unidade);
	//                  v.setSala(*it_sala);

	//                  v.setDisciplina(*it_disc);
	//                  v.setTurma(i);

	//                  v.setDia(dia);

	//                  it_v = vHash.find(v);
	//                  if( it_v != vHash.end() )
	//                  {
	//                     row.insert(it_v->second, -1.0);
	//                  }

	//                  lp->addRow(row);
	//                  restricoes++;
	//               }
	//            }
	//         }
	//      }
	//   }
	//}

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

	char name[100];
	int nnz;
	Constraint c;
	Variable v;
	VariableHash::iterator it_v;

	ITERA_GGROUP(itCampus,problemData->campi,Campus)
	{
		ITERA_GGROUP(itUnidade,itCampus->unidades,Unidade)
		{
			GGroup<int>::iterator itDiasLetUnid =
				itUnidade->diasLetivos.begin();

			for(; itDiasLetUnid != itUnidade->diasLetivos.end(); itDiasLetUnid++)
			{
				ITERA_GGROUP(itDisc,problemData->disciplinas,Disciplina)
				{
					for(int turma = 0; turma < itDisc->num_turmas; turma++)
					{
						c.reset();
						c.setType(Constraint::C_EVITA_SOBREPOSICAO_TD);

						c.setUnidade(*itUnidade);
						c.setDia(*itDiasLetUnid);
						c.setDisciplina(*itDisc);
						c.setTurma(turma);

						sprintf( name, "%s", c.toString().c_str() ); 

						if (cHash.find(c) != cHash.end()) continue;

						nnz = itUnidade->salas.size();

						OPT_ROW row( nnz, OPT_ROW::LESS , 1.0, name );

						ITERA_GGROUP(itCjtSala,itUnidade->conjutoSalas,ConjuntoSala) 
						{
							v.reset();
							v.setType(Variable::V_OFERECIMENTO);

							v.setTurma(turma);
							v.setDisciplina(*itDisc);
							v.setUnidade(*itUnidade);
							v.setSubCjtSala(*itCjtSala);
							v.setDia(*itDiasLetUnid);

							it_v = vHash.find(v);
							if( it_v != vHash.end() )
							{ row.insert(it_v->second, 1.0); }
						}

						if(row.getnnz() != 0)
						{
							cHash[ c ] = lp->getNumRows();

							lp->addRow(row);
							restricoes++;
						}
					}
				}
			}
		}
	}

	//ITERA_GGROUP(it_campus,problemData->campi,Campus) {

	//   ITERA_GGROUP(it_unidade,it_campus->unidades,Unidade) {
	//      for(int dia=0;dia<7;dia++) {
	//         ITERA_GGROUP(it_disc,problemData->disciplinas,Disciplina) {
	//            for(int i=0;i<it_disc->num_turmas;i++) {

	//               c.reset();
	//               c.setType(Constraint::C_EVITA_SOBREPOSICAO_TD);

	//               c.setUnidade(*it_unidade);
	//               c.setDia(dia);
	//               c.setDisciplina(*it_disc);
	//               c.setTurma(i);

	//               sprintf( name, "%s", c.toString().c_str() ); 

	//               if (cHash.find(c) != cHash.end()) continue;

	//               cHash[ c ] = lp->getNumRows();

	//               nnz = it_unidade->salas.size();

	//               /*
	//               int creds = it_disc->cred_praticos + 
	//               it_disc->cred_teoricos;
	//               */

	//               OPT_ROW row( nnz, OPT_ROW::LESS , 1.0, name );

	//               ITERA_GGROUP(it_sala,it_unidade->salas,Sala) 
	//               {

	//                  v.reset();
	//                  v.setType(Variable::V_OFERECIMENTO);

	//                  v.setUnidade(*it_unidade);
	//                  v.setSala(*it_sala);

	//                  v.setDisciplina(*it_disc);
	//                  v.setTurma(i);

	//                  v.setDia(dia);

	//                  it_v = vHash.find(v);
	//                  if( it_v != vHash.end() )
	//                  {
	//                     //row.insert(it_v->second, creds);
	//                     row.insert(it_v->second, 1.0);
	//                  }
	//               }

	//               lp->addRow(row);
	//               restricoes++;
	//            }
	//         }
	//      }
	//   }
	//}

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
	char name[100];
	int nnz;
	Constraint c;
	Variable v;
	VariableHash::iterator it_v;

	ITERA_GGROUP(itCampus,problemData->campi,Campus)
	{
		ITERA_GGROUP(itUnidade,itCampus->unidades,Unidade)
		{
			ITERA_GGROUP(itCjtSala,itUnidade->conjutoSalas,ConjuntoSala)
			{
				ITERA_GGROUP(itDisc,problemData->disciplinas,Disciplina)
				{
					for(int turma = 0; turma < itDisc->num_turmas; turma++)
					{
						c.reset();
						c.setType(Constraint::C_TURMA_DISCIPLINA_SALA);

						c.setUnidade(*itUnidade);
						c.setSubCjtSala(*itCjtSala);
						c.setDisciplina(*itDisc);
						c.setTurma(turma);

						sprintf( name, "%s", c.toString().c_str() ); 

						if (cHash.find(c) != cHash.end()) continue;

						nnz = 8;
						OPT_ROW row( nnz, OPT_ROW::LESS , 0.0, name );

						GGroup<int/*Dias*/>::iterator itDiscSala_Dias =
							problemData->disc_Conjutno_Salas__Dias[std::make_pair<int,int>
                     //problemData->discSala_Dias[std::make_pair<int,int>
							(itDisc->getId(),itCjtSala->getId())].begin();

						for(; itDiscSala_Dias !=                     
                     problemData->disc_Conjutno_Salas__Dias[std::make_pair<int,int>
							//problemData->discSala_Dias[std::make_pair<int,int>
							(itDisc->getId(),itCjtSala->getId())].end(); itDiscSala_Dias++)
						{

							v.reset();
							v.setType(Variable::V_OFERECIMENTO);

							v.setTurma(turma);
							v.setDisciplina(*itDisc);
							v.setUnidade(*itUnidade);
							v.setSubCjtSala(*itCjtSala);
							v.setDia(*itDiscSala_Dias);

							it_v = vHash.find(v);
							if( it_v != vHash.end() )
							{ row.insert(it_v->second, 1.0); }
						}

						v.reset();
						v.setType(Variable::V_ALOC_DISCIPLINA);

						v.setTurma(turma);
						v.setDisciplina(*itDisc);
						v.setUnidade(*itUnidade);
						v.setSubCjtSala(*itCjtSala);

						it_v = vHash.find(v);
						if( it_v != vHash.end() )
						{ row.insert(it_v->second, -7.0); }

						if(row.getnnz() != 0)
						{
							cHash[ c ] = lp->getNumRows();

							lp->addRow(row);
							restricoes++;
						}
						

						//lp->addRow(row);
						//restricoes++;
					}
				}
			}
		}
	}

	//ITERA_GGROUP(it_campus,problemData->campi,Campus) {
	//   ITERA_GGROUP(it_u,it_campus->unidades,Unidade) {
	//      ITERA_GGROUP(it_sala,it_u->salas,Sala) {
	//         ITERA_GGROUP(it_disc,problemData->disciplinas,Disciplina) {
	//            for(int i=0;i<it_disc->num_turmas;++i) {
	//               c.reset();
	//               c.setType(Constraint::C_TURMA_DISCIPLINA_SALA);
	//               c.setUnidade(*it_u);
	//               c.setSala(*it_sala);
	//               c.setDisciplina(*it_disc);
	//               c.setTurma(i);

	//               sprintf( name, "%s", c.toString().c_str() ); 

	//               if (cHash.find(c) != cHash.end()) continue;

	//               cHash[ c ] = lp->getNumRows();

	//               nnz = 8;
	//               OPT_ROW row( nnz, OPT_ROW::LESS , 0.0, name );

	//               for(int t=0;t<7;t++) {
	//                  v.reset();
	//                  v.setDia(t);
	//                  v.setDisciplina(*it_disc);
	//                  v.setUnidade(*it_u);
	//                  v.setSala(*it_sala);
	//                  v.setTurma(i);
	//                  v.setType(Variable::V_OFERECIMENTO);

	//                  it_v = vHash.find(v);
	//                  if( it_v != vHash.end() )
	//                  {
	//                     row.insert(it_v->second, 1.0);
	//                  }
	//               }
	//               v.reset();
	//               v.setTurma(i);
	//               v.setDisciplina(*it_disc);
	//               v.setUnidade(*it_u);
	//               v.setSala(*it_sala);
	//               v.setType(Variable::V_ALOC_DISCIPLINA);

	//               it_v = vHash.find(v);
	//               if( it_v != vHash.end() )
	//               {
	//                  row.insert(it_v->second, -7.0);
	//               }
	//               lp->addRow(row);
	//               restricoes++;
	//            }
	//         }
	//      }
	//   }
	//}

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

	ITERA_GGROUP(itDisc,problemData->disciplinas,Disciplina) 
	{
		for(int turma = 0; turma < itDisc->num_turmas; turma++)
		{
			c.reset();
			c.setType(Constraint::C_TURMA_SALA);
			c.setDisciplina(*itDisc);
			c.setTurma(turma);

			sprintf( name, "%s", c.toString().c_str() ); 

			if (cHash.find(c) != cHash.end()) continue;

         // Pode ser menor esse valor. Na verdade, ele tem que ser igual ao total de conjuntos de salas.
			nnz = problemData->totalSalas;
			OPT_ROW row( nnz, OPT_ROW::LESS , 1.0, name );

			ITERA_GGROUP(itCampus,problemData->campi,Campus)
			{
				ITERA_GGROUP(itUnidade,itCampus->unidades,Unidade) 
				{
					ITERA_GGROUP(itCjtSala,itUnidade->conjutoSalas,ConjuntoSala)
					{
						v.reset();
						v.setType(Variable::V_ALOC_DISCIPLINA);

						v.setTurma(turma);
						v.setDisciplina(*itDisc);
						v.setUnidade(*itUnidade);
						v.setSubCjtSala(*itCjtSala);

						it_v = vHash.find(v);
						if( it_v != vHash.end() )
						{ row.insert(it_v->second, 1.0); }
					}
				}
			}

			if(row.getnnz() != 0)
			{
				cHash[ c ] = lp->getNumRows();

				lp->addRow(row);
				restricoes++;
			}

			//lp->addRow(row);
			//restricoes++;
		}
	}

	//ITERA_GGROUP(it_disc,problemData->disciplinas,Disciplina) {
	//   for(int i=0;i<it_disc->num_turmas;++i) {
	//      c.reset();
	//      c.setType(Constraint::C_TURMA_SALA);
	//      c.setDisciplina(*it_disc);
	//      c.setTurma(i);

	//      sprintf( name, "%s", c.toString().c_str() ); 

	//      if (cHash.find(c) != cHash.end()) continue;

	//      cHash[ c ] = lp->getNumRows();

	//      nnz = problemData->totalSalas;
	//      OPT_ROW row( nnz, OPT_ROW::LESS , 1.0, name );

	//      ITERA_GGROUP(it_campus,problemData->campi,Campus) {
	//         ITERA_GGROUP(it_u,it_campus->unidades,Unidade) {
	//            ITERA_GGROUP(it_sala,it_u->salas,Sala) {
	//               v.reset();
	//               v.setType(Variable::V_ALOC_DISCIPLINA);
	//               v.setTurma(i);
	//               v.setDisciplina(*it_disc);
	//               v.setUnidade(*it_u);
	//               v.setSala(*it_sala);

	//               it_v = vHash.find(v);
	//               if( it_v != vHash.end() )
	//               {
	//                  row.insert(it_v->second, 1.0);
	//               }
	//            }
	//         }
	//      }
	//      lp->addRow(row);
	//      restricoes++;
	//   }
	//}

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

			//cHash[ c ] = lp->getNumRows();

			nnz = problemData->totalSalas;
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

			if(row.getnnz() != 0)
			{
				cHash[ c ] = lp->getNumRows();

				lp->addRow(row);
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
	char name[100];
	int nnz;
	Constraint c;
	Variable v;
	VariableHash::iterator it_v;

	ITERA_GGROUP(itCampus,problemData->campi,Campus)
	{
		GGroup<int>::iterator itDiasLetCampus =
			itCampus->diasLetivos.begin();

		for(; itDiasLetCampus != itCampus->diasLetivos.end(); itDiasLetCampus++ )
		{
			ITERA_GGROUP(itBloco,problemData->blocos,BlocoCurricular)
			{
				c.reset();
				c.setType(Constraint::C_TURMAS_BLOCO);
				c.setBloco(*itBloco);
				c.setDia(*itDiasLetCampus);
				c.setCampus(*itCampus);

				sprintf( name, "%s", c.toString().c_str() ); 

				if (cHash.find(c) != cHash.end()) continue;

				//cHash[ c ] = lp->getNumRows();

				nnz = 100; /*FIX-ME*/
               //itBloco->total_turmas * problemData->totalSalas;

				OPT_ROW row( (nnz+1), OPT_ROW::LESS , 0.0, name );
            //OPT_ROW row( OPT_ROW::LESS , 0.0, name );

				ITERA_GGROUP(itDisc,itBloco->disciplinas,Disciplina)
				{
					ITERA_GGROUP(itUnidade,itCampus->unidades,Unidade)
					{
						ITERA_GGROUP(itCjtSala,itUnidade->conjutoSalas,ConjuntoSala)
						{
							for(int turma = 0; turma < itDisc->num_turmas; turma++)
							{
								v.reset();
								v.setType(Variable::V_OFERECIMENTO);
								v.setTurma(turma);
								v.setDisciplina(*itDisc);
								v.setUnidade(*itUnidade);
								v.setSubCjtSala(*itCjtSala);
								v.setDia(*itDiasLetCampus);

								it_v = vHash.find(v);
								if( it_v != vHash.end() )
								{ row.insert(it_v->second, 1.0); }
							}
						}
					}
				}

				v.reset();
				v.setType(Variable::V_N_SUBBLOCOS);
				v.setBloco(*itBloco);
				v.setDia(*itDiasLetCampus);
				v.setCampus(*itCampus);

				it_v = vHash.find(v);
				if( it_v != vHash.end() )
				{ row.insert(it_v->second, -9999.0); }
				/* Provavelmente esta restrição é inútil */

				if(row.getnnz() != 0)
				{
					cHash[ c ] = lp->getNumRows();

					lp->addRow(row);
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
	char name[100];
	int nnz;
	Constraint c;
	Variable v;
	VariableHash::iterator it_v;

	ITERA_GGROUP(itCampus,problemData->campi,Campus)
	{
		GGroup<int>::iterator itDiasLetCampus =
			itCampus->diasLetivos.begin();

		for(; itDiasLetCampus != itCampus->diasLetivos.end(); itDiasLetCampus++ )
		{
			ITERA_GGROUP(itBloco,problemData->blocos,BlocoCurricular)
			{
				c.reset();
				c.setType(Constraint::C_MAX_CRED_DISC_BLOCO);
				c.setBloco(*itBloco);
				c.setDia(*itDiasLetCampus);
				c.setCampus(*itCampus);

				sprintf( name, "%s", c.toString().c_str() ); 

				if (cHash.find(c) != cHash.end()) continue;

				nnz =  100; /*FIX-ME*/
               //itBloco->total_turmas * problemData->totalSalas;

				OPT_ROW row( nnz + 1, OPT_ROW::LESS , 0.0, name );

				/* Variavel responsavel por armazenar o maximo de creditos disponiveis 
				dentre todas as salas para um dado dia. */
				int maxCredsSalaDia = 0;

				ITERA_GGROUP(itDisc,itBloco->disciplinas,Disciplina)
				{
					ITERA_GGROUP(itUnidade,itCampus->unidades,Unidade)
					{
						ITERA_GGROUP(itCjtSala,itUnidade->conjutoSalas,ConjuntoSala)
						{
							maxCredsSalaDia = (maxCredsSalaDia < itCjtSala->credsMaiorSala(*itDiasLetCampus) ?
								itCjtSala->credsMaiorSala(*itDiasLetCampus) : maxCredsSalaDia);

							for(int turma = 0; turma < itDisc->num_turmas; turma++)
							{
								v.reset();
								v.setType(Variable::V_CREDITOS);
								v.setTurma(turma);
								v.setDisciplina(*itDisc);
								v.setUnidade(*itUnidade);
								v.setSubCjtSala(*itCjtSala);
								v.setDia(*itDiasLetCampus);

								it_v = vHash.find(v);
								if( it_v != vHash.end() )
								{ row.insert(it_v->second, 1.0); }
							}
						}
					}
				}

				v.reset();
				v.setType(Variable::V_N_SUBBLOCOS);
				v.setBloco(*itBloco);
				v.setDia(*itDiasLetCampus);
				v.setCampus(*itCampus);

            it_v = vHash.find(v);
            if( it_v != vHash.end() )
               //{ row.insert(it_v->second, -24.0); /* #Warning: FIXME */ }
            { row.insert(it_v->second, -maxCredsSalaDia); }
            /* Descobrir valor de H_t */

				if(row.getnnz() != 0)
				{
					cHash[ c ] = lp->getNumRows();

					lp->addRow(row);
					restricoes++;
				}

				//lp->addRow(row);
				//++restricoes;
			}
		}
	}

	//ITERA_GGROUP(it_campus,problemData->campi,Campus) {
	//   for(int t=0;t<7;t++) {
	//      ITERA_GGROUP(it_bloco,problemData->blocos,BlocoCurricular) {
	//         c.reset();
	//         c.setType(Constraint::C_MAX_CRED_DISC_BLOCO);
	//         c.setBloco(*it_bloco);
	//         c.setDia(t);
	//         c.setCampus(*it_campus);

	//         sprintf( name, "%s", c.toString().c_str() ); 

	//         if (cHash.find(c) != cHash.end()) continue;

	//         cHash[ c ] = lp->getNumRows();

	//         nnz = it_bloco->total_turmas * 
	//            problemData->totalSalas;

	//         OPT_ROW row( nnz + 1, OPT_ROW::LESS , 0.0, name );

	//         ITERA_GGROUP(it_disc,it_bloco->disciplinas,Disciplina) {
	//            ITERA_GGROUP(it_u,it_campus->unidades,Unidade) {
	//               ITERA_GGROUP(it_sala,it_u->salas,Sala) {
	//                  for(int i=0;i<it_disc->num_turmas;++i) {
	//                     v.reset();
	//                     v.setType(Variable::V_CREDITOS);
	//                     v.setTurma(i);
	//                     v.setDisciplina(*it_disc);
	//                     v.setUnidade(*it_u);
	//                     v.setSala(*it_sala);
	//                     v.setDia(t);

	//                     it_v = vHash.find(v);
	//                     if( it_v != vHash.end() )
	//                     {
	//                        row.insert(it_v->second, 1.0);
	//                     }
	//                  }
	//               }
	//            }
	//         }
	//         v.reset();
	//         v.setType(Variable::V_N_SUBBLOCOS);
	//         v.setBloco(*it_bloco);
	//         v.setDia(t);
	//         v.setCampus(*it_campus);

	//         it_v = vHash.find(v);
	//         if( it_v != vHash.end() )
	//         {
	//            row.insert(it_v->second, -24.0); // #Warning: FIXME
	//         }
	//         /* Descobrir valor de H_t */

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
	char name[100];
	int nnz;

	Constraint c;

	Variable v;
	VariableHash::iterator it_v;

	ITERA_GGROUP(it_bloco,problemData->blocos,BlocoCurricular) {

		GGroup<int>::iterator itDiasLetBloco =
			it_bloco->diasLetivos.begin();

		for(; itDiasLetBloco != it_bloco->diasLetivos.end(); itDiasLetBloco++ )
		{
			c.reset();
			c.setType(Constraint::C_NUM_TUR_BLOC_DIA_DIFUNID);

			c.setBloco(*it_bloco);
			c.setDia(*itDiasLetBloco);

			sprintf( name, "%s", c.toString().c_str() ); 

			//if (cHash.find(c) != cHash.end()) continue;

			cHash[ c ] = lp->getNumRows();

			nnz = problemData->campi.size();

			OPT_ROW row( nnz + 1, OPT_ROW::LESS , 1.0 , 
				name );

			ITERA_GGROUP(it_cp,problemData->campi,Campus) {
				v.reset();
				v.setType(Variable::V_ABERTURA_SUBBLOCO_DE_BLC_DIA_CAMPUS);

				v.setBloco(*it_bloco);
				v.setDia(*itDiasLetBloco);
				v.setCampus(*it_cp);

				it_v = vHash.find(v);
				if( it_v != vHash.end() )
				{
					row.insert(it_v->second, 1.0);
				}
			}

			v.reset();
			v.setType(Variable::V_N_ABERT_TURMA_BLOCO);

			v.setBloco(*it_bloco);
			v.setDia(*itDiasLetBloco);

			it_v = vHash.find(v);
			if( it_v != vHash.end() )
			{
				row.insert(it_v->second, -1.0);
			}

			if(row.getnnz() != 0)
			{
				cHash[ c ] = lp->getNumRows();

				lp->addRow(row);
				restricoes++;
			}

			//lp->addRow(row);
			//++restricoes;
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

	char name[100];
	int nnz;
	Constraint c;
	Variable v;
	VariableHash::iterator it_v;

	ITERA_GGROUP(itCampus,problemData->campi,Campus)
	{
		ITERA_GGROUP(itUnidade,itCampus->unidades,Unidade)
		{
			ITERA_GGROUP(itCjtSala,itUnidade->conjutoSalas,ConjuntoSala)
			{
				ITERA_GGROUP(itDisc,problemData->disciplinas,Disciplina)
			 {
				 GGroup<int/*Dias*/>::iterator itDiscSala_Dias =
                problemData->disc_Conjutno_Salas__Dias[std::make_pair<int,int>
					 //problemData->discSala_Dias[std::make_pair<int,int>
					 (itDisc->getId(),itCjtSala->getId())].begin();

				 for(; itDiscSala_Dias != 
                problemData->disc_Conjutno_Salas__Dias[std::make_pair<int,int>
					 //problemData->discSala_Dias[std::make_pair<int,int>
					 (itDisc->getId(),itCjtSala->getId())].end(); itDiscSala_Dias++)
				 {
					 for(int turma = 0; turma < itDisc->num_turmas; turma++)
					 {
						 c.reset();
						 c.setType(Constraint::C_LIM_CRED_DIAR_DISC);
						 c.setTurma(turma);
						 c.setDisciplina(*itDisc);
						 c.setUnidade(*itUnidade);
						 c.setSubCjtSala(*itCjtSala);
						 c.setDia(*itDiscSala_Dias);

						 sprintf( name, "%s", c.toString().c_str() ); 

						 if (cHash.find(c) != cHash.end()) continue;

						 nnz = 1;

						 OPT_ROW row( 1, OPT_ROW::LESS , itCjtSala->credsMaiorSala(*itDiscSala_Dias) , name );

						 v.reset();
						 v.setType(Variable::V_CREDITOS);
						 v.setTurma(turma);
						 v.setDisciplina(*itDisc);
						 v.setUnidade(*itUnidade);
						 v.setSubCjtSala(*itCjtSala);
						 v.setDia(*itDiscSala_Dias);

						 it_v = vHash.find(v);
						 if( it_v != vHash.end() )
						 { row.insert(it_v->second, 1.0); }

						 if(row.getnnz() != 0)
						 {
							 cHash[ c ] = lp->getNumRows();

							 lp->addRow(row);
							 restricoes++;
						 }
					 }
				 }
				}
			}
		}
	}

	//ITERA_GGROUP(it_campus,problemData->campi,Campus) {
	//   ITERA_GGROUP(it_u,it_campus->unidades,Unidade) {
	//      ITERA_GGROUP(it_sala,it_u->salas,Sala) {
	//         for(int t=0;t<7;t++) {
	//            ITERA_GGROUP(it_disc,problemData->disciplinas,Disciplina) {
	//               for(int i=0;i<it_disc->num_turmas;i++) {
	//                  c.reset();
	//                  c.setType(Constraint::C_LIM_CRED_DIAR_DISC);
	//                  c.setTurma(i);
	//                  c.setDisciplina(*it_disc);
	//                  c.setUnidade(*it_u);
	//                  c.setSala(*it_sala);
	//                  c.setDia(t);

	//                  sprintf( name, "%s", c.toString().c_str() ); 

	//                  if (cHash.find(c) != cHash.end()) continue;

	//                  cHash[ c ] = lp->getNumRows();

	//                  nnz = 1;

	//                  OPT_ROW row( 1, OPT_ROW::LESS , it_disc->max_creds , 
	//                     name );

	//                  v.reset();
	//                  v.setType(Variable::V_CREDITOS);
	//                  v.setTurma(i);
	//                  v.setDisciplina(*it_disc);
	//                  v.setUnidade(*it_u);
	//                  v.setSala(*it_sala);
	//                  v.setDia(t);

	//                  it_v = vHash.find(v);
	//                  if( it_v != vHash.end() )
	//                  {
	//                     row.insert(it_v->second, 1.0);

	//                     lp->addRow(row);
	//                     ++restricoes;
	//                  }

	//                  //lp->addRow(row);
	//                  //++restricoes;
	//               }
	//            }
	//         }
	//      }
	//   }
	//}

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
	char name[100];
	int nnz;
	Constraint c;
	Variable v;
	VariableHash::iterator it_v;

	ITERA_GGROUP(itOferta,problemData->ofertas,Oferta)
	{
		GGroup<DisciplinaPeriodo>::iterator itPrdDisc = 
			itOferta->curriculo->disciplinas_periodo.begin();

		for(; itPrdDisc != itOferta->curriculo->disciplinas_periodo.end(); itPrdDisc++)
		{ 
			Disciplina * ptDisc = problemData->refDisciplinas[(*itPrdDisc).second];

			c.reset();
			c.setType(Constraint::C_CAP_ALOC_DEM_DISC);
			c.setOferta(*itOferta);
			c.setDisciplina(ptDisc);

			sprintf( name, "%s", c.toString().c_str() ); 

			if (cHash.find(c) != cHash.end()) continue;

			nnz = ptDisc->num_turmas;

			int rhs = 0;

			// Calculando P_{d,o}
			ITERA_GGROUP(itDem,problemData->demandas,Demanda)
			{
				if (itDem->disciplina->getId() == ptDisc->getId() &&
					itDem->oferta_id == itOferta->getId())
				{
					rhs += itDem->quantidade;
				}
			}

			OPT_ROW row( nnz , OPT_ROW::EQUAL , rhs , name );

			for(int turma = 0; turma < ptDisc->num_turmas; turma++)
			{
				v.reset();
				v.setType(Variable::V_ALUNOS);
				v.setTurma(turma);
				v.setDisciplina(ptDisc);
				v.setOferta(*itOferta);

				it_v = vHash.find(v);
				if( it_v != vHash.end() )
				{ row.insert(it_v->second, 1.0); }
			}

			v.reset();
			v.setType(Variable::V_SLACK_DEMANDA);
			v.setDisciplina(ptDisc);
			v.setOferta(*itOferta);

			it_v = vHash.find(v);
			if( it_v != vHash.end() )
			{ row.insert(it_v->second, 1.0); }

			if(row.getnnz() != 0)
			{
				cHash[ c ] = lp->getNumRows();

				lp->addRow(row);
				restricoes++;
			}
		}
	}

	//ITERA_GGROUP(it_campus,problemData->campi,Campus) {
	//   ITERA_GGROUP(it_disc,problemData->disciplinas,Disciplina) {
	//      ITERA_GGROUP(it_curso,problemData->cursos,Curso) {
	//         c.reset();
	//         c.setType(Constraint::C_CAP_ALOC_DEM_DISC);
	//         c.setCampus(*it_campus);
	//         c.setDisciplina(*it_disc);
	//         c.setCurso(*it_curso);

	//         sprintf( name, "%s", c.toString().c_str() ); 

	//         if (cHash.find(c) != cHash.end()) continue;

	//         //cHash[ c ] = lp->getNumRows();

	//         nnz = it_disc->num_turmas;

	//         int rhs = 0;
	//         ITERA_GGROUP(it_dem,problemData->demandas,Demanda) {
	//            if (it_dem->disciplina->getId() == it_disc->getId() &&
	//               it_dem->oferta->curso->getId() == it_curso->getId() &&
	//               it_dem->oferta->campus->getId() == it_campus->getId())
	//            {
	//               rhs += it_dem->quantidade;
	//            }
	//         }

	//         OPT_ROW row( nnz , OPT_ROW::EQUAL , rhs , name );

	//         for(int i=0;i<it_disc->num_turmas;++i) {
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
	char name[100];
	int nnz;
	Constraint c;
	Variable v;
	VariableHash::iterator it_v;

	ITERA_GGROUP(itCampus,problemData->campi,Campus)
	{
		//ITERA_GGROUP(itCurso,problemData->cursos,Curso)
		//{
		ITERA_GGROUP(itDisc,problemData->disciplinas,Disciplina)
		{
			for(int turma = 0; turma < itDisc->num_turmas; turma++)
			{
				c.reset();
				c.setType(Constraint::C_CAP_SALA_COMPATIVEL_TURMA);
				c.setCampus(*itCampus);
				c.setDisciplina(*itDisc);
				c.setTurma(turma);

				// >>>     
				sprintf( name, "%s", c.toString().c_str() ); 

				//std::string auxCons = c.toString().c_str();
				//std::stringstream auxCons;
				//auxCons << c.toString();
				//auxCons << "_";
				//auxCons << lp->getNumRows();
				//sprintf( name, "%s", auxCons.str().c_str() ); 

				if (cHash.find(c) != cHash.end()) continue;
				// <<<

				nnz = problemData->ofertas.size() * problemData->cursos.size() +
					itCampus->totalSalas * 7;

				OPT_ROW row( nnz , OPT_ROW::LESS , 0.0 , name );

				GGroup<Oferta*>::iterator itOferta = 
					problemData->ofertasDisc[itDisc->getId()].begin();

				for(; itOferta != problemData->ofertasDisc[itDisc->getId()].end(); itOferta++)
				{
					//if(itOferta->curso_id == itCurso->getId())
					//{
					v.reset();
					v.setType(Variable::V_ALUNOS);
					v.setTurma(turma);
					v.setDisciplina(*itDisc);
					v.setOferta(*itOferta);

					it_v = vHash.find(v);
					if( it_v != vHash.end() )
					{ row.insert(it_v->second, 1.0); }
					//}
				}

				ITERA_GGROUP(itUnidade,itCampus->unidades,Unidade)
				{
					ITERA_GGROUP(itCjtSala,itUnidade->conjutoSalas,ConjuntoSala)
					{
						GGroup<int/*Dias*/>::iterator itDiscSala_Dias =
							problemData->disc_Conjutno_Salas__Dias[std::make_pair<int,int>
                     //problemData->discSala_Dias[std::make_pair<int,int>
							(itDisc->getId(),itCjtSala->getId())].begin();

						for(; itDiscSala_Dias !=                     
							problemData->disc_Conjutno_Salas__Dias[std::make_pair<int,int>
                     //problemData->discSala_Dias[std::make_pair<int,int>
							(itDisc->getId(),itCjtSala->getId())].end(); itDiscSala_Dias++)
				  { 
					  v.reset();
					  v.setType(Variable::V_OFERECIMENTO);
					  v.setTurma(turma);
					  v.setDisciplina(*itDisc);
					  v.setUnidade(*itUnidade);
					  v.setSubCjtSala(*itCjtSala);
					  v.setDia(*itDiscSala_Dias);

					  it_v = vHash.find(v);
					  if( it_v != vHash.end() )
					  { row.insert(it_v->second, -itCjtSala->capTotalSalas()); }
						}
					}
				}

				if(row.getnnz() != 0)
				{
					cHash[ c ] = lp->getNumRows();

					lp->addRow(row);
					restricoes++;
				}
			}
		}
		//}
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
   //getchar();

	int restricoes = 0;

	char name[100];
	int nnz;
	Constraint c;
	Variable v;
	VariableHash::iterator it_v;

   //std::cout << "ANTES(disc_Conjutno_Salas__Dias): " << problemData->disc_Conjutno_Salas__Dias.size() << std::endl;
   //std::cout << "ANTES(problemData->ofertasDisc): " << problemData->ofertasDisc.size() << std::endl;

   ITERA_GGROUP(itCampus,problemData->campi,Campus)
   {
      ITERA_GGROUP(itUnidade,itCampus->unidades,Unidade)
      {
         ITERA_GGROUP(itCjtSala,itUnidade->conjutoSalas,ConjuntoSala)
         {
            ITERA_GGROUP(itDisc,problemData->disciplinas,Disciplina)
            {
               for(int turma = 0; turma < itDisc->num_turmas; turma++)
               {
                  GGroup<int/*Dias*/>::iterator itDiscSala_Dias =                     
                     problemData->disc_Conjutno_Salas__Dias[std::make_pair<int,int>
                     //problemData->discSala_Dias[std::make_pair<int,int>
                     (itDisc->getId(),itCjtSala->getId())].begin();

                  for(; itDiscSala_Dias !=
                     problemData->disc_Conjutno_Salas__Dias[std::make_pair<int,int>
                     //problemData->discSala_Dias[std::make_pair<int,int>
                     (itDisc->getId(),itCjtSala->getId())].end(); itDiscSala_Dias++)
                  {
                     c.reset();
                     c.setType(Constraint::C_CAP_SALA_UNIDADE);
                     c.setUnidade(*itUnidade);
                     c.setSubCjtSala(*itCjtSala);
                     c.setDisciplina(*itDisc);
                     c.setTurma(turma);
                     c.setCampus(*itCampus);
                     c.setDia(*itDiscSala_Dias);

                     sprintf( name, "%s", c.toString().c_str() ); 

                     if (cHash.find(c) != cHash.end()) continue;

                     //nnz = problemData->ofertas.size() * problemData->cursos.size() + 2;
                     nnz = 200;

							int rhs = itCjtSala->capTotalSalas() + 1000;//itUnidade->maiorSala;

							OPT_ROW row( nnz, OPT_ROW::LESS , rhs , name );

                     GGroup<Oferta*>::iterator itOferta = 
                        problemData->ofertasDisc[itDisc->getId()].begin();

                     // ---

                     for(; itOferta != problemData->ofertasDisc[itDisc->getId()].end(); itOferta++)
                     {
                        v.reset();
                        v.setType(Variable::V_ALUNOS);

                        v.setTurma(turma);
                        v.setDisciplina(*itDisc);
                        v.setOferta(*itOferta);

                        it_v = vHash.find(v);
                        if( it_v != vHash.end() )
                        { 
                           row.insert(it_v->second, 1.0);
                        }
                     }

                     // ---

                     v.reset();
                     v.setType(Variable::V_OFERECIMENTO);
                     v.setTurma(turma);
                     v.setDisciplina(*itDisc);
                     v.setUnidade(*itUnidade);
                     v.setSubCjtSala(*itCjtSala);
                     v.setDia(*itDiscSala_Dias);

                     it_v = vHash.find(v);
                     if( it_v != vHash.end() )
                     { 
                        //row.insert(it_v->second, itUnidade->maiorSala);
                        row.insert(it_v->second, 1000);
                     }

                     // ---

                     if(row.getnnz() != 0)
                     {
                        //std::cout << "Size Row: " << row.getnnz() << std::endl;
                        cHash[ c ] = lp->getNumRows();

								lp->addRow(row);
								restricoes++;
							}
						}
					}
				}
			}
		}
	}

   //std::cout << "DEPOIS(disc_Conjutno_Salas__Dias): " << problemData->disc_Conjutno_Salas__Dias.size() << std::endl;
   //std::cout << "DEPOIS(problemData->ofertasDisc): " << problemData->ofertasDisc.size() << std::endl;

   //getchar();

 ////  ITERA_GGROUP(itCampus,problemData->campi,Campus)
 ////  {
 ////     ITERA_GGROUP(itUnidade,itCampus->unidades,Unidade)
 ////     {
 ////        ITERA_GGROUP(itCjtSala,itUnidade->conjutoSalas,ConjuntoSala)
 ////        {
 ////           ITERA_GGROUP(itDisc,problemData->disciplinas,Disciplina)
 ////           {
 ////              for(int turma = 0; turma < itDisc->num_turmas; turma++)
 ////              {
 ////                 GGroup<int/*Dias*/>::iterator itDiscSala_Dias =                     
 ////                    problemData->disc_Conjutno_Salas__Dias[std::make_pair<int,int>
 ////                    //problemData->discSala_Dias[std::make_pair<int,int>
 ////                    (itDisc->getId(),itCjtSala->getId())].begin();

 ////                 for(; itDiscSala_Dias !=
 ////                    problemData->disc_Conjutno_Salas__Dias[std::make_pair<int,int>
 ////                    //problemData->discSala_Dias[std::make_pair<int,int>
 ////                    (itDisc->getId(),itCjtSala->getId())].end(); itDiscSala_Dias++)
 ////                 {
 ////                    c.reset();
 ////                    c.setType(Constraint::C_CAP_SALA_UNIDADE);
 ////                    c.setUnidade(*itUnidade);
 ////                    c.setSubCjtSala(*itCjtSala);
 ////                    c.setDisciplina(*itDisc);
 ////                    c.setTurma(turma);
 ////                    c.setCampus(*itCampus);
 ////                    c.setDia(*itDiscSala_Dias);

	////						sprintf( name, "%s", c.toString().c_str() ); 

	////						if (cHash.find(c) != cHash.end()) continue;

	////						nnz = problemData->ofertas.size() * problemData->cursos.size() + 1;

	////						int rhs = itCjtSala->capTotalSalas() + itUnidade->maiorSala;

	////						OPT_ROW row( nnz, OPT_ROW::LESS , rhs , name );

	////						GGroup<Oferta*>::iterator itOferta = 
	////							problemData->ofertasDisc[itDisc->getId()].begin();

 ////                    // ---

 ////                    for(; itOferta != problemData->ofertasDisc[itDisc->getId()].end(); itOferta++)
 ////                    {
 ////                       v.reset();
 ////                       v.setType(Variable::V_ALUNOS);

	////							v.setTurma(turma);
	////							v.setDisciplina(*itDisc);
	////							v.setOferta(*itOferta);

	////							it_v = vHash.find(v);
	////							if( it_v != vHash.end() )
	////							{ row.insert(it_v->second, 1.0); }
	////						}

 ////                    // ---

 ////                    v.reset();
 ////                    v.setType(Variable::V_OFERECIMENTO);
 ////                    v.setTurma(turma);
 ////                    v.setDisciplina(*itDisc);
 ////                    v.setUnidade(*itUnidade);
 ////                    v.setSubCjtSala(*itCjtSala);
 ////                    v.setDia(*itDiscSala_Dias);

 ////                    it_v = vHash.find(v);
 ////                    if( it_v != vHash.end() )
 ////                    { row.insert(it_v->second, itUnidade->maiorSala); }

 ////                    // ---

 ////                    if(row.getnnz() != 0)
 ////                    {
 ////                       cHash[ c ] = lp->getNumRows();

	////							lp->addRow(row);
	////							restricoes++;
	////						}

	////					}
	////				}
	////			}
	////		}
	////	}
	////}

   //ITERA_GGROUP(itCampus,problemData->campi,Campus)
   //{
   //   ITERA_GGROUP(itUnidade,itCampus->unidades,Unidade)
   //   {
   //      ITERA_GGROUP(itCjtSala,itUnidade->conjutoSalas,ConjuntoSala)
   //      {
   //         ITERA_GGROUP(itDisc,problemData->disciplinas,Disciplina)
   //         {
   //            for(int turma = 0; turma < itDisc->num_turmas; turma++)
   //            {
   //               GGroup<int/*Dias*/>::iterator itDiscSala_Dias =
   //                  problemData->discSala_Dias[std::make_pair<int,int>
   //                  (itDisc->getId(),itCjtSala->getId())].begin();

   //               for(; itDiscSala_Dias != 
   //                  problemData->discSala_Dias[std::make_pair<int,int>
   //                  (itDisc->getId(),itCjtSala->getId())].end(); itDiscSala_Dias++)
   //               {
   //                  c.reset();
   //                  c.setType(Constraint::C_CAP_SALA_UNIDADE);
   //                  c.setUnidade(*itUnidade);
   //                  c.setSubCjtSala(*itCjtSala);
   //                  c.setDisciplina(*itDisc);
   //                  c.setTurma(turma);
   //                  c.setCampus(*itCampus);
   //                  c.setDia(*itDiscSala_Dias);

	//                  sprintf( name, "%s", c.toString().c_str() ); 

	//                  if (cHash.find(c) != cHash.end()) continue;

	//                  nnz = problemData->ofertas.size() * problemData->cursos.size() + 1;

	//                  int rhs = itCjtSala->capTotalSalas() + itUnidade->maiorSala;

	//                  OPT_ROW row( nnz, OPT_ROW::LESS , rhs , name );

   //                  v.reset();
   //                  v.setType(Variable::V_OFERECIMENTO);
   //                  v.setTurma(turma);
   //                  v.setDisciplina(*itDisc);
   //                  v.setUnidade(*itUnidade);
   //                  v.setSubCjtSala(*itCjtSala);
   //                  v.setDia(*itDiscSala_Dias);

	//                  it_v = vHash.find(v);
	//                  if( it_v != vHash.end() )
	//                  { row.insert(it_v->second, itUnidade->maiorSala); }

	//                  GGroup<Oferta*>::iterator itOferta = 
	//                     problemData->ofertasDisc[itDisc->getId()].begin();

	//                  for(; itOferta != problemData->ofertasDisc[itDisc->getId()].end(); itOferta++)
	//                  {
	//                     v.reset();
	//                     v.setType(Variable::V_ALUNOS);

	//                     v.setTurma(turma);
	//                     v.setDisciplina(*itDisc);
	//                     v.setOferta(*itOferta);

	//                     it_v = vHash.find(v);
	//                     if( it_v != vHash.end() )
	//                     { row.insert(it_v->second, 1.0); }
	//                  }

	//                  if(row.getnnz() != 0)
	//                  {
	//                     cHash[ c ] = lp->getNumRows();

	//                     lp->addRow(row);
	//                     restricoes++;
	//                  }

   //               }
   //            }
   //         }
   //      }
   //   }
   //}

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

	char name[100];
	int nnz;
	Constraint c;
	Variable v;
	VariableHash::iterator it_v;

	ITERA_GGROUP(itDisc,problemData->disciplinas,Disciplina)
	{
		for(int turma = 0; turma < itDisc->num_turmas; turma++)
		{
			GGroup<int>::iterator itDiasLetDisc =
				itDisc->diasLetivos.begin();

			for(; itDiasLetDisc != itDisc->diasLetivos.end(); itDiasLetDisc++ )
			{
				c.reset();
				c.setDisciplina(*itDisc);
				c.setType(Constraint::C_TURMA_DISC_DIAS_CONSEC);
				c.setTurma(turma);
				c.setDia(*itDiasLetDisc);

				sprintf( name, "%s", c.toString().c_str() ); 

				if (cHash.find(c) != cHash.end()) continue;

				nnz = problemData->totalSalas * 2 + 1;

				OPT_ROW row( nnz, OPT_ROW::GREATER , -1 , name );

				v.reset();
				v.setType(Variable::V_DIAS_CONSECUTIVOS);
				v.setTurma(turma);
				v.setDisciplina(*itDisc);
				v.setDia(*itDiasLetDisc);

				it_v = vHash.find(v);
				if( it_v != vHash.end() )
				{ row.insert(it_v->second, 1.0); }

				ITERA_GGROUP(itCampus,problemData->campi,Campus)
				{
					ITERA_GGROUP(itUnidade,itCampus->unidades,Unidade)
					{
						ITERA_GGROUP(itCjtSala,itUnidade->conjutoSalas,ConjuntoSala)
						{
							v.reset();
							v.setType(Variable::V_OFERECIMENTO);
							v.setTurma(turma);
							v.setDisciplina(*itDisc);
							v.setUnidade(*itUnidade);
							v.setSubCjtSala(*itCjtSala);
							v.setDia(*itDiasLetDisc);

							it_v = vHash.find(v);
							if( it_v != vHash.end() )
							{ row.insert(it_v->second, -1.0); }

							v.setDia((*itDiasLetDisc)-1);

							it_v = vHash.find(v);
							if( it_v != vHash.end() )
							{ row.insert(it_v->second, 1.0); }
						}
					}
				}

				if(row.getnnz() != 0)
				{
					cHash[ c ] = lp->getNumRows();

					lp->addRow(row);
					restricoes++;
				}

				//lp->addRow(row);
				//restricoes++;
			}
		}
	}

	//ITERA_GGROUP(it_disc,problemData->disciplinas,Disciplina) {
	//   for(int i=0;i<it_disc->num_turmas;++i) {
	//      for(int t=1;t<7;t++) {
	//         c.reset();
	//         c.setDisciplina(*it_disc);
	//         c.setType(Constraint::C_TURMA_DISC_DIAS_CONSEC);
	//         c.setTurma(i);
	//         c.setDia(t);

	//         sprintf( name, "%s", c.toString().c_str() ); 

	//         if (cHash.find(c) != cHash.end()) continue;

	//         cHash[ c ] = lp->getNumRows();

	//         nnz = problemData->totalSalas * 2 + 1;

	//         OPT_ROW row( nnz, OPT_ROW::GREATER , -1 , name );

	//         v.reset();
	//         v.setType(Variable::V_DIAS_CONSECUTIVOS);
	//         v.setTurma(i);
	//         v.setDisciplina(*it_disc);
	//         v.setDia(t);

	//         it_v = vHash.find(v);
	//         if( it_v != vHash.end() )
	//         {
	//            row.insert(it_v->second, 1.0);
	//         }

	//         ITERA_GGROUP(it_campus,problemData->campi,Campus) {
	//            ITERA_GGROUP(it_u,it_campus->unidades,Unidade) {
	//               ITERA_GGROUP(it_sala,it_u->salas,Sala) {
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
	//                     row.insert(it_v->second, -1.0);
	//                  }

	//                  v.setDia(t-1);

	//                  it_v = vHash.find(v);
	//                  if( it_v != vHash.end() )
	//                  {
	//                     row.insert(it_v->second, 1.0);
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

	char name[100];
	int nnz;
	Constraint c;
	Variable v;
	VariableHash::iterator it_v;

	ITERA_GGROUP(itBloco,problemData->blocos,BlocoCurricular)
	{
		for(int turma = 0; turma < itBloco->total_turmas; turma++)
		{
			GGroup<int>::iterator itDiasLetivosBlocoCurric = 
				itBloco->diasLetivos.begin();

			for(; itDiasLetivosBlocoCurric != itBloco->diasLetivos.end(); itDiasLetivosBlocoCurric++)
			{
				c.reset();
				c.setType(Constraint::C_MIN_CREDS_TURM_BLOCO);

				c.setBloco(*itBloco);
				c.setTurma(turma);
				c.setDia(*itDiasLetivosBlocoCurric);

				sprintf( name, "%s", c.toString().c_str() ); 

				if (cHash.find(c) != cHash.end()) continue;

				nnz = (itBloco->disciplinas.size() * problemData->totalConjuntosSalas) + 1;

				OPT_ROW row( nnz, OPT_ROW::GREATER , 0.0 , name );

				ITERA_GGROUP(itDisc,itBloco->disciplinas,Disciplina)
				{
					ITERA_GGROUP(itUnidade,itBloco->campus->unidades,Unidade)
					{
						ITERA_GGROUP(itCjtSala,itUnidade->conjutoSalas,ConjuntoSala)
						{
							v.reset();
							v.setType(Variable::V_CREDITOS);
							v.setTurma(turma);
							v.setDisciplina(*itDisc);
							v.setUnidade(*itUnidade);
							v.setSubCjtSala(*itCjtSala);
							v.setDia(*itDiasLetivosBlocoCurric);               

							it_v = vHash.find(v);
							if( it_v != vHash.end() )
							{ row.insert(it_v->second, 1.0); }
						}
					}					
				}

				v.reset();
				v.setType(Variable::V_MIN_CRED_SEMANA);

				v.setTurma(turma);
				v.setBloco(*itBloco);

				it_v = vHash.find(v);
				if( it_v != vHash.end() )
				{ row.insert(it_v->second, -1.0); }

				/* Para evitar a criação da restrição no caso em que só a variável h seja encontrada. Isso é só uma
				garantia. Como os dias letivos estão sendo respeitados, não devemos notar erros. */
				if(row.getnnz() > 1) 
				{
					cHash[ c ] = lp->getNumRows();

					lp->addRow(row);
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

	char name[100];
	int nnz;
	Constraint c;
	Variable v;
	VariableHash::iterator it_v;

	ITERA_GGROUP(itBloco,problemData->blocos,BlocoCurricular)
	{
		for(int turma = 0; turma < itBloco->total_turmas; turma++)
		{
			GGroup<int>::iterator itDiasLetivosBlocoCurric = 
				itBloco->diasLetivos.begin();

			for(; itDiasLetivosBlocoCurric != itBloco->diasLetivos.end(); itDiasLetivosBlocoCurric++)
			{
				c.reset();
				c.setType(Constraint::C_MAX_CREDS_TURM_BLOCO);

				c.setBloco(*itBloco);
				c.setTurma(turma);
				c.setDia(*itDiasLetivosBlocoCurric);

				sprintf( name, "%s", c.toString().c_str() ); 

				if (cHash.find(c) != cHash.end()) continue;

				nnz = (itBloco->disciplinas.size() * problemData->totalConjuntosSalas) + 1;

				OPT_ROW row( nnz, OPT_ROW::LESS , 0.0 , name );

				ITERA_GGROUP(itDisc,itBloco->disciplinas,Disciplina)
				{
					ITERA_GGROUP(itUnidade,itBloco->campus->unidades,Unidade)
					{
						ITERA_GGROUP(itCjtSala,itUnidade->conjutoSalas,ConjuntoSala)
						{
							v.reset();
							v.setType(Variable::V_CREDITOS);
							v.setTurma(turma);
							v.setDisciplina(*itDisc);
							v.setUnidade(*itUnidade);
							v.setSubCjtSala(*itCjtSala);
							v.setDia(*itDiasLetivosBlocoCurric);

							it_v = vHash.find(v);
							if( it_v != vHash.end() )
							{ row.insert(it_v->second, 1.0); }
						}
					}					
				}

				v.reset();
				v.setType(Variable::V_MAX_CRED_SEMANA);

				v.setTurma(turma);
				v.setBloco(*itBloco);

				it_v = vHash.find(v);
				if( it_v != vHash.end() )
				{ row.insert(it_v->second, -1.0); }

				/* Para evitar a criação da restrição no caso em que só a variável h seja encontrada. Isso é só uma
				garantia. Como os dias letivos estão sendo respeitados, não devemos notar erros. */
				if(row.getnnz() > 1)
				{
					cHash[ c ] = lp->getNumRows();

					lp->addRow(row);
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

	char name[100];
	int nnz;
	Constraint c;
	Variable v;
	VariableHash::iterator it_v;

   ITERA_GGROUP(itCampus,problemData->campi,Campus)
   {
      ITERA_GGROUP(itCurso,problemData->cursos,Curso)
      {
         ITERA_GGROUP(itDisc,problemData->disciplinas,Disciplina)
         {
            for(int turma = 0; turma < itDisc->num_turmas; turma++)
            {
               c.reset();
               c.setType(Constraint::C_ALUNO_CURSO_DISC);
               c.setCampus(*itCampus);
               c.setCurso(*itCurso);
               c.setDisciplina(*itDisc);
               c.setTurma(turma);

               sprintf( name, "%s", c.toString().c_str() ); 

               if (cHash.find(c) != cHash.end()) continue;

               nnz = problemData->ofertasDisc[itDisc->getId()].size() + 1;

               OPT_ROW row( nnz, OPT_ROW::LESS , 0.0 , name );

               GGroup<Oferta*>::iterator itOfertasDisc = 
                  problemData->ofertasDisc[itDisc->getId()].begin();

               for(; itOfertasDisc != problemData->ofertasDisc[itDisc->getId()].end(); itOfertasDisc++)
               {
                  if(itOfertasDisc->campus == *itCampus)
                  {
                     if(itOfertasDisc->curso == *itCurso)
                     {
                        v.reset();
                        v.setType(Variable::V_ALUNOS);
                        v.setTurma(turma);
                        v.setDisciplina(*itDisc);
                        v.setOferta(*itOfertasDisc);

                        it_v = vHash.find(v);
                        if( it_v != vHash.end() )
                        { row.insert(it_v->second, 1.0); }
                     }
                  }
               }

               v.reset();
               v.setType(Variable::V_ALOC_ALUNO);
               v.setTurma(turma);
               v.setDisciplina(*itDisc);
               v.setCurso(*itCurso);
               v.setCampus(*itCampus);

               it_v = vHash.find(v);
               if( it_v != vHash.end() )
               { row.insert(it_v->second, -itCampus->maiorSala * 100 ); }

               if(row.getnnz() != 0)
               {
                  cHash[ c ] = lp->getNumRows();

                  lp->addRow(row);
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
	char name[100];
	int nnz;
	Constraint c;
	Variable v;
	VariableHash::iterator it_v;

	ITERA_GGROUP(itCampus,problemData->campi,Campus) 
	{
		ITERA_GGROUP(itDisc,problemData->disciplinas,Disciplina)
		{
			for(int turma = 0; turma < itDisc->num_turmas; turma++)
			{
				std::map<std::pair<Curso*,Curso*>,bool>::iterator itCC =
					problemData->compat_cursos.begin();

				for(; itCC != problemData->compat_cursos.end(); itCC++)
				{
					if(!itCC->second)
					{
						c.reset();
						c.setType(Constraint::C_ALUNOS_CURSOS_DIF);

						c.setCampus(*itCampus);

						c.setCurso(itCC->first.first);
						c.setCursoIncompat(itCC->first.second);

						c.setDisciplina(*itDisc);
						c.setTurma(turma);

						sprintf( name, "%s", c.toString().c_str() ); 

						if (cHash.find(c) != cHash.end()) continue;

						// Testando com os cursos invertidos

						c.setCurso(itCC->first.second);
						c.setCursoIncompat(itCC->first.first);

						if (cHash.find(c) != cHash.end()) continue;

						nnz = 3;

						OPT_ROW row( nnz, OPT_ROW::LESS , 1.0 , name );

						// ---
						// b -> curso compatível

						v.reset();
						v.setType(Variable::V_ALOC_ALUNO);

						v.setTurma(turma);
						v.setDisciplina(*itDisc);
						v.setCurso(itCC->first.first);
						v.setCampus(*itCampus);

						it_v = vHash.find(v);
						if( it_v != vHash.end() )
						{ row.insert(it_v->second, 1); }

						// ---
						// b -> curso incompatível

						v.reset();
						v.setType(Variable::V_ALOC_ALUNO);

						v.setTurma(turma);
						v.setDisciplina(*itDisc);

						//v.setCurso(itCC->first.second);
						v.setCursoIncompat(itCC->first.second);

						v.setCampus(*itCampus);

						it_v = vHash.find(v);
						if( it_v != vHash.end() )
						{ row.insert(it_v->second, 1); }

						// ---
						// bs

						v.reset();
						v.setType(Variable::V_SLACK_ALOC_ALUNOS_CURSO_INCOMPAT);

						v.setTurma(turma);
						v.setDisciplina(*itDisc);
						v.setCurso(itCC->first.first);
						v.setCursoIncompat(itCC->first.second);
						v.setCampus(*itCampus);

						it_v = vHash.find(v);
						if( it_v != vHash.end() )
						{ row.insert(it_v->second, -1); }

						if(row.getnnz() != 0)
						{
							cHash[ c ] = lp->getNumRows();

							lp->addRow(row);
							restricoes++;
						}

						//lp->addRow(row);
						//restricoes++;

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

	char name[100];
	int nnz;
	Constraint c;
	Variable v;
	VariableHash::iterator it_v;

	ITERA_GGROUP(itCampus,problemData->campi,Campus) 
	{
		ITERA_GGROUP(itUnidade,itCampus->unidades,Unidade)
		{
			ITERA_GGROUP(itCjtSala,itUnidade->conjutoSalas,ConjuntoSala)
			{
				ITERA_GGROUP(itDisc,problemData->disciplinas,Disciplina)
				{
					for(int turma = 0; turma < itDisc->num_turmas; turma++)
					{
						GGroup<int>::iterator itDiasLetDisc =
							itDisc->diasLetivos.begin();

						for(; itDiasLetDisc != itDisc->diasLetivos.end(); itDiasLetDisc++ )
						{
							ITERA_GGROUP(it_fix,problemData->fixacoes,Fixacao)
							{

								if(it_fix->disciplina_id == itDisc->getId() && it_fix->dia_semana == *itDiasLetDisc) 
								{					
									c.reset();
									c.setType(Constraint::C_SLACK_DIST_CRED_DIA);

									c.setTurma(turma);
									c.setDisciplina(*itDisc);
									c.setDia(*itDiasLetDisc);
									c.setSubCjtSala(*itCjtSala);

									sprintf( name, "%s", c.toString().c_str() ); 

									if (cHash.find(c) != cHash.end()) continue;

									nnz = problemData->totalConjuntosSalas * itDisc->num_turmas + 1;

									OPT_ROW row( nnz, OPT_ROW::EQUAL , 0.0 , name );
								
									if(itCjtSala->capTotalSalas() == it_fix->sala->capacidade)
									{
										ITERA_GGROUP(itCampus1,problemData->campi,Campus)
										{
											if(itCampus->getId() == itCampus1->getId())
											{
												ITERA_GGROUP(itUnidade1,itCampus->unidades,Unidade)
												{
													v.reset();
													v.setType(Variable::V_CREDITOS);

													v.setTurma(turma);
													v.setDisciplina(*itDisc);
													v.setUnidade(*itUnidade1);
													v.setSubCjtSala(*itCjtSala);
													v.setDia(*itDiasLetDisc);

													it_v = vHash.find(v);
													if( it_v != vHash.end() )
													{ row.insert(it_v->second, 1.0); }
												}
											}
										}

										v.reset();
										v.setType(Variable::V_SLACK_DIST_CRED_DIA_SUPERIOR);
										v.setTurma(turma);
										v.setDisciplina(*itDisc);
										v.setDia(*itDiasLetDisc);
										v.setSubCjtSala(*itCjtSala);

										it_v = vHash.find(v);
										if( it_v != vHash.end() )
										{ row.insert(it_v->second, 1.0); }

										v.reset();
										v.setType(Variable::V_SLACK_DIST_CRED_DIA_INFERIOR);
										v.setTurma(turma);
										v.setDisciplina(*itDisc);
										v.setDia(*itDiasLetDisc);
										v.setSubCjtSala(*itCjtSala);

										it_v = vHash.find(v);
										if( it_v != vHash.end() )
										{ row.insert(it_v->second, -1.0); }

										v.reset();
										v.setType(Variable::V_ABERTURA);

										v.setCampus(*itCampus);
										v.setDisciplina(*itDisc);
										v.setTurma(turma);

										it_v = vHash.find(v);
										if( it_v != vHash.end() )
										{
											row.insert(it_v->second,
												-(it_fix->disciplina->max_creds));
										}

										if(row.getnnz() != 0)
										{
											cHash[ c ] = lp->getNumRows();

											lp->addRow(row);
											restricoes++;
										}
									}

									//lp->addRow(row);
									//restricoes++;
								}
							}
						}
					}
				}
			}
		}
	}


	//ITERA_GGROUP(itCampus,problemData->campi,Campus) 
	//{
	//	ITERA_GGROUP(itDisc,problemData->disciplinas,Disciplina)
	//	{
	//		for(int turma = 0; turma < itDisc->num_turmas; turma++)
	//		{
	//			GGroup<int>::iterator itDiasLetDisc =
	//				itDisc->diasLetivos.begin();

	//			for(; itDiasLetDisc != itDisc->diasLetivos.end(); itDiasLetDisc++ )
	//			{
	//				ITERA_GGROUP(it_fix,problemData->fixacoes,Fixacao)
	//				{
	//					if(it_fix->disciplina_id == itDisc->getId() && it_fix->dia_semana == *itDiasLetDisc) 
	//					{					
	//						c.reset();
	//						c.setType(Constraint::C_SLACK_DIST_CRED_DIA);

	//						c.setDisciplina(*itDisc);
	//						c.setDia(*itDiasLetDisc);

	//						sprintf( name, "%s", c.toString().c_str() ); 

	//						if (cHash.find(c) != cHash.end()) continue;

	//						nnz = problemData->totalConjuntosSalas * itDisc->num_turmas + 1;

	//						//int rhs = it_fix->disciplina->max_creds ;

	//						OPT_ROW row( nnz, OPT_ROW::EQUAL , 0.0 , name );
	//				
	//						ITERA_GGROUP(itCampus,problemData->campi,Campus)
	//						{
	//							ITERA_GGROUP(itUnidade,itCampus->unidades,Unidade)
	//							{
	//								ITERA_GGROUP(itCjtSala,itUnidade->conjutoSalas,ConjuntoSala)
	//								{
	//									//for(int turma = 0; turma < itDisc->num_turmas; turma++)
	//									//{

	//									//if(itCjtSala->getId() == it_fix->sala->capacidade){
	//									v.reset();
	//									v.setType(Variable::V_CREDITOS);

	//									v.setTurma(turma);
	//									v.setDisciplina(*itDisc);
	//									v.setUnidade(*itUnidade);
	//									v.setSubCjtSala(*itCjtSala);
	//									v.setDia(*itDiasLetDisc);

	//									it_v = vHash.find(v);
	//									if( it_v != vHash.end() )
	//									{ row.insert(it_v->second, 1.0); }
	//									//}
	//								}
	//							}
	//						}

	//						v.reset();
	//						v.setType(Variable::V_SLACK_DIST_CRED_DIA_SUPERIOR);
	//						v.setDisciplina(*itDisc);
	//						v.setDia(*itDiasLetDisc);

	//						it_v = vHash.find(v);
	//						if( it_v != vHash.end() )
	//						{ row.insert(it_v->second, 1.0); }

	//						v.reset();
	//						v.setType(Variable::V_SLACK_DIST_CRED_DIA_INFERIOR);
	//						v.setDisciplina(*itDisc);
	//						v.setDia(*itDiasLetDisc);

	//						it_v = vHash.find(v);
	//						if( it_v != vHash.end() )
	//						{ row.insert(it_v->second, -1.0); }

	//						v.reset();
	//						v.setType(Variable::V_ABERTURA);

	//						v.setCampus(*itCampus);
	//						v.setDisciplina(*itDisc);
	//						v.setTurma(turma);

	//						it_v = vHash.find(v);
	//						if( it_v != vHash.end() )
	//						{
	//							row.insert(it_v->second,
	//								-(it_fix->disciplina->max_creds));
	//						}

	//						if(row.getnnz() != 0)
	//						{
	//							cHash[ c ] = lp->getNumRows();

	//							lp->addRow(row);
	//							restricoes++;
	//						}

	//						//lp->addRow(row);
	//						//restricoes++;
	//					}
	//				}
	//			}
	//		}
	//	}
	//}


	//ITERA_GGROUP(it_disc,problemData->disciplinas,Disciplina) {
	//   for(int dia=0;dia<7;dia++) {
	//      c.reset();
	//      c.setType(Constraint::C_SLACK_DIST_CRED_DIA);

	//      c.setDisciplina(*it_disc);
	//      c.setDia(dia);

	//      sprintf( name, "%s", c.toString().c_str() ); 

	//      if (cHash.find(c) != cHash.end()) continue;

	//      cHash[ c ] = lp->getNumRows();

	//      nnz = problemData->totalSalas * it_disc->num_turmas  + 1;

	//      int rhs = it_disc->max_creds;
	//      if( it_disc->divisao_creditos != NULL )
	//      {
	//         rhs = it_disc->divisao_creditos->dia[dia];
	//      }

	//      OPT_ROW row( nnz, OPT_ROW::EQUAL , rhs , name );

	//      ITERA_GGROUP(it_campus,problemData->campi,Campus) {

	//         ITERA_GGROUP(it_u,it_campus->unidades,Unidade) {
	//            ITERA_GGROUP(it_sala,it_u->salas,Sala) {

	//               ITERA_GGROUP(it_disc,problemData->disciplinas,Disciplina) {
	//                  for(int i=0;i<it_disc->num_turmas;i++) {

	//                     v.reset();
	//                     v.setType(Variable::V_CREDITOS);

	//                     v.setTurma(i);
	//                     v.setDisciplina(*it_disc);
	//                     v.setUnidade(*it_u);
	//                     v.setSala(*it_sala);
	//                     v.setDia(dia);

	//                     it_v = vHash.find(v);
	//                     if( it_v != vHash.end() )
	//                     {
	//                        row.insert(it_v->second, 1.0);
	//                     }
	//                  }
	//               }
	//            }
	//         }
	//      }

	//      v.reset();
	//      v.setType(Variable::V_SLACK_DIST_CRED_DIA_SUPERIOR);

	//      v.setDisciplina(*it_disc);
	//      v.setDia(dia);

	//      it_v = vHash.find(v);
	//      if( it_v != vHash.end() )
	//      {
	//         row.insert(it_v->second, 1.0);
	//      }

	//      v.reset();
	//      v.setType(Variable::V_SLACK_DIST_CRED_DIA_INFERIOR);

	//      v.setDisciplina(*it_disc);
	//      v.setDia(dia);

	//      it_v = vHash.find(v);
	//      if( it_v != vHash.end() )
	//      {
	//         row.insert(it_v->second, -1.0);
	//      }

	//      lp->addRow(row);
	//      ++restricoes;
	//   }
	//}

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
	char name[100];
	int nnz;

	Constraint c;

	Variable v;
	VariableHash::iterator it_v;

	ITERA_GGROUP(it_bloco,problemData->blocos,BlocoCurricular) {
		GGroup<int>::iterator itDiasLetBloco =
			it_bloco->diasLetivos.begin();

		for(; itDiasLetBloco != it_bloco->diasLetivos.end(); itDiasLetBloco++ )
		{
			c.reset();
			c.setType(Constraint::C_VAR_R);

			c.setBloco(*it_bloco);
			c.setCampus(it_bloco->campus);
			c.setDia(*itDiasLetBloco);

			sprintf( name, "%s", c.toString().c_str() ); 

			if (cHash.find(c) != cHash.end()) continue;

			nnz = 2;

			OPT_ROW row( nnz, OPT_ROW::LESS , 0.0, name );

			v.reset();
			v.setType(Variable::V_N_SUBBLOCOS);

			v.setBloco(*it_bloco);
			v.setDia(*itDiasLetBloco);
			v.setCampus(it_bloco->campus);

			it_v = vHash.find(v);
			if( it_v != vHash.end() )
			{
				row.insert(it_v->second, 1.0);
			}

			v.reset();
			v.setType(Variable::V_ABERTURA_SUBBLOCO_DE_BLC_DIA_CAMPUS);

			v.setBloco(*it_bloco);
			v.setDia(*itDiasLetBloco);
			v.setCampus(it_bloco->campus);

			it_v = vHash.find(v);
			if( it_v != vHash.end() )
			{
				row.insert(it_v->second, -9999.0);
			}

			if(row.getnnz() != 0)
			{
				cHash[ c ] = lp->getNumRows();

				lp->addRow(row);
				restricoes++;
			}

			//lp->addRow(row);
			//restricoes++;
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
	char name[100];
	int nnz;

	Constraint c;

	Variable v;
	VariableHash::iterator it_v;

	ITERA_GGROUP(itCampus,problemData->campi,Campus)
	{
		ITERA_GGROUP(itDisc,problemData->disciplinas,Disciplina)
		{
			for(int turma = 0; turma < itDisc->num_turmas; turma++)
			{
				c.reset();
				c.setType(Constraint::C_LIMITA_ABERTURA_TURMAS);
				c.setCampus(*itCampus);
				c.setDisciplina(*itDisc);
				c.setTurma(turma);

				sprintf( name, "%s", c.toString().c_str() );

				if (cHash.find(c) != cHash.end()) continue;

				nnz = 2;

				OPT_ROW row( nnz, OPT_ROW::LESS , 0.0, name );

				v.reset();
				v.setType(Variable::V_ABERTURA);

				v.setTurma(turma);
				v.setDisciplina(*itDisc);
				v.setCampus(*itCampus);

				it_v = vHash.find(v);
				if(it_v != vHash.end())
				{ row.insert(it_v->second, 1.0); }

				// ---

				GGroup<Oferta*>::iterator itOft =
					problemData->ofertasDisc[itDisc->getId()].begin();

				for(; itOft != problemData->ofertasDisc[itDisc->getId()].end(); itOft++)
				{
					if(itOft->campus->getId() == itCampus->getId())
					{            
						v.reset();
						v.setType(Variable::V_ALUNOS);

						v.setTurma(turma);
						v.setDisciplina(*itDisc);
						v.setOferta(*itOft);

						it_v = vHash.find(v);
						if(it_v != vHash.end())
						{ row.insert(it_v->second, -1.0); }
					}
				}

				if(row.getnnz() != 0)
				{
					cHash[ c ] = lp->getNumRows();
					lp->addRow(row);
					restricoes++;
				}
			}
		}
	}

	//ITERA_GGROUP(itOferta,problemData->ofertas,Oferta)
	//{
	//   GGroup<DisciplinaPeriodo>::iterator itPrdDisc = 
	//      itOferta->curriculo->disciplinas_periodo.begin();

	//   for(; itPrdDisc != itOferta->curriculo->disciplinas_periodo.end(); itPrdDisc++)
	//   { 
	//      Disciplina * ptDisc = problemData->refDisciplinas[(*itPrdDisc).second];

	//      for(int turma = 0; turma < ptDisc->num_turmas; turma++)
	//      {
	//         c.reset();

	//         c.setType(Constraint::C_LIMITA_ABERTURA_TURMAS);

	//         c.setOferta(*itOferta);
	//         c.setDisciplina(ptDisc);
	//         c.setTurma(turma);

	//         sprintf( name, "%s", c.toString().c_str() ); 

	//         if (cHash.find(c) != cHash.end()) continue;

	//         nnz = 2;

	//         OPT_ROW row( nnz, OPT_ROW::LESS , 0.0, name );

	//         v.reset();
	//         v.setType(Variable::V_ABERTURA);

	//         v.setTurma(turma);
	//         v.setDisciplina(ptDisc);
	//         v.setCampus(itOferta->campus);

	//         it_v = vHash.find(v);
	//         if(it_v != vHash.end())
	//         { row.insert(it_v->second, 1.0); }

	//         // ---

	//         v.reset();
	//         v.setType(Variable::V_ALUNOS);

	//         v.setTurma(turma);
	//         v.setDisciplina(ptDisc);
	//         v.setOferta(*itOferta);

	//         it_v = vHash.find(v);
	//         if(it_v != vHash.end())
	//         { row.insert(it_v->second, -1.0); }

	//         if(row.getnnz() != 0)
	//         {
	//            cHash[ c ] = lp->getNumRows();
	//            lp->addRow(row);
	//            restricoes++;
	//         }

	//      }
	//   }
	//}

	//ITERA_GGROUP(itCampus,problemData->campi,Campus)
	//{
	//   ITERA_GGROUP(itUnidade,itCampus->unidades,Unidade)
	//   {
	//      ITERA_GGROUP(itCjtSala,itUnidade->conjutoSalas,ConjuntoSala)
	//      {
	//         for(int dia = 0; dia < 7; dia++)
	//         {
	//            ITERA_GGROUP(itOferta,problemData->ofertas,Oferta)
	//            {
	//               GGroup<DisciplinaPeriodo>::iterator itPrdDisc = 
	//                  itOferta->curriculo->disciplinas_periodo.begin();

	//               for(; itPrdDisc != itOferta->curriculo->disciplinas_periodo.end(); itPrdDisc++)
	//               {
	//                  Disciplina * ptDisc = problemData->refDisciplinas[(*itPrdDisc).second];

	//                  if(itCjtSala->getDiscsAssociadas().find(ptDisc) != 
	//                     itCjtSala->getDiscsAssociadas().end())
	//                  {

	//                     for(int turma = 0; turma < ptDisc->num_turmas; turma++)
	//                     {
	//                        c.reset();
	//                        c.setType(Constraint::C_LIMITA_ABERTURA_TURMAS);

	//                        c.setTurma(turma);
	//                        c.setDisciplina(ptDisc);

	//                        sprintf( name, "%s", c.toString().c_str() ); 

	//                        if (cHash.find(c) != cHash.end()) continue;

	//                        nnz = 2;

	//                        OPT_ROW row( nnz, OPT_ROW::LESS , 0.0, name );

	//                        v.reset();
	//                        v.setType(Variable::V_OFERECIMENTO);

	//                        v.setTurma(turma);
	//                        v.setDisciplina(ptDisc);
	//                        v.setUnidade(*itUnidade);
	//                        //v.setSala(*itSala);
	//                        v.setSubCjtSala(*itCjtSala);
	//                        v.setDia(dia);

	//                        it_v = vHash.find(v);
	//                        if(it_v != vHash.end())
	//                        { row.insert(it_v->second, 1.0); }

	//                        v.reset();
	//                        v.setType(Variable::V_ALUNOS);

	//                        v.setTurma(turma);
	//                        v.setDisciplina(ptDisc);
	//                        v.setOferta(*itOferta);

	//                        it_v = vHash.find(v);
	//                        if(it_v != vHash.end())
	//                        { row.insert(it_v->second, -1.0); }

	//                        if(row.getnnz() != 0)
	//                        {
	//                           cHash[ c ] = lp->getNumRows();
	//                           lp->addRow(row);
	//                           restricoes++;
	//                        }
	//                     }
	//                  }
	//               }
	//            }
	//         }
	//      }
	//   }
	//}

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
	char name[100];
	int nnz;

	Constraint c;

	Variable v;
	VariableHash::iterator it_v;

	ITERA_GGROUP(itDisc,problemData->disciplinas,Disciplina)
	{
		if(itDisc->num_turmas > 1)
		{
			for(int turma = 0; turma < (itDisc->num_turmas-1); turma++)
			{
				c.reset();

				c.setType(Constraint::C_ABRE_TURMAS_EM_SEQUENCIA);

				c.setDisciplina(*itDisc);
				c.setTurma(turma);

				sprintf( name, "%s", c.toString().c_str() ); 

				if (cHash.find(c) != cHash.end()) continue;

				nnz = 9999;

				OPT_ROW row( nnz, OPT_ROW::GREATER , 0.0, name );

				ITERA_GGROUP(itCampus,problemData->campi,Campus)
				{
					ITERA_GGROUP(itUnidade,itCampus->unidades,Unidade)
					{
						ITERA_GGROUP(itCjtSala,itUnidade->conjutoSalas,ConjuntoSala)
						{
							GGroup<int/*Dias*/>::iterator itDiscSala_Dias =
                        problemData->disc_Conjutno_Salas__Dias[std::make_pair<int,int>
								//problemData->discSala_Dias[std::make_pair<int,int>
								(itDisc->getId(),itCjtSala->getId())].begin();

							for(; itDiscSala_Dias != 
								problemData->disc_Conjutno_Salas__Dias[std::make_pair<int,int>
                        //problemData->discSala_Dias[std::make_pair<int,int>
								(itDisc->getId(),itCjtSala->getId())].end(); itDiscSala_Dias++)
							{
								v.reset();
								v.setType(Variable::V_OFERECIMENTO);

								v.setTurma(turma);
								v.setDisciplina(*itDisc);
								v.setUnidade(*itUnidade);
								v.setSubCjtSala(*itCjtSala);
								v.setDia(*itDiscSala_Dias);

								it_v = vHash.find(v);
								if( it_v != vHash.end() )
								{ row.insert(it_v->second, 1.0); }

								v.reset();
								v.setType(Variable::V_OFERECIMENTO);

								int turmaSuc = turma + 1;
								v.setTurma(turmaSuc);

								v.setDisciplina(*itDisc);
								v.setUnidade(*itUnidade);
								v.setSubCjtSala(*itCjtSala);
								v.setDia(*itDiscSala_Dias);

								it_v = vHash.find(v);
								if( it_v != vHash.end() )
								{ row.insert(it_v->second, -1.0); }
						 }
						}
					}
				}

				if(row.getnnz() != 0)
				{
					cHash[ c ] = lp->getNumRows();

					lp->addRow(row);
					restricoes++;
				}

			}
		}
	}

	//ITERA_GGROUP(itCampus,problemData->campi,Campus)
	//{
	//   ITERA_GGROUP(itUnidade,itCampus->unidades,Unidade)
	//   {
	//      ITERA_GGROUP(itCjtSala,itUnidade->conjutoSalas,ConjuntoSala)
	//      {
	//         for(int dia = 0; dia < 7; dia++)
	//         {
	//            ITERA_GGROUP(itDisc,problemData->disciplinas,Disciplina)
	//            {
	//               if(itDisc->num_turmas > 1)
	//               {
	//                  for(int turma = 0; turma < (itDisc->num_turmas-1); turma++)
	//                  {
	//                     c.reset();

	//                     c.setType(Constraint::C_ABRE_TURMAS_EM_SEQUENCIA);

	//                     //c.setUnidade(*itUnidade);
	//                     //c.setSubCjtSala(*itCjtSala);
	//                     //c.setDia(dia);
	//                     c.setDisciplina(*itDisc);
	//                     c.setTurma(turma);

	//                     sprintf( name, "%s", c.toString().c_str() ); 

	//                     if (cHash.find(c) != cHash.end()) continue;

	//                     //cHash[ c ] = lp->getNumRows();

	//                     nnz = 2;

	//                     OPT_ROW row( nnz, OPT_ROW::GREATER , 0.0, name );

	//                     v.reset();
	//                     v.setType(Variable::V_OFERECIMENTO);

	//                     v.setTurma(turma);
	//                     v.setDisciplina(*itDisc);
	//                     v.setUnidade(*itUnidade);
	//                     v.setSubCjtSala(*itCjtSala);
	//                     v.setDia(dia);

	//                     it_v = vHash.find(v);
	//                     if( it_v != vHash.end() )
	//                     { row.insert(it_v->second, 1.0); }

	//                     v.reset();
	//                     v.setType(Variable::V_OFERECIMENTO);

	//                     int turmaSuc = turma + 1;
	//                     v.setTurma(turmaSuc);

	//                     v.setDisciplina(*itDisc);
	//                     v.setUnidade(*itUnidade);
	//                     v.setSubCjtSala(*itCjtSala);
	//                     v.setDia(dia);

	//                     it_v = vHash.find(v);
	//                     if( it_v != vHash.end() )
	//                     { row.insert(it_v->second, -1.0); }

	//                     if(row.getnnz() != 0)
	//                     {
	//                        cHash[ c ] = lp->getNumRows();

	//                        lp->addRow(row);
	//                        restricoes++;
	//                     }
	//                  }
	//               }
	//            }
	//         }
	//      }
	//   }
	//}

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

int SolverMIP::cria_restricao_divisao_credito(){

	int restricoes = 0;

	char name[100];
	int nnz;
	Constraint c;
	Variable v;
	VariableHash::iterator it_v;

	ITERA_GGROUP(itDisc,problemData->disciplinas,Disciplina)
	{
		if(itDisc->divisao_creditos != NULL)
		{
			for(int turma = 0; turma < itDisc->num_turmas; turma++)
			{
				GGroup<int>::iterator itDiasLetDisc =
					itDisc->diasLetivos.begin();

				for(; itDiasLetDisc != itDisc->diasLetivos.end(); itDiasLetDisc++ )
				{
					c.reset();
					c.setType(Constraint::C_DIVISAO_CREDITO);
					c.setDisciplina(*itDisc);
					c.setTurma(turma);
					c.setDia(*itDiasLetDisc);

					sprintf( name, "%s", c.toString().c_str() ); 

					if (cHash.find(c) != cHash.end()) continue;

					nnz = problemData->totalSalas + (itDisc->combinacao_divisao_creditos.size() * 2);

					OPT_ROW row( nnz, OPT_ROW::EQUAL , 0.0 , name );

					ITERA_GGROUP(itCampus,problemData->campi,Campus)
					{
						ITERA_GGROUP(itUnidade,itCampus->unidades,Unidade)
						{
							ITERA_GGROUP(itCjtSala,itUnidade->conjutoSalas,ConjuntoSala)
							{
									v.reset();
									v.setType(Variable::V_CREDITOS);
									v.setTurma(turma);
									v.setDisciplina(*itDisc);
									v.setUnidade(*itUnidade);
									v.setSubCjtSala(*itCjtSala);
									v.setDia(*itDiasLetDisc);

									it_v = vHash.find(v);
									if( it_v != vHash.end() )
									{ row.insert(it_v->second, 1.0); }				

									for(unsigned k = 0; k < itDisc->combinacao_divisao_creditos.size();k++)
									{	
										v.reset();
										v.setType(Variable::V_COMBINACAO_DIVISAO_CREDITO);
										v.setDisciplina(*itDisc);
										v.setTurma(turma);
										v.setK(k);

										int d = *itDiasLetDisc;

										//N{d,k,t}
										int numCreditos = (itDisc->combinacao_divisao_creditos[k])[(*itDiasLetDisc)-1].second;

										it_v = vHash.find(v);
										if( it_v != vHash.end() )
										{  row.insert(it_v->second, -numCreditos); }

										v.reset();
										v.setType(Variable::V_SLACK_COMBINACAO_DIVISAO_CREDITO);
										v.setDisciplina(*itDisc);
										v.setTurma(turma);
										v.setDia(*itDiasLetDisc);	

										it_v = vHash.find(v);
										if( it_v != vHash.end() )
										{ row.insert(it_v->second, -1.0); }
										}

							}
						}
					}

					
					if(row.getnnz() != 0)
					{
						cHash[ c ] = lp->getNumRows();

						lp->addRow(row);
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
	char name[100];
	int nnz;
	Constraint c;
	Variable v;
	VariableHash::iterator it_v;

	ITERA_GGROUP(it_disc,problemData->disciplinas,Disciplina) {
		for(int i=0;i<it_disc->num_turmas;++i) {
			c.reset();
			c.setType(Constraint::C_COMBINACAO_DIVISAO_CREDITO);
			c.setDisciplina(*it_disc);
			c.setTurma(i);

			sprintf( name, "%s", c.toString().c_str() ); 

			if (cHash.find(c) != cHash.end()) continue;
			
			nnz = it_disc->combinacao_divisao_creditos.size();
			
			OPT_ROW row( nnz, OPT_ROW::LESS , 1.0, name );

			for(unsigned k = 0; k < it_disc->combinacao_divisao_creditos.size();k++)
			{
				v.reset();
				v.setType(Variable::V_COMBINACAO_DIVISAO_CREDITO);
				v.setTurma(i);
				v.setDisciplina(*it_disc);
				v.setK(k);	 

				it_v = vHash.find(v);
				if( it_v != vHash.end() )
				{
					row.insert(it_v->second, 1.0);
				}
			}

			if(row.getnnz() != 0)
			{
				cHash[ c ] = lp->getNumRows();

				lp->addRow(row);
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
   char name[100];
   int nnz;
   Constraint c;
   Variable v;
   VariableHash::iterator it_v;

   ITERA_GGROUP(itCampus,problemData->campi,Campus)
   {
      ITERA_GGROUP(itUnidade,itCampus->unidades,Unidade)
      {
         ITERA_GGROUP(itCjtSala,itUnidade->conjutoSalas,ConjuntoSala)
         {
            ITERA_GGROUP(itDisc,problemData->disciplinas,Disciplina)
            {
               for(int turma = 0; turma < itDisc->num_turmas; turma++)
               {
                  c.reset();
                  c.setType(Constraint::C_VAR_Y);

                  c.setUnidade(*itUnidade);
                  c.setSubCjtSala(*itCjtSala);
                  c.setDisciplina(*itDisc);
                  c.setTurma(turma);

                  sprintf( name, "%s", c.toString().c_str() ); 

                  if (cHash.find(c) != cHash.end()) continue;

                  nnz = 8;
                  OPT_ROW row( nnz, OPT_ROW::GREATER , 0.0, name );

                  GGroup<int/*Dias*/>::iterator itDiscSala_Dias =
                     problemData->disc_Conjutno_Salas__Dias[std::make_pair<int,int>
                     (itDisc->getId(),itCjtSala->getId())].begin();

                  for(; itDiscSala_Dias !=                     
                     problemData->disc_Conjutno_Salas__Dias[std::make_pair<int,int>
                     (itDisc->getId(),itCjtSala->getId())].end(); itDiscSala_Dias++)
                  {

                     v.reset();
                     v.setType(Variable::V_OFERECIMENTO);

                     v.setTurma(turma);
                     v.setDisciplina(*itDisc);
                     v.setUnidade(*itUnidade);
                     v.setSubCjtSala(*itCjtSala);
                     v.setDia(*itDiscSala_Dias);

                     it_v = vHash.find(v);
                     if( it_v != vHash.end() )
                     { row.insert(it_v->second, 1.0); }
                  }

                  v.reset();
                  v.setType(Variable::V_ALOC_DISCIPLINA);

                  v.setTurma(turma);
                  v.setDisciplina(*itDisc);
                  v.setUnidade(*itUnidade);
                  v.setSubCjtSala(*itCjtSala);

                  it_v = vHash.find(v);
                  if( it_v != vHash.end() )
                  { row.insert(it_v->second, -1.0); }

                  if(row.getnnz() != 0)
                  {
                     cHash[ c ] = lp->getNumRows();

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

void SolverMIP::cria_solucao_inicial(int cnt, int *indices, double *valores)
{
	//int cnt = 0;

	////int * indices = (int *) malloc(vHash.size()*sizeof(int));
	//indices = (int *) malloc(vHash.size()*sizeof(int));
	////double * valores = (double *) malloc(vHash.size()*sizeof(double));
	//valores = (double *) malloc(vHash.size()*sizeof(double));

	VariableHash::iterator itVHash = vHash.begin();

	for(; itVHash != vHash.end(); itVHash++)
	{
		if(itVHash->first.getType() != Variable::V_SLACK_DEMANDA)
		{
			indices[cnt] = itVHash->second;
			valores[cnt] = 0;
			cnt++;
		}
		else
		{
			indices[cnt] = itVHash->second;
			valores[cnt] = -1;
			cnt++;
		}
	}
}