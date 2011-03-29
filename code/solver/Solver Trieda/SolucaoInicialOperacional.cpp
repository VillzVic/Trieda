#include "SolucaoInicialOperacional.h"

CustoAlocacao::CustoAlocacao(Professor & _professor, Aula & _aula) : professor(_professor), aula(_aula)
//CustoAlocacao::CustoAlocacao()
{
   for(unsigned c=0; c<=3; ++c)
   {
      custosAlocacao.push_back(.0);
   }

   custoTotal = .0;

   alfa = 1.0;
   beta = 1.0;
   teta = 1.0;
   gamma = 1.0;

   // Setando o custo referente à "disponibilidade do professor p".
   addCustoDispProf(professor.getCustoDispProf());


}

CustoAlocacao::CustoAlocacao(CustoAlocacao const & cstAlc) :
professor(cstAlc.professor), aula(cstAlc.aula)
{
   this->custosAlocacao = cstAlc.custosAlocacao;
   this->custoTotal = cstAlc.custoTotal;
   this->alfa = cstAlc.alfa;
   this->beta = cstAlc.beta;
   this->teta = cstAlc.teta;
   this->gamma = cstAlc.gamma;
}

CustoAlocacao::~CustoAlocacao()
{
}

Professor & CustoAlocacao::getProfessor() const
{
   return professor;
}

Aula & CustoAlocacao::getAula() const
{
   return aula;
}

double CustoAlocacao::getCustoFixProfTurma() const
{
   return custosAlocacao.at(0);
}

double CustoAlocacao::getCustoPrefProfTurma() const
{
   return custosAlocacao.at(1);
}
double CustoAlocacao::getCustoDispProf() const
{
   return custosAlocacao.at(2);
}

double CustoAlocacao::getCustoDispProfTurma() const
{
   return custosAlocacao.at(3);
}

double CustoAlocacao::getCustoTotal() const
{
   return custoTotal;
}

double CustoAlocacao::getAlfa() const
{
   return alfa;
}

double CustoAlocacao::getBeta() const
{
   return beta;
}

double CustoAlocacao::getTeta() const
{
   return teta;
}

double CustoAlocacao::getGamma() const
{
   return gamma;
}

void CustoAlocacao::addCustoFixProfTurma(double c)
{
   custosAlocacao.at(0) += c;
   custoTotal += (alfa*c);
}

void CustoAlocacao::addCustoPrefProfTurma(double c)
{
   custosAlocacao.at(1) += c;
   custoTotal += (beta*c);
}

void CustoAlocacao::addCustoDispProf(double c)
{
   custosAlocacao.at(2) += c;
   custoTotal += (teta*c);
}

void CustoAlocacao::addCustoDispProfTurma(double c)
{
   custosAlocacao.at(3) += c;
   custoTotal += (gamma*c);
}

void CustoAlocacao::setAlfa(double p)
{
   alfa = p;
}

void CustoAlocacao::setBeta(double p)
{
   beta = p;
}

void CustoAlocacao::setTeta(double p)
{
   teta = p;
}

void CustoAlocacao::setGamma(double p)
{
   gamma = p;
}

bool CustoAlocacao::operator < (CustoAlocacao const & right)
{
   //return (professor < right.professor && aula < right.aula);
   //return custoTotal > right.custoTotal;
   return custoTotal < right.custoTotal;
}

bool CustoAlocacao::operator == (CustoAlocacao const & right)
{
   // Auto Ref. Check
   if(this == &right)
      return true;

   //return (professor == right.professor && aula == right.aula);
   return (alfa == right.alfa) && (beta == right.beta) && (teta == right.teta) && (gamma == right.gamma) && (custosAlocacao.at(0) == right.custosAlocacao.at(0)) && (custosAlocacao.at(1) == right.custosAlocacao.at(1)) && (custosAlocacao.at(2) == right.custosAlocacao.at(2)) && (custosAlocacao.at(3) == right.custosAlocacao.at(3));
}

bool CustoAlocacao::operator > (CustoAlocacao const & right)
{
   //return (!(*this < right) && !(*this == right));
   return custoTotal > right.custoTotal;
}

CustoAlocacao & CustoAlocacao::operator = (CustoAlocacao const & right)
{
   // Auto Ref. Check
   if(this == &right)
      return *this;

   return *new CustoAlocacao(right);
}

// ============================================================================
// ============================================================================

bool ordenaCustosAlocacao(CustoAlocacao * left, CustoAlocacao * right)
{
   //return *left > *right;

   bool result = *left > *right;
   return result;
   //return left > right;
}

bool ordenaAulasPorCustoAlocacao(std::pair<Aula*,std::vector<CustoAlocacao*> > & left, std::pair<Aula*,std::vector<CustoAlocacao*> > & right)
{
   return *(left.second.begin()) > *(right.second.begin());
}


