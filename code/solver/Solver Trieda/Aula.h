#ifndef _AULA_H_
#define _AULA_H_

#include <vector>

#include "GGroup.h"
#include "Disciplina.h"
#include "Oferta.h"
#include "Sala.h"

//#include "input.h"

class Aula
{
public:

   Aula( bool = false );

   virtual ~Aula( void );

   // Ofertas que são atendidas por essa aula
   GGroup< Oferta *, LessPtr<Oferta> > ofertas;

   void setTurma( int );
   void setDisciplina( Disciplina * );
   void setSala( Sala * );
   void setDiaSemana( int );
   void setCreditosTeoricos( int );
   void setCreditosPraticos( int );
   void setAulaVirtual( bool );
   void setAulaFixada( bool );

   int getTurma() const;
   Disciplina * getDisciplina() const;
   Sala * getSala() const;
   int getDiaSemana() const;
   int getCreditosTeoricos() const;
   int getCreditosPraticos() const;
   int getTotalCreditos() const;
   bool eVirtual() const;
   bool eFixada() const;

   virtual bool operator < ( Aula const & right )
   { 
      if ( *disciplina < *right.getDisciplina() )
         return true;
      else if ( *disciplina > *right.getDisciplina() )
         return false;

      if ( turma < right.getTurma() )
         return true;
      else if ( turma > right.getTurma() )
         return false;

      if ( dia_semana < right.getDiaSemana() )
         return true;
      else if ( dia_semana > right.getDiaSemana() )
         return false;

      if ( *sala < *right.getSala() )
         return true;
      else if ( *sala > *right.getSala() )
         return false;

      return false;
   }

   virtual bool operator > ( Aula const & right )
   { 
      if ( *disciplina > *right.getDisciplina() )
         return true;
      else if ( *disciplina < *right.getDisciplina() )
         return false;

      if ( turma > right.getTurma() )
         return true;
      else if ( turma < right.getTurma() )
         return false;

      if ( dia_semana > right.getDiaSemana() )
         return true;
      else if ( dia_semana < right.getDiaSemana() )
         return false;

      if ( *sala > *right.getSala() )
         return true;
      else if ( *sala < *right.getSala() )
         return false;

      return false;
   }

   virtual bool operator == ( Aula const & right )
   { 
      return  (
         //( ofertas == right.ofertas ) &&
         ( turma == right.getTurma() ) &&
         ( *disciplina == *right.getDisciplina() ) &&
         ( *sala == *right.getSala() ) &&
         ( dia_semana == right.getDiaSemana() ) &&
         ( creditos_teoricos == right.getCreditosTeoricos() ) &&
         ( creditos_praticos == right.getCreditosPraticos() ) );
   }

   virtual bool operator != ( Aula const & right )
   { 
      return ( !( *this == right ) );
   }

   void toString() const;

private:

   int turma;
   Disciplina * disciplina;
   Sala * sala;
   int dia_semana;
   int quantidade;
   int creditos_teoricos;
   int creditos_praticos;

   // Indica se uma aula é virtual ou não.
   bool aula_virtual;

   // Informa se uma aula possui fixação do tipo
   // professor + dia + horário, indicando que essa
   // aula não poderá ser trocada na matriz de solução
   bool aula_fixada;
};

#endif