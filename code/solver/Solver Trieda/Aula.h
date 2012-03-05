#ifndef _AULA_H_
#define _AULA_H_

#include <vector>

#include "GGroup.h"
#include "Disciplina.h"
#include "Oferta.h"
#include "Sala.h"

class Aula
{
public:
   Aula( bool = false );
   virtual ~Aula( void );

   // Ofertas que são atendidas por essa aula
   GGroup< Oferta *, LessPtr< Oferta > > ofertas;

   void setTurma( int );
   void setDisciplina( Disciplina * );
   void setSala( Sala * );
   void setDiaSemana( int );
   void setCreditosTeoricos( int );
   void setCreditosPraticos( int );
   void setAulaVirtual( bool );
   void setAulaFixada( bool );
   void setQuantidade( int, Oferta* );
   void setDisciplinaSubstituida( Disciplina * );

   int getTurma() const;
   Disciplina * getDisciplina() const;
   Sala * getSala() const;
   int getDiaSemana() const;
   int getCreditosTeoricos() const;
   int getCreditosPraticos() const;
   int getTotalCreditos() const;
   bool eVirtual() const;
   bool eFixada() const;
   std::map<Oferta*,int> getQuantidade() const;
   int getQuantidadePorOft( Oferta *oft );
   int getQuantidadeTotal();
   Disciplina * getDisciplinaSubstituida() const;

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

      if( ofertas.size() < right.ofertas.size() )
         return true;
      else if ( ofertas.size() > right.ofertas.size() )
         return false;

      GGroup< Oferta *, LessPtr< Oferta > >::iterator it_this = this->ofertas.begin();
      GGroup< Oferta *, LessPtr< Oferta > >::iterator it_right = right.ofertas.begin();
      for (; it_this != this->ofertas.end(); it_this++, it_right++ )
      {
         if ( *it_this < *it_right )
            return true;
         if ( *it_this > *it_right )
            return false;
      }

      if( quantidade < right.quantidade )
         return true;
      else if ( quantidade > right.quantidade )
         return false;
      
	  if( disciplinaSubstituida == NULL && right.getDisciplinaSubstituida() != NULL )
		  return true;
	  else if( disciplinaSubstituida != NULL && right.getDisciplinaSubstituida() == NULL )
		  return false;
	  else if( disciplinaSubstituida != NULL && right.getDisciplinaSubstituida() != NULL )
	  {
		  if( *disciplinaSubstituida < *right.getDisciplinaSubstituida() )
				return true;
		  else if( *right.getDisciplinaSubstituida() < *disciplinaSubstituida )
				return false;
	  }
	  
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

      if( ofertas.size() > right.ofertas.size() )
         return true;
      else if ( ofertas.size() < right.ofertas.size() )
         return false;

      if( quantidade > right.quantidade )
         return true;
      else if ( quantidade < right.quantidade )
         return false;

	  if( disciplinaSubstituida == NULL && right.getDisciplinaSubstituida() != NULL )
		  return false;
	  else if( disciplinaSubstituida != NULL && right.getDisciplinaSubstituida() == NULL )
		  return true;
	  else if( disciplinaSubstituida != NULL && right.getDisciplinaSubstituida() != NULL )
	  {
		  if( *disciplinaSubstituida < *right.getDisciplinaSubstituida() )
				return false;
		  else if( *right.getDisciplinaSubstituida() < *disciplinaSubstituida )
				return true;
	  }

      return false;
   }

   virtual bool operator == ( Aula const & right )
   { 
      return  ( ( turma == right.getTurma() )
         && ( *disciplina == *right.getDisciplina() )
         && ( ofertas == right.ofertas )
         && ( *sala == *right.getSala() )
         && ( dia_semana == right.getDiaSemana() )
         && ( creditos_teoricos == right.getCreditosTeoricos() )
         && ( creditos_praticos == right.getCreditosPraticos() )
         && ( quantidade == right.quantidade ) 
         && ( *disciplinaSubstituida == *right.getDisciplinaSubstituida() ) );
   }

   virtual bool operator != ( Aula const & right )
   { 
      return ( !( *this == right ) );
   }

   void toString();

private:
   int turma;
   Disciplina * disciplina;
   Sala * sala;
   int dia_semana;
   std::map<Oferta*, int> quantidade;
   int creditos_teoricos;
   int creditos_praticos;
	
   // disciplina original, que foi substituida em um curriculo.
   // Se for NULL é porque não houve substituição, e "disciplina" já é a original.
   Disciplina * disciplinaSubstituida;

   // Indica se uma aula é virtual ou não.
   bool aula_virtual;

   // Informa se uma aula possui fixação do tipo
   // professor + dia + horário, indicando que essa
   // aula não poderá ser trocada na matriz de solução
   bool aula_fixada;
};

#endif
