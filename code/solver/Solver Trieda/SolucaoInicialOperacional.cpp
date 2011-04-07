#include "SolucaoInicialOperacional.h"

bool ordenaCustosAlocacao(CustoAlocacao * left, CustoAlocacao * right)
{
	bool result = (*left > *right);
	return result;
}

SolucaoInicialOperacional::SolucaoInicialOperacional(ProblemData & _problemData)
: problemData(_problemData)
{
	executaFuncaoPrioridade();

	// ----------------------------------------------------------------------
	// Inicializando a estrutura que mantem referencia para as 
	// aulas que n�o foram relacionadas (associadas) a nenhum professor.
	ITERA_GGROUP(itAula, problemData.aulas, Aula)
	{ 
		map<pair<Professor*, Aula*>, CustoAlocacao*>::iterator
			itCustoProfTurma = custoProfTurma.begin();

		for(; itCustoProfTurma != custoProfTurma.end(); ++itCustoProfTurma)
			if(itCustoProfTurma->first.second->getDisciplina() == itAula->getDisciplina())
				break;

		if(itCustoProfTurma == custoProfTurma.end())
			aulasNaoRelacionadasProf.add(*itAula);
	}

	// ----------------------------------------------------------------------
	/* Inicializando a estrutura <custosAlocacaoAulaOrdenado>
	para poder auxiliar na aloca��o de aulas a professores. */
	map<pair<Professor*,Aula*>,CustoAlocacao*>::iterator 
		itCustoProfTurma = custoProfTurma.begin();

	for(; itCustoProfTurma != custoProfTurma.end(); ++itCustoProfTurma)
		custosAlocacaoAulaOrdenado.push_back(itCustoProfTurma->second);

	// ----------------------------------------------------------------------
	// Ordenando custosAlocacaoAulaOrdenado.
	std::sort(custosAlocacaoAulaOrdenado.begin(),
			  custosAlocacaoAulaOrdenado.end(), ordenaCustosAlocacao);
	// ----------------------------------------------------------------------
}

SolucaoInicialOperacional::~SolucaoInicialOperacional()
{

}

