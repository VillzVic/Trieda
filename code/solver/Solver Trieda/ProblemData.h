#ifndef _PROBLEM_DATA_H_
#define _PROBLEM_DATA_H_

#include <unordered_set>
#include <unordered_map>
#include <iostream>
#include <vector>
#include <list>
#include <cmath>
#include <map>

#include "DateTime.h"
#include "combinatoria.h"
#include "Calendario.h"
#include "Aluno.h"
#include "AlunoDemanda.h"
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
#include "Equivalencia.h"

#ifdef BUILD_COM_SOL_INICIAL
#include "SolucaoTaticoInicial.h"
#endif

#define USAR_CALLBACK

#ifndef PRE_TATICO
#define PRE_TATICO
#endif

#define READ_SOLUTION

// ----------------------------------
// NOVA ABORDAGEM

#define TATICO_CJT_ALUNOS

#ifndef TATICO_CJT_ALUNOS
#define SEM_CJT_ALUNOS
#endif
// ----------------------------------

#define TATICO_COM_HORARIOS

#ifdef UNIT
//#define ALUNO_UNICO_CALENDARIO // Usado para ativar ou n�o a restri��o v_{i,d,sl1} + v_{i,d,sl2} <= 1
#endif

#define ALUNO_TURNOS_DA_DEMANDA // Usado quando o aluno s� pode ser alocado em turno existente no calendario da Oferta da Demanda que ele requisitou

#define SALA_UNICA_POR_TURMA

#define EQUIVALENCIA_DESDE_O_INICIO

//#define PREMODELO_AGRUPA_ALUNO_POR_CURSO

#ifndef PREMODELO_AGRUPA_ALUNO_POR_CURSO
	//#define PARTICIONA_PREMODELO
#endif

#define USA_ESTIMA_TURMAS



bool less_than( DateTime left, DateTime right );


#ifndef POSSUI_HORARIO_DIA
#define POSSUI_HORARIO_DIA( ggroup, hd ) \
	for ( GGroup< HorarioDia *, LessPtr< HorarioDia > >::iterator it = ( ggroup ).begin(); it != ( ggroup ).end(); ++it ) \
	{\
		if ( it->igual(hd) ) return true;\
	}\
	return false;
#endif



class Variable;

