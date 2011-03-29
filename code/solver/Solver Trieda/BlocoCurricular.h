#pragma once
#include "ofbase.h"

#include "Curso.h"
#include "Disciplina.h"
#include "Demanda.h"
#include "Campus.h"
#include "Curriculo.h"

#include <map>

class BlocoCurricular :
   public OFBase
{
public:
   BlocoCurricular(void);
   ~BlocoCurricular(void);

   Curso* curso;
   Campus* campus;
   Curriculo * curriculo;

   GGroup<Disciplina*> disciplinas;
   GGroup<int> diasLetivos;

   // Associa �s disciplinas do bloco suas respectivas demandas - IGNORANDO TURNO
   std::map<Disciplina*, Demanda*> disciplina_Demanda;

   void setPeriodo(int value) { this->periodo = value; }
   void setTotalTurmas(int value) { this->total_turmas = value; }

   int getPeriodo() const { return this->periodo; }
   int getTotalTurmas() const { return this->total_turmas; }

private:
   int periodo;
   int total_turmas;
};
