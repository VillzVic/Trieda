#ifndef _PROBLEM_DATA_H_
#define _PROBLEM_DATA_H_

#include <iostream>
#include <vector>
#include <list>
#include <map>

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
#include "HorarioDia.h"

class Variable;

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
   GGroup< Demanda *, LessPtr< Demanda > > demandas;
   GGroup< Oferta *, LessPtr< Oferta > > ofertas;
   ParametrosPlanejamento * parametros;
   GGroup< Fixacao *, LessPtr< Fixacao > > fixacoes;

   GGroup< HorarioDia *, LessPtr< HorarioDia > > horariosDia;
   std::vector< HorarioDia * > horariosDiaIdx;
   int maxHorariosDif;

   int getHorarioDiaIdx( HorarioDia * );
   int getHorarioDiaIdx( int, int );

   bool aulaAtendeCurso( Aula *, Curso * );

   // Calcula o tempo NECESS�RIO para
   // se deslocar entre uma aula e outra
   int calculaTempoEntreCampusUnidades(
      Campus *, Campus *, Unidade *, Unidade * );

   int minutosIntervalo( DateTime dt1, DateTime dt2 );

   std::vector< HorarioAula * > horarios_aula_ordenados;
   GGroup< AtendimentoCampusSolucao * > * atendimentosTatico;

   // Para cada dia da semana, informa o
   // conjunto de hor�rios aula dispon�veis nesse dia
   std::map< int, GGroup< HorarioAula *, LessPtr< HorarioAula > > > horarios_aula_dia_semana;

   // Conjunto de professores virtuais alocados na solu��o operacional
   std::vector< Professor * > professores_virtuais;

   GGroup< BlocoCurricular *, LessPtr< BlocoCurricular > > blocos;

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

   // Informa o maior valor de hor�rios
   // dispon�veis entre os professores
   int max_horarios_professor;

   // map de compatibilidade e incompatibilidade entre 2 turmas.
   std::map< std::pair< Curso *, Curso * >, bool > compat_cursos;

   // Dado um curso e uma disciplina, retorna o bloco curricular correspondente
   std::map< std::pair< Curso *, Disciplina * > , BlocoCurricular * > mapCursoDisciplina_BlocoCurricular;

   std::map< int /*Id Campus*/, unsigned /*Tamanho m�dio das salas*/ > cp_medSalas;

   // Armazena todas as disciplinas ofertadas em um campus.
   std::map< int /*Id Campus*/, GGroup< int > /*Id Discs*/ > cp_discs;

   //------------------
   // Essa estrutura ficaria melhor dentro das disciplinas !!!!
   // Armazena todas as salas (sala de aula ou lab) em que uma 
   // disciplina pode ser oferecida.
   std::map< Disciplina *, std::vector< Sala * >, LessPtr< Disciplina > > discSalas;

   //------------------
   // Armazena todas as salas (sala de aula ou lab) em que uma 
   // disciplina, preferencialmente, deve ser oferecida.
   std::map< Disciplina *, GGroup< Sala *, LessPtr< Sala > >, LessPtr< Disciplina > > disc_Salas_Pref;

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
   std::map< int, GGroup< Oferta *, LessPtr< Oferta > > > ofertasDisc;

   // =============================================================================================
   // Estruturas conflitantes !!!

   // A segunda estrutura engloba a primeira. Portanto, basta
   // substituir as ocorr�ncias da primeira estrutura pela segunda.
   // Os hor�rios somente ser�o utilizados no m�dulo operacional.

   // Lista os dias letivos de uma disciplina em rela��o a cada sala.
   std::map< std::pair< int /*idDisc*/, int /*idSala*/ >, GGroup< int > /*Dias*/ > disc_Salas_Dias;

   // Lista todos os hor�rios, para cada dia letivo, comuns entre uma disciplina e uma sala.
   std::map<std::pair< int /*idDisc*/, int /*idSala*/ >, 
	   std::map< int /*Dias*/, GGroup< HorarioAula *, LessPtr< HorarioAula > > > > disc_Salas_Dias_HorariosAula;
   // =============================================================================================

   // Listando os dias letivos de uma disciplina em rela��o a um conjunto de salas de mesmo tipo.
   std::map< std::pair< int /*idDisc*/, int /*idSubCjtSala*/>, GGroup< int > /*Dias*/ > disc_Conjutno_Salas__Dias;

   // Listando os dias letivos comuns entre um bloco curricular
   // e um campus. Obs.: Quando tiver mais de um campus, pode
   // acontecer que uma associa��o entre um bloco curricular
   // que n�o pertence a um determinado campus seja criada. "Arrumar isso depois".
   std::map< std::pair< int /*idBloco*/, int /*idCampus*/ >, GGroup< int > /*Dias*/ > bloco_Campus_Dias;

   // Listando as regras de cr�ditos para cada poss�vel valor de cr�dito.
   std::map< int /*Num. Creds*/, GGroup< DivisaoCreditos * > > creds_Regras;

   // Dias letivos comuns de um professor e uma disciplina.
   std::map< std::pair< int /*idProf*/, int /*idDisc*/>, GGroup< int > /*Dias*/ > prof_Disc_Dias;

   // Filtro de dias letivos para o tatico usando os professores.
   std::map< int, GGroup< int > > disc_Dias_Prof_Tatico;

   bool usarProfDispDiscTatico;

   GGroup< Aula *, LessPtr< Aula > > aulas;

   // Estrutura que agrupa as aulas por bloco curricular e dia.
   std::map< BlocoCurricular *,
             std::map< int /*dia*/, GGroup< Aula *, LessPtr< Aula > > >,
             LessPtr< BlocoCurricular > > blocoCurricularDiaAulas;

   // Estrutura que informa a quais blocos curriculares uma aula pertence.
   std::map< Aula *, GGroup< BlocoCurricular *, LessPtr< BlocoCurricular > >,
             LessPtr< Aula > > aulaBlocosCurriculares;

   // Armazena todos os objetos turnos, de todos os campus
   GGroup< Turno *, LessPtr< Turno > > todos_turnos;

   //----------------------------------------------------------------
   // FIXA��ES
   // Fixa��es - T�tico:
   // Disciplina, Sala
   GGroup< Fixacao *, LessPtr< Fixacao > > fixacoes_Disc_Sala;

   // Disciplina, Sala, Dia, Hor�rio
   GGroup< Fixacao *, LessPtr< Fixacao > > fixacoes_Disc_Sala_Dia_Horario;

   // Disciplina, Dia, Hor�rio
   GGroup< Fixacao *, LessPtr< Fixacao > > fixacoes_Disc_Dia_Horario;
   //----------------------------------------------------------------

   //----------------------------------------------------------------
   // Fixa��es - Operacional:
   // Professor, Disciplina, Sala, Dia, Hor�rio
   GGroup< Fixacao *, LessPtr< Fixacao > > fixacoes_Prof_Disc_Sala_Dia_Horario;

   // Professor, Disciplina, Dia, Hor�rio
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
   // cr�tidos est�o fixados dessa disciplina nesse dia da semana
   std::map< std::pair< Disciplina *, int >, int > map_Discicplina_DiaSemana_CreditosFixados;

   // Dada uma disciplina, informa a sala � qual essa disciplina
   // est� fixada, caso haja a fixa��o disciplina/sala
   std::map< Disciplina *, Sala * > map_Discicplina_Sala_Fixados;

   int creditosFixadosDisciplinaDia( Disciplina *, int, ConjuntoSala * );

   virtual void le_arvore( TriedaInput & );

   //-----------------------------------------------------------------------------------------------
   // Equival�ncias entre discilpinas

   // Armazena od 'ids' das disciplinas compat�veis entre os pares de cursos
   std::map< std::pair< Curso *, Curso * >, std::vector< int /*idDisc*/ > > cursosComp_disc;

   // Dado um curso e um curriculo, retorna-se um map
   // que referencia cada disciplina com sua correspondente
   // disciplina substituta, caso tenha ocorrido uma
   // substitui��o por equival�ncia entre essas disciplinas
   std::map< std::pair< Curso *, Curriculo * >,
			    std::map< Disciplina *, Disciplina * > > map_CursoCurriculo_DiscSubst;

   // Dado um curso e um curriculo, retorna-se um map
   // que referencia, para cada disciplina, as disciplinas que ela substituiu,
   // caso tenha ocorrido uma substitui��o por equival�ncia entre essas disciplinas
   std::map< std::pair< Curso *, Curriculo * >,
			    std::map< Disciplina *, GGroup< Disciplina *, LessPtr< Disciplina > > > > mapGroupDisciplinasSubstituidas;

   // Dada uma disciplina, informamos o seu curso e curriculo
   std::map< Disciplina *, std::pair< Curso *, Curriculo * > > map_Disc_CursoCurriculo;

   // Dada uma disciplina, esse m�todo retorna qual
   // disciplina a substituiu, ou retorna NULL caso
   // a disciplina n�o tenha sido substitu�da
   Disciplina * retornaDisciplinaSubstituta( Curso *, Curriculo *, Disciplina * );

   bool cursosCompativeis( Curso *, Curso * );

   // Armazena as demandas criadas para as disciplinas substitu�das
   std::map< Disciplina *, Demanda * > demandasDisciplinasSubstituidas;

   // Informa se uma disciplina substitu�da foi atendida
   std::map< Disciplina *, bool > disciplinasSubstituidasAtendidas;

   // Busca a demanda da disciplina informada,
   // em cursos compat�veis com o curso tamb�m informado
   Demanda * buscaDemanda( Curso * , Disciplina * );

   // Retorna o conjunto de demandas da disciplina
   // informada, que foi substitu�da por alguma outra
   GGroup< Demanda *, LessPtr< Demanda > > retornaDemandaDisciplinasSubstituidas( Disciplina * );

   // Retorna os pares curso/curriculo dos quais a disciplina informada possui demanda
   GGroup< std::pair< Curso *, Curriculo * > > retornaCursosCurriculosDisciplina( Disciplina * );

	// Dada uma disciplina, e seu par curso/curriculo, retorna-se a oferta dessa disciplina
	Oferta * retornaOfertaDiscilpina( Curso *, Curriculo *, Disciplina * );
   //-----------------------------------------------------------------------------------------------

   // Estrutura utilizada para referenciar as novas disciplinas
   // criadas ap�s a execu��o do m�todo de divis�o de disciplinas.
   GGroup< Disciplina *, LessPtr< Disciplina > > novasDisciplinas;

   std::map< Professor *, GGroup< std::pair< Aula *, Disciplina * > >, LessPtr< Professor > > mapProfessorDisciplinas;
};

#endif
