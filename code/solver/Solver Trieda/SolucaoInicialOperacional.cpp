#include "SolucaoInicialOperacional.h"

CustoAlocacao::CustoAlocacao(Professor & _professor, Aula & _aula) : professor(_professor), aula(_aula)
{
   for(unsigned c=1; c<=4; ++c)
   {
      custosAlocacao[c] = .0;
   }

   custoTotal = 0;

   alfa = 1;
   beta = 1;
   teta = 1;
   gamma = 1;
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

void CustoAlocacao::setCustoFixProfTurma(double c)
{
   custosAlocacao.at(0) = c;
   custoTotal += (alfa*c);
}

void CustoAlocacao::setCustoPrefProfTurma(double c)
{
   custosAlocacao.at(1) = c;
   custoTotal += (beta*c);
}

void CustoAlocacao::setCustoDispProf(double c)
{
   custosAlocacao.at(2) = c;
   custoTotal += (teta*c);
}

void CustoAlocacao::setCustoDispProfTurma(double c)
{
   custosAlocacao.at(3) = c;
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
   return (professor < right.professor && aula < right.aula);
}

bool CustoAlocacao::operator == (CustoAlocacao const & right)
{
   return (professor == right.professor && aula == right.aula);
}


// ============================================================================
// ============================================================================


SolucaoInicialOperacional::SolucaoInicialOperacional(ProblemData & _problemData) : problemData(_problemData)
{
}

SolucaoInicialOperacional::~SolucaoInicialOperacional()
{
}

SolucaoOperacional & SolucaoInicialOperacional::geraSolucaoInicial()
{
   SolucaoOperacional * solucaoInicial = new SolucaoOperacional(&problemData);

   ITERA_GGROUP(itCampus,problemData.campi,Campus)
   {
      ITERA_GGROUP(itProfessor,itCampus->professores,Professor)
      {
         ITERA_GGROUP(itMagisterio,itProfessor->magisterio,Magisterio)
         {
            Disciplina * discMinistradaProf = itMagisterio->disciplina;

            ITERA_GGROUP(itAula,problemData.aulas,Aula)
            {
               Disciplina * discAula = itAula->getDisciplina();

               if(discMinistradaProf == discAula)
               {
                  /*
                  Para o primeiro custo da função de prioridade, tenho que testar agora se
                  existe fixação desse professor para a disciplina dessa aula em questão.

                  Existem duas possibilidades de fixação:
                  
                  1 - fixação de um professor a uma disciplina.
                  2 - fixação de um professor a uma disciplina em um dado dia.

                  No primeiro caso, devo associar um custo. Já no segundo caso, só associo um custo
                  se a aula em questão possuir o campo <diaSemana> igual ao dia da fixação encontrada.
                  */
               }
            }
         }
      }
   }

   return *solucaoInicial;
}

bool SolucaoInicialOperacional::alocaAula(SolucaoOperacional & solucaoOperacional, 
               Professor & professor, 
               int dia, 
               Horario & horario, Aula & aula)
{
   return solucaoOperacional.alocaAula(professor,dia,horario,aula);
}
