#include "SolucaoInicialOperacional.h"

bool ordenaCustosAlocacao(CustoAlocacao * left, CustoAlocacao * right)
{
   bool result = (*left > *right);
   return result;
}

bool ordenaAulasPorCustoAlocacao(std::pair<Aula*,std::vector<CustoAlocacao*> > & left,
                                 std::pair<Aula*,std::vector<CustoAlocacao*> > & right)
{
   return *(left.second.begin()) > *(right.second.begin());
}

SolucaoInicialOperacional::SolucaoInicialOperacional(ProblemData & _problemData)
: problemData(_problemData)
{
   executaFuncaoPrioridade();

   // ----------------------------------------------------------------------
   // Inicializando a estrutura que mantem referencia para as 
   // aulas que não foram relacionadas (alocadas) a nenhum professor.
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
   // Inicializando a estrutura <custosAlocacaoAulaOrdenado>
   // para poder auxiliar na alocação de aulas a professores.
   map<pair<Professor*,Aula*>,CustoAlocacao*>::iterator 
      itCustoProfTurma = custoProfTurma.begin();

   // Para cada custo de alocação registrado, dada uma relação entre um professor e uma aula.
   for(;itCustoProfTurma != custoProfTurma.end(); ++itCustoProfTurma)
   {
      Aula * aula = &itCustoProfTurma->second->getAula();

      std::vector<std::pair<Aula*,std::vector<CustoAlocacao*> > >::iterator
         itCustosAlocacaoAulaOrdenado = custosAlocacaoAulaOrdenado.begin();

      // Para cada aula registrada.
      // Verificando a existência de um registro da aula em questão na estrutura <custosAlocacaoAulaOrdenado> .
      for(;itCustosAlocacaoAulaOrdenado != custosAlocacaoAulaOrdenado.end(); ++itCustosAlocacaoAulaOrdenado)
      {
         // Se encontrei a aula, paro.
         if(itCustosAlocacaoAulaOrdenado->first == aula)
            break;
      }

      // Se não existe um registro, crio um novo. Caso contrário, somente adiciono o <custoAlocacao>.
      if(itCustosAlocacaoAulaOrdenado == custosAlocacaoAulaOrdenado.end())
      {
         vector<CustoAlocacao*> novoVtCustoAlocacao;
         novoVtCustoAlocacao.push_back( itCustoProfTurma->second );

         std::pair<Aula*,std::vector<CustoAlocacao*> > novoElem
            (aula, novoVtCustoAlocacao);

         custosAlocacaoAulaOrdenado.push_back( novoElem );
      }
      else
         itCustosAlocacaoAulaOrdenado->second.push_back(itCustoProfTurma->second);
   }

   // ----------------------------------------------------------------------
   // Ordenando os custos de alocação de cada elemento da estrutura <custosAlocacaoAulaOrdenado>.
   std::vector<std::pair<Aula*,std::vector<CustoAlocacao*> > >::iterator
      itCustosAlocacaoAulaOrdenado = custosAlocacaoAulaOrdenado.begin();

   for(;itCustosAlocacaoAulaOrdenado != custosAlocacaoAulaOrdenado.end(); ++itCustosAlocacaoAulaOrdenado)
   {
      std::sort( itCustosAlocacaoAulaOrdenado->second.begin(),
         itCustosAlocacaoAulaOrdenado->second.end(), ordenaCustosAlocacao );
   }

   // Ordenando a estrutura <custosAlocacaoAulaOrdenado> de acordo com o maior
   // custoAlocacao associado a cada aula.
   std::sort(custosAlocacaoAulaOrdenado.begin(),
      custosAlocacaoAulaOrdenado.end(), ordenaAulasPorCustoAlocacao);
   // ----------------------------------------------------------------------
}

SolucaoInicialOperacional::~SolucaoInicialOperacional()
{

}

