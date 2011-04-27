#ifndef _BLOCO_CURRICULAR_H_
#define _BLOCO_CURRICULAR_H_

#include <map>

#include "ofbase.h"
#include "GGroup.h"
#include "Curso.h"
#include "Disciplina.h"
#include "Demanda.h"
#include "Campus.h"
#include "Curriculo.h"

class BlocoCurricular :
   public OFBase
{
public:
   BlocoCurricular(void);
   virtual ~BlocoCurricular(void);

   Curso * curso;
   Campus * campus;
   Curriculo * curriculo;

   GGroup< Disciplina * > disciplinas;
   GGroup< int > diasLetivos;

   // Associa às disciplinas do bloco
   // suas respectivas demandas - IGNORANDO TURNO
   std::map< Disciplina *, Demanda * > disciplina_Demanda;

   void setPeriodo(int value) { this->periodo = value; }
   void setTotalTurmas(int value) { this->total_turmas = value; }

   int getPeriodo() const { return this->periodo; }
   int getTotalTurmas() const { return this->total_turmas; }

private:
   int periodo;
   int total_turmas;
};

#endif