#ifndef _PROBLEM_DATA_H_
#define _PROBLEM_DATA_H_

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
#include "AtendimentoCampusSolucao.h"
#include "AtendimentoUnidadeSolucao.h"
#include "Aula.h"

#include <iostream>
#include <vector>

// Stores input data
class ProblemData : public OFBase
{
public:
   // Constructor for initial state
   ProblemData();

   // Destructor
   ~ProblemData();

   Calendario * calendario;
   GGroup< TipoSala * > tipos_sala;
   GGroup< TipoContrato * > tipos_contrato;
   GGroup< TipoTitulacao * > tipos_titulacao;
   GGroup< AreaTitulacao * > areas_titulacao;
   GGroup< TipoDisciplina * > tipos_disciplina;
   GGroup< NivelDificuldade * > niveis_dificuldade;
   GGroup< TipoCurso * > tipos_curso;
   GGroup< DivisaoCreditos * > regras_div;
   GGroup< Campus * > campi;
   GGroup< Deslocamento * > tempo_campi;
   GGroup< Deslocamento * > tempo_unidades;
   GGroup< Disciplina * > disciplinas;
   GGroup< Curso * > cursos;
   GGroup< Demanda * > demandas;
   GGroup< Oferta * > ofertas;
   ParametrosPlanejamento * parametros;
   GGroup< Fixacao * > fixacoes;

   vector< HorarioAula * > horarios_aula_ordenados;

   GGroup< AtendimentoCampusSolucao * > * atendimentosTatico;
   //--------------------

   GGroup< BlocoCurricular * > blocos;

   // Armazena para cada campus a demanda de cada disciplina.
   std::map< std::pair< int /*disc_id*/, int /*campus_id*/ >, int > demandas_campus;

   // Dado um par 'Campus' e 'Curso', obtemos
   // todas as 'Demandas' relacionadas a esse par
   std::map< std::pair< Campus *, Curso * >,
	   GGroup< Demanda *, LessPtr< Demanda > > > map_campus_curso_demanda;

   int totalSalas;
   int totalTurmas;
   int totalConjuntosSalas;

   // Informa o maior valor de horários
   // disponíveis entre os professores
   int max_horarios_professor;

   std::map< std::pair< Curso *, Curso * >, bool > compat_cursos;

   // Dado um curso e uma disciplina, retorna o bloco curricular correspondente
   std::map< std::pair< Curso *, Disciplina * > , BlocoCurricular * > mapCursoDisciplina_BlocoCurricular;

   // >>> Variáveis e/ou estruturas de dados para realizar o pré processamento dos dados.

   std::map< int /*Id Campus*/, unsigned /*Tamanho médio das salas*/ > cp_medSalas;

   // Armazena todas as disciplinas ofertadas em um campus.
   std::map< int /*Id Campus*/, GGroup< int > /*Id Discs*/ > cp_discs;

   //------------------
   // Essa estrutura ficaria melhor dentro das disciplinas !!!!
   // Armazena todas as salas (sala de aula ou lab) em que uma 
   // disciplina pode ser oferecida.   
   std::map< int /*Id Disc*/, std::vector< Sala * > > discSalas;

   //------------------
   /* Armazena todas as salas (sala de aula ou lab) em que uma 
   disciplina, preferencialmente, deve ser oferecida. */
   std::map<int/*Id Disc*/,GGroup<Sala*> > disc_Salas_Pref;

   //------------------
   // Relaciona as turmas com alguma disciplina. Armazena tb o tamanho das turmas da
   // disc em questão. A princípio, todas as turmas vão ter o msm tamanho.
   // Lembrando que turmas são representadas por inteiros, definidos em estima_turmas.
   // Por equanto, os ids das turmas não estão sendo utilizados, ou seja, podem existir
   // duas turmas diferentes com o msm id. Isto não é considerado um erro pq, do modo que
   // está implementado, qdo isso acontecer, as turmas serão de discs diferentes.
   //std::map<int/*Id Disciplina*/,GGroup<std::pair<int/*Id Turma*/,int/*Tamanho Turma*/> > > disc_turmas;

   //------------------
   // Estrutura responsavel por referenciar os campus.
   // Nao precisaria dessa estrutura se o FIND do GGroup
   // estivesse funcionando normalmente. VER ISSO DEPOIS
   std::map< int /*Id Campus*/, Campus * > refCampus;

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
   std::map< int /*Id Oferta*/, Oferta * > refOfertas;

   /* Listando todas as ofertas que contem uma disciplina especificada. */
   //Disciplina * discPresenteOferta(Disciplina&,Oferta&);
   std::map< int /*Id disc*/, GGroup< Oferta * > > ofertasDisc;

   // Listando os dias letivos de uma disciplina em relação a cada sala.
   std::map<std::pair< int /*idDisc*/, int /*idSala*/ >, GGroup< int > /*Dias*/ > disc_Salas_Dias;

   // Listando os dias letivos de uma disciplina em relação a um conjunto de salas de mesmo tipo.
   std::map<std::pair<int/*idDisc*/,int/*idSubCjtSala*/>, GGroup<int>/*Dias*/ > disc_Conjutno_Salas__Dias;

   // Listando os dias letivos comuns entre um bloco curricular
   // e um campus. Obs.: Quando tiver mais de um campus, pode
   // acontecer que uma associação entre um bloco curricular
   // que não pertence a um determinado campus seja criada. "Arrumar isso depois".
   std::map< std::pair< int /*idBloco*/, int /*idCampus*/ >, GGroup< int > /*Dias*/ > bloco_Campus_Dias;

   // Listando as regras de créditos para cada possível valor de crédito.
   std::map< int /*Num. Creds*/, GGroup< DivisaoCreditos * > > creds_Regras;

    // Dias letivos comuns de um professor e uma disciplina.
   std::map< std::pair< int /*idProf*/, int /*idDisc*/>, GGroup< int > /*Dias*/ > prof_Disc_Dias;

   // Lista, para cada professor, todas as disciplinas as quais ele é fixado.
   //std::map<Professor*,GGroup<Disciplina*> > prof_Fix_Disc;

   GGroup< Aula * > aulas;

   // Lista para cada par <professor,disciplina> todas as fixacoes existentes.
   std::map< std::pair< Professor *, Disciplina * >, GGroup< Fixacao * > > fixacoesProfDisc;

   /* Estrutura que agrupa as aulas por bloco curricular. */
   std::map<BlocoCurricular*,GGroup<Aula*,LessPtr<Aula> > > blocoCurricularAulas;

   /* Estrutura que informa a quais blocos curriculares uma aula pertence. */
   std::map<Aula *, GGroup<BlocoCurricular*,LessPtr<BlocoCurricular> > > aulaBlocosCurriculares;

   virtual void le_arvore(TriedaInput& raiz);
};

#endif // _PROBLEM_DATA_H_
