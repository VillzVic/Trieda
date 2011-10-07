#ifndef _CONVERTE_HORARIOS_CREDITOS_H_
#define _CONVERTE_HORARIOS_CREDITOS_H_

#include "ofbase.h"

class ConverteHorariosCreditos :
   public OFBase
{
public:
   ConverteHorariosCreditos( void );
   virtual ~ConverteHorariosCreditos( void );

   // Métodos get
   int getTurno() const { return this->turno; }
   int getDiaSemana() const { return this->dia_semana; }

   // Métodos set
   void setTurno( int t ) { this->turno = t; }
   void setDiaSemana( int ds ) { this->dia_semana = ds; }

   GGroup< int > horarios;

   virtual bool operator < ( const ConverteHorariosCreditos & right )
   { 
      return ( ( this->turno < right.getTurno() )
         && ( this->dia_semana < right.getDiaSemana() ) );
   }

   virtual bool operator == ( const ConverteHorariosCreditos & right )
   { 
      return ( this->turno == right.getTurno()
         && this->dia_semana == right.getDiaSemana() );
   }

private:
   int turno;
   int dia_semana;
};

#endif
