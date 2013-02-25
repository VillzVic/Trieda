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
   void setDisciplinaSubstituida( Disciplina *, Oferta* );
   void setHorarioAulaInicial( HorarioAula *h );
   void setHorarioAulaFinal( HorarioAula *h );

   void addHorariosAulaId( int id );
   

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
   GGroup<Disciplina*, LessPtr<Disciplina>> getDisciplinasSubstituidas(Oferta* oft);
   HorarioAula* getHorarioAulaInicial() const;
   HorarioAula* getHorarioAulaFinal() const;
   GGroup<int> getHorariosAulaId() const;

   bool atendeAoCurso( int cursoId );

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

	  if ( hi == NULL && right.getHorarioAulaInicial() != NULL )
		  return true;
	  else if ( hi != NULL && right.getHorarioAulaInicial() == NULL )
		  return false;
	  else if ( hi != NULL && right.getHorarioAulaInicial() != NULL )
	  {
		  if ( *hi < *right.getHorarioAulaInicial() )
			 return true;
		  else if ( *hi > *right.getHorarioAulaInicial() )
			 return false;
	  }

	  if ( hf == NULL && right.getHorarioAulaFinal() != NULL )
		  return true;
	  else if ( hf != NULL && right.getHorarioAulaFinal() == NULL )
		  return false;
	  else if ( hf != NULL && right.getHorarioAulaFinal() != NULL )
	  {
		  if ( *hf < *right.getHorarioAulaFinal() )
			 return true;
		  else if ( *hf > *right.getHorarioAulaFinal() )
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

	  if ( hi != NULL && right.getHorarioAulaInicial() == NULL )
		  return true;
	  else if ( hi == NULL && right.getHorarioAulaInicial() != NULL )
		  return false;
	  else if ( hi != NULL && right.getHorarioAulaInicial() != NULL )
	  {
		  if ( *hi > *right.getHorarioAulaInicial() )
			 return true;
		  else if ( *hi < *right.getHorarioAulaInicial() )
			 return false;
	  }

	  if ( hf != NULL && right.getHorarioAulaFinal() == NULL )
		  return true;
	  else if ( hf == NULL && right.getHorarioAulaFinal() != NULL )
		  return false;
	  else if ( hf != NULL && right.getHorarioAulaFinal() != NULL )
	  {
		  if ( *hf > *right.getHorarioAulaFinal() )
			 return true;
		  else if ( *hf < *right.getHorarioAulaFinal() )
			 return false;
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
		 && ( ( hi!=NULL && right.hi!=NULL && *hi == *right.hi ) ||
			  ( hi==NULL && right.hi==NULL ) )
		 && ( ( hf!=NULL && right.hf!=NULL &&  *hf == *right.hf ) ||
			  ( hf==NULL && right.hf==NULL ) ) );         
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
   HorarioAula *hi;
   HorarioAula *hf;

   // armazena o id de todos os horarios utilizados pela aula (logo, horariosAula entre hi e hf)
   GGroup<int> horariosAulaId;

   // disciplina original, que foi substituida em um curriculo.
   // Se for NULL é porque não houve substituição, e "disciplina" já é a original.
   std::map<Oferta*, GGroup<Disciplina*, LessPtr<Disciplina>>, LessPtr<Oferta>> disciplinaSubstituida;

   // Indica se uma aula é virtual ou não.
   bool aula_virtual;

   // Informa se uma aula possui fixação do tipo
   // professor + dia + horário, indicando que essa
   // aula não poderá ser trocada na matriz de solução
   bool aula_fixada;
};

#endif
