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

//#include "ConjuntoSala.h"

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
   Campus* c;
   TipoSala* t;
   Oferta* o;

   GGroup<BlocoCurricular*> blocos;

   //GGroup<ConjuntoSala*> conjutoSalas;

   // Armazena para cada campus a demanda de cada disciplina.
   std::map<std::pair<int/*disc_id*/,int/*campus_id*/>,int> demandas_campus;
   
   int totalSalas;
   int totalTurmas;
   int totalConjuntosSalas;

   // std::map<std::pair<int/*cursoA_id*/,int/*cursoB_id*/>,bool/*sao_compat?*/> compat_cursos;
   std::map<std::pair<Curso*,Curso*>,bool> compat_cursos;

   // >>> 14/10/2010
   //GGroup<std::map<int/*novo_id_Disc*/,std::pair<int/*id_Disc*/,int/*id_Curriculo*/> > > relacao_disc_curriculo;
   //std::map<int/*novo_id_Disc*/,std::pair<int/*id_Disc*/,int/*id_Curriculo*/> > relacao_disc_curriculo;
   // <<< 14/10/2010

   // >>> 15/10/2010
   //typedef std::map<int/*novo_id_Disc*/,std::pair<int/*id_Disc*/,int/*id_Curriculo*/> > relacao_disc_curriculo;

   // Admitindo que o id de um curso seja único, construiremos um map de relacoes_disc_curriculo para cada curso
   //std::map<int/*id do curso*/,relacao_disc_curriculo> relacao_curso_discs_curric;

   // <<< 15/10/2010

//private:
   // >>> Variáveis e/ou estruturas de dados para realizar o pré processamento dos dados.
   
   std::map<int/*Id Campus*/,unsigned/*Tamanho médio das salas*/> cp_medSalas;

   // Armazena todas as disciplinas ofertadas em um campus.
   std::map<int/*Id Campus*/,GGroup<int>/*Id Discs*/> cp_discs;

   //------------------
   // Essa estrutura ficaria melhor dentro das disciplinas !!!!
   /* 
   Armazena todas as salas (sala de aula ou lab) em que uma 
   disciplina pode ser oferecida.
   */
   //std::map<int/*Id Disc*/,GGroup<Sala*> > discSalas;
   std::map<int/*Id Disc*/,std::vector<Sala*> > discSalas;

   /* Armazena todas as salas (sala de aula ou lab) em que uma 
   disciplina, preferencialmente, deve ser oferecida. */
   std::map<int/*Id Disc*/,GGroup<Sala*> > disc_Salas_Pref;

   //------------------

   /* Relaciona as turmas com alguma disciplina. Armazena tb o tamanho das turmas da
   disc em questão. A princípio, todas as turmas vão ter o msm tamanho.
   Lembrando que turmas são representadas por inteiros, definidos em estima_turmas.
   Por equanto, os ids das turmas não estão sendo utilizados, ou seja, podem existir
   duas turmas diferentes com o msm id. Isto não é considerado um erro pq, do modo que
   está implementado, qdo isso acontecer, as turmas serão de discs diferentes. */
   //std::map<int/*Id Disciplina*/,GGroup<std::pair<int/*Id Turma*/,int/*Tamanho Turma*/> > > disc_turmas;

   /* Estrutura responsavel por referenciar os campus.
   Nao precisaria dessa estrutura se o FIND do GGroup estivesse funcionando normalmente.
   VER ISSO DEPOIS */
   std::map<int/*Id Campus*/,Campus*> refCampus;

   /* Estrutura responsavel por referenciar as unidades.
   Nao precisaria dessa estrutura se o FIND do GGroup estivesse funcionando normalmente.
   VER ISSO DEPOIS */
   std::map<int/*Id Unidade*/,Unidade*> refUnidade;

   /* Estrutura responsavel por referenciar as salas.
   Nao precisaria dessa estrutura se o FIND do GGroup estivesse funcionando normalmente.
   VER ISSO DEPOIS */
   std::map<int/*Id Sala*/,Sala*> refSala;

   /* Estrutura responsavel por referenciar as disciplinas.
   Nao precisaria dessa estrutura se o FIND do GGroup estivesse funcionando normalmente.
   VER ISSO DEPOIS */
   std::map<int/*Id Disc*/,Disciplina*> refDisciplinas;

   /* Estrutura responsavel por referenciar as disciplinas.
   Nao precisaria dessa estrutura se o FIND do GGroup estivesse funcionando normalmente.
   VER ISSO DEPOIS */
   std::map<int/*Id Oferta*/,Oferta*> refOfertas;

   /* Listando todas as ofertas que contem uma disciplina especificada. */
   //Disciplina * discPresenteOferta(Disciplina&,Oferta&);
   std::map<int/*Id disc*/, GGroup<Oferta*> > ofertasDisc;

   /* Listando os dias letivos de uma disciplina em relação a cada sala. */
   std::map<std::pair<int/*idDisc*/,int/*idSala*/>, GGroup<int>/*Dias*/ > disc_Salas_Dias;

   /* Listando os dias letivos de uma disciplina em relação a um conjunto de salas de mesmo tipo. */
   std::map<std::pair<int/*idDisc*/,int/*idSubCjtSala*/>, GGroup<int>/*Dias*/ > disc_Conjutno_Salas__Dias;

   /* Listando os dias letivos comuns entre um bloco curricular e um campus.

      Quando tiver mais de um campus, pode acontecer que uma associação entre um bloco curricular 
      que não pertence a um determinado campus seja criada. Arrumar isso depois.
   */
   std::map<std::pair<int/*idBloco*/,int/*idCampus*/>, GGroup<int>/*Dias*/ > bloco_Campus_Dias;

   /* Listando as regras de créditos para cada possível valor de crédito. */
   std::map<int/*Num. Creds*/,GGroup<DivisaoCreditos*> > creds_Regras;

    /* Dias letivos comuns de um professor e uma disciplina. */
   std::map<std::pair<int/*idProf*/,int/*idDisc*/>, GGroup<int>/*Dias*/ > prof_Disc_Dias;

   // <<<

public:
   // =========== METODOS SET

   // =========== METODOS GET

   // ToDo : implementar os métodos básicos para todos os membros da classe.

   // <<<

public:
   virtual void le_arvore(TriedaInput& raiz);
};

#endif
