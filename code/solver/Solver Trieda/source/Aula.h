#ifndef _AULA_H_
#define _AULA_H_

#include <vector>

#include "GGroup.h"
#include "Disciplina.h"
#include "Oferta.h"
#include "Sala.h"
#include "Unidade.h"

class Aula
{
public:
   Aula( bool = false );
   virtual ~Aula( void );

   // Ofertas que s�o atendidas por essa aula
   GGroup< Oferta *, LessPtr< Oferta > > ofertas;
   
   void setTurma( int );
   void setDisciplina( Disciplina * );
   void setCampus( Campus * );
   void setSala( Sala * );
   void setUnidade( Unidade * );
   void setDiaSemana( int );
   void setCreditosTeoricos( int );
   void setCreditosPraticos( int );
   void setAulaVirtual( bool );
   void setAulaFixada( bool );
   void setQuantPorOferta( int, Oferta* );
   void addCursoAtendido( Curso*, int );
   void setDisciplinaSubstituida( Disciplina *, Oferta* );
   void setHorarioAulaInicial( HorarioAula *h );
   void setHorarioAulaFinal( HorarioAula *h );
   void setDateTimeInicial( DateTime *dt );
   void setDateTimeFinal( DateTime *dt );
   void addHorariosAulaId( int id );
   void addAlunoDemanda( Oferta* oft, Disciplina* orig, int alDemId );
   void addAlunoDemanda( Oferta* oft, Disciplina* orig, GGroup<int> alsDemId );

   int getTurma() const;
   Disciplina * getDisciplina() const;
   Campus * getCampus() const;
   Sala * getSala() const;
   ConjuntoSala * getCjtSala() const;
   Unidade * getUnidade() const;
   int getDiaSemana() const;
   int getCreditosTeoricos() const;
   int getCreditosPraticos() const;
   int getTotalCreditos() const;
   bool eVirtual() const;
   bool eFixada() const;
   std::map<Curso*, int>* getMapCursosAtendidos();
   std::map<Oferta*,int> getMapQuantPorOft() const;
   int getQuantidadePorOft( Oferta *oft );
   int getQuantidadeAlunos();
   GGroup<Disciplina*, LessPtr<Disciplina>> getDisciplinasSubstituidas(Oferta* oft);
   HorarioAula* getHorarioAulaInicial() const;
   HorarioAula* getHorarioAulaFinal() const;
   DateTime* getDateTimeInicial() const;
   DateTime* getDateTimeFinal() const;
   GGroup<int> getHorariosAulaId() const;
   GGroup<int> getAlunosDemanda( Oferta* oft, Disciplina* orig ); 

   std::map<Oferta*, std::map<Disciplina*, GGroup<int /*alDemOrig*/ >, LessPtr<Disciplina>>, LessPtr<Oferta>> * 
	   getMapOftDiscorigAlsDem() { return &mapOftDiscorigAlsDem; }


   bool atendeAoCurso( int cursoId );


   // ----------------------------------
   // Fixa��es

   void fixaSala( bool f ){ _fixaSala = f; }
   void fixaDia( bool f ){ _fixaDia = f; }
   void fixaCredsTeor( bool f ){ _fixaCredsTeor = f; }
   void fixaCredsPrat( bool f ){ _fixaCredsPrat = f; }
   void fixaHi( bool f ){ _fixaHi = f; }
   void fixaHf( bool f ){ _fixaHf = f; }
   void fixaAbertura( bool f ){ _fixaAbertura = f; }
   
   bool fixaSala(){ return _fixaSala; }
   bool fixaDia(){ return _fixaDia; }
   bool fixaCredsTeor(){ return _fixaCredsTeor; }
   bool fixaCredsPrat(){ return _fixaCredsPrat; }
   bool fixaHi(){ return _fixaHi; }
   bool fixaHf(){ return _fixaHf; }
   bool fixaAbertura(){ return _fixaAbertura; }
   
   // ----------------------------------
   // Operadores

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
      
   // ----------------------------------

   void print();
   string toString();

private:
   int turma;
   Disciplina * disciplina;
   Campus *campus;
   Sala * sala;
   Unidade *unidade;
   int dia_semana;
   std::map<Oferta*, int> quantidade;
   int creditos_teoricos;
   int creditos_praticos;
   HorarioAula *hi;
   HorarioAula *hf;

   DateTime* dti;
   DateTime* dtf;
   
   // Cursos que s�o atendidos por essa aula e a respectiva quantidade de alunos
   std::map<Curso*, int> cursosAtendidos;

   // armazena o id de todos os horarios utilizados pela aula (logo, horariosAula entre hi e hf)
   GGroup<int> horariosAulaId;

   // disciplina original, que foi substituida em um curriculo.
   // Se for NULL � porque n�o houve substitui��o, e "disciplina" j� � a original.
//   std::map<Oferta*, GGroup<Disciplina*, LessPtr<Disciplina>>, LessPtr<Oferta>> disciplinaSubstituida;
     
   std::map<Oferta*, std::map<Disciplina*, GGroup<int /*alDemOrig*/ >, LessPtr<Disciplina>>, LessPtr<Oferta>> mapOftDiscorigAlsDem;

   // Indica se uma aula � virtual ou n�o.
   bool aula_virtual;

   // Informa se uma aula possui fixa��o do tipo
   // professor + dia + hor�rio, indicando que essa
   // aula n�o poder� ser trocada na matriz de solu��o
   bool aula_fixada;


   bool _fixaSala;
   bool _fixaDia;
   bool _fixaCredsTeor;
   bool _fixaCredsPrat;
   bool _fixaHi;
   bool _fixaHf;
   bool _fixaAbertura;
};

#endif
