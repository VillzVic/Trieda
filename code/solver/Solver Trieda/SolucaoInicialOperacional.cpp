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
   // aulas que n�o foram relacionadas (alocadas) a nenhum professor.
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
   // para poder auxiliar na aloca��o de aulas a professores.
   map<pair<Professor*,Aula*>,CustoAlocacao*>::iterator 
      itCustoProfTurma = custoProfTurma.begin();

   // Para cada custo de aloca��o registrado, dada uma rela��o entre um professor e uma aula.
   for(;itCustoProfTurma != custoProfTurma.end(); ++itCustoProfTurma)
   {
      Aula * aula = &itCustoProfTurma->second->getAula();

      std::vector<std::pair<Aula*,std::vector<CustoAlocacao*> > >::iterator
         itCustosAlocacaoAulaOrdenado = custosAlocacaoAulaOrdenado.begin();

      // Para cada aula registrada.
      // Verificando a exist�ncia de um registro da aula em quest�o na estrutura <custosAlocacaoAulaOrdenado> .
      for(;itCustosAlocacaoAulaOrdenado != custosAlocacaoAulaOrdenado.end(); ++itCustosAlocacaoAulaOrdenado)
      {
         // Se encontrei a aula, paro.
         if(itCustosAlocacaoAulaOrdenado->first == aula)
		 {
            break;
		 }
      }

      // Se n�o existe um registro, crio um novo. Caso contr�rio, somente adiciono o <custoAlocacao>.
      if(itCustosAlocacaoAulaOrdenado == custosAlocacaoAulaOrdenado.end())
      {
         vector<CustoAlocacao*> novoVtCustoAlocacao;
         novoVtCustoAlocacao.push_back( itCustoProfTurma->second );

         std::pair<Aula*,std::vector<CustoAlocacao*> > novoElem
            (aula, novoVtCustoAlocacao);

         custosAlocacaoAulaOrdenado.push_back( novoElem );
      }
      else
	  {
         itCustosAlocacaoAulaOrdenado->second.push_back(itCustoProfTurma->second);
	  }
   }

   // ----------------------------------------------------------------------
   // Ordenando os custos de aloca��o de cada elemento da estrutura <custosAlocacaoAulaOrdenado>.
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

   1- Alocar o m�ximo de aulas que possuem algum <CustoAlocacao> atribu�do.
      1.1 Tenta-se alocar

   2- Alocar as aulas que n�o possuem nenhum <CustoAlocacao> atribu�do.

   */

   /* Enquanto todas as aulas n�o forem alocadas */
   while(!custosAlocacaoAulaOrdenado.empty())
   {

      std::vector<std::pair<Aula*,std::vector<CustoAlocacao*> > >::iterator
         itCustosAlocacaoAulaOrdenado = custosAlocacaoAulaOrdenado.begin();

      for(; itCustosAlocacaoAulaOrdenado != custosAlocacaoAulaOrdenado.end(); ++itCustosAlocacaoAulaOrdenado)
      {
         Aula & aula = *(itCustosAlocacaoAulaOrdenado->first);
         CustoAlocacao& custoAlocacaoAula = **(itCustosAlocacaoAulaOrdenado->second.begin());
         Professor& professor = custoAlocacaoAula.getProfessor();

         bool alocouProfAula = alocaAula(*solucaoInicial,professor,aula);
         if(alocouProfAula)
         {
            // Para n�o tentar alocar essa aula novamente.
            itCustosAlocacaoAulaOrdenado->second.clear();
         }
         else
         {
			 std::cout << "AULA NAO ALOCADA  !!!! Continuar algoritmo.. . "  << std::endl;
             exit(1);
         }
      }

      // Removendo as aulas alocadas.
      for(itCustosAlocacaoAulaOrdenado = custosAlocacaoAulaOrdenado.end();
         itCustosAlocacaoAulaOrdenado != custosAlocacaoAulaOrdenado.begin();
         --itCustosAlocacaoAulaOrdenado)
      {
         if(itCustosAlocacaoAulaOrdenado->second.empty())
		 {
            custosAlocacaoAulaOrdenado.erase(itCustosAlocacaoAulaOrdenado);
		 }
      }
   }

   if(!custosAlocacaoAulaOrdenado.empty())
   {
      // Aulas que n�o puderam ser alocadas a nenhum professor.
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
		  << "usuario nao associou a disciplina da aula em questao a nenhum professor." << std::endl;

      // CRIAR PROFESSOR VIRTUAL.
      exit(1);
   }

   std::cout << "SolucaoOperacional & SolucaoInicialOperacional::geraSolucaoInicial() NAO IMPLEMENTADO !!!" << std::endl;
   exit(0);

   return *(solucaoInicial);
}

bool SolucaoInicialOperacional::alocaAula(SolucaoOperacional & solucaoOperacional, Professor & professor, Aula & aula)
{
   int diaSemana = aula.getDiaSemana();
   int idOperacionalProf = professor.getIdOperacional();

   // PAREI AQUI:
   // FAZER ESSA FUN��O RETORNAR UMA VETOR DE PONTEIROS PARA OS HORARIOS LIVRES.
   // MAIS FACIL DO QUE UTILIZAR OS PONTEIROS. NAO SEI PQ NAO ESTA FUNCIONANDO.
   vector<Aula*>::iterator itHorarios = solucaoOperacional.getHorariosDia(professor,diaSemana);

   //for(; itHorario = solucaoOperacional.getMatrizAulas()->at(idOperacionalProf)->begin())
   //for(int horario = 0; horario < solucaoOperacional.getTotalHorarios(); ++horario, ++itHorarios)
   for(int horario = 0; horario < (solucaoOperacional.getTotalHorarios()-1); ++horario)
   {
      std::cout << "Aloquei horario: " << horario << std::endl;
      //if(*itHorarios == NULL) // != NULL
      //{
      //   // Antes de alocar, ver se o prof. dispoe da quantidade de creds. livres necess�ria.
      //   //exit(1);
      //}
      
      //++itHorarios;
   }

   //vector<Aula*>::iterator itHorarios

   std::cout << "IMPLEMENTAR: SolucaoInicialOperacional::alocaAula( "
			 << "SolucaoOperacional & solucaoOperacional, Professor & professor, Aula & aula)" << std::endl;

   exit(1);
   return false;
}

//bool SolucaoInicialOperacional::alocaAula(SolucaoOperacional & solucaoOperacional, 
//               Professor & professor,
//               int dia, 
//               Horario & horario, Aula & aula)
//{
//   return solucaoOperacional.alocaAula(professor,dia,horario,aula);
//}

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

//void SolucaoInicialOperacional::preencheEstruturaCustoProfTurmaOrdenado()
//{
//   map<pair<Professor*,Aula*>,CustoAlocacao*>::iterator 
//      itCustoProfTurma = custoProfTurma.begin();
//
//   for(; itCustoProfTurma != custoProfTurma.end(); ++itCustoProfTurma)
//      custoProfTurmaOrdenado.push_back(itCustoProfTurma->second);
//
//   make_heap(custoProfTurmaOrdenado.begin(),custoProfTurmaOrdenado.end());
//}

void SolucaoInicialOperacional::calculaCustoFixProf(Professor& prof , Aula& aula,
													unsigned idCusto, int custo, int maxHorariosCP)
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