SolucaoOperacional & SolucaoInicialOperacional::geraSolucaoInicial()
{
	GGroup< Aula * > aulasNaoAlocadas;
	SolucaoOperacional * solucaoInicial = new SolucaoOperacional(&problemData);

	// ------------------------------------------------------------------------------
	// Inicialmente, todas as aulas (que possuem, pelo menos, um
	// professor associado) serao consideradas como n�o alocadas.
	// Portanto, todas as aulas devem ser adicionadas a estrutura <aulasNaoAlocadas>
	map<pair<Professor*, Aula*>, CustoAlocacao*>::iterator
		itCustoProfTurma = custoProfTurma.begin();
	for(; itCustoProfTurma != custoProfTurma.end(); ++itCustoProfTurma)
	{
		aulasNaoAlocadas.add(itCustoProfTurma->first.second);
	}

	// ------------------------------------------------------------------------------
	// Enquanto a estrutura <custosAlocacaoAulaOrdenado> nao estiver vazia
	while(!custosAlocacaoAulaOrdenado.empty())
	{
		std::vector<CustoAlocacao*>::iterator 
			itCustosAlocacaoAulaOrdenado = custosAlocacaoAulaOrdenado.begin();

		// Para cada custo
		for(; itCustosAlocacaoAulaOrdenado != custosAlocacaoAulaOrdenado.end();
			++itCustosAlocacaoAulaOrdenado)
		{
			Aula & aula = (*itCustosAlocacaoAulaOrdenado)->getAula();
			GGroup<Aula*>::iterator itAulasNaoAlocadas = aulasNaoAlocadas.find(&aula);

			// Checando se a aula do custo em questao ja foi alocada.
			if(itAulasNaoAlocadas != aulasNaoAlocadas.end())
			{
				Professor & professor = (*itCustosAlocacaoAulaOrdenado)->getProfessor();
				bool alocouProfAula = alocaAula(*solucaoInicial, professor, aula);

				// ======================================================
				// PAREI AQUI !!!!
				// Tentar usar essa funcao para alocar. ao inves da funcao de cima.

				// TESTAR !!!! ->>> bool alocouProfAula_2
				// = alocaAula(*solucaoInicial,professor,aula);

				// Assim que pega o vetor de aulas -> solucaoInicial
				// ->getMatrizAulas->at(professor.getIdOperacional());

				// continuar pegando so dados que faltam.
				// bool alocouProfAula = alocaAula_2(,professor,aula);
				// ======================================================
				if(alocouProfAula)
				{
					std::cout << "\nForam alocados " << aula.getTotalCreditos()
							  << " horarios CONSECUTIVOS para a aula da turma " << aula.getTurma()
							  << " da disciplina " << aula.getDisciplina()->getCodigo()
							  << " no dia " << aula.getDiaSemana()
							  << " ao professor " << professor.getCpf() << std::endl;

					// Para n�o tentar alocar esse custo novamente.
					(*itCustosAlocacaoAulaOrdenado) = NULL;

					// Para n�o tentar alocar essa aula novamente.
					aulasNaoAlocadas.remove(itAulasNaoAlocadas);
				}
				else
				{
					cout << "\nTENTATIVA de alocacao de " << aula.getTotalCreditos()
						 << " horarios CONSECUTIVOS para a aula da turma " << aula.getTurma()
						 << " da disciplina " << aula.getDisciplina()->getCodigo()
						 << " no dia " << aula.getDiaSemana()
						 << " ao professor " << professor.getCpf() << " FRACASSOU." << endl;

					// Para n�o tentar alocar esse custo novamente.
					(*itCustosAlocacaoAulaOrdenado) = NULL;		
				}
			}
			else
			{
				// Essa aula ja foi alocada. Portanto, devo remover esse custo sem aloc�-lo.
				(*itCustosAlocacaoAulaOrdenado) = NULL;
			}
		}

		// Removendo os custos das aulas que foram alocadas na rodada atual.
		for(int p = (custosAlocacaoAulaOrdenado.size()-1); p >= 0; --p)
		{
			if(custosAlocacaoAulaOrdenado.at(p) == NULL)
			{
				custosAlocacaoAulaOrdenado.erase(
					custosAlocacaoAulaOrdenado.begin() + p);
			}
		}
	}

	if(aulasNaoAlocadas.size() > 0)
	{
		// Aulas que n�o puderam ser alocadas a nenhum professor.
		std::cout << "ATENCAO: Existem aulas que nao "
				  << "foram alocadas a nenhum professor. " << std::endl;
		// exit(1);

		// Criando uma estrutura que armazene dados dos professores virtuais a serem criados.
		// std::vector< std::vector<Aula*> * > * professoresVirtuais =
		// new std::vector< std::vector<Aula*> * >;

		// Caso ainda nao exista nenhum professor virtual, cria-se um.
		//if(professoresVirtuais->empty())
		//{
		//	int idOperacional = solucaoInicial->getTotalDeProfessores();
		//	
		//	/*
		//	Vai dar problema se for multicampi. !!!! 

		//	Por eqto, considerando apenas um CAMPUS.
		//	*/
		//	// Criando um professor virtual.
		//	Professor * novoProfessor = new Professor(true);
		//	novoProfessor->setIdOperacional(idOperacional);
		//	problemData.campi.begin()->professores.add(novoProfessor);

		//	professoresVirtuais->push_back(new 
		//		std::vector<Aula*> 
		//		((solucaoInicial->getTotalDias()*solucaoInicial->getTotalHorarios()),
		//		NULL));
		//}
		
		// Tentar alocar em algum professor virtual ja existente. Se nao der, crio um novo.

		// Atencao: Para definir o idOperacional do proximo prof a ser criado, basta somar
		// o total de professores da estrutura <solucaoInicial> com o size da estrutura <professoresVirtuais>

		// ALOCAR !!!!!!!!!!!!!!!

		// TODO:
		// Depois que tiver alocado todos as aulas restantes, devo adicionar
		// cada professor virual a solucao que esta sendo criada.

		// FACIL. A estrutura que armazena os dados dos professores virtuais eh
		// igual a estrutura que representa a solucao. Dai, como ela ja foi instanciada,
		// basta adicionar cada professor virtual ao final da solucao. TEM QUE SER ASSIM,
		// POR CAUSA DOS IDS.
	}

	// Aulas, que nem sequer foram associadas a algum professor.
	if(aulasNaoRelacionadasProf.size() > 0)
	{
		std::cout << "ATENCAO: Existem aulas sem professor associado, "
				  << "ou seja, nao foi calculado um custo para ela pq o "
				  << "usuario nao associou a disciplina da aula "
				  << "em questao a nenhum professor." << std::endl;

		// CRIAR PROFESSOR VIRTUAL.
		exit(1);
	}

	return *(solucaoInicial);
}

