#ifndef _NIVEL_DIFICULDADE_HORARIO_H_
#define _NIVEL_DIFICULDADE_HORARIO_H_

#include "ofbase.h"
#include "NivelDificuldade.h"

class NivelDificuldadeHorario :
   public OFBase
{
public:
   NivelDificuldadeHorario( void );
   virtual ~NivelDificuldadeHorario( void );

   int nivel_dificuldade_id;
   GGroup< int > horarios_aula; 

   virtual void le_arvore( ItemNivelDificuldadeHorario & );

   NivelDificuldade * nivel_dificuldade;
};
 
#endif
