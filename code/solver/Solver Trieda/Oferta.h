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

   // Dados pré-processados
   Curriculo* curriculo;
   Curso* curso;
   Turno* turno;
   Campus* campus;

   void le_arvore(ItemOfertaCurso& elem);

   void setCurriculoId(int v) { curriculo_id = v; }
   void setCursoId(int v) { curso_id = v; }
   void setTurnoId(int v) { turno_id = v; }
   void setCampusId(int v) { campus_id = v; }

   int getCurriculoId() { return curriculo_id; }
   int getCursoId() { return curso_id; }
   int getTurnoId() { return turno_id; }
   int getCampusId() { return campus_id; }

private:
   int curriculo_id;
   int curso_id;
   int turno_id;
   int campus_id;
};
