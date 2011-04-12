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
	  {
         if(itCustoProfTurma->first.second->getDisciplina() == itAula->getDisciplina())
		 {
            break;
		 }
	  }

      if(itCustoProfTurma == custoProfTurma.end())
	  {
         aulasNaoRelacionadasProf.add(*itAula);
	  }
   }
   // ----------------------------------------------------------------------

   // ----------------------------------------------------------------------
   // Inicializando a estrutura <custosAlocacaoAulaOrdenado>
   // para poder auxiliar na aloca��o de aulas a professores.
   map<pair<Professor*,Aula*>,CustoAlocacao*>::iterator 
      itCustoProfTurma = custoProfTurma.begin();

   for(; itCustoProfTurma != custoProfTurma.end(); ++itCustoProfTurma)
   {
      custosAlocacaoAulaOrdenado.push_back(itCustoProfTurma->second);
   }
   // ----------------------------------------------------------------------

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
   GGroup<Aula*> aulasNaoAlocadas;
   SolucaoOperacional * solucaoInicial = new SolucaoOperacional(&problemData);
   int total_horarios = solucaoInicial->getTotalHorarios();

   // -----------------------------------------------------------
   // Inicialmente, todas as aulas (que possuem, pelo
   // menos, um professor associado) serao consideradas
   // como n�o alocadas. Portanto, todas as aulas devem
   // ser adicionadas a estrutura <aulasNaoAlocadas>
   map<pair<Professor*, Aula*>, CustoAlocacao*>::iterator
      itCustoProfTurma = custoProfTurma.begin();

   for(; itCustoProfTurma != custoProfTurma.end(); ++itCustoProfTurma)
   {
      aulasNaoAlocadas.add(itCustoProfTurma->first.second);
   }

   // -----------------------------------------------------------
   // Enquanto a estrutura <custosAlocacaoAulaOrdenado> nao estiver vazia
   while(!custosAlocacaoAulaOrdenado.empty())
   {
      std::vector< CustoAlocacao * >::iterator 
         itCustosAlocacaoAulaOrdenado = custosAlocacaoAulaOrdenado.begin();

      // Para cada custo
      for(; itCustosAlocacaoAulaOrdenado != custosAlocacaoAulaOrdenado.end();
		  ++itCustosAlocacaoAulaOrdenado)
      {
         Aula & aula = (*itCustosAlocacaoAulaOrdenado)->getAula();
         GGroup<Aula*>::iterator itAulasNaoAlocadas = aulasNaoAlocadas.find( &(aula) );

         // Checando se a aula do custo em questao ja foi alocada.
         if(itAulasNaoAlocadas != aulasNaoAlocadas.end())
         {
            Professor & professor = (*itCustosAlocacaoAulaOrdenado)->getProfessor();
			int linha_prof = professor.getIdOperacional();

            int coluna_matriz = solucaoInicial->getItHorariosProf(professor, aula.getDiaSemana());
			vector<Aula*>::iterator it_horarios_prof
				= solucaoInicial->getMatrizAulas()->at(linha_prof)->begin();

			it_horarios_prof += coluna_matriz;

            bool alocouProfAula = alocaAulaSeq(solucaoInicial,
				it_horarios_prof, total_horarios, professor, aula);

            if(alocouProfAula)
            {
				//Horario * h = NULL;
				//unsigned int i = 0;
				//for (i = coluna_matriz; i < (total_horarios + coluna_matriz); i++)
				//{
				//	ITERA_GGROUP(it_h, professor.horarios, Horario)
				//	{
				//		h = *(it_h);

				//		int horario_aula_id = (coluna_matriz)%(total_horarios);
				//		HorarioAula horario_aula1 = problemData.horarios_aula_ordenados[horario_aula_id];

				//		HorarioAula horario_aula2 = h->horario_aula;

				//		if ( h->dias_semana.find( aula.getDiaSemana() ) != h->dias_semana.end()
				//			&& horario_aula1->getId() == horario_aula2->getId() )
				//		{

				//		}
				//	}
				//}

				std::cout << "\nForam alocados " << aula.getTotalCreditos()
						  << " horarios CONSECUTIVOS para a aula da turma "
						  << aula.getTurma() << " da disciplina "
						  << aula.getDisciplina()->getCodigo()
						  << " no dia " << aula.getDiaSemana()
						  << " ao professor " << professor.getCpf() << std::endl;

               // Para n�o tentar alocar esse custo novamente.
               (*itCustosAlocacaoAulaOrdenado) = NULL;

               // Para n�o tentar alocar essa aula novamente.
               aulasNaoAlocadas.remove(itAulasNaoAlocadas);
            }
            else
            {
               std::cout << "\nTENTATIVA de alocacao de " << aula.getTotalCreditos()
						 << " horarios CONSECUTIVOS para a aula da turma " << aula.getTurma()
						 << " da disciplina " << aula.getDisciplina()->getCodigo()
						 << " no dia " << aula.getDiaSemana()
						 << " ao professor " << professor.getCpf()
						 << " FRACASSOU." << std::endl;

               // Para n�o tentar alocar esse custo novamente.
               (*itCustosAlocacaoAulaOrdenado) = NULL;		
            }
         }
         else
         {
            // Essa aula ja foi alocada. Portanto,
			 // devo remover esse custo sem aloca-lo.
            (*itCustosAlocacaoAulaOrdenado) = NULL;
         }
      }

      // Removendo os custos das aulas que foram alocadas na rodada atual.
      for(int p = (custosAlocacaoAulaOrdenado.size()-1); p >= 0; --p)
      {
         if(custosAlocacaoAulaOrdenado.at(p) == NULL)
		 {
            custosAlocacaoAulaOrdenado.erase(
				custosAlocacaoAulaOrdenado.begin()+p);
		 }
      }
   }

   if(aulasNaoAlocadas.size() > 0)
   {
      // Aulas que n�o puderam ser alocadas a nenhum professor.
      std::cout << "\nATENCAO: Existem aulas que nao foram alocadas a nenhum professor.\n";
      // exit(1);

      /*Estrutura respons�vel por referenciar os professores virtuais criados */
      std::vector<Professor*> professoresVirtuais;

      /* PAREI AQUI

      USAR ESSA ESTRUTURA INTERNA <professoresVirtuais> PARA TENTAR ALOCAR AS AULAS.
      ELA SO GUARDA UMA REFERENCIA DOS PROFESSORES VIRTUAIS CRIADOS.

      PARA EU DESCOBRIR A LINHA CORRESPONDENTE NA MATRIZ, BASTA USAR O GETIDOPERACIONAL.

      POR EQTO SO TEM 1 PROFESSOR VIRTUAL SENDO CRIADO. DEVE-SE TENTAR ALOCAR AS AULAS PARA ELE
      E, SOMENTE SE FOR NECESSARIO, CRIAR OUTRO PROFESSOR VIRTUAL.

      */

      //MatrizSolucao::iterator itPrimeiroProfVirtualAdd;

      // Criando o primeiro professor virtual.
      {
         // Criando a "agenda semanal" do novo professor
         std::vector<Aula*> * horariosNovoProfessor = new std::vector<Aula*>
            ((solucaoInicial->getTotalDias()*solucaoInicial->getTotalHorarios()), NULL);

         // Criando um professor virtual.
         Professor * novoProfessor = new Professor(true);

         /* Setando alguns dados para o novo professor */
         novoProfessor->tipo_contrato = *problemData.tipos_contrato.begin();

         ITERA_GGROUP(itDisciplina,problemData.disciplinas,Disciplina)
         {
            Magisterio * mag = new Magisterio();
            mag->disciplina = *itDisciplina;
            mag->setDisciplinaId(itDisciplina->getId());
            mag->setNota(10);
            mag->setPreferencia(1);
            novoProfessor->magisterio.add(mag);
         }

         ITERA_GGROUP(itCampus,problemData.campi,Campus)
            ITERA_GGROUP(itHorario,itCampus->horarios,Horario)
               novoProfessor->horarios.add(*itHorario);

         // Adicionando os hor�rios do novo professor � solu��o.
         solucaoInicial->addProfessor(*novoProfessor,*horariosNovoProfessor);

         // Adicionando o novo professor a todos os campus
         ITERA_GGROUP(itCampus,problemData.campi,Campus)
		 {
            itCampus->professores.add(novoProfessor);
		 }

         //itPrimeiroProfVirtualAdd = solucaoInicial->getMatrizAulas()->end();
         ////--itPrimeiroProfVirtualAdd;
      }

      // Enquanto todas as aulas n�o forem alocadas
      while(aulasNaoAlocadas.size() > 0)
      {
         // Obtendo um iterador para o primeiro professor virtual.
         //MatrizSolucao::iterator itProfessoresVirtuais = itPrimeiroProfVirtualAdd;
         std::vector<Professor*>::iterator itProfessoresVirtuais = professoresVirtuais.begin();

         //* Para todos os professores virtuais existentes,
		 // tentar alocar as aulas que n�o puderam ser alocadas
		 // a professores reais.
         // OBS.:
		 // Tenta-se alocar as aulas na ordem em que
		 // os professores virtuais est�o sendo criados.

         // Para cada professor virtual
         //for(; itProfessoresVirtuais != solucaoInicial->getMatrizAulas()->end(); ++itProfessoresVirtuais)
         for(; itProfessoresVirtuais != professoresVirtuais.end(); ++itProfessoresVirtuais)
         {
            //(*itProfessoresVirtuais)->
            //std::vector<Aula*>::iterator itHorariosProfVirtual = 
            //   solucaoInicial->getItHorariosProf(,itAula->getDiaSemana());

            // bool alocouAula = alocaAula(itHorariosProfVirtual, solucaoInicial->getTotalHorarios(),
			//							*itProfessoresVirtuais, *itAula);
         }
      }






      exit(1);
   }

   // Aulas, que nem sequer foram associadas a algum professor.
   if(aulasNaoRelacionadasProf.size() > 0)
   {
      std::cout << "ATENCAO: Existem aulas sem professor associado, "
	            << "ou seja, nao foi calculado um custo para ela pq o "
				<< "usuario nao associou a disciplina da aula "
				<< "em questao a nenhum professor." << std::endl;

      // CRIAR PROFESSOR VIRTUAL.
      // exit(1);
   }

   return *(solucaoInicial);
}

