#ifndef _OFERTA_H_
#define _OFERTA_H_

#include "ofbase.h"
#include "Curriculo.h"
#include "Curso.h"
#include "Turno.h"
#include "Campus.h"

class Oferta :
   public OFBase
{
public:
   Oferta( void );
   virtual ~Oferta( void );

   void le_arvore( ItemOfertaCurso & );

   void setCurriculoId( int v ) { curriculo_id = v; }
   void setCursoId( int v ) { curso_id = v; }
   void setTurnoId( int v ) { turno_id = v; }
   void setCampusId( int v ) { campus_id = v; }
   void setReceita( double v ) { receita = v; }

   int getCurriculoId() { return curriculo_id; }
   int getCursoId() { return curso_id; }
   int getTurnoId() { return turno_id; }
   int getCampusId() { return campus_id; }
   double getReceita() { return receita; }

   // Dados pré-processados
   Curriculo * curriculo;
   Curso * curso;
   Turno * turno;
   Campus * campus;

private:
   int curriculo_id;
   int curso_id;
   int turno_id;
   int campus_id;

   // TRIEDA-958 (Apenas a leitura, falta associar à func. obj.)
   double receita;
};

#endif