SolucaoInicialOperacional::SolucaoInicialOperacional(ProblemData & _problemData) : problemData(_problemData)
{
   executaFuncaoPrioridade();
   //preencheEstruturaCustoProfTurmaOrdenado();

   // ----------------------------------------------------------------------
   /* Inicializando a estrutura que mantem referencia para as 
   aulas que não foram relacionadas (alocadas) a nenhum professor. */

   ITERA_GGROUP(itAula,problemData.aulas,Aula)
   { 
      map<pair<Professor*,Aula*>,CustoAlocacao*>::iterator
         itCustoProfTurma = custoProfTurma.begin();

      for(; itCustoProfTurma != custoProfTurma.end(); ++itCustoProfTurma)
      {
         if(itCustoProfTurma->first.second->getDisciplina() == itAula->getDisciplina())
            break;
      }

      if(itCustoProfTurma == custoProfTurma.end())
         aulasNaoRelacionadasProf.add(*itAula);
   }
   // ----------------------------------------------------------------------
   /* Inicializando a estrutura <custosAlocacaoAulaOrdenado> para poder auxiliar na
   alocação de aulas a professores. */
   map<pair<Professor*,Aula*>,CustoAlocacao*>::iterator 
      itCustoProfTurma = custoProfTurma.begin();

   // Para cada custo de alocação registrado, dada uma relação entre um professor e uma aula.
   for(;itCustoProfTurma != custoProfTurma.end(); ++itCustoProfTurma)
   {
      Aula * aula = &itCustoProfTurma->second->getAula();

      std::vector<std::pair<Aula*,std::vector<CustoAlocacao*> > >::iterator
         itCustosAlocacaoAulaOrdenado = custosAlocacaoAulaOrdenado.begin();

      /* Para cada aula registrada.
      Verificando a existência de um registro da aula em questão na estrutura <custosAlocacaoAulaOrdenado> .
      */
      for(;itCustosAlocacaoAulaOrdenado != custosAlocacaoAulaOrdenado.end(); ++itCustosAlocacaoAulaOrdenado)
      {
         // Se encontrei a aula, paro.
         if(itCustosAlocacaoAulaOrdenado->first == aula)
            break;
      }

      /* Se não existe um registro, crio um novo. Caso contrário, somente adiciono o <custoAlocacao>. */
      if(itCustosAlocacaoAulaOrdenado == custosAlocacaoAulaOrdenado.end())
      {
         vector<CustoAlocacao*> novoVtCustoAlocacao;
         novoVtCustoAlocacao.push_back(itCustoProfTurma->second);

         std::pair<Aula*,std::vector<CustoAlocacao*> > novoElem
            (aula,novoVtCustoAlocacao);

         custosAlocacaoAulaOrdenado.push_back(novoElem);
      }
      else
         itCustosAlocacaoAulaOrdenado->second.push_back(itCustoProfTurma->second);
   }
   // ----------------------------------------------------------------------
   /* Ordenando os custos de alocação de cada elemento da estrutura <custosAlocacaoAulaOrdenado>. */
   std::vector<std::pair<Aula*,std::vector<CustoAlocacao*> > >::iterator
      itCustosAlocacaoAulaOrdenado = custosAlocacaoAulaOrdenado.begin();

   for(;itCustosAlocacaoAulaOrdenado != custosAlocacaoAulaOrdenado.end(); ++itCustosAlocacaoAulaOrdenado)
   {
      sort(itCustosAlocacaoAulaOrdenado->second.begin(),itCustosAlocacaoAulaOrdenado->second.end(),ordenaCustosAlocacao);

      //cout << " =========================== " << endl;
      //for(unsigned i = 0; i < itCustosAlocacaoAulaOrdenado->second.size(); ++i)
      //   cout << "\t*: " << itCustosAlocacaoAulaOrdenado->second.at(i)->getCustoTotal() << endl;
   }
   /* Ordenando a estrutura <custosAlocacaoAulaOrdenado> de acordo com o maior
   custoAlocacao associado a cada aula. */
   sort(custosAlocacaoAulaOrdenado.begin(),custosAlocacaoAulaOrdenado.end(),ordenaAulasPorCustoAlocacao);
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
         Aula & aula = *itCustosAlocacaoAulaOrdenado->first;

         CustoAlocacao & custoAlocacaoAula = **itCustosAlocacaoAulaOrdenado->second.begin();

         Professor & professor = custoAlocacaoAula.getProfessor();

         bool alocouProfAula = alocaAula(*solucaoInicial,professor,aula);

         if(alocouProfAula)
         {
            /* Para não tentar alocar essa aula novamente. */
            itCustosAlocacaoAulaOrdenado->second.clear();
         }
         else
         {
            cout << "AULA NAO ALOCADA  !!!! Continuar algoritmo.. . "  << endl;
            exit(1);
         }

      }

      // Removendo as aulas alocadas.
      for(itCustosAlocacaoAulaOrdenado = custosAlocacaoAulaOrdenado.end();
         itCustosAlocacaoAulaOrdenado != custosAlocacaoAulaOrdenado.begin();
         --itCustosAlocacaoAulaOrdenado)
      {
         if(itCustosAlocacaoAulaOrdenado->second.empty())
            custosAlocacaoAulaOrdenado.erase(itCustosAlocacaoAulaOrdenado);
      }

   }

   if(!custosAlocacaoAulaOrdenado.empty())
   {
      /* Aulas que não puderam ser alocadas a nenhum professor. */
      cout << "ATENCAO: Existem aulas sem professor associado, ou seja, nao foi calculado um custo para ela pq o usuario nao associou a disciplina da aula em questao a nenhum professor." << endl;
      exit(1);
   }

   /* Aulas, que nem sequer foram associadas a algum professor. */
   if(aulasNaoRelacionadasProf.size() > 0)
   {
      cout << "ATENCAO: Existem aulas sem professor associado, ou seja, nao foi calculado um custo para ela pq o usuario nao associou a disciplina da aula em questao a nenhum professor." << endl;
      // CRIAR PROFESSOR VIRTUAL.
      exit(1);
   }

   cout << "SolucaoOperacional & SolucaoInicialOperacional::geraSolucaoInicial() NAO IMPLEMENTADO !!!" << endl;
   exit(0);

   return *solucaoInicial;
}

