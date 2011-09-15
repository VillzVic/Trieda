#ifndef _ALUNO_DEMANDA_H_
#define _ALUNO_DEMANDA_H_

#include "ofbase.h"
#include "Demanda.h"

class AlunoDemanda :
   public OFBase
{
public:
   AlunoDemanda( void );
   virtual ~AlunoDemanda( void );

   virtual void le_arvore( ItemAlunoDemanda & );

   Demanda * demanda;

   void setDemandaId( int value ) { demandaId = value; }
   void setNomeAluno( std::string s ) { nomeAluno = s; }

   int getDemandaId() const { return demandaId; }
   std::string getNomeAluno() const { return nomeAluno; }

private:
   int demandaId;
   std::string nomeAluno;
};

std::ostream & operator << ( std::ostream &, AlunoDemanda & );

#endif