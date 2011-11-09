#ifndef _TURNO_H_
#define _TURNO_H_

#include "ofbase.h"
#include "GGroup.h"
#include "HorarioAula.h"

class Turno :
   public OFBase
{
public:
   Turno( void );
   virtual ~Turno( void );

   virtual void le_arvore( ItemTurno & );

   GGroup< HorarioAula *, LessPtr< HorarioAula > > horarios_aula;

   void setNome( std::string s ) { nome = s; }   
   std::string getNome() const { return nome; }

private:
   std::string nome;
};

#endif
