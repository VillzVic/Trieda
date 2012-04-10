#ifndef _ALUNO_H_
#define _ALUNO_H_

#include "ofbase.h"
#include "AlunoDemanda.h"

class Aluno
{
public:
   Aluno( void );
   Aluno( int id, std::string nome, Oferta* oft );
   virtual ~Aluno( void );
         
   void setAlunoId( int value ) { this->alunoId = value; }
   void setNomeAluno( std::string s ) { this->nomeAluno = s; }
   void setOferta( Oferta* o ) { this->oferta = o; }
   void setOfertaId( int oftId ) { this->ofertaId = oftId; }

   int getAlunoId() const { return this->alunoId; }
   std::string getNomeAluno() const { return this->nomeAluno; }
   Oferta* getOferta() const { return this->oferta; }
   int getOfertaId() const { return this->ofertaId; }

   GGroup< AlunoDemanda*, LessPtr<AlunoDemanda> > demandas;
   
   bool demandaDisciplina( int idDisc );

   virtual bool operator < ( const Aluno & var ) const
   {
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

   virtual bool operator == ( const Aluno & var ) const
   {
      return ( !( *this < var ) && !( var < *this ) );
   }

private:

   int alunoId;
   std::string nomeAluno;
   Oferta *oferta;
   int ofertaId;

};

std::ostream & operator << ( std::ostream &, Aluno & );

#endif