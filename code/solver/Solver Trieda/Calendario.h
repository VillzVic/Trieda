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
   void setTempoAula( double value ) { this->tempo_aula = value; }

   std::string getCodigo() const { return this->codigo; }
   std::string getDescricao() const { return this->descricao; }
   double getTempoAula() const { return this->tempo_aula; }
   int getNroDeHorariosAula(int dia);

   double getTempoTotal( int dia );

   HorarioAula * getProximoHorario( HorarioAula *h );
   
   int retornaNroCreditosEntreHorarios( HorarioAula *hi, HorarioAula *hf );

   bool intervaloEntreHorarios( HorarioAula *hi, HorarioAula *hf );

   bool possuiHorario( HorarioAula *h );

   GGroup<HorarioAula*> retornaHorariosDisponiveisNoDia( int dia );

private:   
   std::string codigo;
   std::string descricao;
   double tempo_aula;
};

#endif