SolucaoOperacional & SolucaoInicialOperacional::geraSolucaoInicial()
{
   SolucaoOperacional * solucaoInicial = new SolucaoOperacional(&problemData);

   /*
   O algoritmo consiste em 2 etapas.

   1- Alocar o máximo de aulas que possuem algum <CustoAlocacao> atribuído.
   1.1 Tenta-se alocar

   2- Alocar as aulas que não possuem nenhum <CustoAlocacao> atribuído.

   */

   /* Enquanto todas as aulas não forem alocadas */
   while(!custosAlocacaoAulaOrdenado.empty())
   {
      std::vector<std::pair<Aula*,std::vector<CustoAlocacao*> > >::iterator
         itCustosAlocacaoAulaOrdenado = custosAlocacaoAulaOrdenado.begin();

      for(; itCustosAlocacaoAulaOrdenado != custosAlocacaoAulaOrdenado.end(); ++itCustosAlocacaoAulaOrdenado)
      {
         Aula & aula = *(itCustosAlocacaoAulaOrdenado->first);
         CustoAlocacao & custoAlocacaoAula = **(itCustosAlocacaoAulaOrdenado->second.begin());

         Professor & professor = custoAlocacaoAula.getProfessor();

         bool alocouProfAula = alocaAula(*solucaoInicial,professor,aula);
         if(alocouProfAula)
         {
            cout << "\nForam alocados " << aula.getTotalCreditos() << " horarios CONSECUTIVOS para a aula da turma " << aula.getTurma() << " da disciplina " <<
               aula.getDisciplina()->getCodigo() << " no dia " << aula.getDiaSemana() << " ao professor "
               << professor.getCpf() << endl;

            // Para não tentar alocar essa aula novamente.
            itCustosAlocacaoAulaOrdenado->second.clear();
         }
         else
         {
            cout << "\nTENTATIVA de alocacao de " << aula.getTotalCreditos() << " horarios CONSECUTIVOS para a aula da turma " << aula.getTurma() << " da disciplina " <<
               aula.getDisciplina()->getCodigo() << " no dia " << aula.getDiaSemana() << " ao professor "
               << professor.getCpf() << " FRACASSOU." << endl;
         }
      }

      // Removendo as aulas que foram alocadas na rodada atual.
      for(int p = (custosAlocacaoAulaOrdenado.size()-1); p >= 0; --p)
      {
         if(custosAlocacaoAulaOrdenado.at(p).second.empty())
            custosAlocacaoAulaOrdenado.erase(custosAlocacaoAulaOrdenado.begin()+p);
      }
   }

   if(!custosAlocacaoAulaOrdenado.empty())
   {
      // Aulas que não puderam ser alocadas a nenhum professor.
      std::cout << "ATENCAO: Existem aulas sem professor associado, "
         << "ou seja, nao foi calculado um custo para ela pq o "
         << "usuario nao associou a disciplina da aula em questao a nenhum professor." << std::endl;

      exit(1);
   }

   // Aulas, que nem sequer foram associadas a algum professor.
   if(aulasNaoRelacionadasProf.size() > 0)
   {
      std::cout << "ATENCAO: Existem aulas sem professor associado, "
				<< "ou seja, nao foi calculado um custo para ela pq o "
				<< "usuario nao associou a disciplina da aula em "
				<< "questao a nenhum professor." << std::endl;

      // CRIAR PROFESSOR VIRTUAL.
      exit(1);
   }

   return *(solucaoInicial);
}

