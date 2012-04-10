#ifndef _CALENDARIO_H_
#define _CALENDARIO_H_

#include "ofbase.h"
#include "GGroup.h"
#include "Turno.h"

class Calendario :
   public OFBase
{
public:
   Calendario( void );
   virtual ~Calendario( void );

   GGroup< Turno *, LessPtr< Turno > > turnos;

   virtual void le_arvore( ItemCalendario & );

   void setCodigo( std::string s ) { this->codigo = s; }
   void setDescricao( std::string s ) { this->descricao = s; }
   void setTempoAula( int value ) { this->tempo_aula = value; }

   std::string getCodigo() const { return this->codigo; }
   std::string getDescricao() const { return this->descricao; }
   int getTempoAula() const { return this->tempo_aula; }
   int getNroDeHorariosAula(int dia);

   HorarioAula * getProximoHorario( HorarioAula *h );
   
   int retornaNroCreditosEntreHorarios( HorarioAula *hi, HorarioAula *hf );

   bool possuiHorario( HorarioAula *h );

private:   
   std::string codigo;
   std::string descricao;
   int tempo_aula;
};

#endif