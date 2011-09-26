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

   void setAlunoId( int value ) { this->alunoId = value; }
   void setDemandaId( int value ) { this->demandaId = value; }
   void setNomeAluno( std::string s ) { this->nomeAluno = s; }

   int getAlunoId() const { return this->alunoId; }
   int getDemandaId() const { return this->demandaId; }
   std::string getNomeAluno() const { return this->nomeAluno; }

   virtual bool operator < ( const AlunoDemanda & var ) const
   {
      if ( (int) this->getDemandaId() < (int) var.getDemandaId() )
      {
         return true;
      }
      else if ( (int) this->getDemandaId() > (int) var.getDemandaId() )
      {
         return false;
      }

      if ( (int) this->getAlunoId() < (int) var.getAlunoId() )
      {
         return true;
      }
      else if ( (int) this->getAlunoId() > (int) var.getAlunoId() )
      {
         return false;
      }

      return false;
   }

   virtual bool operator == ( const AlunoDemanda & var ) const
   {
      return ( !( *this < var ) && !( var < *this ) );
   }

private:
   int demandaId;
   int alunoId;
   std::string nomeAluno;
};

std::ostream & operator << ( std::ostream &, AlunoDemanda & );

#endif