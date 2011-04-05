#include "SolucaoInicialOperacional.h"

CustoAlocacao::CustoAlocacao(Professor & _professor, Aula & _aula)
: professor(_professor), aula(_aula)
//CustoAlocacao::CustoAlocacao()
{
   for(unsigned c = 0; c <= 3; c++)
   {
      custosAlocacao.push_back(.0);
   }

   custo_total = .0;

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
   this->custosAlocacao = (cstAlc.custosAlocacao);
   this->custo_total = (cstAlc.custo_total);
   this->alfa = (cstAlc.alfa);
   this->beta = (cstAlc.beta);
   this->teta = (cstAlc.teta);
   this->gamma = (cstAlc.gamma);
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
   return custo_total;
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
   custo_total += (alfa*c);
}

void CustoAlocacao::addCustoPrefProfTurma(double c)
{
   custosAlocacao.at(1) += c;
   custo_total += (beta*c);
}

void CustoAlocacao::addCustoDispProf(double c)
{
   custosAlocacao.at(2) += c;
   custo_total += (teta*c);
}

void CustoAlocacao::addCustoDispProfTurma(double c)
{
   custosAlocacao.at(3) += c;
   custo_total += (gamma*c);
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
   return (custo_total < right.custo_total);
}

bool CustoAlocacao::operator == (CustoAlocacao const & right)
{
   // Auto Ref. Check
   if(this == &right)
   {
      return true;
   }

   return (
	   (alfa == right.alfa) && (beta == right.beta) &&
	   (teta == right.teta) && (gamma == right.gamma) &&
	   (custosAlocacao.at(0) == right.custosAlocacao.at(0)) &&
	   (custosAlocacao.at(1) == right.custosAlocacao.at(1)) &&
	   (custosAlocacao.at(2) == right.custosAlocacao.at(2)) &&
	   (custosAlocacao.at(3) == right.custosAlocacao.at(3)) );
}

bool CustoAlocacao::operator > (CustoAlocacao const & right)
{
   return (custo_total > right.custo_total);
}

bool CustoAlocacao::operator >= (CustoAlocacao const & right)
{
   return (custo_total >= right.custo_total);
}

CustoAlocacao & CustoAlocacao::operator = (CustoAlocacao const & right)
{
   // Auto Ref. Check
   if(this == &right)
   {
      return *this;
   }

   return *new CustoAlocacao(right);
}