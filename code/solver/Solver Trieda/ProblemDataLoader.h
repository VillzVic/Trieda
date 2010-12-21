#ifndef PROBLEMDATALOADER_H
#define PROBLEMDATALOADER_H

/* O XSD Code Synthesis gera uma estrutura de dados para listas de
objetos que é um pouco estranha e contra-intuitiva. A macro abaixo
gera uma iteração por ela que pode ser representada por um código
mais legível e intuitivo, baseada nos padrões de projeto escolhidos */
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
   ProblemDataLoader( char *inputFile, ProblemData* data );

   // Destructor
   ~ProblemDataLoader();

   // Load the XML file
   void load();

   void carregaDiasLetivosCampus();

   void carregaDiasLetivosDiscs();
   void carregaDiasLetivosSala();

   void criaConjuntoSalas();

   /* Establece os dias letivos comuns entre um bloco curricular e um campus. */
   void estabeleceDiasLetivosBlocoCampus();

   /* Establece os dias letivos comuns entre as salas e suas disciplinas associadas. */
   void estabeleceDiasLetivosDisciplinasSalas();

   /* Establece os dias letivos comuns entre os conjuntos de salas e suas disciplinas associadas. */
   void estabeleceDiasLetivosDiscCjtSala();

   void divideDisciplinas();
   // >>> 14/10/2010
   void armz_disc_curriculo();
   // <<< 14/10/2010
   void gera_refs();
   template<class T> 
   void find_and_set(int id, GGroup<T*>& haystack, T*& needle, bool print) ;
   void cria_blocos_curriculares();
   void estima_turmas();
   void print_stats();
   void cache();
   void print_csv();

private:
   // Input data object of the problem
   ProblemData *problemData;
   char *inputFile;

   // XML parser
   std::auto_ptr<TriedaInput> root;

   // >>> Variáveis e/ou estruturas de dados para realizar o pré processamento dos dados.

   // <<<

   // >>> Métodos para realizar o pré processamento dos dados.

   // =========== METODOS SET

   // =========== METODOS GET

   // =========== METODOS AUXILIARES

   void referenciaCampus();
   void referenciaDisciplinas();
   void referenciaOfertas();

   void calculaTamanhoMedioSalasCampus();

   // Armazena todas as disciplinas ofertadas em um campus.
   void relacionaCampusDiscs();

   /* Calcula a demanda máxima e demanda total da disciplina em questão. Esses valores
   são armazenados na própria disciplina. */
   void calculaDemandas();

   /* Armazena em cada sala, referências para as disciplinas associadas. Armazena também,
   para cada disciplina, ponteiros para as salas compatíveis.*/
   //void carregaDisciplinasAssociadasSalas();
   void associaDisciplinasSalas();

   // Armazena, para cada disciplina, ponteiros para as salas compatíveis.
   void carregaDiscSalas();

   /* Armazena referências para as disciplinas associadas em cada elemento do <ConjuntoSala>. 
   Como o método associaDisciplinasSalas() já associou as disciplinas às salas, basta
   percorrer o map de salas de um dado elemento do <ConjuntoSala> adicionando as salas ao map de
   disciplinas associadas do <ConjuntoSala>.*/
   void associaDisciplinasConjuntoSalas();

   /* Relaciona cada disciplina com as ofertas em que ela aparece. */
   void relacionaDiscOfertas();

   /* Relaciona cada disciplina de um campus com as ofertas em que ela aparece. */
   //void relacionaDiscCampusOfertas();

   /* Inicializando as estruturas presentes em cada Sala, que são responsáveis por 
   informar a quantidade de créditos livres em um dado dia letivo. */
   void calculaCredsLivresSalas();

   /*conjunto de combinações possíveis de divisão de créditos de uma uma disciplina*/
   void combinacaoDivCreditos();

   // <<<

};

#endif
