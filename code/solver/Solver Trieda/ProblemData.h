#ifndef _PROBLEM_DATA_H_
#define _PROBLEM_DATA_H_

#include "input.h"
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

class ProblemData
	: public OFBase
{
public:
   ProblemData();
   virtual ~ProblemData();

   Calendario * calendario;
   GGroup< TipoSala * > tipos_sala;
   GGroup< TipoContrato * > tipos_contrato;
   GGroup< TipoTitulacao * > tipos_titulacao;
   GGroup< AreaTitulacao * > areas_titulacao;
   GGroup< TipoDisciplina * > tipos_disciplina;
   GGroup< NivelDificuldade * > niveis_dificuldade;
   GGroup< TipoCurso * > tipos_curso;
   GGroup< DivisaoCreditos * > regras_div;
   GGroup< Campus *, LessPtr< Campus > > campi;
   GGroup< Deslocamento * > tempo_campi;
   GGroup< Deslocamento * > tempo_unidades;
   GGroup< Disciplina *, LessPtr< Disciplina > > disciplinas;
   GGroup< Curso *, LessPtr< Curso > > cursos;
   GGroup< Demanda * > demandas;
   GGroup< Oferta *, LessPtr< Oferta > > ofertas;
   ParametrosPlanejamento * parametros;
   GGroup< Fixacao *, LessPtr< Fixacao > > fixacoes;

   std::vector< HorarioAula * > horarios_aula_ordenados;

   GGroup< AtendimentoCampusSolucao * > * atendimentosTatico;

   // Para cada dia da semana, informa o
   // conjunto de horários aula disponíveis nesse dia
   std::map< int, GGroup< HorarioAula *, LessPtr< HorarioAula > > > horarios_aula_dia_semana;

   // Conjunto de professores virtuais alocados na solução operacional
   std::vector< Professor * > professores_virtuais;

   GGroup< BlocoCurricular * > blocos;

   // Dado o id de uma disciplina e o id de um campus,
   // retorna o total de demandas desse par 'disciplina/campus'
   // Estrutura : id_disc/id_campus --> demanda
   std::map< std::pair< int, int >, int > demandas_campus;

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

   // map de compatibilidade e incompatibilidade entre 2 turmas.
   std::map< std::pair< Curso *, Curso * >, bool > compat_cursos;

   // Dado um curso e uma disciplina, retorna o bloco curricular correspondente
   std::map< std::pair< Curso *, Disciplina * > , BlocoCurricular * > mapCursoDisciplina_BlocoCurricular;

   std::map< int /*Id Campus*/, unsigned /*Tamanho médio das salas*/ > cp_medSalas;

   // Armazena todas as disciplinas ofertadas em um campus.
   std::map< int /*Id Campus*/, GGroup< int > /*Id Discs*/ > cp_discs;

   //------------------
   // Essa estrutura ficaria melhor dentro das disciplinas !!!!
   // Armazena todas as salas (sala de aula ou lab) em que uma 
   // disciplina pode ser oferecida.   
   std::map< Disciplina *, std::vector< Sala * > > discSalas;

   //------------------
   // Armazena todas as salas (sala de aula ou lab) em que uma 
   // disciplina, preferencialmente, deve ser oferecida.
   std::map< Disciplina *, GGroup< Sala * > > disc_Salas_Pref;

   //------------------
   // Estrutura responsavel por referenciar os campus.
   // Nao precisaria dessa estrutura se o FIND do GGroup
   // estivesse funcionando normalmente. VER ISSO DEPOIS
   std::map< int /*Id Campus*/, Campus * > refCampus;

   // Estrutura responsavel por referenciar as unidades.
   // Nao precisaria dessa estrutura se o FIND do GGroup
   // estivesse funcionando normalmente. VER ISSO DEPOIS
   std::map< int/*Id Unidade*/, Unidade * > refUnidade;

   // Estrutura responsavel por referenciar as salas.
   // Nao precisaria dessa estrutura se o FIND do
   // GGroup estivesse funcionando normalmente. VER ISSO DEPOIS
   std::map< int/*Id Sala*/, Sala * > refSala;

   // Estrutura responsavel por referenciar as disciplinas.
   // Nao precisaria dessa estrutura se o FIND do GGroup
   // estivesse funcionando normalmente. VER ISSO DEPOIS
   std::map< int, Disciplina * > refDisciplinas;

   // Estrutura responsavel por referenciar as disciplinas.
   // Nao precisaria dessa estrutura se o FIND do GGroup
   // estivesse funcionando normalmente. VER ISSO DEPOIS
   std::map< int, Oferta * > refOfertas;

   // Listando todas as ofertas que contem uma disciplina especificada.
   std::map< int, GGroup< Oferta * > > ofertasDisc;

   // =============================================================================================
   // Estruturas conflitantes !!!

   // A segunda estrutura engloba a primeira. Portanto, basta
   // substituir as ocorrências da primeira estrutura pela segunda.
   // Os horários somente serão utilizados no módulo operacional.

   // Lista os dias letivos de uma disciplina em relação a cada sala.
   std::map< std::pair< int /*idDisc*/, int /*idSala*/ >, GGroup< int > /*Dias*/ > disc_Salas_Dias;

   // Lista todos os horários, para cada dia letivo, comuns entre uma disciplina e uma sala.
   std::map<std::pair< int /*idDisc*/, int /*idSala*/ >, 
	   std::map< int /*Dias*/, GGroup< HorarioAula *, LessPtr< HorarioAula > > > > disc_Salas_Dias_HorariosAula;
   // =============================================================================================

   // Listando os dias letivos de uma disciplina em relação a um conjunto de salas de mesmo tipo.
   std::map< std::pair< int /*idDisc*/, int /*idSubCjtSala*/>, GGroup< int > /*Dias*/ > disc_Conjutno_Salas__Dias;

   // Listando os dias letivos comuns entre um bloco curricular
   // e um campus. Obs.: Quando tiver mais de um campus, pode
   // acontecer que uma associação entre um bloco curricular
   // que não pertence a um determinado campus seja criada. "Arrumar isso depois".
   std::map< std::pair< int /*idBloco*/, int /*idCampus*/ >, GGroup< int > /*Dias*/ > bloco_Campus_Dias;

   // Listando as regras de créditos para cada possível valor de crédito.
   std::map< int /*Num. Creds*/, GGroup< DivisaoCreditos * > > creds_Regras;

   // Dias letivos comuns de um professor e uma disciplina.
   std::map< std::pair< int /*idProf*/, int /*idDisc*/>, GGroup< int > /*Dias*/ > prof_Disc_Dias;

   GGroup< Aula *, LessPtr< Aula > > aulas;

   // Estrutura que agrupa as aulas por bloco curricular e dia.
   std::map< BlocoCurricular *,
			 std::map< int /*dia*/, GGroup< Aula *, LessPtr< Aula > > > > blocoCurricularDiaAulas;

   // Estrutura que informa a quais blocos curriculares uma aula pertence.
   std::map< Aula *, GGroup< BlocoCurricular *, LessPtr< BlocoCurricular > > > aulaBlocosCurriculares;

   // Armazena todos os objetos turnos, de todos os campus
   GGroup< Turno *, LessPtr< Turno > > todos_turnos;

   //----------------------------------------------------------------
   // FIXAÇÕES
   // Fixações - Tático:
   // Disciplina, Sala
   GGroup< Fixacao *, LessPtr< Fixacao > > fixacoes_Disc_Sala;

   // Disciplina, Sala, Dia, Horário
   GGroup< Fixacao *, LessPtr< Fixacao > > fixacoes_Disc_Sala_Dia_Horario;

   // Disciplina, Dia, Horário
   GGroup< Fixacao *, LessPtr< Fixacao > > fixacoes_Disc_Dia_Horario;
   //----------------------------------------------------------------

   //----------------------------------------------------------------
   // Fixações - Operacional:
   // Professor, Disciplina, Sala, Dia, Horário
   GGroup< Fixacao *, LessPtr< Fixacao > > fixacoes_Prof_Disc_Sala_Dia_Horario;

   // Professor, Disciplina, Dia, Horário
   GGroup< Fixacao *, LessPtr< Fixacao > > fixacoes_Prof_Disc_Dia_Horario;

   // Professor, Disciplina
   GGroup< Fixacao *, LessPtr< Fixacao > > fixacoes_Prof_Disc;

   // Professor, Disciplina, Sala
   GGroup< Fixacao *, LessPtr< Fixacao > > fixacoes_Prof_Disc_Sala;
   
   // Professor, Sala
   GGroup< Fixacao *, LessPtr< Fixacao > > fixacoes_Prof_Sala;
   //----------------------------------------------------------------

   // Lista para cada par <professor,disciplina> todas as fixacoes existentes.
   std::map< std::pair< Professor *, Disciplina * >, GGroup< Fixacao * > > fixacoesProfDisc;

   // Dado um par disciplina/dia da semana, informa quantos
   // crétidos estão fixados dessa disciplina nesse dia da semana
   std::map< std::pair< Disciplina *, int >, int > map_Discicplina_DiaSemana_CreditosFixados;

   // Dada uma disciplina, informa a sala à qual essa disciplina
   // está fixada, caso haja a fixação disciplina/sala
   std::map< Disciplina *, Sala * > map_Discicplina_Sala_Fixados;

   int creditosFixadosDisciplinaDia( Disciplina *, int, ConjuntoSala * );

   virtual void le_arvore( TriedaInput & );

   //-----------------------------------------------------------------------------------------------
   // Equivalências entre discilpinas

   // Armazena od 'ids' das disciplinas compatíveis entre os pares de cursos
   std::map< std::pair< Curso *, Curso * >, std::vector< int /*idDisc*/ > > cursosComp_disc;

   // Dado um curso e um curriculo, retorna-se um map
   // que referencia cada disciplina com sua correspondente
   // disciplina substituta, caso tenha ocorrido uma
   // substituição por equivalência entre essas disciplinas
   std::map< std::pair< Curso *, Curriculo * >,
			 std::map< Disciplina *, Disciplina * > > map_CursoCurriculo_DiscSubst;

   // Dada uma disciplina, informamos o seu curso e curriculo
   std::map< Disciplina *, std::pair< Curso *, Curriculo * > > map_Disc_CursoCurriculo;

   // Dada uma disciplina, esse método retorna qual
   // disciplina a substituiu, ou retorna NULL caso
   // a disciplina não tenha sido substituída
   Disciplina * retornaDisciplinaSubstituta( Curso *, Curriculo *, Disciplina * );

   std::map< Curso *, GGroup< std::pair< Curriculo *, Oferta * > > > map_Curso_CurriculosOfertas;

   bool cursosCompativeis( Curso *, Curso * );
   //-----------------------------------------------------------------------------------------------
};

#endif