class ProblemData
	: public OFBase
{
public:
   ProblemData( char input[1024], int inputId );
   virtual ~ProblemData( void );
   
   enum MODO
   {
	   TATICO = 1,
	   OPERACIONAL_COM_TAT = 2,
	   OPERACIONAL_SEM_TAT = 3
   };

   int getModoOtimizacao() const { return modoOtimizacao; }
   void setModoOtimizacao(int modo){ modoOtimizacao = modo; }

   GGroup< TurnoIES *, LessPtr< TurnoIES> > turnosIES;
   GGroup< Calendario *, LessPtr< Calendario > > calendarios;
   GGroup< TipoSala *, LessPtr< TipoSala > > tipos_sala;
   GGroup< TipoContrato *, LessPtr< TipoContrato > > tipos_contrato;
   GGroup< TipoTitulacao *, LessPtr< TipoTitulacao > > tipos_titulacao;
   GGroup< AreaTitulacao *, LessPtr< AreaTitulacao > > areas_titulacao;
   GGroup< TipoDisciplina *, LessPtr< TipoDisciplina > > tipos_disciplina;
   GGroup< NivelDificuldade *, LessPtr< NivelDificuldade > > niveis_dificuldade;
   GGroup< TipoCurso *, LessPtr< TipoCurso > > tipos_curso;
   GGroup< DivisaoCreditos *, LessPtr< DivisaoCreditos > > regras_div;
   GGroup< Campus *, LessPtr< Campus > > campi;
   GGroup< Deslocamento *, LessPtr< Deslocamento > > tempo_campi;
   GGroup< Deslocamento *, LessPtr< Deslocamento > > tempo_unidades;
   GGroup< Disciplina *, LessPtr< Disciplina > > disciplinas;
   GGroup< Curso *, LessPtr< Curso > > cursos;
   GGroup< Demanda *, LessPtr< Demanda > > demandas;
   GGroup< Oferta *, LessPtr< Oferta > > ofertas;
   ParametrosPlanejamento * parametros;
   GGroup< Fixacao *, LessPtr< Fixacao > > fixacoes;
   GGroup< AlunoDemanda *, LessPtr< AlunoDemanda > > alunosDemanda;
   GGroup< Aluno *, LessPtr< Aluno > > alunos;
   GGroup< Professor *, LessPtr< Professor > > profsVirtuais; // 1 por tipo de titula��o
   GGroup< Equivalencia*, LessPtr< Equivalencia > > equivalencias; // s� cont�m as teoricas

   std::map< std::pair< int/*oldDisc*/, int/*newDisc*/ >, int /*equiv*/> mapParDisc_EquivId; // s� cont�m as teoricas

   map< int /* cjtAlunosId */, GGroup< Aluno *, LessPtr< Aluno > > > cjtAlunos;
   map< int /* cjtAlunosId */, GGroup< Demanda *, LessPtr< Demanda > > > cjtDemandas;
   map< int /* cjtAlunosId */, GGroup< AlunoDemanda *, LessPtr< AlunoDemanda > > > cjtAlunoDemanda;
   map< Disciplina *, int /* cjtAlunosId */, LessPtr< Disciplina > > cjtDisciplinas;

   GGroup< Demanda *, LessPtr< Demanda > > demandasTotal;
   GGroup< AlunoDemanda *, LessPtr< AlunoDemanda > > alunosDemandaTotal;
   std::map< int/*campusId*/, int > nPrioridadesDemanda;

   std::map< int, GGroup<Calendario*,LessPtr<Calendario>> > mapTurnoSemanasLetivas;

   GGroup< HorarioDia *, LessPtr< HorarioDia > > horariosDia;
   std::vector< HorarioDia * > horariosDiaIdx;
   int maxHorariosDif;

   string getDiaEmString( int dia, bool comGenero );
   void defineFasesDosTurnos();

   int getHorarioDiaIdx( HorarioDia * );
   int getHorarioDiaIdx( int, int );
   
   HorarioDia* getHorarioDiaCorrespondente( HorarioAula *ha, int dia );
   HorarioDia* getHorarioDiaCorrespondente( int horarioAulaId, int dia );

   bool aulaAtendeCurso( int turma, Disciplina *disciplina, Campus *campus, Curso * curso );

   GGroup< Curso*, LessPtr<Curso> > cursosAtendidosNaTurma( int turma, Disciplina *disciplina, Campus *campus );
   
   GGroup< Oferta*, LessPtr<Oferta> > ofertasAtendidasNaTurma( int turma, Disciplina *disciplina, Campus *campus );

   // Calcula o tempo NECESS�RIO para
   // se deslocar entre uma aula e outra
   int calculaTempoEntreCampusUnidades( Campus *, Campus *, Unidade *, Unidade * );

   // Informa quantos minutos h� entre os dados DateTime's
   int minutosIntervalo( DateTime dt1, DateTime dt2 );

   // Verifica a ocorr�ncia de �ltima aula do dia ( D ) e primeira aula do dia ( D + 1 )
   bool verificaUltimaPrimeiraAulas( HorarioDia *, HorarioDia * );

   bool verificaPrimeiraAulas( HorarioDia * h );

   bool verificaUltimaAulas( HorarioDia * h );

   std::map< Calendario *, std::vector< HorarioAula * >, LessPtr< Calendario > > mapCalendarioHorariosAulaOrdenados;
   std::vector< HorarioAula * > horarios_aula_ordenados;

   // M�todo que verifica se uma dada disciplina
   // pertence a uma matriz curricular compat�vel
   // com o calend�rio de um determinado hor�rio de aula
   // OBS.: Uma condi��o pressuposta nesse m�todo � a de que
   // nenhuma disciplina aparecer� em dois ou mais curriculos
   // distintos que possuam calend�rios letivos distintos.
   // Ou seja, uma disciplina n�o ser� oferecida com hor�rios
   // de 40 minutos em uma matriz curricular e em hor�rios de
   // 50 minutos em outra matriz curricular.
   bool verificaDisponibilidadeDisciplinaHorario( Disciplina *, HorarioAula * );

   GGroup< AtendimentoCampusSolucao *, LessPtr< AtendimentoCampusSolucao > > * atendimentosTatico;

   // Para cada dia da semana, informa o
   // conjunto de hor�rios aula dispon�veis nesse dia
   std::map< int, GGroup< HorarioAula *, LessPtr< HorarioAula > > > horarios_aula_dia_semana;

   // Conjunto de professores virtuais alocados na solu��o operacional
   std::vector< Professor * > professores_virtuais;
   std::unordered_set<int> profs_virtuais_ids;

   GGroup< BlocoCurricular *, LessPtr< BlocoCurricular > > blocos;

   // Dado o id de uma disciplina e o id de um campus,
   // retorna o total de demandas desse par 'disciplina/campus'
   // Estrutura : id_disc/id_campus --> demanda
   std::map< std::pair< int, int >, int > demandas_campus;

   // Dado um par 'Campus' e 'Curso', obtemos
   // todas as 'Demandas' relacionadas a esse par
   std::map< std::pair< Campus *, Curso * >, GGroup< Demanda *, LessPtr< Demanda > > > map_campus_curso_demanda;
   
   int totalSalas;
   int totalTurmas;
   int totalConjuntosSalas;
   int totalTurmas_AlDem; // total de turmas existentes referentes aos alunos-demanda atuais existentes

   // Informa o maior valor de hor�rios
   // dispon�veis entre os professores
   int max_horarios_professor;

   // map de compatibilidade e incompatibilidade entre 2 cursos.
   std::map< std::pair< Curso *, Curso * >, bool > compat_cursos;

   // Dado um curso e uma disciplina, retorna o bloco curricular correspondente
   std::map< Trio< Curso *, Curriculo*, Disciplina * > , BlocoCurricular * > mapCursoDisciplina_BlocoCurricular;

   std::map< int /*Id Campus*/, unsigned /*Tamanho m�dio das salas*/ > cp_medSalas;

   // Armazena todas as disciplinas ofertadas em um campus.
   std::map< int /*Id Campus*/, GGroup< int > /*Id Discs*/ > cp_discs;

   //------------------
   // Essa estrutura ficaria melhor dentro das disciplinas !!!!
   // Armazena todas as salas ( sala de aula ou lab ) em que uma 
   // disciplina pode ser oferecida.
   std::map< Disciplina *, GGroup< Sala *, LessPtr< Sala > >, LessPtr< Disciplina > > discSalas;

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

   // Estrutura responsavel por referenciar as ofertas.
   // Nao precisaria dessa estrutura se o FIND do GGroup
   // estivesse funcionando normalmente. VER ISSO DEPOIS
   std::map< int, Oferta * > refOfertas;
   
   // Estrutura responsavel por referenciar as unidades.
   // Nao precisaria dessa estrutura se o FIND do GGroup
   // estivesse funcionando normalmente. VER ISSO DEPOIS
   std::map< int/*Id Professor*/, Professor * > refProfessores;

   // ref cursos
   std::unordered_map<int, Curso*> refCursos;

   // Listando todas as ofertas que contem uma disciplina especificada.
   std::map< int, GGroup< Oferta *, LessPtr< Oferta > > > ofertasDisc;
   std::map< int, GGroup< Oferta *, LessPtr< Oferta > > > discOfertasEquiv;   

   std::map< int, HorarioAula * > refHorarioAula;
   std::map< int, TurnoIES * > refTurnos;
   
   TipoTitulacao* retornaTipoTitulacaoMinimo();
   TipoTitulacao* retornaTitulacao( int id );
   TipoTitulacao* retornaTitulacaoPorId( int tit_id );

   TipoContrato* retornaTipoContratoMinimo();
   TipoContrato* retornaTipoContrato( int id );
   TipoContrato* retornaTipoContratoPorId( int contr_id );

   // Dado o id de um aluno, retorna referencia para o aluno correspondente
   Aluno* retornaAluno( int /*alunoId*/ );

   // Dado o id de um AlunoDemanda, retorna referencia para o AlunoDemanda correspondente
   AlunoDemanda* retornaAlunoDemanda( int /*alunoDemId*/ );

   // =============================================================================================
   // Estruturas conflitantes !!!

   // A segunda estrutura engloba a primeira. Portanto, basta
   // substituir as ocorr�ncias da primeira estrutura pela segunda.

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
   std::map< int /*Num. Creds*/, GGroup< DivisaoCreditos *, LessPtr< DivisaoCreditos > > > creds_Regras;

   // Dias letivos comuns de um professor e uma disciplina.
   std::map< std::pair< int /*idProf*/, int /*idDisc*/>, GGroup< int > /*Dias*/ > prof_Disc_Dias;

   // Filtro de dias letivos para o tatico usando os professores.
   std::map< int, GGroup< int > > disc_Dias_Prof_Tatico;
   
   // Mapeia para cada disciplina, os professor reais que possuem habilita��o para ministra-la.
   std::map< Disciplina*, GGroup<Professor*, LessPtr<Professor>>, LessPtr<Disciplina> > mapDiscProfsHabilit;

   // Retorna o n�mero de professores reais que possuem habilita��o para ministrar a disciplina.
   int getNroProfPorDisc( Disciplina *disciplina );

   bool usarProfDispDiscTatico;

   GGroup< Aula *, LessPtr< Aula > > aulas;
   
   // Estrutura que agrupa as aulas por bloco curricular e dia.
   std::map< BlocoCurricular *,
             std::map< int /*dia*/, GGroup< Aula *, LessPtr< Aula > > >,
             LessPtr< BlocoCurricular > > blocoCurricularDiaAulas;

   // Estrutura que informa a quais blocos curriculares uma aula pertence.
   std::map< Aula *, GGroup< BlocoCurricular *, LessPtr< BlocoCurricular > >,
             LessPtr< Aula > > aulaBlocosCurriculares;
   
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
   std::map< Disciplina *, Sala *, LessPtr< Disciplina > > map_Discicplina_Sala_Fixados;

   int creditosFixadosDisciplinaDia( Disciplina *, int, ConjuntoSala * );

   void le_turnosIES( TriedaInput & );
   virtual void le_arvore( TriedaInput & );

   // Armazena os 'ids' das disciplinas em comum entre os pares de cursos compat�veis
   std::map< std::pair< Curso *, Curso * >, std::vector< int /*idDisc*/ > > cursosComp_disc;

   void preencheCursosCompDisc();

   //-----------------------------------------------------------------------------------------------
   // Equival�ncias entre disciplinas

   // Referencia, para cada disciplina substituida, qual a disciplina que a substituiu,
   // caso tenha ocorrido uma substitui��o por equival�ncia entre essas disciplinas
   std::map< Disciplina*, Disciplina*, LessPtr< Disciplina > > mapDiscSubstituidaPor;

   // Dado um curso e um curriculo, retorna-se um map
   // que referencia, para cada disciplina, as disciplinas que ela substituiu,
   // caso tenha ocorrido uma substitui��o por equival�ncia entre essas disciplinas
   std::map< std::pair< Curso *, Curriculo * >,
	         std::map< Disciplina *, GGroup< Disciplina *, LessPtr< Disciplina > >, LessPtr< Disciplina > > > mapGroupDisciplinasSubstituidas;
   
   // Dada uma disciplina, informamos o seu curso e curriculo
   std::map< Disciplina *, std::pair< Curso *, Curriculo * >, LessPtr< Disciplina > > map_Disc_CursoCurriculo;

   // Retorna para uma dada disciplina substituta e um par <curso, curriculo>,
   // a disciplina original que ela substituiu. Caso n�o tiver havido substitui��o,
   // returna NULL.
   Disciplina * ehSubstitutaDe( Disciplina*, std::pair< Curso *, Curriculo * > );

   // Informa se uma dada disciplina � substituta de alguma outra.
   bool ehSubstituta( Disciplina* d );

   // Dada uma disciplina original, um curso e um curriculo,
   // esse m�todo retorna qual disciplina a substituiu,
   // ou retorna NULL caso a disciplina n�o tenha sido substitu�da
   Disciplina * retornaDisciplinaSubstituta( Curso *, Curriculo *, Disciplina * );

   bool cursosCompativeis( Curso *, Curso * );
   
   // Informa se uma disciplina substitu�da foi atendida
   std::map< Disciplina *, bool, LessPtr< Disciplina > > disciplinasSubstituidasAtendidas;

   // Busca a demanda da disciplina informada,
   // em cursos compat�veis com o curso tamb�m informado
   Demanda * buscaDemanda( Curso * , Disciplina * ); // ??

   // Busca a demandas da disciplina e curso informados
   GGroup< Demanda *, LessPtr< Demanda > > buscaTodasDemandas( Curso * , Disciplina *, Campus *cp );

   // Retorna o conjunto de demandas da disciplina
   // informada, que foi substitu�da por alguma outra
   GGroup< Demanda *, LessPtr< Demanda > > retornaDemandaDisciplinasSubstituidas( Disciplina * );

   // Retorna os pares curso/curriculo dos quais a disciplina informada possui demanda
   GGroup< std::pair< Curso *, Curriculo * > > retornaCursosCurriculosDisciplina( Disciplina * );

	// Dada uma disciplina, e seu par curso/curriculo, retorna-se a oferta dessa disciplina
	Oferta * retornaOfertaDiscilpina( Curso *, Curriculo *, Disciplina * );

	Disciplina* retornaDisciplina( int id );

   // Armazena os 'ids' das disciplinas em comum entre os pares de ofertas compat�veis
   std::map< std::pair< Oferta *, Oferta * >, std::vector< int /*idDisc*/ > > oftsComp_disc;

   void preencheOftsCompDisc();

   void insereDisciplinaEmOftsComp( std::pair<Oferta*, Oferta*> po, int idDisc );

   //-----------------------------------------------------------------------------------------------

   // Estrutura utilizada para referenciar as novas disciplinas
   // criadas ap�s a execu��o do m�todo de divis�o de disciplinas.
   GGroup< Disciplina *, LessPtr< Disciplina > > novasDisciplinas;

   std::map< Professor *, GGroup< std::pair< Aula *, Disciplina * > >,
      LessPtr< Professor > > mapProfessorDisciplinas;

   // Para cada demanda, relacionamos os alunos que est�o associados a ela
   std::map< Demanda *, GGroup< AlunoDemanda *, LessPtr< AlunoDemanda > > > mapDemandaAlunos;

   // Dados os ids de uma oferta e uma disciplina,
   // retorna a demanda correspondente, caso exista
   Demanda * buscaDemanda( int, int );

   Oferta * findOferta( int ) const;
   HorarioAula * findHorarioAula( int );

   GGroup< Professor *, LessPtr< Professor > > getProfessores() const;
   Professor * findProfessor( int );
   Professor * findProfessorVirtual( int id );

   std::map< Professor *, GGroup< Disciplina *,
      LessPtr< Disciplina > >, LessPtr< Professor > > mapProfessorDisciplinasAssociadas;

   // Retorna todas as salas (de todos os campi) do input
   GGroup< Sala *, LessPtr< Sala > > getSalas() const;
   Sala * findSala( int );
   
   // Associa a cada horario de aula o conjunto de horarios pertencentes
   // a outras semanas letivas que n�o se sobrep�em
   std::map< HorarioAula*, std::set<HorarioAula*>, LessPtr<HorarioAula> > compatibilidadesDeHorarios;

   GGroup< HorarioAula *, LessPtr< HorarioAula > > retornaHorariosEmComum( int sala, int disc, int dia );

   // Dado o id de uma unidade, retorna referencia para o campus correspondente
   Campus* retornaCampus( int /*unidId*/ );

   // Resultado da aloca��o de alunos no pre-modelo:
   std::map< Aluno*, GGroup< Trio< int /*campusId*/, int /*turma*/, Disciplina* > >, LessPtr< Aluno > > mapAluno_CampusTurmaDisc;
   std::map< Trio< int /*campusId*/, int /*turma*/, Disciplina* >, GGroup< AlunoDemanda*, LessPtr< AlunoDemanda > > > mapCampusTurmaDisc_AlunosDemanda;

   // Folgas de atendimento ao final do t�tico de p1:
   std::map< Aluno*, GGroup< Trio< int /*campusId*/, int /*turma*/, Disciplina* > >, LessPtr< Aluno > > mapSlackAluno_CampusTurmaDisc;
   std::map< Trio< int /*campusId*/, int /*turma*/, Disciplina* >, GGroup< AlunoDemanda*, LessPtr< AlunoDemanda > > > mapSlackCampusTurmaDisc_AlunosDemanda;

   // Associa quais os calendarios podem ser usados na aloca��o da turma. Necess�rio caso haja alunos
   // na turma pertencentes a ofertas de calendarios diferentes
   std::map< Trio< int /*campusId*/, int /*turma*/, int /*discId*/ >, GGroup< Calendario*, LessPtr<Calendario> > > mapTurma_Calendarios;
   std::map< Trio< int /*campusId*/, int /*turma*/, int /*discId*/ >, GGroup< HorarioDia*, LessPtr<HorarioDia> > > mapTurma_HorComunsAosCalendarios;
   
   // Associa quais os turnos podem ser usados na aloca��o da turma. Necess�rio caso haja alunos
   // na turma pertencentes a ofertas de turnos diferentes. Substitui os maps acima, que eram para Calendario.
   std::map< Trio< int /*campusId*/, int /*turma*/, int /*discId*/ >, GGroup< TurnoIES*, LessPtr<TurnoIES> > > mapTurma_TurnosIES;
   std::map< Trio< int /*campusId*/, int /*turma*/, int /*discId*/ >, GGroup< HorarioDia*, LessPtr<HorarioDia> > > mapTurma_HorComunsAosTurnosIES;
   std::map< Trio< int /*campusId*/, int /*turma*/, int /*discId*/ >, 
	   std::map< int, std::map< DateTime, std::map< DateTime, GGroup< HorarioDia*, LessPtr<HorarioDia> > > > > > mapTurmaDiaDtiDtf_HorComuns;

   void clearMaps( int cpId, int prioridade );
   void preencheMapsTurmasCalendarios();
   void preencheMapsTurmasTurnosIES();
   GGroup< HorarioDia*, LessPtr<HorarioDia> > retornaHorariosDiaIntersecao( Trio< int /*campusId*/, int /*turma*/, int /*discId*/ > trio );
   GGroup< HorarioDia*, LessPtr<HorarioDia> > retornaHorariosDiaUniao( Trio< int /*campusId*/, int /*turma*/, int /*discId*/ > trio );

   GGroup< HorarioDia*, LessPtr<HorarioDia> > retornaHorariosDiaTurnoIESIntersecao( Trio< int /*campusId*/, int /*turma*/, int /*discId*/ > trio );
   GGroup< HorarioDia*, LessPtr<HorarioDia> > retornaHorariosDiaTurnoIESUniao( Trio< int /*campusId*/, int /*turma*/, int /*discId*/ > trio );

   void insereAlunoEmTurma( Aluno* aluno, Trio< int /*campusId*/, int /*turma*/, Disciplina*> trio, std::map<int, double> diasNCreds );
   void removeAlunoDeTurma( Aluno* aluno, Trio< int /*campusId*/, int /*turma*/, Disciplina*> trio, std::map<int, double> diasNCreds );
   
   void insereAlunoEmTurma( AlunoDemanda* alunoDemanda, Trio< int /*campusId*/, int /*turma*/, Disciplina*> trio, GGroup<HorarioDia*> horariosDias );
   void removeAlunoDeTurma( AlunoDemanda* alunoDemanda, Trio< int /*campusId*/, int /*turma*/, Disciplina*> trio, GGroup<HorarioDia*> horariosDias );

   void imprimeAlocacaoAlunos( int campusId, int prioridade, int cjtAlunosId, bool heuristica, int r, string MIP, int runtime );
   void imprimeAlocacoesGerais( int campusId, int prioridade, int rodada, string MIP, int totalAtendsSemDivPT_P1, int totalAtendsSemDivPT_P2,
	   int totalCreditos, int totalCreditosPagos, int totalTurmas, int totalCusto, int totalReceita, int runtime );

   GGroup< AlunoDemanda *, LessPtr< AlunoDemanda > > listSlackDemandaAluno;

   // Retorna a turma da disciplina disc na qual o aluno foi alocado.
   // Caso contrario, retorna -1
   int retornaTurmaDiscAluno( Aluno* aluno, Disciplina* disc );
   
   int retornaMaiorIdDemandas();
   int retornaMaiorIdAlunoDemandas();
   AlunoDemanda* procuraAlunoDemanda( int discId, int alunoId, int prioridade );
   AlunoDemanda* procuraAlunoDemOrigEquiv( Disciplina* disc, Aluno *aluno, int prioridade );
   AlunoDemanda* procuraAlunoDemEquiv( Disciplina* disc, Aluno *aluno, int prioridade );
   
   std::string getEquivFileName( int campusId, int prioridade );
   AlunoDemanda* atualizaAlunoDemandaEquiv( int turma, Disciplina* disciplina, int campusId, Aluno *aluno, int prioridade );

   void atualizaDemandas( int novaPrioridade, int campusId );
   
   int retornaCampusId( int demandaId );

   void calculaDemandas();
   
   bool atendeCursoTurmaDisc( int turma, Disciplina* disc, int campusId, int cursoId );

   int atendeTurmaDiscOferta( int turma, int discId, int ofertaId );

   int getNroDiscsAtendidasNoCurso( Curso *curso );

   bool haDemandaDiscNoCampus( int disciplina, int campusId );
   bool haDemandaDiscNoCampus( int disciplina, int campusId, int prioridade );
   GGroup<AlunoDemanda*, LessPtr<AlunoDemanda>> retornaDemandasDiscNoCampus( int disciplinaId, int campusId, int prioridade );

   int existeTurmaDiscCampus( int turma, int discId, int campusId );
   int receitaMediaTurmaDiscCampus( int turma, int discId, int campusId );

   GGroup<Aluno*, LessPtr<Aluno>> alunosEmComum( int turma1, Disciplina* disc1, int turma2, Disciplina* disc2, Campus* campus );

   bool possuiAlunosEmComum( int turma1, Disciplina* disc1, Campus* campus1, int turma2, Disciplina* disc2, Campus* campus2 );
      
   GGroup<int> diasComunsEntreDisciplinas( Disciplina *disciplina1, Disciplina *disciplina2 );

   void criaCjtAlunos( int campusId, int prioridade, bool FIXAR_P1 );

   int retornaCjtAlunosId( int discId );
   int retornaCjtAlunosId( Aluno* aluno );
   int haDemandaDiscNoCjtAlunosPorOferta( int discId, int oftId, int cjtAlunosId );
   bool haDemandaDiscNoCjtAlunosPorOfertaComEquiv( Disciplina *disciplina, int oftId, int cjtAlunosId );
   int maxDemandaDiscNoCjtAlunosPorOfertaComEquiv( Disciplina *disciplina, int oftId, int cjtAlunosId );
   int haDemandaDiscNoCurso( int discId, int cursoId );
   bool haDemandaDiscNoCursoEquiv( Disciplina *disciplina, int cursoId );
   int maxDemandaDiscNoCursoEquiv( Disciplina *disciplina, int cursoId );
   int haDemandaDiscNoCjtAlunosPorCurso( int discId, int cursoId, int cjtAlunosId );
   int getQtdAlunoDemandaAtualPorCampus( int campusId );

   void imprimeCjtAlunos( int campusId );
   
   bool EQUIV_TRANSITIVIDADE;

   bool verificaDisponibilidadeHorario( HorarioAula* horarioAula, int dia, Sala *sala, Professor *prof, Disciplina* disc );

   double cargaHorariaNaoAtendidaPorPrioridade( int prior, int alunoId );
   double cargaHorariaOriginalRequeridaPorPrioridade( int prior, Aluno* aluno );
   double cargaHorariaAtualRequeridaPorPrioridade( int prior, Aluno* aluno );
   double cargaHorariaJaAtendida( Aluno* aluno );

   int retornaNroCreditos( HorarioAula *hi, HorarioAula *hf, Sala *s, Disciplina *d, int dia );

   bool possuiDemandasAlunoEmComum( Disciplina *disciplina1, Disciplina* disciplina2, int campusId, int P_ATUAL );
   bool possuiNaoAtend(Aluno* aluno);

   bool haDemandaPorFormandos( Disciplina *disciplina, Campus *cp, int P_ATUAL );
   bool haDemandaPorFormandosEquiv( Disciplina *disciplina, Campus *cp, int P_ATUAL );
   int getNroDemandaPorFormandos( Disciplina *disciplina, Campus *cp, int P_ATUAL );
   bool possuiAlunoFormando( int turma, Disciplina *disciplina, Campus *cp );
   int getNroFormandos( int turma, Disciplina *disciplina, Campus *cp );
   bool haAlunoFormandoNaoAlocado( int cpId, int prioridadeAtual );
   bool haAlunoFormandoNaoAlocado( Disciplina *disciplina, int cpId, int prioridadeAtual );
   bool haAlunoFormandoNaoAlocadoEquiv( Disciplina *disciplina, int cpId, int prioridadeAtual );
   
   bool temCalouro( int turma, Disciplina* disciplina, int cpId );
   int getNroCalouros( int turma, Disciplina *disciplina, int campusId );

   bool haFolgaDeAtendimento( int prioridade, Disciplina *disciplina, int campusId );
   int getNroFolgasDeAtendimento( int prioridade, Disciplina *disciplina, int campusId );

   Disciplina* getDisciplinaTeorPrat( Disciplina *disciplina );

   void reduzNroTurmasPorDisciplina( int campusId, int prioridade, int iteracao );
   void removeDemandasEmExcesso( int campusId, int prioridade, int grupoAlunosAtualId );

   GGroup<AlunoDemanda*, LessPtr<AlunoDemanda>> retornaMaxDemandasDiscNoCampus_Equiv( Disciplina* disciplina, int campusId, int prioridade );
   GGroup<AlunoDemanda*, LessPtr<AlunoDemanda>> retornaMaxDemandasDiscNoCampus_EquivTotal( Disciplina* disciplina, int campusId, int prioridade, bool permiteRealocacao=false );
   bool haDemandasDiscNoCampus_Equiv( int disciplinaId, int campusId, int prioridade );

   bool violaMinTurma( int campusId );
   bool violaMinTurma( Trio< int, int, Disciplina* > trio );
   void imprimeFormandos();
   void imprimeCombinacaoCredsDisciplinas();

   void setInputId( int id ) { inputId = id; }
   int getInputId() const { return inputId;}
   std::string inputIdToString();
   std::string getInputFileName() const { return inputFileName; }
   std::string getAlocacaoAlunosFileName( int campusId, int prioridade, int cjtAlunosId, bool heuristica, int r, string MIP );
   std::string getAlocacaoTurmasFileName( int campusId, int prioridade, int cjtAlunosId, bool heuristica, int r, string MIP );
   std::string getAlocacoesFileName();
   std::string getDiscNTurmasFileName( int campusId, int prioridade, int cjtAlunosId, bool heuristica, int r, int tatico );
   std::string getUtilizacaoSalasFileName( int campusId, int prioridade, int cjtAlunosId, bool heuristica, int r, string MIP );

   void confereExcessoP2( int campusId );
   void imprimeResumoDemandasPorAluno();
   void imprimeResumoDemandasPorAlunoPosEquiv();

   void preencheMapDisciplinaAlunosDemanda( bool EQUIVALENCIA );

   void adicionaAlunoDemanda( AlunoDemanda *alunoDemanda );
   void removeAlunoDemanda( AlunoDemanda *alunoDemanda );
   void reinsereAlunoDemanda( AlunoDemanda *alunoDemanda, Disciplina *discAntiga );

   GGroup<HorarioDia*, LessPtr<HorarioDia>> retornaHorariosDiaEmComum( GGroup<Calendario*,LessPtr<Calendario>> &calendarios );
   GGroup<HorarioDia*, LessPtr<HorarioDia>> retornaHorariosDiaTurnoComum( GGroup<Calendario*,LessPtr<Calendario>> &calends );
   GGroup<HorarioDia*, LessPtr<HorarioDia>> retornaHorariosDiaComuns( GGroup<TurnoIES*,LessPtr<TurnoIES>> &turnosIES );

   void preencheMapParCalendHorariosDiaComuns();
   int horariosDiaEmComum( Calendario* sl1, Calendario* sl2 );
   GGroup<HorarioDia*, LessPtr<HorarioDia>> getHorariosDiaEmComum( Calendario* sl1, Calendario* sl2 );

   void imprimeAssociacaoDisciplinaSala();
   void constroiHistogramaDiscDemanda();

   double getPercAcumuladoDemanda( Disciplina *disciplina ) { return mapDiscPerc[disciplina]; }

   void preencheMapTurnoSemanasLetivas();
   void preencheMapTurnoDisciplinas();

   int getNumDemandaPorDiscECalendario( int campusId, Disciplina *disciplina, Calendario *calendario );

	// todo: substituir o mapDisciplinaAlunosDemanda
	// map: [campusId][disciplina][prioridade][Calendario][GGroup<AlunoDemanda>]
	std::map< int/*campusId*/, std::map< Disciplina*, std::map< int /*P*/, map<Calendario*, 
		GGroup<AlunoDemanda*,LessPtr<AlunoDemanda>>, LessPtr<Calendario>> >, LessPtr<Disciplina> > > mapDisciplina_Calendarios_AlunosDemanda;
	
	std::map< int/*campusId*/, std::map< Disciplina*, std::map< int /*P*/, map<TurnoIES*, 
		GGroup<AlunoDemanda*,LessPtr<AlunoDemanda>>, LessPtr<TurnoIES>> >, LessPtr<Disciplina> > > mapDisciplina_TurnoIES_AlunosDemanda;


	void setDateTimesSobrepostos();
	GGroup< std::pair<DateTime,DateTime> >* getDateTimesSobrepostos( DateTime dt );

   std::map<DateTime, GGroup< std::pair<DateTime,DateTime> > > mapDtiSobrepoeDtiDtf;

   std::map<int, std::pair<DateTime*,int> > horarioAulaDateTime; 

   GGroup<DateTime*,LessPtr<DateTime> > horariosInicioValidos;
   GGroup<DateTime*,LessPtr<DateTime> > horariosTerminoValidos;
   DateTime* getDateTimeInicial( DateTime dt );
   DateTime* getDateTimeFinal( DateTime dt );
   GGroup<DateTime*, LessPtr<DateTime> > getDtiAnteriores( DateTime dtf );

   void setInicioTarde( DateTime dt ) { inicio_tarde = dt; }
   void setInicioNoite( DateTime dt ) { inicio_noite = dt; }
   DateTime getInicioTarde() const { return inicio_tarde; }
   DateTime getInicioNoite() const { return inicio_noite; }
   void copiaFasesDosTurnosParaSalas();
   void setFaseDoDiaDosHorariosAula();
   void addHorariosAnalogosDisciplina();

   map< int, std::pair<DateTime,DateTime> > fasesDosTurnos;
   enum FasesTurnos { Manha=1, Tarde=2, Noite=3 };		
   
   int getFaseDoDia( DateTime dt );
   DateTime getFimDaFase( int fase ) const;
   int getNroTotalDeFasesDoDia() const { return nroTurnos; }
   GGroup< HorarioDia*, LessPtr<HorarioDia> > getHorariosDiaPorTurno( Disciplina *disciplina, Calendario *calendario, int turno );
   GGroup< HorarioDia*, LessPtr<HorarioDia> > getHorariosDiaPorTurno( Disciplina *disciplina, TurnoIES *turnoIES, int turno );
   bool haTurnoComumViavel( Disciplina *disciplina, Calendario *calendario );
   bool haTurnoComumViavel( Disciplina *disciplina, TurnoIES *turnoIES );
   GGroup<int> getTurnosComunsViaveis( Disciplina *disciplina, Calendario *calendario );
   GGroup<int> getTurnosComunsViaveis( Disciplina *disciplina, TurnoIES *turnoIES );
   bool alocacaoEquivViavel( Demanda *demanda, Disciplina *disciplina );
   GGroup<int> alocacaoEquivViavelTurnos( Demanda *demanda, Disciplina *disciplinaEquiv );
   GGroup<int> alocacaoEquivViavelTurnosIES( Demanda *demanda, Disciplina *disciplinaEquiv );

   int getNroCredsMaxComum( Disciplina* disciplina, GGroup<HorarioDia*, LessPtr<HorarioDia>> horariosDiaComuns );

   // Guarda para cada disciplina o par <difTurmas, nroNaoAtend> aonde:
   // difTurmas = max turmas - nro turmas abertas
   // nroNaoAtend = o n�mero de n�o atendimentos
   std::map<int /*discId*/, std::pair<int/*difTurmas*/, int/*nroNaoAtend*/> > mapDiscDif;
   
   int getNroNaoAtend( Disciplina *disciplina );

   // Utiliza��o das salas em minutos semanais
   map<ConjuntoSala *, int, LessPtr<ConjuntoSala>> mapCreditosSalas;
   void imprimeUtilizacaoSala( int campusId, int prioridade, int cjtAlunosId, bool heuristica, int r, string MIP );
   void imprimeDiscNTurmas( int campusId, int prioridade, int cjtAlunosId, bool heuristica, int r, int tatico );

   Campus* getCampus(int campusId);
   
   Curso* retornaCursoAtendido( int turma, Disciplina* disciplina, int campusId );
   GGroup< Curso*, LessPtr< Curso > > retornaCursosAtendidos( int turma, Disciplina* disciplina, int campusId );
   int getNumTurmasJaAbertas( Disciplina *disciplina, int campusId );
   
   void preencheDiscSubstitutaPossivelPorDemanda();
   bool ehSubstituivel( int disciplina, int disciplinaEquiv, Curso *curso );
   AlunoDemanda* getAlunoDemandaEquiv( Aluno* aluno, Disciplina *disciplina );

   void addMapAula( int campusId, int turma, Disciplina* disciplina, Aula* aula );
   GGroup<Aula*, LessPtr<Aula>> getMapAulas( int campusId, int turma, Disciplina* disciplina );
   
#ifdef BUILD_COM_SOL_INICIAL
   SolucaoTaticoInicial *getSolTaticoInicial() const { return solTaticoInicial; }
   bool existeSolTaticoInicial() const { return solTaticoInicial->existeSolInicial(); }
   AlunoDemanda* atualizaAlunoDemandaEquivSolInicial( int turma, Disciplina* disciplina, int campusId, Aluno *aluno, int prioridade );
#endif

   bool haSolXInitFileName();

   int getTotalOrigAlunoDemanda( int cpId ) { return totalOrigAlunoDemanda[cpId]; }
   void setTotalOrigAlunoDemanda( int cpId, int v ) { totalOrigAlunoDemanda[cpId] = v; }
 
#ifdef BUILD_COM_SOL_INICIAL
   int getPercAtendInicialFixado( int cpId );
   int getPercAtendAlunosInicialFixado( int cpId );
   int getPercAtendTurmasInicialFixado( int cpId );
   void setNumTurmasTotalEstimados( int cpId, int num ) { numTurmasTotalEstimados[cpId] = num; }
   int getNumTurmasTotalEstimados( int cpId ) { return numTurmasTotalEstimados[cpId]; }
   bool passarPorPreModeloETatico( int cpId );

   int estimaNroVarsV( int campusId );
   bool pularParaTaticoIntegrado( int campusId );
#endif
   
   bool temAtendTatInicialFixado( int alDemId );
   bool possibilidCompartDisc( Oferta *oferta, Disciplina *disciplina, int campusId, int prioridade );
   bool possibilidCompartNoPeriodo( Oferta *oferta, int periodo, int campusId, int prioridade );
   
   void confereCorretudeAlocacoes();
   void confereDadosDeEquivalenciaForcada();

   int getCenarioId() const { return cenarioId_; }
   void setCenarioId( int value ) { cenarioId_ = value; }
   
   void calculaCalendarioSomaInterv();
   int getCalendariosMaxSomaInterv( int dia, int fase );
   int getCalendariosMaxSomaInterv( int dia );
   int getCalendariosMaxInterv( int dia, int fase );

   private:
   
		int modoOtimizacao;

	    map< int /*campusId*/, map< int /*turma*/, map< Disciplina*, GGroup<Aula*, LessPtr<Aula>>, LessPtr<Disciplina> > > > mapAulas;
 
		DateTime inicio_tarde;		
		DateTime inicio_noite;		
		int nroTurnos;

	    // histogram structures
	    GGroup< std::pair<double,Disciplina*> > perc_Disc;
		std::map< Disciplina*, double, LessPtr<Disciplina> > mapDiscPerc;

	    std::map< std::pair<Calendario*,Calendario*>, GGroup<HorarioDia*, LessPtr<HorarioDia>> > map_ParCalend_HorariosDiaComuns;

	    // map: [campusId][disciplina][prioridade][GGroup<AlunoDemanda>]
	    std::map< int, std::map< Disciplina*, std::map< int, GGroup<AlunoDemanda*,LessPtr<AlunoDemanda>> >, LessPtr<Disciplina> > > mapDisciplinaAlunosDemanda;

		void insereDisciplinaEmCursosComp( std::pair<Curso*, Curso*> pc, int idDisc );

		std::string inputFileName;
		int inputId;

		std::map< int /*campusId*/, int > totalOrigAlunoDemanda;			// com divis�o pratica/teorica
		std::map< int /*campusId*/, int > numTurmasTotalEstimados;			// com divis�o pratica/teorica

	   std::map< std::pair< Demanda*, Disciplina* >, bool > mapEquivViavelDemandaDisc;

#ifdef BUILD_COM_SOL_INICIAL
	   SolucaoTaticoInicial * solTaticoInicial;
#endif

	   int cenarioId_;

};

#endif
