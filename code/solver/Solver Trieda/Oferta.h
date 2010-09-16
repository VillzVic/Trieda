#pragma once
#include "ofbase.h"
#include "Curriculo.h"
#include "Curso.h"
#include "Turno.h"
#include "Campus.h"

class Oferta :
   public OFBase
{
public:
   Oferta(void);
   ~Oferta(void);

   int curriculo_id;
   int curso_id;
   int turno_id;
   int campus_id;
   Curriculo* curriculo;
   Curso* curso;
   Turno* turno;
   Campus* campus;

   void le_arvore(ItemOfertaCurso& elem);
};