bool SolucaoInicialOperacional::alocaAula(
	SolucaoOperacional & solucaoOperacional, Professor & professor, Aula & aula)
{
	bool alocou = false;

	int diaSemana = aula.getDiaSemana();
	int idOperacionalProf = professor.getIdOperacional();

	// Otendo um iterador para o in�cio dos hor�rios letivos
	// do professor em quest�o em um dado dia (determinado pela aula).
	vector<Aula*>::iterator itHorarios = solucaoOperacional.getHorariosDia(professor,diaSemana);

	// A ideia aqui � iterar sobre os hor�rios letivos do professor,
	// para o dia em quest�o at� que o primeiro hor�rio livre seja
	// encontrado (pode ser que o professor n�o possua nenhum hor�rio 
	// livre). Uma vez encontrado o hor�rio livre, a ideia agora � obter,
	// sequencialmente, o total de hor�rios livres demandados pela
	// disciplina da aula em quest�o. S� ent�o uma aloca��o pode ser conclu�da.

	bool sequenciaDeHorariosLivres = false;
	vector<Aula*>::iterator itInicioHorariosAlocar = itHorarios;
	int totalCredsAlocar = aula.getTotalCreditos();

	for(int horario = 0; horario < solucaoOperacional.getTotalHorarios(); ++horario, ++itHorarios)
	{
		if(*itHorarios)
		{
			sequenciaDeHorariosLivres = false;
			totalCredsAlocar = aula.getTotalCreditos();
			continue;
		}

		sequenciaDeHorariosLivres = true;

		if(totalCredsAlocar == aula.getTotalCreditos())
		{
			itInicioHorariosAlocar = itHorarios;
		}

		// Se a disciplina possuir apenas um
		// cr�dito para o dia em quest�o, basta aloc�-la.
		if(aula.getTotalCreditos() == 1)
		{
			(*itHorarios) = &aula;
			alocou = true;
			break;
		}
		else if(aula.getTotalCreditos() > 1)
		{
			// Caso a disciplina possua mais de um cr�dito a ser
			// alocado, a ideia aqui � aloca-los em sequ�ncia.
			// Portanto, devo verificar se os cr�ditos demandados est�o livres.

			// Atualizo o total de cr�ditos alocados.
			--totalCredsAlocar;

			if(totalCredsAlocar == 0)
			{
				// Posso parar a busca pq j�
				// encontrei o total de cr�ditos necess�rios.
				break;
			}
		}
	}

	// Se encontrei uma sequ�ncia de hor�rios livres, aloco.
	if(sequenciaDeHorariosLivres)
	{
		for(int horario = 0; horario < aula.getTotalCreditos();
			++horario, ++itInicioHorariosAlocar)
		{
			*itInicioHorariosAlocar = &aula;
		}

		alocou = true;
	}

	return alocou;
}

