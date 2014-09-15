#ifndef _ATENDIMENTO_SALA_H_
#define _ATENDIMENTO_SALA_H_

#include "ofbase.h"
#include "TRIEDA-OutputXSD.h"

class AtendimentoDiaSemana;
class Sala;

class AtendimentoSala:
   public OFBase
{
public:
   AtendimentoSala( int );
   AtendimentoSala( void );
   virtual ~AtendimentoSala( void );

   GGroup< AtendimentoDiaSemana* , LessPtr<AtendimentoDiaSemana> > * atendimentos_dias_semana;
   Sala * sala;

   void setSalaId( std::string s ) { this->sala_id = s; }
   std::string getSalaId() const { return this->sala_id; }

   // procura o atendimento diasemana para um determinado dia. se não encontra cria um novo e retorna-o
    AtendimentoDiaSemana* getAddAtendDiaSemana (int dia);

private:
   std::string sala_id;
};

std::ostream& operator << ( std::ostream &, AtendimentoSala & );

std::istream& operator >> ( std::istream &file, AtendimentoSala* const &ptrAtendSala);

#endif