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

//#define READ_SOLUTION

#define ALUNO_TURNOS_DA_DEMANDA // Usado quando o aluno só pode ser alocado em turno existente no calendario da Oferta da Demanda que ele requisitou

#define EQUIVALENCIA_DESDE_O_INICIO


bool less_than( DateTime left, DateTime right );


class ProblemData
	: public OFBase
{
public:
   ProblemData( char input[1024], int inputId );
   virtual ~ProblemData( void );
   
   void clearHorarioAula();

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
   GGroup< Professor *, LessPtr< Professor > > profsVirtuais; // 1 por tipo de titulação
   GGroup< Equivalencia*, LessPtr< Equivalencia > > equivalencias; // só contém as teoricas

   GGroup< BlocoCurricular *, LessPtr< BlocoCurricular > > blocos;

   std::map< std::pair< int/*oldDisc*/, int/*newDisc*/ >, int /*equiv*/> mapParDisc_EquivId; // só contém as teoricas

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

   // Calcula o tempo NECESSÁRIO para
   // se deslocar entre uma aula e outra
   int calculaTempoEntreCampusUnidades( Campus *, Campus *, Unidade *, Unidade * );

   // Verifica a ocorrência de última aula do dia ( D ) e primeira aula do dia ( D + 1 )
   bool verificaUltimaPrimeiraAulas( HorarioDia *, HorarioDia * );

   bool verificaPrimeiraAulas( HorarioDia * h );

   bool verificaUltimaAulas( HorarioDia * h );

   std::map< Calendario *, std::vector< HorarioAula * >, LessPtr< Calendario > > mapCalendarioHorariosAulaOrdenados;
   std::vector< HorarioAula * > horarios_aula_ordenados;

   GGroup< AtendimentoCampusSolucao *, LessPtr< AtendimentoCampusSolucao > > * atendimentosTatico;

   // Para cada dia da semana, informa o
   // conjunto de horários aula disponíveis nesse dia
   std::map< int, GGroup< HorarioAula *, LessPtr< HorarioAula > > > horarios_aula_dia_semana;

   // Conjunto de professores virtuais alocados na solução operacional
   std::vector< Professor * > professores_virtuais;
   std::unordered_set<int> profs_virtuais_ids;

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

   std::map< int /*Id Campus*/, unsigned /*Tamanho médio das salas*/ > cp_medSalas;

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
   // Lista todos os horários, para cada dia letivo, comuns entre uma disciplina e uma sala.

   std::map<std::pair< int /*idDisc*/, int /*idSala*/ >, 
	   std::map< int /*Dias*/, GGroup< HorarioAula *, LessPtr< HorarioAula > > > > disc_Salas_Dias_HorariosAula;
   // =============================================================================================
   
   // Listando as regras de créditos para cada possível valor de crédito.
   std::map< int /*Num. Creds*/, GGroup< DivisaoCreditos *, LessPtr< DivisaoCreditos > > > creds_Regras;

   // Dias letivos comuns de um professor e uma disciplina.
   std::map< std::pair< int /*idProf*/, int /*idDisc*/>, GGroup< int > /*Dias*/ > prof_Disc_Dias;
       
   // Mapeia para cada disciplina, os professor reais que possuem habilitação para ministra-la.
   std::map< Disciplina*, GGroup<Professor*, LessPtr<Professor>>, LessPtr<Disciplina> > mapDiscProfsHabilit;

   // Retorna o número de professores reais que possuem habilitação para ministrar a disciplina.
   int getNroProfPorDisc( Disciplina *disciplina );

   GGroup< Aula *, LessPtr< Aula > > aulas;
   
   // Estrutura que informa a quais blocos curriculares uma aula pertence.
   //std::map< Aula *, GGroup< BlocoCurricular *, LessPtr< BlocoCurricular > >,
   //          LessPtr< Aula > > aulaBlocosCurriculares;
   
#pragma region FIXAÇÕES - NÃO USADO
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
#pragma endregion


   void le_turnosIES( TriedaInput & );
   virtual void le_arvore( TriedaInput & );
   
   void refDiscEquivalencias();
   void checkParamUsaEquivalencia();
   void leOfertas(TriedaInput & raiz);
   void setModoOtimizacao();
   void leAtendimentoTatico(TriedaInput & raiz);
   void leDemandas(TriedaInput & raiz);
   void refTitContProfReal();
   void criaPerfilProfVirtual();
   void preencheHorSalas(TriedaInput &raiz);

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
   
   // Busca a demandas da disciplina e curso informados
   GGroup< Demanda *, LessPtr< Demanda > > buscaTodasDemandas( Curso * , Disciplina *, Campus *cp );
   
   // Retorna os pares curso/curriculo dos quais a disciplina informada possui demanda
   GGroup< std::pair< Curso *, Curriculo * > > retornaCursosCurriculosDisciplina( Disciplina * );
   
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

   GGroup< Professor *, LessPtr< Professor > > getProfessores() const;
   Professor * findProfessor( int );
   Professor * findProfessorVirtual( int id );

   std::map< Professor *, GGroup< Disciplina *,
      LessPtr< Disciplina > >, LessPtr< Professor > > mapProfessorDisciplinasAssociadas;

   // Retorna todas as salas (de todos os campi) do input
   GGroup< Sala *, LessPtr< Sala > > getSalas() const;
   
   GGroup< HorarioAula *, LessPtr< HorarioAula > > retornaHorariosEmComum( int sala, int disc, int dia );

   // Dado o id de uma unidade, retorna referencia para o campus correspondente
   Campus* retornaCampus( int /*unidId*/ );

   // Resultado da alocação de alunos no pre-modelo:
   std::map< Aluno*, GGroup< Trio< int /*campusId*/, int /*turma*/, Disciplina* > >, LessPtr< Aluno > > mapAluno_CampusTurmaDisc;
   std::map< Trio< int /*campusId*/, int /*turma*/, Disciplina* >, GGroup< AlunoDemanda*, LessPtr< AlunoDemanda > > > mapCampusTurmaDisc_AlunosDemanda;

   // Folgas de atendimento ao final do tático de p1:
   std::map< Aluno*, GGroup< Trio< int /*campusId*/, int /*turma*/, Disciplina* > >, LessPtr< Aluno > > mapSlackAluno_CampusTurmaDisc;
   std::map< Trio< int /*campusId*/, int /*turma*/, Disciplina* >, GGroup< AlunoDemanda*, LessPtr< AlunoDemanda > > > mapSlackCampusTurmaDisc_AlunosDemanda;

   // Associa quais os calendarios podem ser usados na alocação da turma. Necessário caso haja alunos
   // na turma pertencentes a ofertas de calendarios diferentes
   std::map< Trio< int /*campusId*/, int /*turma*/, int /*discId*/ >, GGroup< Calendario*, LessPtr<Calendario> > > mapTurma_Calendarios;
   std::map< Trio< int /*campusId*/, int /*turma*/, int /*discId*/ >, GGroup< HorarioDia*, LessPtr<HorarioDia> > > mapTurma_HorComunsAosCalendarios;
   
   // Associa quais os turnos podem ser usados na alocação da turma. Necessário caso haja alunos
   // na turma pertencentes a ofertas de turnos diferentes. Substitui os maps acima, que eram para Calendario.
   std::map< Trio< int /*campusId*/, int /*turma*/, int /*discId*/ >, GGroup< TurnoIES*, LessPtr<TurnoIES> > > mapTurma_TurnosIES;
   std::map< Trio< int /*campusId*/, int /*turma*/, int /*discId*/ >, GGroup< HorarioDia*, LessPtr<HorarioDia> > > mapTurma_HorComunsAosTurnosIES;
   
   void clearMaps( int cpId, int prioridade );
   void preencheMapsTurmasCalendarios();
   void preencheMapsTurmasTurnosIES();

   void insereAlunoEmTurma( AlunoDemanda* alunoDemanda, Trio< int /*campusId*/, int /*turma*/, Disciplina*> trio, GGroup<HorarioDia*> horariosDias );
   
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
   
   int getNroDiscsAtendidasNoCurso( Curso *curso );

   bool haDemandaDiscNoCampus( int disciplina, int campusId );
   bool haDemandaDiscNoCampus( int disciplina, int campusId, int prioridade );
   GGroup<AlunoDemanda*, LessPtr<AlunoDemanda>> retornaDemandasDiscNoCampus( int disciplinaId, int campusId, int prioridade );

   int existeTurmaDiscCampus( int turma, int discId, int campusId );

   GGroup<Aluno*, LessPtr<Aluno>> alunosEmComum( int turma1, Disciplina* disc1, int turma2, Disciplina* disc2, Campus* campus );

   bool possuiAlunosEmComum( int turma1, Disciplina* disc1, Campus* campus1, int turma2, Disciplina* disc2, Campus* campus2 );
      
   GGroup<int> diasComunsEntreDisciplinas( Disciplina *disciplina1, Disciplina *disciplina2 );

   void criaCjtAlunos( int campusId, int prioridade, bool FIXAR_P1 );

   int retornaCjtAlunosId( int discId );
   int retornaCjtAlunosId( Aluno* aluno );
   int haDemandaDiscNoCurso( int discId, int cursoId );
   bool haDemandaDiscNoCursoEquiv( Disciplina *disciplina, int cursoId );
   int maxDemandaDiscNoCursoEquiv( Disciplina *disciplina, int cursoId );
   int getQtdAlunoDemandaAtualPorCampus( int campusId );

   void imprimeCjtAlunos( int campusId );
   
   bool EQUIV_TRANSITIVIDADE;

   bool verificaDisponibilidadeHorario( HorarioAula* horarioAula, int dia, Sala *sala, Professor *prof, Disciplina* disc );

   double cargaHorariaNaoAtendidaPorPrioridade( int prior, int alunoId );
   double cargaHorariaOriginalRequeridaPorPrioridade( int prior, Aluno* aluno );
   double cargaHorariaAtualRequeridaPorPrioridade( int prior, Aluno* aluno );
   double cargaHorariaJaAtendida( Aluno* aluno );

   bool haAlunoFormandoNaoAlocado( Disciplina *disciplina, int cpId, int prioridadeAtual );
   bool haAlunoFormandoNaoAlocadoEquiv( Disciplina *disciplina, int cpId, int prioridadeAtual );
   
   int getNroFolgasDeAtendimento( int prioridade, Disciplina *disciplina, int campusId );

   Disciplina* getDisciplinaTeorPrat( Disciplina *disciplina );
   
   GGroup<AlunoDemanda*, LessPtr<AlunoDemanda>> retornaMaxDemandasDiscNoCampus_EquivTotal( Disciplina* disciplina, int campusId, int prioridade, bool permiteRealocacao=false );
   bool haDemandasDiscNoCampus_Equiv( int disciplinaId, int campusId, int prioridade );

   bool violaMinTurma( int campusId );
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

   void removeAlunoDemanda( AlunoDemanda *alunoDemanda );
   
   GGroup<HorarioDia*, LessPtr<HorarioDia>> retornaHorariosDiaTurnoComum( GGroup<Calendario*,LessPtr<Calendario>> &calends );
   GGroup<HorarioDia*, LessPtr<HorarioDia>> retornaHorariosDiaComuns( GGroup<TurnoIES*,LessPtr<TurnoIES>> &turnosIES );

   void preencheMapParCalendHorariosDiaComuns();
   
   void imprimeAssociacaoDisciplinaSala();
   void constroiHistogramaDiscDemanda();

   double getPercAcumuladoDemanda( Disciplina *disciplina ) { return mapDiscPerc[disciplina]; }

   void preencheMapTurnoSemanasLetivas();
   void preencheMapTurnoDisciplinas();
   	
	void setDateTimesSobrepostos();
	GGroup< std::pair<DateTime,DateTime> >* getDateTimesSobrepostos( DateTime dt );
   std::map<DateTime, GGroup< std::pair<DateTime,DateTime> > > mapDtiSobrepoeDtiDtf;

   std::map<int, std::pair<DateTime*,int> > horarioAulaDateTime; 
   bool getPairDateTime(int horId, std::pair<DateTime*,int> &) const;

   GGroup<DateTime*,LessPtr<DateTime> > horariosInicioValidos;
   GGroup<DateTime*,LessPtr<DateTime> > horariosTerminoValidos;
   DateTime* getDateTimeInicial( DateTime dt );
   DateTime* getDateTimeFinal( DateTime dt );
   GGroup<DateTime*, LessPtr<DateTime> > getDtiAnteriores( DateTime dtf );

   void setFaseDoDiaDosHorariosAula();
   void addHorariosAnalogosDisciplina();

   map< int, std::pair<DateTime,DateTime> > fasesDosTurnos;
   enum FasesTurnos { Manha=1, Tarde=2, Noite=3 };		
   
   int getFaseDoDia( DateTime dt );
   DateTime getFimDaFase( int fase ) const;
   int getNroTotalDeFasesDoDia() const { return fasesDosTurnos.size(); }
   bool haTurnoComumViavel( Disciplina *disciplina, TurnoIES *turnoIES );
   GGroup<int> getTurnosComunsViaveis( Disciplina *disciplina, TurnoIES *turnoIES );
   bool alocacaoEquivViavel( Demanda *demanda, Disciplina *disciplina );
   GGroup<int> alocacaoEquivViavelTurnosIES( Demanda *demanda, Disciplina *disciplinaEquiv );
   
   // Guarda para cada disciplina o par <difTurmas, nroNaoAtend> aonde:
   // difTurmas = max turmas - nro turmas abertas
   // nroNaoAtend = o número de não atendimentos
   std::map<int /*discId*/, std::pair<int/*difTurmas*/, int/*nroNaoAtend*/> > mapDiscDif;
   
   // Utilização das salas em minutos semanais
   map<ConjuntoSala *, int, LessPtr<ConjuntoSala>> mapCreditosSalas;
   void imprimeUtilizacaoSala( int campusId, int prioridade, int cjtAlunosId, bool heuristica, int r, string MIP );
   void imprimeDiscNTurmas( int campusId, int prioridade, int cjtAlunosId, bool heuristica, int r, int tatico );

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

   int getTotalOrigAlunoDemanda( int cpId ) { return totalOrigAlunoDemanda[cpId]; }
   void setTotalOrigAlunoDemanda( int cpId, int v ) { totalOrigAlunoDemanda[cpId] = v; }

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
 
	    // histogram structures
	    GGroup< std::pair<double,Disciplina*> > perc_Disc;
		std::map< Disciplina*, double, LessPtr<Disciplina> > mapDiscPerc;

	    std::map< std::pair<Calendario*,Calendario*>, GGroup<HorarioDia*, LessPtr<HorarioDia>> > map_ParCalend_HorariosDiaComuns;

	    // map: [campusId][disciplina][prioridade][GGroup<AlunoDemanda>]
	    std::map< int, std::map< Disciplina*, std::map< int, GGroup<AlunoDemanda*,LessPtr<AlunoDemanda>> >, LessPtr<Disciplina> > > mapDisciplinaAlunosDemanda;

		void insereDisciplinaEmCursosComp( std::pair<Curso*, Curso*> pc, int idDisc );

		std::string inputFileName;
		int inputId;

	   std::map< std::pair< Demanda*, Disciplina* >, bool > mapEquivViavelDemandaDisc;

#ifdef BUILD_COM_SOL_INICIAL
	   SolucaoTaticoInicial * solTaticoInicial;
 	   std::map< int /*campusId*/, int > totalOrigAlunoDemanda;			// com divisão pratica/teorica
	   std::map< int /*campusId*/, int > numTurmasTotalEstimados;			// com divisão pratica/teorica
#endif

	   int cenarioId_;

};

#endif
