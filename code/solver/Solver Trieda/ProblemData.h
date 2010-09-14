#ifndef PROBLEMDATA_H
#define PROBLEMDATA_H

#include "GGroup.h"
#include "Calendario.h"
#include "TipoSala.h"
#include "TipoContrato.h"
#include "TipoTitulacao.h"
#include "AreaTitulacao.h"
#include "TipoDisciplina.h"
#include "NivelDificuldade.h"
#include "TipoCurso.h"
#include "DivisaoCreditos.h"
#include "Campus.h"
#include "Deslocamento.h"
#include "Disciplina.h"
#include "Curso.h"
#include "Oferta.h"
#include "Demanda.h"
#include "Fixacao.h"
#include <iostream>

//Stores input data
class ProblemData : public OFBase
{
public:
   //Constructor for initial state
   ProblemData();

   //Destructor
   ~ProblemData();

   //==================================================
   // SET METHODS 
   //==================================================

   /**
   ToDo:
   All set methods of the private attributes should be defined here
   */

   //==================================================
   // GET METHODS 
   //==================================================
   
   /**
   ToDo:
   All get methods of the private attributes should be defined here
   */

//private:

   /**
   ToDo:
   All objects that define the problem input should be declared here
   **/
   Calendario* calendario;
   GGroup<TipoSala*> tipos_sala;
   GGroup<TipoContrato*> tipos_contrato;
   GGroup<TipoTitulacao*> tipos_titulacao;
   GGroup<AreaTitulacao*> areas_titulacao;
   GGroup<TipoDisciplina*> tipos_disciplina;
   GGroup<NivelDificuldade*> niveis_dificuldade;
   GGroup<TipoCurso*> tipos_curso;
   GGroup<DivisaoCreditos*> regras_div;
   GGroup<Campus*> campi;
   GGroup<Deslocamento*> tempo_campi;
   GGroup<Deslocamento*> tempo_unidades;
   GGroup<Disciplina*> disciplinas;
   GGroup<Curso*> cursos;
   GGroup<Oferta*> ofertas;
   GGroup<Fixacao*> fixacoes;

public:
   virtual void le_arvore(TriedaInput& raiz);
};

#endif