bool SolucaoInicialOperacional::alocaAula_2(
	vector<Aula*> & horariosProf, int indicePrimeiroHorarioDia,
	int totalHorarios,  Professor & professor, Aula & aula)
{
	bool alocou = false;

	int diaSemana = aula.getDiaSemana();
	int idOperacionalProf = professor.getIdOperacional();

	// Otendo um iterador para o in�cio dos hor�rios letivos
	// do professor em quest�o em um dado dia (determinado pela aula).
	vector<Aula*>::iterator itHorarios = (horariosProf.begin() + indicePrimeiroHorarioDia);

	// A ideia aqui � iterar sobre os hor�rios letivos do professor,
	// para o dia em quest�o at� que o primeiro hor�rio livre seja
	// encontrado (pode ser que o professor n�o possua nenhum hor�rio 
	// livre). Uma vez encontrado o hor�rio livre, a ideia agora � obter,
	// sequencialmente, o total de hor�rios livres demandados pela
	// disciplina da aula em quest�o. S� ent�o uma aloca��o pode ser conclu�da.

	bool sequenciaDeHorariosLivres = false;
	vector<Aula*>::iterator itInicioHorariosAlocar = itHorarios;
	int totalCredsAlocar = aula.getTotalCreditos();

	for(int horario = 0; horario < totalHorarios; ++horario, ++itHorarios)
	{
		if(*itHorarios)
		{
			sequenciaDeHorariosLivres = false;
			totalCredsAlocar = aula.getTotalCreditos();
			continue;
		}

		sequenciaDeHorariosLivres = true;

		if(totalCredsAlocar == aula.getTotalCreditos())
		{
			itInicioHorariosAlocar = itHorarios;
		}

		// Se a disciplina possuir apenas um
		// cr�dito para o dia em quest�o, basta aloc�-la.
		if(aula.getTotalCreditos() == 1)
		{
			*itHorarios = &aula;
			alocou = true;
			break;
		}
		else if(aula.getTotalCreditos() > 1)
		{
			// Caso a disciplina possua mais de um cr�dito a ser
			// alocado, a ideia aqui � aloca-los em sequ�ncia.
			// Portanto, devo verificar se os cr�ditos demandados est�o livres.

			// Atualizo o total de cr�ditos alocados.
			--totalCredsAlocar;

			if(totalCredsAlocar == 0)
			{
				// Posso parar a busca pq j�
				// encontrei o total de cr�ditos necess�rios.
				break;
			}
		}
	}

	// Se encontrei uma sequ�ncia de hor�rios livres, aloco.
	if(sequenciaDeHorariosLivres)
	{
		for(int horario = 0; horario < aula.getTotalCreditos();
			++horario, ++itInicioHorariosAlocar)
		{
			*itInicioHorariosAlocar = &aula;
		}

		alocou = true;
	}

	return alocou;
}

