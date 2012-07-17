#ifndef _PROBLEM_DATA_H_
#define _PROBLEM_DATA_H_

#include <iostream>
#include <vector>
#include <list>
#include <cmath>
#include <map>

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


class Variable;

class ProblemData
	: public OFBase
{
public:
   ProblemData( void );
   virtual ~ProblemData( void );

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
   GGroup< Professor *, LessPtr< Professor > > profsVirtuais; // 1 por tipo de titulação

   map< int /* cjtAlunosId */, GGroup< Aluno *, LessPtr< Aluno > > > cjtAlunos;
   map< int /* cjtAlunosId */, GGroup< Demanda *, LessPtr< Demanda > > > cjtDemandas;
   map< int /* cjtAlunosId */, GGroup< AlunoDemanda *, LessPtr< AlunoDemanda > > > cjtAlunoDemanda;
   map< Disciplina *, int /* cjtAlunosId */, LessPtr< Disciplina > > cjtDisciplinas;

   GGroup< Demanda *, LessPtr< Demanda > > demandasTotal;
   GGroup< AlunoDemanda *, LessPtr< AlunoDemanda > > alunosDemandaTotal;
   std::map< int/*campusId*/, int > nPrioridadesDemanda;

   GGroup< HorarioDia *, LessPtr< HorarioDia > > horariosDia;
   std::vector< HorarioDia * > horariosDiaIdx;
   int maxHorariosDif;

   int getHorarioDiaIdx( HorarioDia * );
   int getHorarioDiaIdx( int, int );
   
   HorarioDia* getHorarioDiaCorrespondente( HorarioAula *ha, int dia );

   bool aulaAtendeCurso( Aula *, Curso * );

   // Calcula o tempo NECESSÁRIO para
   // se deslocar entre uma aula e outra
   int calculaTempoEntreCampusUnidades( Campus *, Campus *, Unidade *, Unidade * );

   // Informa quantos minutos há entre os dados DateTime's
   int minutosIntervalo( DateTime dt1, DateTime dt2 );

   // Verifica a ocorrência de última aula do dia ( D ) e primeira aula do dia ( D + 1 )
   bool verificaUltimaPrimeiraAulas( HorarioDia *, HorarioDia * );

   bool verificaPrimeiraAulas( HorarioDia * h );

   bool verificaUltimaAulas( HorarioDia * h );

   std::map< Calendario *, std::vector< HorarioAula * >, LessPtr< Calendario > > mapCalendarioHorariosAulaOrdenados;
   std::vector< HorarioAula * > horarios_aula_ordenados;

   // Método que verifica se uma dada disciplina
   // pertence a uma matriz curricular compatível
   // com o calendário de um determinado horário de aula
   // OBS.: Uma condição pressuposta nesse método é a de que
   // nenhuma disciplina aparecerá em dois ou mais curriculos
   // distintos que possuam calendários letivos distintos.
   // Ou seja, uma disciplina não será oferecida com horários
   // de 40 minutos em uma matriz curricular e em horários de
   // 50 minutos em outra matriz curricular.
   bool verificaDisponibilidadeDisciplinaHorario( Disciplina *, HorarioAula * );

   GGroup< AtendimentoCampusSolucao *, LessPtr< AtendimentoCampusSolucao > > * atendimentosTatico;

   // Para cada dia da semana, informa o
   // conjunto de horários aula disponíveis nesse dia
   std::map< int, GGroup< HorarioAula *, LessPtr< HorarioAula > > > horarios_aula_dia_semana;

   // Conjunto de professores virtuais alocados na solução operacional
   std::vector< Professor * > professores_virtuais;

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

   // Informa o maior valor de horários
   // disponíveis entre os professores
   int max_horarios_professor;

   // map de compatibilidade e incompatibilidade entre 2 cursos.
   std::map< std::pair< Curso *, Curso * >, bool > compat_cursos;

   // Dado um curso e uma disciplina, retorna o bloco curricular correspondente
   std::map< Trio< Curso *, Curriculo*, Disciplina * > , BlocoCurricular * > mapCursoDisciplina_BlocoCurricular;

   std::map< int /*Id Campus*/, unsigned /*Tamanho médio das salas*/ > cp_medSalas;

   // Armazena todas as disciplinas ofertadas em um campus.
   std::map< int /*Id Campus*/, GGroup< int > /*Id Discs*/ > cp_discs;

   //------------------
   // Essa estrutura ficaria melhor dentro das disciplinas !!!!
   // Armazena todas as salas ( sala de aula ou lab ) em que uma 
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

   // Estrutura responsavel por referenciar as ofertas.
   // Nao precisaria dessa estrutura se o FIND do GGroup
   // estivesse funcionando normalmente. VER ISSO DEPOIS
   std::map< int, Oferta * > refOfertas;

   // Listando todas as ofertas que contem uma disciplina especificada.
   std::map< int, GGroup< Oferta *, LessPtr< Oferta > > > ofertasDisc;

   std::map< int, HorarioAula * > refHorarioAula;
   std::map< int, Turno * > refTurnos;

   // Dado o id de um aluno, retorna referencia para o aluno correspondente
   Aluno* retornaAluno( int /*alunoId*/ );

   // Dado o id de um AlunoDemanda, retorna referencia para o AlunoDemanda correspondente
   AlunoDemanda* retornaAlunoDemanda( int /*alunoDemId*/ );

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
   std::map< int /*Num. Creds*/, GGroup< DivisaoCreditos *, LessPtr< DivisaoCreditos > > > creds_Regras;

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
   std::map< Disciplina *, Sala *, LessPtr< Disciplina > > map_Discicplina_Sala_Fixados;

   int creditosFixadosDisciplinaDia( Disciplina *, int, ConjuntoSala * );

   virtual void le_arvore( TriedaInput & );

   // Armazena os 'ids' das disciplinas em comum entre os pares de cursos compatíveis
   std::map< std::pair< Curso *, Curso * >, std::vector< int /*idDisc*/ > > cursosComp_disc;

   void preencheCursosCompDisc();

   //-----------------------------------------------------------------------------------------------
   // Equivalências entre disciplinas

   // Referencia, para cada disciplina substituida, qual a disciplina que a substituiu,
   // caso tenha ocorrido uma substituição por equivalência entre essas disciplinas
   std::map< Disciplina*, Disciplina*, LessPtr< Disciplina > > mapDiscSubstituidaPor;

   // Dado um curso e um curriculo, retorna-se um map
   // que referencia, para cada disciplina, as disciplinas que ela substituiu,
   // caso tenha ocorrido uma substituição por equivalência entre essas disciplinas
   std::map< std::pair< Curso *, Curriculo * >,
	         std::map< Disciplina *, GGroup< Disciplina *, LessPtr< Disciplina > >, LessPtr< Disciplina > > > mapGroupDisciplinasSubstituidas;
   
   // Dada uma disciplina, informamos o seu curso e curriculo
   std::map< Disciplina *, std::pair< Curso *, Curriculo * >, LessPtr< Disciplina > > map_Disc_CursoCurriculo;

   // Retorna para uma dada disciplina substituta e um par <curso, curriculo>,
   // a disciplina original que ela substituiu. Caso não tiver havido substituição,
   // returna NULL.
   Disciplina * ehSubstitutaDe( Disciplina*, std::pair< Curso *, Curriculo * > );

   // Informa se uma dada disciplina é substituta de alguma outra.
   bool ehSubstituta( Disciplina* d );

   // Dada uma disciplina original, um curso e um curriculo,
   // esse método retorna qual disciplina a substituiu,
   // ou retorna NULL caso a disciplina não tenha sido substituída
   Disciplina * retornaDisciplinaSubstituta( Curso *, Curriculo *, Disciplina * );

   bool cursosCompativeis( Curso *, Curso * );
   
   // Informa se uma disciplina substituída foi atendida
   std::map< Disciplina *, bool, LessPtr< Disciplina > > disciplinasSubstituidasAtendidas;

   // Busca a demanda da disciplina informada,
   // em cursos compatíveis com o curso também informado
   Demanda * buscaDemanda( Curso * , Disciplina * ); // ??

   // Busca a demandas da disciplina e curso informados
   GGroup< Demanda *, LessPtr< Demanda > > buscaTodasDemandas( Curso * , Disciplina *, Campus *cp );

   // Retorna o conjunto de demandas da disciplina
   // informada, que foi substituída por alguma outra
   GGroup< Demanda *, LessPtr< Demanda > > retornaDemandaDisciplinasSubstituidas( Disciplina * );

   // Retorna os pares curso/curriculo dos quais a disciplina informada possui demanda
   GGroup< std::pair< Curso *, Curriculo * > > retornaCursosCurriculosDisciplina( Disciplina * );

	// Dada uma disciplina, e seu par curso/curriculo, retorna-se a oferta dessa disciplina
	Oferta * retornaOfertaDiscilpina( Curso *, Curriculo *, Disciplina * );

	Disciplina* retornaDisciplina( int id );

   // Armazena os 'ids' das disciplinas em comum entre os pares de ofertas compatíveis
   std::map< std::pair< Oferta *, Oferta * >, std::vector< int /*idDisc*/ > > oftsComp_disc;

   void preencheOftsCompDisc();

   void insereDisciplinaEmOftsComp( std::pair<Oferta*, Oferta*> po, int idDisc );

   //-----------------------------------------------------------------------------------------------

   // Estrutura utilizada para referenciar as novas disciplinas
   // criadas após a execução do método de divisão de disciplinas.
   GGroup< Disciplina *, LessPtr< Disciplina > > novasDisciplinas;

   std::map< Professor *, GGroup< std::pair< Aula *, Disciplina * > >,
      LessPtr< Professor > > mapProfessorDisciplinas;

   // Para cada demanda, relacionamos os alunos que estão associados a ela
   std::map< Demanda *, GGroup< AlunoDemanda *, LessPtr< AlunoDemanda > > > mapDemandaAlunos;

   // Dados os ids de uma oferta e uma disciplina,
   // retorna a demanda correspondente, caso exista
   Demanda * buscaDemanda( int, int );

   Oferta * findOferta( int ) const;
   Turno * findTurno( int );
   HorarioAula * findHorarioAula( int );

   GGroup< Professor *, LessPtr< Professor > > getProfessores() const;
   Professor * findProfessor( int );

   // Para uma dada aula, retorna-se o total de
   // horários do turno ao qual essa aula está alocada
   int totalHorariosTurnoAula( Aula * );

   std::map< Professor *, GGroup< Disciplina *,
      LessPtr< Disciplina > >, LessPtr< Professor > > mapProfessorDisciplinasAssociadas;

   // Retorna todas as salas (de todos os campi) do input
   GGroup< Sala *, LessPtr< Sala > > getSalas() const;
   Sala * findSala( int );
   
   // Associa a cada horario de aula o conjunto de horarios pertencentes
   // a outras semanas letivas que não se sobrepõem
   std::map< HorarioAula*, std::set<HorarioAula*> > compatibilidadesDeHorarios;

   GGroup< HorarioAula *, LessPtr< HorarioAula > > retornaHorariosEmComum( int sala, int disc, int dia );

   // Dado o id de uma unidade, retorna referencia para o campus correspondente
   Campus* retornaCampus( int /*unidId*/ );

   // Resultado da alocação de alunos no pre-modelo:
   std::map< Aluno*, GGroup< Trio< int /*campusId*/, int /*turma*/, Disciplina* > >, LessPtr< Aluno > > mapAluno_CampusTurmaDisc;

   std::map< Trio< int /*campusId*/, int /*turma*/, Disciplina* >, GGroup< AlunoDemanda*, LessPtr< AlunoDemanda > > > mapCampusTurmaDisc_AlunosDemanda;

   void insereAlunoEmTurma( Aluno* aluno, Trio< int /*campusId*/, int /*turma*/, Disciplina*> trio, std::map<int, double> diasNCreds );
   void removeAlunoDeTurma( Aluno* aluno, Trio< int /*campusId*/, int /*turma*/, Disciplina*> trio, std::map<int, double> diasNCreds );
   
   void imprimeAlocacaoAlunos( int campusId, int prioridade, int cjtAlunosId, bool heuristica );

   GGroup< AlunoDemanda *, LessPtr< AlunoDemanda > > listSlackDemandaAluno;

   // Retorna a turma da disciplina disc na qual o aluno foi alocado.
   // Caso contrario, retorna -1
   int retornaTurmaDiscAluno( Aluno* aluno, Disciplina* disc );

   AlunoDemanda* procuraAlunoDemanda( int discId, int alunoId );

   void atualizaDemandas( int novaPrioridade, int campusId );
   
   int retornaCampusId( int demandaId );

   void calculaDemandas();

   int atendeTurmaDiscOferta( int turma, int discId, int ofertaId );

   bool haDemandaDiscNoCampus( int disciplina, int campusId );
   bool haDemandaDiscNoCampus( int disciplina, int campusId, int prioridade );
   
   int existeTurmaDiscCampus( int turma, int discId, int campusId );

   GGroup<Aluno*> alunosEmComum( int turma1, Disciplina* disc1, int turma2, Disciplina* disc2, Campus* campus );

   bool possuiAlunosEmComum( int turma1, Disciplina* disc1, int turma2, Disciplina* disc2, Campus* campus );
      
   GGroup<int> diasComunsEntreDisciplinas( Disciplina *disciplina1, Disciplina *disciplina2 );

   void criaCjtAlunos( int campusId, int prioridade, bool FIXAR_P1 );

   int retornaCjtAlunosId( int discId );
   int retornaCjtAlunosId( Aluno* aluno );
   int haDemandaDiscNoCjtAlunosPorOferta( int discId, int oftId, int cjtAlunosId );
   int haDemandaDiscNoCjtAlunosPorCurso( int discId, int cursoId, int cjtAlunosId );
   bool haDemandaP2DiscNoCampus( int campusId, int P_ATUAL, Disciplina* disciplina );   
   int getQtdAlunoDemandaAtualPorCampus( int campusId );

   void imprimeCjtAlunos( int campusId );
   
   bool EQUIV_TRANSITIVIDADE;

   bool verificaDisponibilidadeHorario( HorarioAula *horarioAula, int dia, Sala *sala, Professor *prof, Disciplina* disc );

   double cargaHorariaNaoAtendidaPorPrioridade( int prior, int alunoId );
   double cargaHorariaRequeridaPorPrioridade( int prior, Aluno* aluno );

   private:
   
		void insereDisciplinaEmCursosComp( std::pair<Curso*, Curso*> pc, int idDisc );

};

#endif