bool SolucaoInicialOperacional::alocaAula(SolucaoOperacional & solucaoOperacional, Professor & professor, Aula & aula)
{
   int diaSemana = aula.getDiaSemana();

   int idOperacionalProf = professor.getIdOperacional();

   /* PAREI AQUI 
   
   FAZER ESSA FUNÇÃO RETORNAR UMA VETOR DE PONTEIROS PARA OS HORARIOS LIVRES.
   MAIS FACIL DO QUE UTILIZAR OS PONTEIROS. NAO SEI PQ NAO ESTA FUNCIONANDO.
   */
   vector<Aula*>::iterator itHorarios = solucaoOperacional.getHorariosDia(professor,diaSemana);

   //for(; itHorario = solucaoOperacional.getMatrizAulas()->at(idOperacionalProf)->begin())
   //for(int horario = 0; horario < solucaoOperacional.getTotalHorarios(); ++horario, ++itHorarios)
   for(int horario = 0; horario < (solucaoOperacional.getTotalHorarios()-1); ++horario)
   {
      cout << "Aloquei horario: " << horario << endl;
      //if(*itHorarios == NULL) // != NULL
      //{
      //   // Antes de alocar, ver se o prof. dispoe da quantidade de creds. livres necessária.
      //   //exit(1);
      //}
      
      //++itHorarios;
   }

   //vector<Aula*>::iterator itHorarios

   cout << "IMPLEMENTAR: SolucaoInicialOperacional::alocaAula(SolucaoOperacional & solucaoOperacional, Professor & professor, Aula & aula)" << endl;
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
         int dispProf = -(itProfessor->horarios.size() - (itCampus->horarios.size()+1));
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
                  /*
                  Para o primeiro custo da função de prioridade, tenho que testar agora se
                  existe fixação desse professor para a disciplina da aula em questão.
                  */

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

                  /* Para o segundo custo considerado para o cálculo da função de prioridade, tenho
                  que somar a nota (preferência) dada pelo professor para a disciplina em questão.
                  
                  Dado que a maior preferência recebe nota 1 e a menor recebe nota 10, basta subtrair a
                  nota (preferência) por 11 e, em seguida, multiplicar o resultado por -1. Assim, somaremos
                  um valor correto ao custo, já que o melhor custo total possuirá o maior valor atribuido.
                  */
                  calculaCustoFixProf(**itProfessor,**itAula,1,itMagisterio->getPreferencia());

                  pair<int,int> chaveGamb (itProfessor->getId(),discMinistradaProf->getId());

                  /* Se o professor e a disciplina da aula em questão se relacionarem: */
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

void SolucaoInicialOperacional::calculaCustoFixProf(Professor & prof ,Aula & aula, unsigned idCusto, int custo, int maxHorariosCP)
{
   pair<Professor*,Aula*> chave (&prof,&aula);
   //pair<Professor*,Aula*> chave (*itProfessor,*itAula);

   map<pair<Professor*,Aula*>,CustoAlocacao*>::iterator 
      itCustoProfTurma = custoProfTurma.find(chave);

   // Criando, se necessário, um novo CustoAlocacao dada a chave em questão.
   if(itCustoProfTurma == custoProfTurma.end())
      custoProfTurma[chave] = new CustoAlocacao(prof,aula);

   if(idCusto == 0 /*custoFixProfTurma*/)
      custoProfTurma[chave]->addCustoFixProfTurma(1);
   else if(idCusto == 1)
   {
      int preferenciaProfDisc = (custo - 11) * (-1);
      custoProfTurma[chave]->addCustoPrefProfTurma(preferenciaProfDisc);
   }
   else if (idCusto == 3)
   {
      /* Aqui, compartilha-se a ideia da terceira restrição da função de prioridade. */
      int custoDispProfTurma = -(custo - (maxHorariosCP+1));
      custoProfTurma[chave]->addCustoDispProfTurma(custoDispProfTurma);
   }
   else
   { std::cout << "ERRO: idCusto (" << idCusto << ") INVALIDO."; exit(0); }
}
