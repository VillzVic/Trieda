#pragma once
#include "ofbase.h"
#include "TipoDisciplina.h"
#include "DivisaoCreditos.h"
#include "HorarioDisponivel.h"
#include "Sala.h"

class Turma;
#include "Turma.h"


class Disciplina :
   public OFBase
{
public:
   Disciplina(void);
   ~Disciplina(void);
//private:
   int id;
   std::string codigo;
   std::string nome;
   int cred_teoricos;
   int cred_praticos;
   bool e_lab;
   TipoDisciplina* tipo;
   DivisaoCreditos* divisao_creditos;
   GGroup<Disciplina*> compatibilidades; /* tem isso mesmo? */
   GGroup<Disciplina*> continencias;
   GGroup<HorarioDisponivel*> horarios;
   GGroup<Sala*> salas;
   GGroup<Turma*> turmas;
public:
   virtual void le_arvore(ItemDisciplina& elem);
};
 