void SolucaoInicialOperacional::executaFuncaoPrioridade()
{
	ITERA_GGROUP(itCampus,problemData.campi,Campus)
	{
		ITERA_GGROUP(itProfessor,itCampus->professores,Professor)
		{
			/* Contabilizando os hor�rios dispon�veis de um professor. 

			Assim, toda vez que um CustoAlocacao for instanciado, o custo
			referente � "disponibilidade do professor p" ser� atualizado.

			H� necessidade de converter o valor para que seja somado corretamente.

			Ex. 
			custoDispProf_A = 10 -> 10 horarios disponiveis
			custoDispProf_B = 5 -> 5 horarios disponiveis

			custoDispProf_B tem prioridade MAIOR que custoDispProf_A.

			prioridade = -(custoDispProf_A - (TOTALHORARIOSCAMPUS+1))

			Dado que TOTALHORARIOSCAMPUS = 10, ent�o:

			prioridade(custoDispProf_A) = 1
			prioridade(custoDispProf_A) = 6

			Estamos adimitindo aqui que a inst. possui apenas um CAMPUS.
			N�o funcionar� para multicampus.

			TODO : ADAPTAR TODO O COD PARA CONTEMPLAR MULTICAMPUS.
			*/

			int horariosProfessor = (itProfessor->horarios.size());
			int horariosCampus = (itCampus->horarios.size()+1);
			int dispProf = -(horariosProfessor - horariosCampus);
			itProfessor->setCustoDispProf(dispProf);

			ITERA_GGROUP(itMagisterio,itProfessor->magisterio,Magisterio)
			{
				Disciplina * discMinistradaProf = itMagisterio->disciplina;

				// TODO : Teria que ser para toda aula do campus em quest�o.
				ITERA_GGROUP(itAula,problemData.aulas,Aula)
				{
					Disciplina * discAula = itAula->getDisciplina();

					if(discMinistradaProf == discAula)
					{
						// Para o primeiro custo da fun��o de prioridade, tenho que testar agora se
						// existe fixa��o desse professor para a disciplina da aula em quest�o.

						std::pair<Professor*,Disciplina*> chave (*itProfessor,discMinistradaProf);

						std::map<std::pair<Professor*,Disciplina*>,GGroup<Fixacao*> >::iterator
							itFixacoesProfDisc = problemData.fixacoesProfDisc.find(chave);

						// Somente se existir, pelo menos, uma fixa��o de um professor a uma disciplina.
						if(itFixacoesProfDisc != problemData.fixacoesProfDisc.end())
						{
							ITERA_GGROUP(itFixacao,itFixacoesProfDisc->second,Fixacao)
							{
								calculaCustoFixProf(**itProfessor,**itAula,0);
							}
						}

						// Para o segundo custo considerado para o c�lculo da fun��o de prioridade, tenho
						// que somar a nota (prefer�ncia) dada pelo professor para a disciplina em quest�o.

						// Dado que a maior prefer�ncia recebe nota 1 e a menor recebe nota 10, basta subtrair a
						// nota (prefer�ncia) por 11 e, em seguida, multiplicar o resultado por -1. Assim, somaremos
						// um valor correto ao custo, j� que o melhor custo total possuir� o maior valor atribuido.
						calculaCustoFixProf(**itProfessor,**itAula,1,itMagisterio->getPreferencia());

						pair<int,int> chaveGamb (itProfessor->getId(),discMinistradaProf->getId());

						// Se o professor e a disciplina da aula em quest�o se relacionarem:
						if(problemData.prof_Disc_Dias.find(chaveGamb) != problemData.prof_Disc_Dias.end())
						{
							int custo = problemData.prof_Disc_Dias[chaveGamb].size();
							calculaCustoFixProf(**itProfessor,**itAula,3,custo,itCampus->horarios.size());
						}
					}
				}
			}
		}
	}
}

void SolucaoInicialOperacional::calculaCustoFixProf(
	Professor& prof , Aula& aula, unsigned idCusto, int custo, int maxHorariosCP)
{
	pair<Professor*,Aula*> chave (&prof,&aula);

	map<pair<Professor*,Aula*>,CustoAlocacao*>::iterator 
		itCustoProfTurma = custoProfTurma.find(chave);

	// Criando, se necess�rio, um novo CustoAlocacao dada a chave em quest�o.
	if(itCustoProfTurma == custoProfTurma.end())
	{
		custoProfTurma[chave] = new CustoAlocacao(prof,aula);
	}

	if(idCusto == 0) // custoFixProfTurma
	{
		custoProfTurma[chave]->addCustoFixProfTurma(1);
	}
	else if(idCusto == 1)
	{
		int preferenciaProfDisc = (custo - 11) * (-1);
		custoProfTurma[chave]->addCustoPrefProfTurma(preferenciaProfDisc);
	}
	else if (idCusto == 3)
	{
		// Aqui, compartilha-se a ideia da
		// terceira restri��o da fun��o de prioridade.
		int custoDispProfTurma = -(custo - (maxHorariosCP+1));
		custoProfTurma[chave]->addCustoDispProfTurma(custoDispProfTurma);
	}
	else
	{
		std::cout << "ERRO: idCusto (" << idCusto << ") INVALIDO.";
		exit(1);
	}
}