bool SolucaoInicialOperacional::alocaAulaSeq(SolucaoOperacional * solucao, vector< Aula * >::iterator itHorariosProf,
										  int totalHorariosConsiderados, Professor & professor, Aula & aula)
{
   bool alocou = false;

   int diaSemana = aula.getDiaSemana();
   int idOperacionalProf = professor.getIdOperacional();

   // A ideia aqui � iterar sobre os hor�rios letivos do professor, para o dia em quest�o at�
   // que o primeiro hor�rio livre seja encontrado (pode ser que o professor n�o possua nenhum hor�rio 
   // livre). Uma vez encontrado o hor�rio livre, a ideia agora � obter, sequencialmente, o total de hor�rios
   // livres demandados pela disciplina da aula em quest�o. S� ent�o uma aloca��o pode ser conclu�da.

   bool sequenciaDeHorariosLivres = false;
   vector< Aula * >::iterator itInicioHorariosAlocar = itHorariosProf;
   int totalCredsAlocar = aula.getTotalCreditos();

   int horario = 0;
   for(; horario < totalHorariosConsiderados;
	   ++horario, ++itHorariosProf)
   {
      // Se a aula n�o for 'NULL'
      if(*itHorariosProf)
      {
         sequenciaDeHorariosLivres = false;
         totalCredsAlocar = aula.getTotalCreditos();
         continue;
      }

      sequenciaDeHorariosLivres = true;
      if(totalCredsAlocar == aula.getTotalCreditos())
	  {
         itInicioHorariosAlocar = itHorariosProf;
		 
	  }

      // Se a disciplina possuir apenas um cr�dito
	  // para o dia em quest�o, basta aloc�-la.
      if(aula.getTotalCreditos() == 1)
      {
         *itHorariosProf = &aula;
         alocou = true;
         break;
      }
      else if(aula.getTotalCreditos() > 1)
      {
         // Caso a disciplina possua mais de um cr�dito
		 // a ser alocado, a ideia aqui � aloca-los em 
         // sequ�ncia. Portanto, devo verificar se os
		 // cr�ditos demandados est�o livres.

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

   // Marca a posi��o do vector de aulas do
   // professor onde ser�o alocadas as aulas
   int coluna_matriz = std::distance(
	   solucao->getMatrizAulas()->at(idOperacionalProf)->begin(), itHorariosProf );

   int dia_semana = ( (coluna_matriz / solucao->getTotalHorarios() ) + 1 );
   int id_horario_aula = ( coluna_matriz % solucao->getTotalHorarios() );
   HorarioAula * horario_aula = problemData.horarios_aula_ordenados[ id_horario_aula ];

   // Se encontrei uma sequ�ncia de hor�rios livres, aloco.
   if(sequenciaDeHorariosLivres)
   {
	   bool encontrou = false;
	   Horario * horario = NULL;

	   GGroup< Horario * >::iterator it_horario
		   = professor.horarios.begin();
	   for (int i = 0; i < totalCredsAlocar; i++)
	   {
		   encontrou = false;
		   id_horario_aula += i;
		   horario_aula = problemData.horarios_aula_ordenados[ id_horario_aula ];

		   for (; it_horario != professor.horarios.end() && !encontrou;
				 it_horario++)
		   {
			   horario = *(it_horario);
			   if (horario->dias_semana.find(dia_semana) != horario->dias_semana.end()
				   && horario->horario_aula->getId() == horario_aula->getId())
			   {
				   encontrou = true;

				   aula.horarios_profs_alocados.push_back(
					   std::make_pair( &(professor), horario ) );
			   }
		   }
	   }

      for(int h = 0; h < aula.getTotalCreditos();
		  ++h, ++itInicioHorariosAlocar)
	  {
		  // Aloca��o da aula
         (*itInicioHorariosAlocar) = &(aula);
	  }

      alocou = true;
   }

   return alocou;
}

void SolucaoInicialOperacional::executaFuncaoPrioridade()
{
   ITERA_GGROUP(itCampus, problemData.campi, Campus)
   {
      ITERA_GGROUP(itProfessor, itCampus->professores, Professor)
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
         //int horariosProfessor = (itProfessor->horarios.size());
         //int horariosCampus = (itCampus->horarios.size())+1;
         //int dispProf = -(horariosProfessor - horariosCampus);
         //itProfessor->setCustoDispProf(dispProf);
         itProfessor->setCustoDispProf(itCampus->horarios.size());

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

void SolucaoInicialOperacional::calculaCustoFixProf(Professor& prof , Aula& aula, unsigned idCusto, int custo, int maxHorariosCP)
{
   pair<Professor*,Aula*> chave (&prof,&aula);

   map<pair<Professor*,Aula*>,CustoAlocacao*>::iterator 
      itCustoProfTurma = custoProfTurma.find(chave);

   // Criando, se necess�rio, um novo CustoAlocacao dada a chave em quest�o.
   if(itCustoProfTurma == custoProfTurma.end())
   {
      custoProfTurma[chave] = new CustoAlocacao(prof,aula);
   }

   if(idCusto == 0 /*custoFixProfTurma*/)
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
      // Aqui, compartilha-se a ideia da terceira restri��o da fun��o de prioridade.
      int custoDispProfTurma = -(custo - (maxHorariosCP+1));
      custoProfTurma[chave]->addCustoDispProfTurma(custoDispProfTurma);
   }
   else
   {
      std::cout << "ERRO: idCusto (" << idCusto << ") INVALIDO.";
      exit(0);
   }
}
