#pragma once
#include "ofbase.h"
#include "TipoDisciplina.h"
#include "DivisaoCreditos.h"
#include "NivelDificuldade.h"
#include "Horario.h"

class Disciplina :
   public OFBase
{
public:
   Disciplina(void);
   ~Disciplina(void);
//private:
   std::string codigo;
   std::string nome;
   int cred_teoricos;
   int cred_praticos;
   bool e_lab;
   int max_alunos_t;
   int max_alunos_p;
   int tipo_disciplina_id;
   int nivel_dificuldade_id;

   DivisaoCreditos* divisao_creditos;
   GGroup<int> equivalentes; 
   GGroup<int> incompativeis;
   GGroup<Horario*> horarios;
   TipoDisciplina* tipo_disciplina;
   NivelDificuldade* nivel_dificuldade;

   int demanda_total;
   int max_demanda;
   int num_turmas;
   int min_creds;
   int max_creds;

public:
   virtual void le_arvore(ItemDisciplina& elem);
};
 