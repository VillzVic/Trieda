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
#include "ParametrosPlanejamento.h"
#include "Fixacao.h"
#include "BlocoCurricular.h"
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
   GGroup<Demanda*> demandas;
   GGroup<Oferta*> ofertas;
   ParametrosPlanejamento* parametros;
   GGroup<Fixacao*> fixacoes;

   GGroup<BlocoCurricular*> blocos;

   std::map<std::pair<int/*disc_id*/,int/*campus_id*/>,int> demandas_campus;
   int total_salas;
   int total_turmas;

   // std::map<std::pair<int/*cursoA_id*/,int/*cursoB_id*/>,bool/*sao_compat?*/> compat_cursos;
   std::map<std::pair<Curso*,Curso*>,bool> compat_cursos;


   // >>> 14/10/2010
   //GGroup<std::map<int/*novo_id_Disc*/,std::pair<int/*id_Disc*/,int/*id_Curriculo*/> > > relacao_disc_curriculo;
   std::map<int/*novo_id_Disc*/,std::pair<int/*id_Disc*/,int/*id_Curriculo*/> > relacao_disc_curriculo;
   // <<< 14/10/2010

public:
   virtual void le_arvore(TriedaInput& raiz);
};

#endif