bool SolucaoInicialOperacional::alocaAula(SolucaoOperacional & solucaoOperacional,
										  Professor & professor, Aula & aula)
{
   bool alocou = false;

   int diaSemana = aula.getDiaSemana();
   int idOperacionalProf = professor.getIdOperacional();

   //* Otendo um iterador para o início dos horários letivos
   // do professor em questão em um dado dia (determinado pela aula).
   vector<Aula*>::iterator itHorarios = solucaoOperacional.getHorariosDia(professor,diaSemana);

   // A ideia aqui é iterar sobre os horários letivos do professor, para o dia em questão até
   // que o primeiro horário livre seja encontrado (pode ser que o professor não possua nenhum horário 
   // livre). Uma vez encontrado o horário livre, a ideia agora é obter, sequencialmente, o total de horários
   // livres demandados pela disciplina da aula em questão. Só então uma alocação pode ser concluída.

   bool sequenciaDeHorariosLivres = false;
   vector<Aula*>::iterator itInicioHorariosAlocar = itHorarios;
   int totalCredsAlocar = aula.getTotalCreditos();

   for(int horario = 0; horario < solucaoOperacional.getTotalHorarios();
	   ++horario, ++itHorarios)
   {
      if(*itHorarios) // != NULL
      {
         sequenciaDeHorariosLivres = false;
         totalCredsAlocar = aula.getTotalCreditos();
         continue;
      }

      sequenciaDeHorariosLivres = true;

      if(totalCredsAlocar == aula.getTotalCreditos())
         itInicioHorariosAlocar = itHorarios;

      // Se a disciplina possuir apenas um
	  // crédito para o dia em questão, basta alocá-la.
      if(aula.getTotalCreditos() == 1)
      {
         *itHorarios = &aula;
         alocou = true;
         break;
      }
      else if(aula.getTotalCreditos() > 1)
      {
         // Caso a disciplina possua mais de um crédito a ser
		 // alocado, a ideia aqui é aloca-los em  sequência.
		 // Portanto, devo verificar se os créditos demandados estão livres.

         // Atualizo o total de créditos alocados.
         --totalCredsAlocar;

         if(totalCredsAlocar == 0)
		 {
            // Posso parar a busca pq já
			// encontrei o total de créditos necessários.
            break;
		 }
      }
   }

   /* Se encontrei uma sequência de horários livres, aloco. */
   if(sequenciaDeHorariosLivres)
   {
      for(int horario = 0; horario < aula.getTotalCreditos(); ++horario, ++itInicioHorariosAlocar)
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
         /* Contabilizando os horários disponíveis de um professor. 

         Assim, toda vez que um CustoAlocacao for instanciado, o custo
         referente à "disponibilidade do professor p" será atualizado.

         Há necessidade de converter o valor para que seja somado corretamente.

         Ex. 
         custoDispProf_A = 10 -> 10 horarios disponiveis
         custoDispProf_B = 5 -> 5 horarios disponiveis

         custoDispProf_B tem prioridade MAIOR que custoDispProf_A.

         prioridade = -(custoDispProf_A - (TOTALHORARIOSCAMPUS+1))

         Dado que TOTALHORARIOSCAMPUS = 10, então:

         prioridade(custoDispProf_A) = 1
         prioridade(custoDispProf_A) = 6

         Estamos adimitindo aqui que a inst. possui apenas um CAMPUS.
         Não funcionará para multicampus.

         TODO : ADAPTAR TODO O COD PARA CONTEMPLAR MULTICAMPUS.
         */
         int horariosProfessor = (itProfessor->horarios.size());
         int horariosCampus = (itCampus->horarios.size()+1);
         int dispProf = -(horariosProfessor - horariosCampus);
         itProfessor->setCustoDispProf(dispProf);

         ITERA_GGROUP(itMagisterio,itProfessor->magisterio,Magisterio)
         {
            Disciplina * discMinistradaProf = itMagisterio->disciplina;

            // TODO : Teria que ser para toda aula do campus em questão.
            ITERA_GGROUP(itAula,problemData.aulas,Aula)
            {
               Disciplina * discAula = itAula->getDisciplina();

               if(discMinistradaProf == discAula)
               {
                  // Para o primeiro custo da função de prioridade, tenho que testar agora se
                  // existe fixação desse professor para a disciplina da aula em questão.

                  std::pair<Professor*,Disciplina*> chave (*itProfessor,discMinistradaProf);

                  std::map<std::pair<Professor*,Disciplina*>,GGroup<Fixacao*> >::iterator
                     itFixacoesProfDisc = problemData.fixacoesProfDisc.find(chave);

                  // Somente se existir, pelo menos, uma fixação de um professor a uma disciplina.
                  if(itFixacoesProfDisc != problemData.fixacoesProfDisc.end())
                  {
                     ITERA_GGROUP(itFixacao,itFixacoesProfDisc->second,Fixacao)
                     {
                        calculaCustoFixProf(**itProfessor,**itAula,0);
                     }
                  }

                  // Para o segundo custo considerado para o cálculo da função de prioridade, tenho
                  // que somar a nota (preferência) dada pelo professor para a disciplina em questão.

                  // Dado que a maior preferência recebe nota 1 e a menor recebe nota 10, basta subtrair a
                  // nota (preferência) por 11 e, em seguida, multiplicar o resultado por -1. Assim, somaremos
                  // um valor correto ao custo, já que o melhor custo total possuirá o maior valor atribuido.
                  calculaCustoFixProf(**itProfessor,**itAula,1,itMagisterio->getPreferencia());

                  pair<int,int> chaveGamb (itProfessor->getId(),discMinistradaProf->getId());

                  // Se o professor e a disciplina da aula em questão se relacionarem:
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

void SolucaoInicialOperacional::calculaCustoFixProf(Professor& prof , Aula& aula,
                                                    unsigned idCusto, int custo, int maxHorariosCP)
{
   pair<Professor*,Aula*> chave (&prof,&aula);

   map<pair<Professor*,Aula*>,CustoAlocacao*>::iterator 
      itCustoProfTurma = custoProfTurma.find(chave);

   // Criando, se necessário, um novo CustoAlocacao dada a chave em questão.
   if(itCustoProfTurma == custoProfTurma.end())
   {
      custoProfTurma[chave] = new CustoAlocacao(prof,aula);
   }

   if(idCusto == 0) // 'custoFixProfTurma'
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
      // Aqui, compartilha-se a ideia da terceira restrição da função de prioridade.
      int custoDispProfTurma = -(custo - (maxHorariosCP+1));
      custoProfTurma[chave]->addCustoDispProfTurma(custoDispProfTurma);
   }
   else
   {
      std::cout << "ERRO: idCusto (" << idCusto << ") INVALIDO.";
      exit(0);
   }
}
