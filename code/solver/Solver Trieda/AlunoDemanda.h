#ifndef _ALUNO_DEMANDA_H_
#define _ALUNO_DEMANDA_H_

#include "ofbase.h"
#include "Demanda.h"

class AlunoDemanda :
   public OFBase
{
public:
   AlunoDemanda( void );
   AlunoDemanda( int id, int alunoId, int prior, Demanda* demanda );
   virtual ~AlunoDemanda( void );

   virtual void le_arvore( ItemAlunoDemanda & );

   Demanda * demanda;

   Demanda * demandaOriginal; // Por default=NULL. Se houver substituição de equivalência por aluno, 
							  // adquire o valor da demanda original, e 'demanda' adquire o valor da nova demanda substituta.

   void setAlunoId( int value ) { this->alunoId = value; }
   void setDemandaId( int value ) { this->demandaId = value; }
   void setPrioridade( int p ) { this->prioridade = p; }

   int getPrioridade() const { return this->prioridade; }
   int getAlunoId() const { return this->alunoId; }
   int getDemandaId() const { return this->demandaId; }

   bool operator < ( const AlunoDemanda & var ) const
   {
      if ( (int) this->getId() < (int) var.getId() )
      {
         return true;
      }
      else if ( (int) this->getId() > (int) var.getId() )
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

      if ( (int) this->getDemandaId() < (int) var.getDemandaId() )
      {
         return true;
      }
      else if ( (int) this->getDemandaId() > (int) var.getDemandaId() )
      {
         return false;
      }

      if ( (int) this->getPrioridade() < (int) var.getPrioridade() )
      {
         return true;
      }
      else if ( (int) this->getPrioridade() > (int) var.getPrioridade() )
      {
         return false;
      }

      return false;
   }

   bool operator == ( const AlunoDemanda & var ) const
   {
      return ( !( *this < var ) && !( var < *this ) );
   }

private:
   int demandaId;
   int alunoId;
   int prioridade;

};

std::ostream & operator << ( std::ostream &, AlunoDemanda & );

#endif