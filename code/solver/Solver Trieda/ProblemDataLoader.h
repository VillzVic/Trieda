#ifndef _PROBLEM_DATA_LOADER_H_
#define _PROBLEM_DATA_LOADER_H_

// O XSD Code Synthesis gera uma estrutura de dados para listas de
// objetos que é um pouco estranha e contra-intuitiva. A macro abaixo
// gera uma iteração por ela que pode ser representada por um código
// mais legível e intuitivo, baseada nos padrões de projeto escolhidos

#ifndef ITERA_SEQ
#define ITERA_SEQ(it,addr,type) for (Grupo##type##::##type##_iterator it = \
   (addr).##type##().begin(); it != (addr).##type##().end(); ++it) 
#endif

#include "ProblemData.h"
#include "input.h"
#include "TRIEDA-InputXSD.h"

class ProblemDataLoader
{
public:
   // Constructor
   ProblemDataLoader( char *, ProblemData *);

   // Destructor
   virtual ~ProblemDataLoader();

   // Load the XML file
   void load();

   void referenciaDisciplinasEquivalentesIncompativeis();

   void referenciaDisciplinasCurriculos();

   // Para cada dia da semana, relaciona os
   // seus respectivos horários de aula disponíveis
   void relacionaHorariosAulaDiaSemana();

   // Carrega os dias letivos dos Campus, Unidades e Salas.
   void carregaDiasLetivosCampusUnidadeSala();

   // Carrega os dias letivos de cada disciplina.
   void carregaDiasLetivosDiscs();

   // Cria os Conjuntos de Salas para cada Unidade.
   void criaConjuntoSalasUnidade();

   // Establece os dias letivos comuns entre um bloco curricular e um campus.
   void estabeleceDiasLetivosBlocoCampus();

   // Establece os dias letivos comuns entre as salas e suas disciplinas associadas.
   void estabeleceDiasLetivosDisciplinasSalas();

   // Establece os dias letivos comuns entre os conjuntos de salas e suas disciplinas associadas.
   void estabeleceDiasLetivosDiscCjtSala();

   // Establece os dias letivos comuns entre os professores e suas disciplinas.
   void estabeleceDiasLetivosProfessorDisciplina();

   /* */
   void disciplinasEquivalentes();

   /* */
   void divideDisciplinas();

   /* */
   template< class T > 
   void find_and_set( int, GGroup< T * > &, T * &, bool );

   template< class T > 
   void find_and_set_lessptr( int, GGroup< T *, LessPtr< T > > &, T * &, bool );

   /* */
   void gera_refs();

   /* */
   void cria_blocos_curriculares();

   /* */
   void estima_turmas();

   /* */
   void print_stats();

   /* */
   void cache();

   /* */
   void criaAulas();

   /* */
   void relacionaBlocoCurricularAulas();

   /* */
   void calculaMaxHorariosProfessor();

   // Cria um vetor ordenado dos horários de
   // aula (todos os horários de todos os professores)
   void criaListaHorariosOrdenados();

   /* */
   void disciplinasCursosCompativeis();

   // Divide as fixações de acordo com seus tipos
   void relacionaFixacoes();

   // Quando houver uma fixação de uma disciplina em um dia
   // da semana, esse método remove os demais dias da semana
   // no conjunto de dias letivos da disciplina em questão
   void verificaFixacoesDiasLetivosDisciplinas();

   // Quando houver uma disciplina que use laboratórios e também
   // existir uma fixação dessa disciplina, devemos criar a fixação
   // correta para a disciplina dividida (créditos práticos)
   void criaFixacoesDisciplinasDivididas();

   GGroup< int > retorna_foxacoes_dias_letivos( Disciplina * );

   bool existe_conjunto_sala__fixacao( Unidade *, Disciplina *, Sala * );

private:
   ProblemData * problemData;
   char * inputFile;
   std::auto_ptr< TriedaInput > root;

   void referenciaCampusUnidadesSalas();
   void referenciaDisciplinas();
   void referenciaOfertas();
   void calculaTamanhoMedioSalasCampus();

   // Armazena todas as disciplinas ofertadas em um campus.
   void relacionaCampusDiscs();

   // Calcula a demanda máxima e demanda total da disciplina
   // em questão. Esses valores são armazenados na própria disciplina.
   void calculaDemandas();

   // Armazena em cada sala, referências para as disciplinas
   // associadas. Armazena também, para cada disciplina,
   // ponteiros para as salas compatíveis.
   void associaDisciplinasSalas();

   // Armazena, para cada disciplina, ponteiros para as salas compatíveis.
   void carregaDiscSalas();

   // Armazena referências para as disciplinas associadas
   // em cada elemento do <ConjuntoSala>. Como o método
   // associaDisciplinasSalas() já associou as disciplinas
   // às salas, basta percorrer o map de salas de um dado
   // elemento do <ConjuntoSala> adicionando as salas ao
   // map de disciplinas associadas do <ConjuntoSala>.
   void associaDisciplinasConjuntoSalas();

   // Relaciona cada disciplina com as ofertas em que ela aparece.
   void relacionaDiscOfertas();

   // Relaciona cada disciplina de um campus com as ofertas em que ela aparece.
   //void relacionaDiscCampusOfertas();

   // Inicializando as estruturas presentes em cada Sala, que são responsáveis por 
   // informar a quantidade de créditos livres em um dado dia letivo.
   void calculaCredsLivresSalas();

   // Conjunto de combinações possíveis de divisão de créditos de uma uma disciplina
   void combinacaoDivCreditos();

   // Relaciona cada possível valor de crédito com as regras de créditos existentes.
   void relacionaCredsRegras();

   // Relaciona cada curso com o seu respectivo campus
   void relacionaCursosCampus();
};

#endif
